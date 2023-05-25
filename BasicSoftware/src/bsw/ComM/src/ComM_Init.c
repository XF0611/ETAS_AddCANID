

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
#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
LOCAL_INLINE boolean ComM_Prv_GetNvMInhibitionStatus
                                                (
                                                 uint8 ChannelId_u8, const uint8 * NvM_InhibitionArray_pcu8
                                                );
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)
LOCAL_INLINE uint32 ComM_Prv_CompareVersionInfo(const Std_VersionInfoType * srcVersionInfo,
                                            const Std_VersionInfoType * destVersionInfo);
#endif

#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)
const ComM_GlobalConfigType_tst * ComM_GlobalConfigPtr_pgcs;
#endif


#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"


/***************************************************************************************************
 Function name    : ComM_Init
 Syntax           : void ComM_Init(void)
 Description      : Initializes the AUTOSAR Communication Manager.
 Parameters       : void
 Return value     : void
 ***************************************************************************************************/
void ComM_Init(const ComM_ConfigType * config)

{
    /* Local Variables Declaration */
    uint8                           ChannelIndex_u8;
    ComM_UserHandleType             UserIndex_tu8;
    ComM_ChannelVarType_tst *       channelRamPtr_pst;
    ComM_UserVarType_tst *          userRamPtr_pst;
    #if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
    uint8                           Index_u8;
    ComM_UserHandleArrayType *      fullComRequestorRamPtr_pst;
    #endif

    #if(COMM_NVM_ENABLED)
    NvM_RequestResultType           NvM_Status_tu8;
    Std_ReturnType                  Ret_NvM_Readval_u8;
    Std_ReturnType                  Ret_NvM_Error_Statusval_u8;
    #endif/*(COMM_NVM_ENABLED)*/

    ComM_GlobalVarType_tst *        globalRamPtr_pst;
    const ComM_UsersType_tst *      userConfigPtr_pcst;
    #if(COMM_INHIBITION_ENABLED)
    const ComM_ChannelType_tst *    channelConfigPtr_pcst;
    #endif  /*  (COMM_INHIBITION_ENABLED)  */

    #if (COMM_PNC_ENABLED != STD_OFF)
    uint8                           PncIndex_u8;
    ComM_PncRamStructType_tst *     pncRamPtr_pst;
    #endif

#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)

    Std_VersionInfoType versionInfo;

    /*****************************************DET START************************************************/
    #if (COMM_DEV_ERROR_DETECT == STD_ON)
    /* Check if pointer passed is valid */
    if (config == NULL_PTR)
    {
        /* Report DET with ComM not initialized*/
        (void) Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_INIT_APIID,COMM_E_WRONG_PARAMETERS);
        return;
    }
    #endif
    /*********************************************DET END************************************************/

    /* Read the pre-compile version info  */
    ComM_GetVersionInfo(&versionInfo);
    /* Compare pre-compile & post-build version info  */
    if (ComM_Prv_CompareVersionInfo(&versionInfo, config->versionInfo) != C_ZERO)
    {
        /* versionInfo doesnt match .. Inform application */
        ComMAppl_IncompatibleGenerator();
        /* Abort further initialisation ..CanNm will remain in NM_STATE_UNINIT state */
        return;
    }

    /* Get Pointer to Global config structure through the Module Base pointer */
    /* MR12 RULE 11.5 VIOLATION:void pointer is been converted to pointer to ComM_GlobalConfigType_tst before using*/
    ComM_GlobalConfigPtr_pgcs = (const ComM_GlobalConfigType_tst * )(config->ComM_GlobalConfigData_pcs);

#else
    /* If config variant is Pre compile, then config is unused */
    (void)config;
#endif

    globalRamPtr_pst = &ComM_GlobalStruct;
    #if(COMM_NVM_ENABLED)
    NvM_Status_tu8 = C_ZERO;
    #endif/*(COMM_NVM_ENABLED)*/

    #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
        /* Default values are set to ComM_ChannelStruct */
    globalRamPtr_pst->ComM_LimitECUToNoCom_b = FALSE;
    #endif  /*  (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */

    /* IMP : initialization of User and PNC ram structures needs to be done before channel structure,
     * so as to ensure correct values for inhibition counters */
#if(COMM_INHIBITION_ENABLED)
    #if(COMM_NVM_ENABLED)

        /*MR12 DIR 1.1 VIOLATION: NvM_ReadBlock's NvM_DstPtr return value should be void as per Autosar guidelines.*/
        Ret_NvM_Readval_u8 = NvM_ReadBlock(COMM_NVM_BLOCK_REFERENCE,&ComM_NvMblock);

        Ret_NvM_Error_Statusval_u8 = NvM_GetErrorStatus(COMM_NVM_BLOCK_REFERENCE,&NvM_Status_tu8);

        if((Ret_NvM_Readval_u8 == E_OK  ) && (Ret_NvM_Error_Statusval_u8 == E_OK))
        {

            /*Copying of data from the variables which are stored in NvM*/
            ComM_GlobalStruct.ComM_EcuGroupClassification_u8 = ComM_NvMblock.ComMEcuGroupClassification_u8;
            ComM_GlobalStruct.ComM_InhibitCounter_u16 = ComM_NvMblock.Inhibitioncounter_u16;

            /*Loop through all Channel to read the ComMNoWakeup value stored in NvM */
            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for'
             *  statement is actually the variable identified as the 'loop control variable.
             * Hence it is an incorrect warning reported by Misra */
            for (ChannelIndex_u8 = C_ZERO, channelRamPtr_pst = &ComM_ChannelStruct[C_ZERO]; ChannelIndex_u8 < COMM_NO_OF_CHANNELS ;++ChannelIndex_u8)
            {
                channelConfigPtr_pcst= &(COMM_GET_CHANNEL_LIST[ChannelIndex_u8]);
                if(channelConfigPtr_pcst->ComMNoWakeUpInhibitionNvmStorage_b == TRUE)
                {
                    /*MR12 RULE 10.3 VIOLATION:  An expression of 'essentially Boolean' type (_Bool) is being converted to unsigned type, 'unsigned char' on assignment.
                     *This is a Incorrect Misra Warning. This is taken care in review.*/
                    /*Invoking a Internal function which extract the bit position from an array*/
                    channelRamPtr_pst->InhibitionReqStatus_u8 = ComM_Prv_GetNvMInhibitionStatus(ChannelIndex_u8, ComM_NvMblock.ComMNoWakeup_u8);
                }
                else
                {
                    /*Store the value from the Config Structure*/
                    channelRamPtr_pst->InhibitionReqStatus_u8 = channelConfigPtr_pcst->InhibitionInitValue_u8;
                }
                ++channelRamPtr_pst;
            }
        }
        else
    #endif/* (COMM_NVM_ENABLED) */
        {

            globalRamPtr_pst->ComM_EcuGroupClassification_u8 = ComM_EcuGroupClassification_Init;
            globalRamPtr_pst->ComM_InhibitCounter_u16 = C_ZERO;
            for (ChannelIndex_u8 = C_ZERO; ChannelIndex_u8 < COMM_NO_OF_CHANNELS ;++ChannelIndex_u8)
            {
                channelRamPtr_pst = &ComM_ChannelStruct[ChannelIndex_u8];
                channelConfigPtr_pcst= &(COMM_GET_CHANNEL_LIST[ChannelIndex_u8]);
                channelRamPtr_pst->InhibitionReqStatus_u8 = channelConfigPtr_pcst->InhibitionInitValue_u8;
            }
        }
#else
        for (ChannelIndex_u8 = C_ZERO ; ChannelIndex_u8 < COMM_NO_OF_CHANNELS ; ChannelIndex_u8++)
        {
            channelRamPtr_pst = &ComM_ChannelStruct[ChannelIndex_u8];
            channelRamPtr_pst->InhibitionReqStatus_u8 = C_ZERO;
        }
#endif  /* (COMM_INHIBITION_ENABLED) */


    /* Default value for all user related variables */
    for (UserIndex_tu8 = C_ZERO ; UserIndex_tu8 < COMM_NO_OF_USERS ; UserIndex_tu8++)
    {
        userRamPtr_pst = &ComM_UserStruct[UserIndex_tu8];
        userConfigPtr_pcst = &(COMM_GET_USER_LIST[UserIndex_tu8]);
        userRamPtr_pst->RequestedUserMode_u8 = COMM_NO_COMMUNICATION;
        userRamPtr_pst->numChannelsInFullCom_u8 = C_ZERO;
        userRamPtr_pst->numChannelsInSilentCom_u8 = C_ZERO;
        userRamPtr_pst->numChannelsInNoCom_u8 = userConfigPtr_pcst->NumAllChannels_u8;

        /* Initialise the indicated mode for all users as NO_COMMUNICATION */
        userRamPtr_pst->LimitToNoComCtr_u16 = C_ZERO;
        userRamPtr_pst->WakeUpInhibitionCtr_u16 = C_ZERO;

        userRamPtr_pst->IndicatedUserMode_u8 = COMM_NO_COMMUNICATION;
    }

    #if (COMM_PNC_ENABLED != STD_OFF)
    /* Initialize all the PNC ram structures */
    for (PncIndex_u8 = C_ZERO; PncIndex_u8 < COMM_NO_OF_PNCS ;PncIndex_u8++)
    {
        pncRamPtr_pst = &ComM_PncRamStruct[PncIndex_u8];
        #if (COMM_PNC_GW_ENABLED != STD_OFF)
        pncRamPtr_pst->ActiveERACnt_u8 = C_ZERO;
        pncRamPtr_pst->ERACnt_u8 = C_ZERO;
        pncRamPtr_pst->ActiveERAStateOld_b = FALSE;
        #endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF)  */
        pncRamPtr_pst->EIRACnt_u8 = C_ZERO;
        pncRamPtr_pst->PncState_en = COMM_PNC_NO_COMMUNICATION;
        pncRamPtr_pst->PrepareSleepTimer_u16 = C_ZERO;
        pncRamPtr_pst->UserRequestCnt_u8 = C_ZERO;
        pncRamPtr_pst->WakeUpStatus_b = FALSE;
        #if(COMM_INHIBITION_ENABLED)
        /* Initialize inhibition counters to C_ZERO, this will be later intialized in the channel structure
         * depending on inhibition value */
        pncRamPtr_pst->LimitToNoComCtr_u16 = C_ZERO;
        pncRamPtr_pst->WakeUpInhibitionCtr_u16 = C_ZERO;
        #endif  /* (COMM_INHIBITION_ENABLED)  */
    }
    #endif  /*  (COMM_PNC_ENABLED != STD_OFF)  */

    /* Default value for all channel related variables */
    for (ChannelIndex_u8 = C_ZERO ; ChannelIndex_u8 < COMM_NO_OF_CHANNELS ; ChannelIndex_u8++)
    {
        channelRamPtr_pst = &ComM_ChannelStruct[ChannelIndex_u8];
        #if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
        fullComRequestorRamPtr_pst = &ComM_FullComRequestors[ChannelIndex_u8];
        #endif
        #if(COMM_INHIBITION_ENABLED)
        channelConfigPtr_pcst = &(COMM_GET_CHANNEL_LIST[ChannelIndex_u8]);
        #endif  /* (COMM_INHIBITION_ENABLED)  */
        /* Default values are set to ComM_ChannelStruct */
        channelRamPtr_pst->CommunicationAllowed_b = FALSE;
        channelRamPtr_pst->ChannelState_e = COMM_NO_COM_NO_PENDING_REQUEST;
        channelRamPtr_pst->ChannelMode_u8 = COMM_NO_COMMUNICATION;
        channelRamPtr_pst->BusSmMode_u8 = COMM_NO_COMMUNICATION;
        channelRamPtr_pst->PassiveRequestState_u8 = C_ZERO;
        channelRamPtr_pst->UserRequestCtr_u16 = C_ZERO_U16;
        channelRamPtr_pst->PncRequestCtr_u8 = C_ZERO;
        #if(COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)
        if(COMM_GET_LIMIT_TO_NO_COM((channelRamPtr_pst->InhibitionReqStatus_u8)) != FALSE)
        {
            ComM_Prv_TranslateInhibitionStatus(ChannelIndex_u8,COMM_LIMITTONOCOM,TRUE);
        }
        #endif  /* (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */
        #if(COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)
        if(COMM_GET_PREVENT_WAKEUP((channelRamPtr_pst->InhibitionReqStatus_u8)) != FALSE)
        {
            ComM_Prv_TranslateInhibitionStatus(ChannelIndex_u8,COMM_PREVENTWAKEUP,TRUE);
        }
        #endif  /* (COMM_PREVENT_WAKEUP_ENABLED != STD_OFF)  */
        channelRamPtr_pst->NmBusSleepIndicationStatus_b = FALSE;
        channelRamPtr_pst->NmPrepareBusSleepIndicationStatus_b = FALSE;
        channelRamPtr_pst->DiagnosticRequestState_b = COMM_INACTIVE_DIAGNOSTICS;
        channelRamPtr_pst->NmNetworkModeStatus_b = FALSE;
        channelRamPtr_pst->NmNetworkRequestStatus_b = FALSE;
        channelRamPtr_pst->MinFullComTimeoutCtr_u16 = C_ZERO;
        #if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
        channelRamPtr_pst->ComMFullComReqNotify_b = FALSE;
        fullComRequestorRamPtr_pst->numberOfRequesters = C_ZERO;
        for(Index_u8 = 0;Index_u8 < COMM_MAX_CHANNEL_REQUESTERS;Index_u8++)
        {
           fullComRequestorRamPtr_pst->handleArray[Index_u8]=COMM_NOT_USED_USER_ID;
        }
        #endif
    }
    /*  Assign the status of ComM as Initialized*/
    globalRamPtr_pst->ComM_InitStatus_en = COMM_INIT;

}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"

/**
 ***************************************************************************************************
 * ComM_Prv_CompareVersionInfo - compare version info structures ie of type Std_VersionInfoType
 *
 * compare version info structures ie of type Std_VersionInfoType.
 *
 * \param   srcVersionInfo      pointer to first Std_VersionInfoType structure
 * \param   destVersionInfo     pointer to second Std_VersionInfoType structure
 * \return  stEqual_u32         result of comparison
 * \retval  0                   memory area contents are equal
 * \retval  1                   memory area contents differ
  ***************************************************************************************************
 */
#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)

LOCAL_INLINE uint32 ComM_Prv_CompareVersionInfo(const Std_VersionInfoType * srcVersionInfo,
                                            const Std_VersionInfoType * destVersionInfo)
{
    uint32 stEqual_u32;
    stEqual_u32 = 1;
    if ((srcVersionInfo->vendorID == destVersionInfo->vendorID) &&
        (srcVersionInfo->moduleID == destVersionInfo->moduleID) &&
        (srcVersionInfo->sw_major_version == destVersionInfo->sw_major_version) &&
        (srcVersionInfo->sw_minor_version == destVersionInfo->sw_minor_version) &&
        (srcVersionInfo->sw_patch_version == destVersionInfo->sw_patch_version)
    )
    {
        stEqual_u32 = 0;
    }
    return stEqual_u32;
}

#endif

/*****************************************************************************************************
 * Function name    : ComM_Prv_GetNvMInhibitionStatus
 * Syntax           : ComM_Prv_GetNvMInhibitionStatus
 *                    (
 *                    uint8 BitPn, uint8 Byte_Val
 *                    )
 * Description      : Internal function to read status of the NoWakeup(per channel) from NvM
 * Parameters       : ChannelId_u8 -> Channel handle Id
 *                  : NvM_InhibitionArray_pcu8 -> Array from which bit value has to be extracted.
 * Return value     : 1 if the value is set.
 *                    0 if the value is reset.
 ****************************************************************************************************/

#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
LOCAL_INLINE boolean ComM_Prv_GetNvMInhibitionStatus(uint8 ChannelId_u8, const uint8 * NvM_InhibitionArray_pcu8)

{
    /* Local variable declaration */
    uint8 byteIndex_u8, bitPosition_u8, Value_u8, mask_u8;
    boolean RetVal_b;

    /* Local variable initialization*/
    byteIndex_u8 = (ChannelId_u8 ) >> 3;
    bitPosition_u8 = (ChannelId_u8) % 8;
    mask_u8 = (uint8)1 << (bitPosition_u8);

    Value_u8 = ((NvM_InhibitionArray_pcu8[byteIndex_u8]) & (mask_u8)) ;
    RetVal_b = (Value_u8 == 0) ? (FALSE) : (TRUE) ;

    return RetVal_b ;

}
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */
