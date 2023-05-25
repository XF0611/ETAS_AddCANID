

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "ComM_Priv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Inline functions declarations
 ***************************************************************************************************
 */

LOCAL_INLINE void ComM_Prv_ModeHandling(ComM_ChannelVarType_tst *     channelRamPtr_pst,
                                        const ComM_ChannelType_tst *  channelConfigPtr_pcst,
                                        ComM_ModeType                 channelMode_tu8,
                                        boolean                       entryToMode_b);

LOCAL_INLINE ComM_StateType ComM_Prv_NoComNoPending_StateHandling(ComM_RequestStructType_tst *  requestStructPtr_pst,
                                                                  ComM_StateType                previousstate_en);

LOCAL_INLINE ComM_StateType ComM_Prv_NoComPending_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                ComM_StateType               previousstate_en);

LOCAL_INLINE ComM_StateType ComM_Prv_FullComReadySleep_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                     ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                     const ComM_ChannelType_tst * channelConfigPtr_pcst,
                                                                     ComM_StateType               previousstate_en,
                                                                     uint8                        numStateChanges_u8);

LOCAL_INLINE ComM_StateType ComM_Prv_FullComNetworkReq_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                     ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                     const ComM_ChannelType_tst * channelConfigPtr_pcst,
                                                                     ComM_StateType               previousstate_en);

#if (COMM_NM_SUPPORT != STD_OFF)
LOCAL_INLINE ComM_StateType ComM_Prv_SilentCom_StateHandling(ComM_RequestStructType_tst *   requestStructPtr_pst,
                                                             ComM_ChannelVarType_tst *      channelRamPtr_pst,
                                                             const ComM_ChannelType_tst *   channelConfigPtr_pcst,
                                                             ComM_StateType                 previousstate_en);

#endif

LOCAL_INLINE ComM_ModeType ComM_Prv_getMode (ComM_StateType channelState_e);

LOCAL_INLINE void ComM_Prv_NotifyChannelMode (NetworkHandleType ChannelIndex_tu8);

#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
LOCAL_INLINE void ComM_Prv_CallRteWrite (uint8 ChannelIndex_u8 );
#endif
/* ComM_Prv_ChannelMainFunction */
/**************************************************************************************
 * Function name    : ComM_Prv_ChannelMainFunction
 * Syntax           : ComM_Prv_ChannelMainFunction
 *                    (
 *                    NetworkHandleType ChannelIndex_tu8
 *                    )
 * Description      : Function which contains different states and happening in various stages.
 * Parameters       : ChannelIndex_tu8
 * Return value     : void
 **************************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* HIS METRIC PATH, V(G), STMT VIOLATION IN ComM_Prv_ChannelMainFunction:
   Function involves optimal Channel state handlings. Splitting the function for HIS metric compliance would decrease
   readability and maintainability. */
void ComM_Prv_ChannelMainFunction(NetworkHandleType ChannelIndex_tu8)
{
    /* Local Variables Declaration */
    ComM_StateType              channelState_en;
    ComM_StateType              previousstate_en;
    ComM_ModeType               channelMode_tu8;
    ComM_ModeType               busSmMode_tu8;
    ComM_RequestStructType_tst  ComM_RequestStruct_st;

    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    ComM_RequestStructType_tst *    requestStructPtr_pst;
    uint8                           numStateChanges_u8;
    boolean                         stateChangePossible_b;
    boolean                         entryToMode_b;
    boolean                         userRequestState_b;
    boolean                         diagnosticRequestState_b;
    #if(COMM_PNC_ENABLED != STD_OFF)
    boolean                     pncRequestState_b;
    #endif

    #if(COMM_INHIBITION_ENABLED)
    ComM_GlobalVarType_tst *        globalRamPtr_pst;
    #endif

    /* Local Variables Initialization */
    stateChangePossible_b = FALSE;
    entryToMode_b = FALSE;
    numStateChanges_u8 = C_ZERO;
    #if(COMM_PNC_ENABLED != STD_OFF)
    pncRequestState_b = FALSE;
    #endif

    channelRamPtr_pst = &ComM_ChannelStruct[ChannelIndex_tu8];
    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[ChannelIndex_tu8]);

    requestStructPtr_pst = &ComM_RequestStruct_st;

    #if(COMM_INHIBITION_ENABLED)
    globalRamPtr_pst = &ComM_GlobalStruct;
    #endif  /*  (COMM_INHIBITION_ENABLED)  */
    /* DET Check for ComM initialization */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    if (ComM_GlobalStruct.ComM_InitStatus_en != COMM_INIT)
    {
        /* Report DET with ComM not initialized status */
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_MAINFNC_APIID,COMM_E_NOT_INITED);
        return;
    }
    #endif /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    /* COMM_DEV_ERROR_DETECT */

    /***************************SNAP SHOT OF EVENTS STARTS********************************/

#if CPU_TYPE == CPU_TYPE_8
    /* user-counter is 16-bit, hence locks are required only in case of 8-bit micro */
    SchM_Enter_ComM_UserNoNest();
#endif
    /* Evaluate if User request is present or not */
    userRequestState_b = (((channelRamPtr_pst->UserRequestCtr_u16) > C_ZERO_U16));
#if CPU_TYPE == CPU_TYPE_8
    SchM_Exit_ComM_UserNoNest();
#endif
    /* If inhibition is active, then depending on inhibition type and channel state,
     * reevaluate userRequestState_b */
    #if(COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)
    /* Ignore user request, if
     * 1. wake up inhibition is enabled in ECU group classification,
     * 2. Wake up inhibition is requested for channel, and
     * 3. channel is in NO COMMUNICATION or SILENT_COMMUNICATION mode */
    if((COMM_GET_PREVENT_WAKEUP(globalRamPtr_pst->ComM_EcuGroupClassification_u8)))
    {
        if((COMM_GET_PREVENT_WAKEUP(channelRamPtr_pst->InhibitionReqStatus_u8)) &&
                ((channelRamPtr_pst->ChannelMode_u8 == COMM_NO_COMMUNICATION)|| (channelRamPtr_pst->ChannelMode_u8 == COMM_SILENT_COMMUNICATION)))
        {
            userRequestState_b = FALSE;
        }
    }
    #endif  /*  (COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)  */
    #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
    /* Ignore user request if
     * 1. limit to no com is enabled in ECU group classification
     * 2. limit to no com is requested for the channel
     * irrespective of the channel state,
     * so as to ensure that the channel goes to NO_COMMUNICATION mode */
    if(COMM_GET_LIMIT_TO_NO_COM((globalRamPtr_pst->ComM_EcuGroupClassification_u8)))
    {
        if(COMM_GET_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8)))
        {
            userRequestState_b = FALSE;
        }
    }
    #endif

    /* Get the status of diagnostics request(DCM) */
    diagnosticRequestState_b = channelRamPtr_pst->DiagnosticRequestState_b;

    /* Requests from Nm and EcuM are considered as Passive requests */
    /* Passive request status is evaluated here */
    requestStructPtr_pst->PassiveRequest_b = ((channelRamPtr_pst->PassiveRequestState_u8) > C_ZERO);

    /* Request from User and DCM are considered as Active requests  */
    requestStructPtr_pst->ActiveRequest_b = (userRequestState_b || diagnosticRequestState_b);

    #if(COMM_PNC_ENABLED != STD_OFF)

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore
     * it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED)
    {
        pncRequestState_b = (channelRamPtr_pst->PncRequestCtr_u8 > C_ZERO);

        /* If PN is enabled, then request from PNC should also be considered to evaluate
         * active request */
        requestStructPtr_pst->ActiveRequest_b = (requestStructPtr_pst->ActiveRequest_b) || (pncRequestState_b);
    }
    #endif  /*  (COMM_PNC_ENABLED != STD_OFF)  */


    /* Snap shot of all requests till the execution of this main function ends here.
     * Any change in the requests beyond this point will be processed in the next main function*/
    /*******************************SNAP SHOT OF EVENTS ENDS***********************************/

    /* Reading the status from channel RAM structure */

    channelState_en = channelRamPtr_pst->ChannelState_e ;

    channelMode_tu8 = channelRamPtr_pst->ChannelMode_u8 ;

    busSmMode_tu8 = channelRamPtr_pst->BusSmMode_u8;

    previousstate_en = channelState_en;

    /* Reading the status from channel configuration structure */

    /*********************************************STATE MACHINE STARTS HERE********************************************/
    /* Channel State machine handling considering only the snap shot of requests */
    /* All the possible state changes are done in this loop*/
    do
    {
        /* Call mode handling function, to
        * 1. Perform mode related events
        * 2. Poll and Retry : Check BusSm state and Request BusSm to go to respective mode */

        ComM_Prv_ModeHandling(channelRamPtr_pst,channelConfigPtr_pcst, channelMode_tu8, entryToMode_b);

        switch (channelState_en)
        {
            case COMM_NO_COM_NO_PENDING_REQUEST:
            {
                channelState_en = ComM_Prv_NoComNoPending_StateHandling(requestStructPtr_pst, previousstate_en);
            }
            break;
            case COMM_NO_COM_REQUEST_PENDING:
            {
                channelState_en = ComM_Prv_NoComPending_StateHandling(requestStructPtr_pst, channelRamPtr_pst, previousstate_en);
            }
            break;
            case COMM_FULL_COM_NETWORK_REQUESTED:
            {
                channelState_en = ComM_Prv_FullComNetworkReq_StateHandling(requestStructPtr_pst, channelRamPtr_pst,
                                                                                 channelConfigPtr_pcst, previousstate_en);
            }
            break;
            case COMM_FULL_COM_READY_SLEEP:
            {
                channelState_en = ComM_Prv_FullComReadySleep_StateHandling(requestStructPtr_pst, channelRamPtr_pst,
                                        channelConfigPtr_pcst, previousstate_en, numStateChanges_u8);
            }
            break;
            #if (COMM_NM_SUPPORT != STD_OFF)
            case COMM_SILENT_COM:
            {
                channelState_en = ComM_Prv_SilentCom_StateHandling(requestStructPtr_pst, channelRamPtr_pst, channelConfigPtr_pcst, previousstate_en);
            }
            break;
            #endif  /*  (COMM_NM_SUPPORT != STD_OFF)  */
            default:
                /* Intentionally Empty */
            break;
        }

        /* Check if state change is possible */
        if(channelRamPtr_pst->ChannelState_e != channelState_en)
        {
            SchM_Enter_ComM_ChannelNoNest();
            /* update the channel state to global structure*/
            previousstate_en = channelRamPtr_pst->ChannelState_e;
            channelRamPtr_pst->ChannelState_e = channelState_en;

            stateChangePossible_b = TRUE;
            numStateChanges_u8++;

            /* Translate state to mode */
            channelMode_tu8 = ComM_Prv_getMode (channelState_en);

            /*Check if this state change leads to mode change*/
            if(channelMode_tu8 != channelRamPtr_pst->ChannelMode_u8)
            {
                /* As state change leads to mode change, set entryToMode_b flag and update channel mode */
                entryToMode_b = TRUE;
                channelRamPtr_pst->ChannelMode_u8 = channelMode_tu8;
            }
            else
            {
                /* State change doesnt lead to mode change. Hence clear the entryToMode flag */
                entryToMode_b = FALSE;
            }
            SchM_Exit_ComM_ChannelNoNest();
        }
        else
        {
            entryToMode_b = FALSE;
            stateChangePossible_b = FALSE;
        }

    } while (stateChangePossible_b != FALSE);

    /*********************************************STATE MACHINE ENDS HERE*********************************************/

    /* If there is a change in BusSm mode,
     * 1. Notify the change in the mode to BSWM and DCM
     * 2. Reevaluate channel state counters for all users of the channel
     * 3. Evaluate if there is a change in leastBusSmMode for any user and
     * if there is a change, notify it to RTE
     */

    /* busSmMode_tu8 contains the mode of BusSm on entry to state machine and
     * channelRamPtr_pst->BusSmMode_u8 contains latest BusSm mode on exit of state machine */

    if(busSmMode_tu8 != channelRamPtr_pst->BusSmMode_u8)
    {

        /* Notify the change to BswM,DCM (per channel) */
        ComM_Prv_NotifyChannelMode(ChannelIndex_tu8);

        /* Reevaluate counters for all users of channel and notify the change in least com mode to RTE*/
        ComM_Prv_UpdateChannelModes(ChannelIndex_tu8,busSmMode_tu8,channelRamPtr_pst->BusSmMode_u8);

    }

    /*If a FullComRequestenabled is set to true then call a Internal function
     * to generate RTE write function based on the Channel.Clear the flag*/

#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
    if(channelRamPtr_pst->ComMFullComReqNotify_b==TRUE)
    {
        ComM_Prv_CallRteWrite(ChannelIndex_tu8);
        channelRamPtr_pst->ComMFullComReqNotify_b= FALSE;
    }
#endif

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/* ComM_Prv_NoComNoPending_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_NoComNoPending_StateHandling
 * Syntax           : ComM_Prv_NoComNoPending_StateHandling
 *                    (
 *                    ComM_RequestStructType_tst * requestStructPtr_pst,
                      ComM_StateType previousstate_en
 *                    )
 * Description      : Internal function for handling COMM_NO_COM_NO_PENDING_REQUEST state
 * Parameters       : requestStructPtr_ps,previousstate_en
 * Return value     : ComM_StateType
 **************************************************************************************/
/* MR12 RULE 8.13 VIOLATION: The requestStructPtr_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE ComM_StateType ComM_Prv_NoComNoPending_StateHandling(ComM_RequestStructType_tst *  requestStructPtr_pst,
                                                                  ComM_StateType                previousstate_en)
{
    ComM_StateType channelState_en;

    channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;

    if (previousstate_en != COMM_NO_COM_NO_PENDING_REQUEST )
    {
        /* HALT STATE : entry to the state is not evaluated here */
    }

    /* Cyclic part of state COMM_NO_COM_NO_PENDING_REQUEST */
    /*For NO_com_No_Pending state Prioritiy of requests is
    *1. Active Request or Passive Request*/
    /* Evaluate if any request (Active or Passive) is present*/
    else
    {
        if ((requestStructPtr_pst->ActiveRequest_b != FALSE) || (requestStructPtr_pst->PassiveRequest_b != FALSE))
        {
            /* Update the channel state to the global structure*/
            channelState_en = COMM_NO_COM_REQUEST_PENDING;
        }
        else
        {
            /* State change is not possible, as there is no request.
             * Channel continues to be in COMM_NO_COM_NO_PENDING_REQUEST state */
        }
    }
    /* Exit of state COMM_NO_COM_NO_PENDING_REQUEST*/
    /* There is no dependent flags which has to be cleared as this is default state. Hence there is nothing to be
     * taken care in this state */
    return channelState_en;

}


/* ComM_Prv_NoComPending_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_NoComPending_StateHandling
 * Syntax           : ComM_Prv_NoComPending_StateHandling
 *                    (
 *                    ComM_RequestStructType_tst * requestStructPtr_pst,
                      ComM_ChannelVarType_tst * channelRamPtr_pst
                      ComM_StateType previousstate_en
 *                    )
 * Description      : Internal function for handling COMM_NO_COM_PENDING_REQUEST state
 * Parameters       : requestStructPtr_pst, channelRamPtr_pst, previousstate_en
 * Return value     : ComM_StateType
 **************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The requestStructPtr_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE ComM_StateType ComM_Prv_NoComPending_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                ComM_StateType               previousstate_en)
{

    ComM_StateType channelState_en;

    channelState_en = COMM_NO_COM_REQUEST_PENDING;

    /* Entry to state COMM_NO_COM_REQUEST_PENDING */
    if (previousstate_en != COMM_NO_COM_REQUEST_PENDING )
    {
        /*Evaluate Communication allowed flag and decide if state change is possible */
        if (channelRamPtr_pst->CommunicationAllowed_b != FALSE)
        {
            /* Communication Allowed is set, so proceed to COMM_FULL_COM_NETWORK_REQUESTED */
            /* As transition from COMM_NO_COM_REQUEST_PENDING to COMM_FULL_COM_NETWORK_REQUESTED*/
            channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;
        }
    }
    /* Cyclic part of state COMM_NO_COM_REQUEST_PENDING */
    else
    {
        /*Check if request for Full_Communication (active or passive request) still exists */
        if ((requestStructPtr_pst->ActiveRequest_b || requestStructPtr_pst->PassiveRequest_b))
        {
            if ((channelRamPtr_pst->CommunicationAllowed_b) != FALSE)
            {
                /* Request for Full_Com is still present and CommunicationAllowed is also set,
                 * so proceed to COMM_FULL_COM_NETWORK_REQUESTED state*/
                channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;
            }
            else
            {
                /* Wait for communication allowed to be set to True */
            }
        }
        else
        {
            channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
        }
    }

    /* Exit of state COMM_NO_COM_REQUEST_PENDING*/

    /* As passiveRequest is processed, clear the flag */
    if(channelState_en != COMM_NO_COM_REQUEST_PENDING)
    {
        /*As Passive requests are already processed, clear the flags */
        channelRamPtr_pst->PassiveRequestState_u8 = C_ZERO;
    }
    return channelState_en;

}

/* ComM_Prv_FullComNetworkReq_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_FullComNetworkReq_StateHandling
 * Syntax           : ComM_Prv_FullComNetworkReq_StateHandling
 *                    (
 *                      ComM_RequestStructType_tst * requestStructPtr_pst,
                        ComM_ChannelVarType_tst * channelRamPtr_pst,
                        const ComM_ChannelType_tst * channelConfigPtr_pcst,
                        ComM_StateType previousstate_en
 *                    )
 * Description      : Internal function for handling COMM_FULL_COM_NETWORK_REQUESTED state
 * Parameters       : requestStructPtr_pst, channelRamPtr_pst, channelConfigPtr_pcst, previousstate_en
 * Return value     : ComM_StateType
 **************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The requestStructPtr_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE ComM_StateType ComM_Prv_FullComNetworkReq_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                     ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                     const ComM_ChannelType_tst * channelConfigPtr_pcst,
                                                                     ComM_StateType               previousstate_en)
{

    ComM_StateType channelState_en;

    channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;

    /* Entry to state COMM_FULL_COM_NETWORK_REQUESTED */
    if (previousstate_en != COMM_FULL_COM_NETWORK_REQUESTED )
    {
        /* Depending on NmVariant perform the entry actions */
        switch (channelConfigPtr_pcst->ComMNmVariant_en)
        {
            /* ComMNmVariant : FULL || PASSIVE -> Depending on request type (Active or Passive) and ComMNmVariant
            (FULL or PASSIVE) either Network request has to be called or state change to ready sleep has to be done */

            #if(COMM_NM_SUPPORT != STD_OFF)
            #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)
            /* ComMNmVariant : FULL, if active request exists, then NetworkRequest has to be called
             * otherwise, transition to ready sleep has to be made */
            case (FULL):
            {
                if(requestStructPtr_pst->ActiveRequest_b)
                {
                    /* Active request is present, hence request network */
                    (void)Nm_NetworkRequest(channelConfigPtr_pcst->ComMChannelId_u8);
                    #if(COMM_PNC_ENABLED != STD_OFF)

                    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration
                     * therefore it will not always be true. */
                    if(COMM_GET_FEATURE_PNC_ENABLED)
                    {
                        /* Nm frame containing pn info will be transmitted before the nm message cycle time elapses.
                         * ComM_Prv_LPncToChannelInterface() will set channelRamPtr_pst ->NmNetworkRequestStatus_b
                         *  if ComMPncNmRequest is set to TRUE*/
                        /* Lock is not required here as the variable is set in pnc main function and comm mainfunction
                         * clears it and it is scheduled after pnc */
                        channelRamPtr_pst ->NmNetworkRequestStatus_b = FALSE;
                    }
                    #endif  /*  (COMM_PNC_ENABLED != STD_OFF)  */
                }
                else
                {
                    /* As there is no active request,proceed directly to Ready sleep as there is no active request present */
                    channelState_en = COMM_FULL_COM_READY_SLEEP;
                }
            }
            break;
            #endif/*#if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)*/

            /* For ComMNmVariant : PASSIVE, there cannot be active request, hence change to Ready sleep directly */
            case (PASSIVE):
            {
                /* As the Nm variant is passive, proceed directly to Ready sleep state */
                channelState_en = COMM_FULL_COM_READY_SLEEP;
            }
            break;
            #endif  /*  #if (COMM_NM_SUPPORT != STD_OFF)  */

            /* ComMNmVariant : LIGHT || NONE -> minimum full com duration timer has to be started */
            case LIGHT:
            case NONE:
            {
                /*Start MinFullComDuration timer*/
                (channelRamPtr_pst->MinFullComTimeoutCtr_u16) = (channelConfigPtr_pcst->TMinFullComModeDuration_u16) ;
            }
            break;

            default :
                /* Intentionally Empty */
            break;

        }
    }
    /* Cyclic part of state COMM_FULL_COM_NETWORK_REQUESTED */
    else
    {
        switch (channelConfigPtr_pcst->ComMNmVariant_en)
        {
            /* If ComM Nm variant PASSIVE, control never comes to cyclic part of COMM_FULL_COM_NETWORK_REQUESTED
             * state.State immediately changes to Ready sleep state from entry of COMM_FULL_COM_NETWORK_REQUESTED
             * state
             * */
            #if ((COMM_NM_SUPPORT != STD_OFF) && (COMM_NM_VARIANT == COMM_NM_VARIANT_FULL))
            case (FULL):
            {
                /* For nmVariant FULL , priority of requests is
                 *             1. Bus sleep indication
                 *             2. Active request
                 *             proceeed to  ready sleep
                 * Process these requests according to above priority */
                /* If bus sleep indication has occurred, proceed to COMM_NO_COM_NO_PENDING_REQUEST state */
                if((channelRamPtr_pst->NmBusSleepIndicationStatus_b) != FALSE)
                {
                    channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                }
                else if(requestStructPtr_pst->ActiveRequest_b != FALSE)
                {
                    #if(COMM_PNC_ENABLED != STD_OFF)

                    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration
                     * therefore it will not always be true. */
                    if(COMM_GET_FEATURE_PNC_ENABLED)
                    {
                        if((channelRamPtr_pst->NmNetworkRequestStatus_b) != FALSE)
                        {
                            (void)Nm_NetworkRequest(channelConfigPtr_pcst->ComMChannelId_u8);
                            /* Lock is not required here as the variable is set in pnc main function and comm mainfunction
                             * clears it and it is scheduled after pnc */
                            channelRamPtr_pst ->NmNetworkRequestStatus_b = FALSE;
                        }
                    }
                    #endif /* #if(COMM_PNC_ENABLED != STD_OFF)  */
                    /* Remain in COMM_FULL_COM_NETWORK_REQUESTED state as long as active request is present */
                }
                else
                {
                    /* As there is no active request, proceed to Ready sleep state */
                    channelState_en = COMM_FULL_COM_READY_SLEEP;
                }
            }
            break;
            #endif  /*  (COMM_NM_SUPPORT != STD_OFF) && (COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)  */

            case LIGHT:
            case NONE :
            {
                /*This check is to ensure we do not decrement the timer when it is zero */
                /*Priority is given to active request when timer expires and there is an active request*/
                if((channelRamPtr_pst->MinFullComTimeoutCtr_u16) != C_ZERO)
                {
                    /* Decrement Minimum full com duration timer*/
                    -- (channelRamPtr_pst->MinFullComTimeoutCtr_u16) ;
                }

                if((channelRamPtr_pst->MinFullComTimeoutCtr_u16) == C_ZERO)
                {
                    if(requestStructPtr_pst->ActiveRequest_b != FALSE)
                    {
                        /* Even though timer expired,as there is active request,COMM_FULL_COM_NETWORK_REQUESTED
                         * state has to be retained  */
                    }
                    else
                    {
                        /* Timer expired and there is no active request, so proceed to Ready sleep state */
                        channelState_en = COMM_FULL_COM_READY_SLEEP;
                    }
                }
            }
            break;

            default :
                /* Intentionally Empty */
            break;
        }
    }

    /* Exit of state COMM_FULL_COM_NETWORK_REQUESTED*/
    if(channelState_en == COMM_NO_COM_NO_PENDING_REQUEST)
    {
        /* Clear the bus sleep indication flag as it is already processed.
           If NmBusSleepIndicationStatus_b is set and processed, then channelState_en will be
           COMM_NO_COM_NO_PENDING_REQUEST. Hence the flag is cleared only in this condition */
        #if(COMM_NM_SUPPORT != STD_OFF)
        channelRamPtr_pst->NmBusSleepIndicationStatus_b = FALSE ;
        #endif  /*  (COMM_NM_SUPPORT != STD_OFF)  */
    }

    return channelState_en;
}


/* ComM_Prv_FullComReadySleep_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_FullComReadySleep_StateHandling
 * Syntax           : ComM_Prv_FullComReadySleep_StateHandling
 *                    (
 *                      ComM_RequestStructType_tst * requestStructPtr_pst,
                        ComM_ChannelVarType_tst * channelRamPtr_pst,
                        const ComM_ChannelType_tst * channelConfigPtr_pcs,
                        ComM_StateType previousstate_en,
                        uint8 numStateChanges_u8
 *                    )
 * Description      : Internal function for handling COMM_FULL_COM_READY_SLEEP state
 * Parameters       : requestStructPtr_pst, channelRamPtr_pst,,previousstate_en,numStateChanges_u8, previousstate_en
 * Return value     : ComM_StateType
 **************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The requestStructPtr_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
/* HIS METRIC PATH, V(G), STMT VIOLATION IN ComM_Prv_FullComReadySleep_StateHandling:
   The function involves optimal  COMM_FULL_COM_READY_SLEEP state handlings through simple if and switch cases.
   Violation of HIS metric compliance has no critical functional impact */
LOCAL_INLINE ComM_StateType ComM_Prv_FullComReadySleep_StateHandling(ComM_RequestStructType_tst * requestStructPtr_pst,
                                                                     ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                                                     const ComM_ChannelType_tst * channelConfigPtr_pcst,
                                                                     ComM_StateType               previousstate_en,
                                                                     uint8                        numStateChanges_u8)

{
    ComM_StateType channelState_en;

    channelState_en = COMM_FULL_COM_READY_SLEEP;


    /* Entry to state COMM_FULL_COM_READY_SLEEP */
    if (previousstate_en != COMM_FULL_COM_READY_SLEEP )
    {
        /* Entry to state COMM_FULL_COM_READY_SLEEP */
        if(previousstate_en == COMM_FULL_COM_NETWORK_REQUESTED)
        {
            switch(channelConfigPtr_pcst->ComMNmVariant_en)
            {
                #if(COMM_NM_SUPPORT != STD_OFF)
                /* Entry to ready sleep state from Network Requested and from Silent Communication has to be differentiated
                 * On Entry to Ready sleep from Network requested, Nm api's needs to be called if Nm is present
                 * but on entry to Ready sleep from Silent Com, No Nm api's needs to be called, as silent com ->
                 * Ready sleep is only triggered by Nm (Nm_NetworkMode) */
                #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)
                case FULL :
                {
                    /* Check if Network request was already called, If so, network released has to be called
                    Otherwise, PassiveStartUp has to be called */

                    /* Whether NetworkRequested was called or not is determined by using numStateChanges_u8
                     * If NetworkRequested was called, state change from Network requested to ready sleep doesn't
                     * happen immediately. If it was not called, it happens immediately and hence the numStateChanges_u8
                     * will be >= 2*/
                    /* There is two state transitions from no com to full com in the same main function when it is passive request for full_com*/
                    if(numStateChanges_u8 >=2)
                    {
                        /* As the entry is because of Passive request, call Nm_PassiveStartUp() */
                        (void)Nm_PassiveStartUp(channelConfigPtr_pcst->ComMChannelId_u8);
                    }
                    else
                    {
                        /* As NetworkRequest was already called, call Nm_NetworkRelease() */
                        (void)Nm_NetworkRelease(channelConfigPtr_pcst->ComMChannelId_u8);
                    }
                }
                break;
                #endif/*  #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)  */
                case PASSIVE :
                {
                    /* As Nm variant is passive, always Nm_PassiveStartUp() has to be called */
                    (void)Nm_PassiveStartUp(channelConfigPtr_pcst->ComMChannelId_u8);
                }
                break;
                #endif  /*  (COMM_NM_SUPPORT != STD_OFF)  */

                /* For other Nm variants (Light and None), silent Com state doesn't exist and
                 * hence entry need not be differentiated*/
                case LIGHT:
                {
                    /* Start NmLightTimeOut timer */
                    channelRamPtr_pst->LightTimeoutCtr_u32 = channelConfigPtr_pcst->NmLightTimeout_u32;
                    /* Check if NmLightTimeOut timer expired i.e. check if value configured for NmLightTimeout timer is zero,
                     * then proceed directly to no_com_no_pending state*/
                    if(channelRamPtr_pst->LightTimeoutCtr_u32 == C_ZERO)
                    {
                        /* As NmLightTimeOut timer has expired,change to COMM_NO_COM_NO_PENDING_REQUEST state */
                        channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                    }
                }
                break;

                case NONE:
                {
                    if(channelConfigPtr_pcst->BusType_en == COMM_BUS_TYPE_INTERNAL)
                    {
                        /* For bus type internal, immediate transition to COMM_NO_COM_NO_PENDING_REQUEST state */
                        channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                    }
                    else
                    {
                        /*For Bus types other than internal,if NM variant is NONE channel will remain in Ready sleep state.
                         * It goes to No Com state only on power off */
                    }
                }
                break;

                default:
                    /* Intentionally Empty */
                break;
            }
        }
    }

    /* Cyclic part of state COMM_FULL_COM_READY_SLEEP */
    else
    {
        switch (channelConfigPtr_pcst->ComMNmVariant_en)
        {
            #if(COMM_NM_SUPPORT != STD_OFF)
            /* For nmVariant FULL, priority of requests is
             *             1. Bus sleep indication
             *             2. Active request
             *             3. Prepare bus sleep indication
             * Process these requests according to above priority */
            #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)
            case FULL :
            {
                if (channelRamPtr_pst->NmBusSleepIndicationStatus_b != FALSE)
                {
                    channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                }
                else if(requestStructPtr_pst->ActiveRequest_b)
                {
                    /* For nmVariant Passive, active request will not affect the state,
                     * channel continues in Ready sleep state * */
                    channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;
                }
                else if (channelRamPtr_pst->NmPrepareBusSleepIndicationStatus_b != FALSE)
                {
                    channelState_en = COMM_SILENT_COM;
                }
                else
                {
                    /* Do nothing */
                }
            }
            break;
            #endif /* #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL) */

            /* For nmVariant PASSIVE, priority of requests is
             *             1. Bus sleep indication
             *             2. Prepare bus sleep indication
             * Process these requests according to above priority */
            case PASSIVE :
            {
                if (channelRamPtr_pst->NmBusSleepIndicationStatus_b != FALSE)
                {
                    channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                }
                else if (channelRamPtr_pst->NmPrepareBusSleepIndicationStatus_b != FALSE)
                {
                    channelState_en = COMM_SILENT_COM;
                }
                else
                {
                    /* Do nothing */
                }
            }
            break;
            #endif  /*  (COMM_NM_SUPPORT != STD_OFF)  */

            /* For nmVariant Light, priority of requests is
             *             1. Active request
             *             2. Expire of LightTimeOutTimer
             * These requests are processed according to above priority*/
            case LIGHT :
            {
                if(requestStructPtr_pst->ActiveRequest_b != FALSE)
                {
                    channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;
                }
                else
                {
                    if((channelRamPtr_pst->LightTimeoutCtr_u32) != C_ZERO )
                    {
                        --channelRamPtr_pst->LightTimeoutCtr_u32;
                        if((channelRamPtr_pst->LightTimeoutCtr_u32) == C_ZERO )
                        {
                            channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
                        }
                        else
                        {
                            /* Wait till Light time out time expires */
                        }
                    }
                }
            }
            break;

            case NONE :
            {
                /* If Nm variant is None, nothing needs to be done in "continuous" state of COMM_FULL_COM_READY_SLEEP
                 * state, as NmVariant NONE for channel other than internal, leaves the channel is FULL_COM for ever */
            }
            break;

            default:
                /* Intentionally Empty */
            break;

        }
    }

    /* Exit of state COMM_FULL_COM_READY_SLEEP*/
    if(channelState_en != COMM_FULL_COM_READY_SLEEP)
    {
        SchM_Enter_ComM_ChannelNoNest();
        /* Since the flag NmPrepareBusSleepIndicationStatus can be set in COMM_FULL_COM_READY_SLEEP, clear as the state is exited  */
        /*If this flag is not cleared then if the next state is n/w requested, later channel  should switch to silent_com*/
        #if(COMM_NM_SUPPORT != STD_OFF)
        channelRamPtr_pst->NmPrepareBusSleepIndicationStatus_b = FALSE ;

        if(channelState_en == COMM_NO_COM_NO_PENDING_REQUEST)
        {
            /* Clear the bus sleep indication flag as it is already processed.
               If NmBusSleepIndicationStatus_b is set and processed, then channelState_en will be COMM_NO_COM_NO_PENDING_REQUEST.
               Hence the flag is cleared only in this condition  */
            channelRamPtr_pst->NmBusSleepIndicationStatus_b = FALSE ;
        }

        #endif  /*  (COMM_NM_SUPPORT != STD_OFF)  */
        SchM_Exit_ComM_ChannelNoNest();
    }

    return channelState_en;

}

/* ComM_Prv_SilentCom_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_SilentCom_StateHandling
 * Syntax           : ComM_Prv_SilentCom_StateHandling
 *                    (
 *                    ComM_RequestStructType_tst * requestStructPtr_pst,
                      ComM_ChannelVarType_tst * channelRamPtr_pst
                      ComM_StateType previousstate_e previousstate_e
 *                    )
 * Description      : Internal function for handling COMM_SILENT_COM state
 * Parameters       : requestStructPtr_pst, channelRamPtr_pst,  previousstate_e
 * Return value     : ComM_StateType
 **************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The requestStructPtr_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE ComM_StateType ComM_Prv_SilentCom_StateHandling(ComM_RequestStructType_tst *   requestStructPtr_pst,
                                                             ComM_ChannelVarType_tst *      channelRamPtr_pst,
                                                             const ComM_ChannelType_tst *   channelConfigPtr_pcst,
                                                             ComM_StateType                 previousstate_en)
{
    ComM_StateType channelState_en;


    channelState_en = COMM_SILENT_COM;

    /* Entry to state COMM_FULL_COM_NETWORK_REQUESTED */
    if (previousstate_en != COMM_SILENT_COM )
    {
        /* HALT STATE : entry to the state is not evaluated here */
    }
    /* Cyclic part of COMM_SILENT_COM mode*/
    else
    {
        /* BusSleepIndication is given priority over Full_Com request, Full Com request if any will be processed in the
         * next main function call */
        /* Priority for comMNmVariant "FULL"
         *             1. BusSleepIndication
         *             2. Active request
         *             3. Network Mode indication */

        /* Priority for comMNmVariant "PASSIVE"
         *             1. BusSleepIndication
         *             2. Network Mode indication */
        if ((channelRamPtr_pst->NmBusSleepIndicationStatus_b) != FALSE)
        {
            channelState_en = COMM_NO_COM_NO_PENDING_REQUEST;
        }
        /* Active request in Silent com state is handled only in case of comMNmVariant "FULL".
         * For comMNmVariant "PASSIVE", active request is ignored, as network cannot be requested actively
         * for a passive channel*/
        else if ((channelConfigPtr_pcst->ComMNmVariant_en == FULL) && (requestStructPtr_pst->ActiveRequest_b != FALSE))
        {
            channelState_en = COMM_FULL_COM_NETWORK_REQUESTED;
        }
        else if ((channelRamPtr_pst->NmNetworkModeStatus_b) != FALSE)
        {
            channelState_en = COMM_FULL_COM_READY_SLEEP;
        }
        else
        {
            /* Continue in Silent Communication state as there is no new request */
        }
    }

    /* Exit of state COMM_SILENT_COM */
    if(channelState_en != COMM_SILENT_COM)
    {
        SchM_Enter_ComM_ChannelNoNest();
        channelRamPtr_pst->NmNetworkModeStatus_b = FALSE;
        channelRamPtr_pst->NmBusSleepIndicationStatus_b = FALSE;
        SchM_Exit_ComM_ChannelNoNest();
    }

    return channelState_en;
}


/* ComM_LBusSm */
/**************************************************************************************
 * Function name    : ComM_Prv_ModeHandling
 * Syntax           : ComM_Prv_ModeHandling
 *                    (
*                        ComM_ChannelVarType_tst * channelRamPtr_pst,
                         const ComM_ChannelType_tst * channelConfigPtr_pcst,
                         ComM_ModeType channelMode_e ,
                         boolean entryToMode_b
*                     )
 * Description      : Internal function to request <Bus>Sm
 * Parameters       : ComM_ChannelVarType_tst * channelRamPtr_pst
                      boolean entryToMode_b,
                      const ComM_ChannelType_tst * channelConfigPtr_pcst,
                      ComM_ModeType channelMode_e
 * Return value     : void
 **************************************************************************************/

LOCAL_INLINE void ComM_Prv_ModeHandling(ComM_ChannelVarType_tst *    channelRamPtr_pst,
                                        const ComM_ChannelType_tst * channelConfigPtr_pcst,
                                        ComM_ModeType                channelMode_tu8 ,
                                        boolean                      entryToMode_b)
{
    boolean                         requestBusSm_b;
    ComM_ModeType                   busSmMode_tu8;
    const ComM_BusSmApiType_tst *   busSm_funcPtr;

    busSmMode_tu8 = channelRamPtr_pst->BusSmMode_u8;
    busSm_funcPtr = &ComM_BusSmApi_acst[channelConfigPtr_pcst->BusType_en];
    requestBusSm_b = FALSE;

    if (entryToMode_b != FALSE)
    {
        requestBusSm_b = TRUE;

        /* If Nm variant is FULL, Call Nm_NetworkRelease */

        if(channelMode_tu8 == COMM_NO_COMMUNICATION)
        {
            #if (COMM_NM_SUPPORT != STD_OFF)
            #if(COMM_NM_VARIANT == COMM_NM_VARIANT_FULL)
            if (channelConfigPtr_pcst->ComMNmVariant_en == FULL)
            {
                (void)Nm_NetworkRelease(channelConfigPtr_pcst->ComMChannelId_u8);
            }
            #endif/* #if(COMM_NM_VARIANT ==COMM_NM_VARIANT_FULL)*/
            #endif/*#if (COMM_NM_SUPPORT != STD_OFF)*/
        }
    }
    /* Cyclic part of mode COMM_NO_COMMUNICATION */
    else
    {
        /* POLL and RETRY mechanism : BusSm is requested for same mode as that of ComM, if it is not already in
         * the same mode. Continuous polling of BusSm mode is done */

        /* POLLING : Poll for the latest mode of BusSm */
        (void)(*busSm_funcPtr->BusSm_GetCurrentComMode)(channelConfigPtr_pcst->ComMChannelId_u8, &busSmMode_tu8);

        /* Update the latest busSm mode to the global variable  */
        channelRamPtr_pst->BusSmMode_u8 = busSmMode_tu8 ;

        /* RETRY : request BusSm for the same mode as that of ComM  */
        if(busSmMode_tu8 != channelMode_tu8)
        {
            requestBusSm_b = TRUE;
        }

        if(channelMode_tu8 == COMM_NO_COMMUNICATION)
        {
            /* If Reset after forcing to No Com is enabled, then once the busSm enters NO_COMMUNICATION mode
             * reset has to be triggered by calling BswM */
            #if((COMM_RESET_AFTER_NO_COM != STD_OFF) && (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF))
            if((COMM_GET_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8)) != FALSE) &&
               (channelRamPtr_pst->BusSmMode_u8 == channelMode_tu8))
            {
                BswM_ComM_InitiateReset();
            }
            #endif
        }
    }

    /* Request BusSm */
    if(requestBusSm_b)
    {
        Std_ReturnType RetValue;

        /* RequestBusSm for mode change depending on Bus type*/
        RetValue = (*busSm_funcPtr->BusSm_RequestComMode)(channelConfigPtr_pcst->ComMChannelId_u8,channelMode_tu8);

        #if (COMM_DEV_ERROR_DETECT != STD_OFF)
        if(RetValue == E_NOT_OK)
        {
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_MAINFNC_APIID,COMM_E_ERROR_IN_PROV_SERVICE);
        }
        #else
        (void)(RetValue);
        #endif  /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    }

}

/* ComM_NotifyChannelMode */
/**************************************************************************************
 * Function name    : ComM_Prv_NotifyChannelMode
 * Syntax           : ComM_Prv_NotifyChannelMode
 *                    (
 *                     NetworkHandleType ChannelIndex_tu8
 *                    )
 * Description      : Internal function to notify change in the channel mode to DCM and BSWM
 * Parameters       : ChannelIndex_tu8 : Channel Id
 * Return value     : void
 **************************************************************************************/

LOCAL_INLINE void ComM_Prv_NotifyChannelMode(NetworkHandleType ChannelIndex_tu8)
{

    /* Local variable declaration */
    ComM_ChannelVarType_tst *   channelRamPtr_pst;
    ComM_ModeType               channelMode_tu8;

    channelRamPtr_pst = &ComM_ChannelStruct[ChannelIndex_tu8];

    /* Latest BusSm Mode is copied on to the local variable */
    channelMode_tu8 = channelRamPtr_pst->BusSmMode_u8;

    /* Notify BswM and DCM regarding Mode change  */
    BswM_ComM_CurrentMode(ChannelIndex_tu8,channelMode_tu8);

    #if (COMM_DCM_SUPPORT != STD_OFF)

    /* Depending on the channel mode, call respective DCM function to notify the mode change  */

    switch(channelMode_tu8)
    {
        case COMM_NO_COMMUNICATION : Dcm_ComM_NoComModeEntered(ChannelIndex_tu8);
            break;
        case COMM_FULL_COMMUNICATION : Dcm_ComM_FullComModeEntered(ChannelIndex_tu8);
            break;
        case COMM_SILENT_COMMUNICATION : Dcm_ComM_SilentComModeEntered(ChannelIndex_tu8);
            break;
        default:
            /* Intentionally Empty */
            break;
    }

    #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
}


/* ComM_Prv_getMode */
/**************************************************************************************
 * Function name    : ComM_Prv_getMode
 * Syntax           : ComM_Prv_getMode
 *                    (
 *                    ComM_StateType ChannelState_e
 *                    )
 * Description      : Function to give mode corresponding to a state
 * Parameters       : ChannelState_e
 * Return value     : ComM_ModeType
 **************************************************************************************/

LOCAL_INLINE ComM_ModeType ComM_Prv_getMode (ComM_StateType channelState_e)
{
    ComM_ModeType channelMode_tu8;
    channelMode_tu8 = COMM_NO_COMMUNICATION;

    switch(channelState_e)
    {
        case COMM_NO_COM_NO_PENDING_REQUEST :
        case COMM_NO_COM_REQUEST_PENDING :
        {
            channelMode_tu8 = COMM_NO_COMMUNICATION;
        }
        break;

        case COMM_FULL_COM_NETWORK_REQUESTED :
        case COMM_FULL_COM_READY_SLEEP :
        {
            channelMode_tu8 = COMM_FULL_COMMUNICATION;
        }
        break;

        case COMM_SILENT_COM :
        {
            channelMode_tu8 = COMM_SILENT_COMMUNICATION;
        }
        break;

        default:
        {
            /* Report DET when invoked with invalid mode */
            #if (COMM_DEV_ERROR_DETECT != STD_OFF)
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_MAINFNC_APIID,COMM_E_WRONG_PARAMETERS);
            #endif
        }
        break;
    }

    return channelMode_tu8;
}

/* ComM_Prv_LCallRteWrite */
/**************************************************************************************
 * Function name    : ComM_Prv_CallRteWrite
 * Syntax           : ComM_Prv_CallRteWrite
 *                    (
 *                    uint8 ChannelIndex_u8
 *                    )
 * Description      : Internal Function to Call Generated Rte_Write Function based on the Channnel
 * Parameters       : ChannelState_e
 * Return value     : void
 **************************************************************************************/
#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
LOCAL_INLINE void ComM_Prv_CallRteWrite(uint8 ChannelIndex_u8)
{
    const ComM_ChannelType_tst * channelConfigPtr_pcs;
    channelConfigPtr_pcs= &(COMM_GET_CHANNEL_LIST[ChannelIndex_u8]);
    if(channelConfigPtr_pcs->ComMFullCommRequestNotificationEnabled_b == TRUE)
    {
        ComM_CurrentChannelRequest_Rte_Write(ChannelIndex_u8,
                (const ComM_UserHandleArrayType *)&ComM_FullComRequestors[ChannelIndex_u8]);
    }
    else
    {
    }
}
#endif


