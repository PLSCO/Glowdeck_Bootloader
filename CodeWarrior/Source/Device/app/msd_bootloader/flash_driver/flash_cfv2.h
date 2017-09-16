/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash_cfv2.h
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief flash driver header file
 *
 *****************************************************************************/
#ifndef _FLASH_CFV2_H_
#define _FLASH_CFV2_H_
/*********************************** Includes ***********************************/

/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/
/* error code */
#define Flash_OK           0x00
#define Flash_FACCERR      0x01
#define Flash_FPVIOL       0x02
#define Flash_NOT_ERASED   0x04
#define Flash_CONTENTERR   0x08
#define Flash_NOT_INIT     0xFF

/* flash commands */
#define FlashCmd_EraseVerify    0x05
#define FlashCmd_Program        0x20
#define FlashCmd_BurstProgram   0x25
#define FlashCmd_SectorErase    0x40
#define FlashCmd_MassErase      0x41
#define BSP_SYSTEM_CLOCK        (80000000L)
#define BSP_BUS_CLOCK           (BSP_SYSTEM_CLOCK/2)
#define BASE_ADDRESS            0x44000000


#define PRDIV8_LIMIT_FREQ   12800000L /* limit freq. for using PRDIV8_PRSC*/
#define FLASH_FREQ          200000L   /* 200 kHz program frequency*/
#define PRDIV8_PRSC         8         /* prescaller 8*/

/*********************************** Constant **********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/
extern void Flash_Init();
extern unsigned char Flash_SectorErase(unsigned long *FlashPtr);
extern unsigned char Flash_ByteProgram(unsigned long *FlashStartAdd,unsigned long *DataSrcPtr,unsigned long NumberOfBytes);
void SpSub(void);
void SpSubEnd(void);
extern void * (memcpy) (void * dst, const void * src, size_t len);
#endif /* _FLASH_CFV2_H_ */
/* EOF */