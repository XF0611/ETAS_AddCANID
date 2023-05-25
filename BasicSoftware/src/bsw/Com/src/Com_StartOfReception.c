

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
 Function name    : Com_StartOfReception
 Description      : Com_StartOfReception initiates reception of the TP I-PDU with ID ComRxPduId.
                    It returns the size of the internal receive buffer for the I-PDU via RxBufferSizePtr.
 Parameter        : idComRxPdu_uo,pduInfoPtr_pcst,tpSduLength_uo,rxBufferSizePtr_puo(out)
 Return value     : BufReq_ReturnType
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
BufReq_ReturnType Com_StartOfReception(
                                PduIdType               idComRxPdu_uo,
                                const PduInfoType*      pduInfoPtr_pcst,
                                PduLengthType           tpSduLength_uo,
                                PduLengthType*          rxBufferSizePtr_puo
                                      )
{
    /* Start: Variable declarations */
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    BufReq_ReturnType           returnValue_en;
    /* End: Variable declarations */

    /* Initialize variable */
    /* Return with BUFREQ_E_NOT_OK if DET occurs or Ipdu group containing this IPDU is not started */
    returnValue_en = BUFREQ_E_NOT_OK;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_StartOfReception, COM_E_UNINIT);
    }
    else if (rxBufferSizePtr_puo == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_StartOfReception, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidRxIpduId(idComRxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_StartOfReception, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        *  to internal Id which is generated through configuration
        *  If PC variant is selected, then no mapping table will be used. */
        idComRxPdu_uo       = COM_GET_RX_IPDU_ID(idComRxPdu_uo);

        rxIpduRamPtr_pst    = &COM_GET_RXPDURAM_S(idComRxPdu_uo);

        /*Currently this parameter is not used*/
        (void)pduInfoPtr_pcst;

        /* Check if the Ipdu group containing this IPDU is started */
        if(Com_Prv_CheckRxIPduStatus(idComRxPdu_uo))
        {
            /* Check if the reception requests fits Ipdu size,
            * else return overflow as we cannot accomodate this */
            if(tpSduLength_uo <= COM_GET_RXIPDU_SIZE(idComRxPdu_uo))
            {
                /* As all the checks are passed, then we accept this reception and
                 * set flag which will be monitored in CopyRxData calls */
                Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,rxIpduRamPtr_pst->rxFlags_u8,COM_TRUE);
                /* Initialise the received length RAM variable to zero here,
                 * This length variable will be incremented in the Com_CopyRxData API */
                rxIpduRamPtr_pst->rxTPIPduLength_uo = COM_ZERO;

                /* Update the TP with Ipdu size for buffer size out parameter */
                *rxBufferSizePtr_puo = COM_GET_RXIPDU_SIZE(idComRxPdu_uo);

                /* Positive condition hence status as BUFREQ_OK */
                returnValue_en = BUFREQ_OK;
            }
            else
            {
                /* return with BUFREQ_E_OVFL */
                returnValue_en = BUFREQ_E_OVFL;
            }
        }
    }

    return returnValue_en;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

