


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


#include "CanIf_Prv.h"



/***********************************************************************************************************************
 * Function name    : CanIf_SetPduMode
 * Syntax           : Std_ReturnType CanIf_SetPduMode(
                                                                        uint8 ControllerId,
                                                                        CanIf_PduModeType PduModeRequest
                                                                        )
 * Description      : This service sets the requested mode at all L-PDUs of the predefined CAN controller.
 * Parameter        : Controller, PduModeRequest
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_SetPduMode(
                                                    uint8 ControllerId,
                                                    CanIf_PduModeType PduModeRequest
                                                 )
{

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;

#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON)&& (CANIF_RB_NODE_CALIBRATION == STD_OFF))
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;
#endif
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_PUBLIC_PN_SUPPORT == STD_ON) || (CANIF_XCORE_CFG_ENABLED != STD_OFF))
    uint8 lCtrlCustId_u8;
#endif
    Std_ReturnType lRetVal_en           = E_NOT_OK;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_PDU_MODE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_PUBLIC_PN_SUPPORT == STD_ON)||(CANIF_XCORE_CFG_ENABLED != STD_OFF))
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)

    /*If PduModeRequest is invalid then Report to DET and return E_NOT_OK */
    if((CANIF_OFFLINE != PduModeRequest) && (CANIF_TX_OFFLINE != PduModeRequest) \
                && (CANIF_ONLINE != PduModeRequest)

    #if(CANIF_TXOFFLINEACTIVE_SUPPORT == STD_ON)
                && (CANIF_TX_OFFLINE_ACTIVE != PduModeRequest)

    #endif


    #if(CANIF_USER_TP_TX_OFFLINE_MODE == STD_ON)
                && (CANIF_TX_TP_ONLINE != PduModeRequest) && (CANIF_TX_USER_TP_ONLINE != PduModeRequest)

    #endif
         )
        {
             CANIF_DET_REPORT_ERROR_NO_CONDITON((CANIF_SET_PDU_MODE_SID), (CANIF_E_PARAM_PDU_MODE))
        }



#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON)&&(CANIF_RB_NODE_CALIBRATION == STD_OFF))
    lCtrlConfig_pst           = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;
#endif
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    if((((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT)!= (uint8)CANIF_CS_STARTED)/*[SWS_CANIF_00874]*/

    {
        lRetVal_en = E_NOT_OK;
    }
    else
    {

        switch(PduModeRequest)
        {
            case CANIF_OFFLINE:
#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Leave Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

                /* Flag set to True to clear all Pipes */
                CanIf_XCore_RxClrPipeFlag_b = TRUE;
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

                break;

            case CANIF_TX_OFFLINE:
#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Leave Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
  #if (CANIF_RB_NODE_CALIBRATION == STD_OFF)
                /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/

                if((lCtrlConfig_pst->PnCtrlEn == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
#else
                    if((lControllerState_p->BlPnCtrl_Calib == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
#endif
                {
                    lControllerState_p->BlPnTxFilter = TRUE;
                }

#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */
                break;

          /*This is for sake of removing warning of switch-case*/
            case CANIF_ONLINE:
#if(CANIF_TXOFFLINEACTIVE_SUPPORT == STD_ON)
            case CANIF_TX_OFFLINE_ACTIVE:
#endif

#if(CANIF_USER_TP_TX_OFFLINE_MODE != STD_OFF)
            case CANIF_TX_TP_ONLINE:
            case CANIF_TX_USER_TP_ONLINE:
#endif
                break;

            default:
                /* Intentionally Empty */
                break;

        }

        /* Copy the status of the Pdu */
        lControllerState_p->Ctrl_Pdu_mode = ((uint8)PduModeRequest)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE);


        lRetVal_en = E_OK;
    }
    return(lRetVal_en);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/***********************************************************************************************************************
 * Function name    : CanIf_GetPduMode
 * Syntax           : Std_ReturnType CanIf_GetPduMode(
                                                                         uint8 ControllerId,
                                                                         CanIf_PduModeType * PduModePtr
                                                                       )
 * Description      : This service reports the current mode of the requested Controller.
 * Parameter        : Controller, ChannelModePtr
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_GetPduMode(
                                                    uint8 ControllerId,
                                                    CanIf_PduModeType * PduModePtr
                                                 )
{

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;

#if(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    uint8 lCtrlCustId_u8;
#endif

    /* If CAN Interface is uninitialivzed, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_PDU_MODE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_GET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)
#if(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_GET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)

    /* If PduModePtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == PduModePtr), CANIF_GET_PDU_MODE_SID, CANIF_E_PARAM_POINTER)

    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

        /* MR12 RULE 10.8, 10.5 VIOLATION:Warning is "Not Critical".The left operand of shift operator is lower in size
             * compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient*/
    /*Copy the PduMode to out parameter*/
    *PduModePtr = (CanIf_PduModeType)((lControllerState_p->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE);

    return E_OK;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
