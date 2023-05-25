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
 * Component : Os_Hooks.c
 * Date      : April 7 2022
 * Version   : 2.0
 * Description  : This module implements basic necessary Hooks and callacks for RTA-OS, e.g. Startup and Shutdown hook.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Os.h"
#include "Target.h"
#include "Uart.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define ERR_LOG_MAX 16

typedef struct Os_ErrorLog
{
    uint32     errCount;
    StatusType errType;
    ISRType    isr;
    TaskType   task;
}OsTrace_ErrorHookLog_t;
OsTrace_ErrorHookLog_t      err_log[ERR_LOG_MAX];


/*
 **********************************************************************************************************************
 * External Declarations
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/
#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CALLOUT_CODE) ErrorHook(StatusType Error)
{
    ISRType ISRInError = 0;
    TaskType TaskInError = 0;
    StatusType Status_Type;
    OsTrace_ErrorHookLog_t* err;
    static uint32     err_cnt;
    /*
    * This code sample shows how to determine which Task or
    * ISR was running when the error occurred.
    */
    ISRInError = GetISRID();
    if (ISRInError != INVALID_ISR)
    {
    /* The error occurred when ISR 'ISRInError' was running */
    } 

    //ETAS BIP POrting TODO Status_Type = GetTaskID(&TaskInError);
    if (TaskInError != INVALID_TASK)
    {
    /* The error occurred when Task 'TaskInError' was running */
    } 
    else
    {
    /* The error occurred during StartOS, Os_Cbk_Idle or ShutdownOS */
    }
    err =  &err_log[err_cnt % (uint8)ERR_LOG_MAX];
    err->errCount = err_cnt;
    err->errType = Error;
    err->isr = ISRInError;
    err->task = TaskInError;
    err_cnt++;

    /*
     * This code sample shows how to determine which error
     * was detected.
     */
    switch (Error){
    case E_OS_ID:
    /* Handle illegal identifier error */
    break;
    case E_OS_VALUE:
    /* Handle illegal value error */
    break;
    case E_OS_STATE:
    /* Handle illegal state error */
    break;
    default:
    /* Handle all other types of error */
    break;
    }
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CALLOUT_CODE) PreTaskHook(void)
{

}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CALLOUT_CODE) PostTaskHook(void)
{

}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h" 


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name     : ProtectionHook
  Description       : Called for timing protection, memory protection and
                      unconfigured traps.
  Parameter (in)    : FatalError - the protection error reported by OS
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : Action to be taken by OS.
  Remarks           : This function contains hardware dependent implementations
                      for memory protection, shall be differs on different micros.
***********************************************************************************/
FUNC(ProtectionReturnType, OS_CALLOUT_CODE) ProtectionHook(StatusType FatalError)
{
    ISRType              ISRInError;
    TaskType             TaskInError;
    StatusType           StatusAPI;
    ProtectionReturnType action = PRO_SHUTDOWN;

    /* Determine which Task or ISR was running when the fault occurred. */
    ISRInError = GetISRID();
    StatusAPI  = GetTaskID(&TaskInError);

    switch (FatalError)
    {
        case E_OS_PROTECTION_MEMORY:
            /* [Hardware dependent code] - A memory protection error has been detected */
            action = PRO_SHUTDOWN;
            break;

        case E_OS_PROTECTION_TIME:
            /* Task exceeds its execution time budget OR A Category 2 ISR exceeds its execution time */
            action = PRO_SHUTDOWN;
            break;

        case E_OS_PROTECTION_ARRIVAL:
            /* Task/Category 2 arrives before its timeframe has expired */
            /* This is the only place where PRO_IGNORE is legal */
            action = PRO_SHUTDOWN;
            break;

        case E_OS_PROTECTION_LOCKED:
            /* Task/Category 2 ISR blocks for too long */
            action = PRO_SHUTDOWN;
            break;

        case E_OS_PROTECTION_EXCEPTION:
            /* Trap occurred:
             * All trap shall be configured explicitly with trap table.
             * And shall not use ProtectionHook to report Protection Exception.
             **/
            action = PRO_SHUTDOWN;
            break;

        case E_OS_STACKFAULT:
            /* Stack overrun for Task/ Category 2 ISR, when StackOverrunHook not provided. */
            /* ENV-116: for SC3 & SC4, handle E_OS_STACKFAULT from ProtectionHook explicitly following OS guide. */
            action = PRO_SHUTDOWN;
            break;

        default:
            /* Never here, default shutdown OS. */
            action = PRO_SHUTDOWN;
            break;
    }

    /* Actions defined by OS: 
     * - PRO_IGNORE
     * - PRO_TERMINATETASKISR
     * - PRO_TERMINATEAPPL
     * - PRO_TERMINATEAPPL_RESTART
     * - PRO_SHUTDOWN
     */
    return action;
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name     : StartupHook
  Description       : Called during OS starting.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks           : [$Satisfies $DD_OS_CALLBACK 018]
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) StartupHook(void) 
{
    /**
    * For a AUTOSAR multicore system, all cores will be synchronized at this point
    * before scheduling starts. 
    * */

    Rtm_Init();
#if (OS_NUM_CORES > 1)
    RTM_CORECHECKPOINT_CORE_OSSTART_SYNCH();
#else
    RTM_CORECHECKPOINT_CORE_OSSTART();
#endif

    /* ETAS BIP Porting */
    VectorTableCore0_Init();

    /**
     * @brief ETAS BIP Porting RTAXIP-293: 
     * Start stopwatch counter.
     */
    Target_TimerInit();

    /**
     * @brief ETAS BIP Porting RTAXIP-293: 
     * Call timer interrupt tick enable only after RTA-OS initialzied and MCAL initialized.
     */
    Target_EnableInterrupts();  /* Setup OS task tick counter and interrupt for schedule table created by Rte. */

    char testStr[40];
    sprintf(testStr, "Welcome to ETAS RTA-CAR world! %03d! \r\n", 666);
    Uart_Transmit(0, testStr, strlen(testStr));

}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name     : ShutdownHook
  Description       : Called during OS Shutdown by ShutdownOS.
  Parameter (in)    : Error - the error code passed which makes OS call ShutdownHook
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks           : 
***********************************************************************************/
FUNC(void, OS_CALLOUT_CODE) ShutdownHook(StatusType Error)
{
    /**
    * For a AUTOSAR multicore system, all cores will be synchronized at this point
    * before OS shutdown. 
    * */
    RTM_CORECHECKPOINT_CORE_OSSTOP();
    EcuM_Shutdown();
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#if defined (OS_SCALABILITY_CLASS_1) || defined (OS_SCALABILITY_CLASS_2)
/* ENV-116: for SC3 & SC4, handle E_OS_STACKFAULT from ProtectionHook explicitly following OS guide. */
#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_StackOverrunHook(Os_StackSizeType Overrun, Os_StackOverrunType Reason)
{
    while(1); /* debug here */
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
#endif /* (OS_SCALABILITY_CLASS_1) || defined ((OS_SCALABILITY_CLASS_2) */


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
/**********************************************************************************
  Function name     : Os_Cbk_Idle
  Description       : Called during OS has nothing to do.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : TRUE:  OS will call this callback every time when idle.
                      FALSE: OS will only call this callback once and will stay in
                             internal at further idle.
  Remarks           :
***********************************************************************************/
uint32 tyFlg = 0;
uint32 tyI = 0;
uint32 tyAddr = 0;
uint8 tyRead[4096]={0};
FUNC(boolean, OS_CALLOUT_CODE) Os_Cbk_Idle(void)
{
	TARGET_IDLE();

    if(tyFlg == 1)
    {
        /* page read with Flash direct address. */
        for(tyI = 0; tyI < 16; tyI++)
        {
            Fls_Read(tyAddr, &tyRead[tyI*256], 256);
            Fls_MainFunction();
            tyAddr += 0x100;
        }
        tyAddr = 0;
    }
    else if(tyFlg == 2)
    {
        //block erase
        for(tyI = 0; tyI < 16; tyI++)
        {
            Fls_Erase(tyAddr, 4096);
            Fls_MainFunction();
            tyAddr += 0x1000;
        }
        tyAddr = 0;
    }

    return TRUE;
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h"


#define OS_START_SEC_CALLOUT_CODE
#include "Os_MemMap.h"
FUNC(void, OS_CALLOUT_CODE) Os_Cbk_InShutdown(void)
{
    while(1); /* debug here */
    /*
        Os_Cbk_InShutdown() is called when the OS is in shutdown, after any shutdown hooks have run.
        It will be called repeatedly in an infinite loop, because ShutdownOS does not return.
        It can be called by each core that is in shutdown.
        A default implementation is supplied in the library that that does nothing.
        The default version is not provided if OS option "Omit Default Implementations" is TRUE.
        In most systems you will perform any shutdown actions in a shutdown hook before this is ever called.
    */
}
#define OS_STOP_SEC_CALLOUT_CODE
#include "Os_MemMap.h" 


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
