

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"
/**
***************************************************************************************************
** Function Name        : CanSM_ClearTrcvWufFlagIndication
**
** Service ID           : 0x08
**
** Description          : This callback function shall indicate the CanIf_ClearTrcvWufFlag API process end for the notified CAN Transceiver.
**
** Sync/Async           : Synchronous
**
** Re-entrancy          : Reentrant for different CAN Transceivers
**
** Input Parameters     : Transceiver - Requested Transceiver
**
** InOut parameter      : None
**
** Output Parameters    : None
**
** Return parameter     : None

***************************************************************************************************
**/
#if(CANSM_PN_SUPPORT_CONFIGD == STD_ON)
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"

void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver)
{

    /*Counter variable*/
	uint8_least network_indx_uo;
    boolean CanSM_st_TrcvConfigd;
	CanSM_NetworkModeStateType_ten CurrNwMode_Temp;

    /*Initialise the flag used to keep track if there is TransceiverId is valid one*/
    CanSM_st_TrcvConfigd = FALSE;
	/*Report a DET if the module is not initialised*/
    CANSM_REPORT_ERROR((CanSM_Init_ab == (boolean)CANSM_UNINITED ),
                           ((uint8)CANSM_CLEARTRCVWUFFLAGIND_SID),
                           ((uint8)CANSM_E_UNINIT)
                          )

    /* Loop through the networks to find the corresponding network for the 'Transceiver' */
	for(network_indx_uo =0; network_indx_uo < CanSM_ConfigSet_pcst->CanSM_SizeOfCanSMNetworks_u8 ; network_indx_uo++)
		{

			CurrNwMode_Temp = CanSM_CurrNw_Mode_en[network_indx_uo];
			/*If the 'TransceiverId' matches the input transceiver of a network passed as parameter*/
			if(CanSM_Network_pcst[network_indx_uo].Trcv_hndle_u8 == Transceiver)
			{
				/*Notify DET if CanSM is not initialized.*/
				CANSM_REPORT_ERROR((  CurrNwMode_Temp == CANSM_BSM_S_NOT_INITIALIZED),
								   ((uint8)CANSM_CLEARTRCVWUFFLAGIND_SID),
								   ((uint8)CANSM_E_UNINIT)
								   )

				/*Set the PN indication flag for the network */
				CanSM_PN_Substate_Ind_ab[network_indx_uo] = TRUE;

				CanSM_st_TrcvConfigd = TRUE;
				break;
			}
		}

       /*Report to DET if 'Transceiver' was invalid*/
        CANSM_REPORT_ERROR(CanSM_st_TrcvConfigd == FALSE,(CANSM_CLEARTRCVWUFFLAGIND_SID),(CANSM_E_PARAM_TRANSCEIVER))

}

#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif
