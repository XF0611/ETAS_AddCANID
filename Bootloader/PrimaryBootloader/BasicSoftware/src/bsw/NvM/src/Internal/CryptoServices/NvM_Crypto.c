
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "rba_MemLib.h"

#include "NvM_Prv_Crypto.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_JobResource.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_Crc.h"
#include "NvM_Prv_Csm.h"
#include "NvM_Prv_Crypto_ServiceTypes.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

#include "NvM_Prv_Crypto_InternalTypes.h"

# define NVM_START_SEC_CODE
# include "NvM_MemMap.h"

static NvM_Prv_Job_StateMachine_tpfct NvM_Prv_Crypto_GetStateMachine(NvM_Prv_idJob_ten idJob_en);

static NvM_Prv_Job_State_tpfct NvM_Prv_Crypto_GetJobStateWrite(NvM_Prv_stJob_ten* stJob_pen);

static NvM_Prv_Job_State_tpfct NvM_Prv_Crypto_GetJobStateRead(NvM_Prv_stJob_ten* stJob_pen);

static void NvM_Prv_Crypto_PollJob(NvM_Prv_stJob_ten* stJob_pen,
                                   NvM_Prv_JobResult_tst* JobResult_pst,
                                   NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartRandomGeneration(NvM_Prv_stJob_ten* stJob_pen,
                                                 NvM_Prv_JobResult_tst* JobResult_pst,
                                                 NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartEncryption(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartDecryption(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartSignatureGeneration(NvM_Prv_stJob_ten* stJob_pen,
                                                    NvM_Prv_JobResult_tst* JobResult_pst,
                                                    NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartSignatureVerification(NvM_Prv_stJob_ten* stJob_pen,
                                                      NvM_Prv_JobResult_tst* JobResult_pst,
                                                      NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_Crypto_StartService(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst,
                                        NvM_Prv_Crypto_dataServiceStart_tst const* dataServiceStart_pcst);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE NvM_Prv_JobResult_ten NvM_Prv_Crypto_CompletePolling(NvM_Prv_stJob_ten* stJob_pen,
                                                                  NvM_Prv_JobData_tst const* JobData_pcst,
                                                                  NvM_Prv_Crypto_ServiceResult_tst const* JobResult_pcst);

#endif

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This private function executes state machines for cryptographic jobs during the NvM job if required.
 * \details
 * This function does nothing and always succeeds if usage of cryptographic services is disabled by NvM configuration.
 * The initial state for a cryptographic job shall be always NvM_Prv_stJob_DoCrypto_e.
 * A cryptographic job has completed if the corresponding state machine has reached the initial state
 * NvM_Prv_stJob_DoCrypto_e.
 *
 * \param [inout] stJob_pen
 * Current state in the state machine for a cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - see individual states for possible values
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
void NvM_Prv_Crypto_DoStateMachine(NvM_Prv_stJob_ten* stJob_pen,
                                   NvM_Prv_JobResult_tst* JobResult_pst,
                                   NvM_Prv_JobData_tst const* JobData_pcst)
{
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

    NvM_Prv_Job_StateMachine_tpfct StateMachine_pfct = NvM_Prv_Crypto_GetStateMachine(JobData_pcst->idJob_en);

    if (NULL_PTR != StateMachine_pfct)
    {
        do
        {
            NvM_Prv_Job_State_tpfct State_pfct = StateMachine_pfct(stJob_pen);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
            TestCd_NvM_JobMonitor_SetState(JobData_pcst, *stJob_pen);
#endif

            if (NULL_PTR != State_pfct)
            {
                State_pfct(stJob_pen, JobResult_pst, JobData_pcst);
            }
            else
            {
                JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            }

            if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
            {
                *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
            }

        } while((NvM_Prv_stJob_DoCrypto_e != *stJob_pen) &&
                (NvM_Prv_JobResult_Pending_e != JobResult_pst->Result_en));
    }
    else
    {
        JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
    }

#else
    // Set stJob_pen to already set value to avoid warnings that stJob_pen can be a pointer to const
    *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
    (void)JobData_pcst;
#endif
}

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

/**
 * \brief
 * This local private function returns the pointer to the cryptographic job state machine for the required NvM job.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * Cryptographic jobs are available only for read and write NvM jobs.
 *
 * \param[IN] idJob_en
 * NvM job ID
 *
 * \return
 * Pointer to the cryptographic job state machine
 */
static NvM_Prv_Job_StateMachine_tpfct NvM_Prv_Crypto_GetStateMachine(NvM_Prv_idJob_ten idJob_en)
{
    NvM_Prv_Job_StateMachine_tpfct GetStateMachine_pfct;

    switch (idJob_en)
    {
        case NvM_Prv_idJob_Read_e:
        case NvM_Prv_idJob_ReadIdConfigForReadAll_e:
            GetStateMachine_pfct = NvM_Prv_Crypto_GetJobStateRead;
        break;

        case NvM_Prv_idJob_Write_e:
        case NvM_Prv_idJob_WriteAll_e:
            GetStateMachine_pfct = NvM_Prv_Crypto_GetJobStateWrite;
        break;

        default:
            GetStateMachine_pfct = NULL_PTR;
        break;
    }

    return GetStateMachine_pfct;
}

/**
 * \brief
 * This local private function returns the pointer to the job step for the current state of the cryptographic write job.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 *
 * \param[IN] stJob_en
 * Current state of the cryptographic write job
 *
 * \return
 * Pointer to the job step
 */
static NvM_Prv_Job_State_tpfct NvM_Prv_Crypto_GetJobStateWrite(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobStateWrite_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_DoCrypto_e:
        case NvM_Prv_stJobWrite_CryptoStartGenerationRandom_e:
            JobStateWrite_pfct = NvM_Prv_Crypto_StartRandomGeneration;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobWrite_CryptoStartGenerationRandom_e;
        break;

        case NvM_Prv_stJobWrite_CryptoStartEncryption_e:
            JobStateWrite_pfct = NvM_Prv_Crypto_StartEncryption;
        break;

        case NvM_Prv_stJobWrite_CryptoStartGenerationSignature_e:
            JobStateWrite_pfct = NvM_Prv_Crypto_StartSignatureGeneration;
        break;

        case NvM_Prv_stJobWrite_CryptoPollGenerationRandom_e:
        case NvM_Prv_stJobWrite_CryptoPollEncryption_e:
        case NvM_Prv_stJobWrite_CryptoPollGenerationSignature_e:
            JobStateWrite_pfct = NvM_Prv_Crypto_PollJob;
        break;

        default:
            JobStateWrite_pfct = NULL_PTR;
        break;
    }

    return JobStateWrite_pfct;
}

/**
 * \brief
 * This local private function returns the pointer to the job step for the current state of the cryptographic read job.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 *
 * \param[IN] stJob_en
 * Current state of the cryptographic read job
 *
 * \return
 * Pointer to the job step
 */
static NvM_Prv_Job_State_tpfct NvM_Prv_Crypto_GetJobStateRead(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobStateRead_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_DoCrypto_e:
        case NvM_Prv_stJobRead_CryptoStartSignatureVerification_e:
            JobStateRead_pfct = NvM_Prv_Crypto_StartSignatureVerification;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobRead_CryptoStartSignatureVerification_e;
        break;

        case NvM_Prv_stJobRead_CryptoStartDecryption_e:
            JobStateRead_pfct = NvM_Prv_Crypto_StartDecryption;
        break;

        case NvM_Prv_stJobRead_CryptoPollSignatureVerification_e:
        case NvM_Prv_stJobRead_CryptoPollDecryption_e:
            JobStateRead_pfct = NvM_Prv_Crypto_PollJob;
        break;

        default:
            JobStateRead_pfct = NULL_PTR;
        break;
    }

    return JobStateRead_pfct;
}

/**
 * \brief
 * This local private function is a cryptographic job step function and polls results of cryptographic services
 * for cryptographic jobs.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * As soon as results of a cryptographic service are available (see NvM_Prv_Csm_GetJobResult)
 * this function checks the result of the cryptographic service.
 *
 * If cryptographic service has failed or verification result is not OK (only signature verification and
 * AEAD decryption have verification result) then this function sets production error NVM_E_RB_CRYPTO_SERVICE_FAILED
 * and finishs current cryptographic job with following results:
 * - decryption and signature verification -> request has found inconsistent block
 * - all other cryptographic services -> request has failed
 *
 * If cryptographic service has succeeded then this function evaluates the results (see NvM_Prv_Crypto_CheckJobResult)
 * and completes polling for valid results (see NvM_Prv_Crypto_CompletePolling).
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e            = cryptographic service is still pending
 * - NvM_Prv_JobResult_Succeeded_e          = cryptographic service has succeeded
 * - NvM_Prv_JobResult_BlockInconsistent_e  = signature verification or data decryption has failed
 *                                            -> read block contains inconsistent data
 * - NvM_Prv_JobResult_Failed_e             = CSM job executed for current cryptographic service has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_PollJob(NvM_Prv_stJob_ten* stJob_pen,
                                   NvM_Prv_JobResult_tst* JobResult_pst,
                                   NvM_Prv_JobData_tst const* JobData_pcst)
{
    if (NvM_Prv_Csm_GetJobResult(&JobResult_pst->Crypto_st))
    {
        if (   (E_OK != JobResult_pst->Crypto_st.Result_uo)
            || (CRYPTO_E_VER_OK != JobResult_pst->Crypto_st.ResultVerification_uo))
        {
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3243]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3244]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3246]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3273]
            // A cryptographic service has failed -> report production error
            JobResult_pst->ProductionError_u8 |= NVM_E_RB_CRYPTO_SERVICE_FAILED;
            JobResult_pst->Crypto_st.Result_uo = E_NOT_OK;

            // If signature verification or decryption has failed then NvM finishs read job with inconsistent block
            if (   (NvM_Prv_Crypto_idService_Decrypt_e == JobResult_pst->Crypto_st.idServiceCrypto_en)
                || (NvM_Prv_Crypto_idService_VerifySignature_e == JobResult_pst->Crypto_st.idServiceCrypto_en))
            {
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3235]
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3233]
                JobResult_pst->Result_en = NvM_Prv_JobResult_BlockInconsistent_e;
                *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
            }
            else
            {
                JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            }
        }
        else
        {
            if (!NvM_Prv_ErrorDetection_IsCryptoResultPlausible(JobData_pcst->idService_uo,
                                                                JobData_pcst->idBlock_uo,
                                                                &JobResult_pst->Crypto_st))
            {
                JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
                JobResult_pst->Crypto_st.Result_uo = E_NOT_OK;
            }
        }

        if (E_OK == JobResult_pst->Crypto_st.Result_uo)
        {
            // Cryptographic service has succeeded and all results are plausible
            JobResult_pst->Result_en = NvM_Prv_Crypto_CompletePolling(stJob_pen,
                                                                      JobData_pcst,
                                                                      &JobResult_pst->Crypto_st);
        }
    }
}

/**
 * \brief
 * This local private function is a cryptographic job step function and starts the cryptographic service
 * for the random number generation.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * This function only prepares all required data. The actual start is done by the function NvM_Prv_Crypto_StartService.
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of random number generation has succeeded
 * - NvM_Prv_JobResult_Succeeded_e = no random number generation configured -> start data encryption
 * - NvM_Prv_JobResult_Failed_e    = start of random number generation has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_StartRandomGeneration(NvM_Prv_stJob_ten* stJob_pen,
                                                 NvM_Prv_JobResult_tst* JobResult_pst,
                                                 NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_Crypto_dataServiceStart_tst dataJobStart_st;
    NvM_Prv_Crypto_ServiceData_tst jobCryptoData_st;
    uint16 LengthRandom_u16 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                              NvM_Prv_Crypto_idService_GenerateRandom_e);

    uint16 LengthEncryptedData_u16 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                     NvM_Prv_Crypto_idService_Encrypt_e);
    boolean isRamCrcUsed_b = NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC);
    // if data encryption and RAM CRC is configured
    // then NvM shall append recalculated CRC to the user data before adding random nr and starting encryption
    if ((0u != LengthEncryptedData_u16) && isRamCrcUsed_b)
    {
        // Append the Crc to the user data
        NvM_Prv_Crc_AppendCrc(JobData_pcst->idBlock_uo,
                              &JobData_pcst->IntBuffer_st.Buffer_pu8[JobData_pcst->IntBuffer_st.OffsetPlainData_u16],
                              JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16,
                              &JobResult_pst->CrcData_st.CrcRamBlk_un);
    }

    // Prefill job results structure for random number generation
    JobResult_pst->Crypto_st.idServiceCrypto_en = NvM_Prv_Crypto_idService_GenerateRandom_e;
    JobResult_pst->Crypto_st.idJobStarted_uo = NvM_Prv_BlkDesc_GetIdJobCsm(JobData_pcst->idBlock_uo,
                                                                           NvM_Prv_Crypto_idService_GenerateRandom_e);
    // member idJobCallback_uo will be set in the callback if Csm fom has completed
    JobResult_pst->Crypto_st.outputPrimaryLength_u32 = LengthRandom_u16;
    JobResult_pst->Crypto_st.outputPrimaryLengthConfigured_u32 = JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    JobResult_pst->Crypto_st.outputSecondaryLength_u32 = 0u;              // not used for random number generation
    JobResult_pst->Crypto_st.outputSecondaryLengthConfigured_u32 = 0u;    // not used for random number generation
    // member Result_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.ResultVerification_uo = CRYPTO_E_VER_OK;     // not used for random number generation

    // Prefill data structure for a cryptographic job
    jobCryptoData_st.outputPrimary_pu8 =
            &(JobData_pcst->IntBuffer_st.Buffer_pu8[*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 +
                                                    JobData_pcst->IntBuffer_st.OffsetPlainData_u16]);
    jobCryptoData_st.outputPrimaryLength_pu32 = &JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    // all other members of jobCryptoData_st are not required to start random number generation

    dataJobStart_st.stJobNextSuccess_en = NvM_Prv_stJobWrite_CryptoPollGenerationRandom_e;
    dataJobStart_st.stJobNextSkipped_en = NvM_Prv_stJobWrite_CryptoStartEncryption_e;
    dataJobStart_st.isCryptoServiceConfigured_b = (0u != LengthRandom_u16);
    dataJobStart_st.idJobToStart_uo = JobResult_pst->Crypto_st.idJobStarted_uo;
    dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartRandomGeneration;

    NvM_Prv_Crypto_StartService(stJob_pen, JobResult_pst, &jobCryptoData_st, &dataJobStart_st);
}

/**
 * \brief
 * This local private function is a cryptographic job step function and starts the cryptographic service
 * for the data encryption.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * This function only prepares all required data. The actual start is done by the function NvM_Prv_Crypto_StartService.
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of data encryption has succeeded
 * - NvM_Prv_JobResult_Succeeded_e = no data encryption configured -> start signature generation
 * - NvM_Prv_JobResult_Failed_e    = start of data encryption has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_StartEncryption(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_Crypto_dataServiceStart_tst dataJobStart_st;
    NvM_Prv_Crypto_ServiceData_tst jobCryptoData_st;
    uint16 LengthEncryptedData_u16 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                     NvM_Prv_Crypto_idService_Encrypt_e);

    // Prefill job results structure for data encryption
    JobResult_pst->Crypto_st.idServiceCrypto_en = NvM_Prv_Crypto_idService_Encrypt_e;
    JobResult_pst->Crypto_st.idJobStarted_uo = NvM_Prv_BlkDesc_GetIdJobCsm(JobData_pcst->idBlock_uo,
                                                                           NvM_Prv_Crypto_idService_Encrypt_e);
    // member idJobCallback_uo will be set in the callback if Csm fom has completed
    JobResult_pst->Crypto_st.outputPrimaryLength_u32 = LengthEncryptedData_u16;
    JobResult_pst->Crypto_st.outputPrimaryLengthConfigured_u32 = LengthEncryptedData_u16;
    JobResult_pst->Crypto_st.outputSecondaryLength_u32 = NvM_Prv_BlkDesc_GetLengthJobCsmTag(JobData_pcst->idBlock_uo);
    JobResult_pst->Crypto_st.outputSecondaryLengthConfigured_u32 = JobResult_pst->Crypto_st.outputSecondaryLength_u32;
    // member Result_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.ResultVerification_uo = CRYPTO_E_VER_OK;     // not used for data encryption

    // Prefill data structure for a cryptographic job
    jobCryptoData_st.inputPrimary_pcu8 = &JobData_pcst->IntBuffer_st.Buffer_pu8[JobData_pcst->IntBuffer_st.OffsetPlainData_u16];
    jobCryptoData_st.inputPrimaryLength_u32 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16;
    jobCryptoData_st.inputSecondary_pcu8 = NvM_Prv_BlkDesc_GetDataJobCsmAssociated(JobData_pcst->idBlock_uo);
    jobCryptoData_st.inputSecondaryLength_u32 = NvM_Prv_BlkDesc_GetLengthJobCsmAssociated(JobData_pcst->idBlock_uo);
    jobCryptoData_st.outputPrimary_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    jobCryptoData_st.outputPrimaryLength_pu32 = &JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    jobCryptoData_st.outputSecondary_pu8 =
            &(jobCryptoData_st.outputPrimary_pu8[JobResult_pst->Crypto_st.outputPrimaryLength_u32]);
    jobCryptoData_st.outputSecondaryLength_pu32 = &JobResult_pst->Crypto_st.outputSecondaryLength_u32;
    // all other members of jobCryptoData_st are not required to start data encryption

    dataJobStart_st.stJobNextSuccess_en = NvM_Prv_stJobWrite_CryptoPollEncryption_e;
    dataJobStart_st.stJobNextSkipped_en = NvM_Prv_stJobWrite_CryptoStartGenerationSignature_e;
    dataJobStart_st.isCryptoServiceConfigured_b = (0u != LengthEncryptedData_u16);
    dataJobStart_st.idJobToStart_uo = JobResult_pst->Crypto_st.idJobStarted_uo;

    if (0u == jobCryptoData_st.inputSecondaryLength_u32)
    {
        // no associated data configured -> use standard data encryption
        dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartEncryption;
    }
    else
    {
        // associated data configured -> use AEAD data encryption
        dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartEncryptionAead;
    }

    NvM_Prv_Crypto_StartService(stJob_pen, JobResult_pst, &jobCryptoData_st, &dataJobStart_st);
}

/**
 * \brief
 * This local private function is a cryptographic job step function and starts the cryptographic service
 * for the data decryption.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * This function only prepares all required data. The actual start is done by the function NvM_Prv_Crypto_StartService.
 *
 * Internal buffer contains only encrypted data (dependent on configuration with a tag (AEAD algorithm) or
 * without a tag (standard algorithm))
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of data decryption has succeeded
 * - NvM_Prv_JobResult_Succeeded_e = no data encryption configured -> finish cryptographic job
 * - NvM_Prv_JobResult_Failed_e    = start of data decryption has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_StartDecryption(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_Crypto_dataServiceStart_tst dataJobStart_st;
    NvM_Prv_Crypto_ServiceData_tst jobCryptoData_st;
    uint16 LengthEncryptedData_u16 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                     NvM_Prv_Crypto_idService_Decrypt_e);
    /* BSW-11963 */ /* [$DD_BSWCODE 1127] */
    // Length of Decrypted data should be Length of Raw Data + length of Crc + Length of Random Number 
    uint16 LengthDecryptedData_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo) +
                                     NvM_Prv_Crc_GetSizeCrc(NvM_Prv_GetCrcType(JobData_pcst->idBlock_uo))+
                                     NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                     NvM_Prv_Crypto_idService_GenerateRandom_e);
    /* END BSW-11963 */
    uint16 LengthTag_u16 = NvM_Prv_BlkDesc_GetLengthJobCsmTag(JobData_pcst->idBlock_uo);
    uint16 LengthAssociated_u16 = NvM_Prv_BlkDesc_GetLengthJobCsmAssociated(JobData_pcst->idBlock_uo);
    uint8 const* PtrAssociated_pcu8 = NvM_Prv_BlkDesc_GetDataJobCsmAssociated(JobData_pcst->idBlock_uo);

    // Prefill job results structure for data decryption
    JobResult_pst->Crypto_st.idServiceCrypto_en = NvM_Prv_Crypto_idService_Decrypt_e;
    JobResult_pst->Crypto_st.idJobStarted_uo = NvM_Prv_BlkDesc_GetIdJobCsm(JobData_pcst->idBlock_uo,
                                                                           NvM_Prv_Crypto_idService_Decrypt_e);
    // member idJobCallback_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.outputPrimaryLength_u32 = LengthEncryptedData_u16;
    JobResult_pst->Crypto_st.outputPrimaryLengthConfigured_u32 = LengthDecryptedData_u16;
    JobResult_pst->Crypto_st.outputSecondaryLength_u32 = 0u;            // not used for data decryption
    JobResult_pst->Crypto_st.outputSecondaryLengthConfigured_u32 = 0u;  // not used for data decryption
    // member Result_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.ResultVerification_uo = 0xFFu;

    // Prefill data structure for a cryptographic job
    jobCryptoData_st.inputPrimary_pcu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    jobCryptoData_st.inputPrimaryLength_u32 = (*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 - LengthTag_u16);
    jobCryptoData_st.inputSecondary_pcu8 = PtrAssociated_pcu8;
    jobCryptoData_st.inputSecondaryLength_u32 = LengthAssociated_u16;
    jobCryptoData_st.inputTertiary_pcu8 =
            &((JobData_pcst->IntBuffer_st.Buffer_pu8)[*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 -
                                                      LengthTag_u16]);
    jobCryptoData_st.inputTertiaryLength_u32 = LengthTag_u16;
    jobCryptoData_st.outputPrimary_pu8 =
            &JobData_pcst->IntBuffer_st.Buffer_pu8[JobData_pcst->IntBuffer_st.OffsetPlainData_u16];
    jobCryptoData_st.outputPrimaryLength_pu32 = &JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    jobCryptoData_st.ResultVerification_puo = &JobResult_pst->Crypto_st.ResultVerification_uo;
    // all other members of jobCryptoData_st are not required to start data decryption

    dataJobStart_st.stJobNextSuccess_en = NvM_Prv_stJobRead_CryptoPollDecryption_e;
    dataJobStart_st.stJobNextSkipped_en = NvM_Prv_stJob_DoCrypto_e;
    dataJobStart_st.isCryptoServiceConfigured_b = (0u != LengthEncryptedData_u16);
    dataJobStart_st.idJobToStart_uo = JobResult_pst->Crypto_st.idJobStarted_uo;

    // if no decryption is configured then job buffer contains user data and random nr
    // update number of used size so that random nr is not considered in following steps
    if (!dataJobStart_st.isCryptoServiceConfigured_b)
    {
        *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
    }

    if (0u == jobCryptoData_st.inputSecondaryLength_u32)
    {
        // no associated data configured -> use standard data decryption
        dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartDecryption;
        // For standard encryption no authentication will be done -> preset verification result
        JobResult_pst->Crypto_st.ResultVerification_uo = CRYPTO_E_VER_OK;
    }
    else
    {
        // associated data configured -> use AEAD data decryption
        dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartDecryptionAead;
    }

    NvM_Prv_Crypto_StartService(stJob_pen, JobResult_pst, &jobCryptoData_st, &dataJobStart_st);
}

/**
 * This local private function is a cryptographic job step function and starts the cryptographic service
 * for the signature generation.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * This function only prepares all required data. The actual start is done by the function NvM_Prv_Crypto_StartService.
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of signature generation has succeeded
 * - NvM_Prv_JobResult_Succeeded_e = no signature configured -> finish cryptographic job
 * - NvM_Prv_JobResult_Failed_e    = start of signature generation has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_StartSignatureGeneration(NvM_Prv_stJob_ten* stJob_pen,
                                                    NvM_Prv_JobResult_tst* JobResult_pst,
                                                    NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_Crypto_dataServiceStart_tst dataJobStart_st;
    NvM_Prv_Crypto_ServiceData_tst jobCryptoData_st;
    uint32 lengthSignature_u32 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                 NvM_Prv_Crypto_idService_GenerateSignature_e);


    // Prefill job results structure for signature generation
    JobResult_pst->Crypto_st.idServiceCrypto_en = NvM_Prv_Crypto_idService_GenerateSignature_e;
    JobResult_pst->Crypto_st.idJobStarted_uo = NvM_Prv_BlkDesc_GetIdJobCsm(JobData_pcst->idBlock_uo,
                                                                           NvM_Prv_Crypto_idService_GenerateSignature_e);
    // member idJobCallback_uo will be set in the callback if Csm fom has completed
    JobResult_pst->Crypto_st.outputPrimaryLength_u32 = lengthSignature_u32;
    JobResult_pst->Crypto_st.outputPrimaryLengthConfigured_u32 = JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    JobResult_pst->Crypto_st.outputSecondaryLength_u32 = 0u;              // not used for signature generation
    JobResult_pst->Crypto_st.outputSecondaryLengthConfigured_u32 = 0u;    // not used for signature generation
    // member Result_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.ResultVerification_uo = CRYPTO_E_VER_OK;     // not used for signature generation

    if (0u != JobResult_pst->Crypto_st.outputPrimaryLength_u32)
    {
        uint16 idPersistant_u16 = NvM_Prv_BlkDesc_GetPersistantId(JobData_pcst->idBlock_uo);

        // append persistent ID to the buffer with user data before signature generation
        rba_MemLib_MemCopy((uint8 const*)(&idPersistant_u16),
                           &(JobData_pcst->IntBuffer_st.Buffer_pu8[*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16]),
                           sizeof(idPersistant_u16));

        *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 += (uint16)sizeof(idPersistant_u16);
    }

    // Prefill data structure for a cryptographic job
    jobCryptoData_st.inputPrimary_pcu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    jobCryptoData_st.inputPrimaryLength_u32 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16;
    jobCryptoData_st.outputPrimary_pu8 = &(JobData_pcst->IntBuffer_st.Buffer_pu8[jobCryptoData_st.inputPrimaryLength_u32]);
    jobCryptoData_st.outputPrimaryLength_pu32 = &JobResult_pst->Crypto_st.outputPrimaryLength_u32;
    // all other members of jobCsmData_st are not required to start signature generation

    dataJobStart_st.stJobNextSuccess_en = NvM_Prv_stJobWrite_CryptoPollGenerationSignature_e;
    dataJobStart_st.stJobNextSkipped_en = NvM_Prv_stJob_DoCrypto_e;
    dataJobStart_st.isCryptoServiceConfigured_b = (0u != lengthSignature_u32);
    dataJobStart_st.idJobToStart_uo = JobResult_pst->Crypto_st.idJobStarted_uo;
    dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartSignatureGeneration;

    NvM_Prv_Crypto_StartService(stJob_pen, JobResult_pst, &jobCryptoData_st, &dataJobStart_st);
}

/**
 * This local private function is a cryptographic job step function and starts the cryptographic service
 * for the signature verification.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * This function only prepares all required data. The actual start is done by the function NvM_Prv_Crypto_StartService.
 *
 * Internal buffer contains data read from non-volatile memory which consists of following parts ordered as defined:
 * - user data (dependent on configuration encrypted or plain data)
 * - persistent ID
 * - actual signature
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of signature verification has succeeded
 * - NvM_Prv_JobResult_Succeeded_e = no signature configured -> start data decryption
 * - NvM_Prv_JobResult_Failed_e    = start of signature verification has failed
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_Crypto_StartSignatureVerification(NvM_Prv_stJob_ten* stJob_pen,
                                                      NvM_Prv_JobResult_tst* JobResult_pst,
                                                      NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_Crypto_dataServiceStart_tst dataJobStart_st;
    NvM_Prv_Crypto_ServiceData_tst jobCryptoData_st;
    uint32 lengthSignature_u32 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                 NvM_Prv_Crypto_idService_VerifySignature_e);

    // Prefill job results structure for signature verification
    JobResult_pst->Crypto_st.idServiceCrypto_en = NvM_Prv_Crypto_idService_VerifySignature_e;
    JobResult_pst->Crypto_st.idJobStarted_uo = NvM_Prv_BlkDesc_GetIdJobCsm(JobData_pcst->idBlock_uo,
                                                                           NvM_Prv_Crypto_idService_VerifySignature_e);
    // member idJobCallback_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.outputPrimaryLength_u32 = 0u;
    JobResult_pst->Crypto_st.outputPrimaryLengthConfigured_u32 = 0u;
    JobResult_pst->Crypto_st.outputSecondaryLength_u32 = 0u;            // not used for signature verification
    JobResult_pst->Crypto_st.outputSecondaryLengthConfigured_u32 = 0u;  // not used for signature verification
    // member Result_uo will be set in the callback if Csm job has completed
    JobResult_pst->Crypto_st.ResultVerification_uo = 0xFFu;

    // Prefill data structure for a cryptographic job
    jobCryptoData_st.inputPrimary_pcu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    jobCryptoData_st.inputPrimaryLength_u32 = (*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 - lengthSignature_u32);
    jobCryptoData_st.inputSecondary_pcu8 = &(JobData_pcst->IntBuffer_st.Buffer_pu8[jobCryptoData_st.inputPrimaryLength_u32]);
    jobCryptoData_st.inputSecondaryLength_u32 = lengthSignature_u32;
    jobCryptoData_st.ResultVerification_puo = &JobResult_pst->Crypto_st.ResultVerification_uo;
    // all other members of jobCsmData_st are not required to start signature generation

    dataJobStart_st.stJobNextSuccess_en = NvM_Prv_stJobRead_CryptoPollSignatureVerification_e;
    dataJobStart_st.stJobNextSkipped_en = NvM_Prv_stJobRead_CryptoStartDecryption_e;
    dataJobStart_st.isCryptoServiceConfigured_b = (0u != lengthSignature_u32);
    dataJobStart_st.idJobToStart_uo = JobResult_pst->Crypto_st.idJobStarted_uo;
    dataJobStart_st.StartJobCsm_pfct = NvM_Prv_Csm_StartSignatureVerification;

    NvM_Prv_Crypto_StartService(stJob_pen, JobResult_pst, &jobCryptoData_st, &dataJobStart_st);
}

/**
 * This local private function starts a cryptographic service for a cryptographic job step function.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 *
 * If cryptographic service start has failed then this function sets production error NVM_E_RB_CRYPTO_SERVICE_FAILED
 * and finishs current cryptographic job.
 *
 * \param [inout] stJob_pen
 * State of the cryptographic job
 * \param [inout] JobResult_pst
 * Pointer to the result of the cryptographic job, possible values:
 * - NvM_Prv_JobResult_Pending_e   = start of cryptographic service has succeeded -> go to polling state
 * - NvM_Prv_JobResult_Succeeded_e = requested cryptographic service not configured-> go to next state
 * - NvM_Prv_JobResult_Failed_e    = start of cryptographic service has failed
 * \param [in] ServiceData_pcst
 * Pointer to the data for cryptographic service to be started
 * \param [in] dataServiceStart_pcst
 * Pointer to the data required to start a cryptographic service
 */
static void NvM_Prv_Crypto_StartService(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst,
                                        NvM_Prv_Crypto_dataServiceStart_tst const* dataServiceStart_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (dataServiceStart_pcst->isCryptoServiceConfigured_b)
    {
        JobResult_pst->Result_en = dataServiceStart_pcst->StartJobCsm_pfct(dataServiceStart_pcst->idJobToStart_uo,
                                                                           ServiceData_pcst);
        if (NvM_Prv_JobResult_Pending_e == JobResult_pst->Result_en)
        {

            *stJob_pen = dataServiceStart_pcst->stJobNextSuccess_en;
        }
        else
        {
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3243]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3244]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3246]
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3273]
            // A cryptographic service has failed -> report production error
            JobResult_pst->ProductionError_u8 |= NVM_E_RB_CRYPTO_SERVICE_FAILED;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        }
    }
    else
    {
        *stJob_pen = dataServiceStart_pcst->stJobNextSkipped_en;
    }
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

/**
 * \brief
 * This local private function is a cryptographic job step function and completes polling of a cryptographic service.
 * \details
 * This function is only available if usage of cryptographic services is enabled by NvM configuration.
 * The caller of this function has to ensure that a cryptographic service has succeeded and its results are valid.
 * Dependent on the current cryptographic service this function does following actions:
 * - update the job buffer if required
 * - update the used size of the job buffer
 * - determine the next state
 *
 * This function does folowing actions for individual cryptographic services:
 * - Random number generation:
 *   - used size of the job buffer = used size + length of the generated random number
 *   - start data encryption
 * - Data encryption:
 *   - set buffer with encrypted data as job buffer
 *   - used size of the job buffer = length of the encrypted data + length of the authentication tag
 *     (if standard encryption is configured then authentication tag length is 0)
 *   - start signature generation
 * - Signature generation:
 *   - used size of the job buffer = used size + size of persistent ID + length of the generated signature
 *   - finish cryptographic job
 * - Signature verification:
 *   - used size of the job buffer = used size - size of persistent ID - length of the signature
 *   - start data decryption
 * - Data decryption:
 *   - set NvM internal buffer with decrypted data as job buffer
 *   - used size of the job buffer = length of the decrypted data - length of the random number
 *     (if no usage of random number is configured then random number length is 0)
 *   - finish cryptographic job
 * - Invalid cryptographic service (shall never happen, is done for robustness):
 *   - ensure that cryptographic job fails -> NvM job which has started cryptographic job will fail too
 *
 * \param[INOUT] stJob_pen
 * State of the cryptographic job
 * \param[INOUT] JobData_pcst
 * Pointer to the data of the current NvM job
 * \param[INOUT] JobResult_pst
 * Pointer to the result of the cryptographic job
 *
 * \return
 * Cryptographic job step result, possible values:
 * - NvM_Prv_JobResult_Succeeded_e = cryptographic service polling completed
 * - NvM_Prv_JobResult_Failed_e    = invalid cryptographic service detected
 */
LOCAL_INLINE NvM_Prv_JobResult_ten NvM_Prv_Crypto_CompletePolling(NvM_Prv_stJob_ten* stJob_pen,
                                                                  NvM_Prv_JobData_tst const* JobData_pcst,
                                                                  NvM_Prv_Crypto_ServiceResult_tst const* JobResult_pcst)
{
    NvM_Prv_JobResult_ten ResultJobCrypto_en = NvM_Prv_JobResult_Succeeded_e;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJobWrite_CryptoPollGenerationRandom_e:
        {
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = (uint16)(*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 +
                                                                        JobResult_pcst->outputPrimaryLength_u32);
            *stJob_pen = NvM_Prv_stJobWrite_CryptoStartEncryption_e;
        }
        break;

        case NvM_Prv_stJobWrite_CryptoPollEncryption_e:
        {
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = (uint16)(JobResult_pcst->outputPrimaryLength_u32 +
                                                                        JobResult_pcst->outputSecondaryLength_u32);
            *stJob_pen = NvM_Prv_stJobWrite_CryptoStartGenerationSignature_e;
        }
        break;

        case NvM_Prv_stJobRead_CryptoPollDecryption_e:
        {
            /* BSW-11963 */ /* [$DD_BSWCODE 1128] */
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = (uint16)(JobResult_pcst->outputPrimaryLength_u32) -
                                                                        NvM_Prv_Crc_GetSizeCrc(NvM_Prv_GetCrcType(JobData_pcst->idBlock_uo)) -
                                                                        NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                         NvM_Prv_Crypto_idService_GenerateRandom_e);
            /* END BSW-11963 */
            *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
        }
        break;

        case NvM_Prv_stJobWrite_CryptoPollGenerationSignature_e:
        {
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = (uint16)(*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 +
                                                                        JobResult_pcst->outputPrimaryLength_u32);
            *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
        }
        break;

        case NvM_Prv_stJobRead_CryptoPollSignatureVerification_e:
        {
            uint16 LengthSignature_u16 = NvM_Prv_BlkDesc_GetLengthJobCsm(JobData_pcst->idBlock_uo,
                                                                         NvM_Prv_Crypto_idService_VerifySignature_e);
            uint16 PositionPersistantID_u16 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 -
                                              LengthSignature_u16 -
                                              (uint16)sizeof(uint16);
            uint16 ReadPersistantId_u16;
            rba_MemLib_MemCopy(&(JobData_pcst->IntBuffer_st.Buffer_pu8)[PositionPersistantID_u16],
                               (uint8*)&ReadPersistantId_u16,
                               sizeof(uint16));
            if (NvM_Prv_ErrorDetection_IsReadPersistantIdConsistent(JobData_pcst->idService_uo,
                                                                    JobData_pcst->idBlock_uo,
                                                                    ReadPersistantId_u16))
            {
                *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = (uint16)(*JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 -
                                                                            LengthSignature_u16 -
                                                                            sizeof(uint16));
                *stJob_pen = NvM_Prv_stJobRead_CryptoStartDecryption_e;
            }
            else
            {
                ResultJobCrypto_en = NvM_Prv_JobResult_Failed_e;
            }
        }
        break;

        default:
        {
            ResultJobCrypto_en = NvM_Prv_JobResult_Failed_e;
        }
        break;
    }

    return ResultJobCrypto_en;
}

#endif

