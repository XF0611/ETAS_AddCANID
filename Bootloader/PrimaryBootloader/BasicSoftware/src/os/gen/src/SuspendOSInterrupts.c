/*
 * This is SuspendOSInterrupts.c, auto-generated for:
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
#define OS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
OS_VOLATILE VAR(uint32, OS_VAR_CLEARED) Os_SuspendOSCount;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
OS_VOLATILE VAR(Os_imaskType, OS_VAR_NO_INIT) Os_SuspendOSImask;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(void, OS_CODE) Os_SuspendOSInterrupts(void) { /* [$UKS 128] [$UKS 109] [$UKS 1545] */

  if (0U == Os_SuspendOSCount) {
    Os_imaskType previous_imask = Os_CurrentImask;
    if (Os_imask_to_ipl(Os_CurrentImask) < 2U ) {
      (void)Os_SetImask(0U);   /* Protect against Cat1 ISR [$EHI 637422] */
      Os_SuspendOSImask = previous_imask; /* [$UKS 129] [$UKS 814] */
      Os_SuspendOSCount++;
      Os_SetImaskLowered(1U);
    
    } else {
      Os_SuspendOSCount++;  
      Os_SuspendOSImask = previous_imask; /* [$UKS 129] [$UKS 814] */
    
    }
  } else {
    Os_SuspendOSCount++;
  }
  
  return;
} /* SuspendOSInterrupts */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


