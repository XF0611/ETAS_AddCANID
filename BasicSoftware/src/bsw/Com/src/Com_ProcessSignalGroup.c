

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


/* This file contains signal groups processing from the received Pdu.
 * Hence complete file contents have to be placed inside this macro */
#ifdef COM_RX_SIGNALGROUP

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
#ifdef COM_RxSigGrpUpdateBit
LOCAL_INLINE boolean Com_Prv_ProcessSigGrpUpdateBit(
                                            Com_SignalGroupIdType       idSigGrp_u16,
                                            const PduInfoType *         pduInfoPtr_pcst
                                                   );
#endif

LOCAL_INLINE void Com_Prv_IpduBuffToSecondBuff(
                                            Com_SignalGroupIdType       idSigGrp_u16,
                                            const uint8 *               bufferPtr_pcu8,
                                            uint8                       validateSigGrp_u8
                                              );

#if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters)
LOCAL_INLINE uint8 Com_Prv_ValidateRxSignalGroup(Com_SignalGroupIdType idSigGrp_u16, const uint8 * bufferPtr_pcu8);

LOCAL_INLINE uint8 Com_Prv_ValidateRxGroupSignal(uint16_least idGrpSig_qu16, Com_SigMax_tuo rxNewValGrpSig_uo);
#endif

LOCAL_INLINE void Com_Prv_CopySecondaryBuffer(Com_RxSigGrpCfg_tpcst rxSigGrpConstPtr_pcst, const uint8 * buffer_pcu8);


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
 Function name    : Com_Prv_ProcessSignalGroup
 Description      : Process rx-signal groups of the received I-PDU.
 Parameter        : idRxPdu_uo      - ID of the received I-PDU.
                  : pduInfoPtr_pcst - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_ProcessSignalGroup(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst             rxIpduConstPtr_pcst;
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
#if defined(COM_SIGNALGROUPGATEWAY) || defined(COM_RxSignalGrpNotify)
    Com_RxSigGrpRam_tpst            rxSigGrpRamPtr_pst;
#endif
    uint16_least                    idxSigGrpId_qu16;
    uint16_least                    idMaxSigGrp_qu16;
    uint8                           validateSigGrp_u8;
#ifdef COM_RxSigGrpUpdateTimeout
    uint16_least                    idxSigTout_qu16;
    uint8                           ipduUpdateBitStatus_u8;
    uint8                           sigGrpUpdateBitStatus_u8;
#endif

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);

#ifdef COM_RxSigGrpUpdateTimeout
    /* If this IPDU has signal groups with update-bits , initialise the pointers
     * Separate deadline monitoring should be performed for signal groups with update-bit
     * Deadline monitoring should not be done if it is disabled
     */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst != NULL_PTR) && (COM_CheckRxIPduDMStatus(idRxPdu_uo)))
    {

        /* Update the variable UpdateBitStatus with value COM_UPDATEBIT_APPLICABLE. ie at least one of the signal group
         * within the IPDU is configured with both update-bit and deadline monitoring. */
        ipduUpdateBitStatus_u8 = COM_UPDATEBIT_APPLICABLE;
    }
    else
    {
        /* Reset the variable to default value ie COM_UPDATEBIT_NOT_APPLICABLE */
        ipduUpdateBitStatus_u8 = COM_UPDATEBIT_NOT_APPLICABLE;
    }

    /* Initialize the variable to be used to refer the first signal timeout configuration */
    idxSigTout_qu16 = COM_ZERO;
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

    idxSigGrpId_qu16  = rxIpduConstPtr_pcst->idFirstRxSigGrp_uo;
    idMaxSigGrp_qu16   = idxSigGrpId_qu16 + rxIpduConstPtr_pcst->numOfSigGrp_u16;

    rxSigGrpConstPtr_pcst  = COM_GET_RXSIGGRP_CONSTDATA(idxSigGrpId_qu16);

#if defined(COM_SIGNALGROUPGATEWAY) || defined(COM_RxSignalGrpNotify)
    rxSigGrpRamPtr_pst    = &COM_GET_RXSIGGRPFLAG(idxSigGrpId_qu16);
#endif

    /* Start processing for signal groups. */
    for ( ; idxSigGrpId_qu16 < idMaxSigGrp_qu16; idxSigGrpId_qu16++)
    {
        validateSigGrp_u8  = COM_SET;

#ifdef COM_RxSigGrpUpdateTimeout
        sigGrpUpdateBitStatus_u8 = ipduUpdateBitStatus_u8;
#endif

        /* SignalGroup is processed only if below conditions are satisfied:
         * 1. Update bit validation for signal groups with Update bit configured.
         * 2. Completely received SignalGroup TRACE[SWS_Com_00575].
         */
        if (
#ifdef COM_RxSigGrpUpdateBit
             (Com_Prv_ProcessSigGrpUpdateBit( (Com_SignalGroupIdType)idxSigGrpId_qu16, pduInfoPtr_pcst)) &&
#endif
             (rxSigGrpConstPtr_pcst->lastByte_uo <= pduInfoPtr_pcst->SduLength)
           )
        {
#if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters) || defined(COM_SIGNALGROUPGATEWAY)
            uint8   constByteValue_u8;
# ifdef COM_SIGNALGROUPGATEWAY
            boolean isGwSigGrp_b;
# endif
# if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters)
            boolean isValidationReq_b;
# endif
            constByteValue_u8 = rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8;

# ifdef COM_SIGNALGROUPGATEWAY
            isGwSigGrp_b      = Com_GetValue(RXSIGGRP,_ISGWSIGGRP,constByteValue_u8);
# endif

# if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters)
            isValidationReq_b =
#  ifdef COM_SIGNALGROUPGATEWAY
                /* TRACE[SWS_Com_00872] skip validation & filtering for gateway signal-group */
                (isGwSigGrp_b == COM_FALSE) &&
#  endif
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
                 * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
                 * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
                 * Hence the warning is suppressed. */
                ((Com_GetValue(RXSIGGRP,_INVACTION,constByteValue_u8) != COM_NONE)
#  ifdef COM_RxFilters
                 || (Com_GetValue(RXSIGGRP,_FILTEREVALREQ,constByteValue_u8))
#  endif
                );

            if (isValidationReq_b)
            {
                validateSigGrp_u8 = Com_Prv_ValidateRxSignalGroup((Com_SignalGroupIdType)idxSigGrpId_qu16,
                                                                               pduInfoPtr_pcst->SduDataPtr);
            }

            if (validateSigGrp_u8 != (uint8)COM_RESET)
# endif /* #if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters) */
#endif /* #if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters) || defined(COM_SIGNALGROUPGATEWAY) */
            {
                /* The processed data is copied into second buffer, because, if the API Com_Recivesignalgroup() is
                 * not interrupt safe there can be data corruption on the group signal values. */
                Com_Prv_IpduBuffToSecondBuff((Com_SignalGroupIdType)idxSigGrpId_qu16, pduInfoPtr_pcst->SduDataPtr,
                                                                                       validateSigGrp_u8);

#ifdef COM_SIGNALGROUPGATEWAY

                if (isGwSigGrp_b)
                {
                    /* This part of code updates the Rx Gateway queue If the signal group is to be updated into
                     * signalgroup buffer by one of the below means
                     * 1. Reception of Ipdu and thereby signal update after update bit processing
                     */
                    /* If the signal is recieved first time after the last call of Com_MainFunctionRouteSignals then
                     * 1. The fill bit per signal has to be set
                     * 2. Queue entry has to be done for the signal
                     * For the further receptions of the signal, Queue and signal fill bit are not set */
                    Com_SetRamValue(RXSIGGRP,_SIGNALFILLBIT,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8,COM_TRUE);

                    /* This flag will tell if any GW signal groups in the Ipdu are updated */
                    COM_GET_IS_GWSIG_UPDATED( idRxPdu_uo ) = COM_TRUE;
                }
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */

                /* If sigGrpUpdateBitStatus_u8 is
                 * COM_UPDATEBIT_NOT_APPLICABLE(0x0), then it remains unchanged.
                 * COM_UPDATEBIT_APPLICABLE(0x1), then it is changed to COM_UPDATEBIT_SET(0x2) here. */
#ifdef COM_RxSigGrpUpdateTimeout
                sigGrpUpdateBitStatus_u8 <<= COM_ONE;
#endif
            }
        }
        else
        {
            /* Either Update bit is received with reset state or signalgroup is partially received */
            validateSigGrp_u8 = COM_RESET;
        }

#ifdef COM_RxSigGrpUpdateTimeout
         /* If update bit and timeout is configured */
        if (sigGrpUpdateBitStatus_u8 != COM_UPDATEBIT_NOT_APPLICABLE)
        {
            Com_Prv_LoadSigGrpTimeoutValue((Com_SignalGroupIdType)idxSigGrpId_qu16, sigGrpUpdateBitStatus_u8, &idxSigTout_qu16);
        }
#endif

#ifdef COM_RxSignalGrpNotify
        /* Set _SIGNOTIF flag to invoke configured SignalGroup-ComNotification */
        Com_SetRamValue(RXSIGGRP,_SIGNOTIF,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8,(validateSigGrp_u8 != (uint8)COM_RESET));
#endif

        rxSigGrpConstPtr_pcst++;
#if defined(COM_SIGNALGROUPGATEWAY) || defined(COM_RxSignalGrpNotify)
        rxSigGrpRamPtr_pst++;
#endif

    }/* End of for loop */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_RxSigGrpUpdateBit
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessSigGrpUpdateBit
 Description      : Private function to validate signalgroup UpdateBit
 Parameter        : idSigGrp_u16    - ID of rx-signal group.
                  : pduInfoPtr_pcst  - Contains the length (SduLength) of the received I-PDU and
                                  a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : COM_FALSE   - UpdateBit is configured & received with reset state.
                    COM_TRUE    - updatebit is set(other cases).
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_ProcessSigGrpUpdateBit(
                                                 Com_SignalGroupIdType       idSigGrp_u16,
                                                 const PduInfoType *         pduInfoPtr_pcst
                                                   )
{
    Com_RxSigGrpCfg_tpcst            rxSigGrpConstPtr_pcst;
    boolean                          isUpdateBitSet_b;

    /* Initialize update bit to true */
    isUpdateBitSet_b = COM_TRUE;

    rxSigGrpConstPtr_pcst  = COM_GET_RXSIGGRP_CONSTDATA(idSigGrp_u16);

    /* Check if the signal group contained within this IPDU has update-bit configured */
    if (Com_GetValue(RXSIGGRP,_UPDBITCONF,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8))
    {
        PduLengthType   updateBitByteNo_uo;

        /* Find the byte No of this Update bit */
        updateBitByteNo_uo     = (PduLengthType)(rxSigGrpConstPtr_pcst->updateBitPos_uo >> 3u);

        /* Start: Check if the Update bit is SET or NOT */
        if ((updateBitByteNo_uo + COM_ONE) <= pduInfoPtr_pcst->SduLength)
        {
            /* TRACE[SWS_Com_00802] If a signal or signal group with an attached update bit is received and
             * the update-bit is not set, the AUTOSAR COM module shall discard this signal.
             * If the signal has not been updated, the signal will not be routed via the signal gateway.
             * It will only be discarded.
             */
            isUpdateBitSet_b  = Com_GetUpdateBitValue( pduInfoPtr_pcst->SduDataPtr,
                                                       rxSigGrpConstPtr_pcst->updateBitPos_uo,
                                                       updateBitByteNo_uo );
        }
        else
        {
            /* TRACE[SWS_Com_00794] In case of receiving of a smaller I-PDU than expected results into receiving a
             * signal without its configured update-bit, the AUTOSAR COM module shall treat this signal as if
             * its update-bit was set and interpret such a signal as updated.(SRS_Com_02046) */
        }
    } /* End of UpdateBit check */

    return isUpdateBitSet_b;
}

#endif /* #ifdef COM_RxSigGrpUpdateBit */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_IpduBuffToSecondBuff
 Description      : Internal function used for copying the signal group contents from IPdu buffer to second buffer
 Parameter        : idSigGrp_u16      -> Signal Group Id
                  : bufferPtr_pcu8    -> Pointer to Rx SignalGroup static configuration
                  : validateSigGrp_u8 -> Flag to tell if the group signal value should be copied
                                         with Initial values/received value
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_IpduBuffToSecondBuff(
                                            Com_SignalGroupIdType  idSigGrp_u16,
                                            const uint8 *          bufferPtr_pcu8,
                                            uint8                  validateSigGrp_u8
                                              )
{
    Com_RxSigGrpCfg_tpcst        rxSigGrpConstPtr_pcst;
    Com_MainFunc_tuo             idRxMainFunc_uo;

    rxSigGrpConstPtr_pcst = COM_GET_RXSIGGRP_CONSTDATA(idSigGrp_u16);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo     = (COM_GET_RX_IPDU_CONSTDATA(rxSigGrpConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;

# if !(defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters))
    (void)validateSigGrp_u8;
# endif

# if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters)
    if (validateSigGrp_u8 != COM_REPLACE_TO_INIT)
# endif
    {

# ifdef COM_RX_SIGNALGROUP_ARRAY
        if (Com_GetValue(RXSIGGRP,_ARRAYACCESS,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8))
        {
            Com_RxSigGrpArrayCfg_tpcst    rxSigGrpArrayConstPtr_pcst;
            uint8                       * rxSigGrpArrayBufRamPtr_pu8;

            rxSigGrpArrayConstPtr_pcst  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( rxSigGrpConstPtr_pcst->idxSigGrpArray_uo );

            rxSigGrpArrayBufRamPtr_pu8 =
                               &COM_GET_RXSIGGRP_ARRAY_BUFF( idRxMainFunc_uo, rxSigGrpArrayConstPtr_pcst->idxRxSigGrpBuf_uo );

            Com_Get_SchM_Enter_Com_RxSigGrpArrayBuff

            /* Copy GroupSignal's received data into RxSignalGroupArray Buffer from IPdu Buffer.
             * As it is UINT8 array copy, so RxSignalGroupArray Buffer can also hold other
             * interleaved Signal's received data of the same IPdu */
            Com_ByteCopy((uint8 *)(rxSigGrpArrayBufRamPtr_pu8),
                   (const uint8 *)(bufferPtr_pcu8 + rxSigGrpArrayConstPtr_pcst->firstByteNo_uo),
                                  (rxSigGrpConstPtr_pcst->lastByte_uo - rxSigGrpArrayConstPtr_pcst->firstByteNo_uo));

            Com_Get_SchM_Exit_Com_RxSigGrpArrayBuff
        }
        else
# endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */
        {
            Com_Prv_CopySecondaryBuffer(rxSigGrpConstPtr_pcst, bufferPtr_pcu8);
        }

    }
# if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters)
    else
    {
        /* Discard GroupSignal's received data and place its SignalInitValues in RxSignalGroupArray Buffer */
        Com_Prv_SigGrpReplaceToInit(rxSigGrpConstPtr_pcst, idRxMainFunc_uo);
    }
# endif /* # if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters) */
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopySecondaryBuffer
 Description      : Internal function used for copying the signal group contents from IPdu buffer to second buffer
 Parameter        : rxSigGrpConstPtr_pcst  -> Pointer to Rx SignalGroup static configuration
                  : buffer_pcu8            -> Address where the data is available
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_CopySecondaryBuffer(Com_RxSigGrpCfg_tpcst rxSigGrpConstPtr_pcst, const uint8 * buffer_pcu8)
{

    /* Local pointer to hold the address of Rx GroupSignal static configuration */
#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    Com_RxIpduRam_tpst              rxIpduRamPtr_pst;
#endif
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    PduLengthType                   startByteNo_uo;
    PduIdType                       idIpdu_uo;
    uint16                          dynGrpSigLength_u16;
#endif
    uint16_least                    idxGrpSig_qu16;
    Com_MainFunc_tuo                idRxMainFunc_uo;
    uint8                           sigType_u8;

    rxGrpSigConstPtr_pcst = COM_GET_RXGRPSIG_CONSTDATA(rxSigGrpConstPtr_pcst->idFirstGrpSig_uo);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo     = (COM_GET_RX_IPDU_CONSTDATA(rxSigGrpConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;
#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    idIpdu_uo           = rxSigGrpConstPtr_pcst->idComIPdu_uo;
    rxIpduRamPtr_pst    = &COM_GET_RXPDURAM_S(idIpdu_uo);
#endif

    Com_Get_SchM_Enter_Com_RxGrpSigSecBuff

    for (idxGrpSig_qu16 = rxSigGrpConstPtr_pcst->numOfGrpSig_uo; idxGrpSig_qu16 > COM_ZERO; idxGrpSig_qu16--)
    {
        sigType_u8 = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);

        if(sigType_u8 != (uint8)COM_UINT8_N)
        {
            uint8   sigEndianess_u8;

            sigEndianess_u8 = Com_GetValue(RXGRPSIG,_ENDIANESS,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);

/* FC_VariationPoint_START */
# ifdef COM_RXGRPSIG_FLOAT64SUPP
            if(sigType_u8 == COM_FLOAT64)
            {
                float64 rxNewVal_f64;

                rxNewVal_f64 = Com_Prv_UnPackFloatSignal( sigEndianess_u8, rxGrpSigConstPtr_pcst->bitPos_uo, buffer_pcu8 );

                COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = rxNewVal_f64;
            }
            else
# endif
/* FC_VariationPoint_END */
#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
            if (sigType_u8 == COM_UINT8_DYN)
            {
                /* Calculate Dynamic Signal Start Position */
                startByteNo_uo = (PduLengthType)(rxGrpSigConstPtr_pcst->bitPos_uo >> 3u);

                /* Check if Dynamic Signal has been received with atleast one byte */
                if (rxIpduRamPtr_pst->rxIPduLength_uo > startByteNo_uo)
                {
                    /* Calculate its received length */
                    dynGrpSigLength_u16 = (uint16)(rxIpduRamPtr_pst->rxIPduLength_uo - startByteNo_uo);
                }
                else
                {
                    /* Report it as 0, if it is not received yet */
                    dynGrpSigLength_u16 = (COM_ZERO);
                }
                Com_ByteCopy( &COM_GET_SECRXGRPSIGTYPEUINT8DYN_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo),
                              (buffer_pcu8 +(rxGrpSigConstPtr_pcst->bitPos_uo >> 3u)),
                              dynGrpSigLength_u16 );
                rxIpduRamPtr_pst->dynRxGrpSigLength_u16 = dynGrpSigLength_u16;
            }
            else
#endif
            {
                Com_SigMax_tuo  rxGrpSigNewVal_uo;

                rxGrpSigNewVal_uo = Com_Prv_UnpackSignal( sigEndianess_u8,
                                                           rxGrpSigConstPtr_pcst->bitPos_uo,
                                                           rxGrpSigConstPtr_pcst->bitSize_uo, buffer_pcu8,
                                                           ((sigType_u8 & COM_ONE)) != COM_ZERO);

                /* Now copy corresponding Group signal buffer with the received value */
                Com_Prv_UpdateRxGroupSignalSecondBuffer(rxGrpSigConstPtr_pcst, rxGrpSigNewVal_uo, idRxMainFunc_uo);
            }
        }
        else
        {
            Com_ByteCopy( &COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo),
                          (buffer_pcu8 +(rxGrpSigConstPtr_pcst->bitPos_uo >> 3u)),
                           rxGrpSigConstPtr_pcst->bitSize_uo );
        }
        rxGrpSigConstPtr_pcst++;
    }
    Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
}


#if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ValidateRxSignalGroup
 Description      : Private function to validate and process the configured filters for the given signal group
 Parameter        : idSigGrp_u16  - Signal Group Id to be validated
                    bufferPtr_pcu8 - IPdu buffer pointer
 Return value     : COM_RESET            - discard signalgroup
                    COM_SET              - valid signalgroup is received, copy into local buffer
                    COM_REPLACE_TO_INIT  - replace groupsignals with SignalInit values
 **********************************************************************************************************************
*/
LOCAL_INLINE uint8 Com_Prv_ValidateRxSignalGroup(Com_SignalGroupIdType idSigGrp_u16, const uint8 * bufferPtr_pcu8)
{
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
    Com_SigMax_tuo                  rxNewValGrpSig_uo;
    uint16_least                    idxIdGrpSig_qu16;
    uint16_least                    idMaxGrpSig_qu16;
    uint8                           constByteValue_u8;
    uint8                           validateSigGrp_u8;
    uint8                           sigType_u8;
#ifdef COM_RxFilters
    uint8                           filterAlgo_u8;
#endif

    validateSigGrp_u8      = COM_SET;
    rxNewValGrpSig_uo      = COM_ZERO;

    rxSigGrpConstPtr_pcst  = COM_GET_RXSIGGRP_CONSTDATA(idSigGrp_u16);

    idxIdGrpSig_qu16       = rxSigGrpConstPtr_pcst->idFirstGrpSig_uo;
    idMaxGrpSig_qu16       = idxIdGrpSig_qu16 + rxSigGrpConstPtr_pcst->numOfGrpSig_uo;

    rxGrpSigConstPtr_pcst  = COM_GET_RXGRPSIG_CONSTDATA(idxIdGrpSig_qu16);

    for( ; ((validateSigGrp_u8 == COM_SET) && (idxIdGrpSig_qu16 < idMaxGrpSig_qu16)); idxIdGrpSig_qu16++)
    {
        constByteValue_u8 = rxGrpSigConstPtr_pcst->rxGrpSigFields_u8;
#ifdef COM_RxFilters
        filterAlgo_u8 = rxGrpSigConstPtr_pcst->filterAlgo_u8;
#endif

        if ((Com_GetValue(RXGRPSIG,_INVACTION,constByteValue_u8))
#ifdef COM_RxFilters
           || (filterAlgo_u8 != COM_NOTCONFIGURED)
#endif
          )
        {
            sigType_u8 = Com_GetValue(RXGRPSIG,_TYPE,constByteValue_u8);

            /*Unpack the group signal value */
            if(sigType_u8 == COM_UINT8_N)
            {
                rxNewValGrpSig_uo = Com_Prv_UnpackOpaqueSignal(rxGrpSigConstPtr_pcst->bitPos_uo,rxGrpSigConstPtr_pcst->bitSize_uo,bufferPtr_pcu8);
            }
            /* FC_VariationPoint_START */
#ifdef COM_RXGRPSIG_FLOAT64SUPP
            else if(sigType_u8 == COM_FLOAT64)
            {
                /* Do nothing */
            }
#endif
            /* FC_VariationPoint_END */
            else
            {
                rxNewValGrpSig_uo = Com_Prv_UnpackSignal(Com_GetValue(RXGRPSIG,_ENDIANESS,constByteValue_u8),
                                                          rxGrpSigConstPtr_pcst->bitPos_uo,
                                                          rxGrpSigConstPtr_pcst->bitSize_uo, bufferPtr_pcu8,
                                                          ((sigType_u8 & COM_ONE) != COM_ZERO));
            }

            validateSigGrp_u8 = Com_Prv_ValidateRxGroupSignal(idxIdGrpSig_qu16, rxNewValGrpSig_uo);
        }

        rxGrpSigConstPtr_pcst++;
    }

#if (defined (COM_RxFilters) && defined (COM_F_MASKEDNEWDIFFERSOLD)) && \
                                                    (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout))
    /* Reset the DM filtering flag only if it was set during the previous timeout session */
    if(validateSigGrp_u8 != COM_RESET)
    {
        /* Resetting this flag which is maintained at SignalGroup level,
        * for further reception of the signal, if Rx deadline monitoring timer elapses,
        * again this flag shall be set in Com_MainFunctionRx()
        * This flag is resetted after processing all the ComGroupSignal inside this SignalGroup */
        Com_SetRamValue(RXSIGGRP,_DMFILTERING,COM_GET_RXSIGGRPFLAG(idSigGrp_u16).rxSigGrpRAMFields_u8,COM_FALSE);
    }
#endif /* #if (defined (COM_RxFilters) && defined (COM_F_MASKEDNEWDIFFERSOLD)) &&
                                                (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout)) */

    return validateSigGrp_u8;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ValidateRxGroupSignal
 Description      : check if RxGroupSignal is received with valid value
 Parameter        : idGrpSig_qu16            - Signal Group Id to be validated
                    rxNewValGrpSig_uo      - Rx GroupSignal value
 Return value     : COM_RESET               - invalid GroupSignal value is received and InvalidAction is notify
                    COM_REPLACE_TO_INIT     - invalid GroupSignal value is received and InvalidAction is replace
                    COM_SET                 - valid GroupSignal is received
 **********************************************************************************************************************
*/
LOCAL_INLINE uint8 Com_Prv_ValidateRxGroupSignal(uint16_least idGrpSig_qu16, Com_SigMax_tuo rxNewValGrpSig_uo)
{
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
#ifdef COM_RxSigGrpInvalid
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
#endif
#if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) && \
                             (defined(COM_RxIPduTimeout) || defined(COM_RxSigGrpUpdateTimeout))
    Com_RxSigGrpRam_tpst            rxSigGrpRamPtr_pst;
#endif
    Com_RxIntSignalGroupId_tuo      idSignalGrp_uo;
#ifdef COM_RxSigGrpInvalid
    uint8                           constByteValue_u8;
#ifdef COM_RXGRPSIG_FLOAT64SUPP
    uint8                           sigType_u8;
#endif
#endif
    uint8                           validateSigGrp_u8;

    validateSigGrp_u8 = COM_SET;

    rxGrpSigConstPtr_pcst   = COM_GET_RXGRPSIG_CONSTDATA(idGrpSig_qu16);
    idSignalGrp_uo          = rxGrpSigConstPtr_pcst->idSigGrp_uo;

#if !(defined(COM_RxSigGrpInvalid) || (defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) && \
                                      (defined(COM_RxIPduTimeout) || defined(COM_RxSigGrpUpdateTimeout))))
    (void)idSignalGrp_uo;
#endif

#if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) && \
   (defined(COM_RxIPduTimeout) || defined(COM_RxSigGrpUpdateTimeout))
    rxSigGrpRamPtr_pst    = &COM_GET_RXSIGGRPFLAG(idSignalGrp_uo);
#endif

#ifdef COM_RxSigGrpInvalid

    rxSigGrpConstPtr_pcst  = COM_GET_RXSIGGRP_CONSTDATA(idSignalGrp_uo);

    constByteValue_u8 = rxGrpSigConstPtr_pcst->rxGrpSigFields_u8;

#ifdef COM_RXGRPSIG_FLOAT64SUPP
    sigType_u8 = Com_GetValue(RXGRPSIG,_TYPE,constByteValue_u8);
#endif

    /* For Float signals the invalid value is type casted to UNIT32 before comparing */
    if ((Com_GetValue(RXGRPSIG,_INVACTION,constByteValue_u8))
        && (rxGrpSigConstPtr_pcst->dataInvalidVal_u32 == rxNewValGrpSig_uo)
        /* FC_VariationPoint_START */
#ifdef COM_RXGRPSIG_FLOAT64SUPP
        && (sigType_u8 != COM_FLOAT64)
#endif
        /* FC_VariationPoint_END */
        )
    {
        if (Com_GetValue(RXSIGGRP,_INVACTION,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8) == (uint8)COM_REPLACE)
        {
            /*  set the flag to Replace all the Group Signals(Com_RxNewVal_GrpSig) which is under
             * the same Signal Group with Initial value */
            validateSigGrp_u8 = COM_REPLACE_TO_INIT;
        }
#ifdef COM_RxSigGrpInvalidNotify
        else
        {
            /* If invalid action is NOTIFY */
            /* Check for invalid notification, only if SigGrpNotify_Flag is TRUE ie invalid notifcation
             * function not called yet..
             * The invalid notification function should be called only once for signal group
             */
            validateSigGrp_u8 = COM_RESET;

            /* Call Invalid notification for the whole Signal Group as soon as the
             * first group signal with invalid value is encountered
             */
            if(rxSigGrpConstPtr_pcst->invalidNotification_pfct != NULL_PTR)
            {
                rxSigGrpConstPtr_pcst->invalidNotification_pfct();
            }
        }
#endif /* #ifdef COM_RxSigGrpInvalidNotify */
        /* TRACE[SWS_Com_00680] SignalGroup is invalid, so no filtering process has to be done for this signal group
         * Break out of the for loop, as no more processing of group signals is required  */
    }
    else
#endif /* #ifdef COM_RxSigGrpInvalid */
    {
#ifdef COM_RxFilters
        uint8   filterAlgo_u8;
        boolean filterVal_b;

        /* The Groupsignal is valid, hence perform filtering */
        filterAlgo_u8 = rxGrpSigConstPtr_pcst->filterAlgo_u8;

        /* Proceed only if the GroupSignal is configured with a filter */
        if (filterAlgo_u8 != COM_NOTCONFIGURED)
        {
            /* Get return value of the filter which need not be returned if signal is invalid */
#if defined(COM_F_MASKEDNEWDIFFERSOLD) && (defined(COM_RxIPduTimeout) || defined(COM_RxSigGrpUpdateTimeout))
            if(!((Com_GetRamValue(RXSIGGRP,_DMFILTERING,rxSigGrpRamPtr_pst->rxSigGrpRAMFields_u8)) &&
                 (COM_MASKED_NEW_DIFFERS_MASKED_OLD == filterAlgo_u8)))
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout)) */
            {
                /* Call the Com_Prv_Filter API only if the conditions meet:
                 * 1. If the GroupSignal is not invalidated
                 * 2. This SignalGroup was not timed out */
                /* The last parameter is Information from where the call to Com_Prv_Filter happens */
                /* NOTE on last parameter:
                 * 1.Currently the values for the parameter callContext_u8 are COM_TRUE/COM_FALSE
                 * 2.It is used only for filter ONE_EVERYN */
                filterVal_b = Com_Prv_Filter((uint32)rxNewValGrpSig_uo,filterAlgo_u8,rxGrpSigConstPtr_pcst->idxFilter_uo,COM_TRUE);

                if(COM_FALSE == filterVal_b)
                {
                    /* TRACE[SWS_Com_00836]: Break out of the for loop, as no more processing of group signals is
                     * required and discard the whole signal group */
                    validateSigGrp_u8 = COM_RESET;
                }
            }
        }
#endif /* #ifdef COM_RxFilters */
    }
    return validateSigGrp_u8;
}
#endif /* #if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters) */

#endif /* #ifdef COM_RX_SIGNALGROUP */

