/*
 * Os_ISRWrapper
 *
 * Copyright ETAS GmbH 2022
 *
 * [$Satisfies $TgtCore 54]
 */
/*
 * This is Os_Wrapper.c, auto-generated for:
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
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/*lint --e{715} suppress possible "previous_imask not referenced" */
FUNC(void, OS_CODE_FAST) Os_ISRWrapper(Os_imaskType previous_imask, uint32 isr_index, uint32 channel) {
  ISRType saved_ISR;
  Os_MeterInfoRefType previous_meter;
  uint8 previous_access;
  P2CONST(Os_ApplicationConfigurationType, TYPEDEF, OS_VAR) app;
  Os_StopwatchTickType now;
  Os_StackSizeType spsize;
  Os_StackValueType spnow;
  Os_StackValueType sp_pre_isr;
  static const Os_StackValueType isr_overhead = 0U; /*lint !e960 !e435 !e712 !e569 Arith overflow is intended */
  Os_StackSizeType spoverrun;


  ApplicationType previous_appoverride;
  (void)Os_SetImask(1U);  /* ETAS BIP Porting */
  now = Os_Cbk_GetStopwatch();
  previous_appoverride = Os_AppOverride; Os_AppOverride = INVALID_OSAPPLICATION; /* [$EHI 559562] */
  spnow = Os_GetSP();
  previous_meter = Os_CurrentMeteredObject;
  previous_access = Os_AppAccess;
  saved_ISR = Os_RunningISR;
  Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->previous = now;
  sp_pre_isr = spnow - isr_overhead; /*lint !e845 Can be subtracting zero on some configurations */
  spsize = Os_CurrentMeteredObject->stackbase - sp_pre_isr;
  if (spsize > Os_CurrentMeteredObject->stackusage) { Os_CurrentMeteredObject->stackusage = spsize; }
  if (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackmax) { Os_CurrentMeteredObject->stackmax = Os_CurrentMeteredObject->stackusage; }
  if ((spoverrun = Os_CurrentMeteredObject->stackusage - Os_CurrentMeteredObject->stackbudget), (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackbudget)) { /* [MISRA 2012 Rule 12.3] */ /*lint !e9008 !e9084 */
    
    Os_Cbk_StackOverrunHook(spoverrun, OS_BUDGET); /* [$UKS 481] [$UKS 1232] [$UKS 1233] [$UKS 1904] */
    Os_CurrentMeteredObject->stackbudget = 0xffffffffU;
  }
  Os_RunningISR = &Os_const_isrs[isr_index];
  app = &Os_const_applications[Os_RunningISR->application];
  Os_AppAccess = app->access;
  Os_CurrentMeteredObject = &(Os_RunningISR->dynamic->meter);
  Os_CurrentMeteredObject->elapsed = 0U;
  Os_CurrentMeteredObject->stackbase = spnow;
  Os_CurrentMeteredObject->stackusage = 0U;
  Os_CurrentMeteredObject->stackbudget = Os_RunningISR->stackbudget;
  Os_CurrentMeteredObject->previous = now;
    Os_Cbk_ISRStart(Os_RunningISR);
    (void)Os_SetImask(Os_RunningISR->imask);
    Os_RunningISR->entry_function();
  (void)Os_SetImask(1U);
  now = Os_Cbk_GetStopwatch();
  Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
  Os_CurrentMeteredObject->previous = now;
  if (Os_CurrentMeteredObject->elapsed > Os_CurrentMeteredObject->max) {
    Os_CurrentMeteredObject->max = Os_CurrentMeteredObject->elapsed;
  }
  if (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackmax) { Os_CurrentMeteredObject->stackmax = Os_CurrentMeteredObject->stackusage; }
  Os_Cbk_ISREnd(Os_RunningISR);
  //ETAS BIP Porting TODO /* Clear pending state */
  Os_RunningISR = saved_ISR; 
  {
  boolean want_dispatch;
  if (2U == previous_imask) {
    want_dispatch = (Os_ReadyTasks.p0 > Os_RunningTPMask.t0) /*lint !e931 PC-lint thinks there are side effects here */;
    if (want_dispatch) {  /* [$UKS 99] */
      Os_CurrentMeteredObject = &Os_IdleMeter;
      Os_CurrentMeteredObject->previous  = now;
      Os_Dispatch();  /* [$UKS 170] */
      now = Os_Cbk_GetStopwatch();
      Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
      Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
    } else {
      Os_CurrentMeteredObject = &Os_IdleMeter;
      Os_CurrentMeteredObject->previous  = now;
      now = Os_Cbk_GetStopwatch();
      Os_CurrentMeteredObject->elapsed += (now - Os_CurrentMeteredObject->previous);
      Os_CurrentMeteredObject->cumulative += (now - Os_CurrentMeteredObject->previous);
    }
  }
  }
  Os_CurrentMeteredObject = previous_meter;
  Os_AppAccess = previous_access;
  Os_CurrentMeteredObject->previous = now;
  Os_AppOverride = previous_appoverride; /* [$EHI 559562] */
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


