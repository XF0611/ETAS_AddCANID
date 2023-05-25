/*
 * This is Os_Cfg_KL.c, auto-generated for:
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
#include "Os_Safe_Cfg.h"
#include "IKernel.h"
/* Code start */


/*======================= Counter Activation Code ========================== */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [$UKS 166] */
FUNC(void, OS_CODE) Os_ActivateTaskKL(TaskType TaskID) {
  if (TaskID->dynamic->activation_count == TaskID->activation_count) { /* [$UKS 1897] */
    Os_ErrorLog(E_OS_LIMIT, OSServiceId_ActivateTask, (Os_BiggestType)TaskID,(Os_BiggestType)0U,(Os_BiggestType)0U /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */);
    return; /* [MISRA 2012 Rule 15.5] */ /*lint !e904 */
  } /* [$UKS 791] [$UKS 64] */
  TaskID->dynamic->seen_activation = TRUE;
  TaskID->dynamic->last_activation = Os_Cbk_GetStopwatch();
  TaskID->dynamic->activation_count++;
  Os_ReadyTasks.p0 |= TaskID->pset.p0; /*lint !e931 PC-lint thinks there are side effects here */ /* [$UKS 62] */
  /* No Tasks on core 0 are accessible from other cores. Activation can only be done from the owning core. */
} /* Os_ActivateTaskKL */ /*lint !e818 !e715 suppress possible "not referenced" */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


