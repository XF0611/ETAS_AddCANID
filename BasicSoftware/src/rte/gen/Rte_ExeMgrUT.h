/** @file     Rte_ExeMgrUT.h
  *
  * @brief    Application header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef RTE_EXEMGRUT_H
#define RTE_EXEMGRUT_H

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
#include "Rte_ExeMgrUT_Type.h"

#include "Rte_DataHandleType.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateReset)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_ExecuteReset)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_ExecuteShutdown)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_GetWakeupSource)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_ResetInfo)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_StartupSource)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_TriggerBswBg)
#elif defined(RTE_RUNNABLEAPI_RE_ExeMgr_ValidateWakeup)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/*******************************************************
 ***
 *** Constants
 ***
 *******************************************************/

#ifndef RTE_CORE
#define RTE_E_EcuM_ShutdownTarget_E_NOT_OK ((Std_ReturnType)1)
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Public Types
 ***
 *******************************************************/

/* BEGIN: SWC types (core visible) */
struct Rte_CDS_ExeMgrUT {
   /* Inter-runnable variable handles */
   P2VAR(ExeMgr_StartupSourceType                               , AUTOMATIC, RTE_DATA) Irv_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource;
   P2VAR(ExeMgr_StartupSourceType                               , AUTOMATIC, RTE_DATA) Irv_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource;
   P2VAR(ExeMgr_StartupSourceType                               , AUTOMATIC, RTE_DATA) Irv_RE_ExeMgr_StartupSource_ExeMgr_StartupSource;
   /* Exclusive Area Handles */
   Rte_EnterFP_ExeMgrUT_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA Entry_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA;
   Rte_ExitFP_ExeMgrUT_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA  Exit_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA;
};
typedef struct Rte_CDS_ExeMgrUT Rte_CDS_ExeMgrUT;
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

#define RTE_START_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(struct Rte_CDS_ExeMgrUT, RTE_CONST) Rte_Inst_ExeMgrUT;
#define RTE_STOP_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"


#ifndef RTE_CORE
typedef CONSTP2CONST(struct Rte_CDS_ExeMgrUT, AUTOMATIC, RTE_CONST) Rte_Instance;
#endif /* RTE_CORE */

/*******************************************************
 ***
 *** API Mapping macros
 ***
 *******************************************************/

/* API Mapping Macros */
#ifndef RTE_CORE

#define EcuM_START_SEC_CODE
#include "EcuM_MemMap.h"
FUNC(Std_ReturnType, EcuM_CODE) EcuM_GetLastShutdownTarget(P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, RTE_APPL_DATA) shutdownTarget,
                                                           P2VAR(EcuM_ShutdownModeType, AUTOMATIC, RTE_APPL_DATA) shutdownMode);

FUNC(Std_ReturnType, EcuM_CODE) EcuM_GetShutdownCause(P2VAR(EcuM_ShutdownCauseType, AUTOMATIC, RTE_APPL_DATA) shutdownCause);

FUNC(Std_ReturnType, EcuM_CODE) EcuM_GetShutdownTarget(P2VAR(EcuM_ShutdownTargetType, AUTOMATIC, RTE_APPL_DATA) shutdownTarget,
                                                       P2VAR(EcuM_ShutdownModeType, AUTOMATIC, RTE_APPL_DATA) shutdownMode);

FUNC(Std_ReturnType, EcuM_CODE) EcuM_SelectShutdownCause(VAR(EcuM_ShutdownCauseType, AUTOMATIC) shutdownCause);

FUNC(Std_ReturnType, EcuM_CODE) EcuM_SelectShutdownTarget(VAR(EcuM_ShutdownTargetType, AUTOMATIC) shutdownTarget,
                                                          VAR(EcuM_ShutdownModeType, AUTOMATIC) shutdownMode);

#define EcuM_STOP_SEC_CODE
#include "EcuM_MemMap.h"
#define RTE_START_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
extern CONST(Rte_SelfType_EcuM, RTE_CONST) Rte_Self_CPT_EcuM;
#define RTE_STOP_SEC_EcucPartition_SysCore_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
#define RTE_EXEMGRUT_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_ExeMgrUT_PP_BswM_APPMode_MDGP_APPMode(VAR(uint8, AUTOMATIC) data);
FUNC(uint8, RTE_CODE) Rte_Mode_ExeMgrUT_PP_BswM_APPMode_MDGP_APPMode(void);
FUNC(void, RTE_CODE) Rte_IrTrigger_ExeMgrUT_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource(void);
#define RTE_EXEMGRUT_STOP_SEC_CODE
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateReset) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ResetInfo)
/* Inline function call optimization; Rte_Call_RP_EcuM_SelectResetTarget_GetLastShutdownTarget to EcuM_GetLastShutdownTarget */
#define Rte_Call_RP_EcuM_SelectResetTarget_GetLastShutdownTarget( shutdownTarget, shutdownMode ) EcuM_GetLastShutdownTarget(shutdownTarget, shutdownMode)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ResetInfo)
/* Inline function call optimization; Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownCause to EcuM_GetShutdownCause */
#define Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownCause( shutdownCause ) EcuM_GetShutdownCause(shutdownCause)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
/* Inline function call optimization; Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownTarget to EcuM_GetShutdownTarget */
#define Rte_Call_RP_EcuM_SelectResetTarget_GetShutdownTarget( shutdownTarget, shutdownMode ) EcuM_GetShutdownTarget(shutdownTarget, shutdownMode)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ExecuteReset) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
/* Inline function call optimization; Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownCause to EcuM_SelectShutdownCause */
#define Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownCause( shutdownCause ) EcuM_SelectShutdownCause(shutdownCause)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ExecuteReset) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
/* Inline function call optimization; Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownTarget to EcuM_SelectShutdownTarget */
#define Rte_Call_RP_EcuM_SelectResetTarget_SelectShutdownTarget( shutdownTarget, shutdownMode ) EcuM_SelectShutdownTarget(shutdownTarget, shutdownMode)

#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_TriggerBswBg)
/* Inline function call optimization; Rte_Enter_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA to empty definition */
#define Rte_Enter_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA() /* Null Body. Either the EA Strategy is 'task blocking', mappings of REs to tasks mean that no lock is required, or RteExclusiveAreaImplMechanism is NONE. Body is required to support indirect API. */
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_TriggerBswBg)
/* Inline function call optimization; Rte_Exit_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA to empty definition */
#define Rte_Exit_ExeMgr_TriggerBswBg_EXCLUSIVE_AREA() /* Null Body. Either the EA Strategy is 'task blocking', mappings of REs to tasks mean that no lock is required, or RteExclusiveAreaImplMechanism is NONE. Body is required to support indirect API. */
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_GetWakeupSource)
#define Rte_IrTrigger_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource() (Rte_IrTrigger_ExeMgrUT_RE_ExeMgr_GetWakeupSource_ITP_GetWakeupSource())
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ValidateWakeup)
#define Rte_IrTrigger_RE_ExeMgr_ValidateWakeup_ITP_ValidateWakeupSource() /* nothing */
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
#define Rte_Mode_PP_BswM_APPMode_MDGP_APPMode() (Rte_Mode_ExeMgrUT_PP_BswM_APPMode_MDGP_APPMode())
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_ExecuteReset) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_MainFunction)
#define Rte_Switch_PP_BswM_APPMode_MDGP_APPMode( data ) (Rte_Switch_ExeMgrUT_PP_BswM_APPMode_MDGP_APPMode(data))
#endif

/* Rte_IrvI API (implicit access) */
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_EvaluateWakeup)
#define Rte_IrvIRead_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource() (*Rte_Inst_ExeMgrUT.Irv_RE_ExeMgr_EvaluateWakeup_ExeMgr_StartupSource)
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_StartupSource)
#define Rte_IrvIRead_RE_ExeMgr_StartupSource_ExeMgr_StartupSource() (*Rte_Inst_ExeMgrUT.Irv_RE_ExeMgr_StartupSource_ExeMgr_StartupSource)
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_GetWakeupSource)
#define Rte_IrvIWrite_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource(data) ((*Rte_Inst_ExeMgrUT.Irv_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource) = (data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_RE_ExeMgr_GetWakeupSource)
#define Rte_IrvIWriteRef_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource() ((P2VAR(ExeMgr_StartupSourceType, AUTOMATIC, RTE_DATA)) (Rte_Inst_ExeMgrUT.Irv_RE_ExeMgr_GetWakeupSource_ExeMgr_StartupSource))
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** RE Prototypes
 ***
 *******************************************************/

#define ExeMgrUT_START_SEC_CODE
#include "ExeMgrUT_MemMap.h"
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_EvaluateReset(void);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_EvaluateWakeup(void);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_ExecuteReset(VAR(EcuM_ShutdownCauseType, AUTOMATIC) resetReason,
                                                 VAR(EcuM_ShutdownModeType, AUTOMATIC) resetGroup);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_ExecuteShutdown(void);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_GetWakeupSource(void);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_MainFunction(void);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_ResetInfo(P2VAR(EcuM_ShutdownCauseType, AUTOMATIC, RTE_APPL_DATA) resetReason,
                                              P2VAR(EcuM_ShutdownModeType, AUTOMATIC, RTE_APPL_DATA) resetGroup);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_StartupSource(P2VAR(ExeMgr_StartupSourceType, AUTOMATIC, RTE_APPL_DATA) startupSource);
FUNC(void, ExeMgrUT_CODE) RE_ExeMgr_TriggerBswBg(void);
#define ExeMgrUT_STOP_SEC_CODE
#include "ExeMgrUT_MemMap.h"

/* Initial values for data element prototypes */

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !RTE_EXEMGRUT_H */
