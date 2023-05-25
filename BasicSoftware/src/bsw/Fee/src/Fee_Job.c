

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#include "Fee_Idx_Cfg.h"

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv.h"
#include "Fee_Cfg_SchM.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define FEE_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"
static Fee_Prv_Job_tst Fee_Prv_Job_st;  /* Internal variables of the job unit */
#define FEE_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

/**
 * \brief   Function called during Fee_Init(). Initializes all the variables of this unit.
 */
void Fee_Prv_JobInit(void)
{
    uint16_least idxBlk_qu16;
    uint8_least  idxRequester_qu8;

    /* Init the job queue */
    for(idxRequester_qu8 = 0u; idxRequester_qu8 < (uint8_least)FEE_PRV_REQUESTER_MAX_E; idxRequester_qu8++)
    {
        Fee_Prv_Job_st.jobs_ast[idxRequester_qu8].type_en = FEE_PRV_JOBTYPE_NONE_E;
        Fee_Prv_Job_st.results_aen[idxRequester_qu8] = MEMIF_JOB_OK;
    }

    /* Init the migration status per block */
    for(idxBlk_qu16 = 0u; idxBlk_qu16 < FEE_NUM_BLOCKS; idxBlk_qu16++)
    {
        Fee_Prv_Job_st.firstMigrationResult_aen[idxBlk_qu16] = MEMIF_RB_MIGRATION_RESULT_INIT_E;
    }
}

/**
 * \brief   Try to place an order in the internal job slot.
 *          Based on block configuration the right job slot is chosen.
 *          If the Fee is not ready or the job slot is occupied E_NOT_OK is returned.
 *
 * \param   FeeDeviceName_en    Device instance for which job has to be performed
 * \param   apiId_u8            The ID of the API which places the job
 * \param   jobDesc_pcst        The order that shall be placed
 *
 * \return  E_OK                Order placed successfully
 * \return  E_NOT_OK            Order could not be placed
 */
Std_ReturnType Fee_Prv_JobPut(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, Fee_Prv_JobDesc_tst const * jobDesc_pcst)
{
    Std_ReturnType              result_en       = E_NOT_OK;
    Fee_Prv_ConfigRequester_ten requester_en;

    /* Select the requester based on the type of the job. */
    if ((FEE_PRV_JOBTYPE_TRIGGERREORG_E == jobDesc_pcst->type_en) ||
        (FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E == jobDesc_pcst->type_en))
    {
        /* This is not a block related job, mark all such jobs as Adapter jobs */
        requester_en = FEE_PRV_REQUESTER_ADAPTER_E;
    }
    else
    {
        /* Check block configuration to find out who is the user */
        requester_en = Fee_Prv_ConfigGetBlockRequesterByBlockNr(jobDesc_pcst->blockNumber_u16);
    }

    SchM_Enter_Fee_Order();
    /*
     * Make sure the job slot is free.
     * A spin lock is needed since the main function might want to finish a job asynchronous.
     */
    if(FEE_PRV_JOBTYPE_NONE_E == Fee_Prv_Job_st.jobs_ast[requester_en].type_en)
    {
        /* Set job and result of the current job */
        Fee_Prv_Job_st.jobs_ast   [requester_en] = *jobDesc_pcst;
        Fee_Prv_Job_st.results_aen[requester_en] = MEMIF_JOB_PENDING;
        result_en = E_OK;
    }
    SchM_Exit_Fee_Order();

    /*
     * In case placing the order of NvM or an adapter did not work trigger Det error.
     * Do not do this for internal orders.
     * Do not do this under interrupt lock!
     */
    if(E_NOT_OK == result_en)
    {
        switch(requester_en)
        {
            case FEE_PRV_REQUESTER_NVM_E:     Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_BUSY         ); break;
            case FEE_PRV_REQUESTER_ADAPTER_E: Fee_Prv_LibDetReport(FeeDeviceName_en, apiId_u8, FEE_E_BUSY_INTERNAL); break;
            default:                            /* Not DET report for internal orders */                             break;
        }
    }

    return(result_en);
}

/**
 * \brief   Called by the main function if it wants to begin an order.
 *          NvM orders have priority over normal adapter orders.
 *          If a stop mode or a trigger reorg request is pending, this request gets prioritized over all normal jobs.
 *
 * \return  The next job that shall be started.
 */
Fee_Prv_JobDesc_tst * Fee_Prv_JobNext(void)
{
    uint32 idxRequesterLoop_u32;
    Fee_Prv_JobDesc_tst * result_pst = NULL_PTR;
    uint32 idxRequesterNextJob_u32 = (uint32)FEE_PRV_REQUESTER_MAX_E;

    for(idxRequesterLoop_u32 = (uint32)FEE_PRV_REQUESTER_NVM_E;
            (idxRequesterLoop_u32 < (uint32)FEE_PRV_REQUESTER_MAX_E);
                 idxRequesterLoop_u32++ )
    {
        if(FEE_PRV_JOBTYPE_NONE_E != Fee_Prv_Job_st.jobs_ast[idxRequesterLoop_u32].type_en)
        {
            /* The requester index for the next job to be processed will be set:
             * - if a job in the current user slot available but no job of a higher priority slot already selected
             * - if stop mode shall be entered or a reorg shall be triggered (may overwrite an already selected block oriented job)
             */
            if(((uint32)FEE_PRV_REQUESTER_MAX_E   == idxRequesterNextJob_u32) ||
               (FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E == Fee_Prv_Job_st.jobs_ast[idxRequesterLoop_u32].type_en) ||
               (FEE_PRV_JOBTYPE_TRIGGERREORG_E    == Fee_Prv_Job_st.jobs_ast[idxRequesterLoop_u32].type_en))
            {
                idxRequesterNextJob_u32 = idxRequesterLoop_u32;
            }
        }
    }

    /* If a new job is available return the pointer to the appropriate job decriptor */
    if((uint32)FEE_PRV_REQUESTER_MAX_E != idxRequesterNextJob_u32)
    {
        /* Begin job under interrupt lock since at the same time an external requester might modify the job. */
        SchM_Enter_Fee_Order();
        result_pst = &Fee_Prv_Job_st.jobs_ast[idxRequesterNextJob_u32];
        SchM_Exit_Fee_Order();
    }

    return(result_pst);
}

/**
 * \brief   Called by the main function if it wants to finish a job.
 *          If a callback is configured the callback is called.
 *
 * \param   jobDesc_pst        Job that shall be finished
 * \param   result_en          Result of that job
 * \param   migrationResult_en Migration result of that job (only valid for read jobs)
 *
 * \attention   This function calls Fee user callbacks under spin lock!
 *              That means the Fee user shall only do very short operations in its callback!
 */
void Fee_Prv_JobEnd(Fee_Prv_JobDesc_tst * jobDesc_pst,
                    MemIf_JobResultType result_en,
                    MemIf_Rb_MigrationResult_ten migrationResult_en)
{
    Fee_Prv_ConfigNotification_tpfn notification_pfn    = NULL_PTR;
    Fee_Prv_ConfigRequester_ten     requester_en;
    MemIf_Rb_MigrationResult_ten    migrationResultUpdate_en = Fee_Prv_Job_st.firstMigrationResult_aen[jobDesc_pst->blockNumber_u16];

    /* Select the requester based on the type of the job. */
    if ((FEE_PRV_JOBTYPE_TRIGGERREORG_E == jobDesc_pst->type_en) ||
        (FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E == jobDesc_pst->type_en))
    {
        /* This is not a block related job, mark all such jobs as Adapter jobs */
        requester_en = FEE_PRV_REQUESTER_ADAPTER_E;
    }
    else
    {
        /* Check block configuration to find out who is the user */
        requester_en = Fee_Prv_ConfigGetBlockRequesterByBlockNr(jobDesc_pst->blockNumber_u16);

        /* Find out the configured notification for this job results and this block
         *
         * Note:
         * Calling the job end notification in case of a block invalid result is an AUTOSAR deviation.
         * AUTOSAR specifies the error callback in this case.
         * This behaviour was introduces for PS-EC, but now the DGS adapter has a special handling to exactly address
         * this case. It means we provided a workaround from AUTOSAR for PS-EC, but now PS-EC has a workaround for our
         * workaround in their software.
         * Best would be to remove this deviation, but since this is incompatible we will not be able to do it.
         */
        if((MEMIF_JOB_OK        == result_en) ||
           (MEMIF_BLOCK_INVALID == result_en))
        {
            notification_pfn = Fee_Prv_ConfigGetJobEndNotification(jobDesc_pst->blockNumber_u16);
        }
        else
        {
            notification_pfn = Fee_Prv_ConfigGetJobErrorNotification(jobDesc_pst->blockNumber_u16);
        }
    }

    /* Calculate migration result in case a read job is finished */
    if(FEE_PRV_JOBTYPE_READ_E == jobDesc_pst->type_en)
    {
        /* Check if migration result was already set */
        if(MEMIF_RB_MIGRATION_RESULT_INIT_E == Fee_Prv_Job_st.firstMigrationResult_aen[jobDesc_pst->blockNumber_u16])
        {
            migrationResultUpdate_en = migrationResult_en;
        }
    }

    /* Finish a job under spin lock */
    SchM_Enter_Fee_Order();

    Fee_Prv_Job_st.results_aen[requester_en] = result_en;

    /* Migration result gets always updated to minimize the spinlock time by avoiding condition checks */
    Fee_Prv_Job_st.firstMigrationResult_aen[jobDesc_pst->blockNumber_u16] = migrationResultUpdate_en;

    jobDesc_pst->type_en = FEE_PRV_JOBTYPE_NONE_E;

    if(NULL_PTR != notification_pfn)
    {
        notification_pfn();
    }

    SchM_Exit_Fee_Order();
}

/**
 * \brief   Returns the job type which is currently active in the job slot of the given requester.
 *
 * \param   requester_en    Select the job slot to be checked (NvM, Adapter, Debug)
 *
 * \return  FEE_PRV_JOBTYPE_NONE_E               No job is currently processed
 * \return  FEE_PRV_JOBTYPE_READ_E               A read job is currently processed
 * \return  FEE_PRV_JOBTYPE_WRITE_E              A write job is currently processed
 * \return  FEE_PRV_JOBTYPE_INVALIDATE_E         An invalidate job is currently processed
 * \return  FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E   A maintainance job is currently processed
 * \return  FEE_PRV_JOBTYPE_TRIGGERREORG_E       A triggered reorg job is currently processed
 * \return  FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E    A job to enter into stop mode iscurrently processed
 */
Fee_Prv_JobType_ten Fee_Prv_JobGetActive(Fee_Prv_ConfigRequester_ten requester_en)
{
    Fee_Prv_JobType_ten activeJobType_en;

    activeJobType_en = Fee_Prv_Job_st.jobs_ast[requester_en].type_en;

    return(activeJobType_en);
}

/**
 * \brief   Returns the result of the job for the given requester.
 *
 * \param   requester_en                Select the job slot to be checked (NvM, Adapter, Debug)
 *
 * \return  MEMIF_JOB_OK                Last job executed successfully
 * \return  MEMIF_JOB_FAILED            Last job failed unexpected
 * \return  MEMIF_JOB_PENDING           Last job is still running
 * \return  MEMIF_BLOCK_INCONSISTENT    Last job was a read and
 *                                      a) Not a single instance of all instances has consistent data
 *                                      b) The block is not present at all
 * \return  MEMIF_BLOCK_INVALIDATED     Last job was a read and the block was invalidated intentionally
 *
 * \attention   Since the job result can change asynchronously a time consuming spin lock is needed!
 *              Please consider this when creating do/while wait loops for the Fee.
 */
MemIf_JobResultType Fee_Prv_JobGetResult(Fee_Prv_ConfigRequester_ten requester_en)
{
    MemIf_JobResultType result_en;

    /* According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    result_en = Fee_Prv_Job_st.results_aen[requester_en];

    return(result_en);
}

/**
 * \brief   Returns the migration result of data reading during initialisation
 *
 * \param   Blocknumber  Block number as generated in Fee_Cfg.h
 *
 * \return  MEMIF_RB_MIGRATION_RESULT_INIT_E               Initialization value, result not yet available
 * \return  MEMIF_RB_MIGRATION_RESULT_NOT_NECESSARY_E      RAM size == EEPROM size (migration not necessary)
 * \return  MEMIF_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E    RAM size < EEPROM size (remaining data discarded)
 * \return  MEMIF_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E     RAM size > EEPROM size (remaining data filled with zero)
 * \return  MEMIF_RB_MIGRATION_RESULT_NOT_DONE_E           Migration not done, because data were not read
 * \return  MEMIF_RB_MIGRATION_RESULT_DEACTIVATED_E        Migrations is deactivated by configuration
 *
 * \attention   Since the migration result can change asynchronously a time consuming spin lock is needed!
 *              Please consider this when creating do/while wait loops for the Fee.
 */
MemIf_Rb_MigrationResult_ten Fee_Prv_JobGetMigrationResult(uint16 blockNr_u16)
{
    MemIf_Rb_MigrationResult_ten result_en;

    /* According to the prerequisites that RTA-BSW has to run on >= 32 bit controllers and data are always aligned,
     * atomic access is guaranteed by the hardware for 32 bit memory access
     */
    result_en = Fee_Prv_Job_st.firstMigrationResult_aen[blockNr_u16];

    return(result_en);
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

#endif
