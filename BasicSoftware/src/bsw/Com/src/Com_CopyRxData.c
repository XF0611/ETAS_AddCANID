

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
 Function name    : Com_CopyRxData
 Description      : Called once upon reception of each segment. Within this call, the received data is copied to the
                    receive TP buffer. The API should only be called with an SduLength greater 0 if the RxBufferSizePtr
                    returned by the previous API call indicates sufficient receive buffer (SduLength = RxBufferSizePtr).
                    The function must only be called if the connection has been accepted by an initial call to
                    Com_StartOfReception.
 Parameter        : idPdu_uo,pduInfoPtr_pcst,rxBufferSizePtr_puo(out)
 Return value     : BufReq_ReturnType
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
BufReq_ReturnType Com_CopyRxData(
                            PduIdType           idPdu_uo,
                            const PduInfoType*  pduInfoPtr_pcst,
                            PduLengthType*      rxBufferSizePtr_puo
                                )
{

    /* Start: Variable declarations */
    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    BufReq_ReturnType           returnValue_en;
    /* End: Variable declarations */

    /* Initialize varibale */
    /* Return value with BUFREQ_E_NOT_OK indicates, IPDU is not started or large data pdu flag has not set or
     * DET error has occured */
    returnValue_en = BUFREQ_E_NOT_OK;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyRxData, COM_E_UNINIT);
    }
    else if ((pduInfoPtr_pcst == NULL_PTR) || (rxBufferSizePtr_puo == NULL_PTR))
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyRxData, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_CopyRxData, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then idPdu_uo which is passed to this function will be changed to internal Id
         * which is generated through configuration. If PC variant is selected, then no mapping table will be used.
         */

        idPdu_uo                = COM_GET_RX_IPDU_ID(idPdu_uo);

        rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idPdu_uo);

        rxIpduRamPtr_pst        = &COM_GET_RXPDURAM_S(idPdu_uo);

        /* Check whether RxPdu flag is set, which will be done in Com_StartOfReception */
        if(Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,rxIpduRamPtr_pst->rxFlags_u8))
        {
            /* Check whether the requested length fits the Ipdu */
            /* This check is to protect from RAM corruption when TP recieves more than what was expected */
            if((rxIpduRamPtr_pst->rxTPIPduLength_uo + pduInfoPtr_pcst->SduLength) <= COM_GET_RXIPDU_SIZE(idPdu_uo))
            {
                /*Check if the requested length is a non-zero value*/
                if(COM_ZERO != pduInfoPtr_pcst->SduLength)
                {
                    /* Now Copy the Data into the IPDU buffer */
                     Com_ByteCopy((rxIpduConstPtr_pcst->buffPtr_pau8 + rxIpduRamPtr_pst->rxTPIPduLength_uo),
                                  (uint8*)(pduInfoPtr_pcst->SduDataPtr), pduInfoPtr_pcst->SduLength);
                     rxIpduRamPtr_pst->rxTPIPduLength_uo += pduInfoPtr_pcst->SduLength ;
                }
                /* Update the out parameter with the remaining buffer length */
                *rxBufferSizePtr_puo = COM_GET_RXIPDU_SIZE(idPdu_uo) - rxIpduRamPtr_pst->rxTPIPduLength_uo;
                /* Return with BUFREQ_OK indicates Data copied successfully */
                returnValue_en = BUFREQ_OK;

            } /* if((RxIpduRamPtr->RxTPIPduLength + pduInfoPtr_pcst->SduLength) <= rxIpduConstPtr_pcst->Size) */
            else
            {
                /* If the TP request more than the available RxBuffer size return with BUFREQ_E_OVFL */
                returnValue_en = BUFREQ_E_OVFL;
            }
        }/* if(Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,RxIpduRamPtr->RxFlags)) */
    }

    return returnValue_en;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

