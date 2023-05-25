

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/
#ifdef COM_RX_IPDUCOUNTER
LOCAL_INLINE boolean Com_Prv_ProcessRxIPduCounter(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst);
#endif


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/
/*
 **********************************************************************************************************************
 Function name    : Com_RxIndication
 Description      : Service called by the lower layer after an I-PDU has been received.
 Parameter        : idRxPdu_uo,pduInfoPtr_pcst
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_RxIndication(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_RxIndication, COM_E_UNINIT);
    }
    else if (pduInfoPtr_pcst == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_RxIndication, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxIpduId(idRxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_RxIndication, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idRxPdu_uo              = COM_GET_RX_IPDU_ID(idRxPdu_uo);

        rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);
        rxIpduRamPtr_pst        = &COM_GET_RXPDURAM_S(idRxPdu_uo);

        /* Check if the Ipdu group containing this IPDU is started */
        if(Com_Prv_CheckRxIPduStatus(idRxPdu_uo))
        {
            boolean sigProcessing_b;

            sigProcessing_b = Com_GetValue(RXIPDU,_SIGPROC,rxIpduConstPtr_pcst->rxIPduFields_u8);

            /* Long lock: To avoid mulitple short locks. If ComEnablePduBasedLocks parameter is enabled,
             * then long lock will be enabled, otherwise short lock will be enabled */
            Com_Get_SchM_Enter_Com_RxIndication
            Com_Get_SchM_Enter_Com_RxPduBuffer

            /* The AUTOSAR COM module does not copy or handle additional received data for not configured signals in
             * case the received data length is greater than expected. */
            rxIpduRamPtr_pst->rxIPduLength_uo = ((pduInfoPtr_pcst->SduLength > COM_GET_RXIPDU_SIZE(idRxPdu_uo)) ?
                                          (COM_GET_RXIPDU_SIZE(idRxPdu_uo)) : (pduInfoPtr_pcst->SduLength));

#if defined (COM_RxIPduDeferredProcessing) || defined (COM_COPY_IMMEDIATE_RXIPDU)
#ifndef COM_COPY_IMMEDIATE_RXIPDU
            if (COM_DEFERRED == sigProcessing_b)
#endif
            {
                /* Now Copy the received Data into the RxIPDU buffer */
                Com_ByteCopy(rxIpduConstPtr_pcst->buffPtr_pau8, (uint8*)(pduInfoPtr_pcst->SduDataPtr), rxIpduRamPtr_pst->rxIPduLength_uo);
            }
#endif /* #if defined (COM_RxIPduDeferredProcessing) || defined (COM_COPY_IMMEDIATE_RXIPDU) */

            /* This operation of setting the bit-field is moved inside the lock, to avoid corrupting the bit-fields */
#ifdef COM_ENABLE_MAINFUNCTION_RX
            Com_SetRamValue(RXIPDU,_INDICATION, rxIpduRamPtr_pst->rxFlags_u8, COM_TRUE);
#endif

            Com_Get_SchM_Exit_Com_RxPduBuffer
            Com_Get_SchM_Exit_Com_RxIndication

            if (COM_IMMEDIATE == sigProcessing_b)
            {
#if defined(COM_RxIPduCallOuts) || defined(COM_RX_IPDUCOUNTER)

                /* In case callout/counter mismatch occurs, IPDU can be discarded */
                if (Com_Prv_IsValidRxIpdu(idRxPdu_uo, pduInfoPtr_pcst))
#endif
                {
                    Com_Prv_ProcessRxIPdu(idRxPdu_uo, pduInfoPtr_pcst);
                }
            }
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxIPdu
 Description      : Service called by the lower layer after an I-PDU has been received.
 Parameter        : idRxPdu_uo    - Id of the Rx ipdu
                  : pduInfoPtr_pcst - Pointer to the buffer + sdu length where the received data is stored
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_ProcessRxIPdu(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    boolean                     isGwRxIpdu_b;
#endif

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);
#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)
    rxIpduRamPtr_pst   = &COM_GET_RXPDURAM_S(idRxPdu_uo);

    isGwRxIpdu_b = Com_GetValue(RXIPDU,_ISGWPDU,rxIpduConstPtr_pcst->rxIPduFields_u8);

    /* Initialize the flag only if the RxIPdu has gateway signals or signal groups */
    if (isGwRxIpdu_b)
    {
        COM_GET_IS_GWSIG_UPDATED(idRxPdu_uo) = COM_FALSE;
    }
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)*/

    /* This lock is required only if ComIPduProcessing is IMMEDIATE, as it could interefere with
     * Com_MainFunctionRx() for time-out related part */
    Com_Get_SchM_Enter_Com_RxIndication

    /* Process all the Signals in the IPdu
     * 1. Update bit validation for signals with Update bit configured
     * 2. Timeout monitoring for Signals with Update bit
     * 3. Data Invalid value validation
     * 4. Validation of filters for Signals with filter configured */
    if (rxIpduConstPtr_pcst->numOfSig_u16 > COM_ZERO)
    {
        Com_Prv_ProcessSignal(idRxPdu_uo, pduInfoPtr_pcst);
    }

#ifdef COM_RX_SIGNALGROUP

    /* Process all the Signal groups in the IPdu
     * 1. Update bit validation for signal groups with Update bit configured
     * 2. Timeout monitoring for signal groups with Update bit
     * 3. Data Invalid value validation for group signals with Invalid value configured */
    if (rxIpduConstPtr_pcst->numOfSigGrp_u16 > COM_ZERO)
    {
        Com_Prv_ProcessSignalGroup(idRxPdu_uo, pduInfoPtr_pcst);
    }
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

    /* This part of code updates the Rx Gateway queue If
     * 1. If the RxIPdu has gateway signals or signal groups
     * 2. Any Gw signal in the Ipdu is updated in the signal buffer
     * 3. Already an entry into the queue does not exist
     */
    if ((isGwRxIpdu_b) && (COM_GET_IS_GWSIG_UPDATED(idRxPdu_uo)))
    {
        /* here lock is required only to protect _GWPDU_TOBEPROCESSED flag of RxFlags.
         * As it is critical to the queue addition */
        Com_Get_SchM_Enter_Com_RxPduBuffer

        if (Com_GetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,rxIpduRamPtr_pst->rxFlags_u8) == COM_FALSE)
        {
            /* There is a new reception of Ipdu/ Ipdu timeout caused a new update on the Signal, Hence set the flag
             * to process in next Com_MainFunctionRouteSignals
             * Once the Ipdu is processed, this flag will hold COM_TRUE in Com_MainFunctionRouteSignals
             * If any new receptions are invoked on the same Ipdu before the next processing is done,
             * 1. Signal buffers are updated
             * 2. Queue is not updated */
            (void)Com_Prv_WriteSigGwReceiveQueue(idRxPdu_uo);

            Com_SetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,rxIpduRamPtr_pst->rxFlags_u8,COM_TRUE);
        }
        Com_Get_SchM_Exit_Com_RxPduBuffer
    }
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#if defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT)
    if((pduInfoPtr_pcst->MetaDataPtr != NULL_PTR) && (rxIpduConstPtr_pcst->rxMetaDataPtr_pcst != NULL_PTR))
    {
        /* Copy the given MetaData into RxMetaData internal buffer */
        Com_ByteCopy( (rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataValue_pau8),
                      (uint8*)(pduInfoPtr_pcst->MetaDataPtr),
                      rxIpduConstPtr_pcst->rxMetaDataPtr_pcst->RxmetaDataLength_u8);
    }
#endif /* #if defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT) */
    Com_Get_SchM_Exit_Com_RxIndication

#if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)

    Com_Prv_InvokeRxNotifications( idRxPdu_uo );

#endif
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#if defined(COM_RxIPduCallOuts) || defined(COM_RX_IPDUCOUNTER)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_IsValidRxIpdu
 Description      : Check for configured IPDU counter or IPDU callout
 Parameter        : idPdu_uo    - Id of the Rx I-PDU
                  : pduInfoPtr_pcst - Pointer to the PduInfo(buffer + length) where the received data is stored
 Return value     : true  - Proceed further to process RxIPdu
                  : false - discard received RxIPdu
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
boolean Com_Prv_IsValidRxIpdu(PduIdType idPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst     rxIpduConstPtr_pcst;
    boolean                 isValidRxIpdu_b;

    isValidRxIpdu_b   = COM_TRUE;

    rxIpduConstPtr_pcst  = COM_GET_RX_IPDU_CONSTDATA(idPdu_uo);

#ifdef COM_RX_IPDUCOUNTER

    /* If IPduCounter is configured */
    if (rxIpduConstPtr_pcst->idxRxIpduCntr_uo != COM_RXIPDU_CNTR_INV_IDX)
    {
        /* If received counter value is not within the expected counter range, then no further processing is done. */
        isValidRxIpdu_b = Com_Prv_ProcessRxIPduCounter(idPdu_uo, pduInfoPtr_pcst);
    }
    else
    {
        /* do nothing: If Ipdu counter is not configured, then proceed further to process Ipdu */
    }

#endif /* End of #ifdef COM_RX_IPDUCOUNTER */

#ifdef COM_RxIPduCallOuts

    /* If IPdu callout is configured for the Pdu, Callout is called,
     * if it returns false, no further processing is done */
    if (
# ifdef COM_RX_IPDUCOUNTER
            (isValidRxIpdu_b) &&
# endif
            (rxIpduConstPtr_pcst->callOut_pfct != NULL_PTR)
       )
    {
        isValidRxIpdu_b = rxIpduConstPtr_pcst->callOut_pfct(COM_GET_RXIPDU_HANDLEID(idPdu_uo), pduInfoPtr_pcst);
    }
    else
    {
        /* If callout is not configured, proceed further to process Ipdu */
    }

#endif /* End of #ifdef COM_RxIPduCallOuts */

    return isValidRxIpdu_b;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* End of #if defined(COM_RX_IPDUCOUNTER) && defined(COM_RxIPduCallOuts) */


#ifdef COM_RX_IPDUCOUNTER
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxIPduCounter
 Description      : Process RxIPdu counter and check if the frame can be processed further
 Parameter        : idRxPdu_uo    - Id of the Rx I-PDU
                  : pduInfoPtr_pcst - Pointer to the PduInfo(buffer + length) where the received data is stored
 Return value     : COM_ACCEPT - Proceed further to process RxIPdu
                  : COM_REJECT - Ignore received RxIPdu
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_ProcessRxIPduCounter(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst             rxIpduConstPtr_pcst;
    Com_RxIpduRam_tpst              rxIpduRamPtr_pst;
    Com_RxIpduCntrCfg_tpcst         rxIpduCntrConstPtr_pcst;
    uint8 *                         counterBytePtr_pu8;
    PduLengthType                   counterByteNo_uo;
    uint16                          counterMaxValue_u16;
    uint8                           rxCounterValue_u8;
    uint8                           counterMinExpValue_u8;
    uint8                           counterMaxExpValue_u8;
    boolean                         counterStatus_b;

    counterStatus_b         = COM_ACCEPT;

    rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);
    rxIpduCntrConstPtr_pcst = COM_GET_RX_IPDU_CNTR_CONSTDATA(rxIpduConstPtr_pcst->idxRxIpduCntr_uo);

    counterByteNo_uo   = (PduLengthType)(rxIpduCntrConstPtr_pcst->bitPos_uo / 8u);

    /* Check if counter byte is received */
    if (counterByteNo_uo < pduInfoPtr_pcst->SduLength)
    {
        rxIpduRamPtr_pst      = &COM_GET_RXPDURAM_S(idRxPdu_uo);

        counterBytePtr_pu8    = (uint8 *)(pduInfoPtr_pcst->SduDataPtr + counterByteNo_uo);

        rxCounterValue_u8 = COM_UNPACK_CNTR_FROM_IPDU_BUFF(
                                                    (*counterBytePtr_pu8),
                                             (uint8)(rxIpduCntrConstPtr_pcst->bitPos_uo % 8u),
                                                     rxIpduCntrConstPtr_pcst->bitSize_u8
                                                          );

        /* Check if this is not the first reception after Ipdu is started */
        if (!(Com_GetRamValue(RXIPDU,_IS_FIRST_RECEPTION,rxIpduRamPtr_pst->rxFlags_u8)))
        {
            /* Calculate 2^BitSize */
            counterMaxValue_u16   = (uint16)COM_GET_BASE_2_POWER_X(rxIpduCntrConstPtr_pcst->bitSize_u8);

            /* Calculate expected counter range based upon last received counter value */
            counterMinExpValue_u8 = COM_GET_CNTR_WRAP_AROUND_VALUE(
                 ((uint16)COM_GET_RXIPDU_LAST_CNTR_VAL(rxIpduConstPtr_pcst->idxRxIpduCntr_uo) + COM_ONE), counterMaxValue_u16
                                                                  );
            counterMaxExpValue_u8 = COM_GET_CNTR_WRAP_AROUND_VALUE(
                 ((uint16)counterMinExpValue_u8 + (uint16)rxIpduCntrConstPtr_pcst->threshold_u8), counterMaxValue_u16
                                                                  );

            /* TRACE[SWS_Com_00590] At reception of an I-PDU containing an I-PDU counter, the AUTOSAR COM module
             * shall discard the I-PDU, after setting the next expected value as defined in SWS_Com_00588,
             * if no I-PDU replication for this I-PDU is configured and:
             * received I-PDU counter < expected I-PDU counter OR
             * received I-PDU counter > expected I-PDU counter + ComIPduCounterThreshold
             * with respect to counter wrap-around.
             */
            if (counterMinExpValue_u8 <= counterMaxExpValue_u8)
            {
                /* Check if received counter is within the expected range */
                if (!((rxCounterValue_u8 < counterMinExpValue_u8) || (rxCounterValue_u8 > counterMaxExpValue_u8)))
                {
                    /* Accept RxIPdu */
                }
                else
                {
                    /* Reject RxIPdu */
                    counterStatus_b = COM_REJECT;
                }
            }
            /* (counterMinExpValue_u8 > counterMaxExpValue_u8) */
            /* Check if received counter is within the expected range */
            else if (!((rxCounterValue_u8 < counterMinExpValue_u8) && (rxCounterValue_u8 > counterMaxExpValue_u8)))
            {
                /* Accept RxIPdu */
            }
            else
            {
                /* Reject RxIPdu */
                counterStatus_b = COM_REJECT;
            }

            /* TRACE[SWS_Com_00727] In case a mismatch of the expected and the received I-PDU counter is detected,
             * the AUTOSAR COM module shall notify this mismatch independently of the configured Threshold. */
            if ((rxCounterValue_u8 != counterMinExpValue_u8) && (rxIpduCntrConstPtr_pcst->counterErrNotification_pfct != NULL_PTR))
            {
                rxIpduCntrConstPtr_pcst->counterErrNotification_pfct(COM_GET_RXIPDU_HANDLEID(idRxPdu_uo), counterMinExpValue_u8, rxCounterValue_u8);
            }
            else
            {
                /* do nothing: error notification is not configured */
            }
        }
        else
        {
            /* TRACE[SWS_Com_00587] For all I-PDUs with ComIPduDirection configured to RECEIVE that have a configured
             * ComIPduCounter, the AUTOSAR COM module shall accept any incoming I-PDU, regardless of the value of
             * the I-PDU counter, after the I-PDU was initialized by Com_Init or reinitialized by Com_IpduGroupControl.
             */
            Com_SetRamValue(RXIPDU,_IS_FIRST_RECEPTION,rxIpduRamPtr_pst->rxFlags_u8,COM_FALSE);
        }

        /* TRACE[SWS_Com_00588] At reception of an I-PDU containing an I-PDU counter, the AUTOSAR COM module shall
         * set the next expected value to the value following the received value with respect
         * to counter wrap-around. */
        /* DesignDecision: In our implementation only last received counter is stored, next expected value is
         * calculated in the next reception. */
        COM_GET_RXIPDU_LAST_CNTR_VAL(rxIpduConstPtr_pcst->idxRxIpduCntr_uo) = rxCounterValue_u8;
    }
    else
    {
        /* Accept RxIPdu.
         * DesignDecision: But expected counter value is unchanged, in case counter byte is not received */
    }

    return counterStatus_b;

} /* End of Com_Prv_ProcessRxIPduCounter */

#endif /* #ifdef COM_RX_IPDUCOUNTER */


#if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InvokeRxNotifications
 Description      : Invoke notifications configured at RxIPdu, Signal, SignalGroup
 Parameter        : idRxPdu_uo - Rx-ComIPdu ID
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_Prv_InvokeRxNotifications(PduIdType idRxPdu_uo)
{
    Com_RxIpduCfg_tpcst             rxIpduConstPtr_pcst;
#ifdef COM_RxSignalNotify
    Com_RxSigCfg_tpcst              rxSigConstPtr_pcst;
    Com_RxSigRam_tpst               rxSigRamPtr_pst;
#endif
#ifdef COM_RxSignalGrpNotify
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
    Com_RxSigGrpRam_tpst            rxSigGrpRamPtr_pst;
#endif
#if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)
    uint16_least                    index_qu16;
#endif

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);

#ifdef COM_RxIPduNotification

    /* Ipdu notification is called for the Ipdu */
    if (rxIpduConstPtr_pcst->notification_pfct != NULL_PTR)
    {
        rxIpduConstPtr_pcst->notification_pfct();
    }
#endif

#if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)

    /* Check if any signal notification callbacks are configured for the IPdu */
    if (Com_GetValue(RXIPDU,_NOTIFCBK,rxIpduConstPtr_pcst->rxIPduFields_u8))
    {

#ifdef COM_RxSignalNotify

        rxSigConstPtr_pcst = COM_GET_RXSIG_CONSTDATA(rxIpduConstPtr_pcst->idRxSig_uo);
        rxSigRamPtr_pst   = &COM_GET_RXSIGNALFLAG(rxIpduConstPtr_pcst->idRxSig_uo);

        /* Signal notifications are called for all the signals with notifications configured */
        for (index_qu16 = rxIpduConstPtr_pcst->numOfSig_u16; index_qu16 != COM_ZERO; index_qu16--)
        {
            if (rxSigConstPtr_pcst->notification_pfct != NULL_PTR)
            {
                if (Com_GetRamValue(RXSIG,_SIGNOTIF,rxSigRamPtr_pst->rxSigRAMFields_u8))
                {
                    Com_SetRamValue(RXSIG,_SIGNOTIF,rxSigRamPtr_pst->rxSigRAMFields_u8,COM_FALSE);
                    rxSigConstPtr_pcst->notification_pfct();
                }
            }
            rxSigConstPtr_pcst++;
            rxSigRamPtr_pst++;
        }
#endif /* COM_RxSignalNotify */

#ifdef COM_RxSignalGrpNotify

        rxSigGrpConstPtr_pcst   = COM_GET_RXSIGGRP_CONSTDATA(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);
        rxSigGrpRamPtr_pst      = &COM_GET_RXSIGGRPFLAG(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);

        /* Signal group notifications are called for all the signal groups with notifications configured */
        for (index_qu16 = rxIpduConstPtr_pcst->numOfSigGrp_u16; index_qu16 != COM_ZERO; index_qu16--)
        {
            if (rxSigGrpConstPtr_pcst->notification_pfct != NULL_PTR)
            {
                if (Com_GetRamValue(RXSIGGRP,_SIGNOTIF,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8))
                {
                    Com_SetRamValue(RXSIGGRP,_SIGNOTIF,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8,COM_FALSE);
                    rxSigGrpConstPtr_pcst->notification_pfct();
                }
            }
            rxSigGrpConstPtr_pcst++;
            rxSigGrpRamPtr_pst++;
        }
#endif /* #ifdef COM_RxSignalGrpNotify */

    } /* if (Com_GetValue(RXIPDU,_NOTIFCBK,RxIpduConstPtr->rxIPduFields)) */

#endif /* #if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify) */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify) */


/* FC_VariationPoint_START */
/*
 **********************************************************************************************************************

 Function name    : Com_ReadRxIPduLength
 Description      : Service for Reading the length of the received IPdu
                    The API returns COM_SERVICE_NOT_AVAILABLE in case the RxPduId is not in range or
                    the corresponding IPdu Group is not started and updates the RxIPdu length with Zero.
                    In normal case it returns E_OK by updating the RxIPdu length.
 Parameter        : idRxPdu_uo         - ID of the reception IPDU
                    rxIPduLengthPtr_puo - memory location to update the PduLength for the requested RxIPDU
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#ifdef COM_ENABLE_READRXIPDULENGTH
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_ReadRxIPduLength(PduIdType idRxPdu_uo, PduLengthType * rxIPduLengthPtr_puo)
{
    uint8       status_u8;

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReadRxIPduLength, COM_E_UNINIT);
    }
    else if (rxIPduLengthPtr_puo == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReadRxIPduLength, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxIpduId(idRxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReadRxIPduLength, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idRxPdu_uo  = COM_GET_RX_IPDU_ID(idRxPdu_uo);

        /* Check if Ipdu Group is started */
        if(Com_Prv_CheckRxIPduStatus(idRxPdu_uo))
        {
            *rxIPduLengthPtr_puo = COM_GET_RXPDURAM_S(idRxPdu_uo).rxIPduLength_uo;
            status_u8 = E_OK;
        }
        else
        {
            /* This part will be executed when IPDU Group is Stopped (Zero is returned as IPdu length) */
            *rxIPduLengthPtr_puo = COM_ZERO;
        }
    }

    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_ENABLE_READRXIPDULENGTH */
/* FC_VariationPoint_END */

