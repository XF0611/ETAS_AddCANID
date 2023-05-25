
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

/*
 ***************************************************************************************************
 * Inline functions declarations
 ***************************************************************************************************
 */

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"

/* UNSPECIFIED variable definitions */

ComM_GlobalVarType_tst          ComM_GlobalStruct;
ComM_UserVarType_tst            ComM_UserStruct[COMM_NO_OF_USERS];
ComM_ChannelVarType_tst         ComM_ChannelStruct[COMM_NO_OF_CHANNELS];
ComM_NvMStorageType_tst         ComM_NvMblock;

#if (COMM_PNC_ENABLED != STD_OFF)

ComM_PncRamStructType_tst   ComM_PncRamStruct[COMM_NO_OF_PNCS];
boolean                     ComM_EIRA_RxSigVal[COMM_NO_OF_POSSIBLE_BUSTYPECH_WITH_EIRA][COMM_NO_OF_PNCS];

#if (COMM_PNC_GW_ENABLED != STD_OFF)
boolean                 ComM_ERA_RxSigVal[COMM_NO_OF_CH_WITH_ERA][COMM_NO_OF_PNCS];
#endif      /* (COMM_PNC_GW_ENABLED != STD_OFF) */

#endif        /* (COMM_PNC_ENABLED != STD_OFF) */

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "ComM_MemMap.h"

#if (COMM_PNC_ENABLED != STD_OFF)
LOCAL_INLINE boolean ComM_Prv_Getbit(uint8 PncId_u8, const uint8 * PnSignalVal_pu8);
#endif        /* (COMM_PNC_ENABLED != STD_OFF) */

#if(COMM_INHIBITION_ENABLED)
LOCAL_INLINE void ComM_Prv_TranslateInhibitionToUser(const uint8 *             Users_pcu8,
                                                      uint8                     numUsers_u8,
                                                      ComM_InhibitionType_ten   InhibitionType_en,
                                                      boolean               Status_b);
LOCAL_INLINE void ComM_Prv_UpdateInhibitionCounter(uint16 *                InhibitionCounter_pu16,
                                                    boolean                 Status_b,
                                                    uint16                  InhibitionCounterMax_u16);
#endif/*  (COMM_INHIBITION_ENABLED)  */

/***************************************************************************************************
 * Function name    : ComM_Prv_ValidateUserId
 * Syntax           : ComM_Prv_ValidateUserId
 *                    (
 *                     ComM_UserHandleType UserId
 *                    )
 * Description      : Internal function to check if the UserId is valid or not
 * Parameters       : User -> External UserId
 * Return value     : 1 -> UserId is valid.
 *                    0 -> UserId is Invalid.
 **************************************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

boolean ComM_Prv_ValidateUserId (ComM_UserHandleType UserId_tu8)
{
#if (COMM_USERID_MIN != 0)
    if((UserId_tu8 < COMM_USERID_MIN) || (UserId_tu8 > COMM_USERID_MAX))
#else
    if(UserId_tu8 > COMM_USERID_MAX)
#endif
    {
        return FALSE;
    }
    else
    {
        return (ComM_UserId_MappingTable_acst[UserId_tu8] == COMM_USER_INVALID_ID ) ? (FALSE) : (TRUE) ;
    }

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if (COMM_PNC_ENABLED != STD_OFF)

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/* ComM_Prv_PncToChannelInterface */
/**************************************************************************************
 * Function name    : ComM_Prv_PncToChannelInterface
 * Syntax           : ComM_Prv_PncToChannelInterface
 *                    (
 *                      PNCHandleType PncIndex_tu8,ComM_PncModeType PncState_en,ComM_PncModeType PncPreviousState_en
 *                      boolean  ActiveERAState_b
 *                    )
 * Description      : Interface between PNC and channel state machines.
 * Parameters       : PncIndex_tu8            : PNC Id
 *                    PncState_en            : Current State of PNC
 *                    PncPreviousState_en    : Previous PNC state
 *                    ActiveERAState_b    : This specifies if active user request/ERA indication to a
                                            channel for a GWType "active" is present.
 * Return value     : void
 **************************************************************************************/
/* MR12 RULE 8.13 VIOLATION: The pncRamPtr_ps pointer variables are modified in main function. Hence it
 * cant be P2CONST type */
void ComM_Prv_PncToChannelInterface(PNCHandleType      PncIndex_tu8,
                                     ComM_PncModeType   PncState_en,
                                     ComM_PncModeType   PncPreviousState_en,
                                     boolean            ActiveERAState_b)

{
    /* Local variable declaration */
    uint8   loopCtr_u8;
    uint8   ChannelId_u8;
    uint8   numChannels_u8;

    ComM_ChannelVarType_tst *           channelRamPtr_pst;
    const ComM_PncConfigStruct_tst *    pncConfigPtr_pcst;
    const ComM_ChannelType_tst *        channelConfigPtr_pcst;

    /* Local variable initialization */
    pncConfigPtr_pcst = &(COMM_GET_PNC_LIST[PncIndex_tu8]);
    numChannels_u8 = pncConfigPtr_pcst->NumChannels_u8;

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the
     * configuration therefore it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {

    /* Protect this area, so that multiple PNC's should not interfere with each other */
    SchM_Enter_ComM_PNCNoNest();
    /* For all channels of this PNC, evaluate the number of PNC's requested or released */
    for(loopCtr_u8 = 0; loopCtr_u8 < numChannels_u8 ;loopCtr_u8++)
    {
        ChannelId_u8 = (pncConfigPtr_pcst->Channels_pcu8[loopCtr_u8]);

        channelRamPtr_pst = &ComM_ChannelStruct[ChannelId_u8];

        channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[ChannelId_u8]);

        /* As the PNC state is COMM_PNC_REQUESTED, increment the counter */
        if (PncState_en == COMM_PNC_REQUESTED)
        {
            #if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
            if((channelConfigPtr_pcst->PncGwType_en == COMM_GATEWAY_TYPE_PASSIVE) &&
               ((channelConfigPtr_pcst->TxSigId_u16)!= COMM_COMSIGNAL_INVALID_ID) && (ActiveERAState_b == FALSE))
            {
                /* Do Nothing */
                /* According to req SWS_ComM_00966, since COMM_ZERO_PNC_VECTOR_AVOIDANCE is set, channel has to be
                 * released. Hence don't increment the PncRequestCtr_u8 value for this channel*/
            }
            else
            #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */
            {
                ++(channelRamPtr_pst->PncRequestCtr_u8);

                /* If PncNmRequest is enabled, for this channel, set NmNetworkRequestStatus so that
                 * Network is requested in the channel main function   */
                if(channelConfigPtr_pcst->PncNmRequest_b != FALSE)
                {
                    /* This variable need not be locked with channel lock, as in the channel main function,
                     * this is cleared only if it is set and processed. Hence there cannot be any inconsistencies */
                    channelRamPtr_pst->NmNetworkRequestStatus_b = TRUE;
                }
            }
        }

        /* As the PNC has changed from COMM_PNC_REQUESTED to COMM_PNC_READY_SLEEP state, decrement the counter */
        else if((PncState_en == COMM_PNC_READY_SLEEP) && (PncPreviousState_en == COMM_PNC_REQUESTED))
        {
            #if ((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
            if((channelConfigPtr_pcst->PncGwType_en == COMM_GATEWAY_TYPE_PASSIVE) &&
                ((channelConfigPtr_pcst->TxSigId_u16)!= COMM_COMSIGNAL_INVALID_ID) && (ActiveERAState_b == FALSE))
            {
                /*  Do Nothing */
                /*  Accroding to the req SWS_ComM_00966, if pncRamPtr_ps->ActiveERAState_b is false then
                 *  this channel's PncRequestCtr_u8 is already decremeted in the else part of PNC Requested function,
                 *  so don't decrement further. Else decrement. */
            }
            else
            #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF)  */
            {
                if(channelRamPtr_pst->PncRequestCtr_u8 != C_ZERO)
                {
                    --(channelRamPtr_pst->PncRequestCtr_u8);
                }
                else
                {
                    /* Control shouldn't come here.If at all happens, report to DET */
                    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
                    (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_PNC_MAINFNC_APIID,COMM_E_CNTR_UNDERFLW);
                    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
                }
            }
        }
        else
        {
            /* No affect on the channel state machine */
        }
    }
    SchM_Exit_ComM_PNCNoNest();
    }
    #if !((COMM_PNC_GW_ENABLED != STD_OFF) && (COMM_ZERO_PNC_VECTOR_AVOIDANCE != STD_OFF))
    (void)ActiveERAState_b;
    #endif  /*  (COMM_PNC_GW_ENABLED == STD_OFF) || (COMM_ZERO_PNC_VECTOR_AVOIDANCE == STD_OFF)  */
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif        /* (COMM_PNC_ENABLED != STD_OFF) */


#if (COMM_PNC_ENABLED != STD_OFF)

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
/* ComM_Prv_EIRA_CallBack */
/**************************************************************************************
 * Function name    : ComM_Prv_EIRA_CallBack
 * Syntax           : ComM_Prv_EIRA_CallBack
 *                    (
 *                    uint8 PnBusId_u8
 *                    )
 * Description      : EIRA call back, whenever the value of signal changes
 * Parameters       : PnBusId_u8 : BusId
 * Return value     : void
 **************************************************************************************/

void ComM_Prv_EIRA_CallBack(uint8 PnBusId_u8)
{
    /*Local variable declaration*/
    uint8                               PncIndex_u8;
    uint8                               SigValue_au8[COMM_PNC_VECTOR_LENGTH];
    uint16                              ComSigId_u16;
    ComM_PncRamStructType_tst *         pncRamPtr_pst;
    const ComM_PncConfigStruct_tst *    pncConfigPtr_pcst;
    boolean                             EIRA_PncSigValue_b;

    /* Local variable initialization */
    SchM_Enter_ComM_EIRANoNest();

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore
     * it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {

        ComSigId_u16 = COMM_GET_EIRASIG_TABLE[PnBusId_u8];

        if(ComSigId_u16 != COMM_COMSIGNAL_INVALID_ID)
        {
            /* MR12 DIR 1.1 VIOLATION: Com_ReceiveSignal's SignalDataPtr return value should be void as
             * per Autosar guidelines. */
            (void)Com_ReceiveSignal(ComSigId_u16,&SigValue_au8);

            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is
             * actually the variable identified as the 'loop control variable.
             * Hence it is an incorrect warning reported by Misra */
            for(PncIndex_u8=0, pncRamPtr_pst = &ComM_PncRamStruct[0], pncConfigPtr_pcst = &(COMM_GET_PNC_LIST[0]); PncIndex_u8 < COMM_NO_OF_PNCS; PncIndex_u8++)
            {
                EIRA_PncSigValue_b = ComM_Prv_Getbit(pncConfigPtr_pcst->PncId_u8,SigValue_au8);

                if(ComM_EIRA_RxSigVal[PnBusId_u8][PncIndex_u8]!= EIRA_PncSigValue_b)
                {
                    ComM_EIRA_RxSigVal[PnBusId_u8][PncIndex_u8] = EIRA_PncSigValue_b;

                    if(EIRA_PncSigValue_b != FALSE)
                    {
                        ++pncRamPtr_pst->EIRACnt_u8;
                    }
                    else
                    {
                        --pncRamPtr_pst->EIRACnt_u8;
                    }
                }
                pncRamPtr_pst++;
                pncConfigPtr_pcst++;
            }
        }
    }
    SchM_Exit_ComM_EIRANoNest();

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif        /* (COMM_PNC_ENABLED != STD_OFF)  */


#if (COMM_PNC_ENABLED != STD_OFF)

#if (COMM_PNC_GW_ENABLED != STD_OFF)

/* ComM_Prv_ERA_CallBack */
/**************************************************************************************
 * Function name    : ComM_Prv_ERA_CallBack
 * Syntax           : ComM_Prv_ERA_CallBack
 *                    (
 *                    uint8 PnBusId_u8, ComM_PncGatewayType_ten channelGwType_en
 *                    )
 * Description      : ERA call back, whenever the signal value changes
 * Parameters       : PnBusId_u8       : PNC BusId to which the ERA indication is received
 *                    channelGwType_en : Indicates the Gateway type(Active/Passive)
 * Return value     : void
 **************************************************************************************/

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/* HIS METRIC LEVEL VIOLATION IN ComM_Prv_ERA_CallBack: The function contains necessary checks for processing ERA
 * indication and updating corresponding counters. The deviation has no functional impact and not critical */
void ComM_Prv_ERA_CallBack(uint8 PnBusId_u8, ComM_PncGatewayType_ten channelGwType_en)
{
    /*Local variable declaration*/
    uint8                               PncIndex_u8;
    boolean                             ERA_PncSigValue_b;
    uint16                              ComSigId_u16;
    uint8                               SigValue_au8[COMM_PNC_VECTOR_LENGTH];
    ComM_PncRamStructType_tst *         pncRamPtr_pst;
    const ComM_PncConfigStruct_tst *    pncConfigPtr_pcst;

    SchM_Enter_ComM_ERANoNest();

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore
     * it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {

        ComSigId_u16 = COMM_GET_ERASIG_TABLE[PnBusId_u8];

        if(ComSigId_u16 != COMM_COMSIGNAL_INVALID_ID)
        {
            /* MR12 DIR 1.1 VIOLATION: Com_ReceiveSignal's SignalDataPtr return value should be void as per
             * Autosar guidelines. */
            (void)Com_ReceiveSignal(ComSigId_u16,&SigValue_au8);

            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is
             * actually the variable identified as the 'loop control variable.
             * Hence it is an incorrect warning reported by Misra */
            for(PncIndex_u8=0, pncRamPtr_pst = &ComM_PncRamStruct[0], pncConfigPtr_pcst = &(COMM_GET_PNC_LIST[0]); PncIndex_u8 < COMM_NO_OF_PNCS; PncIndex_u8++)
            {
                /* ActiveERACnt_u8 and ERACnt_u8 has to be updated only for the Gateway coordinated PNCs */
                if(pncConfigPtr_pcst->isPncGwCoordinated_b != FALSE)
                {
                    ERA_PncSigValue_b = ComM_Prv_Getbit(pncConfigPtr_pcst->PncId_u8 , SigValue_au8);

                    if(ComM_ERA_RxSigVal[PnBusId_u8][PncIndex_u8] != ERA_PncSigValue_b)
                    {
                        ComM_ERA_RxSigVal[PnBusId_u8][PncIndex_u8] = ERA_PncSigValue_b;

                        if(ERA_PncSigValue_b != FALSE)
                        {
                           /* ActiveERACnt_u8 and ERACnt_u8 has to be updated and read consistently. But with the below
                            * order of implementation, locks are not required  in the Pnc main function */
                           /* If ActiveERACnt_u8 is updated before ERACnt_u8,then
                            * even though main function interrupts this function, when ActiveERACnt_u8 is updated but
                            * ERACnt_u8 is not updated, state change will not happen and both these updated counters are
                            * considered only in the next Pnc main function. Thus avoiding locks.*/
                           /* If the channel is coordinated actively, then increment the ActiveERACnt_u8 counter */
                            if (channelGwType_en == COMM_GATEWAY_TYPE_ACTIVE)
                            {
                                ++pncRamPtr_pst->ActiveERACnt_u8;
                            }
                            ++(pncRamPtr_pst->ERACnt_u8);
                        }
                        else
                        {
                            /* ActiveERACnt_u8 and ERACnt_u8 has to be updated and read consistently. But with the below
                             * order of implementation, locks are not required  in the Pnc main function */
                            /* If ActiveERACnt_u8 is updated before ERACnt_u8,then
                             * even though main function interrupts this function, when ActiveERACnt_u8 is updated but
                             * ERACnt_u8 is not updated, state change will not happen and both these updated counters are
                             * considered only in the next Pnc main function. Thus avoiding locks.*/
                            /* If the channel is coordinated actively, then increment the ActiveERACnt_u8 counter */
                            if (channelGwType_en == COMM_GATEWAY_TYPE_ACTIVE)
                            {
                                --pncRamPtr_pst->ActiveERACnt_u8;
                            }
                            --(pncRamPtr_pst->ERACnt_u8);
                        }
                    }
                }
                pncRamPtr_pst++;
                pncConfigPtr_pcst++;
            }
        }
    }

    SchM_Exit_ComM_ERANoNest();

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif      /* (COMM_PNC_GW_ENABLED != STD_OFF)  */
#endif      /* (COMM_PNC_ENABLED != STD_OFF)  */


#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/* ComM_LgetLeastComMode */
/**************************************************************************************
 * Function name    : ComM_Prv_getLeastBusSmMode
 * Syntax           : ComM_Prv_getLeastBusSmMode
 *                    (
 *                    ComM_UserHandleType UserIndex
 *                    )
 * Description      : Internal function to get the least BusSM mode among all channels
 * Parameters       : UserIndex : User Id
 * Return value     : ComM_ModeType : Least busSm mode of all channels
 **************************************************************************************/

ComM_ModeType ComM_Prv_getLeastBusSmMode(ComM_UserHandleType UserIndex_tu8)

{

    /* Local variables declaration */

    ComM_ModeType           leastModeAmongChannels_tu8;
    ComM_UserVarType_tst *  userRamPtr_pst;

    userRamPtr_pst = &ComM_UserStruct[ComM_UserId_MappingTable_acst[UserIndex_tu8]];

    if(userRamPtr_pst->numChannelsInNoCom_u8 != C_ZERO)
    {
        leastModeAmongChannels_tu8 = COMM_NO_COMMUNICATION;
    }
    else if(userRamPtr_pst->numChannelsInSilentCom_u8 != C_ZERO)
    {
            leastModeAmongChannels_tu8 = COMM_SILENT_COMMUNICATION;
    }
    else
    {
        leastModeAmongChannels_tu8 = COMM_FULL_COMMUNICATION;
    }
    return leastModeAmongChannels_tu8;
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/***************************************************************************************************
 * Function name    : ComM_Prv_TranslateInhibitionStatus
 * Syntax           : ComM_Prv_TranslateInhibitionStatus
 *                    (
 *                     NetworkHandleType Channel,ComM_InhibitionType_ten InhibitionType, boolean Status
 *                    )
 * Description      : Local function to translate channels inhibition status to users and PNC's.
 *                    To be called only if the change in inhibition status is valid, as this check is not present
 *                    in the API
 * Parameters       : Channel -> Handle to identify a communication channel
 *                    InhibitionType -> prevent wake up or limit to no com
 *                    Status -> Inhibition requested or released status
 * Return value     : void
 **************************************************************************************************/

#if(COMM_INHIBITION_ENABLED)

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
void ComM_Prv_TranslateInhibitionStatus(NetworkHandleType          Channel_tu8,
                                         ComM_InhibitionType_ten    InhibitionType_en,
                                         boolean                    Status_b)
{
    /* Local variable declaration */

    const ComM_ChannelType_tst * channelConfigPtr_pcst;

    #if (COMM_PNC_ENABLED != STD_OFF)
    uint8 numPnc_u8;
    const uint8 * pncs_pcu8;
    #endif

    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel_tu8]);

    #if (COMM_PNC_ENABLED != STD_OFF)
    pncs_pcu8 = channelConfigPtr_pcst->Pncs_pcu8;
    #endif /*  (COMM_PNC_ENABLED != STD_OFF)  */
    /* For all users and PNC's of channel update inhibition counter depending on the inhibition type and requested status */

    /* IMP : API is called only if the change in inhibition status is valid */

    /* Depending on the inhibition type and requested status update inhibition counter
     * 1. For all direct users of channel
     * 2. For all PNC's of channel
     * 3. For all users assigned to PNC of channel */

    /* 1. For all direct users of the channel */
    ComM_Prv_TranslateInhibitionToUser(channelConfigPtr_pcst->DirectUsers_pcu8,channelConfigPtr_pcst->numDirectUsers_u8, InhibitionType_en,Status_b);

    #if(COMM_PNC_ENABLED != STD_OFF)
    /* If PNC is enabled, 2. For all PNC's of channel */

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore
     * it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {
        for(numPnc_u8 = C_ZERO; numPnc_u8 < channelConfigPtr_pcst->numPncs_u8; ++numPnc_u8)
        {
            uint8 pncId_u8;
            ComM_PncRamStructType_tst *         pncRamPtr_ps;
            const ComM_PncConfigStruct_tst *    pncConfigPtr_pcs;

            pncId_u8 = pncs_pcu8[numPnc_u8];
            pncRamPtr_ps = &ComM_PncRamStruct[pncId_u8];
            pncConfigPtr_pcs = &(COMM_GET_PNC_LIST[pncId_u8]);

            if(InhibitionType_en == COMM_PREVENTWAKEUP)
            {
                ComM_Prv_UpdateInhibitionCounter(&(pncRamPtr_ps->WakeUpInhibitionCtr_u16),Status_b,pncConfigPtr_pcs->NumChannels_u8);
            }
            else
            {
                ComM_Prv_UpdateInhibitionCounter(&(pncRamPtr_ps->LimitToNoComCtr_u16),Status_b,pncConfigPtr_pcs->NumChannels_u8);
            }

            /* 3. For all users assigned to PNC of channel  */
            ComM_Prv_TranslateInhibitionToUser(pncConfigPtr_pcs->Users_pcu8,pncConfigPtr_pcs->NumUsers_u8,InhibitionType_en,Status_b);

        }
    }
    #endif  /*  if(COMM_PNC_ENABLED != STD_OFF)  */
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif /*  (COMM_INHIBITION_ENABLED)  */

#if(COMM_INHIBITION_ENABLED)

/****************************************************************************************************************
 * Function name    : ComM_Prv_TranslateInhibitionToUser
 * Syntax           : ComM_Prv_TranslateInhibitionToUser
 *                    (
 *                     const uint8 * Users_p,ComM_InhibitionType_ten InhibitionType,
 *                     boolean Status,uint8 numUsers
 *                    )
 * Description      : Local function to translate channel's inhibition status to users.
 * Parameters       : Users_p -> Pointer to users of channel or PNC to which inhibition status has to be translated
 *                    numUsers -> Number of users of channel or PNC to which inhibition status has to be translated
 *                    InhibitionType -> prevent wake up or limit to no com
 *                    Status -> Inhibition requested or released status
 * Return value     : void
 ****************************************************************************************************************/

LOCAL_INLINE void ComM_Prv_TranslateInhibitionToUser(const uint8 *             Users_pcu8,
                                                      uint8                     numUsers_u8,
                                                      ComM_InhibitionType_ten   InhibitionType_en,
                                                      boolean                   Status_b)
{
    uint8                       loopCounter_u8;
    uint8                       userId_u8;
    ComM_UserVarType_tst *      userRamPtr_ps;
    const ComM_UsersType_tst *  userConfigPtr_pcs;

    for(loopCounter_u8 = C_ZERO; loopCounter_u8 < numUsers_u8; ++loopCounter_u8)
    {
        userId_u8 = Users_pcu8[loopCounter_u8];
        userConfigPtr_pcs = &(COMM_GET_USER_LIST[ComM_UserId_MappingTable_acst[userId_u8]]);
        userRamPtr_ps = &ComM_UserStruct[ComM_UserId_MappingTable_acst[userId_u8]];

        if(InhibitionType_en == COMM_PREVENTWAKEUP)
        {
            ComM_Prv_UpdateInhibitionCounter(&(userRamPtr_ps->WakeUpInhibitionCtr_u16),Status_b,userConfigPtr_pcs->NumAllChannels_u8);
        }
        else
        {
            ComM_Prv_UpdateInhibitionCounter(&(userRamPtr_ps->LimitToNoComCtr_u16),Status_b,userConfigPtr_pcs->NumAllChannels_u8);
        }
    }

}
#endif /*  (COMM_INHIBITION_ENABLED)  */


#if(COMM_INHIBITION_ENABLED)

/****************************************************************************************************************
 * Function name    : ComM_Prv_UpdateInhibitionCounter
 * Syntax           : ComM_Prv_UpdateInhibitionCounter
 *                    (
 *                     uint8 * InhibitionCounter_u8,
 *                     boolean Status, uint16 InhibitionCounterMax_u16
 *                    )
 * Description      : Local function to update inhibition counter depending on status.
 * Parameters       : InhibitionCounter_u8 -> Pointer to inhibition counter to be updated
 *                    Status -> Inhibition requested or released status
 *                    InhibitionCounterMax_u16 -> Max value of the Inhibition counter that can be incremented for PNC/Channel
 * Return value     : void
 ****************************************************************************************************************/

LOCAL_INLINE void ComM_Prv_UpdateInhibitionCounter(uint16 *  InhibitionCounter_pu16,
                                                   boolean   Status_b,
                                                   uint16    InhibitionCounterMax_u16)
{
    if(Status_b != FALSE)
    {
        if((*InhibitionCounter_pu16) == InhibitionCounterMax_u16 )
        {
            /* Control shouldn't come here.If at all happens, report to DET */
            #if (COMM_DEV_ERROR_DETECT != STD_OFF)
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_INHIBITION_COUNTER,COMM_E_IHHIBITION_CNTR_OVERFLW);
            return;
            #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
        }
        else
        {
            ++(*InhibitionCounter_pu16);
        }
    }
    else
    {
        if((*InhibitionCounter_pu16) == C_ZERO_U16 )
        {
            /* Control shouldn't come here.If at all happens, report to DET */
            #if (COMM_DEV_ERROR_DETECT != STD_OFF)
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_INHIBITION_COUNTER,COMM_E_IHHIBITION_CNTR_UNDERFLW);
            return;
            #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */
        }
        else
        {
            --(*InhibitionCounter_pu16);
        }
    }
}
#endif/*  (COMM_INHIBITION_ENABLED)  */


/***************************************************************************************************
 * Function name    : ComM_Prv_UpdateChannelModes
 * Syntax           : ComM_Prv_UpdateChannelModes
 *                    (
 *                     NetworkHandleType Channel,ComM_ModeType PreviousMode_tu8, ComM_ModeType CurrentMode_tu8
 *                    )
 * Description      : Internal function to update channel mode counters for a user
 *                    This function is called within channel lock, so consistency is taken care
 * Parameters       : Channel -> Handle to identify a communication channel
 *                    PreviousMode_tu8 -> Previous mode of the channel
 *                    CurrentMode_tu8 -> Changed mode of the channel
 * Return value     : void
 **************************************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

void ComM_Prv_UpdateChannelModes(NetworkHandleType  Channel_tu8,
                                  ComM_ModeType      PreviousMode_tu8,
                                  ComM_ModeType      CurrentMode_tu8)
{
    /* Local variable declaration */
    uint8                       numUsers_u8;
    uint8                       userId_u8;
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;

    ComM_UserVarType_tst *      userRamPtr_pst;
    const uint8 *               allUsers_pcu8;
    uint8                       userId_internal_u8;

    #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
    const ComM_UsersType_tst *  userConfigPtr_pcst;
    #endif

    channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[Channel_tu8]);
    allUsers_pcu8 = channelConfigPtr_pcst->AllUsers_pcu8;

    if(PreviousMode_tu8 != CurrentMode_tu8)
    {
        for(numUsers_u8= C_ZERO; numUsers_u8 < channelConfigPtr_pcst->numAllUsers_u8; ++numUsers_u8)
        {
            SchM_Enter_ComM_Channel();
            userId_u8 = allUsers_pcu8[numUsers_u8];
            userId_internal_u8 = ComM_UserId_MappingTable_acst[userId_u8];
            userRamPtr_pst = &ComM_UserStruct[userId_internal_u8];

            #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
            userConfigPtr_pcst = &(COMM_GET_USER_LIST[userId_internal_u8]);
            #endif  /*  if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */

            /* Depending on previous mode, decrement respective counter */
            switch(PreviousMode_tu8)
            {
                case COMM_NO_COMMUNICATION : --(userRamPtr_pst->numChannelsInNoCom_u8);
                    break;
                case COMM_FULL_COMMUNICATION: --(userRamPtr_pst->numChannelsInFullCom_u8);
                    break;
                case COMM_SILENT_COMMUNICATION: --(userRamPtr_pst->numChannelsInSilentCom_u8);
                    break;
                default:
                    /* Intentionally Empty */
                    break;
            }

            /* Depending on current mode, increment respective counter */
            switch(CurrentMode_tu8)
            {
                case COMM_NO_COMMUNICATION : ++(userRamPtr_pst->numChannelsInNoCom_u8);
                    break;
                case COMM_FULL_COMMUNICATION: ++(userRamPtr_pst->numChannelsInFullCom_u8);
                    break;
                case COMM_SILENT_COMMUNICATION: ++(userRamPtr_pst->numChannelsInSilentCom_u8);
                    break;
                default:
                    /* Intentionally Empty */
                    break;
            }

            #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
            /* If limit to no com is enabled and all channels of the user are in NO_COMMUNICATION,
             * clear the user request */
            if((userRamPtr_pst->LimitToNoComCtr_u16 > C_ZERO) &&
                    ((userRamPtr_pst->numChannelsInNoCom_u8) == (userConfigPtr_pcst->NumAllChannels_u8)))
            {
                (void)ComM_RequestComMode(userId_u8,COMM_NO_COMMUNICATION);
            }
            #endif /*  if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */

            #if(COMM_RTE_SUPPORT != STD_OFF)
            /* Notify RTE if the least ComMode of the user changes  */
            ComM_Prv_RteNotifyLowestComMode(userId_u8);
            #endif  /*  if(COMM_RTE_SUPPORT != STD_OFF)  */
            SchM_Exit_ComM_Channel();
        }
    }
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


#if (COMM_RTE_SUPPORT != STD_OFF)

/* ComM_Prv_RteNotifyLowestComMode */
/**************************************************************************************
 * Function name    : ComM_Prv_RteNotifyLowestComMode
 * Syntax           : ComM_Prv_RteNotifyLowestComMode
 *                    (
 *                     ComM_UserHandleType UserIndex
 *                    )
 * Description      : Internal function to notify the lowest com mode of user to RTE
 * Parameters       : UserIndex : User Id
 * Return value     : void
 **************************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

void ComM_Prv_RteNotifyLowestComMode(ComM_UserHandleType UserIndex_tu8)
{

    ComM_ModeType           lowestUserMode_tu8;
    ComM_UserVarType_tst *  userRamPtr_pst;
    uint8                   userId_internal_u8;

    userId_internal_u8 = ComM_UserId_MappingTable_acst[UserIndex_tu8];
    userRamPtr_pst = &ComM_UserStruct[userId_internal_u8];

    lowestUserMode_tu8 = ComM_Prv_getLeastBusSmMode(UserIndex_tu8);

    /* Notify the lowest mode to RTE only if there is a change in the mode w.r.t the
     * previously indicated mode */

    if(userRamPtr_pst->IndicatedUserMode_u8 != lowestUserMode_tu8)
    {
        userRamPtr_pst->IndicatedUserMode_u8 = lowestUserMode_tu8;
        ComM_Prv_Rte_Switch_UM_currentMode (UserIndex_tu8,lowestUserMode_tu8);
    }

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"
#endif  /*  if(COMM_RTE_SUPPORT != STD_OFF)  */



#if (COMM_SYNCHRONOUS_WAKE_UP != STD_OFF )

/* ComM_Prv_SyncWakeUpIndication */
/**************************************************************************************
 * Function name    : ComM_Prv_SyncWakeUpIndication
 * Syntax           : ComM_Prv_SyncWakeUpIndication
 *                    (
 *                     void
 *                    )
 * Description      : Internal function to set wakeup indication for all channels and PNC's
 *                    Used Only when synchronous wake up feature is enabled.
 * Parameters       : void
 * Return value     : void
 **************************************************************************************/

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

void ComM_Prv_SyncWakeUpIndication(void)
{
    uint8 loopCtr_u8;

    SchM_Enter_ComM_ChannelNoNest();

    for (loopCtr_u8 = C_ZERO; loopCtr_u8 < COMM_NO_OF_CHANNELS ;loopCtr_u8++)
    {
        ComM_ChannelVarType_tst * channelRamPtr_ps;
        channelRamPtr_ps = &ComM_ChannelStruct[loopCtr_u8];

        if(channelRamPtr_ps->ChannelMode_u8 == COMM_NO_COMMUNICATION)
        {
            COMM_SET_BIT((channelRamPtr_ps -> PassiveRequestState_u8),(0x01));
        }

    }
    SchM_Exit_ComM_ChannelNoNest();

    #if (COMM_PNC_ENABLED != STD_OFF)
    /* Check for PNC enabled */

    /* MR12 RULE 14.3 VIOLATION: The value of COMM_GET_FEATURE_PNC_ENABLED depends on the configuration therefore it will not always be true. */
    if(COMM_GET_FEATURE_PNC_ENABLED != FALSE)
    {
        ComM_PncRamStructType_tst * pncRamPtr_ps;
        SchM_Enter_ComM_PNCWakeUpNoNest();

        for (loopCtr_u8 = C_ZERO; loopCtr_u8 < COMM_NO_OF_PNCS ;loopCtr_u8++)
        {
            pncRamPtr_ps = &ComM_PncRamStruct[loopCtr_u8];

            if(pncRamPtr_ps->PncState_en == COMM_PNC_NO_COMMUNICATION)
            {
                pncRamPtr_ps->WakeUpStatus_b = TRUE;
            }
        }
        SchM_Exit_ComM_PNCWakeUpNoNest();
    }

    #endif  /*   (COMM_PNC_ENABLED != STD_OFF)  */
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif

#if (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)

/* ComM_Prv_ProcessLimitToNoCom */
/**************************************************************************************
 * Function name    : ComM_Prv_ProcessLimitToNoCom
 * Syntax           : ComM_Prv_ProcessLimitToNoCom
 *                    (
 *                     NetworkHandleType Channel,boolean Status
 *                    )
 * Description      : Internal function to set limit to NoCom
 * Parameters       : Channel-> The Network Channel for the requested state of ComM state machine.
 *                    Status-> Status of limit to no com
 * Return value     : E_OK: Successfully changed inhibition status for the channel
 *                    E_NOT_OK: Changed of inhibition status for the channel failed
 **************************************************************************************/
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

void ComM_Prv_ProcessLimitToNoCom(NetworkHandleType Channel_tu8, boolean Status_b)
{
     /* Local variable declaration */
    ComM_ChannelVarType_tst * channelRamPtr_pst;

    /* Local variable initialization */
    channelRamPtr_pst = &ComM_ChannelStruct[Channel_tu8];

    /* Proceed only if the request has changed from the previous request */
    if((COMM_GET_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8))) != Status_b)
    {
        SchM_Enter_ComM_LimitationNoNest();
        /* If Status is 'true', set Limit to no com bit, else reset the bit */
        if(Status_b != FALSE)
        {
            /* Set limit to no com Status_b for the channel */
            COMM_SET_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8));
            /* Update limit to no com counter for all users of channel */
            ComM_Prv_TranslateInhibitionStatus(Channel_tu8, COMM_LIMITTONOCOM, Status_b);
        }
        else
        {
            /* Clear limit to no com status for the channel */
            COMM_CLR_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8));
            /* Update limit to no com counter for all users of channel */
            ComM_Prv_TranslateInhibitionStatus(Channel_tu8, COMM_LIMITTONOCOM, Status_b);
        }
        SchM_Exit_ComM_LimitationNoNest();
    }

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#endif


/***************************************************************************************************
 * Function name    : ComM_Nm_TransmissionFailure
 * Syntax           : ComM_Nm_TransmissionFailure
 *                    (
 *                     NetworkHandleType Channel
 *                    )
 * Description      : Indicates that NM message transmission has failed
 * Parameters       : Channel -> Handle to identify a communication channel
 * Return value     : void
 **************************************************************************************************/

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
void ComM_Nm_TransmissionFailure
(
  NetworkHandleType Channel_tu8
)
{
    (void)(Channel_tu8);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


/*****************************************************************************************************
 * Function name    : ComM_Nm_NetworkTimeoutException
 * Syntax           : ComM_Nm_NetworkTimeoutException
 *                    (
 *                    NetworkHandleType Channel
 *                    )
 * Description      : Indicates that NM-Timeout time has expired when Network is still not released
 * Parameters       : Channel -> Handle to identify a communication channel
 * Return value     : void
 ****************************************************************************************************/

#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"
void ComM_Nm_NetworkTimeoutException
(
  NetworkHandleType Channel_tu8
)
{
    (void)(Channel_tu8);
}
#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

#if (COMM_PNC_ENABLED != STD_OFF)

/*****************************************************************************************************
 * Function name    : ComM_Prv_Getbit
 * Syntax           : ComM_Prv_Getbit
 *                    (
 *                    uint8 PncId, uint8 Signal_Array[COMM_PNC_VECTOR_LENGTH]
 *                    )
 * Description      :
 * Parameters       : PncId -> Id of PNC
 *                  : Signal_Array[COMM_PNC_VECTOR_LENGTH] -> Array where Calculated data needs to be copied
 * Return value     : 1 if the signal value is set.
 *                    0 if the signal value is reset.
 ****************************************************************************************************/

LOCAL_INLINE boolean ComM_Prv_Getbit(uint8 PncId_u8, const uint8 * PnSignalVal_pu8)
{
    uint8 mask_u8 = C_ZERO;
    uint8 byteIndex_u8 = C_ZERO;
    uint8 bitPosition_u8 = C_ZERO;
    uint8 Sig_Value_u8 = C_ZERO;
    boolean RetVal_b = FALSE;

    byteIndex_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION) >> 3;
    bitPosition_u8 = (PncId_u8 - (uint8)COMM_PNC_VECTOR_STARTBITPOSITION) % 8;
    mask_u8 = (uint8)1 << (bitPosition_u8) ;

    Sig_Value_u8 = ((*(PnSignalVal_pu8 + byteIndex_u8)) & (mask_u8));

    RetVal_b = (Sig_Value_u8 == 0) ? (FALSE) : (TRUE) ;

    return RetVal_b;
}
#endif        /* (COMM_PNC_ENABLED != STD_OFF) */
