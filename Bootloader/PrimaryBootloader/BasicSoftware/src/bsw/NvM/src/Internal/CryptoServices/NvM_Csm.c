
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Cbk.h"

#include "NvM_Prv_Csm.h"
#include "NvM_Prv_Csm_Types.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static NvM_Prv_JobResult_ten NvM_Prv_Csm_GetJobStatus(Std_ReturnType ResultJobCsm_uo);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/**
 * \brief
 * Csm job result.
 * \details
 * This variable allows asynchronous evaluation of the Csm job result stored during the corresponding callback,
 * see NvM_Csm_Callback and NvM_Prv_Csm_SetJobResult.
 * Read access to this variable shall be done only with enabled scheduler lock, see NvM_Prv_Csm_GetJobResult
 */
static NvM_Prv_Csm_JobResult_tst NvM_Prv_Csm_JobResult_st;

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This public function defines the callback for Csm jobs and sets corresponding results.
 * \details
 * This function stores the Csm job result and the Csm job ID for later evaluation in a static variable
 * (s. NvM_Prv_Csm_JobResult_st).
 * Typically a Csm job invokes a callback function if it finishes and this function enables the Csm job callback
 * to set the result and execute all required actions.
 * To ensure the results consistency this function writes Csm job results with active scheduler lock.
 *
 * \param[IN] jobId
 * Csm job ID for which the Csm has invoked this callback function.
 * \param[IN] result
 * Csm job result, possible values depend on the Csm job type.
 */
void NvM_Csm_Callback(uint32 jobId, Std_ReturnType result)
{
    SchM_Enter_NvM_Main();

    NvM_Prv_Csm_JobResult_st.idJobCallback_uo = jobId;
    NvM_Prv_Csm_JobResult_st.Result_uo = result;
    NvM_Prv_Csm_JobResult_st.isResultValid_b = TRUE;

    SchM_Exit_NvM_Main();
}

/**
 * \brief
 * This private function provides access to the result of the last Csm job for its evaluation.
 * \details
 * This function copies the result of the last Csm job into the result structure for cryptographic services
 * provided by the caller. The copying is always done with active scheduler lock.
 * This function copies the result only if it is valid, i.e. the Csm has set the result via corresponding callback.
 * Afterwards this functions invalidates the result.
 *
 * \param[OUT] JobResult_pst
 * Pointer to the result structure for cryptographic services.
 *
 * \return
 * Information whether result of the last Csm job has been copied into the passed result structure
 * - true = valid result is available and has been copied
 * - false = no valid result is available
 */
boolean NvM_Prv_Csm_GetJobResult(NvM_Prv_Crypto_ServiceResult_tst* JobResult_pst)
{
    boolean isResultJobCsmAvailable_b = FALSE;

    SchM_Enter_NvM_Main();

    if (NvM_Prv_Csm_JobResult_st.isResultValid_b)
    {
        // Csm has invoked the configured callback
        isResultJobCsmAvailable_b = TRUE;

        JobResult_pst->idJobCallback_uo = NvM_Prv_Csm_JobResult_st.idJobCallback_uo;
        JobResult_pst->Result_uo = NvM_Prv_Csm_JobResult_st.Result_uo;
        NvM_Prv_Csm_JobResult_st.isResultValid_b = FALSE;

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
        if (TestCd_NvM_st.Arguments.CsmJobFail_b)
        {
            JobResult_pst->Result_uo = E_NOT_OK;
        }
        if (!TestCd_NvM_st.Arguments.CsmJobDecryptWrongLength_b)
        {
            JobResult_pst->outputPrimaryLength_u32 = JobResult_pst->outputPrimaryLengthConfigured_u32;
        }
        if (TestCd_NvM_st.Arguments.CsmJobVerificationFail_b)
        {
            JobResult_pst->ResultVerification_uo = CRYPTO_E_VER_NOT_OK;
        }
#endif
    }

    SchM_Exit_NvM_Main();

    return (isResultJobCsmAvailable_b);
}

/**
 * \brief
 * This private function starts random number generation on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartRandomGeneration(uint32 idJobCsm_uo,
                                                        NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1129] */
#if (NVM_ADD_RANDOM_USED  == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_RandomGenerate(idJobCsm_uo,
                                                       ServiceData_pcst->outputPrimary_pu8,
                                                       ServiceData_pcst->outputPrimaryLength_pu32));
#else
    (void) idJobCsm_uo;
    (void) ServiceData_pcst;
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts data encryption on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartEncryption(uint32 idJobCsm_uo,
                                                  NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1130] */
#if (NVM_STANDARD_DATA_ENCRYTION_USED  == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_Encrypt(idJobCsm_uo,
                                                CRYPTO_OPERATIONMODE_SINGLECALL,
                                                ServiceData_pcst->inputPrimary_pcu8,
                                                ServiceData_pcst->inputPrimaryLength_u32,
                                                ServiceData_pcst->outputPrimary_pu8,
                                                ServiceData_pcst->outputPrimaryLength_pu32));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts data decryption on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartDecryption(uint32 idJobCsm_uo,
                                                  NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1131] */
#if (NVM_STANDARD_DATA_ENCRYTION_USED  == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_Decrypt(idJobCsm_uo,
                                                CRYPTO_OPERATIONMODE_SINGLECALL,
                                                ServiceData_pcst->inputPrimary_pcu8,
                                                ServiceData_pcst->inputPrimaryLength_u32,
                                                ServiceData_pcst->outputPrimary_pu8,
                                                ServiceData_pcst->outputPrimaryLength_pu32));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts AEAD data encryption on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartEncryptionAead(uint32 idJobCsm_uo,
                                                      NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1132] */
#if (NVM_AEAD_DATA_ENCRYPTION_USED  == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_AEADEncrypt(idJobCsm_uo,
                                                    CRYPTO_OPERATIONMODE_SINGLECALL,
                                                    ServiceData_pcst->inputPrimary_pcu8,
                                                    ServiceData_pcst->inputPrimaryLength_u32,
                                                    ServiceData_pcst->inputSecondary_pcu8,
                                                    ServiceData_pcst->inputSecondaryLength_u32,
                                                    ServiceData_pcst->outputPrimary_pu8,
                                                    ServiceData_pcst->outputPrimaryLength_pu32,
                                                    ServiceData_pcst->outputSecondary_pu8,
                                                    ServiceData_pcst->outputSecondaryLength_pu32));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts AEAD data decryption on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartDecryptionAead(uint32 idJobCsm_uo,
                                                      NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1133] */
#if (NVM_AEAD_DATA_ENCRYPTION_USED == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_AEADDecrypt(idJobCsm_uo,
                                                    CRYPTO_OPERATIONMODE_SINGLECALL,
                                                    ServiceData_pcst->inputPrimary_pcu8,
                                                    ServiceData_pcst->inputPrimaryLength_u32,
                                                    ServiceData_pcst->inputSecondary_pcu8,
                                                    ServiceData_pcst->inputSecondaryLength_u32,
                                                    ServiceData_pcst->inputTertiary_pcu8,
                                                    ServiceData_pcst->inputTertiaryLength_u32,
                                                    ServiceData_pcst->outputPrimary_pu8,
                                                    ServiceData_pcst->outputPrimaryLength_pu32,
                                                    ServiceData_pcst->ResultVerification_puo));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts signature generation on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartSignatureGeneration(uint32 idJobCsm_uo,
                                                           NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1134] */
#if (NVM_ADD_SIGNATURE_USED == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_SignatureGenerate(idJobCsm_uo,
                                                          CRYPTO_OPERATIONMODE_SINGLECALL,
                                                          ServiceData_pcst->inputPrimary_pcu8,
                                                          ServiceData_pcst->inputPrimaryLength_u32,
                                                          ServiceData_pcst->outputPrimary_pu8,
                                                          ServiceData_pcst->outputPrimaryLength_pu32));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This private function starts signature verification on the Csm.
 *
 * \param[IN] idJobCsm_uo
 * ID of the Csm job to be started.
 * \param[INOUT] ServiceData_pcst
 * Pointer to the data structure for cryptographic services.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
NvM_Prv_JobResult_ten NvM_Prv_Csm_StartSignatureVerification(uint32 idJobCsm_uo,
                                                             NvM_Prv_Crypto_ServiceData_tst const* ServiceData_pcst)
{
#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CsmJobStartFail_b)
    {
        return NvM_Prv_JobResult_Failed_e;
    }
#endif
/* BSW-11964 */ /* [$DD_BSWCODE 1135] */
#if (NVM_ADD_SIGNATURE_USED == STD_ON)
    return NvM_Prv_Csm_GetJobStatus(Csm_SignatureVerify(idJobCsm_uo,
                                                        CRYPTO_OPERATIONMODE_SINGLECALL,
                                                        ServiceData_pcst->inputPrimary_pcu8,
                                                        ServiceData_pcst->inputPrimaryLength_u32,
                                                        ServiceData_pcst->inputSecondary_pcu8,
                                                        ServiceData_pcst->inputSecondaryLength_u32,
                                                        ServiceData_pcst->ResultVerification_puo));
#else
    (void) (idJobCsm_uo);
    (void) (ServiceData_pcst);
    return NvM_Prv_JobResult_Failed_e;
#endif
/* END BSW-11964 */
}

/**
 * \brief
 * This local private function evaluates the Csm job status.
 *
 * \param[IN] ResultJobCsm_uo
 * Result of the start attempt of a Csm job.
 *
 * \return
 * Csm job status:
 * - NvM_Prv_JobResult_Pending_e = Csm job start succeeded
 * - NvM_Prv_JobResult_Failed_e = Csm job start failed
 */
static NvM_Prv_JobResult_ten NvM_Prv_Csm_GetJobStatus(Std_ReturnType ResultJobCsm_uo)
{
    NvM_Prv_JobResult_ten stJobCsm_en;

    if (E_OK == ResultJobCsm_uo)
    {
        stJobCsm_en = NvM_Prv_JobResult_Pending_e;
    }
    else
    {
        stJobCsm_en = NvM_Prv_JobResult_Failed_e;
    }

    return stJobCsm_en;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif

