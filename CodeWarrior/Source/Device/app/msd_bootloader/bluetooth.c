/************************************
 *  bluetooth.c - manages all communication with BC127
 *  
 *  Rudy Moore
 *  PLSCO
 */

#include "user_config.h"

#ifdef BLUETOOTH_UART1

#include "types.h"
#include "derivative.h" /* include peripheral declarations */
#include "Boot_loader_task.h"
#include "Bootloader.h"
#include "bluetooth.h"

#define TX_BUFFER_SIZE 40
#define RX_BUFFER_SIZE 1024			// if changed, must be changed in the DMA routine and alignment attribute
#define IRQ_PRIORITY  64  // 0 = highest priority, 255 = lowest
#define IRQ_UART1_STATUS 47
#define NVIC_SET_PRIORITY(irqnum, priority)  (*((volatile uint8_t *)0xE000E400 + (irqnum)) = (uint8_t)(priority))
#define NVIC_ENABLE_IRQ(n)    (*((volatile uint32_t *)0xE000E100 + ((n) >> 5)) = (1 << ((n) & 31)))

/**** global variables ****/

// ISR modified variables
static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];

static volatile uint8_t rx_buffer[RX_BUFFER_SIZE] __attribute__((aligned(1024)));
#define RX_BUFFER_HEAD DMA_TCD0_DADDR
static volatile uint8_t * rx_buffer_tail = rx_buffer;



static volatile uint8_t transmitting = 0;
static volatile uint8_t tx_buffer_head = 0;
static volatile uint8_t tx_buffer_tail = 0;


static uint32_t LEDtimer;

uint_8          bluetooth_error=FLASH_IMAGE_SUCCESS;
extern int		ALLDONE;
extern uint_8   filetype;             /* image file type */



#define C2_ENABLE		    UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK
#define C2_TX_ACTIVE		C2_ENABLE | UART_C2_TIE_MASK
#define C2_TX_COMPLETING	C2_ENABLE | UART_C2_TCIE_MASK
#define C2_TX_INACTIVE		C2_ENABLE

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/*****************************************************************************
 *
 *    @name     bluetooth_init
 *
 *    @brief    This function initializes the bluetooth serial port
 *
 *    @param    None
 *
 *    @return   None
 *
 ****************************************************************************
 * Intializes the bluetooth serial port
 ***************************************************************************/
void bluetooth_init(void)
{
	
#define baud 9600
#define F_CPU 48000000 // this code defines this frequency all over the place. look carfully, especially at UART?_BDH definitions if you change crystals.
#define BAUD2DIV(baud)  (((F_CPU * 2) + ((baud) >> 1)) / (baud))
	
// returns true when (Wait) ?????seconds have gone by. 	
#define SLEEPEXPIRED(timer,wait) (DWT_CYCCNT - (timer) >= (wait)*F_CPU/4/1000)
// needed after each expiration of the timer
#define RESETTIMER(timer) timer = DWT_CYCCNT
	
	int divisor = BAUD2DIV(baud);
	
	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;	// turn on clock, TODO: use bitband
	tx_buffer_head = 0;
	tx_buffer_tail = 0;
	transmitting = 0;
	
	/** serial port pin settings */
	
#ifdef MCU_MK20D7
	PORTC_PCR3 = PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_PFE_MASK | PORT_PCR_MUX(3); // these pins are used when compiled for MCU_MK70F12
#endif
	
	PORTC_PCR4 = PORT_PCR_DSE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_MUX(3);
	
	// verifying numbers, from the Manual:
	// UART baud rate = UART module clock / (16 * ((BDH<<8|BDL) + BRFD))
	//  For UART0 and 1, module clock = system clock
	// 	   UART0 and UART1 are clocked by F_CPU, UART2 is clocked by F_BUS
	//     system clock verified to be 48Mhz. See BSP_CORE_CLOCK in main_kinetis.c
	//	9600 = 48000000 / (16 * (SBR + BRFD))
	//  16 * (SBR + BRFD) = 48000000 / 9600
	//  SBR + BRFD = 48000000 / (9600 * 16) = 312.5
	//  SBR = 312; BDH = 0x01, BDL = 0x38 
	//  BRFD = .5 = UART1_C4/32 = 0x10
	
	// from defines above:
	// BAUD2DIV = (((48000000 * 2) + (9600 / 2)) / 9600) = 10000.5 = 0x2710 = (0x01 << 13) | (0x38 << 5) | 0x08
	//  so it matches.
	
	UART1_BDH = (divisor >> 13) & 0x1F;
	UART1_BDL = (divisor >> 5) & 0xFF;
	UART1_C4 = divisor & 0x1F;
	
	UART1_C1 = 0; // configures 8-bit, no parity
	
#ifdef BLUETOOTH_UART_FIFO	
	// set up FIFOs. At 9600 baud, 8 word fifos mean you have 8 milliseconds to service an interrupt if the fifo is empty
	UART1_C2 = 0; // ensures that Transmit Enable and Receive Enable are turned off before adjusting PFIFO.
	UART1_PFIFO = UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK; // enables the transmit and receive fifos. UART0 and UART1 have 8 byte fifos
	UART1_CFIFO = UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK // TXFLUSH and RXFLUSH must issued after any FIFO change
			 | UART_CFIFO_RXOFE_MASK | UART_CFIFO_TXOFE_MASK ;		// enable the fifo overflow interrupts - may as well know when we've fucked it all up by missing chars.
#else
	UART1_PFIFO = 0; // no fifo since we're setting up DMA
#endif
	
	// UART DMA RECEIVE CONFIGURATION
	//enable DMA clocks
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK; 
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
	
	DMAMUX_CHCFG0 = 0;							// disable the channel first
	 // config the dma channel
	DMA_TCD0_CSR = 0;
	DMA_CR = 0;				// default control register
	DMA_DCHPRI0 = 0;		// DMA channel 0 priority default to 0
	DMA_EEI = 0;			// all error interrupts are cleared. might be needed if we're debugging..
	// now set up TCD
	DMA_TCD0_SADDR = (uint32_t) &UART1_D;		// reading from UART1 data register
	DMA_TCD0_SOFF = 0;
	DMA_TCD0_ATTR = DMA_ATTR_SMOD(0) | DMA_ATTR_SSIZE(0)    // source does not iterate - no modulo, source is a byte in size, 
			| DMA_ATTR_DMOD(10) | DMA_ATTR_DSIZE(0); 		// destination is a byte, 1 kB of buffer space, address must align with a 1k boundary
	DMA_TCD0_NBYTES_MLNO = 1;								// one byte is transferred each time.
	DMA_TCD0_SLAST = 0;										// the source address never changes
	DMA_TCD0_DADDR = (uint32_t) &rx_buffer;
	DMA_TCD0_DOFF = 1;										// incoming byte increment by 1 through the rx_buffer, wrapping around at the 1k boundary
	DMA_TCD0_CITER_ELINKNO = 1;								// single major loop. CITER and BITER must be initialized to the same value.
	DMA_TCD0_BITER_ELINKNO = 1;
	DMA_TCD0_DLASTSGA = 0;									// no adjustment to destination address... assuming that destination address has not been changed.
	
	 // enable the dma channel
	DMA_ERQ |= DMA_ERQ_ERQ0_MASK;
	DMAMUX_CHCFG0 = DMAMUX_CHCFG_ENBL_MASK | 0x04;						// sets channel 0 source to UART1 receive
	UART1_C5 = UART_C5_RDMAS_MASK;				// Now, when an character is received, as long as the receive interrupt is enabled, a DMA request will be sent.
	
	UART1_C2 = C2_ENABLE;						// enable the UART
	
	// setup the interrupts - now used for TX only...
	NVIC_SET_PRIORITY(IRQ_UART1_STATUS, IRQ_PRIORITY);
	NVIC_ENABLE_IRQ(IRQ_UART1_STATUS);
	
#define DEMCR_TRCENA			(1 << 24)	 	// Enable debugging & monitoring blocks
#define DWT_CTRL_CYCCNTENA		(1 << 0)		// Enable cycle count

	// timer init
	DEMCR |= DEMCR_TRCENA;
	DWT_CTRL |= DWT_CTRL_CYCCNTENA;				// this enables the Debug unit's cycle counter
	RESETTIMER(LEDtimer);

	/*
	// BC127 baud rate
	bluetooth_putstr((uint8_t *)"SET BAUD=2400\r");
	while (!SLEEPEXPIRED(LEDtimer,100));
	RESETTIMER(LEDtimer);
	divisor = BAUD2DIV(2400);
	UART1_BDH = (divisor >> 13) & 0x1F;
	UART1_C4 = divisor & 0x1F;
	UART1_BDL = (divisor >> 5) & 0xFF;
	*/
}

// a *small* string comparison routine
// comparison ends when str2 is null terminated or there's a mismatch
// str1 doesn't control the end because we don't want to find str1 in a subset of str2, only the other way around works.
// maximum of 50 characters compared
uint32_t bluetooth_strcmp( uint8_t * str1, uint8_t * str2)
{
	uint32_t cnt = 50;
	
	while ((uint8_t)(*((uint_8*)str2)) && cnt) {
		// str1 != str2 would be enough, except that we don't want the wildcard character to match on a null.
		if (!((uint8_t)(*((uint_8*)str1))) || (((uint8_t)(*((uint_8*)str2)) != '?') && ((uint8_t)(*((uint_8*)str2)) != (uint8_t)(*((uint_8*)str1))) ) ) return 1;
		else {
			str1++; str2++; cnt--;
		}
	}
	
	return 0; // match found!
}

/* converts ascii digits into a sixteen bit number. warning, no checking! */
uint32_t bluetooth_str2num( uint8_t *str1 )
{
	return (uint32_t)(((*str1)&0xf)<<12) | ((*(str1+1)&0xf)<<8) | ((*(str1+2)&0xf)<<4) | (*(str1+3)&0xf);
}

typedef enum statesenum { UNKNOWNCONNECTION, READSTATUS, ADVERTISE, WAITFORHEADER, PULLDOWNFLASH, SENDENDSIGNAL } states;
typedef enum bluetootherrorsenum { NOERROR, FLASHBUFFEROVERRUN, SIZEWRONG } bluetootherrors;
#define MAXCOMMANDBUFFER 255
#define MAXFLASHCOUNT (MAXCOMMANDBUFFER - 15)
#define BLE_DATA_FRAME_SIZE 20

// number of 2 second attempts to make before issuing ADVERTISING ON
#define STATUSATTEMPTS 5

void bluetooth_task(void)
{
	static uint32_t flashtotalsize = 0;
	static uint32_t flashcurrentsize = 0;
	static uint8_t command[MAXCOMMANDBUFFER];			// largest SPP packet with 20 chars is 36 bytes large
	static uint8_t commandcount = 0;
	static uint32_t flashcount = 0;
	static uint8_t writingflashbuffer = 0;
	static uint8_t statusattemptcountdown = STATUSATTEMPTS;
//	static states state = UNKNOWNCONNECTION;
	static states state = WAITFORHEADER;
	
	static bluetootherrors errorcode = NOERROR;
	static uint8_t senterrorcode=0;
		
	static uint32_t timercounter;
	
	static uint32_t totallines = 0;
	static uint32_t maxrxqueue=0;
	
	
	
	uint32_t c;
	uint32_t processline = 0;
	
	// buffer collection
	if ((c = bluetooth_getchar()) != 0xffffffff ) {
		command[commandcount] = c;
		if (commandcount < (MAXCOMMANDBUFFER-1)) commandcount++;	// extra chars write over end of buffer
		command[commandcount] = 0;									// necessary since the buffer is being reused but we don't want strcmp to match on old data
		
		// if we're getting a data field, we have to suspend 0x0d processing while collecting the data field
		if ((state == WAITFORHEADER || state == PULLDOWNFLASH) &&				// check to see if we're in a collecting state
				(!(bluetooth_strcmp(command,(uint8_t *)"RECV BLE "))) &&		// check for iOS preamble
				(flashcount < MAXFLASHCOUNT)) {
			if (!flashcount) {
				flashcount = MAXFLASHCOUNT - BLE_DATA_FRAME_SIZE;				// BLE implementation can only handle a static number of characters at a time
			} else if (flashcount < MAXFLASHCOUNT) {							// check to see if we've maxed out the number of characters we are supposed to receive
				flashcount++;
			}
		} else if ((state == WAITFORHEADER || state == PULLDOWNFLASH) &&		// check to see if we're in a collecting state
				(!(bluetooth_strcmp(command,(uint8_t *)"RECV ??P ???? "))) &&	// check to see if the proper preamble has arrived - matches "SPP" or "IAP"
				(flashcount < MAXFLASHCOUNT))	{	
			if (!flashcount) {
				c = bluetooth_str2num(command+9);
				if (c < MAXFLASHCOUNT-1) flashcount = MAXFLASHCOUNT - c;
				else {
					flashcount = 1;												// this tries to absorb any 0x0d in the code to keep alignment, but it's still an error
					bluetooth_error = FLASH_IMAGE_ERROR_FRAME_TOO_LARGE;		// won't actually work during the header, but should catch the error on the next flash command...
				}
			} else if (flashcount < MAXFLASHCOUNT) {							// check to see if we've maxed out the number of characters we are supposed to receive
				flashcount++;
			}
	
		} else if ((c & 0xff) == 0x0d) {									// at end of line, we process the line
			commandcount = 0;
			processline = 1;
		}
	}

	// handles possibility of stalled input after sending a STATUS command
	if (state == READSTATUS && SLEEPEXPIRED(timercounter,4000)) {
		processline=1;
		commandcount=0;						// reset whatever you were attempting to read...
		if (statusattemptcountdown--) {
			state = UNKNOWNCONNECTION;
		} else {
			statusattemptcountdown = STATUSATTEMPTS;
			state = ADVERTISE;
		}
	}
	
	
	// buffer processing
	// messages are like these:
	// RECV SPP 0014 <20 bytes><0x0A><0x0D>
	if (processline) {
		processline = 0;					// you only get one processline per completed buffer
		flashcount = 0;

		switch (state) {
												
			case UNKNOWNCONNECTION:
				// request STATUS
				bluetooth_putstr((uint8_t *)"STATUS\r");
				state = READSTATUS;
				RESETTIMER(timercounter);	// save the current cycle counter
				break;
			case READSTATUS:
				// process line for "CONNECTED" (alternatives "CONNECTED SPP" and "CONNECTED BLE")
				// timeout back to READSTATUS after 2 seconds
				if (!bluetooth_strcmp(command,(uint8_t *)"STATE CONNECTED")) {
					state = WAITFORHEADER;
				} else { // something was received that was not a CONNECTED message, let's check status again.
					state = UNKNOWNCONNECTION;
				}
				break;
			case ADVERTISE:
				bluetooth_putstr((uint8_t *)"ADVERTISING ON\r");		
				bluetooth_putstr((uint8_t *)"DISCOVERABLE ON\r");
				state = UNKNOWNCONNECTION;
				break;
				
			case WAITFORHEADER:
				// detect either "RECV SPP ???? GLOW" or "RECV BLE GLOW"
				// parse ???? bytes
				// check 20 bytes in data section
				
				/* 
				if (!bluetooth_strcmp(command,(uint8_t *)"RECV SPP 0004 GFU^")) {
					active_file = TRUE; // this prevents any other flash writer from running.
					erase_flash();
					
					filetype = UNKNOWN; 
			//		pixel(3,0,0x00ff00ff);	// just to let you know the erase happened here.
					
					state = WAITFORHEADER;				 
				} else 
				*/
				if (!bluetooth_strcmp(command,(uint8_t *)"RECV SPP 0028 GLOW")) {
					flashtotalsize =  ((uint32_t)command[18] << 24) | 
									 (((uint32_t)command[19] << 16) & 0x00ff0000) |
									 (((uint32_t)command[20] <<  8) & 0x0000ff00) |
									 ( (uint32_t)command[21]        & 0x000000ff);
					flashcurrentsize = 0;
					active_file = TRUE; // this prevents any other flash writer from running.
					erase_flash();
					filetype = UNKNOWN; 
				
					bluetooth_error = FLASH_IMAGE_SUCCESS;
					if (bluetooth_error == FLASH_IMAGE_SUCCESS) bluetooth_error = FlashApplication(command+34, 20);
					flashcurrentsize += 20;
										
					state = PULLDOWNFLASH;				 
				} else if (!bluetooth_strcmp(command,(uint8_t *)"RECV SPP 0014 GLOW")) {
					flashtotalsize =  ((uint32_t)command[18] << 24) | 
									 (((uint32_t)command[19] << 16) & 0x00ff0000) |
									 (((uint32_t)command[20] <<  8) & 0x0000ff00) |
									 ( (uint32_t)command[21]        & 0x000000ff);
					flashcurrentsize = 0;
					active_file = TRUE; // this prevents any other flash writer from running.
					erase_flash();
					filetype = UNKNOWN; 
					bluetooth_error = FLASH_IMAGE_SUCCESS;
					
					state = PULLDOWNFLASH;				 
				} else if (!bluetooth_strcmp(command,(uint8_t *)"RECV BLE GLOW")) {
					flashtotalsize =  ((uint32_t)command[13] << 24) | 
									 (((uint32_t)command[14] << 16) & 0x00ff0000) |
									 (((uint32_t)command[15] <<  8) & 0x0000ff00) |
									 ( (uint32_t)command[16]        & 0x000000ff);
					flashcurrentsize = 0;
					active_file = TRUE; // this prevents any other flash writer from running.
					erase_flash();
					filetype = UNKNOWN; 
					state = PULLDOWNFLASH;
				} else if (SLEEPEXPIRED(timercounter,10000)) { // if ten second go by without getting the header, assume something is crashed and start looking for devices
						state = UNKNOWNCONNECTION;
					}
				break;
				
			case PULLDOWNFLASH:
				// start flashing
				if (!bluetooth_strcmp(command,(uint8_t *)"RECV SPP ???? ")) {
					c = bluetooth_str2num(command+9);
					if (c > MAXFLASHCOUNT) bluetooth_error = FLASH_IMAGE_ERROR_FRAME_TOO_LARGE;
					if (c & 0x3) bluetooth_error = FLASH_IMAGE_ERROR_FRAME_NOT_DIVISIBLE_BY_FOUR;
					if (bluetooth_error == FLASH_IMAGE_SUCCESS) bluetooth_error = FlashApplication(command+14, c);
					flashcurrentsize += c;
					totallines++;
				} else if (!bluetooth_strcmp(command,(uint8_t *)"RECV BLE ")) {
					if (bluetooth_error == FLASH_IMAGE_SUCCESS) bluetooth_error = FlashApplication(command+9, BLE_DATA_FRAME_SIZE);
					flashcurrentsize += BLE_DATA_FRAME_SIZE;
				}
				// filling graphic
				// this takes about 43msec to complete, so it could be a problem depending on how long the flashing takes.
				if (bluetooth_error != FLASH_IMAGE_SUCCESS) { 
					if (!senterrorcode) { // forces this to only be sent once					
						pixel(3,0,bluetooth_error);
						senterrorcode++;
				
					}
				} else if (SLEEPEXPIRED(LEDtimer,400)) { 
					RESETTIMER(LEDtimer);
					pixel(2,flashcurrentsize,flashtotalsize);
				}
				
				// are we finished?
				if (flashcurrentsize >= flashtotalsize) {
					state = SENDENDSIGNAL;
					bluetooth_putstr((uint8_t *)"SEND GFU:OK^\r");
					ALLDONE = 1;									// turns off the LEDS like the disk loader does
				}
				break;
				
			case SENDENDSIGNAL:
					// any new lines that come in should be ignored. we've finished programming the flash!
				
				break;
										
		}
		
	}
		
	

}


void bluetooth_end(void)
{
	if (!(SIM_SCGC4 & SIM_SCGC4_UART1_MASK)) return;
//	while (transmitting) {}  // wait for buffered data to send TODO: ???
	NVIC_DISABLE_IRQ(IRQ_UART1_STATUS);
	UART1_C2 = 0;
	PORTC_PCR3 = PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_MUX(1);
	PORTC_PCR4 = PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_MUX(1);
}

void bluetooth_putchar(uint32_t c)
{
	uint32_t head;

	if (!(SIM_SCGC4 & SIM_SCGC4_UART1_MASK)) return;
	head = tx_buffer_head;
	if (++head >= TX_BUFFER_SIZE) head = 0;
	if (tx_buffer_tail == head) {					// the transmit buffer is completely full, we need to wait until it's empty.
		// options: 
		//		1. change the priority of the TX IRQ (this is what the Teensy code does)
		//      2. wait. but this could block forever
		//		3. give an error message. great idea. but, to who?
		//		4. write over the tail!
		// option 4:
		head = tx_buffer_head;
	}
	tx_buffer[head] = c;
	transmitting = 1;
	tx_buffer_head = head;
	UART1_C2 = C2_TX_ACTIVE;
}

#define MAXSENDSIZE 255
void bluetooth_putstr(uint8_t *command) {
	uint8_t i;
	
	for (i=0; command[i] && i<=MAXSENDSIZE; i++) {
		bluetooth_putchar(command[i]);
	}

}

uint32_t bluetooth_getchar(void)
{
	uint32_t c;
	
	if (((uint8_t*)RX_BUFFER_HEAD) == rx_buffer_tail) return -1;
	c = (uint32_t) *rx_buffer_tail;
	if (++rx_buffer_tail >= rx_buffer + RX_BUFFER_SIZE) rx_buffer_tail = rx_buffer;
	return c;
}


// status interrupt combines 
//   Transmit data below watermark  UART_S1_TDRE -> UART_S1_TDRE_MASK
//									UART_C2_TIE -> UART_C2_TIE_MASK
//   Transmit complete              UART_S1_TC -> UART_S1_TC_MASK
//   Idle line                      UART_S1_IDLE -> UART_S1_IDLE_MASK
//   Receive data above watermark   UART_S1_RDRF -> UART_S1_RDRF_MASK
//   LIN break detect               UART_S2_LBKDIF -> UART_S2_LBKDIF_MASK
//   RxD pin active edge            UART_S2_RXEDGIF -> UART_S2_RXEDGIF_MASK

void bluetooth_status_isr(void)
{
	uint32_t head, tail, n;
	uint8_t c;

	/*** RX PATH not used since we're usng the DMA to receive RX characters
	// check receive buffer
	if (UART1_S1 & UART_S1_RDRF_MASK) { 								// receive databuffer read fifo full
		n = UART1_D;													// grab the word

																		// add the word to the rx_buffer
		head = rx_buffer_head + 1;
		if (head >= RX_BUFFER_SIZE) head = 0;
		if (head != rx_buffer_tail) {
			rx_buffer[head] = n;
			rx_buffer_head = head; 
		}
	}
	*****/
	
	// check transmit buffer
	c = UART1_C2;
	if ((c & UART_C2_TIE_MASK) && (UART1_S1 & UART_S1_TDRE_MASK)) {		// Transmit interrupt enabled and below watermark
		head = tx_buffer_head;
		tail = tx_buffer_tail;
		if (head == tail) {												// buffer empty
			UART1_C2 = C2_TX_COMPLETING;
		} else {
			if (++tail >= TX_BUFFER_SIZE) tail = 0;
			n = tx_buffer[tail];										// pop next word off tx fifo

			UART1_D = n;												// send the word
			tx_buffer_tail = tail;
		}
	}
	
	// check for transmit complete
	if ((c & UART_C2_TCIE_MASK) && (UART1_S1 & UART_S1_TC_MASK)) {
		transmitting = 0;
		UART1_C2 = C2_TX_INACTIVE;
	}
	
	/* Not using fifos since the DMA is being implemented 
	// This may belong in an error interrupt routine...
	c = UART1_SFIFO;
	if (c & UART_SFIFO_RXOF_MASK) {
		// receive buffer overflow!!
		// service those interrupts faster!
	}
	if (c & UART_SFIFO_TXOF_MASK) {
		// transmit buffer overflow!!
		// wait until the transmitter's ready to transmit!
	}
	if (c & UART_SFIFO_RXUF_MASK) {
		// receive buffer underflow!!
		// you pulled too much data, you ninny
	}
	*/
	
}

#endif // BLUETOOTH_UART1
/***
 * This code is based off code from the Teensyduino Core Library. The following notice is reproduced from there.
 * 
 * /* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows 
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
