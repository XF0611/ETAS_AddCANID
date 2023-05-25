/*********************************************************************************************************************
*
* Description:     Sleep function implementation.
* FileName:        sleep.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2020/11/08     modified
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "Sleep.h"
#include "Os_TimeServices.h"
#include "Dcm.h"
#include "EcuMgt.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
/*
 * Timeout in milliseconds before triggering the ECU Reset
 */
#define SLEEP_THRESHOLD_LIMIT_TIME	100U
#define SLEEP_TIMEOUT_MS            20000U
#define SLEEP_TIMEOUT_COUNT_TIMES   15
/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/
typedef enum
{
    SLEEP_TIMER_STOPPED = 0,
    SLEEP_TIMER_STARTED = 1
} Sleep_OperationType;
/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
/*
 * A 32 bit timer for monitoring sleep timeout
 */
static uint32 Sleep_Timer = 0U;
/*
 * Status of the timer (Started/Stopped)
 */
static Sleep_OperationType Sleep_TimerStatus = SLEEP_TIMER_STOPPED;
/*
 * Flag for setting the sleep timer status
 */
static uint8 Sleep_SleepState = FALSE;

static uint32 Sleep_TimerTimeoutCount = 0U;
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/*
 * Periodic function called from OsTask every 20ms
 */
void Sleep_MonitorMainFunction(void)                                                        /* PRQA S 1503 */
{
    /*
     * Threshold limit timer for triggering ECU Reset
     */
    static uint32 Sleep_TimerThreshold = 0U;
    /* Check if Sleep Timer is running */
    if (Sleep_TimerStatus == SLEEP_TIMER_STARTED)
    {
        /* Check if Sleep Timer has timed-out */
        if (TRUE == OS_Timer32_IsElapsed(Sleep_Timer, SLEEP_TIMEOUT_MS))
        {
            Sleep_TimerTimeoutCount++;

            if(Sleep_TimerTimeoutCount < (uint32)SLEEP_TIMEOUT_COUNT_TIMES)
            {
                
                /*Restart Timer*/
                (void)OS_Timer32_Start(&Sleep_Timer);
            }
            else
            {
                /* Sleep Timer timeout. Set tranceiver to sleep*/
                //TODO:

                if (Sleep_SleepState == FALSE)
                {
                    (void)OS_Timer32_Start(&Sleep_TimerThreshold);
                }

                if (TRUE == OS_Timer32_IsElapsed(Sleep_TimerThreshold, SLEEP_THRESHOLD_LIMIT_TIME))
                {
                    /* check if tranceiver was set to sleep in previous cycle */
                    if (Sleep_SleepState == TRUE)
                    {
                        /* This is executed because Tranceiver didn't sleep because of invalid network activity
                        * So reset the controller */
                        EcuMgt_PerformReset();
                    }
                }

                /* set flag so that in next iteration, EcuReset can happen in the next cycle */
                Sleep_SleepState = TRUE;
            }
        }
    }
}


/*
 * Function to start or stop the sleep timer.
 * To be called from
 * 		1. During initialization
 * 		2. From DCM service callbacks
 */
void Sleep_StartStopTimer(void)                                                     /* PRQA S 1503 */
{
    Dcm_SesCtrlType currSession;

    /* Get the Current Diagnostic Session from DCM */
    (void)Dcm_GetSesCtrlType(&currSession);

    /* When in programming session, Sleep Timer should be stopped */
    if (currSession == DCM_PROGRAMMING_SESSION)
    {
        /* Use the timer service from Os to stop the timer */
        (void)OS_Timer32_Stop(&Sleep_Timer);

        /* Set a flag for monitoring */
        Sleep_TimerStatus = SLEEP_TIMER_STOPPED;
    }
    /* When in default session, Sleep Timer should be reinitialized and started */
    else if ((currSession == DCM_DEFAULT_SESSION) || (DCM_EXTENDED_DIAGNOSTIC_SESSION == currSession))
    {
        /* Use the timer service from Os to stop the timer */
        (void)OS_Timer32_Start(&Sleep_Timer);

        Sleep_TimerTimeoutCount = 0;
        /* Set a flag for monitoring */
        Sleep_TimerStatus = SLEEP_TIMER_STARTED;

        /* Clear the Sleep_SleepState flag to avoid any lockdown */
        Sleep_SleepState = FALSE;
    }
    else
    {
        /* Should not reach here as there are only two sessions in PBL */
    }
}
