
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Uds_Inf.h"
#include "Rte_Dcm.h"

#if ( ( DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && ( DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF ) )

#define DCM_DSP_CC_COMMTYPEMSK  0x0Fu        /* Mask to remove the higher nibble in Communication type */
#define DCM_DSP_CC_REQLEN       0x02u        /* Request length of Communication control service */
#define DCM_DSP_CC_RESLEN       0x01u        /* Response length of Communication control service */
#define DCM_DSP_CC_POSCTRLTYPE  0x00u        /*Index to get the sub-function from the request */
#define DCM_DSP_CC_POSCOMMTYPE  0x01u        /*Index to get the Communication type from the request */
#define DCM_DSP_CC_POSSUBNODE   0x02u        /*Index to get the Node Identification Number from the request */
#define DCM_DSP_CC_SUBNETMASK   0xF0u        /* Mask to remove the lower nibble in Communication Type */
#define DCM_SUBNET_DISEN_SPECIFIED_COMTYPE 0x0 /* */
#define DCM_SUBNET_DISEN_REQUEST_RECIEVED_NODE 0xF/* */
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* Variable to store the communication mode */
static Dcm_CommunicationModeType Dcm_stCommunicationMode_u8 ;
/* Variable to store the index to ComM structure within Dcm */
static uint8 Dcm_idxCommChannel_u8 ;
/* Variable to store the current state of the Communication control state machine */
static NetworkHandleType Dcm_nrSubNet_u8;      /* Static variable to store the subnet number */
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 ***********************************************************************************************************
 *  Dcm_Prv_CheckNodeId : This function is used to check whether the nodeIdentification-Number received
 *  in the request message is configured in atleast one of the DcmDspComControlSubNodes.
 *
 * \param       dataSubNodeID_u8 : Requested node id
 *              dataNegRespCode_u8 : Negative Response Code, to be filled if id is not found
 *
 * \retval      dataReturnVal_u8 : Sub node index of the requested node id
 * \seealso
 *
 ***********************************************************************************************************
 */
#if (DCM_CFG_NUM_SUB_NODES != 0)
static uint16 Dcm_Prv_CheckNodeId(uint16 dataSubNodeID_u8, Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint8 idxIndex_u8;
    uint8 dataReturnVal_u8;

    /* Loop through all nodes to fetch the sub node index of the requested node id */
    for(idxIndex_u8 =0; idxIndex_u8 < DCM_CFG_NUM_SUB_NODES; idxIndex_u8++)
    {
        if((Dcm_SubNode_table[idxIndex_u8].SubNodeId_u8 == dataSubNodeID_u8) &&
                (Dcm_SubNode_table[idxIndex_u8].SubNodeUsed_b))
        {
            /* Requested Node Id is available in the list of supported NodeIds */
            break;
        }
    }
    if (idxIndex_u8 < DCM_CFG_NUM_SUB_NODES)
    {
        /* If node id was found, return the valid index of the sub node */
        dataReturnVal_u8 = idxIndex_u8;
    }
    else
    {
        /* [SWS_Dcm_01078] : If node id was not found, send NRC 0x31 */
        dataReturnVal_u8 = DCM_CFG_NUM_SUB_NODES;
        /*Set Negative response code */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
        /* Optional: Report development error "DCM_E_SUBNODE_NOT_SUPPORTED " to DET module */
        DCM_DET_ERROR(DCM_CC_ID , DCM_E_SUBNODE_NOT_SUPPORTED )

    }
    return(dataReturnVal_u8);
}
#endif

/**
 ***********************************************************************************************************
 *  Dcm_GetCommModeType_u8 :The function is used to derive the communication mode from the sub-function
 *  byte and the communication type (both from tester request)
 *
 *  The array adrComModeIndex_au8 gives the starting index to the array CommModeType for each of the valid
 *  communication type.The first element of adrComModeIndex_au8 is not valid . The second element
 *  gives the starting index to the array CommModeType corresponding to CommunicationType 0x01 (Normal
 *  Communication)and so on.The third element gives the starting index corresponding to 0x02(NM communication)
 *  and so on.The index obtained thus from the communication type (parameter of the function) is added
 *  with the control type(parameter of the function) to get the index of the CommunicationModeType from the
 *  array CommModeType corresponding to the control type and communication type passed to the function and this
 *  is returned back
 *
 *
 * \param           dataCommType_u8  : Communication type from the tester request
 *                  ControlType_u8 : COntrol type from tester request
 *
 * \retval          Dcm_CommunicationModeType
 * \seealso
 *
 ***********************************************************************************************************
 */
static Dcm_CommunicationModeType Dcm_Prv_GetCommModeType_u8(uint8 ControlType_u8,
        uint8 dataCommType_u8 )
{
    /* Array with index to the CommunicationModeType array for each communication type */
    uint8 adrComModeIndex_au8[4]={0,0,6,12};

    /* Array of communication mode types */
    Dcm_CommunicationModeType CommModeType[18] =
    {
            DCM_ENABLE_RX_TX_NORM,
            DCM_ENABLE_RX_DISABLE_TX_NORM,
            DCM_DISABLE_RX_ENABLE_TX_NORM,
            DCM_DISABLE_RX_TX_NORMAL,
            DCM_ENABLE_RX_DISABLE_TX_NORM,
            DCM_ENABLE_RX_TX_NORM,
            DCM_ENABLE_RX_TX_NM,
            DCM_ENABLE_RX_DISABLE_TX_NM,
            DCM_DISABLE_RX_ENABLE_TX_NM,
            DCM_DISABLE_RX_TX_NM,
            DCM_ENABLE_RX_DISABLE_TX_NM,
            DCM_ENABLE_RX_TX_NM,
            DCM_ENABLE_RX_TX_NORM_NM,
            DCM_ENABLE_RX_DISABLE_TX_NORM_NM,
            DCM_DISABLE_RX_ENABLE_TX_NORM_NM,
            DCM_DISABLE_RX_TX_NORM_NM,
            DCM_ENABLE_RX_DISABLE_TX_NORM_NM,
            DCM_ENABLE_RX_TX_NORM_NM
    };

    /* Return the CommunicationModeType corresponding to the control type and the communication type */
    return(CommModeType[adrComModeIndex_au8[dataCommType_u8]+ControlType_u8]);

}

/**
 **************************************************************************************************
 *  Dcm_Dsp_ChkCommunicationModeEntry :The function is used to check if the mode entry notification is for
 *  the requested mode or not
 *
 * \param           CommunicationMode_Cbk_u8  : Mode for which the notification was received
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */

#if ( DCM_CFG_NUM_SPECIFIC_SUBNETS != 0 )
static uint8 Dcm_Prv_DspChkSubnetID_u8(NetworkHandleType dataSubnetID_u8,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint8 idxIndex_u8;
    uint8 dataReturnVal_u8;

    for(idxIndex_u8 =0;idxIndex_u8<DCM_CFG_NUM_SPECIFIC_SUBNETS;idxIndex_u8++)
    {
        if(Dcm_specific_subnet_table[idxIndex_u8].SpecificSubNetId_u8 == dataSubnetID_u8)
        {
            break;
        }
    }
    if (idxIndex_u8 < DCM_CFG_NUM_SPECIFIC_SUBNETS)
    {
        dataReturnVal_u8 = idxIndex_u8;
    }
    else
    {
        dataReturnVal_u8 = DCM_CFG_NUM_SPECIFIC_SUBNETS;
        /*Set Negative response code */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
        /* Report development error "DCM_E_SUBNET_NOT_SUPPORTED " to DET module if the DET module is enabled */
        DCM_DET_ERROR(DCM_CC_ID , DCM_E_SUBNET_NOT_SUPPORTED )

    }
    return(dataReturnVal_u8);
}

#endif

/**
 **************************************************************************************************
 *  Dcm_Prv_CC_Check :This function will check whether communication channel
 *
 *  \param[in]       pMsgContext  Pointer to message structure
 *  \param[out]      dataNegRespCode_u8  :   pointer to update the negative response code
 *  \retval          None
 *  \seealso
 *
 **************************************************************************************************
 */

static Std_ReturnType Dcm_Prv_CC_Check(const Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    uint8 controlType_u8;     /* Local variable to store the control Type */
    uint8 dataCommType_u8;    /* Local variable to get lower nibble of Communication Type */
    uint8 minReqDataLen_u8;   /* Local variable to calculate minimum length the request based on controlType */
#if (DCM_CFG_NUM_SUB_NODES != 0)
    uint16 subNodeId_u16;     /* Local variable to store Node ID from the request */
    uint16 idx_subNode_u16;   /* Local variable to get the index of the Sub Node */
#endif
    Std_ReturnType retVal_u8;

    /* Initalization of local variable */
    minReqDataLen_u8 = 0x00;

    controlType_u8 = pMsgContext->reqData[DCM_DSP_CC_POSCTRLTYPE];

    if(controlType_u8 < 0x04u)
    {
        /* If the ControlType is 0 to 3, then request length must be 2bytes */
        minReqDataLen_u8 = DCM_DSP_CC_REQLEN;
    }
    else
    {
#if (DCM_CFG_NUM_SUB_NODES != 0)
        /* If the request is received for sub-function 04 or 05, then min length is 4 bytes (excluding SID)
         * because nodeId parameter is only available if the controlType value is set to 04 or 05 */
        minReqDataLen_u8 = 0x04u;
        /* Store Node ID from the request */
        subNodeId_u16 = (uint16)(((uint16)(pMsgContext->reqData[DCM_DSP_CC_POSSUBNODE])) << 8u)+
                (uint16)((uint16)(pMsgContext->reqData[DCM_DSP_CC_POSSUBNODE+1]));
        /* Get the index of the sub node */
        idx_subNode_u16 = Dcm_Prv_CheckNodeId(subNodeId_u16,dataNegRespCode_u8);
#endif
    }

    if (pMsgContext->reqDataLen== minReqDataLen_u8)
    {
        dataCommType_u8 = (pMsgContext->reqData[DCM_DSP_CC_POSCOMMTYPE] & DCM_DSP_CC_COMMTYPEMSK);

        if((dataCommType_u8 > 0x0) && (dataCommType_u8 < 0x4))
        {
            if((controlType_u8 < 4u)
#if (DCM_CFG_NUM_SUB_NODES != 0)
                    || (idx_subNode_u16 < DCM_CFG_NUM_SUB_NODES)
#endif
            )
            {
                retVal_u8 = DcmAppl_DcmCommControlConditionCheck(controlType_u8, dataCommType_u8,
                        &pMsgContext->reqData[DCM_DSP_CC_POSCTRLTYPE],((uint16)(pMsgContext->reqDataLen)),dataNegRespCode_u8);
                if(E_OK == retVal_u8)
                {
                    *dataNegRespCode_u8=0x00;
                    /* Call the API to get the communication mode from the control type and communication type */
                    Dcm_stCommunicationMode_u8 = Dcm_Prv_GetCommModeType_u8(controlType_u8,dataCommType_u8);

                    if(controlType_u8 < 4u)
                    {
                        /* Get the subnet id from the request */
                        Dcm_nrSubNet_u8 = (uint8)(pMsgContext->reqData[DCM_DSP_CC_POSCOMMTYPE] & (uint8)DCM_DSP_CC_SUBNETMASK);
                        Dcm_nrSubNet_u8 =  Dcm_nrSubNet_u8>>4u;
                        /* Check if Bit 7 to Bit 4 are set */
                        switch (Dcm_nrSubNet_u8)
                        {
                            case DCM_SUBNET_DISEN_REQUEST_RECIEVED_NODE:
                                /* All bits from Bit 4 to Bit 7 are set, thus we need to handle this for the Active ComM Channel */
                                /* Set the subnet number to the Rx channel id of the received Pdu Id */
                                Dcm_idxCommChannel_u8 = Dcm_active_commode_e[Dcm_DsldConnTable_pcst[Dcm_DsldGlobal_st.nrActiveConn_u8].channel_idx_u8].ComMChannelId;
                                break;

                            case DCM_SUBNET_DISEN_SPECIFIED_COMTYPE:
                                /* SubNet is zero */\
                                break;

                            default :
#if ( DCM_CFG_NUM_SPECIFIC_SUBNETS != 0 )
                                /* SubNet is between 0x1 and 0xE */
                                Dcm_idxCommChannel_u8 = Dcm_Prv_DspChkSubnetID_u8(Dcm_nrSubNet_u8,dataNegRespCode_u8);
                                retVal_u8 = ((*dataNegRespCode_u8==DCM_E_REQUESTOUTOFRANGE) ? E_NOT_OK : E_OK);
#else
                                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                                retVal_u8 = E_NOT_OK;
                                /* Report development error "DCM_E_SUBNET_NOT_SUPPORTED " to DET module if the DET module is enabled */
                                DCM_DET_ERROR(DCM_CC_ID , DCM_E_SUBNET_NOT_SUPPORTED )

#endif
                                break;
                        }
                    }
                    else
                    {
#if (DCM_CFG_NUM_SUB_NODES != 0)
                            /* Get the index of the channel referred in the sub node */
                            Dcm_idxCommChannel_u8 = Dcm_SubNode_table[idx_subNode_u16].SubNodeComMChannelId;
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
                            BswM_Dcm_CommunicationMode_CurrentState(Dcm_idxCommChannel_u8,Dcm_stCommunicationMode_u8);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
                            (void)((*(Dcm_SubNode_table[idx_subNode_u16].switch_fp))(Dcm_stCommunicationMode_u8));
#endif
                            DcmAppl_DcmSwitchCommunicationControl(Dcm_idxCommChannel_u8,Dcm_stCommunicationMode_u8);
#endif
                    }
                }
                else if(retVal_u8 == DCM_E_PENDING)
                {
                    /* Application need more time to validate the conditions, then do nothing wait for next main cycle */
                    *dataNegRespCode_u8 = 0x00;
                }
                else
                {
                    if (0x00 == *dataNegRespCode_u8)
                    {
                        /*Set NRC ConditionsNotCorrect*/
                        *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
                    }
                }
            }
            else
            {
                /*Set Negative response code */
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                retVal_u8 = E_NOT_OK;
            }
        }
        else
        {
            /*Set Negative response code */
            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
            retVal_u8 = E_NOT_OK;
        }
    }
    else
    {
        *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        retVal_u8 = E_NOT_OK;
    }
    return retVal_u8;
}


/**
 **************************************************************************************************
 *  Dcm_DcmCommunicationControl :The purpose of Communication Control service is to switch on/off the
 *  transmission and/or the reception of certain messages of one or many servers (e.g. application
 *  communication messages). For example: To decrease Flash programming times, it shall be possible
 *  to disable the normal bus communication by means of the Communication Control diagnostic service.
 *  On reception of this service (SID 0x28) DSD will invoke Dcm_DcmCommunicationControl function.
 *  Communication control service is allowed only in non default session and suppresses positive response
 *  is supported by this service.
 *
 * \param           pMsgContext    Pointer to message structure
 *
 * \retval          None
 * \seealso
 *
 **************************************************************************************************
 */

Std_ReturnType Dcm_DcmCommunicationControl (Dcm_SrvOpStatusType OpStatus,
        Dcm_MsgContextType * pMsgContext,
        Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{

    Std_ReturnType retVal_u8;
	Std_ReturnType ComChkRetVal_u8 = E_NOT_OK; /* To store communication control check return value*/
    *dataNegRespCode_u8 = 0x00;
    retVal_u8 = E_NOT_OK;
    /*Opstatus possed by the central statemachine is unused currently, opstatus variables related to service needs to be
     * removed and the one passed by the central statemachine needs to be used, this would be taken care with task XXXXX*/

    if(OpStatus == DCM_CANCEL)
    {

        retVal_u8 = E_OK;
    }
    else
    {
        ComChkRetVal_u8 = Dcm_Prv_CC_Check(pMsgContext,dataNegRespCode_u8);
        if (E_OK == ComChkRetVal_u8)
        {
            retVal_u8 = E_OK;
            /* Set length of response */
            pMsgContext->resDataLen   = DCM_DSP_CC_RESLEN;
            /* Fill the sub function into the response */
            pMsgContext->resData[DCM_DSP_CC_POSCTRLTYPE] = pMsgContext->reqData[DCM_DSP_CC_POSCTRLTYPE];
        }
        else if(DCM_E_PENDING == ComChkRetVal_u8)
        {
			/* Service needs more time to validate the communication control checks, So to continue with the same in next main cycle
			   update the retVal with Pending */
            retVal_u8 = DCM_E_PENDING;
        }
        else
        {
            retVal_u8 = E_NOT_OK;
        }

    }
    return retVal_u8;
}

/**
 **************************************************************************************************
 * Dcm_DspCommunicationControlSwitchMode : The function will inform the BswM and the SchM/ application
 * regarding the change of mode for CommunicationControl Modes owned by Dcm
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
static void Dcm_Prv_DspCommunicationControlSwitchMode(void)
{
    uint8 idxIndex_u8;
    if ( Dcm_nrSubNet_u8 == 0)
    {
        for(idxIndex_u8=0;idxIndex_u8 < DCM_CFG_NUM_ALLCHANNELS_MODE_INFO;idxIndex_u8++)
        {
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
            BswM_Dcm_CommunicationMode_CurrentState(Dcm_AllChannels_ForModeInfo[idxIndex_u8].AllComMChannelId,
                    Dcm_stCommunicationMode_u8);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
            (void)((*(Dcm_AllChannels_ForModeInfo[idxIndex_u8].switch_fp))(Dcm_stCommunicationMode_u8));
#endif
            DcmAppl_DcmSwitchCommunicationControl(Dcm_AllChannels_ForModeInfo[idxIndex_u8].AllComMChannelId,
                    Dcm_stCommunicationMode_u8);
        }
    }
#if ( DCM_CFG_NUM_SPECIFIC_SUBNETS != 0 )
    else if (( Dcm_nrSubNet_u8 != 0 ) && ( Dcm_nrSubNet_u8 != 0xF ))
    {
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
        BswM_Dcm_CommunicationMode_CurrentState(Dcm_specific_subnet_table[Dcm_idxCommChannel_u8].SpecificComMChannelId,
                Dcm_stCommunicationMode_u8);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
        (void)((*(Dcm_specific_subnet_table[Dcm_idxCommChannel_u8].switch_fp))(Dcm_stCommunicationMode_u8));
#endif
        DcmAppl_DcmSwitchCommunicationControl(Dcm_specific_subnet_table[Dcm_idxCommChannel_u8].SpecificComMChannelId,
                Dcm_stCommunicationMode_u8);
    }
#endif
    else
    {
        /*Subnet ID is 0xF*/
        for(idxIndex_u8=0; idxIndex_u8 < DCM_CFG_NUM_ALLCHANNELS_MODE_INFO;idxIndex_u8++)
        {
            if(Dcm_AllChannels_ForModeInfo[idxIndex_u8].AllComMChannelId == Dcm_idxCommChannel_u8)
            {
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
                BswM_Dcm_CommunicationMode_CurrentState(Dcm_AllChannels_ForModeInfo[idxIndex_u8].AllComMChannelId,
                        Dcm_stCommunicationMode_u8);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
                (void)((*(Dcm_AllChannels_ForModeInfo[idxIndex_u8].switch_fp))(Dcm_stCommunicationMode_u8));
#endif
                DcmAppl_DcmSwitchCommunicationControl(Dcm_AllChannels_ForModeInfo[idxIndex_u8].AllComMChannelId,
                        Dcm_stCommunicationMode_u8);
                break;
            }
        }
    }
}

/**
 *******************************************************************************
 * Dcm_Prv_DspCommCntrlConfirmation : API used for confirmation of response sent
 *                                      for CommunicationControl (0x28) service.
 * \param           dataIdContext_u8    Service Id
 * \param           dataRxPduId_u8      PDU Id on which request is Received
 * \param           dataSourceAddress_u16    Tester Source address id
 * \param           status_u8                Status of Tx confirmation function
 *
 * \retval          None
 * \seealso
 *
 *******************************************************************************
 */
void Dcm_Prv_DspCommCntrlConfirmation(
    Dcm_IdContextType dataIdContext_u8,
    PduIdType dataRxPduId_u8,
    uint16 dataSourceAddress_u16,
    Dcm_ConfirmationStatusType status_u8)
{
    if((status_u8 == DCM_RES_POS_OK)||(status_u8 == DCM_RES_POS_NOT_OK))
    {
        Dcm_Prv_DspCommunicationControlSwitchMode();
    }
    DcmAppl_DcmConfirmation(dataIdContext_u8,dataRxPduId_u8,dataSourceAddress_u16,status_u8);
}
/**
 **************************************************************************************************
 * Dcm_Prv_CCMmodeStatusCheck : The function will check whether the configured mode rule is TRUE .
 * If the mode rule fails, it will reset the mode of the communication channels back to the default mode
 * and inform BswM and SchM/ application regarding the change in mode.
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

static boolean Dcm_Prv_CCMmodeStatusCheck(void)
{

    uint8 dataNRC_u8;
    boolean flgStatus_b;
    Std_ReturnType retVal_u8;
    /*Initialize the flag status value to TRUE*/
    flgStatus_b = TRUE;
    dataNRC_u8 = 0x00;
    /* Call the DcmAppl callback to check for conditions for re-enabling communication control */
    if(Dcm_ComMUserReEnableModeRuleRef !=NULL_PTR)
    {
        retVal_u8 = (*Dcm_ComMUserReEnableModeRuleRef)();
        /* If the mode rule returns any value other than E_OK */
        if((retVal_u8!=E_OK))
        {
            /*Reset the flag status to false*/
            flgStatus_b = FALSE;
        }
    }

#if(DCM_CFG_DSP_MODERULEFORCC!=DCM_CFG_OFF)
    /*If flag status is set and check for conditions for re-enabling communication control*/
    if ((flgStatus_b!=FALSE) && (Dcm_ComMReEnableModeRuleRef != NULL_PTR))
    {
        flgStatus_b = (*Dcm_ComMReEnableModeRuleRef)(&dataNRC_u8);
    }
#endif

    if(dataNRC_u8 != 0u)
    {
        /*Reset the flag status to false*/
        flgStatus_b = FALSE;

    }

    return flgStatus_b;
}

/**
 **************************************************************************************************
 *  Dcm_Prv_ResetCommunicationMode :This function will check whether communication channel is in the default
 *  mode(DCM_ENABLE_RX_TX_NORM_NM) by calling the Appl function / generated function.
 *  If the channel is not in the default mode  then it will call
 *  BswM_Dcm_CommunicationMode_CurrentState and DcmAppl_DcmSwitchCommunicationControl/corresponding
 *  SchM_Switch_Dcm_DcmCommunicationControl_ComMChannel_XXX with  DCM_ENABLE_RX_TX_NORM_NM.
 *
 *  \param           None
 *  \retval          None
 *  \seealso
 *
 **************************************************************************************************
 */
static void Dcm_Prv_ResetCommunicationMode(void)
{
    uint8 idxLoop_u8;
    boolean stStatus_b;

    for(idxLoop_u8=0;idxLoop_u8 < DCM_CFG_NUM_ALLCHANNELS_MODE_INFO;idxLoop_u8++)
    {

        /*Check whether the channel is in Default communication mode ie, DCM_ENABLE_RX_TX_NORM_NM*/
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
        stStatus_b = (*(Dcm_AllChannels_ForModeInfo[idxLoop_u8].checkmode_fp))();
#else
        stStatus_b = DcmAppl_DcmIsComModeDefault(Dcm_AllChannels_ForModeInfo[idxLoop_u8].AllComMChannelId);
#endif
        /*Check if stStatus flag is set to FALSE*/
        if (stStatus_b == FALSE)
        {
            /* Channel is  not in Default communication mode.Call  BswM_Dcm_CommunicationMode_CurrentState and
             * DcmAppl_DcmSwitchCommunicationControl/corresponding
             SchM_Switch_Dcm_DcmCommunicationControl_ComMChannel_XXX with  DCM_ENABLE_RX_TX_NORM_NM */
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
            BswM_Dcm_CommunicationMode_CurrentState(Dcm_AllChannels_ForModeInfo[idxLoop_u8].AllComMChannelId,
                    DCM_ENABLE_RX_TX_NORM_NM);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
            (void)((*(Dcm_AllChannels_ForModeInfo[idxLoop_u8].switch_fp))(DCM_ENABLE_RX_TX_NORM_NM));
#endif
            DcmAppl_DcmSwitchCommunicationControl(Dcm_AllChannels_ForModeInfo[idxLoop_u8].AllComMChannelId,
                    DCM_ENABLE_RX_TX_NORM_NM);
        }
        else
        {
            /*Channel is alredy in the default mode;Do nothing*/
        }
    }
    /*Check the  communication mode of channels associated with specific subnets*/
#if ( DCM_CFG_NUM_SPECIFIC_SUBNETS != 0 )
    for(idxLoop_u8=0;idxLoop_u8 < DCM_CFG_NUM_SPECIFIC_SUBNETS;idxLoop_u8++)
    {

        /*Check whether the channel is in Default communication mode ie, DCM_ENABLE_RX_TX_NORM_NM*/
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
        stStatus_b = (*(Dcm_specific_subnet_table[idxLoop_u8].checkmode_fp))();
#else
        stStatus_b = DcmAppl_DcmIsComModeDefault(Dcm_specific_subnet_table[idxLoop_u8].SpecificComMChannelId);
#endif
        /*Check if stStatus flag is set to FALSE*/
        if (stStatus_b == FALSE)
        {
            /* Channel is  not in Default communication mode.Call  BswM_Dcm_CommunicationMode_CurrentState and
             * DcmAppl_DcmSwitchCommunicationControl/corresponding
               SchM_Switch_Dcm_DcmCommunicationControl_ComMChannel_XXX with  DCM_ENABLE_RX_TX_NORM_NM */
#if (DCM_CFG_DSP_BSWMDCM_ENABLED != DCM_CFG_OFF)
            BswM_Dcm_CommunicationMode_CurrentState(Dcm_specific_subnet_table[idxLoop_u8].SpecificComMChannelId,
                    DCM_ENABLE_RX_TX_NORM_NM);
#endif
#if(DCM_CFG_RTESUPPORT_ENABLED!=DCM_CFG_OFF)
            (void)((*(Dcm_specific_subnet_table[idxLoop_u8].switch_fp))(DCM_ENABLE_RX_TX_NORM_NM));
#endif
            DcmAppl_DcmSwitchCommunicationControl(Dcm_specific_subnet_table[idxLoop_u8].SpecificComMChannelId,
                    DCM_ENABLE_RX_TX_NORM_NM);
        }
        else
        {
            /*Channel is alredy in the default mode;Do nothing*/
        }
    }
#endif
}

/**
 **************************************************************************************************
 *  Dcm_Prv_CC_Mainfunction :This function will check session changes and if transitioning to default session or
 *  upon any diagnostic session change where the new session do not support UDS Service CommunicationControl
 *  it will reset the mode of the communication channels back to the default mode
 *
 *  \param           None
 *  \retval          None
 *  \seealso
 *
 **************************************************************************************************
 */

void Dcm_Prv_CC_Mainfunction (void)
{

    Dcm_SesCtrlType ActiveSession_u8;
    Std_ReturnType retVal_u8;

    boolean CC_Resetmoderuleflg_b;
    boolean CC_Resetsessionflg_b;

    CC_Resetsessionflg_b = TRUE;
    CC_Resetmoderuleflg_b = Dcm_Prv_CCMmodeStatusCheck();

    /* Get the currently active session in the server */
    retVal_u8 = Dcm_GetSesCtrlType(&ActiveSession_u8);

    if((retVal_u8 == E_OK)&&(Dcm_CC_ActiveSession_u8 != ActiveSession_u8))
    {
        // session changed and new session stored
        Dcm_CC_ActiveSession_u8 =  ActiveSession_u8;
        if ( ActiveSession_u8 != DCM_DEFAULT_SESSION )
        {
            CC_Resetsessionflg_b = Dcm_Prv_CC_IsSupportedInSession(ActiveSession_u8);
        }
        else
        {
            CC_Resetsessionflg_b = FALSE;
        }
    }
    if((CC_Resetsessionflg_b == FALSE)||(CC_Resetmoderuleflg_b == FALSE ))
    {
        Dcm_Prv_ResetCommunicationMode();
    }

}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

