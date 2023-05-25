
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"

/***************************************************************************************************
** Function Name        : CanSM_ControllerModeIndication
**
** Service ID           : 0x07
**
** Description          : This callback shall notify the CanSM module about a CAN controller mode change.
**
** Sync/Async           : Synchronous
**
** Re-entrancy          : Reentrant (only for different CAN controllers)
**
** Input Parameters     : ControllerId - CAN controller, whose mode has changed
**                        ControllerMode - Notified CAN controller mode
**
** InOut parameter      : None
**
** Output Parameters    : None
**
** Return parameter     : None

***************************************************************************************************/

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
void CanSM_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode)
{
    /*Variable to hold the network index for the given controller*/
    uint8 network_indx_u8;
    /*To hold the controller Id*/
    uint8_least CanSM_ControllerId_u8;
    /*Used in FOR loop to access CAN controllers of the corresponding network*/
    uint8_least CanSM_Ctrl_index_u8;
    /*Hold the counter number to know that all the controllers are stopped*/
    uint8 CanSM_Controller_Counter_u8;
    /*Temporary pointer to hold the adress of configured network*/
    const CanSM_NetworkConf_tst * CanSM_NetworkConf_ps;

    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
    /*Variable to hold current network mode*/
    CanSM_NetworkModeStateType_ten CurNwMode_Temp_uo;
    #endif

    /*Initialise the local variable with value 0*/
    CanSM_Controller_Counter_u8 =  CANSM_ZERO;

    /* Notify DET for Invalid Controller ID passed*/
    CANSM_REPORT_ERROR((ControllerId >= CANSM_NUM_CAN_CONTROLLERS),
                       (uint8)CANSM_CTRLMODEINDICATION_SID,
                       (uint8)CANSM_E_PARAM_CONTROLLER)

    /* Notify DET if CanSM module is not initialized */
    CANSM_REPORT_ERROR((CanSM_Init_ab == (boolean)CANSM_UNINITED),
                           ((uint8)CANSM_CTRLMODEINDICATION_SID),
                           ((uint8)CANSM_E_UNINIT)
                           )

    /*If the given controller id is valid, Get the assocoaited network Id */
    network_indx_u8 = (uint8)CanSM_ConfigSet_pcst->CanSM_NetworktoCtrlConf_pcu8[ControllerId];

    /* Report DET : Development Error for Invalid networkID */
    CANSM_REPORT_ERROR((network_indx_u8 == CANSM_INVALID_ID), (uint8)CANSM_CTRLMODEINDICATION_SID,
            (uint8)CANSM_E_INVALID_NETWORK_HANDLE)

    /*Hold the address of the configured network*/
    CanSM_NetworkConf_ps = &CanSM_Network_pcst[network_indx_u8];

    #if (CANSM_DEV_ERROR_DETECT != STD_OFF)
    /*Make local copy of Current Network Mode*/
    CurNwMode_Temp_uo = CanSM_CurrNw_Mode_en[network_indx_u8];
    #endif

    /* Notify DET if CanSM network is not initialized */
    CANSM_REPORT_ERROR((CANSM_BSM_S_NOT_INITIALIZED == CurNwMode_Temp_uo),
                           ((uint8)CANSM_CTRLMODEINDICATION_SID),
                           ((uint8)CANSM_E_UNINIT)
                           )

    /* Update the Controller mode indication array for the controller id passed as TRUE*/
    CanSM_ControllerIndications_ab[ControllerId] = TRUE;

    /* Initialize the local variable with the controllerId provided by CanIF */
	CanSM_ControllerId_u8 = ControllerId;

    /*Check if the Controller mode is in CANIF_CS_STOPPED */
    if(ControllerMode==CANIF_CS_STOPPED)
    {
        /*Update the CanSM_ControllerState_en for stopped state */
        CanSM_ControllerState_en[CanSM_ControllerId_u8]=CANSM_ControllerState_STOPPED;
    }

    /*Check if the Controller mode is in CANIF_CS_STARTED */
    if(ControllerMode==CANIF_CS_STARTED)
    {
        /*Update the CanSM_ControllerState_en for started state */
        CanSM_ControllerState_en[CanSM_ControllerId_u8]=CANSM_ControllerState_STARTED;
    }

    /*Check if the Controller mode is in CANIF_CS_STARTED */
    if(ControllerMode==CANIF_CS_SLEEP)
    {
        /*Update the CanSM_ControllerState_en for sleep state */
        CanSM_ControllerState_en[CanSM_ControllerId_u8]=CANSM_ControllerState_SLEEP;
    }

    if(CanSM_NetworkConf_ps->SizeofController_u8 > 1u)
    {
        /* Loop for all the Controllers in the network*/
        for(CanSM_Ctrl_index_u8 = 0; CanSM_Ctrl_index_u8 < CanSM_NetworkConf_ps->SizeofController_u8 ;CanSM_Ctrl_index_u8++)
        {
            /*Store the CAN controller ID belonging to the network*/
            CanSM_ControllerId_u8 = CanSM_NetworkConf_ps->Cntrl_startidx_pu8[CanSM_Ctrl_index_u8];

            if(CanSM_ControllerIndications_ab[CanSM_ControllerId_u8] == TRUE)
            {
                CanSM_Controller_Counter_u8++ ;
            }
        }
        if(CanSM_Controller_Counter_u8 == CanSM_NetworkConf_ps->SizeofController_u8 )
        {
            /*Set the Controller Mode Indication of the network to TRUE*/
            CanSM_Ctrl_ModeInd_ab[network_indx_u8] = TRUE;

            /* Loop for all the Controllers in the network*/
            for(CanSM_Ctrl_index_u8 = 0; CanSM_Ctrl_index_u8 < CanSM_NetworkConf_ps->SizeofController_u8 ;CanSM_Ctrl_index_u8++)
            {
                /*Store the CAN controller ID belonging to the network*/
                CanSM_ControllerId_u8 = CanSM_NetworkConf_ps->Cntrl_startidx_pu8[CanSM_Ctrl_index_u8];

                /*Reset the Controller mode indications for all the CAN controllers of the network */
                CanSM_ControllerIndications_ab[CanSM_ControllerId_u8] = FALSE;
            }
        }
    }
    else
    {
        /*Set the Controller Mode Indication of the network to TRUE*/
        CanSM_Ctrl_ModeInd_ab[network_indx_u8] = TRUE;
        /*Reset the Controller mode indications for the CAN controller of the network */
        CanSM_ControllerIndications_ab[ControllerId] = FALSE;
    }

}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
