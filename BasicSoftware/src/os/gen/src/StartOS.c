/*
 * This is StartOS.c, auto-generated for:
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


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(boolean, OS_CODE) Os_StartOS(AppModeType Mode) {
  boolean want_dispatch; /* [$UKS 19] */
  
  Os_Initialize_SP(); /* ETAS BIP Porting */

  if ((OS_READ_AUX_IRQ_CTRL() & 0x00000800U) != 0U) { Os_ShutdownOS(E_OS_STATE); } /* AUX_IRQ_CTRL not configured as single stack. */
  OS_EI();
  
  
  (void)Os_SetImask(1U);  /* [$UKS 12] */ /* ETAS BIP Porting */
  if (0U != Os_CurrentAppMode) {
    return FALSE; /* [MISRA 2012 Rule 15.5] */ /*lint !e904 */ /* Already started [$UKS 9] */
  }
  Os_StartOS_DataInit();  /* [$UKS 14] (before StartAuto) */
  Os_CurrentAppMode = Mode;
  if (0U == Os_CurrentAppMode) {
    return FALSE; /* [MISRA 2012 Rule 15.5] */ /*lint !e904 */ /* Zero appmode [$UKS 9] */
  }

  Os_AppOverride = INVALID_OSAPPLICATION;
  Os_AppAccess = 1U;
  Os_StartOS_StartAuto();
  StartupHook();  /* [$UKS 11] [$UKS 1507] */
  
  
  want_dispatch = (Os_ReadyTasks.p0 > Os_RunningTPMask.t0) /*lint !e931 PC-lint thinks there are side effects here */;
  if (want_dispatch) {
    Os_Dispatch(); /* [$UKS 85] */
  }
  (void)Os_SetImask(2U); /* ETAS BIP Porting */
  return TRUE;
} /* StartOS */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


