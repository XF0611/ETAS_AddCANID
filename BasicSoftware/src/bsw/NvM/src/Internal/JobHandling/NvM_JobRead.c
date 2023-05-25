
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv_Job.h"
#include "NvM_Prv_Crc.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_JobResource.h"
#include "NvM_Prv_InternalBuffer.h"
#include "NvM_Prv_ExplicitSynchronization.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static void NvM_Prv_JobRead_Start(NvM_Prv_stJob_ten* stJob_pen,
                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                  NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_GetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_CrcCheck(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
                                    NvM_Prv_JobResult_tst* JobResult_pst,
                                    NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_RecalcUserDataCrc(NvM_Prv_stJob_ten* stJob_pen,
                                              NvM_Prv_JobResult_tst* JobResult_pst,
                                              NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_CalcNvBlkCrc(NvM_Prv_stJob_ten* stJob_pen,
                                         NvM_Prv_JobResult_tst* JobResult_pst,
                                         NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_ExtractAdminData(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_DoCrypto(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_SetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRead_StartImplicitRecovery(NvM_Prv_stJob_ten* stJob_pen,
                                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                                  NvM_Prv_JobData_tst const* JobData_pcst);

#define NVM_STOP_SEC_CODE
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
 * This private function returns the pointer to the job step for the current state of the read job.
 * \details
 * This function returns a NULL pointer if passed read job state is invalid. In this case the read job will fail.
 *
 * \param[INOUT] stJob_pen
 * Pointer to the current state of the read job
 *
 * \return
 * Pointer to the job step function or NULL pointer for invalid read job state
 */
NvM_Prv_Job_State_tpfct NvM_Prv_JobRead_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobRead_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
        case NvM_Prv_stJobRead_Start_e:
            JobRead_State_pfct = NvM_Prv_JobRead_Start;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobRead_Start_e;
        break;

        case NvM_Prv_stJobRead_GetUserData_e:
            JobRead_State_pfct = NvM_Prv_JobRead_GetUserData;
        break;

        case NvM_Prv_stJobRead_CrcCheck_e:
            JobRead_State_pfct = NvM_Prv_JobRead_CrcCheck;
        break;

        case NvM_Prv_stJob_DoMemIf_e:
        case NvM_Prv_stJob_PollMemIf_e:
            JobRead_State_pfct = NvM_Prv_JobRead_DoMemIf;
        break;

        case NvM_Prv_stJobRead_RecalcUserDataCrc_e:
            JobRead_State_pfct = NvM_Prv_JobRead_RecalcUserDataCrc;
        break;

        case NvM_Prv_stJobRead_CalcNvBlkCrc_e:
            JobRead_State_pfct = NvM_Prv_JobRead_CalcNvBlkCrc;
        break;

        case NvM_Prv_stJobRead_ExtractAdminData_e:
            JobRead_State_pfct = NvM_Prv_JobRead_ExtractAdminData;
        break;
        case NvM_Prv_stJob_DoCrypto_e:
        case NvM_Prv_stJobRead_CryptoStartSignatureVerification_e:
        case NvM_Prv_stJobRead_CryptoPollSignatureVerification_e:
        case NvM_Prv_stJobRead_CryptoStartDecryption_e:
        case NvM_Prv_stJobRead_CryptoPollDecryption_e:
            JobRead_State_pfct = NvM_Prv_JobRead_DoCrypto;
        break;

        case NvM_Prv_stJobRead_SetUserData_e:
            JobRead_State_pfct = NvM_Prv_JobRead_SetUserData;
        break;

        case NvM_Prv_stJobRead_StartImplicitRecovery_e:
            JobRead_State_pfct = NvM_Prv_JobRead_StartImplicitRecovery;
        break;

        default:
            JobRead_State_pfct = NULL_PTR;
        break;
    }

    return JobRead_State_pfct;
}

/**
 * \brief
 * This local private function is a read job step function and starts the read job.
 * \details
 * This function determines whether the block read is mandatory or can be skipped due to the configuration.
 * The NvM reads a block from the medium without additional checks in following cases:
 * - temporary RAM block is used for current read job
 * - permanent RAM block is used for current read job but data in the permanent RAM block is invalid
 *
 * The NvM checks whether the block read can be skipped in following cases:
 * - permanent RAM block is used for current read job and data in the permanent RAM block is valid
 *
 * Before skipping the block read the NvM checks whether a CRC check for data in the permanent RAM block is configured.
 * If no CRC check is configured then the NvM finishes the read job immediately.
 * In this case permanent RAM block is valid only because of quick initialization feature.
 * If CRC check is configured then the NvM initiates a CRC check to confirm that data in the permanent RAM block
 * is really valid.
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Succeeded_e = Read job started successfully, continue the read job (block read or CRC check required)
 * - NvM_Prv_JobResult_Skipped_e  = no read is required because permanent RAM block contains valid data
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_Start(NvM_Prv_stJob_ten* stJob_pen,
                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                  NvM_Prv_JobData_tst const* JobData_pcst)
{
    // TRACE[SWS_NvM_00362] Before reading data from NV memory NvM checks RAM block CRC
    //                      to avoid overwriting of still valid data with outdated data from NV memory
    boolean isPRamBlockValid_e = NvM_Prv_Block_IsPRamBlockValid(JobData_pcst->idBlock_uo);

    if (JobData_pcst->isPRamBlockUsed_b && isPRamBlockValid_e)
    {
        // Permanent RAM block is used for current read request AND RAM block is valid
        // -> check if RAM block CRC is enabled
        if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
        {
            // RAM block CRC is enabled
            // -> check if RAM block is consistent

            // TRACE[SWS_NvM_00362] Before NvM can check the RAM block CRC for blocks with configured explicit
            //                      synchronization the RAM block data must be copied into the NvM RAM mirror
            //                      via write callback
            *stJob_pen = NvM_Prv_stJobRead_GetUserData_e;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
        }
        else
        {
            // RAM block CRC is disabled
            // -> RAM block with disabled RAM block CRC can be valid only if quick initialization is active
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2802] Request the reading of the block
            //                                               only if the NvM RAM block status bit is not set to VALID.
            *stJob_pen = NvM_Prv_stJob_Finished_e;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Skipped_e;
        }
    }
    else
    {
        // Temporary RAM block is used for current read request OR RAM block is invalid
        // -> read data from NV memory
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
    }
}

/**
 * \brief
 * This local private function is a read job step function and gets user data for a CRC check if required.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - permanent RAM block is used for current read job and data in the permanent RAM block is valid
 * - additional CRC check for data in the permanent RAM block is configured by the user to ensure
 *   that data is valid and consistent
 *
 * Before starting the CRC recalculation this function ensures that the job buffer contains user data.
 * Normally the RAM block used in current read job contains user data and no additional actions are required.
 *
 * The NvM has to copy user data into the job buffer only if current block is configured for explicit configuration
 * and permanent RAM block is used for current read job.
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Succeeded_e  = NvM checks the CRC over the user data
 * - NvM_Prv_JobResult_Pending_e    = copying data into the job buffer is still in progress -> wait 1 cycle
 * - NvM_Prv_JobResult_Failed_e     = NvM continues the read job (CRC check or reading from medium required)
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_GetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM))
    {
        // TRACE[SWS_NvM_00362] Before NvM can check the RAM block CRC for blocks with configured explicit
        //                      synchronization the RAM block data must be copied into the NvM RAM mirror
        //                      via write callback,
        //                      if current block is not configured for explicit synchronization then this step
        //                      always succeeds
        JobResult_pst->Result_en = NvM_Prv_ExplicitSync_CopyData(NvM_Prv_GetCopyFctForWrite(JobData_pcst->idBlock_uo),
                                                                 JobData_pcst->idBlock_uo,
                                                                 &JobData_pcst->cntrExpSyncOperations_u8,
                                                                 JobData_pcst->IntBuffer_st.Buffer_pu8);
    }

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // TRACE[SWS_NvM_00362] Copying data for explicit synchronization has succeeded or
        //                      current block is not configured for explicit synchronization ->
        //                      Initiate RAM block CRC recalculation
        *stJob_pen = NvM_Prv_stJobRead_CrcCheck_e;

        // Prepare CRC recalculation over the user data
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
        JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    }
    else if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
    {
        // If copying data for explicit synchronization has failed then no CRC check can be done
        // -> read data from NV memory
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
    }
    else
    {
        // If copying data for explicit synchronization is still pending do nothing
    }
}

/**
 * \brief
 * This local private function is a read job step function and checks the CRC over the user data in permanent RAM block.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - permanent RAM block is used for current read job and data in the permanent RAM block is valid
 * - additional CRC check for data in the permanent RAM block is configured by the user
 * - job buffer contains user data
 *
 * This function recalculates the CRC over the data in the permanent RAM block and checks if it is equal to the CRC
 * calculated during last successful read/write/restore job.
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e   = CRC recalculation is still in progress -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e = NvM continues the read job (reads data from medium) or
 *                                   no read is required (permanent RAM block contains valid data)
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_CrcCheck(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst)
{
    // TRACE[SWS_NvM_00362] Recalculate RAM block CRC
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
    *stJob_pen = NvM_Prv_stJobRead_CrcCheck_e;

    // Check whether RAM block CRC recalculation has finished
    // Note: RAM block CRC recalculation cannot fail, it is only possible that recalculation takes several cycles.
    //       In this case Result_en will be NvM_Prv_JobStepResult_Pending_e.
    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // CRC calculation finished
        // TRACE[SWS_NvM_00364] Check whether calculated RAM block CRC matches stored RAM block Crc
        if (NvM_Prv_Crc_CheckRamBlockCrc(JobData_pcst->idBlock_uo, &JobResult_pst->CrcData_st.Calculation_st.Crc_un))
        {
            // calculated RAM block CRC matches stored RAM block CRC -> finish read job successfully
            *stJob_pen = NvM_Prv_stJob_Finished_e;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_MemIfSkipped_e;
        }
        else
        {
            // Stored RAM CRC is inconsistent -> read data from NV memory
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        }
    }
}

/**
 * \brief
 * This local private function is a read job step function and polls the MemIf to complete the read job.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - permanent RAM block is used for current read job and data in the permanent RAM block is invalid OR
 *   temporary RAM block is used
 *
 *  This function determines further job steps dependent on the result of the MemIf job:
 * - MEMIF_JOB_OK             = MemIf read job has succeeded and job buffer contains data read from the medium ->
 *                              - if user data is encrypted and no RAM CRC is configured ->
 *                                calculate the CRC over the data read from the non-volatile memory
 *                              - if user data is not encrypted or RAM CRC is configured ->
 *                                calculate the CRC over the user data read from the non-volatile memory
 * - MEMIF_BLOCK_INCONSISTENT = MemIf has not found valid data on the medium -> start implicit recovery if required
 * - MEMIF_BLOCK_INVALID      = MemIf has read invalidated block -> finish job with corresponding result
 * - MEMIF_JOB_PENDING        = MemIf read job is still in progress -> wait 1 cycle
 * - all other return values  = MemIf read job has failed -> start implicit recovery if required
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e           = MemIf job is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e         = MemIf job has read data from the medium -> see description above
 * - NvM_Prv_JobResult_BlockInconsistent_e = MemIf job has not found valid data -> start implicit recovery if required
 * - NvM_Prv_JobResult_BlockInvalidated_e  = MemIf job has read invalidated block -> finish job with corresponding result
 * - NvM_Prv_JobResult_Failed_e            = MemIf read job has failed -> start implicit recovery if required
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
                                    NvM_Prv_JobResult_tst* JobResult_pst,
                                    NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_BlockIdType idBlock_uo = JobData_pcst->idBlock_uo;
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_MemIf_e, stJob_pen, JobResult_pst, JobData_pcst);

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // MemIf job has succeeded, data read from the no-volatile memory is in the internal buffer

        if (   !NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC)
            || JobData_pcst->isEncryptionEnabled_b)
        {
            // Read data is encrypted or no RAM CRC is configured

            if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_NV_CRC))
            {
                // NV CRC is configured
                // -> calculate the CRC over the data read from the non-volatile memory
                *stJob_pen = NvM_Prv_stJobRead_CalcNvBlkCrc_e;

                // Update number of used bytes in internal buffer so that it is correct after CRC recalculation
                *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 -= NvM_Prv_Crc_GetSizeCrc(NvM_Prv_GetCrcType(idBlock_uo));

                // Prepare CRC recalculation over the read data
                JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
                JobResult_pst->CrcData_st.Calculation_st.Length_u16 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16;
                JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
                JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
            }
            else
            {
                // no NV CRC is configured
                // -> extract administrative data from the read data
                *stJob_pen = NvM_Prv_stJobRead_ExtractAdminData_e;
            }
        }
        else
        {
            // Read data is not encrypted and RAM CRC is configured
            // -> calculate the CRC over the user data read from the non-volatile memory,
            //    user data is always at the beginning of the NV block
            *stJob_pen = NvM_Prv_stJobRead_RecalcUserDataCrc_e;

            // Prepare CRC recalculation over the user data
            JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
            JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(idBlock_uo);
            JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
            JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
        }
    }
    // TRACE[SWS_NvM_00882] Read job for NvM_ReadPRAMBlock shall load default values
    //                      if lower layer has read inconsistent value
    else if (NvM_Prv_JobResult_BlockInconsistent_e == JobResult_pst->Result_en)
    {
        // MemIf job has read inconsistent block -> start implicit recovery
        *stJob_pen = NvM_Prv_stJobRead_StartImplicitRecovery_e;
    }
    else if (NvM_Prv_JobResult_BlockInvalidated_e == JobResult_pst->Result_en)
    {
        // MemIf job has returned invalidated block -> finish read job
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else if (NvM_Prv_JobResult_Pending_e == JobResult_pst->Result_en)
    {
        // MemIf job is still pending -> remain in this state
    }
    else
    {
        // TRACE[SWS_NvM_00789] Read job for NvM_ReadPRAMBlock shall load default values if lower layer fails
        // TRACE[SWS_NvM_00882] Read job for NvM_ReadPRAMBlock shall load default values if lower layer fails

        // MemIf job has failed -> start implicit recovery, update final job result
        JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        *stJob_pen = NvM_Prv_stJobRead_StartImplicitRecovery_e;
    }
}

/**
 * \brief
 * This local private function is a read job step function and recalculates the CRC over the user data.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - MemIf read job has succeeded AND (user data is not encrypted and RAM CRC is configured)
 * - User data decryption has succeeded and RAM CRC is configured
 *
 *  This function determines further job steps dependent on user data encryption
 * - user data encrypted     = check if recalculated CRC over user data is equal to the CRC calculated
 *                             during last successful read/write/restore job ->
 *                             - CRC check successful -> copy user data into the RAM block used in current job
 *                             - CRC check not successful -> start implicit recovery if required
 * - user data not encrypted = check if NV CRC is configured ->
 *                             - NV CRC -> complete CRC recalculation over the read data
 *                             - no NV CRC -> extract administrative data from the read data
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e   = CRC recalculation is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e = CRC recalculation has succeeded -> see description above
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_RecalcUserDataCrc(NvM_Prv_stJob_ten* stJob_pen,
                                              NvM_Prv_JobResult_tst* JobResult_pst,
                                              NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_BlockIdType idBlock_uo = JobData_pcst->idBlock_uo;
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
    {
        // TRACE[SWS_NvM_00201] Read job for NvM_ReadBlock shall recalculate CRC over the RAM block
        //                      after the copy process if configured
        // TRACE[SWS_NvM_00786] Read job for NvM_ReadPRAMBlock shall recalculate CRC over the RAM block
        //                      after the copy process if configured
        NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
        *stJob_pen = NvM_Prv_stJobRead_RecalcUserDataCrc_e;
    }

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // CRC recalculation over the user data has succeeded or is not configured

        // Copy CRC over the user data for later usage
        JobResult_pst->CrcData_st.CrcRamBlk_un.Crc32_u32 = JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32;

        if (JobData_pcst->isEncryptionEnabled_b)
        {
            // Read data is encrypted, NvM has arrived in this state after decrypting read data
            // -> check if recalculated CRC over decrypted user data matches the CRC stored with encrypted user data

            // Update number of used bytes in internal buffer so that it is correct after CRC recalculation
            *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(idBlock_uo);

            if (NvM_Prv_Crc_CheckCrc(idBlock_uo,
                                     &JobData_pcst->IntBuffer_st.Buffer_pu8[JobData_pcst->IntBuffer_st.OffsetPlainData_u16],
                                     JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16,
                                     &JobResult_pst->CrcData_st.Calculation_st.Crc_un))
            {
                *stJob_pen = NvM_Prv_stJobRead_SetUserData_e;
            }
            else
            {
                // Crc is not equal
                *stJob_pen = NvM_Prv_stJobRead_StartImplicitRecovery_e;
                JobResult_pst->ProductionError_u8 |= NVM_E_RB_INTEGRITY_FAILED;
                JobResult_pst->Result_en = NvM_Prv_JobResult_BlockInconsistent_e;
            }
        }
        else
        {
            // Read data is not encrypted, NvM has arrived in this state after reading data from the non-volatile memory

            if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_NV_CRC))
            {
                // -> complete the CRC calculation over the data read from the non-volatile memory
                *stJob_pen = NvM_Prv_stJobRead_CalcNvBlkCrc_e;

                // Update number of used bytes in internal buffer so that it is correct after CRC recalculation
                *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 -= NvM_Prv_Crc_GetSizeCrc(NvM_Prv_GetCrcType(idBlock_uo));
                // Prepare CRC recalculation over the remaining data read from the non-volatile memory
                JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = FALSE;
                JobResult_pst->CrcData_st.Calculation_st.Length_u16 = (  *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16
                                                                       - NvM_Prv_GetBlockSize(idBlock_uo));
                JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 =
                        &JobData_pcst->IntBuffer_st.Buffer_pu8[NvM_Prv_GetBlockSize(idBlock_uo)];
            }
            else
            {
                // -> extract administrative data from the read data
                *stJob_pen = NvM_Prv_stJobRead_ExtractAdminData_e;
            }
        }
    }
    else
    {
        // CRC recalculation cannot fail.
        // It is only possible that CRC recalculation is still pending and in this case NvM remains in current state
    }
}

/**
 * \brief
 * This local private function is a read job step function and calculates the CRC over the data read
 * from the non-volatile memory.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - MemIf read job has succeeded AND (user data is encrypted or no RAM CRC is configured),
 *   i.e. this function calculates the CRC over whole data read from the non-volatile memory
 * - User data is not encrypted and CRC over user data has been recalculated,
 *   i.e. this function calculates the CRC over the remaining data read from the non-volatile memory
 *
 *  This function determines further job steps dependent on the consistency of the CRC read from the non-volatile memory
 * - calculated CRC matches the CRC read from the non-volatile memory or no NV CRC is configured ->
 *   extract administrative data from the read data
 * - calculated CRC doesn't match the CRC read from the non-volatile memory ->
 *   start implicit recovery if required
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e   = CRC recalculation is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e = CRC recalculation has succeeded -> see description above
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_CalcNvBlkCrc(NvM_Prv_stJob_ten* stJob_pen,
                                         NvM_Prv_JobResult_tst* JobResult_pst,
                                         NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_NV_CRC))
    {
        NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
        *stJob_pen = NvM_Prv_stJobRead_CalcNvBlkCrc_e;

        if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
        {
            // CRC calculation over data read from the non-volatile memory has succeeded

            // Extract and compare the Nv CRC read from the non-volatile memory against the calculated CRC
            if (NvM_Prv_Crc_CheckCrc(JobData_pcst->idBlock_uo,
                                     JobData_pcst->IntBuffer_st.Buffer_pu8,
                                     JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16,
                                     &JobResult_pst->CrcData_st.Calculation_st.Crc_un))
            {
                // calculated CRC matches the CRC read from the non-volatile memory
                // -> read data is consistent, extract administrative data from the read data
                *stJob_pen = NvM_Prv_stJobRead_ExtractAdminData_e;
            }
            else
            {
                // calculated CRC does not match the CRC read from the non-volatile memory
                // -> read data is inconsistent, start implicit recovery
                JobResult_pst->ProductionError_u8 |= NVM_E_RB_INTEGRITY_FAILED;
                JobResult_pst->Result_en = NvM_Prv_JobResult_BlockInconsistent_e;
                *stJob_pen = NvM_Prv_stJobRead_StartImplicitRecovery_e;
            }
        }
        else
        {
            // CRC calculation cannot fail.
            // It is only possible that CRC calculation is still pending and in this case NvM remains in current state
        }
    }
    else
    {
        // Nv block CRC is not configured
        // -> extract administrative data from the read data
        *stJob_pen = NvM_Prv_stJobRead_ExtractAdminData_e;
    }
}

/**
 * \brief
 * This local private function is a read job step function and extracts administrative data out of the data read
 * from the non-volatile memory.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - CRC read from the non-volatile memory (NV CRC) is consistent
 * - no NV CRC is configured
 *
 * This function always continues with cryptographic services.
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Succeeded_e = administrative data extracted -> handle cryptographic services
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_ExtractAdminData(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst)
{
    // Extract write counter out of the data read from the non-volatile memory
    NvM_Prv_Block_ExtractWriteCounter(JobData_pcst->idBlock_uo,
                                      JobData_pcst->IntBuffer_st.Buffer_pu8,
                                      JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16);

    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
}

/**
 * \brief
 * This local private function is a read job step function and handles cryptographic services.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - data read from the non-volatile memory is consistent or no NV CRC is configured
 *
 * This function executes the state machine for cryptographic read job
 * (see NvM_Prv_Crypto_DoStateMachine and NvM_Prv_Crypto_GetJobStepRead).
 *
 * After completing cryptographic read job this function determines further job steps dependent on user data encryption:
 * - user data encrypted     -> recalculate CRC over the decrypted user data to check it for consistency
 *                              (see state NvM_Prv_JobRead_RecalcUserDataCrc)
 * - user data not encrypted ->  copy user data into the RAM block
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e           = Cryptographic services are still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_BlockInconsistent_e = Cryptographic services has found inconsistent data ->
 *                                           start implicit recovery if required
 * - NvM_Prv_JobResult_Failed_e            = Cryptographic services has failed -> start implicit recovery if required
 * - NvM_Prv_JobResult_Succeeded_e         = Cryptographic services has succeeded -> see description above
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_DoCrypto(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_CRYPTO))
    {
        NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crypto_e, stJob_pen, JobResult_pst, JobData_pcst);
    }

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // Cryptographic services for read job have succeeded or are not configured

        if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC)
            && JobData_pcst->isEncryptionEnabled_b)
        {
            // read data is encrypted
            // -> calculate the CRC over the decrypted user data
            *stJob_pen = NvM_Prv_stJobRead_RecalcUserDataCrc_e;

            // Prepare CRC recalculation over the decrypted user data
            JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
            JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
            JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
            JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 =
                    &JobData_pcst->IntBuffer_st.Buffer_pu8[JobData_pcst->IntBuffer_st.OffsetPlainData_u16];
        }
        else
        {
            // read data is not encrypted
            // -> copy read user data into the RAM block
            *stJob_pen = NvM_Prv_stJobRead_SetUserData_e;
        }
    }
    else if (   (NvM_Prv_JobResult_BlockInconsistent_e == JobResult_pst->Result_en)
             || (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en))
    {
        *stJob_pen = NvM_Prv_stJobRead_StartImplicitRecovery_e;
    }
    else
    {
        // Cryptographic services for read job are still pending ->
        // remain in this state, do not update final job result
    }
}

/**
 * \brief
 * This local private function is a read job step function and copies read data into the RAM block used for current job.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - data read from the non-volatile memory is decrypted (if required) and consistent
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e   = Copying of user data is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Failed_e    = Copying of user data has failed -> finish job
 * - NvM_Prv_JobResult_Succeeded_e = Copying of user data has succeeded -> finish job
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_SetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    uint16 OffsetIntBuffer_uo = JobData_pcst->IntBuffer_st.OffsetPlainData_u16;

    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM)
        && JobData_pcst->isPRamBlockUsed_b)
    {
        JobResult_pst->Result_en = NvM_Prv_ExplicitSync_CopyData(NvM_Prv_GetCopyFctForRead(JobData_pcst->idBlock_uo),
                                                                 JobData_pcst->idBlock_uo,
                                                                 &JobData_pcst->cntrExpSyncOperations_u8,
                                                                 &JobData_pcst->IntBuffer_st.Buffer_pu8[OffsetIntBuffer_uo]);
    }
    else
    {
        if (JobData_pcst->isIntBufferToUse_b)
        {
            JobResult_pst->Result_en = NvM_Prv_InternalBuffer_CopyData(JobData_pcst->idBlock_uo,
                                                                       JobData_pcst->UserData_un,
                                                                       &JobData_pcst->IntBuffer_st.Buffer_pu8[OffsetIntBuffer_uo],
                                                                       FALSE);
        }
    }

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // Copying data for explicit synchronization has succeeded or was not required ->
        // Job is finished, update final job result and save CRC

        if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC)
            && JobData_pcst->isPRamBlockUsed_b)
        {
            // Set calculated CRC if permanent RAM block is used
            NvM_Prv_Crc_SetRamBlockCrc(JobData_pcst->idBlock_uo, &JobResult_pst->CrcData_st.CrcRamBlk_un);
        }

        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
    {
        // Copying data for explicit synchronization has failed ->
        // finish read job, update final job result
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else
    {
        // Copying data for explicit synchronization is still pending ->
        // remain in this state, do not update final job result
    }
}

/**
 * \brief
 * This local private function is a read job step function and starts implicit recovery
 * if default data is available for the current block.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - reading user data from the medium has failed or inconsistent data has been read
 * - Some of the checks has failed (CRC check, cryptographic services)
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e   = Implicit recovery has been started and NvM will execute a restore job
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobRead_StartImplicitRecovery(NvM_Prv_stJob_ten* stJob_pen,
                                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                                  NvM_Prv_JobData_tst const* JobData_pcst)
{
    // TRACE[SWS_NvM_00788] Read job for NvM_ReadPRAMBlock remains RAM block as is if there are no default values

    // Implicit recovery will be started only if MemIf read job did not finish successfully
    // -> start implicit recovery only for blocks with available default data
    // Note: As opposed to NVM389, implicit recovery is (currently?) not supported for DATASET blocks
    // TRACE[SWS_NvM_00951] Read job for NvM_ReadBlock or NvM_ReadPRAMBlock shall provide implicit recovery
    //                      only for blocks of type native and redundant
    if ((NvM_Prv_IsDefaultDataAvailable(JobData_pcst->idBlock_uo)) &&
        (NVM_BLOCK_DATASET != NvM_Prv_GetBlockType(JobData_pcst->idBlock_uo)))
    {

        // Start implicit recovery
        NvM_Prv_Job_Start(stJob_pen,
                          &JobResult_pst->Result_en,
                          JobData_pcst,
                          NvM_Prv_idJob_RestoreForImplicitRecovery_e,
                          JobData_pcst->idxDataset_u8);
    }
    else
    {
        // Finish read job
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

