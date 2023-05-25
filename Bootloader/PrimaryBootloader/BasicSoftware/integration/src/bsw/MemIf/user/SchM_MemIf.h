/** @file     SchM_MemIf.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.4.1 
  */

#ifndef SchM_MemIf
#define SchM_MemIf

#include "SchM_MemIf_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_MemIf_Rb_MainFunction)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
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

#define MEMIF_START_SEC_CODE
#include "MemIf_MemMap.h"
FUNC(void, MEMIF_CODE) MemIf_Rb_MainFunction(void);
#define MEMIF_STOP_SEC_CODE
#include "MemIf_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_MemIf */
