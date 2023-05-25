
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_JobQueue.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_InternalBuffer.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_ProcessMultiBlock.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE uint8 NvM_Prv_Job_GetIdxDataset(NvM_Prv_idJob_ten idJob_en,
                                             NvM_BlockIdType idBlock_uo,
                                             boolean isAuxServiceActive_b);

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

void NvM_Prv_Job_StartRecalcRamBlkCrc(NvM_BlockIdType idBlock_uo)
{
    NvM_Prv_Job_tst Job_st;

    Job_st.stJob_en = NvM_Prv_stJob_Idle_e;
    Job_st.JobResult_st.Result_en = NvM_Prv_JobResult_Pending_e;
    Job_st.JobResult_st.ProductionError_u8 = 0u;
    Job_st.JobResult_st.CrcData_st.CrcRamBlk_un.Crc32_u32 = 0u;
    Job_st.JobResult_st.CrcData_st.Calculation_st.isFirstCall_b = FALSE;
    Job_st.JobResult_st.CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;

    Job_st.JobData_st.idJob_en = NvM_Prv_idJob_RecalcRamBlkCrc_e;
    Job_st.JobData_st.idQueue_uo = NvM_Prv_idQueue_nrQueues_e;
    Job_st.JobData_st.idService_uo = NVM_SERVICE_ID_INIT;
    Job_st.JobData_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    Job_st.JobData_st.idBlock_uo = idBlock_uo;
    Job_st.JobData_st.idxDataset_u8 = 0u;
    Job_st.JobData_st.isMultiServiceActive_b = FALSE;
    Job_st.JobData_st.isAuxServiceActive_b = FALSE;
    Job_st.JobData_st.isPRamBlockUsed_b = TRUE;
    Job_st.JobData_st.isIntBufferToUse_b = NvM_Prv_IsBlockSelected(Job_st.JobData_st.idBlock_uo,
                                                                   NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM);
    // For RAM Block CRC job encryption is not relevant but setting isEncryptionEnabled_b to FALSE ensures that
    // the offset to plain data in the buffer is set to 0.
    Job_st.JobData_st.isEncryptionEnabled_b = FALSE;
    Job_st.JobData_st.cntrExpSyncOperations_u8 = 0u;
    Job_st.JobData_st.UserData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(idBlock_uo);

    NvM_Prv_JobQueue_Enqueue(&Job_st);
}

/**
 * \brief
 * This private function prepares a internal job before starting.
 * \details
 * This function fills the structure required for job execution (s. NvM_Prv_Job_tst) and starts the job.
 * After starting the job this function enqueues it into the job queue.
 *
 * \param[inout] idJob_en
 * ID of the job to be prepared and started
 * \param[in] Request_pcst
 * Pointer to the structure containing user request data
 * \param[in] idReqQueue_uo
 * ID of the request queue providing user request data
 */
void NvM_Prv_Job_Prepare(NvM_Prv_idJob_ten idJob_en,
                         NvM_Prv_QueueEntry_tst* Request_pst,
                         NvM_Prv_idQueue_tuo idReqQueue_uo)
{
    NvM_Prv_Job_tst Job_st;

    Job_st.stJob_en = NvM_Prv_stJob_Idle_e;
    Job_st.JobResult_st.Result_en = NvM_Prv_JobResult_Pending_e;
    Job_st.JobResult_st.ProductionError_u8 = 0u;
    Job_st.JobResult_st.CrcData_st.CrcRamBlk_un.Crc32_u32 = 0u;
    Job_st.JobResult_st.CrcData_st.Calculation_st.isFirstCall_b = FALSE;
    Job_st.JobResult_st.CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;

    Job_st.JobData_st.idQueue_uo = idReqQueue_uo;
    Job_st.JobData_st.idService_uo = Request_pst->idService_uo;
    Job_st.JobData_st.ServiceBit_uo = Request_pst->ServiceBit_uo;
    Job_st.JobData_st.idBlock_uo = Request_pst->idBlock_uo;
    Job_st.JobData_st.UserData_un.ptrRamBlock_pv = Request_pst->BlockData_un.ptrRamBlock_pv;

    Job_st.JobData_st.isMultiServiceActive_b = (NvM_Prv_idQueue_Multi_e == idReqQueue_uo);
    Job_st.JobData_st.isAuxServiceActive_b = FALSE;
#if (NVM_RB_AUX_INTERFACE == STD_ON)
    Job_st.JobData_st.isAuxServiceActive_b = (NvM_Prv_idQueue_Aux_e == idReqQueue_uo);
#endif

    // TRACE[NVM338] The job of NvM_WriteBlock writes to the NV block which is selected by the data index
    //               when the block is of type DATASET
    // TRACE[NVM339] Same as NVM338, applied to NvM_WriteAll
    // TRACE[NVM340] The job of NvM_ReadBlock reads only the NV block selected by the data index
    //               for a block of type DATASET
    // TRACE[SWS_NvM_00802] The job of NvM_WritePRAMBlock writes to the NV block which is selected by the data index
    //                      when the block is of type DATASET
    // (Fee/Ea block-ID for an NV-block of an NvM-block of type DATASET) = (Fee/Ea block-ID) + (NV-index)
    Job_st.JobData_st.idxDataset_u8 = NvM_Prv_Job_GetIdxDataset(idJob_en,
                                                                Request_pst->idBlock_uo,
                                                                Job_st.JobData_st.isAuxServiceActive_b);

    Job_st.JobData_st.isPRamBlockUsed_b =
            (Job_st.JobData_st.UserData_un.ptrRamBlock_pv == NvM_Prv_GetPRamBlockAddress(Job_st.JobData_st.idBlock_uo));

    // A cryptographic service or NV CRC or block write counter or explicit synchronization is used
    // for current block
    // -> NvM will use internal buffer for current job (NvM copies user data into/from the internal buffer)
    // Neither a cryptographic service nor NV CRC nor block write counter nor explicit synchronization is used
    // for current job
    // -> NvM uses user data directly for current job (NvM avoids copying user data)
    Job_st.JobData_st.isIntBufferToUse_b = (   NvM_Prv_IsBlockSelected(Job_st.JobData_st.idBlock_uo,
                                                                       NVM_PRV_BLOCK_FLAG_USE_CRYPTO)
                                            || NvM_Prv_IsBlockSelected(Job_st.JobData_st.idBlock_uo,
                                                                       NVM_PRV_BLOCK_FLAG_USE_NV_CRC)
                                            || (NvM_Prv_IsBlockSelected(Job_st.JobData_st.idBlock_uo,
                                                                        NVM_PRV_BLOCK_FLAG_CNTR_WRITE))
                                            || (   NvM_Prv_IsBlockSelected(Job_st.JobData_st.idBlock_uo,
                                                                           NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM)
                                                && Job_st.JobData_st.isPRamBlockUsed_b
                                               )
                                           );

    Job_st.JobData_st.isEncryptionEnabled_b = FALSE;
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
    Job_st.JobData_st.isEncryptionEnabled_b = (0u != NvM_Prv_BlkDesc_GetLengthJobCsm(Request_pst->idBlock_uo,
                                                                                     NvM_Prv_Crypto_idService_Encrypt_e));
#endif

    Job_st.JobData_st.cntrExpSyncOperations_u8 = 0u;

    NvM_Prv_Job_Start(&Job_st.stJob_en,
                      &Job_st.JobResult_st.Result_en,
                      &Job_st.JobData_st,
                      idJob_en,
                      Job_st.JobData_st.idxDataset_u8);

    NvM_Prv_JobQueue_Enqueue(&Job_st);

    if (Job_st.JobData_st.isMultiServiceActive_b)
    {
        NvM_Prv_Multi_GoToNextBlock(Request_pst);
    }
}

/**
 * \brief
 * This private function starts an internal job.
 * \details
 * To start an internal job this function updates job data due to passed parameters, checks whether the job ID is valid
 * and invokes start callback if configured and required.
 * For invalid job IDs this function sets the job result to failed, i.e. the NvM does not execute this job but only
 * evaluates job result.
 * This function invokes start callback only if configured (s. NvM_Prv_InvokeSingleBlockStartCallback) and
 * for non-auxiliary services.
 *
 * \param[inout] stJob_pen
 * Pointer to the current job state, all jobs start with idle state
 * \param[inout] Result_pen
 * Pointer to the current job, all valid jobs start with pending result
 * \param[inout] JobData_pcst
 * Pointer to the structure containing job data to be updated, only job ID and dataset index can be updated
 * \param[in] idJob_en
 * ID of the job to be started
 * \param[in] idxDataset_u8
 * Dataset index to be used in started job
 */
void NvM_Prv_Job_Start(NvM_Prv_stJob_ten* stJob_pen,
                       NvM_Prv_JobResult_ten* Result_pen,
                       NvM_Prv_JobData_tst const* JobData_pcst,
                       NvM_Prv_idJob_ten idJob_en,
                       uint8 idxDataset_u8)
{
    // The following union is necessary to remove an otherwise unavoidable compiler warning
    // due to a discarded const qualifier
    union
    {
        NvM_Prv_JobData_tst const* JobData_pcst;
        NvM_Prv_JobData_tst* JobData_pst;
    } JobData_un;
    JobData_un.JobData_pcst = JobData_pcst;

    JobData_un.JobData_pst->idJob_en = idJob_en;
    JobData_un.JobData_pst->idxDataset_u8 = idxDataset_u8;

    *stJob_pen = NvM_Prv_stJob_Idle_e;

    if (NvM_Prv_ErrorDetection_IsJobIdValid(NVM_SERVICE_ID_MAIN_FUNCTION,
                                            JobData_pcst->idBlock_uo,
                                            idJob_en))
    {
        *Result_pen = NvM_Prv_JobResult_Pending_e;

        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3055] Invoke block-start callback function if configured
        // SingleBlockStartCallback shall be called only if available and only for non-auxiliary services
        if (!JobData_pcst->isAuxServiceActive_b)
        {
            NvM_Prv_InvokeSingleBlockStartCallback(JobData_pcst->idBlock_uo, JobData_pcst->idService_uo);
        }
    }
    else
    {
        // This function may only be called with a known NvM_Prv_Main_st.idActiveJob_rMwM_en
        // (we should actually never get into this branch; it's just there for robustness)
        *Result_pen = NvM_Prv_JobResult_Failed_e;
    }
}

void NvM_Prv_Job_DoStateMachine(NvM_Prv_Job_tst* Job_pst,
                                NvM_Prv_Job_StateMachine_tpfct StateMachine_pfct)
{
    do
    {
        NvM_Prv_Job_State_tpfct State_pfct = StateMachine_pfct(&Job_pst->stJob_en);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    TestCd_NvM_JobMonitor_SetState(&Job_pst->JobData_st, Job_pst->stJob_en);
#endif

        if (NULL_PTR != State_pfct)
        {
            State_pfct(&Job_pst->stJob_en, &Job_pst->JobResult_st, &Job_pst->JobData_st);
        }
        else
        {
            Job_pst->JobResult_st.Result_en = NvM_Prv_JobResult_Failed_e;
            Job_pst->stJob_en = NvM_Prv_stJob_Finished_e;
        }

    } while((NvM_Prv_stJob_Finished_e != Job_pst->stJob_en) &&
            (NvM_Prv_JobResult_Pending_e != Job_pst->JobResult_st.Result_en));
}

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

NvM_Prv_stJob_ten NvM_Prv_Job_GetState(void)
{
    NvM_Prv_stJob_ten stJob_en = NvM_Prv_stJob_Idle_e;
    NvM_Prv_Job_tst* Job_pst = NvM_Prv_JobQueue_GetEntry(0u);

    if (NULL_PTR != Job_pst)
    {
        stJob_en = Job_pst->stJob_en;
    }

    return stJob_en;
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
/**
 * \brief
 * This local private function returns the dataset index to be used in current job.
 * \details
 * For non-dataset blocks the NvM always uses the dataset index 0.
 *
 * If the NvM removes blocks configured as non-resistant to layout change the NvM also uses the dataset index 0.
 *
 * For dataset blocks the NvM always uses the dataset index set by the user via NvM_SetDataIndex.
 * \attention
 * If current job is part of an auxiliary service then the NvM uses the dataset index set by the user
 * via NvM_Rb_AuxSetDataIndex, in this case to get the dataset index the NvM must use the block ID
 * NVM_PRV_AUX_ADMIN_BLOCK.
 *
 */
LOCAL_INLINE uint8 NvM_Prv_Job_GetIdxDataset(NvM_Prv_idJob_ten idJob_en,
                                             NvM_BlockIdType idBlock_uo,
                                             boolean isAuxServiceActive_b)
{
    uint8 idxDataset_u8;
    if (   (NVM_BLOCK_DATASET != NvM_Prv_GetBlockType(idBlock_uo))
        || (NvM_Prv_idJob_InvalidateForRemoveNonResistant_e == idJob_en)
       )
    {
        idxDataset_u8 = 0;
    }
    else
    {
#if (NVM_RB_AUX_INTERFACE == STD_ON)
        if (isAuxServiceActive_b)
        {
            idBlock_uo = NVM_PRV_AUX_ADMIN_BLOCK;
        }
#else
        (void)isAuxServiceActive_b;
#endif
        idxDataset_u8 = NvM_Prv_Block_GetIdxDataset(idBlock_uo);
    }
    return idxDataset_u8;
}

