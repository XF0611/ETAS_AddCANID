/*
 * This is Os_masks.c, auto-generated for:
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
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
OS_VOLATILE VAR(Os_imaskType, OS_VAR_NO_INIT) Os_CurrentImask;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
static CONST(uint32, OS_CONST) Os_imask2ipl[3] = {
  2U, 1U, 0U
};
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [$TgtCore 68] */
FUNC(Os_imaskType, OS_CODE) Os_SetImask(Os_imaskType imask) {
  Os_imaskType prev = Os_CurrentImask;

  if(0U == _arc_aux_read(AUX_STATUS32) & 0x00000080UL) {/* kernel mode */OS_DI();}

  /* set imask */
  arc_int_ipm_set(imask);

  Os_CurrentImask = imask;
  if(0U == _arc_aux_read(AUX_STATUS32) & 0x00000080UL) {/* kernel mode */OS_DSB(); OS_ISB(); OS_EI();}
  return prev;
}

FUNC(Os_imaskType, OS_CODE) Os_SetImaskDis(Os_imaskType imask) {
  Os_imaskType prev = Os_CurrentImask;

  /* set imask */
  arc_int_ipm_set(imask);

  Os_CurrentImask = imask;
  OS_DSB();
  OS_ISB();
  return prev;
}

/*
 * When lowering priority we are always adding bits so can ignore
 * the clears.
 */
FUNC(void, OS_CODE) Os_SetImaskLowered(Os_imaskType imask) {
  /* Allow interrupts that are latched to clear themselves (race condition) */

  if(0U == _arc_aux_read(AUX_STATUS32) & 0x00000080UL) {/* kernel mode */OS_EI(); OS_DSB(); OS_ISB(); OS_DI();}

  /* set imask */
  arc_int_ipm_set(imask);

  Os_CurrentImask = imask;

  if(0U == _arc_aux_read(AUX_STATUS32) & 0x00000080UL) {/* kernel mode */OS_DSB(); OS_ISB(); OS_EI();}
}

/* ETAS BIP Porting */
FUNC(uint32, OS_CODE) Os_imask_to_ipl(Os_imaskType imask) {
  uint32 ret = 0U;
  if (imask < 3U ) {
    ret = Os_imask2ipl[imask];
  }
  return ret;
}

/*lint --e{715} suppress possible "maskInfo not referenced" */
FUNC(void, OS_CODE) Os_EnableSourceMask(const Os_ISRSourceType * const maskInfo) {
  /* ETAS BIP Porting */
  arc_int_enable(*maskInfo);
}

/*lint --e{715} suppress possible "maskInfo not referenced" */
FUNC(void, OS_CODE) Os_DisableSourceMask(const Os_ISRSourceType * const maskInfo) {
  /* ETAS BIP Porting */
  arc_int_disable(*maskInfo);
}

/*lint --e{715} suppress possible "maskInfo not referenced" */
FUNC(void, OS_CODE) Os_ClearPendingSource(const Os_ISRSourceType * const maskInfo) {
  /* ETAS BIP Porting */
  if(1U == arc_int_probe(*maskInfo))
  {
    //ETAS BIP Porting TODO clear the interrupt flag register for this IRQ.
  }
}

FUNC(boolean, OS_CODE) Os_IsDisabled(const Os_ISRSourceType * const maskInfo) {
  /* ETAS BIP Porting */
  return 0U == arc_int_enabled(*maskInfo);
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

