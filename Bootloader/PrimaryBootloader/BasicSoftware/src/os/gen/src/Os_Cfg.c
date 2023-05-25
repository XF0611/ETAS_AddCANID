/*
 * This is Os_Cfg.c, auto-generated for:
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
 *   [$UKS 650]
 */
#include "OsMinusConfig.h"
#include "Os_Safe_Cfg.h"
#include "IKernel.h"
/* Code start */
/* [$UKS 651] */
/*=============================== Basic Data ================================ */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
OS_VOLATILE VAR(AppModeType, OS_VAR_CLEARED) Os_CurrentAppMode;
VAR(boolean, OS_VAR_CLEARED) Os_InProtectionHook;
VAR(boolean, OS_VAR_CLEARED) Os_InShutdownHook;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
VAR(Os_StackValueType, OS_VAR_NO_INIT) Os_StackBase;
VAR(ApplicationStateType, OS_VAR_NO_INIT) Os_dyn_appstate[1U + OS_NUM_APPLICATIONS];
VAR(Os_TaskDynType, OS_VAR_NO_INIT) Os_dyn_tasks[OS_NUM_TASKS];
VAR(Os_ISRDynType, OS_VAR_NO_INIT) Os_dyn_isrs[OS_NUM_ISRS];
VAR(Os_ResourceDynType, OS_VAR_NO_INIT) Os_dyn_resources[OS_NUM_RESOURCES];
VAR(Os_CounterDynType, OS_VAR_NO_INIT) Os_dyn_counters[OS_NUM_COUNTERS];
VAR(Os_ScheduleTableDynType, OS_VAR_NO_INIT) Os_dyn_scheduletables[OS_NUM_SCHEDULETABLES];
VAR(Os_ErrorInformationType, OS_VAR_NO_INIT) Os_ErrorInformation/*-public_accessibility-*/;
OS_VOLATILE VAR(Os_psetType, OS_VAR_NO_INIT) Os_ReadyTasks /*lint --e(9018) A union type */;
OS_VOLATILE VAR(ISRType, OS_VAR_NO_INIT) Os_RunningISR;
OS_VOLATILE VAR(TaskType, OS_VAR_NO_INIT) Os_RunningTask;
OS_VOLATILE VAR(Os_tpmaskType, OS_VAR_NO_INIT) Os_RunningTPMask /*lint --e(9018) A union type */;
VAR(Os_MeterInfoRefType, OS_VAR_NO_INIT) Os_CurrentMeteredObject;
VAR(Os_MeterInfoType, OS_VAR_NO_INIT) Os_IdleMeter;
VAR(uint8, OS_VAR_NO_INIT) Os_AppAccess;
OS_VOLATILE VAR(ApplicationType, OS_VAR_NO_INIT) Os_AppOverride;
VAR(Os_StackSizeType, OS_VAR_NO_INIT) Os_GetStackValueAdjust;
VAR(boolean, OS_VAR_NO_INIT)  Os_InErrorHook;
OS_VOLATILE VAR(TaskType, OS_VAR_NO_INIT) Os_ChainTaskRef;
VAR(Os_StackSizeType, OS_VAR_NO_INIT) Os_GetStackUsageAdjust;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_FAST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

CONST(Os_ApplicationConfigurationType, OS_CONST_FAST) Os_const_applications[1U + OS_NUM_APPLICATIONS] = {
  {0U, 
  0U,
  }, /* INVALID_OSAPPLICATION */
  {1U, 
  2U,
  }, /* OsApp_AppCore */
  {2U, 
  4U,
  }, /* OsApp_ComCore */
  {3U, 
  8U,
  }, /* OsApp_SysCore */
};

/*=============================== ISRs ===================================== */
CONST(Os_ISRType, OS_CONST_FAST) Os_const_isrs[OS_NUM_ISRS] = {
  {Os_Entry_SystemCounter,
  &Os_dyn_isrs[0],
  1,
  512U,
  9U,
  3U,
  },
  {Os_Entry_Gpt1_Isr,
  &Os_dyn_isrs[1],
  1,
  512U,
  9U,
  3U,
  },
  {Os_Entry_Can0_InterruptHandler_0,
  &Os_dyn_isrs[2],
  1,
  512U,
  9U,
  3U,
  },
  {Os_Entry_Can0_InterruptHandler_1,
  &Os_dyn_isrs[3],
  1,
  512U,
  9U,
  3U,
  },
  {Os_Entry_Can0_InterruptHandler_2,
  &Os_dyn_isrs[4],
  1,
  512U,
  9U,
  3U,
  },
  {Os_Entry_Can0_InterruptHandler_3,
  &Os_dyn_isrs[5],
  1,
  512U,
  9U,
  3U,
  },
};

/*=============================== Tasks ===================================== */
CONST(Os_TaskType, OS_CONST_FAST) Os_const_tasks0[21U] = {  /* [$UKS 59] [$UKS 44] [$UKS 45] [$UKS 48] [$UKS 49] */
  {
    &Os_dyn_tasks[0],
    Os_Entry_OsTask_SysCore_BswBg_100ms,
    { 1U },
    { 1U },
    { 1U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[1],
    Os_Entry_OsTask_SysCore_BswBg_1ms,
    { 2U },
    { 3U },
    { 3U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[2],
    Os_Entry_OsTask_AppCore_ASW_10ms,
    { 4U },
    { 7U },
    { 7U },
    512U,
    1U,
    11U,
    1U,
  },
  {
    &Os_dyn_tasks[3],
    Os_Entry_OsTask_SysCore_ASW_100ms,
    { 8U },
    { 15U },
    { 15U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[4],
    Os_Entry_OsTask_SysCore_ASW_50ms,
    { 16U },
    { 31U },
    { 31U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[5],
    Os_Entry_OsTask_ComCore_ASW_10ms,
    { 32U },
    { 63U },
    { 63U },
    512U,
    1U,
    13U,
    2U,
  },
  {
    &Os_dyn_tasks[6],
    Os_Entry_OsTask_SysCore_ASW_10ms,
    { 64U },
    { 127U },
    { 127U },
    16000U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[7],
    Os_Entry_OsTask_AppCore_BSW_10ms,
    { 128U },
    { 255U },
    { 255U },
    512U,
    1U,
    11U,
    1U,
  },
  {
    &Os_dyn_tasks[8],
    Os_Entry_OsTask_ComCore_BSW_10ms,
    { 256U },
    { 511U },
    { 511U },
    512U,
    1U,
    13U,
    2U,
  },
  {
    &Os_dyn_tasks[9],
    Os_Entry_OsTask_SysCore_BSW_10ms,
    { 512U },
    { 1023U },
    { 1023U },
    16000U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[10],
    Os_Entry_OsTask_SysCore_BSW_1ms,
    { 1024U },
    { 2047U },
    { 2047U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[11],
    Os_Entry_OsTask_SysCore_BSW_SwcRequest,
    { 2048U },
    { 4095U },
    { 4095U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[12],
    Os_Entry_OsTask_SysCore_DRE,
    { 4096U },
    { 8191U },
    { 8191U },
    512U,
    2U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[13],
    Os_Entry_OsTask_SysCore_Startup,
    { 8192U },
    { 16383U },
    { 2097151U },
    512U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[14],
    Os_Entry_OsTask_ECU_Startup,
    { 16384U }, /* ETAS BIP Porting IPL0 Tasks, priority bit set value for the task */
    { 32767U }, /* ETAS BIP Porting IPL0 Tasks, priority bit mask (1) for any same and lower priority tasks */
    { 32767U }, /* ETAS BIP Porting IPL0 Tasks, priority bit mask (1) for any same and lower priority tasks (no ECC task configured) */
    3072U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[15],
    Os_Entry_osTask_1ms,
    { 32768U },
    { 65535U },
    { 65535U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[16],
    Os_Entry_osTask_5ms,
    { 65536U },
    { 131071U },
    { 131071U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[17],
    Os_Entry_osTask_10ms,
    { 131072U },
    { 262143U },
    { 262143U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[18],
    Os_Entry_osTask_20ms,
    { 262144U },
    { 524287U },
    { 524287U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[19],
    Os_Entry_osTask_StartUp,
    { 524288U },
    { 1048575U },
    { 1048575U },
    1024U,
    1U,
    9U,
    3U,
  },
  {
    &Os_dyn_tasks[20],
    Os_Entry_osTask_15ms,
    { 1048576U },
    { 2097151U },
    { 2097151U },
    1024U,
    1U,
    9U,
    3U,
  },
};

CONST(TaskType, OS_CONST_FAST) Os_const_tasks[OS_NUM_TASKS] = {
  &Os_const_tasks0[0],
  &Os_const_tasks0[1],
  &Os_const_tasks0[2],
  &Os_const_tasks0[3],
  &Os_const_tasks0[4],
  &Os_const_tasks0[5],
  &Os_const_tasks0[6],
  &Os_const_tasks0[7],
  &Os_const_tasks0[8],
  &Os_const_tasks0[9],
  &Os_const_tasks0[10],
  &Os_const_tasks0[11],
  &Os_const_tasks0[12],
  &Os_const_tasks0[13],
  &Os_const_tasks0[14],
  &Os_const_tasks0[15],
  &Os_const_tasks0[16],
  &Os_const_tasks0[17],
  &Os_const_tasks0[18],
  &Os_const_tasks0[19],
  &Os_const_tasks0[20],
};

/*=============================== Resources ================================ */
CONST(Os_ResourceType, OS_CONST_FAST) Os_const_resources[OS_NUM_RESOURCES] = {
  { &Os_dyn_resources[0],
    { 255U }
    , 2U
  },
  { &Os_dyn_resources[1],
    { 511U }
    , 4U
  },
  { &Os_dyn_resources[2],
    { 32767U }
    , 8U
  },
  { &Os_dyn_resources[3],
    { 32767U }
    , 8U
  },
};

CONST(CoreIdType, OS_CONST_FAST) Os_TotalNumberOfCores = OS_NUM_CORES; /* [$UKS 1782] */

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_FAST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */





/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */


/*=============================== StartOS Code =============================== */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/*lint --e{528} : inlined Os_memclr is not always used */
static inline void Os_memclr(uint8 *f, uint32 len); static inline void Os_memclr(uint8 *f, uint32 len) { while (len != 0U) { *f = 0U; f++; len--; } }

FUNC(void, OS_CODE) Os_StartOS_DataInit(void) {
  Os_RunningTask = INVALID_TASK;
  Os_ChainTaskRef = INVALID_TASK;
  Os_RunningISR = INVALID_ISR;
  Os_RunningTPMask.t0 = 0U;
  Os_ReadyTasks.p0 = 0U;
  Os_IdleMeter.elapsed = 0U;
  Os_IdleMeter.max = 0U;
  Os_InErrorHook = FALSE;
  Os_AppAccess = 1U;
  Os_AppOverride = INVALID_OSAPPLICATION;
  Os_IdleMeter.previous = Os_Cbk_GetStopwatch();
  Os_IdleMeter.stackbudget = 0xffffffffU;
  Os_CurrentMeteredObject = &Os_IdleMeter;
  Os_CurrentMeteredObject->stackbase = Os_GetSP();
  Os_GetStackUsageAdjust = 0U;
  Os_GetStackUsageAdjust = Os_GetStackUsage();
  Os_CurrentMeteredObject->stackusage = 0U;
  Os_CurrentMeteredObject->stackmax = 0U;
  { /* Supports Os_GetStackValue() */
  Os_StackValueType svbase;
  Os_StackValueType svtest;
  Os_GetStackValueAdjust = 0U;
  svbase = Os_GetSP();
  svtest = Os_GetStackValue();  /* Typically uses some stack */
  Os_GetStackValueAdjust = svbase - svtest;
  }
  Os_CurrentMeteredObject->stackbase = Os_StackBase;

  Os_memclr((uint8 *)Os_dyn_tasks, sizeof(Os_dyn_tasks)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */ /* [$UKS 1245] */
  Os_memclr((uint8 *)Os_dyn_isrs, sizeof(Os_dyn_isrs)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */ /* [$UKS 1250] */
  Os_memclr((uint8 *)Os_dyn_resources, sizeof(Os_dyn_resources)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */
  Os_memclr((uint8 *)Os_dyn_counters, sizeof(Os_dyn_counters)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */ /* [$UKS 14] [$UKS 1962] */
  Os_memclr((uint8 *)Os_dyn_scheduletables, sizeof(Os_dyn_scheduletables)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */
  Os_memclr((uint8 *)Os_dyn_appstate, sizeof(Os_dyn_appstate)/sizeof(uint8)); /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e9087 !e740 */ /* [$UKS  1309] */
  Os_IocInit(); /* [$UKS 1466] */

}

FUNC(void, OS_CODE) Os_StartOS_StartAuto(void) {

  Os_ActivateTaskKL(&Os_const_tasks0[13]);
  Os_ActivateTaskKL(&Os_const_tasks0[14]);
  Os_ActivateTaskKL(&Os_const_tasks0[19]);
} /*lint !e818 !e715 suppress possible "not referenced" */
/*=============================== Error Handling Code ======================== */



FUNC(void, OS_CODE) Os_ErrorLog(StatusType os_status, OSServiceIdType os_service, Os_BiggestType os_p1, Os_BiggestType os_p2, Os_BiggestType os_p3) { /* Logging is enabled for tracing and hooks */
  Os_imaskType previous_imask;
  Os_ErrorInformationType *info = &Os_ErrorInformation;
  previous_imask = Os_SetImask(1U);  /* [$UKS 453] [$UKS 454] */ /* ETAS BIP Porting */
  
  /* [$UKS 455] [$UKS 461] ErrorHook is configurable */
  if (!Os_InErrorHook) {  /* [$UKS 456] */
    uint8 previous_access = Os_AppAccess;
    Os_InErrorHook = TRUE;
    info->service_id = os_service;
    info->service_param1 = os_p1; /* [$UKS 459] [$UKS 460] */
    info->service_param2 = os_p2;
    info->service_param3 = os_p3;
     /* [MISRA 2012 Rule 11.3] */ /*lint !e9087 !e740 */
    Os_AppAccess = 1U;
    ErrorHook(os_status);
    Os_AppAccess = previous_access;
    Os_InErrorHook = FALSE;
  }
  (void)Os_SetImask(previous_imask);
}




/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */



