

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
 Function name    : Com_TriggerIPDUSend
 Description      : Service used within the Ipdu-Callout.
 The I-PDU with the given ID is triggered for transmission
 Parameter        : idPdu_uo-> Id of the Tx ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_TriggerIPDUSend(PduIdType idPdu_uo)
{
#ifdef COM_TxIPduTimeOut
    Com_TxIpduCfg_tpcst      txIpduConstPtr_pcst;
#endif

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerIpduSend, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TriggerIpduSend, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        idPdu_uo = COM_GET_TX_IPDU_ID(idPdu_uo);

#ifdef COM_TxIPduTimeOut

        txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idPdu_uo);
#endif

        if (Com_Prv_CheckTxIPduStatus(idPdu_uo))
        {
#ifdef COM_TP_IPDUTYPE
            /* Check whether the large data pdu flag has been reset */
            if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,COM_GET_TXPDURAM_S(idPdu_uo).txFlags_u16) == COM_FALSE)
#endif
            {
                Com_SendIpduInfo_tst sendIpduFlag_st  = COM_CLEAR_SENDIPDU_FLAGS;

                /* As signal transfer property information is not known. It is assumed that, for transmission
                 * from this API, the signal transfer property is PENDING
                 * i.e, sendIpduFlag_st.isSigTriggered_u1 is COM_RESET */
                /* indicate no mode change */
                /* sendIpduFlag_st.isModeChangd_u1 = COM_RESET; */

                sendIpduFlag_st.isEventTrig_u1       = COM_SET;

                /* TRACE[SWS_Com_00388] Ignore repetitions */
                sendIpduFlag_st.ignoreRepetitions_u1 = COM_SET;

# ifdef COM_TxIPduTimeOut
                /* The timeout handling should happen as usual */
                sendIpduFlag_st.isTimeoutReq_u1 = COM_BOOL_TO_UNSIGNED(txIpduConstPtr_pcst->timeout_u16 != COM_ZERO);
# endif

                Com_Prv_SendIpdu((PduIdType) idPdu_uo, sendIpduFlag_st);
            }
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

