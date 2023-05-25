/** @file     SchM_Dcm.h
  *
  * @brief    Basic Software Scheduler Module Interlink header file
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.5.3
  */

#ifndef SchM_Dcm
#define SchM_Dcm

#include "SchM_Dcm_Type.h"
#include "Rte_Intl.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
   /* Do nothing for this and all elif clauses below (rte_sws_2751) */
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_ComM_FullComModeEntered)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_ComM_NoComModeEntered)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_ComM_SilentComModeEntered)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_CopyRxData)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_CopyTxData)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_GetActiveProtocol)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_GetSecurityLevel)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_GetSesCtrlType)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_GetVin)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_Init)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_ResetToDefaultSession)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_SetActiveDiagnostic)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_StartOfReception)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_TpRxIndication)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_TpTxConfirmation)
#elif defined(RTE_RUNNABLEAPI_CE_Dcm_TxConfirmation)
#else
   /* None of above defined so define value including all APIs (rte_sws_2751) */
   #define RTE_PRV_ALL_API
#endif

/* API Mapping Macros */
#ifndef RTE_CORE

#define RTE_DCM_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(VAR(uint8, AUTOMATIC) data);
FUNC(Std_ReturnType, RTE_CODE) SchM_Switch_Dcm_DcmControlDTCSetting(VAR(uint8, AUTOMATIC) data);
FUNC(Std_ReturnType, RTE_CODE) SchM_Switch_Dcm_DcmDiagnosticSessionControl(VAR(uint8, AUTOMATIC) data);
FUNC(Std_ReturnType, RTE_CODE) SchM_Switch_Dcm_DcmEcuReset(VAR(uint8, AUTOMATIC) data);
#define RTE_DCM_STOP_SEC_CODE
#include "Rte_MemMap.h"
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
/* Inline read optimization; SchM_Mode_Dcm_R_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS to direct read */
#define SchM_Mode_Dcm_R_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS()  ( 11 )
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
#define SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS( data ) (SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
#define SchM_Switch_Dcm_DcmControlDTCSetting( data ) (SchM_Switch_Dcm_DcmControlDTCSetting(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
#define SchM_Switch_Dcm_DcmDiagnosticSessionControl( data ) (SchM_Switch_Dcm_DcmDiagnosticSessionControl(data))
#endif
#if defined(RTE_PRV_ALL_API) || defined(RTE_RUNNABLEAPI_BSWSE_DcmMainFunction)
#define SchM_Switch_Dcm_DcmEcuReset( data ) (SchM_Switch_Dcm_DcmEcuReset(data))
#endif

#endif /* RTE_CORE */

/*******************************************************
 ***
 *** Schedulable Entity Prototypes
 ***
 *******************************************************/

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
FUNC(void, DCM_CODE) Dcm_MainFunction(void);
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"

#ifdef __cplusplus
} /* extern C */
#endif /* __cplusplus */

#endif /* !SchM_Dcm */