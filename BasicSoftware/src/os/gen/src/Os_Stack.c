/*
 * This is Os_Stack.c, auto-generated for:
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

#ifndef _lint /* Do not try to lint these - they are assembly code rather than C */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [$Satisfies $TgtCore 79] */
FUNC(Os_StackValueType, OS_CODE_FAST) Os_GetSP(void) {
  asm(" mov %r0, %sp");
  asm(" j_s [%blink]");
  return 0U;
}

FUNC(void, OS_CODE_FAST) Os_SetSP(MemoryStartAddressType s) {
  asm("mov %sp, %r0");
}

/* [$Satisfies $TgtCore 89] */
FUNC(void, OS_CODE_FAST)  Os_MoveSP(Os_StackSizeType val) {
  asm(" add %sp, %sp, %r0");
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


#endif

