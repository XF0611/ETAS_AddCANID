


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

/*******************************************************************************
 * Function name    : ComM_EcuM_WakeUpIndication
 * Syntax           : void ComM_EcuM_WakeUpIndication( NetworkHandleType Channel )
 * Description      : Notification of a wake up on the corresponding channel.
 * Parameters       : Channel
 * Return value     : void
 ******************************************************************************/
void ComM_EcuM_WakeUpIndication(NetworkHandleType Channel)
{

    /* Local variables declaration */
    #if (COMM_SYNCHRONOUS_WAKE_UP == STD_OFF )
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)
    ComM_ChannelVarType_tst *       masterChannelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    uint8                           masterChannelId_u8;
    #endif  /*  #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  */
    #endif

    /*****************************************DET START************************************************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* Check if ComM is initialized */
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_ECUM_WAKEUP_INDICATION,COMM_E_NOT_INITED);
        return;
    }
    /* Check if the channel is present in the configured channels */
    if (Channel >= COMM_NO_OF_CHANNELS)
    {
        /* Report DET if the channel is not valid*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_ECUM_WAKEUP_INDICATION,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /*********************************************DET END************************************************/

    /* If COMM_SYNCHRONOUS_WAKE_UP is enabled,
    change state of all channels to Wait For RunMode State */
    #if (COMM_SYNCHRONOUS_WAKE_UP != STD_OFF )

    /*  As synchronous wake up is enabled, all channels needs to be waken up*/
    /*  Wake up detected on a particular channel should wake up all channels */
    ComM_Prv_SyncWakeUpIndication();

    #else  /* COMM_SYNCHRONOUS_WAKE_UP disabled */

    /* Wake up only the requested channel */
    channelRamPtr_pst = &ComM_ChannelStruct[Channel];

    SchM_Enter_ComM_ChannelNoNest();
    /* Wake up indication is processed only in COMM_NO_COMMUNICATION mode, in other modes it is ignored */
    if(channelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION)
    {
        #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)
        channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel]);
        if(channelConfigPtr_pcst->ComMMasterChannelPresent_b == TRUE)
        {
            /* Managing Channel Id is copied to a local buffer */
            masterChannelId_u8 = channelConfigPtr_pcst->masterChannelId_u8;
            masterChannelRamPtr_pst = &ComM_ChannelStruct[masterChannelId_u8];
            /* Wake up indication is processed only in COMM_NO_COMMUNICATION mode, in other modes it is ignored */
            /* MR12 RULE 14.3 VIOLATION:'if' controlling expression is a constant expression and its value is 'true'.
             * This expression is not true always. This expression value gets updated in the channel state machine for every main function.
             * Hence it is an incorrect warning reported by Misra */
            if(masterChannelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION)
            {
                COMM_SET_BIT((masterChannelRamPtr_pst ->PassiveRequestState_u8 ),(0x01));
            }
        }
        #endif  /*  #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  */

        COMM_SET_BIT((channelRamPtr_pst ->PassiveRequestState_u8 ),(0x01));
    }
    SchM_Exit_ComM_ChannelNoNest();

    #endif  /*  (COMM_SYNCHRONOUS_WAKE_UP != STD_OFF )  */

    (void)(Channel);

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

