/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Det_Integration.c
 * Date      : Feb 15, 2020
 * Version   : 1.0
 * Description  : This module implements Det Error Fault callout functions
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Det_Prv.h"
#include "Dem_Prv_Det.h"
#include "FiM_Det.h"
#include "CanTp.h"
#include "CanSM.h"
#include "EcuM_Prv.h"
#include "CanNm_Prv.h"
#include "CanTp_Prv.h"
#include "EcuM_Cfg_Startup.h"
/*************************************************************************************************
							CALLOUTS
*************************************************************************************************/

#define DET_START_SEC_CODE
#include "Det_MemMap.h"

#if (DET_CFG_HOOKCOUNT > 0)
/**********************************************************************************
  Function name		:	Det_ReportErrorHook
  Description		  :	This function handles reported BSW error from BSW modules.
  Parameter	(in)	:	ModuleId:   BSW module ID
  Parameter	(in)	:	InstanceId: BSW module instance ID
  Parameter	(in)	:	ApiId:      BSW module API ID
  Parameter	(in)	:	ErrorId:    Error ID
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks: This function is intended to be called by Det as ErrorHook. All Generic DET
  Error shall be handled via this callout.
***********************************************************************************/
void Det_ReportErrorHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    uint32 action;

    /**
     * @brief ETAS BIP Integration Guide: 6.3.1	Det Error Hook
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
#if(0)
    if( ModuleId == ECUM_MODULE_ID && InstanceId == ECUM_INSTANCE_ID && ApiId == ECUM_START_TWO_APIID && ErrorId == ECUM_E_UNINIT)
    {
        /* This is EcuM startup known debug design - An invalid wakeup source was passed as a parameter.  */
        return;
    }
    if( ModuleId == ECUM_MODULE_ID && InstanceId == ECUM_INSTANCE_ID && ApiId == ECUM_CLEAR_WKEVENT_APIID && ErrorId == ECUM_E_UNKNOWN_WAKEUP_SOURCE)
    {
        /* This is EcuM startup known debug design - An invalid wakeup source was passed as a parameter.  */
        return;
    }
    if( ModuleId == ECUM_MODULE_ID && InstanceId == ECUM_INSTANCE_ID && ApiId == ECUM_CLEAR_WKEVENT_APIID && ErrorId == ECUM_E_SERVICE_DISABLED)
    {
        /* This is EcuM startup known debug design - An invalid wakeup source was passed as a parameter.  */
        return;
    }
    if( ModuleId == ECUM_MODULE_ID && InstanceId == ECUM_INSTANCE_ID && ApiId == ECUM_CLEAR_WKEVENT_APIID && ErrorId == ECUM_E_RAM_CHECK_FAILED)
    {
        /* This is EcuM startup known debug design - An invalid wakeup source was passed as a parameter.  */
        return;
    }

    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_SETEVENTSTATUS && ErrorId == DEM_E_UNINIT)
    {
        /* DEM can be reported before Dem_Init but after Dem_PreInit */
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_DISTMEMORY && ErrorId == DEM_E_UNINIT)
    {
        /* DEM can be reported before Dem_Init but after Dem_PreInit */
        return;
    }
#if(DEM_CFG_J1939DCM_FREEZEFRAME_SUPPORT || DEM_CFG_J1939DCM_EXPANDED_FREEZEFRAME_SUPPORT)
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_DEM_J1939DCMGETNEXTFREEZEFRAME && ErrorId == DEM_E_WRONG_CONDITION)
    {
        /* Det error report: No valid data available by the SW-C */
        return;
    }
#endif
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_SETDTCFILTER && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: Wrong filter configuration */
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_DEM_GETDTCOFEVENT && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong dtc of event */
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_DEM_SETDTCSUPPRESSION && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: reported incase DTC FOrmat is neither OBD nor UDS or the event memory entry exists already*/
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_SETENABLECONDITION && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: reported incase DTC FOrmat is neither OBD nor UDS or the event memory entry exists already*/
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_ENVGETSIZEOFEDR && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: reported incase DTC FOrmat is neither OBD nor UDS or the event memory entry exists already*/
        return;
    }
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_GETSIZEOFEXTENDEDDATARECORDSELECTION && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong DTC configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_GETNEXTFREEZEFRAMEDATA && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong freeframe data configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_GETSIZEOFFREEZEFRAMESELECTION && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong  freeframe data configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_DEM_EVMEMGETNUMBEROFEVENTENTRIES && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong number of event memory entries configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_GETDTCBYOCCURRENCETIME && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong number of event memory entries configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_GETEVENTMEMORYOVERFLOW && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong number of event memory entries configuration*/
        return;
    }
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_DEM_EVMEMGETNUMBEROFEVENTENTRIES && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong number of event memory entries configuration*/
        return;
    }
#if (DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON)
	if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
	ApiId == DEM_DET_APIID_DEM_GETINDICATORSTATUS && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: wrong number of event memory entries configuration*/
        return;
    }
#endif
    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_DEM_GETDEBOUNCINGOFEVENT && ErrorId == DEM_E_WRONG_CONFIGURATION)
    {
        /* Det error report: Wrong debouncing of event */
        return;
    }

    if( ModuleId == DEM_MODULE_ID && InstanceId == DEM_INSTANCE_ID && \
        ApiId == DEM_DET_APIID_ENVDARETRIEVE && ErrorId == DEM_E_NODATAAVAILABLE)
    {
        /* Det error report: No valid data available by the SW-C */
        return;
    }


    if( ModuleId == FIM_MODULE_ID && InstanceId == FIM_INSTANCE_ID && \
        ApiId == FIM_PRIV_FIDCOUNTDEC_ID && ErrorId == FIM_E_FID_COUNTERNEGATIVE)
    {
        /*  It's not error of software */
        return;
    }
	if(ModuleId == CANSM_MODULE_ID && ApiId == CANSM_MAINFUNCTION_SID && ErrorId == CANSM_E_MODE_REQUEST_TIMEOUT)
    {
        /* DET debug break point: Mode request for a network failed more often as allowed by configuration, Report DET about exceeding of T_REPEAT_MAX counter, no special error handling. */
        return;
    }
	if(ModuleId == CANSM_MODULE_ID && ApiId == CANSM_REQUESTCOMMODE_SID &&  ErrorId == CANSM_E_INVALID_NETWORK_HANDLE)
    {
        /* DET debug: Mode request for a network failed more often as allowed by configuration, no special error handling. */
        return;
    }

#if (COMM_DEV_ERROR_DETECT != STD_OFF)
    if(ModuleId == COMM_MODULE_ID && InstanceId == COMM_INSTANCE_ID && \
        ApiId == COMM_PREVENT_WAKEUP_APIID && ErrorId == COMM_E_WRONG_PARAMETERS)
    {
        /* Prevent wakeup with wrong channels debug */
        return;
    }

    if( (ModuleId == COMM_MODULE_ID) && && (InstanceId == COMM_INSTANCE_ID) && \
        (ApiId == COMM_ECUM_PNCWAKEUP_INDICATION) && (ErrorId == COMM_E_ERROR_IN_PROV_SERVICE))
    {
        /* Wake up feature is not possible to provide debug. */
        return;
    }

    if( (ModuleId == COMM_MODULE_ID) && && (InstanceId == COMM_INSTANCE_ID) && \
        (ApiId == COMM_MAINFNC_APIID) && (ErrorId == COMM_E_ERROR_IN_PROV_SERVICE))
    {
        /* Request Bus xSM for mode change but fail to provide debug. */
        return;
    }
#endif /* #if (COMM_DEV_ERROR_DETECT != STD_OFF) */
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_PADDING)
    {
        /* Dataleng check debug, PDU received with a length smaller than 8 bytes . */
        return;
    }
    if(ModuleId == CANNM_MODULE_ID && ApiId == CANNM_SID_MAINFUNCTION &&\
	((ErrorId == CANNM_E_NET_START_IND))
    )
    {
        /* DET error debug: Report DET if NM message is received in Bus-Sleep Mode */
    	/* TODO: And runtime error handling shall follow system requirement */
        return;
    }

    if(ModuleId == CANNM_MODULE_ID && ApiId == CANNM_SID_MAINFUNCTION &&\
	((ErrorId == CANNM_E_NETWORK_TIMEOUT))
    )
    {
        /* DET error debug: Report DET if the NM Timeout has expired abnormally */
    	/* TODO: And runtime error handling shall follow system requirement */
        return;
    }
    if(ModuleId == CAN_MODULE_ID && ApiId == CAN_ICOM_RXPROCESSING_SID &&\
	((ErrorId == CAN_E_DATALOST))
    )
    {
        /* DET error debug: Report DET if the message lost */
    	Can_Init(&Can_Config); /* TODO: This decision action better make by application before it's executed */
        return;
    }

    if(ModuleId == CAN_MODULE_ID && ApiId == CAN_TX_CONFIRMATION_PROCESSING_SID &&\
	((ErrorId == CAN_E_DATALOST))
    )
    {
        /* DET error debug: Report DET if transmit history overflow */
        return;
    }
    if(ModuleId == CAN_MODULE_ID && ApiId == CAN_RXFIFO_PROCESSING_SID &&\
	((ErrorId == CAN_E_DATALOST))
    )
    {
        /* DET error debug: Report DET if the message lost */
    	Can_Init(&Can_Config); /* TODO: This decision action better make by application before it's executed */
        return;
    }
    if(ModuleId == CAN_MODULE_ID && ApiId == CAN_RXCMFIFO_PROCESSING_SID &&\
	((ErrorId == CAN_E_DATALOST))
    )
    {
        /* DET error debug: Report DET if the message lost */
    	/* From AR 4.2.2 afterward, this action should have a better classification and called by CAN driver via Det_ReportTransientFaultHook */
    	Can_Init(&Can_Config); /* TODO: This decision action better make by application (e.g. ExeMgr) before it's executed */
        return;
    }
#endif
}
#endif /* #if  (DET_CFG_HOOKCOUNT > 0) */

/**********************************************************************************
  Function name		:	Det_ReportRuntimeErrorHook
  Description		  :	This function handles reported BSW runtime error from BSW modules.
  Parameter	(in)	:	ModuleId:   BSW module ID
  Parameter	(in)	:	InstanceId: BSW module instance ID
  Parameter	(in)	:	ApiId:      BSW module API ID
  Parameter	(in)	:	ErrorId:    Error ID
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks: This function is intended to be called by Det as ErrorHook. All runtime DET error
  shall be handled via this hook.
***********************************************************************************/
#if (DET_CFG_RUNTIMECALLOUT_SUPPORTED)
void Det_ReportRuntimeErrorHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{

    DET_UNUSED_PARAM(InstanceId);
    DET_UNUSED_PARAM(ApiId);
    DET_UNUSED_PARAM(ErrorId);

    /**
     * @brief ETAS BIP Integration Guide: 6.3.1	Det Error Hook
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
#if(0)
	//CanTp_Prv_ReportRunTimeError Det_ReportRuntimeError TODO:
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_MAINFUNCTION && ErrorId == CANTP_E_COM)
    {
        /* Runtime error debug: check  error occurred during a reception or a transmission data */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_MAINFUNCTION && ErrorId == CANTP_E_TX_COM)
    {
        /* Runtime error debug: check  error occurred during a reception or a transmission, any timeout error */
        return;
    }

    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_COM)
    {
        /* Runtime error debug: check  error occurred during a data reception  */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_RX_COM)
    {
        /* Runtime error debug: check  error occurred during a reception timeout error */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_TX_COM)
    {
        /* Runtime error debug: check  error occurred during a transmission timeout error */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_PARAM_POINTER)
    {
        /* Runtime error debug: check  error occurred with NULL pointer */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_RXINDICATION && ErrorId == CANTP_E_INVALID_TATYPE)
    {
        /* Runtime error debug: check  error occurred that the message can not be sent with a Single Frame (SF) */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_TRIGGERTRANSMIT && ErrorId == CANTP_E_COM)
    {
        /* Runtime error debug: check  error occurred during a data transmission  */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_TRIGGERTRANSMIT && ErrorId == CANTP_E_PARAM_POINTER)
    {
        /* Runtime error debug: check  error occurred with NULL pointer  */
        return;
    }
#if(CANTP_CHANGE_PARAMETER_API == CANTP_ON)
	if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_CHANGEPARAMETER &&\
	(ErrorId == CANTP_E_PARAM_ID || ErrorId == CANTP_E_PARAM_CONFIG)
    {
        /* Runtime error debug: check  error occurred during a data transmission  */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_CHANGEPARAMETER && ErrorId == CANTP_E_OPER_NOT_SUPPORTED)
    )
    {
        /* Runtime error debug: returns error if wrong Parameter or wrong range of parameter values for STMin or BS. */
        return;
    }
#endif
#if(CANTP_READ_PARAMETER_API == CANTP_ON)
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_READPARAMETER && ErrorId == CANTP_E_PARAM_POINTER)
    )
    {
        /* Runtime error debug: if wrong parameter pionter. */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_READPARAMETER && ErrorId == CANTP_E_PARAM_ID)
    )
    {
        /* Runtime error debug: if wrong Parameter or wrong range of parameter values for STMin or BS. */
        return;
    }
#endif
#if(CANTP_CANTPLITE_SUPPORT != CANTP_ON)
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_CANCELTRANSMIT && ErrorId == CANTP_E_OPER_NOT_SUPPORTED)
    {
        /* Runtime error debug: check the validity of CanTpTxSduId parameter. */
        return;
    }
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_CANCELRECEIVE && ErrorId == CANTP_E_OPER_NOT_SUPPORTED)
    {
        /* Runtime error debug: check the validity of CanTpRxSduId parameter */
        return;
    }
#endif
#if(CANTP_VERSION_INFO_API == STD_ON)
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_GETVERSIONINFO && ErrorId == CANTP_E_PARAM_POINTER)
    {
        /* Runtime error debug: check the validity of CanTpRxSduId parameter */
        return;
    }
#endif
#if(CANTP_VARIANT != CANTP_PRE_COMPILE)
    if(ModuleId == CANTP_MODULE_ID && ApiId == CANTP_INIT && ErrorId == CANTP_E_PARAM_POINTER)
    {
        /* Runtime error debug: check the returns the version information of the CanTp module TODO */
        return;
    }
#endif

#endif
}
#endif /* #if (DET_CFG_RUNTIMECALLOUT_SUPPORTED) */
/**********************************************************************************
  Function name		:	Det_ReportTransientFaultHook
  Description		  :	This function handles reported BSW transient fault from BSW modules.
  Parameter	(in)	:	ModuleId:   BSW module ID
  Parameter	(in)	:	InstanceId: BSW module instance ID
  Parameter	(in)	:	ApiId:      BSW module API ID
  Parameter	(in)	:	ErrorId:    Error ID
  Parameter	(inout)	:	None
  Parameter	(out)	:	None
  Return value		:	None
  Remarks: This function is intended to be called by Det as ErrorHook.
  This come from An initial concept to have more classification error type come from difference modules (low level).
  e.g. program, erase, read, compare...is supported by Bosch driver IFX/JDP from Cubas 16.x.x
***********************************************************************************/
Std_ReturnType Det_ReportTransientFaultHook(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId)
{
    Std_ReturnType retval = E_OK;

    DET_UNUSED_PARAM(ModuleId);
    DET_UNUSED_PARAM(InstanceId);
    DET_UNUSED_PARAM(ApiId);
    DET_UNUSED_PARAM(ErrorId);

    /* User Integration required. */

    return retval;
}
#define DET_STOP_SEC_CODE
#include "Det_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
