/*
 * This is ResumeOSInterrupts.c, auto-generated for:
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
FUNC(void, OS_CODE) Os_ResumeOSInterrupts(void) {  /* [$UKS 124] [$UKS 1546] */

  /* Assumed to be entered with OS interrupts disabled */
  
  if (Os_SuspendOSCount != 0U) {  /* [$UKS 815] */
    OS_VOLATILE Os_imaskType original_imask;
      original_imask = Os_SuspendOSImask; 
      if (0U == --Os_SuspendOSCount) { 
        
        (void)Os_SetImask(original_imask);  /* [$UKS 125] [$UKS 126] [$UKS 127] [$UKS 109] */
      }
  }
  return;
} /* ResumeOSInterrupts */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

