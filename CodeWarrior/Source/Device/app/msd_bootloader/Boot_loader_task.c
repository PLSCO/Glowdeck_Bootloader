/**
\brief Include the OS and BSP dependent files that define IO functions and
 basic types. You may like to change these files for your board and RTOS.
 Based on Freescale bootloader demo for k60. 

\author Xavi Vilajosana <xvilajosana@eecs.berkeley.edu>, June 2012.
 */

#include "derivative.h"
#include "hidef.h"
#include <string.h>
#include <stdlib.h>    /* ANSI memory controls */
#include "stdio.h"
//#include "sci.h"
#include "Bootloader.h"
#include "Boot_loader_task.h"

#if (defined MCU_MK60N512VMD100 |defined MCU_MK20D7 )
#include "flash_FTFL.h"
#endif

/********************** Memory protected area *******************/

#if (defined MCU_MK60N512VMD100||defined MCU_MK20D7)
	  // Protect bootloader flash 0x0 - 0xBFFF
	  #pragma define_section cfmconfig ".cfmconfig" ".cfmconfig" ".cfmconfig" far_abs R
	  static __declspec(cfmconfig) uint_8 _cfm[0x10] = {
	   /* NV_BACKKEY3: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY2: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY1: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY0: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY7: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY6: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY5: KEY=0xFF */
	    0xFFU,
	   /* NV_BACKKEY4: KEY=0xFF */
	    0xFFU,
	   /* NV_FPROT3: PROT=0xF8 */
	    PROT_VALUE3,
	   /* NV_FPROT2: PROT=0xFF */
	    PROT_VALUE2,
	   /* NV_FPROT1: PROT=0xFF */
	    PROT_VALUE1,
	   /* NV_FPROT0: PROT=0xFF */
	    PROT_VALUE0,
	   /* NV_FSEC: KEYEN=1,MEEN=3,FSLACC=3,SEC=2 */
	    0x7EU,
	   /* NV_FOPT: ??=1,??=1,??=1,??=1,??=1,??=1,EZPORT_DIS=1,LPBOOT=1 */
	    0xFFU,
	   /* NV_FEPROT: EPROT=0xFF */
	    0xFFU,
	   /* NV_FDPROT: DPROT=0xFF */
	    0xFFU
	  };
#else
	#error Undefined MCU for flash protection
#endif
	
/**************************************************************************
   Global variables
**************************************************************************/
extern uint_8                       filetype;      /* image file type */     
static uint_32                      New_sp,New_pc; /* stack pointer and program counter */
boolean								active_file = FALSE;

/**************************************************************************
   Funciton prototypes
**************************************************************************/
#if MAX_TIMER_OBJECTS
extern uint_8 TimerQInitialize(uint_8 ControllerId);
#endif
#define EEPROM_SIZE 512
#define FTFL_FCNFG_EEERDY		((uint8_t)0x01)	
#define FTFL_FCNFG_RAMRDY		((uint8_t)0x02)
#define EEESIZE 0x35//512bytes
#define FlexRAM ((uint8_t *)0x14000000)
#define __disable_irq() asm("CPSID i");
#define __enable_irq()	asm("CPSIE i");
void eeprom_initialize(void)
{
	uint32_t count=0;
	uint16_t do_flash_cmd[] = {
		0xf06f, 0x037f, 0x7003, 0x7803,
		0xf013, 0x0f80, 0xd0fb, 0x4770};
	uint8_t status;

	if (FTFL_FCNFG & FTFL_FCNFG_RAMRDY) {
		// FlexRAM is configured as traditional RAM
		// We need to reconfigure for EEPROM usage
		FTFL_FCCOB0 = 0x80; // PGMPART = Program Partition Command
		FTFL_FCCOB4 = EEESIZE; // EEPROM Size
		FTFL_FCCOB5 = 0x03; // 0K for Dataflash, 32K for EEPROM backup
		__disable_irq();
		// do_flash_cmd() must execute from RAM.  Luckily the C syntax is simple...
		(*((void (*)(volatile uint8_t *))((uint32_t)do_flash_cmd | 1)))(&FTFL_FSTAT);
		__enable_irq();
		status = FTFL_FSTAT;
		if (status & 0x70) {
			FTFL_FSTAT = (status & 0x70);
			return; // error
		}
	}
	// wait for eeprom to become ready (is this really necessary?)
	while (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) {
		if (++count > 20000) break;
	}
}

uint8_t eeprom_read_byte(const uint8_t *addr)
{
	uint32_t offset = (uint32_t)addr;
	if (offset >= EEPROM_SIZE) return 0;
	if (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) eeprom_initialize();
	return FlexRAM[offset];
}

static void flexram_wait(void)
{
	while (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) {
		// TODO: timeout
	}
}

void eeprom_write_byte(uint8_t *addr, uint8_t value)
{
	uint32_t offset = (uint32_t)addr;

	if (offset >= EEPROM_SIZE) return;
	if (!(FTFL_FCNFG & FTFL_FCNFG_EEERDY)) eeprom_initialize();
	if (FlexRAM[offset] != value) {
		FlexRAM[offset] = value;
		flexram_wait();
	}
}

#define ARM_DEMCR		(*(volatile uint32_t *)0xE000EDFC) // Debug Exception and Monitor Control
#define ARM_DEMCR_TRCENA		(1 << 24)	 // Enable debugging & monitoring blocks
#define ARM_DWT_CTRL		(*(volatile uint32_t *)0xE0001000) // DWT control register
#define ARM_DWT_CTRL_CYCCNTENA		(1 << 0)		// Enable cycle count
#define ARM_DWT_CYCCNT		(*(volatile uint32_t *)0xE0001004) // Cycle count register
#define sei() __enable_irq()
#define cli() __disable_irq()

// sets the WS2812 pixels
// takes about 45 mSec to complete during which interrupts are turned off

// at 9600 baud and a serial port (UART1) with a one-byte buffer, each byte must be serviced within 1 mSec, so this can't be run while you're allowing serial port access.

#define GREEN 0
#define RED 1
#define BLUE 2
#define NUMLEDS 56
#define LENGTHOFDROP 8
#define F_CPU 48000000							// one cycle is .25 uSec
#define CYCLES_800_T0H  (F_CPU / 4000000)		// 0 is 12 cycles long (3 uSec)
#define CYCLES_800_T1H  (F_CPU / 1200000)		// 1 is 40 cycles long (10 uSec)
#define CYCLES_800      (F_CPU / 800000)		// reset is 90 cycles long (22.5 uSec)
#define setPin GPIOC_PSOR  = (1 << 2);
#define clearPin GPIOC_PCOR  = (1 << 2);

void pixel(uint8_t mode,uint32_t part,uint32_t full)
{
	uint8_t pinmask;
	uint8_t mask;

	uint8_t numwhite=0; 			
	static uint8_t dropletstart = LENGTHOFDROP; 
	uint8_t droplet = dropletstart;
	uint32_t cyc;
    uint8_t i=0;
    uint8_t end=0;
    uint32_t fulllocal;

	if (part && full) numwhite  = (part * NUMLEDS) / full; // this depends on the multiplication happening first so as to not need floats
	fulllocal = full;
	
	ARM_DEMCR |= ARM_DEMCR_TRCENA;
	ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;			// this enables the Debug unit's cycle counter
	cyc = ARM_DWT_CYCCNT + CYCLES_800;

	cli();											// disable interrupt processing
	while(i<NUMLEDS) {
		uint8_t end = 0;
		while (end < 3) {
			if(numwhite) {				// filled section is all white  
				pinmask = 90;	// turning the brightness of the white down mostly for power savings
			} else if (end==BLUE && mode==2) {
				switch (droplet) {
					case LENGTHOFDROP: pinmask = 255; break;
					case (LENGTHOFDROP-1): pinmask = 150; break;
					case (LENGTHOFDROP-2): pinmask = 75; break;
					case (LENGTHOFDROP-3): pinmask = 30; break;
					default: pinmask = 0; break;        			
				}
				if (!(--droplet)) droplet = LENGTHOFDROP;       		
			} else if (end==BLUE && mode==1) {			// normal blue on mode
				pinmask = 255;
			} else if (end==RED  && mode==3 && (fulllocal & 0x00000001)) { 			// error in flashing turns LEDs RED, with the first 8 bits being the error code
				pinmask = 70;
			} else {
				pinmask = 0;
			}
		
			for (mask = 0x80; mask; mask >>= 1) {
				  if (pinmask & mask) {
						  while (ARM_DWT_CYCCNT - cyc < CYCLES_800) ;     // 22.5 usec low (plus overhead)
						  cyc = ARM_DWT_CYCCNT;
						  setPin;
						  while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T1H) ; // 10 uSec high
						  clearPin;
				  } else {
						  while (ARM_DWT_CYCCNT - cyc < CYCLES_800) ;     // 22.5 uSec low
						  cyc = ARM_DWT_CYCCNT;
						  setPin;
						  while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T0H) ; // 3 uSec high
						  clearPin;
				  }
			}
			end++;
		}
		i++;
		if (numwhite) numwhite--;
		if (mode == 3) fulllocal = (fulllocal >> 1) | 0x80000000; // fills full with 1s so the tail end is bright.
	}
	while (ARM_DWT_CYCCNT - cyc < CYCLES_800) ;
	sei();              // Re-enable interrupts

	if (mode==2) { dropletstart--; if (!dropletstart) dropletstart = LENGTHOFDROP; }
}








/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : GPIO_Init
* Returned Value : none
* Comments       : Init LEDs and Buttons
*
*END*--------------------------------------------------------------------*/
void GPIO_Bootloader_Init()
{
    /* Body */
#if defined(MCU_MK60N512VMD100)
    SIM_SCGC5   |= SIM_SCGC5_PORTA_MASK;    /* Enable clock gating to PORTA */
    /* Enable LEDs Port A pin 28 & 29 */                                    
    PORTA_PCR28 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR28 = PORT_PCR_MUX(1);
    PORTA_PCR29 |= PORT_PCR_SRE_MASK        /* Slow slew rate */
                |  PORT_PCR_ODE_MASK        /* Open Drain Enable */
                |  PORT_PCR_DSE_MASK        /* High drive strength */
            ;
    PORTA_PCR29 = PORT_PCR_MUX(1);
    GPIOA_PSOR  |= 1 << 28 | 1 << 29;
    GPIOA_PDDR  |= 1 << 28 | 1 << 29;
    /* set in put PORTA pin 19 */
    PORTA_PCR19 =  PORT_PCR_MUX(1);
    GPIOA_PDDR &= ~((uint_32)1 << 19);
    PORTA_PCR19 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/

    
#elif (defined(MCU_MK20D5)|defined(MCU_MK20D7))    
    SIM_SCGC5   |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK;    /* Enable clock gating to port B & C */
    /* Enable LEDs Port B pin 0 & 1 */
    /*PORTB_PCR0 |= PORT_PCR_SRE_MASK        
                |  PORT_PCR_ODE_MASK        
                |  PORT_PCR_DSE_MASK        
            ;
    PORTB_PCR0 |= PORT_PCR_MUX(1);*/
   PORTC_PCR2 |= PORT_PCR_DSE_MASK        
            ;
   PORTC_PCR2 |= PORT_PCR_MUX(1);
    
   GPIOC_PCOR = (1 << 2);
   GPIOC_PDDR  |= (1 << 2);
    /* set input PORTC pin 1 and PORTB pin 0*/
   PORTB_PCR0 =  PORT_PCR_MUX(1);
       GPIOB_PDDR &= ~((uint_32)1 << 0);
       PORTB_PCR0 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
    PORTE_PCR1 =  PORT_PCR_MUX(1);
    GPIOE_PDDR &= ~((uint_32)1 << 1);
    PORTE_PCR1 |= PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;/* pull up*/
#endif
//TODO select what pin will be used in openmote k20 and configure it 
    
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : Switch_mode
* Returned Value : none
* Comments       : Jump between application and bootloader
*
*END*--------------------------------------------------------------------*/
void Switch_mode(void)
{
	eeprom_initialize();
	volatile uint_32  temp = 1; 
	volatile uint_32  temp2 = 1;
	if(eeprom_read_byte(0) == 13){
		temp = 0;
		eeprom_write_byte(0,0);
	}
    /* Body */
      /* default the button is not pressed */

    /* Get PC and SP of application region */
    New_sp  = ((uint_32_ptr)IMAGE_ADDR)[0];
    New_pc  = ((uint_32_ptr)IMAGE_ADDR)[1];
    /* Check switch is pressed*/

    if(temp)
    	temp =(uint_32) ((1<<1) & GPIOE_PDIR); 
    if(temp != 1)
    	temp2 =(uint_32) ((1<<0) & GPIOB_PDIR); 


    if(temp && temp2) //&& temp2)
    {
        if((New_sp != 0xffffffff)&&(New_pc != 0xffffffff))
        {
            /* Run the application */

            asm
            {
                ldr   r4,=New_sp
                ldr   sp, [r4]
                ldr   r4,=New_pc
                ldr   r5, [r4]
                blx   r5
            }

        } /* EndIf */
    }

} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : erase_flash
* Returned Value : none
* Comments       : erase flash memory in application area
*
*END*--------------------------------------------------------------------*/

uint_8 erase_flash(void)
{ 
    /* Body */
    uint_8 error = FALSE;
    uint_8 i;
    Flash_Init(59);
    DisableInterrupts;                      
    for (i=0;i<(MAX_FLASH1_ADDRESS -(uint_32) IMAGE_ADDR)/ERASE_SECTOR_SIZE;i++)
    {
#if (!defined __MK_xxx_H__)    
        error = Flash_SectorErase((uint_32*)((uint_32) IMAGE_ADDR + ERASE_SECTOR_SIZE*i)) ; /* ERASE 4k flash memory */
#else
        error = Flash_SectorErase((uint_32) IMAGE_ADDR + ERASE_SECTOR_SIZE*i) ; /* ERASE 4k flash memory */
#endif
        if(error != Flash_OK)
        {
            break;
        } /* Endif */
    } /* Endfor */
    EnableInterrupts;
    return error;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : SetOutput
* Returned Value : None
* Comments       : Set out put of the LEDs
*     
*END*--------------------------------------------------------------------*/

void SetOutput
    (
        /* [IN] the output pin */
        uint_32 output,
        /* [IN] the state to set */
        boolean state
    ) 
{
    /* Body */


/* For TWR-K60 */
#if (defined MCU_MK60N512VMD100)
    if(state)
        GPIOA_PCOR |= output; 
    else
        GPIOA_PSOR |= output; 
#endif
    
    
#if (defined MCU_MK20D7)
    //if(state)
    //    GPIOC_PCOR |= output; 
   // else
      //  GPIOC_PSOR |= output; 
#endif

} /* EndBody */

  /* EOF */
