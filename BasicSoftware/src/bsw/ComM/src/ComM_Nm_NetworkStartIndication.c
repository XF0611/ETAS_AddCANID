


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
 Function name    : ComM_Nm_NetworkStartIndication
 Syntax           : void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
 Description      : Indication that a NM-message has been received in the Bus Sleep Mode
 Parameter        : Channel -> Id of the channel
 Return value     : void
 ***************************************************************************************************/
void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
    /* Local Variables Declaration */
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    /* Local variable initialization */
    channelRamPtr_pst = &ComM_ChannelStruct[Channel];
    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel]);

    /**********************************************DET START****************************************************/
    /*Check if ComM is initialized */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_NETWORKSTART_INDICATION,COMM_E_NOT_INITED);
        return;
    }

    /* Check if the channel is present in the configured channels */
    if (Channel >= COMM_NO_OF_CHANNELS)
    {
        /* Report DET with Invalid channel parameter*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NM_NETWORKSTART_INDICATION,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /************************************************DET END*****************************************************/

    /* Check if the Channel's NmVariant is FULL or PASSIVE */
    /* Network start indication needs to be considered only in No_Com mode*/
    if ((channelConfigPtr_pcst->ComMNmVariant_en == FULL)||(channelConfigPtr_pcst->ComMNmVariant_en == PASSIVE))
    {
        SchM_Enter_ComM_ChannelNoNest();

        if( channelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION )
        {
            COMM_SET_BIT((channelRamPtr_pst -> PassiveRequestState_u8),(0x04));
        }

        SchM_Exit_ComM_ChannelNoNest();
    }
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


