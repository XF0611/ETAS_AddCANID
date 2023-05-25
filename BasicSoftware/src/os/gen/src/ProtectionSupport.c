/*
 * This is ProtectionSupport.c, auto-generated for:
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

FUNC(void, OS_CODE) Os_ProtectionLog(StatusType os_status)
{
  Os_imaskType previous_imask = Os_SetImaskDis(1U); /* [$UKS 576] */  /* ETAS BIP Porting */
  /* [$UKS 1127] */  /* ProtectionHook is configured */
  if (!Os_InProtectionHook) {  /*lint !e9013 */ /* [$UKS 456] */
    ProtectionReturnType action;
    boolean skip_shutdown = FALSE;
    Os_InProtectionHook = TRUE;
    action = ProtectionHook(os_status);
    Os_InProtectionHook = FALSE;
    switch (action) {
      case PRO_IGNORE:
        /* [$UKS 1138] [$UKS 1139] */
        if (os_status == E_OS_PROTECTION_ARRIVAL) { skip_shutdown = TRUE; }
        break;
      case PRO_TERMINATETASKISR:
        /* [$UKS 577] */
        /* [$UKS 583] - drop through to shutdown if did not terminate */
        break;
      case PRO_TERMINATEAPPL:
        /* [$UKS 583] - drop through to shutdown if did not terminate */
        break;
      case PRO_TERMINATEAPPL_RESTART:
        /* [$UKS 583] - drop through to shutdown if did not terminate */
        break;
      case PRO_SHUTDOWN:
        /* [$UKS 580] - Drop through to shutdown */
        break;
      default:
        /* [$UKS 588] - Drop through to shutdown  */
        break;
    }
    if (!skip_shutdown) {
    /* Do shutdown */
      Os_ShutdownOS(os_status);
    }
  }
  (void)Os_SetImaskDis(previous_imask);
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


