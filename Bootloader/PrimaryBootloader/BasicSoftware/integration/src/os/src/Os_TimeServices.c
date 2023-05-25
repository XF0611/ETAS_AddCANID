/*********************************************************************************************************************
*
* Description:     Os_TimeServices module implementation
* FileName:        Os_TimeServices.c
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
* FIF5MI        22/02/2018      First Version.
**********************************************************************************************************************/

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Os.h"
#include "Os_TimeServices.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/


/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Function Name:     OS_Timer32_Start
* Return Value:      None
* Parameters List:   uint32 *const pu32Timer
* Description:       Start 32-bit virtual timer.
*********************************************************************************************************************/
void OS_Timer32_Start (uint32 *const pu32Timer)     /* PRQA S 1503 */
{
    TickType startTimerValue;
    if(NULL_PTR == pu32Timer)
    {
        /*Nothing todo*/
    }
    else
    {
        GetCounterValue(Millisecond_TickCounter, &startTimerValue);          /* PRQA S 3200 */
        *pu32Timer = startTimerValue;
        if(0u == *pu32Timer)
        {
            *pu32Timer = ~(uint32)0u;
        }
    } 
}

/*********************************************************************************************************************
* Function Name:     OS_Timer32_Stop
* Return Value:      None
* Parameters List:   uint32 *const pu32Timer
* Description:       Stop 32-bit virtual timer.
*********************************************************************************************************************/
void OS_Timer32_Stop (uint32 *const pu32Timer)      /* PRQA S 1503 */
{
    if(NULL_PTR == pu32Timer)
    {
        /*Nothing todo*/
    }
    else
    {
        *pu32Timer = 0;
    }
}

/*********************************************************************************************************************
* Function Name:     OS_Timer32_IsStarted
* Return Value:      boolean --> FALSE: not started or elapsed.
*                            --> TRUE: started.
* Parameters List:   None
* Description:       Check if 32-bit virtual timer is started.
*********************************************************************************************************************/
boolean OS_Timer32_IsStarted (const uint32 u32Timer)        /* PRQA S 1503 */
{
    boolean isTimerStarted;
    if(0u == u32Timer)
    {
        isTimerStarted = FALSE;
    }
    else
    {
        isTimerStarted = TRUE;
    }
    return isTimerStarted;
}

/*********************************************************************************************************************
* Function Name:     OS_Timer32_IsElapsed
* Return Value:      boolean --> FALSE: not elapsed.
*                            --> TRUE: elapsed.
* Parameters List:   const uint32 u32Timer
*                    const uint32 u32Timeout
* Description:       Check if 32-bit virtual timer is elapsed,
*********************************************************************************************************************/
boolean OS_Timer32_IsElapsed (const uint32 u32Timer, const uint32 u32Timeout)       /* PRQA S 1503 */
{
    TickType elapsedTimerValue;
    boolean isTimerElapsed;
    TickType u32TimerVal = u32Timer;
    GetElapsedCounterValue(Millisecond_TickCounter, &u32TimerVal, &elapsedTimerValue);           /* PRQA S 3200 */

    if(OS_TICKS2MS_Millisecond_TickCounter(elapsedTimerValue) >= u32Timeout)
    {
        isTimerElapsed = TRUE;
    }
    else
    {
        isTimerElapsed = FALSE;
    } 
    return isTimerElapsed;
}


/**
 * @brief
 * @return None
 */
void OsIntegration_DisableAllInterrupt(void)
{
    /*Disable Compare */
	//STM0_ICR.B.CMP0EN = 0U;
    /*Disable all interrupts*/
    //Os_StopScheduleTable(NewScheduleTable1);
    //Os_SuspendAllInterrupts();
    Os_SuspendOSInterrupts();
}

/**
 * @brief
 * @return None
 */
void OsIntegration_EnableAllInterrupt(void)
{

    /*Enable All interrupts*/
    //Os_StartScheduleTableSynchron(NewScheduleTable1);
    //STM0_ICR.B.CMP0EN = 1U;
//    Os_ResumeAllInterrupts();
    Os_ResumeOSInterrupts();
}
