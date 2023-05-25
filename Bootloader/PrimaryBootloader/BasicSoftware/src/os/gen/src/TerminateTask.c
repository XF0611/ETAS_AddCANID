/*
 * This is TerminateTask.c, auto-generated for:
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
FUNC(StatusType, OS_CODE) Os_TerminateTask(void) {
  StatusType api_retval = E_OK; /* [$UKS 66] [$UKS 67] [$UKS 68] [$UKS 71] [$UKS 72] [$UKS 69] [$UKS 794] [$UKS 164] [$UKS 200] [$UKS 171] */
  
  
  if (INVALID_ISR != Os_RunningISR) {
    api_retval += E_OS_CALLEVEL;  Os_ErrorLog(api_retval, OSServiceId_TerminateTask, (Os_BiggestType)0, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */

  } else if (INVALID_TASK != Os_RunningTask) {
    if (Os_RunningTask->dynamic->resource_count!= 0U) { api_retval = E_OS_RESOURCE;  goto api_error_exit; }  /* [$UKS 795] */
    /* [$UKS  796] Call at interrupt level, E_OS_CALLEVEL */
    if (!((2U /* ETAS BIP Porting */ == Os_CurrentImask))) { api_retval = E_OS_CALLEVEL;  goto api_error_exit; }
  } else { /* Neither task nor ISR is running */ }
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_TerminateTask, (Os_BiggestType)0, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  return api_retval;
} /* TerminateTask */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



