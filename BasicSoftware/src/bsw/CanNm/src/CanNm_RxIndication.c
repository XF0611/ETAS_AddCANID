

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"

LOCAL_INLINE void CanNm_ProcessRxPdu(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps,
        const PduInfoType * PduInfoPtr
);

#if (CANNM_CAR_WAKEUP_RX_ENABLED  != STD_OFF)
LOCAL_INLINE void CanNm_CarWakeUpIndication (
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps,
        const PduInfoType * PduInfoPtr
);
#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_RxIndication
 Description      : This is the AUTOSAR interface to get the indication of a received NM message by CAN
                    This function is called by the CanIf after a CAN NM I-PDU has been received.
 Parameter        : PduInfoPtr - Pointer to received data and length
                  : RxPduId - ID of the received I-PDU
 Return value     : None
 ***************************************************************************************************
 */

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

void CanNm_RxIndication( PduIdType RxPduId,
                                           const PduInfoType * PduInfoPtr
                                          )
{

    /* Pointer to Configuration Data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

#if (CANNM_PN_ENABLED != STD_OFF)
    boolean ProcessPdu_b;
    /* local array to received data */
    uint8 data_u8[CANNM_PN_INFOLENGTH];
    const uint8 * PnFilterMask_pcu8;
    uint8 PNIBitStatus_u8;
    uint8_least index_ui;
# if (CANNM_ERACALC_ENABLED != STD_OFF)
    uint8 start_index_status;
# endif
#endif

    /**** End Of Declarations ****/

    /********************************* Start: DET *************************************/

    /* Report DET if RxPduId is Invalid */
    CANNM_DET_REPORT_ERROR((RxPduId >= CANNM_NUMBER_OF_CHANNELS),
            (uint8)RxPduId, CANNM_SID_RXINDICATION, CANNM_E_INVALID_PDUID)

    /* Report DET if CANNM is Uninitialized */
    CANNM_DET_REPORT_ERROR((CanNm_RamData_s[RxPduId].State_en == NM_STATE_UNINIT),
                (uint8)RxPduId, CANNM_SID_RXINDICATION, CANNM_E_NO_INIT)

    /* Report DET if received Pointer is a Null Pointer*/
    CANNM_DET_REPORT_ERROR(((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR)),
            (uint8)RxPduId, CANNM_SID_RXINDICATION, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/
    /* Initialize pointer to RAM structure */
    RamPtr_ps = &CanNm_RamData_s[RxPduId];

    /* Initialize pointer to Config structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(RxPduId);

    /* If PN is enabled, perform corresponding pre-processing of the received IPdu */
#if (CANNM_PN_ENABLED != STD_OFF)
    {
        /* TRACE[SWS_CanNm_00410] : Process the PDU if CanNmAllNmMessagesKeepAwake is true*/
        ProcessPdu_b = TRUE;

        /* Read the PNI status bit from the CBV */
        PNIBitStatus_u8 = (PduInfoPtr->SduDataPtr[ConfigPtr_pcs->ControlBitVectorPos_u8] & CANNM_CBV_PNI_STATUS);

        /* Local copy for the PN Filter Mask array */
       PnFilterMask_pcu8 = CanNm_GlobalConfigData_pcs->PnFilterMask_pcu8;

       /* Filter the recieved PN data with configured filter mask */
       for (index_ui = 0; index_ui < CANNM_PN_INFOLENGTH; index_ui++)
       {
           data_u8[index_ui] = PduInfoPtr->SduDataPtr[CANNM_PN_INFO_OFFSET +index_ui] & PnFilterMask_pcu8[index_ui];
       }

        /* Rx PDU filtering is required only if Pn is activated for the channel
        If AllNmMessagesKeepAwake_b is set, then PDU has to be processed anyways */
        if((!CANNM_GetAllNmMessagesKeepAwake(ConfigPtr_pcs)) && (CANNM_GetPnMsgFilteringStatus(RamPtr_ps)))
        {
            ProcessPdu_b = FALSE;
            /* TRACE[SWS_CanNm_00412] : Process the PN information as PNI bit is set*/
            if(PNIBitStatus_u8 != 0)
            {
                /* Since PNI bit is set, perform RX PDU filtering */
                for (index_ui = 0; index_ui < CANNM_PN_INFOLENGTH; index_ui++)
                {
                    /* Check if any PN , which is relevant for ECU, is requested */
                    if (data_u8[index_ui] != 0)
                    {
                        /* Relevant PN is requested, so process the PDU*/
                        ProcessPdu_b = TRUE;
                    }
                }

            }
            else
            {
                /* TRACE[SWS_CanNm_00411] : Ignore the PDU, as PNI bit is not set */
                ProcessPdu_b = FALSE;

            }
        }

/* ERA and EIRA calculation should be done independent of CanNmAllNmMessagesKeepAwake parameter */
#if (CANNM_EIRACALC_ENABLED != STD_OFF || CANNM_ERACALC_ENABLED != STD_OFF)
        if (CANNM_GetPnEnabledStatus(ConfigPtr_pcs))
         {
            if (PNIBitStatus_u8 != 0)
            {
                /* Loop over all the PN-Info bytes to check for PN's which are relevant to this ECU and
                 * which are requested */
                for (index_ui = 0; index_ui < CANNM_PN_INFOLENGTH; index_ui++)
                {
                    /* Check if any PN , which is relevant for ECU, is requested */
                    if (data_u8[index_ui] != 0)
                    {

# if (CANNM_ERACALC_ENABLED != STD_OFF)
                        start_index_status = CANNM_GetStartIndex_ERAStatus(ConfigPtr_pcs->ChannelOffset_u8);
                        if(CANNM_GetEraEnabledStatus(ConfigPtr_pcs))
                        {
                            CanNm_ERACurrentStatus_au8[start_index_status + index_ui] = CanNm_ERACurrentStatus_au8[start_index_status + index_ui] | data_u8[index_ui];
                        }
# endif
# if (CANNM_EIRACALC_ENABLED != STD_OFF)
                        CanNm_EIRACurrentStatus_au8[index_ui] = CanNm_EIRACurrentStatus_au8[index_ui] | data_u8[index_ui];
# endif
                    }
                }
            }
         }
#endif

        if (ProcessPdu_b != FALSE)
        {
            CanNm_ProcessRxPdu(ConfigPtr_pcs,RamPtr_ps,PduInfoPtr);
        }
    }

#else
    /* TRACE[SWS_CanNm_00409]: PN not enabled, process the received Pdu */
    CanNm_ProcessRxPdu(ConfigPtr_pcs,RamPtr_ps,PduInfoPtr);
#endif /* #ifdef CANNM_PN_ENABLED */

#if (CANNM_CAR_WAKEUP_RX_ENABLED  != STD_OFF)
    if(ConfigPtr_pcs->stCanNmCarWakeUpRxEnabled_b == TRUE)
    {
        CanNm_CarWakeUpIndication(ConfigPtr_pcs,RamPtr_ps,PduInfoPtr);
    }
#endif
    return;
}
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


/***************************************************************************************************
 Function name    : CanNm_ProcessRxPdu
 Description      : This inline function processes the received Pdu.
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
                  : PduInfoPtr - pointer to the received data buffer
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE void CanNm_ProcessRxPdu(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps,
        const PduInfoType * PduInfoPtr
)
{
    /* Pointer to the received Nm message */
    uint8 * SduPtr;

    /* Copy the Node Id */
    if (ConfigPtr_pcs->NodeIdPos_u8 != CANNM_PDU_OFF)
    {
        /*Copy received node id to RxNodeId ram variable*/
        RamPtr_ps->RxNodeId_u8 = PduInfoPtr->SduDataPtr[ConfigPtr_pcs->NodeIdPos_u8];

    }

    /* Copy the Control bit vector */
    if (ConfigPtr_pcs->ControlBitVectorPos_u8 != CANNM_PDU_OFF)
    {
        /*Copy received control bit vector to RxCtrlBitVector_u8 ram variable*/
        RamPtr_ps->RxCtrlBitVector_u8 = PduInfoPtr->SduDataPtr[ConfigPtr_pcs->ControlBitVectorPos_u8];

    }

    /* Calculate the pointer to received Nm message */
    SduPtr = &(PduInfoPtr->SduDataPtr[0]);

    /* Suspend interrupts to provide Data consistency */
    SchM_Enter_CanNm_RxIndicationNoNest();

    /* This is an internal function with predictable run-time; hence doesn't affect interrupt lock */
    CanNm_CopyBuffer(SduPtr,RamPtr_ps->RxBuffer_au8,ConfigPtr_pcs->PduLength_u8);

    /* Enable interrupts */
    SchM_Exit_CanNm_RxIndicationNoNest();

    /* Set the Rx Indication flag */
    RamPtr_ps->RxIndication_b = TRUE;

    /* indication that at least one message is received */
    /* set at this point so that indication is effective only after copying this PDU's data */
    RamPtr_ps->RxStatus_b = TRUE;

    /* Indicate reception to Nm Interface if configured to do so */
#if (CANNM_PDU_RX_INDICATION_ENABLED != STD_OFF)
    Nm_PduRxIndication(ConfigPtr_pcs->NetworkHandle);
#endif


}

#if (CANNM_CAR_WAKEUP_RX_ENABLED  != STD_OFF)
LOCAL_INLINE void CanNm_CarWakeUpIndication (
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps,
        const PduInfoType * PduInfoPtr
        )
{
    /* Local Variable to Store the Status of CarWakeUp bit*/
    uint8 CarWakeUpBit_u8;

    /* Get The Status Of CarWakeUp Bit from the Recently received NM-PDU */

    CarWakeUpBit_u8 = ( (PduInfoPtr->SduDataPtr[ConfigPtr_pcs->stCarWakeUpBytePosition_u8] >> (ConfigPtr_pcs->stCarWakeUpBitPosition_u8)) & (1u) );

    if (CarWakeUpBit_u8 != 0)
    {
        #if (CANNM_CAR_WAKEUP_FILTER_ENABLED != STD_OFF)
            if(ConfigPtr_pcs->stCarWakeUpFilterEnabled_b)
            {
                if(RamPtr_ps->RxNodeId_u8 == ConfigPtr_pcs->stCarWakeUpFilterNodeId_u8)
                {
                   Nm_CarWakeUpIndication(ConfigPtr_pcs->NetworkHandle);
                }
            }
            else
            {
                Nm_CarWakeUpIndication(ConfigPtr_pcs->NetworkHandle);
            }
        #else
           Nm_CarWakeUpIndication(ConfigPtr_pcs->NetworkHandle);
        #endif
    }


}
#endif

