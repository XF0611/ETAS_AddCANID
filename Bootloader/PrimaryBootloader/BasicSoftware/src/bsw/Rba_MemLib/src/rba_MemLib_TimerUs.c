

#include "rba_MemLib.h"
#include "rba_MemLib_Cfg.h"


#if !defined(RBA_MEMLIB_USE_TIMER)
#error "Please define macro RBA_MEMLIB_USE_TIMER"
#endif
#if !defined(RBA_MEMLIB_USE_TIMER_CHECK)
#error "Please define macro RBA_MEMLIB_USE_TIMER_CHECK"
#endif
#if !defined(RBA_MEMLIB_BYPASS_TIMER_US)
#error "Please define macro RBA_MEMLIB_BYPASS_TIMER_US"
#endif

/*
 **********************************************************************************************************************
 * includes (macro dependent)
 **********************************************************************************************************************
*/

#if defined(RBA_MEMLIB_VECU_ENABLE) && (RBA_MEMLIB_VECU_ENABLE == STD_ON)
#define RBA_MEMLIB_TIMSIM   STD_ON
#else
#define RBA_MEMLIB_TIMSIM   STD_OFF
#endif



#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US == STD_OFF) && (RBA_MEMLIB_TIMSIM == STD_OFF)
#include "rba_MemLib_Cfg_SchM.h"
#endif




/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#if (RBA_MEMLIB_USE_TIMER != STD_OFF)

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

uint32 rba_MemLib_tiTimerUs;

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

#endif

/****/


#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && defined(RBA_MEMLIB_VECU_ENABLE) && (RBA_MEMLIB_VECU_ENABLE == STD_ON)

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

uint32 rba_MemLib_tiTimerUs_Incr;            /* Timer increment with each update step */

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

#endif

/****/

#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US == STD_OFF) && (RBA_MEMLIB_TIMSIM == STD_OFF)

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

static uint32 rba_MemLib_tiTimerTicks;

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

/****/

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_8
#include "rba_MemLib_MemMap.h"

static boolean rba_MemLib_flgInit;

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_8
#include "rba_MemLib_MemMap.h"

#endif



/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"




#if (RBA_MEMLIB_TIMSIM == STD_ON)
/*
 **********************************************************************************************************************
 * Timer-Simulation/VECU enabled part
 **********************************************************************************************************************
*/

/** ******************************************************************************************
 * \brief   Update the internal us-timer
 *          This function needs to be called periodically to update the internbal timer
 *          Here it is a mere simulation for Vecu
*/
void    rba_MemLib_UpdateTimerUs(void)
{
#if (RBA_MEMLIB_USE_TIMER != STD_OFF)

    /* Set the default value for the increment if it is 0 */
    if (rba_MemLib_tiTimerUs_Incr == 0uL)
    {
        rba_MemLib_tiTimerUs_Incr = 10uL;
    }

    /* Increment the free-running timer (1us resolution */
    rba_MemLib_tiTimerUs += rba_MemLib_tiTimerUs_Incr;
#endif

    return;
}


#else
/*
 **********************************************************************************************************************
 * Timer-Simulation/VECU disabled part  --> real device
 **********************************************************************************************************************
*/


/** ******************************************************************************************
 * \brief   Update the internal us-timer
 *          This function needs to be called periodically to update the internbal timer
 *          The update interval must be smaller than the timer-interval of the configured
 *          system timer
*/
void    rba_MemLib_UpdateTimerUs(void)
{
    /* Standard calculation of TimerUs if NO Bypass is present */
#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US == STD_OFF)

    uint32 tiTicksNew;
    uint32 tiTicksOld;
    uint32 tiTicksDiff;
    uint32 tiUsDiff;

    /* Enter the critical section */
    /* Within this critical section the data consistency needs to be ensured between
     * the internal variables and the captured timer
     */
    SchM_Enter_rba_MemLib_TimerUs();

    /* Capture the new timer-stamp */
    tiTicksNew = rba_MemLib_GetTimerTicks();

    /* Old time stamp */
    tiTicksOld = rba_MemLib_tiTimerTicks;

    if (rba_MemLib_flgInit != FALSE)
    {
        /* Determine the time-difference between this and the last call (in ticks) */
        tiTicksDiff = rba_MemLib_CalcDiffInTimerTicks(tiTicksNew,tiTicksOld);

        /* Convert difference to us and back to timer ticks to minimize truncating effects */
        /* Note: timer ticks have higher resolution than us, so the values are
         * truncated. Then the related reference timer is also not incremented
         * so much. The missed values are then considered within the next update.
         */
        tiUsDiff    = rba_MemLib_ConvertTimerTicksToUs(tiTicksDiff);
        tiTicksDiff = rba_MemLib_ConvertUsToTimerTicks(tiUsDiff);

        /* Update the reference (wrap around is intended!) */
        tiTicksOld  += tiTicksDiff;
        rba_MemLib_tiTimerTicks = tiTicksOld;

        /* Update the resulting timer (wrap around is intended!) */
        rba_MemLib_tiTimerUs  += tiUsDiff;
    }
    else
    {
        /* Perform an online verification of the timer functionality.
         * Only if this is passed then the initialization is finished.
         * Note: Critical section is prolonged in a similar way as during operation -> OK
         */

        /* Initialize timer */
        rba_MemLib_tiTimerTicks = tiTicksNew;

#if RBA_MEMLIB_USE_TIMER_CHECK != STD_OFF
        {
            uint32 ti_u32;
            boolean flgErr=FALSE;

            /* Exit if resolution of timer is NOT sufficient (normally: 1us has to correspond to >=1 tick) */
            /* Exit if resolution of timer is too high (unrealistic = causing calculation issues) */
            ti_u32 = rba_MemLib_ConvertUsToTimerTicks(1uL);
            if ( (ti_u32< 1uL) || ( ti_u32 > 4000uL) )
            {
                flgErr = TRUE;
            }

            /* Exit if value range is not sufficient (normally: 1000000us should not cause a calculation problem) */
            /* Exit if AND mask is not sufficient */
            if ((rba_MemLib_ConvertUsToTimerTicks(1000000uL) & RBA_MEMLIB_TIMERBITMASK )< (ti_u32*1000000uL))
            {
                flgErr = TRUE;
            }

            /* Check whether timer is running AND no error -> leave INIT */
            if ((!flgErr) && (tiTicksOld > 0uL))
            {
                /* Is the timer actually changed, then it is assumed that it is running */
                if (tiTicksNew != tiTicksOld)
                {
                    /* register the completed init */
                    rba_MemLib_flgInit = TRUE;
                }
            }
        }
#else
        /* Check whether timer is running -> leave INIT */
        if (tiTicksOld > 0uL)
        {
            /* Is the timer actually changed, then it is assumed that it is running */
             if (tiTicksNew != tiTicksOld)
             {
                 /* register the completed init */
                 rba_MemLib_flgInit = TRUE;
             }
       }
#endif
    }

    /* Leave the critical section */
    SchM_Exit_rba_MemLib_TimerUs();

    /* TimerUs Bypass handling */
#elif (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US != STD_OFF)

    /* Determine Timer value from provided Bypass function */
    rba_MemLib_tiTimerUs  = rba_MemLib_Callout_BypassTimerUs();

#endif
    return;
}


#endif

#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"


