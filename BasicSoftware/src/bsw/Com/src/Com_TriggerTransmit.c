

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
 Function name    : Com_TriggerTransmit TRACE[SWS_Com_00001]
 Description      : Service called by the lower layer when an AUTOSAR COM I-PDU shall be transmitted.
                    Within this function, AUTOSAR COM shall copy the contents of its I-PDU transmit
                    buffer to the L-PDU buffer given by PduInfoPtr.
 Parameter        : idTxPdu_uo    -> id of the Tx ipdu
                  : pduInfoPtr_pst -> Pointer to the address where the COM module shall copy the ipdu data
                                  and length information.
 Return value     : E_OK/E_NOT_OK
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
Std_ReturnType Com_TriggerTransmit(PduIdType idTxPdu_uo, PduInfoType * pduInfoPtr_pst)
{

    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
#endif
    PduLengthType               size_uo;
#if ((defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) || \
      defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) || \
      defined(COM_TxIPduCallOuts))
    uint16                      constByteValue_u16;
#endif
    Std_ReturnType              status_u8;

    /* Return value with E_NOT_OK indicates, IPDU is not started or DET error has occured */
    status_u8 = E_NOT_OK;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerTransmit, COM_E_UNINIT);
    }
    else if (pduInfoPtr_pst == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerTransmit, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidTxIpduId(idTxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerTransmit, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed to internal Id
         * which is generated through configuration. If PC variant is selected, then no mapping table will be used.
         */
        idTxPdu_uo          = COM_GET_TX_IPDU_ID(idTxPdu_uo);

        txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idTxPdu_uo);

#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
        txIpduRamPtr_pst    = &COM_GET_TXPDURAM_S(idTxPdu_uo);
#endif

#if ((defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) || \
      defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) || \
      defined(COM_TxIPduCallOuts))
        constByteValue_u16 = txIpduConstPtr_pcst->txIPduFields_u16;
#endif

        /* update static PduLength */
        size_uo = COM_GET_TXIPDU_SIZE(idTxPdu_uo);

        /*Check if Ipdu Group is started */
        if (Com_Prv_CheckTxIPduStatus(idTxPdu_uo))
        {
            status_u8 = E_OK;
        }
        else
        {
            /* This part will be executed when IPDU Group is Stopped (old value will be returned)
             * and return E_NOT_OK */
        }

        /* TRACE[SWS_Com_00475] ComMinimumDelayTime: It is not possible to monitor the minimum delay time for I-PDUs
         * that are requested using the Com_TriggerTransmit API.
         * No MDT monitoring is required */

        SchM_Enter_Com_TxIpduProtArea(TRIGGERTRANSMIT);

#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
        /* The total Ipdu size will be sum of Ipdu static length and dynamic length for a dynamic IPdu.
         * The Dynamic length of the Ipdu is updated in the call to Com_SendDynSignal.*/
        /* Check if Dynamic Signal is supported */
        if (Com_GetValue(TXIPDU,_ISDYNIPDU,constByteValue_u16))
        {
            size_uo += (PduLengthType)(txIpduRamPtr_pst->dynLength_u16);
        }
#endif /* #if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT) */

        /* Dont check for Ipdu Group Stopped/Started i.e normal reaction on Com_TriggerTransmit( ) */
        Com_ByteCopy(pduInfoPtr_pst->SduDataPtr, txIpduConstPtr_pcst->buffPtr_pu8, size_uo);

        pduInfoPtr_pst->SduLength = size_uo;

        /* Trigger Transmit is processed even if the IPDU Group is STOPPED */
        /* If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to TriggerTransmit,
         * the AUTOSAR COM module shall clear all update-bits of all contained signals and signal groups
         * after the contents of this I-PDU was requested by a call to Com_TriggerTransmit.*/
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if (Com_GetValue(TXIPDU,_CLRUPDBIT,constByteValue_u16) == (uint16)COM_CLRUPDATEBIT_TRIGGERTRANSMIT)
        {
            Com_Prv_ClearUpdateBits(txIpduConstPtr_pcst);
        }
#endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */
        SchM_Exit_Com_TxIpduProtArea(TRIGGERTRANSMIT);

        /* The callout is processed over the data given to the IF buffer
         * Note: As the data pointer given to application is a Pointer to variable, There are chances that application
         * layer can update the data. Any data update from application which happens over this target buffer are passed
         * to the lower layer. How ever this updated data is not stored in the local buffers of Com.
         * Impact: Application cannot assume that the data updates done in one Com Callout function will be preserved
         * across different calls of Com callout. */
#ifdef COM_TxIPduCallOuts
        /* TRACE[SWS_Com_00766] In case a ComIPduTriggerTransmitCallout is configured for an I-PDU and
         * the I-PDU is started,the AUTOSAR COM module shall invoke this I-PDU callout
         * within every execution of Com_TriggerTransmit for this I-PDU. */
        if (status_u8 != E_NOT_OK)
        {
            if (Com_GetValue(TXIPDU,_ISCALLOUTFRMTRIGTRANS,constByteValue_u16))
            {
                /* TRACE[SWS_Com_00395] When Com_TriggerTransmit is called, the AUTOSAR COM module shall ignore the
                 * return value from the ComIPduTriggerTransmitCallout. */
                (void)txIpduConstPtr_pcst->callOut_pfct(COM_GET_TXIPDU_HANDLEID(idTxPdu_uo), pduInfoPtr_pst);
            }
        }
#endif
    }
    return(status_u8);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

