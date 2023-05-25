

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
#ifdef COM_RxSigUpdateBit
LOCAL_INLINE boolean Com_Prv_ProcessSigUpdateBit(
                                        Com_SignalIdType    idSignal_u16,
                                        const PduInfoType * pduInfoPtr_pcst
                                                );
#endif

LOCAL_INLINE boolean Com_Prv_CopyRxSignal(
                                        Com_SignalIdType    idSignal_u16,
                                        const PduInfoType * pduInfoPtr_pcst,
                                        Com_Bitsize_tuo     size_uo
                                         );


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
 Function name    : Com_Prv_ProcessSignal
 Description      : Process rx-signals of the received I-PDU.
 Parameter        : idRxPdu_uo      - ID of the received I-PDU.
                  : pduInfoPtr_pcst - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_ProcessSignal(PduIdType idRxPdu_uo, const PduInfoType * pduInfoPtr_pcst)
{
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
#ifdef COM_RxSignalNotify
    Com_RxSigRam_tpst           rxSigRamPtr_pst;
#endif
    uint16_least                idxIdSig_qu16;
    uint16_least                idMaxRxSig_qu16;
#ifdef COM_RxSigUpdateTimeout
    uint16_least                idxSigTout_qu16;
    uint8                       ipduUpdateBitStatus_u8;
    uint8                       sigUpdateBitStatus_u8;
#endif
    /* This variable is used to decide whether to copy the new value of the signal into signal buffer or not */
    boolean                     processSignal_b;

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);

#ifdef COM_RxSigUpdateTimeout
    /* If this IPDU has signals with update-bits , initialise the pointers
     * Separate deadline monitoring should be performed for signals with update-bit
     * Deadline monitoring should not be done if it is disabled
     */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((rxIpduConstPtr_pcst->signalTimeoutRef_pcst != NULL_PTR) && (COM_CheckRxIPduDMStatus(idRxPdu_uo)))
    {

        /* Update the variable UpdateBitStatus with value COM_UPDATEBIT_APPLICABLE
         * ie at least one of the signal within the IPDU is configured with both update-bit and deadline monitoring.
         */
        ipduUpdateBitStatus_u8 = COM_UPDATEBIT_APPLICABLE;
    }
    else
    {
        ipduUpdateBitStatus_u8 = COM_UPDATEBIT_NOT_APPLICABLE;
    }

    /* Initialize the variable to be used to refer the first signal timeout configuration */
    idxSigTout_qu16 = COM_ZERO;
#endif /* #ifdef COM_RxSigUpdateTimeout */

    idxIdSig_qu16   = rxIpduConstPtr_pcst->idRxSig_uo;
    idMaxRxSig_qu16  = idxIdSig_qu16 + rxIpduConstPtr_pcst->numOfSig_u16;

    rxSigConstPtr_pcst   = COM_GET_RXSIG_CONSTDATA(idxIdSig_qu16);
#ifdef COM_RxSignalNotify
    rxSigRamPtr_pst     = &COM_GET_RXSIGNALFLAG(idxIdSig_qu16);
#endif

    for ( ; idxIdSig_qu16 < idMaxRxSig_qu16; idxIdSig_qu16++ )
    {
        /* Initialise to TRUE i.e., proceed further to process the received signal */
        processSignal_b = COM_TRUE;
#ifdef COM_RxSigUpdateTimeout
        sigUpdateBitStatus_u8 = ipduUpdateBitStatus_u8;
#endif

#ifdef COM_RxSigUpdateBit
        /* Returns false if signal UpdateBit is configured and is received with reset state,
         * otherwise true is returned */
        if (Com_Prv_ProcessSigUpdateBit( (Com_SignalIdType)idxIdSig_qu16, pduInfoPtr_pcst ))
#endif /* #ifdef COM_RxSigUpdateBit */
        {
            Com_Bitsize_tuo     size_uo;
            uint8               constByteValue_u8;
            uint8               sigType_u8;
            uint8               sigEndianess_u8;

            constByteValue_u8  = rxSigConstPtr_pcst->general_u8;

            sigType_u8            = Com_GetValue(GEN,_TYPE,constByteValue_u8);
            sigEndianess_u8       = Com_GetValue(GEN,_ENDIANESS,constByteValue_u8);

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            if (sigType_u8 != COM_UINT8_DYN)
#endif
            {
                PduLengthType       sigLastByteNo_uo;

                size_uo = rxSigConstPtr_pcst->bitSize_uo;

                /* Endian behavior: calculate the last byte the signal span in the Rx-IPdu buffer
                 * LITTLE/OPAQUE : MSB > LSB; i.e, signal last byte is same as the byte where MSB is configured.
                 * BIG           : LSB > MSB; i.e, signal last byte is same as the byte where LSB is configured.
                 */
                if (sigEndianess_u8 != COM_BIGENDIAN)
                {
                    Com_Bitposition_tuo     lastBitPosition_uo;

                    /* Currently COM_OPAQUE is same as COM_LITTLEENDIAN and
                     * its SignalLength in bytes is stored in BitSize */
                    lastBitPosition_uo = (rxSigConstPtr_pcst->bitPos_uo +
                                      ((sigType_u8 == COM_UINT8_N) ? ((Com_Bitposition_tuo)size_uo * 8u) : size_uo));

                    sigLastByteNo_uo   = (PduLengthType)((lastBitPosition_uo != COM_ZERO) ? ((lastBitPosition_uo - COM_ONE) >> 3u) : (COM_ZERO));
                }
                else
                {
                    /* COM_BIGENDIAN */
                    sigLastByteNo_uo   = (PduLengthType)(rxSigConstPtr_pcst->bitPos_uo >> 3u);
                }

                /* TRACE[SWS_Com_00574] When unpacking an I-PDU, the AUTOSAR COM module shall check the
                 * received data length(PduInfoPtr->SduLength) and
                 * unpack and notify only completely received signals via ComNotification.
                 * Check if the Signal is part of the received rx pdu length */
                if ((sigLastByteNo_uo + COM_ONE) > pduInfoPtr_pcst->SduLength)
                {
                    /* Signal is not received or received partially, Ignore further processing */
                    processSignal_b = COM_FALSE;
                }
            }
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            else
            {
                PduLengthType       startByteNo_uo;

                /* Calculate Dynamic Signal Start Position */
                startByteNo_uo = (PduLengthType)(rxSigConstPtr_pcst->bitPos_uo >> 3u);

                /* Check if Dynamic Signal is received with atleast one byte */
                size_uo = ((COM_GET_RXPDURAM_S(idRxPdu_uo).rxIPduLength_uo > startByteNo_uo) ?

                       /* Calculate its received length */
                       (Com_Bitsize_tuo)( COM_GET_RXPDURAM_S(idRxPdu_uo).rxIPduLength_uo - startByteNo_uo ) :

                       /* Design Decision: Even if 0 bytes are received for a dynamic signal in RxIPdu,
                        * configured Signal-ComNotification is always invoked.
                        * If it is not received, neither extract nor copy Dynamic Signal.
                        */
                       (COM_ZERO));
            }
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */

            /* If complete signal is received, proceed to unpack and copy signal */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
             * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
             * system behaviour. Hence suppressed. */
            if ((processSignal_b) && Com_Prv_CopyRxSignal((Com_SignalIdType)idxIdSig_qu16, pduInfoPtr_pcst, size_uo))
            {

#ifdef COM_RxSigUpdateTimeout
                /* If sigUpdateBitStatus_u8 is
                 * COM_UPDATEBIT_NOT_APPLICABLE(0x0), then it remains unchanged.
                 * COM_UPDATEBIT_APPLICABLE(0x1), then it is changed to COM_UPDATEBIT_SET(0x2) here. */
                sigUpdateBitStatus_u8 <<= COM_ONE;
#endif
            }
            else
            {
                processSignal_b = COM_FALSE;
            }
        } /* End of Com_Prv_ProcessSigUpdateBit() check */
#ifdef COM_RxSigUpdateBit
        else
        {
            processSignal_b = COM_FALSE;
        }
#endif

#ifdef COM_RxSigUpdateTimeout
        /* If update-bit and timeout is configured */
        if (sigUpdateBitStatus_u8 != COM_UPDATEBIT_NOT_APPLICABLE)
        {
            Com_Prv_LoadSigTimeoutValue( (Com_SignalIdType)idxIdSig_qu16, sigUpdateBitStatus_u8, &idxSigTout_qu16 );
        }
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSignalNotify
        /* Set the _SIGNOTIF flag to invoke configured signal-ComNotification */
        Com_SetRamValue(RXSIG,_SIGNOTIF,rxSigRamPtr_pst->rxSigRAMFields_u8,processSignal_b);
#else
        /* return value from CopyRxSignal() is not used, if Signal notifications are not configured */
        (void)(processSignal_b);
#endif

        rxSigConstPtr_pcst++;
#ifdef COM_RxSignalNotify
        rxSigRamPtr_pst++;
#endif

    } /* End of for loop */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_RxSigUpdateBit
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessSigUpdateBit
 Description      : Private function to validate signal UpdateBit
 Parameter        : idSignal_u16        - ID of rx-signal.
                  : pduInfoPtr_pcst     - Contains the length (SduLength) of the received I-PDU and
                                  a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : COM_FALSE   - UpdateBit is configured & received with reset state.
                    COM_TRUE    - updatebit is set(other cases).
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_ProcessSigUpdateBit(
                                            Com_SignalIdType    idSignal_u16,
                                            const PduInfoType * pduInfoPtr_pcst
                                                )
{
    Com_RxSigCfg_tpcst               rxSigConstPtr_pcst;
    boolean                          isUpdateBitSet_b;

    /* Initialize update bit to true */
    isUpdateBitSet_b = COM_TRUE;

    rxSigConstPtr_pcst  = COM_GET_RXSIG_CONSTDATA(idSignal_u16);

    /* Check for the update bit if configured */
    if (Com_GetValue(GEN,_UPDBITCONF,rxSigConstPtr_pcst->general_u8))
    {
        PduLengthType       updateBitByteNo_uo;

        updateBitByteNo_uo     = (PduLengthType)(rxSigConstPtr_pcst->updateBitPos_uo >> 3u);

        /* Start: Check if the Update bit is SET or NOT */
        if ((updateBitByteNo_uo + COM_ONE) <= pduInfoPtr_pcst->SduLength)
        {
            /* TRACE[SWS_Com_00802] If a signal or signal group with an attached update bit is received and
             * the update-bit is not set, the AUTOSAR COM module shall discard this signal.
             * If the signal has not been updated, the signal will not be routed via the signal gateway.
             * It will only be discarded.
             * If the RTE reads a signal with an associated cleared update-bit, the init value or
             * the last received value is returned. */
            isUpdateBitSet_b  = Com_GetUpdateBitValue(pduInfoPtr_pcst->SduDataPtr,
                                                      rxSigConstPtr_pcst->updateBitPos_uo,
                                                      updateBitByteNo_uo);
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
#endif /* #ifdef COM_RxSigUpdateBit */


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CopyRxSignal
 Description      : Private function to unpack and copy rx-signal
 Parameter        : idSignal_u16   - rx-signal to be processed
                    pduInfoPtr_pcst - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
                    size_uo       - received signal length
 Return value     : true  - signal is copied into buffer
                    false - signal is discarded
 **********************************************************************************************************************
*/

LOCAL_INLINE boolean Com_Prv_CopyRxSignal(
                                    Com_SignalIdType    idSignal_u16,
                                    const PduInfoType * pduInfoPtr_pcst,
                                    Com_Bitsize_tuo     size_uo
                                         )
{
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
    Com_SigMax_tuo              rxSigNewVal_uo;
    PduIdType                   idRxPdu_uo;
    Com_MainFunc_tuo            idRxMainFunc_uo;
    uint8                       constByteValue_u8;
    uint8                       sigType_u8;
    uint8                       sigEndianess_u8;
    boolean                     copySignal_b;

    rxSigConstPtr_pcst      = COM_GET_RXSIG_CONSTDATA(idSignal_u16);

    idRxPdu_uo              = rxSigConstPtr_pcst->idComIPdu_uo;
    rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);
    idRxMainFunc_uo         = rxIpduConstPtr_pcst->idMainFunc_uo; /* Fetch the Rx-MainFunction internal Id */

    constByteValue_u8       = rxSigConstPtr_pcst->general_u8;

    sigType_u8              = Com_GetValue(GEN,_TYPE,constByteValue_u8);
    sigEndianess_u8         = Com_GetValue(GEN,_ENDIANESS,constByteValue_u8);

    rxSigNewVal_uo          = COM_ZERO;
    copySignal_b            = COM_TRUE;

    /* 1. For signals with update bit;this part will be executed when the Update bit is SET
     * 2. For signals without update bit,process the signal further */
    /* Unpack the signal value */
/* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP
    if (sigType_u8 == COM_FLOAT64)
    {
        float64     rxSigNewVal_f64;

        rxSigNewVal_f64 = Com_Prv_UnPackFloatSignal( sigEndianess_u8, rxSigConstPtr_pcst->bitPos_uo, pduInfoPtr_pcst->SduDataPtr );

        Com_Get_SchM_Enter_Com_RxSigBuff(RXINDICATION)
        /* Update the received data into the signal buffer */
        COM_GET_SIGTYPEFLOAT64_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) = rxSigNewVal_f64;
        Com_Get_SchM_Exit_Com_RxSigBuff(RXINDICATION)
    }
    else
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */
/* FC_VariationPoint_END   */
    {
        /* Only UINT8_N and UINT8_DYN signals have type value greater than 8 */
        if ( !((sigType_u8 == COM_UINT8_N)
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
             ||(sigType_u8 == COM_UINT8_DYN)
#endif
            ))
        {
            rxSigNewVal_uo = Com_Prv_UnpackSignal( sigEndianess_u8, rxSigConstPtr_pcst->bitPos_uo, size_uo, pduInfoPtr_pcst->SduDataPtr,
                                                                                    ((sigType_u8 & COM_ONE) != COM_ZERO));
        }
        else
        {
            rxSigNewVal_uo = Com_Prv_UnpackOpaqueSignal(rxSigConstPtr_pcst->bitPos_uo, size_uo, pduInfoPtr_pcst->SduDataPtr);
        }

#if defined(COM_RxSigInvalid) || defined(COM_RxFilters)

#ifdef COM_SIGNALGATEWAY
        /* TRACE[SWS_Com_00872] skip validation & filtering for gateway signal */
        if (Com_GetValue(RXSIG,_ISGWSIG,rxSigConstPtr_pcst->rxSignalFields_u8) == COM_FALSE)
#endif
        {
            /* In the following function
             * 1. Perform data Invalidation processing (REPLACE or NOTIFY)
             * 2. Signal filter processing
             */
            copySignal_b = Com_Prv_ValidateRxSignal( idSignal_u16, &rxSigNewVal_uo );
        }

#endif /* #if defined(COM_RxSigInvalid) || defined(COM_RxFilters) */
    }

    /* Update the signal buffer,
     * For signals with    update bit : Signal update bit set + signal validation pass
     * For signals without update bit : Signal validation pass
     */
#if defined(COM_RxSigInvalid) || defined(COM_RxFilters)
    if (copySignal_b)
#endif /* #if defined(COM_RxSigInvalid) || defined(COM_RxFilters) */
    {
        /* Proceed to copy signal buffer only for non-zero size */
        if ((size_uo != COM_ZERO)
/* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP
        /* Signal copy is done above for FLOAT64 type signals, so below it is done for other types */
        && (sigType_u8 != COM_FLOAT64)
#endif
/* FC_VariationPoint_END   */
           )
        {
            Com_SigBuffIndex_tuo        idxSigBuff_uo;

            idxSigBuff_uo = rxSigConstPtr_pcst->idxSigBuff_uo;

            /* NOTE on Exclusive area COM_RXINDICATION:
             * If there is a possibility that Com_RxIndication() can be interrupted by
             * Com_ReceiveSignal() or Com_Init() function for any IPdu,
             * then this exclusive area needs to be defined in Com_Cfg_SchM.h file.
             */
            /* Copy the new value of the signal into signal buffer */

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            if (sigType_u8 != COM_UINT8_DYN)
#endif
            {
                Com_Get_SchM_Enter_Com_RxSigBuff(RXINDICATION)

                if (sigType_u8 != COM_UINT8_N)
                {
                    /* Signal copy is done for rx signals */
                    Com_Prv_UpdateRxSignalBuffer(rxSigConstPtr_pcst, rxSigNewVal_uo, idRxMainFunc_uo);
                }
                else
                {
                    /* UINT8_N signal */
                    Com_ByteCopy( &COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo,idxSigBuff_uo),
                                  (pduInfoPtr_pcst->SduDataPtr + (rxSigConstPtr_pcst->bitPos_uo >> 3u)), size_uo );
                }
                Com_Get_SchM_Exit_Com_RxSigBuff(RXINDICATION)

            }
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            else
            {
                Com_Get_SchM_Enter_Com_RxSigDynBuff

                /* UINT8_DYN signal */
                Com_ByteCopy( &COM_GET_SIGTYPEUINT8DYN_BUFF(idRxMainFunc_uo,idxSigBuff_uo),
                              (pduInfoPtr_pcst->SduDataPtr + (rxSigConstPtr_pcst->bitPos_uo >> 3u)), size_uo );

                Com_Get_SchM_Exit_Com_RxSigDynBuff
            }
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */
        }

#ifdef COM_SIGNALGATEWAY
        /* This part of code updates the Rx Gateway queue If the signal is updated in the signal buffer by one
         * of the below means
         * 1. Reception of Ipdu and thereby signal update after update bit processing
         */
        if (Com_GetValue(RXSIG,_ISGWSIG,rxSigConstPtr_pcst->rxSignalFields_u8))
        {
            /* If the signal is recieved first time after the last call of Com_MainFunctionRouteSignals then
             * 1. The fill bit per signal has to be set
             * 2. Queue entry has to be done for the signal
             * For the further receptions of the signal, Queue and signal fill bit are not set */
            Com_SetRamValue(RXSIG,_SIGNALFILLBIT,COM_GET_RXSIGNALFLAG(idSignal_u16).rxSigRAMFields_u8,COM_TRUE);

            /* This flag will tell if any GW signals in the Ipdu are updated */
            COM_GET_IS_GWSIG_UPDATED( idRxPdu_uo ) = COM_TRUE;
        }
#endif /* #ifdef COM_SIGNALGATEWAY */

    } /* if (copySignal_b) */

    return copySignal_b;
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ValidateRxSignal
 Description      : Internal function used for validating Rx signal
 Parameter        : idSignal_u16    -> Id of the Signal
                  : rxNewValPtr_puo -> Pointer to New Signal value
 Return value     : true  - Copy new signal value into signal buffer
                    false - Do not copy new signal value into signal buffer
 **********************************************************************************************************************
*/
#if defined(COM_RxSigInvalid) || defined (COM_RxFilters)

boolean Com_Prv_ValidateRxSignal(Com_SignalIdType idSignal_u16, Com_SigMax_tuo * rxNewValPtr_puo)
{
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;
#if defined (COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) && \
   (defined(COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
    Com_RxSigRam_tpst           rxSigRamPtr_pst;
#endif
    uint8                       rxSignalFields_u8;
    boolean                     isValidRxSig_b;

    isValidRxSig_b          = COM_TRUE;

    rxSigConstPtr_pcst      = COM_GET_RXSIG_CONSTDATA(idSignal_u16);

#if defined (COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) && \
   (defined(COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
    rxSigRamPtr_pst         = &COM_GET_RXSIGNALFLAG(idSignal_u16);
#endif
    rxSignalFields_u8 = rxSigConstPtr_pcst->rxSignalFields_u8;
    /*Check For the Data Invalid Action and the value*/
#ifdef COM_RxSigInvalid
    {
        uint8   invAction_u8;

        invAction_u8 = Com_GetValue(RXSIG,_INVACTION,rxSignalFields_u8);

        /* For Float signals the invalid value is type casted to UNIT32 before comaparing */
        /* Check if the signal has invalid value configured and the newly received value is invalid value */
        if ((invAction_u8 != COM_NONE) && (rxSigConstPtr_pcst->dataInvalidVal_u32 == *rxNewValPtr_puo))
        {
            if ( invAction_u8 == (uint8)COM_REPLACE )
            {
                Com_SigMax_tuo  rxSigVal_uo;

                /* Sign extension is performed on Init value during the below copy */
                /* Read the initial value */
                rxSigVal_uo=Com_Prv_GetRxSigInitValue(rxSigConstPtr_pcst);

#ifdef COM_RXSIG_INT64
                /* TRACE[SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue)
                 * of a signal to the size of its ComSignalType.
                 * Init_Val by default will be sign extended only for 32bits. Currently for COM_SINT64 signal types,
                 * if the signal is of Negative, it is required to perform the sign extension till 64bits explicity. */
                if ( Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8) == COM_SINT64 )
                {
                    rxSigVal_uo = (Com_SigMax_tuo)((sint64)((sint32)rxSigVal_uo));
                }
                else
                {
                    /* do nothing for other signal types */
                }
#endif /* #ifdef COM_RXSIG_INT64 */

                /* TRACE[SWS_Com_00681] The AUTOSAR COM module shall replace the signals value by its configured
                 * ComSignalInitValue. After the replacement, the normal signal processing like filtering and
                 * notification shall take place as if the ComSignalInitValue would have been received instead of
                 * the ComSignalDataInvalidValue */
                (*rxNewValPtr_puo) = rxSigVal_uo;
            }
            /* If invalid value is not configured ie if flag rxSignalFields.DataInvalidType is COM_NONE, then this
             * check is performed at the beginning itself.
             * Hence the other value will be COM_NOTIFY.
             */
#ifdef COM_RxSigInvalidNotify
            else
            {
                /* TRACE[SWS_Com_00680] The AUTOSAR COM module shall notify the RTE via the configured
                 * ComInvalidNotification function. In this case, no other signal processing like filtering or the
                 * normal signal indication shall take place. */
                /* Signal is invalid,and the Invalid Action is configured
                 * hence do not copy new value of signal into signal buffer */
                isValidRxSig_b = COM_FALSE;
                /* If this flag is set, do not call regular signal notification function */
                /* Call Invalid Notification */
                if(rxSigConstPtr_pcst->invalidNotification_pfct != NULL_PTR)
                {
                    rxSigConstPtr_pcst->invalidNotification_pfct();
                }
                /* In this case, the signal buffer should contain the previous value of the signal,
                 * leave the signal buffer unchanged
                 */
            }
#endif /* #ifdef COM_RxSigInvalidNotify */
        }
    }
#endif /* #ifdef COM_RxSigInvalid */

#ifdef COM_RxFilters
    {
        uint8   filterAlgo_u8;

        filterAlgo_u8 = Com_GetValue(RXSIG,_FILTRALG,rxSignalFields_u8);

        /* Proceed only if the signal filter is configured */
        if ( ( COM_FALSE != isValidRxSig_b ) && ( COM_NOTCONFIGURED != filterAlgo_u8 ) )
        {
            /* The signal is valid, hence perform filtering */
            /* Get The Return value of the Filter which need not be returned if signal is invalid */
            /* The last parameter is Information from where the call to Com_Prv_Filter happens */
            /* NOTE on last parameter:
            * 1.Currently the values for the parameter callContext_u8 are COM_TRUE/COM_FALSE
            * 2.It is used only for filter ONE_EVERYN */
#if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
            /* If the filter algo is COM_MASKED_NEW_DIFFERS_MASKED_OLD and the first valid reception of the signal after
             * the reception deadline monitoring timeout should not be filtered out */
            if(!((Com_GetRamValue(RXSIG,_DMFILTERING,rxSigRamPtr_pst->rxSigRAMFields_u8)) &&
                 (COM_MASKED_NEW_DIFFERS_MASKED_OLD == filterAlgo_u8)))
            {
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout)) */
                isValidRxSig_b = Com_Prv_Filter((uint32)(*rxNewValPtr_puo), filterAlgo_u8, rxSigConstPtr_pcst->idxFilter_uo,
                                                                                                     COM_TRUE);
#if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
            }
            else
            {
                /* Resetting this flag, for further reception of the signal, if Rx deadline monitoring timer elapses,
                 * again this flag shall be set in Com_MainFunctionRx() */
                Com_SetRamValue(RXSIG,_DMFILTERING,rxSigRamPtr_pst->rxSigRAMFields_u8,COM_FALSE);
            }
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout)) */
        }
    }
#endif /* #ifdef COM_RxFilters */

    return (isValidRxSig_b);
}
#endif /* #if defined(COM_RxSigInvalid) || defined (COM_RxFilters) */

