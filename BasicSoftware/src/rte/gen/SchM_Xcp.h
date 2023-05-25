/** @file     SchM_Xcp.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef SchM_Xcp
#define SchM_Xcp

#include "SchM_Xcp_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_CE_Init)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_SE_EventChannel_100ms)
#elif defined(RTE_RUNNABLEAPI_SE_EventChannel_10ms)
#elif defined(RTE_RUNNABLEAPI_SE_EventChannel_50ms)
#elif defined(RTE_RUNNABLEAPI_SE_MainFunction)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/* API Mapping Macros */
#ifndef RTE_CORE

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Schedulable Entity Prototypes
 ***
 *******************************************************/

#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE) Xcp_EventChannel_100ms(void);
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE) Xcp_EventChannel_10ms(void);
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE) Xcp_EventChannel_50ms(void);
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"
#define XCP_START_SEC_CODE
#include "Xcp_MemMap.h"
FUNC(void, XCP_CODE) Xcp_MainFunction(void);
#define XCP_STOP_SEC_CODE
#include "Xcp_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_Xcp */
