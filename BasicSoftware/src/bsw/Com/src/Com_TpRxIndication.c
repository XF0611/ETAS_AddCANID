

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
 Function name    : Com_TpRxIndication
 Description      : By a call to Com_TpRxIndication the AUTOSAR COM module starts the processing
                    of the received large or dynamic length IPDU.
 Parameter        : idPdu_uo,result_u8
 Return value     : Void
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_TpRxIndication(PduIdType idPdu_uo, Std_ReturnType result_u8)
{

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_TpRxIndication, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidRxIpduId(idPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_TpRxIndication, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        Com_RxIpduCfg_tpcst     rxIpduConstPtr_pcst;
        Com_RxIpduRam_tpst      rxIpduRamPtr_pst;

        /* If PB variant is selected, then idPdu_uo which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idPdu_uo = COM_GET_RX_IPDU_ID(idPdu_uo);

        rxIpduConstPtr_pcst   = COM_GET_RX_IPDU_CONSTDATA(idPdu_uo);
        rxIpduRamPtr_pst      = &COM_GET_RXPDURAM_S(idPdu_uo);

        /* Check whether RxPdu flag is set, which will be done in Com_StartOfReception */
        if (Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,rxIpduRamPtr_pst->rxFlags_u8))
        {
            PduInfoType     rxPduInfo_st;
            boolean         isValidRxIpdu_b;

            rxPduInfo_st.SduDataPtr = rxIpduConstPtr_pcst->buffPtr_pau8;
            /* store received length through CopyRxData */
            rxPduInfo_st.SduLength  = rxIpduRamPtr_pst->rxTPIPduLength_uo;

            /*
             * TRACE[SWS_Com_00720] When a large I-PDU is received successfully via the TP interface,
             * the I-PDU callout shall take place within the invocation of Com_TpRxIndication.
             */
            /*
             * TRACE[SWS_Com_00838] In case Com_TpRxIndication is called with value E_NOT_OK
             * for the parameter result OR
             * TRACE[SWS_Com_00839] When the I-PDU callout in context of Com_TpRxIndication
             * returns false OR
             * [DesignDecision] When configured IPDU counter mismatch occurs, then
             * the I-PDU shall be considered as incorrectly received and therefore all
             * included signals shall be treated exactly like invalidated signals.
             * That is, the AUTOSAR COM module shall execute all configured ComDataInvalidActions
             * for all included signals and signal groups.
             */
            /* MR12 RULE 13.5 VIOLATION:  Not invoking Com_Prv_isValidRxIpdu_b function for unsuccessful confirmation
             * has no impact, hence warning is suppressed. */
            isValidRxIpdu_b =    (E_OK == result_u8)
#if defined(COM_RxIPduCallOuts) || defined(COM_RX_IPDUCOUNTER)
                            && (Com_Prv_IsValidRxIpdu(idPdu_uo, &rxPduInfo_st))
#endif
                            ;

#ifdef COM_RxIPduDeferredProcessing
            if (Com_GetValue(RXIPDU,_SIGPROC,rxIpduConstPtr_pcst->rxIPduFields_u8) == COM_IMMEDIATE)
#endif
            {
                if (isValidRxIpdu_b)
                {
                    /* store valid length */
                    rxIpduRamPtr_pst->rxIPduLength_uo = rxPduInfo_st.SduLength;

                    Com_Prv_ProcessRxIPdu(idPdu_uo,&rxPduInfo_st);
                }
#ifdef COM_RXIPDU_TP_INVALIDATION
                else
                {
                    /* Check if invalid-actions are configured for signal(group)s contained in TP-IPDU */
                    if (Com_GetValue(RXIPDU,_TP_INV_CFG,rxIpduConstPtr_pcst->rxIPduFields_u8))
                    {
                        Com_Prv_ExecuteRxInvalidActions(idPdu_uo);
                    }
                    else
                    {
                        /* invalid actions are not configured */
                    }
                }
#endif /* end of #ifdef COM_RXIPDU_TP_INVALIDATION */
            }
#ifdef COM_RxIPduDeferredProcessing
            else
            {
                if (isValidRxIpdu_b)
                {
                    /* store valid length, deferred IPDU shall be processed in MainFunctionRx */
                    rxIpduRamPtr_pst->rxIPduLength_uo = rxPduInfo_st.SduLength;
                }
                else
                {
                    Com_SetRamValue(RXIPDU,_IS_INVALID,rxIpduRamPtr_pst->rxFlags_u8,COM_TRUE);
                }
            }
#endif /* end of #ifdef COM_RxIPduDeferredProcessing */

            /*
             * TRACE[SWS_Com_00715] When a (large) I-PDU with a configured reception deadline
             * monitoring is received successfully, the AUTOSAR COM module shall reset the reception
             * deadline monitoring timer for this (large) I-PDU at invocation of the function
             * Com_RxIndication or Com_TpRxIndication respectively.
             */
            /*
             * [DesignDecision] IPDU timer is not reloaded here, instead _INDICATION flag will be used
             * to reload timer in MainFunctionRx
             */
#ifdef COM_ENABLE_MAINFUNCTION_RX
            Com_SetRamValue(RXIPDU,_INDICATION,rxIpduRamPtr_pst->rxFlags_u8,COM_TRUE);
#endif

            /* Reset The large pdu Rx flag, to accept new reception */
            Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,rxIpduRamPtr_pst->rxFlags_u8,COM_FALSE);
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

