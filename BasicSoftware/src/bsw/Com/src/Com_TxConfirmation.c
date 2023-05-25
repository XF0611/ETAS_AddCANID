

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"
#include "Com_Prv_Inl.h"


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
 Function name    : Com_TxConfirmation
 Description      : Service called by the lower layer after the PDU has been transmitted on the network.
 Parameter        : idTxPdu_uo -> Id of the Tx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
#ifdef COM_ECUC_AR_RELEASE_AR45
void Com_TxConfirmation(PduIdType idTxPdu_uo,Std_ReturnType result_u8)
#else
void Com_TxConfirmation(PduIdType idTxPdu_uo)
#endif
{
#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TxConfirmation, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idTxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TxConfirmation, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idTxPdu_uo = COM_GET_TX_IPDU_ID(idTxPdu_uo);

        /* Check if Ipdu Group is started / Stopped */
        if (Com_Prv_CheckTxIPduStatus(idTxPdu_uo))
        {
            Com_Prv_InternalTxConfirmation(idTxPdu_uo);
        }
    }
#ifdef COM_ECUC_AR_RELEASE_AR45
    (void)result_u8;
#endif
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_InternalTxConfirmation
 Description      : Common function to process the TxConfirmation for both normal & TP IPDU types
 Parameter        : idTxPdu_uo -> Id of the Tx Ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_Prv_InternalTxConfirmation(PduIdType idTxPdu_uo)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;

#if defined(COM_TxIPduTimeOut)  || defined(COM_TxIPduNotification) ||\
    defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)
    uint16 constByteValue_u16;
#endif

#ifdef COM_TxIPduNotification
    boolean     notifyFlag_b;

    notifyFlag_b = COM_FALSE;    /* Initializing to COM_FALSE */
#endif

    txIpduConstPtr_pcst      = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);
    txIpduRamPtr_pst        = &COM_GET_TXPDURAM_S(idTxPdu_uo);

#if defined(COM_TxIPduTimeOut)  || defined(COM_TxIPduNotification) ||\
    defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)
    constByteValue_u16  = txIpduConstPtr_pcst->txIPduFields_u16;
#endif

    SchM_Enter_Com_TxIpduProtArea(TXCONFIRMATION);

    /* TRACE[SWS_Com_00469] If ComMinimumDelayTime of an I-PDU is configured greater than 0, the AUTOSAR COM module
     * shall (re-)load an already running and not yet elapsed minimum delay time counter of that I-PDU with
     * ComMinimumDelayTime on a successful transmit confirmation. */
    if (txIpduRamPtr_pst->cntrMinDelayTime_u16 != COM_ZERO)
    {
#ifdef COM_TXPDU_DEFERREDPROCESSING
        /* If DEFERREDPROCESSING is enabled, transmissions are done always from MainFuncionTx.
         * Hence MDT offset is not required. */
        txIpduRamPtr_pst->cntrMinDelayTime_u16 = txIpduConstPtr_pcst->minDelayTime_u16;
#else
        /* If DEFERREDPROCESSING is disabled, add an offset of 1.
         * This is required because TxConfirmation can come immediately, which loads MDT timer.
         * Now this would be reduced by 1 immediately in next cyclic task, leading to early timeout of MDT timer. */
        txIpduRamPtr_pst->cntrMinDelayTime_u16 = txIpduConstPtr_pcst->minDelayTime_u16 +
                                   (Com_GetRamValue(TXIPDU,_EVENTTRIG,txIpduRamPtr_pst->txFlags_u16) ? COM_ONE : COM_ZERO);
#endif
    }

    /* TRACE[SWS_Com_00305] In case of sending an I-PDU with ComTxModeMode DIRECT or MIXED and
     * ComTxModeNumberOfRepetitions greater than 0, the AUTOSAR COM module shall call PduR_ComTransmit periodically,
     * with period ComTxModeRepetitionPeriod until ComTxModeNumberOfRepetitions plus one successful confirmations for
     * this send-request are received. */
    if (txIpduRamPtr_pst->cntrRepetitions_u8 != COM_ZERO)
    {
        --txIpduRamPtr_pst->cntrRepetitions_u8;
    }
    if (txIpduRamPtr_pst->cntrRepetitions_u8 == COM_ZERO)
    {
        Com_SetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16,COM_TRUE);  /*This is checked in Com_MainFunctionTx()*/

#ifdef COM_TxIPduTimeOut
        if (Com_GetValue(TXIPDU,_NONETOSTATUS,constByteValue_u16))
        {
            /* TRACE[SWS_Com_00697] In case transmission deadline monitoring is configured for an I-PDU with
             * transmission mode NONE only, the AUTOSAR COM module shall reset the transmission deadline monitoring
             * timer for this I-PDU when a successful transmit confirmation is received. */
            txIpduRamPtr_pst->cntrTxTimeout_u16 = txIpduConstPtr_pcst->timeout_u16;
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_START);
        }
        else
        {
            /* TRACE[SWS_Com_00880] When the AUTOSAR COM receives a transmit confirmation for an I-PDU, it shall cancel
             * all running transmission deadline monitoring timers for all contained signals and signal groups.
             * TRACE[SWS_Com_00308] For an I-PDU with ComTxModeMode DIRECT and ComTx-ModeNumberOfRepetitions > 0,
             * the AUTOSAR COM module shall cancel the transmission deadline monitoring timer after
             * ComTxModeNumberOfRepetitions plus one successfully received transmit confirmations. */
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_STOP);
        }
#endif /* #ifdef COM_TxIPduTimeOut */

        /* TRACE[SWS_Com_00577] If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to Confirmation,
         * the AUTOSAR COM module shall clear all update-bits of all contained signals and signal groups after this
         * I-PDU was sent out via PduR_ComTransmit, PduR_ComTransmit returned E_OK and
         * the I-PDU was successfully confirmed.*/
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if (Com_GetValue(TXIPDU,_CLRUPDBIT,constByteValue_u16) == (uint16)COM_CLRUPDATEBIT_CONFIRMATION)
        {
            Com_Prv_ClearUpdateBits(txIpduConstPtr_pcst);
        }
#endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */

#ifdef COM_TxIPduNotification
        /* If ComIPduSignalProcessing is IMMEDIATE
         * the signal indications / confirmations are performed in Com_TxConfirmation.
         * hence notifications are invoked here */
        notifyFlag_b = (Com_GetValue(TXIPDU,_SIGPROC,constByteValue_u16) == COM_IMMEDIATE);
#endif
    }
    SchM_Exit_Com_TxIpduProtArea(TXCONFIRMATION);

#ifdef COM_TxIPduNotification
    if ((notifyFlag_b) && (txIpduConstPtr_pcst->notification_pfct != NULL_PTR))
    {
        /* Notification callback function for IPDU.
         * This function is generated through code generator.
         * Individual signal and signal group notifications functions are called from within this function.
         * If IPDU has configured TX notification callback funcion, this function
         * is also called from the generated function
         */
        txIpduConstPtr_pcst->notification_pfct();
    }
#endif /* #ifdef COM_TxIPduNotification */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

