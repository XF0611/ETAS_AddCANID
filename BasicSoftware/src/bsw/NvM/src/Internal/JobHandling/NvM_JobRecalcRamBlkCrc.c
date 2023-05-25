
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv_Job.h"
#include "NvM_Prv_Crc.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ExplicitSynchronization.h"
#include "NvM_Prv_JobResource.h"


/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static void NvM_Prv_JobRecalcRamBlkCrc_ExplicitSyncWriteToNvM(NvM_Prv_stJob_ten* stJob_pen,
                                                              NvM_Prv_JobResult_tst* JobResult_pst,
                                                              NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobRecalcRamBlkCrc_DoCrc(NvM_Prv_stJob_ten* stJob_pen,
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

NvM_Prv_Job_State_tpfct NvM_Prv_JobRecalcRamBlkCrc_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobRecalcRamBlkCrc_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
        case NvM_Prv_stRecalcRamBlkCrc_ExplicitSyncWriteToNvM_e:
            JobRecalcRamBlkCrc_State_pfct = NvM_Prv_JobRecalcRamBlkCrc_ExplicitSyncWriteToNvM;
            *stJob_pen = NvM_Prv_stRecalcRamBlkCrc_ExplicitSyncWriteToNvM_e;
        break;

        case NvM_Prv_stRecalcRamBlkCrc_Do_e:
            JobRecalcRamBlkCrc_State_pfct = NvM_Prv_JobRecalcRamBlkCrc_DoCrc;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stRecalcRamBlkCrc_Do_e;
        break;

        default:
            JobRecalcRamBlkCrc_State_pfct = NULL_PTR;
        break;
    }

    return JobRecalcRamBlkCrc_State_pfct;
}


static void NvM_Prv_JobRecalcRamBlkCrc_ExplicitSyncWriteToNvM(NvM_Prv_stJob_ten* stJob_pen,
                                                              NvM_Prv_JobResult_tst* JobResult_pst,
                                                              NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

#if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    if (NvM_Prv_IsBlockSelected(JobData_pcst->idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM))
    {
        // TRACE[SWS_NvM_00362] Before NvM can check the RAM block CRC for blocks with configured explicit
        //                      synchronization the RAM block data must be copied into the NvM RAM mirror
        //                      via write callback,
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
        *stJob_pen = NvM_Prv_stRecalcRamBlkCrc_Do_e;

        // Prepare CRC recalculation over the user data
        JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = TRUE;
        JobResult_pst->CrcData_st.Calculation_st.Length_u16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
        JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32 = 0u;
        JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8 = JobData_pcst->IntBuffer_st.Buffer_pu8;
    }
    else if (NvM_Prv_JobResult_Failed_e == JobResult_pst->Result_en)
    {
        // If copying data for explicit synchronization has failed then no CRC check can be done
        // -> Signal that it failed so that in NvM_Prv_UpdateBlockStatus_RecalcRamBlkCrc()
        // the further processing can be done the same way as if data has been changed.
        *stJob_pen = NvM_Prv_stJob_Finished_e;
    }
    else
    {
        // If copying data for explicit synchronization is still pending do nothing
    }

#else
    (void)JobData_pcst;
    *stJob_pen = NvM_Prv_stJob_Finished_e;
#endif
}


static void NvM_Prv_JobRecalcRamBlkCrc_DoCrc(NvM_Prv_stJob_ten* stJob_pen,
                                             NvM_Prv_JobResult_tst* JobResult_pst,
                                             NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

#if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    *JobData_pcst->IntBuffer_st.UsedSizeInBytes_pu16 = NvM_Prv_GetBlockSize(JobData_pcst->idBlock_uo);
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_Crc_e, stJob_pen, JobResult_pst, JobData_pcst);
    *stJob_pen = NvM_Prv_stRecalcRamBlkCrc_Do_e;

    if (NvM_Prv_JobResult_Pending_e != JobResult_pst->Result_en)
    {
        *stJob_pen = NvM_Prv_stJob_Finished_e;

        // Copy CRC over the user data for later usage
        JobResult_pst->CrcData_st.CrcRamBlk_un.Crc32_u32 = JobResult_pst->CrcData_st.Calculation_st.Crc_un.Crc32_u32;
    }

#else
    (void)JobData_pcst;
    *stJob_pen = NvM_Prv_stJob_Finished_e;
#endif
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

