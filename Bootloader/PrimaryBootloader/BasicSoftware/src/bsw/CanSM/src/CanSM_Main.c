
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"


/**************************************************************************************************************************************************************************************************************/
/**
***************************************************************************************************
** Function Name        : CanSM_MainFunction
**
** Service ID           : None
**
** Description          : his is the Main Function which is called cyclically by the BSW schedular
**
** Sync/Async           : Asynchronous
**
** Re-entrancy          :
**
** Input Parameters     : network
**
** InOut parameter      : None
**
** Output Parameters    : None
**
** Return parameter     : None

***************************************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
void CanSM_MainFunction(void)
{
    /* Local variable to hold network index */
    uint8 CanSM_NetworkIdx_u8;
    /*To keep a track of current network mode*/
    CanSM_NetworkModeStateType_ten CanSM_CurrNwMode_en;
    /*To keep a track of current BOR state */
    CanSM_BusOffRecoveryStateType_ten CanSM_CurrBORState_en;
    /*To keep the track of Tx Timeout Exception state*/
    CanSM_TxTimeoutException_Substates_ten CanSM_TxToutException_state_en;
    /*Temporary pointer to hold the adress of configured network*/
    const CanSM_NetworkConf_tst * CanSM_NetworkConf_ps;
    /*To hold the controller Id*/
    uint8 CanSM_ControllerId_u8;
    /*Used in FOR loop to access CAN controllers of the corresponding network*/
    uint8 CanSM_Ctrl_index_u8;
    /*Temporary variable to hold the ComM mode */
    ComM_ModeType CanSM_ComM_Mode_uo;

    #if(CANSM_SET_BAUDRATE_API == STD_ON)
    uint16 CanSM_BRConfidId_u16;
    #endif
    /*local variable to hold the DEM Flag value*/
    #if(CANSM_DSM_REINIT_ENABLED != STD_OFF)
    uint16 CanSM_DsmFlag_u8;
    #endif
    /*To hold the ComM status*/
    ComM_ModeType CanSM_ReqComMode_uo;
    /*To keep a track of current BOR mode Enable/Disable */
    uint8 CanSM_BORMode_u8;

    /*Loop through all he networks*/
    for(CanSM_NetworkIdx_u8 =0; CanSM_NetworkIdx_u8 < CanSM_ConfigSet_pcst->CanSM_SizeOfCanSMNetworks_u8 ; CanSM_NetworkIdx_u8++)
    {

        /*Hold the address of the configured network*/
        CanSM_NetworkConf_ps = &CanSM_Network_pcst[CanSM_NetworkIdx_u8];

        /*Make a local copy of current network mode */
        CanSM_CurrNwMode_en = CanSM_CurrNw_Mode_en[CanSM_NetworkIdx_u8];

        /* Get the Current BOR State only after the Mutex is accquired */
        CanSM_CurrBORState_en = CanSM_currBOR_State_en[CanSM_NetworkIdx_u8];

        /*Get the current Tx Timeout Exception state */
        CanSM_TxToutException_state_en = CanSM_TxTimeoutexception_Substates_en[CanSM_NetworkIdx_u8];

        CanSM_ReqComMode_uo = CanSM_ReqComM_Mode_en[CanSM_NetworkIdx_u8];

        CanSM_BORMode_u8=  CanSM_BORMode_au8[CanSM_NetworkIdx_u8];


        #if (CANSM_SET_BAUDRATE_API == STD_ON)
        /*Make local copy of CanSM_BaudRateConfigID_Value_au16[CanSM_NetworkIdx_u8]*/
        CanSM_BRConfidId_u16 = CanSM_BaudRateConfigID_Value_au16[CanSM_NetworkIdx_u8];
        #endif

        /*Assign the DEM_Flag Vlue to Local Variable*/
        #if(CANSM_DSM_REINIT_ENABLED != STD_OFF)
         CanSM_DsmFlag_u8=CanSM_DSM_ReInit_Status_ast[CanSM_NetworkIdx_u8].DEMReinitFlag_u8;
        #endif

         /*Check the flag if it is set to CANSM_DSM_REQUESTED */
        #if(CANSM_DSM_REINIT_ENABLED != STD_OFF)
        if(CanSM_DsmFlag_u8 == CANSM_DSM_REQUESTED)
        {
            /*Report to DEM the error and status*/
            Dem_ReportErrorStatus(CanSM_Network_pcst[CanSM_NetworkIdx_u8].BusOffEventId_uo,CanSM_DSM_ReInit_Status_ast[CanSM_NetworkIdx_u8].CanSM_DEM_Eventstatus_u8 ) ;
            /*Update the flag to CANSM_DSM_REQUEST_COMPLETED*/
            CanSM_DSM_ReInit_Status_ast[CanSM_NetworkIdx_u8].DEMReinitFlag_u8 = CANSM_DSM_REQUEST_COMPLETED;
        }
        #endif
        /* Increment the timer for the network if Running*/
        if(CanSM_TimerConfig_ast[CanSM_NetworkIdx_u8].stTimer == CANSM_TIMER_RUNNING)
        {
            CanSM_TimerConfig_ast[CanSM_NetworkIdx_u8].cntTick_u16++;
        }

        #if (CANSM_SET_BAUDRATE_API == STD_ON)
        if(CanSM_BRConfidId_u16 != 0xFFFFu)
        {
            (void)CanSM_SetBaudrate_StateMachine((NetworkHandleType)CanSM_NetworkIdx_u8);
        }
        #endif

        if(CanSM_TxToutException_state_en != CANSM_TxTimeoutException_DEFAULT)
        {
            CanSM_TxTimeoutException_StateMachine((NetworkHandleType)CanSM_NetworkIdx_u8);
        }
        /* No Transition required if same ComM mode is requested - Runtime Optimization*/
        if((CanSM_ReqComMode_uo != (ComM_ModeType)CanSM_CurrNwMode_en))
        {
            CanSM_NetworkModeTrans((NetworkHandleType)CanSM_NetworkIdx_u8,CanSM_ReqComMode_uo);

        }

        if(CanSM_WakeUpValidation_Substates_en[CanSM_NetworkIdx_u8] != CANSM_WAKEUP_VALIDATION_DEFAULT)
        {
            CanSM_WakeUpValidation_StateMachine((NetworkHandleType)CanSM_NetworkIdx_u8);
        }
        if(CanSM_BOR_SilentCom_ab[CanSM_NetworkIdx_u8] == TRUE)
        {
            CanSM_BOR_CtrlsStart_SilentCom(CanSM_NetworkIdx_u8);

        }

        /*This condition is to put the BOR state to CANSM_S_NO_BUS_OFF for passing Guarding contion G_BUS_OFF_PASSIVE*/
        if((CanSM_BORMode_au8[CanSM_NetworkIdx_u8]==(uint8)CANSM_RECOVERY_ENABLED) && (CanSM_CurrBORState_en == CANSM_S_BUS_OFF_CHECK) &&
         /* MR12 RULE 13.5 VIOLATION: Warning is "Not Critical".Below conditional statement first checks the
            if the Tx Confirmation for the intended network is true . Evaluation(or no evaluation)of the 2nd or 3rd operand does not impact the system
            behaviour.Redesign is not recommended as the change would decrease maintainability and increase
            runtime*/
        #if (CANSM_BOR_TX_CONFIRMATION_POLLING != STD_OFF)
        (
        ((CanSM_NetworkConf_ps->BorTxConfPolling_b != FALSE)  &&  (E_OK == CanSM_CheckTxRxNotification((uint8)CanSM_NetworkIdx_u8))) ||
        ((CanSM_NetworkConf_ps->BorTxConfPolling_b == FALSE)  &&   (CanSM_TimerConfig_ast[CanSM_NetworkIdx_u8].cntTick_u16 >
        CanSM_NetworkConf_ps->BorTimeTxEnsured_u16))
        )
        #else
          (CanSM_TimerConfig_ast[CanSM_NetworkIdx_u8].cntTick_u16 > CanSM_NetworkConf_ps->BorTimeTxEnsured_u16 )
        #endif
        )
        {
            /*Report DEM that No Bus Off Event is Present*/
            Dem_ReportErrorStatus(CanSM_Network_pcst[CanSM_NetworkIdx_u8].BusOffEventId_uo,DEM_EVENT_STATUS_PASSED) ;
            /* FC_VariationPoint_START */
            /*Update the intended network variable with latest DEM event status to be used to provide status to DEM module on request*/
            #if(CANSM_DSM_REINIT_ENABLED != STD_OFF)
            CanSM_DSM_ReInit_Status_ast[CanSM_NetworkIdx_u8].CanSM_DEM_Eventstatus_u8 = DEM_EVENT_STATUS_PASSED;
            #endif
            /* FC_VariationPoint_END */

            /* Critical section Enter*/
            SchM_Enter_CanSM_BOR_Nw_ModesNoNest();

            /* Update the BOR State */
            CanSM_currBOR_State_en[CanSM_NetworkIdx_u8] = CANSM_S_NO_BUS_OFF;

            /*Update the global array for the network as FALSE */
            CanSM_BusOff_Indicated_ab[CanSM_NetworkIdx_u8] = FALSE;

            /*Set the Timer to Elapsed Mode */
            CanSM_TimerConfig_ast[CanSM_NetworkIdx_u8].stTimer = CANSM_TIMER_ELAPSED;

            /*Reset Bus Counter*/
            CanSM_BusOff_Cntr_au8[CanSM_NetworkIdx_u8] = CANSM_ZERO;

            /*Reset the T_REPEAT_MAX counter*/
            CanSM_Num_Unsuccessful_ModeReq_au8[CanSM_NetworkIdx_u8]= CANSM_ZERO;

            /* Critical Section Exit*/
            SchM_Exit_CanSM_BOR_Nw_ModesNoNest();
        }

        if(CanSM_Rb_DisableBusOffRecovery_ab[CanSM_NetworkIdx_u8] == TRUE)
            {
                /*Disable the Bus off Reovery for the intended CanSM Network*/
                CanSM_BORMode_au8[CanSM_NetworkIdx_u8] = CANSM_RECOVERY_DISABLED;
               /* Loop for All the Controllers in the network and Set them to STOPPED Mode*/
                for(CanSM_Ctrl_index_u8 = 0; CanSM_Ctrl_index_u8 < CanSM_NetworkConf_ps->SizeofController_u8 ;CanSM_Ctrl_index_u8++)
                {
                    /*Store the CAN controller ID belonging to the network*/
                    CanSM_ControllerId_u8 = CanSM_NetworkConf_ps->Cntrl_startidx_pu8[CanSM_Ctrl_index_u8];

                       /*If E_NOT_OK is returned for any of the controller, increment the T_REPEAT_MAX counter and break*/
                       (void)CanIf_SetControllerMode(CanSM_ControllerId_u8,CANIF_CS_STOPPED);

                    /* Set PDU to OFFLINE Mode */
                    /*As retry mechanism cannot be implemented, CanSm expects CanIf to give E_OK for the below function call. Hence retrun value is not checked */
                    (void)(CanIf_SetPduMode((uint8)CanSM_ControllerId_u8,CANIF_OFFLINE));

                }
            }
        /* Check if BOR State Machine is enabled */
        else if((CanSM_BORMode_u8 == (uint8)CANSM_RECOVERY_ENABLED) &&  ((CanSM_BusOff_Indicated_ab[CanSM_NetworkIdx_u8] == TRUE) || (CanSM_BusOffISRPend_ab[CanSM_NetworkIdx_u8])))
        {

            CanSM_BusOffTransitions((NetworkHandleType)CanSM_NetworkIdx_u8);
        }
        else if(CanSM_CurrNw_Mode_en[CanSM_NetworkIdx_u8]== CANSM_BSM_S_NOCOM)
        {
            /*Update CanSM_BusSMMode_au8[] to CANSM_BSM_S_NOCOM i.e COMM_NO_COMMUNICATION*/
            CanSM_BusSMMode_au8[CanSM_NetworkIdx_u8] = (ComM_ModeType)CANSM_BSM_S_NOCOM;
            /*Notify ComM*/
            CanSM_ComM_Mode_uo = COMM_NO_COMMUNICATION;
            /*Update the ComM with current communication mode*/
            ComM_BusSM_ModeIndication(CanSM_NetworkConf_ps->ComM_channelId_uo, &CanSM_ComM_Mode_uo);
        }
        else
        {
            /* Intentionally kept empty*/
        }
     }
}
#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
