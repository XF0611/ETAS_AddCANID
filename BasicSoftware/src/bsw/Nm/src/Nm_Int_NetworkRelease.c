

#include "Nm_Priv.h"


#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

Std_ReturnType Nm_Prv_NetworkRelease(NetworkHandleType NetworkHandle_u16 , Nm_NetworkRequestSource_ten source_en)
{
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    const Nm_BusNmApiType_tst * FuncPtr_pcst;
    NetworkHandleType Nm_NetworkHandle_u8;
    Std_ReturnType RetVal_u8;

    Nm_NetworkHandle_u8 =NM_GET_HANDLE(NetworkHandle_u16);

    RetVal_u8 = E_NOT_OK;
    ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
    MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];
    GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];
    FuncPtr_pcst = &Nm_BusNmApi[ConfDataPtr_pcst->BusNmType];

    if (source_en == NM_EXTERNAL_REQUEST)
    {
        /*decreament ComM counter as channel is been release from ComM*/
        SchM_Enter_Nm_ChangeStateNoNest();
        if(MainNetworkRamDataPtr_pst->ComMRequest_b == TRUE)
        {
            GlobalRamDataPtr_pst->ctComMRequests_u8--;
            MainNetworkRamDataPtr_pst->ComMRequest_b = FALSE;
        }
        SchM_Exit_Nm_ChangeStateNoNest();
        RetVal_u8 = E_OK;
    }
    else
    {
        switch(MainNetworkRamDataPtr_pst->ChannelState_en)
        {
            case NM_CHANNEL_STATE_NORMAL:
            {
                if (ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL)
                {
                    MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP;
                    /*Start the shutdown delay timer for the specific channel*/
                    MainNetworkRamDataPtr_pst->ShutdownDelayTimeStamp_u32 = SwFRTimer;
                    /*set sleep ready bit to 1 to indicate other ecu in the channel to indicate that the active node is ready to sleep*/
                    RetVal_u8 = (*FuncPtr_pcst->BusNm_SetSleepReadyBit_pfct)(NetworkHandle_u16,NM_SETBIT);
                }
                else
                {
                    MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_WAIT_SLEEP;
                    /*Call Bus Specific Network Release*/
                    RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRelease_pfct)(NetworkHandle_u16);

                }
                break;
            }

            case NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP:
            {
                //always active channnel would be called
                MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_WAIT_SLEEP;
                /*Call Bus Specific Network Release*/
                RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRelease_pfct)(NetworkHandle_u16);

                break;
            }

            case NM_CHANNEL_STATE_INIT:
            case NM_CHANNEL_STATE_WAIT_SLEEP:
            case NM_CHANNEL_STATE_BUS_SLEEP:
            case NM_CHANNEL_STATE_UNINIT:
            default:
            {
                /* Intentionally Empty */
                break;
            }
        }
        if((MainNetworkRamDataPtr_pst->ChannelModeStatus_b == TRUE) && \
            (MainNetworkRamDataPtr_pst->ChannelState_en != NM_CHANNEL_STATE_WAIT_SLEEP) && \
            (MainNetworkRamDataPtr_pst->ChannelState_en != NM_CHANNEL_STATE_INIT))
        {
            MainNetworkRamDataPtr_pst->ChannelState_en = NM_CHANNEL_STATE_WAIT_SLEEP;
            //Call Bus Specific Network Release
            RetVal_u8 = (*FuncPtr_pcst->BusNm_NetworkRelease_pfct)(NetworkHandle_u16);
        }
    }
    return RetVal_u8;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
