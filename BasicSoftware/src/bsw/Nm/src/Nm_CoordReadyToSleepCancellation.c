

 /* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"



#if (NM_COORDINATOR_SYNC_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

void Nm_CoordReadyToSleepCancellation(NetworkHandleType NetworkHandle)
{
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    NetworkHandleType Nm_NetworkHandle_u8;

    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /*Check if channel is part of any cluster*/
        if((ConfDataPtr_pcst->ClusterIdx != 0xFF))
        {

            GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];
            MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];
            /*check if the channel is passive configured*/
            if((ConfDataPtr_pcst->NmChannelType == NM_PASSIVE_CHANNEL) && (ConfDataPtr_pcst->NmChannelSleepMaster == FALSE))
            {
                /*Increament the passive counter as there is ready to sleep cancellation*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if ((MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_WAIT_SLEEP) && (MainNetworkRamDataPtr_pst->RemoteSleepStatus_b ==TRUE))
                {
                    GlobalRamDataPtr_pst->ctPassive_u8++;
                    MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = FALSE;
                }
                SchM_Exit_Nm_ChangeStateNoNest();
            }
        }
    }
    else
    {
#if (NM_AR45_VERSION != STD_OFF) 
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_COORDREADYTOSLEEPCANCELLATION, NM_E_INVALID_CHANNEL);
#else
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_COORDREADYTOSLEEPCANCELLATION, NM_E_HANDLE_UNDEF);
#endif	
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
