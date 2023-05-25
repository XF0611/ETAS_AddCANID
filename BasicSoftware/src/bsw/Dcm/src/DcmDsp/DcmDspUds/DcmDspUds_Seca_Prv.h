


#ifndef DCMDSPUDS_SECA_PRV_H
#define DCMDSPUDS_SECA_PRV_H

#include "DcmDspUds_Seca_Inf.h"

#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF))

/************** DEFINES***********************/
/* Defines for Compare Key StateMachine */
#define CK_CallAsyncAppl 0x00u
#define CK_CallDcmAppl 0x01u
#define CK_CallSetAttemptCounter 0x02u
#define DCM_FAILEDATMCOUNTER_MAXVALUE 0xFFu
/*********************************************/

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Std_ReturnType s_CompareKeyStatus_u8;
static uint8 CompareKey_StateMachine_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if( DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
static boolean s_Dcm_SecaRteCallPlaced_b; // For ASCP handling of Rte_Read and Rte_Result call
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
#endif


#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
extern uint8 Dcm_GetattemptCounterWaitCycle_u8;/*The counter for wait cycles of  GetSecurityAttemptCounter*/
#endif
extern uint8 Dcm_DspSecTabIdx_u8;
extern Dcm_SecLevelType  Dcm_dataSecLevel_u8;
extern uint8 Dcm_DspSecAccType_u8;
extern Dcm_OpStatusType Dcm_DspSecaOpStatus_u8;

#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"
extern boolean Dcm_DspChgSecLevel_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_Prv_DspSecurityConfirmation(
        Dcm_IdContextType dataIdContext_u8,        PduIdType dataRxPduId_u8,
        uint16 dataSourceAddress_u16,Dcm_ConfirmationStatusType status_u8);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/***********************************************************************************************************************
 *    FUNCTION-LIKE MACROS
***********************************************************************************************************************/
#define Dcm_Prv_CompareKeyActive()    ((CompareKey_StateMachine_u8 == CK_CallAsyncAppl)?TRUE:FALSE)
#define Dcm_Prv_ResetCKStateMachine() (CompareKey_StateMachine_u8 = CK_CallAsyncAppl)
/**********************************************************************************************************************/



/***********************************************************************************************************************
 Function name  : Dcm_Prv_HasWaitCounterCycleExhausted
 Description    : Set dataNegRespCode_u8 if GetAttemptCounter cycle has not expired
***********************************************************************************************************************/
#if(DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
LOCAL_INLINE void Dcm_Prv_HasWaitCounterCycleExhausted(Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    *dataNegRespCode_u8 = 0x00;
    if(Dcm_GetattemptCounterWaitCycle_u8 <= DCM_DSP_SECURITY_MAX_ATTEMPT_COUNTER_READOUT_TIME)
    {
        *dataNegRespCode_u8 = DCM_E_CONDITIONSNOTCORRECT;
    }
}
#endif


#if(DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name  : Dcm_Prv_IsSeedAlreadyStored
 Description    : For the security level, check whether the seed is already stored.
                   TRUE  -> If seed is already stored (atleast one seed buffer data is not zero)
                   FALSE -> If seed is not stored
***********************************************************************************************************************/
LOCAL_INLINE boolean Dcm_Prv_IsSeedAlreadyStored(const uint32 nrSeedLen_u32,const uint8 SecTabIdx_u8)
{
    boolean Flag = FALSE;
    uint32 idxSeed_u32;

    for(idxSeed_u32 =0; (idxSeed_u32 < nrSeedLen_u32); idxSeed_u32++)
    {
        /* Check if SeedBuffer is valid */
        if(Dcm_Dsp_Seca[SecTabIdx_u8].SeedBuff[idxSeed_u32] != 0x00u)
        {
            /* SeedBuffer has Valid Data Proceed Restoration */
            Flag = TRUE;
            break;
        }
    }
    return Flag;
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_StoreSeed
 Description    : Store the Seed returned by application for that security level in local Seed Buffer
***********************************************************************************************************************/
/* TRACE: BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2945 */
LOCAL_INLINE void Dcm_Prv_StoreSeed(const uint32 SeedLength_u32,const Dcm_MsgContextType* pMsgContext)
{
    uint32 idxSeed_u32;
    /* Loop for the seed size for this Access Type */
    for(idxSeed_u32=0x00; (idxSeed_u32 < SeedLength_u32); idxSeed_u32++)
    {
        /* Store the seed for use in Request Seed state */
        Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].SeedBuff[idxSeed_u32] = pMsgContext->resData[idxSeed_u32 + 0x1u];
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_UpdateBufferWithStoredSeed
 Description    : Update the stored Seed for the requested security level into Dcm response buffer
***********************************************************************************************************************/
/* TRACE: BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-2945 */
LOCAL_INLINE void Dcm_Prv_UpdateBufferWithStoredSeed(Dcm_MsgContextType* pMsgContext,const uint32 SeedLength_u32,
                                                     const uint8 SecTabIdx_u8)
{
    uint32 idxSeed_u32;
    for(idxSeed_u32 =0; (idxSeed_u32 < SeedLength_u32); idxSeed_u32++)
    {
        /* Store the seed in the response buffer */
        pMsgContext->resData[idxSeed_u32 + 0x1u] = Dcm_Dsp_Seca[SecTabIdx_u8].SeedBuff[idxSeed_u32];
    }
    /* Fill the Access Type in the response buffer */
    pMsgContext->resData[0] = pMsgContext->reqData[0];

    /* Update response data length */
    pMsgContext->resDataLen = (Dcm_MsgLenType)(idxSeed_u32 + 1u);
}

#endif /* DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF */

#if (DCM_CFG_IN_PARAM_MAXLEN >0u)

/***********************************************************************************************************************
 Function name  : Dcm_Prv_StoreSecurityAddressRecord
 Description    : Copy Security Access Data Record into Dcm_InParameterBuf which is then passed to application
***********************************************************************************************************************/
LOCAL_INLINE void Dcm_Prv_StoreSecurityAddressRecord(const Dcm_MsgContextType* pMsgContext,
                                                     const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst)
{
    if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 != 0x00u)
    {
        /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object
         * type pointer is converted to void pointer*/
        DCM_MEMCOPY(&(Dcm_InParameterBuf_au8[0]),&(pMsgContext->reqData[1]),Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32);
    }
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_UpdateSeedResponseForActiveSec
 Description    : Update Dcm Response buffer with zeros for the configured Seed Length
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00323] */
LOCAL_INLINE void Dcm_Prv_UpdateSeedResponseForActiveSec(Dcm_MsgContextType* pMsgContext,uint32 SeedSize_u32)
{
    uint32 idxSeedCounter;
    // Store SubFunction
    pMsgContext->resData[0] = Dcm_DspSecAccType_u8;
    // Update Seed Response to zeros
    for(idxSeedCounter = 0x01u; idxSeedCounter <= SeedSize_u32;idxSeedCounter++)
    {
        pMsgContext->resData[idxSeedCounter] = 0x00u;
    }
    pMsgContext->resDataLen = idxSeedCounter;
}

#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_CheckLengthReturned
 Description    : Check whether the Seed Length returned by application is within configured limits
                  configured Seed size has maximum seed length, in case the application returns a length greater than
                  this then set NRC General Reject and log DET
***********************************************************************************************************************/
/* TRACE: BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3949 */

LOCAL_INLINE Std_ReturnType Dcm_Prv_CheckLengthReturned(uint32 SeedLength_u32,const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_OK;
    if(SeedLength_u32 > Dcm_ptrSecurityConfig_pcst->Seed_size_u32)
    {
        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
        DCM_DET_ERROR(DCM_SECURITYACCESS_ID,DCM_E_INTERFACE_BUFFER_OVERFLOW);
        ReturnVal_u8 = E_NOT_OK;
    }
    return ReturnVal_u8;
}

#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_AsyncFncForFlexibleSeedApl
 Description    : Application API to obtain Seed in case when UsePort is configured to Async Fnc
                  SeedLength_u32 - InOut parameter.
                  During call -> It contains the maximum seed size that can be filled
                  After the application returns with E_OK, it contains the Flexible length filled by application

***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_AsyncFncForFlexibleSeedApl(uint32* SeedLength_u32,const Dcm_MsgContextType* pMsgContext,
                                                       const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                       Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_NOT_OK;
    if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 != 0x00u)
    {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr8))(
                Dcm_dataSecLevel_u8,               /* Security Level */
                SeedLength_u32, /* Seed size */
                Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32,/*ADR size*/
                &(Dcm_InParameterBuf_au8[0]),/* Security Access Data Record */
                &(pMsgContext->resData[1]), /* Buffer for Seed */
                Dcm_DspSecaOpStatus_u8,
                dataNegRespCode_u8);
#endif
    }
    else
    {
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr8))(
                Dcm_dataSecLevel_u8,               /* Security Level */
                SeedLength_u32, /* Seed size */
                Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32,/*ADR size*/
                NULL_PTR,/* Security Access Data Record */
                &(pMsgContext->resData[1]), /* Buffer for Seed */
                Dcm_DspSecaOpStatus_u8,
                dataNegRespCode_u8);
    }
    return ReturnVal_u8;
}
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_SSCPForFlexibleSeedApl
 Description    : Application API to obtain Seed in case of Synchronous server call point handling
                  SeedLength_u32 - InOut parameter.
                  During call -> It contains the maximum seed size that can be filled
                  After the application returns with E_OK, it contains the Flexible length filled by application

***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_SSCPForFlexibleSeedApl(uint32* SeedLength_u32,const Dcm_MsgContextType* pMsgContext,
                           const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_NOT_OK;
    if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 !=0u)
    {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
        /* Call the RTE port configured for this security level */
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr1))(SeedLength_u32,
                     &(Dcm_InParameterBuf_au8[0]),Dcm_DspSecaOpStatus_u8,&(pMsgContext->resData[1]),dataNegRespCode_u8);
#endif
    }
    else
    {
        /* Call the RTE port configured for this security level */
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr9))(SeedLength_u32,Dcm_DspSecaOpStatus_u8,
                         &(pMsgContext->resData[1]),dataNegRespCode_u8);
    }
    return ReturnVal_u8;
}

#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_ASCPForFlexibleSeedApl
 Description    : Application API to obtain Seed in case of asynchronous server call point handling
                  SeedLength_u32 - InOut parameter.
                  In RTE_CALL -> It is passed as In parameter (maximum seed size that can be filled)
                  In RTE_RESULT -> It is passed as Out parameter (Flexible seed length filled by application)
***********************************************************************************************************************/
/* BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3246 */
LOCAL_INLINE Std_ReturnType Dcm_Prv_ASCPForFlexibleSeedApl(uint32* SeedLength_u32,const Dcm_MsgContextType* pMsgContext,
                           const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType AplReturnval_u8 = RTE_E_TIMEOUT;
    /* Check whether the Rte_Read is already invoked */
    if (!s_Dcm_SecaRteCallPlaced_b)
    {
        if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 != 0x00u)
        {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
            /* Call the Rte_Call API configured for this security level */
            AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr10))(*SeedLength_u32,
                                                                  &(Dcm_InParameterBuf_au8[0]),Dcm_DspSecaOpStatus_u8);
#endif
        }
        else
        {
            /* Call the Rte_Call API configured for this security level */
            AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr11))(*SeedLength_u32,Dcm_DspSecaOpStatus_u8);
        }

        if (AplReturnval_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = TRUE;
            AplReturnval_u8 = DCM_E_PENDING;
        }
    }
    else
    {
        AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_ResultFp.Dcm_GetSeed_ptr12))
                                              (SeedLength_u32,&(pMsgContext->resData[1]),dataNegRespCode_u8);
        if (AplReturnval_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
        else if(AplReturnval_u8 == RTE_E_NO_DATA)
        {
            AplReturnval_u8 = DCM_E_PENDING;
        }
        else
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
    }
    return AplReturnval_u8;
}

#endif
#endif
/***********************************************************************************************************************
 Function name  : Dcm_Prv_CallSeedApplicationForFlexibleLength
 Description    : Call appropriate application function based on use port configured
                  The application functions are for Flexible Length of seed
                  There are three variations:
                  1: Asynch client server with Asynchronous server call point
                  2: Asynch client server with synchronous server call point
                  3: Async Fnc where application name is configured
***********************************************************************************************************************/
/* TRACE: BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3956 */
LOCAL_INLINE Std_ReturnType Dcm_Prv_CallSeedApplicationForFlexibleLength(uint32* SeedLength_u32,
                                                        const Dcm_MsgContextType* pMsgContext,
                                                        const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_NOT_OK;
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_CLIENT_SERVER)
    {
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
        if(Dcm_ptrSecurityConfig_pcst->UseAsynchronousServerCallPoint_b)
        {
            ReturnVal_u8 = Dcm_Prv_ASCPForFlexibleSeedApl(SeedLength_u32,pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        else
#endif
        {
            ReturnVal_u8 = Dcm_Prv_SSCPForFlexibleSeedApl(SeedLength_u32,pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        if(FALSE != Dcm_IsInfrastructureErrorPresent_b(ReturnVal_u8))
        {
            ReturnVal_u8 = E_NOT_OK;
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
        }
    }
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_FNC)
    {
        ReturnVal_u8 = Dcm_Prv_AsyncFncForFlexibleSeedApl(SeedLength_u32,pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
    }
#endif

    // Verify whether the seed length returned is within limits
    if(ReturnVal_u8 == E_OK)
    {
        ReturnVal_u8 = Dcm_Prv_CheckLengthReturned(*SeedLength_u32,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
    }
    return ReturnVal_u8;
}

#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name  : Dcm_Prv_AsyncFncForSeedApl
 Description    : Application API to obtain Seed in case when UsePort is configured to Async Fnc
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_AsyncFncForSeedApl(const Dcm_MsgContextType* pMsgContext,
                                                       const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                       Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_NOT_OK;
    uint32 nrSeedLen_u32 = Dcm_ptrSecurityConfig_pcst->Seed_size_u32;

    if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 != 0x00u)
    {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr4))(
                Dcm_dataSecLevel_u8,               /* Security Level */
                nrSeedLen_u32, /* Seed size */
                Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32,/*ADR size*/
                &(Dcm_InParameterBuf_au8[0]),/* Security Access Data Record */
                &(pMsgContext->resData[1]), /* Buffer for Seed */
                Dcm_DspSecaOpStatus_u8,
                dataNegRespCode_u8);
#endif
    }
    else
    {
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr4))(
                Dcm_dataSecLevel_u8,               /* Security Level */
                nrSeedLen_u32, /* Seed size */
                Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32,/*ADR size*/
                NULL_PTR,/* Security Access Data Record */
                &(pMsgContext->resData[1]), /* Buffer for Seed */
                Dcm_DspSecaOpStatus_u8,
                dataNegRespCode_u8);
    }
    return ReturnVal_u8;
}
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_SSCPForSeedApl
 Description    : Application API to obtain Seed in case when UsePort is configured to Async Client server
                  with synchronous server call point handling
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_SSCPForSeedApl(const Dcm_MsgContextType* pMsgContext,
                          const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                          Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8=E_NOT_OK;
    if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32 != 0u )
    {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
        /* Call the RTE port configured for this security level */
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr))(&(Dcm_InParameterBuf_au8[0]),
                Dcm_DspSecaOpStatus_u8,&(pMsgContext->resData[1]),dataNegRespCode_u8);
#endif
    }
    else
    {
        /* Call the RTE port configured for this security level */
        ReturnVal_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr3))(Dcm_DspSecaOpStatus_u8,
                &(pMsgContext->resData[1]),dataNegRespCode_u8);
    }
    return ReturnVal_u8;
}
#endif

#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_ASCPForSeedApl
 Description    : Application API to obtain Seed in case when UsePort is configured to Async Client server
                  with Asynchronous server call point handling
***********************************************************************************************************************/
/*  BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3246 */
LOCAL_INLINE Std_ReturnType Dcm_Prv_ASCPForSeedApl(const Dcm_MsgContextType* pMsgContext,
        const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType AplReturnval_u8 = RTE_E_TIMEOUT;
    /* Check whether the Rte_Read is already invoked */
    if (!s_Dcm_SecaRteCallPlaced_b)
    {
        if(Dcm_ptrSecurityConfig_pcst->AccDataRecsize_u32!=0u)
        {
#if (DCM_CFG_IN_PARAM_MAXLEN >0u)
            /* Call the Rte_Call API configured for this security level */
            AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr5))(&(Dcm_InParameterBuf_au8[0]),Dcm_DspSecaOpStatus_u8);
#endif
        }
        else
        {

            /* Call the Rte_Call API configured for this security level */
            AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_fp.Dcm_GetSeed_ptr6))(Dcm_DspSecaOpStatus_u8);
        }

        if (AplReturnval_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = TRUE;
            AplReturnval_u8 = DCM_E_PENDING;
        }
    }
    else
    {
        AplReturnval_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_GetSeed_ResultFp.Dcm_GetSeed_ptr7))(&(pMsgContext->resData[1]),dataNegRespCode_u8);
        if (AplReturnval_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
        else if(AplReturnval_u8 == RTE_E_NO_DATA)
        {
            AplReturnval_u8 = DCM_E_PENDING;
        }
        else
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
    }
    return AplReturnval_u8;
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CallSeedApplication
 Description    : Call appropriate application function based on use port configured
                  There are three variations:
                  1: Asynch client server with Asynchronous server call point
                  2: Asynch client server with synchronous server call point
                  3: Async Fnc where application name is configured
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00324], [SWS_Dcm_00862]*/
LOCAL_INLINE Std_ReturnType Dcm_Prv_CallSeedApplication(const Dcm_MsgContextType* pMsgContext,
                                                        const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ReturnVal_u8 = E_NOT_OK;
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_CLIENT_SERVER)
    {
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
        if(Dcm_ptrSecurityConfig_pcst->UseAsynchronousServerCallPoint_b)
        {
            ReturnVal_u8 = Dcm_Prv_ASCPForSeedApl(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        else
#endif
        {
            ReturnVal_u8 = Dcm_Prv_SSCPForSeedApl(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        if(FALSE != Dcm_IsInfrastructureErrorPresent_b(ReturnVal_u8))
        {
            ReturnVal_u8 = E_NOT_OK;
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
        }
    }
#endif

#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_FNC)
    {
        ReturnVal_u8 = Dcm_Prv_AsyncFncForSeedApl(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
    }
#endif
    return ReturnVal_u8;
}


#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)

#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name  : Dcm_Prv_SetAttemptCounterASP
 Description    : Passed the updated Failed attempt counter updated by Dcm to the application(ASCP)
                  - Only when Compare Key operation is succesful(E_OK) or the received KEy is invalid this function
                    is called
                  - In case of Successful operation - Failed Counter is passed as zero
                  - For incorrect KEy - Attempt counter is incremented by one and passed
***********************************************************************************************************************/

LOCAL_INLINE Std_ReturnType Dcm_Prv_SetAttemptCounterASP(const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst)
{
    Std_ReturnType dataRetValue_u8 = E_NOT_OK;
    if (!s_Dcm_SecaRteCallPlaced_b)
    {
        dataRetValue_u8 =(*(Dcm_ptrSecurityConfig_pcst->Dsp_SetAttempCounter_fp.Dcm_SetSecurityAttemptCounter_pfct))
                                    (Dcm_DspSecaOpStatus_u8, Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].FailedAtm_cnt_u8);

        if (dataRetValue_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = TRUE;
            dataRetValue_u8 = DCM_E_PENDING;
        }
    }
    else
    {
        dataRetValue_u8 =(*(Dcm_ptrSecurityConfig_pcst->Dsp_SetAttempCounter_Resultfp.Dcm_SetSecurityAttemptCounter_pfct1))();
        if (dataRetValue_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
        else if(dataRetValue_u8 == RTE_E_NO_DATA)
        {
            dataRetValue_u8 = DCM_E_PENDING;
        }
        else
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
    }
    return dataRetValue_u8;
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_SetAttemptCounter
 Description    : Passed the updated Failed attempt counter updated by Dcm to the application(ASCP)
                  - Only when Compare Key operation is succesful(E_OK) or the received KEy is invalid this function
                    is called
                  - In case of Successful operation - Failed Counter is passed as zero
                  - For incorrect KEy - Attempt counter is incremented by one and passed
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_SetAttemptCounter(const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                            /* MR12 RULE 8.13 VIOLATION: pointer needed for async client server use port*/
                                                      Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ApplReturn_u8 = E_OK;/* If Attempt counter is a Null pointer, return E_OK */
    if(Dcm_ptrSecurityConfig_pcst->Dsp_SetAttempCounter_fp.Dcm_SetSecurityAttemptCounter_pfct != NULL_PTR)
    {
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
        if (Dcm_ptrSecurityConfig_pcst->UseAsynchronousServerCallPoint_b)
        {
            ApplReturn_u8 = Dcm_Prv_SetAttemptCounterASP(Dcm_ptrSecurityConfig_pcst);
        }
        else
#endif
        {
            ApplReturn_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_SetAttempCounter_fp.Dcm_SetSecurityAttemptCounter_pfct))
              (Dcm_DspSecaOpStatus_u8, Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].FailedAtm_cnt_u8);
        }
#if (DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
        if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_CLIENT_SERVER)
        {
            if(FALSE != Dcm_IsInfrastructureErrorPresent_b(ApplReturn_u8))
            {
                *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                ApplReturn_u8= E_NOT_OK;
            }
        }
#endif
    }
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER == DCM_CFG_OFF)
    (void)dataNegRespCode_u8;
#endif
    return ApplReturn_u8;
}

#endif

#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ASCPForKeyApplFlexibleLen
 Description    : Asynchronous server call point for Flexible length compare key application
***********************************************************************************************************************/
/* BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3246 */
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE void Dcm_Prv_ASCPForKeyApplFlexibleLen(const Dcm_MsgContextType* pMsgContext,
        const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* Check whether the Rte_Read is already invoked */
    if (!s_Dcm_SecaRteCallPlaced_b)
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr6))
                ((pMsgContext->reqDataLen-1u),&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8);

        if (s_CompareKeyStatus_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = TRUE;
            s_CompareKeyStatus_u8 = DCM_E_PENDING;
        }
    }
    else
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_ResultFp.Dcm_CompareKey_ptr5))(dataNegRespCode_u8);
        if ((s_CompareKeyStatus_u8 == RTE_E_OK) || (s_CompareKeyStatus_u8 == DCM_E_COMPARE_KEY_FAILED))
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
        else if(s_CompareKeyStatus_u8 == RTE_E_NO_DATA)
        {
            s_CompareKeyStatus_u8 = DCM_E_PENDING;
        }
        else
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
    }
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CompareKeyApplForFlexibleLen
 Description    : Function to call either configured/Rte Generated Compare Key application when Flexible length
                     feature is used
***********************************************************************************************************************/
/* TRACE: BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3949 */
LOCAL_INLINE Std_ReturnType Dcm_Prv_CompareKeyApplForFlexibleLen(const Dcm_MsgContextType* pMsgContext,
                                                                 const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                                 Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_CLIENT_SERVER)
    {
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
        if (Dcm_ptrSecurityConfig_pcst->UseAsynchronousServerCallPoint_b)
        {
            Dcm_Prv_ASCPForKeyApplFlexibleLen(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        else
#endif
        {
            s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))
                    ((pMsgContext->reqDataLen-1u),&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8, dataNegRespCode_u8);
        }

        if(FALSE != Dcm_IsInfrastructureErrorPresent_b(s_CompareKeyStatus_u8))
        {
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            s_CompareKeyStatus_u8= E_NOT_OK;
        }
    }
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_FNC)
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))
                ((pMsgContext->reqDataLen-1u),&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8, dataNegRespCode_u8);
    }
#endif

    if(s_CompareKeyStatus_u8 != DCM_E_PENDING)
    {
        CompareKey_StateMachine_u8 = CK_CallDcmAppl;
    }
    return s_CompareKeyStatus_u8 ;
}
#endif

#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ASCPForKeyAppl
 Description    : Asynchronous server call point function for Compare Key
***********************************************************************************************************************/
/* BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-3246 */
LOCAL_INLINE void Dcm_Prv_ASCPForKeyAppl(const Dcm_MsgContextType* pMsgContext,
        const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
        Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    /* Check whether the Rte_Read is already invoked */
    if (!s_Dcm_SecaRteCallPlaced_b)
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr4))(&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8);

        if (s_CompareKeyStatus_u8 == RTE_E_OK)
        {
            s_Dcm_SecaRteCallPlaced_b = TRUE;
            s_CompareKeyStatus_u8 = DCM_E_PENDING;
        }
    }
    else
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_ResultFp.Dcm_CompareKey_ptr5))(dataNegRespCode_u8);
        if ((s_CompareKeyStatus_u8 == RTE_E_OK) || (s_CompareKeyStatus_u8 == DCM_E_COMPARE_KEY_FAILED))
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
        else if(s_CompareKeyStatus_u8 == RTE_E_NO_DATA)
        {
            s_CompareKeyStatus_u8 = DCM_E_PENDING;
        }
        else
        {
            s_Dcm_SecaRteCallPlaced_b = FALSE;
        }
    }
}
#endif

/***********************************************************************************************************************
 Function name  : Dcm_Prv_CompareKeyAppl
 Description    : Function to call either configured/Rte Generated Compare Key application
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_00324],[SWS_Dcm_00863] */
LOCAL_INLINE Std_ReturnType Dcm_Prv_CompareKeyAppl(const Dcm_MsgContextType* pMsgContext,
                                                   const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                   Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_CLIENT_SERVER)
    {
#if(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
        if (Dcm_ptrSecurityConfig_pcst->UseAsynchronousServerCallPoint_b)
        {
            Dcm_Prv_ASCPForKeyAppl(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        else
#endif
        {
            s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr1))
                                    (&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8, dataNegRespCode_u8);
        }
        /* If infrastructure error is present, then set NRC to general reject */
        if(FALSE != Dcm_IsInfrastructureErrorPresent_b(s_CompareKeyStatus_u8))
        {
            * dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            s_CompareKeyStatus_u8= E_NOT_OK;
        }
    }
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    if(Dcm_ptrSecurityConfig_pcst->usePort == USE_ASYNCH_FNC)
    {
        s_CompareKeyStatus_u8 = (*(Dcm_ptrSecurityConfig_pcst->Dsp_CompareKey_fp.Dcm_CompareKey_ptr3))
        (Dcm_ptrSecurityConfig_pcst->Key_size_u32,&(pMsgContext->reqData[1]),Dcm_DspSecaOpStatus_u8, dataNegRespCode_u8);
    }
#endif

    if(s_CompareKeyStatus_u8 != DCM_E_PENDING)
    {
        CompareKey_StateMachine_u8 = CK_CallDcmAppl;
    }
    return s_CompareKeyStatus_u8 ;
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessKeyResult
 Description    : Once the KEy application is called its result is processed here.
***********************************************************************************************************************/
/* TRACE: [SWS_Dcm_01397] */
LOCAL_INLINE void Dcm_Prv_ProcessKeyResult(Dcm_MsgContextType* pMsgContext)
{
    if(s_CompareKeyStatus_u8 == E_OK)
    {
        Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
        /* Reset Failed Attempt Counter */
        Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].FailedAtm_cnt_u8 = 0x00u;
        /* Fill response bytes */
        pMsgContext->resData[0] = (Dcm_DspSecAccType_u8+1u);
        pMsgContext->resDataLen = 0x1u;
    }
    else if(s_CompareKeyStatus_u8 == DCM_E_COMPARE_KEY_FAILED )
    {
        Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
        if(Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].FailedAtm_cnt_u8 < DCM_FAILEDATMCOUNTER_MAXVALUE)
        {
            /* Increment failed attempts counters */
            Dcm_Dsp_Seca[Dcm_DspSecTabIdx_u8].FailedAtm_cnt_u8++;
        }
    }
    else if(s_CompareKeyStatus_u8 == DCM_E_PENDING)
    {
        Dcm_DspSecaOpStatus_u8 = DCM_PENDING;
    }
    else
    {
        Dcm_DspSecaOpStatus_u8 = DCM_INITIAL;
    }
}

/***********************************************************************************************************************
 Function name  : Dcm_Prv_ProcessCompKeySM
 Description    : Compare Key State Machine to handle 3 application functions-
                   1: Configured/Rte generated Compare Key application
                   2: DcmAppl application
                   3: Set Attempt Counter application
***********************************************************************************************************************/
LOCAL_INLINE Std_ReturnType Dcm_Prv_ProcessCompKeySM(Dcm_MsgContextType* pMsgContext,
                                                     const Dcm_Dsp_Security_t* Dcm_ptrSecurityConfig_pcst,
                                                     Dcm_NegativeResponseCodeType* dataNegRespCode_u8)
{
    Std_ReturnType ApplReturn_u8 = E_NOT_OK;

    if(CompareKey_StateMachine_u8 == CK_CallAsyncAppl)
    {
#if(DCM_CFG_DSP_SECA_USEFLEXIBLELENGTH != DCM_CFG_OFF)
        if(TRUE == Dcm_ptrSecurityConfig_pcst->UseFlexibleLength)
        {
            ApplReturn_u8 = Dcm_Prv_CompareKeyApplForFlexibleLen(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }
        else
#endif
        {
            ApplReturn_u8 = Dcm_Prv_CompareKeyAppl(pMsgContext,Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
        }

        Dcm_Prv_ProcessKeyResult(pMsgContext);
    }
    if(CompareKey_StateMachine_u8 == CK_CallDcmAppl)
    {
        /* Depending on compare key status, call appropriate Dcm Appl to give notification*/
        ApplReturn_u8 = (s_CompareKeyStatus_u8 == E_OK)?
                        DcmAppl_DcmSecaValidKey((uint8)Dcm_DspSecTabIdx_u8):
                        DcmAppl_DcmSecaInvalidKey((uint8)Dcm_DspSecTabIdx_u8);
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
        if(ApplReturn_u8 != DCM_E_PENDING)
        {
            ApplReturn_u8 = E_OK;
            CompareKey_StateMachine_u8 = CK_CallSetAttemptCounter;
        }
#endif
    }
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
    if(CompareKey_StateMachine_u8 == CK_CallSetAttemptCounter)
    {
        if((s_CompareKeyStatus_u8 == E_OK) || (s_CompareKeyStatus_u8 == DCM_E_COMPARE_KEY_FAILED))
        {
            ApplReturn_u8 = Dcm_Prv_SetAttemptCounter(Dcm_ptrSecurityConfig_pcst,dataNegRespCode_u8);
            if(ApplReturn_u8 != DCM_E_PENDING)
            {
                ApplReturn_u8 = E_OK;
            }
        }
    }
#endif

    if(ApplReturn_u8 != DCM_E_PENDING)
    {
        CompareKey_StateMachine_u8 = CK_CallAsyncAppl;
        ApplReturn_u8 = (ApplReturn_u8 >s_CompareKeyStatus_u8)?ApplReturn_u8:s_CompareKeyStatus_u8;
    }
    return ApplReturn_u8;
}

#endif /* DCM_CFG_DSPUDSSUPPORT_ENABLED && DCM_CFG_DSP_SECURITYACCESS_ENABLED */
#endif /* DCMDSPUDS_SECA_PRV_H */
