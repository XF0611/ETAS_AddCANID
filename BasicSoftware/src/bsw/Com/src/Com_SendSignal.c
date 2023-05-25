

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
LOCAL_INLINE Com_SigMax_tuo Com_Prv_SigBufftoIpduBuff(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv);


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
 Function name    : Com_SendSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter.
 Parameter        : idSignal_u16 -> Id of the signal.
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/COM_BUSY
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_SendSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv )
{
#ifdef COM_TP_IPDUTYPE
    Com_TxIpduRam_tpst              txIpduRamPtr_pst;  /* Local pointer to Tx IPdu   static configuration */
#endif
    uint8                           status_u8;        /* Return status */

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignal, COM_E_UNINIT);
    }
    else if (signalDataPtr_pcv == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignal, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    if(Com_Prv_IsValidTxSignalId(idSignal_u16))
    {
        /* Convert external HandleId to internal HandleId for Tx Signal
         * IMP : before accessing the tables, it is required to convert the IDs
         * For pre-compile , conversion is not required. Access Macro will return the same ID,
         * which is passed to it */
        idSignal_u16      = COM_GET_TXSIGNAL_ID(idSignal_u16);

#ifdef COM_TP_IPDUTYPE
        txIpduRamPtr_pst  = &COM_GET_TXPDURAM_S( COM_GET_TXSIG_CONSTDATA( idSignal_u16 )->idComIPdu_uo );

        /* Proceed only if Large Data tranmission is not in Progress */
        if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
        {
            /* The TxIPdu transmission of large Data IPdu is in progress, hence no signal update is allowed.
             * Until the transmission is completed. */
             status_u8 = COM_BUSY;
        }
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            status_u8 = Com_Prv_InternalSendSignal(idSignal_u16,signalDataPtr_pcv);
        }
    }
#ifdef COM_TX_SIGNALGROUP
    else if(Com_Prv_IsValidTxGroupSignalId(idSignal_u16))
    {
        /* Convert external HandleId to internal HandleId for Tx Signal
         * IMP : before accessing the tables, it is required to convert the IDs
         * For pre-compile , conversion is not required. Access Macro will return the same ID,
         * which is passed to it */
        idSignal_u16    = COM_GET_TXGRPSIGNAL_ID(idSignal_u16);

        Com_Prv_UpdateShadowSignal(idSignal_u16, signalDataPtr_pcv);
        status_u8       = E_OK;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */
    else
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignal, COM_E_PARAM);
    }

    return status_u8;

} /* End of Com_SendSignal() */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InternalSendSignal
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter.
                    This is an internal function to update the signal.
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : idSignal_u16 -> Internal Id of the signal. (Relevant for post-build case)
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_Prv_InternalSendSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv)
{
#ifdef COM_TxSigUpdateBit
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#endif
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_SigMax_tuo              txSigNewVal_uo;
    PduIdType                   idIpdu_uo;
#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition) || defined(COM_EffectiveSigTOC) ||\
    defined(COM_TxFilters)    || defined(COM_TxIPduTimeOut)
    uint16                      constTxSignalFields_u16;    /* Varible to hold the Signal Property */
#endif
#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition) || defined(COM_EffectiveSigTOC)
    uint8                       txSigTP_u8;         /* Varible to hold the Signal Transfer Property */
#endif
    uint8                       isSigTriggered_u8;
    uint8                       status_u8;             /* Return status */

    status_u8               = E_OK;
    isSigTriggered_u8       = COM_RESET;

    txSigConstPtr_pcst      = COM_GET_TXSIG_CONSTDATA(idSignal_u16);
    idIpdu_uo               = txSigConstPtr_pcst->idComIPdu_uo;
#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition) || defined(COM_EffectiveSigTOC) ||\
    defined(COM_TxFilters)    || defined(COM_TxIPduTimeOut)
    constTxSignalFields_u16 = txSigConstPtr_pcst->txSignalFields_u16; /* Load the signal property */
#endif

#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition) || defined(COM_EffectiveSigTOC)
    /* Load the Signal Transfer Property */
    txSigTP_u8              = (uint8)Com_GetValue(TXSIG,_TP,constTxSignalFields_u16);
#endif

    SchM_Enter_Com_TxIpduProtArea(SENDSIGNAL);

#ifdef COM_TxSigUpdateBit

    txIpduConstPtr_pcst          = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

    /* Set update bit if it is configured */
    if (Com_GetValue(GEN,_UPDBITCONF,txSigConstPtr_pcst->general_u8))
    {
        PduLengthType       byteNo_uo;

        /* Calculate byte position of update bit in IPDU buffer */
        byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->updateBitPos_uo >> 3u);

        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(txIpduConstPtr_pcst, txSigConstPtr_pcst->updateBitPos_uo, byteNo_uo)
    }
#endif /* #ifdef COM_TxSigUpdateBit */

    /* Copy signal data from application buffer into IPdu buffer. */
    txSigNewVal_uo = Com_Prv_SigBufftoIpduBuff(idSignal_u16, signalDataPtr_pcv);

#if (!defined(COM_EffectiveSigTOC) && !defined(COM_TxFilters))
    (void)txSigNewVal_uo;
#endif

    SchM_Exit_Com_TxIpduProtArea(SENDSIGNAL);

#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)

    if (COM_SIG_TP_IS_TRIGGERED(txSigTP_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
    else
#endif
#ifdef COM_EffectiveSigTOC

    if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(txSigTP_u8))
    {
        if (Com_GetValue(GEN,_TYPE,txSigConstPtr_pcst->general_u8) == COM_UINT8_N)
        {
            isSigTriggered_u8 = (uint8)txSigNewVal_uo;
        }
        else if (COM_GET_OLDVALTRIGONCHANGE(txSigConstPtr_pcst->idxOldValue_uo) != txSigNewVal_uo)
        {
            isSigTriggered_u8 = COM_SET;
            COM_GET_OLDVALTRIGONCHANGE(txSigConstPtr_pcst->idxOldValue_uo) = txSigNewVal_uo;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
#endif
    {
        /* signal transfer property: PENDING OR
         * signal transfer property: TRIG_ON_CHANGE & signal value has not changed after the last update.
         */
    }

    /* TRACE[SWS_Com_00334] The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs. So the IPDU Group status is checked only after Signal packing is done. */
    if (Com_Prv_CheckTxIPduStatus(idIpdu_uo))
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
        if (Com_GetValue(TXSIG,_FILTRALG,constTxSignalFields_u16) != (uint16)COM_NOTCONFIGURED)
        {
            /* If TMS changes, Com_Prv_SigTxChangeMode() returns COM_TRUE */
            if (Com_Prv_SigTxChangeMode(idSignal_u16, (uint32)txSigNewVal_uo))
            {
                sendIpduFlag_st.isModeChangd_u1 = COM_SET;
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        /* Check if signal-timeout is configured */
        if (Com_GetValue(TXSIG,_TOENABLED,constTxSignalFields_u16))
        {
            sendIpduFlag_st.isTimeoutReq_u1 = COM_SET;
        }
#endif

#if defined(COM_SigTriggeredWithoutRepetition) || defined(COM_SigTriggeredOnChangeWithoutRepetition)
        /* Check if signal transfer property is configured as WITHOUT_REPETITION */
        if (COM_SIG_TP_IS_WITHOUT_REPETITION(txSigTP_u8))
        {
            sendIpduFlag_st.ignoreRepetitions_u1 = COM_SET;
        }
#endif

        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduId_tuo)idIpdu_uo, sendIpduFlag_st );

    } /* End of Com_Prv_CheckTxIPduStatus */
    else
    {
        /* corresponding I-PDU group is stopped, so donot process txIPdu */
        status_u8 = COM_SERVICE_NOT_AVAILABLE;
    }

    return status_u8;

} /* End of Com_Prv_InternalSendSignal */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_SigBufftoIpduBuff
 Description      : Pack/Copy signal buffer into IPdu buffer
 Parameter        : idSignal_u16      -> Internal Id of the signal. (Relevant for post-build case)
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
 Return value     : returns signal buffer
 **********************************************************************************************************************
*/
LOCAL_INLINE Com_SigMax_tuo Com_Prv_SigBufftoIpduBuff(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    PduIdType                   idIpdu_uo;
    Com_SigMax_tuo              txSigNewVal_uo;
    PduLengthType               byteNo_uo;
    uint8                       type_u8;
    uint8                       constGeneral_u8;

    txSigConstPtr_pcst  = COM_GET_TXSIG_CONSTDATA(idSignal_u16);
    idIpdu_uo           = txSigConstPtr_pcst->idComIPdu_uo;
    constGeneral_u8     = txSigConstPtr_pcst->general_u8;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    type_u8             = Com_GetValue(GEN,_TYPE,constGeneral_u8);
    txSigNewVal_uo      = COM_ZERO;

    if (type_u8 != (uint8)COM_UINT8_N)
    {
        uint8       sigEndianess_u8;

        sigEndianess_u8 = Com_GetValue(GEN,_ENDIANESS,constGeneral_u8);

        /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in the AUTOSAR specification,
           so suppressing warning "Cast from a pointer to void to a pointer to object". */
        switch(type_u8)
        {
        case COM_SINT8:
        case COM_UINT8:
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            txSigNewVal_uo = (*(const uint8 * )signalDataPtr_pcv);
            break;
        case COM_SINT16:
        case COM_UINT16:
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            txSigNewVal_uo = (*(const uint16 * )signalDataPtr_pcv);
            break;
        case COM_BOOLEAN:
            /* Update the Src_Buf with the data i.e typecast the data to the boolean variable*/
            txSigNewVal_uo = COM_BOOL_TO_UNSIGNED(*(const boolean * )signalDataPtr_pcv);
            break;
        case COM_SINT32:
        case COM_UINT32:
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            txSigNewVal_uo = (*(const uint32 * )signalDataPtr_pcv);
            break;
#ifdef COM_TXSIG_INT64
        case COM_UINT64:
        case COM_SINT64:
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            txSigNewVal_uo = *(const uint64 * )signalDataPtr_pcv;
            break;
#endif /* #ifdef COM_TXSIG_INT64 */

            /* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case COM_FLOAT32:
        {
            uint32 txSigNewVal_u32;
            txSigNewVal_u32 = COM_ZERO;
            /* Convert the float value into 32 bit stream and then pack the value */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type, based on the pointers address alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy( (void *)&txSigNewVal_u32, signalDataPtr_pcv, COM_F32_SIZE_IN_BYTES );

            txSigNewVal_uo = txSigNewVal_u32;
        }
            break;
#endif /* #ifdef COM_FLOAT32SUPP */
            /* FC_VariationPoint_END */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
        }
        /* FC_VariationPoint_START */
#ifdef COM_TXSIG_FLOAT64SUPP
        if (type_u8 == COM_FLOAT64)
        {
            float64 txSigNewVal_f64;

            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in the AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            txSigNewVal_f64 = *(const float64 * )signalDataPtr_pcv;

            /* Convert the float value into 32 bit stream and then pack the value */
            Com_Prv_PackFloatSignal(sigEndianess_u8, txSigConstPtr_pcst->bitPos_uo, txSigNewVal_f64, txIpduConstPtr_pcst->buffPtr_pu8);
        }
        else
#endif /* #ifdef COM_TXSIG_FLOAT64SUPP */
        /* FC_VariationPoint_END */
        {
            Com_Prv_PackSignal(sigEndianess_u8, txSigConstPtr_pcst->bitPos_uo, txSigConstPtr_pcst->bitSize_uo, txSigNewVal_uo,
                           txIpduConstPtr_pcst->buffPtr_pu8);
        }
    }
    else
    {
#ifdef COM_EffectiveSigTOC
        /* Varible to hold the Signal Transfer Property */
        uint8                       txSigTP_u8;
#endif
        /* Find the byteNo_uo of the IPDU buffer */
        byteNo_uo = (PduLengthType)(txSigConstPtr_pcst->bitPos_uo >> 3u);
        /* The locks inside the function Com_ByteCopy() are removed.
         * Hence locks are used here
         */

#ifdef COM_EffectiveSigTOC
        /* Load the Signal Transfer Property */
        txSigTP_u8      = (uint8)Com_GetValue(TXSIG,_TP,txSigConstPtr_pcst->txSignalFields_u16);

        if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(txSigTP_u8))
        {
            /* MR12 DIR 1.1, RULE 21.16 VIOLATION: 1.signalDataPtr_pcv is defined as void pointer in the AUTOSAR
             * specification, so suppressing warning "Cast from a pointer to object type to a pointer to void".
             * 2.Memcmp expects valid Type argument to be passed, but here signalDataPtr_pcv is of void type. So, no functional impact.*/
            if((COM_MEMCOMPARE((txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo),signalDataPtr_pcv,(uint32)txSigConstPtr_pcst->bitSize_uo)) != 0x00)
            {

                /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in the AUTOSAR specification,
                   so suppressing warning "Cast from a pointer to void to a pointer to object". */
                Com_ByteCopy((txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo),(const uint8*)signalDataPtr_pcv,(uint32)txSigConstPtr_pcst->bitSize_uo);

                txSigNewVal_uo = COM_ONE;
            }
        }
        else
#endif
        {
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in the AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            Com_ByteCopy((txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo),(const uint8*)signalDataPtr_pcv,(uint32)txSigConstPtr_pcst->bitSize_uo);

        }
    }

    return txSigNewVal_uo;

} /* End of Com_Prv_SigBufftoIpduBuff */

