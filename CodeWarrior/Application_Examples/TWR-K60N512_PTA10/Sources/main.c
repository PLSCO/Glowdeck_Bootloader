/******************************************************************************
*                                                  
*  File Name: main.c
*                                                                          
*  Purpose: This file is for a USB Mass-Storage Device bootloader.  This file 
*           has the main routine for the user application with the bootloader.
*           In this case, the application simply blinks an LED on PTA10 using
*           a timer interrupt.  
*******************************************************************************/



#include <stdio.h>

#include "derivative.h" /* include peripheral declarations */
#include "Bootloader.h"



int main(void)
{
	int counter = 0;
	uint_32_ptr pdst,psrc;
	uint_32 i;
	
	// Copy Application Stored Interrupt Vector table to RAM
	pdst=(uint_32*)0x1FFF0000;
	psrc=IMAGE_ADDR;
	  for (i=0;i<0x100;i++,pdst++,psrc++)
	  {
	    *pdst=*psrc;
	  }
	
	  // Redirect the vector table to the new copy in RAM 
	  SCB_VTOR = (uint_32)0x1FFF0000;

	  // Enable clocks to peripherals
	  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;     // Enable clock gate for PTA
	  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;		 // Enable clock gate for PIT0                       

	  // Setup PTA10 to drive LED
	  GPIOA_PDOR &= ~0x400;            	// Drive PTA10 low          
	  GPIOA_PDDR |= 0x0400;				// Set PTA10 as output                       
	  PORTA_PCR10 = (PORTA_PCR10 & ~( PORT_PCR_ISF_MASK |	// Clear interrupt flag
									  PORT_PCR_LK_MASK)		// Not locked
								| (	  PORT_PCR_MUX(1)));	// GPIO function
	  
	  // Configure the PIT for periodic interrupt
	  PIT_MCR = 0;          			// Enable device clock 
	  PIT_TCTRL0 = 0;       			// Clear control register 
	  PIT_TFLG0 = PIT_TFLG_TIF_MASK;    // Clear timer flag register 
	  PIT_LDVAL0 = 0x00A00000; 			// Set up load register 
	  PIT_TCTRL0 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;		// Enable PIT and interrupt
	  
	  // Setup NVIC for PIT interrupt
	  NVICIP68 = 0x80;  				// NVIC priority for PIT interrupt                           
	  NVICISER2 |= 0x10; 				// SETENA|=0x10    
	  NVICICPR2 |= 0x10;

	  // Enable global interrupts
	  asm ("CPSIE  i");
	  
	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}


void pit0_isr(void) {
	  PIT_TFLG0 = PIT_TFLG_TIF_MASK;        // Clear timer flag register 
	  (void)PIT_LDVAL0;						// Read LDVAL0 to clear flag
	  GPIOA_PDOR ^= 0x400;            		// Toggle LED on PTA10          
}
