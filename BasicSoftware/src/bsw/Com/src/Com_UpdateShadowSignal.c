

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
 Function name    : Com_UpdateShadowSignal
 Description      : Service updates a group signal with the data, referenced by signalDataPtr_pcv
                    The update of the group signal data is done in the shadow buffer
 Parameter        : idSignal_u16        -> Id of the GroupSignal
                  : signalDataPtr_pcv   -> Pointer to the address where application data is available
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_UpdateShadowSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv)
{
# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_UpdateShadowSignal, COM_E_UNINIT);
    }
    else if (signalDataPtr_pcv == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_UpdateShadowSignal, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidTxGroupSignalId(idSignal_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_UpdateShadowSignal, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* Convert external HandleId to internal HandleId for Tx Signal
         * IMP : before accessing the tables, it is required to convert the IDs
         * For pre-compile , conversion is not required. Access Macro will return the same ID,
         * which is passed to it */
        idSignal_u16      = COM_GET_TXGRPSIGNAL_ID(idSignal_u16);

        Com_Prv_UpdateShadowSignal(idSignal_u16, signalDataPtr_pcv);
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_UpdateShadowSignal
 Description      : Service updates a group signal with the data, referenced by SignalDataPtr
                    The update of the group signal data is done in the shadow buffer
 Parameter        : idSignal_u16        -> Id of the GroupSignal
                  : signalDataPtr_pcv   -> Pointer to the address where application data is available
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_UpdateShadowSignal(Com_SignalIdType idSignal_u16, const void * signalDataPtr_pcv)
{
#ifdef COM_EffectiveSigGrpTOC
    Com_TxSigGrpRam_tpst            txSigGrpRamPtr_pst;
#endif
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    Com_SigMax_tuo                  txNewVal_uo;             /* Local variable to store the new value */
    uint16                          constByteValue_u16;
    uint16                          type_u16;                 /* Type of the GroupSignal */
    Com_MainFunc_tuo                idTxMainFunc_uo;

    txGrpSigConstPtr_pcst   = COM_GET_TXGRPSIG_CONSTDATA(idSignal_u16);

    /* Fetch the Tx-MainFunction internal Id */
    idTxMainFunc_uo = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(txGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;

    /*Get the Bit field Byte value*/
    constByteValue_u16  = txGrpSigConstPtr_pcst->txGrpSigFields_u16;
    type_u16             = Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16);
    txNewVal_uo         = COM_ZERO;

    /* Even if the IPDU Group is stopped this API will update the data in the Internal Buffer */
    if(type_u16 != (uint8)COM_UINT8_N)
    {
        switch(type_u16)
        {
        /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in AUTOSAR specification,
           so suppressing warning "Cast from a pointer to void to a pointer to object". */
        case COM_UINT8:
        case COM_SINT8:
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            txNewVal_uo = *(const uint8 * )signalDataPtr_pcv;
            break;
        case COM_UINT16:
        case COM_SINT16:
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            txNewVal_uo = *(const uint16 * )signalDataPtr_pcv;
            break;
        case COM_BOOLEAN:
            /* Update the Src_Buf with the data i.e typecast the data to the boolean variable*/
            txNewVal_uo = COM_BOOL_TO_UNSIGNED(*(const boolean * )signalDataPtr_pcv);
            break;
        case COM_UINT32:
        case COM_SINT32:
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            txNewVal_uo = *(const uint32 * )signalDataPtr_pcv;
            break;
#ifdef COM_TXGRPSIG_INT64
        case COM_UINT64:
        case COM_SINT64:
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            txNewVal_uo = *(const uint64 * )signalDataPtr_pcv;
            break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
        case COM_FLOAT32:
        {
            uint32 txNewVal_u32;

            txNewVal_u32 = COM_ZERO;

            /* Convert the float value into 32 bit stream and then pack the value into shadow buffer */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type, based on the pointers alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy( (void *)(&txNewVal_u32),(const void *)signalDataPtr_pcv, COM_F32_SIZE_IN_BYTES );

            txNewVal_uo = txNewVal_u32;
        }
            break;
#endif /* #ifdef COM_GRPSIGFLOAT32SUPP */

#ifdef COM_TXGRPSIG_FLOAT64SUPP
        case COM_FLOAT64:
            /* Pack the float value into float64 shadow buffer */
            /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            SchM_Enter_Com_TxGrpSigBuffer();
            COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo) =
                    *(const float64 * )signalDataPtr_pcv;
            SchM_Exit_Com_TxGrpSigBuffer();
            break;
#endif
/* FC_VariationPoint_END */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are
             * allowed to be configured, hence default case is intentionally left empty.
             */
            break;
        }

        /* Call a generic internal api which copies the TxNewVal data into corresponding GroupSignal buffer */
        Com_Prv_CopyTxGroupSignalData(idSignal_u16, txNewVal_uo, idTxMainFunc_uo);
        /* Before updating endianess conversion should be performed */
        /* Update the Signal in the Shadow buffer, here you have to find the bitposition in the shadow
        buffer for this signal  */
#ifdef COM_EffectiveSigGrpTOC
        txSigGrpRamPtr_pst = &COM_GET_TXSIGGRPFLAG(txGrpSigConstPtr_pcst->idSigGrp_uo);
        /* Check if the group signal contributes to evaluation of transfer property of signal group */
        if((Com_GetValue(TXGRPSIG,_TP,constByteValue_u16))  &&
           (COM_GET_OLDVALTRIGONCHANGE(txGrpSigConstPtr_pcst->idxOldValue_uo) != (Com_OldValTrigOnChng_tauo)txNewVal_uo))
        {
            /* Update the old value */
            COM_GET_OLDVALTRIGONCHANGE(txGrpSigConstPtr_pcst->idxOldValue_uo) = (Com_OldValTrigOnChng_tauo)txNewVal_uo;
            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,txSigGrpRamPtr_pst ->txSigGrpRAMFields_u8,COM_TRUE);
        }
#endif
    }
    else
    {
        /* The locks inside the function Com_ByteCopy() are removed.
         * Hence locks are used here
         * Copy the *signalDataPtr_pcv to all the Bytes of this signal in Shadow buffer */
        SchM_Enter_Com_TxGrpSigBuffer();

        /* MR12 RULE 11.5 VIOLATION: signalDataPtr_pcv is defined as void pointer in AUTOSAR specification,
           so suppressing warning "Cast from a pointer to void to a pointer to object". */
        Com_ByteCopy(&COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                (const uint8 * )signalDataPtr_pcv,(uint32)txGrpSigConstPtr_pcst->bitSize_uo);

        SchM_Exit_Com_TxGrpSigBuffer();
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */

