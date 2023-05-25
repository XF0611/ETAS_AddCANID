
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "MemIf.h"
// TRACE[NVM089] Check version compatibility of included header files
#if (!defined(MEMIF_AR_RELEASE_MAJOR_VERSION) || (MEMIF_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
# error "AUTOSAR major version undefined or mismatched"
#endif

#include "NvM_Prv.h"
#include "NvM_Prv_MemIf.h"
#include "NvM_Prv_Crypto.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_Job_Types.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
# define NVM_START_SEC_CODE
# include "NvM_MemMap.h"

static NvM_Prv_Job_State_tpfct NvM_Prv_MemIf_GetStateFct(NvM_Prv_idJob_ten idJob_en,
                                                         NvM_Prv_stJob_ten* stJob_pen);

static void NvM_Prv_MemIf_Poll(NvM_Prv_stJob_ten* stJob_pen,
                               NvM_Prv_JobResult_tst* JobResult_pst,
                               NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_MemIf_InitiateRead(NvM_Prv_stJob_ten* stJob_pen,
                                       NvM_Prv_JobResult_tst* JobResult_pst,
                                       NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_MemIf_InitiateWrite(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_MemIf_InitiateInvalidate(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_MemIf_InitiateMaintain(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst);

# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

void NvM_Prv_MemIf_DoStateMachine(NvM_Prv_stJob_ten* stJob_pen,
                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                  NvM_Prv_JobData_tst const* JobData_pcst)
{
    do
    {
        NvM_Prv_Job_State_tpfct State_pfct = NvM_Prv_MemIf_GetStateFct(JobData_pcst->idJob_en,
                                                                       stJob_pen);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
        TestCd_NvM_JobMonitor_SetState(JobData_pcst, *stJob_pen);
#endif

        if (NULL_PTR != State_pfct)
        {
            State_pfct(stJob_pen, JobResult_pst, JobData_pcst);
        }
        else
        {
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        }

    } while((NvM_Prv_stJob_DoMemIf_e != *stJob_pen) &&
            (NvM_Prv_JobResult_Pending_e != JobResult_pst->Result_en));
}

static NvM_Prv_Job_State_tpfct NvM_Prv_MemIf_GetStateFct(NvM_Prv_idJob_ten idJob_en,
                                                         NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct State_pfct;

    if (NvM_Prv_stJob_PollMemIf_e == *stJob_pen)
    {
        State_pfct = NvM_Prv_MemIf_Poll;
    }
    else
    {
        switch (idJob_en)
        {
            case NvM_Prv_idJob_Read_e:
            case NvM_Prv_idJob_ReadIdConfigForReadAll_e:
            {
                *stJob_pen = NvM_Prv_stJobRead_InitiateMemIf_e;
                State_pfct = NvM_Prv_MemIf_InitiateRead;
            }
            break;

            case NvM_Prv_idJob_Write_e:
            case NvM_Prv_idJob_WriteAll_e:
            {
                *stJob_pen = NvM_Prv_stJobWrite_InitiateMemIf_e;
                State_pfct = NvM_Prv_MemIf_InitiateWrite;
            }
            break;

            case NvM_Prv_idJob_Erase_e:
            case NvM_Prv_idJob_Invalidate_e:
            case NvM_Prv_idJob_InvalidateForFirstInitAll_e:
            case NvM_Prv_idJob_InvalidateForRemoveNonResistant_e:
            {
                *stJob_pen = NvM_Prv_stJobInvalidate_InitiateMemIf_e;
                State_pfct = NvM_Prv_MemIf_InitiateInvalidate;
            }
            break;

            case NvM_Prv_idJob_Maintain_e:
            {
                *stJob_pen = NvM_Prv_stJobMaintain_InitiateMemIf_e;
                State_pfct = NvM_Prv_MemIf_InitiateMaintain;
            }
            break;

            default:
            {
                State_pfct = NULL_PTR;
            }
            break;
        }
    }

    return State_pfct;
}

static void NvM_Prv_MemIf_Poll(NvM_Prv_stJob_ten* stJob_pen,
                               NvM_Prv_JobResult_tst* JobResult_pst,
                               NvM_Prv_JobData_tst const* JobData_pcst)
{
    // TRACE[NVM346] Check the status of the currently active MemIf job
    MemIf_JobResultType ResultMemIf_en = MemIf_GetJobResult(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo));
    // If only Fee or only Ea is used then function MemIf_GetJobResult will not use idxDevice and
    // compiler generates a warning "unused parameter".
    // Following line was added to avoid this warning
    (void)JobData_pcst;

    if (MEMIF_JOB_PENDING != ResultMemIf_en)
    {
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
    }

    switch (ResultMemIf_en)
    {
        case MEMIF_JOB_PENDING:
        {
            JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;
        }
        break;
        case MEMIF_JOB_OK:
        {
            JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
        }
        break;
        case MEMIF_BLOCK_INCONSISTENT:
        {
            JobResult_pst->Result_en = NvM_Prv_JobResult_BlockInconsistent_e;
        }
        break;
        case MEMIF_BLOCK_INVALID:
        {
            JobResult_pst->Result_en = NvM_Prv_JobResult_BlockInvalidated_e;
        }
        break;
        default:
        {
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        }
        break;
    }
}

/**
 * \brief
 * This local private function is a read job step function and initiates a MemIf read job.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - permanent RAM block is used for current read job and data in the permanent RAM block is invalid OR
 *   temporary RAM block is used
 *
 * This function gets the number of bytes to be read from the block configuration
 * (see NvM_Prv_BlkDesc_GetBlockSizeStored).
 *
 * \param [inout] stJob_pen
 * State of the read job
 * \param [inout] JobResult_pst
 * Pointer to the job results, possible values:
 * - NvM_Prv_JobResult_Pending_e = MemIf job initiation has succeeded -> wait 1 cycle before polling the MemIf
 * - NvM_Prv_JobResult_Failed_e  = MemIf job initiation has failed -> start implicit recovery if required
 * \param [in] JobData_pcst
 * Pointer to the data of the current NvM job
 */
static void NvM_Prv_MemIf_InitiateRead(NvM_Prv_stJob_ten* stJob_pen,
                                       NvM_Prv_JobResult_tst* JobResult_pst,
                                       NvM_Prv_JobData_tst const* JobData_pcst)
{
    // MemIf job initiation has succeeded -> Poll MemIf job
    JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;
    *stJob_pen = NvM_Prv_stJob_PollMemIf_e;

    *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_BlkDesc_GetBlockSizeStored(JobData_pcst->idBlock_uo);

    // Start a MemIf_Read job
    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_MIGRATION))
    {
        // Reads all blocks where Migration or variable block length feature is active
        // Migration feature can be active without variable block length feature
        // Variable block length feature neeads activation of Migration feature
        if (E_OK != MemIf_Rb_VarLenRead(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                                        NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) + JobData_pcst->idxDataset_u8,
                                        0,
                                        JobData_pcst->IntBuffer_st.Buffer_pu8,
                                        *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16))
        {
            // MemIf job initiation has failed
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        }
    }
    else
    {
        // Reads a normal block without Migration or variable block length feature active
        if (E_OK != MemIf_Read(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                               NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) + JobData_pcst->idxDataset_u8,
                               0,
                               JobData_pcst->IntBuffer_st.Buffer_pu8,
                               *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16))
        {
            // MemIf job initiation has failed
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        }
    }
}

/**
 * \brief
 * This local private function is a write job step function and initiates a MemIf write job.
 * \details
 * The NvM reaches this job step only if following preconditions are true:
 * - CRC recalculation has succeeded
 * - write is required
 * - cryptographic services has succeeded
 * - job buffer contains data to be written
 *
 * \param [inout] stJob_pen
 * State of the write job
 * \param [inout] JobResult_pst
 * Pointer to the job results,  possible values:
 * - NvM_Prv_JobResult_Pending_e = MemIf job initiation has succeeded -> wait 1 cycle before polling the MemIf
 * - NvM_Prv_JobResult_Failed_e  = MemIf job initiation has failed -> finish job
 * \param [in] JobData_pcst
 * Constant pointer to the data of the current NvM job
 */
static void NvM_Prv_MemIf_InitiateWrite(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    // MemIf job initiation has succeeded ->
    // poll MemIf job
    JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;
    *stJob_pen = NvM_Prv_stJob_PollMemIf_e;

#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
    // Write needed -> increment write counters and continue with job
    NvM_Prv_WriteMonCntrHandling(JobData_pcst->idBlock_uo,
                                 NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo),
                                 FALSE);
#endif

    // Start a MemIf_Write job
    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_VARIABLE_BLOCK_LENGTH))
    {
        // Write blocks where the block length is variable (defined in NvMRbNvBlockLengthString)
        // Migration feature must be active here
        if (E_OK != MemIf_Rb_VarLenWrite(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                                         NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) + JobData_pcst->idxDataset_u8,
                                         JobData_pcst->IntBuffer_st.Buffer_pu8,
                                         NvM_Prv_BlkDesc_GetBlockSizeStored(JobData_pcst->idBlock_uo)))
        {
            // Initiating write job has failed
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        }
    }
    else
    {
        // Write blocks where the length is defined as numeric literal in NvMNvBlockLength
        // Migration feature could be active here
        if (E_OK != MemIf_Write(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                                NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) + JobData_pcst->idxDataset_u8,
                                JobData_pcst->IntBuffer_st.Buffer_pu8))
        {
            // Initiating write job has failed
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        }
    }
}

static void NvM_Prv_MemIf_InitiateInvalidate(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst)
{
    // MemIf job initiation has succeeded -> Poll MemIf job
    JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;
    *stJob_pen = NvM_Prv_stJob_PollMemIf_e;

    // Start a MemIf_InvalidateBlock job
    if (E_OK != MemIf_InvalidateBlock(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                                      NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) + JobData_pcst->idxDataset_u8))
    {
        // Initiating invalidate job has failed
        JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
    }
}

static void NvM_Prv_MemIf_InitiateMaintain(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst)
{
    // MemIf job initiation has succeeded -> poll MemIf job
    JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;
    *stJob_pen = NvM_Prv_stJob_PollMemIf_e;

    // Start a MemIf_Rb_BlockMaintenance job
    if (E_OK != MemIf_Rb_BlockMaintenance(NvM_Prv_GetIdxDevice(JobData_pcst->idBlock_uo),
                                          NvM_Prv_GetIdBlockMemIf(JobData_pcst->idBlock_uo) +
                                                                  JobData_pcst->idxDataset_u8))
    {
        // Initiating maintain job has failed
        JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
    }

    // If maintenance feature is disabled then function MemIf_Rb_BlockMaintenance will be replaced by E_NOT_OK and
    // compiler generates a warning about "unused parameter".
    // Following line was added to avoid this warning
    (void)JobData_pcst;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

