


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "ComM_Priv.h"

#if (COMM_PNC_ENABLED != STD_OFF)

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
LOCAL_INLINE void ComM_Prv_PncNoCommunication_StateHandling(NetworkHandleType               PncIndex_tu8,
                                                            ComM_PncEventStructType_tst *   PncEventStruct_pst,
                                                            ComM_PncRamStructType_tst *     pncRamPtr_pst,
                                                            ComM_PncModeType                previousPncState_en);

LOCAL_INLINE void ComM_Prv_PncRequested_StateHandling(NetworkHandleType                 PncIndex_tu8,
                                                      ComM_PncEventStructType_tst *     PncEventStruct_pst,
                                                      ComM_PncRamStructType_tst *       pncRamPtr_pst,
                                                      const ComM_PncConfigStruct_tst *  pncConfigPtr_pcst,
                                                      ComM_PncModeType                  previousPncState_en);

LOCAL_INLINE void ComM_Prv_PncPrepareSleep_StateHandling(NetworkHandleType                  PncIndex_tu8,
                                                         ComM_PncEventStructType_tst *      PncEventStruct_pst,
                                                         ComM_PncRamStructType_tst *        pncRamPtr_pst,
                                                         const ComM_PncConfigStruct_tst *   pncConfigPtr_pcst,
                                                         ComM_PncModeType                   previousPncState_en);

LOCAL_INLINE void ComM_Prv_PncReadySleep_StateHandling(NetworkHandleType                PncIndex_tu8,
                                                       ComM_PncEventStructType_tst *    PncEventStruct_pst,
                                                       ComM_PncRamStructType_tst *      pncRamPtr_pst,
                                                       const ComM_PncConfigStruct_tst * pncConfigPtr_pcst,
                                                       ComM_PncModeType                 previousPncState_en);

LOCAL_INLINE void ComM_Prv_Setbit(uint8 PncId_u8, uint8 * PnTxSignalVal_pu8);


LOCAL_INLINE void ComM_Prv_Resetbit(uint8 PncId_u8, uint8 * PnTxSignalVal_pu8);

#if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
LOCAL_INLINE void ComM_Prv_PncToPassiveChannelInterface(ComM_ChannelVarType_tst *  channelRamPtr_pst,
                                                         boolean                    PncActiveRequestStatus_b,
                                                         boolean                    PncNmRequest_b);
#endif /*  ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))  */
/*****************************************************************************************************
 * Function name    : ComM_Prv_Setbit
 * Syntax           : ComM_Prv_Setbit
 *                    (
 *                    uint8 PncId_u8, uint8 * PnTxSignalVal_pu8
 *                    )
 * Description      : Internal funtion to set signal value in an uint8 array
 * Parameters       : PncId_u8 -> Configured PncId_u8.
 *                  : PnTxSignalVal_pu8 -> Unit_8 Array
 * Return value     : void
 ****************************************************************************************************/

LOCAL_INLINE void ComM_Prv_Setbit(uint8 PncId_u8, uint8 * PnTxSignalVal_pu8)

{
    /* Local variable declaration*/
    uint8   byteIndex_u8;
    uint8   bitPosition_u8;
    uint8   mask_u8;

    /* Local variable initialization*/
    byteIndex_u8 = C_ZERO;
    bitPosition_u8 = C_ZERO;
    mask_u8 = C_ZERO;

    byteIndex_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION ) >> 3;
    bitPosition_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION) % 8 ;
    mask_u8 = ((uint8)1 << bitPosition_u8) ;

    *(PnTxSignalVal_pu8+byteIndex_u8) = (uint8)(*(PnTxSignalVal_pu8+byteIndex_u8) | (mask_u8)) ;
}

/*****************************************************************************************************
 * Function name    : ComM_Prv_Resetbit
 * Syntax           : ComM_Prv_Resetbit
 *                    (
 *                    uint8 PncId_u8, uint8 * PnTxSignalVal_pu8
 *                    )
 * Description      : Internal funtion to reset signal value in an uint8 array
 * Parameters       : PncId_u8 -> Configured PncId_u8.
 *                  : PnTxSignalVal_pu8 -> Unit_8 Array
 * Return value     : void
 ****************************************************************************************************/

LOCAL_INLINE void ComM_Prv_Resetbit(uint8 PncId_u8, uint8 * PnTxSignalVal_pu8)
{
    /* Local variable declaration*/
    uint8   byteIndex_u8;
    uint8   bitPosition_u8;
    uint8   mask_u8;

    /* Local variable initialization*/
    byteIndex_u8 = C_ZERO;
    bitPosition_u8 = C_ZERO;
    mask_u8 = C_ZERO;

    byteIndex_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION) >> 3;
    bitPosition_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION) % 8 ;
    mask_u8 = (~ ((uint8)1 << bitPosition_u8));

    *(PnTxSignalVal_pu8+byteIndex_u8) = (uint8)(*(PnTxSignalVal_pu8+byteIndex_u8) & (mask_u8));
}

#if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
/* ComM_LUpdatePncActiveReqCount */
/**************************************************************************************
 * Function name    : ComM_Prv_PncToPassiveChannelInterface
 * Syntax           : ComM_Prv_PncToPassiveChannelInterface
 *                    (
 *                      ComM_ChannelVarType_tst * channelRamPtr_pst, boolean PncActiveRequestStatus_b,
 *                      boolean PncNmRequest_b
 *                    )
 * Description      : Updating the Active PNC request to the counter for a Gateway Type=Passive.
 * Parameters       : channelRamPtr_pst         -> ComM_ChannelVarType_tst structure
 *                    PncActiveRequestStatus_b  -> This specifies if the active request for the PNC is present.
 *                    PncNmRequest_b             -> This specifies if ComMPncNmRequest is set in the configuration.
 **************************************************************************************/


LOCAL_INLINE void ComM_Prv_PncToPassiveChannelInterface(ComM_ChannelVarType_tst * channelRamPtr_pst,
                                                         boolean                    PncActiveRequestStatus_b,
                                                         boolean                    PncNmRequest_b)
{
   /*If the Active user request via PNC to the channel is present, then increment the counter */
    if(PncActiveRequestStatus_b != FALSE)
    {
        /*Since active request for PNC is present, channel has to be requested hence incrementing the counter*/
        ++(channelRamPtr_pst->PncRequestCtr_u8);

        /* If PncNmRequest_b is enabled, for this channel, set NmNetworkRequestStatus so that
         * Network is requested in the channel main function   */
        if(PncNmRequest_b != FALSE)
        {
            /* This variable need not be locked with channel lock, as in the channel main function,
             * this is cleared only if it is set and processed. Hence there cannot be any inconsistencies */
            channelRamPtr_pst->NmNetworkRequestStatus_b = TRUE;
        }
    }
    else
    {
        /*Since active request for PNC is removed, channel has to be released hence decrementing the counter*/
        if((channelRamPtr_pst->PncRequestCtr_u8) != C_ZERO)
        {
            -- (channelRamPtr_pst->PncRequestCtr_u8);
        }
        else
        {
            /* Control shouldn't come here.If at all happens, report to DET */
            #if (COMM_DEV_ERROR_DETECT != STD_OFF)
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_REQ_COMMODE_APIID,COMM_E_CNTR_UNDERFLW);
            #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
        }
    }
}

#endif /*  (COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */

/* ComM_Prv_PncMainFunction */
/**************************************************************************************
 * Function name    : ComM_Prv_PncMainFunction
 * Syntax           : ComM_Prv_PncMainFunction
 *                    (
 *                    PNCHandleType PncIndex_tu8
 *                    )
 * Description      : Function which contains different states and happening in various stages.
 * Parameters       : PncIndex_tu8
 * Return value     : void
 **************************************************************************************/

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
void ComM_Prv_PncMainFunction(PNCHandleType PncIndex_tu8)
{
    /* Local variable declaration */
    ComM_PncModeType                    pncStateOnEntry_en;
    ComM_PncRamStructType_tst *         pncRamPtr_pst;
    const ComM_PncConfigStruct_tst *    pncConfigPtr_pcst;
    ComM_PncModeType                    previousPncState_en;
    ComM_PncEventStructType_tst         PncEventStruct_st;
    ComM_PncEventStructType_tst *       PncEventStruct_pst;
    boolean                             stateChangePossible_b;
    boolean                             userRequestState_b;

    #if(COMM_INHIBITION_ENABLED)
    ComM_GlobalVarType_tst *            globalRamPtr_pst;
    #endif  /*  #if (COMM_INHIBITION_ENABLED)  */
    /* Initialization of locals */

    PncEventStruct_pst = &PncEventStruct_st;
    stateChangePossible_b = FALSE;
    PncEventStruct_pst->wakeupRequestState_b = FALSE;
    pncRamPtr_pst = &ComM_PncRamStruct[PncIndex_tu8];
    pncConfigPtr_pcst = &(COMM_GET_PNC_LIST[PncIndex_tu8]);
    #if(COMM_INHIBITION_ENABLED)
    globalRamPtr_pst = &ComM_GlobalStruct;
    #endif  /*  #if (COMM_INHIBITION_ENABLED)  */
    /***********************SNAPSHOT OF REQUESTS STARTS here*********************/
    /* Evaluate if user request is present */
    userRequestState_b = ((pncRamPtr_pst->UserRequestCnt_u8) > C_ZERO);
    /* If any of the inhibition is enabled, then user request has to be ignored depending on inhibition type and PNC state */

    #if(COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)

    /* Ignore user request if,
     * 1. wake up inhibition is enabled in ECU group classification
     * 2. wakeup inhibition counter translated to this PNC is > C_ZERO and
     * 3. PNC is in No communication or prepare sleep state
     (PNC prepare sleep state is identical to channel's Silent communication state) */
    if((COMM_GET_PREVENT_WAKEUP((globalRamPtr_pst->ComM_EcuGroupClassification_u8))) &&
       ((pncRamPtr_pst->WakeUpInhibitionCtr_u16) > C_ZERO) &&
       (((pncRamPtr_pst->PncState_en) == COMM_PNC_NO_COMMUNICATION) || ((pncRamPtr_pst->PncState_en) == COMM_PNC_PREPARE_SLEEP)))
    {
        userRequestState_b = FALSE;
    }
    #endif  /*  (COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)  */

    #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)

    /* Ignore user request if
     * 1. limit to no com is enabled in ECU group classification
     * 2. limit to no com counter translated to this PNC is > C_ZERO
     * irrespective of PNC state, so as to ensure that PNC as well as channel goes to no com*/

    if ((COMM_GET_LIMIT_TO_NO_COM(globalRamPtr_pst->ComM_EcuGroupClassification_u8)) &&
            ((pncRamPtr_pst->LimitToNoComCtr_u16) != C_ZERO))
    {
        userRequestState_b = FALSE;
    }
    #endif  /*  (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */

    #if (COMM_PNC_GW_ENABLED != STD_OFF)

    /* Lock is not required here as the ActiveERACnt_u8 and ERACnt_u8 are set whenever there is a EIRA/ERA
     * indication from Nm. These requests are processed in the respective Pnc main functions.*/

    /* Evaluate activeERAState_b, that is, either if user request is present or any active ERA == 1 */
    PncEventStruct_pst->activeERAState_b = ((pncRamPtr_pst->ActiveERACnt_u8 > C_ZERO) || (userRequestState_b));

    /* Evaluate if channel is requested actively, that is, either by User or ERACnt > C_ZERO */
    PncEventStruct_pst->activeRequestState_b = ((userRequestState_b)||((pncRamPtr_pst->ERACnt_u8) > C_ZERO));

    #else
    /* Evaluate activeERAState_b, that is, either if user request is present */
    PncEventStruct_pst->activeRequestState_b = ((pncRamPtr_pst->UserRequestCnt_u8) > C_ZERO);
    (void)userRequestState_b;
    #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF)  */

    /* Evaluate passive request state of channel, ie., EIRACnt > C_ZERO */
    PncEventStruct_pst->passiveRequestState_b = ((pncRamPtr_pst->EIRACnt_u8) > C_ZERO);

    /* Evaluate if  wake up is indicated on the PN */
    PncEventStruct_pst->wakeupRequestState_b = pncRamPtr_pst->WakeUpStatus_b;

    pncStateOnEntry_en = pncRamPtr_pst->PncState_en;

    previousPncState_en = pncStateOnEntry_en;

    /*************************SNAPSHOT OF REQUESTS ENDS here***********************/

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED)
    {

        /***********************PNC state machine handling starts here****************/
        do
        {
            switch (pncRamPtr_pst->PncState_en)
            {
                case COMM_PNC_NO_COMMUNICATION:
                {
                    ComM_Prv_PncNoCommunication_StateHandling(PncIndex_tu8, PncEventStruct_pst, pncRamPtr_pst,
                                                                        previousPncState_en);
                }
                break;

                case COMM_PNC_REQUESTED:
                {
                    ComM_Prv_PncRequested_StateHandling(PncIndex_tu8, PncEventStruct_pst, pncRamPtr_pst,
                                                            pncConfigPtr_pcst, previousPncState_en);
                }
                break;

                case COMM_PNC_READY_SLEEP:
                {
                    ComM_Prv_PncReadySleep_StateHandling(PncIndex_tu8, PncEventStruct_pst, pncRamPtr_pst,
                                                               pncConfigPtr_pcst, previousPncState_en);
                }
                break;

                case COMM_PNC_PREPARE_SLEEP:
                {
                    ComM_Prv_PncPrepareSleep_StateHandling(PncIndex_tu8, PncEventStruct_pst, pncRamPtr_pst,
                                                                    pncConfigPtr_pcst, previousPncState_en);
                }
                break;

                default:
                    /* Intentionally Empty */
                break;
            }

            if(pncStateOnEntry_en != pncRamPtr_pst->PncState_en)
            {
                stateChangePossible_b = TRUE;
                /* Evaluate previous state of PNC */
                previousPncState_en = pncStateOnEntry_en;
                /* Update local copy of pnc state */
                pncStateOnEntry_en = pncRamPtr_pst->PncState_en;
            }
            else
            {
                stateChangePossible_b = FALSE;
            }

        } while (stateChangePossible_b != FALSE);

        /************************PNC state machine handling ends here****************/
    }

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/* ComM_Prv_PncNoCommunication_StateHandling */
/**************************************************************************************
 * Function name    : ComM_Prv_PncNoCommunication_StateHandling
 * Syntax           : ComM_Prv_PncNoCommunication_StateHandling
 *                    (
 *                    NetworkHandleType PncIndex,
                      ComM_PncEventStructType_tst * comM_PncEventStruct,
                      ComM_PncRamStructType_tst * pncRamPtr_pst,
                      ComM_PncModeType previousPncState_e
 *                    )
 * Description      : Internal function for handling COMM_PNC_NO_COMMUNICATION state
 * Parameters       : PncIndex, comM_PncEventStruct, pncRamPtr_pst,previousPncState_e
 * Return value     : ComM_PncModeType
 **************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The PncEventStruct_ps pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE void ComM_Prv_PncNoCommunication_StateHandling(NetworkHandleType               PncIndex_tu8,
                                                            ComM_PncEventStructType_tst *   PncEventStruct_pst,
                                                            ComM_PncRamStructType_tst *     pncRamPtr_pst,
                                                            ComM_PncModeType                previousPncState_en)
{
    ComM_PncModeType   pncState_en;

    pncState_en = COMM_PNC_NO_COMMUNICATION;

    /* Entry to state COMM_PNC_NO_COMMUNICATION */
    if(previousPncState_en !=  COMM_PNC_NO_COMMUNICATION)
    {
        /* Notify BswM regarding state change */
        BswM_ComM_CurrentPNCMode(PncIndex_tu8,COMM_PNC_NO_COMMUNICATION);
    }
    /* Cyclic part of COMM_PNC_NO_COMMUNICATION state */
    else
    {
        /* Process the requests in COMM_PNC_NO_COMMUNICATION state as per below priority
         * 1. Active request ((User request) or (any ERA == 1 if Gateway is enabled))
         * 2. Passive request (External request from other ECU's through EIRA)
         * 3. WakeUp indication, if synchronous wake up is enabled */

        if(PncEventStruct_pst->activeRequestState_b != FALSE)
        {
            pncState_en = COMM_PNC_REQUESTED;
        }
        else if (PncEventStruct_pst->passiveRequestState_b != FALSE)
        {
            pncState_en = COMM_PNC_READY_SLEEP;
        }
        else if (PncEventStruct_pst->wakeupRequestState_b != FALSE)
        {
            pncState_en = COMM_PNC_PREPARE_SLEEP;
        }
        else
        {
            /* PNC is not requested so,remain in COMM_PNC_NO_COMMUNICATION state */
        }
    }

    /* Exit of COMM_PNC_NO_COMMUNICATION state */
    if(pncState_en != COMM_PNC_NO_COMMUNICATION)
    {
        SchM_Enter_ComM_PNCWakeUpNoNest();
        /* Update PN state to the RAM structure */
        pncRamPtr_pst->PncState_en = pncState_en;
        /* Clear synchronous wake up as, if at all present,it is already processed */
        pncRamPtr_pst->WakeUpStatus_b = FALSE;

        SchM_Exit_ComM_PNCWakeUpNoNest();
    }

}


/* ComM_Prv_PncRequested_StateHandling */
/**************************************************************************************
* Function name    : ComM_Prv_PncRequested_StateHandling
* Syntax           : ComM_Prv_PncRequested_StateHandling
*                    (
*                    NetworkHandleType PncIndex_tu8,
                     ComM_PncEventStructType_tst * comM_PncEventStruct,
                     ComM_PncRamStructType_tst * pncRamPtr_pst,
                     const ComM_PncConfigStruct_tst * pncConfigPtr_pcst,
                     ComM_PncModeType previousPncState_enn,
*                    )
* Description      : Internal function for handling COMM_PNC_REQUESTED  state
* Parameters       : PncIndex_tu8, comM_PncEventStruct, pncRamPtr_pst, previousPncState_en, pncConfigPtr_pcst
* Return value     : void
**************************************************************************************/
/* MR12 RULE 8.13 VIOLATION: The PncEventStruct_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
/* HIS METRIC PATH, V(G), STMT VIOLATION IN ComM_Prv_PncRequested_StateHandling:
   Each condition check involves processing of COMM_PNC_REQUESTED State handling. The violation is not critical and
   has no functional impact. */
LOCAL_INLINE void ComM_Prv_PncRequested_StateHandling(NetworkHandleType                 PncIndex_tu8,
                                                      ComM_PncEventStructType_tst *     PncEventStruct_pst,
                                                      ComM_PncRamStructType_tst *       pncRamPtr_pst,
                                                      const ComM_PncConfigStruct_tst *  pncConfigPtr_pcst,
                                                      ComM_PncModeType                  previousPncState_en)
{
   /* Local variable declaration */
    ComM_PncModeType                pncState_en;
    uint8                           channelId_u8;
    uint8                           loopCtr_u8;
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    boolean                         ActiveERAStatus_b;

    /* Initialization of locals */
    pncState_en = COMM_PNC_REQUESTED;
    ActiveERAStatus_b = TRUE;

    /* First entry to COMM_PNC_REQUESTED state */
    if(previousPncState_en != COMM_PNC_REQUESTED)
    {

        /* Notify BswM regarding state change */
        BswM_ComM_CurrentPNCMode(PncIndex_tu8,COMM_PNC_REQUESTED);

        #if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
        ActiveERAStatus_b = PncEventStruct_pst->activeERAState_b;
        #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */

        /* Invoke PN to channel interface function, so as to REQUEST channel */
        /*  PncEventStruct_pst->activeERAState_b speicifies if there is a active user request/ERA indication to a
         * channel for a GWType "active".  Only then the channel is requested for full communication for GWType "passive"
         * if ComM0PncVectorAvoidance is set to true in configuration*/
        ComM_Prv_PncToChannelInterface( PncIndex_tu8,COMM_PNC_REQUESTED,previousPncState_en,ActiveERAStatus_b);

        /* Loop through all channels of this PN and depending on
        * 1. Gateway enabled or disabled,
        * 2. if assigned PNC is non Gateway coordinated PNC
        * 3. Gateway type and
        * 4. the kind of request(active or passive),
        * evaluate comSigVal_b for the channel  */

        /* For a non Gateway coordinated PNC, the bit representing this PNC in Com signals with
         * ComMPncComSignalDirection "TX" assigned to this PNC is set upon entering the
         * PNC sub state COMM_PNC_REQUESTED */

        /* For a passively coordinated channel,comSigVal_b is set only if there is a user request or
        * ERA of any actively coordinated channel is 1 */

        for (loopCtr_u8=0; loopCtr_u8 < pncConfigPtr_pcst->NumChannels_u8; loopCtr_u8++)
        {
            channelId_u8 = (pncConfigPtr_pcst->Channels_pcu8[loopCtr_u8]);

            channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[channelId_u8]);
            channelRamPtr_pst = &(ComM_ChannelStruct[channelId_u8]);

            if((channelConfigPtr_pcst->TxSigId_u16)!= COMM_COMSIGNAL_INVALID_ID)
            {
                #if (COMM_PNC_GW_ENABLED != STD_OFF)
                if ((pncConfigPtr_pcst->isPncGwCoordinated_b == FALSE) ||
                        (channelConfigPtr_pcst->PncGwType_en == COMM_GATEWAY_TYPE_ACTIVE) ||
                        ((channelConfigPtr_pcst->PncGwType_en == COMM_GATEWAY_TYPE_PASSIVE) &&
                                (PncEventStruct_pst->activeERAState_b)))
                {
                    SchM_Enter_ComM_PNCTxSigNoNest();
                    ComM_Prv_Setbit(pncConfigPtr_pcst->PncId_u8, (channelRamPtr_pst->PnTxSigVal_u8));
                    SchM_Exit_ComM_PNCTxSigNoNest();
                    /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void
                     * as per Autosar guidelines. */
                    (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16, (channelRamPtr_pst->PnTxSigVal_u8));
                }
                else
                {
                    SchM_Enter_ComM_PNCTxSigNoNest();
                    ComM_Prv_Resetbit(pncConfigPtr_pcst->PncId_u8, (channelRamPtr_pst->PnTxSigVal_u8));
                    SchM_Exit_ComM_PNCTxSigNoNest();
                    /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value
                     * should be void as per Autosar guidelines. */
                    #if (COMM_ZERO_PNC_VECTOR_AVOIDANCE == STD_OFF)
                    (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16, (channelRamPtr_pst->PnTxSigVal_u8));
                    #endif  /*  (COMM_ZERO_PNC_VECTOR_AVOIDANCE == STD_OFF)  */
                }
                /* Update the value of activeERAState_b to activeERAStateOld_b */
                pncRamPtr_pst->ActiveERAStateOld_b = PncEventStruct_pst->activeERAState_b;
                #else
                SchM_Enter_ComM_PNCTxSigNoNest();
                ComM_Prv_Setbit(pncConfigPtr_pcst->PncId_u8, (channelRamPtr_pst->PnTxSigVal_u8));
                SchM_Exit_ComM_PNCTxSigNoNest();
                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void
                 * as per Autosar guidelines. */
                (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16, (channelRamPtr_pst->PnTxSigVal_u8));

                #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF)  */

            }
        }
    }
    /* Cyclic part of COMM_PNC_REQUESTED state */
    else
    {
        /* Continue in COMM_PNC_REQUESTED state till active request is present */
        if(PncEventStruct_pst->activeRequestState_b != FALSE)
        {
        /* Remain in COMM_PNC_REQUESTED state as the PNC is being requested actively */
        #if (COMM_PNC_GW_ENABLED != STD_OFF)

         /* Transmit comSigVal_b for Passive channel only if the previously indicated value changes */
            if(pncRamPtr_pst->ActiveERAStateOld_b != PncEventStruct_pst->activeERAState_b)
            {
                for (loopCtr_u8=0; loopCtr_u8 < pncConfigPtr_pcst->NumChannels_u8; loopCtr_u8++)
                {
                   channelId_u8 = pncConfigPtr_pcst->Channels_pcu8[loopCtr_u8];
                   channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[channelId_u8]);
                   channelRamPtr_pst = &ComM_ChannelStruct[channelId_u8];

                    if ((channelConfigPtr_pcst->PncGwType_en == COMM_GATEWAY_TYPE_PASSIVE))
                    {
                        if((channelConfigPtr_pcst->TxSigId_u16)!= COMM_COMSIGNAL_INVALID_ID)
                        {

                            #if (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)

                            SchM_Enter_ComM_PNCNoNest();
                            /* If there is a Active user request then this channel is to be put to full com.
                             * Hence increment the PncRequestCtr_u8 value for this channel*/
                            ComM_Prv_PncToPassiveChannelInterface(channelRamPtr_pst, PncEventStruct_pst->activeERAState_b,
                                                                   channelConfigPtr_pcst->PncNmRequest_b);
                            SchM_Exit_ComM_PNCNoNest();

                            #endif  /*  (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */
                            if(PncEventStruct_pst->activeERAState_b != FALSE)
                            {
                                SchM_Enter_ComM_PNCTxSigNoNest();
                                ComM_Prv_Setbit(pncConfigPtr_pcst->PncId_u8, (channelRamPtr_pst->PnTxSigVal_u8));
                                SchM_Exit_ComM_PNCTxSigNoNest();
                                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void
                                 * as per Autosar guidelines. */
                                (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16, &(channelRamPtr_pst->PnTxSigVal_u8[0]));
                            }
                            else
                            {
                                SchM_Enter_ComM_PNCTxSigNoNest();
                                ComM_Prv_Resetbit(pncConfigPtr_pcst->PncId_u8, (channelRamPtr_pst->PnTxSigVal_u8));
                                SchM_Exit_ComM_PNCTxSigNoNest();
                                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void
                                 * as per Autosar guidelines. */
                                 #if (COMM_ZERO_PNC_VECTOR_AVOIDANCE == STD_OFF)
                                /* Com_SendSignal should not be called if the COMM_ZERO_PNC_VECTOR_AVOIDANCE is set */
                                (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16, (channelRamPtr_pst->PnTxSigVal_u8));
                                #endif  /*  (COMM_ZERO_PNC_VECTOR_AVOIDANCE == STD_OFF)  */
                            }

                        }
                    }
                }
                /* As the value of activeERAState_b has changed, update it to activeERAStateOld_b */
                pncRamPtr_pst->ActiveERAStateOld_b = PncEventStruct_pst->activeERAState_b;
            }
        #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF )  */
        }
        else
        {
         /* As PNC is not being requested actively, go to COMM_PNC_READY_SLEEP state */
            pncState_en = COMM_PNC_READY_SLEEP;
        }
    }
    /* Exit of COMM_PNC_REQUESTED state */
    if(pncState_en != COMM_PNC_REQUESTED)
    {
        /* Update PN state to the RAM structure */
        pncRamPtr_pst->PncState_en = pncState_en;
    }
}


/* ComM_Prv_PncReadySleep_StateHandling */
/**************************************************************************************
* Function name    : ComM_Prv_PncReadySleep_StateHandling
* Syntax           : ComM_Prv_PncReadySleep_StateHandling
*                    (
*                    NetworkHandleType PncIndex_tu8,
                     ComM_PncEventStructType_tst * comM_PncEventStruct,
                     ComM_PncRamStructType_tst * pncRamPtr_pst,
                     const ComM_PncConfigStruct_tst * pncConfigPtr_pcst,
                     ComM_PncModeType previousPncState_en
*                    )
* Description      : Internal function for handling COMM_PNC_READY_SLEEP  state
* Parameters       : PncIndex_tu8, comM_PncEventStruct, pncRamPtr_pst, previousPncState_en, pncConfigPtr_pcst
* Return value     : void
**************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The PncEventStruct_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE void ComM_Prv_PncReadySleep_StateHandling(NetworkHandleType                PncIndex_tu8,
                                                       ComM_PncEventStructType_tst *    PncEventStruct_pst,
                                                       ComM_PncRamStructType_tst *      pncRamPtr_pst,
                                                       const ComM_PncConfigStruct_tst * pncConfigPtr_pcst,
                                                       ComM_PncModeType                 previousPncState_en)
{
    /* Local Variables Declaration */
    ComM_PncModeType                pncState_en;
    uint8                           channelId_u8;
    uint8                           loopCtr_u8;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    boolean                         ActiveERAStateOldStatus_b;

    /* Local variable initialization */
    pncState_en = COMM_PNC_READY_SLEEP;
    ActiveERAStateOldStatus_b = FALSE;

    /* First entry to COMM_PNC_READY_SLEEP state */
    if(previousPncState_en != COMM_PNC_READY_SLEEP)
    {
        /* Notify BswM regarding state change */
        BswM_ComM_CurrentPNCMode(PncIndex_tu8,COMM_PNC_READY_SLEEP);

        /*Copying the ActiveERAStateOld_b status to a local variable*/
        #if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
        ActiveERAStateOldStatus_b = pncRamPtr_pst->ActiveERAStateOld_b;
        pncRamPtr_pst->ActiveERAStateOld_b = FALSE;
        #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */

        /* Invoke PN to channel interface function, so as to RELEASE channel */
        /*  ActiveERAStateOldStatus_b speicifies if there was a active user request/ERA indication to a
         * channel for a GWType "active".  Only then the channel is released in ready sleep for GWType "passive"
         * if ComM0PncVectorAvoidance is set to true in configuration. Or else the PncRequestCtr_u8 counter will under flow*/
        ComM_Prv_PncToChannelInterface(PncIndex_tu8,COMM_PNC_READY_SLEEP,previousPncState_en,ActiveERAStateOldStatus_b);

        /* On transition from COMM_PNC_REQUESTED -> COMM_PNC_READY_SLEEP state,
         * call Com_SendSignal() with '0' for all Com signals assigned to this PN */
        if(previousPncState_en == COMM_PNC_REQUESTED)
        {
            /* For all ComM channels of this PNC,call Com_SendSignal() with '0' as this PN is not requested actively anymore  */
            for(loopCtr_u8=0; loopCtr_u8 < pncConfigPtr_pcst->NumChannels_u8; loopCtr_u8++)
            {
                channelId_u8 = pncConfigPtr_pcst->Channels_pcu8[loopCtr_u8];
                channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[channelId_u8]);
                channelRamPtr_pst = &ComM_ChannelStruct[channelId_u8];

                /* Check and proceed only if there is any Tx signal configured */
                if((channelConfigPtr_pcst->TxSigId_u16)!= COMM_COMSIGNAL_INVALID_ID)
                {
                    SchM_Enter_ComM_PNCTxSigNoNest();
                    ComM_Prv_Resetbit(pncConfigPtr_pcst->PncId_u8, &(channelRamPtr_pst->PnTxSigVal_u8[0]));
                    SchM_Exit_ComM_PNCTxSigNoNest();
                      /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void
                       * as per Autosar guidelines. */
                    (void)Com_SendSignal(channelConfigPtr_pcst->TxSigId_u16,&channelRamPtr_pst->PnTxSigVal_u8[0]);
                }

            }
        }
    }
    /* Cyclic part of COMM_PNC_READY_SLEEP state */
    else
    {
        /* Process the requests as per below priority
         * 1.Active request ((User request) or (any ERA == 1 if Gateway is enabled))
         * 2.Passive request (External request from other ECU's through EIRA)*/
        if((PncEventStruct_pst->activeRequestState_b) != FALSE)
        {
            pncState_en = COMM_PNC_REQUESTED;
        }
        else if((PncEventStruct_pst->passiveRequestState_b) != FALSE)
        {
            /* PNC is being requested passively, hence remain in COMM_PNC_READY_SLEEP */
        }
        else
        {
            pncState_en = COMM_PNC_PREPARE_SLEEP;
        }
    }
    /* Exit of COMM_PNC_READY_SLEEP state  */
    if(pncState_en != COMM_PNC_READY_SLEEP)
    {
        /* Update PN state to the RAM structure */
        pncRamPtr_pst->PncState_en = pncState_en;
    }

}

/* ComM_Prv_PncPrepareSleep_StateHandling */
/**************************************************************************************
* Function name    : ComM_Prv_PncPrepareSleep_StateHandling
* Syntax           : ComM_Prv_PncPrepareSleep_StateHandling
*                    (
*                    NetworkHandleType PncIndex_tu8,
                     ComM_PncEventStructType_tst * comM_PncEventStruct,
                     ComM_PncRamStructType_tst * pncRamPtr_ps,
                     const ComM_PncConfigStruct_tst * pncConfigPtr_pcs,
                     ComM_PncModeType previousPncState_e
*                    )
* Description      : Internal function for handling COMM_PNC_READY_SLEEP  state
* Parameters       : PncIndex_tu8, comM_PncEventStruct, pncRamPtr_ps, previousPncState_e, pncConfigPtr_pcs
* Return value     : void
**************************************************************************************/

/* MR12 RULE 8.13 VIOLATION: The PncEventStruct_pst pointer variables are modified in main function.
 * Hence it cant be P2CONST type */
LOCAL_INLINE void ComM_Prv_PncPrepareSleep_StateHandling(NetworkHandleType                  PncIndex_tu8,
                                                         ComM_PncEventStructType_tst *      PncEventStruct_pst,
                                                         ComM_PncRamStructType_tst *        pncRamPtr_pst,
                                                         const ComM_PncConfigStruct_tst *   pncConfigPtr_pcst,
                                                         ComM_PncModeType                   previousPncState_en)
{
    ComM_PncModeType   pncState_en;

    pncState_en = COMM_PNC_PREPARE_SLEEP;

    /* First entry to COMM_PNC_PREPARE_SLEEP state */
    if(previousPncState_en != COMM_PNC_PREPARE_SLEEP)
    {
        /* Notify BswM regarding state change */
        BswM_ComM_CurrentPNCMode(PncIndex_tu8,COMM_PNC_PREPARE_SLEEP);

        /* Start Prepare sleep timer */
        pncRamPtr_pst->PrepareSleepTimer_u16 = pncConfigPtr_pcst->PrepareSleepTimer_u16;
    }
    /* Cyclic part of COMM_PNC_PREPARE_SLEEP state */
    else
    {
        /* If the prepare sleep timer has not already expired, decrement the timer*/
        if((pncRamPtr_pst->PrepareSleepTimer_u16)!= C_ZERO)
        {
            --pncRamPtr_pst->PrepareSleepTimer_u16;
        }
        /* Process the requests as per below priority
         * 1.Active request ((User request) or (any ERA == 1 if Gateway is enabled))
         * 2.Passive request (External request from other ECU's through EIRA)
         * 3.Prepare sleep timer has expired*/
        if(PncEventStruct_pst->activeRequestState_b != FALSE)
        {
            pncState_en = COMM_PNC_REQUESTED;
        }
        else if(PncEventStruct_pst->passiveRequestState_b != FALSE)
        {
            pncState_en = COMM_PNC_READY_SLEEP;
        }
        else if(pncRamPtr_pst->PrepareSleepTimer_u16 == C_ZERO)
        {
            pncState_en = COMM_PNC_NO_COMMUNICATION;
        }
        else
        {
            /* As there are no requests, remain in COMM_PNC_PREPARE_SLEEP state itself till timer expires */
        }
    }
    /* Exit of COMM_PNC_PREPARE_SLEEP state  */
    if(pncState_en != COMM_PNC_PREPARE_SLEEP)
    {
        /* Update PN state to the RAM structure */
        pncRamPtr_pst->PncState_en = pncState_en;
    }

}

#endif  /*   (COMM_PNC_ENABLED != STD_OFF)  */

