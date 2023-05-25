/*
 * This is GetStackUsage.c, auto-generated for:
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
FUNC(Os_StackSizeType, OS_CODE) Os_GetStackUsage(void) {
  Os_StackSizeType ret;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 1238] */ /* ETAS BIP Porting */
  {
  Os_StackValueType spnow;
  spnow = Os_GetSP() + Os_GetStackUsageAdjust;
  ret = Os_CurrentMeteredObject->stackbase - spnow; /* [$UKS 1239] [$UKS 1241] */
  if (ret > Os_CurrentMeteredObject->stackusage) { Os_CurrentMeteredObject->stackusage = ret; } /* [$UKS 1240] */
  if (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackmax) { Os_CurrentMeteredObject->stackmax = Os_CurrentMeteredObject->stackusage; }
  }
  {
  Os_StackSizeType spoverrun;
  if ((spoverrun = Os_CurrentMeteredObject->stackusage - Os_CurrentMeteredObject->stackbudget), (Os_CurrentMeteredObject->stackusage > Os_CurrentMeteredObject->stackbudget)) { /* [MISRA 2012 Rule 12.3] */ /*lint !e9008 !e9084 */
      
      Os_Call_StackOverrunHook(spoverrun, OS_BUDGET) /* No ; because this is a macro */ /* [$UKS 1236] [$UKS 1237] [$UKS 1262] [$UKS 1232] [$UKS 1233] [$UKS 1904] */
      Os_CurrentMeteredObject->stackbudget = 0xffffffffU;
    }
  }
  (void)Os_SetImask(previous_imask);
  return ret;
} /* GetStackUsage */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


