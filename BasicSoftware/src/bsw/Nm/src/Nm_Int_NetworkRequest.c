

#include "Nm_Priv.h"

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
LOCAL_INLINE void Nm_Prv_ComMRequestUpdate(boolean * ComMRequestPtr_pb ,
                                                            uint8 * ctComMRequestsPtr_pu8,
                                                            boolean active_b);


Std_ReturnType Nm_Prv_NetworkRequest(boolean active_b, NetworkHandleType NetworkHandle_u8 , Nm_NetworkRequestSource_ten source_en)
{
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    const Nm_BusNmApiType_tst * FuncPtr_pcst;
    NetworkHandleType Nm_NetworkHandle_u8;
    Std_ReturnType RetVal_u8;


    Nm_NetworkHandle_u8 =NM_GET_HANDLE(NetworkHandle_u8);

    ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
    MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];
    GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];
    FuncPtr_pcst=&Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

    RetVal_u8 = E_NOT_OK;

    switch(MainNetworkRamDataPtr_pst->ChannelState_en)
    {
        case NM_CHANNEL_STATE_INIT:
        {
            if(ConfDataPtr_pcst->NmSynchronizingNetwork != FALSE)
            {
                GlobalRamDataPtr_pst->SyncPointState_en = NM_SYNCPOINT_INIT;
            }
            //check the source for network request
            if(source_en == NM_EXTERNAL_REQUEST)
            {
                MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_NORMAL;
                RetVal_u8=(*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle_u8);

                if(ConfDataPtr_pcst->NmChannelSleepMaster == FALSE)
                {

                    MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = FALSE;

                    //active channel
                    if(ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL)
                    {
                        SchM_Enter_Nm_ChangeStateNoNest();
                        GlobalRamDataPtr_pst->ctActive_u8++;
                        SchM_Exit_Nm_ChangeStateNoNest();
                    }
                    else //Passive channel
                    {
                        SchM_Enter_Nm_ChangeStateNoNest();
                        GlobalRamDataPtr_pst->ctPassive_u8++;
                        SchM_Exit_Nm_ChangeStateNoNest();
                    }
                }

                Nm_Prv_ComMRequestUpdate(&(MainNetworkRamDataPtr_pst->ComMRequest_b),&(GlobalRamDataPtr_pst->ctComMRequests_u8),active_b);
            }
            else
            {
                RetVal_u8 = E_OK;
                // ComM_Nm_NetworkRestartIndication();
            }
            break;

        }

        case NM_CHANNEL_STATE_NORMAL:
        {
            if(source_en == NM_EXTERNAL_REQUEST)
            {
                Nm_Prv_ComMRequestUpdate(&(MainNetworkRamDataPtr_pst->ComMRequest_b),&(GlobalRamDataPtr_pst->ctComMRequests_u8),active_b);
                RetVal_u8 = E_OK;
            }
            break;
        }

        case NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP:
        {
            /*comes here in Abort shutdown case which is not handled in current design*/
            if(source_en == NM_EXTERNAL_REQUEST)
            {
                Nm_Prv_ComMRequestUpdate(&(MainNetworkRamDataPtr_pst->ComMRequest_b),&(GlobalRamDataPtr_pst->ctComMRequests_u8),active_b);
                MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_NORMAL;
                RetVal_u8 = E_OK;
            }
            break;
        }
        case NM_CHANNEL_STATE_WAIT_SLEEP:
        {
            /*comes here in Abort shutdown case which is not handled in current design*/
            if(source_en == NM_EXTERNAL_REQUEST)
            {
                Nm_Prv_ComMRequestUpdate(&(MainNetworkRamDataPtr_pst->ComMRequest_b),&(GlobalRamDataPtr_pst->ctComMRequests_u8),active_b);
                MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_NORMAL;
                RetVal_u8 = E_OK;
            }
            else
            {
                if(ConfDataPtr_pcst->NmChannelType == NM_PASSIVE_CHANNEL)
                {
                    MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_NORMAL;
                    RetVal_u8=(*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle_u8);

                    if(ConfDataPtr_pcst->NmChannelSleepMaster == FALSE)
                    {
                        MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = FALSE;
                    }

                }
                else
                {
                    RetVal_u8 = E_OK;
                }
            }
            break;
        }
        case NM_CHANNEL_STATE_BUS_SLEEP:
        {
            if(source_en == NM_EXTERNAL_REQUEST)
            {
                MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_NORMAL;
                RetVal_u8=(*FuncPtr_pcst->BusNm_NetworkRequest_pfct)(NetworkHandle_u8);

                Nm_Prv_ComMRequestUpdate(&(MainNetworkRamDataPtr_pst->ComMRequest_b),&(GlobalRamDataPtr_pst->ctComMRequests_u8),active_b);

                if(GlobalRamDataPtr_pst->ClusterState_en != NM_CLUSTER_PASSIVE)
                {
                    if(ConfDataPtr_pcst->NmChannelSleepMaster == FALSE)
                    {
                        MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = FALSE;
                        //active channel
                        if(ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL)
                        {
                            /*increament active counter as active channel is request for the channel*/
                            SchM_Enter_Nm_ChangeStateNoNest();
                            GlobalRamDataPtr_pst->ctActive_u8++;
                            SchM_Exit_Nm_ChangeStateNoNest();
                        }
                        else //Passive channel
                        {
                            /*increament passive counter as passive channel is request for the channel*/
                            SchM_Enter_Nm_ChangeStateNoNest();
                            GlobalRamDataPtr_pst->ctPassive_u8++;
                            SchM_Exit_Nm_ChangeStateNoNest();
                        }
                    }//mastersleep
                }//not in passive
            }
            else
            {
                RetVal_u8 = E_OK;
                //ComM_Nm_RestartIndication();
            }
            break;
        }

        case NM_CHANNEL_STATE_UNINIT:
        default:
        {
            /* Intentionally Empty */
            break;
        }
    }
    return RetVal_u8;
}

#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif

/***************************************************************************************************
 * Function name    : Nm_Prv_ComMRequestUpdate
 * Syntax           : LOCAL_INLINE void Nm_Prv_ComMRequestUpdate(boolean * ComMRequestPtr ,
                                                            uint8 * ctComMRequestsPtr,
                                                            boolean active)
 * Description      : Updating the ComM request to the counter.
 * Parameter        : ComMRequestPtr_pb -> status flag for channel ipdates as per the request from network request or passive startup
                      ctComMRequestsPtr_pu8 ->counter to keep track for number of ComM request per cluster
                      active_b -> variable to distinguish the request came from network request or passive startup
 * Return value     : void
 ***************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

LOCAL_INLINE void Nm_Prv_ComMRequestUpdate(boolean * ComMRequestPtr_pb ,
                                                            uint8 * ctComMRequestsPtr_pu8,
                                                            boolean active_b)
{
    /*Increament ComM counter to indicate there is an active_b request on a channel in the cluster*/
    SchM_Enter_Nm_ChangeStateNoNest();
    if((active_b == TRUE) && ((*ComMRequestPtr_pb) == FALSE))
    {
        (*ComMRequestPtr_pb) = TRUE;
        (*ctComMRequestsPtr_pu8)++;
    }
    SchM_Exit_Nm_ChangeStateNoNest();
}


#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif

