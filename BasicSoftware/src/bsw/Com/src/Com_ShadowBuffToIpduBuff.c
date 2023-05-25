

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"


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
 Function name    : Com_Prv_ShadowBuffToIpduBuff
 Description      : Internal function used for copying the Signal Group data from the shadow buffer to IPDU buffer
 Parameter        : idSignalGrp_u16 -> Id of the SignalGroup
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_ShadowBuffToIpduBuff(Com_SignalGroupIdType idSignalGrp_u16)
{
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    Com_TxIpduRam_tpst              txIpduRamPtr_pst;
#endif
    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    Com_SigMax_tuo                  txGrpSigNewVal_uo;
    uint16_least                    idxTxGrpSig_qu16;
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    PduLengthType                   startByteNo_uo;
    uint16                          dynLength_u16;
    PduIdType                       idIpdu_uo;
#endif
    uint16                          constByteValue_u16;
    uint16                          sigType_u16;
    Com_MainFunc_tuo                idTxMainFunc_uo;       /* Fetch the Tx-MainFunction internal Id */

    txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(idSignalGrp_u16);
    txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);
    txIpduConstPtr_pcst   = COM_GET_TX_IPDU_CONSTDATA(txSigGrpConstPtr_pcst->idComIPdu_uo);
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    idIpdu_uo             = txSigGrpConstPtr_pcst->idComIPdu_uo;
    txIpduRamPtr_pst      = &COM_GET_TXPDURAM_S(idIpdu_uo);
#endif
    idTxMainFunc_uo       = txIpduConstPtr_pcst->idMainFunc_uo;

    /*To avoid misra warning*/
    txGrpSigNewVal_uo  = COM_ZERO;

    for (idxTxGrpSig_qu16 = txSigGrpConstPtr_pcst->numOfGrpSig_uo; idxTxGrpSig_qu16 > COM_ZERO; idxTxGrpSig_qu16--)
    {
        constByteValue_u16 = txGrpSigConstPtr_pcst->txGrpSigFields_u16;

        sigType_u16 = Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16);
        if(sigType_u16 != (uint8)COM_UINT8_N)
        {
            uint8 sigEndianess_u8;

            sigEndianess_u8 = Com_GetValue(TXGRPSIG,_ENDIANESS,constByteValue_u16);

            /* Now copy the bytes to shadow buffer */
            /* Size of the shadow buffer */
            switch(sigType_u16 >> 1u)
            {
            case 0x00u: /* UINT8, SINT8 signal */
            case 0x03u: /* Boolean Signal      */
                txGrpSigNewVal_uo = COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo);
                break;

            case 0x01u:
                /* UINT16 signal */
                txGrpSigNewVal_uo = COM_GET_TXGRPSIGTYPEU16_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo);
                break;

            case 0x02u:
                /* UINT32 signal */
                /* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
            case (COM_FLOAT32 >> 1u):
                /* FLOAT32 signal */
#endif
                /* FC_VariationPoint_END */
                txGrpSigNewVal_uo = COM_GET_TXGRPSIGTYPEU32_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo);
                break;

#ifdef COM_TXGRPSIG_INT64
            case 0x08u:
                /* UINT64 signal */
                txGrpSigNewVal_uo = COM_GET_TXGRPSIGTYPEU64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo);
                break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

            default:
                /**
                 * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are
                 * allowed to be configured, hence default case is intentionally left empty.
                 */
                /**
                 * Group signal type UINT8_DYN and FLOAT64 is not handled/processed in switch case statements.
                 * Both group signal types are handled/processed separately
                 */
                break;

            }

#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
            if(sigType_u16 == COM_UINT8_DYN)
            {
                /* Calculate Dynamic Signal Start Position */
                startByteNo_uo  = (PduLengthType)(txGrpSigConstPtr_pcst->bitPos_uo >> 3u);
                dynLength_u16   = txIpduRamPtr_pst->dynTxGrpSigLength_u16;

                /* Now copy the Group Signal into the SignalDataPtr from Shadow Buffer */
                Com_ByteCopy( ( txIpduConstPtr_pcst->buffPtr_pu8 + startByteNo_uo ),
                                &COM_GET_TXGRPSIGTYPEUINT8DYN_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                                dynLength_u16 );
                txIpduRamPtr_pst->dynLength_u16 = dynLength_u16;
            }
            else
#endif /* #ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT */
            /* FC_VariationPoint_START */
#ifdef COM_TXGRPSIG_FLOAT64SUPP
            if(sigType_u16 == COM_FLOAT64)
            {
                /* Convert the float value into 32 bit stream and then pack the value */
                Com_Prv_PackFloatSignal(
                            sigEndianess_u8, txGrpSigConstPtr_pcst->bitPos_uo,
                            COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                            txIpduConstPtr_pcst->buffPtr_pu8
                                   );
            }
            else
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */
            /* FC_VariationPoint_END */
            {
                /*Now Update the Ipdu buffer with the value obtained*/
                Com_Prv_PackSignal(sigEndianess_u8,txGrpSigConstPtr_pcst->bitPos_uo,txGrpSigConstPtr_pcst->bitSize_uo,
                               txGrpSigNewVal_uo,txIpduConstPtr_pcst->buffPtr_pu8);
            }
        }
        else
        {
            /* sigType_u16 == COM_UINT8_N */
            PduLengthType byteNo_uo;
            /* Find the starting byte position(byteNo_uo) of ComGroupSignal in the IPDU buffer */
            byteNo_uo = ( PduLengthType )( txGrpSigConstPtr_pcst->bitPos_uo >> 3 );
            /* Now copy the Group Signal into the SignalDataPtr from Shadow Buffer */
            Com_ByteCopy( ( txIpduConstPtr_pcst->buffPtr_pu8 + byteNo_uo ),
                            &COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                            txGrpSigConstPtr_pcst->bitSize_uo );
        }

        txGrpSigConstPtr_pcst++;
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /*#ifdef COM_TX_SIGNALGROUP*/

