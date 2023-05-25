


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"

/*This hold the current config set details*/
#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
const CanIf_ConfigType * CanIf_Prv_ConfigSet_tpst;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
const CanIf_XCore_ConfigType * CanIf_XCore_Config_pcst;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif
/* FC_VariationPoint_END */

/**********************************************************************************************************************
** Function name     : CanIf_SetControllerMode                                                                       **
** Syntax            : Std_ReturnType CanIf_SetBaudrate(uint8 ControllerId, CanIf_ControllerModeType ControllerMode) **
** Service ID[hex]   : 0x03                                                                                          **
** Sync/Async        : Asynchronous                                                                                  **
** Reentrancy        : Reentrant for different ControllerIds. Non reentrant for the same ControllerId.               **
** Parameters (in)   : ControllerId, ControllerMode                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service calls the corresponding CAN Driver service for changing of the CAN controller    **
**                     mode.                                                                                         **
**                     necessary baud rate modifications the controller might have to reset.                         **
**********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
{

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;

    Can_ReturnType lCanStatus_en        = CAN_NOT_OK;
    Std_ReturnType lRetVal_en           = E_NOT_OK;


    /* Transition mode of the can driver */
    Can_StateTransitionType lCanMode;

    /* Local pointer to the Controller Config structure */
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;

    #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /* Local Pointer to the Driver Config structure */
    const CanIf_CtrlDrvCfgStruct * lCtrlDrvConfig_pst;
    #endif

    /* Local variable to store controller CustId */
    uint8 lCtrlCustId_u8;


    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_CONTROLLER_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CONTROLLERID)

    /* Obtain the custId for the requested controller in the selected variant */
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CONTROLLERID)

    /* If Controller mode is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CANIF_CS_STARTED != ControllerMode) && (CANIF_CS_STOPPED != ControllerMode) && \
                (CANIF_CS_SLEEP != ControllerMode), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CTRLMODE)

    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Get configured data of requested controller*/
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;

    #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /* Select the Can Driver for which this request needs to be forwared */
    lCtrlDrvConfig_pst = CanIf_Prv_ConfigSet_tpst->CanCtrlDrvCont_Ptr +(lCtrlConfig_pst->CanDrvIndx);
    #endif

    /* Requested mode change */
    switch(ControllerMode)
    {

        case CANIF_CS_SLEEP:

        /* Request the selected Can Driver to set the Controller mode to CAN_T_SLEEP */
        #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
            lCanStatus_en = lCtrlDrvConfig_pst->SetCrtlMode(lCtrlConfig_pst->CtrlCanCtrlRef, CAN_T_SLEEP);
        #else
            /* BSW-10855 */ /* [$DD_BSWCODE 25562] */
            lCanStatus_en = CanIf_SetControllerMode_Integration((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), CAN_T_SLEEP);
        #endif
            /*Update the Pdu Mode to CANIF_OFFLINE if Can Driver retruns CAN_OK */
            if(lCanStatus_en != CAN_NOT_OK)
            {
                lRetVal_en = E_OK;

                /*Set PduMode to CANIF_OFFLINE*/
                lControllerState_p->Ctrl_Pdu_mode = ((uint8)CANIF_OFFLINE)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE);


#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
   #if (CANIF_RB_NODE_CALIBRATION == STD_OFF)
                /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/
                if((lCtrlConfig_pst->PnCtrlEn == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
#else
                    if((lControllerState_p->BlPnCtrl_Calib == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
#endif
                {
                    /*As per requirement [SWS_CANIF_00749] PnTxFilter shall be enabled */
                    lControllerState_p->BlPnTxFilter = TRUE;
                }

#endif
            #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
                lControllerState_p->ValidationSts_b = CANIF_RESET;
            #endif
#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))

                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                /*If buffering is enabled, clear all the buffers assigned to requested controller*/
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
            }

            break;


        case CANIF_CS_STARTED:
       #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
           lCanStatus_en = lCtrlDrvConfig_pst->SetCrtlMode(lCtrlConfig_pst->CtrlCanCtrlRef,CAN_T_START);
       #else
           /* BSW-10855 */ /* [$DD_BSWCODE 25562] */
           lCanStatus_en = CanIf_SetControllerMode_Integration((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), CAN_T_START);
       #endif
            if(lCanStatus_en!= CAN_NOT_OK)
            {
               #if (CANIF_LITE_CONFIGURATION == STD_ON)
               /*Set PduMode to CANIF_ONLINE*/
                lControllerState_p->Ctrl_Pdu_mode = (ControllerMode<<CANIF_Ctrl_BIT_SHIFT)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_CTRL_MODE);

               #endif
               lRetVal_en = E_OK;
            }

            break;


        case CANIF_CS_STOPPED:

            if((((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT) == (uint8)CANIF_CS_SLEEP)

            {
                lCanMode = CAN_T_WAKEUP;        /*SWS_CANIF_00487*/
            }
            else
            {
                lCanMode = CAN_T_STOP;          /*[SWS_CANIF_00480], [SWS_CANIF_00488]*/
            }
      #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
          lCanStatus_en = lCtrlDrvConfig_pst->SetCrtlMode(lCtrlConfig_pst->CtrlCanCtrlRef,lCanMode);
      #else
          /* BSW-10855 */ /* [$DD_BSWCODE 25562] */
          lCanStatus_en = CanIf_SetControllerMode_Integration((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), lCanMode);
      #endif
            if(lCanStatus_en != CAN_NOT_OK)
            {
                lRetVal_en = E_OK;

                /*Set PduMode to CANIF_TX_OFFLINE*/
                lControllerState_p->Ctrl_Pdu_mode = ((uint8)CANIF_TX_OFFLINE)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE);

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                /*If buffering is enabled, clear all the buffers assigned to requested controller*/
                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Enter Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

            }
            break;
        default:
            /* Intentionally Empty */
            break;
    }

return(lRetVal_en);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"




/**********************************************************************************************************************
 * Function name    : CanIf_GetControllerMode
 * Syntax           : Std_ReturnType CanIf_GetControllerMode(
 *                                         uint8 ControllerId,
 *                                         CanIf_ControllerModeType * ControllerModePtr
 *                                                                              )
 *
 * Description      : Service reports about the current status of the requested CAN Controller.
 * Parameter        : Controller, ControllerModePtr
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

Std_ReturnType CanIf_GetControllerMode(
                                           uint8 ControllerId,
                                           CanIf_ControllerModeType * ControllerModePtr
                                                        )
{

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;
#if CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON
    uint8 lCtrlCustId_u8;
#endif
    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_CONTROLLER_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_GET_CONTROLLER_SID, \
                                                                            CANIF_E_PARAM_CONTROLLERID)
#if CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_GET_CONTROLLER_SID, \
                                                                            CANIF_E_PARAM_CONTROLLERID)

    /* If ControllerModePtr is null, report to DET and return E_NOT_OK*/
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == ControllerModePtr), CANIF_GET_CONTROLLER_SID,CANIF_E_PARAM_POINTER)

    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

       /* MR12 RULE 10.8, 10.5 VIOLATION:Warning is "Not Critical".The left operand of shift operator is lower in size
     * compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient  */
    /* Copy the status of the controller */
    *ControllerModePtr = (CanIf_ControllerModeType)(((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT);


    return E_OK;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"



/***********************************************************************************************************************
 * Function name    : CanIf_ControllerBusOff
 * Syntax           : void CanIf_ControllerBusOff(
                                                                    uint8 ControllerId
                                                                    )
 * Description      : Service indicates a CAN controller BusOff event referring to the corresponding
                      CAN controller. This service is called by the CAN driver.
 * Parameter        : Controller
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

void CanIf_ControllerBusOff( uint8 ControllerId )
{

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON) || (CANIF_XCORE_CFG_ENABLED != STD_OFF))
    uint8 lCtrlCustId_u8;
#endif
    /*Pointer to the structure CanIf_CallbackFuncType */
    const CanIf_CallbackFuncType * lCallBackPtr_p;


    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONTROLLER_BUSOFF_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((ControllerId >= CANIF_TOTAL_CTRLS), \
                                                                    CANIF_CONTROLLER_BUSOFF_SID,CANIF_E_PARAM_CONTROLLER)
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_XCORE_CFG_ENABLED != STD_OFF))
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((lCtrlCustId_u8 == 0xFFu), CANIF_CONTROLLER_BUSOFF_SID,CANIF_E_PARAM_CONTROLLER)



    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Get the address of structure having dispatch function pointers*/
    lCallBackPtr_p = &CanIf_Callback;

    /* Enter Critical Section */
    SchM_Enter_CanIf_ControllerState();

#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT!= STD_OFF

    /* Reset Tx Confirmation state for the controller*/
    lControllerState_p->CanIf_TxCnfmStatus = CANIF_NO_NOTIFICATION;

#endif

    /*Set PduMode to CANIF_TX_OFFLINE*/
    lControllerState_p->Ctrl_Pdu_mode = ((uint8)CANIF_TX_OFFLINE)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_PDU_MODE);

    /* Leave Critical Section */
    SchM_Exit_CanIf_ControllerState();

#if((CANIF_PUBLIC_TXBUFFERING == STD_ON)&&(CANIF_CFG_TX_FEATURE_ENABLED==STD_ON))
    /* Enter Critical Section */
    SchM_Enter_CanIf_TxBufAccessNoNest();

    /*If buffering is enabled, clear all the buffers assigned to requested controller*/
    CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

    /* Leave Critical Section */
    SchM_Exit_CanIf_TxBufAccessNoNest();

#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
    CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

    if(lCallBackPtr_p->User_ControllerBusOff != NULL_PTR)
    {
        /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/
        lCallBackPtr_p->User_ControllerBusOff(ControllerId);
    }

} /* End of CanIf_ControllerBusOff() */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/***********************************************************************************************************************
 * Function name    : CanIf_ControllerModeIndication_Internal
 * Syntax           : void CanIf_ControllerModeIndication_Internal(
                                                                    uint8 ControllerId,
                                                                    CanIf_ControllerModeType ControllerMode
                                                                    )
 * Description      : Service indicates a mode change is successful for the particular
                      CAN controller. This service is called by the CAN driver.
 * Parameter        : Controller
 * Return value     : None
 * BSW-10854
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* [$DD_BSWCODE 25561] */
void CanIf_ControllerModeIndication_Internal( uint8 ControllerId,
        CanIf_ControllerModeType ControllerMode )
{
    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;
    /*Pointer to the structure CanIf_CallbackFuncType */
    const CanIf_CallbackFuncType * lCallBackPtr_p;
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    uint8 lCtrlCustId_u8;
#endif

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONTROLLER_MODE_IND_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((ControllerId >= CANIF_TOTAL_CTRLS), \
                                                                CANIF_CONTROLLER_MODE_IND_SID,CANIF_E_PARAM_CONTROLLER)
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((lCtrlCustId_u8 == 0xFFu), CANIF_CONTROLLER_MODE_IND_SID,CANIF_E_PARAM_CONTROLLER)


    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Get the address of structure having dispatch function pointers*/
    lCallBackPtr_p =  &CanIf_Callback;

    if(ControllerMode != CANIF_CS_UNINIT)
    {
        /*Update the controller mode*/
        lControllerState_p->Ctrl_Pdu_mode = ((((uint8)ControllerMode)<<CANIF_Ctrl_BIT_SHIFT)|((lControllerState_p->Ctrl_Pdu_mode)&CANIF_BIT_MASK_CTRL_MODE));

    }

    /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/

    if(lCallBackPtr_p->User_ControllerModeIndication != NULL_PTR)
    {
        /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/
        lCallBackPtr_p->User_ControllerModeIndication(ControllerId, ControllerMode);
    }

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
