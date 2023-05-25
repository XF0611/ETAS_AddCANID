/*
 * This is ShutdownOS.c, auto-generated for:
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

/*lint --e{715} parameters are not necessarily referenced */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(void, OS_CODE) Os_ShutdownOS(StatusType Error) {  /* [$UKS 20] [$UKS 22] */
    
    (void)Os_SetImaskDis(1U); /* [$UKS 1521] */  /* ETAS BIP Porting */
    Os_RunningISR = INVALID_ISR;
    Os_RunningTask = INVALID_TASK;
    Os_AppOverride = INVALID_OSAPPLICATION;
    if (!Os_InShutdownHook) {
      Os_InShutdownHook = TRUE;
      

      
      Os_AppAccess = 1U;
      
      ShutdownHook(Error);  /* [$UKS 23] */
      Os_InShutdownHook = FALSE;
    }
    /* [$UKS 21] */ /* [$UKS 1521] */
    (void)Os_SetImask(0U);  /* ETAS BIP Porting */
    Os_CurrentAppMode = 0U;

    for (;;) { /*lint !e716 */
      Os_Cbk_InShutdown();
    }
  } /* ShutdownOS */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



