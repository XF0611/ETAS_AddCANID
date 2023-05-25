

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
 Function name    : Com_TpTxConfirmation
 Description      : This function is called by the PduR after a large I-PDU has been transmitted via the transport
                    protocol on its network.
 Parameter        : idPdu_uo,result_u8
 Return value     : Void
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_TpTxConfirmation(PduIdType idPdu_uo, Std_ReturnType result_u8)
{

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TpTxConfirmation, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TpTxConfirmation, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then idPdu_uo which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idPdu_uo = COM_GET_TX_IPDU_ID(idPdu_uo);

        if (Com_Prv_CheckTxIPduStatus(idPdu_uo))
        {
            Com_TxIpduRam_tpst txIpduRamPtr_pst;

            txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(idPdu_uo);

            /* Check whether the large data pdu flag has been set */
            if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
            {
                if (E_OK == result_u8)
                {
                    /* Process TxConfirmation only if the transmission was successfull */
                    Com_Prv_InternalTxConfirmation(idPdu_uo);
                }

                /* TRACE[SWS_Com_00662] After successful initiation of a transmission of a large I-PDU via
                 * PduR_ComTransmit, the AUTOSAR COM module shall not modify the data of its
                 * internal transmit buffer of this I-PDU unless the AUTOSAR COM module is notified
                 * about either the successful transmission or an error indicating that the transmission
                 * was aborted. */
                /* Keep IPDU buffer locked until TxConfirmation processing is complete.
                 * This is done here, to clear update bits only for already transmitted signals.
                 * and to prevent clearing of update-bits that are not actually transmitted. */
                Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
            }
        }
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

