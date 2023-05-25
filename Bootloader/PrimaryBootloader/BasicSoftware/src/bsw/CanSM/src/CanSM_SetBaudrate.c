
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"
/**
***************************************************************************************************
** Function Name        : CanSM_SetBaudrate
**
** Service ID           : 0x0d
**
** Description          : This service shall start an asynchronous process to change the baud rate for the configured
**                        CAN controllers of a certain CAN network. Depending on necessary baud rate modifications the
**                        controllers might have to reset.
**
** Sync/Async           : Synchronous
**
** Re-entrancy          : Reentrant for different Networks. Non reentrant for the same Network.
**
** Input Parameters     : Network-Handle of the addressed CAN network for the baud rate change
**                        BaudRateConfigID - references a baud rate configuration by ID
**
** InOut parameter      : None
**
** Output Parameters    : None
**
** Return parameter     : E_OK: Service request accepted, setting of (new) baud rate started
**                        E_NOT_OK: Service request not accepted

***************************************************************************************************/
#if (CANSM_SET_BAUDRATE_API == STD_ON)
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
Std_ReturnType CanSM_SetBaudrate(NetworkHandleType Network,const uint16 BaudRateConfigID)

{
    /*To keep a track of Return value*/
    Std_ReturnType CanSM_FuncVal_uo;
    /*To keep a track of the BSWM mode*/
	CanSM_BswMCurrentStateType CanSM_BswM_Mode_en;
	/*To keep a track of current network mode */
    CanSM_NetworkModeStateType_ten CanSM_CurrNwMode_en;
	/*To keep a track of current BOR mode */
    CanSM_BusOffRecoveryStateType_ten CanSM_CurrBORState_en;

	CanSM_FuncVal_uo = E_NOT_OK;

	/*Notify DET if CanSM is not initialized and return E_NOT_OK*/
    CANSM_REPORT_ERROR_NOK((CanSM_Init_ab == (boolean)CANSM_UNINITED),
                           ((uint8)CANSM_SETBAUDRATE_SID),
                           ((uint8)CANSM_E_UNINIT))

    /*Get the CanSM network Index value corresponding to the ComM channel Id received */
	Network = CanSM_GetHandle(Network);

    /*Notify DET if network handle is invalid and return E_NOT_OK*/
    CANSM_REPORT_ERROR_NOK(((uint8)Network >= (CanSM_ConfigSet_pcst->CanSM_SizeOfCanSMNetworks_u8)),
                            ((uint8)CANSM_SETBAUDRATE_SID),
                            ((uint8)CANSM_E_INVALID_NETWORK_HANDLE))

    /*Make a local copy of current network mode and current BOR state for the network*/
    CanSM_CurrNwMode_en = CanSM_CurrNw_Mode_en[Network];
    CanSM_CurrBORState_en = CanSM_currBOR_State_en[Network];

   	/*Return E_NOT_OK if the intended CanSM network is not initialised*/
    if(CanSM_Network_Init_ab[Network] == TRUE)
		{
			/*Store the baudrate if no baudrate change is already in progress*/
			if(CanSM_CurrNwMode_en != CANSM_BSM_S_SET_BAUDRATE)
				{

				/*Accept the change only if module is initialised and network is in CANSM_BSM_S_FULLCOM and BOR mode is CANSM_S_NO_BUS_OFF and configID is not 0xffffu*/
					if((CanSM_CurrNwMode_en == CANSM_BSM_S_FULLCOM) && (CanSM_CurrBORState_en == CANSM_S_NO_BUS_OFF) && (BaudRateConfigID != (uint16)0xFFFFu) )
						{

	                        /*Make local copy of BaudRateConfigID*/
	                        CanSM_BaudRateConfigID_Value_au16[Network] = BaudRateConfigID;

					        /*Set the return value to E_OK*/
					        CanSM_FuncVal_uo = E_OK;

							/*Set current network mode to CANSM_BSM_S_SET_BAUDRATE*/
							CanSM_CurrNw_Mode_en[Network]=CANSM_BSM_S_SET_BAUDRATE;

							/*First substate is CANSM_BSM_CHANGE_BR_SYNC */
							CanSM_ChangeBR_Substates_en[Network] = CANSM_BSM_CHANGE_BR_SYNC;

						   /*Notify BswM about the baudrate change*/
							CanSM_BswM_Mode_en = CANSM_BSWM_CHANGE_BAUDRATE;

							/* Give notification to the BswM about the current mode*/
							BswM_CanSM_CurrentState(CanSM_Network_pcst[Network].ComM_channelId_uo,CanSM_BswM_Mode_en);

						}
				}
		}
	else
		{
	        CanSM_FuncVal_uo = E_NOT_OK;
		}
    return (CanSM_FuncVal_uo);
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
#endif
