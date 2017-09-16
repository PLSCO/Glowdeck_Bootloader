/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:  This file includes operation of serial communication interface.
*
*END************************************************************************/
#include "types.h"
#include "sci_cfv1_plus.h"
#include "derivative.h"
#include "user_config.h"

#include <Uart.h>


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : sci_init
* Returned Value   :
* Comments         : This function initializes the UART0 baud rate.
*    
*
*END*----------------------------------------------------------------------*/

void sci_init(void) 
{

}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : TERMIO_PutChar
* Returned Value   :
* Comments         :
*                     This function sends a char via SCI.
*
*END*----------------------------------------------------------------------*/
void TERMIO_PutChar(char send) 
{
	char dummy;
	/* Wait until transmitter is idle */
	while(!(UART0_S1 & UART_S1_TC_MASK)){};
	dummy = UART0_S1;
	/* Send the character */
	UART0_D  = send;    
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : TERMIO_GetChar
* Returned Value   : the char get via SCI
* Comments         :
*                     This function gets a char via SCI.
*
*END*----------------------------------------------------------------------*/
char TERMIO_GetChar(void) 
{
	char dummy;
	
	while(!(UART0_S1 & UART_S1_RDRF_MASK)){};

	dummy = UART0_S1;
	return UART0_D; 
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : Initialize UART
* Returned Value   :
* Comments         :
*                    This function initializes the UART for console printf/scanf
*
*END*----------------------------------------------------------------------*/
UARTError InitializeUART(UARTBaudRate baudRate)
{
	uint_32 	baud_divisor;
	uint_32	fractional_fine_adjust;
	/* Calculate baud settings */
	/* UART baud rate = UART module clock / (16 × (SBR[SBR] +BRFD)) */
	fractional_fine_adjust = (uint_32)(SYSTEM_CLOCK/(baudRate/2));
	baud_divisor = (uint_32)(fractional_fine_adjust >>5);
	fractional_fine_adjust -= (baud_divisor <<5);
	if (baud_divisor > 0x1fff) 
	{
		return kUARTUnknownBaudRate;
	}
	/* Set baudrate */
	UART0_BDH |= ((baud_divisor>>8) & UART_BDH_SBR_MASK);
	UART0_BDL = (uint_8)(baud_divisor & 0xFF);

	/* 8 bits data, no parity */
	UART0_C1 = 0x0;
	
	UART0_C4  |= UART_C4_BRFA(fractional_fine_adjust);
	
	/* Enable receiver and transmitter */ 
	UART0_C2  |= UART_C2_RE_MASK | UART_C2_TE_MASK;

 return kUARTNoError;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : WriteUARTN
* Returned Value   :
* Comments         :
*                    This function writes N bytes on the SCI1 console output
*
*END*----------------------------------------------------------------------*/
UARTError WriteUARTN(const void* bytes, unsigned long length) 
{
  int i;
  char* src = (char*)bytes;
  
  for(i = 0; i< length; i++) 
  {
   TERMIO_PutChar(*src++);
  }

 return kUARTNoError;  
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : ReadUARTN
* Returned Value   :
* Comments         :
*                    This function reads N bytes on the SCI1 console input
*
*END*----------------------------------------------------------------------*/
UARTError ReadUARTN(void* bytes, unsigned long length)
{
  int i;
  char *dst = (char*)bytes;
  
  for(i = 0; i< length; i++) 
  {
   *dst++ = TERMIO_GetChar();
  }
  
  return kUARTNoError;
}
/* EOF */
