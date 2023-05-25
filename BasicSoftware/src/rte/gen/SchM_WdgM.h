/** @file     SchM_WdgM.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef SchM_WdgM
#define SchM_WdgM

#include "SchM_WdgM_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_CheckpointReached)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_DeInit)
#elif defined(RTE_RUNNABLEAPI_GetFirstExpiredSEID)
#elif defined(RTE_RUNNABLEAPI_GetGlobalStatus)
#elif defined(RTE_RUNNABLEAPI_GetLocalStatus)
#elif defined(RTE_RUNNABLEAPI_GetMode)
#elif defined(RTE_RUNNABLEAPI_Init)
#elif defined(RTE_RUNNABLEAPI_MainFunction)
#elif defined(RTE_RUNNABLEAPI_PerformReset)
#elif defined(RTE_RUNNABLEAPI_SetMode)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/* API Mapping Macros */
#ifndef RTE_CORE
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_SetMode)
/* Inline read optimization; SchM_Mode_WdgM_Current_WdgMSupervisionCycle to direct read */
#define SchM_Mode_WdgM_Current_WdgMSupervisionCycle()  ( 0 )
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_SetMode)
/* Inline read optimization; SchM_Switch_WdgM_WdgMSupervisionCycle to direct read */
#define SchM_Switch_WdgM_WdgMSupervisionCycle( data )  ( ((VAR(Std_ReturnType, AUTOMATIC))SCHM_E_OK) )
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Schedulable Entity Prototypes
 ***
 *******************************************************/

#define WDGM_START_SEC_CODE
#include "WdgM_MemMap.h"
FUNC(void, WDGM_CODE) WdgM_MainFunction(void);
#define WDGM_STOP_SEC_CODE
#include "WdgM_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_WdgM */