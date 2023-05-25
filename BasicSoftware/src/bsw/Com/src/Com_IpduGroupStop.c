

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"


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
 Function name    : Com_Prv_TxIPduStop
 Description      : Service to stop the ipdu
 Parameter        : idIpdu_uo - Id of the ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_TxIPduStop(Com_IpduId_tuo idIpdu_uo)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#ifdef COM_ERRORNOTIFICATION
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;

    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
#endif

    txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idIpdu_uo);

#ifdef COM_ERRORNOTIFICATION
    /* TRACE[SWS_Com_00479] If an I-PDU is stopped as result of a call to Com_IpduGroupControl, the AUTOSAR COM module
     * shall immediately invoke the configured ComErrorNotification, for outstanding not confirmed
     * transmitted signals/ signal groups of the stopped I-PDU.
     * Outstanding not confirmed I-PDU's can be one of the below possibilities:
     * => Tx-Timeout timer is started, i.e., already transmitted to the lower layer, waiting for the confirmation.
     * => N-times repetitions is not completed yet, its confirmation is pending
     * => Large data transmission request is initiated */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if (   (Com_GetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16) == COM_START)
        || (txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO)
#ifdef COM_TP_IPDUTYPE
        || (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16))
#endif
       )

    {

        if ((Com_GetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16) == COM_FALSE) &&
            (txIpduConstPtr_pcst->errorNotification_pfct != NULL_PTR))
        {
            txIpduConstPtr_pcst->errorNotification_pfct();
        }
    }
#endif /* #ifdef COM_ERRORNOTIFICATION */

#ifdef COM_TxIPduTimeOut
    /* TRACE[SWS_Com_00115] If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * the AUTOSAR COM module shall cancel the deadline monitoring for all pending confirmations. */
    Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_STOP);
#endif

#if (COM_RETRY_FAILED_TX_REQUESTS == STD_ON)
    /* TRACE[SWS_Com_00777] - If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * cancel any potential retries with respect to ComRetryFailedTransmitRequests. */
    Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif

    /* TRACE[SWS_Com_00800] The AUTOSAR COM module shall ignore any transmit confirmations for a stopped I-PDU. */
    /* Reset all other Tx Global variables including Min Delays */
    Com_SetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

#ifdef COM_TxIPduTimeOut
    txIpduRamPtr_pst->cntrTxTimeout_u16 = COM_ZERO;
#endif

    /* TRACE[SWS_Com_00777] - If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * cancel any outstanding transmission requests for this I-PDU. */
    txIpduRamPtr_pst->cntrRepetitions_u8 = COM_ZERO;
    Com_SetRamValue(TXIPDU,_MDT,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
    txIpduRamPtr_pst->cntrMinDelayTime_u16 = COM_ZERO;

#ifdef COM_TP_IPDUTYPE
    /* TRACE[SWS_Com_00714] If a large I-PDU is stopped while its transmission is already in progress,
     * the AUTOSAR COM module shall stop the transmission process immediately.
     * Reset the large data Tx Pdu flag, if the Ipdu group is stopped. */
    Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

