/** @file     Rte_CalUT.h
  *
  * @brief    Application header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_CALUT_H
#define RTE_CALUT_H

#ifndef RTE_CORE
#ifdef RTE_APPLICATION_HEADER_FILE
#error Multiple application header files included.
#endif /* RTE_APPLICATION_HEADER_FILE */
#define RTE_APPLICATION_HEADER_FILE
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Includes
 ***
 *******************************************************/

#include "Rte.h"
#include "Rte_Intl.h"
#include "Rte_CalUT_Type.h"

#include "Rte_DataHandleType.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_RE_CalUT)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/*******************************************************
 ***
 *** Constants
 ***
 *******************************************************/


/*******************************************************
 ***
 *** Public Types
 ***
 *******************************************************/

/* BEGIN: SWC types (core visible) */
struct Rte_CDS_CalUT {
   /* Per-Instance Memorys (sic) */
   P2VAR(uint32, AUTOMATIC, RTE_DATA) Pim_ArTypedPIM_1;
};
typedef struct Rte_CDS_CalUT Rte_CDS_CalUT;
/* END: SWC types (core visible) */

/*******************************************************
 ***
 *** Private Types
 ***
 *******************************************************/

/* BEGIN: SWC types (private) */
/* END: SWC types (private) */

/*******************************************************
 ***
 *** Instance Declarations
 ***
 *******************************************************/

#define RTE_START_SEC_EcucPartition_AppCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(struct Rte_CDS_CalUT, RTE_CONST) Rte_Inst_CalUT;
#define RTE_STOP_SEC_EcucPartition_AppCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"


#ifndef RTE_CORE
typedef CONSTP2CONST(struct Rte_CDS_CalUT, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** API Mapping macros
 ***
 *******************************************************/

/* API Mapping Macros */
#ifndef RTE_CORE
#define RTE_START_SEC_EcucPartition_AppCore_VAR_CLEARED_32
#include "Rte_MemMap.h"
extern VAR(uint32, RTE_APPL_DATA) Rte_ArPim_CalUT_ArTypedPIM_1;
#define RTE_STOP_SEC_EcucPartition_AppCore_VAR_CLEARED_32
#include "Rte_MemMap.h"
#define RTE_START_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(uint32, RTE_EcucPartition_AppCore_CALPRM) SpeedLimitUpper;
#define RTE_STOP_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
#define RTE_START_SEC_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(uint32, RTE_CALPRM) speedMax;
#define RTE_STOP_SEC_CALPRM_32
#include "Rte_MemMap.h"
#define RTE_START_SEC_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(uint32, RTE_CALPRM) speedMin;
#define RTE_STOP_SEC_CALPRM_32
#include "Rte_MemMap.h"
/* Inline calprm optimization; Rte_CData_SpeedLimitUpper to direct access */
#define Rte_CData_SpeedLimitUpper() ( (volatile CONST(uint32, RTE_EcucPartition_AppCore_CALPRM)) SpeedLimitUpper )
/* Inline read optimization; Rte_Pim_ArTypedPIM_1 to direct read */
#define Rte_Pim_ArTypedPIM_1()  ( (P2VAR(uint32, AUTOMATIC, RTE_APPL_DATA)) &Rte_ArPim_CalUT_ArTypedPIM_1 )
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_CalUT)
/* Inline calprm optimization; Rte_Prm_RP_SpeedLimit_speedMax to direct access */
#define Rte_Prm_RP_SpeedLimit_speedMax() ( (volatile CONST(uint32, RTE_CALPRM)) speedMax )
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_CalUT)
/* Inline calprm optimization; Rte_Prm_RP_SpeedLimit_speedMin to direct access */
#define Rte_Prm_RP_SpeedLimit_speedMin() ( (volatile CONST(uint32, RTE_CALPRM)) speedMin )
#endif
/* Inline calprm optimization; Rte_CData_SpeedLimitLower to direct access */
#define RTE_START_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(uint32, RTE_EcucPartition_AppCore_CALPRM) SpeedLimitLower;
#define RTE_STOP_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
#define Rte_CData_SpeedLimitLower() ( (volatile CONST(uint32, RTE_EcucPartition_AppCore_CALPRM)) SpeedLimitLower )
/* Inline calprm optimization; Rte_CData_sharedCURVE to direct access */
#define RTE_START_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(IDT_CURVE, RTE_EcucPartition_AppCore_CALPRM) sharedCURVE;
#define RTE_STOP_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
#define Rte_CData_sharedCURVE() ( (volatile P2CONST(float32, AUTOMATIC, RTE_EcucPartition_AppCore_CALPRM)) sharedCURVE )
/* Inline calprm optimization; Rte_CData_sharedMAP to direct access */
#define RTE_START_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
extern volatile CONST(IDT_MAP, RTE_EcucPartition_AppCore_CALPRM) sharedMAP;
#define RTE_STOP_SEC_EcucPartition_AppCore_CALPRM_32
#include "Rte_MemMap.h"
#define Rte_CData_sharedMAP() ( (volatile P2CONST(float32, AUTOMATIC, RTE_EcucPartition_AppCore_CALPRM)) sharedMAP )

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** RE Prototypes
 ***
 *******************************************************/

#define CalUT_START_SEC_CODE
#include "CalUT_MemMap.h"
FUNC(void, CalUT_CODE) RE_CalUT(void);
#define CalUT_STOP_SEC_CODE
#include "CalUT_MemMap.h"

/* Initial values for data element prototypes */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_CALUT_H */