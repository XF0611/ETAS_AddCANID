
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_JobQueue.h"

/*
 **********************************************************************************************************************
 * Local declarations
 **********************************************************************************************************************
 */
static NvM_Prv_Job_StateMachine_tpfct NvM_Prv_Job_GetStateMachine(NvM_Prv_idJob_ten idJob_en);

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
#define NvM_Prv_Job_Idle_pfct                               NULL_PTR
#define NvM_Prv_Job_Read_pfct                               NvM_Prv_JobRead_GetStateFct
#define NvM_Prv_Job_Write_pfct                              NvM_Prv_JobWrite_GetStateFct
#define NvM_Prv_Job_Erase_pfct                              NvM_Prv_JobInvalidate_GetStateFct
#define NvM_Prv_Job_Restore_pfct                            NvM_Prv_JobRestore_GetStateFct
#define NvM_Prv_Job_Maintain_pfct                           NvM_Prv_JobMaintain_GetStateFct
#define NvM_Prv_Job_Validate_pfct                           NvM_Prv_JobValidate_GetStateFct
#define NvM_Prv_Job_Invalidate_pfct                         NvM_Prv_JobInvalidate_GetStateFct
#define NvM_Prv_Job_ReadIdConfigForReadAll_pfct             NvM_Prv_JobRead_GetStateFct
#define NvM_Prv_Job_InvalidateForFirstInitAll_pfct          NvM_Prv_JobInvalidate_GetStateFct
#define NvM_Prv_Job_RestoreForImplicitRecovery_pfct         NvM_Prv_JobRestore_GetStateFct
#define NvM_Prv_Job_InvalidateForRemoveNonResistant_pfct    NvM_Prv_JobInvalidate_GetStateFct
#define NvM_Prv_Job_RecalcRamBlkCrc_pfct                    NvM_Prv_JobRecalcRamBlkCrc_GetStateFct
#define NvM_Prv_Job_WriteAll_pfct                           NvM_Prv_JobWrite_GetStateFct
#define NvM_Prv_Job_Suspend_pfct                            NULL_PTR
#define NvM_Prv_Job_Invalid_pfct                            NULL_PTR

/**
 * Function-like macro: LIST_ENTRY_JOB_START_FCT
 *
 * This macro generates an entry for a start job function in a list of all jobs provided by NvM.
 * s.a. macro LIST_ID_JOB to generate a list for all jobs provided by NvM
 *
 * Paramater:
 * Name: name of the job for which a start job function will be generated.
 */
#define LIST_ENTRY_JOB_START_FCT(Name)      NvM_Prv_Job_##Name##_pfct,

#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

static NvM_Prv_Job_StateMachine_tpfct const NvM_Prv_Job_StateMachines_capfct[NvM_Prv_idJob_Count_e] =
{
        /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
        LIST_ID_JOB(LIST_ENTRY_JOB_START_FCT)
};

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static NvM_Prv_Job_StateMachine_tpfct NvM_Prv_Job_GetStateMachine(NvM_Prv_idJob_ten idJob_en)
{
    if (idJob_en >= NvM_Prv_idJob_Count_e)
    {
        idJob_en = NvM_Prv_idJob_Invalid_e;
    }
    return NvM_Prv_Job_StateMachines_capfct[idJob_en];
}

void NvM_Prv_MainFunctionJobStart(void)
{
    uint16 cntrJob_u16 = 0u;
    NvM_Prv_Job_tst* Job_pst;

    do
    {
        Job_pst = NvM_Prv_JobQueue_GetEntry(cntrJob_u16);
        ++cntrJob_u16;

        if ((NULL_PTR != Job_pst) && (NvM_Prv_stJob_Finished_e != Job_pst->stJob_en))
        {
            NvM_Prv_Job_StateMachine_tpfct StateMachine_pfct = NvM_Prv_Job_GetStateMachine(Job_pst->JobData_st.idJob_en);

            if (NULL_PTR != StateMachine_pfct)
            {
                NvM_Prv_Job_DoStateMachine(Job_pst, StateMachine_pfct);
            }
            else
            {
                Job_pst->JobResult_st.Result_en = NvM_Prv_JobResult_Failed_e;
                Job_pst->stJob_en = NvM_Prv_stJob_Finished_e;
            }
        }

    } while (NULL_PTR != Job_pst);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

