/** @file     SchM_BswM.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef SchM_BswM
#define SchM_BswM

#include "SchM_BswM_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchAppRun)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchPostRun)
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchPrepShutdown)
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchRun)
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchShutdown)
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchStartupTwo)
#elif defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_SwitchAppMode)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_HARD)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_JUMPTOBOOTLOADER)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_SOFT)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_APP_RUN)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_POST_RUN)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_PREP_SHUTDOWN)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_RUN)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_SHUTDOWN)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_ONE)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_TWO)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_DEFAULT_SESSION)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_PROGRAMMING_SESSION)
#elif defined(RTE_RUNNABLEAPI_SE_BswM_MainFunction)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/* API Mapping Macros */
#ifndef RTE_CORE

#define RTE_BSWM_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(VAR(uint8, AUTOMATIC) data);
FUNC(uint8, RTE_CODE) SchM_Mode_BswM_MDGR_BswM_MRP_BSWM_MDG(void);
FUNC(uint8, RTE_CODE) SchM_Mode_BswM_MDGR_BswM_MRP_DCM_ModeNotification_EcuReset(void);
#define RTE_BSWM_STOP_SEC_CODE
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_APP_RUN) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_POST_RUN) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_PREP_SHUTDOWN) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_RUN) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_SHUTDOWN) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_ONE) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_TWO)
#define SchM_Mode_BswM_MDGR_BswM_MRP_BSWM_MDG() (SchM_Mode_BswM_MDGR_BswM_MRP_BSWM_MDG())
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_DEFAULT_SESSION) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_PROGRAMMING_SESSION)
/* Inline read optimization; SchM_Mode_BswM_MDGR_BswM_MRP_DCM_DiagnosticSessionControl to direct read */
#define SchM_Mode_BswM_MDGR_BswM_MRP_DCM_DiagnosticSessionControl()  ( 0 )
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_HARD) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_JUMPTOBOOTLOADER) || defined(RTE_RUNNABLEAPI_SE_BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_SOFT)
#define SchM_Mode_BswM_MDGR_BswM_MRP_DCM_ModeNotification_EcuReset() (SchM_Mode_BswM_MDGR_BswM_MRP_DCM_ModeNotification_EcuReset())
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchAppRun) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchPostRun) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchPrepShutdown) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchRun) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchShutdown) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_BswMSwitchStartupTwo) || defined(RTE_RUNNABLEAPI_CE_BswM_Action_BswM_AI_SwitchAppMode)
#define SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE( data ) (SchM_Switch_BswM_MDGP_AUTOSAR_BswM_ModeDeclarationGroups_MDG_ECUM_STATE(data))
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Schedulable Entity Prototypes
 ***
 *******************************************************/

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_HARD(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_JUMPTOBOOTLOADER(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_DfrdBswNotification_BswM_MRP_DCM_ModeNotification_EcuReset_SOFT(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_APP_RUN(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_POST_RUN(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_PREP_SHUTDOWN(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_RUN(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_SHUTDOWN(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_ONE(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_BSWM_MDG_ECUM_STATE_STARTUP_TWO(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_DEFAULT_SESSION(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_Cfg_ImdtBswNotification_BswM_MRP_DCM_DiagnosticSessionControl_PROGRAMMING_SESSION(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
FUNC(void, BSWM_CODE) BswM_MainFunction(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_BswM */