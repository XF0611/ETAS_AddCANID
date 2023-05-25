/*
 * This is ElapsedTime.c, auto-generated for:
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
FUNC(Os_StopwatchTickType, OS_CODE) Os_GetElapsedTime(void) {
  Os_StopwatchTickType ret;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 1855] [$UKS 1848] [$UKS 1849] [$UKS 1850] [$UKS 1851] */ /* ETAS BIP Porting */
  ret = Os_CurrentMeteredObject->cumulative + (Os_Cbk_GetStopwatch() - Os_CurrentMeteredObject->previous); /* [$UKS 1856] [$UKS 1857] [$UKS 1858] */
  (void)Os_SetImask(previous_imask);
  return ret;
} /* GetElapsedTime */




 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(Os_StopwatchTickType, OS_CODE) Os_GetTaskElapsedTime(TaskType TaskID) {
  Os_StopwatchTickType ret; /* [$UKS 1859] [$UKS 1848] [$UKS 1850] */
  ret = Os_IsTaskValid(TaskID) ? TaskID->dynamic->meter.cumulative : 0U; /* [$UKS 1860] [$UKS 1861] [$UKS 1862] */
  return ret;
} /* GetTaskElapsedTime */


 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_ResetTaskElapsedTime(TaskType TaskID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 1871] [$UKS 1853] [$UKS 1873] */ /* ETAS BIP Porting */
  /* Error if the task is not valid */
  if (!(Os_IsTaskValid(TaskID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 1874] */
  if ((0U == (TaskID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; }  /* [$UKS 1875] */
  TaskID->dynamic->meter.cumulative = 0U; /* [$UKS 1872] */
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_Os_ResetTaskElapsedTime, (Os_BiggestType)TaskID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* ResetTaskElapsedTime */





 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(Os_StopwatchTickType, OS_CODE) Os_GetISRElapsedTime(ISRType ISRID) {
  Os_StopwatchTickType ret; /* [$UKS 1863] [$UKS 1848] [$UKS 1851] */
  ret = Os_IsISRValid(ISRID) ? ISRID->dynamic->meter.cumulative : 0U; /* [$UKS 1864] [$UKS 1865] [$UKS 1866] */
  return ret;
} /* GetISRElapsedTime */


 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_ResetISRElapsedTime(ISRType ISRID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 1876] [$UKS 1854] [$UKS 1878] */ /* ETAS BIP Porting */
  /* Error if the task is not valid */
  if (!(Os_IsISRValid(ISRID))) { api_retval = E_OS_ID;  goto api_error_exit; } /* [$UKS 1879] */
  if ((0U == (ISRID->access & Os_AppAccess))) { api_retval = E_OS_ACCESS;  goto api_error_exit; }  /* [$UKS 1880] */
  ISRID->dynamic->meter.cumulative = 0U; /* [$UKS 1877] */
  goto api_exit;
api_error_exit:
  Os_ErrorLog(api_retval, OSServiceId_Os_ResetISRElapsedTime, (Os_BiggestType)ISRID, (Os_BiggestType)0, (Os_BiggestType)0); /* [MISRA 2012 Rule 11.4/11.6] */ /*lint !e9078 !e923 !e9030 !e9087 !e931 !e826 !e571 */
api_exit:
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* ResetISRElapsedTime */





 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(Os_StopwatchTickType, OS_CODE) Os_GetIdleElapsedTime(Os_IdleType IdleID) {
  Os_StopwatchTickType ret;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U); /* [$UKS 1867] [$UKS 1848] [$UKS 1849] */ /* ETAS BIP Porting */
  ret = 0U; /* [$UKS 1895] */
  {
  const Os_MeterInfoType *meter = (Os_MeterInfoType *)0;
  if (OS_CORE_CURRENT == IdleID) { /* [$UKS 1869] */
    meter = &Os_IdleMeter;
  }

  if ((Os_MeterInfoType *)0 != meter) {
    /* [$UKS 1868] */
    ret = meter->cumulative;
    if (meter == Os_CurrentMeteredObject) {
      ret += (Os_Cbk_GetStopwatch() - meter->previous);
    }
  }
  }
  (void)Os_SetImask(previous_imask);
  return ret;
} /* GetIdleElapsedTime */


 /* [MISRA 2012 Rule 15.1] */ /*lint -e801 */
FUNC(StatusType, OS_CODE) Os_ResetIdleElapsedTime(Os_IdleType IdleID) {
  StatusType api_retval = E_OK;
  Os_imaskType previous_imask;
  previous_imask = Os_SetImask(1U);  /* [$UKS 1881] [$UKS 1852] [$UKS 1883] */ /* ETAS BIP Porting */
  {
  Os_MeterInfoType *meter = (Os_MeterInfoType *)0;
  if (OS_CORE_CURRENT == IdleID) {
    meter = &Os_IdleMeter;
  }
  if ((Os_MeterInfoType *)0 != meter) {
    meter->cumulative = 0U;  /* [$UKS 1882] */
    meter->previous = Os_Cbk_GetStopwatch();
  }  else {
    /* Invalid core [$UKS 1884] */
    api_retval = E_OS_ID;
  }
  }
  (void)Os_SetImask(previous_imask);
  return api_retval;
} /* ResetIdleElapsedTime */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


