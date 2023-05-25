

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"


/***************************************************************************************************
 Function name    : ComM_Nm_PrepareBusSleepMode
 Syntax           : void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
 Description      : Notification that the network management has entered Prepare Bus-Sleep Mode.
 Parameter        : Channel -> Id of the channel
 Return value     : void
 ***************************************************************************************************/
void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
    /* Local Variables Declaration */
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    /* Local variable initialization */
    channelRamPtr_pst = &ComM_ChannelStruct[Channel];
    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel]);
    /*****************************************DET START************************************************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* DET check for communication manager initialization */
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_PREPARE_BUSSLEEPMODE,COMM_E_NOT_INITED);
        return;
    }
    /* Check if the channel is present in the configured channels */
    if (Channel >= COMM_NO_OF_CHANNELS)
    {
        /* Report DET with Invalid channel parameter*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_PREPARE_BUSSLEEPMODE,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /*********************************************DET END************************************************/

    /* Process prepare bus sleep indication only if Channel's NmVariant is FULL or PASSIVE */
    /* Bus sleep indication is considered only in COMM_FULL_COM_READY_SLEEP state */

    if ((channelConfigPtr_pcst->ComMNmVariant_en==PASSIVE)||(channelConfigPtr_pcst->ComMNmVariant_en==FULL))
    {
        SchM_Enter_ComM_ChannelNoNest();

        if(channelRamPtr_pst->ChannelState_e == COMM_FULL_COM_READY_SLEEP)
        {
            channelRamPtr_pst -> NmPrepareBusSleepIndicationStatus_b = TRUE;
        }

        SchM_Exit_ComM_ChannelNoNest();
    }

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

