/*
 * This is StartScheduleTableRel.c, auto-generated for:
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
FUNC(StatusType, OS_CODE) Os_StartScheduleTableRel(ScheduleTableType ScheduleTableID, TickType Offset) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 329] */ /* ETAS BIP Porting */
  
  if (!(Os_IsScheduleTableValid(ScheduleTableID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 330] */
  if ((0U == (ScheduleTableID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 898] */
  if (Offset == 0U) { api_retval = E_OS_VALUE;  goto api_error_exit; } /* [$UKS 332] */
  if (Offset > (ScheduleTableID->counter->base.maxallowedvalue - ScheduleTableID->initial)) { api_retval = E_OS_VALUE;  goto api_error_exit; } /* [$UKS 333] */
  /* [$UKS 1569] */
  
  
  if (!(ScheduleTableID->dynamic->state == SCHEDULETABLE_STOPPED)) { api_retval = E_OS_STATE;  goto api_error_exit; } /* [$UKS 334] */
  /* [$UKS 335] */
  ScheduleTableID->dynamic->next = (void *)0;
  ScheduleTableID->dynamic->config = ScheduleTableID->config; /* [$UKS 313] */
  Os_CounterRelativeStart(ScheduleTableID->counter, Offset + (TickType)ScheduleTableID->initial, &(ScheduleTableID->dynamic->match));  /* [$UKS 1959] */
  ScheduleTableID->dynamic->state = SCHEDULETABLE_RUNNING;    /* [$UKS 309] [$UKS 317] */
  
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_StartScheduleTableRel, (Os_BiggestType)ScheduleTableID, (Os_BiggestType)Offset, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* StartScheduleTableRel */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


