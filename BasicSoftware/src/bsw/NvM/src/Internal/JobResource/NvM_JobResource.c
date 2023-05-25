
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv_JobResource.h"
#include "NvM_Prv_Crc.h"
#include "NvM_Prv_MemIf.h"
#include "NvM_Prv_Crypto.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_JobResource_Lock(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                        NvM_Prv_JobData_tst const* JobData_pcst);

static void NvM_Prv_JobResource_Release(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                        NvM_Prv_JobData_tst const* JobData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

static NvM_Prv_Job_State_tpfct const NvM_Prv_JobResourceStateMachines_acpfct[NvM_Prv_idJobResource_NrJobResources_e] =
{
        NvM_Prv_MemIf_DoStateMachine,
        NvM_Prv_Crypto_DoStateMachine,
        NvM_Prv_Crc_Calculate
};

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/// Definition of the array for all internal jobs processed in parallel.
static NvM_Prv_JobData_tst const* NvM_Prv_JobResources_apcst[NvM_Prv_idJobResource_NrJobResources_e];

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

void NvM_Prv_JobResource_Init(void)
{
    NvM_Prv_idJobResource_tuo idJobResource_uo;
    for (idJobResource_uo = 0; idJobResource_uo < NvM_Prv_idJobResource_NrJobResources_e; ++idJobResource_uo)
    {
        NvM_Prv_JobResources_apcst[idJobResource_uo] = NULL_PTR;
    }
}

void NvM_Prv_JobResource_DoStateMachine(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                        NvM_Prv_stJob_ten* stJob_pen,
                                        NvM_Prv_JobResult_tst* JobResult_pst,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    JobResult_pst->Result_en = NvM_Prv_JobResult_Pending_e;

    if (NvM_Prv_JobResource_Lock(idJobResource_uo, JobData_pcst))
    {
        NvM_Prv_JobResourceStateMachines_acpfct[idJobResource_uo](stJob_pen, JobResult_pst, JobData_pcst);

        if (NvM_Prv_JobResult_Pending_e != JobResult_pst->Result_en)
        {
            NvM_Prv_JobResource_Release(idJobResource_uo, JobData_pcst);
        }
    }
    // If current job cannot lock the required job resource then it will remain pending
}

static boolean NvM_Prv_JobResource_Lock(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    boolean isLocked_b;
    if (NULL_PTR == NvM_Prv_JobResources_apcst[idJobResource_uo])
    {
        NvM_Prv_JobResources_apcst[idJobResource_uo] = JobData_pcst;
        isLocked_b = TRUE;
    }
    else if (JobData_pcst == NvM_Prv_JobResources_apcst[idJobResource_uo])
    {
        isLocked_b = TRUE;
    }
    else
    {
        isLocked_b = FALSE;
    }
    return isLocked_b;
}

static void NvM_Prv_JobResource_Release(NvM_Prv_idJobResource_tuo idJobResource_uo,
                                        NvM_Prv_JobData_tst const* JobData_pcst)
{
    if (JobData_pcst == NvM_Prv_JobResources_apcst[idJobResource_uo])
    {
        NvM_Prv_JobResources_apcst[idJobResource_uo] = NULL_PTR;
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

