

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
 Function name    : ComM_Nm_BusSleepMode
 Syntax           : void ComM_Nm_BusSleepMode(NetworkHandleType Channel)
 Description      : Notification that the network management has entered Bus-Sleep Mode.
 Parameter        : Channel -> Channel Id
 Return value     : void
 ***************************************************************************************************/
void ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
    /* Local Variables Declaration */
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;

    channelRamPtr_pst = &ComM_ChannelStruct[Channel];
    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel]);
    /* Local Variables Declaration */
    /*****************************************DET START************************************************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* Check if ComM is initialized */
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_BUS_SLEEPMODE,COMM_E_NOT_INITED);
        return;
    }
    /* Check if the channel is present in the configured channels */
    if (Channel >= COMM_NO_OF_CHANNELS)
    {
        /* Report DET with Invalid channel parameter*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_BUS_SLEEPMODE,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /*********************************************DET END************************************************/

    /* Check if the channel's Nm variant is FULL or PASSIVE */
    /* Bus sleep indication is processed only if the channel is in COMM_FULL_COMMUNICATION or COMM_SILENT_COMMUNICATION
     * mode.In other modes, it is ignored*/

    if ((channelConfigPtr_pcst->ComMNmVariant_en==FULL)||(channelConfigPtr_pcst->ComMNmVariant_en==PASSIVE))
    {
        SchM_Enter_ComM_ChannelNoNest();

        if((channelRamPtr_pst->ChannelMode_u8 == COMM_FULL_COMMUNICATION)|| (channelRamPtr_pst->ChannelMode_u8 == COMM_SILENT_COMMUNICATION))
        {
            channelRamPtr_pst -> NmBusSleepIndicationStatus_b = TRUE;
        }

        SchM_Exit_ComM_ChannelNoNest();
    }


}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

