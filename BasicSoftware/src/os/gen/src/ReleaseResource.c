/*
 * This is ReleaseResource.c, auto-generated for:
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
FUNC(StatusType, OS_CODE) Os_ReleaseResource(ResourceType ResID) {
  StatusType api_retval = E_OK;
  boolean want_dispatch = FALSE;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 207] [$UKS 206] [$UKS 169] */ /* ETAS BIP Porting */
  
  /* Error if the resource is not valid */
  if (!(Os_IsResourceValid(ResID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 208] */
  if ((0U == (ResID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 841] */
  /* Error if the resource is not occupied */
  if (0U == ResID->dynamic->access_count) { api_retval = E_OS_NOFUNC;  goto api_error_exit; } /* [$UKS 839] */

    /* Task-only resource */
    /* Error if ISR tries to release the resource */
    if (INVALID_ISR != Os_RunningISR) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 840] */

    if (INVALID_TASK != Os_RunningTask) {
      /* Error if tpmask is below Os_RunningTask base_tpmask  [$UKS 840] */
      /* [MISRA 2012 Rule 16.1] */ /*lint -save  -e9042 */
      if ((INVALID_TASK != Os_RunningTask) && (ResID->tpmask.t0 < Os_RunningTask->base_tpmask.t0)) { api_retval = E_OS_ACCESS;  goto api_error_exit; }
      /*lint -restore */
    }
    if (INVALID_TASK != Os_RunningTask) {
      Os_RunningTask->dynamic->resource_count--;
    }
    Os_RunningTPMask = ResID->dynamic->saved_priority.tpmask; /* Change the task priority. [$UKS 187] */
  ResID->dynamic->access_count = 0U; /* Clear the access count */
  
  
  if ((0U == previous_imask)) {
    want_dispatch = (Os_ReadyTasks.p0 > Os_RunningTPMask.t0) /*lint !e931 PC-lint thinks there are side effects here */;
    if (want_dispatch) {
      Os_Dispatch();
    }
  }
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_ReleaseResource, (Os_BiggestType)ResID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* ReleaseResource */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


