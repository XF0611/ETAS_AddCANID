/*
 * This is Os_Cfg.h, auto-generated for:
 *   Project: ETAS_EcuC_Partitions_AppCore_EcucValues
 *   Target:  TDA4VM-R5-TI
 *   Variant: TDA4VM_MCU_R5FSS0_CORE0
 *   Version: 5.0.3
 *   [$UKS 650]
 */
#ifndef OS_CFG_H
#define OS_CFG_H
#include "Os_Safe_Cfg.h"
/* -------- Time Monitoring --------- */
#define OS_TIME_MONITORING  /* [$UKS 959] */
/* -------- Time Recording --------- */
#define OS_ELAPSED_TIME_RECORDING
/* -------- Activation Monitoring --------- */
#define OS_ACTIVATION_MONITORING  /* [$UKS 1282] */
/* -------- Stack Monitoring --------- */
#define OS_STACK_MONITORING  /* [$UKS 954] */

/* -------- AppMode declarations --------- */

/* ----- OS-Application declarations ----- */
#define OsApp_AppCore ((ApplicationType)1U) /* [$UKS 412] */
#define OsApp_ComCore ((ApplicationType)2U) /* [$UKS 412] */
#define OsApp_SysCore ((ApplicationType)3U) /* [$UKS 412] */

/* ----- PeripheralArea declarations ----- */

/* -------- Event declarations --------- */

/* -------- Task declarations --------- */
#define OsTask_SysCore_BswBg_100ms (&Os_const_tasks0[0])
#define OS_TPL_FOR_OsTask_SysCore_BswBg_100ms (0U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_BswBg_100ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_BswBg_100ms (0U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_BswBg_100ms)
#define OsTask_SysCore_BswBg_1ms (&Os_const_tasks0[1])
#define OS_TPL_FOR_OsTask_SysCore_BswBg_1ms (1U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_BswBg_1ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_BswBg_1ms (1U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_BswBg_1ms)
#define OsTask_AppCore_ASW_10ms (&Os_const_tasks0[2])
#define OS_TPL_FOR_OsTask_AppCore_ASW_10ms (2U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_AppCore_ASW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_AppCore_ASW_10ms (2U) /* [$UKS 2185] */
DeclareTask(OsTask_AppCore_ASW_10ms)
#define OsTask_SysCore_ASW_100ms (&Os_const_tasks0[3])
#define OS_TPL_FOR_OsTask_SysCore_ASW_100ms (3U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_ASW_100ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_ASW_100ms (3U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_ASW_100ms)
#define OsTask_SysCore_ASW_50ms (&Os_const_tasks0[4])
#define OS_TPL_FOR_OsTask_SysCore_ASW_50ms (4U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_ASW_50ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_ASW_50ms (4U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_ASW_50ms)
#define OsTask_ComCore_ASW_10ms (&Os_const_tasks0[5])
#define OS_TPL_FOR_OsTask_ComCore_ASW_10ms (5U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_ComCore_ASW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_ComCore_ASW_10ms (5U) /* [$UKS 2185] */
DeclareTask(OsTask_ComCore_ASW_10ms)
#define OsTask_SysCore_ASW_10ms (&Os_const_tasks0[6])
#define OS_TPL_FOR_OsTask_SysCore_ASW_10ms (6U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_ASW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_ASW_10ms (6U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_ASW_10ms)
#define OsTask_AppCore_BSW_10ms (&Os_const_tasks0[7])
#define OS_TPL_FOR_OsTask_AppCore_BSW_10ms (7U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_AppCore_BSW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_AppCore_BSW_10ms (7U) /* [$UKS 2185] */
DeclareTask(OsTask_AppCore_BSW_10ms)
#define OsTask_ComCore_BSW_10ms (&Os_const_tasks0[8])
#define OS_TPL_FOR_OsTask_ComCore_BSW_10ms (8U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_ComCore_BSW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_ComCore_BSW_10ms (8U) /* [$UKS 2185] */
DeclareTask(OsTask_ComCore_BSW_10ms)
#define OsTask_SysCore_BSW_10ms (&Os_const_tasks0[9])
#define OS_TPL_FOR_OsTask_SysCore_BSW_10ms (9U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_BSW_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_BSW_10ms (9U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_BSW_10ms)
#define OsTask_SysCore_BSW_1ms (&Os_const_tasks0[10])
#define OS_TPL_FOR_OsTask_SysCore_BSW_1ms (10U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_BSW_1ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_BSW_1ms (10U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_BSW_1ms)
#define OsTask_SysCore_BSW_SwcRequest (&Os_const_tasks0[11])
#define OS_TPL_FOR_OsTask_SysCore_BSW_SwcRequest (11U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_BSW_SwcRequest (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_BSW_SwcRequest (11U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_BSW_SwcRequest)
#define OsTask_SysCore_DRE (&Os_const_tasks0[12])
#define OS_TPL_FOR_OsTask_SysCore_DRE (12U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_DRE (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_DRE (12U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_DRE)
#define OsTask_SysCore_Startup (&Os_const_tasks0[13])
#define OS_TPL_FOR_OsTask_SysCore_Startup (13U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_SysCore_Startup (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_SysCore_Startup (13U) /* [$UKS 2185] */
DeclareTask(OsTask_SysCore_Startup)
#define OsTask_ECU_Startup (&Os_const_tasks0[14])
#define OS_TPL_FOR_OsTask_ECU_Startup (14U) /* [$UKS 2007] */
#define OS_IMASK_FOR_OsTask_ECU_Startup (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_OsTask_ECU_Startup (14U) /* [$UKS 2185] */
DeclareTask(OsTask_ECU_Startup)
#define osTask_1ms (&Os_const_tasks0[15])
#define OS_TPL_FOR_osTask_1ms (15U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_1ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_1ms (15U) /* [$UKS 2185] */
DeclareTask(osTask_1ms)
#define osTask_5ms (&Os_const_tasks0[16])
#define OS_TPL_FOR_osTask_5ms (16U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_5ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_5ms (16U) /* [$UKS 2185] */
DeclareTask(osTask_5ms)
#define osTask_10ms (&Os_const_tasks0[17])
#define OS_TPL_FOR_osTask_10ms (17U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_10ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_10ms (17U) /* [$UKS 2185] */
DeclareTask(osTask_10ms)
#define osTask_20ms (&Os_const_tasks0[18])
#define OS_TPL_FOR_osTask_20ms (18U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_20ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_20ms (18U) /* [$UKS 2185] */
DeclareTask(osTask_20ms)
#define osTask_StartUp (&Os_const_tasks0[19])
#define OS_TPL_FOR_osTask_StartUp (19U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_StartUp (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_StartUp (19U) /* [$UKS 2185] */
DeclareTask(osTask_StartUp)
#define osTask_15ms (&Os_const_tasks0[20])
#define OS_TPL_FOR_osTask_15ms (20U) /* [$UKS 2007] */
#define OS_IMASK_FOR_osTask_15ms (2) /* [$UKS 2009] */
#define OS_INDEX_FOR_osTask_15ms (20U) /* [$UKS 2185] */
DeclareTask(osTask_15ms)
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, OS_TPL_FOR_TASK, OS_IMASK_FOR_TASK) */
/* [MISRA 2012 Rule 20.10] */ /*lint -estring(9024, OS_TPL_FOR_TASK) */
#define OS_TPL_FOR_TASK(n) OS_TPL_FOR_##n  /* [$UKS 2008] */
/* [MISRA 2012 Rule 20.10] */ /*lint -estring(9024, OS_IMASK_FOR_TASK) */
#define OS_IMASK_FOR_TASK(n) OS_IMASK_FOR_##n  /* [$UKS 2010] */

/* -------- ISR declarations --------- */
#define SystemCounter (&Os_const_isrs[0U])
#define OS_IMASK_FOR_SystemCounter (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_SystemCounter (0U) /* [$UKS 2186] */
DeclareISR(SystemCounter)
#define Gpt1_Isr (&Os_const_isrs[1U])
#define OS_IMASK_FOR_Gpt1_Isr (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_Gpt1_Isr (1U) /* [$UKS 2186] */
DeclareISR(Gpt1_Isr)
#define Can0_InterruptHandler_0 (&Os_const_isrs[2U])
#define OS_IMASK_FOR_Can0_InterruptHandler_0 (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_Can0_InterruptHandler_0 (2U) /* [$UKS 2186] */
DeclareISR(Can0_InterruptHandler_0)
#define Can0_InterruptHandler_1 (&Os_const_isrs[3U])
#define OS_IMASK_FOR_Can0_InterruptHandler_1 (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_Can0_InterruptHandler_1 (3U) /* [$UKS 2186] */
DeclareISR(Can0_InterruptHandler_1)
#define Can0_InterruptHandler_2 (&Os_const_isrs[4U])
#define OS_IMASK_FOR_Can0_InterruptHandler_2 (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_Can0_InterruptHandler_2 (4U) /* [$UKS 2186] */
DeclareISR(Can0_InterruptHandler_2)
#define Can0_InterruptHandler_3 (&Os_const_isrs[5U])
#define OS_IMASK_FOR_Can0_InterruptHandler_3 (1) /* [$UKS 2009] */
#define OS_INDEX_FOR_Can0_InterruptHandler_3 (5U) /* [$UKS 2186] */
DeclareISR(Can0_InterruptHandler_3)
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, OS_IMASK_FOR_ISR) */
/* [MISRA 2012 Rule 20.10] */ /*lint -estring(9024, OS_IMASK_FOR_ISR) */
#define OS_IMASK_FOR_ISR(n) OS_IMASK_FOR_##n  /* [$UKS 2011] */

/* -------- Resource declarations --------- */
#define RTE_RESOURCE_OS_APP_OsApp_AppCore (&Os_const_resources[0U])
DeclareResource(RTE_RESOURCE_OS_APP_OsApp_AppCore)
#define RTE_RESOURCE_OS_APP_OsApp_ComCore (&Os_const_resources[1U])
DeclareResource(RTE_RESOURCE_OS_APP_OsApp_ComCore)
#define RES_AUTOSAR_ECUM (&Os_const_resources[2U])
DeclareResource(RES_AUTOSAR_ECUM)
#define RTE_RESOURCE_OS_APP_OsApp_SysCore (&Os_const_resources[3U])
DeclareResource(RTE_RESOURCE_OS_APP_OsApp_SysCore)

/* -------- Counter declarations --------- */
#define Millisecond_TickCounter (&Os_const_counters[0U])
DeclareCounter(Millisecond_TickCounter)
#define Rte_TickCounter (&Os_const_counters[1U])
DeclareCounter(Rte_TickCounter)

/* -------- Alarm declaration --------- */

/* -------- ScheduleTable declaration --------- */
#define Fbl_ScheduleTable (&Os_const_scheduletables[0U])
#define OS_DURATION_Fbl_ScheduleTable (22U)
DeclareScheduleTable(Fbl_ScheduleTable)
#define Rte_ScheduleTable (&Os_const_scheduletables[1U])
#define OS_DURATION_Rte_ScheduleTable (100U)
DeclareScheduleTable(Rte_ScheduleTable)

/* -------- Tracepoint declarations --------- */
#define OSApp_OsApp_AppCore (1U)
#define OSApp_OsApp_ComCore (2U)
#define OSApp_OsApp_SysCore (3U)

/* -------- TaskTracepoint declarations --------- */

/* -------- Interval declarations --------- */

/* -------- Filtered APIs --------- */
#define Os_LogTracepoint(TpointID,Category) /* never */
#define Os_LogTracepointValue(TpointID,Value,Category) /* never */
#define Os_LogTracepointData(TpointID,Data,Length,Category) /* never */
#define Os_LogTaskTracepoint(TTpointID,Category) /* never */
#define Os_LogTaskTracepointValue(TTpointID,Value,Category) /* never */
#define Os_LogTaskTracepointData(TTpointID,Data,Length,Category) /* never */
#define Os_LogIntervalStart(IntervalID,Category) /* never */
#define Os_LogIntervalStartValue(IntervalID,Value,Category) /* never */
#define Os_LogIntervalStartData(IntervalID,Data,Length,Category) /* never */
#define Os_LogIntervalEnd(IntervalID,Category) /* never */
#define Os_LogIntervalEndValue(IntervalID,Value,Category) /* never */
#define Os_LogIntervalEndData(IntervalID,Data,Length,Category) /* never */
#ifndef OS_TRACE_NAMESPACE_PURE /* [$UKS 1154] */
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, LogTrace*, LogTaskTrace*, LogInterval*) */  #define LogTracepoint(TpointID,Category) Os_LogTracepoint(TpointID,Category)
  #define LogTracepointValue(TpointID,Value,Category) Os_LogTracepointValue(TpointID,Value,Category)
  #define LogTracepointData(TpointID,Data,Length,Category) Os_LogTracepointData(TpointID,Data,Length,Category)
  #define LogTaskTracepoint(TTpointID,Category) Os_LogTaskTracepoint(TTpointID,Category)
  #define LogTaskTracepointValue(TTpointID,Value,Category) Os_LogTaskTracepointValue(TTpointID,Value,Category)
  #define LogTaskTracepointData(TTpointID,Data,Length,Category) Os_LogTaskTracepointData(TTpointID,Data,Length,Category)
  #define LogIntervalStart(IntervalID,Category) Os_LogIntervalStart(IntervalID,Category)
  #define LogIntervalStartValue(IntervalID,Value,Category) Os_LogIntervalStartValue(IntervalID,Value,Category)
  #define LogIntervalStartData(IntervalID,Data,Length,Category) Os_LogIntervalStartData(IntervalID,Data,Length,Category)
  #define LogIntervalEnd(IntervalID,Category) Os_LogIntervalEnd(IntervalID,Category)
  #define LogIntervalEndValue(IntervalID,Value,Category) Os_LogIntervalEndValue(IntervalID,Value,Category)
  #define LogIntervalEndData(IntervalID,Data,Length,Category) Os_LogIntervalEndData(IntervalID,Data,Length,Category)
#endif

/* [MISRA 2012 Rule 1.3] */ /*lint -estring(9023, Os_LogCat1ISRStart) */
/* [MISRA 2012 Rule 20.10] */ /*lint -esym(9024, Os_LogCat1ISRStart) */
#define Os_LogCat1ISRStart(IsrId)  /* [$UKS 1036] [$UKS 1037] [$UKS 1177] */
/* [MISRA 2012 Rule 1.3] */ /*lint -estring(9023, Os_LogCat1ISREnd) */
/* [MISRA 2012 Rule 20.10] */ /*lint -esym(9024, Os_LogCat1ISREnd) */
#define Os_LogCat1ISREnd(IsrId)  /* [$UKS 1038] [$UKS 1039] [$UKS 1178] */

#ifndef OS_TRACE_NAMESPACE_PURE /* [$UKS 1154] */
  #define LogCat1ISRStart Os_LogCat1ISRStart
  #define LogCat1ISREnd Os_LogCat1ISREnd
#endif
#endif /* OS_CFG_H */
