
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
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

static void NvM_Prv_JobWrite_GetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                         NvM_Prv_JobResult_tst* JobResult_pst,
                                         NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobWrite_RecalcUserDataCrc(NvM_Prv_stJob_ten* stJob_pen,
                                               NvM_Prv_JobResult_tst* JobResult_pst,
                                               NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobWrite_DoCrypto(NvM_Prv_stJob_ten* stJob_pen,
                                      NvM_Prv_JobResult_tst* JobResult_pst,
                                      NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobWrite_AppendAdminData(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobWrite_CalcNvCrc(NvM_Prv_stJob_ten* stJob_pen,
                                       NvM_Prv_JobResult_tst* JobResult_pst,
                                       NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobWrite_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
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
 * This private function returns the pointer to the job step for the current state of the write job.
 * \details
 * This function returns a NULL pointer if passed write job state is invalid. In this case the write job will fail.
 *
 * \param [inout] stJob_en
 * Pointer to the current state of the write job
 *
 * \return
 * Pointer to the job step function or NULL pointer for invalid write job state
 */
NvM_Prv_Job_State_tpfct NvM_Prv_JobWrite_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobWrite_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
        case NvM_Prv_stJobWrite_GetUserData_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_GetUserData;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobWrite_GetUserData_e;
        break;

        case NvM_Prv_stJobWrite_RecalcUserDataCrc_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_RecalcUserDataCrc;
        break;

        case NvM_Prv_stJob_DoCrypto_e:
        case NvM_Prv_stJobWrite_CryptoStartGenerationRandom_e:
        case NvM_Prv_stJobWrite_CryptoStartEncryption_e:
        case NvM_Prv_stJobWrite_CryptoStartGenerationSignature_e:
        case NvM_Prv_stJobWrite_CryptoPollGenerationRandom_e:
        case NvM_Prv_stJobWrite_CryptoPollEncryption_e:
        case NvM_Prv_stJobWrite_CryptoPollGenerationSignature_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_DoCrypto;
        break;

        case NvM_Prv_stJobWrite_AppendAdminData_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_AppendAdminData;
        break;

        case NvM_Prv_stJobWrite_CalcNvBlkCrc_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_CalcNvCrc;
        break;

        case NvM_Prv_stJob_DoMemIf_e:
        case NvM_Prv_stJob_PollMemIf_e:
            JobWrite_State_pfct = NvM_Prv_JobWrite_DoMemIf;
        break;

        default:
            JobWrite_State_pfct = NULL_PTR;
        break;
    }

    return JobWrite_State_pfct;
}

/**
 * \brief
 * This local private function is a write job step function and copies user data into the internal buffer if required.
 * \details
 * After successful copying the NvM internal buffer contains user data.
 *
 * \param [inout] stJob_pen
 * Pointer to the current job state
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e   = Copying of user data is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Failed_e    = Copying of user data has failed -> finish write job
 * - NvM_Prv_JobResult_Succeeded_e = Copying of user data has succeeded -> recalculate CRC over user data
 *                                                                         (CRC will be used to check if write can be omitted)
 * \param [in] JobData_pcst
 * Constant pointer to the job data
 */
static void NvM_Prv_JobWrite_GetUserData(NvM_Prv_stJob_ten* stJob_pen,
                                         NvM_Prv_JobResult_tst* JobResult_pst,
                                         NvM_Prv_JobData_tst const* JobData_pcst)
{
    uint16 OffsetPlainData_u16 = JobData_pcst->IntBuffer_st.OffsetPlainData_u16;
    uint8* IntBuffer_pu8 = &JobData_pcst->IntBuffer_st.Buffer_pu8[OffsetPlainData_u16];

    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
    // In case write-monitoring is active and the block to be written is the WriteMon-block itself the counters
    // have to be increased before the CRC is calculated. This can be safely done here as the WriteMon-block is
    // always written.
    // CAUTION: WriteMonitorCounters shall be updated before copying block data into the internal buffer.
    //          Otherwise NvM will copy outdated values into the internal buffer.
    NvM_Prv_WriteMonCntrHandling(JobData_pcst->idBlock_uo,
                                 NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo),
                                 TRUE);
#endif

    *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);

    if (JobData_pcst->isIntBufferToUse_b)
    {
        // Check whether explicit synchronization is enabled and permanent RAM block is used
        // Attention: if temporary RAM block is used for blocks with explicit synchronization
        //            then user data will be copied into the internal buffer via NvM_Prv_InternalBuffer_CopyData
        if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM)
            && JobData_pcst->isPRamBlockUsed_b)
        {
            JobResult_pst->Result_en = NvM_Prv_ExplicitSync_CopyData(NvM_Prv_GetCopyFctForWrite(JobData_pcst->idBlock_uo),
                                                                     JobData_pcst->idBlock_uo,
                                                                     &JobData_pcst->cntrExpSyncOperations_u8,
                                                                     IntBuffer_pu8);
        }
        else
        {
            JobResult_pst->Result_en = NvM_Prv_InternalBuffer_CopyData(JobData_pcst->idBlock_uo,
                                                                       JobData_pcst->UserData_un,
                                                                       IntBuffer_pu8,
                                                                       TRUE);
        }
    }

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // Copying data into the internal buffer has succeeded or was not required ->
        // continue with CRC recalculation over the user data
        *stJob_pen = NvM_Prv_stJobWrite_RecalcUserDataCrc_e;

        // Prepare CRC recalculation over the user data
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
        JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = IntBuffer_pu8;
    }
    else if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
    {
        // Copying data for explicit synchronization has failed ->
        // finish write job
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else
    {
        // Copying data into the internal buffer is still pending ->
        // remain in this state, job result is pending
    }
}

/**
 * \brief
 * This local private function is a write job step function and recalculates the CRC over user data if configured.
 * \details
 * The NvM reaches this job step only if job buffer contains user data.
 *
 * This function determines further job steps dependent on the RAM CRC configuration
 * - RAM CRC configured    = check if user has changed data compared to last successful read/write/restore ->
 *                           - user has changed data     -> continue with cryptographic services
 *                           - user has not changed data -> finish job
 * - no RAM CRC configured = continue with cryptographic services
 *
 * \param [inout] stJob_pen
 * Pointer to the current job state
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e                = CRC recalculation is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e              = CRC recalculation has succeeded or not configured, user data changed ->
 *                                                continue with cryptographic services
 * - NvM_Prv_JobResult_Succeeded_MemIfSkipped_e = CRC recalculation has succeeded, user data not changed ->
 *                                                finish job
 * \param [in] JobData_pcst
 * Constant pointer to the job data
 */
static void NvM_Prv_JobWrite_RecalcUserDataCrc(NvM_Prv_stJob_ten* stJob_pen,
                                               NvM_Prv_JobResult_tst* JobResult_pst,
                                               NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
    {
        // TRACE[SWS_NvM_00212] Write job for NvM_WriteBlock shall recalculate CRC over the RAM block
        //                      before the copy process if configured
        // TRACE[SWS_NvM_00801] Write job for NvM_WritePRAMBlock shall recalculate CRC over the RAM block
        //                      before the copy process if configured
        NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
        *stJob_pen = NvM_Prv_stJobWrite_RecalcUserDataCrc_e;

        if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
        {
            // CRC recalculation over the user data has succeeded ->
            // check whether the write is required

            // Write is required in following cases:
            // - User has changed data in PRAM block and notified NvM about it via NvM_SetRamBlockStatus,
            //   NvM has recalculated the CRC over new user data in background (see NvM_JobRecalcRamBlkCrc.c) and
            //   has determined that data has been changed -> write is required without additional CRC checks
            // - CRC compare mechanism is enabled for current block->
            //   check if user wants to write data which has been changed comparing to last successful read or write

            boolean isWriteReq_b = TRUE;
            boolean isWriteReqDueToBackgroundCrcRecalc_b = NvM_Prv_Block_IsWriteRequired(JobData_pcst->idBlock_uo);

            if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo,
                                           NVM_PRV_BLOCK_FLAG_USE_CRC_COMP_MECHANISM)
                && !isWriteReqDueToBackgroundCrcRecalc_b)
            {
                isWriteReq_b = !NvM_Prv_Crc_CheckRamBlockCrc(JobData_pcst->idBlock_uo,
                                                             &JobResult_pst->CrcData_st.Calculation_st.Crc_un);
            }

            if (isWriteReq_b)
            {
                // Copy CRC over the user data for later usage
                JobResult_pst->CrcData_st.CrcRamBlk_un.Crc32_u32 =
                        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32;

                // write is required ->  continue with cryptographic services
                *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
            }
            else
            {
                // recalculated CRC is equal to RAM block CRC ->
                // NvM can finish write job successfully without writing into the NV memory
                // TRACE[SWS_NvM_00852]
                // TRACE[SWS_NvM_00854] Skip writing because the data has not been changed since last successful
                //                      read or write job
                JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_MemIfSkipped_e;
                *stJob_pen = NvM_Prv_stJob_Finished_e;
            }
        }
        else
        {
            // CRC recalculation cannot fail.
            // It is only possible that CRC recalculation is still pending and in this case NvM remains in current state
        }
    }
    else
    {
        // CRC recalculation over the user data is not configured ->
        // write is required -> continue with cryptographic services
        *stJob_pen = NvM_Prv_stJob_DoCrypto_e;
    }
}

/**
 * \brief
 * This local private function is a write job step function and handles cryptographic services if configured.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - CRC recalculation has succeeded
 * - write is required
 *
 * This function executes the state machine for cryptographic write job
 * (see NvM_Prv_Crypto_DoStateMachine and NvM_Prv_Crypto_GetJobStepWrite).
 *
 * \param [inout] stJob_pen
 * Pointer to the current job state
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e   = Cryptographic services are still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Failed_e    = Cryptographic services has failed -> finish job
 * - NvM_Prv_JobResult_Succeeded_e = Cryptographic services has succeeded -> append administrative data
 * \param [in] JobData_pcst
 * Constant pointer to the job data
 */
static void NvM_Prv_JobWrite_DoCrypto(NvM_Prv_stJob_ten* stJob_pen,
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
        // Cryptographic services for write job have succeeded or are not configured ->
        // append administrative data to the internal buffer which will be written into the non-volatile memory
        *stJob_pen = NvM_Prv_stJobWrite_AppendAdminData_e;
    }
    else if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
    {
        // Cryptographic services for write job have failed ->
        // finish write job, update final job result
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else
    {
        // Cryptographic services for write job are still pending ->
        // remain in this state, do not update final job result
    }
}

/**
 * \brief
 * This local private function is a write job step function and appends administrative data to the data to be written
 * into the non-volatile memory.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - write is required
 * - cryptographic services have succeeded
 *
 * This function always continues with CRC calculation over the data to be written into the non-volatile memory (NV CRC).
 *
 * \param [inout] stJob_pen
 * Pointer to the current job state
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Succeeded_e = administrative data appended -> calculate NV CRC
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobWrite_AppendAdminData(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst)
{
    // Increase write counter
    // TRACE[BSW_SWS_AR_NVRAMManager_Ext-3287] NvM shall increase the counter before initiating write operation
    //                                         on lower layer (MemIf) if enabled
    // TRACE[BSW_SWS_AR_NVRAMManager_Ext-3288] NvM shall store write counter persistently together with user data
    //                                         if enabled
    // If NvM has reached this state during a write job then a write operation on lower level will be initiated
    // definitely
    uint16 UsedSize_u16 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16;
    uint16 CntrSize_u16 = NvM_Prv_Block_AppendWriteCounter(JobData_pcst->idBlock_uo,
                                                           &JobData_pcst->IntBuffer_st.Buffer_pu8[UsedSize_u16]);

    *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 += CntrSize_u16;

    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    // Copying administrative data into the internal buffer succeeds always or is not required ->
    // continue with CRC calculation over the data to be written into the non-volatile memory
    *stJob_pen = NvM_Prv_stJobWrite_CalcNvBlkCrc_e;

    // Prepare CRC calculation over the data to be written into the non-volatile memory
    if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC)
        && !JobData_pcst->isEncryptionEnabled_b)
    {
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = FALSE;
        JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 =
                &JobData_pcst->IntBuffer_st.Buffer_pu8[NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo)];
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = (  *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16
                                                               - NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo));
    }
    else
    {
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
        JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16;
    }
}

/**
 * \brief
 * This local private function is a write job step function and calculates the CRC over data to be written
 * into the non-volatile memory if configured.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - write is required
 * - cryptographic services have succeeded
 * - job buffer contains all data to be written
 *
 * This function always continues with MemIf write job.
 *
 * \param [inout] stJob_pen
 * Pointer to the current job state
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e   = CRC calculation is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e = CRC calculation has succeeded or is not configured -> continue with MemIf job
 * \param [in] JobData_pcst
 * Constant pointer to the job data
 */
static void NvM_Prv_JobWrite_CalcNvCrc(NvM_Prv_stJob_ten* stJob_pen,
                                         NvM_Prv_JobResult_tst* JobResult_pst,
                                         NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_NV_CRC))
    {
        NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
        *stJob_pen = NvM_Prv_stJobWrite_CalcNvBlkCrc_e;

        if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
        {
            // CRC calculation over data to be written into the non-volatile memory has succeeded ->
            // continue with MemIf job
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;

            // Append the Crc at the end of the buffer
            NvM_Prv_Crc_AppendCrc(JobData_pcst->idBlock_uo,
                                  JobData_pcst->IntBuffer_st.Buffer_pu8,
                                  JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16,
                                  &JobResult_pst->CrcData_st.Calculation_st.Crc_un);
        }
        else
        {
            // CRC calculation cannot fail.
            // It is only possible that CRC calculation is still pending and in this case NvM remains in current state
        }
    }
    else
    {
        // CRC recalculation over data to be written into the non-volatile memory is not configured ->
        // initiate MemIf write job
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
    }
}

/**
 * \brief
 * This local private function is a write job step function and polls the MemIf to complete the write job.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - CRC recalculation has succeeded
 * - write is required
 * - cryptographic services has succeeded
 * - job buffer contains data to be written
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e           = MemIf job is still pending -> wait 1 cycle
 * - NvM_Prv_JobResult_Succeeded_e         = MemIf job has written data to the non-volatile memory -> finish job
 * - NvM_Prv_JobResult_Failed_e            = MemIf write job has failed -> finish job
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_JobWrite_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_MemIf_e, stJob_pen, JobResult_pst, JobData_pcst);

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // MemIf job has succeeded -> finish write job
        *stJob_pen = NvM_Prv_stJob_Finished_e;

        // NvM has successfully written user data into the non-volatile memory ->
        // update Ram block CRC so that it matches user data in non-volatile memory
        if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
        {
            NvM_Prv_Crc_SetRamBlockCrc(JobData_pcst->idBlock_uo, &JobResult_pst->CrcData_st.CrcRamBlk_un);
        }
    }
    else if (NvM_Prv_JobResult_Pending_e == JobResult_pst->Result_en)
    {
        // MemIf job is still pending -> remain in this state, do not update final job result
    }
    else
    {
        // MemIf job has failed -> finish write job, update final job result
        JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

