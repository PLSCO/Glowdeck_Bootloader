/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <MK20D7.h>

#define __MK_xxx_H__
#define BOOTLOADER_APP
#define printf printf_kinetis
#define sprintf sprintf_kinetis

#define BSP_LED2 1 << 28
#define BSP_LED3 1 << 29

#define SYSCLK   48000000/*FSL: fixed for 48MHz clk, then cpu and bus have the same freq*/
#define BSP_CLOCK_SRC                   (16000000ul)       // 8MHz crystal, oscillator freq
#define EXTERNAL_OSC /*FSL: only if external osc is used*/

#define TERM_PORT                       UART1_BASE_PTR
#define USED_PIT0

#define NO_PRINTF_SUPPORT

