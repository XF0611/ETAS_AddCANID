

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#ifdef COM_METADATA_SUPPORT
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
 Function name    : Com_TriggerIPDUSendWithMetaData
 Description      : By a call to this API, the AUTOSAR COM module updates
                    its internal metadata for the I-PDU with the given ID by copying the metadata
                    from the given position and with respect to the globally configured metadata
                    length of the I-PDU. Then the I-PDU is triggered for transmission.
 Parameters       : idPdu_uo   -> The I-PDU-ID of the I-PDU that shall be triggered for sending
                    metaData_pu8 -> A pointer to the metadata for the triggered send-request
 Return value     : Std_ReturnType
                     E_OK     -  I-PDU was triggered for transmission
                     E_NOT_OK -  I-PDU is stopped, the transmission could not be triggered
 **********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(metaData_pu8) in a function prototype should be declared as pointer to
 * const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as below.
 * Hence suppressed */
Std_ReturnType Com_TriggerIPDUSendWithMetaData(PduIdType idPdu_uo, uint8 * metaData_pu8 )
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Std_ReturnType              returnVal_u8;

    returnVal_u8 = E_NOT_OK; /* Initialize Return value */

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerIPDUSendWithMetaData, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerIPDUSendWithMetaData, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idPdu_uo = COM_GET_TX_IPDU_ID(idPdu_uo);

        txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA( idPdu_uo );

        /* Check if received I-PDU is metadata I-PDU and if the given IPdu supports MetaData */
        if(
#ifdef COM_ECUC_AR_RELEASE_AR45
           Com_GetValue(TXIPDU,_IS_METADATAPDU,txIpduConstPtr_pcst->txIPduFields_u16) &&
#endif
          (txIpduConstPtr_pcst->txMetaData_pcst != NULL_PTR))
        {
            /* Check if IPdu Group containing the given IPdu is started */
            if (Com_Prv_CheckTxIPduStatus( idPdu_uo ))
            {
                Com_TxIpduRam_tpst txIpduRamPtr_pst;

                txIpduRamPtr_pst = &COM_GET_TXPDURAM_S( idPdu_uo );

# ifdef COM_TP_IPDUTYPE
                /* Proceed only if large data transmission is not in progress */
                if (COM_FALSE == Com_GetRamValue(TXIPDU, _LARGEDATAINPROG, txIpduRamPtr_pst->txFlags_u16))
# endif
                {
                    Com_SendIpduInfo_tst sendIpduFlag_st = COM_CLEAR_SENDIPDU_FLAGS;

                    /* As signal transfer property information is not known. It is assumed that, for transmission
                     * from this API, the signal transfer property is PENDING
                     * i.e, sendIpduFlag_st.isSigTriggered_u1 is COM_RESET */
                    /* indicate no mode change */
                    /* sendIpduFlag_st.isModeChangd_u1 = COM_RESET; */

                    sendIpduFlag_st.isEventTrig_u1      = COM_SET;

                    /* TRACE[SWS_Com_00388] Ignore repetitions */
                    sendIpduFlag_st.ignoreRepetitions_u1 = COM_SET;

# ifdef COM_TxIPduTimeOut
                    /* The timeout handling should happen as usual */
                    sendIpduFlag_st.isTimeoutReq_u1 = COM_BOOL_TO_UNSIGNED(txIpduConstPtr_pcst->timeout_u16 != COM_ZERO);
# endif

                    /* If no explicit meta data is given for a send request, the AUTOSAR COM module uses configured
                     * ComMetaDataDefault value for sending. Otherwise, copy the MetaData provided by caller */
                    if (NULL_PTR != metaData_pu8)
                    {
                        /* Lock the given TxIPdu buffer from updation in other API's */
                        SchM_Enter_Com_TxIpduProtArea( TRIGGERIPDUSENDWITHMETADATA );

#ifdef COM_ECUC_AR_RELEASE_AR45
                        /* Copy the given MetaData into TxIPdu Buffer */
                        Com_ByteCopy( (txIpduConstPtr_pcst->txMetaData_pcst->TxmetaDataValue_pau8),
                                      ((const uint8 * )metaData_pu8),
                                       txIpduConstPtr_pcst->txMetaData_pcst->TxmetaDataLength_u8);
#else
                        /* Copy the given MetaData into TxIPdu Buffer */
                        Com_ByteCopy( (txIpduConstPtr_pcst->buffPtr_pu8 + COM_GET_TXIPDU_SIZE(idPdu_uo)),
                                      ((const uint8 * )metaData_pu8),
                                      txIpduConstPtr_pcst->txMetaData_pcst->TxmetaDataLength_u8);
#endif

                        /* Set flag, that is to be used in Com_Prv_SendIpdu to retain caller metadata */
                        Com_SetRamValue(TXIPDU,_METADATA_REQUEST,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);

                        /* Release-Lock for the given TxIPdu buffer updation in other API's */
                        SchM_Exit_Com_TxIpduProtArea( TRIGGERIPDUSENDWITHMETADATA );
                    }
                    else
                    {
                        /* metadata field will be replaced with default metadata in Com_Prv_SendIpdu */
                    }

                    /* Send the given TxIPdu to the lower layer */
                    Com_Prv_SendIpdu( idPdu_uo, sendIpduFlag_st );

                    returnVal_u8 = E_OK;
                }
            }
        }
    }

    return returnVal_u8;
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

#endif /* #ifdef COM_METADATA_SUPPORT */

