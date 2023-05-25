

#ifndef COM_PRV_INL_H
#define COM_PRV_INL_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function sets TMS data in to be accessesd via CurrentTxModePtr
 * after evaluating TMS mode for the IPdu
 */
LOCAL_INLINE void Com_Prv_SetCurrentTxModePtr(
                                    Com_TxIpduRam_tpst              txIpduRamPtr_pst,
                                    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst,
                                    boolean                         modeRequested_b
                                         );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to update Rx-GroupSignal application buffers
 */
LOCAL_INLINE void Com_Prv_UpdateRxSignalBuffer(
                                    Com_RxSigCfg_tpcst              rxSigConstPtr_pcst,
                                    Com_SigMax_tuo                  rxNewValSig_uo,
                                    Com_MainFunc_tuo                idRxMainFunc_uo
                                          );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to update Rx-GroupSignal secondary buffers
 */
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_UpdateRxGroupSignalSecondBuffer(
                                    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst,
                                    Com_SigMax_tuo                  rxNewVal_uo,
                                    Com_MainFunc_tuo                idRxMainFunc_uo
                                                     );
#endif /* #ifdef COM_RX_SIGNALGROUP */

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to initialize signal intial value into the provided IPdu buffer
 */
LOCAL_INLINE void Com_Prv_InitializePduBuffWithSignalInitValue(
                                    uint8 *                         buffPtr_pu8,
                                    Com_SigMax_tuo                  sigInitVal_uo,
                                    Com_Bitposition_tuo             sigBitPos_uo,
                                    Com_Bitsize_tuo                 sigBitSize_uo,
                                    uint8                           sigType_u8,
                                    uint8                           sigEndianess_u8
                                             );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to initialize Rx-signal group with initial values
 */
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE
void Com_Prv_SigGrpReplaceToInit(Com_RxSigGrpCfg_tpcst rxSigGrpConstPtr_pcst, Com_MainFunc_tuo idRxMainFunc_uo);
#endif /* #ifdef COM_RX_SIGNALGROUP */

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to compute the init value
 */
#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetTxGrpSigInitValue(Com_TxGrpSigCfg_tpcst txGrpSigConstPtr_pcst);
#endif
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetRxSigInitValue(Com_RxSigCfg_tpcst rxSigConstPtr_pcst);
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetTxSigInitValue(Com_TxSigCfg_tpcst txSigConstPtr_pcst);
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetRxGrpSigInitValue(Com_RxGrpSigCfg_tpcst rxGrpSigConstPtr_pcst);
#endif

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
LOCAL_INLINE void Com_CallTxNotifyCallback(Com_IpduId_tuo idIpdu_uo);
LOCAL_INLINE void Com_CallTxErrNotifyCallback(Com_IpduId_tuo idIpdu_uo);
LOCAL_INLINE void Com_CallTxTONotifyCallback(Com_IpduId_tuo idIpdu_uo);
LOCAL_INLINE void Com_CallRxTONotifyCallback(Com_IpduId_tuo idIpdu_uo);
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

/* Inline function to check for the RxIPdu and TxIpdu status*/
LOCAL_INLINE boolean Com_Prv_CheckRxIPduStatus(PduIdType idIpdu_uo);
LOCAL_INLINE boolean Com_Prv_CheckTxIPduStatus(PduIdType idIpdu_uo);

/**
 * @ingroup COM_PRV_INL_H
 * Service to check if the IpduControl via RbaNds_EcuVariant value is set     \n
 * @param  IpduId - Id of the Rx Ipdu   \n
 * @return Boolean : Status of the Ipdu, either started or stopped \n
 */
#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
LOCAL_INLINE boolean Com_Prv_CheckRxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo);
#endif

/**
 * @ingroup COM_PRV_INL_H
 * Service to check if the IpduControl via RbaNds_EcuVariant value is set     \n
 * @param  IpduId - Id of the Tx Ipdu   \n
 * @return Boolean : Status of the Ipdu, either started or stopped \n
 */
#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
LOCAL_INLINE boolean Com_Prv_CheckTxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo);
#endif

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_CopyTxGroupSignalData(
                                    Com_SignalIdType                idGrpSignal_u16,
                                    Com_SigMax_tuo                  txGrpSigNewVal_uo,
                                    Com_MainFunc_tuo                idTxMainFunc_uo
                                           );
#endif /* #ifdef COM_TX_SIGNALGROUP */

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to handle gateway and non-gateway use-case for tx-signal/signalGroup
 */
LOCAL_INLINE void Com_Prv_ProceedToSendIpdu(Com_IpduId_tuo  idComTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st);

#ifdef COM_TxFilters
LOCAL_INLINE boolean Com_Prv_CheckModeChange(Com_IpduId_tuo idTxIpdu_uo);
#endif

#ifdef COM_TX_IPDUCOUNTER
LOCAL_INLINE void Com_Prv_TxIpduInitializeCounter(PduIdType idComTxPdu_uo);
#endif

#ifdef COM_RxSigUpdateTimeout
LOCAL_INLINE void Com_Prv_LoadSigTimeoutValue(
                                        Com_SignalIdType        idSignal_u16,
                                        uint8                   sigUpdateBitStatus_u8,
                                        uint16_least *          idxSigToutPtr_pqu16
                                             );
#endif /* end of #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
LOCAL_INLINE void Com_Prv_LoadSigGrpTimeoutValue(
                                        Com_SignalGroupIdType   idSigGrp_u16,
                                        uint8                   sigUpdateBitStatus_u8,
                                        uint16_least *          idxSigToutPtr_pqu16
                                                );
#endif /* end of #ifdef COM_RxSigGrpUpdateTimeout */

LOCAL_INLINE boolean Com_Prv_IsValidRxIpduId(PduIdType idPdu_uo);
LOCAL_INLINE boolean Com_Prv_IsValidTxIpduId(PduIdType idPdu_uo);

LOCAL_INLINE boolean Com_Prv_IsValidRxSignalId(Com_SignalIdType idSignal_u16);
LOCAL_INLINE boolean Com_Prv_IsValidTxSignalId(Com_SignalIdType idSignal_u16);

#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE boolean Com_Prv_IsValidRxGroupSignalId(Com_SignalIdType idSignal_u16);
LOCAL_INLINE boolean Com_Prv_IsValidRxSignalGroupId(Com_SignalGroupIdType idSigGrp_u16);
#endif /* end of #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE boolean Com_Prv_IsValidTxGroupSignalId(Com_SignalIdType idSignal_u16);
LOCAL_INLINE boolean Com_Prv_IsValidTxSignalGroupId(Com_SignalGroupIdType idSigGrp_u16);
#endif /* end of #ifdef COM_TX_SIGNALGROUP */

LOCAL_INLINE boolean Com_Prv_IsValidIpduGroupId(Com_IpduGroupIdType idIpduGroup_u16);


/**********************************************************************************************************************
 * Inline Function Definition
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Function name    : Com_Prv_SetCurrentTxModePtr [Internal]
 Description      : Inline function sets TMS data in to be accessesd via CurrentTxModePtr
                    after evaluating TMS mode for the IPdu

----------------------------------------------------------------------------------------|
 * Mode                  |    Configuration details                              | Value|
----------------------------------------------------------------------------------------|
 * COM_MODE_VALID        |    Both the TRUE and FALSE modes are configured.      |    00|
 * COM_TRUEMODE_VALID    |    Only TRUE mode is configured                       |    01|
 * COM_FALSEMODE_VALID   |    Only FALSE mode is configured                      |    10|
 * COM_MODE_INVALID      |    Both the TRUE and FALSE mode are not configured    |    11|
----------------------------------------------------------------------------------------|
This INLINE function checks the TMS generated values against the requested mode and performs action
|-------------|--------------|----------|--------------------------------------------------------------------------------------------------
|ModeRequested|Configured Mode values   |         Description                                           :      Action to be taken
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    0       |      0        |      0   |  FALSE mode is requested and IPdu TMS is COM_MODE_VALID       : Increment to the false mode data
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    0       |      0        |      1   |  FALSE mode is requested and IPdu TMS is COM_TRUEMODE_VALID   : Point to NONE mode data
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    0       |      1        |      0   |  FALSE mode is requested and IPdu TMS is COM_FALSEMODE_VALID  : Do nothing
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    0       |      1        |      1   |  FALSE mode is requested and IPdu TMS is COM_MODE_INVALID     : Do nothing
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    1       |      0        |      0   |  TRUE mode is requested and IPdu TMS is COM_MODE_VALID        : Do nothing
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    1       |      0        |      1   |  TRUE mode is requested and IPdu TMS is COM_TRUEMODE_VALID    : Do nothing
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    1       |      1        |      0   |  TRUE mode is requested and IPdu TMS is COM_FALSEMODE_VALID   : Point to NONE mode data
|------------|---------------|----------|--------------------------------------------------------------------------------------------------
|    1       |      1        |      1   |  TRUE mode is requested and IPdu TMS is COM_MODE_INVALID      : Do nothing
|------------|---------------|----------|--------------------------------------------------------------------------------------------------

where 0 in column 1 stands for FALSE mode
and   1 in column 1 stands for TRUE mode

 Parameter        : txIpduRamPtr_pst        - pointer to Txipdu ram data
                  : txIpduConstPtr_pcst     - pointer to Txipdu Rom data
                  : modeRequested_b         - mode requested
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE void Com_Prv_SetCurrentTxModePtr(
                                    Com_TxIpduRam_tpst              txIpduRamPtr_pst,
                                    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst,
                                    boolean                         modeRequested_b
                                         )
{
    uint16      tmsStatus_u16;

    tmsStatus_u16 = Com_GetValue(TXIPDU,_TMS,txIpduConstPtr_pcst->txIPduFields_u16);

    /* Get the latest TM pointer */
    txIpduRamPtr_pst->currentTxModePtr_pcst = txIpduConstPtr_pcst->tMConstPtr_pcst;

    if (modeRequested_b == COM_FALSE)
    {
        if (tmsStatus_u16 == COM_TRUEMODE_VALID)
        {
            /*If yes,load NONE mode data */
            txIpduRamPtr_pst->currentTxModePtr_pcst = &Com_NONE_TransModeInfo_cst;
        }
        else if (tmsStatus_u16 == COM_MODE_VALID)
        {
            /*Increment to FALSE mode data, if FALSE mode is also configured */
            txIpduRamPtr_pst->currentTxModePtr_pcst++;
        }
        else
        {
            /* COM_FALSEMODE_VALID: generated TM configuration */
            /* MODE_INVALID:        generated TM configuration */
        }
    }
    else
    {
        if (tmsStatus_u16 == COM_FALSEMODE_VALID)
        {
            txIpduRamPtr_pst->currentTxModePtr_pcst = &Com_NONE_TransModeInfo_cst;
        }
        else
        {
            /* COM_MODE_VALID:     generated TM configuration */
            /* COM_TRUEMODE_VALID: generated TM configuration */
            /* MODE_INVALID:       generated TM configuration */
        }
    }
}

/**********************************************************************************************************************
 Function name    : Com_Prv_UpdateRxGroupSignalSecondBuffer [Internal]
 Description      : Inline function copy Group signal value into the Group signal Second buffer
 Parameter        : rxGrpSigConstPtr_pcst   - pointer to RxGroupSignal configuration
                  : rxNewVal_uo             - latest RxGroupSignal data
                  : idRxMainFunc_uo         - Internally generated Rx-MainFunction-Id
 Return value     : None
 *********************************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_UpdateRxGroupSignalSecondBuffer(
                                    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst,
                                    Com_SigMax_tuo                  rxNewVal_uo,
                                    Com_MainFunc_tuo                idRxMainFunc_uo
                                                     )
{
    uint8   type_u8;

    type_u8 = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);

    switch(type_u8 >> 1u)
    {
    case 0x00u:
        /* UINT08 signal */
        COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = (uint8)rxNewVal_uo;
        break;

    case 0x01u:
        /* UINT16 signal */
        COM_GET_SECRXGRPSIGTYPEU16_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = (uint16)rxNewVal_uo;
        break;

    case 0x02u:
        /* UINT32 signal */
        /* FC_VariationPoint_START */
# ifdef COM_GRPSIGFLOAT32SUPP
    case (COM_FLOAT32 >> 1u):
        /* FLOAT32 signal */
# endif
        /* FC_VariationPoint_END */
        COM_GET_SECRXGRPSIGTYPEU32_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = (uint32)rxNewVal_uo;
        break;

# ifdef COM_RXGRPSIG_INT64
    case 0x08u:
        /* UINT64/SINT64 signal */
        COM_GET_SECRXGRPSIGTYPEU64_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = rxNewVal_uo;
        break;
# endif /* #ifdef COM_RXGRPSIG_INT64 */

    case 0x03u:
        /*Boolean Signal*/
        COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) = (uint8)rxNewVal_uo;
        break;

    case 0x04u:
        /* UINT8_N signal */
        Com_ByteCopyInit(&COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo),
                         (uint32)rxNewVal_uo,rxGrpSigConstPtr_pcst->bitSize_uo);
        break;

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    case (COM_UINT8_DYN >> 1u):
        {
            /* COM_UINT8_DYN group signal */
            Com_ByteCopyInit(&COM_GET_SECRXGRPSIGTYPEUINT8DYN_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo),
                             (uint32)rxNewVal_uo,rxGrpSigConstPtr_pcst->bitSize_uo);
        }
        break;
#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

    default:
        /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
        break;

    }
}
#endif

/**********************************************************************************************************************
 Function name    : Com_Prv_UpdateRxSignalBuffer [Internal]
 Description      : Inline function to copy signal value into the signal buffer and
                    is used when signal value as a variable is being passed
 Parameter        : RxSigConstPtr       - pointer to RxSignal configuration
                  : rxNewValSig_uo     - latest RxSignal data
                  : idRxMainFunc_uo     - Internally generated Rx-MainFunction-Id
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE void Com_Prv_UpdateRxSignalBuffer(
                                    Com_RxSigCfg_tpcst              rxSigConstPtr_pcst,
                                    Com_SigMax_tuo                  rxNewValSig_uo,
                                    Com_MainFunc_tuo                idRxMainFunc_uo
                                          )
{
    uint8       type_u8;

    type_u8 = Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8);

    switch(type_u8 >> 1u)
    {
    case 0x00u:     /* COM_UINT8 & COM_SINT8 */
    case (COM_BOOLEAN >> 1u):
        COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) = (uint8)rxNewValSig_uo;
        break;

    case 0x01u:     /* COM_UINT16 & COM_SINT16 */
        COM_GET_SIGTYPEUINT16_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) = (uint16)rxNewValSig_uo;
        break;

    case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
        /* COM_FLOAT32 signal */
#ifdef COM_FLOAT32SUPP
    case (COM_FLOAT32 >> 1u):
#endif /* #ifdef COM_FLOAT32SUPP */
/* FC_VariationPoint_END */
        COM_GET_SIGTYPEUINT32_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) = (uint32)rxNewValSig_uo;
        break;

#ifdef COM_RXSIG_INT64
    case 0x08u:     /* COM_UINT64 & COM_SINT64 */
        COM_GET_SIGTYPEUINT64_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo) = rxNewValSig_uo;
        break;
#endif /* #if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64) */

    case 0x04u:     /* COM_UINT8_N */
        Com_ByteCopyInit(&COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo,rxSigConstPtr_pcst->idxSigBuff_uo),
                         (uint32)rxNewValSig_uo,rxSigConstPtr_pcst->bitSize_uo);
        break;
    default:
        /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
        break;
    }
}

/**********************************************************************************************************************
 Function name    : Com_Prv_InitializePduBuffWithSignalInitValue [Internal]
 Description      : Inline function to call required interface to pack signal data into Ipdu buffer
 Parameter        : buffPtr_pu8         - pointer to (SignalGroup)IPdu Buffer
                  : sigInitVal_uo       - (Group)Signal InitValue
                  : sigBitPos_uo        - (Group)Signal Bit Position
                  : sigBitSize_uo       - (Group)Signal Bit Size
                  : sigType_u8          - (Group)Signal Type
                  : sigEndianess_u8     - (Group)Signal Endianness
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE void Com_Prv_InitializePduBuffWithSignalInitValue(
                                    uint8 *                         buffPtr_pu8,
                                    Com_SigMax_tuo                  sigInitVal_uo,
                                    Com_Bitposition_tuo             sigBitPos_uo,
                                    Com_Bitsize_tuo                 sigBitSize_uo,
                                    uint8                           sigType_u8,
                                    uint8                           sigEndianess_u8
                                                          )
{
    if ( sigType_u8 != (uint8)COM_UINT8_N )
    {
/* FC_VariationPoint_START */
#if (defined(COM_TXSIG_FLOAT64SUPP) || defined(COM_TXGRPSIG_FLOAT64SUPP) ||\
    (defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_RXGRPSIG_FLOAT64SUPP)))
        if ( sigType_u8 == COM_FLOAT64 )
        {
            /* Convert the float value into 32 bit stream and then pack the value */
            Com_Prv_PackFloatSignal( sigEndianess_u8, sigBitPos_uo, Com_getfloat64InitValue(sigInitVal_uo), buffPtr_pu8 );
        }
        else
#endif
/* FC_VariationPoint_END */
        {
            /* Pack the signal with Initial Value */
            Com_Prv_PackSignal( sigEndianess_u8, sigBitPos_uo, sigBitSize_uo, sigInitVal_uo, buffPtr_pu8 );
        }
    }
    else
    {
        PduLengthType byteOffset_uo;
        /* Find the starting byte position(ByteNo_uo) of Signal in the IPDU buffer */
        byteOffset_uo = ( PduLengthType )( sigBitPos_uo >> 3 );

        /* Now copy the Signal into the IPDU buffer */
        Com_ByteCopyInit( (buffPtr_pu8 + byteOffset_uo), (uint32)sigInitVal_uo, sigBitSize_uo );
    }
}

/**********************************************************************************************************************
 Function name    : Com_Prv_SigGrpReplaceToInit
 Description      : If the SigGrp is enabled with ArrayAccess, RxSigGrpBuffer will be packed with
                    GroupSignal Init Values OR else
                    GroupSignals Shadow buffers will be replaced with its Init Values
 Parameter        : rxSigGrpConstPtr_pcst   - pointer to RxSignalGroup configuration
                  : idRxMainFunc_uo         - Internally generated RxMainFunction-ID
 Return value     : None
 *********************************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_SigGrpReplaceToInit(Com_RxSigGrpCfg_tpcst rxSigGrpConstPtr_pcst, Com_MainFunc_tuo idRxMainFunc_uo)
{
    Com_RxGrpSigCfg_tpcst               rxGrpSigConstPtr_pcst;      /* Local pointer to Rx GroupSignal static configuration */
# ifdef COM_RX_SIGNALGROUP_ARRAY
    Com_RxSigGrpArrayCfg_tpcst          rxSigGrpArrayConstPtr_pcst; /* Local pointer to Rx SignalGroup Array configuration  */
    uint8 *                             rxSigGrpArrayBufRamPtr_pu8;
    PduLengthType                       sigGrpBitOffset_uo;
# endif
    uint16_least                        idxRxGrpSig_qu16;
    Com_SigMax_tuo                      sigValue_uo;
# if (defined(COM_RX_SIGNALGROUP_ARRAY) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_INT64))
    uint8                               sigType_u8;  /* Signal Type */
# endif
# ifdef COM_RX_SIGNALGROUP_ARRAY
    boolean                             isSigGrpArrayAccess_b;

    isSigGrpArrayAccess_b = Com_GetValue(RXSIGGRP,_ARRAYACCESS,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8);

    if ( isSigGrpArrayAccess_b )
    {
        rxSigGrpArrayConstPtr_pcst  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( rxSigGrpConstPtr_pcst->idxSigGrpArray_uo );

        rxSigGrpArrayBufRamPtr_pu8 = &COM_GET_RXSIGGRP_ARRAY_BUFF(idRxMainFunc_uo, rxSigGrpArrayConstPtr_pcst->idxRxSigGrpBuf_uo );

        sigGrpBitOffset_uo        = ( rxSigGrpArrayConstPtr_pcst->firstByteNo_uo * 8u );

        Com_Get_SchM_Enter_Com_RxSigGrpArrayBuff
    }
    else
# endif /* # ifdef COM_RX_SIGNALGROUP_ARRAY */
    {
        Com_Get_SchM_Enter_Com_RxGrpSigSecBuff
    }

    rxGrpSigConstPtr_pcst = COM_GET_RXGRPSIG_CONSTDATA( rxSigGrpConstPtr_pcst->idFirstGrpSig_uo );

    for ( idxRxGrpSig_qu16 = rxSigGrpConstPtr_pcst->numOfGrpSig_uo; idxRxGrpSig_qu16 > COM_ZERO; idxRxGrpSig_qu16-- )
    {
# if (defined(COM_RX_SIGNALGROUP_ARRAY) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_INT64))
        sigType_u8 = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);
# endif
        sigValue_uo=Com_Prv_GetRxGrpSigInitValue(rxGrpSigConstPtr_pcst);

#ifdef COM_RXGRPSIG_INT64
        /* TRACE[SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
         * to the size of its ComSignalType.
         * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
         * If negative values are configured, by default these values will be sign extended only till 32bits.
         * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicity. */
        sigValue_uo = (sigType_u8 == COM_SINT64) ? (Com_SigMax_tuo)((sint64)((sint32)sigValue_uo)) : sigValue_uo;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

# ifdef COM_RX_SIGNALGROUP_ARRAY
        if ( isSigGrpArrayAccess_b )
        {
            Com_Bitposition_tuo relativeSigBitPos_uo; /* Relative BitPosition in RxSignalGroup Array */

            /* The Buffer into which Signal Values needs to be packed will be available with starting address
             * of the Rx-SignalGroup Array Buffer.
             * But we have GroupSignal: Bit_Pos, i.e., Signal's actual LSB in the IPdu Buffer.
             * So, Rx-SignalGroup BitOffset(sigGrpBitOffset_uo) is removed from each of the GroupSignal and
             * is used as BitPosition to pack its value into the Rx-SignalGroup Array Buffer */
            relativeSigBitPos_uo = rxGrpSigConstPtr_pcst->bitPos_uo - ( Com_Bitposition_tuo )sigGrpBitOffset_uo;

            Com_Prv_InitializePduBuffWithSignalInitValue(
                                              rxSigGrpArrayBufRamPtr_pu8,
                                              sigValue_uo,
                                              relativeSigBitPos_uo,
                                              rxGrpSigConstPtr_pcst->bitSize_uo,
                                              sigType_u8,
                                              Com_GetValue(RXGRPSIG,_ENDIANESS,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8) );
        }
        else
# endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */
        {
/* FC_VariationPoint_START */
# ifdef COM_RXGRPSIG_FLOAT64SUPP
            if(sigType_u8 == COM_FLOAT64)
            {
                COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(idRxMainFunc_uo,rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo) =
                                                                      Com_getfloat64InitValue(sigValue_uo);
            }
            else
# endif
/* FC_VariationPoint_END */
            {
                Com_Prv_UpdateRxGroupSignalSecondBuffer(rxGrpSigConstPtr_pcst, sigValue_uo, idRxMainFunc_uo);
            }
        }
# if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)
        /* TRACE[SWS_Com_00603] The AUTOSAR COM module shall set the old_value of the filtering mechanisms
         * for each signal to the ComSignalInitValue during start-up */
        if ( ( COM_INIT != Com_InitStatus_en ) && ( COM_MASKED_NEW_DIFFERS_MASKED_OLD == rxGrpSigConstPtr_pcst->filterAlgo_u8 ) )
        {
            COM_GET_F_OLDVALUE(rxGrpSigConstPtr_pcst->idxFilter_uo) = (uint32)sigValue_uo;
        }
# endif /* # if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) */

        rxGrpSigConstPtr_pcst++;
    }
# ifdef COM_RX_SIGNALGROUP_ARRAY

    if ( isSigGrpArrayAccess_b )
    {
        Com_Get_SchM_Exit_Com_RxSigGrpArrayBuff
    }
    else
# endif /* # ifdef COM_RX_SIGNALGROUP_ARRAY */
    {
        Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
    }
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
/**********************************************************************************************************************
 Function name    : Com_Prv_GetTxGrpSigInitValue
 Description      : Inline Function is used too return the init_value or Init_valIndex
 Return value     : txGrpSigNewVal_uo
                    Init_ValIndex  will be returned if Float64 is used
                    Else InitValue will be returned.
 *********************************************************************************************************************/
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetTxGrpSigInitValue(Com_TxGrpSigCfg_tpcst txGrpSigConstPtr_pcst)
{
    Com_SigMax_tuo  txGrpSigNewVal_uo;

#ifdef  COM_INITVALOPTIMIZATION
# ifdef COM_TXGRPSIG_FLOAT64SUPP
    if(Com_GetValue(TXGRPSIG,_TYPE,txGrpSigConstPtr_pcst->txGrpSigFields_u16) == COM_FLOAT64 )
    {
        txGrpSigNewVal_uo = txGrpSigConstPtr_pcst->idxInitValue_uo;
    }
    else
# endif
    {
        txGrpSigNewVal_uo = COM_GET_UNIQUE_INIT_VALUE_CONSTDATA(txGrpSigConstPtr_pcst->idxInitValue_uo);
    }
#else
    txGrpSigNewVal_uo     = txGrpSigConstPtr_pcst->initVal_u32;
#endif

    return txGrpSigNewVal_uo;
}
#endif /* end of #ifdef COM_TX_SIGNALGROUP */


/**********************************************************************************************************************
 Function name    : Com_Prv_GetRxSigInitValue
 Description      : Inline Function is used too return the init_value or Init_valIndex
 Return value     : rxSigVal_uo
                    Init_ValIndex  will be returned if Float64 is used
                    Else InitValue will be returned.
 *********************************************************************************************************************/
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetRxSigInitValue(Com_RxSigCfg_tpcst rxSigConstPtr_pcst)
{
    Com_SigMax_tuo  rxSigVal_uo;

#ifdef  COM_INITVALOPTIMIZATION
# ifdef COM_RXSIG_FLOAT64SUPP
    if(Com_GetValue(GEN,_TYPE,rxSigConstPtr_pcst->general_u8) == COM_FLOAT64 )
    {
        rxSigVal_uo = rxSigConstPtr_pcst->idxInitValue_uo;
    }
    else
# endif
    {
        rxSigVal_uo = COM_GET_UNIQUE_INIT_VALUE_CONSTDATA(rxSigConstPtr_pcst->idxInitValue_uo);
    }
#else
    rxSigVal_uo     = rxSigConstPtr_pcst->initVal_u32;
#endif

    return rxSigVal_uo;
}


/**********************************************************************************************************************
 Function name    : Com_Prv_GetTxSigInitValue
 Description      : Inline Function is used too return the init_value or Init_valIndex
 Return value     : txSigNewVal_uo
                    Init_ValIndex  will be returned if Float64 is used
                    Else InitValue will be returned.
 *********************************************************************************************************************/

LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetTxSigInitValue(Com_TxSigCfg_tpcst txSigConstPtr_pcst)
{
    Com_SigMax_tuo  txSigNewVal_uo;

#ifdef  COM_INITVALOPTIMIZATION
# ifdef COM_TXSIG_FLOAT64SUPP
    if(Com_GetValue(GEN,_TYPE,txSigConstPtr_pcst->general_u8) == COM_FLOAT64 )
    {
        txSigNewVal_uo = txSigConstPtr_pcst->idxInitValue_uo;
    }
    else
# endif
    {
        txSigNewVal_uo = COM_GET_UNIQUE_INIT_VALUE_CONSTDATA(txSigConstPtr_pcst->idxInitValue_uo);
    }
#else
    txSigNewVal_uo     = txSigConstPtr_pcst->initVal_u32;
#endif
    return txSigNewVal_uo;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_GetRxGrpSigInitValue
 Description      : Inline Function is used too return the init_value or Init_valIndex
 Return value     : sigValue_uo
                    Init_ValIndex  will be returned if Float64 is used
                    Else InitValue will be returned.
 *********************************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE Com_SigMax_tuo Com_Prv_GetRxGrpSigInitValue(Com_RxGrpSigCfg_tpcst rxGrpSigConstPtr_pcst)
{
    Com_SigMax_tuo  sigValue_uo;

#ifdef  COM_INITVALOPTIMIZATION
# ifdef COM_RXGRPSIG_FLOAT64SUPP
    if(Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8) == COM_FLOAT64 )
    {
        sigValue_uo = rxGrpSigConstPtr_pcst->idxInitValue_uo;
    }
    else
# endif
    {
        sigValue_uo = COM_GET_UNIQUE_INIT_VALUE_CONSTDATA(rxGrpSigConstPtr_pcst->idxInitValue_uo);
    }
#else
    sigValue_uo     = rxGrpSigConstPtr_pcst->initVal_u32;
#endif

    return sigValue_uo;
}
#endif /* end of #ifdef COM_RX_SIGNALGROUP */


#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_TxIPduNotification
LOCAL_INLINE void Com_CallTxNotifyCallback(Com_IpduId_tuo idIpdu_uo)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;     /* Local pointer to Tx ipdu static configuration */
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;      /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpCfg_tpcst       txSigGrpConstPtr_pcst;   /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least                 index_qu8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */

    idIpdu_uo = COM_GET_TX_IPDU_ID(idIpdu_uo);
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    txSigConstPtr_pcst = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);
#ifdef COM_TX_SIGNALGROUP
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_qu8 = txIpduConstPtr_pcst->numOfSig_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigConstPtr_pcst->notification_pfct != NULL_PTR)
        {
            txSigConstPtr_pcst->notification_pfct();
        }
        txSigConstPtr_pcst++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_qu8 = txIpduConstPtr_pcst->numOfSigGroups_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigGrpConstPtr_pcst->notification_pfct != NULL_PTR)
        {
            txSigGrpConstPtr_pcst->notification_pfct();
        }
        txSigGrpConstPtr_pcst++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

}
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_ERRORNOTIFICATION
LOCAL_INLINE void Com_CallTxErrNotifyCallback(Com_IpduId_tuo idIpdu_uo)
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;    /* Local pointer to Tx ipdu static configuration */
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;    /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpCfg_tpcst       txSigGrpConstPtr_pcst; /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least                 index_qu8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */

    idIpdu_uo = COM_GET_TX_IPDU_ID(idIpdu_uo);
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    txSigConstPtr_pcst = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);
#ifdef COM_TX_SIGNALGROUP
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_qu8 = txIpduConstPtr_pcst->numOfSig_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigConstPtr_pcst->errorNotification_pfct != NULL_PTR)
        {
            txSigConstPtr_pcst->errorNotification_pfct();
        }
        txSigConstPtr_pcst++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_qu8 = txIpduConstPtr_pcst->numOfSigGroups_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigGrpConstPtr_pcst->errorNotification_pfct != NULL_PTR)
        {
            txSigGrpConstPtr_pcst->errorNotification_pfct();
        }
        txSigGrpConstPtr_pcst++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

}
#endif /* #ifdef COM_ERRORNOTIFICATION */

#ifdef COM_TxIPduTimeOutNotify
LOCAL_INLINE void Com_CallTxTONotifyCallback(Com_IpduId_tuo idIpdu_uo)
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;    /* Local pointer to Tx ipdu static configuration */
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;    /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpCfg_tpcst       txSigGrpConstPtr_pcst; /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least                 index_qu8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */

    idIpdu_uo = COM_GET_TX_IPDU_ID(idIpdu_uo);
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
    txSigConstPtr_pcst = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);
#ifdef COM_TX_SIGNALGROUP
    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_qu8 = txIpduConstPtr_pcst->numOfSig_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigConstPtr_pcst->timeoutNotification_pfct != NULL_PTR)
        {
            txSigConstPtr_pcst->timeoutNotification_pfct();
        }
        txSigConstPtr_pcst++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_qu8 = txIpduConstPtr_pcst->numOfSigGroups_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(txSigGrpConstPtr_pcst->timeoutNotification_pfct != NULL_PTR)
        {
            txSigGrpConstPtr_pcst->timeoutNotification_pfct();
        }
        txSigGrpConstPtr_pcst++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

}
#endif /* #ifdef COM_TxIPduTimeOutNotify */
#ifdef COM_RxIPduTimeoutNotify
LOCAL_INLINE void Com_CallRxTONotifyCallback(Com_IpduId_tuo idIpdu_uo)
{

    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;    /* Local pointer to Rx ipdu static configuration */
    Com_RxSigCfg_tpcst          rxSigConstPtr_pcst;    /* Local pointer to Rx signal static configuration */
#ifdef COM_RX_SIGNALGROUP
    Com_RxSigGrpCfg_tpcst       rxSigGrpConstPtr_pcst; /* Local pointer to hold the address of Rx SignalGroup static configuration */
#endif /* #ifdef COM_RX_SIGNALGROUP */
    uint8_least                 index_qu8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */

    idIpdu_uo = COM_GET_RX_IPDU_ID(idIpdu_uo);
    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);
    rxSigConstPtr_pcst = COM_GET_RXSIG_CONSTDATA(rxIpduConstPtr_pcst->idRxSig_uo);
#ifdef COM_RX_SIGNALGROUP
    rxSigGrpConstPtr_pcst = COM_GET_RXSIGGRP_CONSTDATA(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);
#endif /* #ifdef COM_RX_SIGNALGROUP */

    for(index_qu8 = rxIpduConstPtr_pcst->numOfSig_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(rxSigConstPtr_pcst->timeoutNotification_pfct != NULL_PTR)
        {
            rxSigConstPtr_pcst->timeoutNotification_pfct();
        }
        rxSigConstPtr_pcst++;
    }
#ifdef COM_RX_SIGNALGROUP
    for(index_qu8 = rxIpduConstPtr_pcst->numOfSigGrp_u16; index_qu8 > COM_ZERO; index_qu8--)
    {
        if(rxSigGrpConstPtr_pcst->timeoutNotification_pfct != NULL_PTR)
        {
            rxSigGrpConstPtr_pcst->timeoutNotification_pfct();
        }
        rxSigGrpConstPtr_pcst++;
    }
#endif /* #ifdef COM_RX_SIGNALGROUP */

}
#endif /* #ifdef COM_RxIPduTimeoutNotify */
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE void Com_Prv_CopyTxGroupSignalData(
                                Com_SignalIdType        idGrpSignal_u16,
                                Com_SigMax_tuo          txGrpSigNewVal_uo,
                                Com_MainFunc_tuo        idTxMainFunc_uo
                                           )
{
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_TxGrpSigBuffIndex_tuo       idxGrpSigBuff_uo;
    uint16                          type_u16;

    txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(idGrpSignal_u16);
    idxGrpSigBuff_uo = txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo;
    type_u16 = Com_GetValue(TXGRPSIG,_TYPE,txGrpSigConstPtr_pcst->txGrpSigFields_u16);

    switch(type_u16 >> 1u)
    {
        case 0x00u: /* UINT8 and SINT8 type */
        case 0x03u: /* Boolean type */
        {
            COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo) = (uint8)txGrpSigNewVal_uo;
        }
        break;

        /* UINT16 and SINT16 type */
        case 0x01u:
        {
#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_TxGrpSigBuffer();
#endif
            COM_GET_TXGRPSIGTYPEU16_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo) = (uint16)txGrpSigNewVal_uo;
#if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_TxGrpSigBuffer();
#endif
        }
        break;

        /* UINT32,SINT32 and FLOAT32 type */
        case 0x02u:
        /* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
        case (COM_FLOAT32 >> 1u):
#endif
        /* FC_VariationPoint_END */
        {
#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_TxGrpSigBuffer();
#endif
            COM_GET_TXGRPSIGTYPEU32_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo) = (uint32)txGrpSigNewVal_uo;
#if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_TxGrpSigBuffer();
#endif
        }
        break;

        /* UINT64 and SINT64 type */
#ifdef COM_TXGRPSIG_INT64
        case 0x08u:
        {
            SchM_Enter_Com_TxGrpSigBuffer();
            COM_GET_TXGRPSIGTYPEU64_BUFF(idTxMainFunc_uo, idxGrpSigBuff_uo) = (uint64)txGrpSigNewVal_uo;
            SchM_Exit_Com_TxGrpSigBuffer();
        }
        break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are
             * allowed to be configured, hence default case is intentionally left empty.
             */
        break;
   }

}
#endif /* #ifdef COM_TX_SIGNALGROUP */


/**********************************************************************************************************************
 Function name    : Com_Prv_CheckTxIPduStatus
 Description      : Inline function to check TxIPdu status
 Parameter        : idIpdu_uo - TxIPdu Id
 Return value     : COM_TRUE  - TxIPdu is started
                    COM_FALSE - TxIPdu is stopped
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_CheckTxIPduStatus(PduIdType idIpdu_uo)
{
    boolean     txIPduStatus_b;

    txIPduStatus_b = (Com_GetRamValue(TXIPDU,_PDUSTATUS,COM_GET_TXPDURAM_S(idIpdu_uo).txFlags_u16))
                /* FC_VariationPoint_START */
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
                 * of operands does not impact the system behaviour even though of volatile-qualified,
                 * Hence the warning is suppressed. */
#ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION
                && (Com_Prv_CheckTxIPduCalibrationVarStatus(idIpdu_uo))
#endif /* #ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION */
#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
                && (Com_Prv_CheckTxIPduControlViaRbaNdsEcuVariant(idIpdu_uo))
#endif /* #ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
                /* FC_VariationPoint_END */
                         ;

    return txIPduStatus_b;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_CheckRxIPduStatus
 Description      : Inline function to check RxIPdu status
 Parameter        : idIpdu_uo - RxIPdu Id
 Return value     : COM_TRUE  - RxIPdu is started
                    COM_FALSE - RxIPdu is stopped
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_CheckRxIPduStatus(PduIdType idIpdu_uo)
{
    boolean     rxIPduStatus_b;

    rxIPduStatus_b = (Com_GetRamValue(RXIPDU,_PDUSTATUS,COM_GET_RXPDURAM_S(idIpdu_uo).rxFlags_u8))
                /* FC_VariationPoint_START */
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
                 * of operands does not impact the system behaviour even though of volatile-qualified,
                 * Hence the warning is suppressed. */
#ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION
                && (Com_Prv_CheckRxIPduCalibrationVarStatus(idIpdu_uo))
#endif /* #ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION */
#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
                && (Com_Prv_CheckRxIPduControlViaRbaNdsEcuVariant(idIpdu_uo))
#endif /* #ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
                /* FC_VariationPoint_END */
                        ;

    return rxIPduStatus_b;
}

#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CheckRxIPduControlViaRbaNdsEcuVariant
 Description      : Service invoked internally by rba_ComScl to set/reset the status of Rx Ipdu
 Parameter        : idIpdu_uo -> id of the Rx ipdu (passed ID is internal ID)
 Return value     : RxIpdu status maintained by rba_ComScl
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_CheckRxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo)
{
    uint16   index_u16;
    uint8    bitOffset_u8;

    index_u16    = idIpdu_uo >> 3u;
    bitOffset_u8 = (uint8)(idIpdu_uo % 8u);

    /* Extract the bit value for this ipdu in Com_RxIpduControlVector_au8 and return from this function */
    return (((Com_RxIpduControlVector_au8[index_u16] >> bitOffset_u8) & COM_ONE) != COM_ZERO);
}
#endif /* end of COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */


#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_CheckTxIPduControlViaRbaNdsEcuVariant
 Description      : Service invoked internally by rba_ComScl to set/reset the status of Tx Ipdu
 Parameter        : idIpdu_uo -> id of the Tx ipdu (passed ID is internal ID)
 Return value     : TxIpdu status maintained by rba_ComScl
 **********************************************************************************************************************
*/
LOCAL_INLINE boolean Com_Prv_CheckTxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo)
{
    uint16   index_u16;
    uint8    bitOffset_u8;

    index_u16    = idIpdu_uo >> 3u;
    bitOffset_u8 = (uint8)(idIpdu_uo % 8u);

    /* Extract the bit for this ipdu in Com_TxIpduControlVector_au8 and return from this function */
    return (((Com_TxIpduControlVector_au8[index_u16] >> bitOffset_u8) & COM_ONE) != COM_ZERO);
}
#endif /* end of COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */


/**********************************************************************************************************************
 Function name    : Com_Prv_ProceedToSendIpdu
 Description      : 1. For gateway txIpdu - set the flags here and defer the transmission till
                        Com_MainFunctionRouteSignals
                    2. For non-gateway txIPdu - trigger the IPdu only for event type or for mode change.
 Parameter        : IpduId_ui        -> Internal Id of the TxIPdu
                    sendIpduFlag_st  -> flags to decide ipdu transmission
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE void Com_Prv_ProceedToSendIpdu(Com_IpduId_tuo idComTxPdu_uo, Com_SendIpduInfo_tst sendIpduFlag_st)
{
    Com_TxIpduRam_tpst        txIpduRamPtr_pst;

    txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idComTxPdu_uo);

#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)

    if (Com_GetValue(TXIPDU,_ISGWPDU,COM_GET_TX_IPDU_CONSTDATA(idComTxPdu_uo)->txIPduFields_u16))
    {
        /* locks are required here to protect TxFlags */
        SchM_Enter_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);

        /* If Signal is a part of Gw-Ipdu, then defer till Com_MainFunctionRouteSignals */
        if (sendIpduFlag_st.isSigTriggered_u1 != COM_RESET)
        {
            /* For Tx Ipdus with Gw signals the event transmissions are done from Com_MainFunctionRouteSignals.
             * The below step will raise a trigger in the next call of Com_MainFunctionRouteSignals, and
             * the Tx Ipdu is transmitted.
             * The Gw Ipdus which are cyclically triggered are sent from Com_mainfunctionTx */
            Com_SetRamValue(TXIPDU,_ISGWTRIGGRD,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
        }

#ifdef COM_TxFilters
        /* If the TMS changes, set the global TMS flag to consolidate transmission mode change
         * for the Gw-Tx IPdu after its last transmission */
        if (sendIpduFlag_st.isModeChangd_u1 != COM_RESET)
        {
            /* Consolidate the TMS Changes */
            if (Com_GetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16))
            {
                Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
            }
            else
            {
                /* Store the transmission mode change and shall be used while triggering the Tx-IPdu from
                 * Com_MainFunctionRouteSignals() */
                Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        if (sendIpduFlag_st.isTimeoutReq_u1 != COM_RESET)
        {
            /* This flag is set to start Timeout monitoring in the next call of Com_MainFunctionRouteSignals */
            Com_SetRamValue(TXIPDU,_ISTOENABLED,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);
        }
#endif /* #ifdef COM_TxIPduTimeOut */

        SchM_Exit_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);
    }
    else
#endif /* #if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY) */
    {
        /* Process Tx-IPdu Transmission.
         * If this signal TX request causes mode change to MIXED/PERIODIC TM & signal transfer property is PENDING,
         * then do not transmit the IPDU. Because IPDU will be transmitted immediately,
         * due to periodic part of MIXED/PERIODIC mode.
         */
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status.
         * Evaluation(or no evaluation) of the 2nd operand does not impact the system behaviour.
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if (((sendIpduFlag_st.isSigTriggered_u1 != COM_RESET) &&
             (COM_TX_MODE_IS_EVENT(Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8))))
#ifdef COM_TxFilters
            || (sendIpduFlag_st.isModeChangd_u1 != COM_RESET)
#endif
           )
        {

            /* this flag will be set, as call happens apart from Com_MainFunctionTx */
            sendIpduFlag_st.isEventTrig_u1 = COM_SET;

            Com_Prv_SendIpdu((PduIdType)idComTxPdu_uo, sendIpduFlag_st);
        }
    }
}   /* End of Com_Prv_ProceedToSendIpdu */


#ifdef COM_TxFilters

/***************************************************************************************************
 Function name    : Com_Prv_CheckModeChange
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal or
                    signal group which is updated.
 Parameter        : idTxIpdu_uo -> Id of the Tx ipdu
                  : SignalId -> Id of the signal
                  : TxNewVal -> New signal value
                  : IsSignalGroup -> boolean to check whether it is an SignalGroup or not
 Return value     : COM_TRUE/COM_FALSE
 ***************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_CheckModeChange(Com_IpduId_tuo idTxIpdu_uo)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    boolean                     returnValue_b;
    boolean                     mode_b;

    /* Initialization of variables */
    txIpduConstPtr_pcst  = COM_GET_TX_IPDU_CONSTDATA(idTxIpdu_uo);
    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idTxIpdu_uo);

    mode_b            = COM_FALSE;
    returnValue_b     = COM_FALSE;

    if (txIpduRamPtr_pst->cntrTrueTMC_u16 != COM_ZERO)
    {
        /* After evaluating filter for current signal/signal group, check if one of the signal/signal group filter
         * state is TRUE
         * Otherwise init value for Mode ie COM_FALSE is used
         */
        mode_b = COM_TRUE;
    }

    /*If there is a Mode change detected for the IPdu */
    if(Com_GetRamValue(TXIPDU,_TXMODESTATE,txIpduRamPtr_pst->transMode_u8) != mode_b)
    {
        returnValue_b = COM_TRUE;
        /* Update the Com_TxModeState with the Mode */
        Com_SetRamValue(TXIPDU,_TXMODESTATE,txIpduRamPtr_pst->transMode_u8,mode_b);
        /* Update the CurrentTxModePtr */
        Com_Prv_SetCurrentTxModePtr(txIpduRamPtr_pst,txIpduConstPtr_pcst,mode_b);

        /* Set the mode to latest mode */
        Com_SetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8,txIpduRamPtr_pst->currentTxModePtr_pcst->mode_u8);

        /* Note: Timer update because of mode change happens in Com_sendIpdu */
        /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
        /* Initialise MixedPhaseShift state, when IPdu tx mode is calculated in Com_IPduGroupStart() */
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,txIpduRamPtr_pst->transMode_u8,txIpduRamPtr_pst->currentTxModePtr_pcst->mixedPhaseShift_b);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
        /* FC_VariationPoint_END */
    }

    return(returnValue_b);
}

#endif /* #ifdef COM_TxFilters */

#ifdef COM_TX_IPDUCOUNTER
/**********************************************************************************************************************
 Function name    : Com_Prv_TxIpduInitializeCounter
 Description      : Internal function to reset counter value
 Parameter        : idComTxPdu_uo - Id of the Tx IPDU
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE void Com_Prv_TxIpduInitializeCounter(PduIdType idComTxPdu_uo)
{
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxIpduCntrCfg_tpcst         txIpduCntrConstPtr_pcst;
    uint8 *                         counterBytePtr_pu8;

    txIpduConstPtr_pcst      = COM_GET_TX_IPDU_CONSTDATA(idComTxPdu_uo);
    txIpduCntrConstPtr_pcst  = COM_GET_TX_IPDU_CNTR_CONSTDATA(txIpduConstPtr_pcst->idxTxIpduCntr_uo);

    counterBytePtr_pu8      = (uint8 *)(txIpduConstPtr_pcst->buffPtr_pu8 + (txIpduCntrConstPtr_pcst->bitPos_uo / 8u));

    /* TRACE[SWS_Com_00687] The AUTOSAR COM module shall set the I-PDU counter to 0, after the I-PDU was
     * reinitialized by Com_IpduGroupStart. */
    COM_GET_TXIPDU_CNTR_VAL(txIpduConstPtr_pcst->idxTxIpduCntr_uo) = COM_ZERO;

    /* IPdu is packed with counter value 'zero', this is required if Com_TriggerTransmit is invoked
     * before any transmission. */
    COM_PACK_CNTR_INTO_IPDU_BUFF((uint8 *)(counterBytePtr_pu8), (uint8)(txIpduCntrConstPtr_pcst->bitPos_uo % 8u),
                                           txIpduCntrConstPtr_pcst->bitSize_u8, COM_ZERO);
} /* End of Com_Prv_TxIpduInitializeCounter */

#endif /* #ifdef COM_TX_IPDUCOUNTER */


#ifdef COM_RxSigUpdateTimeout
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_LoadSigTimeoutValue
 Description      : Private function to load timeout value for Signal
 Parameter        : idSignal_u16           - ID of rx-signal.
                    sigUpdateBitStatus_u8  - NOT_APPLICABLE: none of the rx-signals are configured with UB & TO
                                             APPLICABLE    : some of the rx-signals are configured with UB & TO
                    idxSigToutPtr_pqu16    - Offset to be used to refer to the signal with UB & TO
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_LoadSigTimeoutValue(
                                        Com_SignalIdType    idSignal_u16,
                                        uint8               sigUpdateBitStatus_u8,
                                        uint16_least *      idxSigToutPtr_pqu16
                                             )
{
    Com_RxSigCfg_tpcst                  rxSigConstPtr_pcst;
    Com_RxIpduCfg_tpcst                 rxIpduConstPtr_pcst;
    const Com_SignalTimeoutInfo_tst *   sigTimeOutConstPtr_pcst;
    uint16 *                            sigTimeOutRamPtr_pu16;

    rxSigConstPtr_pcst  = COM_GET_RXSIG_CONSTDATA(idSignal_u16);

    /* Check if deadline monitoring needs to be performed for signal with update-bit, within
     * this IPDU.
     * This variable would be set UBDATEBIT_APPLICABLE only if atleast one the signal group within
     * the IPDU has configured update-bit & timeout value and deadline monitoring for IPDU
     * is not disabled.
     * check if present signal with update-bit has configured time out.
     * Only the signals with update-bit and configured timeout will be present in structure
     * Com_SignalTimeoutInfo_tst.
     */
    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(rxSigConstPtr_pcst->idComIPdu_uo);

    if (*idxSigToutPtr_pqu16 < rxIpduConstPtr_pcst->signalTimeoutRef_pcst->numWithUpdateBit_u8 )
    {
        /* Increment pointers to refer to current signal with update-bit & configured timeout
         * (within this IPDU) */
        sigTimeOutConstPtr_pcst = &rxIpduConstPtr_pcst->signalTimeoutRef_pcst->timeoutInfoRef_pcst[*idxSigToutPtr_pqu16];

        if (sigTimeOutConstPtr_pcst->idSig_u16 == (uint16)idSignal_u16)
        {
            if (sigUpdateBitStatus_u8 == COM_UPDATEBIT_SET)
            {
                /* Increment pointer to refer to current signal timer
                 * (within this IPDU) */
                sigTimeOutRamPtr_pu16 = &rxIpduConstPtr_pcst->signalTimeoutRef_pcst->timeoutTicks_pu16[*idxSigToutPtr_pqu16];

                /* Reload the timer for signal with TimeOut value
                 * Timer should be reloaded only if corresponding update-bit is set for the signal
                 * It is ensured by the code generator that if TimeOut value is zero, then the
                 * signal is not considered for deadline monitoring and hence an entry is the
                 * structure Com_SignalTimeoutInfo_tst is not present.
                 * Hence there is no need to check for zero value of TimeOut.
                 */
                Com_Get_SchM_Enter_Com_RxSigToTicks

/* FC_VariationPoint_START */
#if (COM_ENABLE_JITTERFLITER == STD_ON)
                /* CC requirements:
                 * An extra cycle is added to the timeout value after a PDU has been received incase filter is ON
                 * Provides a chance incase of jitter( extra cycle is added in normal case and hence
                 * if JITTERFILTER is enabled extra two cycles are added)
                 */
                (*sigTimeOutRamPtr_pu16) = sigTimeOutConstPtr_pcst->timeout_u16 + 2u;
#else
/* FC_VariationPoint_END   */
                /* Extra cycle is added to prevent early timeouts */
                (*sigTimeOutRamPtr_pu16) = sigTimeOutConstPtr_pcst->timeout_u16 + 1u;
/* FC_VariationPoint_START */
#endif /* #ifdef COM_ENABLE_JITTERFLITER */
/* FC_VariationPoint_END   */

                Com_Get_SchM_Exit_Com_RxSigToTicks
            }

            /* Increment signal TO instance to refer to next signal with update-bit & configured timeout
             * (within this IPDU) */
            (*idxSigToutPtr_pqu16)++;

        } /* Current signal matches with the signal in the timeout configuration */
    }
}
#endif /* #ifdef COM_RxSigUpdateTimeout */


#ifdef COM_RxSigGrpUpdateTimeout
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_LoadSigGrpTimeoutValue
 Description      : Private function to load timeout value for SignalGroup
 Parameter        : idSigGrp_u16            - ID of rx-signal group.
                    sigUpdateBitStatus_u8   - NOT_APPLICABLE: none of the rx-signals are configured with UB & TO
                                              APPLICABLE    : some of the rx-signals are configured with UB & TO
                    idxSigToutPtr_pqu16     - Offset to be used to refer to the signal with UB & TO
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_LoadSigGrpTimeoutValue(
                                            Com_SignalGroupIdType   idSigGrp_u16,
                                            uint8                   sigUpdateBitStatus_u8,
                                            uint16_least *          idxSigToutPtr_pqu16
                                                )
{
    Com_RxSigGrpCfg_tpcst               rxSigGrpConstPtr_pcst;
    Com_RxIpduCfg_tpcst                 rxIpduConstPtr_pcst;
    const Com_SignalTimeoutInfo_tst *   sigTimeOutConstPtr_pcst;
    uint16 *                            sigTimeOutRamPtr_pu16;

    rxSigGrpConstPtr_pcst  = COM_GET_RXSIGGRP_CONSTDATA(idSigGrp_u16);

    /* Check if deadline monitoring needs to be performed for signal groups with update-bit, within
     * this IPDU.
     * This variable would be set UBDATEBIT_APPLICABLE only if atleast one the signal group within
     * the IPDU has configured update-bit & timeout value and deadline monitoring for IPDU
     * is not disabled.
     * check if present signal group with update-bit has configured time out.
     * Only the signals with update-bit and configured timeout will be present in structure
     * Com_SignalTimeoutInfo_tst.
     */
    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(rxSigGrpConstPtr_pcst->idComIPdu_uo);

    if (*idxSigToutPtr_pqu16 < rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->numWithUpdateBit_u8 )
    {
        /* Increment pointers to refer to current signal with update-bit & configured timeout
         * (within this IPDU) */
        sigTimeOutConstPtr_pcst = &rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->timeoutInfoRef_pcst[*idxSigToutPtr_pqu16];

        if (sigTimeOutConstPtr_pcst->idSig_u16 == (uint16)idSigGrp_u16)
        {
            if (sigUpdateBitStatus_u8 == COM_UPDATEBIT_SET)
            {
                /* Increment pointer to refer to current signal timer
                 * (within this IPDU) */
                sigTimeOutRamPtr_pu16 = &rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->timeoutTicks_pu16[*idxSigToutPtr_pqu16];

                /* Reload the timer for signal with Timeout value
                 * Timer should be reloaded only if corresponding update-bit is set for the signal group
                 * It is ensured by the code generator that if TimeOut value is zero, then the
                 * signal group is not considered for deadline monitoring and hence an entry is the
                 * structure Com_SignalTimeoutInfo_tst is not present.
                 * Hence there is no need to check for zero value of Timeout.
                 */
                Com_Get_SchM_Enter_Com_RxSigGrpToTicks

/* FC_VariationPoint_START */
#if (COM_ENABLE_JITTERFLITER == STD_ON)
                /* CC requirements:
                 * An extra cycle is added to the timeout value after a PDU has been received incase filter is ON
                 * Provides a chance incase of jitter(extra cycle is added in normal case and hence
                 * if JITTERFILTER is enabled extra two cycles are added)
                 */
                (*sigTimeOutRamPtr_pu16) = sigTimeOutConstPtr_pcst->timeout_u16 + 2u;
#else
/* FC_VariationPoint_END   */
                /* Extra cycle is added to prevent early timeouts */
                (*sigTimeOutRamPtr_pu16) = sigTimeOutConstPtr_pcst->timeout_u16 + 1u;
/* FC_VariationPoint_START */
#endif /* #ifdef COM_ENABLE_JITTERFLITER */
/* FC_VariationPoint_END   */

                Com_Get_SchM_Exit_Com_RxSigGrpToTicks
            }

            /* Increment signal TO instance to refer to next signal with update-bit & configured timeout
             * (within this IPDU) */
            (*idxSigToutPtr_pqu16)++;

        } /* Current signalGroup matches with the signal in the timeout configuration */
    }
}
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidRxIpduId
 Description      : check if Rx IPDU ID is valid
 Parameter        : idPdu_uo - Id of the Rx IPDU
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidRxIpduId(PduIdType idPdu_uo)
{
    return (COM_IS_VALID_RXIPDU_ID(idPdu_uo));
}

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidTxIpduId
 Description      : check if Tx IPDU ID is valid
 Parameter        : idPdu_uo - Id of the Tx IPDU
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidTxIpduId(PduIdType idPdu_uo)
{
    return (COM_IS_VALID_TXIPDU_ID(idPdu_uo));
}

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidRxSignalId
 Description      : check if Rx Signal ID is valid
 Parameter        : idSignal_u16 - Id of the Rx Signal
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidRxSignalId(Com_SignalIdType idSignal_u16)
{
    return (COM_IS_VALID_RXSIGNAL_ID(idSignal_u16));
}

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidTxSignalId
 Description      : check if Tx Signal ID is valid
 Parameter        : idSignal_u16 - Id of the Tx Signal
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidTxSignalId(Com_SignalIdType idSignal_u16)
{
    return (COM_IS_VALID_TXSIGNAL_ID(idSignal_u16));
}

#ifdef COM_RX_SIGNALGROUP
/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidRxGroupSignalId
 Description      : check if Rx Group Signal ID is valid
 Parameter        : idSignal_u16 - Id of the Rx Group Signal
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidRxGroupSignalId(Com_SignalIdType idSignal_u16)
{
    return (COM_IS_VALID_RX_GRPSIGNAL_ID(idSignal_u16));
}

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidRxSignalGroupId
 Description      : check if Rx Signal Group ID is valid
 Parameter        : idSigGrp_u16 - Id of the Rx Signal Group
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidRxSignalGroupId(Com_SignalGroupIdType idSigGrp_u16)
{
    return (COM_IS_VALID_RXSIGNALGRP_ID(idSigGrp_u16));
}
#endif /* end of #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidTxGroupSignalId
 Description      : check if Tx Group Signal ID is valid
 Parameter        : idSignal_u16 - Id of the Tx Group Signal
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidTxGroupSignalId(Com_SignalIdType idSignal_u16)
{
    return (COM_IS_VALID_TX_GRPSIGNAL_ID(idSignal_u16));
}

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidTxSignalGroupId
 Description      : check if Tx Signal Group ID is valid
 Parameter        : idSigGrp_u16 - Id of the Tx Signal Group
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidTxSignalGroupId(Com_SignalGroupIdType idSigGrp_u16)
{
    return (COM_IS_VALID_TXSIGNALGRP_ID(idSigGrp_u16));
}
#endif /* end of #ifdef COM_TX_SIGNALGROUP */

/**********************************************************************************************************************
 Function name    : Com_Prv_IsValidIpduGroupId
 Description      : check if IPDU Group ID is valid
 Parameter        : idIpduGroup_u16 - Id of the IPDU Group
 Return value     : true - if it is a valid ID
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_IsValidIpduGroupId(Com_IpduGroupIdType idIpduGroup_u16)
{
    return (COM_IS_VALID_IPDUGRP_ID(idIpduGroup_u16));
}


#endif /* COM_PRV_INL_H */

