/********************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
**************************************************************************//*!
 *
 * @file flash_cfv2.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief The file includes flash routines
 *
 *****************************************************************************/

/*********************************** Includes ***********************************/
#include "hidef.h" /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "flash_cfv2.h" /* include flash driver header file */
#include "string.h"

/*********************************** Macros ************************************/

/*********************************** Defines ***********************************/

/*********************************** Constant **********************************/

/*********************************** Variables *********************************/

/*********************************** Prototype *********************************/

/*********************************** Function **********************************/
/*******************************************************************************
 * Function:        Flash_Init
 *
 * Description:     Set the flash clock
 *
 * Returns:         never return
 *
 * Notes:
 *
 *******************************************************************************/
void Flash_Init()
{
    /* Body */
    if (BSP_BUS_CLOCK > PRDIV8_LIMIT_FREQ)
    {
        MCF_CFM_CFMCLKD = MCF_CFM_CFMCLKD_PRDIV8 | MCF_CFM_CFMCLKD_DIV((BSP_BUS_CLOCK+PRDIV8_PRSC*FLASH_FREQ-1) / (PRDIV8_PRSC * FLASH_FREQ) -1);
    }
    else
    {
        MCF_CFM_CFMCLKD = MCF_CFM_CFMCLKD_DIV((BSP_BUS_CLOCK+FLASH_FREQ-1) / FLASH_FREQ -1);
    } /* EndIf */
} /* EndBody */

/*******************************************************************************
 * Function:        Flash_SectorErase
 *
 * Description:     erase a sector of the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char Flash_SectorErase(unsigned long *FlashPtr)
{
    /* Body */
    unsigned char Return = Flash_OK;
    unsigned long *FlashStartAdd_virtual;
    /* Allocate space on stack to run flash command out of SRAM */
    char localbuf[100];
    int (*RunOnStack)(void) = (int(*)(void))localbuf;
    memcpy(localbuf, (void*)SpSub, (char*)SpSubEnd - (char*)SpSub);
    FlashStartAdd_virtual =  FlashPtr +  (BASE_ADDRESS/4);
    if (MCF_CFM_CFMCLKD&MCF_CFM_CFMCLKD_DIVLD)
    {
        /* flash is init */
        /* wait until FCBEF is set in FSTAT */
        while ((MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_CBEIF) == 0) {}
        /* check for errors */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_ACCERR)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x10;
        } /* EndIf */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_PVIOL)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x20;
        } /* EndIf */
        /* dummy write to flash */
        *FlashStartAdd_virtual = (unsigned long)0x11111111;
        /* write command */
        MCF_CFM_CFMCMD = FlashCmd_SectorErase;
        RunOnStack();
        /* check for errors */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_ACCERR)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x10;
            /* update return value*/
            Return |= Flash_FACCERR;
        } /* EndIf */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_PVIOL)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x20;
            /* update return value*/
            Return |= Flash_FPVIOL;
        } /* EndIf */
    }
    else
    {
        /* flash is not init */
        Return = Flash_NOT_INIT;
    } /* EndIf */
    /* function return */
    return  Return;
} /* EndBody */

/*******************************************************************************
 * Function:        Flash_ByteProgram
 *
 * Description:     byte program the flash
 *
 * Returns:         Error Code
 *
 * Notes:
 *
 *******************************************************************************/
unsigned char Flash_ByteProgram
    (
        unsigned long *FlashStartAdd,
        unsigned long *DataSrcPtr,
        unsigned long NumberOfBytes
    )
{
    /* Body */
    unsigned long *FlashStartAdd_virtual;
    unsigned long size_buffer;
    unsigned char Return = Flash_OK;
    /* Allocate space on stack to run flash command out of SRAM */
    char localbuf[100];
    int (*RunOnStack)(void) = (int(*)(void))localbuf;
    memcpy(localbuf, (void*)SpSub, (uint_32)((char*)SpSubEnd - (char*)SpSub));
    FlashStartAdd_virtual =  FlashStartAdd +  (BASE_ADDRESS/4);
    
    
    if (NumberOfBytes == 0)
    {
        return Flash_CONTENTERR;
    }
    else
    {
        size_buffer = (NumberOfBytes - 1)/4 + 1;
    } /* EndIf */

    if (MCF_CFM_CFMCLKD&MCF_CFM_CFMCLKD_DIVLD)
    {
        /* flash is init */
        /* wait until FCBEF is set in FSTAT */
        while ((MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_CBEIF) == 0) {}
        /* check for errors */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_ACCERR)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x10;
        } /* EndIf */
        if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_PVIOL)
        {
            /* clear error flag */
            MCF_CFM_CFMUSTAT = 0x20;
        } /* EndIf */
        while ((size_buffer) && (Return == Flash_OK))
        {
            /* write data to flash and increase pointers by 1 */
            *FlashStartAdd_virtual++ = *DataSrcPtr++;
            /* write command */
            MCF_CFM_CFMCMD = FlashCmd_Program;
            RunOnStack();
            /* check for errors */
            if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_ACCERR)
            {
                /* clear error flag */
                MCF_CFM_CFMUSTAT = 0x10;
                /* update return value*/
                Return |= Flash_FACCERR;
            } /* EndIf */
            if (MCF_CFM_CFMUSTAT&MCF_CFM_CFMUSTAT_PVIOL)
            {
                /* clear error flag */
                MCF_CFM_CFMUSTAT = 0x20;
                /* update return value*/
                Return |= Flash_FPVIOL;
            } /* EndIf */
            /* decrement byte count */
            size_buffer --;
        } /* EndWhile */
    }
    else
    {
        /* flash is not init */
        Return = Flash_NOT_INIT;
    } /* EndIf */
    /* function return */
    return  Return;
} /* EndBody */

/*******************************************************************************
 * Function:        SpSub
 *
 * Description:     Execute the Flash write while running out of SRAM
 *
 * Returns:
 *
 * Notes:
 *
 *******************************************************************************/
void SpSub(void)
{
    /* Body */
    MCF_CFM_CFMUSTAT |= MCF_CFM_CFMUSTAT_CBEIF;
    while (!(MCF_CFM_CFMUSTAT & MCF_CFM_CFMUSTAT_CCIF)) {};/*wait until execution complete*/
} /* EndBody */

/* Leave this immediately after SpSub */
void SpSubEnd(void) {}
/* EOF */