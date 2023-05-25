
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"

/* NM Interface header file for declaration of callback notifications */
#include "Nm_Cbk.h"

#if (NM_LINNM_SUPPORT != STD_OFF)

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE void LinNm_Prv_ChangeState(LinNm_RamType_tst * RamPtr_pst, Nm_StateType nmState_en, Nm_ModeType nmMode_en);

LOCAL_INLINE void LinNm_Prv_ComputeSwFrTimer(LinNm_RamType_tst * RamPtr_pst);

LOCAL_INLINE void LinNm_Prv_GotoBusSleepMode(const NetworkHandleType nmChannelHandle_cu8, LinNm_RamType_tst * RamPtr_pst);

LOCAL_INLINE void LinNm_Prv_GotoNetworkMode(const NetworkHandleType nmChannelHandle_cu8, LinNm_RamType_tst * RamPtr_pst);

static void LinNm_Prv_InternalMainProcess(const NetworkHandleType LinNm_NetworkHandle_cu8);

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
/**********************************************************************************************************************
 Function name    : LinNm_Init
 Syntax           : void LinNm_Init(void)
 Description      : This is the AUTOSAR interface for initialization called by EcuM.
 Parameter        : NONE
 Return value     : NONE
 **********************************************************************************************************************/


void LinNm_Init(void)
{
    /* Pointer to RAM data */
    LinNm_RamType_tst * RamPtr_pst;

    /* variable to hold Channel handle */
    uint8_least Channel_u8;

    /*********************************************** End Of Declarations ****************************************/

     /* Initialize all the channels which are configured as active */
     for(Channel_u8 = 0; Channel_u8 < (uint8_least)NM_MAX_LINNM_CHANNELS; Channel_u8++)
     {
         /* initialize pointer to RAM structure */
         RamPtr_pst = &LinNm_RamData_ast[Channel_u8];

         /* Initialize LinNm's mode to bus-sleep mode */
         RamPtr_pst->Mode_e = NM_MODE_BUS_SLEEP;

         /* Initialize LinNm's state to bus-sleep mode */
         RamPtr_pst->State_e = NM_STATE_BUS_SLEEP;

         /* Initialize LinNm's internal global variable network request status */
         RamPtr_pst->NetworkReqState_e = LINNM_NETWORK_RELEASED_E;

         /* Initialize the free running timer */
         RamPtr_pst->ctSwFrTimer_u32 = 0;

         /* Initialize the LinNm Timeout Timer */
         RamPtr_pst->ctLINNMTimeoutTimer_u32 = 0;

       }
}

/***************************************************************************************************
 Function name    : LinNm_MainFunction
 Syntax           : void LinNm_MainFunction( void )
 Description      : Main function of the LinNm which processes the algorithm described in document SWS LinNm.
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/
void LinNm_MainFunction(void)
{

    /* Variable to hold the Network Handle of LinNm */
    NetworkHandleType LinNm_NetworkHandle;

    for(LinNm_NetworkHandle = 0; LinNm_NetworkHandle < NM_MAX_LINNM_CHANNELS ; LinNm_NetworkHandle++)
    {
        LinNm_Prv_InternalMainProcess(LinNm_NetworkHandle);
    }

}

/***************************************************************************************************
 Function name    : LinNm_Prv_InternalMainProcess
 Syntax           : void LinNm_Prv_InternalMainProcess( const NetworkHandleType LinNm_NetworkHandle )
 Description      : This is an internal main function of LINNM.
                    This is an internal function that does processing of state machine for all channels.
                    This is called by each channel specific Main Function that is scheduled in certain raster.
 Parameter        : LinNm_NetworkHandle - Identification of the LINNM-channel
 Return value     : None
 ***************************************************************************************************/
static void LinNm_Prv_InternalMainProcess(const NetworkHandleType LinNm_NetworkHandle_cu8)
{

     /* pointer to RAM data */
    LinNm_RamType_tst * RamPtr_pst;

     /* initialize the pointer to RAM data structure */
     RamPtr_pst = &LinNm_RamData_ast[LinNm_NetworkHandle_cu8];

    /* Compute the Free Running timer */
     LinNm_Prv_ComputeSwFrTimer(RamPtr_pst);

     if(RamPtr_pst->State_e == NM_STATE_NORMAL_OPERATION)
     {
         if(RamPtr_pst->NetworkReqState_e != LINNM_NETWORK_REQUESTED_E)
         {
             if(((RamPtr_pst->ctSwFrTimer_u32) - (RamPtr_pst->ctLINNMTimeoutTimer_u32)) >= LINNM_TIMEOUT_TIMER )
             {

                 /* Go to Bus Sleep Mode */
                 LinNm_Prv_ChangeState(RamPtr_pst, NM_STATE_BUS_SLEEP, NM_MODE_BUS_SLEEP);

                 /* Notify to the upper layer about changes of the LinNm states if configuration parameter
                                           LINNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
#if (LINNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
                 Nm_StateChangeNotification(LinNm_ConfData_auo[LinNm_NetworkHandle_cu8], NM_STATE_NORMAL_OPERATION, NM_STATE_BUS_SLEEP);
#endif

                 /* indicate NmIf about Bus Sleep mode */
                 Nm_BusSleepMode(LinNm_ConfData_auo[LinNm_NetworkHandle_cu8]);

             }
             else
             {
                 /* stay in network mode until timer gets expired */
             }
         }
         else
         {
              /*stay in network mode until network release is called */
         }
     }
     else
     {
          /* stay in the bus sleep state */
     }
}









/**********************************************************************************************************************
 Function name    : LinNm_PassiveStartUp
 Syntax           : Std_ReturnType LinNm_PassiveStartUp( NetworkHandleType nmChannelHandle )
 Description      : Passive startup of the AUTOSAR LIN NM.
 Parameter        : nmChannelHandle - Identification of the NM-channel
 Return value     : Std_ReturnType   - E_OK: No error
                                    - E_NOT_OK: Passive startup of network management has failed

 **********************************************************************************************************************/
Std_ReturnType LinNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{

    /* pointer to RAM data */
    LinNm_RamType_tst * RamPtr_pst;

    /* Return Value of the API */
    Std_ReturnType RetVal_u8;

    /* Variable to hold the Network Handle of LinNm */
    NetworkHandleType LinNm_NetworkHandle_u8;

    /*********************************************** End Of Declarations ****************************************/

    /* Receive the Internal LinNmChannel structure index from the received  ComM ChannelID*/
    LinNm_NetworkHandle_u8 = NM_LINNM_GET_HANDLE(nmChannelHandle);

    /* initialize the pointer to RAM data structure */
    RamPtr_pst = &LinNm_RamData_ast[LinNm_NetworkHandle_u8];

    /* check whether the channel handle is within the allowed range and LinNm is initialised*/
    if((LinNm_NetworkHandle_u8 < (uint8)NM_MAX_LINNM_CHANNELS)&&(RamPtr_pst->State_e != NM_STATE_UNINIT))
    {
        /* check if LinNm is in Bus Sleep State */
        if(RamPtr_pst->State_e == NM_STATE_BUS_SLEEP)
        {

              /* Goto Bus Network Mode */
              LinNm_Prv_GotoNetworkMode(nmChannelHandle,RamPtr_pst);


#if(LINNM_TIMEOUT_TIMER != 0U)

              /* set the Network Requested Status to released as it will be processed in the main function */
               RamPtr_pst->NetworkReqState_e = LINNM_NETWORK_RELEASED_E;

               /* start LinNm-Timeout time */
               RamPtr_pst->ctLINNMTimeoutTimer_u32 = RamPtr_pst->ctSwFrTimer_u32;
#else
               /* Goto Bus Sleep without staying in Network Mode */
               LinNm_Prv_GotoBusSleepMode(nmChannelHandle,RamPtr_pst);
#endif


             /* return successful transition */
             RetVal_u8 = E_OK;

         }
         else
         {

             /* service is rejected if the current state is not bus sleep*/
             RetVal_u8 = E_NOT_OK;
         }


    }
    else
    {

        /*  service is rejected if the hanlde id is not within the specified limit or if the module is not initialised*/
        RetVal_u8 = E_NOT_OK;
    }


    return RetVal_u8;
}


#if (LINNM_PASSIVE_MODE_ENABLED != STD_ON)
 /**********************************************************************************************************************
  Function name    : LinNm_NetworkRequest
  Syntax           : Std_ReturnType LinNm_NetworkRequest( NetworkHandleType nmChannelHandle )
  Description      : Request the network, since ECU needs to communicate on the bus
  Parameter        : nmChannelHandle - Identification of the NM-channel
  Return value     : Std_ReturnType   - E_OK: No error
                                     - E_NOT_OK: Requesting of network has failed

  **********************************************************************************************************************/
 Std_ReturnType LinNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{


    /* pointer to RAM data */
     LinNm_RamType_tst * RamPtr_pst;

    /* Return Value of the API */
    Std_ReturnType RetVal_u8;

    /* Variable to hold the Network Handle of LinNm */
    NetworkHandleType LinNm_NetworkHandle_u8;

    /*********************************************** End Of Declarations ****************************************/

    /* Receive the Internal LinNmChannel structure index from the received  ComM ChannelID*/
    LinNm_NetworkHandle_u8 = NM_LINNM_GET_HANDLE(nmChannelHandle);

    /* initialize the pointer to RAM data structure */
    RamPtr_pst = &LinNm_RamData_ast[LinNm_NetworkHandle_u8];

    /* check whether the channel handle is within the allowed range and LinNm is initialised*/
    if((LinNm_NetworkHandle_u8 < (uint8)NM_MAX_LINNM_CHANNELS)&&(RamPtr_pst->State_e != NM_STATE_UNINIT))
    {
        /* check if LinNm is in Bus Sleep State */
        if(RamPtr_pst->State_e == NM_STATE_BUS_SLEEP)
        {
            /* Set network status to request bus */
            LinNm_RamData_ast[LinNm_NetworkHandle_u8].NetworkReqState_e = LINNM_NETWORK_REQUESTED_E;

            /* Goto Bus Network Mode */
            LinNm_Prv_GotoNetworkMode(nmChannelHandle,RamPtr_pst);


            /* return successful transition */
            RetVal_u8 = E_OK;

         }
         else
         {

             /* service is rejected if the current state is not bus sleep*/
             RetVal_u8 = E_NOT_OK;
         }

    }
    else
    {

        /* service is rejected if the hanlde id is not within the specified limit or if the module is not initialised*/
        RetVal_u8 = E_NOT_OK;
    }


    return RetVal_u8;

 }

 /**********************************************************************************************************************
  Function name    : LinNm_NetworkRelease
  Syntax           : Std_ReturnType LinNm_NetworkRelease( NetworkHandleType nmChannelHandle )
  Description      : Release the network, since ECU does not want to communicate on the bus.
  Parameter        : nmChannelHandle - Identification of the NM-channel
  Return value     : Std_ReturnType   - E_OK: No error
                                     - E_NOT_OK: Releasing of network has failed

  **********************************************************************************************************************/

 Std_ReturnType LinNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{

    /* pointer to RAM data */
     LinNm_RamType_tst * RamPtr_pst;

    /* Return Value of the API */
    Std_ReturnType RetVal_u8;

    /* Variable to hold the Network Handle of LinNm */
    NetworkHandleType LinNm_NetworkHandle_u8;

    /*********************************************** End Of Declarations ****************************************/

    /* Receive the Internal LinNmChannel structure index from the received  ComM ChannelID*/
    LinNm_NetworkHandle_u8 = NM_LINNM_GET_HANDLE(nmChannelHandle);

    /* initialize the pointer to RAM data structure */
    RamPtr_pst = &LinNm_RamData_ast[LinNm_NetworkHandle_u8];

    /* check whether the channel handle is within the allowed range and LinNm is initialised*/
    if((LinNm_NetworkHandle_u8 < (uint8)NM_MAX_LINNM_CHANNELS)&&(RamPtr_pst->State_e != NM_STATE_UNINIT))
    {
        /* check if LinNm is Normal operation state */
        if(RamPtr_pst->State_e == NM_STATE_NORMAL_OPERATION)
        {
            /* Set network status to release bus */
            LinNm_RamData_ast[LinNm_NetworkHandle_u8].NetworkReqState_e = LINNM_NETWORK_RELEASED_E;

            /* Goto Bus Sleep without staying in Network Mode */
            LinNm_Prv_GotoBusSleepMode(nmChannelHandle,RamPtr_pst);


            /* return successful transition */
            RetVal_u8 = E_OK;

         }
        else
         {

            /* service is rejected if the current state is not normal operation state*/
            RetVal_u8 = E_NOT_OK;
         }

     }
     else
      {

           /* service is rejected if the hanlde id is not within the specified limit or if the module is not initialised*/
         RetVal_u8 = E_NOT_OK;
      }


      return RetVal_u8;

}
#endif

 /**********************************************************************************************************************
  Function name    : LinNm_GetState
  Syntax           : Std_ReturnType LinNm_GetState(
                                                    NetworkHandleType nmNetworkHandle
                                                   Nm_StateType*  nmStatePtr,
                                                   Nm_ModeType*  nmModePtr
                                                 )
  Description      : Returns the state of the network management.The function LinNm_GetState shall be called
                     (e.g. LinNm_GetState function is called if channel is configured as LIN).
  Parameter        : nmNetworkHandle - Identification of the NM-channel
                     nmStatePtr      - Pointer where state of the network management shall be copied to
                     nmModePtr       - Pointer to the location where the mode of the network management shall be copied to
  Return value     : Std_ReturnType   - E_OK: No error
                                       E_NOT_OK : Error
  **********************************************************************************************************************/

Std_ReturnType LinNm_GetState(NetworkHandleType nmNetworkHandle,
                                                      Nm_StateType * nmStatePtr,
                                                       Nm_ModeType * nmModePtr)
{

    /* pointer to RAM data */
    LinNm_RamType_tst * RamPtr_pst;

    /* Return Value of the API */
    Std_ReturnType RetVal_u8;

    /* Variable to hold the Network Handle of LinNm */
    NetworkHandleType LinNm_NetworkHandle_u8;

    /*********************************************** End Of Declarations ****************************************/

    /* Receive the Internal LinNmChannel structure index from the received  ComM ChannelID*/
    LinNm_NetworkHandle_u8 = NM_LINNM_GET_HANDLE(nmNetworkHandle);

    /* initialize the pointer to RAM data structure */
    RamPtr_pst = &LinNm_RamData_ast[LinNm_NetworkHandle_u8];

    /* check whether the channel handle is within the allowed range */
    if((LinNm_NetworkHandle_u8 < (uint8)NM_MAX_LINNM_CHANNELS) && (RamPtr_pst->State_e != NM_STATE_UNINIT) && (nmStatePtr != NULL_PTR) && (nmModePtr != NULL_PTR))
    {

        /* suspend interrupts to provide Data consistency */
        SchM_Enter_Nm_GetStateNoNest();

        /* copy the current LinNm state */
        *nmStatePtr  = LinNm_RamData_ast[LinNm_NetworkHandle_u8].State_e;

        /* copy the current LinNm mode */
        *nmModePtr   = LinNm_RamData_ast[LinNm_NetworkHandle_u8].Mode_e;

        /* enable interrupts */
        SchM_Exit_Nm_GetStateNoNest();


        /* return successful transition */
        RetVal_u8 = E_OK;

    }
    else
    {

        /* service is rejected */
        RetVal_u8 = E_NOT_OK;
    }


    return RetVal_u8;
}


/**
 ***************************************************************************************************
 Function name    : LinNm_Prv_ChangeState

 Description      : This is an internal function of LinNm. This function changes the state and mode of
                    LinNm without any data-inconsistency problem
 Parameter        : RamPtr_pst - pointer to the RAM data structure of the current channel
                  : nmState_e - state to be changed to
                  : nmMode_e - mode to be changed to
 Return value     : None
 ***************************************************************************************************
 */

LOCAL_INLINE void LinNm_Prv_ChangeState(LinNm_RamType_tst * RamPtr_pst, Nm_StateType nmState_en, Nm_ModeType nmMode_en)
{

    /* protect from interrupt to ensure data consistency between state and mode updates */
    SchM_Enter_Nm_ChangeStateNoNest();

    /* update state and modes */
    RamPtr_pst->State_e  =  nmState_en;

    RamPtr_pst->Mode_e   =  nmMode_en;

    /* unprotect */
    SchM_Exit_Nm_ChangeStateNoNest();

    return;
}

/*****************************************************************************************************
 * @ingroup LINNM_INL_H
 *
 *  This inline function computes the free running timer using Main Function period.            \n
 *
 *  @param    RamPtr_pst - pointer to the RAM structure                       \n
 *
 *  @return None \n
 *  *****************************************************************************************************
 */
LOCAL_INLINE void LinNm_Prv_ComputeSwFrTimer(LinNm_RamType_tst * RamPtr_pst)
{
    /* Timer to hold current time stamp of an event */
    /* Timer to hold previous time stamp of an event */
    /* return value of functions */
    /**** End Of Declarations ****/
    /* SW Free Running timer handling */
    /* read the current time */
    RamPtr_pst->ctSwFrTimer_u32 += LINNM_MAINFUNCTION_PERIOD;
}

/*****************************************************************************************************
 * @ingroup LINNM_INL_H
 *
 *  This inline function does transition to Bus Sleep mode from Network Mode.           \n
 *
 *  @param    RamPtr_pst - pointer to the RAM structure                       \n
 *
 *  @return None \n
 *  *****************************************************************************************************
 */

LOCAL_INLINE void LinNm_Prv_GotoBusSleepMode(const NetworkHandleType nmChannelHandle_cu8, LinNm_RamType_tst * RamPtr_pst)
{
    /* Go to Bus Sleep Mode */
    LinNm_Prv_ChangeState(RamPtr_pst, NM_STATE_BUS_SLEEP, NM_MODE_BUS_SLEEP);

    /* Notify to the upper layer about changes of the LinNm states if configuration parameter
                             LINNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
#if (LINNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
     Nm_StateChangeNotification(nmChannelHandle_cu8, NM_STATE_NORMAL_OPERATION, NM_STATE_BUS_SLEEP);
#endif

     /* indicate NmIf about Bus Sleep mode */
     Nm_BusSleepMode(nmChannelHandle_cu8);
}

/*****************************************************************************************************
 * @ingroup LINNM_INL_H
 *
 *  This inline function does transition to Network Mode from Bus Sleep Mode.           \n
 *
 *  @param    RamPtr_pst - pointer to the RAM structure                       \n
 *
 *  @return None \n
 *  *****************************************************************************************************
 */

LOCAL_INLINE void LinNm_Prv_GotoNetworkMode(const NetworkHandleType nmChannelHandle_cu8, LinNm_RamType_tst * RamPtr_pst)
{
    /* Go to Network Mode */
    LinNm_Prv_ChangeState(RamPtr_pst, NM_STATE_NORMAL_OPERATION, NM_MODE_NETWORK);

    /* Notify to the upper layer about changes of the LinNm states if configuration parameter
                       LINNM_STATE_CHANGE_IND_ENABLED is set to TRUE */
#if (LINNM_STATE_CHANGE_IND_ENABLED != STD_OFF)
    Nm_StateChangeNotification(nmChannelHandle_cu8, NM_STATE_BUS_SLEEP, NM_STATE_NORMAL_OPERATION);
#endif

    /* indicate NmIf about Network mode */
    Nm_NetworkMode(nmChannelHandle_cu8);
}



/**********************************************************************************************************************
 Function name    : LinNm_SetSleepReadyBit
 Syntax           : Std_ReturnType LinNm_SetSleepReadyBit( NetworkHandleType nmChannelHandle ,
                                                          boolean nmSleepReadyBit)
 Description      : Empty function to be compliant with NM specifications.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                     nmSleepReadyBit- Value written to ReadySleep Bit in CBV
 Return value     :  E_OK: No error

 **********************************************************************************************************************/
#if (LINNM_COORDINATOR_SYNC_SUPPORT_ENABLED != STD_OFF)
Std_ReturnType LinNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle,
                                                           boolean nmSleepReadyBit)
{

    /* This API is just a dummy to satisfy NM interface linkage. It shall always return E_OK. */
    (void)(nmChannelHandle);
    (void)(nmSleepReadyBit);

    return (E_OK);
}
#endif

#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
