/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************
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
**************************************************************************//*!
*
* @file RealTimerCounter.c
*
* @author
*
* @version
*
* @date    
*
* @brief   This file configures Real Time Counter (RTC) for Timer 
*          Implementation
*****************************************************************************/

/******************************************************************************
* Includes
*****************************************************************************/
#include <string.h>
#include "derivative.h"     /* include peripheral declarations */
#include "types.h"          /* Contains User Defined Data Types */
#include "RealTimerCounter.h"
#if MAX_TIMER_OBJECTS

/*****************************************************************************
* Local Functions Prototypes
*****************************************************************************/
static uint_8 TimerInit(void);
static void EnableTimerInterrupt(void);
static void DisableTimerInterrupt(void);

/****************************************************************************
* Global Variables
****************************************************************************/
/* Array of Timer Objects */
TIMER_OBJECT g_TimerObjectArray[MAX_TIMER_OBJECTS];
/*****************************************************************************
* Global Functions Prototypes
*****************************************************************************/
uint_8 TimerQInitialize(uint_8 controller_ID);
uint_8 AddTimerQ(PTIMER_OBJECT pTimerObject);
uint_8 RemoveTimerQ(uint_8 index);

void interrupt VectorNumber_Vrtc Timer_ISR(void);

/*****************************************************************************
* Global Functions
*****************************************************************************/
/******************************************************************************
*
*   @name        TimerQInitialize
*
*   @brief       Initializes RTC, Timer Object Queue and System Clock Counter
*
    @param       controller_ID    : Controller ID
*
*   @return      None
*****************************************************************************
* This function initializes System Clock Counter, Timer Queue and Initializes
* System Timer
*****************************************************************************/
uint_8 TimerQInitialize(uint_8 controller_ID)
{
    UNUSED (controller_ID)
    (void)memset(g_TimerObjectArray, (int)NULL, sizeof(g_TimerObjectArray));
    return TimerInit();
}

/******************************************************************************
*
*   @name        AddTimerQ
*
*   @brief       Adds Timer Object to Timer Queue
*
    @param       pTimerObject  : Pointer to Timer Object
*
*   @return      None
*****************************************************************************
* Adds Timer Object to Timer Queue
*****************************************************************************/
uint_8 AddTimerQ(PTIMER_OBJECT pTimerObject)
{
    uint_8 index;
    if(pTimerObject == NULL)
    {
        return (uint_8)ERR_INVALID_PARAM;   
    } /* EndIf */
    if(pTimerObject->msCount == (unsigned int)INVALID_TIME_COUNT)
    {
    return (uint_8)ERR_INVALID_PARAM;
    } /* EndIf */
    for(index = 0; index < MAX_TIMER_OBJECTS; index++)
    {
        /* Disable Timer Interrupts */
        DisableTimerInterrupt();
        if(g_TimerObjectArray[index].pfnTimerCallback == NULL)
        {
            (void)memcpy(&g_TimerObjectArray[index], pTimerObject, sizeof(TIMER_OBJECT)); 
            /* Enable Timer Interrupts */
            EnableTimerInterrupt();
            break;
        } /* EndIf */
        /* Enable Timer Interrupts */
        EnableTimerInterrupt();
    } /* EndFor */
    if(index == MAX_TIMER_OBJECTS)
    {
        return (uint_8)ERR_TIMER_QUEUE_FULL;
    } /* EndIf */
    return index;
} /* EndBody */

/******************************************************************************
*
*   @name        RemoveTimerQ
*
*   @brief       Removes Timer Object from Timer Queue
*
    @param       index : Index of Timer Object
*
*   @return      None
*****************************************************************************
* Removes Timer Object from Timer Queue
*****************************************************************************/
uint_8 RemoveTimerQ(uint_8 index)
{
    /* Body */
    if(index >= MAX_TIMER_OBJECTS)
    {
    	return (uint_8)ERR_INVALID_PARAM;
    } /* EndIf */
    /* Disable Timer Interrupts */
    DisableTimerInterrupt();
    (void)memset(&g_TimerObjectArray[index], (int)NULL, sizeof(TIMER_OBJECT));
    /* Enable Timer Interrupts */
    EnableTimerInterrupt();
    return (uint_8)ERR_SUCCESS;
} /* EndBody */

/******************************************************************************
*   @name        TimerInit
*
*   @brief       This is RTC initialization function
*
*   @return      None
*
******************************************************************************
* Intiializes the RTC module registers
*****************************************************************************/
static uint_8 TimerInit(void)
{
    /* Body */
    RTCSC_RTCLKS = 0x02; /* Select Internal Clock Source */
    RTCSC_RTIF = 0x01; /* Clear previous RTC Interrupt */
    EnableTimerInterrupt();
    RTCMOD = 0x20;  /* 1 ms Interrupt Generation */
    /* Start RTC by Reseting Counter to 0 */ 
    RTCSC_RTCPS = 0x08; /* Prescaler = 1 Clock = IRCLK */
    return ERR_SUCCESS;
} /* EndBody */

/******************************************************************************
*   @name        EnableTimerInterrupt
*
*   @brief       This routine enables Timer Interrupt
*
*   @return      None
*
******************************************************************************
* Enables RTC Timer Interrupt
*****************************************************************************/
static void EnableTimerInterrupt(void)
{
    /* Body */
    /* Enable Timer Interrupt */
    RTCSC_RTIE = 0x01;
    return;
} /* EndBody */

/******************************************************************************
*   @name        DisableTimerInterrupt
*
*   @brief       This routine disables Timer Interrupt
*
*   @return      None
*
******************************************************************************
* Disables RTC Timer Interrupt
*****************************************************************************/
static void DisableTimerInterrupt(void)
{
    /* Body */
    /* Disable Timer Interrupt */
    RTCSC_RTIE = 0x00;
    return;
} /* EndBody */

/******************************************************************************
*   @name        Timer_ISR
*
*   @brief       This routine services RTC Interrupt
*
*    @param       None
*
*   @return      None
*
******************************************************************************
* Services RTC Interrupt. If a Timer Object expires, then removes the object 
* from Timer Queue and Calls the callback function (if registered)
*****************************************************************************/
void interrupt VectorNumber_Vrtc Timer_ISR(void)
{
    /* Body */
    uint_8 index;
    if(RTCSC & RTCSC_RTIF_MASK)
    {
        /* Clear RTC Interrupt */
        RTCSC_RTIF = 1; /* Write 1 to Clear */

        /* Call Pending Timer CallBacks */
        for (index = 0; index < MAX_TIMER_OBJECTS; index++)
        {
            PTIMER_OBJECT ptemp = &g_TimerObjectArray[index];
            if(ptemp->pfnTimerCallback == NULL)
            {
                continue;
            } /* EndIf */
            ptemp->msCount--;
            if (ptemp->msCount < 0) 
            {
                PFNTIMER_CALLBACK pfnTimerCallback = ptemp->pfnTimerCallback;
#ifdef TIMER_CALLBACK_ARG
                void *parg = ptemp->arg;
#endif
                (void)RemoveTimerQ(index);
#ifdef TIMER_CALLBACK_ARG
                pfnTimerCallback(parg);
#else
                pfnTimerCallback();
#endif
            } /* EndIf */
        } /* EndFor */
    } /* EndIf */
} /* EndBody */
#endif

/* EOF */
