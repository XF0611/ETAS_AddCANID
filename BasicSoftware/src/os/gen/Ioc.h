/*
 * This is Ioc.h, auto-generated for:
 *   Project: ETAS_EcuC_Partitions_AppCore_EcucValues
 *   Target:  TDA4VM-R5-TI
 *   Variant: TDA4VM_MCU_R5FSS0_CORE0
 *   Version: 5.0.3
 *   [$UKS 1359] [$UKS 1334] [$UKS 1454]
 */
#ifndef OS_IOC_H
#define OS_IOC_H
/* -- Start expansion of <OsIoc.h> -- */
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
#define IocWrite_Rte_Rx_000010(value) (Os_Ioc_Rte_Rx_000010 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000010(value) (*value = Os_Ioc_Rte_Rx_000010, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000011(value) (Os_Ioc_Rte_Rx_000011 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000011(value) (*value = Os_Ioc_Rte_Rx_000011, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000012(value) (Os_Ioc_Rte_Rx_000012 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000012(value) (*value = Os_Ioc_Rte_Rx_000012, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000013(value) (Os_Ioc_Rte_Rx_000013 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000013(value) (*value = Os_Ioc_Rte_Rx_000013, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000014(value) (Os_Ioc_Rte_Rx_000014 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000014(value) (*value = Os_Ioc_Rte_Rx_000014, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000015(value) (Os_Ioc_Rte_Rx_000015 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000015(value) (*value = Os_Ioc_Rte_Rx_000015, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000016(value) (Os_Ioc_Rte_Rx_000016 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000016(value) (*value = Os_Ioc_Rte_Rx_000016, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
#define IocWrite_Rte_Rx_000017(value) (Os_Ioc_Rte_Rx_000017 = (uint8)value, IOC_E_OK) /* Only callable from: OsApp_SysCore (Trusted) */
#define IocRead_Rte_Rx_000017(value) (*value = Os_Ioc_Rte_Rx_000017, IOC_E_OK) /* Only callable from: OsApp_ComCore (Trusted) */
extern FUNC(void, OS_CODE) Os_ioc_memcpy(void *dest, const void *source, uint32 length);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_IOC_NO_INIT_CORE0_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
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
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000010;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000011;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000012;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000013;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000014;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000015;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000016;
extern VAR(uint8, OS_VAR_NO_INIT) Os_Ioc_Rte_Rx_000017;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_IOC_NO_INIT_CORE0_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* -- End expansion of <OsIoc.h> -- */
#endif /* OS_IOC_H */
