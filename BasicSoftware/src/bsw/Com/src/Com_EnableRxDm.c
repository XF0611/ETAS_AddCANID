

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
#if (COM_ENABLE_JITTERFLITER == STD_ON)
# define COM_RXTIMER_MAX_DUALCYCLE              (COM_RXTIMER_MAX - 2u)
#else
# define COM_RXTIMER_MAX_DUALCYCLE              (COM_RXTIMER_MAX - 1u)
#endif

#if(((COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE) && (COM_NUM_TOTAL_IPDU_GRP % 8u != 0)) || \
     (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE))
# define COM_ENABLE_CLEAR_UNUSED_BITS
#endif


/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/
#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)

LOCAL_INLINE void Com_Prv_ProcessRxDMIPduGroupVector(const uint8 * ipduGroupVector_pcu8);

#endif

#ifdef COM_ENABLE_CLEAR_UNUSED_BITS
LOCAL_INLINE void Com_Prv_ClearUnusedBits(Com_IpduGroupVector ipduGroupVector_au8);
#endif

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
 Function name    : Com_ReceptionDMControl
 Description      : Service Enables deadline monitoring for the I-PDU groups within the given I-PDU group.
 Parameter        : ipduGroupVector_au8 -> contains the requested deadline monitoring state (disabled = 0/ enabled = 1)
                    for all I-PDU groups.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(Com_IpduGroupVector) in a function prototype should be declared as pointer
 to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
 below, Hence suppressed */
void Com_ReceptionDMControl(Com_IpduGroupVector ipduGroupVector_au8)
{

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceptionDMControl, COM_E_UNINIT);
    }
    else if (ipduGroupVector_au8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReceptionDMControl, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
        Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
        uint16_least                idxIdIpdu_qu16;
#endif

#ifdef COM_ENABLE_CLEAR_UNUSED_BITS
        Com_Prv_ClearUnusedBits((uint8 *) (ipduGroupVector_au8));
#endif

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)

        /* Extract ipduGroupVector and identify the IpduGroups with different states */
        Com_Prv_ProcessRxDMIPduGroupVector((const uint8 * )(ipduGroupVector_au8));

        rxIpduRamPtr_pst   = &COM_GET_RXPDURAM_S(0);

        for (idxIdIpdu_qu16 = COM_ZERO; idxIdIpdu_qu16 < COM_GET_NUM_RX_IPDU; idxIdIpdu_qu16++)
        {
            if (COM_GET_IPDUCOUNTER_DM(idxIdIpdu_qu16) > COM_ZERO)
            {
                /*If the state is changed from RESET to SET*/
                /*As the counters are already updated, no necessary actions are required the other way around */
                if ((Com_GetRamValue(RXIPDU,_DMSTATUS,rxIpduRamPtr_pst->rxFlags_u8)) == COM_STOP)
                {
                    if(Com_Prv_EnableRxDeadlineMonitoring((Com_IpduId_tuo)idxIdIpdu_qu16))
                    {
                        Com_SetRamValue(RXIPDU,_DMSTATUS,rxIpduRamPtr_pst->rxFlags_u8, COM_START);
                    }
                }
            }
            else
            {
                if (Com_GetRamValue(RXIPDU,_DMSTATUS,rxIpduRamPtr_pst->rxFlags_u8))
                {
                    Com_SetRamValue(RXIPDU,_DMSTATUS,rxIpduRamPtr_pst->rxFlags_u8, COM_STOP);
                }
            }
            rxIpduRamPtr_pst++;
        } /* End of Rx IPdu for loop */
#endif /* #if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout) */
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ClearUnusedBits
 Description      : Service to clear the unused bitfields in the group vector.
 Parameter        : ipduGroupVector_au8 - I-PDU group vector containing the latest Rx-DM activation state
                                      (stopped = 0/started = 1) for all I-PDU groups.
 Return value     : None
 **********************************************************************************************************************
*/
#ifdef COM_ENABLE_CLEAR_UNUSED_BITS
LOCAL_INLINE void Com_Prv_ClearUnusedBits(Com_IpduGroupVector ipduGroupVector_au8)
{

    /* START: Clear Unused BitFields
     * If the Group vector is filled with non-zero in unconfigured bitfields,
     * it might cause an exception in the software. */

    uint16  byteNo_u16;
    byteNo_u16 = ( uint16 )( COM_GET_NUM_TOTAL_IPDU_GRP / 8u );

#if(COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
    if ( byteNo_u16 < COM_NUM_ARR_IPDUGRPVECT )
#endif
    {
        ipduGroupVector_au8[ byteNo_u16 ] &= ( ( uint8 )( 0xFFu ) >> ( 8u - ( COM_GET_NUM_TOTAL_IPDU_GRP % 8u ) ) );

#if(COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)

        byteNo_u16++;
        while ( byteNo_u16 < COM_NUM_ARR_IPDUGRPVECT )
        {
            ipduGroupVector_au8[ byteNo_u16 ] = COM_ZERO;
            byteNo_u16++;
        }
#endif
    }
    /* END: Clear Unused BitFields */
}
#endif /* end of COM_ENABLE_CLEAR_UNUSED_BITS*/


#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxDMIPduGroupVector
 Description      : Service to process IPduGroupVector and to modify the local IPduGroups states as requested
                    by the application.
 Parameter        : ipduGroupVector_pcu8 - I-PDU group vector containing the latest Rx-DM activation state
                                      (stopped = 0/started = 1) for all I-PDU groups.
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_ProcessRxDMIPduGroupVector(const uint8 * ipduGroupVector_pcu8)
{
    /* Local pointer which holds the address of the array which stores the ipdu id */
    const Com_IpduId_tuo *      ipduRefPtr_pcuo;
    /* Local pointer to hold the address of Ipdu group structure                   */
    Com_IpduGrpCfg_tpcst        ipduGrpConstPtr_pcst;
    uint16_least                index_qu16;
    uint16_least                numOfRxIpdus_qu16;
    Com_IpduGroupIdType         idIpduGrp_u16; /* To store the Ipdu Group Id */
    uint8                       byteVal_u8;
    uint8                       bitOffset_u8;
    uint8                       pduCounterVal_u8;

    for (index_qu16 = COM_ZERO; index_qu16 < COM_NUM_ARR_IPDUGRPVECT; index_qu16++)
    {
        if (Com_IpduGrpVector_DM_au8[index_qu16] != ipduGroupVector_pcu8[index_qu16] )
        {
            /* Perform an XOR to find the Ipdu Groups which have changed the states */
            byteVal_u8 = (uint8)((Com_IpduGrpVector_DM_au8[index_qu16]) ^ (ipduGroupVector_pcu8[index_qu16]));
            bitOffset_u8 = COM_ZERO;

            /*If there are some Ipdu groups which have changed the status*/
            do
            {
                /*Check for the status of the changed Ipdu groups and perform actions accordingly*/
                /* Check if any of the IPdu Group is changed by performing right bit shift*/
                if (COM_RESET != (byteVal_u8 & COM_ONE))
                {
                    /*Get the Ipdu group Id if the status is changed*/
                    idIpduGrp_u16   = (Com_IpduGroupIdType)(8u * index_qu16)+ bitOffset_u8 ;

                    /* Get the latest status of the IPdu group and store the any of the below value into PduCounterVal
                     * 0x01u eq +1, to increment the IPDU_COUNTER
                     * 0xFFu eq -1, to decrement the IPDU_COUNTER */
                    pduCounterVal_u8   = ((((ipduGroupVector_pcu8[index_qu16] >> bitOffset_u8) & COM_ONE) != COM_ZERO) ?
                                                                                                   (COM_ONE) : (0xFFu));

                    ipduGrpConstPtr_pcst = COM_GET_IPDUGRP_CONSTDATA(idIpduGrp_u16);

                    ipduRefPtr_pcuo      = COM_GET_IPDUGRP_IPDUREF_CONSTDATA(ipduGrpConstPtr_pcst->idFirstIpdu_u16);

                    numOfRxIpdus_qu16 = ipduGrpConstPtr_pcst->numOfRxPdus_u16;

                    while (numOfRxIpdus_qu16 > COM_ZERO)
                    {
                        /*Check if the DM state of the Ipdu Group is changed and if it is an RX IPdu*/
                         COM_GET_IPDUCOUNTER_DM(*ipduRefPtr_pcuo) = (uint8)
                        (COM_GET_IPDUCOUNTER_DM(*ipduRefPtr_pcuo) + pduCounterVal_u8);

                        ipduRefPtr_pcuo++;
                        numOfRxIpdus_qu16--;
                    }/* while (NoofPdus_u16 > 0 ) */
                } /* if (COM_RESET != (byteVal_u8 & 1u)) */

                bitOffset_u8 ++;
                byteVal_u8 >>= COM_ONE;

            } while (byteVal_u8 > COM_ZERO);

            /* update the status of the IPdu group */
            Com_IpduGrpVector_DM_au8[index_qu16] = ipduGroupVector_pcu8[index_qu16] ;
        } /* if (Com_IpduGrpVector_DM_au8[index_qu16] != ipduGroupVector[index_qu16] ) */

    } /* End of For loop */
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_EnableRxDeadlineMonitoring
 Description      : Function to enable the Rx DeadlineMonitoring.
 Parameter        : idIpdu_uo -> Id of the Rx ipdu
 Return value     : returns true if Rx Deadline monitoring is to be enabled for the given Rx IPDU
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

boolean Com_Prv_EnableRxDeadlineMonitoring(Com_IpduId_tuo idIpdu_uo)
{
    Com_RxIpduCfg_tpcst                 rxIpduConstPtr_pcst;
#ifdef COM_RxIPduTimeout
    Com_RxIpduRam_tpst                  rxIpduRamPtr_pst;
#endif
#if (defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout))
    const Com_SignalTimeoutInfo_tst *    sigTimeOutConstPtr_pcst;
    uint16 *                             sigTimeOutRamPtr_pu16;
    uint16_least                         index_qu16;
    uint16_least                         numOfSigWithUpdateBit_qu16;
#endif
    boolean                              isRxIpduDmReq_b;

    isRxIpduDmReq_b     = COM_FALSE;

    rxIpduConstPtr_pcst = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);

#ifdef COM_RxIPduTimeout
    rxIpduRamPtr_pst    = &COM_GET_RXPDURAM_S(idIpdu_uo);

    /* Rx Deadline monitoring is required only if ComTimeout(non-zero) is configured, independent of FirstTimeout */
    if (rxIpduConstPtr_pcst->timeout_u16 != COM_ZERO)
    {
        isRxIpduDmReq_b = COM_TRUE;
    }

    /* If First timeout is not configured , load the timer with specific value ie COM_RXTIMER_MAX(0xFFFF).
     * If First timeout is configured then load the actual value
     */
    rxIpduRamPtr_pst->cntrRxTimeout_u16 = (rxIpduConstPtr_pcst->firstTimeout_u16 == COM_ZERO) ?
            COM_RXTIMER_MAX : (rxIpduConstPtr_pcst->firstTimeout_u16 + COM_ONE);

#endif /* #ifdef COM_RxIPduTimeout */

#ifdef COM_RxSigUpdateTimeout
    /* If this IPDU has signals with update-bits , initialise the pointers.
     * Separate deadline monitoring should be performed for signals with update-bit
     */
    if (rxIpduConstPtr_pcst->signalTimeoutRef_pcst != NULL_PTR)
    {
        isRxIpduDmReq_b = COM_TRUE;

        /* Get the number of signals with update-bit & configured timeout, within this IPDU */
        numOfSigWithUpdateBit_qu16   = rxIpduConstPtr_pcst->signalTimeoutRef_pcst->numWithUpdateBit_u8;
        /* Initialise pointer to Com_SignalTimeoutInfo */
        sigTimeOutConstPtr_pcst     = rxIpduConstPtr_pcst->signalTimeoutRef_pcst->timeoutInfoRef_pcst;
        /* Initialise pointer to signal RAM timer */
        sigTimeOutRamPtr_pu16       = rxIpduConstPtr_pcst->signalTimeoutRef_pcst->timeoutTicks_pu16;

        SchM_Enter_Com_RxSigToTicks();

        for (index_qu16 = numOfSigWithUpdateBit_qu16; index_qu16 != COM_ZERO; index_qu16--)
        {
            /* If FirstTimeOutFactor is not configured then load timer with COM_RXTIMER_MAX(0xFFFF)
             * If FirstTimeOutFactor is configured then load the ComFirstTimeout value
             */
            (*sigTimeOutRamPtr_pu16) = (sigTimeOutConstPtr_pcst->firstTimeout_u16 == COM_ZERO) ?
                    COM_RXTIMER_MAX : (sigTimeOutConstPtr_pcst->firstTimeout_u16 + COM_ONE);

            /* Increment pointers to refer next signal with update-bit(within this IPDU) */
            sigTimeOutConstPtr_pcst++;
            sigTimeOutRamPtr_pu16++;

        }/*End of for loop*/

        SchM_Exit_Com_RxSigToTicks();
    }
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
    /* If this IPDU has signal groups with update-bits , initialise the pointers.
     * Separate deadline monitoring should be performed for signal groups with update-bit
     */
    if (rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst != NULL_PTR)
    {
        isRxIpduDmReq_b = COM_TRUE;

        /* Get the number of signals with update-bit & configured timeout, within this IPDU */
        numOfSigWithUpdateBit_qu16   = rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->numWithUpdateBit_u8;
        /* Initialise pointer to Com_SignalGrpTimeoutInfo */
        sigTimeOutConstPtr_pcst     = rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->timeoutInfoRef_pcst;
        /* Initialise pointer to signalGroup timer */
        sigTimeOutRamPtr_pu16       = rxIpduConstPtr_pcst->signalGrpTimeoutRef_pcst->timeoutTicks_pu16;

        SchM_Enter_Com_RxSigGrpToTicks();

        for (index_qu16 = numOfSigWithUpdateBit_qu16; index_qu16 != COM_ZERO; index_qu16--)
        {
            /* If FirstTimeOutFactor is not configured then load timer with COM_RXTIMER_MAX(0xFFFF)
             * If FirstTimeOutFactor is configured then load the ComFirstTimeout value
             */
            (*sigTimeOutRamPtr_pu16) = (sigTimeOutConstPtr_pcst->firstTimeout_u16 == COM_ZERO) ?
                    COM_RXTIMER_MAX : (sigTimeOutConstPtr_pcst->firstTimeout_u16 + COM_ONE);

            /* Increment pointers to refer next signal group with update-bit(within this IPDU)
             * If no next signal group with update-bit is present in the IPDU, then the if() check fails and
             * the code inside if() block is not executed.
             */
            sigTimeOutConstPtr_pcst++;
            sigTimeOutRamPtr_pu16++;
        }/*End of for loop*/

        SchM_Exit_Com_RxSigGrpToTicks();
    }
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

    return isRxIpduDmReq_b;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout) */


#ifdef COM_RX_DUAL_CYCLE_SUPPPORT
/*
 **********************************************************************************************************************
 Function name    : Com_SetRxIPduTimeoutTicks
 Description      : To update new(dual-cycle-time) IPDU based reception deadline value
 Parameter        : idRxPdu_uo       - Id of the Rx IPDU
                  : timeoutTicks_u16 - Rx IPDU deadline value in 'ticks', i.e., multiple of referred MainFunctionRx
 Return value     : E_OK     - new deadline will be considered from next call to MainFunctionRx
                  : E_NOT_OK - new deadline value is not considered.
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
Std_ReturnType Com_SetRxIPduTimeoutTicks(PduIdType idRxPdu_uo, uint16 timeoutTicks_u16)
{

# ifdef COM_RxIPduTimeout

    Com_RxIpduCfg_tpcst         rxIpduConstPtr_pcst;
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    Std_ReturnType              retValue_u8;

    retValue_u8 = E_NOT_OK;

#  if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetRxIPduTimeoutTicks, COM_E_UNINIT);
    }
    else if (timeoutTicks_u16 >= COM_RXTIMER_MAX_DUALCYCLE)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetRxIPduTimeoutTicks, COM_E_INVALID_VALUE);
    }
    else if (!Com_Prv_IsValidRxIpduId(idRxPdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetRxIPduTimeoutTicks, COM_E_PARAM);
    }
    else
#  endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idRxPdu_uo              = COM_GET_RX_IPDU_ID(idRxPdu_uo);

        rxIpduConstPtr_pcst     = COM_GET_RX_IPDU_CONSTDATA(idRxPdu_uo);
        rxIpduRamPtr_pst        = &COM_GET_RXPDURAM_S(idRxPdu_uo);

        /* is IPDU ComTimeout configured */
        if (rxIpduConstPtr_pcst->timeout_u16 != COM_ZERO)
        {
            retValue_u8 = E_OK;

            /* does new tick-value differs from previously updated ticks */
            if (rxIpduRamPtr_pst->rxDualCycTimeout_u16 != timeoutTicks_u16)
            {
                rxIpduRamPtr_pst->rxDualCycTimeout_u16 = timeoutTicks_u16;

                /* locks are acquired to protect 'RxFlags' */
                SchM_Enter_Com_RxPduBuffer();

                Com_SetRamValue(RXIPDU,_RELOAD_RXTICKS, rxIpduRamPtr_pst->rxFlags_u8, COM_TRUE);

                SchM_Exit_Com_RxPduBuffer();
            }
        }
    }

    return retValue_u8;
# else

    (void)idRxPdu_uo;
    (void)timeoutTicks_u16;

    return E_NOT_OK;

# endif /* end of #ifdef COM_RxIPduTimeout */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* end of COM_RX_DUAL_CYCLE_SUPPPORT */


