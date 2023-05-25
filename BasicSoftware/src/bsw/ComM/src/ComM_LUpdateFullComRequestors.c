

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"

#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

ComM_UserHandleArrayType ComM_FullComRequestors[COMM_NO_OF_CHANNELS]; /*Global ram structure for FullComRequestors*/

/*
 ***************************************************************************************************
 * Functions
 ***************************************************************************************************
 */

LOCAL_INLINE void ComM_Prv_UpdateUser(ComM_UserHandleType User_tu8);

LOCAL_INLINE void ComM_Prv_DeleteUser(ComM_UserHandleType User_tu8);



#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/***************************************************************************************************
 * Function name    : ComM_Prv_UpdateFullComRequestors
 * Syntax           : void ComM_Prv_UpdateFullComRequestors(ComM_UserHandleType User, ComM_ModeType ComMode)
 * Description      : Update FullComRequestors for the channels corresponding to the user requesting COMM_FULL_COMMUNICATION mode.
 * Parameter        : User -> Handle of the user who requests a mode
                      ComMode -> COMM_FULL_COMMUNICATION / COMM_NO_COMMUNICATION
 * Return value     : void
 ***************************************************************************************************/
/* User Requesting Communication mode*/
void ComM_Prv_UpdateFullComRequestors(ComM_UserHandleType User_tu8, ComM_ModeType ComMode_tu8)
{
    /* Local Variables Declaration */
    ComM_UserVarType_tst *      userRamPtr_pst;
    const ComM_UsersType_tst *  userConfPtr_pcst;

    /* Local variable initialization */
    userRamPtr_pst = &ComM_UserStruct[ComM_UserId_MappingTable_acst[User_tu8]];
    userConfPtr_pcst = &(COMM_GET_USER_LIST[ComM_UserId_MappingTable_acst[User_tu8]]);

    if (ComMode_tu8 == COMM_FULL_COMMUNICATION)
    {
        #if (COMM_INHIBITION_ENABLED)
            if((userRamPtr_pst->LimitToNoComCtr_u16 != C_ZERO) ||
                ((userRamPtr_pst->WakeUpInhibitionCtr_u16 != C_ZERO) && (userRamPtr_pst->numChannelsInFullCom_u8 != userConfPtr_pcst->NumAllChannels_u8)))
            {
                /* If inhibition is active, then FULL_COM request for the user may not be processed for channels of the user
                     * which have inhibition enabled and FullComRequestors is not updated  */
            }
            else
            {
                /*update user in FullComRequestors*/
                ComM_Prv_UpdateUser(User_tu8);
            }
        #else
            {
                /*update user in FullComRequestors*/
                ComM_Prv_UpdateUser(User_tu8);
            }
        #endif

    }
    else if(ComMode_tu8 == COMM_NO_COMMUNICATION)
    {
        /*Delete user from FullComRequestors*/
        ComM_Prv_DeleteUser(User_tu8);

    }
    else
    {
        /*do nothing*/
    }

}


#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/***************************************************************************************************
 * Function name    : ComM_Prv_UpdateUser
 * Syntax           : void ComM_Prv_UpdateUser(ComM_UserHandleType User,ComM_ModeType ComMode)
 * Description      : Updating the user handle for coresponding channels in FullComRequestors.
 * Parameter        : User -> Handle of the user who requests a mode
 * Return value     : void
 ***************************************************************************************************/

LOCAL_INLINE void ComM_Prv_UpdateUser(ComM_UserHandleType User_tu8)
{

    /* Local Variables Declaration */
    ComM_UserHandleArrayType *  fullComRequestorRamPtr_pst;
    ComM_ChannelVarType_tst *   channelRamPtr_pst;
    const ComM_UsersType_tst *  userConfigPtr_pcst;
    ComM_UserHandleArrayType    fullComRequestor_st;
    uint8                       loopCounter_u8;
    uint8                       channelId_u8;
    uint8                       numChannels_u8;
    uint8                       Index_u8;

    /* Local variable initialization */
    userConfigPtr_pcst = &(COMM_GET_USER_LIST[ComM_UserId_MappingTable_acst[User_tu8]]);
    numChannels_u8 = userConfigPtr_pcst->NumDirectChannels_u8;

    /*loop through channels coresponding to user*/
    for(loopCounter_u8 = C_ZERO;loopCounter_u8 < numChannels_u8;loopCounter_u8++)
    {
       /*Get the channel id*/
       channelId_u8 = (userConfigPtr_pcst->DirectChannels_pcu8[loopCounter_u8]);

       channelRamPtr_pst = &ComM_ChannelStruct[channelId_u8];

       fullComRequestorRamPtr_pst = &ComM_FullComRequestors[channelId_u8];

       /*Critical section start*/
       SchM_Enter_ComM_FullComRequestors();

       /*Get the local copy of ComM_FullComRequestors for the channel id*/
       fullComRequestor_st.numberOfRequesters = fullComRequestorRamPtr_pst->numberOfRequesters;

       for(Index_u8 = 0;Index_u8 < COMM_MAX_CHANNEL_REQUESTERS;Index_u8++)
       {
           fullComRequestor_st.handleArray[Index_u8] = fullComRequestorRamPtr_pst->handleArray[Index_u8];

       }

       /*add userid into handlearray*/
       fullComRequestor_st.handleArray[fullComRequestor_st.numberOfRequesters] =User_tu8;

       /*increment numberOfRequesters*/
       fullComRequestor_st.numberOfRequesters++;

       /*update local copy to global ComM_FullComRequestors*/
       fullComRequestorRamPtr_pst->numberOfRequesters = fullComRequestor_st.numberOfRequesters;

       for(Index_u8 = 0;Index_u8 < COMM_MAX_CHANNEL_REQUESTERS;Index_u8++)
       {
           fullComRequestorRamPtr_pst->handleArray[Index_u8]=fullComRequestor_st.handleArray[Index_u8];

       }

       /*Critical section end*/
       SchM_Exit_ComM_FullComRequestors();

       /*Set a Flag for the call rte_Write to be called from ComM_MainFunction for corresponding to the channel*/

       channelRamPtr_pst->ComMFullComReqNotify_b = TRUE;
   }

}

/***************************************************************************************************
 * Function name    : ComM_Prv_DeleteUser
 * Syntax           : void ComM_Prv_DeleteUser(ComM_UserHandleType User)
 * Description      : delting the user handle for coresponding channels in FullComRequestors.
 * Parameter        : User -> Handle of the user who requests a mode
 * Return value     : void
 ***************************************************************************************************/

LOCAL_INLINE void ComM_Prv_DeleteUser(ComM_UserHandleType User_tu8)
{
    /* get the channel id;
     * get local copy of the fullcomrequestors using channelid as index
     * delete user from the handleArray by searching user in the array.
     * rearrange the array values such that invalid handle(0xFF) are after valid handle values.
     * Decrement numberOfRequesters
     * copy back to fullcomrequestors */

    /* Local Variables Declaration */
    ComM_UserHandleArrayType *  fullComReqRamPtr_pst;
    ComM_ChannelVarType_tst *   channelRamPtr_pst;
    const ComM_UsersType_tst *  userCfgPtr_pcs;
    ComM_UserHandleArrayType    fullComRequestors_st;
    uint8                       loopCount_u8;
    uint8                       channelId_u8;
    uint8                       numofChannels_u8;
    uint8                       arrayIndex_u8;
    uint8                       searchIndex_u8;
    uint8                       nextElementIndex_u8;

    /* Local variable initialization */
    userCfgPtr_pcs = &(COMM_GET_USER_LIST[ComM_UserId_MappingTable_acst[User_tu8]]);
    numofChannels_u8 = userCfgPtr_pcs->NumDirectChannels_u8;

    /*loop through channels coresponding to user*/
    for(loopCount_u8 = C_ZERO;loopCount_u8 < numofChannels_u8;loopCount_u8++)
    {
        /*Get the channel id*/
        channelId_u8 = (userCfgPtr_pcs->DirectChannels_pcu8[loopCount_u8]);

        fullComReqRamPtr_pst = &ComM_FullComRequestors[channelId_u8];

        channelRamPtr_pst = &ComM_ChannelStruct[channelId_u8];

        /*Critical section start*/
        SchM_Enter_ComM_FullComRequestors();

        /*Get the local copy of ComM_FullComRequestors for the channel id*/
        fullComRequestors_st.numberOfRequesters = fullComReqRamPtr_pst->numberOfRequesters;

        for(arrayIndex_u8 = 0;arrayIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS;arrayIndex_u8++)
        {
            fullComRequestors_st.handleArray[arrayIndex_u8] = fullComReqRamPtr_pst->handleArray[arrayIndex_u8];

        }

        /*Search user in the handlearray and invalidate the user*/
        searchIndex_u8 =0;

        while ((searchIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS) && (User_tu8 != fullComRequestors_st.handleArray[searchIndex_u8]))
        {

            searchIndex_u8++;
        }
        if(searchIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS)
        {
            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
             * Hence it is an incorrect warning reported by Misra */
            /*on sucessfull search, the searched user value is replaced with the next element in the array by shifting the elements of the array*/
            for(nextElementIndex_u8=searchIndex_u8+1;nextElementIndex_u8<COMM_MAX_CHANNEL_REQUESTERS;searchIndex_u8++)
            {
                fullComRequestors_st.handleArray[searchIndex_u8]=fullComRequestors_st.handleArray[nextElementIndex_u8];
                nextElementIndex_u8++;

            }
            /*Due to shift of elements of the array last element of the array is filled with invalid userid*/
            fullComRequestors_st.handleArray[nextElementIndex_u8-1]=COMM_NOT_USED_USER_ID;

            /*Decrement numberOfRequesters*/
            fullComRequestors_st.numberOfRequesters--;

            /*update local copy to global ComM_FullComRequestors*/
            fullComReqRamPtr_pst->numberOfRequesters = fullComRequestors_st.numberOfRequesters;

            for(arrayIndex_u8 = 0;arrayIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS;arrayIndex_u8++)
            {
                fullComReqRamPtr_pst->handleArray[arrayIndex_u8] = fullComRequestors_st.handleArray[arrayIndex_u8];
            }
            /*Set a Flag for the call rte_Write to be called from ComM_MainFunction for corresponding to the channel*/

            channelRamPtr_pst->ComMFullComReqNotify_b = TRUE;

        }
        else
        {
            /*user not found in the handlearray*/

        }
        /*Critical section end*/
        SchM_Exit_ComM_FullComRequestors();

    }

}
#endif




