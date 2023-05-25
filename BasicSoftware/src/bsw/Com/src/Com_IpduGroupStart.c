

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
LOCAL_INLINE void Com_Prv_RxIPduStart(Com_IpduId_tuo idIpdu_uo, boolean initialize_b);

LOCAL_INLINE void Com_Prv_TxIPduStart(Com_IpduId_tuo idIpdu_uo, boolean initialize_b);

LOCAL_INLINE void Com_Prv_ProcessIPduGroupVector(const uint8 * ipduGroupVector_pcu8);

#if defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))
LOCAL_INLINE void Com_Prv_UpdateBitAndFilter(Com_IpduId_tuo ipduId_uo);
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
 Function name    : Com_IpduGroupControl
 Description      : Service for controlling the Ipdu groups
 Parameter        : ipduGroupVector_au8 - I-PDU group vector containing the activation state (stopped = 0/started = 1)
                                          for all I-PDU groups.
                  : initialize_b        - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_IpduGroupControl(Com_IpduGroupVector ipduGroupVector_au8, boolean initialize_b)
{
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    uint16_least                idxIdIpdu_qu16;

#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_IpduGroupControl, COM_E_UNINIT);
    }
    else if (ipduGroupVector_au8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_IpduGroupControl, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* A call to the API Com_ReceptionDMControl is made to set the DM vector with the same value
         * as that of the ipduGroupVector_au8 */
        Com_ReceptionDMControl(ipduGroupVector_au8);

        /* Extract ipduGroupVector_au8 and flag changed IpduGroups */
        Com_Prv_ProcessIPduGroupVector((const uint8 * )ipduGroupVector_au8);

        rxIpduRamPtr_pst     = &COM_GET_RXPDURAM_S(0);

        for (idxIdIpdu_qu16 = COM_ZERO; idxIdIpdu_qu16 < COM_GET_NUM_RX_IPDU; idxIdIpdu_qu16++)
        {
            /* If any of the Ipdu Group containing the IPdu is active,
             * i.e., counter will have non-zero value */
            if (COM_GET_IPDUCOUNTER_S(idxIdIpdu_qu16) > COM_ZERO)
            {
                /* If the PDU state is changed from STOP to START */
                if (Com_GetRamValue(RXIPDU,_PDUSTATUS,rxIpduRamPtr_pst->rxFlags_u8) == COM_STOP)
                {
                    Com_Prv_RxIPduStart((Com_IpduId_tuo)idxIdIpdu_qu16,initialize_b);

                    Com_SetRamValue(RXIPDU,_PDUSTATUS,rxIpduRamPtr_pst->rxFlags_u8,COM_START);
                }
            }
            else
            {
                /* If the PDU state is changed from START to STOP */
                if (Com_GetRamValue(RXIPDU,_PDUSTATUS,rxIpduRamPtr_pst->rxFlags_u8))
                {
                    Com_SetRamValue(RXIPDU,_PDUSTATUS,rxIpduRamPtr_pst->rxFlags_u8,COM_STOP);

#ifdef COM_ENABLE_MAINFUNCTION_RX
                    Com_SetRamValue(RXIPDU,_INDICATION,rxIpduRamPtr_pst->rxFlags_u8,COM_FALSE);
#endif

                    /* Large Pdu Rx status is reset, No further calls for this reception are processed */
#ifdef COM_TP_IPDUTYPE
                    Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,rxIpduRamPtr_pst->rxFlags_u8,COM_FALSE);
#endif
                }
            }

            rxIpduRamPtr_pst++;
        } /*End of RxIpdu for loop */

        txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(0);

        for (idxIdIpdu_qu16 = COM_ZERO; idxIdIpdu_qu16 < COM_GET_NUM_TX_IPDU; idxIdIpdu_qu16++)
        {
            /* If any of the Ipdu Group containing the IPdu is active,
             * i.e., counter will have non-zero value */
            if (COM_GET_IPDUCOUNTER_S(idxIdIpdu_qu16 + COM_GET_NUM_RX_IPDU) > COM_ZERO)
            {
                /* If the PDU state is changed from STOP to START */
                if (Com_GetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16) == COM_STOP)
                {
                    /* NOTE: This below order is to be maintained, to avoid any interrupt related race conditions.
                     * REASON: If the below function call is interrupted, by any other API,
                     * as the _PDUSTATUS is set after the function returns,
                     * the interrupting API returns without any effect */
                    Com_Prv_TxIPduStart((Com_IpduId_tuo)idxIdIpdu_qu16,initialize_b);

                    Com_SetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16,COM_START);
                }
            }
            else
            {
                /* If the PDU state is changed from START to STOP */
                if (Com_GetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16))
                {
                    /* NOTE: This below order is to be maintained, to avoid any interrupt related race conditions.
                     * REASON: If the below function call is interrupted, by any other API,
                     * as the _PDUSTATUS is set before the function call,
                     * the interrupting API returns without any effect */
                    Com_SetRamValue(TXIPDU,_PDUSTATUS,txIpduRamPtr_pst->txFlags_u16,COM_STOP);

                    Com_Prv_TxIPduStop((Com_IpduId_tuo)idxIdIpdu_qu16);
                }
            }

            txIpduRamPtr_pst++;
        } /*  End of TxIpdu for loop  */
    }/* End of DET Check */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_ProcessIPduGroupVector
 Description      : Service to process IPduGroupVector and to modify the local IPduGroups states as requested
                    by the application.
 Parameter        : ipduGroupVector_pcu8 - I-PDU group vector containing the latest activation state
                                      (stopped = 0/started = 1) for all I-PDU groups.
                  : Initialize      - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_ProcessIPduGroupVector(const uint8 * ipduGroupVector_pcu8)
{
    const Com_IpduId_tuo *      ipduRefPtr_pcuo;
    Com_IpduGrpCfg_tpcst        ipduGrpConstPtr_pcst;
    uint16_least                index_qu16;
    uint16_least                numOfPdus_qu16;
    Com_IpduGroupIdType         idIpduGrp_u16;
    uint8                       byteVal_u8;
    uint8                       bitOffset_u8;
    uint8                       pduCounterVal_u8;

    for (index_qu16 = COM_ZERO; index_qu16 < COM_NUM_ARR_IPDUGRPVECT; index_qu16++)
    {
        if (Com_IpduGrpVector_au8[index_qu16] != ipduGroupVector_pcu8[index_qu16])
        {
            /* Perform an XOR to find the Ipdu Groups which have changed the states */
            byteVal_u8   = (uint8)(Com_IpduGrpVector_au8[index_qu16] ^ ipduGroupVector_pcu8[index_qu16]);
            bitOffset_u8 = COM_ZERO;

            /* If there are some Ipdu groups which have changed the status */
            do
            {
                /* Check for the status of the changed Ipdu groups and perform actions accordingly.
                 * Check if any of the IPdu Group is changed by performing right bit shift */
                if (COM_RESET != (byteVal_u8 & COM_ONE))
                {
                    /* Get the Ipdu group Id if the status is changed */
                    idIpduGrp_u16   = (8u * (uint16)index_qu16) + bitOffset_u8;

                    /* Get the latest status of the IPdu group and store the any of the below value
                     * 0x01u eq +1, to increment the IPDU_COUNTER
                     * 0xFFu eq -1, to decrement the IPDU_COUNTER */
                    pduCounterVal_u8   = ((((ipduGroupVector_pcu8[index_qu16] >> bitOffset_u8) & COM_ONE) != COM_ZERO) ?
                                                                                            (0x01u) : (0xFFu));

                    ipduGrpConstPtr_pcst = COM_GET_IPDUGRP_CONSTDATA(idIpduGrp_u16);
                    ipduRefPtr_pcuo      = COM_GET_IPDUGRP_IPDUREF_CONSTDATA(ipduGrpConstPtr_pcst->idFirstIpdu_u16);

                    /* Is current IpduGroup is last member generated in the structure COM_GET_IPDUGRP_CONSTDATA */
                    if (idIpduGrp_u16 != (COM_GET_NUM_TOTAL_IPDU_GRP - COM_ONE))
                    {
                        /* Difference between the current IPduGroup Index to the next Index provides
                         * the total number of Pdus referred to the IPduGroup.*/
                        numOfPdus_qu16 = (ipduGrpConstPtr_pcst + COM_ONE)->idFirstIpdu_u16 - ipduGrpConstPtr_pcst->idFirstIpdu_u16;
                    }
                    else
                    {
                        /* In case,current IPduGroup is the last member in the generated table, then the total number
                         * of Pdus are stored in the separate pre-processor directive */
                        numOfPdus_qu16 = COM_GET_NUM_IPDUS_IN_LAST_IPDUGRP;
                    }

                    while (numOfPdus_qu16 > COM_ZERO)
                    {
                        /* Below counter shall increment if the latest state is started otherwise decrement it. */
                         COM_GET_IPDUCOUNTER_S(*ipduRefPtr_pcuo) = (uint8)
                        (COM_GET_IPDUCOUNTER_S(*ipduRefPtr_pcuo) + pduCounterVal_u8);

                        ipduRefPtr_pcuo++;
                        numOfPdus_qu16--;

                    } /* while (numOfPdus_u16 > 0u) */

                } /* if (COM_RESET != (byteVal_u8 & 1u)) */

                bitOffset_u8++;
                byteVal_u8 >>= COM_ONE;

            } while (byteVal_u8 > COM_ZERO);

            /* update the status of the IPdu group */
            Com_IpduGrpVector_au8[index_qu16] = ipduGroupVector_pcu8[index_qu16];

        } /* if (Com_IpduGrpVector_au8[index_qu16] != ipduGroupVector[index_u16] ) */

    } /* End of For loop */
}


/*
 **********************************************************************************************************************
 Function name    : Com_ClearIpduGroupVector
 Description      : Service to clear Ipdu group vector
 Parameter        : ipduGroupVector_au8 - I-PDU group vector containing
                    the activation state (stopped = 0/ started = 1) for all I-PDU groups.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_ClearIpduGroupVector(Com_IpduGroupVector ipduGroupVector_au8)
{
#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ClearIpduGroupVector, COM_E_UNINIT);
    }
    else if (ipduGroupVector_au8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ClearIpduGroupVector, COM_E_PARAM_POINTER);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        uint16_least   index_qu16;

        for (index_qu16 = COM_ZERO; index_qu16 < COM_NUM_ARR_IPDUGRPVECT; index_qu16++)
        {
            ipduGroupVector_au8[index_qu16] = COM_ZERO;
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_SetIpduGroup
 Description      : Service to set bit of the given Ipdu group vector that corresponds to the given I-PDU group.
 Parameter        : ipduGroupVector_au8 - I-PDU group vector to be modified
                  : idIpduGroup_u16     - ipduGroup used to identify the corresponding bit in the I-PDU group vector
                  : bitval_b            - New value of the corresponding bit
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_SetIpduGroup(Com_IpduGroupVector ipduGroupVector_au8, Com_IpduGroupIdType idIpduGroup_u16, boolean bitval_b)
{
#if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetIpduGroup, COM_E_UNINIT);
    }
    else if (ipduGroupVector_au8 == NULL_PTR)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetIpduGroup, COM_E_PARAM_POINTER);
    }
    else if (!Com_Prv_IsValidIpduGroupId(idIpduGroup_u16))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetIpduGroup, COM_E_PARAM);
    }
    else
#endif /* end of COM_PRV_ERROR_HANDLING */
    {
        uint16      index_u16;
        uint8       bitOffset_u8;

        idIpduGroup_u16 = COM_GET_IPDUGRP_ID(idIpduGroup_u16);

        index_u16     = (uint16)(idIpduGroup_u16 >> 3u);
        bitOffset_u8  = (uint8) (idIpduGroup_u16 % 8u);

        if (bitval_b)
        {
            ipduGroupVector_au8[index_u16] |= ((uint8)(COM_ONE << bitOffset_u8));
        }
        else
        {
            ipduGroupVector_au8[index_u16] &= ((uint8)(~(COM_ONE << bitOffset_u8)));
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_TxIPduStart
 Description      : Service to start the Tx ipdu
 Parameter        : idIpdu_uo     -> Id of the ipdu
                  : initialize_b  -> flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 **********************************************************************************************************************
*/

LOCAL_INLINE void Com_Prv_TxIPduStart(Com_IpduId_tuo idIpdu_uo, boolean initialize_b)
{
#if defined(COM_TxIPduTimeOut) || defined(COM_TX_IPDUCOUNTER)
    Com_TxIpduCfg_tpcst         txIpduConstPtr_pcst;
#endif
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    uint8                       latestTransMode_u8;

#if defined(COM_TxIPduTimeOut) || defined(COM_TX_IPDUCOUNTER)
    txIpduConstPtr_pcst = COM_GET_TX_IPDU_CONSTDATA(idIpdu_uo);
#endif
    txIpduRamPtr_pst   = &COM_GET_TXPDURAM_S(idIpdu_uo);

    /* The below Initialize check should be only after Com_LatestTransMode is calculated */
    if(initialize_b)
    {
#if defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))
        /* Call function to update bit   */
        Com_Prv_UpdateBitAndFilter(idIpdu_uo);
#endif

        /* Call Com_Prv_TxChangeMode( ) to calculate the Com_LatestTransMode[i] */
        Com_Prv_TxChangeMode(idIpdu_uo);
        /* Note: Even though if the Com_Tick_Tx is updated in Com_Prv_TxChangeMode(),
                it is once again updated in Ipdu Group Start i.e below */
        latestTransMode_u8 = Com_GetRamValue(TXIPDU,_LATESTMODE,txIpduRamPtr_pst->transMode_u8);

        /* Reset the global TMS status flag on the start of IPduGroup */
        Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);

        /* time period and offset attributes of IPDUs in Periodic or Mixed Transmission Mode */
        /* The CurrentTxModePtr points to the structure information of the current TxModeState */
        if( COM_TX_MODE_IS_CYCLIC(latestTransMode_u8) )
        {
            /* I.e Load the TxTimeOffset for cyclic type Pdu's(PERIODIC or MIXED) */
            txIpduRamPtr_pst->cntrTimePeriod_u16 = COM_GET_TXIPDU_TIMEOFFSET(idIpdu_uo, txIpduRamPtr_pst->currentTxModePtr_pcst);
        }

        /* the minimum delay time attribute of IPDUs in Direct/N-Times or Mixed Transmission Mode */
        /*Reset it to Zero i.e No Min delay check as soon as IpduGroup is started */
        txIpduRamPtr_pst->cntrMinDelayTime_u16 = COM_ZERO;

#ifdef COM_TxIPduTimeOut
        if(Com_GetValue(TXIPDU,_NONETOSTATUS,txIpduConstPtr_pcst->txIPduFields_u16))
        {
            /* TRACE[SWS_Com_00696] In case transmission deadline monitoring is configured for an I-PDU with
             * transmission mode NONE only, the AUTOSAR COM module shall start the transmission deadline monitoring
             * timer for this I-PDU upon the start of the I-PDU group to which the I-PDU belongs to. */
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_START);
            txIpduRamPtr_pst->cntrTxTimeout_u16 = txIpduConstPtr_pcst->timeout_u16;
        }
        else
#endif/* #ifdef COM_TxIPduTimeOut */
        {
#ifdef COM_TxIPduTimeOut

            /* Reset it to Stop.
               Note: This Flag has to be set to START only in SendIpdu() i,e just before Sending the IPDU */
            /* TRACE[SWS_Com_00835] In case transmission deadline monitoring is configured for an I-PDU with
             * transmission mode NONE and another transmission mode, the transmission deadline monitoring
             * shall be disabled whenever the transmission mode NONE is active */
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_STOP);

            txIpduRamPtr_pst->cntrTxTimeout_u16 = COM_ZERO; /* Reset the timer to zero */
#endif/* #ifdef COM_TxIPduTimeOut */
            Com_SetRamValue(TXIPDU,_CONFIR,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
        }
    } /* End of Initialize */
    else
    {
        /* Check if IPDU group is TX group
         * Cyclic IPDU's will be sent out cyclically after the call of this API
         */

        /* Call Com_Prv_TxChangeMode() to calculate the Com_LatestTransMode[i] */
        Com_Prv_TxChangeMode((Com_IpduId_tuo)idIpdu_uo);
        /* Note: Eventhough if the Com_Tick_Tx is updated in Com_Prv_TxChangeMode(),
                it is once again updated in Ipdu Group Start i.e below */

        /* The AUTOSAR COM module shall start the transmission deadline monitoring timer
         * for this I-PDU upon the start of the I-PDU group to which the I-PDU belongs to*/
#ifdef COM_TxIPduTimeOut
        if(Com_GetValue(TXIPDU,_NONETOSTATUS,txIpduConstPtr_pcst->txIPduFields_u16))
        {
            Com_SetRamValue(TXIPDU,_TICKTXTO,txIpduRamPtr_pst->txFlags_u16,COM_START);
            txIpduRamPtr_pst->cntrTxTimeout_u16 = txIpduConstPtr_pcst->timeout_u16;
        }
#endif
    }

#ifdef COM_TX_IPDUCOUNTER
    /* Check if IPduCounter is configured */
    if (txIpduConstPtr_pcst->idxTxIpduCntr_uo != COM_TXIPDU_CNTR_INV_IDX)
    {
        /* Locks are required here, to protect Tx Ipdu buffer due to the signal updates as per below requirement.
         * TRACE[SWS_Com_00334] By a call to the functions: Com_SendSignal, Com_SendSignalGroup, or
         * Com_InvalidateSignal, the AUTOSAR COM module shall update the values of its internal buffers even
         * for stopped I-PDUs. */
        SchM_Enter_Com_TxIpduProtArea(IPDUGROUPSTART);

        /* TRACE[SWS_Com_00687] The AUTOSAR COM module shall set the I-PDU counter to 0, after the I-PDU was
         * initialized by Com_Init or reinitialized by Com_IpduGroupStart. */
        Com_Prv_TxIpduInitializeCounter( (PduIdType)idIpdu_uo );

        SchM_Exit_Com_TxIpduProtArea(IPDUGROUPSTART);
    }
    else
    {
        /* do nothing: counter is not configured */
    }
#endif /* #ifdef COM_TX_IPDUCOUNTER */

#ifdef COM_TP_IPDUTYPE
    /* By default release the large Ipdu buffer lock, which is acquired in Com_Prv_SendIpdu() */
    Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,txIpduRamPtr_pst->txFlags_u16,COM_FALSE);
#endif
}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_RxIPduStart
 Description      : Service to start the Rx-IPDU
 Parameter        : idIpdu_uo       - Id of the Rx-IPDU
                  : initialize_b    - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 **********************************************************************************************************************
*/
LOCAL_INLINE void Com_Prv_RxIPduStart(Com_IpduId_tuo idIpdu_uo, boolean initialize_b)
{
#if (defined(COM_RxFilters) && defined(COM_F_ONEEVERYN))
    Com_RxIpduCfg_tpcst             rxIpduConstPtr_pcst;
    Com_RxSigCfg_tpcst              rxSigConstPtr_pcst;
    uint16_least                    index_qu16;  /* Index for both Rx-SignalGroup and Rx-Signal */
# ifdef COM_RX_SIGNALGROUP
    Com_RxSigGrpCfg_tpcst           rxSigGrpConstPtr_pcst;
    Com_RxGrpSigCfg_tpcst           rxGrpSigConstPtr_pcst;
    uint16_least                    idxGrpSig_qu16;   /* Index for Rx-GroupSignal */
# endif

    rxIpduConstPtr_pcst   = COM_GET_RX_IPDU_CONSTDATA(idIpdu_uo);

    /* Rx-SignalGroup */
# ifdef COM_RX_SIGNALGROUP
    rxSigGrpConstPtr_pcst = COM_GET_RXSIGGRP_CONSTDATA(rxIpduConstPtr_pcst->idFirstRxSigGrp_uo);

    for (index_qu16 = rxIpduConstPtr_pcst->numOfSigGrp_u16; index_qu16 != COM_ZERO; index_qu16--)
    {
        if (Com_GetValue(RXSIGGRP,_FILTEREVALREQ,rxSigGrpConstPtr_pcst->rxSignalGrpFields_u8))
        {
            rxGrpSigConstPtr_pcst = COM_GET_RXGRPSIG_CONSTDATA(rxSigGrpConstPtr_pcst->idFirstGrpSig_uo);

            for (idxGrpSig_qu16 = rxSigGrpConstPtr_pcst->numOfGrpSig_uo; idxGrpSig_qu16 != COM_ZERO; idxGrpSig_qu16--)
            {
                if (rxGrpSigConstPtr_pcst->filterAlgo_u8 == (uint8)COM_ONE_EVERY_N)
                {
                    /* TRACE[SWS_Com_00787] reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N */
                    COM_GET_ONEEVERYN(rxGrpSigConstPtr_pcst->idxFilter_uo).occurrence_u8 = COM_ZERO;
                }
                rxGrpSigConstPtr_pcst++;
            }
        }
        rxSigGrpConstPtr_pcst++;
    }
# endif /* #ifdef COM_RX_SIGNALGROUP */

    /* Rx-Signal */
    rxSigConstPtr_pcst  = COM_GET_RXSIG_CONSTDATA(rxIpduConstPtr_pcst->idRxSig_uo);

    for (index_qu16 = rxIpduConstPtr_pcst->numOfSig_u16; index_qu16 != COM_ZERO; index_qu16--)
    {
        if (Com_GetValue(RXSIG,_FILTRALG,rxSigConstPtr_pcst->rxSignalFields_u8) == (uint8)COM_ONE_EVERY_N)
        {
            /* TRACE[SWS_Com_00787] reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N */
            COM_GET_ONEEVERYN(rxSigConstPtr_pcst->idxFilter_uo).occurrence_u8 = COM_ZERO;
        }
        rxSigConstPtr_pcst++;
    }

#else
    /*  Misra warning removal for unused parameter */
    (void)idIpdu_uo;
#endif/* #if (defined(COM_RxFilters) && defined(COM_F_ONEEVERYN)) */

    (void)initialize_b; /* This variable is retained for future use */

#ifdef COM_RX_IPDUCOUNTER
    /* Set the flag until first successful reception of an Rx IPDU with counter */
    Com_SetRamValue(RXIPDU,_IS_FIRST_RECEPTION,COM_GET_RXPDURAM_S(idIpdu_uo).rxFlags_u8,COM_TRUE);
#endif

}


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_UpdateBitAndFilter
 Description      : Service to clear update bit if configured for all the configured signal and signalgroup
 Parameter        : ipduId_uo      -> Id of the ipdu
 Return value     : None
 **********************************************************************************************************************
*/
#if defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))

LOCAL_INLINE void Com_Prv_UpdateBitAndFilter(Com_IpduId_tuo ipduId_uo)
{

    Com_TxIpduCfg_tpcst             txIpduConstPtr_pcst;
#if defined(COM_TxSigUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))
    Com_TxSigCfg_tpcst              txSigConstPtr_pcst;
#endif

#if defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined(COM_F_ONEEVERYN)
    Com_TxGrpSigCfg_tpcst           txGrpSigConstPtr_pcst;
#endif

#if defined(COM_TxSigGrpUpdateBit) || \
    (defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))
    Com_TxSigGrpCfg_tpcst           txSigGrpConstPtr_pcst;
    uint16_least                    idxSigGrp_qu16;
#endif

    uint16_least                    idxSig_qu16;

#if defined (COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)
    PduLengthType                   byteNo_uo;
#endif

    txIpduConstPtr_pcst   = COM_GET_TX_IPDU_CONSTDATA(ipduId_uo);

#if defined(COM_TxSigUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))

    txSigConstPtr_pcst    = COM_GET_TXSIG_CONSTDATA(txIpduConstPtr_pcst->idTxSig_uo);

    for(idxSig_qu16 = txIpduConstPtr_pcst->numOfSig_u16; idxSig_qu16 > COM_ZERO; idxSig_qu16--)
    {

# ifdef COM_TxSigUpdateBit

        /* Get ClearBit and Update Bit */
        if (Com_GetValue(GEN,_UPDBITCONF,txSigConstPtr_pcst->general_u8))
        {
            /* Find the Byte No, where the Update bit lies */
            byteNo_uo = (PduLengthType)((txSigConstPtr_pcst->updateBitPos_uo) >> 3u);
            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst,txSigConstPtr_pcst->updateBitPos_uo,byteNo_uo)
        }
# endif /* # ifdef COM_TxSigUpdateBit */

# if defined(COM_TxFilters) && defined(COM_F_ONEEVERYN)

        /* OCCURRENCE shall be set to zero by Com_IpduGroupStart*/
        if(Com_GetValue(TXSIG,_FILTRALG,txSigConstPtr_pcst->txSignalFields_u16) == (uint16)COM_ONE_EVERY_N)
        {
            COM_GET_ONEEVERYN(txSigConstPtr_pcst->idxFilter_uo).occurrence_u8 = COM_ZERO;
        }
# endif /* # if defined(COM_TxFilters) && defined(COM_F_ONEEVERYN) */

        txSigConstPtr_pcst++;
    }

#endif /* #if defined(COM_TxSigUpdateBit) || (defined(COM_TxFilters) && defined(COM_F_ONEEVERYN)) */


#if defined(COM_TxSigGrpUpdateBit) || \
    (defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))

    txSigGrpConstPtr_pcst   = COM_GET_TXSIGGRP_CONSTDATA(txIpduConstPtr_pcst->idFirstTxSigGrp_uo);

    for(idxSigGrp_qu16 = txIpduConstPtr_pcst->numOfSigGroups_u16; idxSigGrp_qu16 > COM_ZERO; idxSigGrp_qu16--)
    {

# if (defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined(COM_F_ONEEVERYN))

        txGrpSigConstPtr_pcst = COM_GET_TXGRPSIG_CONSTDATA(txSigGrpConstPtr_pcst->idFirstGrpSig_uo);

        for (idxSig_qu16 = txSigGrpConstPtr_pcst->numOfGrpSig_uo; idxSig_qu16 > COM_ZERO; idxSig_qu16--)
        {
            /* OCCURRENCE shall be set to zero by Com_IpduGroupStart*/
            if(txGrpSigConstPtr_pcst->filterAlgo_u8 == (uint8)COM_ONE_EVERY_N)
            {
                COM_GET_ONEEVERYN(txGrpSigConstPtr_pcst->idxFilter_uo).occurrence_u8 = COM_ZERO;
            }
            txGrpSigConstPtr_pcst++;
        }
# endif /* # if (defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined(COM_F_ONEEVERYN)) */

# ifdef COM_TxSigGrpUpdateBit

        /* Clear Update bits*/
        if (Com_GetValue(TXSIGGRP,_UPDBITCONF,txSigGrpConstPtr_pcst->txSignalGrpFields_u8))
        {
            /*Find the Byte No, where the Update bit lies */
            byteNo_uo = (PduLengthType)((txSigGrpConstPtr_pcst->updateBitPos_uo) >> 3u);
            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(txIpduConstPtr_pcst,txSigGrpConstPtr_pcst->updateBitPos_uo,byteNo_uo)
        }
# endif  /* # ifdef COM_TxSigGrpUpdateBit */

        txSigGrpConstPtr_pcst++;
    }
#endif /* # if defined(COM_TxSigGrpUpdateBit) || (defined(COM_TX_SIGNALGROUP) && defined(COM_TxFilters) &&
               defined(COM_F_ONEEVERYN)) */
}

#endif /* #if defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit) || (defined(COM_TxFilters) &&
              defined(COM_F_ONEEVERYN)) */

