/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Os_Cbk.c
 * Date      : Sep 4 2019
 * Version   : 1.0
 * Description  : This module implements additional callback functions for RTA-OS.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Os.h"

#include "rba_BswSrv.h"
#include "Rtm.h"


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/**********************************************************************************
  Function name     : Os_Cbk_GetStopwatch
  Description       : Callback function from OS to return a timestamp in the system
                      Used for Time monitor when OS timing monitor enabled.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : tick of unsigned 32bit type
  Remarks           :
***********************************************************************************/
#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
#if defined (OS_TIME_MONITORING)
FUNC(Os_StopwatchTickType, OS_CALLOUT_CODE) Os_Cbk_GetStopwatch(void)
{
    Os_StopwatchTickType stopwatch;
    stopwatch = Mcu_Rb_GetSysTicks();
    return stopwatch;
}
#endif /* OS_TIME_MONITORING */
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_TaskStart
  Description         : This is get called at the point that a Task is about to start
                        execution to measure times of execution tasks.
  Parameter (in)      : task - current task to be executed
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_TaskStart() implements if OS option 'Additional 
                        Task Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_TaskStart(TaskType task)
{
# if (RTM_NUM_CORES > 1)
    /**
     * @brief Workaround for Multicore Rtm Integration for current Rtm who 
     * (1) use core local timer/counter for stopwatch timestamp <and>
     * (2) records task activation timestamp at the calling Master core <and>
     * (3) records the Task end timestamp at core local
     */
    task->dynamic->last_activation = Os_Cbk_GetStopwatch();
#endif
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE                   
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_TaskEnd
  Description         : This is get called at the point that a Task finishes execution
                        to measure execution time of tasks.
  Parameter (in)      : task - current task which to be finishing
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_TaskEnd() implements if OS option 'Additional 
                        Task Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_TaskEnd(TaskType task) 
{
    Rtm_TaskEnd(task);
}
#define OS_STOP_SEC_CALLOUT_CODE                   
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_ISRStart
  Description         : This is get called at the point that a CAT2 ISR is about
                        to start execution.
  Parameter (in)      : isr - current ISR to be executed.
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_ISRStart() implements if OS option 'Additional 
                        ISR Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_ISRStart(ISRType isr)
{

}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_ISREnd
  Description         : This is get called at the point that a CAT2 ISR is about
                        to end execution.
  Parameter (in)      : isr - current ISR to be ended.
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_ISREnd() implements if OS option 'Additional 
                      ISR Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_ISREnd(ISRType isr)
{
    Rtm_IsrEnd(isr);
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_CrosscoreISRStart
  Description         : This is get called at the point that a cross-core ISR is about
                        to start execution.
  Parameter (in)      : isr - current ISR to be started.
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_CrosscoreISRStart() implements if OS option 'Additional 
                        ISR Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_CrosscoreISRStart(CoreIdType core_id)
{
    
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name       : Os_Cbk_CrosscoreISREnd
  Description         : This is get called at the point that a cross-core ISR is about
                        to end execution.
  Parameter (in)      : isr - current ISR to be ended.
  Parameter (inout)   : None
  Parameter (out)     : None
  Return value        : None
  Remarks             : Os_Cbk_CrosscoreISREnd() implements if OS option 'Additional 
                        ISR Hooks' is enabled.
  Remarks             :
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_CrosscoreISREnd(CoreIdType core_id)
{
    
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
