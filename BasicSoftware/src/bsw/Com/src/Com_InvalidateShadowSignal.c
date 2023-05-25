

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
 Function name    : Com_InvalidateShadowSignal
 Description      : Service for Invalidating the shadow signal (i.e Group signal)
 Parameter        : idSignal_u16
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_InvalidateShadowSignal(Com_SignalIdType idSignal_u16)
{
#ifdef COM_TxGrpSigInvalid

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateShadowSignal, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxGroupSignalId(idSignal_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateShadowSignal, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* Convert external HandleId to internal HandleId for Tx Group Signal
         * IMP : before accessing the tables, it is required to convert the IDs
         * For pre-compile, conversion is not required. Access Macro will return the same ID, which is passed to it */
        idSignal_u16 = COM_GET_TXGRPSIGNAL_ID(idSignal_u16);

        (void)Com_Prv_InternalInvalidateShadowSignal(idSignal_u16);
    }

#else
    (void)idSignal_u16;
#endif /* #ifdef COM_TxGrpSigInvalid */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InternalInvalidateShadowSignal
 Description      : Service for Invalidating the shadow signal (i.e Group signal)
                    This is an internal function to invalidate a group signal
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : idSignal_u16 -> Internal Id of the group signal. (Relevant for post-build case)
 Return value     : None
 **********************************************************************************************************************
*/
#ifdef COM_TxGrpSigInvalid

#define COM_START_SEC_CODE
#include "Com_MemMap.h"

uint8 Com_Prv_InternalInvalidateShadowSignal(Com_SignalIdType idSignal_u16)
{
    Com_TxGrpSigCfg_tpcst       txGrpSigConstPtr_pcst;
    /* TxGrpSig_NewVal datatype is uint32, as currently maximum 32bits are supported for Invalid value */
    uint32                      txGrpSigNewVal_u32;
    uint16                      constByteValue_u16;
    uint16                      type_u16;
    Com_MainFunc_tuo            idTxMainFunc_uo;
    uint8                       status_u8;
    status_u8 = COM_SERVICE_NOT_AVAILABLE;

    txGrpSigConstPtr_pcst   = COM_GET_TXGRPSIG_CONSTDATA(idSignal_u16);
    constByteValue_u16      = txGrpSigConstPtr_pcst->txGrpSigFields_u16;

    /* As per the SWS Page 42/43 (V2.1), we are supposed to call the Com_SendSignal( ) i.e Com_UpdateShadowSignal */
    /* As per COM286:By this call no send request shall be initiated */
    /*As per COM288, there is statement as "For processing, a Data invalid value must have been configured" */
    /* Pack the data invalid value into the shadow buffer for normal signals*/
    /*no call to UpdateShadow Signal is required for the same*/
    if(Com_GetValue(TXGRPSIG,_INVACTION,constByteValue_u16))
    {
        type_u16           = Com_GetValue(TXGRPSIG,_TYPE,constByteValue_u16);
        txGrpSigNewVal_u32 = txGrpSigConstPtr_pcst->dataInvalidVal_u32;
        status_u8          = E_OK;
        /* Fetch the Tx-MainFunction internal Id */
        idTxMainFunc_uo = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(txGrpSigConstPtr_pcst->idSigGrp_uo))->idComIPdu_uo))->idMainFunc_uo;

        if(type_u16 == COM_UINT8_N)
        {
            Com_ByteCopyInit(&COM_GET_TXGRPSIGTYPEU8_BUFF(idTxMainFunc_uo, txGrpSigConstPtr_pcst->idxTxGrpSigBuff_uo),
                             txGrpSigNewVal_u32, (uint32)txGrpSigConstPtr_pcst->bitSize_uo);
        }
        else
        {
            Com_Prv_CopyTxGroupSignalData(idSignal_u16, txGrpSigNewVal_u32, idTxMainFunc_uo);
        }
    }
    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TxGrpSigInvalid */

#endif /* #ifdef COM_TX_SIGNALGROUP */

