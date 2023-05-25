
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"

/***************************************************************************************************
** Function Name        : CanSM_TransceiverModeIndication
**
** Service ID           : 0x09
**
** Description          : This callback shall notify the CanSM module about a CAN transceiver mode change.
**
** Sync/Async           : Synchronous
**
** Re-entrancy          : Reentrant for different CAN Transceivers
**
** Input Parameters     : TransceiverId - CAN transceiver, whose mode has changed
**                        TransceiverMode- Notified CAN transceiver mode
**
** InOut parameter      : None
**
** Output Parameters    : None
**
** Return parameter     : None

***************************************************************************************************/
#if (CANSM_CFG_TRCV_CANIF_SUPPORT == STD_ON)
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
void CanSM_TransceiverModeIndication(uint8 TransceiverId,CanTrcv_TrcvModeType TransceiverMode)
{

    /*Variable to hold the network index for the given controller*/
	uint8_least network_indx_uo;
	#if (CANSM_DEV_ERROR_DETECT != STD_OFF)
	/*Variable to keep track if the TransceiverId is valid */
    boolean CanSM_TrcvConfigd_b;
	#endif
    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
    /*Variable to hold current network mode*/
    CanSM_NetworkModeStateType_ten CurNwMode_Temp_uo;
    #endif

    /*local variable to store CanTrcv state */
    CanSM_ReqCanTrcv_States  CanSM_CanTrcvState_en;

    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
    /*Initialise the flag used to keep track if the TransceiverId is valid*/
	CanSM_TrcvConfigd_b = FALSE;
    #endif
	/* This input parameter is not used in the api*/
	(void)TransceiverMode;
	
		/*Initialize CanSM_CanTrcvState_en */
	CanSM_CanTrcvState_en = CANSM_CANTRCV_DEFAULT;


    /*Report a DET if the module is not initialised*/
    CANSM_REPORT_ERROR((CanSM_Init_ab == (boolean)CANSM_UNINITED ),
                           ((uint8)CANSM_TRCVMODEINDICATION_SID),
                           ((uint8)CANSM_E_UNINIT)
                          )

    /* Loop through the networks to which the transceiver belongs to, and update the CanSM_Trcv_ModeInd_ab[] */
	for(network_indx_uo =0; network_indx_uo < CanSM_ConfigSet_pcst->CanSM_SizeOfCanSMNetworks_u8 ; network_indx_uo++)
        {
            /*Check if the 'TransceiverId' matches the transceiver's id of a network*/
            if(CanSM_Network_pcst[network_indx_uo].Trcv_hndle_u8==TransceiverId)
                {
                    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
                    /*Make local copy of Current Network Mode*/
                    CurNwMode_Temp_uo = CanSM_CurrNw_Mode_en[network_indx_uo];
                    #endif

                    /*Notify DET if CanSM network is not initialized.*/
                    CANSM_REPORT_ERROR((CANSM_BSM_S_NOT_INITIALIZED == CurNwMode_Temp_uo),
                                       ((uint8)CANSM_TRCVMODEINDICATION_SID),
                                       ((uint8)CANSM_E_UNINIT))

                    /*Condition needs to be checked for the requested request and CanSM_CanTrcvState_en is updated accordingly */
                    if(TransceiverMode == CANTRCV_TRCVMODE_NORMAL)
                    {
                        CanSM_CanTrcvState_en = CANSM_CANTRCV_NORMAL;
                    }
                    else if(TransceiverMode == CANTRCV_TRCVMODE_SLEEP)
                    {
                        CanSM_CanTrcvState_en = CANSM_CANTRCV_SLEEP;
                    }
                    else if(TransceiverMode == CANTRCV_TRCVMODE_STANDBY)
                    {
                        CanSM_CanTrcvState_en = CANSM_CANTRCV_STANDBY;
                    }
                    else
                    {
                        /* Nothing to do*/
                    }

                    /* Check if the Trcv mode obtained is same as requested and also if mode change request was made from CanSm */
                    if((CanSM_ReqMode_a[network_indx_uo]== CanSM_CanTrcvState_en) && (CanSM_ModeChangeReq_flag[network_indx_uo] == TRUE))
                    {
                        /*Set CanSM_Trcv_ModeInd_a[] of the network to TRUE */
                        CanSM_Trcv_ModeInd_ab[network_indx_uo]=TRUE;
                        CanSM_ModeChangeReq_flag[network_indx_uo] = FALSE;
                    }
                    else
                    {
                        /* Nothing to do*/
                    }

                    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
                    CanSM_TrcvConfigd_b = TRUE;
                    #endif
                    break;
                }
        }

        /* Notify DET for Invalid Transceiver ID.
         * Condition is TRUE, since the validity for the TransceiverId is checked in the preceding 'for' loop */
        CANSM_REPORT_ERROR(CanSM_TrcvConfigd_b == FALSE, (uint8)CANSM_TRCVMODEINDICATION_SID,
                (uint8)CANSM_E_PARAM_TRANSCEIVER)
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif
