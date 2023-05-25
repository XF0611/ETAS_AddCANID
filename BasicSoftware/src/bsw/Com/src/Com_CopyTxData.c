

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


#ifdef COM_TP_IPDUTYPE
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
 Function name    : Com_CopyTxData
 Description      : At invocation of Com_CopyTxData the AUTOSAR COM module copies the re-quested transmit data of the
                    large IPDU with ID PduId from its internal transmit buffer to the location specified by the
                    PduInfoPtr.
                    The function Com_CopyTxData also calculates and sets the availableDataPtr to the amount of remaining
                    bytes for the transmission of this large I-PDU. When TpDataState is TP_DATARETRY,
                    Com_CopyTxData copies previously transmitted data again.
 Parameter        : idPdu_uo,pduInfoPtr_pcst,retryInfoPtr_pst,availableDataPtr_puo(out)
 Return value     : BufReq_ReturnType
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(RetryInfoPtr) in a function prototype should be declared as pointer
 to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
 below, Hence suppressed */
#ifdef COM_ECUC_AR_RELEASE_AR45
BufReq_ReturnType Com_CopyTxData(
                            PduIdType               idPdu_uo,
                            const PduInfoType*      pduInfoPtr_pcst,
                            const RetryInfoType*    retryInfoPtr_pcst,
                            PduLengthType*          availableDataPtr_puo
                                )
#else
BufReq_ReturnType Com_CopyTxData(
                            PduIdType               idPdu_uo,
                            const PduInfoType*      pduInfoPtr_pcst,
                            RetryInfoType*          retryInfoPtr_pcst,
                            PduLengthType*          availableDataPtr_puo
                                )
#endif
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    BufReq_ReturnType           returnValue_en;

    /* Initialize variable */

    /* Return value with BUFREQ_E_NOT_OK indicates, IPDU is not started or large data pdu flag has not set or
     * DET error has occured*/
    returnValue_en = BUFREQ_E_NOT_OK;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyTxData, COM_E_UNINIT);
    }
    else if ((pduInfoPtr_pcst == NULL_PTR) || (availableDataPtr_puo == NULL_PTR))
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyTxData, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidTxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyTxData, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then idPdu_uo which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */

        idPdu_uo            = COM_GET_TX_IPDU_ID(idPdu_uo);

        txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idPdu_uo);

        /* Currently Retransmission feature is not supported */
        (void)retryInfoPtr_pcst;

        /* Check whether the large data pdu flag has been set */
        if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
        {
            /* Ensure that this is not query request */
            if (pduInfoPtr_pcst->SduLength != COM_ZERO)
            {
                /* Requested length should be less than the PduLength */
                if ((pduInfoPtr_pcst->SduLength + txIpduRamPtr_pst->txTPIPduLength_uo) <= COM_GET_TPTXIPDULENGTH(idPdu_uo))
                {

                    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idPdu_uo);

                    /* Now Copy the Data from the IPDU buffer to SduDataPointer */
                    Com_ByteCopy((uint8*)(pduInfoPtr_pcst->SduDataPtr),
                                         (txIpduConstPtr_pcst->buffPtr_pu8 + txIpduRamPtr_pst->txTPIPduLength_uo),
                                          pduInfoPtr_pcst->SduLength);

                    /* Keep track of how many bytes of data has been copied into the lower layer buffer,
                     * which is used as an offset for Ipdu buffer */
                    txIpduRamPtr_pst->txTPIPduLength_uo += pduInfoPtr_pcst->SduLength;

                    /* Update the out parameter availableDataPtr, with the number of bytes remaining */
                    *availableDataPtr_puo = COM_GET_TPTXIPDULENGTH(idPdu_uo) - txIpduRamPtr_pst->txTPIPduLength_uo;
                    /* Positive condition hence status as BUFREQ_OK */
                    returnValue_en = BUFREQ_OK;
                }
            }
            else
            {
                /* Update the out parameter availableDataPtr, with the number of bytes remaining */
                *availableDataPtr_puo = COM_GET_TPTXIPDULENGTH(idPdu_uo) - txIpduRamPtr_pst->txTPIPduLength_uo;
                /* Positive condition hence status as BUFREQ_OK */
                returnValue_en = BUFREQ_OK;
            }
        }
    }
    return returnValue_en;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

