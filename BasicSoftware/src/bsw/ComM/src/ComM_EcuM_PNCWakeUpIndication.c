


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
void ComM_EcuM_PNCWakeUpIndication(PNCHandleType PNCid)
{
    #if (COMM_PNC_ENABLED != STD_OFF)
    /* Local variables declaration */

    ComM_PncRamStructType_tst *              pncRamPtr_pst;
    ComM_ChannelVarType_tst *                channelRamPtr_pst;
    #if (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )
    const ComM_PncConfigStruct_tst *        pncConfigPtr_pcst;
    uint8                                   numChannels;
    #endif  /*  (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )  */
    uint8                                loopCtr_u8;
    /*****************************************DET START************************************************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* Check if ComM is initialized */
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_ECUM_PNCWAKEUP_INDICATION,COMM_E_NOT_INITED);
        return;
    }
    /* Check if the channel is present in the configured channels */
    if (PNCid >= COMM_NO_OF_PNCS)
    {
        /* Report DET if the channel is not valid*/

        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_ECUM_PNCWAKEUP_INDICATION,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /*********************************************DET END************************************************/

    pncRamPtr_pst = &ComM_PncRamStruct[PNCid];
    #if (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )
    pncConfigPtr_pcst = &(COMM_GET_PNC_LIST[PNCid]);
    numChannels = pncConfigPtr_pcst->NumChannels_u8;
    #endif  /*  (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )  */

    /* Check if PNC is enabled, done for Postbuild as PN can later be dis/enabled postbuild time */
    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {
        /* If PNC is in state NO_COMMUNICATION, set WakeUpStatus_b flag */
        SchM_Enter_ComM_PNCWakeUpNoNest();
        if(pncRamPtr_pst->PncState_en == COMM_PNC_NO_COMMUNICATION)
        {
            pncRamPtr_pst->WakeUpStatus_b = TRUE;
        }
        SchM_Exit_ComM_PNCWakeUpNoNest();


        SchM_Enter_ComM_ChannelNoNest();
        /* Lock required for consistent update of PassiveRequestState_u8 in API's and main function along with channel state */
        #if (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )

        for(loopCtr_u8 = C_ZERO; loopCtr_u8 < numChannels ;loopCtr_u8++)
        {
            uint8 ChannelId;

            ChannelId = (pncConfigPtr_pcst->Channels_pcu8[loopCtr_u8]);

            channelRamPtr_pst = &ComM_ChannelStruct[ChannelId];

            if(channelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION)
            {
                COMM_SET_BIT((channelRamPtr_pst -> PassiveRequestState_u8),(0x01));
            }
        }

        #endif  /*  (COMM_SYNCHRONOUS_WAKE_UP != STD_ON )  */

        /* If COMM_SYNCHRONOUS_WAKE_UP is enabled */
        #if (COMM_SYNCHRONOUS_WAKE_UP != STD_OFF )

        /*  As synchronous wake up is enabled, all channels needs to be waken up*/
        /*  Wake up detected on a particular PNC should wake up all channels of the PNC */
        /* Set Wake up indication for the PNC on which wake up is indicated and its linked channels*/

        for (loopCtr_u8 = C_ZERO; loopCtr_u8 < COMM_NO_OF_CHANNELS ;loopCtr_u8++)
        {
            channelRamPtr_pst = &ComM_ChannelStruct[loopCtr_u8];

            if(channelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION)
            {
                COMM_SET_BIT((channelRamPtr_pst -> PassiveRequestState_u8),(0x01));
            }

        }
        #endif  /*  (COMM_SYNCHRONOUS_WAKE_UP != STD_OFF )  */

        SchM_Exit_ComM_ChannelNoNest();
    }

    #else   /* #if (COMM_PNC_ENABLED != STD_OFF) */

    (void)(PNCid);

    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* This API shouldnt be called if PN is disabled. However, it is not put under feature switch as EcuM
     * expects this interface always independent of availability of PN
     * Hence report to DET */

    (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_ECUM_PNCWAKEUP_INDICATION,COMM_E_ERROR_IN_PROV_SERVICE);
    return;

    #endif  /* #if (COMM_DEV_ERROR_DETECT != STD_OFF) */

    #endif  /* #if (COMM_PNC_ENABLED != STD_OFF) */
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"



