
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

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*****************************************************************************************************
 * Function
 *****************************************************************************************************
 *****************************************************************************************************
 * Function name: Nm_SynchronizeMode
 * Syntax: void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle)
 * Description : Notification that the network management has entered Synchronize Mode of FrNm.
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle)
{
    NetworkHandleType Nm_NetworkHandle_u8;  /* Network handle is received from ComM */
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
     // Declare local variable for channels,cluster and conf channnel
    /**** End of declarations ****/
#endif
    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(nmNetworkHandle);
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        if((ConfDataPtr_pcst->ClusterIdx != 0xFF))
        {
            MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];
            MainNetworkRamDataPtr_pst->ChannelMode_en = NM_CHANNEL_MODE_SYNCHRONIZE;
        }
#endif
    }
    else
    {
#if (NM_AR45_VERSION != STD_OFF) 
        NM_DET_REPORT_ERROR(nmNetworkHandle, NM_SID_SYNCHRONIZEMODE, NM_E_INVALID_CHANNEL);
#else
        NM_DET_REPORT_ERROR(nmNetworkHandle, NM_SID_SYNCHRONIZEMODE, NM_E_HANDLE_UNDEF);
#endif			
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
