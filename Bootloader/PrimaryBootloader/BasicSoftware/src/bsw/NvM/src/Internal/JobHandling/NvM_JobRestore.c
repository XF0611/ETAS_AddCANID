
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "rba_MemLib.h"

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

static void NvM_Prv_JobRestore_Start(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRestore_CopyFromRom(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRestore_ExplicitSync(NvM_Prv_stJob_ten* stJob_pen,
                                            NvM_Prv_JobResult_tst* JobResult_pst,
                                            NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRestore_InitCallback(NvM_Prv_stJob_ten* stJob_pen,
                                            NvM_Prv_JobResult_tst* JobResult_pst,
                                            NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRestore_Crc(NvM_Prv_stJob_ten* stJob_pen,
                                   NvM_Prv_JobResult_tst* JobResult_pst,
                                   NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRestore_StartWrite(NvM_Prv_stJob_ten* stJob_pen,
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

NvM_Prv_Job_State_tpfct NvM_Prv_JobRestore_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobRestore_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
        case NvM_Prv_stJobRestore_Start_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_Start;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobRestore_Start_e;
        break;

        case NvM_Prv_stJobRestore_CopyFromRom_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_CopyFromRom;
        break;

        case NvM_Prv_stJobRestore_ExplicitSync_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_ExplicitSync;
        break;

        case NvM_Prv_stJobRestore_InitCallback_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_InitCallback;
        break;

        case NvM_Prv_stJobRestore_Crc_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_Crc;
        break;

        case NvM_Prv_stJobRestore_StartWrite_e:
            JobRestore_State_pfct = NvM_Prv_JobRestore_StartWrite;
        break;

        default:
            JobRestore_State_pfct = NULL_PTR;
        break;
    }
    return JobRestore_State_pfct;
}

static void NvM_Prv_JobRestore_Start(NvM_Prv_stJob_ten* stJob_pen,
                                     NvM_Prv_JobResult_tst* JobResult_pst,
                                     NvM_Prv_JobData_tst const* JobData_pcst)
{
    // NvM starts the restore job only if default data is available
    // -> so it is not necessary to check availability of default data here

    *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);

    // TRACE[NVM392] Restore job is done without any MemIf interaction
    // TRACE[NVM392] Leave the NV block untouched
    // Determine the source of the block defaults
    if (NULL_PTR != NvM_Prv_GetRomBlockAddress(JobData_pcst->idBlock_uo))
    {
        // A ROM block is configured for this block
        // TRACE[NVM267] Fill the block data buffer with ROM block data if configured
        // TRACE[SWS_NvM_00816] Fill the block data buffer with ROM block data if configured
        *stJob_pen = NvM_Prv_stJobRestore_CopyFromRom_e;
    }
    else
    {
        // An initialization callback is configured for this block
        *stJob_pen = NvM_Prv_stJobRestore_InitCallback_e;
    }
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
}

static void NvM_Prv_JobRestore_CopyFromRom(NvM_Prv_stJob_ten* stJob_pen,
                                           NvM_Prv_JobResult_tst* JobResult_pst,
                                           NvM_Prv_JobData_tst const* JobData_pcst)
{
    // A ROM block is configured for this block
    // TRACE[NVM267] Fill the block data buffer with ROM block data
    // TRACE[SWS_NvM_00816] Fill the block data buffer with ROM block data
    uint8* adrDestination_pu8;
    uint32 BlockSize_u32 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
    uint8 nrNonVolatileBlocks_u8 = NvM_Prv_GetNrNonVolatileBlocks(JobData_pcst->idBlock_uo);
    uint8 const* adrSource_pcu8;
    NvM_Prv_ptrRomBlock_tun ptrRomBlock_un;
    ptrRomBlock_un.ptrRomBlock_pcv = NvM_Prv_GetRomBlockAddress(JobData_pcst->idBlock_uo);
    adrSource_pcu8 = ptrRomBlock_un.ptrRomBlock_pcu8;

    if (NVM_BLOCK_DATASET == NvM_Prv_GetBlockType(JobData_pcst->idBlock_uo))
    {
        // This DATASET block has its dataset-index pointing to a ROM block
        // (otherwise it wouldn't have been queued by NvM_RestoreBlockDefaults or by NvM_ReadBlock)
        // -> Move the source pointer to the start address of the indexed ROM block
        adrSource_pcu8 += BlockSize_u32 * (JobData_pcst->idxDataset_u8 - nrNonVolatileBlocks_u8);
    }

    if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM)
        && JobData_pcst->isPRamBlockUsed_b)
    {
        adrDestination_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
        // After copying data from ROM data NvM starts explicit synchronization if configured
        *stJob_pen = NvM_Prv_stJobRestore_ExplicitSync_e;
    }
    else
    {
        adrDestination_pu8 = JobData_pcst->UserData_un.ptrRamBlock_pu8;
        // After copying data from ROM data NvM calls initialization callback if configured
        *stJob_pen = NvM_Prv_stJobRestore_InitCallback_e;
    }

    // Perform the restore job by copying data from the ROM-block to the RAM-block
    rba_MemLib_MemCopy(adrSource_pcu8, adrDestination_pu8, BlockSize_u32);

    // Copying data from ROM is always done immediately and cannot fail
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
}

static void NvM_Prv_JobRestore_ExplicitSync(NvM_Prv_stJob_ten* stJob_pen,
                                            NvM_Prv_JobResult_tst* JobResult_pst,
                                            NvM_Prv_JobData_tst const* JobData_pcst)
{
    // trigger the read callback for explicit synchronization
    JobResult_pst->Result_en = NvM_Prv_ExplicitSync_CopyData(NvM_Prv_GetCopyFctForRead(JobData_pcst->idBlock_uo),
                                                             JobData_pcst->idBlock_uo,
                                                             &JobData_pcst->cntrExpSyncOperations_u8,
                                                             JobData_pcst->IntBuffer_st.Buffer_pu8);

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // Copying data for explicit synchronization has succeeded ->
        // call initialization callback before starting CRC recalculation, do not update final job result
        // An initialization callback is configured for this block
        *stJob_pen = NvM_Prv_stJobRestore_InitCallback_e;
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

static void NvM_Prv_JobRestore_InitCallback(NvM_Prv_stJob_ten* stJob_pen,
                                            NvM_Prv_JobResult_tst* JobResult_pst,
                                            NvM_Prv_JobData_tst const* JobData_pcst)
{
    // Next step is CRC recalculation,
    // only if initialization callback has failed NvM finishs restore job
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
    {
        // TRACE[SWS_NvM_00821] Restore job for NvM_RestorePRAMBlockDefaults shall recalculate CRC over the RAM block
        //                      after the copy process if configured
        *stJob_pen = NvM_Prv_stJobRestore_Crc_e;

        // Prepare CRC recalculation over the user data
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
        // Buffer with user data depends on block configuration
        if (   NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM)
            && JobData_pcst->isPRamBlockUsed_b)
        {
            JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
        }
        else
        {
            JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->UserData_un.ptrRamBlock_pu8;
        }
    }
    else
    {
        // No CRC calculation needed -> go to next state, do not update final result
        *stJob_pen = NvM_Prv_stJobRestore_StartWrite_e;
    }

    // InitBlockCallback shall be called only if available and only for non-auxiliary services
    if (!JobData_pcst->isAuxServiceActive_b)
    {
        // Invoke the init block callback
        Std_ReturnType stInitCallback_uo = NvM_Prv_InvokeInitBlockCallback(JobData_pcst->idBlock_uo);
        if (E_OK != stInitCallback_uo)
        {
            // If init callback has failed finish restore job
            *stJob_pen = NvM_Prv_stJob_Finished_e;
            JobResult_pst->Result_en = NvM_Prv_JobResult_Failed_e;
        }
    }
    // Copying data via initialization callback is always done immediately
}

static void NvM_Prv_JobRestore_Crc(NvM_Prv_stJob_ten* stJob_pen,
                                   NvM_Prv_JobResult_tst* JobResult_pst,
                                   NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
    *stJob_pen = NvM_Prv_stJobRestore_Crc_e;

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // Copy CRC over the user data for later usage
        JobResult_pst->CrcData_st.CrcRamBlk_un.Crc32_u32 = JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32;

        if (JobData_pcst->isPRamBlockUsed_b)
        {
            // Set calculated CRC if permanent RAM block is used
            NvM_Prv_Crc_SetRamBlockCrc(JobData_pcst->idBlock_uo, &JobResult_pst->CrcData_st.Calculation_st.Crc_un);
        }

        // CRC calculation finished -> go to next state, do not update final result
        *stJob_pen = NvM_Prv_stJobRestore_StartWrite_e;
    }
}

static void NvM_Prv_JobRestore_StartWrite(NvM_Prv_stJob_ten* stJob_pen,
                                          NvM_Prv_JobResult_tst* JobResult_pst,
                                          NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    // NvM reaches this step only if restore job has succeeded, i.e.
    // - the copy operation from ROM to RAM was successful or
    // - the Init-Callback was successful
    // - and crc handling was successful
    // Notes:
    // - A finally failed explicit sync read callback sets the job result to MEMIF_JOB_FAILED,
    //   so we neither trigger an init block callback nor a write job in this case
    // - The init block callback doesn't need to be prohibited for jobs triggered by an auxiliary user here
    //   because we only come here if the current service is FirstInitAll
    if (NVM_SERVICE_ID_RB_FIRST_INIT_ALL == JobData_pcst->idService_uo)
    {

        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2834] If this restore job was part of a first initialization,
        //                                               trigger a write job
        NvM_Prv_Job_Start(stJob_pen,
                          &JobResult_pst->Result_en,
                          JobData_pcst,
                          NvM_Prv_idJob_Write_e,
                          JobData_pcst->idxDataset_u8);
    }
    else
    {

        // Finish restore job after starting write job
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

