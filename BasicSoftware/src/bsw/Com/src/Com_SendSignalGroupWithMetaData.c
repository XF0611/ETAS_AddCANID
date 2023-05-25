

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#ifdef COM_ECUC_AR_RELEASE_AR45

#if defined(COM_TX_SIGNALGROUP) && defined(COM_METADATA_SUPPORT)

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
 Function name    : Com_SendSignalGroupWithMetaData
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and meta data provided via metadataPtr parameter.
 Parameter        : idSignal_u16 -> Id of the signal.
                  : signalDataPtr_pcv -> The pointer to the address where the application data is available.
                  : metaDataPtr_pu8 -> The pointer provides meta data
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/COM_BUSY
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"


uint8 Com_SendSignalGroupWithMetaData(Com_SignalGroupIdType idSignalGroup_u16, const uint8 * metaDataPtr_pu8)
{
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxSigGrpCfg_tpcst       txSigGrpConstPtr_pcst; /* Local pointer to Tx SignalGroup static configuration */
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;  /* Local pointer to Tx IPdu   static configuration */
    PduIdType                   idIpdu_uo;
    uint8                       status_u8;           /* Return status */

    status_u8 = COM_SERVICE_NOT_AVAILABLE;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignalGroup, COM_E_UNINIT);
    }
    else if (metaDataPtr_pu8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignalGroup, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidTxSignalGroupId(idSignalGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SendSignalGroup, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idSignalGroup_u16     = COM_GET_TXSIGNALGRP_ID(idSignalGroup_u16);

        txSigGrpConstPtr_pcst = COM_GET_TXSIGGRP_CONSTDATA(idSignalGroup_u16);
        idIpdu_uo             = txSigGrpConstPtr_pcst->idComIPdu_uo;
        txIpduConstPtr_pcst   = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);

        /* Check if received signal is a part of metadata I-PDU and if the given I-PDU supports MetaData */
        if(Com_GetValue(TXIPDU,_IS_METADATAPDU,txIpduConstPtr_pcst->txIPduFields_u16) &&
          (txIpduConstPtr_pcst->txMetaData_pcst != NULL_PTR))
        {
            txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idIpdu_uo);
        /* Proceed only if Large Data tranmission is not in Progress */
#ifdef COM_TP_IPDUTYPE
            if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
            {
                status_u8 = COM_BUSY;
            }
            else
#endif /*#ifdef COM_TP_IPDUTYPE*/
            {
                status_u8 = Com_Prv_InternalSendSignalGroup(idSignalGroup_u16);

                if (NULL_PTR != metaDataPtr_pu8)
                {
                    /* Lock the given TxIPdu buffer from updation in other API's */
                    SchM_Enter_Com_TxIpduProtArea(TRANSMIT_METADATA);

                    /* Copy the given MetaData into TxIPdu Buffer */
                    Com_ByteCopy( (txIpduConstPtr_pcst->txMetaData_pcst->TxmetaDataValue_pau8),
                                  ((const uint8 * )metaDataPtr_pu8),
                                   txIpduConstPtr_pcst->txMetaData_pcst->TxmetaDataLength_u8);

                    /* Set flag, that is to be used in Com_Prv_SendIpdu to retain caller metadata */
                    Com_SetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);

                    /* Release-Lock for the given TxIPdu buffer updation in other API's */
                    SchM_Exit_Com_TxIpduProtArea(TRANSMIT_METADATA);
                }
            }
        }
    }
    return status_u8;
}


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined (COM_TX_SIGNALGROUP) && defined(COM_METADATA_SUPPORT) */

#endif

