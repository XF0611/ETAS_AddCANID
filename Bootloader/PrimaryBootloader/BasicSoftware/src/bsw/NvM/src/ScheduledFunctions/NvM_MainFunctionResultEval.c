
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_JobQueue.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_ProcessMultiBlock.h"
#include "NvM_Prv_Crc.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef NvM_RequestResultType (*NvM_Prv_JobEvalReqResult_tpfct)(NvM_Prv_Job_tst const* Job_pcst);
typedef void (*NvM_Prv_JobEvalProductionErrors_tpfct)(NvM_Prv_Job_tst* Job_pst);
typedef void (*NvM_Prv_JobUpdateBlockStatus_tpfct)(NvM_Prv_Job_tst const* Job_pcst);

/*
 **********************************************************************************************************************
 * Local declarations
 **********************************************************************************************************************
 */
static NvM_RequestResultType NvM_Prv_EvalReqResult(NvM_Prv_Job_tst const* Job_pcst);
static NvM_RequestResultType NvM_Prv_EvalReqResult_Read(NvM_Prv_Job_tst const* Job_pcst);
static NvM_RequestResultType NvM_Prv_EvalReqResult_InvalidateForRemoveNonResistant(NvM_Prv_Job_tst const* Job_pcst);
static NvM_RequestResultType NvM_Prv_EvalReqResult_RestoreForImplicitRecovery(NvM_Prv_Job_tst const* Job_pcst);
static NvM_RequestResultType NvM_Prv_EvalReqResult_ReadIdConfigForReadAll(NvM_Prv_Job_tst const* Job_pcst);

static void NvM_Prv_EvalProductionErrors(NvM_Prv_Job_tst* Job_pst);
static void NvM_Prv_EvalProductionErrors_Read(NvM_Prv_Job_tst* Job_pst);
static void NvM_Prv_EvalProductionErrors_ReadIdConfigForReadAll(NvM_Prv_Job_tst* Job_pst);

static void NvM_Prv_UpdateBlockStatus_Restore(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatus_Read(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatus_Write(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatus_WriteAll(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatusJobValidate(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatusIdConfigForReadAll(NvM_Prv_Job_tst const* Job_pcst);
static void NvM_Prv_UpdateBlockStatus_RecalcRamBlkCrc(NvM_Prv_Job_tst const* Job_pcst);

static NvM_Prv_JobEvalReqResult_tpfct NvM_Prv_GetFctJobEvalReqResult(NvM_Prv_idJob_ten idJob_en);
static NvM_Prv_JobEvalProductionErrors_tpfct NvM_Prv_GetFctJobEvalProductionErrors(NvM_Prv_idJob_ten idJob_en);
static NvM_Prv_JobUpdateBlockStatus_tpfct NvM_Prv_GetFctJobUpdateBlockStatus(NvM_Prv_idJob_ten idJob_en);

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_EvaluateJobs(NvM_Prv_Job_tst* Job_pst, NvM_Prv_MainStates_tst* MainStates_pst);
LOCAL_INLINE boolean NvM_Prv_EvalCommonResults(NvM_Prv_Job_tst* Job_pst,
                                               NvM_RequestResultType* stReqResult_puo,
                                               boolean isResultPlausible_b);
LOCAL_INLINE void NvM_Prv_MainFunctionResultEval_FinalBlockCallbacks(NvM_RequestResultType stRequestResult_uo,
                                                                     NvM_BlockIdType idActiveBlock_uo,
                                                                     NvM_Prv_idService_tuo idActiveService_uo,
                                                                     boolean IsAuxServiceActive_b);
LOCAL_INLINE void NvM_Prv_MainFunctionResultEval_ResetMainStates(NvM_BlockIdType idBlock_uo,
                                                                 NvM_Prv_idService_tuo idService_uo,
                                                                 NvM_Prv_ServiceBit_tuo ServiceBit_uo,
                                                                 NvM_RequestResultType stRequestResult_uo);

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

#define NvM_Prv_EvalResult_Idle_pfct                            NULL_PTR
#define NvM_Prv_EvalResult_Read_pfct                            NvM_Prv_EvalReqResult_Read
#define NvM_Prv_EvalResult_Write_pfct                           NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_Erase_pfct                           NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_Restore_pfct                         NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_Maintain_pfct                        NULL_PTR
#define NvM_Prv_EvalResult_Validate_pfct                        NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_Invalidate_pfct                      NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_ReadIdConfigForReadAll_pfct          NvM_Prv_EvalReqResult_ReadIdConfigForReadAll
#define NvM_Prv_EvalResult_InvalidateForFirstInitAll_pfct       NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_RestoreForImplicitRecovery_pfct      NvM_Prv_EvalReqResult_RestoreForImplicitRecovery
#define NvM_Prv_EvalResult_InvalidateForRemoveNonResistant_pfct NvM_Prv_EvalReqResult_InvalidateForRemoveNonResistant
#define NvM_Prv_EvalResult_RecalcRamBlkCrc_pfct                 NULL_PTR
#define NvM_Prv_EvalResult_WriteAll_pfct                        NvM_Prv_EvalReqResult
#define NvM_Prv_EvalResult_Suspend_pfct                         NULL_PTR
#define NvM_Prv_EvalResult_Invalid_pfct                         NULL_PTR

/**
 * Function-like macro: LIST_ENTRY_JOB_EVAL_REQ_RESULT_FCT
 *
 * This macro generates an entry for a function to evaluate request result in a list of all jobs provided by NvM.
 * s.a. macro LIST_ID_JOB to generate a list for all jobs provided by NvM
 *
 * Paramater:
 * Name: name of the job for which a function will be generated.
 */
#define LIST_ENTRY_JOB_EVAL_REQ_RESULT_FCT(Name)        NvM_Prv_EvalResult_##Name##_pfct,

static NvM_Prv_JobEvalReqResult_tpfct const NvM_Prv_JobEvalReqResultFcts_capfct[NvM_Prv_idJob_Count_e] =
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_ID_JOB(LIST_ENTRY_JOB_EVAL_REQ_RESULT_FCT)
};


#define NvM_Prv_EvalProdErrors_Idle_pfct                            NULL_PTR
#define NvM_Prv_EvalProdErrors_Read_pfct                            NvM_Prv_EvalProductionErrors_Read
#define NvM_Prv_EvalProdErrors_Write_pfct                           NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_Erase_pfct                           NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_Restore_pfct                         NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_Maintain_pfct                        NULL_PTR
#define NvM_Prv_EvalProdErrors_Validate_pfct                        NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_Invalidate_pfct                      NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_ReadIdConfigForReadAll_pfct          NvM_Prv_EvalProductionErrors_ReadIdConfigForReadAll
#define NvM_Prv_EvalProdErrors_InvalidateForFirstInitAll_pfct       NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_RestoreForImplicitRecovery_pfct      NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_InvalidateForRemoveNonResistant_pfct NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_RecalcRamBlkCrc_pfct                 NULL_PTR
#define NvM_Prv_EvalProdErrors_WriteAll_pfct                        NvM_Prv_EvalProductionErrors
#define NvM_Prv_EvalProdErrors_Suspend_pfct                         NULL_PTR
#define NvM_Prv_EvalProdErrors_Invalid_pfct                         NULL_PTR

/**
 * Function-like macro: LIST_ENTRY_JOB_EVAL_PROD_ERRORS_FCT
 *
 * This macro generates an entry for a function to evaluate production errors in a list of all jobs provided by NvM.
 * s.a. macro LIST_ID_JOB to generate a list for all jobs provided by NvM
 *
 * Paramater:
 * Name: name of the job for which a function will be generated.
 */
#define LIST_ENTRY_JOB_EVAL_PROD_ERRORS_FCT(Name)       NvM_Prv_EvalProdErrors_##Name##_pfct,

static NvM_Prv_JobEvalProductionErrors_tpfct const NvM_Prv_JobEvalProductionErrorsFcts_capfct[NvM_Prv_idJob_Count_e] =
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_ID_JOB(LIST_ENTRY_JOB_EVAL_PROD_ERRORS_FCT)
};


#define NvM_Prv_UpdateStatus_Idle_pfct                              NULL_PTR
#define NvM_Prv_UpdateStatus_Read_pfct                              NvM_Prv_UpdateBlockStatus_Read
#define NvM_Prv_UpdateStatus_Write_pfct                             NvM_Prv_UpdateBlockStatus_Write
#define NvM_Prv_UpdateStatus_Erase_pfct                             NULL_PTR
#define NvM_Prv_UpdateStatus_Restore_pfct                           NvM_Prv_UpdateBlockStatus_Restore
#define NvM_Prv_UpdateStatus_Maintain_pfct                          NULL_PTR
#define NvM_Prv_UpdateStatus_Validate_pfct                          NvM_Prv_UpdateBlockStatusJobValidate
#define NvM_Prv_UpdateStatus_Invalidate_pfct                        NULL_PTR
#define NvM_Prv_UpdateStatus_ReadIdConfigForReadAll_pfct            NvM_Prv_UpdateBlockStatusIdConfigForReadAll
#define NvM_Prv_UpdateStatus_InvalidateForFirstInitAll_pfct         NULL_PTR
#define NvM_Prv_UpdateStatus_RestoreForImplicitRecovery_pfct        NvM_Prv_UpdateBlockStatus_Restore
#define NvM_Prv_UpdateStatus_InvalidateForRemoveNonResistant_pfct   NULL_PTR
#define NvM_Prv_UpdateStatus_RecalcRamBlkCrc_pfct                   NvM_Prv_UpdateBlockStatus_RecalcRamBlkCrc
#define NvM_Prv_UpdateStatus_WriteAll_pfct                          NvM_Prv_UpdateBlockStatus_WriteAll
#define NvM_Prv_UpdateStatus_Suspend_pfct                           NULL_PTR
#define NvM_Prv_UpdateStatus_Invalid_pfct                           NULL_PTR

/**
 * Function-like macro: LIST_ENTRY_JOB_UPDATE_BLOCK_STATUS_FCT
 *
 * This macro generates an entry for a function to update block status in a list of all jobs provided by NvM.
 * s.a. macro LIST_ID_JOB to generate a list for all jobs provided by NvM
 *
 * Paramater:
 * Name: name of the job for which a function will be generated.
 */
#define LIST_ENTRY_JOB_UPDATE_BLOCK_STATUS_FCT(Name)    NvM_Prv_UpdateStatus_##Name##_pfct,

static NvM_Prv_JobUpdateBlockStatus_tpfct const NvM_Prv_JobUpdateBlockStatusFcts_capfct[NvM_Prv_idJob_Count_e] =
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_ID_JOB(LIST_ENTRY_JOB_UPDATE_BLOCK_STATUS_FCT)
};

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * NvM-private code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static NvM_RequestResultType NvM_Prv_EvalReqResult(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_RequestResultType stRequestResult_uo;

    // TRACE[SWS_NvM_00809] Write job for NvM_WritePRAMBlock shall set request result NVM_REQ_OK
    //                      after successful writing of redundant block
    // TRACE[SWS_NvM_00820] Read job for NvM_RestorePRAMBlockDefaults  shall set request result NVM_REQ_OK
    //                      after successful restoring
    if (    (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
         || (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == Job_pcst->JobResult_st.Result_en) )
    {
        stRequestResult_uo = NVM_REQ_OK;
    }
    else if (NvM_Prv_JobResult_Skipped_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_BLOCK_SKIPPED;
    }
    else
    {
        stRequestResult_uo = NVM_REQ_NOT_OK;
    }
    return stRequestResult_uo;
}

static NvM_RequestResultType NvM_Prv_EvalReqResult_Read(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_RequestResultType stRequestResult_uo;
    // TRACE[SWS_NvM_00775] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_OK
    //                      after successful reading
    // TRACE[SWS_NvM_00782] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_OK
    //                      after successful reading of redundant block
    if (    (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
         || (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == Job_pcst->JobResult_st.Result_en) )
    {
        stRequestResult_uo = NVM_REQ_OK;
    }
    // TRACE[SWS_NvM_00770] Read job for NvM_ReadPRAMBlock shall not copy NV block to the RAM block
    //                      if dataset block is invalidated
    // TRACE[SWS_NvM_00776] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_NV_INVALIDATED
    //                      if read block is invalidated
    // TRACE[SWS_NvM_00785] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_NV_INVALIDATED
    //                      if both NV blocks of redundant block are invalidated
    else if (NvM_Prv_JobResult_BlockInvalidated_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_NV_INVALIDATED;
    }
    // TRACE[SWS_NvM_00771] Read job for NvM_ReadPRAMBlock shall not copy NV block to the RAM block
    //                      if dataset block is inconsistent
    // TRACE[SWS_NvM_00778] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_INTEGRITY_FAILED
    //                      if read block is inconsistent
    else if (NvM_Prv_JobResult_BlockInconsistent_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_INTEGRITY_FAILED;
    }
    else if (NvM_Prv_JobResult_Skipped_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_BLOCK_SKIPPED;
    }
    // TRACE[SWS_NvM_00780] Read job for NvM_ReadPRAMBlock shallset request result NVM_REQ_NOT_OK
    //                      if reading has failed
    else
    {
        stRequestResult_uo = NVM_REQ_NOT_OK;
    }
    return stRequestResult_uo;
}

static NvM_RequestResultType NvM_Prv_EvalReqResult_InvalidateForRemoveNonResistant(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_RequestResultType stRequestResult_uo;
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_NV_INVALIDATED;
    }
    else
    {
        stRequestResult_uo = NVM_REQ_NOT_OK;
    }
    return stRequestResult_uo;
}

static NvM_RequestResultType NvM_Prv_EvalReqResult_RestoreForImplicitRecovery(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_RequestResultType stRequestResult_uo;

    // TRACE[SWS_NvM_00882] Read job for NvM_ReadPRAMBlock shall set request result NVM_REQ_RESTORED_FROM_ROM
    //                      if loading default values has succeeded
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_RESTORED_FROM_ROM;
    }
    else
    {
        stRequestResult_uo = NVM_REQ_NOT_OK;
    }
    return stRequestResult_uo;
}

/**
 * \brief
 * This function evaluates the request result if the configuration ID has been read for ReadAll service.
 *
 * \details
 * The request result depends on the read job result and the read value of the configuration ID:
 * - reading configuration ID has succeeded AND configuration ID has not changed -> NVM_REQ_OK
 * - reading configuration ID has succeeded AND configuration ID has changed -> NVM_REQ_NOT_OK
 * - invalidated configuration ID has been read (someone has intentionally invalidated it) -> NVM_REQ_NV_INVALIDATED
 * - inconsistent configuration ID has been read
 *   (configuration ID has got corrupted in NV memory or was never written into the NV memory) -> NVM_REQ_NV_INVALIDATED
 * - reading configuration ID has failed -> NVM_REQ_INTEGRITY_FAILED
 * - all other job results -> NVM_REQ_NOT_OK
 *
 * If the configuration ID was not read successfully then this function sets it to trigger special actions.
 * Following scenarios are possible:
 * - set configuration ID to value different from compiled value (s. NVM_COMPILED_CONFIG_ID)
 *   so that NvM will overwrite configuration ID in non-volatile memory, this is required in following cases:
 *   - inconsistent configuration ID has been read
 *   - reading configuration ID has failed
 * - set configuration ID to compiled value (s. NVM_COMPILED_CONFIG_ID) so that NvM will not overwrite configuration ID
 *   in non-volatile memory, this is required in following cases:
 *   - invalidated configuration ID has been read
 *   - all other job results
 *
 * If configured then this function initiates the internal multi-block service for block initialization
 * after a ReadAll service.
 *
 * \param JobData_pcst
 * Pointer to the structure with job data.
 * The caller has to make sure that this pointer is valid and points to plausible data.
 */
static NvM_RequestResultType NvM_Prv_EvalReqResult_ReadIdConfigForReadAll(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_RequestResultType stRequestResult_uo;

    if (   (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
        || (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == Job_pcst->JobResult_st.Result_en) )
    {
        if (NvM_Prv_Block_HasIdConfigChanged())
        {
            stRequestResult_uo = NVM_REQ_NOT_OK;
        }
        else
        {
            stRequestResult_uo = NVM_REQ_OK;
        }
    }
    else if (NvM_Prv_JobResult_BlockInvalidated_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_NV_INVALIDATED;
        // set ConfigID to compiled value to signal that ConfigId has not changed and to avoid writing it
        NvM_Prv_Block_SetIdConfig(NVM_COMPILED_CONFIG_ID);
    }
    else if (NvM_Prv_JobResult_BlockInconsistent_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_NV_INVALIDATED;
        // set ConfigID to signal that ConfigId has changed and to trigger writing it
        NvM_Prv_Block_SetIdConfig((uint16)(~((uint16)NVM_COMPILED_CONFIG_ID)));
    }
    else if (NvM_Prv_JobResult_Failed_e == Job_pcst->JobResult_st.Result_en)
    {
        stRequestResult_uo = NVM_REQ_INTEGRITY_FAILED;
        // set ConfigID to signal that ConfigId has changed and to trigger writing it
        NvM_Prv_Block_SetIdConfig((uint16)(~((uint16)NVM_COMPILED_CONFIG_ID)));
    }
    else
    {
        stRequestResult_uo = NVM_REQ_NOT_OK;
        // set ConfigID to compiled value to signal that ConfigId has not changed and to avoid writing it
        NvM_Prv_Block_SetIdConfig(NVM_COMPILED_CONFIG_ID);
    }

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) || (NVM_RB_INIT_AT_LAYOUT_CHANGE == STD_ON))
    NvM_Prv_InitAfterReadAll(Job_pcst->JobResult_st.Result_en);
#endif

    return stRequestResult_uo;
}

static void NvM_Prv_EvalProductionErrors(NvM_Prv_Job_tst* Job_pst)
{
    if (   (NvM_Prv_JobResult_Succeeded_e != Job_pst->JobResult_st.Result_en)
        && (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e != Job_pst->JobResult_st.Result_en) )
    {
        Job_pst->JobResult_st.ProductionError_u8 |= NVM_E_RB_REQ_FAILED;
    }
}

static void NvM_Prv_EvalProductionErrors_Read(NvM_Prv_Job_tst* Job_pst)
{
    if (NvM_Prv_JobResult_BlockInconsistent_e == Job_pst->JobResult_st.Result_en)
    {
        Job_pst->JobResult_st.ProductionError_u8 |= NVM_E_RB_INTEGRITY_FAILED;
    }
    else if (NvM_Prv_JobResult_Failed_e == Job_pst->JobResult_st.Result_en)
    {
        Job_pst->JobResult_st.ProductionError_u8 |= NVM_E_RB_REQ_FAILED;
    }
    else
    {
        // do nothing
    }
}

static void NvM_Prv_EvalProductionErrors_ReadIdConfigForReadAll(NvM_Prv_Job_tst* Job_pst)
{
    if (NvM_Prv_JobResult_BlockInconsistent_e == Job_pst->JobResult_st.Result_en)
    {
        // Production error NVM_E_RB_INTEGRITY_FAILED which could be entered by the read job
        // is reset here due to the requirement SWS_NvM_00306 to avoid its reporting.
        Job_pst->JobResult_st.ProductionError_u8 &= (~NVM_E_RB_INTEGRITY_FAILED);
    }
    else if (NvM_Prv_JobResult_Failed_e == Job_pst->JobResult_st.Result_en)
    {
        Job_pst->JobResult_st.ProductionError_u8 |= NVM_E_RB_REQ_FAILED;
    }
    else
    {
        // do nothing
    }
}

static void NvM_Prv_UpdateBlockStatus_Restore(NvM_Prv_Job_tst const* Job_pcst)
{
    // TRACE[SWS_NvM_00774] Restore job for NvM_ReadPRAMBlock shall set the RAM block to valid and changed
    //                      after successful restoring default data
    // TRACE[SWS_NvM_00820] Restore job for NvM_RestorePRAMBlockDefaults  shall set the RAM block to valid and changed
    //                      after successful restoring default data
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        if (Job_pcst->JobData_st.isPRamBlockUsed_b)
        {
            uint8 maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                        | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            uint8 maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
        }
    }
}

static void NvM_Prv_UpdateBlockStatus_Read(NvM_Prv_Job_tst const* Job_pcst)
{
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        if (Job_pcst->JobData_st.isPRamBlockUsed_b)
        {

            uint8 maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                        | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            uint8 maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_VALID;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
        }

        if (NvM_Prv_IsBlockSelected(Job_pcst->JobData_st.idBlock_uo, NVM_PRV_BLOCK_FLAG_WRITE_ONCE))
        {
            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, NVM_BLOCK_STATE_WRITE_PROT, NVM_BLOCK_STATE_WRITE_PROT);
        }
    }
    else if (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == Job_pcst->JobResult_st.Result_en)
    {
        // After a soft-reset if data has survived for a block with RAM CRC the read job skips reading from
        // media if CRC is consistent. In this case the Changed-flag and WriteReq-flag shall not be reset in
        // order to allow the writing of the block later.
        if (Job_pcst->JobData_st.isPRamBlockUsed_b)
        {

            uint8 maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ;

            uint8 maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_VALID;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
        }

        // In case that no access to medium has been done write protection must not be activated for
        // NVM_PRV_BLOCK_FLAG_WRITE_ONCE blocks
    }
    else
    {
        // nothing to do
    }
    NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, NVM_BLOCK_STATE_READ_DONE, NVM_BLOCK_STATE_READ_DONE);
}

static void NvM_Prv_UpdateBlockStatus_Write(NvM_Prv_Job_tst const* Job_pcst)
{
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        if (Job_pcst->JobData_st.isPRamBlockUsed_b)
        {

            uint8 maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                        | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            uint8 maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_VALID;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
        }

        if (NvM_Prv_IsBlockSelected(Job_pcst->JobData_st.idBlock_uo, NVM_PRV_BLOCK_FLAG_WRITE_ONCE))
        {
            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, NVM_BLOCK_STATE_WRITE_PROT, NVM_BLOCK_STATE_WRITE_PROT);
        }
    }
    else if (NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == Job_pcst->JobResult_st.Result_en)
    {
        // Write Job has skipped writing to media because of CRC compare mechanism
        if (Job_pcst->JobData_st.isPRamBlockUsed_b)
        {

            uint8 maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                        | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            uint8 maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_VALID;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
        }
    }
    else
    {
        // nothing to do
    }

}

static void NvM_Prv_UpdateBlockStatus_WriteAll(NvM_Prv_Job_tst const* Job_pcst)
{
    NvM_Prv_UpdateBlockStatus_Write(Job_pcst);

#if (NVM_RB_SET_WRITEALL_TRIGGER_API == STD_ON)
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2980]
    // Set the flag back to the init state after finishing the WriteAll job for the current block
    NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL, 0u);
#endif
}

static void NvM_Prv_UpdateBlockStatusJobValidate(NvM_Prv_Job_tst const* Job_pcst)
{
    if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
    {
        uint8 maskBitsToChange_u8;
        uint8 maskBitsNewValue_u8;

        if (NvM_Prv_IsBlockSelected(Job_pcst->JobData_st.idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
        {
            maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_CHANGED
                                  | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ;

            maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_CHANGED
                                  | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ;
        }
        else
        {
            maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_CHANGED;

            maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_CHANGED;
        }

        NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
    }
}

static void NvM_Prv_UpdateBlockStatusIdConfigForReadAll(NvM_Prv_Job_tst const* Job_pcst)
{
    if (NvM_Prv_Block_InitIdConfigDuringWriteAll())
    {
        // TRACE[NVM310] Mark the config ID block as VALID/CHANGED to enable its writing during WriteAll
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2901] Signal the writing of the config ID block in WriteAll
        //                                               when dynamic conf is active and extended dynamic conf is inactive
        NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo,
                               NVM_BLOCK_STATE_RAM_MASK,
                               NVM_BLOCK_STATE_RAM_VALID_CHANGED);
    }
    NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, NVM_BLOCK_STATE_READ_DONE, NVM_BLOCK_STATE_READ_DONE);
}

static void NvM_Prv_UpdateBlockStatus_RecalcRamBlkCrc(NvM_Prv_Job_tst const* Job_pcst)
{
    uint8 maskBitsToChange_u8;
    uint8 maskBitsNewValue_u8;

    // This check is to make sure that the application has not changed the data and called SetRamBlockStatus
    // in the mean time again. -> If still ongoing then SetRamBlockStatus() has not been called since start of
    // CRC recalculation.
    if (NvM_Prv_Block_IsRamCrcRecalcOngoing(Job_pcst->JobData_st.idBlock_uo))
    {
        // RAM block CRC recalculation was not interrupted by "another" NvM_SetRamBlockStatus() call
        if (NvM_Prv_JobResult_Succeeded_e == Job_pcst->JobResult_st.Result_en)
        {
            if (NvM_Prv_Crc_CheckRamBlockCrc(Job_pcst->JobData_st.idBlock_uo,
                                             &Job_pcst->JobResult_st.CrcData_st.CrcRamBlk_un))
            {
                // Data has not been changed (CRC identical) -> just end CRC recalc

                maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                      | NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING;

                maskBitsNewValue_u8 = 0u;
            }
            else
            {
                // Data has changed -> must be written during next write operation
                maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING
                                      | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                      | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

                maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;
            }

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
            // Saving of RAM Block CRC has to be done after CRC comparison (NvM_Prv_Crc_CheckRamBlockCrc())
            // => therefore it is beeing done here instead of doing it in NvM_JobRecalcRamBlkCrc.c.
            NvM_Prv_Crc_SetRamBlockCrc(Job_pcst->JobData_st.idBlock_uo,
                                       &Job_pcst->JobResult_st.CrcData_st.Calculation_st.Crc_un);
        }
        else if (NvM_Prv_JobResult_Failed_e == Job_pcst->JobResult_st.Result_en)
        {
            // Job Result failed -> Explicit Sync callback has failed (CRC recalculation calculation cannot fail)
            // If Explicit Sync callback fails, then status flags shall be set in the same way as if CRC calculation
            // finished and a data change has been detected. -> NvM shall shall write data during next NvM_WriteAll
            maskBitsToChange_u8 =   NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING
                                  | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                  | NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            maskBitsNewValue_u8 =   NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ;

            NvM_Prv_Block_SetState(Job_pcst->JobData_st.idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);
            // Ram Block CRC is not set, as it was not possible to retrieve the data and calculate it
        }
        else
        {
            // is never reached
        }
    }

    return;
}

static NvM_Prv_JobEvalReqResult_tpfct NvM_Prv_GetFctJobEvalReqResult(NvM_Prv_idJob_ten idJob_en)
{
    if (idJob_en >= NvM_Prv_idJob_Count_e)
    {
        idJob_en = NvM_Prv_idJob_Invalid_e;
    }
    return NvM_Prv_JobEvalReqResultFcts_capfct[idJob_en];
}

static NvM_Prv_JobEvalProductionErrors_tpfct NvM_Prv_GetFctJobEvalProductionErrors(NvM_Prv_idJob_ten idJob_en)
{
    if (idJob_en >= NvM_Prv_idJob_Count_e)
    {
        idJob_en = NvM_Prv_idJob_Invalid_e;
    }
    return NvM_Prv_JobEvalProductionErrorsFcts_capfct[idJob_en];
}

static NvM_Prv_JobUpdateBlockStatus_tpfct NvM_Prv_GetFctJobUpdateBlockStatus(NvM_Prv_idJob_ten idJob_en)
{
    if (idJob_en >= NvM_Prv_idJob_Count_e)
    {
        idJob_en = NvM_Prv_idJob_Invalid_e;
    }
    return NvM_Prv_JobUpdateBlockStatusFcts_capfct[idJob_en];
}

void NvM_Prv_MainFunctionResultEval(NvM_Prv_MainStates_tst* MainStates_pst)
{
    NvM_Prv_Job_tst* Job_pst = NvM_Prv_JobQueue_GetEntry(0u);

    if (   (NULL_PTR != Job_pst)
        && (NvM_Prv_stJob_Finished_e == Job_pst->stJob_en)
       )
    {
        NvM_Prv_EvaluateJobs(Job_pst, MainStates_pst);
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_EvaluateJobs(NvM_Prv_Job_tst* Job_pst,
                                       NvM_Prv_MainStates_tst* MainStates_pst)
{
    boolean isRequestResultEvaluated_b = FALSE;
    boolean isResultPlausible_b = FALSE;
    NvM_RequestResultType stRequestResult_uo = NVM_REQ_NOT_OK;

    NvM_Prv_idService_tuo idActiveService_uo = Job_pst->JobData_st.idService_uo;
    boolean isMultiServiceActive_b = Job_pst->JobData_st.isMultiServiceActive_b;
    NvM_BlockIdType idBlock_uo = Job_pst->JobData_st.idBlock_uo;
#if (NVM_RB_AUX_INTERFACE == STD_ON)
    if (Job_pst->JobData_st.isAuxServiceActive_b)
    {
        idBlock_uo = NVM_PRV_AUX_ADMIN_BLOCK;
    }

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3027] Set migration result for auxiliary read
    NvM_Prv_SetAuxMigrationResult(idActiveService_uo, Job_pst->JobData_st.idBlock_uo);
#endif

    // We rely heavily on the fact that we know really all possible job results
    // -> check this assumption and report DET error if development error detection is enabled
    isResultPlausible_b = NvM_Prv_ErrorDetection_IsJobResultMemIfPlausible(NVM_SERVICE_ID_MAIN_FUNCTION,
                                                                           Job_pst->JobData_st.idBlock_uo,
                                                                           Job_pst->JobResult_st.Result_en);

    // Set scheduling lock
    SchM_Enter_NvM_Main();

    isRequestResultEvaluated_b = NvM_Prv_EvalCommonResults(Job_pst,
                                                           &stRequestResult_uo,
                                                           isResultPlausible_b);

    // Part to be done after result evaluation WITHIN scheduling lock
    if (isRequestResultEvaluated_b)
    {
        // Set global variables from local copies to update results/states for the user
        // TRACE[NVM175] Indicate the request result to the user
        NvM_Prv_Block_SetRequestResult(idBlock_uo, stRequestResult_uo);
        NvM_Prv_ErrorDetection_SetProductionError(Job_pst->JobData_st.idBlock_uo,
                                                  Job_pst->JobResult_st.ProductionError_u8);
    }

    // Reset the states of NvM's main state machine because the current job is now completely finished
    NvM_Prv_MainFunctionResultEval_ResetMainStates(idBlock_uo,
                                                   idActiveService_uo,
                                                   Job_pst->JobData_st.ServiceBit_uo,
                                                   stRequestResult_uo);

    // Reset scheduling lock
    SchM_Exit_NvM_Main();

    // Part to be done after result evaluation WITHOUT scheduling lock
    if (isRequestResultEvaluated_b)
    {
        // Invoke observer and singleblock callbacks if applicable
        NvM_Prv_MainFunctionResultEval_FinalBlockCallbacks(stRequestResult_uo,
                                                           idBlock_uo,
                                                           idActiveService_uo,
                                                           Job_pst->JobData_st.isAuxServiceActive_b);
    }

    NvM_Prv_JobQueue_Dequeue();

    if (NvM_Prv_JobQueue_IsEmpty())
    {
        if (!isMultiServiceActive_b)
        {
            MainStates_pst->idServiceActive_uo = NVM_SERVICE_ID_INIT;
            MainStates_pst->idQueueActive_uo = NvM_Prv_idQueue_nrQueues_e;
        }
    }
    else
    {
        Job_pst = NvM_Prv_JobQueue_GetEntry(0u);
        MainStates_pst->idServiceActive_uo = Job_pst->JobData_st.idService_uo;
        MainStates_pst->idQueueActive_uo = Job_pst->JobData_st.idQueue_uo;
    }
}

// ====================================================================================================================
LOCAL_INLINE boolean NvM_Prv_EvalCommonResults(NvM_Prv_Job_tst* Job_pst,
                                               NvM_RequestResultType* stReqResult_puo,
                                               boolean isResultPlausible_b)
{
    boolean isReqResultEvaluated_b = FALSE;

    if (isResultPlausible_b)
    {
        // Pointer to the function to evaluate request results of the currently active job
        NvM_Prv_JobEvalReqResult_tpfct JobEvalReqResult_pfct = NvM_Prv_GetFctJobEvalReqResult(Job_pst->JobData_st.idJob_en);
        // Pointer to the function to evaluate production errors occured during the currently active job
        NvM_Prv_JobEvalProductionErrors_tpfct JobEvalProductionErrors_pfct =
                NvM_Prv_GetFctJobEvalProductionErrors(Job_pst->JobData_st.idJob_en);
        // Pointer to the function to update the status of the block handled during the currently active job
        NvM_Prv_JobUpdateBlockStatus_tpfct JobUpdateBlockStatus_pfct =
                NvM_Prv_GetFctJobUpdateBlockStatus(Job_pst->JobData_st.idJob_en);

        if (NULL_PTR != JobEvalReqResult_pfct)
        {
            *stReqResult_puo = JobEvalReqResult_pfct(Job_pst);
            isReqResultEvaluated_b = TRUE;

            // TRACE[NVM395] Maintain overall result of the multiblock operation
            if (Job_pst->JobData_st.isMultiServiceActive_b &&
                ((NvM_Prv_JobResult_BlockInconsistent_e == Job_pst->JobResult_st.Result_en) ||
                 (NvM_Prv_JobResult_Failed_e == Job_pst->JobResult_st.Result_en)))
            {
                // TRACE[NVM301] Set final multiblock result to NVM_REQ_NOT_OK (applies to NvM_ReadAll)
                // TRACE[NVM318] Same as NVM301, applied to NvM_WriteAll
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2836] NVM_REQ_NOT_OK is one
                //                                               of the two possible request results of first init
                NvM_Prv_Multi_SetResult(NVM_REQ_NOT_OK);
            }
        }
        if (NULL_PTR != JobEvalProductionErrors_pfct)
        {
            JobEvalProductionErrors_pfct(Job_pst);
        }
        if (NULL_PTR != JobUpdateBlockStatus_pfct)
        {
            JobUpdateBlockStatus_pfct(Job_pst);
        }
    }
    return isReqResultEvaluated_b;
}

// ====================================================================================================================

LOCAL_INLINE void NvM_Prv_MainFunctionResultEval_FinalBlockCallbacks(NvM_RequestResultType stRequestResult_uo,
                                                                     NvM_BlockIdType idActiveBlock_uo,
                                                                     NvM_Prv_idService_tuo idActiveService_uo,
                                                                     boolean IsAuxServiceActive_b)
{
    // Invoke observer and singleblock callbacks if applicable
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2842] Invoke the observer callback if this callback routine is configured
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2844] Invoke the observer callback before the singleblock callback
    NvM_Prv_InvokeObserverCallback(idActiveBlock_uo, idActiveService_uo, stRequestResult_uo);

    // TRACE[NVM347] Invoke singleblock callback if callback routine is configured
    // TRACE[NVM281] Processing of this block finished -> invoke callback function (applies to NvM_ReadAll)
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2839] Same as NVM281, but applied to first init
    // TRACE[NVM740] The callback is invoked upon termination of the block operation
    // TRACE[NVM742] The caller is only actively notified if a callback has been configured
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2930]
    // Don't invoke the singleblock callback if the current service is triggered by an auxiliary user
    if (!IsAuxServiceActive_b)
    {
        // TRACE[NVM741] Supply the ID of the just finished block operation to the callback owner
        NvM_Prv_InvokeSingleBlockCallback(idActiveBlock_uo, idActiveService_uo, stRequestResult_uo);
    }
}

// ====================================================================================================================

LOCAL_INLINE void NvM_Prv_MainFunctionResultEval_ResetMainStates(NvM_BlockIdType idBlock_uo,
                                                                 NvM_Prv_idService_tuo idService_uo,
                                                                 NvM_Prv_ServiceBit_tuo ServiceBit_uo,
                                                                 NvM_RequestResultType stRequestResult_uo)
{
    uint16 ServiceBitMask_uo;

    if (NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL == idService_uo)
    {
        ServiceBitMask_uo = (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e) |
                            (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e);
    }
    else if ((NVM_SERVICE_ID_READ_ALL == idService_uo) &&
             (NVM_RB_CONFIG_ID_BLOCK != idBlock_uo) &&
             (NVM_REQ_RESTORED_FROM_ROM != stRequestResult_uo))
    {
        ServiceBitMask_uo = (1u << NvM_Prv_ServiceBit_ReadAll_e) |
                            (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e);
    }
    else
    {
        ServiceBitMask_uo = (1u << ServiceBit_uo);
    }

    NvM_Prv_Block_ClearRequests(idBlock_uo, ServiceBitMask_uo);
}
