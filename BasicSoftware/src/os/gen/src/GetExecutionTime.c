/*
 * This is GetExecutionTime.c, auto-generated for:
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
FUNC(Os_StopwatchTickType, OS_CODE) Os_GetExecutionTime(void) {
  Os_StopwatchTickType ret;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 529] [$UKS 538] [$UKS 1121] */ /* ETAS BIP Porting */
  /* [$UKS 539] [$UKS 540] [$UKS 541] [$UKS 542] */
  ret = Os_CurrentMeteredObject->elapsed + (Os_Cbk_GetStopwatch() - Os_CurrentMeteredObject->previous);
  (void)Os_SetImask(previous_imask);
  return ret;
} /* GetExecutionTime */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


