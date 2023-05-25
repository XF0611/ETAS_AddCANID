/*
 * This is ActivateTask.c, auto-generated for:
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
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_ActivateTask(TaskType TaskID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;

  OS_ISB(); /* ETAS BIP Porting */
  OS_SWI_KERNEL(); /* ETAS BIP Porting */
  previous_imask = Os_SetImask(1U); /* [$UKS 61] [$UKS 790] [$UKS 63] [$UKS 51] [$UKS 166] [$UKS 1530] [$UKS 1531] */  /* ETAS BIP Porting */
  
  if (!(Os_IsTaskValid(TaskID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 792] Error if the task is not valid */
  /* [$UKS 793] [$UKS 1528] */
  if ((0U == (TaskID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; }
  /* [MISRA 2012 Rule 16.1] */ /*lint -save  -e9042 -e9008 */
      if (TaskID->dynamic->activation_count == TaskID->activation_count) { api_retval = E_OS_LIMIT;  goto api_error_exit; } /* [$UKS 791] [$UKS 64] */
      /*lint -restore */
  /* [$UKS 65] */
  TaskID->dynamic->seen_activation = TRUE;
  TaskID->dynamic->last_activation = Os_Cbk_GetStopwatch();
  TaskID->dynamic->activation_count++;
  Os_ReadyTasks.p0 |= TaskID->pset.p0; /*lint !e931 PC-lint thinks there are side effects here */ /* [$UKS 62] */
  {
  boolean want_dispatch = (Os_ReadyTasks.p0 > Os_RunningTPMask.t0) /*lint !e931 PC-lint thinks there are side effects here */;
  if ((2U /* ETAS BIP Porting */ == previous_imask) && want_dispatch) {
    Os_Dispatch(); 
  }
  }
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_ActivateTask, (Os_BiggestType)TaskID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  OS_ISB(); /* ETAS BIP Porting */
  OS_SWI_USER(); /* ETAS BIP Porting */
  return api_retval;
} /* ActivateTask */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


