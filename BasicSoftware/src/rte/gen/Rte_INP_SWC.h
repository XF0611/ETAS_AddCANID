/** @file     Rte_INP_SWC.h
  *
  * @brief    Application header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_INP_SWC_H
#define RTE_INP_SWC_H

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
#include "Rte_INP_SWC_Type.h"

#include "Rte_DataHandleType.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_RE_INP_SWC)
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
struct Rte_CDS_INP_SWC {
   uint8 Rte_Dummy;
};
typedef struct Rte_CDS_INP_SWC Rte_CDS_INP_SWC;
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

#define RTE_START_SEC_EcucPartition_ComCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(struct Rte_CDS_INP_SWC, RTE_CONST) Rte_Inst_INP_SWC;
#define RTE_STOP_SEC_EcucPartition_ComCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"


#ifndef RTE_CORE
typedef CONSTP2CONST(struct Rte_CDS_INP_SWC, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** API Mapping macros
 ***
 *******************************************************/

/* API Mapping Macros */
#ifndef RTE_CORE

#define RTE_INP_SWC_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig00_ComRx_0x81_ETAS01_Sig00_ComRx_0x81(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig01_ComRx_0x81_ETAS01_Sig01_ComRx_0x81(P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig02_ComRx_0x81_ETAS01_Sig02_ComRx_0x81(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig03_ComRx_0x81_ETAS01_Sig03_ComRx_0x81(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig04_ComRx_0x81_ETAS01_Sig04_ComRx_0x81(P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig05_ComRx_0x81_ETAS01_Sig05_ComRx_0x81(P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig06_ComRx_0x81_ETAS01_Sig06_ComRx_0x81(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig07_ComRx_0x81_ETAS01_Sig07_ComRx_0x81(P2VAR(uint16, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS01_Sig08_ComRx_0x81_ETAS01_Sig08_ComRx_0x81(P2VAR(uint16, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS02_Sig09_ComRx_0x261_ETAS02_Sig09_ComRx_0x261(P2VAR(boolean, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig34_ComRx_0x456_ETAS06_Sig34_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig35_ComRx_0x456_ETAS06_Sig35_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig36_ComRx_0x456_ETAS06_Sig36_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig37_ComRx_0x456_ETAS06_Sig37_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig38_ComRx_0x456_ETAS06_Sig38_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig39_ComRx_0x456_ETAS06_Sig39_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig40_ComRx_0x456_ETAS06_Sig40_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
FUNC(Std_ReturnType, RTE_CODE) Rte_Read_INP_SWC_RP_ETAS06_Sig41_ComRx_0x456_ETAS06_Sig41_ComRx_0x456(P2VAR(uint8, AUTOMATIC, RTE_APPL_DATA) data);
#define RTE_INP_SWC_STOP_SEC_CODE
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig00_ComRx_0x81_ETAS01_Sig00_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig00_ComRx_0x81_ETAS01_Sig00_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig01_ComRx_0x81_ETAS01_Sig01_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig01_ComRx_0x81_ETAS01_Sig01_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig02_ComRx_0x81_ETAS01_Sig02_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig02_ComRx_0x81_ETAS01_Sig02_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig03_ComRx_0x81_ETAS01_Sig03_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig03_ComRx_0x81_ETAS01_Sig03_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig04_ComRx_0x81_ETAS01_Sig04_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig04_ComRx_0x81_ETAS01_Sig04_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig05_ComRx_0x81_ETAS01_Sig05_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig05_ComRx_0x81_ETAS01_Sig05_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig06_ComRx_0x81_ETAS01_Sig06_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig06_ComRx_0x81_ETAS01_Sig06_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig07_ComRx_0x81_ETAS01_Sig07_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig07_ComRx_0x81_ETAS01_Sig07_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS01_Sig08_ComRx_0x81_ETAS01_Sig08_ComRx_0x81( data ) (Rte_Read_INP_SWC_RP_ETAS01_Sig08_ComRx_0x81_ETAS01_Sig08_ComRx_0x81(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS02_Sig09_ComRx_0x261_ETAS02_Sig09_ComRx_0x261( data ) (Rte_Read_INP_SWC_RP_ETAS02_Sig09_ComRx_0x261_ETAS02_Sig09_ComRx_0x261(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig34_ComRx_0x456_ETAS06_Sig34_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig34_ComRx_0x456_ETAS06_Sig34_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig35_ComRx_0x456_ETAS06_Sig35_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig35_ComRx_0x456_ETAS06_Sig35_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig36_ComRx_0x456_ETAS06_Sig36_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig36_ComRx_0x456_ETAS06_Sig36_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig37_ComRx_0x456_ETAS06_Sig37_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig37_ComRx_0x456_ETAS06_Sig37_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig38_ComRx_0x456_ETAS06_Sig38_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig38_ComRx_0x456_ETAS06_Sig38_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig39_ComRx_0x456_ETAS06_Sig39_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig39_ComRx_0x456_ETAS06_Sig39_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig40_ComRx_0x456_ETAS06_Sig40_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig40_ComRx_0x456_ETAS06_Sig40_ComRx_0x456(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_INP_SWC)
#define Rte_Read_RP_ETAS06_Sig41_ComRx_0x456_ETAS06_Sig41_ComRx_0x456( data ) (Rte_Read_INP_SWC_RP_ETAS06_Sig41_ComRx_0x456_ETAS06_Sig41_ComRx_0x456(data))
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** RE Prototypes
 ***
 *******************************************************/

#define INP_SWC_START_SEC_CODE
#include "INP_SWC_MemMap.h"
FUNC(void, INP_SWC_CODE) RE_INP_SWC(void);
#define INP_SWC_STOP_SEC_CODE
#include "INP_SWC_MemMap.h"

/* Initial values for data element prototypes */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_INP_SWC_H */
