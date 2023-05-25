

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
 Function name    : Com_InvalidateSignalGroup
 Description      : Service for Invalidating the Signal Group
 Parameter        : idSignalGroup_u16 -> Id of the Signal Group
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint8 Com_InvalidateSignalGroup(Com_SignalGroupIdType idSignalGroup_u16)
{
#ifdef COM_TxGrpSigInvalid
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
    uint16_least                    idxIdGrpSig_qu16;
    uint16_least                    idMaxTxGrpSig_qu16;
    /* Status of InvalidValue is configured for any of the GroupSignal or not  */
    uint8                           invalidValueFound_u8;
    uint8                           status_u8;

    /* Initialize status variable */
    invalidValueFound_u8 = COM_ZERO;
    status_u8 = COM_SERVICE_NOT_AVAILABLE;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateSignalGroup, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxSignalGroupId(idSignalGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_InvalidateSignalGroup, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */

        idSignalGroup_u16 = COM_GET_TXSIGNALGRP_ID(idSignalGroup_u16);

        /* If the IPdu group is stopped the call to the function shall still update the values
         * in the COM internal buff-ers but the COM_SERVICE_NOT_AVAILABLE is returned */
        /* For Each Group signal of this Signal Group perform the Invalidate shadow signal */

        txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(idSignalGroup_u16);

        txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);

        idxIdGrpSig_qu16 = txSigGrpConstPtr_pcst->idFirstGrpSig_uo;
        idMaxTxGrpSig_qu16  = idxIdGrpSig_qu16 + txSigGrpConstPtr_pcst->numOfGrpSig_uo;

        for ( ; idxIdGrpSig_qu16 < idMaxTxGrpSig_qu16; idxIdGrpSig_qu16++ )
        {
            /*Note: Uint8[n] check is done in Com_InvalidateShadowSignal() so no need of Check here*/
            /* Call to  Com_Prv_InternalInvalidateShadowSignal( ) has to be done to invalidate the signal,
             * with the signal Id */
            if (Com_GetValue(TXGRPSIG,_INVACTION,txGrpSigConstPtr_pcst->txGrpSigFields_u16))
            {
                status_u8 = Com_Prv_InternalInvalidateShadowSignal((Com_SignalIdType)idxIdGrpSig_qu16);
                /* Set the flag indicating ComSignalDataInvalidValue is configured for any of the group signals
                 * in SignalGroup */
                invalidValueFound_u8 = COM_ONE;
            }
            txGrpSigConstPtr_pcst++;
        }
        /* Check if ComSignalDataInvalidValue is configured for any of the group signals in SignalGroup */
        if (invalidValueFound_u8 != COM_ZERO)
        {
#ifdef COM_TP_IPDUTYPE
            /* Proceed only if Large Data tranmission is not in Progress */
            if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,COM_GET_TXPDURAM_S(txSigGrpConstPtr_pcst->idComIPdu_uo).txFlags_u16))
            {
                status_u8 = COM_BUSY;

            }
            else
#endif /*#ifdef COM_TP_IPDUTYPE*/
            {
                /* TRACE[SWS_Com_00645] Com_SendSignalGroup() is performed internally. As per design
                * Com_Prv_InternalSendSignalGroup() is called. */
                status_u8 = Com_Prv_InternalSendSignalGroup(idSignalGroup_u16);
            }
        }
        else
        {
            /* Return COM_SERVICE_NOT_AVAILABLE if no ComSignalDataInvalidValue is configured for any of the
             * group signals */
        }
    }
    /* Note: if IPdu Group is stopped, then the Com_Prv_InternalSendSignalGroup( ) will return COM_SERVICE_NOT_AVAILABLE */
    return (status_u8);
#else
    (void)idSignalGroup_u16;
    return( COM_SERVICE_NOT_AVAILABLE );
#endif /* #ifdef COM_TxGrpSigInvalid */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */


