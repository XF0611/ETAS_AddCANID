#include "ITarget.h"
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, Os_Call_StackOverrunHook, Os_Call_TimeOverrunHook) */
#define Os_Call_StackOverrunHook(x,y) Os_Cbk_StackOverrunHook((x),(y));
//ETAS BIP Porting #define Os_Call_TimeOverrunHook(x)    Os_Cbk_TimeOverrunHook(x);


/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
//ETAS BIP Porting extern FUNC(void, OS_CODE) Os_SetEventKL(TaskType TaskID, EventMaskType Mask);
extern FUNC(void, OS_CODE) Os_ActivateTaskKL(TaskType TaskID);
extern FUNC(void, OS_CODE) Os_CounterRelativeStart(CounterType ctr, TickType increment, TickRefType match);
extern FUNC(boolean, OS_CODE) Os_IsCounterValid(CounterType CounterID);
extern FUNC(boolean, OS_CODE) Os_IsScheduleTableValid(ScheduleTableType ScheduleTableID);
extern FUNC(boolean, OS_CODE) Os_IsResourceValid(ResourceType ResID);
extern FUNC(boolean, OS_CODE) Os_IsTaskValid(TaskType TaskID);
extern FUNC(boolean, OS_CODE) Os_IsISRValid(ISRType ISRID);
//ETAS BIP Porting extern FUNC(void, OS_CODE) Os_CheckPeripheralAccess(StatusType *err, AreaIdType Area, Os_ucharConstRefType address, Os_PeripheralAddressType offsetInBytes);
extern FUNC(void, OS_CODE) Os_IocInit(void);
extern FUNC(void, OS_CODE) Os_ErrorLog(StatusType os_status, OSServiceIdType os_service, Os_BiggestType os_p1, Os_BiggestType os_p2, Os_BiggestType os_p3);
extern FUNC(void, OS_CODE) Os_StartOS_DataInit(void);
extern FUNC(void, OS_CODE) Os_StartOS_StartAuto(void);
extern FUNC(void, OS_CODE) Os_Dispatch(void);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
//ETAS BIP Porting extern CONST(uint8, OS_CONST) Os_const_expiry_actions[];
//ETAS BIP Porting extern CONST(uint8, OS_CONST) Os_const_expiry_intervals[];
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern VAR(boolean, OS_VAR_CLEARED) Os_InProtectionHook;
extern VAR(boolean, OS_VAR_CLEARED) Os_InShutdownHook;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
//ETAS BIP Porting extern VAR(Os_ResourceDynType, OS_VAR_NO_INIT) Os_dyn_resources[];
extern VAR(Os_CounterDynType, OS_VAR_NO_INIT) Os_dyn_counters[];
extern VAR(Os_ScheduleTableDynType, OS_VAR_NO_INIT) Os_dyn_scheduletables[];
extern VAR(Os_StackSizeType, OS_VAR_NO_INIT) Os_GetStackValueAdjust;
//ETAS BIP Porting extern VAR(boolean, OS_VAR_NO_INIT)  Os_InErrorHook;
extern OS_VOLATILE VAR(TaskType, OS_VAR_NO_INIT) Os_ChainTaskRef;
extern VAR(Os_StackSizeType, OS_VAR_NO_INIT) Os_GetStackUsageAdjust;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
