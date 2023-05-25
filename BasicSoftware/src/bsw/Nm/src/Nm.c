
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

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



/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_Init

 * Description:   This is the Initialization function, if EcuM exists, it'll call this interface for
 *                initialization of Nm Interface.
 * Parameter:     ConfigPtr - Pointer to the selected configuration set. \n
 * The Configuration pointer ConfigPtr is currently not used and shall therefore be set NULL_PTR value. \n
 * Return:        void
 *************************************************************************************************************/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_Init( const Nm_ConfigType * ConfigPtr)
{
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    NetworkHandleType ClusterLoopIdx_u8;

    for (ClusterLoopIdx_u8 = 0; ClusterLoopIdx_u8 < NM_NUMBER_OF_CLUSTERS ;ClusterLoopIdx_u8++)
    {
        Nm_Reset(ClusterLoopIdx_u8);
    }

#endif
    (void)ConfigPtr;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_Reset

 * Description:   This is the Initialization function, if EcuM exists, it'll call this interface for
 *                initialization of Nm Interface.
 * Parameter:     None
 * Return:        void
 *************************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
void Nm_Reset(const NetworkHandleType ClusterIdx)
{

    const Nm_ConfigType * ConfDataPtr_pcst;
    Nm_NetworkRamType_tst * MainNetworkRamDataPtr_pst;
    Nm_GlobalRamType_tst * GlobalRamDataPtr_pst;
    NetworkHandleType ChannelLoopIdx_u8;

    GlobalRamDataPtr_pst = &Nm_GlobalData_s[ClusterIdx];

    //Initializze cluster elements
    GlobalRamDataPtr_pst->ClusterState_en      =   NM_CLUSTER_INIT;
    GlobalRamDataPtr_pst->ctComMRequests_u8    =   NM_COUNTER_RESET;
    GlobalRamDataPtr_pst->ctActive_u8          =   NM_COUNTER_RESET;
    GlobalRamDataPtr_pst->ctPassive_u8         =   NM_COUNTER_RESET;
    GlobalRamDataPtr_pst->ctBusSleep_u8        =   NM_COUNTER_RESET;
    GlobalRamDataPtr_pst->InitStatus_b        =   TRUE;
    GlobalRamDataPtr_pst->SyncPointState_en    =   NM_SYNCPOINT_INVALID;

    for (ChannelLoopIdx_u8 = 0; ChannelLoopIdx_u8 < NM_NUMBER_OF_CHANNELS; ChannelLoopIdx_u8++)
    {

        MainNetworkRamDataPtr_pst   = &Nm_ChannelData_s[ChannelLoopIdx_u8];
        ConfDataPtr_pcst             = &Nm_ConfData_cs[ChannelLoopIdx_u8];

        if(ConfDataPtr_pcst->ClusterIdx == ClusterIdx)
        {
            // Initialise all the channel elements
            MainNetworkRamDataPtr_pst->ChannelState_en  = NM_CHANNEL_STATE_INIT;
            MainNetworkRamDataPtr_pst->ShutdownDelayTimeStamp_u32 =NM_COUNTER_RESET;
            MainNetworkRamDataPtr_pst->ComMRequest_b =FALSE;
            MainNetworkRamDataPtr_pst->ChannelMode_en  = NM_CHANNEL_MODE_INIT;
            MainNetworkRamDataPtr_pst->ChannelModeStatus_b  = FALSE;

            if (ConfDataPtr_pcst->NmChannelSleepMaster== TRUE)
            {
                MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = TRUE;
            }
            else
            {
                MainNetworkRamDataPtr_pst->RemoteSleepStatus_b = FALSE;
            }
        }

    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

