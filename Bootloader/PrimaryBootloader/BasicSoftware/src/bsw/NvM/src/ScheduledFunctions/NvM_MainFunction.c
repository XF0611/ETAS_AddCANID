
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Prv_HideRteApi.h"

#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_ErrorDetection.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
#include "NvM_Prv_JobQueue.h"
#endif
/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
/**
 * Function-like macro: LIST_ENTRY_ID_JOB_FOR_SERVICE_BIT
 *
 * This macro generates an entry for a job ID in a list of all service bits provided by NvM.
 * s.a. macro LIST_SERVICE_BIT to generate a list for all service bits provided by NvM
 *
 * Paramater:
 * Unused: unused
 * JobName: name of the job for which a job ID will be generated.
 */
#define LIST_ENTRY_ID_JOB_FOR_SERVICE_BIT(Unused, JobName)     NvM_Prv_idJob_##JobName##_e,

/**
 *  Protection mechanism configurable via NvM_Cfg_SchM.h, enabled by default
 *
 *  TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3265]
 *  The re-entrant protection mechanism shall be provided as configurable option
 */
#ifndef NVM_RB_USE_PROTECTION
#define NVM_RB_USE_PROTECTION   (TRUE)
#endif


#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

static NvM_Prv_idJob_ten const NvM_Prv_MapServiceBitToIdJob_caen[NvM_Prv_ServiceBit_nr_e] =
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_SERVICE_BIT(LIST_ENTRY_ID_JOB_FOR_SERVICE_BIT)
};

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

/*
 * Flag to hold the main function access status
 */
static volatile boolean NvM_Prv_flgUsedSema_b;

#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/**
 * Global NvM state information which is not block-specific.
 */
NvM_Prv_MainStates_tst NvM_Prv_Main_st;

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"


#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/*
 **********************************************************************************************************************
 * Static Declaration
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_MainFunctionIntern(void);
LOCAL_INLINE boolean NvM_Prv_TryToGetLock(void);
LOCAL_INLINE void NvM_Prv_ReleaseLock(void);

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
void NvM_Prv_InitializeMainStates(void)
{
    // Preset NvM state to Idle
    NvM_Prv_Main_st.Activity_rAMwM_en = NVM_PRV_ACTIVITY_IDLE;
    // Preset ID of the active queue to a value which signals that no queue is active
    NvM_Prv_Main_st.idQueueActive_uo = NvM_Prv_idQueue_nrQueues_e;
    // Preset ID of the active service to a value which signals that no service is active
    NvM_Prv_Main_st.idServiceActive_uo = NVM_SERVICE_ID_INIT;
}

void NvM_Prv_UninitializeMainStates(void)
{
    // Preset NvM state to NotInit to make sure that NvM will not accept any requests
    NvM_Prv_Main_st.Activity_rAMwM_en = NVM_PRV_ACTIVITY_NOT_INIT;
    // Preset ID of the active queue to a value which signals that no queue is active
    NvM_Prv_Main_st.idQueueActive_uo = NvM_Prv_idQueue_nrQueues_e;
    // Preset ID of the active service to a value which signals that no service is active
    NvM_Prv_Main_st.idServiceActive_uo = NVM_SERVICE_ID_INIT;
}

boolean NvM_Prv_IsNvmInitialized(void)
{
    return (NVM_PRV_ACTIVITY_NOT_INIT != NvM_Prv_Main_st.Activity_rAMwM_en);
}

/**
 * \brief
 * This NvM private function returns whether the NvM is currently idle.
 *
 * \detailed
 * The state idle means that NvM is neither processing user requests nor executing internal tasks
 * like RAM block CRC re-calculation.
 *
 * \return
 * - TRUE = NvM is idle
 * - FALSE = NvM is busy
 */
boolean NvM_Prv_IsNvmIdle(void)
{
    return (NVM_PRV_ACTIVITY_IDLE == NvM_Prv_Main_st.Activity_rAMwM_en);
}

/**
 * \brief
 * This NvM private function returns whether the recalculation of RAM block CRCs is currently ongoing.
 *
 * \return
 * - TRUE = NvM is re-calculating RAM block CRCs
 * - FALSE = NvM is not re-calculating RAM block CRCs
 */
boolean NvM_Prv_IsCrcRecalcActive(void)
{
    return (NVM_PRV_ACTIVITY_RAM_BLOCK_CRC == NvM_Prv_Main_st.Activity_rAMwM_en);
}

boolean NvM_Prv_IsMultiActive(void)
{
    return (NvM_Prv_idQueue_Multi_e == NvM_Prv_Main_st.idQueueActive_uo);
}

NvM_Prv_idService_tuo NvM_Prv_GetActiveService(void)
{
    return NvM_Prv_Main_st.idServiceActive_uo;
}

NvM_Prv_idQueue_tuo NvM_Prv_GetActiveQueue(void)
{
    return NvM_Prv_Main_st.idQueueActive_uo;
}

void NvM_Prv_UpdateActiveServiceData(NvM_Prv_idService_tuo idService_uo, NvM_Prv_idQueue_tuo idReqQueue_uo)
{
    NvM_Prv_Main_st.idServiceActive_uo = idService_uo;
    NvM_Prv_Main_st.idQueueActive_uo = idReqQueue_uo;
}

NvM_Prv_idJob_ten NvM_Prv_GetJobId(NvM_Prv_ServiceBit_tuo ServiceBit_uo)
{
    if (ServiceBit_uo >= NvM_Prv_ServiceBit_nr_e)
    {
        ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    }

    return NvM_Prv_MapServiceBitToIdJob_caen[ServiceBit_uo];
}

/**
 * \brief
 * The internal main function does the processing of NvM jobs.
 *
 */
LOCAL_INLINE void NvM_Prv_MainFunctionIntern(void)
{
    // TRACE[NVM464] Service for performing the processing of the NvM jobs
    // TRACE[NVM256] Same as NVM464
    // TRACE[NVM324] No assumption is made about the invocation timing relative to other BSW main processing functions

    // TRACE[NVM257] Check for proper initialization before doing any real job processing
    // Note: This is done in a strict way here, thereby trying to catch general RAM corruption e.g. by wild pointers
    if ((NvM_Prv_Main_st.Activity_rAMwM_en != NVM_PRV_ACTIVITY_NOT_INIT) &&
        (NvM_Prv_Main_st.Activity_rAMwM_en != NVM_PRV_ACTIVITY_IDLE) &&
        (NvM_Prv_Main_st.Activity_rAMwM_en != NVM_PRV_ACTIVITY_BUSY) &&
        (NvM_Prv_Main_st.Activity_rAMwM_en != NVM_PRV_ACTIVITY_RAM_BLOCK_CRC))
    {
        // Disallow any further job processing
        NvM_Prv_Main_st.Activity_rAMwM_en = NVM_PRV_ACTIVITY_NOT_INIT;
    }

    // If NvM is not initialized
    // -> refuse to do anything, report error to Det if this functionality is enabled
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_MAIN_FUNCTION, 0u))
    {
        if (NvM_Prv_MainFunctionArbitrate(&NvM_Prv_Main_st))
        {
            NvM_Prv_MainFunctionJobStart();
            NvM_Prv_MainFunctionResultEval(&NvM_Prv_Main_st);
        }
        // TRACE[NVM349] If no further job processing is possible, immediately return
    }
}

/*
 * \brief The function check the global flag for re-entrant invocation and reports the re-entrant error if detected
 *
 * \return
 * - TRUE = Lock available and accquired, proceed for execution
 * - FALSE = Lock not available, prevent re-entrant execution
 */
LOCAL_INLINE boolean NvM_Prv_TryToGetLock(void)
{
    boolean isLockAvailable_b = FALSE;

#if (NVM_RB_USE_PROTECTION != FALSE)
    // Disable interrupts
    SchM_Enter_NvM_Main();
#endif

    // Is the Main function not invoked yet, then reserve it and allow the execution
    if (!NvM_Prv_flgUsedSema_b)
    {
        NvM_Prv_flgUsedSema_b = TRUE;
        isLockAvailable_b = TRUE;
    }

#if (NVM_RB_USE_PROTECTION != FALSE)
    // Enable interrupts
    SchM_Exit_NvM_Main();
#endif

    if (!isLockAvailable_b)
    {
        // Report re-entrant error
        NvM_Prv_ErrorDetection_ReportReentrantError(NVM_SERVICE_ID_MAIN_FUNCTION);
    }

#if (NVM_RB_USE_PROTECTION == FALSE)
    // Protection disabled, only detection by default
    isLockAvailable_b = TRUE;
#endif

    return isLockAvailable_b;
}

/**
 * \brief Reset the flag having reentrant information
 *
 */
LOCAL_INLINE void NvM_Prv_ReleaseLock(void)
{
    // Reset the access flag
    NvM_Prv_flgUsedSema_b = FALSE;
    return;
}

/**
 * \brief
 * MainFunction of NvM, does the processing of NvM jobs.
 *
 */
void NvM_MainFunction(void)
{
    // Local flag to store the lock accuring status
    boolean flgUsed_b = FALSE;

    // Check for re-entrant invocation of MainFunction
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3264] - NvM shall provide a mechanism to prevent re-entrant execution.
    flgUsed_b = NvM_Prv_TryToGetLock();

    if (flgUsed_b)
    {
        // Internal processing of NvM Mainfunction
        NvM_Prv_MainFunctionIntern();

        // Release the lock
        NvM_Prv_ReleaseLock();
    }

    return;
}

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

NvM_Prv_JobData_tst* NvM_Prv_GetActiveJobData(void)
{
    NvM_Prv_JobData_tst* JobData_pst;
    NvM_Prv_Job_tst* Job_pst = NvM_Prv_JobQueue_GetEntry(0u);
    if (NULL_PTR == Job_pst)
    {
        JobData_pst = NULL_PTR;
    }
    else
    {
        JobData_pst = &Job_pst->JobData_st;
    }
    return JobData_pst;
}

NvM_Prv_Activities_ten NvM_Prv_Main_GetCurrentState(void)
{
    return NvM_Prv_Main_st.Activity_rAMwM_en;
}

/**
 * \brief
 * Setter function for the re-entrant lock status flag
 *
 */
void NvM_Prv_SetSemaphoreFlag(boolean flg_b)
{
    NvM_Prv_flgUsedSema_b = flg_b;
    return;
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

