

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Prv.h"
#include "Nm_Cbk.h"
#if (!defined(NM_AR_RELEASE_MAJOR_VERSION) || (NM_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(NM_AR_RELEASE_MINOR_VERSION) || (NM_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_PassiveStartUp
 Description      : This is the AUTOSAR interface for triggering passive startup of the AUTOSAR CAN
                    NM. It is called either by ComM or by Nm Interface when NetworkStartIndication
                    is given.It triggers the transition from Bus-Sleep Mode or Prepare Bus Sleep
                    Mode to the Network Mode in Repeat Message State. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : E_OK - No error
                  : E_NOT_OK - Passive startup of network management has failed
 ***************************************************************************************************/

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

Std_ReturnType CanNm_PassiveStartUp( NetworkHandleType nmChannelHandle)
{
    /* Pointer to configuration data */
    const CanNm_ChannelConfigType * ConfigPtr_pcs;

    /* Pointer to RAM data */
    CanNm_RamType * RamPtr_ps;

    /* Return value of the API */
    Std_ReturnType RetVal_en;

    /* Variable to hold the Network Handle of CanNm */
    NetworkHandleType CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm Channel Index from the received ComM ChannelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if Network Handle is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_PASSIVESTARTUP, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is Uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                            nmChannelHandle, CANNM_SID_PASSIVESTARTUP, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* initialize the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

	/* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /* TRACE[SWS_CanNm_00128] : API is valid if CanNm is in Bus Sleep Mode or Prepare Bus Sleep Mode */
    if((RamPtr_ps->State_en == NM_STATE_BUS_SLEEP) || (RamPtr_ps->State_en == NM_STATE_PREPARE_BUS_SLEEP))
    {
        /* set the pointer to Configuration structure of Channel nmChannelHandle */
        ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

        /* Transit to Network Mode */
        CanNm_ChangeState(RamPtr_ps, NM_STATE_REPEAT_MESSAGE, NM_MODE_NETWORK);

        /* start NM-Timeout time */
        CanNm_StartTimer(RamPtr_ps->ctNMTimeoutTimer);

        /* indicate NmIf about Network mode */
        Nm_NetworkMode(nmChannelHandle);

        #if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
        /* start transmission */
        CanNm_StartTransmission(CanNm_NetworkHandle);
        #endif

        if (ConfigPtr_pcs->RepeatMessageTime != 0U)
        {
            /* start Repeat Message timer */
            CanNm_StartTimer(RamPtr_ps->ctRepeatMessageTimer);

            /* Notify to the upper layer about changes of the CanNm states if configuration parameter
             CANNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
            #if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
            Nm_StateChangeNotification(nmChannelHandle, NM_STATE_BUS_SLEEP, NM_STATE_REPEAT_MESSAGE);
            #endif
        }
        else
        {
            /* if network is released, goto Ready Sleep state */
            RamPtr_ps->State_en = NM_STATE_READY_SLEEP;

            /* Notify to the upper layer about changes of the CanNm states if configuration parameter
               CANNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
            #if (CANNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
            Nm_StateChangeNotification(nmChannelHandle, NM_STATE_REPEAT_MESSAGE, NM_STATE_READY_SLEEP);
            #endif

            /* Stop transmission */
            RamPtr_ps->MsgTxStatus_b = FALSE;
         }

         /* return successful transition */
        RetVal_en = E_OK;
    }

    /* TRACE[SWS_CanNm_00147] : The API is called in Network Mode */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


