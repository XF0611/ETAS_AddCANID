

#ifndef CANNM_INL_H
#define CANNM_INL_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Prv.h"
#include "CanIf.h"
#if (!defined(CANIF_AR_RELEASE_MAJOR_VERSION) || (CANIF_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#include "Nm_Cbk.h"
#if (!defined(NM_AR_RELEASE_MAJOR_VERSION) || (NM_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(NM_AR_RELEASE_MINOR_VERSION) || (NM_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

LOCAL_INLINE void CanNm_ComputeSwFrTimer(
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_CancelRemoteSleepInd(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_GotoNetworkMode(
        NetworkHandleType CanNm_NetworkHandle,
        Nm_StateType PrevState,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_RepeatMessageToNO(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_RepeatMessageToRS(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_GotoRepeatMessage(
        NetworkHandleType CanNm_NetworkHandle,
        Nm_StateType PrevState,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_NormalOperationToRS(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_ReadySleepToNO(
        NetworkHandleType CanNm_NetworkHandle,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_GotoPrepareBusSleep(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_GotoBusSleep(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_UpdateTxPdu(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);

LOCAL_INLINE void CanNm_CopyBuffer(
        const uint8 * SrcPtr,
        uint8 * DestPtr,
        uint8 Len
);

LOCAL_INLINE Std_ReturnType CanNm_MessageTransmit(
                                        const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                        CanNm_RamType * RamPtr_ps
                                     );

LOCAL_INLINE Nm_StateType CanNm_NetworkModeProcessing(
                #if (CANNM_PN_ENABLED != STD_OFF && CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                            const NetworkHandleType CanNm_NetworkHandle,
                #endif
                #if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF || CANNM_BUS_LOAD_REDUCTION_ENABLED != STD_OFF || \
                     CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF )
                            const CanNm_ChannelConfigType * ConfigPtr_pcs,
                #endif
                CanNm_RamType * RamPtr_ps,
                boolean PduRxInd_b
                #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                   ,boolean PduTxConfirmation_b
                #endif
);

LOCAL_INLINE void CanNm_CaseBusSleep(
                                                  #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                                   const NetworkHandleType CanNm_NetworkHandle,
                                                  #endif
                                                   const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                                  #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                                   CanNm_RamType * RamPtr_ps,
                                                  #endif
                                                   boolean PduRxInd_b
);

LOCAL_INLINE void CanNm_CasePrepareBusSleep(
                                        const NetworkHandleType CanNm_NetworkHandle,
                                        const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                        CanNm_RamType * RamPtr_ps,
                                        boolean PduRxInd_b
);


LOCAL_INLINE void CanNm_CaseReadySleep(
                                #if (CANNM_NODE_DETECTION_ENABLED != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                        const NetworkHandleType CanNm_NetworkHandle,
                                #endif
                                        const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                        CanNm_RamType * RamPtr_ps
                                #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                        ,uint8 CtrlBitVector_u8
                                #endif
);

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
LOCAL_INLINE void CanNm_CaseNormalOperation(
                            #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                const NetworkHandleType CanNm_NetworkHandle,
                            #endif
                                const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                CanNm_RamType * RamPtr_ps
                            #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                ,uint8 CtrlBitVector_u8
                            #endif
            );
#endif

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
LOCAL_INLINE void CanNm_CaseRepeatMessage(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
);
#endif

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
LOCAL_INLINE void CanNm_Init_EIRA_Status(void);
#endif

#if (CANNM_ERACALC_ENABLED != STD_OFF)
LOCAL_INLINE void CanNm_Init_ERA_Status(void);
#endif

#if((CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_LOADABLE) && (CANNM_VERSION_INFO_API != STD_OFF))
LOCAL_INLINE boolean CanNm_CompareVersionInfo(
                                             const Std_VersionInfoType * srcVersionInfo,
                                             const Std_VersionInfoType * destVersionInfo
                                                            );
#endif

/*
 ***************************************************************************************************
 * Inline functions
 ***************************************************************************************************
 */

/***************************************************************************************************
 Function name    : CanNm_NetworkModeProcessing
 Description      : This is an internal function of CanNm for Network Mode operations
 Parameter        : CanNm_NetworkHandle - Identification of the channel
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Parameter        : PduRxInd_b          - NM PDU reception indication flag
 Parameter        : PduTxConfirmation_b - Tx confirmation flag
 Return value     : Nm_StateType        - State of network to be updated
 ***************************************************************************************************/

LOCAL_INLINE Nm_StateType CanNm_NetworkModeProcessing(
                #if (CANNM_PN_ENABLED != STD_OFF && CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                            const NetworkHandleType CanNm_NetworkHandle,
                #endif
                #if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF || CANNM_BUS_LOAD_REDUCTION_ENABLED != STD_OFF || \
                            CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF )
                            const CanNm_ChannelConfigType * ConfigPtr_pcs,
                #endif
                CanNm_RamType * RamPtr_ps,
                boolean PduRxInd_b
                #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                   ,boolean PduTxConfirmation_b
                #endif
)

{
        /* local copy of CanNm state [to reduce run-time] */
        Nm_StateType StateCopy_e;
#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
        /* To store local copy of ComM ChannelId */
        NetworkHandleType nmChannelHandle;
#endif
        /* Store a local copy of CanNm state */
        StateCopy_e = RamPtr_ps->State_en;

#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
        /* Store ComM ChannelId from the configuration */
        nmChannelHandle = ConfigPtr_pcs->NetworkHandle;
#endif
        if (PduRxInd_b != FALSE)
        {
            /* TRACE[SWS_CanNm_00098] : Restart NMTimeoutTimer on any Rx indication */
            CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
            /* Prepare for next RemoteSleep detection after every NM message reception */
            CanNm_StartTimer(RamPtr_ps->ctRemoteSleepIndTimer);

            /* Cancellation of RemoteSleepIndication upon reception is allowed only in these states */
            if ((StateCopy_e == NM_STATE_NORMAL_OPERATION) || (StateCopy_e == NM_STATE_READY_SLEEP))
            {
                CanNm_CancelRemoteSleepInd(ConfigPtr_pcs,RamPtr_ps);
            }
#endif

#if (CANNM_BUS_LOAD_REDUCTION_ENABLED != STD_OFF)

            /* check if it is normal operation mode and bus load reduction is activated*/
            if ((StateCopy_e == NM_STATE_NORMAL_OPERATION) && (ConfigPtr_pcs->stBusLoadReductionActive_b != FALSE))
            {
                /* Reload the NmMsgCycleTimer with MsgReducedTime */
                RamPtr_ps->MsgCyclePeriod = ConfigPtr_pcs->MsgReducedTime;

                /* Restart the NmMessageCycleTimer */
                CanNm_StartTimer(RamPtr_ps->PrevMsgCycleTimestamp);

                /* Restart the NmMessageTimeoutTimer*/
                CanNm_StartTimer(RamPtr_ps->PrevMsgTimeoutTimestamp);
            }
#endif

#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
        /* Check if this is the first reception of NM-PDU with NmCoordinatorSleepReady BIT set to 1 under network mode
         *  or after Nm_CoordReadyToSleepCancellation is called */
        if((RamPtr_ps->CoordReadySleepBit_u8 == 0x08) && (RamPtr_ps->stCoordReadySleepInd_b == FALSE))
        {
           Nm_CoordReadyToSleepIndication(nmChannelHandle);

            /* Indicates that Nm_CoordReadyToSleepIndication has been called.
             * Helps to give the Nm_CoordReadyToSleepIndication to Nm only for the first reception of NM-PDU with NmCoordinatorSleepReady BIT set to 1 */
            RamPtr_ps->stCoordReadySleepInd_b = TRUE;
        }
        /*Check if this is the first reception of NM-PDU with NmCoordinatorSleepReady Bit set to 0, after Nm_CoordReadyToSleepIndication is called
         * and it is still in Network mode */
        if((RamPtr_ps->CoordReadySleepBit_u8 == 0x00) && (RamPtr_ps->stCoordReadySleepInd_b != FALSE))
        {
          Nm_CoordReadyToSleepCancellation(nmChannelHandle);

            RamPtr_ps->stCoordReadySleepInd_b = FALSE;
        }
#endif
        }
        else
        {
/* Tx_Confirmation can not be received if Passive mode is Enabled*/
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
            if (PduTxConfirmation_b != FALSE)
            {
                /* TRACE[SWS_CanNm_00099] : Restart NMTimeoutTimer on Tx confirmation */
                CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);
            }
#endif

        }
#if (CANNM_PN_ENABLED != STD_OFF)
/* When Passive Mode is on there will be no Transmission of NM Pdus*/
# if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
        /* Check the flag for spontaneous transmission of Nm pdu */
        if(RamPtr_ps->PnHandleMultipleNetworkRequests_b != FALSE)
        {
            /* Reset the flag to false  */
            RamPtr_ps->PnHandleMultipleNetworkRequests_b = FALSE;

            /* TRACE[SWS_CanNm_00454] : Load the number of Immediate Nm for spontaneous transmission */
            RamPtr_ps->ImmediateNmTx_u8 = ConfigPtr_pcs->ImmediateNmTransmissions_u8;

            /* TRACE[SWS_CanNm_00444] : Change to or restart the Repeat Message state */
            CanNm_GotoRepeatMessage(CanNm_NetworkHandle, StateCopy_e, ConfigPtr_pcs, RamPtr_ps);

            /* Copy the current CanNm state to local copy after change or restart  */
            StateCopy_e = NM_STATE_REPEAT_MESSAGE;
        }
# endif
#endif
        return StateCopy_e;
}

/***************************************************************************************************
 Function name    : CanNm_CaseBusSleep
 Description      : This is an internal function of CanNm for Bus Sleep Mode operations
 Parameter        : CanNm_NetworkHandle - Identification of the channel
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Parameter        : PduRxInd_b          - NM PDU reception indication flag
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_CaseBusSleep(
                                                  #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                                   const NetworkHandleType CanNm_NetworkHandle,
                                                  #endif
                                                   const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                                  #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                                   CanNm_RamType * RamPtr_ps,
                                                  #endif
                                                   boolean PduRxInd_b
)

{
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Network Request state */
    CanNm_NetworkStateType stNetworkState_e;
#endif
    /* To store local copy of ComM ChannelId */
    NetworkHandleType nmChannelHandle;

    /* Store globals to local variables: this reduces code size & run-time */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;
#endif

    /* Store ComM ChannelId from the configuration */
    nmChannelHandle = ConfigPtr_pcs->NetworkHandle;

    /* When ECU is the PASSIVE ECU it will only wake up by the Passive start up */
    #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)

            /* TRACE[SWS_CanNm_00129] : Transit to Network Mode if network is requested */
            if (stNetworkState_e == CANNM_NETWORK_REQUESTED_E)
            {
#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
                if(ConfigPtr_pcs->RetryFirstMessageRequest_b != FALSE)
                {
                    /* Set first message flag indicating that it is the first message after transition from Bus Sleep Mode to Repeat Message State */
                    RamPtr_ps->FirstMessage_b = TRUE;
                }
#endif
                /* Store number of immediate Nm Pdus */
                RamPtr_ps->ImmediateNmTx_u8 = ConfigPtr_pcs->ImmediateNmTransmissions_u8;

                /* TRACE[SWS_CanNm_00401] : Set active wakeup bit in CBV if CanNmActiveWakeupBitEnabled is TRUE */
                if(ConfigPtr_pcs->ActiveWakeupBitEnabled_b == TRUE)
                {
                    RamPtr_ps->TxCtrlBitVector_u8 = RamPtr_ps->TxCtrlBitVector_u8 | CANNM_ACTIVE_WAKEUP_BIT_POS;
                }

                /* Internal function to do routine work of transition to Network Mode */
                CanNm_GotoNetworkMode(CanNm_NetworkHandle, NM_STATE_BUS_SLEEP, ConfigPtr_pcs, RamPtr_ps);
            }
            else
    #endif
            {
                if (PduRxInd_b != FALSE)
                {
#if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
                    if(ConfigPtr_pcs->RetryFirstMessageRequest_b != FALSE)
                    {
                        /* Set first message flag indicating that it is the first message after transition from Bus Sleep Mode to Repeat Message State */
                        RamPtr_ps->FirstMessage_b = TRUE;
                    }
#endif
                    /* TRACE[SWS_CanNm_00127] : Indicate Nm about network start */
                    Nm_NetworkStartIndication(nmChannelHandle);

                    /* TRACE[SWS_CanNm_00336] : Report DET if NM message is received in Bus-Sleep Mode */
                    #if (CANNM_DEV_ERROR_DETECT != STD_OFF)
                        (void)Det_ReportError((CANNM_MODULE_ID), nmChannelHandle, CANNM_SID_MAINFUNCTION,
                                CANNM_E_NET_START_IND);
                    #endif
                }
            }
}

/***************************************************************************************************
 Function name    : CanNm_CasePrepareBusSleep
 Description      : This is an internal function of CanNm for Prepare Bus Sleep Mode operations
 Parameter        : CanNm_NetworkHandle - Identification of the channel
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Parameter        : PduRxInd_b          - NM PDU reception indication flag
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_CasePrepareBusSleep(
        const NetworkHandleType CanNm_NetworkHandle,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps,
        boolean PduRxInd_b
)

{

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Network Request state */
    CanNm_NetworkStateType stNetworkState_e;
#endif
    /* store globals to local variables: this reduces code size & run-time */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;
#endif

    /* TRACE[SWS_CanNm_00124] : Go to Network Mode on Rx Indication */
    if (PduRxInd_b != FALSE)
    {
        /* This is a function to do the routine work of transition to Network Mode */
        CanNm_GotoNetworkMode(CanNm_NetworkHandle, NM_STATE_PREPARE_BUS_SLEEP, ConfigPtr_pcs, RamPtr_ps);
    }
    else
    {
    /* Network cannot be requested in passive mode*/
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
        /* TRACE[SWS_CanNm_00123] : Go to Network Mode on Network Request */
        if(stNetworkState_e == CANNM_NETWORK_REQUESTED_E)
        {
            /* Store number of immediate Nm Pdus */
            RamPtr_ps->ImmediateNmTx_u8 = ConfigPtr_pcs->ImmediateNmTransmissions_u8;

             /* TRACE[SWS_CanNm_00401] : Set active wakeup bit in CBV if CanNmActiveWakeupBitEnabled is TRUE */
            if(ConfigPtr_pcs->ActiveWakeupBitEnabled_b == TRUE)
            {
                RamPtr_ps->TxCtrlBitVector_u8 = RamPtr_ps->TxCtrlBitVector_u8 | CANNM_ACTIVE_WAKEUP_BIT_POS;
            }

            /* This is an internal function to do the routine work of transition to Network Mode */
            CanNm_GotoNetworkMode(CanNm_NetworkHandle, NM_STATE_PREPARE_BUS_SLEEP, ConfigPtr_pcs, RamPtr_ps);
        }
        else
#endif
        {
            /* TRACE[SWS_CanNm_00115] : Transit to Bus Sleep Mode on expiration of Wait Bus Sleep time */
            if (CanNm_TimerExpired(RamPtr_ps->ctWaitBusSleepTimer,ConfigPtr_pcs->WaitBusSleepTime) != FALSE)
            {
                CanNm_GotoBusSleep(ConfigPtr_pcs, RamPtr_ps);
            }
        }
    }

    /* TRACE[SWS_CanNm_00122] : Trigger transmission of an asynchronous message to ensure that other nodes do not enter
      Bus Sleep */
#if (CANNM_IMMEDIATE_RESTART_ENABLED != STD_OFF)
    if (stNetworkState_e == CANNM_NETWORK_REQUESTED_E)
    {
        (void)CanNm_MessageTransmit(ConfigPtr_pcs,RamPtr_ps);
    }
#endif

}

/***************************************************************************************************
 Function name    : CanNm_CaseReadySleep
 Description      : This is an internal function of CanNm for Ready Sleep state operations
 Parameter        : CanNm_NetworkHandle - Identification of the channel
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Parameter        : CtrlBitVector_u8    - Control Bit Vector
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_CaseReadySleep(
                                #if (CANNM_NODE_DETECTION_ENABLED != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                        const NetworkHandleType CanNm_NetworkHandle,
                                #endif
                                        const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                        CanNm_RamType * RamPtr_ps
                                #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                        ,uint8 CtrlBitVector_u8
                                #endif
)

{
#if ((CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF) && (CANNM_NODE_DETECTION_ENABLED != STD_OFF))
    /* To store local copy of ComM ChannelId */
    NetworkHandleType nmChannelHandle;
#endif

#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
        /* Repeat Message request state */
        uint8 stRepeatMessage_u8;
        /* Local copy Tx Repeat Message request state */
        boolean RepeatMessageRequest_b;
#endif

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Network Request state */
    CanNm_NetworkStateType stNetworkState_e;
#endif

    /* Timer to hold NM-Timeout */
    CanNm_TimerType tiNMTimeoutTimer;

#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
        /* Read the repeat message bit from the control bit vector */
        stRepeatMessage_u8 = (CtrlBitVector_u8 & CANNM_READ_RPTMSG_MASK);
        /* Read the status of Tx Repeat message request */
        RepeatMessageRequest_b = RamPtr_ps->TxRptMsgStatus_b;
#endif

/* Store globals to local variables: this reduces code size & run-time */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;
#endif
    tiNMTimeoutTimer = RamPtr_ps->ctNMTimeoutTimer;

#if ((CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF) && (CANNM_NODE_DETECTION_ENABLED != STD_OFF))
    /* Store ComM ChannelId from the configuration */
    nmChannelHandle = ConfigPtr_pcs->NetworkHandle;
#endif

#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
        /* TRACE[SWS_CanNm_00111,SWS_CanNm_00112] : Go to Repeat Message State on repeat message bit indication or
         * repeat message request if NodeDetection is enabled for the channel */
        if((ConfigPtr_pcs->NodeDetectionEnabled_b != FALSE) && ((stRepeatMessage_u8 != 0) || (RepeatMessageRequest_b != FALSE)))
        {
            /* This is an internal function to do the routine work of transition to Repeat Message state */
            CanNm_GotoRepeatMessage(CanNm_NetworkHandle,NM_STATE_READY_SLEEP, ConfigPtr_pcs, RamPtr_ps);

# if (CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF)
            /* TRACE[SWS_CanNm_00014] : Notify Repeat Message Bit reception to Nm if NodeDetection and RepeatMsgInd is
             * enabled for the channel */
            if((ConfigPtr_pcs->RepeatMsgIndEnabled_b != FALSE) && (stRepeatMessage_u8 != 0))
            {
                Nm_RepeatMessageIndication(nmChannelHandle);
            }
# endif
        }
    else
#endif
    {
        /* TRACE[SWS_CanNm_00109] : Go to Prepare Bus Sleep Mode on expiration of NM Timeout Timer */
        if ((CanNm_TimerExpired(tiNMTimeoutTimer, ConfigPtr_pcs->NMTimeoutTime) != FALSE) &&
                (RamPtr_ps->stCanNmComm != FALSE))
        {
            /* This is an internal function to do the routine work of transition to Prepare Bus Sleep state */
            CanNm_GotoPrepareBusSleep(ConfigPtr_pcs, RamPtr_ps);
        }
        else
        {
/* Network cannot be requested if passive mode is enabled */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
            /* TRACE[SWS_CanNm_00110] : Goto Normal Operation state if Network is requested */
            if (stNetworkState_e == CANNM_NETWORK_REQUESTED_E)
            {
                CanNm_ReadySleepToNO(CanNm_NetworkHandle,ConfigPtr_pcs, RamPtr_ps);
            }
            else
#endif
            {
                /* Stay in Ready Sleep */
            }
        }
    }
}

/***************************************************************************************************
 Function name    : CanNm_CaseNormalOperation
 Description      : This is an internal function of CanNm for Normal Operation state operations
 Parameter        : CanNm_NetworkHandle - Identification of the channel
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Parameter        : CtrlBitVector_u8    - Control Bit Vector
 Return value     : None
 ***************************************************************************************************/

/*This State is only available if the Passive Mode is disabled */
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
LOCAL_INLINE void CanNm_CaseNormalOperation(
        #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
            const NetworkHandleType CanNm_NetworkHandle,
        #endif
            const CanNm_ChannelConfigType * ConfigPtr_pcs,
            CanNm_RamType * RamPtr_ps
        #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
            ,uint8 CtrlBitVector_u8
        #endif
            )

{

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF) || ((CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF) && (CANNM_NODE_DETECTION_ENABLED != STD_OFF))
    /* To store local copy of ComM ChannelId */
    NetworkHandleType nmChannelHandle;
#endif

# if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
    /* Repeat Message request state */
    uint8 stRepeatMessage_u8;
    /* Local copy Tx Repeat Message request state */
    boolean RepeatMessageRequest_b;
# endif

    /* Network Request state */
    CanNm_NetworkStateType stNetworkState_e;

    /* Timer to hold NM-Timeout */
    CanNm_TimerType tiNMTimeoutTimer;

    /* Store globals to local variables: this reduces code size & run-time */
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;


# if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
    /* Read the repeat message bit from the control bit vector */
    stRepeatMessage_u8 = (CtrlBitVector_u8 & CANNM_READ_RPTMSG_MASK);
    /* Read the status of Tx Repeat message request */
    RepeatMessageRequest_b = RamPtr_ps->TxRptMsgStatus_b;
# endif

    tiNMTimeoutTimer = RamPtr_ps->ctNMTimeoutTimer;

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF) || ((CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF) && (CANNM_NODE_DETECTION_ENABLED != STD_OFF))
    /* Store ComM ChannelId from the configuration */
    nmChannelHandle = ConfigPtr_pcs->NetworkHandle;
#endif

    /* These timer expirations are valid only if transmission ability is enabled */
    if (RamPtr_ps->stCanNmComm != FALSE)
    {
        /* TRACE[SWS_CanNm_00117] : Restart NM Timeout timer if it expired */
        if (CanNm_TimerExpired(tiNMTimeoutTimer, ConfigPtr_pcs->NMTimeoutTime) != FALSE)
        {
            CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

            /* Report DET that the NM Timeout has expired abnormally */
            CANNM_DET_REPORT_ERROR_NO_RET((CanNm_TimerExpired(tiNMTimeoutTimer, ConfigPtr_pcs->NMTimeoutTime) != FALSE),
                                    ConfigPtr_pcs->NetworkHandle, CANNM_SID_MAINFUNCTION, CANNM_E_NETWORK_TIMEOUT)
        }

        /* TRACE[SWS_CanNm_00150] : Indicate remote sleep to Nm if RemoteSleepTimer has expired */
# if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)

        if (CanNm_TimerExpired(RamPtr_ps->ctRemoteSleepIndTimer,ConfigPtr_pcs->RemoteSleepIndTime) != FALSE)
        {
            if (CANNM_REMOTE_SLEEP_DETECTED() == FALSE)
            {
                CANNM_SET_REMOTE_SLEEP();
                CanNm_StartTimer(RamPtr_ps->ctRemoteSleepIndTimer);
                Nm_RemoteSleepIndication(nmChannelHandle);
            }
        }
# endif
    }
# if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
         /* TRACE[SWS_CanNm_00119,SWS_CanNm_00120] : Go to Repeat Message state on repeat message bit indication or
               repeat message request if Node Detection is enabled for the channel */
           if((ConfigPtr_pcs->NodeDetectionEnabled_b != FALSE) && ((stRepeatMessage_u8 != 0) || (RepeatMessageRequest_b != FALSE)))
            {
                /* This is an internal function to do the routine work of transition to Repeat Message state */
                CanNm_GotoRepeatMessage(CanNm_NetworkHandle,NM_STATE_NORMAL_OPERATION, ConfigPtr_pcs, RamPtr_ps);
#  if (CANNM_REPEAT_MSG_IND_ENABLED != STD_OFF)
                if((ConfigPtr_pcs->RepeatMsgIndEnabled_b != FALSE) && (stRepeatMessage_u8 != 0))
                {
                    /* TRACE[SWS_CanNm_00014] : Notify Repeat Message Bit reception to Nm if Node Detection and
                     * RepeatMsgInd is enabled for the channel */
                    Nm_RepeatMessageIndication(nmChannelHandle);
                }
#  endif
             }
         else
# endif
        {
            /* TRACE[SWS_CanNm_00118] : Go to Ready Sleep State if network is released */
            if (stNetworkState_e == CANNM_NETWORK_RELEASED_E)
            {
                /* This is an internal function to do the routine work of transition to Ready Sleep state */
                CanNm_NormalOperationToRS(ConfigPtr_pcs, RamPtr_ps);
            }
            else
            {
                /* stay in Normal Operation */
            }
        }
}
#endif /* End of the Passive Mode Switch */

/***************************************************************************************************
 Function name    : CanNm_CaseRepeatMessage
 Description      : This is an internal function of CanNm for Repeat Message state operations
 Parameter        : ConfigPtr_pcs       - Pointer to Config data structure currently under use
 Parameter        : RamPtr_ps           - Pointer to RAM data structure currently under use
 Return value     : None
 ***************************************************************************************************/

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
LOCAL_INLINE void CanNm_CaseRepeatMessage(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    /* local copy of CanNmRepeatMessageTime [to reduce run-time] */
    CanNm_TimerType RepeatMsgTime;

    /* Network Request state */
    CanNm_NetworkStateType stNetworkState_e;

    /* Timer to hold NM-Timeout */
    CanNm_TimerType tiNMTimeoutTimer;

    /* Store globals to local variables: this reduces code size & run-time */
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;

    tiNMTimeoutTimer = RamPtr_ps->ctNMTimeoutTimer;
    RepeatMsgTime = ConfigPtr_pcs->RepeatMessageTime;

     /* TRACE[SWS_CanNm_00101] : Restart NM timeout timer if it has expierd */
     if ((CanNm_TimerExpired(tiNMTimeoutTimer, ConfigPtr_pcs->NMTimeoutTime) != FALSE) &&
             (RamPtr_ps->stCanNmComm != FALSE))
     {
         CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

         /* Report DET that the NM Timeout has expired abnormally */
         CANNM_DET_REPORT_ERROR_NO_RET((CanNm_TimerExpired(tiNMTimeoutTimer, ConfigPtr_pcs->NMTimeoutTime) != FALSE),
                                                 ConfigPtr_pcs->NetworkHandle, CANNM_SID_MAINFUNCTION, CANNM_E_NETWORK_TIMEOUT)
     }

     /* TRACE[SWS_CanNm_00102] : Leave Repeat Message state if RepeatMessageTimer has expired */
     if (CanNm_TimerExpired(RamPtr_ps->ctRepeatMessageTimer, RepeatMsgTime) != FALSE)
     {

# if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
         /* Check if Node Detection is enabled for the channel */
         if(ConfigPtr_pcs->NodeDetectionEnabled_b != FALSE)
         {
             /* TRACE[SWS_CanNm_00107] : Clear Repeat Message request bit when Repeat Message state is left */
             RamPtr_ps->TxCtrlBitVector_u8 = ( RamPtr_ps->TxCtrlBitVector_u8 & CANNM_RESET_RPTMSG_MASK );
         }
# endif

         /* TRACE[SWS_CanNm_00103,SWS_CanNm_00106] : Transit to Normal Operation or Ready Sleep state based on network
            request state */
         if (stNetworkState_e == CANNM_NETWORK_RELEASED_E)
         {
             CanNm_RepeatMessageToRS(ConfigPtr_pcs, RamPtr_ps);
         }
         else
         {
             CanNm_RepeatMessageToNO(ConfigPtr_pcs, RamPtr_ps);
         }
     }
     else
     {
         /* Stay in Repeat Message State until RepeatMessageTimer expires */
     }
}
#endif /* End of the Passive Mode Switch */

/***************************************************************************************************
 Function name    : CanNm_Init_EIRA_Status
 Description      : This inline function initialises PN EIRA timer, EIRA Current status and  Global
                    status arrays
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
LOCAL_INLINE void CanNm_Init_EIRA_Status(void)
{

    /* Variable to hold byte position in the buffer */
    uint8_least Index_ui;

    /* Initialize the timers for all the PN's */
    for(Index_ui = 0; Index_ui < CANNM_GET_NUMBER_OF_PN_CONFIGURED(); Index_ui++)
    {
        CanNm_PNTimer_au32[Index_ui] = 0;
    }
    /* Initialize the current and global status of all the PN's */
    for (Index_ui = 0;Index_ui < CANNM_PN_INFOLENGTH;Index_ui++)
    {
        CanNm_EIRACurrentStatus_au8[Index_ui] = 0;
        CanNm_EIRAGlobalStatus_au8[Index_ui] = 0;
    }
}
#endif

/***************************************************************************************************
 Function name    : CanNm_Init_ERA_Status
 Description      : This inline function initialises PN ERA timer, ERA Current status and  Global
                    status arrays
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/

#if (CANNM_ERACALC_ENABLED != STD_OFF)

LOCAL_INLINE void CanNm_Init_ERA_Status(void)
{
    /* Variable to hold byte position in the buffer */
    uint16_least Index_ui;

    /* Initialize the timers for all the PN's */
    for(Index_ui = 0; Index_ui < (CANNM_GET_NUMBER_OF_PN_CONFIGURED() * CANNM_NUMBER_OF_ERA_CHANNELS); Index_ui++)
    {
        CanNm_PNTimer_ERA_au32[Index_ui] = 0;
    }
    /* Initialize the cuurent and global status of all the PN's */
    for (Index_ui = 0;Index_ui < CANNM_ERA_ARRAYSIZE;Index_ui++)
    {
        CanNm_ERAGlobalStatus_au8[Index_ui] = 0;
        CanNm_ERACurrentStatus_au8[Index_ui] = 0;
    }
}
#endif

/***************************************************************************************************
 Function name    : CanNm_ComputeSwFrTimer
 Description      : This inline function computes the free running timer using Main Function period.
 Parameter        : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_ComputeSwFrTimer(
        CanNm_RamType * RamPtr_ps
)
{
    /* Read the current time */
    RamPtr_ps->ctSwFrTimer += CANNM_MAINFUNCTION_PERIOD;
}

/***************************************************************************************************
 Function name    : CanNm_CancelRemoteSleepInd
 Description      : This inline function cancels Remote Sleep indication if detected already.
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_CancelRemoteSleepInd(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    if (CANNM_REMOTE_SLEEP_DETECTED() != FALSE)
    {
        CANNM_RESET_REMOTE_SLEEP();

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
        Nm_RemoteSleepCancellation(ConfigPtr_pcs->NetworkHandle);
#else
        (void)ConfigPtr_pcs;
#endif
    }
}

/***************************************************************************************************
 Function name    : CanNm_GotoNetworkMode
 Description      : This inline function does transition to to Network Mode from either Bus Sleep
                    state or Prepare Bus Sleep state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
                    CanNm_NetworkHandle - Identification of the NM-channel
                    PrevState - state from which Network Mode is entered
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_GotoNetworkMode(
        NetworkHandleType CanNm_NetworkHandle,
        Nm_StateType PrevState,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    /* Do state transition to repeat message state */
    CanNm_ChangeState(RamPtr_ps, NM_STATE_REPEAT_MESSAGE, NM_MODE_NETWORK);

    /* Indicate Nm about Network mode */
    Nm_NetworkMode(ConfigPtr_pcs->NetworkHandle);

    /* Start Repeat Message timer */
    if(ConfigPtr_pcs->RepeatMessageTime != 0U)
    {
        CanNm_StartTimer(RamPtr_ps->ctRepeatMessageTimer);
    }

#if (CANNM_PASSIVE_MODE_ENABLED != STD_OFF)
    //* if network is released, goto Ready Sleep state */
    RamPtr_ps->State_en = NM_STATE_READY_SLEEP;

    CanNm_ChangeState(RamPtr_ps, NM_STATE_READY_SLEEP, NM_MODE_NETWORK);

    /* Notify to the upper layer about changes of the CanNm states if configuration parameter
       CANNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, NM_STATE_REPEAT_MESSAGE, NM_STATE_READY_SLEEP);
#endif
#endif

    /* Start NM-Timeout time */
    CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

    /* Start transmission */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    CanNm_StartTransmission(CanNm_NetworkHandle);
#else
    (void)CanNm_NetworkHandle;
#endif

#if ((CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, PrevState, NM_STATE_REPEAT_MESSAGE);
#else
    (void)PrevState;
#endif
}

/***************************************************************************************************
 Function name    : CanNm_RepeatMessageToNO
 Description      : This inline function does transition to Normal Operation state from Repeat Message state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_RepeatMessageToNO(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    /* Goto Normal Operation state if Network is requested */
    RamPtr_ps->State_en = NM_STATE_NORMAL_OPERATION;

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
    CanNm_StartTimer(RamPtr_ps->ctRemoteSleepIndTimer);

    /* Set the Remote Sleep state to Undetected */
    CANNM_RESET_REMOTE_SLEEP();
#endif

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle,NM_STATE_REPEAT_MESSAGE,NM_STATE_NORMAL_OPERATION);
#else
    (void)ConfigPtr_pcs;
#endif
}

/***************************************************************************************************
 Function name    : CanNm_RepeatMessageToRS
 Description      : This inline function does transition to Ready Sleep state from Repeat Message state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_RepeatMessageToRS(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
	RamPtr_ps->State_en = NM_STATE_READY_SLEEP;

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, NM_STATE_REPEAT_MESSAGE, NM_STATE_READY_SLEEP);
#else
    (void)ConfigPtr_pcs;
#endif

    /* Stop Tx */
    RamPtr_ps->MsgTxStatus_b = FALSE;
}

/***************************************************************************************************
 Function name    : CanNm_GotoRepeatMessage
 Description      : This function does transition to Repeat Message from either Normal Operation or Ready Sleep state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
                    CanNm_NetworkHandle - Identification of the NM-channel
                    PrevState - state from which Network Mode is entered
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_GotoRepeatMessage(
        NetworkHandleType CanNm_NetworkHandle,
        Nm_StateType PrevState,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
	/* Transition to Repeat Message state  */
	RamPtr_ps->State_en = NM_STATE_REPEAT_MESSAGE;
#if (CANNM_NODE_DETECTION_ENABLED != STD_OFF)
        /* Reset the Repeat Message Status bit */
        RamPtr_ps->TxRptMsgStatus_b = FALSE;
#endif
    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, PrevState, NM_STATE_REPEAT_MESSAGE);
#else
    (void)PrevState;
#endif

    /* Start Repeat Message timer */
    if(ConfigPtr_pcs->RepeatMessageTime != 0U)
    {
        /* Start Repeat Message timer */
        CanNm_StartTimer(RamPtr_ps->ctRepeatMessageTimer);
    }

    /* Start transmission */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    CanNm_StartTransmission(CanNm_NetworkHandle);
#else
    (void)CanNm_NetworkHandle;
#endif

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
    CanNm_CancelRemoteSleepInd(ConfigPtr_pcs,RamPtr_ps);
#endif

#if ((CANNM_STATE_CHANGE_IND_ENABLED == STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED != STD_OFF) && \
        (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_OFF))
    (void)ConfigPtr_pcs;
#endif
}

/***************************************************************************************************
 Function name    : CanNm_NormalOperationToRS
 Description      : This inline function does transition to Ready Sleep state from Normal Operation state.
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_NormalOperationToRS(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
	RamPtr_ps->State_en = NM_STATE_READY_SLEEP;

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, NM_STATE_NORMAL_OPERATION, NM_STATE_READY_SLEEP);
#else
    (void)ConfigPtr_pcs;
#endif

    /* Stop Tx */
    RamPtr_ps->MsgTxStatus_b = FALSE;
}

/***************************************************************************************************
 Function name    : CanNm_ReadySleepToNO
 Description      : This inline function does transition to Normal Operation state from Ready Sleep state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_ReadySleepToNO(
        NetworkHandleType CanNm_NetworkHandle,
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    /* goto Normal Operation state if Network is requested */
    RamPtr_ps->State_en = NM_STATE_NORMAL_OPERATION;

    /* Start transmission */
    CanNm_StartTransmission(CanNm_NetworkHandle);

#if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
    CanNm_StartTimer(RamPtr_ps->ctRemoteSleepIndTimer);

    /* Set the Remote Sleep state to Undetected */
    CANNM_RESET_REMOTE_SLEEP();
#endif

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle,NM_STATE_READY_SLEEP,NM_STATE_NORMAL_OPERATION);
#else
    (void)ConfigPtr_pcs;
#endif
}

/***************************************************************************************************
 Function name    : CanNm_GotoPrepareBusSleep
 Description      : This inline function does transition to Prepare Bus Sleep state from Ready Sleep state
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_GotoPrepareBusSleep(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{

    /* Reset active wakeup bit in transmit control bit vector after Network mode is left */
    RamPtr_ps->TxCtrlBitVector_u8 = RamPtr_ps->TxCtrlBitVector_u8 & ((uint8)(~(CANNM_ACTIVE_WAKEUP_BIT_POS)));

    /* goto Prepare Bus sleep mode as NMTimeout has expired */
    CanNm_ChangeState(RamPtr_ps, NM_STATE_PREPARE_BUS_SLEEP, NM_MODE_PREPARE_BUS_SLEEP);

    /* TRACE[SWS_CanNm_00114] :  Notify NM Interface about transition to Prepare Bus Sleep */
    Nm_PrepareBusSleepMode(ConfigPtr_pcs->NetworkHandle);

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, NM_STATE_READY_SLEEP, NM_STATE_PREPARE_BUS_SLEEP);
#endif

    /* Start wait bus sleep time */
    /* NM shall wait in Prepare Bus sleep mode until ctWaitBusSleepTimer expires */
    CanNm_StartTimer(RamPtr_ps->ctWaitBusSleepTimer);
}

/***************************************************************************************************
 Function name    : CanNm_GotoBusSleep
 Description      : This inline function does transition to Bus Sleep mode from Prepare Bus Sleep mode
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_GotoBusSleep(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{
    /* Goto Bus Sleep as timer expired */
    CanNm_ChangeState(RamPtr_ps, NM_STATE_BUS_SLEEP, NM_MODE_BUS_SLEEP);

    /* Clear the Rx status information for this NM Sleep-Cycle */
    RamPtr_ps->RxStatus_b = FALSE;
#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
    RamPtr_ps->CoordReadySleepBit_u8 = 0x00;
    RamPtr_ps->stCoordReadySleepInd_b = FALSE;
    RamPtr_ps->TxCtrlBitVector_u8 = RamPtr_ps->TxCtrlBitVector_u8 & (~(CANNM_COORD_READY_SLEEP_BIT_MASK));
#endif

    /* Inform Nm about change of state */
#if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(ConfigPtr_pcs->NetworkHandle, NM_STATE_PREPARE_BUS_SLEEP, NM_STATE_BUS_SLEEP);
#endif

    /* TRACE[SWS_CanNm_00126] : Notify Nm about transition to Bus Sleep mode */
    /* This triggers shut down of the corresponding bus */
    Nm_BusSleepMode(ConfigPtr_pcs->NetworkHandle);
}

/***************************************************************************************************
 Function name    : CanNm_UpdateTxPdu
 Description      : This inline function updates User Data in the outgoing NM PDU
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
LOCAL_INLINE void CanNm_UpdateTxPdu(
        const CanNm_ChannelConfigType * ConfigPtr_pcs,
        CanNm_RamType * RamPtr_ps
)
{

    /* Pointer to Tx buffer */
    uint8 * TxBufferPtr;

    /* Variable to hold PduInfo */
#if (( CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || ( CANNM_EIRACALC_ENABLED != STD_OFF))
    PduInfoType PduInfo_s;
#endif

#if(CANNM_USER_DATA_ENABLED != STD_OFF)
    /* Pointer to user data */
    uint8 * UserDataPtr;
    /* Index of the User bytes in Tx buffer */
    uint8_least UserDataOffset;
#endif

#if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    /*Local buffer to fetch data from Com */
    uint8  TxDataBuffer_au8[CANNM_PDU_LENGTH_MAX];
    /* Pointer to local buffer */
   uint8 * TxDataBuffer_pu8;
#endif

    /**** End Of Declarations ****/
    RamPtr_ps = RamPtr_ps;


#if(CANNM_USER_DATA_ENABLED != STD_OFF)
    UserDataOffset = ConfigPtr_pcs->PduLength_u8 - ConfigPtr_pcs->UserDataLength_u8;
    UserDataPtr = &(RamPtr_ps->UserDataBuffer_au8[0]);
#endif

 /* Get the user data bytes if COM User data is enabled */
#if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
    PduInfo_s.SduDataPtr = TxDataBuffer_au8;
    PduInfo_s.SduLength  = ConfigPtr_pcs->PduLength_u8;
    (void)PduR_CanNmTriggerTransmit(ConfigPtr_pcs->PduRId,&PduInfo_s);

    /* Pointer to position where user data is present */
    TxDataBuffer_pu8 = &TxDataBuffer_au8[UserDataOffset];
    /* Copy user data received from Com to UserDataBuffer */
    CanNm_CopyBuffer(TxDataBuffer_pu8,RamPtr_ps->UserDataBuffer_au8,ConfigPtr_pcs->UserDataLength_u8);

#endif



    /* Enter Critical Section */
    SchM_Enter_CanNm_UpdateTxPduDataNoNest();

    /* Check if Control bit vector is enabled */
    if(ConfigPtr_pcs->ControlBitVectorPos_u8 != CANNM_PDU_OFF )
    {
        /* Initialize the local TxBuffer */
        TxBufferPtr = &(RamPtr_ps->TxBuffer_au8[0]);

        /* Store the Control bit vector to be transmitted in the TxBuffer */
        TxBufferPtr[ConfigPtr_pcs->ControlBitVectorPos_u8] = RamPtr_ps->TxCtrlBitVector_u8 ;
    }

#if(CANNM_USER_DATA_ENABLED != STD_OFF)

    /* Initialize the local TxBuffer */
    TxBufferPtr = &(RamPtr_ps->TxBuffer_au8[UserDataOffset]);

    /* This is an internal function with predictable run-time; hence doesn't affect interrupt lock */
    CanNm_CopyBuffer(UserDataPtr,TxBufferPtr,ConfigPtr_pcs->UserDataLength_u8);

#endif

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
    {
        uint8 * data_pu8;
        const uint8 * PnFilterMask_pcu8;
        uint8 index;
        /* Index of the User bytes in Tx buffer */
        uint8 PNDataOffset;

        PNDataOffset  = CANNM_PN_INFO_OFFSET - (ConfigPtr_pcs->PduLength_u8 - ConfigPtr_pcs->UserDataLength_u8);
        data_pu8 = &PduInfo_s.SduDataPtr[PNDataOffset];
        PnFilterMask_pcu8 = CanNm_GlobalConfigData_pcs->PnFilterMask_pcu8;
        /* EIRA should not be calculated when
         * 1. The corresponding channel has configured CanNmPNEnabled to FALSE
         * 2. The received message has no PNI bit set in the CBV  */
        if (CANNM_GetPnEnabledStatus(ConfigPtr_pcs) != FALSE)
        {

            for (index = 0;index < CANNM_PN_INFOLENGTH;index++)
            {
                //Check if any PN , which is relevant for ECU, is requested
                if ((data_pu8[index] & PnFilterMask_pcu8[index]) != 0u)
                {
                    CanNm_EIRACurrentStatus_au8[index] = CanNm_EIRACurrentStatus_au8[index] | data_pu8[index];
                }
            }
        }
    }
#endif

    /* Leave Critical Section */
    SchM_Exit_CanNm_UpdateTxPduDataNoNest();

}
#endif

/***************************************************************************************************
 Function name    : CanNm_CopyBuffer
 Description      : This inline function copies data from SrcPtr to DestPtr
 Parameter        : SrcPtr - pointer to source address
                  : DestPtr - pointer to destination address
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE void CanNm_CopyBuffer(
        const uint8 * SrcPtr,
        uint8 * DestPtr,
        uint8 Len
)
{
    uint8_least Index_ui;

    for(Index_ui = 0; Index_ui < Len; Index_ui++)
    {
        *DestPtr = *SrcPtr;
        SrcPtr++;
        DestPtr++;
    }
}

/***************************************************************************************************
 Function name    : CanNm_MessageTransmit
 Description      : This inline function is for transmitting NM PDUs by calling CanIf_Tranmsit
 Parameter        : ConfigPtr_pcs - pointer to the Configuration structure
                  : RamPtr_ps - pointer to the RAM structure
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE Std_ReturnType CanNm_MessageTransmit(
                                        const CanNm_ChannelConfigType * ConfigPtr_pcs,
                                        CanNm_RamType * RamPtr_ps
                                     )

{
     /* Variable to store return value of the function */
     Std_ReturnType RetValOfFuncs_ui;

     /* Set TxTimeoutMonitoringActive_b flag for monitoring message timeout time */
     RamPtr_ps->TxTimeoutMonitoringActive_b = TRUE;

     /* Transmit the PDU */
     RetValOfFuncs_ui = CanIf_Transmit(ConfigPtr_pcs->TxPduId, &(RamPtr_ps->Pdu_s));

     if(RetValOfFuncs_ui == E_OK)
     {
       /* Start Message timeout time only if message is accepted */
       CanNm_StartTimer(RamPtr_ps->PrevMsgTimeoutTimestamp);

        #if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
            if(ConfigPtr_pcs->RetryFirstMessageRequest_b != FALSE)
            {
                /* Transmit request was accepted by CanIf*/
                RamPtr_ps->FirstTransmissionFailure_b = FALSE;
            }
        #endif

     }
     else
     {
         /* Stop monitoring Msg timeout time as transmission is unsuccessful */
         RamPtr_ps->TxTimeoutMonitoringActive_b = FALSE;

        #if (CANNM_RETRY_FIRST_MSG_REQ_ENABLED != STD_OFF)
            if(ConfigPtr_pcs->RetryFirstMessageRequest_b != FALSE)
            {
                /* Transmit request was not accepted by CanIf*/
                RamPtr_ps->FirstTransmissionFailure_b = TRUE;
            }
        #endif
     }

     return RetValOfFuncs_ui;
}

#if ((CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_LOADABLE) && (CANNM_VERSION_INFO_API != STD_OFF))

/***************************************************************************************************
 Function name    : CanNm_CompareVersionInfo
 Description      : This inline function to compare version info structures
 Parameter        : srcVersionInfo    -  pointer to first Std_VersionInfoType structure
                  : destVersionInfo   -  pointer to second Std_VersionInfoType structure
 Return value     : True - memory area contents are equal
                    False - memory area contents differ
 ***************************************************************************************************/

LOCAL_INLINE boolean CanNm_CompareVersionInfo(
                                              const Std_VersionInfoType * srcVersionInfo,
                                              const Std_VersionInfoType * destVersionInfo
                                                              )
{
    boolean stEqual_b;
    stEqual_b = TRUE;
    if ((srcVersionInfo->vendorID == destVersionInfo->vendorID) &&
        (srcVersionInfo->moduleID == destVersionInfo->moduleID) &&
        (srcVersionInfo->sw_major_version == destVersionInfo->sw_major_version) &&
        (srcVersionInfo->sw_minor_version == destVersionInfo->sw_minor_version) &&
        (srcVersionInfo->sw_patch_version == destVersionInfo->sw_patch_version)
    )
    {
        stEqual_b = FALSE;
    }
    return stEqual_b;
}
#endif

#endif /* CANNM_INL_H */


