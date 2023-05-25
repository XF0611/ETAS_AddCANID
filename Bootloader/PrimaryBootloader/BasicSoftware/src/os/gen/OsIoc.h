/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, IocRead*, IocWrite*, IocSend*, IocRec*) */
#include "Rte_Type.h"

#define IOC_E_OK        RTE_E_OK         /* [$UKS 1360] */
#define IOC_E_NOK       RTE_E_NOK        /* [$UKS 1361] */
#define IOC_E_LIMIT     RTE_E_LIMIT      /* [$UKS 1362] */
#define IOC_E_LOST_DATA RTE_E_LOST_DATA  /* [$UKS 1363] */
#define IOC_E_NO_DATA   RTE_E_NO_DATA    /* [$UKS 1364] */

/* IOC internal data */




/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
#define IocWrite_Rte_Rx_000000(value) (Os_Ioc_Rte_Rx_000000 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000000(value) (*value = Os_Ioc_Rte_Rx_000000, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000001(value) (Os_Ioc_Rte_Rx_000001 = (boolean)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000001(value) (*value = Os_Ioc_Rte_Rx_000001, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000002(value) (Os_Ioc_Rte_Rx_000002 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000002(value) (*value = Os_Ioc_Rte_Rx_000002, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000003(value) (Os_Ioc_Rte_Rx_000003 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000003(value) (*value = Os_Ioc_Rte_Rx_000003, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000004(value) (Os_Ioc_Rte_Rx_000004 = (boolean)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000004(value) (*value = Os_Ioc_Rte_Rx_000004, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000005(value) (Os_Ioc_Rte_Rx_000005 = (boolean)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000005(value) (*value = Os_Ioc_Rte_Rx_000005, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000006(value) (Os_Ioc_Rte_Rx_000006 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000006(value) (*value = Os_Ioc_Rte_Rx_000006, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000007(value) (Os_Ioc_Rte_Rx_000007 = (uint16)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000007(value) (*value = Os_Ioc_Rte_Rx_000007, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000008(value) (Os_Ioc_Rte_Rx_000008 = (uint16)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000008(value) (*value = Os_Ioc_Rte_Rx_000008, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000009(value) (Os_Ioc_Rte_Rx_000009 = (boolean)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000009(value) (*value = Os_Ioc_Rte_Rx_000009, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
extern FUNC(void, OS_CODE) Os_ioc_memcpy(void *dest, const void *source, uint32 length);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_IOC_NO_INIT_CORE0_UNSPECIFIED
//ETAS BIP Porting TODO #include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000000;
extern VAR(boolean, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000001;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000002;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000003;
extern VAR(boolean, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000004;
extern VAR(boolean, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000005;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000006;
extern VAR(uint16, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000007;
extern VAR(uint16, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000008;
extern VAR(boolean, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000009;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_IOC_NO_INIT_CORE0_UNSPECIFIED
//ETAS BIP Porting TODO #include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
