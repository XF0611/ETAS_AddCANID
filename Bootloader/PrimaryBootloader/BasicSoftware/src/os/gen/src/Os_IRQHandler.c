/*
 * Copyright ETAS GmbH 2022
 *
 * Notes:
 *  Multiple nested IRQs are allowed. [$TargetAM27RR5TI 96]
 *  Handler is in ARM mode, entered in system mode with I disabled.
 *  [$TgtTDA4GHS 19] [$TgtTDA4TI 19] VIM Category 2 interrupts use IRQ
 *  [$TgtTDA4GHS 94] [$TgtTDA4TI 94] The cross-core interrupt will be handled via IRQ because it operates at the lowest IPL.
 */
/*
 * This is Os_IRQHandler.c, auto-generated for:
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

//ETAS BIP Porting TODO
#include "arc_exception.h"


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern void DefaultInterruptHandler(void);
typedef struct {
  Os_imaskType dispatch_imask;
  uint32 cat2_index;
} cat2info_t;
static CONST(cat2info_t, OS_CONST) Os_cat2info[NUM_EXC_ALL] = {/* size of OS_NUM_ISRS+1 */
  /* ETAS BIP Porting: only the Interrupt vector which is configured as CAT2 ISR shall have cat2_index value which equals to pos. of Os_const_isrs[]. 
   *                   others interrupt vector which are not configured shall not have cat2_index value match Os_const_isrs[].
   */
  {  1U, 0U },  {  1U, 1U },  {  1U, 2U },  {  1U, 3U },  {  1U, 4U },  {  1U, 5U },  {  1U, 6U },  {  1U, 7U },  {  1U, 8U },  {  1U, 9U },
  {  1U, 10U }, {  1U, 11U }, {  1U, 12U }, {  1U, 13U }, {  1U, 14U }, {  1U, 15U }, {  1U, 0U /*ETAS BIP Porting*/ }, {  1U, 17U }, {  1U, 18U }, {  1U, 19U },
  {  1U, 20U }, {  1U, 21U }, {  1U, 22U }, {  1U, 23U }, {  1U, 24U }, {  1U, 25U }, {  1U, 26U }, {  1U, 27U }, {  1U, 28U }, {  1U, 29U },
  {  1U, 30U }, {  1U, 31U }, {  1U, 32U }, {  1U, 33U }, {  1U, 34U }, {  1U, 35U }, {  1U, 1U /*ETAS BIP Porting*/ }, {  1U, 37U }, {  1U, 38U }, {  1U, 2U /*ETAS BIP Porting*/ },
  {  1U, 3U /*ETAS BIP Porting*/ }, {  1U, 4U /*ETAS BIP Porting*/ }, {  1U, 5U /*ETAS BIP Porting*/ }, {  1U, 43U }, {  1U, 44U }, {  1U, 45U }, {  1U, 46U }, {  1U, 47U }, {  1U, 48U }, {  1U, 49U },
  {  1U, 50U }, {  1U, 51U }, {  1U, 52U }, {  1U, 53U }, {  1U, 54U }, {  1U, 55U }, {  1U, 56U }, {  1U, 57U }, {  1U, 58U }, {  1U, 59U },
  {  1U, 60U }, {  1U, 61U }, {  1U, 62U }, {  1U, 63U }, {  1U, 64U }, {  1U, 65U }, {  1U, 66U }, {  1U, 67U }, {  1U, 68U }, {  1U, 69U },
  {  1U, 70U }, {  1U, 71U }, {  1U, 72U }, {  1U, 73U }, {  1U, 74U }, {  1U, 75U }, {  1U, 76U }, {  1U, 77U }, {  1U, 78U }, {  1U, 79U },
  {  1U, 80U }, {  1U, 81U }, {  1U, 82U }, {  1U, 83U }, {  1U, 84U }, {  1U, 85U }, {  1U, 86U }, {  1U, 87U }, {  1U, 88U }, {  1U, 89U },
  {  1U, 90U }, {  1U, 91U }, {  1U, 92U }, {  1U, 93U }, {  1U, 94U }, {  1U, 95U }, {  1U, 96U }, {  1U, 97U }, {  1U, 98U }, {  1U, 99U },
  {  1U, 100U }, {  1U, 101U }, {  1U, 102U }, {  1U, 103U }, {  1U, 104U }, {  1U, 105U }, {  1U, 106U }, {  1U, 107U }, {  1U, 108U }, {  1U, 109U },
  {  1U, 110U }, {  1U, 111U }, {  1U, 112U }, {  1U, 113U }, {  1U, 114U }, {  1U, 115U }, {  1U, 116U }, {  1U, 117U }, {  1U, 118U }, {  1U, 119U },
  {  1U, 120U }, {  1U, 121U }, {  1U, 122U }, {  1U, 123U }, {  1U, 124U }, {  1U, 125U }, {  1U, 126U }, {  1U, 127U }, 
};
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
FUNC(void, OS_CODE_FAST) Os_IRQHandler(void) {
  Os_imaskType prev;
  uint32 actirq_val;
  uint32 irq_vec; /* ETAS BIP Porting */
  boolean is_valid;

  irq_vec = _arc_aux_read(AUX_IRQ_CAUSE);               /* Read IRQ_CAUSE to get the active interrupt vector */
  is_valid = (arc_int_probe(irq_vec) == 0U)?FALSE:TRUE; /* Is it flagged valid? */

  actirq_val = _arc_aux_read(AUX_IRQ_ACT); /* Read IRQACT to get the IRQ number */
  if (arc_int_enabled(irq_vec) == 0U) { /* Check IRQ validity */
    is_valid = FALSE;
  }

  prev = Os_CurrentImask;      /* Read incoming priority */

  if (is_valid) 
  {
    (void)Os_SetImask(Os_cat2info[irq_vec].dispatch_imask);  /* Set the running IPL */
    if(_arc_aux_read(AUX_IRQ_HINT) == irq_vec) {_arc_aux_write(AUX_IRQ_HINT, 0U);}
    OS_DSB();
    OS_EI();    /* Allow nested interrupts to occur after the next instruction */
    /* [$TargetAM27RR5TI 26] OS uses SYS/USER mode */
    /* [$TargetAM27RR5TI 27] ISRs use SYS/USER mode stack */
    Os_ISRWrapper(prev, Os_cat2info[irq_vec].cat2_index, actirq_val);  /* Call the Cat2 handler */
    OS_DI();   /* Disable all IRQ interrupts */
  }
  else
  {
    OS_DI();
    //ETAS BIP Porting TODO /* Clear pending */
    OS_DSB();
    DefaultInterruptHandler();
  }
  OS_DI();
  Os_SetImaskLowered(prev); /* Restore entry priority */
  OS_EI();
}

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

