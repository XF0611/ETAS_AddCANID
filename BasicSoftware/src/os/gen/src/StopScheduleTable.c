/*
 * This is StopScheduleTable.c, auto-generated for:
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
FUNC(StatusType, OS_CODE) Os_StopScheduleTable(ScheduleTableType ScheduleTableID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 341] */ /* ETAS BIP Porting */
  
  /* Error if not valid */
  if (!(Os_IsScheduleTableValid(ScheduleTableID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 342] */
  if ((0U == (ScheduleTableID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; } /* [$UKS 900] */
  /* [$UKS 1571] */
  
  
  if (ScheduleTableID->dynamic->state == SCHEDULETABLE_STOPPED) { api_retval = E_OS_NOFUNC;  goto api_error_exit; } /* [$UKS 343] [$UKS 308] */
  ScheduleTableID->dynamic->state = SCHEDULETABLE_STOPPED; /* [$UKS  344] */
  
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_StopScheduleTable, (Os_BiggestType)ScheduleTableID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* StopScheduleTable */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



