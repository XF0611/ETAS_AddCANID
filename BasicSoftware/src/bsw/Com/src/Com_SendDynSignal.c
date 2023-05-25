

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)

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

#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
LOCAL_INLINE uint8 Com_Prv_SendDynSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16);
LOCAL_INLINE uint8 Com_Prv_CopyDynSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16);
#endif

#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
LOCAL_INLINE uint8 Com_Prv_CopyDynGroupSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16);
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
 Function name    : Com_SendDynSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : idSignal_u16 -> Id of the signal.
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
                  : length_u16 -> Length of the dynamic length signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/E_NOT_OK/COM_BUSY
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_SendDynSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16)
{
#ifdef COM_TP_IPDUTYPE
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#endif
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
#endif
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    Com_TxGrpSigCfg_tpcst       txGrpSigConstPtr_pcst;
#endif
    PduIdType                   idIpdu_uo;
    uint16                      type_u16;
    uint8                       status_u8;
    /* Initialize variable */
    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendDynSignal, COM_E_UNINIT);
    }
    else if (signalDataPtr_pcv == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendDynSignal, COM_E_PARAM_POINTER);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
    /* Check for dynamic signal */
    if(Com_Prv_IsValidTxSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16        = COM_GET_TXSIGNAL_ID(idSignal_u16);
        txSigConstPtr_pcst  = COM_GET_TXSIG_CONSTDATA(idSignal_u16);
        idIpdu_uo           = txSigConstPtr_pcst->idComIPdu_uo;

# ifdef COM_TP_IPDUTYPE
        txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idIpdu_uo);
# endif

        type_u16            = Com_GetValue(GEN,_TYPE,txSigConstPtr_pcst->general_u8);

        /* Check whether the large data pdu flag has been reset */
# ifdef COM_TP_IPDUTYPE
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
        {
            status_u8 = COM_BUSY;
        }
        else
# endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            /* If the application requests with length greater than the maximum length of the signal */
            if((length_u16 <= txSigConstPtr_pcst->bitSize_uo) && ((uint8)COM_UINT8_DYN == type_u16))
            {
                status_u8 = Com_Prv_SendDynSignal(idSignal_u16, signalDataPtr_pcv, length_u16);
            }
            else
            {
                status_u8 = E_NOT_OK;
            }
        }
    }
    else
#endif /* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    /* Check for dynamic group signal */
    if(Com_Prv_IsValidTxGroupSignalId(idSignal_u16))
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idSignal_u16            = COM_GET_TXGRPSIGNAL_ID(idSignal_u16);
        txGrpSigConstPtr_pcst   = COM_GET_TXGRPSIG_CONSTDATA(idSignal_u16);

        type_u16                = Com_GetValue(TXGRPSIG,_TYPE,txGrpSigConstPtr_pcst->txGrpSigFields_u16);

        /* If the application requests with length greater than the maximum length of the signal */
        if((length_u16 <= txGrpSigConstPtr_pcst->bitSize_uo) && ((uint8)COM_UINT8_DYN == type_u16))
        {
            status_u8 = Com_Prv_CopyDynGroupSignal(idSignal_u16, signalDataPtr_pcv, length_u16);
        }
        else
        {
            status_u8 = E_NOT_OK;
        }
    }
    else
#endif /* #ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT */
    {
        /* Dynamic signal/dynamic group signal id is not exist,
         * hence return value is COM_SERVICE_NOT_AVAILABLE */
        COM_DET_REPORT_ERROR(COMServiceId_SendDynSignal, COM_E_PARAM);
    }

    return status_u8;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_SendDynSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : idSignal_u16 -> Id of the signal.
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
                  : length_u16 -> Length of the dynamic length signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
LOCAL_INLINE uint8 Com_Prv_SendDynSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16)
{
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
#if defined(COM_TxFilters) || defined(COM_TxIPduTimeOut) || defined(COM_SigTriggeredWithoutRepetition)
    uint16                      constByteValue_u16; /* Variable to hold Signal property */
#endif
    PduIdType                   idIpdu_uo;
    uint8                       status_u8;         /* Return status */
    uint8                       isSigTriggered_u8;

    status_u8           = E_OK;

    txSigConstPtr_pcst  = COM_GET_TXSIG_CONSTDATA(idSignal_u16);
    idIpdu_uo           = txSigConstPtr_pcst->idComIPdu_uo;

#if defined(COM_TxFilters) || defined(COM_TxIPduTimeOut) || defined(COM_SigTriggeredWithoutRepetition)
    /* Get signal property to variable */
    constByteValue_u16  = txSigConstPtr_pcst->txSignalFields_u16;
#endif

    isSigTriggered_u8   = Com_Prv_CopyDynSignal(idSignal_u16, signalDataPtr_pcv, length_u16);

    /* TRACE[SWS_Com_00334] The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs.
     * So the IPDU Group status is checked only after Signal packing is done. */
    if(Com_Prv_CheckTxIPduStatus(idIpdu_uo))
    {
        /* All unused flags are reset here */
        Com_SendIpduInfo_tst sendIpduFlag_st = COM_CLEAR_SENDIPDU_FLAGS;

        /* Store if signal is triggered */
        sendIpduFlag_st.isSigTriggered_u1 = isSigTriggered_u8;

#ifdef COM_TxFilters
        /* The Dynamic signal can have ALWAYS/NEVER as filter algorithm.
         * Evaluate TMS only if IPDU doesnt have a fixed Transmission Mode during configuration time and
         * the signal contributes to TMS calculation
         */
        if(Com_GetValue(TXSIG,_FILTRALG,constByteValue_u16) != (uint16)COM_NOTCONFIGURED)
        {
             /* Com_SigTxChangeMode() returns COM_TRUE, if TMS changes,
             * Here the value sent is '0' as filter algorithm can only be always/never/One_every_n */
            if(Com_Prv_SigTxChangeMode(idSignal_u16,COM_ZERO))
            {
                sendIpduFlag_st.isModeChangd_u1 = COM_SET;
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        /* Check if signal-timeout is configured */
        sendIpduFlag_st.isTimeoutReq_u1 = COM_BOOL_TO_UNSIGNED(Com_GetValue(TXSIG,_TOENABLED,constByteValue_u16));
#endif

#ifdef COM_SigTriggeredWithoutRepetition
        if (COM_TRIGGERED_WITHOUT_REPETITION == Com_GetValue(TXSIG,_TP,constByteValue_u16))
        {
            /* Set the flag to indicate Signal TP is TRIGGERED_WITHOUT_REPETITION
             * (This flag stops the loading of NumberOfRepetitions) */
            sendIpduFlag_st.ignoreRepetitions_u1 = COM_SET;
        }
#endif /* COM_SigTriggeredWithoutRepetition */

        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduId_tuo)idIpdu_uo, sendIpduFlag_st );

    }/*End of If IpduGroupStarted*/
    else
    {
        status_u8 = COM_SERVICE_NOT_AVAILABLE;
    }
    return status_u8;
}
#endif /* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */

#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyDynSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : idSignal_u16        -> Id of the signal.
                  : signalDataPtr_pcv   -> The pointer to the address where the application data is available.
                  : length_u16          -> Length of the dynamic length signal
 Return value     : COM_SET/COM_RESET
 **********************************************************************************************************************
*/
LOCAL_INLINE uint8 Com_Prv_CopyDynSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16)
{

    /* Local pointer to hold the Tx ipdu status information */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    PduLengthType               byteNo_uo;
    /* Variable to hold Signal property */
    PduIdType                   idIpdu_uo;
#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)
    /* Variable to hold the signal transfer property */
    uint8                       txSigTP_u8;
#endif
    uint8                       isSigTriggered_u8;

    isSigTriggered_u8       = COM_RESET;

    txSigConstPtr_pcst      = COM_GET_TXSIG_CONSTDATA(idSignal_u16);

    idIpdu_uo               = txSigConstPtr_pcst->idComIPdu_uo;
    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idIpdu_uo);

    /* The locks here is to protect 1. The Tx Ipdu buffer data update, 2. Dynamic signal length_u16 update */
    SchM_Enter_Com_TxIpduProtArea(SENDDYNSIGNAL);

#ifdef COM_TxSigUpdateBit
    /*As per COM334: the signal has to be updated in Internal buffer even if IPDU Group is STOPPED*/
    /* Update bit if this bit is configured */
    if(Com_GetValue(GEN,_UPDBITCONF,txSigConstPtr_pcst->general_u8))
    {
        /* Find the byteNo_u8 of the IPDU buffer */
        byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->updateBitPos_uo >> 3u);
        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(txIpduConstPtr_pcst,txSigConstPtr_pcst->updateBitPos_uo,byteNo_uo)
    }
#endif
    if(length_u16 != COM_ZERO)
    {
        /* Find the byteNo_u8 of the IPDU buffer */
        byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->bitPos_uo >> 3u);
        /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pcv is defined as void pointer in the AUTOSAR
           specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
        Com_ByteCopy((txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo), (const uint8*)signalDataPtr_pcv, (uint32)length_u16);

    }

    /* The dynamic Length of the Ipdu, is updated here */
    txIpduRamPtr_pst->dynLength_u16 = length_u16;
    SchM_Exit_Com_TxIpduProtArea(SENDDYNSIGNAL);

#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)
    /* Get the Signal transfer property */
    txSigTP_u8 = (uint8)Com_GetValue(TXSIG,_TP,txSigConstPtr_pcst->txSignalFields_u16);

    if (COM_SIG_TP_IS_TRIGGERED(txSigTP_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
#endif
    return isSigTriggered_u8;
}
#endif /* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */


#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyDynGroupSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : idSignal_u16        -> Id of the signal.
                  : signalDataPtr_pcv   -> The pointer to the address where the application data is available.
                  : length_u16          -> Length of the dynamic length signal
 Return value     : E_OK
 **********************************************************************************************************************
*/
LOCAL_INLINE uint8 Com_Prv_CopyDynGroupSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv, uint16 length_u16)
{

    /* Local pointer to hold the Tx ipdu status information */
    Com_TxIpduRam_tpst              txIpduRamPtr_pst;
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    PduIdType                       idIpdu_uo;
    Com_MainFunc_tuo                idTxMainFunc_uo;       /* Fetch the Tx-MainFunction internal Id */
    uint8                           status_u8;

    txGrpSigConstPtr_pcst   = COM_GET_TXGRPSIG_CONSTDATA(idSignal_u16);
    txSigGrpConstPtr_pcst   = COM_GET_TXSIGGRP_CONSTDATA(txGrpSigConstPtr_pcst->idSigGrp_uo);
    idIpdu_uo               = txSigGrpConstPtr_pcst->idComIPdu_uo;
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idIpdu_uo);
    txIpduConstPtr_pcst     = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    idTxMainFunc_uo         = txIpduConstPtr_pcst->idMainFunc_uo;
    status_u8               = E_NOT_OK;

    if(Com_GetValue(TXGRPSIG,_ISDYNGRPSIG,txGrpSigConstPtr_pcst->txGrpSigFields_u16))
    {
        /* The locks here is to protect 1. The Tx shadow buffer data update, 2. Dynamic group signal length update */
        SchM_Enter_Com_TxIpduProtArea(SEND_DYNGROUPSIGNAL);

        if(length_u16 != COM_ZERO)
        {
            /* MR12 RULE 11.5 VIOLATION: For signalDataPtr_pcv is defined as void pointer in the AUTOSAR
               specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
            Com_ByteCopy(&COM_GET_TXGRPSIGTYPEUINT8DYN_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                                                                   (const uint8*)signalDataPtr_pcv, (uint32)length_u16);

        }

        /* The dynamic Length of the Ipdu, is updated here */
        txIpduRamPtr_pst->dynTxGrpSigLength_u16 = length_u16;
        SchM_Exit_Com_TxIpduProtArea(SEND_DYNGROUPSIGNAL);
        status_u8 = E_OK;
    }
    return status_u8;
}
#endif /* #ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#endif /* #if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) */
