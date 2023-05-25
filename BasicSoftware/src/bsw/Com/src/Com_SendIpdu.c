

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Com_Prv.h"
#include "PduR_Com.h"

#if(COM_ENABLE_INTER_MODULE_CHECKS)
/* [SWS_BSW_00036] Inter Module Checks */
# if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(COM_ENABLE_INTER_MODULE_CHECKS) */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_LoadPeriodicModeInfo(PduIdType idTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st);

LOCAL_INLINE boolean Com_Prv_LoadEventModeInfo(PduIdType idTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st);

LOCAL_INLINE PduLengthType Com_Prv_CalculateTxPduLength(PduIdType idTxPdu_uo);

LOCAL_INLINE void Com_Prv_ProcessTxReqStatus(PduIdType idTxPdu_uo, Std_ReturnType ipduTransStatus_u8);

LOCAL_INLINE void Com_Prv_LoadMinimumDelayTime(Com_TxIpduRam_tpst txIpduRamPtr_pst, Com_TxIpduCfg_tpcst txIpduConstPtr_pcst);

#ifdef COM_TX_IPDUCOUNTER

LOCAL_INLINE void Com_Prv_TxIpduIncrementCounter(PduIdType idTxPdu_uo);
LOCAL_INLINE void Com_Prv_TxIpduDecrementCounter(PduIdType idTxPdu_uo);

#endif /* #ifdef COM_TX_IPDUCOUNTER */


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/

/* Below table details the transmission behavior in combination with Mode change
-----------------------------------------------------------------------------------------------------------------------
    TxMode        ModeChange    Other Cases            Flag value  N - Number of repetitions
-----------------------------------------------------------------------------------------------------------------------
    None            No              NA                    : 00 -- No transmission
    None            Yes             NA                    : 00 -- No transmission
    Periodic        No              NA                    : 00 -- No transmission
    Periodic        Yes             NA                    : 01 -- COM_TRANS_IN_PROC
    Direct          No              N>0                   : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Direct          No              N=0                   : 00 -- COM_RESET
    Direct          Yes             N>0                   : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Direct          Yes             N=0                   : 00 -- COM_RESET
    Mixed           No         Pending signal             : 00 -- No transmission
    Mixed           No         Trig signal,N=0            : 00 -- COM_RESET
    Mixed           No         Trig signal,N>0            : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Mixed           Yes        Pending signal             : 02 -- COM_MIXED_PENDING
    Mixed           Yes        Trig signal,N=0            : 03 -- COM_MIXED_TRIG
    Mixed           Yes        Trig signal,N>0            : 03 -- COM_MIXED_TRIG
    COM_RESET         : Transmission will happen in Com_Prv_SendIpdu immediately
    COM_TRANS_IN_PROC : Transmission will be initiated in Com_MainFunctionTx
    COM_MIXED_PENDING : N-times transmission should be cancelled + periodic Transmission should be started
    COM_MIXED_TRIG    : Periodic transmission should be cancelled + N-times transmission should be initiated.
*/

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_SendIpdu
 Description      : Internal function used for Transmitting Tx Ipdu's
 Parameter        : idTxPdu_uo      - Id of the Tx ipdu
                    sendIpduFlag_st    - flags to decide ipdu transmission
 Return value     : Void
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_SendIpdu(PduIdType idTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIpdu_b;

    sendIpdu_b     = COM_FALSE;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
    txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idTxPdu_uo);

    Com_Get_SchM_Enter_Com_TxIPdu
    Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_TXFLAGS)

    /* This flag is to check if the Call to Com_Prv_SendIpdu happened from
     * 1. Com_SendSignal()
     * 2. Com_SendSignalGroup()
     * 3. Com_SwitchIpduTxMode()
     * 4. Com_TriggerIPduSend()
     * 5. Com_SendDynSignal()
     * 6. Com_TriggerIPDUSendWithMetaData()
     * 7. Com_MainFunctionRouteSignals()
     * 8. Com_SendSignalGroupArray()
     * 9. Com_SendSignalWithMetaData()              if COM_METADATA_SUPPORT, COM_ECUC_AR_RELEASE_AR45 macro are enabled
     * 10.Com_SendSignalGroupWithMetaData()         if COM_METADATA_SUPPORT, COM_TX_SIGNALGROUP and COM_ECUC_AR_RELEASE_AR45
     *                                              macros are enabled
     * 11.Com_SendSignalGroupArrayWithMetaData()    if COM_METADATA_SUPPORT, COM_TX_SIGNALGROUP_ARRAY and COM_ECUC_AR_RELEASE_AR45
     *                                              macros are enabled
     */
    if (sendIpduFlag_st.isEventTrig_u1 != COM_RESET)
    {
        boolean isEventMode_b;

        /* Stop the current RepetitionTicks. As Pdu needs to be triggered immediately either for event transmission or
         * for mode change. It is loaded accordingly in the later checks.
         */
        txIpduRamPtr_pst->cntrRepPeriod_u16 = COM_ZERO;

        /* Check if transmission mode has changed */
        if (sendIpduFlag_st.isModeChangd_u1 != COM_RESET)
        {
            /* Periodic transmisions are always triggered from Com_MainFunctionTx to maintain the periodicity */
            isEventMode_b = Com_Prv_LoadPeriodicModeInfo(idTxPdu_uo, sendIpduFlag_st);
        }
        else
        {
            /* If there is no mode change, then Com_Prv_SendIpdu is called for transmission of event message.
             * N-Repetitions are to be loaded.
             */
            isEventMode_b = COM_TRUE;
        }

        /* This part will be executed, Only for the event transmissions for Ipdus with:
         * 1. Direct Mode
         * 2. Mixed mode with the signal transfer property Triggered
         * In case of mode change for Ipdus with tx mode:
         * 1. Direct mode
         * 2. Mixed mode with the signal triggered transfer property
         */
        if (isEventMode_b)
        {
            sendIpdu_b = Com_Prv_LoadEventModeInfo(idTxPdu_uo, sendIpduFlag_st);
        }
    }
    /* If call is from Com_MainFunctionTx */
    else
    {
        /* MDT checks happen in Com_MainFunctionTx, hence no MDT related checks here. */
        sendIpdu_b = COM_TRUE;

        /* RESET the flag, as this IPdu is triggered from the cyclic task */
        Com_SetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

#ifdef COM_TxIPduTimeOut
        /* In case of periodic transmission from PERIODIC or MIXED mode, reload timeout only if it is not started yet.
         * In case of N-repetitions transmission, it is already reloaded in the call to "Send" api as per below req:
         * The monitoring timer is started upon completion of the call to the SendMessage,
         * SendDynamicMessage or SendZeroMessage API service.
         */
        if((COM_TX_MODE_IS_CYCLIC(Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8))) &&
           (txIpduConstPtr_pcst->timeout_u16 != COM_ZERO))
        {
            if (Com_GetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16) == COM_STOP)
            {
                /* Start the TxConfirmation Timer now i.e set the Timer value to the one configured */
                Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_START);

                txIpduRamPtr_pst->cntrTxTimeout_u16 = txIpduConstPtr_pcst->timeout_u16;
            }
        }
#endif /* #ifdef COM_TxIPduTimeOut */

    } /* if (sendIpduFlag_st.isEventTrig_u1 != COM_RESET) */

    Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_TXFLAGS)

    if (sendIpdu_b)
    {
        PduInfoType         txIPduInfo_st;
        Std_ReturnType      ipduTransStatus_u8;
#ifdef COM_TxIPduCallOuts
        boolean             isTransmitCallout_b;
        boolean             ipduCalloutStatus_b;
#endif
#if defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT)
        Com_TxMetaData_tpcst metaDataInfoPtr_pcst;
#endif
        /* Assign IpduBuffer address and its length to the local variable */
        txIPduInfo_st.SduDataPtr = txIpduConstPtr_pcst->buffPtr_pu8;
        txIPduInfo_st.SduLength  = Com_Prv_CalculateTxPduLength( idTxPdu_uo );

#if defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT)

        Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)

        metaDataInfoPtr_pcst = txIpduConstPtr_pcst->txMetaData_pcst;

        if(Com_GetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16))
        {
            /* Copy the given MetaData into Tx PduInfoType */
            txIPduInfo_st.MetaDataPtr = metaDataInfoPtr_pcst->TxmetaDataValue_pau8;
        }
        else
        {
            /* Copy the given default MetaData into PduInfoType */
            txIPduInfo_st.MetaDataPtr = metaDataInfoPtr_pcst->TxmetaDataDefaultValue_au8;
        }
        /* application metadata is retained and reset flag for next transmissions */
        Com_SetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

        Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)

#endif /* #if defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT) */

#ifdef COM_TxIPduCallOuts
        /* _ISCALLOUTFRMTRIGTRANS flag indicates if the callout has to be invoked from here(false) or
         * from TriggerTransmit(true) */
        isTransmitCallout_b = !(Com_GetValue(TXIPDU,_ISCALLOUTFRMTRIGTRANS,txIpduConstPtr_pcst->txIPduFields_u16));

        /* Continue to call PduR_ComTransmit, if
         * + callout is not configured OR
         * + callout is configured and returns TRUE OR
         * + callout is configured, but it is to be called from Com_TriggerTransmit.
         * If callout is configured and returns FALSE, then dont call PduR_ComTransmit.
         */
        if ((txIpduConstPtr_pcst->callOut_pfct != NULL_PTR) && (isTransmitCallout_b))
        {
            Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)
            ipduCalloutStatus_b = txIpduConstPtr_pcst->callOut_pfct(COM_GET_TXIPDU_HANDLEID(idTxPdu_uo), &txIPduInfo_st);
            Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)
        }
        else
        {
            ipduCalloutStatus_b = COM_TRUE;
        }

        if (ipduCalloutStatus_b)
#endif /* #ifdef COM_TxIPduCallOuts */
        {
#ifdef COM_TX_IPDUCOUNTER

            /* Check if IPduCounter is configured */
            if (txIpduConstPtr_pcst->idxTxIpduCntr_uo != COM_TXIPDU_CNTR_INV_IDX)
            {
                Com_Prv_TxIpduIncrementCounter( idTxPdu_uo );
            }
            else
            {
                /* do nothing: counter is not configured */
            }
#endif /* #ifdef COM_TX_IPDUCOUNTER */

#ifdef COM_MULTICORE_SUPPORT
            Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)
#else
            Com_Get_SchM_Exit_Com_TxIPdu
#endif

            /* Call PduR_ComTransmit() */
            ipduTransStatus_u8 = PduR_ComTransmit(COM_GET_TXIPDU_PDURID(idTxPdu_uo), &txIPduInfo_st);

#ifdef COM_MULTICORE_SUPPORT
            Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)
#else
            Com_Get_SchM_Enter_Com_TxIPdu
#endif
            /* Process transmission request status */
            Com_Prv_ProcessTxReqStatus(idTxPdu_uo, ipduTransStatus_u8);
        }
#ifdef COM_TxIPduCallOuts
        else
        {
            /* As specified in OSEK COM, if the I-PDU callout returns false
             * the I-PDU will not be processed any further */
        }
#endif /* #ifdef COM_TxIPduCallOuts */

    }
    Com_Get_SchM_Exit_Com_TxIPdu

} /* End of Com_Prv_SendIpdu */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_LoadPeriodicModeInfo
 Description      : Private function to load periodic ticks in case of PERIODIC or MIXED mode
 Parameter        : idTxPdu_uo     - Id of the Tx ipdu
                    sendIpduFlag_st - Flags to decide Ipdu Transmission
 Return value     : true -> is event(Direct/Mixed) transmision mode
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_LoadPeriodicModeInfo(PduIdType idTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     isEventMode_b;

    isEventMode_b  = COM_FALSE;

    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_uo);

    /* If mode has changed, cancel the existing n-repetition transmissions.
     * As repetitions from the new transmission mode has to be used.
     */
    txIpduRamPtr_pst->cntrRepetitions_u8 = COM_ZERO;

    /* Fetch latest transmission mode */
    switch (Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8))
    {
        case COM_TXMODE_PERIODIC:
        {
            if(sendIpduFlag_st.isSwtIpduTxMode_u1 == COM_RESET)
            {
                /* TRACE[SWS_Com_00495] The below counter will be decremented in the next call of Com_MainFunctionTx()
                 * and IPDU will be transmitted if IPDU tx mode is PERIODIC */
                txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_ONE;
            }
            else
            {
                /*TRACE[SWS_Com_00881] When the transmission mode of an I-PDU is explicitly set by Com_SwitchIpduTxMode,
                 the AUTOSAR COM shall defer the cyclic transmissions of this I-PDU by ComTxModeTimeOffset.*/
                txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEOFFSET(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
            }
        }
        break;

        case COM_TXMODE_DIRECT:
        {
            /* TRACE[SWS_Com_00582] If a change of the TMS causes a change to the transmission mode DIRECT,
             * an immediate (within the next main function at the latest unless shifted due to the MDT) direct/
             * n-times transmission to the underlying layer shall be initiated.
             * The above requirement clarifies the behavior in case the TMS-switch to ComTxModeMode DIRECT
             * was triggered by a signal with ComTransferProperty PENDING.
             */
            /* N-Repetitions are to be loaded. */
            isEventMode_b = COM_TRUE;
        }
        break;

        case COM_TXMODE_MIXED:
        {
            /* TRACE[SWS_Com_00495] If TMS changes to MIXED mode because of a PENDING signal, then IPDU will be sent in
             * periodic mode immediately, respecting MDT. */
            if (sendIpduFlag_st.isSigTriggered_u1 == COM_RESET)
            {
                if(sendIpduFlag_st.isSwtIpduTxMode_u1 == COM_RESET)
                {
                    /* The below counter will be decremented in the next call of Com_MainFunctionTx() and
                     * IPDU will be transmitted if IPDU tx mode is PERIODIC or MIXED */
                    txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_ONE;
                }
                else
                {
                    /* TRACE[SWS_Com_00881] When the transmission mode of an I-PDU is explicitly set by Com_SwitchIpduTxMode,
                       the AUTOSAR COM shall defer the cyclic transmissions of this I-PDU by ComTxModeTimeOffset.*/
                    txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEOFFSET(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
                }
            }
            else
            {
                /* The CurrentTxModePtr points to the structure information of the current TxModeState. */
                txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);

                /* Periodic transmission is always done from Com_MainFunctionTx,
                 * hence to maintain the periodicity between the first & second transmission,
                 * extra tick is added here.
                 * For subsequent periodic transmissions, they are reloaded in Com_MainFunctionTx.
                 */
                txIpduRamPtr_pst->cntrTimePeriod_u16++;

                /* N-Repetitions are to be loaded. */
                isEventMode_b = COM_TRUE;
            }
        }
        break;

        case COM_TXMODE_NONE:
        {
            /* do nothing */
        }
        break;

        default:
        {
            /* do nothing */
        }
        break;
    }

    return isEventMode_b;

} /* End of Com_Prv_LoadPeriodicModeInfo */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_LoadEventModeInfo
 Description      : Private function to load event(Direct/Mixed) mode configuration
 Parameter        : idTxPdu_uo       - Id of the Tx ipdu
                    sendIpduFlag_st - flags to decide ipdu transmission
 Return value     : true -> Proceed to SendIpdu
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_LoadEventModeInfo(PduIdType idTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st)
{
#ifdef COM_TxIPduTimeOut
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#endif
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIpdu_b;

    sendIpdu_b     = COM_FALSE;

#ifdef COM_TxIPduTimeOut
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
#endif
    txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idTxPdu_uo);

    /* N-Repetitions are reloaded only here. */
    if (sendIpduFlag_st.ignoreRepetitions_u1 == COM_RESET)
    {
        /* The CurrentTxModePtr points to the structure information of the current TxModeState.
         * Update the counter for number of repetitions,
         * - If TMS changes to DIRECT/MIXED mode with triggered signal transfer property.
         * - TRACE[SWS_Com_00279] New event transmission.
         * then send the IPDU synchronous with Com_MainFunctionTx, respecting MDT */
        txIpduRamPtr_pst->cntrRepetitions_u8 = COM_GET_TXIPDU_NUMOFREPETITION(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
    }
    else
    {
        /* Repetitions are not loaded if
         * 1. The updated Signal/SigGrp is configured with TransferProperty
         *    TriggeredOnChangeWithoutRepetition or TriggeredWithoutRepetition.
         * 2. Call is from Com_TriggerIpduSend or Com_TriggerIPDUSendWithMetaData then
         *    the outstanding transmissions for the direct/Mixed mode with N > 0 should be cancelled */
        txIpduRamPtr_pst->cntrRepetitions_u8 = COM_ZERO;
    }

#ifdef COM_TxIPduTimeOut
    /* The Transmission mode is Mixed/Direct */
    if (sendIpduFlag_st.isTimeoutReq_u1 != COM_RESET)
    {
        /* The timer is started only in case the signal/signal group has configured timeout.
         * Start the TxConfirmation Timer now i.e Set the Timer value to the one configured */
        Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_START);

        /* Note 1: If the transmission deadline monitoring timer runs out,there shall be an
         * ipdu-error-notification/signal-TimeoutNotification, regardless of the reason.
         * Even if it was postponed because of the MDT or it was filtered out via an I-PDU callout
         * Note 2: Timeout_Fact + 1,because in the immediate call to Com_MainfunctionTx(),
         * this Tick will be decremented */
        txIpduRamPtr_pst->cntrTxTimeout_u16 = (txIpduConstPtr_pcst->timeout_u16 + COM_ONE);
    }
#endif /* #ifdef COM_TxIPduTimeOut */

    /* 1. Check for Min Delay Timer of this IPDU,If MDT is not ZERO, SET a Flag, so that as soon as MDT becomes
     *    Zero in Com_MainfunctionTx(), this IPDU Can be transmitted.
     * 2. The Mentioned Sentence can be supported if MDT required for DIRECT Frames */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO) &&
        (Com_GetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16) == COM_FALSE))
    {

        /* Check if it is a single transmission. */
        if (txIpduRamPtr_pst->cntrRepetitions_u8 == COM_ZERO)
        {
            /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
            /* In case of a last transmission, reload Mixed-PhaseShift peridiocity(Com_Tick_Tx).
             * Check if MixedPhaseShift is enabled.
             * NOTE : PhaseShift is applicable for COM_MIXED mode only.If mode is COM_PERIODIC then this
             * bitfield is initialised to COM_FALSE.
             * If enabled, then periodic timer has to be reloaded here. */
            if (Com_GetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8))
            {
                txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);

                txIpduRamPtr_pst->cntrTimePeriod_u16++;
            }
#endif /* #ifdef COM_MIXEDPHASESHIFT */
            /* FC_VariationPoint_END */

            /* This flag does multiple operations:
             * - In case of deferred processing, flag enables the TxIPdu to be transmitted from Com_MainFunctionTx.
             * - In case of non-deferred processing, flag will add an extra offset to configured MDT value
             * to prevent MDT-timer from early expiry.
             */
            Com_SetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);

            /* FC_VariationPoint_START */
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* Com Design and optimisation requirement: The transmissions are never done event based;
             * Whenever there is an event transmission, Com flags it and initiates the transmission in
             * the next MainFunctionTx
             * Please note this holds good for Ipdus transmitted from API Com_TriggerIpduSend.
             *
             * TRACE[SWS_Com_00466] If I-PDU contains gated signals with ComTransferProperty TRIGGERED that are newly
             * received, the sending may be performed in either Com_MainFunctionTx or Com_MainFunctionRouteSignals.
             * Hence even for gated TxIPdu, transmission is also handled through MainFunctionTx
             */
            txIpduRamPtr_pst->cntrRepPeriod_u16 = COM_ONE;
#else
            /* FC_VariationPoint_END   */

            /* In case of non-deferred processing, trigger immediate transmission */
            sendIpdu_b = COM_TRUE;

            /* FC_VariationPoint_START */
#endif /*COM_TXPDU_DEFERREDPROCESSING*/
            /* FC_VariationPoint_END   */
        }
        else
        {
            /* Com Design and optimisation requirement: The transmissions are never done event based;
             * Whenever there is an event transmission, Com flags it and initiates the transmission in
             * the next MainFunctionTx
             * Please note this holds good for Ipdus transmitted from API Com_TriggerIpduSend.
             *
             * TRACE[SWS_Com_00466] If I-PDU contains gated signals with ComTransferProperty TRIGGERED that are newly
             * received, the sending may be performed in either Com_MainFunctionTx or Com_MainFunctionRouteSignals.
             * Hence even for gated TxIPdu, transmission is also handled through MainFunctionTx
             */
            txIpduRamPtr_pst->cntrRepPeriod_u16 = COM_ONE;

        } /* END of if (txIpduRamPtr_pst->cntrRepetitions_u8 == COM_ZERO) */
    }
    else
    {
        /* This Flag has to be RESET in Com_MainFunctionTx just before calling Com_Prv_SendIpdu */
        Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);

        /* Periodic timer update for mixed phase shift when msg triggered within ComMinimumDelayTime */
#ifdef COM_MIXEDPHASESHIFT
        if (Com_GetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8))
        {
            txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
        }
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }

#if (COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION == STD_OFF)
    /* This flag is set to load MDT for event triggered I-PDU transmissions */
    Com_SetRamValue(TXIPDU,_MDTLOAD,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
#endif

    return sendIpdu_b;

} /* End of Com_Prv_LoadEventModeInfo */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CalculateTxPduLength
 Description      : Private function to calculate PduLength
 Parameter        : idTxPdu_uo - Id of the Tx ipdu
 Return value     : PduLength
 **********************************************************************************************************************
*/
LOCAL_INLINE PduLengthType Com_Prv_CalculateTxPduLength(PduIdType idTxPdu_uo)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) || defined(COM_METADATA_SUPPORT)
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#endif
    PduLengthType               txPduLength_uo;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) || defined(COM_METADATA_SUPPORT)
    txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idTxPdu_uo);
#endif
    txPduLength_uo = COM_GET_TXIPDU_SIZE(idTxPdu_uo);

#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
    /* The total Ipdu size will be sum of Ipdu static length and dynamic length for a dynamic IPdu.
     * The Dynamic length of the Ipdu is updated in the call to Com_SendDynSignal. */
    if (Com_GetValue(TXIPDU,_ISDYNIPDU,txIpduConstPtr_pcst->txIPduFields_u16))
    {
        Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)

        txPduLength_uo += (PduLengthType)txIpduRamPtr_pst->dynLength_u16;

        Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)
    }
    else
#endif /* #if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) */
#if !defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT)
    /* If IPdu supports MetaData then the total IPdu size will be sum of IPdu static length and
     * MetaDataLength */
    if (txIpduConstPtr_pcst->txMetaData_pcst != NULL_PTR)
    {
        Com_TxMetaData_tpcst metaDataInfoPtr_pcst;

        metaDataInfoPtr_pcst = txIpduConstPtr_pcst->txMetaData_pcst;

        txPduLength_uo += metaDataInfoPtr_pcst->TxmetaDataLength_u8;

        Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)

        /* If transmission request is not pending from Com_TriggerIPDUWithMetaData, then fill default metadata */
        if (Com_GetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16) == COM_FALSE)
        {
            /* Restore default metadata in case request is not pending from TriggerIpduSendWithMetadata */
            Com_ByteCopy( (txIpduConstPtr_pcst->buffPtr_pu8 + COM_GET_TXIPDU_SIZE(idTxPdu_uo)),
                          (&metaDataInfoPtr_pcst->TxmetaDataDefaultValue_au8[0]),
                          ( metaDataInfoPtr_pcst->TxmetaDataLength_u8) );
        }
        else
        {
            /* application metadata is retained and reset flag for next transmissions */
            Com_SetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }

        Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)
    }
    else
#endif /* #if !defined(COM_ECUC_AR_RELEASE_AR45) && defined(COM_METADATA_SUPPORT) */
    {
        /* Pdu is neither of dynamic nor metadata,
         * Hence, SduLength has to be provided only with the already updated static length */
    }

#ifdef COM_TP_IPDUTYPE
    /* Finally store the calculated Tx-PduLength into global variable
     * that shall be used in Com_CopyTxData() */
    COM_GET_TPTXIPDULENGTH(idTxPdu_uo) = txPduLength_uo;
#endif

    return txPduLength_uo;

} /* End of Com_Prv_CalculateTxPduLength */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessTxReqStatus
 Description      : Private function to process transmission request status
 Parameter        : idTxPdu_uo         - Id of the Tx ipdu
                    ipduTransStatus_u8 - E_OK/E_NOT_OK
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_ProcessTxReqStatus(PduIdType idTxPdu_uo, Std_ReturnType ipduTransStatus_u8)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idTxPdu_uo);

    Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_TXFLAGS)
#if (COM_RETRY_FAILED_TX_REQUESTS == STD_OFF)
    /* TRACE[SWS_Com_00698] If ComMinimumDelayTime of an I-PDU is configured greater than 0 and
     * ComRetryFailedTransmitRequests is set to FALSE, the AUTOSAR COM module shall load and
     * start the minimum delay time counter upon transmission of that I-PDU to the PDU Router
     * via PduR_ComTransmit. */
    Com_Prv_LoadMinimumDelayTime(txIpduRamPtr_pst, txIpduConstPtr_pcst);
#endif
    if (ipduTransStatus_u8 == E_OK)
    {

#if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON)
        /* If ComRetryFailedTransmitRequests is enabled and call to PduR_ComTransmit() returns E_OK,
         * then clear the failed Tx-IPdu requests */
        Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

        /* TRACE[SWS_Com_00828] If ComMinimumDelayTime of an I-PDU is configured greater than 0 and
         * ComRetryFailedTransmitRequests is set to TRUE, the AUTOSAR COM module shall load and
         * start the minimum delay time counter upon transmission of that I-PDU to the PDU Router
         * via PduR_ComTransmit only in case PduR_ComTransmit returns E_OK */
        Com_Prv_LoadMinimumDelayTime(txIpduRamPtr_pst, txIpduConstPtr_pcst);
#endif /* #if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON) */

        /* If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to Transmit, the AUTOSAR COM module
         * shall clear all update-bits of all contained signals and signal groups
         * after this I-PDU was sent out via PduR_ComTransmit and PduR_ComTransmit returned E_OK. */
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if (Com_GetValue(TXIPDU,_CLRUPDBIT,txIpduConstPtr_pcst->txIPduFields_u16) == (uint16)COM_CLRUPDATEBIT_TRANSMIT)
        {
            Com_Prv_ClearUpdateBits(txIpduConstPtr_pcst);
        }
#endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */

#ifdef COM_TP_IPDUTYPE
        if (Com_GetValue(TXIPDU,_ISLARGEDATAPDU,txIpduConstPtr_pcst->txIPduFields_u16))
        {
            /* For TP-type Pdus, lock the buffer for further updates, until TP transmission is completed. i.e.,
             * TRACE[SWS_Com_00662] After successful initiation of a transmission of a large I-PDU via PduR_ComTransmit,
             * the AUTOSAR COM module shall not modify the data of its internal transmit buffer of this I-PDU. */
            Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
            txIpduRamPtr_pst->txTPIPduLength_uo = COM_ZERO;
        }
#endif /* #ifdef COM_TP_IPDUTYPE */
    }
    else
    {

#ifdef COM_TX_IPDUCOUNTER

        /* Check if lower layer returned E_NOT_OK and IPduCounter is configured */
        if (txIpduConstPtr_pcst->idxTxIpduCntr_uo != COM_TXIPDU_CNTR_INV_IDX)
        {
            Com_Prv_TxIpduDecrementCounter( idTxPdu_uo );
        }
#endif /* #ifdef COM_TX_IPDUCOUNTER */

#if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON)
        /* If ComRetryFailedTransmitRequests is enabled and call to PduR_ComTransmit() returns E_NOT_OK,
         * then store the failed Tx-IPdu requests */
        Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
#endif

    } /* End of ipduTransStatus_u8 check */

    Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_TXFLAGS)

} /* End of Com_Prv_ProcessTxReqStatus */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_LoadMinimumDelayTime
 Description      : Private function to load minimum delay time
 Parameter        : TxIpduRamPtr_pst        - pointer to TxIPDU RAM Memory
                    txIpduConstPtr_pcst     - pointer to TxIPDU ROM data
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_LoadMinimumDelayTime(Com_TxIpduRam_tpst txIpduRamPtr_pst, Com_TxIpduCfg_tpcst txIpduConstPtr_pcst)
{
    /* If ComMinimumDelayTime of an I-PDU is configured greater than 0, the AUTOSAR COM module shall load and
     * start the MDT counter upon transmission of that I PDU to the PDU Router via PduR_ComTransmit.
     * NOTE : Check (TxIpduRamPtr->Com_MinDelayTick == 0) is done because of the below reasons.
     * If interrupts are enabled, then interrupt could be called between the call of PduR_ComTransmit() above
     * and Com_TxConfirmation(). In this case Com_MinDelayTick will be overwritten here, leading to deadlock.
     */
    if ((txIpduConstPtr_pcst->minDelayTime_u16 != COM_ZERO) && (txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO))
    {
#if (COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION == STD_OFF)
        if (Com_GetRamValue(TXIPDU,_MDTLOAD,txIpduRamPtr_pst->txFlags_u16))
        {
            /* This flag is reset to not to load MDT for repeated transmission */
            Com_SetRamValue(TXIPDU,_MDTLOAD,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif

#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* If DEFERREDPROCESSING is enabled, transmissions are done always from MainFuncionTx.
             * Hence MDT offset is not required. */
            txIpduRamPtr_pst->cntrMinDelayTime_u16 = txIpduConstPtr_pcst->minDelayTime_u16;
#else
            /* If DEFERREDPROCESSING is disabled, add an offset of 1.
             * As this is reduced by 1 immediately in next cyclic task, leading to early timeout of MDT timer */
            txIpduRamPtr_pst->cntrMinDelayTime_u16 = txIpduConstPtr_pcst->minDelayTime_u16 +
                                (Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16) ? COM_ONE : COM_ZERO);
#endif /* #ifdef COM_TXPDU_DEFERREDPROCESSING */

#if (COM_ENABLE_MDT_FOR_CYCLIC_TRANSMISSION == STD_OFF)
        }
#endif
    }
} /* End of Com_Prv_LoadMinimumDelayTime */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ClearUpdateBits
 Description      : Internal function used for clearing the update bits of signals and signal groups in the IPdu
 Parameter        : txIpduConstPtr_pcst -> Pointer to Tx ipdu static configuration
 Return value     : None
 **********************************************************************************************************************
*/
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_ClearUpdateBits(Com_TxIpduCfg_tpcst txIpduConstPtr_pcst)
{
#ifdef COM_TxSigUpdateBit
    Com_TxSigCfg_tpcst              txSigConstPtr_pcst;
#endif
#ifdef COM_TxSigGrpUpdateBit
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
#endif
    uint16_least                    index_qu16;
    PduLengthType                   byteNo_uo;

    /* After an I-PDU is sent to lower layers and no synchronous error is returned
     * by the lower layer the update-bits of all signals and signal groups belonging
     * to the I-PDU sent shall be cleared.*/

#ifdef COM_TxSigUpdateBit
    /* Clear all Update Bits if configured in signals */
    txSigConstPtr_pcst = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);

    for(index_qu16 = txIpduConstPtr_pcst->numOfSig_u16; index_qu16 != COM_ZERO; index_qu16--)
    {
        /* Check if UpdateBit is configure for the signal */
        if(Com_GetValue(GEN,_UPDBITCONF,txSigConstPtr_pcst->general_u8))
        {
            /* Find the Byte No, where the Update bit lies */
            byteNo_uo = (PduLengthType)((txSigConstPtr_pcst->updateBitPos_uo) >> 3u);

            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst, txSigConstPtr_pcst->updateBitPos_uo, byteNo_uo)
        }
        txSigConstPtr_pcst++;
    }
#endif /* #ifdef COM_TxSigUpdateBit*/

#ifdef COM_TxSigGrpUpdateBit
    /* Clear all Update Bits if configured in signal groups */
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);

    for(index_qu16 = txIpduConstPtr_pcst->numOfSigGroups_u16; index_qu16 != COM_ZERO; index_qu16--)
    {
        /* Check if UpdateBit is configure for the signal group */
        if(Com_GetValue(TXSIGGRP,_UPDBITCONF,txSigGrpConstPtr_pcst->txSignalGrpFields_u8))
        {
            /* Find the Byte No, where the Update bit lies */
            byteNo_uo = (PduLengthType)((txSigGrpConstPtr_pcst->updateBitPos_uo) >> 3u);

            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst, txSigGrpConstPtr_pcst->updateBitPos_uo, byteNo_uo)
        }
        txSigGrpConstPtr_pcst++;
    }
#endif /* #ifdef COM_TxSigGrpUpdateBit*/
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif/* #if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) */


#ifdef COM_TX_IPDUCOUNTER
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxIpduIncrementCounter
 Description      : Internal function to increment counter value
 Parameter        : idTxPdu_uo - Id of the Tx IPDU
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_TxIpduIncrementCounter(PduIdType idTxPdu_uo)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxIpduCntrCfg_tpcst         txIpduCntrConstPtr_pcst;
    uint8 *                         counterBytePtr_pu8;
    uint8                           counterValue_u8;
    uint8                           counterBitOffset_u8;

    txIpduConstPtr_pcst      = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
    txIpduCntrConstPtr_pcst  = COM_GET_TX_IPDU_CNTR_CONSTDATA(txIpduConstPtr_pcst->idxTxIpduCntr_uo);

    counterBytePtr_pu8      = (uint8 *)(txIpduConstPtr_pcst->buffPtr_pu8 + (txIpduCntrConstPtr_pcst->bitPos_uo / 8u));
    counterBitOffset_u8 = (uint8)(txIpduCntrConstPtr_pcst->bitPos_uo % 8u);

    /* Locks are required here to consistently pack the ipdu-counter */
    Com_Get_SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA)

    /* TRACE[SWS_Com_00688] The AUTOSAR COM module shall increase the I-PDU counter of an I-PDU by one,
     * with respect to counter wrap around, directly before the I-PDU is sent out by PduR_ComTransmit. */
    /* => Especially for Size = 8, wrap around will be done automatically in the below statement. */
    counterValue_u8 = (uint8)((uint16)COM_GET_TXIPDU_CNTR_VAL(txIpduConstPtr_pcst->idxTxIpduCntr_uo) + COM_ONE);

    if ((uint16)counterValue_u8 < COM_GET_BASE_2_POWER_X(txIpduCntrConstPtr_pcst->bitSize_u8))
    {
        /* Incremented counter is already within its range, hence wrap around is not req */
    }
    else
    {
        /* Reset counter back to 'zero' in case counter exceeds maximum value */
        counterValue_u8 = COM_ZERO;
    }

    /* Pack counter value into IPdu buffer and also into global IPdu counter */
    COM_PACK_CNTR_INTO_IPDU_BUFF((uint8 *)(counterBytePtr_pu8), counterBitOffset_u8,
                                           txIpduCntrConstPtr_pcst->bitSize_u8, counterValue_u8);
    /* Save incremented TxIPdu counter value */
    COM_GET_TXIPDU_CNTR_VAL(txIpduConstPtr_pcst->idxTxIpduCntr_uo) = counterValue_u8;

    Com_Get_SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA)

} /* End of Com_Prv_TxIpduIncrementCounter */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxIpduDecrementCounter
 Description      : Internal function to decrement counter value
 Parameter        : idTxPdu_uo - Id of the Tx IPDU
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_TxIpduDecrementCounter(PduIdType idTxPdu_uo)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxIpduCntrCfg_tpcst         txIpduCntrConstPtr_pcst;
    uint8 *                         counterBytePtr_pu8;
    uint8                           counterValue_u8;
    uint8                           counterBitOffset_u8;

    txIpduConstPtr_pcst      = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
    txIpduCntrConstPtr_pcst  = COM_GET_TX_IPDU_CNTR_CONSTDATA(txIpduConstPtr_pcst->idxTxIpduCntr_uo);

    counterBytePtr_pu8      = (uint8 *)(txIpduConstPtr_pcst->buffPtr_pu8 + (txIpduCntrConstPtr_pcst->bitPos_uo / 8u));
    counterBitOffset_u8 = (uint8)(txIpduCntrConstPtr_pcst->bitPos_uo % 8u);

    counterValue_u8 = COM_GET_TXIPDU_CNTR_VAL(txIpduConstPtr_pcst->idxTxIpduCntr_uo);

    /* TRACE[SWS_Com_00688] In case the return value of PduR_ComTransmit is not equal to E_OK the update of
     * the I-PDU counter shall be revoked (by decrementing the I-PDU counter by one) */
    if (counterValue_u8 != COM_ZERO)
    {
        counterValue_u8--;
    }
    else
    {
        /* If counter is 'zero' then assign maximum counter value */
        counterValue_u8 = (uint8)COM_MAX_U_VALUE_OF_X_BITS(txIpduCntrConstPtr_pcst->bitSize_u8);
    }

    COM_PACK_CNTR_INTO_IPDU_BUFF((uint8 *)(counterBytePtr_pu8), counterBitOffset_u8,
                                 txIpduCntrConstPtr_pcst->bitSize_u8, counterValue_u8);
    /* Save decremented TxIPdu counter value */
    COM_GET_TXIPDU_CNTR_VAL(txIpduConstPtr_pcst->idxTxIpduCntr_uo) = counterValue_u8;

} /* End of Com_Prv_TxIpduDecrementCounter */

#endif /* #ifdef COM_TX_IPDUCOUNTER */

