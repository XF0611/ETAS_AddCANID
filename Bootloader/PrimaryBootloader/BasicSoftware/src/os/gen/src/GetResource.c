/*
 * This is GetResource.c, auto-generated for:
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
FUNC(StatusType, OS_CODE) Os_GetResource(ResourceType ResID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 204] [$UKS 181]*/ /* ETAS BIP Porting */
  
  /* Error if the resource is not valid */
  if (!(Os_IsResourceValid(ResID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 198] */
  if ((0U == (ResID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 838] */
  /* Error if the resource is already in use */
  if (0U != ResID->dynamic->access_count) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 205] */

    /* Task-only resource */
    /* Error if ISR tries to lock the resource */
    if (INVALID_ISR != Os_RunningISR) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 205] */

    /* Error if tpmask is below Os_RunningTask base_tpmask [$UKS 205] */
    /* [MISRA 2012 Rule 16.1] */ /*lint -save  -e9042 */
      if ((INVALID_TASK != Os_RunningTask) && (ResID->tpmask.t0 < Os_RunningTask->base_tpmask.t0)) { api_retval = E_OS_ACCESS;  goto api_error_exit; }
      /*lint -restore */

    /* Up the access count */
    ResID->dynamic->access_count++;
    if (INVALID_TASK != Os_RunningTask) {
      Os_RunningTask->dynamic->resource_count++;
    }
    /* Save the current task priority mask */
    ResID->dynamic->saved_priority.tpmask = Os_RunningTPMask;

    /* Change the task priority iff higher. [$UKS 186] */
    if ((0U == previous_imask) && (ResID->tpmask.t0 > Os_RunningTPMask.t0)) {
       Os_RunningTPMask = ResID->tpmask;
    }
    

  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_GetResource, (Os_BiggestType)ResID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* GetResource */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


