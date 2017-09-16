/******************************************************************************
*                                                  
*  File Name: main.c
*                                                                          
*  Purpose: This file is for a USB Mass-Storage Device bootloader.  This file 
*           has the main routine for the user application with the bootloader.
*           In this case, the application simply blinks an LED on PTC0 using
*           a timer interrupt.  
*******************************************************************************/

#include "Bootloader.h"
#include "support_common.h" /* include peripheral declarations and more */
#if (CONSOLE_IO_SUPPORT || ENABLE_UART_SUPPORT)
/* Standard IO is only possible if Console or UART support is enabled. */
#include <stdio.h>
#endif

__declspec(interrupt) void pit0_isr(void);

int main(void)
{
	int counter = 0;
	uint_32_ptr pdst,psrc;
	uint_32 i;
	
	// Copy Application Stored Interrupt Vector table to RAM
	pdst=(uint_32*)0x20000000;
	psrc=IMAGE_ADDR;
	  for (i=0;i<0x100;i++,pdst++,psrc++)
	  {
	    *pdst=*psrc;
	  }
	
	// Move vector table to RAM
	asm(move.l	#0x20000000, d0);
    asm(movec 	d0,VBR); 
	asm(nop);
	
    // Enable Interrupts for PIT0 (55)
    MCF_INTC0_IMRH &= ~(MCF_INTC_IMRH_INT_MASK55);
    MCF_INTC0_IMRL &= ~(MCF_INTC_IMRL_MASKALL);
    MCF_INTC0_ICR55 = MCF_INTC_ICR_IL(3)|MCF_INTC_ICR_IP(4);
	
	/* Global interrupt enable */
    asm (move.w #0x2000,sr);
	
    // Set PTC0 as output to drive LED
	MCF_GPIO_PTCPAR &= 0x01; 	//set DTIN0 as GPIO
    MCF_GPIO_DDRTC  |= 0x01;	//set DTIN0 as output
    MCF_GPIO_PORTTC |= 0x01;	//DTIN0 output high    
	
    // Setup PIT timing
    MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PRE(8);
    MCF_PIT_PMR(0) = 0xA0;

    // Enable PIT Interrupt
    MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_PIE;
    
    // Start PIT0
    MCF_PIT_PCSR(0) |= MCF_PIT_PCSR_EN;

	for(;;) {	   
	   	counter++;
	}
}

// ISR for periodic interrupt
__declspec(interrupt) void pit0_isr(void)
{


    MCF_PIT0_PCSR |= MCF_PIT_PCSR_PIF;		// Clear PIT interrupt flag
    MCF_GPIO_PORTTC ^= 0x01;				// Toggle LED on PTC0
    
}
