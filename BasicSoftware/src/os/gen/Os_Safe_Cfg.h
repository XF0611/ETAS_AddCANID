#include "Os_Target_Cfg.h"
#define OS_NUM_APPMODES (2U)
#define OS_NUM_APPLICATIONS (3U)
#define OS_NUM_SPINLOCKS (0U)
#define OS_NUM_TRUSTED_FUNCTIONS (0U)
#define OS_NUM_IOC_CALLBACK_FUNCTIONS (Os_const_ioc_function_count)
#define OS_NUM_EVENTS (0U)
#define OS_NUM_TASKS (15U)
#define OS_NUM_ISRS (2U)
#define OS_NUM_RESOURCES (4U)
#define OS_NUM_ALARMS (0U)
#define OS_NUM_SCHEDULETABLES (1U)
#define OS_NUM_PERIPHERALAREAS (0U)
#define OS_NUM_TRACEPOINTS (3U)
#define OS_NUM_TASKTRACEPOINTS (0U)
#define OS_NUM_INTERVALS (0U)
#define OS_NUM_TRACECATEGORIES (0U)
#define OS_TRACE_CATEGORY_ALWAYS ((Os_TraceCategoriesType)(0x80000000UL))
#define OS_TRACE_CATEGORY_NEVER  ((Os_TraceCategoriesType)(0x00000000UL))
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, OS_TICKS2*) */
#define OS_NUM_COUNTERS (2U)
#define OSCYCLEDURATION (3.33333333333333) /* [$UKS 1224] */
#define OSCYCLESPERSECOND (300000000U) /* [$UKS 1225] */
#define OSSWTICKDURATION (10) /* [$UKS 1226] */
#define OSSWTICKSPERSECOND (100000000U) /* [$UKS 1227] */
#define OS_TICKS2NS_Millisecond_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1000000U)) /* [$UKS 843] */
#define OS_TICKS2US_Millisecond_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1000U)) /* [$UKS 843] */
#define OS_TICKS2MS_Millisecond_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1U)) /* [$UKS 843] */
#define OS_TICKS2SEC_Millisecond_TickCounter(ticks) ((PhysicalTimeType)((ticks) / 1000U)) /* [$UKS 843] */
#define OSMAXALLOWEDVALUE_Millisecond_TickCounter (65535U) /* [$UKS 219] */
#define OSTICKSPERBASE_Millisecond_TickCounter (1U) /* [$UKS 220] */
#define OSTICKDURATION_Millisecond_TickCounter OS_TICKS2NS_Millisecond_TickCounter(1U) /* [$UKS 221] */
#define OSMINCYCLE_Millisecond_TickCounter (1U) /* [$UKS 222] */
#define OS_TICKS2NS_Rte_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1000000U)) /* [$UKS 843] */
#define OS_TICKS2US_Rte_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1000U)) /* [$UKS 843] */
#define OS_TICKS2MS_Rte_TickCounter(ticks) ((PhysicalTimeType)((ticks) * 1U)) /* [$UKS 843] */
#define OS_TICKS2SEC_Rte_TickCounter(ticks) ((PhysicalTimeType)((ticks) / 1000U)) /* [$UKS 843] */
#define OSMAXALLOWEDVALUE_Rte_TickCounter (65535U) /* [$UKS 219] */
#define OSTICKSPERBASE_Rte_TickCounter (1U) /* [$UKS 220] */
#define OSTICKDURATION_Rte_TickCounter OS_TICKS2NS_Rte_TickCounter(1U) /* [$UKS 221] */
#define OSMINCYCLE_Rte_TickCounter (1U) /* [$UKS 222] */
#define OSMAXALLOWEDVALUE OSMAXALLOWEDVALUE_SystemCounter /* [$UKS 215] */
#define OSTICKSPERBASE OSTICKSPERBASE_SystemCounter /* [$UKS 216] */
#define OSMINCYCLE OSMINCYCLE_SystemCounter /* [$UKS 218] */
#define OSTICKDURATION OSTICKDURATION_SystemCounter /* [$UKS 217] */
#define OS_NUM_CORES (1U)
#define OS_NUM_OS_CORES (1U)
#define OS_CORE_ID_MASTER (0U)  /* [$UKS 1609] */

/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern FUNC(StatusType, OS_CODE) Os_IncrementCounter_Millisecond_TickCounter(void);
extern FUNC(StatusType, OS_CODE) Os_IncrementCounter_Rte_TickCounter(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_BswBg_100ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_BswBg_1ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_AppCore_ASW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_ASW_100ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_ASW_50ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_ComCore_ASW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_ASW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_AppCore_BSW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_ComCore_BSW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_BSW_10ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_BSW_1ms(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_BSW_SwcRequest(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_DRE(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_SysCore_Startup(void);
extern FUNC(void, OS_CODE) Os_Entry_OsTask_ECU_Startup(void);
extern FUNC(void, OS_CODE) Os_Entry_SystemCounter(void);
extern FUNC(void, OS_CODE) Os_Entry_Gpt1_Isr(void);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
