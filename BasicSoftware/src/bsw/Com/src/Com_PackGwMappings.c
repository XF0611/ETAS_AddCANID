

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
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

#ifdef COM_SIGNALGATEWAY
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

/*
 **********************************************************************************************************************
 * Function name  : Com_Prv_PackRxSignalGwBufferData
 * Description    : This function is called to
 *                  1. Set Update bit if relevant
 *                  2. Copy the data from the Rx signal buffer and pack into Tx Ipdu buffer
 *                  3. Evaluate transfer property and transmission mode selection for each signal and
 *                  update the Tx Ipdu flag accordingly
 * Parameter      : Com_RxSigCfg_tpcst rxSigConstPtr_pcst
 *                : Com_SignalIdType      idTxGwDest_u16
 * Return value   : void
 **********************************************************************************************************************
*/
void Com_Prv_PackRxSignalGwBufferData(Com_RxSigCfg_tpcst rxSigConstPtr_pcst, Com_SignalIdType idTxGwDest_u16)
{
    Com_TxSigCfg_tpcst          txSigConstPtr_pcst;
    Com_SigBuffIndex_tuo        idxRxSigBuff_uo;
    uint8                       type_u8;
    Com_MainFunc_tuo            idRxMainFunc_uo;

    txSigConstPtr_pcst      = COM_GET_TXSIG_CONSTDATA(idTxGwDest_u16);
    type_u8                 = Com_GetValue(GEN,_TYPE,txSigConstPtr_pcst->general_u8);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo         = (COM_GET_RX_IPDU_CONSTDATA(rxSigConstPtr_pcst->idComIPdu_uo))->idMainFunc_uo;

    idxRxSigBuff_uo         = rxSigConstPtr_pcst->idxSigBuff_uo;

    if(type_u8 != (uint8)COM_UINT8_N)
    {
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
        float64                 txNewVal_f64;
#endif
        Com_SigMax_tuo          txNewVal_uo;

        txNewVal_uo         = COM_ZERO;

        /* Refer to Com_Cfg_SchM file; this is a multi-core lock */
        Com_Get_SchM_Enter_Com_RxSigGwMcLockProtArea

        /* Com_Prv_InternalSendSignal expects SignalDataPtr address to a variable whose type is similar to that of signal
         * type i.e., n-bytes of data will be read from the passed address. So, for integer signals
         * copy n-bytes of data to be routed in the first n-address location of local variable "TxNewVal".
         * Hence typecast is added for local variable while copying received signal */

        switch(type_u8 >> 1u)
        {
        case 0x00u:     /* COM_UINT8 & COM_SINT8 */
        case (COM_BOOLEAN >> 1u):
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            *((uint8 *)&txNewVal_uo) = COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo);
            break;

        case 0x01u:     /* COM_UINT16 & COM_SINT16 */
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            /* MR12 RULE 11.3 VIOLATION: typecast is inline with the data read operation, hence suppressed */
            *((uint16 *)&txNewVal_uo) = COM_GET_SIGTYPEUINT16_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo);
            break;

        case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case (COM_FLOAT32 >> 1u):
#endif /* #ifdef COM_FLOAT32SUPP */
/* FC_VariationPoint_END */
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            /* MR12 RULE 11.3 VIOLATION: typecast is inline with the data read operation, hence suppressed */
            *((uint32 *)&txNewVal_uo) = COM_GET_SIGTYPEUINT32_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo);
            break;

#if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64)
        case 0x08u:     /* COM_UINT64 & COM_SINT64 */
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            *((uint64 *)&txNewVal_uo)  = COM_GET_SIGTYPEUINT64_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo);
            break;
#endif /* #if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64) */

/* FC_VariationPoint_START */
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
        case (COM_FLOAT64 >> 1u):
            txNewVal_f64 = COM_GET_SIGTYPEFLOAT64_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo);
            break;
#endif /* #if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
        }

        Com_Get_SchM_Exit_Com_RxSigGwMcLockProtArea

/* FC_VariationPoint_START */
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
        if(type_u8 == COM_FLOAT64)
        {
            /* 1. Set the UpdateBit, if configured
             * 2. Update TxIPdu buffer with the routed Signal data
             * Return value is based on IPduGroup status that can be ignored here, as it is checked later
             * in Com_MainFunctionRouteSignals()
             */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_Prv_InternalSendSignal() definition.
               But the void pointer is always deferenced to a same type. Hence the alignment warning
               can safely be ignored. */
            (void)Com_Prv_InternalSendSignal( idTxGwDest_u16, (const void *)&txNewVal_f64 );

        }
        else
#endif /* #if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
        {
            /* 1. Set the UpdateBit, if configured
             * 2. Update TxIPdu buffer with the routed Signal data
             * 3. Evaluate TMC of Signal
             * 4. Evaluate TMS of TxIPdu
             * Return value is based on IPduGroup status that can be ignored here, as it is checked later
             * in Com_MainFunctionRouteSignals()
             */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_Prv_InternalSendSignal() definition.
               But the void pointer is always deferenced to a same/lesser integer type & it is 32bit aligned.
               Hence the alignment warning can safely be ignored. */
            (void)Com_Prv_InternalSendSignal( idTxGwDest_u16, (const void *)&txNewVal_uo );

        }
    }
    else
    {
        /* Refer to Com_Cfg_SchM file; this is a multi-core lock */
        Com_Get_SchM_Enter_Com_RxSigGwMcLockProtArea

        /* SignalType: UINT8_N */
        /* 1. Set the UpdateBit, if configured
         * 2. Update TxIPdu buffer with the routed Signal data
         * 3. Evaluate TMC of Signal
         * 4. Evaluate TMS of TxIPdu
         * Return value is based on IPduGroup status that can be ignored here, as it is checked later
         * in Com_MainFunctionRouteSignals()
         */
        /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_Prv_InternalSendSignal() definition.
           But the void pointer is always deferenced to a same type. Hence the alignment warning
           can safely be ignored. */
        (void)Com_Prv_InternalSendSignal( idTxGwDest_u16, (const void *)&COM_GET_SIGTYPEUINT8_BUFF(idRxMainFunc_uo, idxRxSigBuff_uo) );

        Com_Get_SchM_Exit_Com_RxSigGwMcLockProtArea
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_SIGNALGATEWAY */


#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_PackRxGrpSigGwBufferData
 Description      : This function is called to
                    Copy the data from the Rx GroupSignal buffer to its corresponding Tx GroupSignal
 Parameter        : Com_RxGrpSigCfg_tpcst RxGrpSigConstPtr
                  : Com_TxIntGrpSignalId_tuo idTxGwDest_uo
 Return value     : void
 **********************************************************************************************************************
*/

void Com_Prv_PackRxGrpSigGwBufferData(Com_RxGrpSigCfg_tpcst rxGrpSigConstPtr_pcst, Com_TxIntGrpSignalId_tuo idTxGwDest_uo)
{
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_TxSigGrpRam_tpst            txSigGrpRamPtr_pst;
    Com_SigMax_tuo                  txNewVal_uo;         /* Required to check for TriggeredOnChange GroupSignal */
    Com_RxGrpSigBuffIndex_tuo       idxRxGrpSignalBuff_uo;
    uint8                           type_u8;             /* Type of the GroupSignal */
    Com_MainFunc_tuo                idRxMainFunc_uo;
    Com_MainFunc_tuo                idTxMainFunc_uo;

    /* Com_Cfg_SchM
     * Tx-GroupSignal buffers are not protected here, as this is the only place where gateway buffers are updated. */

    txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(idTxGwDest_uo);

    txSigGrpRamPtr_pst   = &COM_GET_TXSIGGRPFLAG(txGrpSigConstPtr_pcst->idSigGrp_uo);

    type_u8 = Com_GetValue(RXGRPSIG,_TYPE,rxGrpSigConstPtr_pcst->rxGrpSigFields_u8);

    /* Fetch the Rx-MainFunction internal Id */
    idRxMainFunc_uo = (COM_GET_RX_IPDU_CONSTDATA((COM_GET_RXSIGGRP_CONSTDATA(rxGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;
    /* Fetch the Tx-MainFunction internal Id */
    idTxMainFunc_uo = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(txGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;

    txNewVal_uo = COM_ZERO;

    idxRxGrpSignalBuff_uo = rxGrpSigConstPtr_pcst->idxRxGrpSigBuff_uo;

    /* Even if the IPDU Group is stopped this API will update the data into internal buffer */
    if(type_u8 != (uint8)COM_UINT8_N)
    {
        switch(type_u8 >> 1u)
        {
            case 0x00u:     /* COM_UINT8 & COM_SINT8 */
            case (COM_BOOLEAN >> 1u):
            {
                /* Update the Src_Buf with the data i.e typecast the data to uint8*/
                txNewVal_uo = COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo);
                COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo) = (uint8)txNewVal_uo;
            }
            break;

            case 0x01u:     /* COM_UINT16 & COM_SINT16 */
            {
#if (CPU_TYPE == CPU_TYPE_8)
                Com_Get_SchM_Enter_Com_RxGrpSigSecBuff
#endif
                /* Update the Src_Buf with the data i.e typecast the data to uint16*/
                txNewVal_uo = COM_GET_SECRXGRPSIGTYPEU16_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo);
#if (CPU_TYPE == CPU_TYPE_8)
                Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
#endif
                COM_GET_TXGRPSIGTYPEU16_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo)  = (uint16)txNewVal_uo;
            }
            break;

            case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
            case (COM_FLOAT32 >> 1u):
#endif
/* FC_VariationPoint_END */
            {
#if (CPU_TYPE != CPU_TYPE_32)
                Com_Get_SchM_Enter_Com_RxGrpSigSecBuff
#endif
                /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
                txNewVal_uo = COM_GET_SECRXGRPSIGTYPEU32_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo);
#if (CPU_TYPE != CPU_TYPE_32)
                Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
#endif
                COM_GET_TXGRPSIGTYPEU32_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo)  = (uint32)txNewVal_uo;
            }
            break;

#if defined(COM_RXGRPSIG_INT64) && defined(COM_TXGRPSIG_INT64)
            case 0x08u:     /* COM_UINT64 & COM_SINT64 */
            {
                Com_Get_SchM_Enter_Com_RxGrpSigSecBuff
                /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
                txNewVal_uo = COM_GET_SECRXGRPSIGTYPEU64_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo);
                Com_Get_SchM_Exit_Com_RxGrpSigSecBuff

                COM_GET_TXGRPSIGTYPEU64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo)  = txNewVal_uo;
            }
            break;
#endif /* #if defined(COM_RXGRPSIG_INT64) && defined(COM_TXGRPSIG_INT64) */

/* FC_VariationPoint_START */
#if defined(COM_TXGRPSIG_FLOAT64SUPP) && defined(COM_RXGRPSIG_FLOAT64SUPP)
            case (COM_FLOAT64 >> 1u):
            {
                Com_Get_SchM_Enter_Com_RxGrpSigSecBuff

                /* Pack the float value into float64 shadow buffer */
                COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo) =
                             COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo);

                Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
            }
            break;
#endif /* #if defined(COM_TXGRPSIG_FLOAT64SUPP) && defined(COM_RXGRPSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
        }

        /* Before updating endianess conversion should be performed */
        /* Update the Signal in the Shadow buffer, here you have to find the bitposition in the shadow
         * buffer for this signal  */

#ifdef COM_EffectiveSigGrpTOC
        /* Check if the group signal contributes to evaluation of transfer property of signal group */
        if((Com_GetValue(TXGRPSIG,_TP,txGrpSigConstPtr_pcst->txGrpSigFields_u16))  &&
           (COM_GET_OLDVALTRIGONCHANGE(txGrpSigConstPtr_pcst->idxOldValue_uo) != (Com_OldValTrigOnChng_tauo)txNewVal_uo))
        {
            /* Update the old value */
            COM_GET_OLDVALTRIGONCHANGE(txGrpSigConstPtr_pcst->idxOldValue_uo) = (Com_OldValTrigOnChng_tauo)txNewVal_uo;

            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,txSigGrpRamPtr_pst->txSigGrpRAMFields_u8,COM_TRUE);
        }
#endif
    }
    else
    {
        /* Route UINT8_N GroupSignal data */
        Com_Get_SchM_Enter_Com_RxGrpSigSecBuff

        Com_ByteCopy(&COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                     &COM_GET_SECRXGRPSIGTYPEU8_BUFF(idRxMainFunc_uo, idxRxGrpSignalBuff_uo),
                     (uint32)txGrpSigConstPtr_pcst->bitSize_uo);

        Com_Get_SchM_Exit_Com_RxGrpSigSecBuff
    }

    /* This is to flag the signal group, that a group signal within it was updated */
    Com_SetRamValue(TXSIGGRP,_ISGWUPDTD,txSigGrpRamPtr_pst->txSigGrpRAMFields_u8,COM_TRUE);

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP)) */

