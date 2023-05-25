


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

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_RemoteSleepIndication

 * Description:   This is the NM call-back interface to indicate detection of Remote Sleep status,
 *                this is called by either CanNm or FrNm, it in turn notifies user application.
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/

#if (NM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_RemoteSleepIndication(NetworkHandleType NetworkHandle)
{
    const Nm_UserCallbackType_tst * UserCallback_pcst;     /* Holds user callback pointers */
    /*#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
     * declare varibale for channle ,cluster
     * #endif
     * */
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
#endif

    NetworkHandleType Nm_NetworkHandle_u8;                       /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle_u8 = NM_GET_HANDLE(NetworkHandle);
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle_u8 < NM_NUMBER_OF_CHANNELS)
    {
        /* Gets User callback data and holds in UserCallback_pcst */
        UserCallback_pcst = &Nm_UserCallbacks;
        /* Indicate Remote Sleep situation to the application */
        if (UserCallback_pcst->RemoteSleepIndication != NULL_PTR)
        {
            /* Call-back functions called by Nm, defined by User application */
            (*UserCallback_pcst->RemoteSleepIndication)(NetworkHandle);
        }
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
        ConfDataPtr_pcst = &Nm_ConfData_cs[Nm_NetworkHandle_u8];
        /*Check if channel is part of any cluster*/
        if((ConfDataPtr_pcst->ClusterIdx != 0xFF))
        {

            MainNetworkRamDataPtr_pst = &Nm_ChannelData_s[Nm_NetworkHandle_u8];
            GlobalRamDataPtr_pst = &Nm_GlobalData_s[ConfDataPtr_pcst->ClusterIdx];


            if((ConfDataPtr_pcst->NmChannelType == NM_ACTIVE_CHANNEL) && (ConfDataPtr_pcst->NmChannelSleepMaster == FALSE))
            {
                /*Decreament the active counter and set remotesleep status to true as remotessleep indication is received on active channel*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if((MainNetworkRamDataPtr_pst->ChannelState_en == NM_CHANNEL_STATE_NORMAL) && (MainNetworkRamDataPtr_pst->RemoteSleepStatus_b == FALSE))
                {
                    GlobalRamDataPtr_pst->ctActive_u8--;
                    MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = TRUE;
                }
                SchM_Exit_Nm_ChangeStateNoNest();
            }
        }
        #endif
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

