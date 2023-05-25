
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_JobResource.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static void NvM_Prv_JobMaintain_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
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

NvM_Prv_Job_State_tpfct NvM_Prv_JobMaintain_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobMaintain_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
            JobMaintain_State_pfct = NvM_Prv_JobMaintain_DoMemIf;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJob_DoMemIf_e;
        break;

        case NvM_Prv_stJob_DoMemIf_e:
        case NvM_Prv_stJob_PollMemIf_e:
            JobMaintain_State_pfct = NvM_Prv_JobMaintain_DoMemIf;
        break;

        default:
            JobMaintain_State_pfct = NULL_PTR;
        break;
    }

    return JobMaintain_State_pfct;
}

static void NvM_Prv_JobMaintain_DoMemIf(NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_MemIf_e, stJob_pen, JobResult_pst, JobData_pcst);

    if (NvM_Prv_JobResult_Succeeded_e == JobResult_pst->Result_en)
    {
        // MemIf job has succeeded -> finish write job
        *stJob_pen = NvM_Prv_stJob_Finished_e;
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

