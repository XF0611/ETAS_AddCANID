

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

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

LOCAL_INLINE Std_ReturnType Com_Prv_ReadSigGwReceiveQueue(
                                                   PduIdType *            idComRxPdu_puo,
                                                   Com_RxGwQueueIndex_tuo rxGwQueueWrite_uo
                                                      );

LOCAL_INLINE void Com_Prv_PrepareTxIpdu(void);

LOCAL_INLINE void Com_Prv_ReadRxQueueAndProcess(void);

#ifdef COM_SIGNALGATEWAY

LOCAL_INLINE void Com_Prv_ProcessRxSignalGateway(Com_RxIpduCfg_tpcst rxIpduConstPtr_pcst);

#endif

#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))

LOCAL_INLINE void Com_Prv_ProcessRxSigGrpGateway(Com_RxIpduCfg_tpcst rxIpduConstPtr_pcst);

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
 Function name    : Com_MainFunctionRouteSignals
 Description      : Service that Calls the signal gateway part of the AUTOSAR COM module
                    to forward received signals to be routed.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_MainFunctionRouteSignals( void )
{
    /* TRACE[SWS_Com_00666] A call to Com_MainFunctionRouteSignals shall simply return
     * if the AUTOSAR COM module was not previously initialized with a call to Com_Init.
     * TRACE[SWS_Com_00804] Error code(COM_E_UNINIT) is not reported,
     * if Com_MainFunctionRouteSignals service is called
     * before the AUTOSAR COM module was initialized with Com_Init or after a call to Com_DeInit */
    if (Com_InitStatus_en != COM_UNINIT)
    {
        /* Call function to read Rx queue and process */
        Com_Prv_ReadRxQueueAndProcess();

        /* Call function to send the IPdu */
        Com_Prv_PrepareTxIpdu();
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_WriteSigGwReceiveQueue
 Description      : this function writes into the Rx Gw queue and updated the pointers accordingly
                    to Be considered before using this function
 Parameter        : idComRxPdu_uo
 Return value     : Std_ReturnType
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
Std_ReturnType Com_Prv_WriteSigGwReceiveQueue(PduIdType idComRxPdu_uo)
{
    /* Local variables */
    Com_RxGwQueueRam_tpst       rxGwQueuePtr_pst;
    uint8                       status_u8;

    /* Initialization of variables */
    rxGwQueuePtr_pst = COM_GET_RXGWQUEUE_S;
    status_u8 = E_OK;

    /* The write operation need not be conditional,
     * 1. As the length of the queue is fixed and queue need not be updated for any new updates on the signal */
    rxGwQueuePtr_pst->rxGwQueuePtr_puo[rxGwQueuePtr_pst->rxGwQueueWrite_uo] = idComRxPdu_uo;
    /* Perform a modulo to get to the next available location */
    rxGwQueuePtr_pst->rxGwQueueWrite_uo = ( Com_RxGwQueueIndex_tuo )
                                              ( ( rxGwQueuePtr_pst->rxGwQueueWrite_uo + COM_ONE ) %
                                              ( COM_GET_NUM_GWSRC_IPDUS + COM_ONE ) );

# if (COM_PRV_ERROR_HANDLING == STD_ON)
   /* If the buffer is full and there is a request to write into the queue, It is taken care that no new request are
    * recieved after that. Hence if DET is enabled call DET Function to check DET Error COM_E_PARAM
    */
    if (((rxGwQueuePtr_pst->rxGwQueueWrite_uo) % (COM_GET_NUM_GWSRC_IPDUS+COM_ONE)) == rxGwQueuePtr_pst->rxGwQueueRead_uo)
    {
        COM_DET_REPORT_ERROR(COMServiceId_MainFunctionRouteSignals, COM_E_PARAM);
        status_u8 = E_NOT_OK;
    }
# endif /* end of COM_PRV_ERROR_HANDLING */

    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ReadSigGwReceiveQueue
 Description      : this function reads from the Rx Gw queue and updated the pointers accordingly
 Parameter        : (IN/OUT)idComRxPdu_puo
                    (IN)    rxGwQueueWrite_uo
 Return value     : Std_ReturnType
 **********************************************************************************************************************
*/
LOCAL_INLINE Std_ReturnType Com_Prv_ReadSigGwReceiveQueue(
                                                   PduIdType *            idComRxPdu_puo,
                                                   Com_RxGwQueueIndex_tuo rxGwQueueWrite_uo
                                                      )
{
    Com_RxGwQueueRam_tpst       rxGwQueuePtr_pst;
    Std_ReturnType              retVal_uo;

    rxGwQueuePtr_pst    = COM_GET_RXGWQUEUE_S;
    retVal_uo           = E_NOT_OK;

    /* This operation is performed before every read to confirm if the data is present in the queue */
    if(rxGwQueueWrite_uo != rxGwQueuePtr_pst->rxGwQueueRead_uo)
    {
        /* The Read operation need not be conditional,
         * 1. As the length of the queue is fixed and queue need not be updated for any new updates on the signal */
        *idComRxPdu_puo = rxGwQueuePtr_pst->rxGwQueuePtr_puo[rxGwQueuePtr_pst->rxGwQueueRead_uo];
        /* Perform a modulo to get to the next available location */
        rxGwQueuePtr_pst->rxGwQueueRead_uo = ( Com_RxGwQueueIndex_tuo )
                                                 ( ( rxGwQueuePtr_pst->rxGwQueueRead_uo + COM_ONE ) %
                                                 ( COM_GET_NUM_GWSRC_IPDUS + COM_ONE ) );
        retVal_uo = E_OK;
    }

    return(retVal_uo);
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_PrepareTxIpdu
 Description      : This function process the Ipdu transmission
 Parameter        : None
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_PrepareTxIpdu(void)
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    uint16_least                idxIdTxIpdu_qu16;
    uint16_least                numOfIpdus_qu16;
#if defined (COM_SIGNALGROUPGATEWAY) && defined (COM_TX_SIGNALGROUP)
    Com_TxSigGrpRam_tpst        txSigGrpRamPtr_pst;
    uint16_least                idxIdTxSigGrp_qu16;
    uint16_least                idMaxTxSigGrp_qu16;
#endif
    boolean                     isTriggered_b;
#ifdef COM_TxFilters
    boolean                     isTMSChanged_b;
#endif
#ifdef COM_TxIPduTimeOut
    boolean                     isTimeoutReq_b;
#endif

    /* Calculate number of TxIPdu-IdS which shall be processed for default ComMainFunction.
     * DesignDecision: Gateway TxIPduS are restricted to default MainFunctionTx.
     * Hence first-IPDU-Id starts with zero. */
    numOfIpdus_qu16     = COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (uint16)ComMainFunction_Internal_MainFunctionTx).numOfIpdus_uo;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(0);
    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(0);

    for(idxIdTxIpdu_qu16 = COM_ZERO; idxIdTxIpdu_qu16 < numOfIpdus_qu16; idxIdTxIpdu_qu16++)
    {
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
        *  HIS-LEVEL parameter will be violated if more conditional statements are added.
        *  Hence the warning is suppressed. */
        /* Check if IPdu group is started */
        if((Com_GetValue(TXIPDU,_ISGWPDU,txIpduConstPtr_pcst->txIPduFields_u16)) &&
           (Com_Prv_CheckTxIPduStatus((PduIdType)idxIdTxIpdu_qu16)))
        {

            /* Process Tx-IPdu transmission */
#if defined (COM_SIGNALGROUPGATEWAY) && defined (COM_TX_SIGNALGROUP)

            idxIdTxSigGrp_qu16  = txIpduConstPtr_pcst->idFirstTxSigGrp_uo;
            idMaxTxSigGrp_qu16  = idxIdTxSigGrp_qu16 + txIpduConstPtr_pcst->numOfSigGroups_u16;

            txSigGrpRamPtr_pst  = &COM_GET_TXSIGGRPFLAG(idxIdTxSigGrp_qu16);

            for ( ; idxIdTxSigGrp_qu16 < idMaxTxSigGrp_qu16; idxIdTxSigGrp_qu16++ )
            {
                if (Com_GetRamValue(TXSIGGRP,_ISGWUPDTD,txSigGrpRamPtr_pst->txSigGrpRAMFields_u8))
                {
                    Com_SetRamValue(TXSIGGRP,_ISGWUPDTD,txSigGrpRamPtr_pst->txSigGrpRAMFields_u8,COM_FALSE);

                    /* 1. Set the UpdateBit
                     * 2. Update TxIPdu buffer with the latest Tx-GroupSignal data
                     * 3. Evaluate TMC of GroupSignals
                     * 4. Evaluate TMS of TxIPdu
                     * Return value is based on IPduGroup status that can be ignored here,
                     * as it is checked later in Com_MainFunctionRouteSignals()
                     */
                    (void)Com_Prv_InternalSendSignalGroup( (Com_SignalGroupIdType)idxIdTxSigGrp_qu16 );
                }
                txSigGrpRamPtr_pst++;
            }
#endif /* #if defined (COM_SIGNALGROUPGATEWAY) && defined (COM_TX_SIGNALGROUP) */

            /* locks are required to protect Com_TxFlags */
            SchM_Enter_Com_TxIpduProtArea(MAINFUNCTION_ROUTE_SIGNALS);

            /* read and clear the global flags */
            isTriggered_b = Com_GetRamValue(TXIPDU,_ISGWTRIGGRD,txIpduRamPtr_pst->txFlags_u16);
            Com_SetRamValue(TXIPDU,_ISGWTRIGGRD,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

#ifdef COM_TxFilters
            isTMSChanged_b = Com_GetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16);
            Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif

#ifdef COM_TxIPduTimeOut
            isTimeoutReq_b = Com_GetRamValue(TXIPDU,_ISTOENABLED,txIpduRamPtr_pst->txFlags_u16);
            Com_SetRamValue(TXIPDU,_ISTOENABLED,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif
            SchM_Exit_Com_TxIpduProtArea(MAINFUNCTION_ROUTE_SIGNALS);

            /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status.
            * Evaluation(or no evaluation) of the 2nd or 3rd operands does not impact the system behaviour,
            * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
            * Hence the warning is suppressed. */
            if (((isTriggered_b) &&
                (COM_TX_MODE_IS_EVENT(Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8))))
#ifdef COM_TxFilters
                || (isTMSChanged_b)
#endif
               )
            {

                /* All unused flags are reset here */
                Com_SendIpduInfo_tst sendIpduFlag_st = COM_CLEAR_SENDIPDU_FLAGS;

                /* this flag will be set, as call happens from Com_MainFunctionRouteSignals */
                sendIpduFlag_st.isEventTrig_u1      = COM_SET;

                /* For the direct part of the Mixed TM, Signal Transfer property is set directly from
                 * _ISGWTRIGGRD flag. In all the other cases, it is ignored. */
#ifdef COM_TxFilters
                sendIpduFlag_st.isSigTriggered_u1   = COM_BOOL_TO_UNSIGNED(isTriggered_b);
#else
                sendIpduFlag_st.isSigTriggered_u1   = COM_SET;
#endif

#ifdef COM_TxFilters
                sendIpduFlag_st.isModeChangd_u1     = COM_BOOL_TO_UNSIGNED(isTMSChanged_b);
#endif
                /* Set the timeout flag if any updated signal contributes to timeout */
#ifdef COM_TxIPduTimeOut
                sendIpduFlag_st.isTimeoutReq_u1     = COM_BOOL_TO_UNSIGNED(isTimeoutReq_b);
#endif
                /* Call function for Transmitting Tx Ipdu's */
                Com_Prv_SendIpdu((PduIdType)idxIdTxIpdu_qu16, sendIpduFlag_st);
            }

        } /* end of IPdu group started check */

        txIpduConstPtr_pcst++;
        txIpduRamPtr_pst++;
    } /* end of for loop */
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ReadRxQueueAndProcess
 Description      : This function writes into the Rx Gw queue and update the pointers accordingly
 Parameter        : None
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_ReadRxQueueAndProcess(void)
{

    /* Variable declaration */
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    Com_RxGwQueueRam_tpst       rxGwQueuePtr_pst;
    Com_RxGwQueueIndex_tuo      rxGwQueueWrite_uo;
    PduIdType                   idIpdu_uo;
    uint8                       ipduSyncCounter_u8;

    rxGwQueuePtr_pst          = COM_GET_RXGWQUEUE_S;

    /* Take a local copy of the Write pointer
     * Only the receptions till this step are processed in the current call of Com_MainFunctionRouteSignals()*/
    rxGwQueueWrite_uo = rxGwQueuePtr_pst->rxGwQueueWrite_uo;
    {
        /* Run through the loop for all the new elements in the queue, and process each element in the queue */
        /* Read the queue here; The queue read pointer is moved to the next location in the below operation
         * Read is to be performed carefully, if called two time the read pointer is moved */
        while (Com_Prv_ReadSigGwReceiveQueue(&idIpdu_uo,rxGwQueueWrite_uo) == (Std_ReturnType)E_OK)
        {
            rxIpduConstPtr_pcst   = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);
            rxIpduRamPtr_pst      = &COM_GET_RXPDURAM_S(idIpdu_uo);

            /* Start the counter with this value, as this the maximum times the Ipdu will be processed for
             * synchronisation of Signals within Ipdu */
            ipduSyncCounter_u8 = COM_ZERO;

            /* This below part of the code(do-while) is needed for Signal level data synchronisation within one Rx Ipdu
             * Scenario:  If while processing this signal in the queue There was an update on the rx side,
             * it is possible that one signal can have old value and other signals can have new value.
             * Method : During the complete Gw routing operation if a new update on the Ipdu was recieved,
             * then all the signals are updated to new values  */
            do
            {
                Com_Get_SchM_Enter_Com_RxGateway
                Com_Get_SchM_Enter_Com_RxPduBuffer

                /* Here the Ipdu entry is popped out of the queue, this flag is reset to capture the changes in
                 * Com_RxIndication. Set the Ipdu processed flag to TRUE, this is to capture any further updates
                 * during the processing */
                Com_SetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,rxIpduRamPtr_pst->rxFlags_u8,COM_FALSE);
                Com_Get_SchM_Exit_Com_RxPduBuffer

#ifdef COM_SIGNALGATEWAY
                if(rxIpduConstPtr_pcst->numOfGwSrcSignals_u16 > COM_ZERO)
                {
                    /* Call function to process SignalGateway data */
                    Com_Prv_ProcessRxSignalGateway(rxIpduConstPtr_pcst);
                }
#endif /* #ifdef COM_SIGNALGATEWAY */

#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))
                if(rxIpduConstPtr_pcst->numOfGwSrcSignalGrps_u16 > COM_ZERO)
                {
                    /* Call function to process SignalGroup Gateway data */
                    Com_Prv_ProcessRxSigGrpGateway(rxIpduConstPtr_pcst);
                }
#endif /* #if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP)) */

                /* This part will control the loop to run 2 times after the main processing
                 * in case there was a new Reception of same Ipdu */
                ipduSyncCounter_u8 =
                         ((Com_GetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,rxIpduRamPtr_pst->rxFlags_u8)) ?
                         ((ipduSyncCounter_u8 + COM_ONE) % (COM_MAXIMUM_GWSYNC_REPITIONS + COM_ONE)) : (COM_ZERO));

                Com_Get_SchM_Exit_Com_RxGateway

            }while (ipduSyncCounter_u8 != COM_ZERO);
        }
    }
}


#ifdef COM_SIGNALGATEWAY
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxSignalGateway
 Description      : This function writes into the Rx Gw queue for Signals and trigger for transmission
                    of signals
 Parameter        : rxIpduConstPtr_pcst  -> Address of the RxIpdu
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_ProcessRxSignalGateway(Com_RxIpduCfg_tpcst rxIpduConstPtr_pcst)
{

    Com_GwMapSigCfg_tpcst           gwMapSigConfPtr_pcst;
    Com_GwMapSigIdCfg_tpcst         gwMapSigIdConfPtr_pcst;
    Com_RxSigRam_tpst               rxSigRamPtr_pst;
    Com_RxSigCfg_tpcst              rxSigConstPtr_pcst;
    Com_SignalIdType                idTxGwDest_u16;
    uint16_least                    idxIdRxSig_qu16;
    uint16_least                    idMaxRxSig_qu16;
    uint16_least                    index_qu16;

    idxIdRxSig_qu16         = rxIpduConstPtr_pcst->idRxSig_uo;
    idMaxRxSig_qu16         = idxIdRxSig_qu16 + rxIpduConstPtr_pcst->numOfGwSrcSignals_u16;

    rxSigConstPtr_pcst      = COM_GET_RXSIG_CONSTDATA(idxIdRxSig_qu16);
    rxSigRamPtr_pst         = &COM_GET_RXSIGNALFLAG(idxIdRxSig_qu16);

    for( ; idxIdRxSig_qu16 < idMaxRxSig_qu16; idxIdRxSig_qu16++)
    {
        /* The processing of the signals within one Rx Ipdu could be done with an earlier signal entry in the queue;
         * This is to ensure that only new data updates are processed and not missed for other signals in the Ipdu
         * This will also ensure Update bit handling for an Rx signal with set update bit
         */
        /* Process only the 1. Gw signals fill bit set  */
        if(Com_GetRamValue(RXSIG,_SIGNALFILLBIT,rxSigRamPtr_pst ->rxSigRAMFields_u8))
        {
            /* clear signal data update flag, this is to capture any further updates during the processing */
            Com_SetRamValue(RXSIG,_SIGNALFILLBIT,rxSigRamPtr_pst ->rxSigRAMFields_u8,COM_FALSE);

            gwMapSigConfPtr_pcst   = COM_GET_GWSIG_CONSTDATA(idxIdRxSig_qu16);
            gwMapSigIdConfPtr_pcst = COM_GET_GWSIGIDMAP_CONSTDATA(gwMapSigConfPtr_pcst->idxGwMapSigDestIdArray_u16);

            /* Loop for 1:N signalgatewaying,this packs the signaldata into all the configured destination Ipdubuffer*/
            for(index_qu16 = gwMapSigConfPtr_pcst->destCount_u8; index_qu16 > COM_ZERO; index_qu16--)
            {
                idTxGwDest_u16 = gwMapSigIdConfPtr_pcst->gwMapDestId_u16;

                Com_Prv_PackRxSignalGwBufferData(rxSigConstPtr_pcst,idTxGwDest_u16);

                gwMapSigIdConfPtr_pcst++;
            }
        }
        rxSigRamPtr_pst++;
        rxSigConstPtr_pcst++;
    }
}

#endif /* #ifdef COM_SIGNALGATEWAY */


#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxSigGrpGateway
 Description      : This function writes into the Rx Gw queue for signal groups and trigger for transmission
                    of signal groups
 Parameter        : rxIpduConstPtr_pcst      -> Address to Ipdu configuration
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_ProcessRxSigGrpGateway(Com_RxIpduCfg_tpcst rxIpduConstPtr_pcst)
{

    /* Variable Declaration */
    Com_RxSigGrpCfg_tpcst               rxSigGrpConstPtr_pcst;
    Com_RxSigGrpRam_tpst                rxSigGrpRamPtr_pst;
    Com_RxGrpSigCfg_tpcst               rxGrpSigConstPtr_pcst;
    Com_GwMapGrpSigCfg_tpcst            gwMapGrpSigCfgPtr_pcst;
    Com_GwMapGrpSigIdCfg_tpcst          gwMapGrpSigIdCfgPtr_pcst;
    Com_TxIntGrpSignalId_tuo            idTxGwDest_uo;
    uint16_least                        idxIdRxSigGrp_qu16;
    uint16_least                        idMaxRxSigGrp_qu16;
    uint16_least                        idxIdRxGrpSig_qu16;
    uint16_least                        idMaxRxGrpSig_qu16;
    uint16_least                        index_qu16;

    idxIdRxSigGrp_qu16      = rxIpduConstPtr_pcst->idFirstRxSigGrp_uo;
    idMaxRxSigGrp_qu16      = idxIdRxSigGrp_qu16 + rxIpduConstPtr_pcst->numOfGwSrcSignalGrps_u16;

    rxSigGrpConstPtr_pcst   = COM_GET_RXSIGGRP_CONSTDATA(idxIdRxSigGrp_qu16);
    rxSigGrpRamPtr_pst      = &COM_GET_RXSIGGRPFLAG(idxIdRxSigGrp_qu16);

    for( ; idxIdRxSigGrp_qu16 < idMaxRxSigGrp_qu16; idxIdRxSigGrp_qu16++)
    {
        /* The processing of the signal groups within one Rx Ipdu could be done with an earlier signal entry in the queue;
         * This is to ensure that only new data updates are processed and not missed for other signal groups in the Ipdu
         * This will also ensure Update bit handling for an Rx signal group with set update bit
         */
        /* Process only the 1. Gw signal group fill bit set  */
        /* Report an error if the GrpSignal to be routed is not a Gw signal id */
        if (Com_GetRamValue(RXSIGGRP,_SIGNALFILLBIT,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8))
        {
            /* clear signal-group data update flag, this is to capture any further updates during the processing */
            Com_SetRamValue(RXSIGGRP,_SIGNALFILLBIT,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8,COM_FALSE);

            idxIdRxGrpSig_qu16 = rxSigGrpConstPtr_pcst->idFirstGrpSig_uo;
            idMaxRxGrpSig_qu16  = idxIdRxGrpSig_qu16 + rxSigGrpConstPtr_pcst->numOfGwSrcGrpSignals_u16;

            rxGrpSigConstPtr_pcst   = COM_GET_RXGRPSIG_CONSTDATA(idxIdRxGrpSig_qu16);

            for( ; idxIdRxGrpSig_qu16 < idMaxRxGrpSig_qu16; idxIdRxGrpSig_qu16++ )
            {
                gwMapGrpSigCfgPtr_pcst   = COM_GET_GWGRPSIG_CONSTDATA((idxIdRxGrpSig_qu16 - COM_GET_NUM_GRPSIG_NOGW));
                gwMapGrpSigIdCfgPtr_pcst =
                             COM_GET_GWGRPSIGIDMAP_CONSTDATA(gwMapGrpSigCfgPtr_pcst->idxGwMapGrpSigDestIdArray_uo);

                for(index_qu16 = gwMapGrpSigCfgPtr_pcst->destCount_u8; index_qu16 > COM_ZERO; index_qu16--)
                {
                    idTxGwDest_uo = gwMapGrpSigIdCfgPtr_pcst->gwMapDestGrpSigId_uo;

                    /* Refer to Com_Cfg_SchM file; this is a multi-core lock */
                    Com_Get_SchM_Enter_Com_RxSigGwMcLockProtArea

                    Com_Prv_PackRxGrpSigGwBufferData(rxGrpSigConstPtr_pcst,idTxGwDest_uo);

                    Com_Get_SchM_Exit_Com_RxSigGwMcLockProtArea
                    gwMapGrpSigIdCfgPtr_pcst++;
                }
                rxGrpSigConstPtr_pcst++;
            }
        }
        rxSigGrpConstPtr_pcst++;
        rxSigGrpRamPtr_pst++;
    }
}

#endif /* #if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) || defined (COM_TX_SIGNALGROUP)) */

#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

