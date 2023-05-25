
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_JobResource.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static void NvM_Prv_JobInvalidate_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
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

NvM_Prv_Job_State_tpfct NvM_Prv_JobInvalidate_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobInvalidate_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
            JobInvalidate_State_pfct = NvM_Prv_JobInvalidate_DoMemIf;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        break;

        case NvM_Prv_stJob_DoMemIf_e:
        case NvM_Prv_stJob_PollMemIf_e:
            JobInvalidate_State_pfct = NvM_Prv_JobInvalidate_DoMemIf;
        break;

        default:
            JobInvalidate_State_pfct = NULL_PTR;
        break;

    }

    return JobInvalidate_State_pfct;
}

static void NvM_Prv_JobInvalidate_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
                                          NvM_Prv_JobResult_tst* JobResult_pst,
                                          NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_MemIf_e, stJob_pen, JobResult_pst, JobData_pcst);

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // MemIf job has succeeded -> finish invalidate job
        *stJob_pen = NvM_Prv_stJob_Finished_e;

        // Check whether the next DATASET block needs to be invalidated
        // -> if so, advance the current dataset index to the next DATASET instance and start another invalidation
        if ((NvM_Prv_idJob_InvalidateForFirstInitAll_e == JobData_pcst->idJob_en) ||
            (NvM_Prv_idJob_InvalidateForRemoveNonResistant_e == JobData_pcst->idJob_en))
        {
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2850] Invalidate all the NV blocks of an NvM block
            //                                               of type DATASET which is selected for FirstInitAll
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2852] Remove all the NV blocks of a non-resistant NvM block
            //                                               of type DATASET during the removal phase
            if ((NVM_BLOCK_DATASET == NvM_Prv_GetBlockType(JobData_pcst->idBlock_uo)) &&
                (JobData_pcst->idxDataset_u8 < (NvM_Prv_GetNrNonVolatileBlocks(JobData_pcst->idBlock_uo) - 1)))
            {
                NvM_Prv_Job_Start(stJob_pen,
                                  &JobResult_pst->Result_en,
                                  JobData_pcst,
                                  JobData_pcst->idJob_en,
                                  JobData_pcst->idxDataset_u8 + 1);
            }
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

