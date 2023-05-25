
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface header file for declaration of callback notifications,
 * this is included by the <Bus>Nm modules */
#include "Nm_Cbk.h"
/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"
/* Included to access ComM APIs related with NM */
#include "ComM.h"
#if(NM_ENABLE_INTER_MODULE_CHECKS)
# if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(NM_ENABLE_INTER_MODULE_CHECKS) */

#if(NM_STATE_REPORT_ENABLED != STD_OFF)

#include "Com.h"
#if(NM_ENABLE_INTER_MODULE_CHECKS)
# if (!defined(COM_AR_RELEASE_MAJOR_VERSION) || (COM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(COM_AR_RELEASE_MINOR_VERSION) || (COM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(NM_ENABLE_INTER_MODULE_CHECKS) */

#endif
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

#if (NM_AR45_VERSION != STD_OFF)

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_TxTimeoutException
 * Syntax :       void Nm_TxTimeoutException(const NetworkHandleType NetworkHandle)
 * Description:   This is the NM call-back interface notifying Transmission timeout. This is called by FrNm
 *                when it detects timeout in getting Tx Confirmation.
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_TxTimeoutException(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_TransmissionFailure(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/*****************************************************************************************************
 * Function name: Nm_NetworkTimeoutException
 * Syntax: void Nm_NetworkTimeoutException(const NetworkHandleType NetworkHandle)
 * Description : This is the NM call-back interface notifying expiration of NM-Timeout time.
 *               This is called by either CNm or FrNm when a timeout of NM-Timeout timer is detected
 *               This in turn notifies ComM.
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_NetworkTimeoutException(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_NetworkTimeoutException(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/*****************************************************************************************************
 * Function name: Nm_CarWakeUpIndication
 * Syntax: void Nm_CarWakeUpIndication (NetworkHandleType NetworkHandle)
 * Description : This is the NM call-back interface notifying Car Wake Up Indication.
 *               This is called by either CanNm or FrNm when CarWakeUp request is received in the NM-PDU
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_CarWakeUpIndication(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
       {
			#if (NM_CAR_WAKEUP_CALLOUT_ENABLED != STD_OFF)
			   	NM_USER_CARWAKEUPINDICATION(NetworkHandle);
			#else
			   	BswM_NmIf_CarWakeUpIndication(NetworkHandle);
			#endif
       }
    else
    {
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CARWAKEUPINDICATION, NM_E_INVALID_CHANNEL);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
/*****************************************************************************************************
 * Function name: Nm_PduRxIndication
 * Syntax: void Nm_PduRxIndication(
 *                                      const NetworkHandleType NetworkHandle
 *                                     )
 * Description : This is the NM call-back interface to notify that the NM has received a message.
 *               This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_PDU_RX_INDICATION_ENABLED. It in turn notifies the application
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#if (NM_PDU_RX_INDICATION_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_PduRxIndication(NetworkHandleType NetworkHandle)
{
    const Nm_UserCallbackType_tst * UserCallback_pcst;
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    UserCallback_pcst = &Nm_UserCallbacks;

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* call application call-back */
        if(UserCallback_pcst->PduRxIndication != NULL_PTR)
        {
            (*UserCallback_pcst->PduRxIndication)(NetworkHandle);
        }
    }
    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/*****************************************************************************************************
 * Function name: Nm_StateChangeNotification
 * Syntax: void Nm_StateChangeNotification(
 *                                              const NetworkHandleType nmNetworkHandle,
 *                                              const Nm_StateType nmPreviousState,
 *                                              const Nm_StateType nmCurrentState
 *                                             )
 * Description: This is the NM call-back interface to notify state changes. This is called by either CanNm
 *              or FrNm. It in turn notifies the application
 * Parameter1: nmNetworkHandle - Identification of the NM-channel
 * Parameter2: nmPreviousState - previous state of NM
 * Parameter3: nmCurrentState  - current state of NM
 * Return: void
 ****************************************************************************************************/

#if (NM_STATE_CHANGE_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_StateChangeNotification(NetworkHandleType nmNetworkHandle, Nm_StateType nmPreviousState,
                                                                               Nm_StateType nmCurrentState)
{
    const Nm_UserCallbackType_tst * UserCallback_pcst;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    const Nm_ConfigType * ConfDataPtr_pcst;
#endif
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* Since Repeat Message state has many entry points, check has to be done whether the
         * state change call is for entry into Network Mode
         */
        if ((nmCurrentState == NM_STATE_REPEAT_MESSAGE) && (nmPreviousState == NM_STATE_SYNCHRONIZE))
        {
            /* Notify ComM about Network mode */
            ComM_Nm_NetworkMode(nmNetworkHandle);
        }
        else
        {
            /* the transition is either within Network Mode OR it is for non-FrNm node;
             * hence no further notification is made
             */
        }

        UserCallback_pcst = &Nm_UserCallbacks;
        /* Update the application about NM state change */
        if (UserCallback_pcst->StateChangeInd != NULL_PTR)
        {
            (*UserCallback_pcst->StateChangeInd)(nmNetworkHandle, nmPreviousState, nmCurrentState);
        }


        /* Check if NMS report is enabled for the channel */
#if (NM_STATE_REPORT_ENABLED != STD_OFF)

        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];

        if (ConfDataPtr_pcst->NMSSignal != NM_NMS_REPORT_DISABLED)
        {
            uint8 NMSValue_u8;
            NMSValue_u8 = 0xFF;
            if (nmCurrentState == NM_STATE_REPEAT_MESSAGE)
            {
                switch(nmPreviousState)
                {
                    case NM_STATE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from BusSleepMode) */
                        NMSValue_u8 = NM_NMS_RM_BSM;
                        break;
                    }

                    case  NM_STATE_SYNCHRONIZE:
                    {
                          /* NM in state RepeatMessage (transition from Synchronize) */
                         NMSValue_u8 = NM_NMS_RM_BSM;
                         break;
                    }

                    case NM_STATE_PREPARE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from PrepareBusSleepMode) */
                        NMSValue_u8 = NM_NMS_RM_PBSM;
                        break;
                    }

                    case NM_STATE_READY_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from ReadySleep) */
                        NMSValue_u8 = NM_NMS_RM_RS;
                        break;
                    }

                    case NM_STATE_NORMAL_OPERATION:
                    {
                        /* NM in state RepeatMessage (transition from NormalOperation) */
                        NMSValue_u8 = NM_NMS_RM_NO;
                        break;
                    }

                    default:
                        /* Intentionally empty */
                        break;

                }

            }
            else if (nmCurrentState == NM_STATE_NORMAL_OPERATION)
            {
                if (nmPreviousState == NM_STATE_REPEAT_MESSAGE)
                {
                    /* NM in state NormalOperation (transition from RepeatMessage) */
                    NMSValue_u8 = NM_NMS_NO_RM;
                }
                else if (nmPreviousState == NM_STATE_READY_SLEEP)
                {
                    /* NM in state NormalOperation (transition from ReadySleep) */
                    NMSValue_u8 = NM_NMS_NO_RS;

                }
                else
                {
                    /* Intentionally Empty */
                }

            }
            else
            {
                /* Intentionally Empty */
            }
            /* NMS value is within the range defined, hence report the state change */
            if (NMSValue_u8 != 0xFF)
            {
                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void as per Autosar guidelines. */
                (void)Com_SendSignal(ConfDataPtr_pcst->NMSSignal, &NMSValue_u8);
            }
        }
#endif /* #if (NM_STATE_REPORT_ENABLED != STD_OFF) */
    }

    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/*****************************************************************************************************
 * Function name: Nm_RepeatMessageIndication
 * Syntax: void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
 * Description : This is an NM call-back interface to notify that an NM msg is received with Repeat Message
 *               Bit set. This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_REPEAT_MESSAGE_IND_ENABLED. It in turn notifies the application
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

/*This feature RepeatMessageIndication is now in channel and this should be taken care while using it in future.*/
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
{
	(void)nmNetworkHandle;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/*****************************************************************************************************
 * Function name: Nm_SychronizationPoint
 * Syntax: void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
 * Description : This is an NM call-back interface to notify the NM Coordinator functionality that this is a suitable
 *               point in time to initiate the coordinated shutdown on.This is called by Cyclic channel eg.FrNm.
 *               This interface is available depending on the configuration parameter NM_COORDINATOR_SUPPORT_ENABLED.
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
{
    // Declare local variable for channels,cluster and conf channnel
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    NetworkHandleType Nm_NetworkHandle_u8;

    Nm_NetworkHandle_u8 = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /*Check if channel is part of any cluster*/
        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];
            MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];

            /*check if the channel is configured for synchronization and whether the channel is active*/
            if((ConfDataPtr_pcst->NmSynchronizingNetwork !=FALSE) && (ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL))
            {
                /*checking the cluster has reached proper state before setting he Syncpoint to reached state*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if((GlobalRamDataPtr_pst->SyncPointState_en == NM_SYNCPOINT_READY) &&  \
                    (MainNetworkRamDataPtr_pst->RemoteSleepStatus_b == TRUE) && \
                    (MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_NORMAL))
                {
                    GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_REACHED;
                }
                SchM_Exit_Nm_ChangeStateNoNest();

            }
        }
    }
    else
    {
        NM_DET_REPORT_ERROR(nmNetworkHandle, NM_SID_SYNCHRONIZATIONPOINT, NM_E_INVALID_CHANNEL);
    }

}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#else


/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_TxTimeoutException
 * Syntax :       void Nm_TxTimeoutException(const NetworkHandleType NetworkHandle)
 * Description:   This is the NM call-back interface notifying Transmission timeout. This is called by FrNm
 *                when it detects timeout in getting Tx Confirmation.
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/

#if (NM_PASSIVE_MODE_ENABLED == STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_TxTimeoutException(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_TransmissionFailure(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/*****************************************************************************************************
 * Function name: Nm_NetworkTimeoutException
 * Syntax: void Nm_NetworkTimeoutException(const NetworkHandleType NetworkHandle)
 * Description : This is the NM call-back interface notifying expiration of NM-Timeout time.
 *               This is called by either CNm or FrNm when a timeout of NM-Timeout timer is detected
 *               This in turn notifies ComM.
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_NetworkTimeoutException(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_NetworkTimeoutException(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/*****************************************************************************************************
 * Function name: Nm_CarWakeUpIndication
 * Syntax: void Nm_CarWakeUpIndication (NetworkHandleType NetworkHandle)
 * Description : This is the NM call-back interface notifying Car Wake Up Indication.
 *               This is called by either CanNm or FrNm when CarWakeUp request is received in the NM-PDU
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_CarWakeUpIndication(NetworkHandleType NetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
       {
            #if (NM_CAR_WAKEUP_CALLOUT_ENABLED != STD_OFF)
                NM_USER_CARWAKEUPINDICATION(NetworkHandle);
            #else
                BswM_NmIf_CarWakeUpIndication(NetworkHandle);
            #endif
       }
    else
    {
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CARWAKEUPINDICATION, NM_E_HANDLE_UNDEF);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
/*****************************************************************************************************
 * Function name: Nm_PduRxIndication
 * Syntax: void Nm_PduRxIndication(
 *                                      const NetworkHandleType NetworkHandle
 *                                     )
 * Description : This is the NM call-back interface to notify that the NM has received a message.
 *               This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_PDU_RX_INDICATION_ENABLED. It in turn notifies the application
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#if (NM_PDU_RX_INDICATION_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_PduRxIndication(NetworkHandleType NetworkHandle)
{
    const Nm_UserCallbackType_tst * UserCallback_pcst;
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    UserCallback_pcst = &Nm_UserCallbacks;

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* call application call-back */
        if(UserCallback_pcst->PduRxIndication != NULL_PTR)
        {
            (*UserCallback_pcst->PduRxIndication)(NetworkHandle);
        }
    }
    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/*****************************************************************************************************
 * Function name: Nm_StateChangeNotification
 * Syntax: void Nm_StateChangeNotification(
 *                                              const NetworkHandleType nmNetworkHandle,
 *                                              const Nm_StateType nmPreviousState,
 *                                              const Nm_StateType nmCurrentState
 *                                             )
 * Description: This is the NM call-back interface to notify state changes. This is called by either CanNm
 *              or FrNm. It in turn notifies the application
 * Parameter1: nmNetworkHandle - Identification of the NM-channel
 * Parameter2: nmPreviousState - previous state of NM
 * Parameter3: nmCurrentState  - current state of NM
 * Return: void
 ****************************************************************************************************/

#if (NM_STATE_CHANGE_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_StateChangeNotification(NetworkHandleType nmNetworkHandle, Nm_StateType nmPreviousState,
                                                                               Nm_StateType nmCurrentState)
{
    const Nm_UserCallbackType_tst * UserCallback_pcst;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    const Nm_ConfigType * ConfDataPtr_pcst;
#endif
    NetworkHandleType Nm_NetworkHandle_u8;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* Since Repeat Message state has many entry points, check has to be done whether the
         * state change call is for entry into Network Mode
         */
        if ((nmCurrentState == NM_STATE_REPEAT_MESSAGE) && (nmPreviousState == NM_STATE_SYNCHRONIZE))
        {
            /* Notify ComM about Network mode */
            ComM_Nm_NetworkMode(nmNetworkHandle);
        }
        else
        {
            /* the transition is either within Network Mode OR it is for non-FrNm node;
             * hence no further notification is made
             */
        }

        UserCallback_pcst = &Nm_UserCallbacks;
        /* Update the application about NM state change */
        if (UserCallback_pcst->StateChangeInd != NULL_PTR)
        {
            (*UserCallback_pcst->StateChangeInd)(nmNetworkHandle, nmPreviousState, nmCurrentState);
        }


        /* Check if NMS report is enabled for the channel */
#if (NM_STATE_REPORT_ENABLED != STD_OFF)

        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];

        if (ConfDataPtr_pcst->NMSSignal != NM_NMS_REPORT_DISABLED)
        {
            uint8 NMSValue_u8;
            NMSValue_u8 = 0xFF;
            if (nmCurrentState == NM_STATE_REPEAT_MESSAGE)
            {
                switch(nmPreviousState)
                {
                    case NM_STATE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from BusSleepMode) */
                        NMSValue_u8 = NM_NMS_RM_BSM;
                        break;
                    }

                    case  NM_STATE_SYNCHRONIZE:
                    {
                          /* NM in state RepeatMessage (transition from Synchronize) */
                         NMSValue_u8 = NM_NMS_RM_BSM;
                         break;
                    }

                    case NM_STATE_PREPARE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from PrepareBusSleepMode) */
                        NMSValue_u8 = NM_NMS_RM_PBSM;
                        break;
                    }

                    case NM_STATE_READY_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from ReadySleep) */
                        NMSValue_u8 = NM_NMS_RM_RS;
                        break;
                    }

                    case NM_STATE_NORMAL_OPERATION:
                    {
                        /* NM in state RepeatMessage (transition from NormalOperation) */
                        NMSValue_u8 = NM_NMS_RM_NO;
                        break;
                    }

                    default:
                        /* Intentionally empty */
                        break;

                }

            }
            else if (nmCurrentState == NM_STATE_NORMAL_OPERATION)
            {
                if (nmPreviousState == NM_STATE_REPEAT_MESSAGE)
                {
                    /* NM in state NormalOperation (transition from RepeatMessage) */
                    NMSValue_u8 = NM_NMS_NO_RM;
                }
                else if (nmPreviousState == NM_STATE_READY_SLEEP)
                {
                    /* NM in state NormalOperation (transition from ReadySleep) */
                    NMSValue_u8 = NM_NMS_NO_RS;

                }
                else
                {
                    /* Intentionally Empty */
                }

            }
            else
            {
                /* Intentionally Empty */
            }
            /* NMS value is within the range defined, hence report the state change */
            if (NMSValue_u8 != 0xFF)
            {
                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void as per Autosar guidelines. */
                (void)Com_SendSignal(ConfDataPtr_pcst->NMSSignal, &NMSValue_u8);
            }
        }
#endif /* #if (NM_STATE_REPORT_ENABLED != STD_OFF) */
    }

    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/*****************************************************************************************************
 * Function name: Nm_RepeatMessageIndication
 * Syntax: void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
 * Description : This is an NM call-back interface to notify that an NM msg is received with Repeat Message
 *               Bit set. This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_REPEAT_MESSAGE_IND_ENABLED. It in turn notifies the application
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#if(NM_REPEAT_MESSAGE_IND_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

/*This feature RepeatMessageIndication is now in channel and this should be taken care while using it in future.*/
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
{
    (void)nmNetworkHandle;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/*****************************************************************************************************
 * Function name: Nm_SychronizationPoint
 * Syntax: void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
 * Description : This is an NM call-back interface to notify the NM Coordinator functionality that this is a suitable
 *               point in time to initiate the coordinated shutdown on.This is called by Cyclic channel eg.FrNm.
 *               This interface is available depending on the configuration parameter NM_COORDINATOR_SUPPORT_ENABLED.
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
{
    // Declare local variable for channels,cluster and conf channnel
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    NetworkHandleType Nm_NetworkHandle_u8;

    Nm_NetworkHandle_u8 = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /*Check if channel is part of any cluster*/
        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];
            MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];

            /*check if the channel is configured for synchronization and whether the channel is active*/
            if((ConfDataPtr_pcst->NmSynchronizingNetwork !=FALSE) && (ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL))
            {
                /*checking the cluster has reached proper state before setting he Syncpoint to reached state*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if((GlobalRamDataPtr_pst->SyncPointState_en == NM_SYNCPOINT_READY) &&  \
                    (MainNetworkRamDataPtr_pst->RemoteSleepStatus_b == TRUE) && \
                    (MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_NORMAL))
                {
                    GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_REACHED;
                }
                SchM_Exit_Nm_ChangeStateNoNest();

            }
        }
    }
    else
    {

        NM_DET_REPORT_ERROR(nmNetworkHandle, NM_SID_SYNCHRONIZATIONPOINT, NM_E_HANDLE_UNDEF);

    }

}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif
