
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static void NvM_Prv_JobValidate(NvM_Prv_stJob_ten* stJob_pen,
                                NvM_Prv_JobResult_tst* JobResult_pst,
                                NvM_Prv_JobData_tst const* JobData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

 /*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

NvM_Prv_Job_State_tpfct NvM_Prv_JobValidate_GetStateFct(NvM_Prv_stJob_ten* stJob_pen)
{
    NvM_Prv_Job_State_tpfct JobValidate_State_pfct = NULL_PTR;

    switch (*stJob_pen)
    {
        case NvM_Prv_stJob_Idle_e:
            JobValidate_State_pfct = NvM_Prv_JobValidate;
            // set job state to ease debugging and to avoid MISRA warning that stJob_pen can be a pointer to const
            *stJob_pen = NvM_Prv_stJobValidate_Start_e;
        break;

        default:
            JobValidate_State_pfct = NULL_PTR;
        break;
    }
    return JobValidate_State_pfct;
}

static void NvM_Prv_JobValidate(NvM_Prv_stJob_ten* stJob_pen,
                                NvM_Prv_JobResult_tst* JobResult_pst,
                                NvM_Prv_JobData_tst const* JobData_pcst)
{
    (void)JobData_pcst;
    *stJob_pen = NvM_Prv_stJob_Finished_e;
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

