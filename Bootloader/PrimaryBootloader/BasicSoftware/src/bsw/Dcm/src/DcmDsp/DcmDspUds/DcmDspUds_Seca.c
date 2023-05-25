
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Seca_Inf.h"
#include "Rte_Dcm.h"

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF))
#include "DcmDspUds_Seca_Prv.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Dcm_SecLevelType Dcm_dataSecLevel_u8;
static Dcm_SecLevelType CurrentSecLevel_u8;
static uint8 s_idxSecTab_u8;
static Std_ReturnType s_SrvRetValue_u8;
uint8  Dcm_DspSecTabIdx_u8;
uint8  Dcm_DspSecAccType_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"

#if( DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name  : Dcm_ResetAsynchSecaFlag
 Description    : Reset all the Flags for Asynchronous server call point handling
                  This API is invoked by Rdbi_Ini to reset Flags in case of Protocol Preemption
***********************************************************************************************************************/
void Dcm_ResetAsynchSecaFlag(void)
{
     /* Reset the Flags defined for Asynchronous server call point handling */
     s_Dcm_SecaRteCallPlaced_b = FALSE;
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CheckNRCForSeedRequest
 Description    : Perform NRC checks for Seed Request in the order
 *                  1: Check Attempt Counter exhausted
 *                  2: Check Security level available
 *                  3: DataRecordSize is incorrect
 *                  4: Failed attempt counter exceeds number of attempts
 *                  5: If Seed request is received when delay timer has not yet expired
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_01350], BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2949 */
static void Dcm_Prv_CheckNRCForSeedRequest(const Dcm_MsgContextType* pMsgContext,
                                           Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    *dataNegRespCode_u8 = 0x00u;
#if(DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
    Dcm_Prv_HasWaitCounterCycleExhausted(dataNegRespCode_u8);
    if(0x00 == *dataNegRespCode_u8)
#endif
    {
        *dataNegRespCode_u8 = (s_idxSecTab_u8 < DCM_CFG_DSP_NUMSECURITY)?0x00u:DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
    if(0x00 == *dataNegRespCode_u8)
    {
        /* As security level is valid the index will be within bounds
         * Pointer can be initialized with the security array now */
        Dcm_ptrSecurityConfig_pcst = &Dcm_Dsp_Security[s_idxSecTab_u8];

        if(pMsgContext->reqDataLen != (0x01u + Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32))
        {
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
        else if((Dcm_ptrSecurityConfig_pcst->NumAttLock_u8 > 0) &&
                (Dcm_Dsp_Seca[s_idxSecTab_u8].FailedAtm_cnt_u8 >= Dcm_ptrSecurityConfig_pcst->NumAttLock_u8))
        {
            *dataNegRespCode_u8 = DCM_E_EXCEEDNUMBEROFATTEMPTS;
        }
#if (DCM_CFG_DSP_DELAYTIMERMONITORING == DCM_PRV_REQUEST_SEED)
        /* Check whether delay timer is still running */
        else if(Dcm_Dsp_Seca[s_idxSecTab_u8].Residual_delay_u32 > Dcm_Dsp_SecaGlobaltimer_u32)
        {
            *dataNegRespCode_u8 = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
        }
#endif
        else
        {
            (void)(Dcm_GetSecurityLevel(&CurrentSecLevel_u8));
        }
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CheckNRCForKeyRequest
 Description    : Performs NRC Checks for Key request in the order
                   1: Requested Security level is configured
                   2: Request sequence error check (before key request, seed request should be successfully executed)
                   3: Key request length check
                   4: Residual delay check
***********************************************************************************************************************/
/* TRACE:[SWS_Dcm_CONSTR_6075] */
static void Dcm_Prv_CheckNRCForKeyRequest(const Dcm_MsgContextType* pMsgContext,
                                          Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    *dataNegRespCode_u8 = (s_idxSecTab_u8 < DCM_CFG_DSP_NUMSECURITY)?0x00u:DCM_E_SUBFUNCTIONNOTSUPPORTED;
    if(0x00u == *dataNegRespCode_u8)
    {
        Dcm_ptrSecurityConfig_pcst = &Dcm_Dsp_Security[s_idxSecTab_u8];
        Dcm_dataSecLevel_u8   = ((Dcm_DspSecAccType_u8+1)>>1u);
        if(pMsgContext->reqData[0] != (Dcm_DspSecAccType_u8 +1u))
        {
            *dataNegRespCode_u8 = DCM_E_REQUESTSEQUENCEERROR;
        }
        /* Since the level uses flexbile length, the length of key received should not
         * exceed the configured limit */
        else if((TRUE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength) &&
                (pMsgContext->reqDataLen > (Dcm_ptrSecurityConfig_pcst->Key_size_u32+1u)))
        {
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
        else if( (FALSE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength) &&
                (pMsgContext->reqDataLen != (Dcm_ptrSecurityConfig_pcst->Key_size_u32+1u)))
        {
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }

#if (DCM_CFG_DSP_DELAYTIMERMONITORING == DCM_PRV_SEND_KEY)
        else if(Dcm_Dsp_Seca[s_idxSecTab_u8].Residual_delay_u32 > Dcm_Dsp_SecaGlobaltimer_u32)
        {
            *dataNegRespCode_u8 = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
        }
#endif
        else
        {
            *dataNegRespCode_u8 = 0x00u;
        }
    }
}
/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessSeedRequest
 Description    : A received Seed request is first processed here
***********************************************************************************************************************/
static void Dcm_Prv_ProcessSeedRequest(Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8 loopcounter;
    *dataNegRespCode_u8 = 0x00;
    // Loop the security table array to point to requested Security level
    for(loopcounter=0x00u; (loopcounter < DCM_CFG_DSP_NUMSECURITY) &&
    (Dcm_Dsp_Security[loopcounter].Security_level_u8 != Dcm_dataSecLevel_u8); loopcounter++)
    {
        /* Do Nothing */
    }
    s_idxSecTab_u8 = loopcounter;

    // NRC Check for Seed Request
    Dcm_Prv_CheckNRCForSeedRequest(pMsgContext,dataNegRespCode_u8);

    if(0x00 == *dataNegRespCode_u8)
    {
        Dcm_DspSecAccType_u8 = pMsgContext->reqData[0]; /* Store for later use */
        Dcm_DspSecTabIdx_u8 = s_idxSecTab_u8;
        // Requested Security level is already active
        if(Dcm_dataSecLevel_u8 == CurrentSecLevel_u8)
        {
            Dcm_Prv_UpdateSeedResponseForActiveSec(pMsgContext,Dcm_ptrSecurityConfig_pcst->Seed_size_u32);
            s_SrvRetValue_u8 = E_OK;
            Dcm_DspSecTabIdx_u8 = 0x00u;
            Dcm_DspSecAccType_u8 = 0x00u;
        }
        else
        {
            #if(DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
            /* Seed storing is not applicable for flexible length handling */
            if( (FALSE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength) &&
                 Dcm_Prv_IsSeedAlreadyStored(Dcm_ptrSecurityConfig_pcst->Seed_size_u32,s_idxSecTab_u8) )
            {
                Dcm_Prv_UpdateBufferWithStoredSeed(pMsgContext,Dcm_ptrSecurityConfig_pcst->Seed_size_u32,s_idxSecTab_u8);
                s_SrvRetValue_u8 = E_OK;
            }
            else
            #endif
            {
                #if (DCM_CFG_IN_PARAM_MAXLEN >0u)
                Dcm_Prv_StoreSecurityAddressRecord(pMsgContext,Dcm_ptrSecurityConfig_pcst);
                #endif
                // Update State Machine to Next state to generate the seed from application callbacks
                Dcm_SrvOpstatus_u8 = DCM_CHECKDATA;
            }
        }
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessKeyRequest
 Description    : Newly received Key request is processed here.
                 1: Store the index of requested security level
                 2: Perform NRC checks
                 3: Update the state machine to perform key application handling
***********************************************************************************************************************/
static void Dcm_Prv_ProcessKeyRequest(const Dcm_MsgContextType* pMsgContext,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint8 loopcounter;
    *dataNegRespCode_u8 = 0x00;

    for(loopcounter=0x00u; (loopcounter < DCM_CFG_DSP_NUMSECURITY) &&
    (Dcm_Dsp_Security[loopcounter].Security_level_u8 != Dcm_dataSecLevel_u8);loopcounter++)
    {
        /* Do Nothing */
    }
    s_idxSecTab_u8 = loopcounter;
    // Perform NRC checks for received Key request
    Dcm_Prv_CheckNRCForKeyRequest(pMsgContext,dataNegRespCode_u8);
    if(0x00 == *dataNegRespCode_u8)
    {
        Dcm_SrvOpstatus_u8 = DCM_PROCESSSERVICE;
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_ResetAsynchSecaFlag
 Description    : Reset all the Flags for Asynchronous server call point handling
                  This API is invoked by Rdbi_Ini to reset Flags in case of Protocol Preemption
***********************************************************************************************************************/
void Dcm_ResetAccessType(void)
{
    Dcm_DspSecAccType_u8=0;
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_HandleNRCForCompareKey
 Description    : If compare Key opertaion failed, this function is called to perform and set NRC
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00660],[SWS_Dcm_01349],BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2947 */
static void Dcm_Prv_HandleNRCForCompareKey(Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint32 nrDelay_u32;
    Dcm_ptrSecurityConfig_pcst = &Dcm_Dsp_Security[s_idxSecTab_u8];
    *dataNegRespCode_u8 = DCM_E_INVALIDKEY; /* Invalid key is received */
    /* Check if the Delay Counter value has reached the max limit */
    if((Dcm_Dsp_Seca[s_idxSecTab_u8].FailedAtm_cnt_u8) < Dcm_ptrSecurityConfig_pcst->NumAttDelay_u8)
    {
        *dataNegRespCode_u8 = DCM_E_INVALIDKEY; /* Invalid key is received */
    }
    if((Dcm_Dsp_Seca[s_idxSecTab_u8].FailedAtm_cnt_u8) >= Dcm_ptrSecurityConfig_pcst->NumAttDelay_u8)
    {
        /* Check whether DelayTime_u32 is configured with value 0 */
        if(Dcm_ptrSecurityConfig_pcst->DelayTime_u32 > 0x00u)
        {
            nrDelay_u32 = ((Dcm_ptrSecurityConfig_pcst->DelayTime_u32) * DCM_CFG_TASK_TIME_MS);

            /* DcmAppl_DcmGetUpdatedDelayTime should return Residual Delay time in Counts of DcmTaskTime */
            nrDelay_u32 = DcmAppl_DcmGetUpdatedDelayTime(Dcm_ptrSecurityConfig_pcst->Security_level_u8,
                    Dcm_Dsp_Seca[s_idxSecTab_u8].FailedAtm_cnt_u8,
                    nrDelay_u32);
            Dcm_Dsp_Seca[s_idxSecTab_u8].Residual_delay_u32 = (Dcm_Dsp_SecaGlobaltimer_u32 + (nrDelay_u32 / DCM_CFG_TASK_TIME_MS));
        }
        else
        {
            /* Assign Residual_delay_u32 with the value 0,so that there will not be any delay effect and ecu will not be locked */
            Dcm_Dsp_Seca[s_idxSecTab_u8].Residual_delay_u32 = 0x00u;
        }

        #if (DCM_CFG_DSP_NRC0x36MAXDELAYCOUNT != DCM_CFG_OFF)
        *dataNegRespCode_u8 = DCM_E_EXCEEDNUMBEROFATTEMPTS;
        #else
        *dataNegRespCode_u8 = DCM_E_INVALIDKEY;
        #endif
    }
    /* If Configured value for Lock count is nonzero */
    if(Dcm_ptrSecurityConfig_pcst->NumAttLock_u8 > 0)
    {
        /* If FailedAtm_cnt_u8 exceeds maximum count */
        if(Dcm_Dsp_Seca[s_idxSecTab_u8].FailedAtm_cnt_u8 == Dcm_ptrSecurityConfig_pcst->NumAttLock_u8)
        {
            /* Provide Notification to DcmAppl that Lock counter is exceeded */
            DcmAppl_DcmSecurityLevelLocked((uint8)s_idxSecTab_u8);
            *dataNegRespCode_u8 = DCM_E_EXCEEDNUMBEROFATTEMPTS;
        }
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessKeyApplication
 Description    : The CompareKey application is called and based on its return further operations are done
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00325], [SWS_Dcm_01150] */
static void Dcm_Prv_ProcessKeyApplication(Dcm_MsgContextType* pMsgContext,
                                          Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ApplRet_u8;
#if (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
    uint32 SeedCounter_u32;
#endif
    s_SrvRetValue_u8 = E_NOT_OK;
    ApplRet_u8 = Dcm_Prv_ProcessCompKeySM(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);

    switch(ApplRet_u8)
    {
        case E_OK:
        {
            Dcm_DspChgSecLevel_b = TRUE;
#if (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
           /* Clear seed only when the security level is not of flexible type */
            if(FALSE == Dcm_Dsp_Security[s_idxSecTab_u8].UseFlexibleLength)
            {
                /* Clearing Stored Seed For the Security level */
                for(SeedCounter_u32=0x0;(SeedCounter_u32 < Dcm_Dsp_Security[s_idxSecTab_u8].Seed_size_u32);SeedCounter_u32++)
                {
                    /* Store the seed for use in Request KEY state */
                    Dcm_Dsp_Seca[s_idxSecTab_u8].SeedBuff[SeedCounter_u32] = 0x00;
                }
            }
#endif
            s_SrvRetValue_u8 = E_OK;
            *dataNegRespCode_u8 = 0x00;
        }
        break;

        case DCM_E_COMPARE_KEY_FAILED:
           Dcm_Prv_HandleNRCForCompareKey(dataNegRespCode_u8);
           break;

        case DCM_E_PENDING:
            s_SrvRetValue_u8 = DCM_E_PENDING;
            *dataNegRespCode_u8 = 0x00;
            break;

        default:
            if(0x00 == *dataNegRespCode_u8)
            {
                *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
            }
            break;
    }
}

#if(DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Dsp_SecaClearSeed
 Description    : The stored Seed data in SeedBuff is reset for all security levels
***********************************************************************************************************************/
void Dcm_Dsp_SecaClearSeed(void)
{
    uint8 idxSecTab_u8; /* Local variable to iterate through the security table */
    uint32 SeedIdx_u32;  /* Local variable to iterate through the seed buffer */
    uint32 Seedlen_u32;  /* Store length of seed for the respective security level */
    for (idxSecTab_u8=0;idxSecTab_u8 < DCM_CFG_DSP_NUMSECURITY; idxSecTab_u8++)
    {
        /* Clear the seed only when security level is not of flexible length type */
        if(FALSE == Dcm_Dsp_Security[idxSecTab_u8].UseFlexibleLength)
        {
            /* Obtain the seed length from the security level configuration */
            Seedlen_u32 = Dcm_Dsp_Security[idxSecTab_u8].Seed_size_u32;
            /* Loop for the seed size for this Access Type */
            for (SeedIdx_u32=0x0; (SeedIdx_u32 < Seedlen_u32); SeedIdx_u32++)
            {
                /* Store the seed for use in Request KEY state */
                Dcm_Dsp_Seca[idxSecTab_u8].SeedBuff[SeedIdx_u32] = 0x0;
            }
        }
    }
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessSeedApplication
 Description    : After Seed request is checked, relevant application is called to obtain Seed data
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00659] */
static void Dcm_Prv_ProcessSeedApplication(Dcm_MsgContextType* pMsgContext,
                                           Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    uint32 SeedLength_u32;
    Std_ReturnType applRetValue_u8;
    s_SrvRetValue_u8 = E_NOT_OK; // Initialize service return value to E_NOT_OK

#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)
    if(TRUE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength)
    {
        /* In case of Flexible length, seed length configuration contains the maximum seed size that can be sent
         * If configured Seed size is greater than TxBuffer size configured , then the Seed size is set to maximum buffer
         * size that is allowed and is sent to application to avoid buffer corruption */
        SeedLength_u32 = (Dcm_ptrSecurityConfig_pcst->Seed_size_u32 > (pMsgContext->resMaxDataLen -0x01u))?
                                (pMsgContext->resMaxDataLen -0x01u):(Dcm_ptrSecurityConfig_pcst->Seed_size_u32);
        applRetValue_u8 = Dcm_Prv_CallSeedApplicationForFlexibleLength(&SeedLength_u32,pMsgContext,
                                                                       Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
    }
    else
#endif
    {
        /* Seed length will be the configured seed size */
        SeedLength_u32 = Dcm_ptrSecurityConfig_pcst->Seed_size_u32;
        applRetValue_u8 = Dcm_Prv_CallSeedApplication(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
    }

    // Based on return value process further
    switch(applRetValue_u8)
    {
        case E_OK:
        {
            *dataNegRespCode_u8 = 0x00;
            /* Update the response data */
            pMsgContext->resData[0] = Dcm_DspSecAccType_u8;
            pMsgContext->resDataLen = (SeedLength_u32 + 1u);

            /* If seed storing is enabled, then store the seed for the current security level
             * For Flexible length handling, seed storing is not allowed */
            #if(DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
            if(FALSE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength)
            {
                Dcm_Prv_StoreSeed(SeedLength_u32,pMsgContext);
            }
            #endif
            Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
            s_SrvRetValue_u8=E_OK;
        }
        break;

        case DCM_E_PENDING:
        {
            Dcm_DspSecaOpStatus_u8 = DCM_PENDING;
            s_SrvRetValue_u8 = DCM_E_PENDING;
            *dataNegRespCode_u8 = 0x00;
        }
        break;

        default:
        {
            if(*dataNegRespCode_u8 == 0x00u)
            {
                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            }
            Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
        }
        break;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_ProcessNewSecaRequest
 Description      : Received SECA request is handled here
                    1: Identify whether the request is Seed/key request
                    2: Call appropriate Seed or Key handling functions
***********************************************************************************************************************/
static void Dcm_Prv_ProcessNewSecaRequest(Dcm_MsgContextType* pMsgContext,
                                          Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* Based on sub-function received, check whether it's a Seed request or a Key request */
    uint8 dataSubFunc_u8 = pMsgContext->reqData[0];
    *dataNegRespCode_u8 = 0x00;

    /* If subfunction is odd, then its Seed request */
    if((dataSubFunc_u8 & 0x01u) > 0u)
    {
        Dcm_dataSecLevel_u8 = ((dataSubFunc_u8 + 1u)>>1u);
        Dcm_Prv_ProcessSeedRequest(pMsgContext,dataNegRespCode_u8);
    }
    else if(dataSubFunc_u8 != 0x0u)
    {
        Dcm_dataSecLevel_u8 = (dataSubFunc_u8 >> 1u);
        Dcm_Prv_ProcessKeyRequest(pMsgContext,dataNegRespCode_u8);
    }
    else
    {
        /* Security level is not configured */
        *dataNegRespCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
    }
}

/***********************************************************************************************************************
 Function name    : Dcm_DcmSecurityAccess
 Description      : Security access Main function
                    Based on OpStatus appropriate processing is done
***********************************************************************************************************************/
 /* TRACE[SWS_Dcm_00252] */

Std_ReturnType Dcm_DcmSecurityAccess(Dcm_SrvOpStatusType OpStatus, Dcm_MsgContextType* pMsgContext,
                                     Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    s_SrvRetValue_u8 = E_NOT_OK;
    *dataNegRespCode_u8  = 0x0;

    /* If OpStatus is set to DCM_CANCEL then call the Ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dsp_SecaIni();
        s_SrvRetValue_u8 = E_OK;
    }
    else
    {
        /* Processing of a new SECA request is done in this state
         * Based on whether the request is Seed or Key further processing is done */
        if (Dcm_SrvOpstatus_u8==(DCM_INITIAL))
        {
            Dcm_Prv_ProcessNewSecaRequest(pMsgContext,dataNegRespCode_u8);
        }
        /* Seed application Handling */
        if (Dcm_SrvOpstatus_u8 ==(DCM_CHECKDATA))
        {
            Dcm_Prv_ProcessSeedApplication(pMsgContext,dataNegRespCode_u8);
        }
        /* Key application Handling */
        if (Dcm_SrvOpstatus_u8 ==(DCM_PROCESSSERVICE))
        {
            Dcm_Prv_ProcessKeyApplication(pMsgContext,dataNegRespCode_u8);
        }
        /* Check if the Negative Response Code is set */
        if(*dataNegRespCode_u8 != 0x0u)
        {
            /* Move back to Request seed state */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            /* Reset the globals to avoid Invalid Unlock Sequence */
            Dcm_DspSecAccType_u8 = 0x0;   /* Reset Access Type Stored */
            Dcm_DspSecTabIdx_u8  = 0x0;    /* Reset the Security table index Stored */
            s_SrvRetValue_u8=E_NOT_OK;
        }
    }
    return s_SrvRetValue_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif /* (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && ( DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF) */
