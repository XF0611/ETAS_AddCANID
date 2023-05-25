/*
 * TDA4VM-R5-TI, TDA4-R5-GHS
 *
 * Copyright ETAS GmbH 2022
 *
 * Notes:
 *  [$TgtTDA4GHS 23] [$TgtTDA4TI 23] Os_InitializeVectorTable initializes the VIM
 *
 */
/*
 * This is Os_vec_init.c, auto-generated for:
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
extern FUNC(void, OS_CODE) Os_Entry_SystemCounter(void);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern FUNC(void, OS_CODE) Os_Entry_Gpt1_Isr(void);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
FUNC(void, OS_CODE) Os_InitializeExceptionTable(void) {
  //ETAS BIP Porting TODO
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [$Satisfies $TgtCore 53] */
FUNC(void, OS_CODE) Os_InitializeVectorTable(void) {
  /* ETAS BIP Porting */
  /* Disable all interrupts */
  OS_DI();

  Os_InitializeExceptionTable();

  //ETAS BIP Porting TODO to change to configurable.
  arc_int_disable(CHIP_SYS_TIMER_INTNO);
  int_handler_install(CHIP_SYS_TIMER_INTNO, Os_Entry_SystemCounter);
  arc_int_enable(CHIP_SYS_TIMER_INTNO);

  //ETAS BIP Porting TODO to change to configurable.
  arc_int_disable(36 /*IRQ_TIMER1*/);
  int_handler_install(36 /*IRQ_TIMER1*/, Os_Entry_Gpt1_Isr);
  arc_int_enable(36 /*IRQ_TIMER1*/);

  Os_CurrentImask = 0U;
  arc_int_ipm_set(Os_CurrentImask);

  /* Enable all interrupts */
  OS_EI();

}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

