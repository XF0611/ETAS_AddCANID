
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"


/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_MainFunction
 * Syntax :       void Nm_MainFunction( void )
 * Description:   This is the main function of NM Coordinator, this handles all channels that are being
 *                coordinated for shutdown.
 * Parameter:     void
 * Return:        void
 *************************************************************************************************************/
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static void Nm_ComputeSwFRTimer(const uint32 MainFunctionCycleTime);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_MainFunction(void)
{

    const Nm_ConfigType * ConfDataPtr_pcst;
#if (NM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF)
    const Nm_BusNmApiType_tst * FuncPtr_pcst;
#endif
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;


    Std_ReturnType RetVal_u8;
    NetworkHandleType NetworkHandle_u8;
    uint8 ClusterLoopIdx_u8;
    uint8 ChannelLoopIdx_u8;
    boolean stateChangePossible_b;
    static Nm_ClusterStateType_ten PrevClusterState_en;

    /*Computation of Nm timer*/
    Nm_ComputeSwFRTimer(NM_MAIN_FUNCTION_CYCLETIME);

    //loop through every cluster
    for(ClusterLoopIdx_u8=0;ClusterLoopIdx_u8 < NM_NUMBER_OF_CLUSTERS;ClusterLoopIdx_u8++)
    {
        GlobalRamDataPtr_pst = &Nm_GlobalData_s[ClusterLoopIdx_u8];

        if(GlobalRamDataPtr_pst->InitStatus_b != TRUE)
        {
            /********************************* Start: DET *************************************/
            /* Report DET if NM is Uninitialized */
            NM_DET_REPORT_ERROR(ClusterLoopIdx_u8, NM_SID_MAINFUNCTION,NM_E_UNINIT);

            /*********************************  End: DET  *************************************/
        }

        stateChangePossible_b = FALSE;

        do{
            switch(GlobalRamDataPtr_pst->ClusterState_en)
            {
                case NM_CLUSTER_INIT:
                {
                    stateChangePossible_b = FALSE;

                    /*check if there is any network request or passive request*/
                    if((GlobalRamDataPtr_pst->ctActive_u8 > 0) || (GlobalRamDataPtr_pst->ctPassive_u8 > 0) || (GlobalRamDataPtr_pst->ctComMRequests_u8 >0))
                    {
                        PrevClusterState_en = GlobalRamDataPtr_pst->ClusterState_en;
                        GlobalRamDataPtr_pst->ClusterState_en=NM_CLUSTER_ACTIVE_ACTIVE;
                        stateChangePossible_b = TRUE;
                    }
                    break;
                }//case NM_CLUSTER_INIT

                case NM_CLUSTER_ACTIVE_ACTIVE:
                {
                    if(stateChangePossible_b == TRUE)
                    {
                        stateChangePossible_b=FALSE;
                        if(PrevClusterState_en == NM_CLUSTER_INIT)
                        {
                            if((GlobalRamDataPtr_pst->ctActive_u8 == 0) && (GlobalRamDataPtr_pst->ctComMRequests_u8 == 0))
                            {
                                PrevClusterState_en = GlobalRamDataPtr_pst->ClusterState_en;
                                GlobalRamDataPtr_pst->ClusterState_en = NM_CLUSTER_ACTIVE_PASSIVE;
                                stateChangePossible_b = TRUE;
                            }
                        }
                        else if(PrevClusterState_en == NM_CLUSTER_ACTIVE_PASSIVE)
                        {
                            for(ChannelLoopIdx_u8 = 0 ;ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS; ChannelLoopIdx_u8++)
                            {
                                ConfDataPtr_pcst = &Nm_ConfData_cs[ChannelLoopIdx_u8];
                                NetworkHandle_u8 = ConfDataPtr_pcst->comMHandle;
                                if((ConfDataPtr_pcst->NmChannelType == NM_PASSIVE_CHANNEL) && (ConfDataPtr_pcst->ClusterIdx == ClusterLoopIdx_u8))
                                {
                                    /*call internal network request to request bus specific network request and handle the states accordingly*/
                                    RetVal_u8 = Nm_Prv_NetworkRequest(FALSE , NetworkHandle_u8 , NM_INTERNAL_REQUEST);
                                }
                            }
                        }
                        /*Previous state of cluster  is Passive*/
                        else
                        {
                            //To be handled in abort shutdown case.
                        }

                    }
                    else
                    {
                        /*Check if only passive channel is requested and active request for any channel form ComM within cluster */
                        if((GlobalRamDataPtr_pst->ctActive_u8 == 0) && (GlobalRamDataPtr_pst->ctComMRequests_u8 == 0))
                        {
                            PrevClusterState_en = GlobalRamDataPtr_pst->ClusterState_en;
                            GlobalRamDataPtr_pst->ClusterState_en = NM_CLUSTER_ACTIVE_PASSIVE;
                            stateChangePossible_b = TRUE;
                        }
                    }
                    break;
                }//case NM_CLUSTER_ACTIVE_ACTIVE

                case NM_CLUSTER_ACTIVE_PASSIVE:
                {
                    if(stateChangePossible_b == TRUE)
                    {
                        stateChangePossible_b = FALSE;
                        for(ChannelLoopIdx_u8 = 0;ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS;ChannelLoopIdx_u8++)
                        {
                            ConfDataPtr_pcst = &Nm_ConfData_cs[ChannelLoopIdx_u8];
                            NetworkHandle_u8 = ConfDataPtr_pcst->comMHandle;
                            MainNetworkRamDataPtr_pst=&Nm_ChannelData_s[ChannelLoopIdx_u8];
                            if((ConfDataPtr_pcst->NmChannelType == NM_PASSIVE_CHANNEL)&&(ConfDataPtr_pcst->ClusterIdx == ClusterLoopIdx_u8) \
                                    &&(MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_NORMAL))
                            {
                                /*call bus specific release for passive channel and handle channel state accordingly*/
                                RetVal_u8 = Nm_Prv_NetworkRelease(NetworkHandle_u8,NM_INTERNAL_REQUEST);
                            }
                        }
                    }
                    else
                    {
                        if((GlobalRamDataPtr_pst->ctActive_u8 > 0) || (GlobalRamDataPtr_pst->ctComMRequests_u8 > 0))
                        {
                            PrevClusterState_en = GlobalRamDataPtr_pst->ClusterState_en;
                            GlobalRamDataPtr_pst->ClusterState_en = NM_CLUSTER_ACTIVE_ACTIVE;
                            stateChangePossible_b = TRUE;
                        }
                        else if(GlobalRamDataPtr_pst->ctPassive_u8 == 0)
                        {
                            PrevClusterState_en = GlobalRamDataPtr_pst->ClusterState_en;
                            GlobalRamDataPtr_pst->ClusterState_en = NM_CLUSTER_PASSIVE;
                            stateChangePossible_b = TRUE;
                        }
                        else if(GlobalRamDataPtr_pst->ctComMRequests_u8 == 0)
                        {
                            for(ChannelLoopIdx_u8 = 0;ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS;ChannelLoopIdx_u8++)
                            {
                                ConfDataPtr_pcst = &Nm_ConfData_cs[ChannelLoopIdx_u8];
                                NetworkHandle_u8 = ConfDataPtr_pcst->comMHandle;
                                MainNetworkRamDataPtr_pst=&Nm_ChannelData_s[ChannelLoopIdx_u8];

                                if(((MainNetworkRamDataPtr_pst->ChannelMode_en == NM_CHANNEL_MODE_BUS_SLEEP) \
                                     || (MainNetworkRamDataPtr_pst->ChannelMode_en == NM_CHANNEL_MODE_PREPARE_BUS_SLEEP) \
                                     || (MainNetworkRamDataPtr_pst->ChannelMode_en == NM_CHANNEL_MODE_SYNCHRONIZE)) \
                                     && (ConfDataPtr_pcst->ClusterIdx == ClusterLoopIdx_u8))
                                {
                                    MainNetworkRamDataPtr_pst->ChannelModeStatus_b = TRUE;
                                    //Call Bus Specific Network Release
                                    RetVal_u8 = Nm_Prv_NetworkRelease(NetworkHandle_u8,NM_INTERNAL_REQUEST);
                                }
                            }
                        }
                        else
                        {
                            /* Do nothing*/
                        }
                    }

                    break;
                }//case NM_CLUSTER_ACTIVE_PASSIVE

                case NM_CLUSTER_PASSIVE:
                {
                    if(stateChangePossible_b == TRUE)
                    {
                        stateChangePossible_b=FALSE;
                        GlobalRamDataPtr_pst->ctBusSleep_u8 = 0;
                        /*Syncpoint would be initialized if Nm_SynchronizingNetwork is enabled */
                        /*Check whether Syncpoint is initialized*/
                        if(GlobalRamDataPtr_pst->SyncPointState_en == NM_SYNCPOINT_INIT)
                        {
                            GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_READY;
                        }
                        else
                        {
                            GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_REACHED;
                        }


                    }//if(stateChangePossible_b)
                    else
                    {
                        if(GlobalRamDataPtr_pst->SyncPointState_en == NM_SYNCPOINT_REACHED)
                        {
                           GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_INVALID;
                           /*loop through every active channel to initiate shutdown procedure*/
                           for(ChannelLoopIdx_u8 = 0 ; ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS ; ChannelLoopIdx_u8++)
                           {
                               ConfDataPtr_pcst=&Nm_ConfData_cs[ChannelLoopIdx_u8];
                               NetworkHandle_u8 = ConfDataPtr_pcst->comMHandle;
                               MainNetworkRamDataPtr_pst=&Nm_ChannelData_s[ChannelLoopIdx_u8];

                               if((ConfDataPtr_pcst->ClusterIdx == ClusterLoopIdx_u8))
                               {
                                   if((ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL) && (MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_NORMAL))
                                   {
                                       RetVal_u8 = Nm_Prv_NetworkRelease(NetworkHandle_u8,NM_INTERNAL_REQUEST);
                                   }

                                   if((MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP) || \
                                           (MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_WAIT_SLEEP))
                                   {
                                       /*Increament BusSleep counter based on number of channel within a cluster are expected to go to bus sleep*/
                                       SchM_Enter_Nm_ChangeStateNoNest();
                                       GlobalRamDataPtr_pst->ctBusSleep_u8++;
                                       SchM_Exit_Nm_ChangeStateNoNest();
                                   }
                               }// if(part of cluster)
                           }//for loop
                        }//if(syncpointreached)

                        /*check all the bus have went to sleep*/
                        if((GlobalRamDataPtr_pst->SyncPointState_en == NM_SYNCPOINT_INVALID) && (GlobalRamDataPtr_pst->ctBusSleep_u8 == 0))
                        {
                            /*reset all the channels within the cluster*/
                            Nm_Reset(ClusterLoopIdx_u8);
                            stateChangePossible_b = TRUE;
                        }
                    }//else
                    break;
                }//case NM_CLUSTER_PASSIVE
                default:
                    /* Intentionally Empty */
                    break;
            }//switch

        }while(stateChangePossible_b == TRUE);

    }// for loop through clusters


/* Checking each channel whether shutdown delay timer has expired or not*/
    for(ChannelLoopIdx_u8 = 0; ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS ; ChannelLoopIdx_u8++)
    {
        ConfDataPtr_pcst = &Nm_ConfData_cs[ChannelLoopIdx_u8];
        MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[ChannelLoopIdx_u8];

        if(ConfDataPtr_pcst->ClusterIdx != 0xFF)
        {
            if(MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP)
            {
                /*Wait for Shutdown timer to expire*/
                if(Nm_TimerExpired(MainNetworkRamDataPtr_pst->ShutdownDelayTimeStamp_u32,ConfDataPtr_pcst->ShutdownDelay))
                {
                   NetworkHandle_u8 = ConfDataPtr_pcst->comMHandle;
#if (NM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF)
                   /*Call busSynchronization to send the Nm message*/
                   FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];
                   RetVal_u8 = (*FuncPtr_pcst->BusNm_RequestBusSynchronization_pfct)(NetworkHandle_u8);
#endif
                   /*call Bus Specific Network release and handle the channel state accordingly*/
                   RetVal_u8 = Nm_Prv_NetworkRelease(NetworkHandle_u8,NM_INTERNAL_REQUEST);
                }
            }
        }
    }//for loop channels
    (void)RetVal_u8;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/*****************************************************************************************************
 * Function name: Nm_ComputeSwFRTimer
 * Description: This function calculate the Free running timer for NM Coordinator
 * Parameter: MainFunctionCycleTime - period of MainFunction calls
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static void Nm_ComputeSwFRTimer(const uint32 MainFunctionCycleTime)
{
    SwFRTimer += MainFunctionCycleTime;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif /* #if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF) */

