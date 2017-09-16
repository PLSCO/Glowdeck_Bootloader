/******************************************************************************
*                                                  
*  File Name: main.c
*                                                                          
*  Purpose: This file is for a USB Mass-Storage Device bootloader.  This file 
*           has the main routine for the user application with the bootloader.
*           In this case, the application simply blinks an LED on PTE2 using
*           a timer interrupt.  
*******************************************************************************/


#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "Bootloader.h"
#include "startcf.h"

typedef void (* pFun)(void);
__interrupt void Timer_Overflow(void);
__interrupt void dummy_ISR(void);


// Declare a dummy ISR to fill the unused vectors
__interrupt void dummy_ISR(void) 
{
  
  
}

// Declare the stored vector table for the application, 
// and locate it at the beginning of unprotected flash
void  (* const RAM_vector[])()@(int)&IMAGE_ADDR[0]= {
    (pFun)&__SP_INIT,             // vector_0  INITSP
    (pFun)&_startup,              // vector_1  INITPC
    (pFun)&dummy_ISR,             // vector_2  Vaccerr
    (pFun)&dummy_ISR,             // vector_3  Vadderr
    (pFun)&dummy_ISR,             // vector_4  Viinstr
    (pFun)&dummy_ISR,             // vector_5  VReserved5
    (pFun)&dummy_ISR,             // vector_6  VReserved6
    (pFun)&dummy_ISR,             // vector_7  VReserved7
    (pFun)&dummy_ISR,             // vector_8  Vprviol
    (pFun)&dummy_ISR,             // vector_9  Vtrace
    (pFun)&dummy_ISR,             // vector_10 Vunilaop
    (pFun)&dummy_ISR,             // vector_11 Vunilfop
    (pFun)&dummy_ISR,             // vector_12 Vdbgi
    (pFun)&dummy_ISR,             // vector_13 VReserved13
    (pFun)&dummy_ISR,             // vector_14 Vferror
    (pFun)&dummy_ISR,             // vector_15 VReserved15
    (pFun)&dummy_ISR,             // vector_16 VReserved16
    (pFun)&dummy_ISR,             // vector_17 VReserved17
    (pFun)&dummy_ISR,             // vector_18 VReserved18
    (pFun)&dummy_ISR,             // vector_19 VReserved19
    (pFun)&dummy_ISR,             // vector_20 VReserved20
    (pFun)&dummy_ISR,             // vector_21 VReserved21
    (pFun)&dummy_ISR,             // vector_22 VReserved22
    (pFun)&dummy_ISR,             // vector_23 VReserved23
    (pFun)&dummy_ISR,             // vector_24 Vspuri
    (pFun)&dummy_ISR,             // vector_25 VReserved25
    (pFun)&dummy_ISR,             // vector_26 VReserved26
    (pFun)&dummy_ISR,             // vector_27 VReserved27 
    (pFun)&dummy_ISR,             // vector_28 VReserved28
    (pFun)&dummy_ISR,             // vector_29 VReserved29
    (pFun)&dummy_ISR,             // vector_30 VReserved30
    (pFun)&dummy_ISR,             // vector_31 VReserved31
    (pFun)&dummy_ISR,             // vector_32 Vtrap0
    (pFun)&dummy_ISR,             // vector_33 Vtrap1
    (pFun)&dummy_ISR,             // vector_34 Vtrap2 
    (pFun)&dummy_ISR,             // vector_35 Vtrap3
    (pFun)&dummy_ISR,             // vector_36 Vtrap4
    (pFun)&dummy_ISR,             // vector_37 Vtrap5
    (pFun)&dummy_ISR,             // vector_38 Vtrap6
    (pFun)&dummy_ISR,             // vector_39 Vtrap7
    (pFun)&dummy_ISR,             // vector_40 Vtrap8
    (pFun)&dummy_ISR,             // vector_41 Vtrap9 
    (pFun)&dummy_ISR,             // vector_42 Vtrap10
    (pFun)&dummy_ISR,             // vector_43 Vtrap11
    (pFun)&dummy_ISR,             // vector_44 Vtrap12
    (pFun)&dummy_ISR,             // vector_45 Vtrap13 
    (pFun)&dummy_ISR,             // vector_46 Vtrap14
    (pFun)&dummy_ISR,             // vector_47 Vtrap15
    (pFun)&dummy_ISR,             // vector_48 VReserved48
    (pFun)&dummy_ISR,             // vector_49 VReserved49
    (pFun)&dummy_ISR,             // vector_50 VReserved50
    (pFun)&dummy_ISR,             // vector_51 VReserved51
    (pFun)&dummy_ISR,             // vector_52 VReserved52
    (pFun)&dummy_ISR,             // vector_53 VReserved53
    (pFun)&dummy_ISR,             // vector_54 VReserved54
    (pFun)&dummy_ISR,             // vector_55 VReserved55
    (pFun)&dummy_ISR,             // vector_56 VReserved56
    (pFun)&dummy_ISR,             // vector_57 VReserved57
    (pFun)&dummy_ISR,             // vector_58 VReserved58
    (pFun)&dummy_ISR,             // vector_59 VReserved59
    (pFun)&dummy_ISR,             // vector_60 VReserved60
    (pFun)&dummy_ISR,             // vector_61 Vunsinstr
    (pFun)&dummy_ISR,             // vector_62 VReserved62
    (pFun)&dummy_ISR,             // vector_63 VReserved63
    (pFun)&dummy_ISR,             // vector_64 Virq
    (pFun)&dummy_ISR,             // vector_65 Vlvd
    (pFun)&dummy_ISR,             // vector_66 Vlol
    (pFun)&dummy_ISR,             // vector_67 Vspi1
    (pFun)&dummy_ISR,             // vector_68 Vspi2 
    (pFun)&dummy_ISR,             // vector_69 Vusb
    (pFun)&dummy_ISR,             // vector_70 VReserved70
    (pFun)&dummy_ISR,             // vector_71 Vtpm1ch0
    (pFun)&dummy_ISR,             // vector_72 Vtpm1ch1
    (pFun)&dummy_ISR,             // vector_73 Vtpm1ch2
    (pFun)&dummy_ISR,             // vector_74 Vtpm1ch3 
    (pFun)&dummy_ISR,             // vector_75 Vtpm1ch4
    (pFun)&dummy_ISR,             // vector_76 Vtpm1ch5
    (pFun)&Timer_Overflow,             // vector_77 Vtpm1ovf
    (pFun)&dummy_ISR,             // vector_78 Vtpm2ch0
    (pFun)&dummy_ISR,             // vector_79 Vtpm2ch1
    (pFun)&dummy_ISR,             // vector_80 Vtpm2ovf
    (pFun)&dummy_ISR,             // vector_81 Vsci1err
    (pFun)&dummy_ISR,             // vector_82 Vsci1rx
    (pFun)&dummy_ISR,             // vector_83 Vsci1tx
    (pFun)&dummy_ISR,             // vector_84 Vsci2err
    (pFun)&dummy_ISR,             // vector_85 Vsci2rx
    (pFun)&dummy_ISR,             // vector_86 Vsci2tx
    (pFun)&dummy_ISR,             // vector_87 Vkeyboard
    (pFun)&dummy_ISR,             // vector_88 Vadc
    (pFun)&dummy_ISR,             // vector_89 Vacmpx
    (pFun)&dummy_ISR,             // vector_90 Viic1x
    (pFun)&dummy_ISR,             // vector_91 Vrtc
    (pFun)&dummy_ISR,             // vector_92 Viic2x
    (pFun)&dummy_ISR,             // vector_93 Vcmt
    (pFun)&dummy_ISR,             // vector_94 Vcanwu
    (pFun)&dummy_ISR,             // vector_95 Vcanerr 
    (pFun)&dummy_ISR,             // vector_96 Vcanrx
    (pFun)&dummy_ISR,             // vector_97 Vcantx
    (pFun)&dummy_ISR,             // vector_98 Vrnga
    (pFun)&dummy_ISR,             // vector_99 VReserved99
    (pFun)&dummy_ISR,             // vector_100 VReserved100
    (pFun)&dummy_ISR,             // vector_101 VReserved101 
    (pFun)&dummy_ISR,             // vector_102 VReserved102
    (pFun)&dummy_ISR,             // vector_103 VReserved103
    (pFun)&dummy_ISR,             // vector_104 VL7swi 
    (pFun)&dummy_ISR,             // vector_105 VL6swi
    (pFun)&dummy_ISR,             // vector_106 VL5swi
    (pFun)&dummy_ISR,             // vector_107 VL4swi
    (pFun)&dummy_ISR,             // vector_108 VL3swi
    (pFun)&dummy_ISR,             // vector_109 VL2swi
    (pFun)&dummy_ISR,             // vector_110 VL1swi
};

// Periodic timer ISR
__interrupt void Timer_Overflow(void)
{

   TPM1SC_TOF = 0;     	// clear timer interrupt flag
   PTED_PTED2 ^=1;		// Toggle LED on PTE2

}



void main(void) {

	  uint_32_ptr pdst,psrc;
	  byte i;

	  // Move vector table to start of RAM, 0x800000
	asm (move.l  #0x00800000,d0);
	asm (movec  d0,vbr);
	
	// Copy stored application interrupt vectors
	// to re-directed vector table in RAM
	pdst=(uint_32_ptr)0x00800000;
	psrc=(uint_32_ptr)&RAM_vector;
	  for (i=0;i<111;i++,pdst++,psrc++)
	  {
	    *pdst=*psrc;
	  }
	

	  EnableInterrupts;
  /* include your code here */

	  PTEDD_PTEDD2 = 1;			// Setup PTE2 as output to drive LED
	  PTED_PTED2 = 0;			// Turn on LED at PTE2
	  
	  // Setup timer for periodic interrupt
	  TPM1SC_CLKSx = 0x01;   	// bus clock
	  TPM1SC_PS = 7;      		// bus clock
	  TPM1MOD = 23437;    		// Period is 500ms
	  
	  TPM1SC_TOIE = 1;			// Enable timer interrupt
  

  for(;;) {
    __RESET_WATCHDOG();	/* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

