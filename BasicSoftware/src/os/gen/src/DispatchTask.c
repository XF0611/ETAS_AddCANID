/*
 * This is DispatchTask.c, auto-generated for:
 *   Project: ETAS_EcuC_Partitions_AppCore_EcucValues
 *   Target:  TDA4VM-R5-TI
 *   Variant: TDA4VM_MCU_R5FSS0_CORE0
 *   Version: 5.0.3
 * **************************
 *  WARNING
 *   This source code has been generated for debugging and quality assurance
 *   purposes only. It is recommended that all RTA-OS code used in a series
 *   production ECU is generated and compiled using the RTA-OS Generator tool
 *   to ensure consistency of code and compilation options.
 * **************************
 */
#include "OsMinusConfig.h"
#include "IKernel.h"
/* Code start */



/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

FUNC(void, OS_CODE) Os_Dispatch(void) {
  P2CONST(Os_ApplicationConfigurationType, TYPEDEF, OS_VAR) app;
  Os_StopwatchTickType now = Os_Cbk_GetStopwatch();
  Os_MeterInfoRefType previous_meter = Os_CurrentMeteredObject;
  const Os_tpmaskType previous_priority = Os_RunningTPMask; /* save active priority */ /*lint !e9018 A union type */
  const TaskType previous_task = Os_RunningTask; /* save current task */
  ApplicationType previous_appoverride;
  uint8 previous_access = Os_AppAccess;
  Os_StackSizeType spoverrun;
  Os_StackSizeType spsize;
  Os_StackValueType spnow;
  previous_appoverride = Os_AppOverride; Os_AppOverride = INVALID_OSAPPLICATION; /* [$EHI 559562] */

  Os_pset0Type readyPsets; /* ETAS BIP Porting */
  Os_pset0Type leadingZeros = 0U; /* ETAS BIP Porting */

  spnow = Os_GetSP();
  
  Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->previous = now;
  spsize = Os_CurrentMeteredObject->stackbase -  spnow; if (spsize > Os_CurrentMeteredObject->stackusage) { Os_CurrentMeteredObject->stackusage = spsize; } /* [$UKS 1244] [$UKS 1249] */
  if ((spoverrun = Os_CurrentMeteredObject->stackusage - Os_CurrentMeteredObject->stackbudget), (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackbudget)) { /* [MISRA 2012 Rule 12.3] */ /*lint !e9008 !e9084 */
    
    Os_Cbk_StackOverrunHook(spoverrun, OS_BUDGET); /* [$UKS 481] [$UKS 1236] [$UKS 1237] [$UKS 1232] [$UKS 1233] [$UKS 1904] */
    Os_CurrentMeteredObject->stackbudget = 0xffffffffU;
  }
  if (INVALID_TASK != Os_RunningTask) {PostTaskHook();}  /* [$UKS 177] */
  do {

    /* ETAS BIP Porting */
    readyPsets = Os_ReadyTasks.p0;  
    leadingZeros = 0U;
    while((readyPsets & 0x80000000U) != 0x80000000U)
    {
      leadingZeros++;
      readyPsets = readyPsets << 1;
    }

    Os_RunningTask = (&Os_const_tasks0[(31U - leadingZeros /* ETAS BIP Porting OS_CLZ(Os_ReadyTasks.p0) */)]); /*lint !e931 !e9034 !e747 */
    
    
    Os_RunningTask->dynamic->resource_count = 0U;
    Os_RunningTPMask = Os_RunningTask->tpmask; /* [$UKS 199] [$UKS 201] */
    app = &Os_const_applications[Os_RunningTask->application];
    Os_AppAccess = app->access;
    Os_CurrentMeteredObject = &(Os_RunningTask->dynamic->meter);
    Os_CurrentMeteredObject->elapsed = 0U;
    Os_CurrentMeteredObject->previous = now;
    Os_CurrentMeteredObject->stackbase = spnow;
    Os_CurrentMeteredObject->stackusage = 0U;
    Os_CurrentMeteredObject->stackbudget = Os_RunningTask->stackbudget;
    PreTaskHook();   /* [$UKS 178] */
      
    Os_Cbk_TaskStart(Os_RunningTask); /* [$UKS 1802] [$UKS 1803] [$UKS 1804] [$UKS 1805] */
    (void)Os_SetImask(2U);  /* ETAS BIP Porting */ /* [$UKS 93] Allow interrupts precedence */
    OS_ISB(); /* ETAS BIP Porting */
    OS_SWI_USER(); /* ETAS BIP Porting */
      Os_RunningTask->entry_function();
    OS_ISB(); /* ETAS BIP Porting */
    OS_SWI_KERNEL(); /* ETAS BIP Porting */
    (void)Os_SetImask(1U);    /* ETAS BIP Porting */
    now = Os_Cbk_GetStopwatch();
    Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
    Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
    Os_CurrentMeteredObject->previous = now;
    if (Os_CurrentMeteredObject->elapsed > Os_CurrentMeteredObject->max) {
      Os_CurrentMeteredObject->max = Os_CurrentMeteredObject->elapsed;
    }
    if (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackmax) { Os_CurrentMeteredObject->stackmax = Os_CurrentMeteredObject->stackusage; }
    Os_Cbk_TaskEnd(Os_RunningTask); /* [$UKS 1806] [$UKS 1807] [$UKS 1808] [$UKS 1809]*/
    
    {
      PostTaskHook();   /* [$UKS 177] */
    }

    
    
      
      Os_RunningTask->dynamic->activation_count--;
      if (Os_RunningTask->dynamic->activation_count == 0U) {
        Os_ReadyTasks.p0 &= ~(Os_RunningTask->pset.p0); /*lint !e931 PC-lint thinks there are side effects here */
      }
    if (INVALID_TASK != Os_ChainTaskRef) {
      Os_ChainTaskRef->dynamic->seen_activation = TRUE;
      Os_ChainTaskRef->dynamic->last_activation = Os_Cbk_GetStopwatch();
      Os_ChainTaskRef->dynamic->activation_count++;
      Os_ReadyTasks.p0 |= Os_ChainTaskRef->pset.p0; /*lint !e931 PC-lint thinks there are side effects here */ /* [$UKS 62] */
      Os_ChainTaskRef = INVALID_TASK;
    }
    
  } while ((Os_ReadyTasks.p0 > previous_priority.t0) /*lint !e931 PC-lint thinks there are side effects here */);
  Os_RunningTPMask = previous_priority;  /* [$UKS 77] [$UKS 78] */
  Os_AppOverride = previous_appoverride;
  Os_RunningTask = previous_task;
  Os_AppAccess = previous_access;
  
  Os_CurrentMeteredObject = previous_meter;
  Os_CurrentMeteredObject->previous = now;
  if (INVALID_TASK != Os_RunningTask) {PreTaskHook();}
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



