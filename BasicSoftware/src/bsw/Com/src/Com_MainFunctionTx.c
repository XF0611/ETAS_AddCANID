

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)
# include "PduR_Com.h"
#if(COM_ENABLE_INTER_MODULE_CHECKS)
/* [SWS_BSW_00036] Inter Module Checks */
# if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(COM_ENABLE_INTER_MODULE_CHECKS) */
#endif


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
#ifdef COM_TxIPduTimeOut
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_IPduTimeOut   (uint16_least idTxPdu_qu16);
#endif

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_Modes         (uint16_least idTxPdu_qu16);

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_DirectMode    (uint16_least idTxPdu_qu16);

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_MixedMode     (uint16_least idTxPdu_qu16);

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_NoneMode      (uint16_least idTxPdu_qu16);

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_PeriodicMode  (uint16_least idTxPdu_qu16);

LOCAL_INLINE void Com_Prv_MainFunctionTx_SendIPdu         (uint16_least idTxPdu_qu16);

#if (COM_ENABLE_PERIODIC_FRAME_SYNC != STD_OFF)
LOCAL_INLINE void Com_Prv_MainFunctionTxModeProcessing  (uint16_least idTxPdu_qu16);

LOCAL_INLINE void Com_Prv_MainFunctionTxReloadTimePeriod(uint16_least idTxPdu_qu16);
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
 Function name    : Com_Prv_InternalMainFunctionTx
 Description      : Service to perform the processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : idTxMainFunc_uo - Internal Tx-ComMainFunction Id
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_InternalMainFunctionTx(Com_MainFunc_tuo idTxMainFunc_uo)
{

    /* TRACE[SWS_Com_00665]A call to Com_MainFunctionTx shall simply return if the AUTOSAR COM module was not
     * previously initialized with a call to Com_Init.
     * TRACE[SWS_Com_00804] Error code(COM_E_UNINIT) is not reported,
     * if Com_MainFunctionTx service is called
     * before the AUTOSAR COM module was initialized with Com_Init or after a call to Com_DeInit*/
    if (Com_InitStatus_en != COM_UNINIT)
    {
#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
        Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#endif
        Com_TxIpduRam_tpst          txIpduRamPtr_pst;
        uint16_least                idxIdTxIpdu_qu16;
        uint16_least                numOfIpdus_qu16;
        boolean                     sendIPdu_b;

#ifdef COM_TxIPduTimeOut
        boolean                     timeoutFlag_b;
#endif

#ifdef COM_TxIPduNotification
        boolean                     notifyFlag_b;
#endif

    /* Initializing Flags */
#ifdef COM_TxIPduTimeOut
        timeoutFlag_b = COM_FALSE;
#endif

#ifdef COM_TxIPduNotification
        notifyFlag_b  = COM_FALSE;
#endif
        /* Get the first TxIPdu-Id to be processed for given ComMainFunction */
        idxIdTxIpdu_qu16    = COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (uint16)idTxMainFunc_uo).idFirstIpdu_uo;
        numOfIpdus_qu16     = (idxIdTxIpdu_qu16 + COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (uint16)idTxMainFunc_uo).numOfIpdus_uo);

#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
        txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idxIdTxIpdu_qu16);
#endif
        txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idxIdTxIpdu_qu16);

        for ( ; idxIdTxIpdu_qu16 < numOfIpdus_qu16; idxIdTxIpdu_qu16++ )
        {
            /* Check if IPdu group is started */
            if(Com_Prv_CheckTxIPduStatus((PduIdType)idxIdTxIpdu_qu16))
            {
                SchM_Enter_Com_TxIpduProtArea(MAINFUNCTIONTX);

                /* Check if TxConfirmation is received for the current TxIPdu */
                if (Com_GetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16))
                {
                    Com_SetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#ifdef COM_TxIPduNotification
                    /* If ComIPduSignalProcessing is DEFERRED
                     * signal indication / confirmations are deferred for example to a cyclic task(MainFunctionTx).
                     * hence notifications are invoked here */
                    notifyFlag_b = (Com_GetValue(TXIPDU,_SIGPROC,txIpduConstPtr_pcst->txIPduFields_u16) == COM_DEFERRED);
#endif /* #ifdef COM_TxIPduNotification */
                }
#ifdef COM_TxIPduTimeOut
                else
                {
                    /* Start: Tx Timeout Check */

                    timeoutFlag_b = Com_Prv_MainFunctionTx_IPduTimeOut(idxIdTxIpdu_qu16);

# ifndef COM_TxIPduTimeOutNotify
                    (void)timeoutFlag_b;
# endif /* # ifndef COM_TxIPduTimeOutNotify */

                    /*End: Tx Timeout Check*/
                }
#endif /* #ifdef COM_TxIPduTimeOut */

                sendIPdu_b = Com_Prv_MainFunctionTx_Modes(idxIdTxIpdu_qu16);

                SchM_Exit_Com_TxIpduProtArea(MAINFUNCTIONTX);

#ifdef COM_TxIPduTimeOutNotify
                /*Check if Timeout Notification function need to be called */
                if(timeoutFlag_b)
                {
                    timeoutFlag_b = COM_FALSE;
                    /* Call Ipdu Error Notification if configured
                     * This generated function will call the timeout notification callback functions of all signals and
                     * signal groups within this IPdu.
                     */
                    if(txIpduConstPtr_pcst->timeoutNotification_pfct != NULL_PTR)
                    {
                        txIpduConstPtr_pcst->timeoutNotification_pfct();
                    }
                }
                else
#endif /* #ifdef COM_TxIPduTimeOutNotify */
#ifdef COM_TxIPduNotification
                /*Check if Deferred Notification function need to be called */
                if(notifyFlag_b)
                {
                    notifyFlag_b = COM_FALSE;
                    /* Notification callback function for IPDU.
                     * This function is generated through code generator.
                     * Individual signal and signal group notifications functions are called from within this function.
                     * If IPDU has configured TX notification callback funcion, this function
                     * is also called from the generated function
                     */
                    if(txIpduConstPtr_pcst->notification_pfct != NULL_PTR)
                    {
                        txIpduConstPtr_pcst->notification_pfct();
                    }
                }
                else
#endif /* #ifdef COM_TxIPduNotification */
                {
                    /*Do nothing*/
                }

                /* Send the IPdu */
                if (sendIPdu_b)
                {
                    Com_Prv_MainFunctionTx_SendIPdu(idxIdTxIpdu_qu16);
                }

            } /* end of IPdu group started check */
#if (COM_ENABLE_PERIODIC_FRAME_SYNC != STD_OFF)
            else if (Com_GetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16))
            {
                Com_Prv_MainFunctionTxModeProcessing(idxIdTxIpdu_qu16);
            }
            else
            {
                /* Do nothing */
            }
#endif

#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
            txIpduConstPtr_pcst++ ;
#endif
            txIpduRamPtr_pst++;
        } /* end of for loop */
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_TxIPduTimeOut
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_IPduTimeOut
 Description      : Service to perform the IPdu Timeout processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_IPduTimeOut(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)
    /* Local pointer to hold the address of the Tx ipdu static configuration */
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    boolean                     isCancelTransmitSupported_b;
#endif
    boolean                     timeoutFlag_b;

    timeoutFlag_b = COM_FALSE;

#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_qu16);
#endif
    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    /* Handle Tx timeout if enabled */

    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
    * HIS-LEVEL parameter will be violated if more conditional statements are added.
    * Hence the warning is suppressed. */
    if((Com_GetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16) == COM_FALSE) &&
       (Com_GetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16) == COM_START))
    {

        /*Reduce Tick Timeout*/
        if (txIpduRamPtr_pst->cntrTxTimeout_u16 != COM_ZERO)
        {
            --txIpduRamPtr_pst->cntrTxTimeout_u16;
        }
        if(txIpduRamPtr_pst->cntrTxTimeout_u16 == COM_ZERO)
        {
            timeoutFlag_b = COM_TRUE; /*Timeout Notification need to be called for this Ipdu*/

            /*This Flag has to be set to START only in SendIpdu()i.e just before Sending IPDU*/
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_STOP);

#if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON)
            /* TRACE[SWS_Com_00775] If ComRetryFailedTransmitRequests is enabled and
             * a transmission deadline monitoring timeout occurs for an I-PDU,
             * the AUTOSAR COM module shall expire any pending transmission request for this I-PDU. */
            Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif

            /* TRACE[SWS_Com_00392] If a transmission deadline monitoring timeout occurs before the
             * N-Times transmission is complete, then the AUTOSAR COM module shall not initiate
             * further transmissions for this N-Times transmission. */
            txIpduRamPtr_pst->cntrRepetitions_u8 = COM_ZERO;

            /* TRACE[SWS_Com_00469] [NOTE] the minimum delay timer is reset
             * if the transmission deadline monitoring timer expires */
            txIpduRamPtr_pst->cntrMinDelayTime_u16 = COM_ZERO;

#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)

            isCancelTransmitSupported_b = Com_GetValue(TXIPDU,_ISCANCELTRANSMITSUPPORTED,txIpduConstPtr_pcst->txIPduFields_u16);
            /* If the Pdu is of TP type and cancel transmit is supported for this pdu */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
             * HIS-LEVEL parameter will be violated if more conditional statements are added.
             * Hence the warning is suppressed. */
            /* Check is there ongoing transmission, then only call the cancel transmit */
            if((Com_GetValue(TXIPDU,_ISLARGEDATAPDU,txIpduConstPtr_pcst->txIPduFields_u16)) &&
               (isCancelTransmitSupported_b) &&
               (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16)))
            {

                /* Timeout occurred cancel the ongoing transmission.
                 * As per the specification, the AUTOSAR COM ignores the return code */
                ( void )PduR_ComCancelTransmit(COM_GET_TXIPDU_PDURID(idTxPdu_qu16));
            }
#endif /* #if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE) */
        }
    }
    return timeoutFlag_b;
}

#endif /* #ifdef COM_TxIPduTimeOut */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_Modes
 Description      : Service to perform the tranmission mode processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_Modes(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIPdu_b;

    sendIPdu_b     = COM_FALSE;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    /* Decrement the MDT timer, if timer is not already zero and also Transmission is not blocked because of
     * non receipt of Com_TxConfirmation().
     */
    if (txIpduRamPtr_pst->cntrMinDelayTime_u16 != COM_ZERO)
    {
        /* Decrement the MDT timer, if timer is not already zero */
        --txIpduRamPtr_pst->cntrMinDelayTime_u16;
    }

    switch(Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8))
    {
    case COM_TXMODE_DIRECT:
        sendIPdu_b = Com_Prv_MainFunctionTx_DirectMode(idTxPdu_qu16);
        break;

    case COM_TXMODE_MIXED:
        sendIPdu_b = Com_Prv_MainFunctionTx_MixedMode(idTxPdu_qu16);
        break;

    case COM_TXMODE_NONE:
        sendIPdu_b = Com_Prv_MainFunctionTx_NoneMode(idTxPdu_qu16);
        break;

    case COM_TXMODE_PERIODIC:
        sendIPdu_b = Com_Prv_MainFunctionTx_PeriodicMode(idTxPdu_qu16);
        break;

    default:
        /* Validations and code generation shall take care to generate the valid transmission mode.
         * In case, if the program control reach here, nothing shall be done
         * Hence it is left empty. */
        break;
    }

#if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON)
    /* TRACE[SWS_Com_00773] Trigger TxIPdu, if ComRetryFailedTransmitRequests is enabled and last call to
     * PduR_ComTransmit() returned E_NOT_OK. Retry failed tranmissions only if MDT has already expired */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status.
     * Evaluation(or no evaluation) of the 2nd operand does not impact the system behaviour,
     * Hence the warning is suppressed. */
    if ((txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO) &&
         Com_GetRamValue(TXIPDU,_RETRYFAILEDTXREQ,txIpduRamPtr_pst->txFlags_u16))
    {

        sendIPdu_b = COM_TRUE;
    }
#endif /* #if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON) */

    return sendIPdu_b;
}



/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_DirectMode
 Description      : Service to perform the Direct Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_DirectMode(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIPdu_b;

    sendIPdu_b     = COM_FALSE;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if (txIpduRamPtr_pst->cntrRepPeriod_u16 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrRepPeriod_u16;
    }
    /*This condition is for the DIRECT && N=0 && Triggered and Com_TriggerIPDUSend()*/
    if(txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO)
    {
        /* Check if MDT flag is TRUE*/
        if(Com_GetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16))
        {
            /*Call SendIpdu*/
            sendIPdu_b = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* the first check is for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup
             * /Com_SendDynSignal/Com_TriggerIpduSend */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of
             * intermediate variables to store and read here, may read obsolete value.
             * Evaluation(or no evaluation) shall not impact the system behaviour.
             * Hence suppressed. */
            if((txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO) &&
               (Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16)))
            {

                /*Call SendIpdu*/ /*This Send IPDU is for Direct frames of MIXED mode*/
                sendIPdu_b = COM_TRUE;
            }
            else
#endif/* #ifdef COM_TXPDU_DEFERREDPROCESSING */
            if((txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO) && (txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO))
            {
                /*Call SendIpdu*/
                sendIPdu_b = COM_TRUE;
            }
            else
            {

            }
        }
        if (txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO)
        {
            if (txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO)
            {
                txIpduRamPtr_pst->cntrRepPeriod_u16 = txIpduRamPtr_pst->currentTxModePtr_pcst->repetitionPeriod_u16;
            }
        }
    }
    return sendIPdu_b;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_MixedMode
 Description      : Service to perform the Mixed Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_MixedMode(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIPdu_b;

    sendIPdu_b     = COM_FALSE;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if (txIpduRamPtr_pst->cntrTimePeriod_u16 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrTimePeriod_u16;
    }
    if (txIpduRamPtr_pst->cntrRepPeriod_u16 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrRepPeriod_u16;
    }
    if(txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO)
    {
        /* Check if MDT flag is TRUE */
        if(Com_GetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16))
        {
            /*Call sendIPdu_b*/
            sendIPdu_b = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* the first check if for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup
             * /Com_SendDynSignal/Com_TriggerIpduSend */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
             * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
             * system behaviour. Hence suppressed. */
            if((txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO) &&
               (Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16)))
            {

                /*Call SendIpdu*/ /* This Send IPDU is for Direct frames of MIXED mode */
                sendIPdu_b = COM_TRUE;
            }
            else
#endif
            /* Regular N-times transmission */
            if((txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO) && (txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO))
            {
                sendIPdu_b = COM_TRUE;
                /*Call sendIPdu_b*/ /*This Send IPDU is for Direct frames of MIXED mode*/
            }
            else
            {
                if((txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)&&(txIpduRamPtr_pst->cntrRepetitions_u8 == COM_ZERO))
                {
                    sendIPdu_b = COM_TRUE;
                    /* Call sendIPdu_b*/ /* This Send IPDU is for Periodic Frames of MIXED mode */
                }
            }
        }
        if ((txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO) && (txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO))
        {
            txIpduRamPtr_pst->cntrRepPeriod_u16 = txIpduRamPtr_pst->currentTxModePtr_pcst->repetitionPeriod_u16;
        }
    }/*if(Com_MinDelayTick[TxPduId] == 0)*/
    else
    {
        if((txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO) && (txIpduRamPtr_pst->cntrRepetitions_u8 == COM_ZERO))
        {
            /* Usecase diagram 5b from Appendix A, postpone periodic transmission to respect MDT */
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
        }
    }
    if(txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)
    {
        /* Load tick for the Tx IPDU */
        txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_qu16, txIpduRamPtr_pst->currentTxModePtr_pcst);
    }
    return sendIPdu_b;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_NoneMode
 Description      : Service to perform the None Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_NoneMode(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIPdu_b;

    sendIPdu_b     = COM_FALSE;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if(txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO)
    {
        /* Check if MDT is TRUE */
        if(Com_GetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16))
        {
            /*Call sendIPdu_b*/
            sendIPdu_b = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* This part of code is executed when Com_TriggerIpduSend triggers transmission from NONE Ipdus */
            if(Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16))
            {
                if(txIpduRamPtr_pst->cntrRepPeriod_u16 != COM_ZERO)
                {
                    txIpduRamPtr_pst->cntrRepPeriod_u16--;
                }
                if(txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO)
                {
                    /* Trigger transmission for Ipdu */
                    sendIPdu_b = COM_TRUE;
                }
            }
#endif
        }
    }
    return sendIPdu_b;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_PeriodicMode
 Description      : Service to perform the Periodic Mode transmision processing of the AUTOSAR COM transmission
                    activities that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE boolean Com_Prv_MainFunctionTx_PeriodicMode(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     sendIPdu_b;

    sendIPdu_b     = COM_FALSE;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if (txIpduRamPtr_pst->cntrTimePeriod_u16 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrTimePeriod_u16;
    }
    if(txIpduRamPtr_pst->cntrMinDelayTime_u16 == COM_ZERO )
    {
        /* Check if MDT flag is TRUE*/
        if(Com_GetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16))
        {
            /*This condition is for the DIRECT && N=0 && Triggered and Com_TriggerIPDUSend()*/
            /*Call sendIPdu_b*/
            sendIPdu_b = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }
        /* the first check if for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup/
         * Com_SendDynSignal/Com_TriggerIpduSend */
        else
        {
            if(txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)
            {
                /*Call sendIPdu_b*/ /* This Send IPDU is for Periodic Frames */
                sendIPdu_b = COM_TRUE;
            }
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* This part of code is executed when Com_TriggerIpduSend triggers transmission from Periodic Ipdus */
            else if(Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16))
            {
                if (txIpduRamPtr_pst->cntrRepPeriod_u16 != COM_ZERO)
                {
                    txIpduRamPtr_pst->cntrRepPeriod_u16--;
                }
                if(txIpduRamPtr_pst->cntrRepPeriod_u16 == COM_ZERO)
                {
                    /* Trigger transmission for Ipdu */
                    sendIPdu_b = COM_TRUE;
                }
            }
            else
            {

            }
#endif /* #ifdef COM_TXPDU_DEFERREDPROCESSING */
        }
    }
    else
    {
        if(txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)
        {
            /* Usecase diagram 5b from Appendix A, Postponed periodic transmission to respect MDT */
            Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
        }
    }
    /*the Timer is started only if the timer is already not running */
    if(txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)
    {
        /* Load tick for the Tx IPDU */
        txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_qu16, txIpduRamPtr_pst->currentTxModePtr_pcst);
    }
    return sendIPdu_b;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_SendIPdu
 Description      : Service to initiate transmission to PDUR
 Parameter        : idTxPdu_qu16 -> Tx IPDU ID
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_MainFunctionTx_SendIPdu(uint16_least idTxPdu_qu16)
{
    Com_SendIpduInfo_tst sendIpduFlag_st = COM_CLEAR_SENDIPDU_FLAGS;

    /* The bit field is loaded as below
     * 1. Call is from proc function,so COM_RESET
     * 2. No mode change happens from Com_MainFunctionTx ,so COM_RESET
     * 3. Signal transfer property information is not used, so COM_RESET
     * 4. Timeout monitoring is done for PERIODIC and MIXED frames.
     * */
    /* sendIpduFlag_u16.isEventTrig = COM_RESET */

    /* In case of Tp large pdu: If already a transmission request has been place, then do not trigger new request
     * In case of Normal pdu: Then this flag will never be set, so always Com_Prv_SendIpdu() will be called */
#ifdef COM_TP_IPDUTYPE
    if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,COM_GET_TXPDURAM_S(idTxPdu_qu16).txFlags_u16) == COM_FALSE)
#endif
    {
        Com_Prv_SendIpdu((PduIdType)idTxPdu_qu16,sendIpduFlag_st);
    }
}

#if (COM_ENABLE_PERIODIC_FRAME_SYNC != STD_OFF)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTxModeProcessing
 Description      : Service to perform the tranmission mode processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : idTxPdu_qu16 -> Tx IPDU ID
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_MainFunctionTxModeProcessing(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst        txIpduRamPtr_pst;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if( COM_TX_MODE_IS_CYCLIC(Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8)) )
    {
        Com_Prv_MainFunctionTxReloadTimePeriod(idTxPdu_qu16);
    }
    else
    {
        /* Program control reach here for DIRECT or NONE transmission mode, nothing shall be done.
         * Hence it is left empty. */
    }
}

/*
 **********************************************************************************************************************
 Function name    : Com_Prv_MainFunctionTxReloadTimePeriod
 Description      : Service to perform the reduce the periodic tick value and when it reaches to zero
                    reload it back with configured periodic tick value.
 Parameter        : idTxPdu_qu16 -> Tx IPDU ID
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_MainFunctionTxReloadTimePeriod(uint16_least idTxPdu_qu16)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRam_tpst        txIpduRamPtr_pst;
    txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_qu16);

    if (txIpduRamPtr_pst->cntrTimePeriod_u16 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrTimePeriod_u16;
    }
    /*the Timer is started only if the timer is already not running */
    if(txIpduRamPtr_pst->cntrTimePeriod_u16 == COM_ZERO)
    {
        /* Load tick for the Tx IPDU */
        txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEPERIOD(idTxPdu_qu16, txIpduRamPtr_pst->currentTxModePtr_pcst);
    }
}
#endif /* #if (COM_CONTROL_TXIPDU_RUNTIME == STD_ON) */

/* FC_VariationPoint_START */
/*
 **********************************************************************************************************************
 Function name    : Com_IsTxScheduled
 Description      : this function checks whether the Tx frame is scheduled to be sent in the coming cycle
 Parameter        : idTxPdu_uo
                    callerTaskCycleInMs_u16- this parameter is the value of caller's task cycle in ms.
                                        i.e : if the Caller is in 1ms, then the ComCallerTaskCycle = 1
 Return value     : boolean

 To Be considered before using this function
 ----------------------------------------------------------------------------------------------------------------------
 1. this function shall be used for only Periodic type Ipdu and not for other Pdu types

 When Com_IsTxScheduled is called in different task cycle as compared to MainFunctionTx:
 In this scenario, an additional parameter(ComCallerTaskCycle) has to be passed by the caller.
 lets say,
  Tb : Time raster of MainFunctionTx
  Tc : Time raster of caller of Com_IsTxScheduled(ComCallerTaskCycle)
 Time raster passed by the caller is in ms, hence the ticks are converted into time in ms,
 multiplying ticks by Tb will serve the purpose.
 After the conversion, the value hence obtained is the remaining time for Ipdu transmission in ms

 There are three cases observed in this scenario, presented below
 Case 1: (Tb = Tc)
 If Com_IsTxScheduled is called in the same raster as MainFunctionTx,
 then if remaining time is greater than caller time, the caller can wait for another raster to update IPdu data
 otherwise Ipdu data update can happen in current raster.

 Case 2: (Tb < Tc)
 If Com_IsTxScheduled is called slower than MainFunctionTx,
 then if remaining time is greater than caller time, the caller can wait for another raster to update IPdu data
 otherwise Ipdu data update can happen in current raster.

 Case 3: (Tb > Tc)
 If Com_IsTxScheduled is called faster than MainFunctionTx,
 Here the Com internal ticks would not have enough resolution to acknowledge the last raster before transmission.
 Hence when the Com ticks reach value 1, the caller can do Ipdu data update in the current raster.

 Scenarios analysed:
 1. For 10ms frame, the Com_Tick_Tx will be always 1 as the MainFunctionTx (function above) loads the
 value at the end of MainFunctionTx. Hence the If condition will fail and function will always return TRUE
 2. For >10ms frame, the Com_Tick_Tx will never have 0 value as the value is loaded at the end of the
 MainFunctionTx. When the Tick value becomes 1, this function will return true and SCL processing
 will happen.
 3. for >10ms Frame, if minimum delay is configured, then whever the value of Mindelay is 1, this function
 will return TRUE as the frame can have request for transmission and the next transmission criteria of minimum delay
 will be over in the subsequent call of MainFunctionTx (min delay becomes 0 and can have any transmission based on
 signal or transmission mode)
 4. For any frame, if transmission mode is other than 'periodic' then
 this service will return TRUE always to be on the safer side.
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
boolean Com_IsTxScheduled(PduIdType idTxPdu_uo, uint16 callerTaskCycleInMs_u16)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE
    uint32                      ticksInMs_u32;
    Com_TimeBase_tuo            timeBaseInMs_uo;
#endif
    boolean                     isTxScheduled_b;

    isTxScheduled_b = COM_TRUE;

#ifndef COM_TXSCHEDULED_DIFF_TIMEBASE
    (void)callerTaskCycleInMs_u16;
#endif

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_IsTxScheduled, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idTxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_IsTxScheduled, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idTxPdu_uo   = COM_GET_TX_IPDU_ID(idTxPdu_uo);

        txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idTxPdu_uo);

       /* If the Pdu has varied Transmission mode, then it is safe to return TRUE always as Transmission mode selecion
        * happens based on the signal filter algo and if the function returns true at wrong time there could be chances
        * that the signal update could be missed and transmission mode cannot happen (Autosar Com Use case 3)
        * This check has to the first check as this has the highest priority, this part checks whether
        * the configured Pdu is Periodic and it is scheduled to be Transmitted in the coming cycle */
        if (Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8) == COM_TXMODE_PERIODIC)
        {
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE

            /* Configured time-base is stored in ms.(i.e 1ms = 1) */
            timeBaseInMs_uo = COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo))->idMainFunc_uo).timeBaseInMs_uo;

            /* In normal scenario, it is not intended that caller of Com_IsTxScheduled is called in lesser task cycle than
             * MainFunctionTx. Check if this is the case. */
            if (callerTaskCycleInMs_u16 < timeBaseInMs_uo)
#endif
            {
                /* Then return true when txIpduRamPtr_pst->Com_Tick_Tx = 1 or 0.
                 * Hence caller has to update the data a few cycles earlier */
                if (txIpduRamPtr_pst->cntrTimePeriod_u16 > COM_ONE)
                {
                    isTxScheduled_b = COM_FALSE;
                }
                else
                {
                    /* isTxScheduled_b = COM_TRUE */
                }
            }
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE
            else
            {
                /* The ticks are multiplied by the time in ms to get the remaining time in ms */
                ticksInMs_u32 = txIpduRamPtr_pst->cntrTimePeriod_u16;
                /* This service is invoked before call to MainFunctionTx in the OS task sequence, if Com computes
                 * the remaining time from un-decremented ticks, then time-value will be more than the actual remaining time.
                 * Hence remaining time is computed from the decremented tick counter locally,
                 * but the global tick counter will still be changed in MainFunctionTx. */
                ticksInMs_u32 = ((ticksInMs_u32 != COM_ZERO) ?
                                ((ticksInMs_u32 - COM_ONE) * timeBaseInMs_uo) : (uint32)COM_ZERO);

                /* If remaining time for scheduled periodic transmission happens to be less than the callers cycle,
                 * then return true to let the caller update the data */
                if (ticksInMs_u32 > callerTaskCycleInMs_u16)
                {
                    isTxScheduled_b = COM_FALSE;
                }
                else
                {
                    /* isTxScheduled_b = COM_TRUE */
                }
            }
#endif /* #ifdef COM_TXSCHEDULED_DIFF_TIMEBASE */
        }
        else
        {
            /* To be on the safer side, function return 'true' for other transmission modes */
            /* isTxScheduled_b = COM_TRUE */
        }
    }

    return isTxScheduled_b;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* FC_VariationPoint_END */


/*
 **********************************************************************************************************************
 Function name    : COM_ProvideTxIpduStatus
 Description      : The service Com_ProvideTxIpduStatus returns the status of Tx I-PDU referenced by PduId.
 Parameter        : idPdu_uo
 Return value     : boolean
 **********************************************************************************************************************
*/
#ifdef COM_PROVIDE_IPDU_STATUS

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
boolean Com_ProvideTxIpduStatus(PduIdType idPdu_uo)
{
    boolean     pduStatus_b;

    pduStatus_b = COM_FALSE;

    /* If the Pdu Id passed is not an Tx Ipdu Id or Ipdu is not started, then status false is returned */
# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ProvideTxIpduStatus, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_ProvideTxIpduStatus, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idPdu_uo = COM_GET_TX_IPDU_ID(idPdu_uo);

        pduStatus_b = Com_Prv_CheckTxIPduStatus(idPdu_uo);
    }

    return(pduStatus_b);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_PROVIDE_IPDU_STATUS */

