/*
 * This is Os_Cfg_Counters.c, auto-generated for:
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
 *   
 */
#include "OsMinusConfig.h"
#include "Os_Safe_Cfg.h"
#include "IKernel.h"
/* Code start */
/* [$UKS 210] [$UKS 260] [$UKS 261] [$UKS 262] [$UKS 263] [$UKS 264] [$UKS 651] */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

extern Os_ScheduleTableDynType *Os_PerformEP(ScheduleTableType stc, Os_ScheduleTableDynType *stdref, TickType wrapval);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/*=============================== Counter Data ============================ */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_FAST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



CONST(Os_CounterType, OS_CONST_FAST) Os_const_counters[OS_NUM_COUNTERS] = { /* $UKS 210 */
  { &Os_dyn_counters[0], Os_IncrementCounter_Millisecond_TickCounter, {65535U, 1U, 1U},
  9U, /* ETAS BIP Porting Counter access by OsApp_SysCore */
  3U, /* ETAS BIP Porting OsApp_SysCore */
  },
  { &Os_dyn_counters[1], Os_IncrementCounter_Rte_TickCounter, {65535U, 1U, 1U},
  9U, /* ETAS BIP Porting Counter access by OsApp_SysCore */
  3U, /* ETAS BIP Porting OsApp_SysCore */
  },
};


/*=============================== ScheduleTable Data ============================ */
CONST(Os_ScheduleTableType, OS_CONST_FAST) Os_const_scheduletables[OS_NUM_SCHEDULETABLES] = {
  { &Os_dyn_scheduletables[0], &Os_const_counters[1],
    TRUE,
    
    0U, 0U,
    9U,
    3U,
  },
};

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_FAST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


CONST(uint8, OS_CONST) Os_const_expiry_intervals[] = {
  1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,1U,
};
CONST(uint8, OS_CONST) Os_const_expiry_actions[] = {
  /* Rte_ScheduleTable */
  6U, 12U, 8U, 14U, 10U, 20U, 22U, 3U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 10U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 6U, 12U, 14U, 20U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 23U, 22U, 1U,
};
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/*========== IncrementCounter_Millisecond_TickCounter ======================== */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_IncrementCounter_Millisecond_TickCounter(void) {
  TickType now;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 242] [$UKS 856] [$UKS 270] [$UKS 271] [$UKS 1121] */ /* ETAS BIP Porting */
  now = 1U + Os_dyn_counters[0].type_dependent.sw.count;
  if (now > Os_const_counters[0].base.maxallowedvalue) {
    now = 0U;
  }
  Os_dyn_counters[0].type_dependent.sw.count = now;
  
  
  (void)Os_SetImask(previous_imask);
  return E_OK;
} /* IncrementCounter_Millisecond_TickCounter */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/*========== IncrementCounter_Rte_TickCounter ======================== */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_IncrementCounter_Rte_TickCounter(void) {
  TickType now;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 242] [$UKS 856] [$UKS 270] [$UKS 271] [$UKS 1121] */ /* ETAS BIP Porting */
  now = 1U + Os_dyn_counters[1].type_dependent.sw.count;
  if (now > Os_const_counters[1].base.maxallowedvalue) {
    now = 0U;
  }
  Os_dyn_counters[1].type_dependent.sw.count = now;
  
  
  { /* [$UKS 304] [$UKS 306] [$UKS 307] [$UKS 910] */
  Os_ScheduleTableDynType *current_std = &Os_dyn_scheduletables[0];
  ScheduleTableType current_stc = &Os_const_scheduletables[0];
  while (current_std < &Os_dyn_scheduletables[1]) { /*lint !e946 pointer does reference the same array */
    if ((current_std->state == SCHEDULETABLE_RUNNING) && (now == current_std->match)) {
      (void)Os_PerformEP(current_stc, current_std, (TickType)(65536U));
    }
    current_std++;
    current_stc++;
  } }
  if ((2U /* ETAS BIP Porting */ == previous_imask) && (Os_ReadyTasks.p0 > Os_RunningTPMask.t0) /*lint !e931 PC-lint thinks there are side effects here */) {
    Os_Dispatch(); 
  }
  (void)Os_SetImask(previous_imask);
  return E_OK;
} /* IncrementCounter_Rte_TickCounter */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/*======================= Counter Start Code ========================== */
/* In MultiCore, this is only called within an AlarmAccess lock */
FUNC(void, OS_CODE) Os_CounterRelativeStart(CounterType ctr, TickType increment, TickRefType match)
{
  TickType now = ctr->dynamic->type_dependent.sw.count;

  if ((ctr->base.maxallowedvalue - now) >= increment) {
    *match = increment + now;
  } else {
    *match = (increment + now) - (1U + ctr->base.maxallowedvalue);
  }
}

/*========================================================================== */
/* [MISRA 2012 Rule 17.2] */ /*lint -esym(9070, Os_PerformEP) */
Os_ScheduleTableDynType *Os_PerformEP(ScheduleTableType stc, Os_ScheduleTableDynType *stdref, TickType wrapval)
{ /* [$UKS 1566]  [$UKS 1567] */
  Os_ScheduleTableDynType *current_std = stdref;
  ScheduleTableType current_stc = stc;
  uint8 cfg_index;
  TickType next_interval;
  P2CONST(uint8, TYPEDEF, OS_VAR) current_action;

  
  
  next_interval = (Os_const_expiry_intervals[(current_std->config & 127U)]);
  current_action = (&Os_const_expiry_actions[current_std->config >> 7]);
  

  

  for(;;) { /*lint !e716 */
    cfg_index = (((*current_action & 30U) >> 1U)); /* [MISRA 2012 Rule 10.6] */ /*lint !e9031 !e9034 */

    if (cfg_index == 0U) {
      break;    /* Final */
    }
    
    
    current_std->config += 128U;
    

    Os_ActivateTaskKL(Os_const_tasks[cfg_index - 1U]);
    if ((*(current_action++) & 1U) != 0U) {
      break;    /* Final action */ /* [MISRA 2012 Rule 15.4] */ /*lint !e9011 */
    }
  }
  
  
  current_std->config += 1U;
  if (cfg_index == 0U) { /* [$UKS 314] */
    current_std->state = SCHEDULETABLE_STOPPED;
    if (current_std->next != (void *)0) {
      current_std->next->dynamic->state = SCHEDULETABLE_RUNNING;   /* [$UKS 310] */
      next_interval += ((TickType)(current_std->next->initial) - (TickType)(current_stc->initial)); /* [MISRA 2012 Rule 10.6] */ /*lint !e9031 !e9034 */
      if (next_interval == 0U) {
        
        (void)Os_PerformEP(current_std->next, current_std->next->dynamic, wrapval);
        
      } else {
        /* switch refs so that next match gets set on the new table */
        
        current_std->next->dynamic->match = current_std->match;
        current_std = current_std->next->dynamic;
        current_std->next = (ScheduleTableType)0;  /* This is the next of the one we have just switched to */
      }
    } else {
        if (current_stc->repeat) {
          current_std->config = current_stc->config;
          current_std->state = SCHEDULETABLE_RUNNING;
        }
    } /* [$UKS 311] [$UKS 312] */
  }
  
  if (current_std->state == SCHEDULETABLE_RUNNING) {
    /* Set next match */
    if (current_std->match < (wrapval - next_interval)) {
      current_std->match += next_interval;
    } else {
      current_std->match -= (wrapval - next_interval);
    }
  }
  
  return current_std;
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


