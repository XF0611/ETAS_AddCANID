

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#ifdef COM_TX_SIGNALGROUP
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
 Function name    : Com_SendSignalGroup
 Description      : Service copies the content of the associated shadow buffer to the associated
                    I-PDU buffer.Prior to this call, all group signals should be updated in the
                    shadow buffer by the call of Com_UpdateShadowSignal.
 Parameter        : idSignalGroup_u16 -> Id of the SignalGroup
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/COM_BUSY
 **********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
uint8 Com_SendSignalGroup(Com_SignalGroupIdType idSignalGroup_u16)
{
#ifdef COM_TP_IPDUTYPE
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst; /* Local pointer to Tx SignalGroup static configuration */
#endif
    uint8                           status_u8;           /* Return status */

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignalGroup, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxSignalGroupId(idSignalGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignalGroup, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idSignalGroup_u16    = COM_GET_TXSIGNALGRP_ID(idSignalGroup_u16);
#ifdef COM_TP_IPDUTYPE
        txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA( idSignalGroup_u16 );
#endif

        /* Proceed only if Large Data tranmission is not in Progress */
#ifdef COM_TP_IPDUTYPE
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,COM_GET_TXPDURAM_S(txSigGrpConstPtr_pcst->idComIPdu_uo).txFlags_u16))
        {
            status_u8 = COM_BUSY;
        }
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            status_u8 = Com_Prv_InternalSendSignalGroup(idSignalGroup_u16);
        }

    }
    return status_u8;
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InternalSendSignalGroup
 Description      : Service copies the content of the associated shadow buffer to the associated
                    I-PDU buffer.Prior to this call, all group signals should be updated in the
                    shadow buffer by the call of Com_UpdateShadowSignal.
                    This is an internal function to update the signal group.
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : idSignalGroup_u16 -> Internal Id of the SignalGroup (relevant for post-build)
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
uint8 Com_Prv_InternalSendSignalGroup(Com_SignalGroupIdType idSignalGroup_u16)
{
#ifdef COM_EffectiveSigGrpTOC
    /* Local pointer to hold the address of SignalGroup status structure */
    Com_TxSigGrpRam_tpst            txSigGrpRamPtr_pst;
#endif
    /* Local pointer to hold the address of Tx SignalGroup static configuration */
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
#if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY)
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
#endif /* #if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY) */
    PduIdType                       idIpdu_uo;
    uint8                           constByteValue_u8;
    uint8                           status_u8;              /* Return status */
    uint8                           isSigTriggered_u8;
#if (defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition) || defined(COM_EffectiveSigGrpTOC))
    uint8                           transferProperty_u8;
#endif

    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(idSignalGroup_u16);
    idIpdu_uo             = txSigGrpConstPtr_pcst->idComIPdu_uo;
    constByteValue_u8     = txSigGrpConstPtr_pcst->txSignalGrpFields_u8;
    status_u8             = E_OK;
    isSigTriggered_u8     = COM_RESET;

#if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY)
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
#endif /* #if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_EffectiveSigGrpTOC
    txSigGrpRamPtr_pst = &COM_GET_TXSIGGRPFLAG(idSignalGroup_u16);
#endif

    /* Pack: Shadow buffer to Ipdu Buffer */
    SchM_Enter_Com_TxIpduProtArea(SENDSIGNALGRP);

#ifdef COM_TX_SIGNALGROUP_ARRAY
    if (Com_GetValue(TXSIGGRP,_ARRAYACCESS,constByteValue_u8) == COM_FALSE)
#endif
    {
        Com_Prv_ShadowBuffToIpduBuff(idSignalGroup_u16);
    }
#ifdef COM_TX_SIGNALGROUP_ARRAY
    else
    {
        /* GroupSignal's are already updated into the IPduBuffer in Com_SendSignalGroupArray() */
    }
#endif

#ifdef COM_TxSigGrpUpdateBit
    /* Update bit if this bit is configured */
    if(Com_GetValue(TXSIGGRP,_UPDBITCONF,constByteValue_u8))
    {
        PduLengthType byteNo_uo;
        /*Find the Byte No, where the Update bit lies */
        byteNo_uo = (PduLengthType)((txSigGrpConstPtr_pcst->updateBitPos_uo)>>3u);
        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(txIpduConstPtr_pcst,txSigGrpConstPtr_pcst->updateBitPos_uo,byteNo_uo)
    }
#endif
    SchM_Exit_Com_TxIpduProtArea(SENDSIGNALGRP);

#if (defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition) || defined(COM_EffectiveSigGrpTOC))
    transferProperty_u8 = Com_GetValue(TXSIGGRP,_TP,constByteValue_u8);
#endif

#if defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition)
    /* Set variable isSigTriggered_u8, if SGs TP is Triggered or TriggeredWithoutRepetition */
    if (COM_SIG_TP_IS_TRIGGERED(transferProperty_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
    else
#endif
    {
#ifdef COM_EffectiveSigGrpTOC
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
         * to store and read here, may read obsolete value. Evaluation(or no evaluation) of 2nd operand
         * shall not impact the system behaviour. Hence suppressed. */
        /* If SGs TP is TriggeredOnChange or TriggeredWithoutRepetition set the variable isSigTriggered_u8 */
        if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(transferProperty_u8) &&
           (Com_GetRamValue(TXSIGGRP,_TRIGONCHNG,txSigGrpRamPtr_pst ->txSigGrpRAMFields_u8)))
        {

            isSigTriggered_u8 = COM_SET;
            /* Reset the flag, so that next transmission is not triggered unless one of the group signal value
             * changes
             */
            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,txSigGrpRamPtr_pst->txSigGrpRAMFields_u8,COM_FALSE);
        }
#endif /* #ifdef COM_EffectiveSigGrpTOC */
    }

    /* TRACE[SWS_Com_00334] The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs.
     * So the IPDU Group status is checked only after SignalGroup packing is done. */
    if(Com_Prv_CheckTxIPduStatus(idIpdu_uo))
    {
        /* All unused flags are reset here */
        Com_SendIpduInfo_tst sendIpduFlag_st = COM_CLEAR_SENDIPDU_FLAGS;

        /* Store if signal is triggered */
        sendIpduFlag_st.isSigTriggered_u1 = isSigTriggered_u8;

#ifdef COM_TxFilters
        /* Tx Filters are enabled and hence based on the TMC, TMS will be calculated.
         * Evaluate TMS only if IPDU doesnt have a fixed TM during configuration time and the signal contributes
         * to TMS calculation.
         */
        if(Com_GetValue(TXSIGGRP,_FILTEREVALREQ,constByteValue_u8))
        {
            /* If TMS changes, Com_Prv_SigGrpTxChangeMode() returns COM_TRUE */
            sendIpduFlag_st.isModeChangd_u1 = COM_BOOL_TO_UNSIGNED(Com_Prv_SigGrpTxChangeMode(idSignalGroup_u16));
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        sendIpduFlag_st.isTimeoutReq_u1 = COM_BOOL_TO_UNSIGNED(Com_GetValue(TXSIGGRP,_TOENABLED,constByteValue_u8));
#endif /* #ifdef COM_TxIPduTimeOut */

#if defined(COM_SigGrpTriggeredOnChangeWithoutRepetition) || defined(COM_SigGrpTriggeredWithoutRepetition)
        if (COM_SIG_TP_IS_WITHOUT_REPETITION(transferProperty_u8))
        {
            /* Set IpduFlag indicating SignalGroup's Tp is TriggeredOnChangeWithoutRepetition or
             * TriggeredWithoutRepetition (This flag stops to load NumberOfRepetitions) */
            sendIpduFlag_st.ignoreRepetitions_u1 = COM_SET;
        }
#endif
        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduId_tuo)idIpdu_uo, sendIpduFlag_st );

    }/* End of If IpduGroupStarted */
    else
    {
        status_u8 = COM_SERVICE_NOT_AVAILABLE;
    }

    return (status_u8);
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */


