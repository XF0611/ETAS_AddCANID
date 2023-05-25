


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


#include "CanIf_Prv.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;

    /* Local variable to store CustId for given PduId for the selected variant */
    uint16 ltxPduCustId_t;

    /*local pointer to ROM structure of controller */
    const CanIf_Cfg_CtrlConfig_tst * lCtrlConfig_pst;

    /* Pointer to Controller State configuration */
    CanIf_ControllerStateType * lControllerState_p;

    /* Local variable to hold the controller ID */
    uint8 lControllerId_u8;

#if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /*Local variable to hold the custId for controller */
     uint8 lCtrlCustId_u8;

     /*Local pointer to CtrlDrv structure*/
     const CanIf_CtrlDrvCfgStruct * lCtrlDrvConfig_pst;
#endif

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
    /* Local pointer to notification status array */
    CanIf_NotifStatusType * lTxNotifPtr;
#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    uint8 llocalCoreId_u8 ;
    uint8 ldestCoreId_u8;
    uint16 lpipeId_u16 ;
    CanIf_XCore_PipeTxRxParams_st lpipeParams_st;
    PduInfoType pduInfoDummy_st;
#endif
/* FC_VariationPoint_END */

#if(CANIF_RB_NODE_CALIBRATION == STD_ON)
    /* Local pointer to store calibrated Controller Id */
    uint8 lCalibCtrlId;
#endif

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
    const CanIf_Cfg_TxBufferConfig_tst* lCanIf_TxBufferConfig_p;
    PduIdType lCanIf_TxPduId_temp = 0;
    uint8* lDataBuf_p;
    CanIf_Cfg_CanIdBuffer_tst * lCanIdBuf_p;
    uint32 lBufferId;
    uint8 sdu_temp[CANIF_PRV_MAX_BUFFER_SIZE] = {0};
    Can_PduType Pdu_temp = {NULL_PTR, 0, 0, 0};
    Can_HwHandleType lHth_uo = 0;
    Can_ReturnType lCanRetVal;
    Std_ReturnType lTxBufferRetVal = E_NOT_OK;
    uint8 lClearIndex;
    uint8 lDataMaxLen;
    Pdu_temp.sdu = &sdu_temp[0];

#endif

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_TX_CONFIRMATION_SID, CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET(CanTxPduId >= CANIF_TOTAL_TXPDUS, CANIF_TX_CONFIRMATION_SID,CANIF_E_PARAM_LPDU)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanTxPduId];

    /* If CanTxPduId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((ltxPduCustId_t == 0xFFFFu), CANIF_TX_CONFIRMATION_SID, CANIF_E_PARAM_LPDU)

    lTxPduConfig_pst   = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;
#if (CANIF_RB_NODE_CALIBRATION == STD_OFF)
    lCtrlConfig_pst    = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanIf_CtrlConfigPtr;
#else
    lCalibCtrlId = CanIf_CanCtrl_CanIfCtrl_MappingTable_acst[lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->getCtrlCfg()];
    lCtrlConfig_pst = &(CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr[lCalibCtrlId]);
#endif
    lControllerId_u8   = lCtrlConfig_pst->CtrlId;

#if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
    /* Store custId for the requested controller for the selected variant */
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[lControllerId_u8];

    /* Get configured data of requested controller */
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;

    /* Get the CtrlDrv information mapped to the requested Pdu */
    lCtrlDrvConfig_pst = CanIf_Prv_ConfigSet_tpst->CanCtrlDrvCont_Ptr +(lCtrlConfig_pst->CanDrvIndx);
#endif

    /*Get the current status of the linked controller */
    lControllerState_p = CanIf_Prv_ControllerState_ast + lControllerId_u8;


#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
    lTxNotifPtr = CanIf_Prv_TxNotification_aen;
#endif

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
    /*Identify the Buffer for this HTH*/
    lCanIf_TxBufferConfig_p = lTxPduConfig_pst->CanIf_TxBufferConfigPtr;
    lDataBuf_p                 = lCanIf_TxBufferConfig_p->DataBuf;
    lCanIdBuf_p                  = lCanIf_TxBufferConfig_p->CanIdBuf;
    lDataMaxLen                  = lCanIf_TxBufferConfig_p->CanIfBufferMaxDataLength;
    lBufferId                = (uint32)lCanIf_TxBufferConfig_p->CanIfBufferId;
    lHth_uo                   = lCanIf_TxBufferConfig_p->CanIf_HthConfigPtr->CanObjectId;
#endif


#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)


    /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/

        lControllerState_p->BlPnTxFilter = FALSE;

#endif


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)
/*Update the TxCnfmStatus  if controller is in STARTED mode*/
if( ((lControllerState_p->CanIf_TxCnfmStatus) != CANIF_TX_RX_NOTIFICATION) && ((((lControllerState_p->Ctrl_Pdu_mode)& CANIF_BIT_MASK_PDU_MODE)>>CANIF_Ctrl_BIT_SHIFT) == (uint8)CANIF_CS_STARTED))
{
    /* Set Tx Confirmation state for the controller*/
    lControllerState_p->CanIf_TxCnfmStatus = CANIF_TX_RX_NOTIFICATION;
}
#endif

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)

    if (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  != CANIF_TXBUFFER_EMPTY)            /*Buffer is NOT empty*/
    {

        /* Enter Critical Section */
        SchM_Enter_CanIf_TxBufAccessNoNest();               /*RR*/

        /*Read the PDU having highest priority*/
        lTxBufferRetVal = CanIf_Prv_ReadTxBuffer(lCanIf_TxBufferConfig_p, &Pdu_temp);

        /*  to clear data from buffer */
        /*[SWS_CANIF_00183]*/

                /* Clear the read data from DataBuf and mark the index as free*/
                lClearIndex = (CanIf_Prv_TxBufferRam_ast[lBufferId].last_index);
               *(lDataBuf_p + ((lCanIdBuf_p[lClearIndex].BufferIndex)*(lDataMaxLen+1)) + 0)    = 0xFF;       /*Index is available*/


                 CanIf_Prv_TxPduRam_ast[(lCanIdBuf_p + lClearIndex)->swPduHandle].pdu_buffered_flag = FALSE;
                 (lCanIdBuf_p + lClearIndex)->BufferIndex = 0;
                  (lCanIdBuf_p + lClearIndex)->CanId = 0;
                  (lCanIdBuf_p + lClearIndex)->SduLength = 0;
                 (lCanIdBuf_p + lClearIndex)->swPduHandle = 0;

                 (CanIf_Prv_TxBufferRam_ast[lBufferId].last_index) = (lClearIndex - 1)&0xFFu;
                 if((CanIf_Prv_TxBufferRam_ast[lBufferId].last_index) == 0xFF)  /*All elements are read from the buffer*/
                 {
                     (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus) = CANIF_TXBUFFER_EMPTY;
                 }
                 else if(CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus == CANIF_TXBUFFER_FULL)
                 {
                     (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus) = CANIF_TXBUFFER_READY;
                 }
                 else
                 {
                     /*Do nothing*/
                 }

        /* Leave Critical Section */
        SchM_Exit_CanIf_TxBufAccessNoNest();

        if(lTxBufferRetVal == E_OK)
        {
            /*Call CanIf_Write_Integration() for the pdu*/
        #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
            lCanRetVal = lCtrlDrvConfig_pst->CanWrite(lHth_uo,&Pdu_temp);
        #else
            /* BSW-9662 */ /* [$DD_BSWCODE 17054] */
            lCanRetVal = CanIf_Write_Integration(lHth_uo, &Pdu_temp);
        #endif

            if((lCanRetVal == CAN_BUSY))
            {
       /*If transmit buffering is enabled, CanIf will store a Tx L-PDU in a CanIf Transmit
        L-PDU Buffer (CanIfTxBuffer), if it is rejected by CanDrv at Transmit Request.*/
          #if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
           /*  CanIfTxBuffer is configured with a buffer size greater than zero    */
             if((lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIfBufferSize) > 0)
                {
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();               /*RR*/
                lCanIf_TxPduId_temp = Pdu_temp.swPduHandle;
                /*[SWS_CANIF_00183]*/
                (void)CanIf_Prv_WriteTxBuffer(lCanIf_TxPduId_temp, Pdu_temp);
                 /* Leave Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
            }
           #endif
          }
        }
    }
#endif

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)

    /* Check if the Notification status can be read */
if( FALSE != lTxPduConfig_pst->TxPduReadNotifyStatus )
{

    /* Update the notification status */
    *(lTxNotifPtr + CanTxPduId) = CANIF_TX_RX_NOTIFICATION;

}
#endif
/* Indication to UL about confirmation of data transmission */
if((NULL_PTR != lTxPduConfig_pst->UserTxConfirmation) && (((lControllerState_p->Ctrl_Pdu_mode)& CANIF_BIT_MASK_CTRL_MODE) == (uint8)CANIF_ONLINE))
{

    if ((lTxPduConfig_pst->TxPduTxUserUL == PDUR ) || (lTxPduConfig_pst->TxPduTxUserUL == USER ) || (CANIF_XCORE_CFG_ENABLED == STD_OFF))
    {
        (void)CanIf_XCore_LocalCore_TxConfirmation(lTxPduConfig_pst);
    }
    else
    {
		/* FC_VariationPoint_START */
        #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
        llocalCoreId_u8 = Schm_CanIf_XCore_GetCurrentCoreId();
        ldestCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[lTxPduConfig_pst->TxPduTxUserUL];
        lpipeId_u16 = CanIf_XCore_Config_pcst->CanIf_XCoreTxConfirmationPipeMatrix[llocalCoreId_u8][ldestCoreId_u8];
        pduInfoDummy_st.SduDataPtr = NULL_PTR;
        pduInfoDummy_st.SduLength = 0;

        lpipeParams_st.userType_e = lTxPduConfig_pst->TxPduTxUserUL;
        lpipeParams_st.PduId = CanTxPduId;
        lpipeParams_st.PduInfoPtr = &pduInfoDummy_st;
        lpipeParams_st.ControllerId_u8 = lControllerId_u8;

        if(lpipeId_u16 == CANIF_XCORE_INVALID_PIPE_ID)
        {
            (void)CanIf_XCore_LocalCore_TxConfirmation(lTxPduConfig_pst);
        }
        else
        {
            (void)CanIf_XCore_PipeWrite(lpipeId_u16, &lpipeParams_st);
        }
        #endif
		/* FC_VariationPoint_END */
    }
}
#endif/*End of #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)*/
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
Std_ReturnType CanIf_XCore_LocalCore_TxConfirmation(const CanIf_Cfg_TxPduConfig_tst * CanIf_TxPduConfig_pst)
{
    Std_ReturnType lretval = E_OK;
    /* Pointer to Tx Pdu configuration */
    const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;

    /* If Ptr is NULL, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_TxPduConfig_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

    lTxPduConfig_pst = CanIf_TxPduConfig_pst;

    lTxPduConfig_pst->UserTxConfirmation(lTxPduConfig_pst->TxPduTargetPduId);

    return lretval;
}
#endif/*End of #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
