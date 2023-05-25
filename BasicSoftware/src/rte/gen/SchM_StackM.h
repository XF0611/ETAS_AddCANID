/** @file     SchM_StackM.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef SchM_StackM
#define SchM_StackM

#include "SchM_StackM_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_StackM_GetStackInfo)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_StackM_ScanAllStacks)
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

#define STACKM_START_SEC_CODE
#include "StackM_MemMap.h"
FUNC(void, STACKM_CODE) StackM_ScanAllStacks(void);
#define STACKM_STOP_SEC_CODE
#include "StackM_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_StackM */