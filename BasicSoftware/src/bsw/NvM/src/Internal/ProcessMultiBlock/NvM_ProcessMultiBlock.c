
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_JobQueue.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ProcessMultiBlock.h"

#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
# include "BswM_NvM.h"
// TRACE[NVM089] Check version compatibility of included header files
# if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
#endif

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef NvM_Prv_idJob_ten (*NvM_Prv_ProcessMulti_tpfct)(NvM_Prv_QueueEntry_tst* QueueEntry_pst);

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
#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

static NvM_Prv_ProcessMulti_tpfct NvM_Prv_ProcessMulti_pfct = NULL_PTR;
static NvM_Prv_Multi_Cancel_tpfct NvM_Prv_Multi_Cancel_pfct = NULL_PTR;
// TRACE[NVM393] Separate variable for the final result of asynchronous multiblock requests
static NvM_RequestResultType NvM_Prv_Multi_Result_uo = NVM_REQ_OK;

#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessConfigId(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessUserBlocks(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessCancel(NvM_Prv_QueueEntry_tst* QueueEntry_pst);

#if (NVM_DYNAMIC_CONFIGURATION || NVM_RB_INIT_AT_LAYOUT_CHANGE)
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_WriteConfigId(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
#endif

LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_FinishMulti(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
LOCAL_INLINE void NvM_Prv_NotifyMultiStart(NvM_Prv_idService_tuo idService_uo);
LOCAL_INLINE void NvM_Prv_NotifyMultiEnd(NvM_Prv_idService_tuo idService_uo);
LOCAL_INLINE boolean NvM_Prv_CheckIsFinalCallbackRequired(NvM_Prv_idService_tuo idService_uo);
LOCAL_INLINE boolean NvM_Prv_StartSecondaryMulti(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
LOCAL_INLINE NvM_BlockIdType NvM_Prv_FindNextBlock(NvM_BlockIdType idBlock_uo,
                                                   NvM_Prv_idService_tuo idService_uo,
                                                   NvM_Prv_ServiceBit_tuo ServiceBit_uo);
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_GetJobIdForMulti(NvM_Prv_idService_tuo idService_uo,
                                                        NvM_Prv_ServiceBit_tuo ServiceBit_uo,
                                                        NvM_BlockIdType idBlock_uo);

#if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE)
LOCAL_INLINE boolean NvM_Prv_InitiateInitAfterReadAll(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
#endif

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
LOCAL_INLINE boolean NvM_Prv_InitiateMaintenance(NvM_Prv_QueueEntry_tst* QueueEntry_pst);
#endif

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

void NvM_Prv_Multi_Initialize(void)
{
    NvM_Prv_ProcessMulti_pfct = NULL_PTR;
    NvM_Prv_Multi_Cancel_pfct = NULL_PTR;
    NvM_Prv_Multi_Result_uo = NVM_REQ_OK;
}

boolean NvM_Prv_Multi_IsInProgress(void)
{
    return (NULL_PTR != NvM_Prv_ProcessMulti_pfct);
}

void NvM_Prv_Multi_SetResult(NvM_RequestResultType Result_uo)
{
    NvM_Prv_Multi_Result_uo = Result_uo;
}

NvM_RequestResultType NvM_Prv_Multi_GetResult(void)
{
    return NvM_Prv_Multi_Result_uo;
}

void NvM_Prv_Multi_Cancel(NvM_Prv_Multi_Cancel_tpfct Cancel_pfct)
{
    // Cancel requested multi-block service only if NvM is not already cancelling another multi-block service
    if (NULL_PTR == NvM_Prv_Multi_Cancel_pfct)
    {
        NvM_Prv_Multi_Cancel_pfct = Cancel_pfct;
    }
}

void NvM_Prv_Multi_GoToNextBlock(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    ++QueueEntry_pst->idBlock_uo;
}

NvM_Prv_idJob_ten NvM_Prv_Multi_Process(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;

    // start multi block service if not yet started
    if (NULL_PTR == NvM_Prv_ProcessMulti_pfct)
    {
        if (NVM_RB_CONFIG_ID_BLOCK == QueueEntry_pst->idBlock_uo)
        {
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_ProcessConfigId;
        }
        else
        {
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_ProcessUserBlocks;
        }

        // TRACE[NVM395] Maintain overall result of the multiblock operation
        NvM_Prv_Multi_Result_uo = NVM_REQ_OK;

        // Multiblock request acknowledged
        // -> notify the user that the multiblock operation is being started
        NvM_Prv_NotifyMultiStart(QueueEntry_pst->idService_uo);
    }

    // cancel multi-block service if requested by user
    if (NULL_PTR != NvM_Prv_Multi_Cancel_pfct)
    {
        NvM_Prv_ProcessMulti_pfct = NvM_Prv_ProcessCancel;
    }

    // process multi block service
    while ((NULL_PTR != NvM_Prv_ProcessMulti_pfct) && (NvM_Prv_idJob_Idle_e == idJob_en))
    {
        idJob_en = NvM_Prv_ProcessMulti_pfct(QueueEntry_pst);
    }

    return idJob_en;
}


#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
// ====================================================================================================================
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessConfigId(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;

    if (NvM_Prv_JobQueue_IsEmpty())
    {
        if (NvM_Prv_Block_IsRequestPending(NVM_RB_CONFIG_ID_BLOCK, (1u << QueueEntry_pst->ServiceBit_uo)))
        {
            idJob_en = NvM_Prv_GetJobIdForMulti(QueueEntry_pst->idService_uo,
                                                QueueEntry_pst->ServiceBit_uo,
                                                NVM_RB_CONFIG_ID_BLOCK);
            QueueEntry_pst->BlockData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(QueueEntry_pst->idBlock_uo);
        }
        else
        {
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_ProcessUserBlocks;
        }
    }
    else
    {
        idJob_en = NvM_Prv_idJob_Suspend_e;
    }

    return idJob_en;
}

// ====================================================================================================================
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessUserBlocks(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;

    QueueEntry_pst->idBlock_uo = NvM_Prv_FindNextBlock(QueueEntry_pst->idBlock_uo,
                                                       QueueEntry_pst->idService_uo,
                                                       QueueEntry_pst->ServiceBit_uo);

    if (QueueEntry_pst->idBlock_uo < NVM_CFG_NR_BLOCKS)
    {
#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
        if (!NvM_Prv_JobQueue_IsEmpty() && (NvMConf_NvMBlockDescriptor_NvM_WriteMon == QueueEntry_pst->idBlock_uo))
        {
            idJob_en = NvM_Prv_idJob_Suspend_e;
        }
        else
#endif
        {
            idJob_en = NvM_Prv_GetJobIdForMulti(QueueEntry_pst->idService_uo,
                                                QueueEntry_pst->ServiceBit_uo,
                                                QueueEntry_pst->idBlock_uo);
            QueueEntry_pst->BlockData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(QueueEntry_pst->idBlock_uo);
        }
    }
    else
    {
        if (NvM_Prv_JobQueue_IsEmpty())
        {
#if (NVM_DYNAMIC_CONFIGURATION || NVM_RB_INIT_AT_LAYOUT_CHANGE)
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_WriteConfigId;
#else
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_FinishMulti;
#endif
        }
        else
        {
            idJob_en = NvM_Prv_idJob_Suspend_e;
        }
    }

    return idJob_en;
}

// ====================================================================================================================
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_ProcessCancel(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
    if (NVM_SERVICE_ID_WRITE_ALL == QueueEntry_pst->idService_uo)
    {
        NvM_Prv_Block_ClearRequests(NVM_PRV_MULTI_BLOCK, (1u << NvM_Prv_ServiceBit_Maintain_e));
    }
#endif

    if (NvM_Prv_JobQueue_IsEmpty())
    {
        if (NvM_Prv_Multi_Cancel_pfct())
        {
            NvM_Prv_Multi_Result_uo = NVM_REQ_CANCELED;
        }

        QueueEntry_pst->idBlock_uo = NVM_CFG_NR_BLOCKS;
        NvM_Prv_Multi_Cancel_pfct = NULL_PTR;
        NvM_Prv_ProcessMulti_pfct = NvM_Prv_FinishMulti;
    }
    else
    {
        idJob_en = NvM_Prv_idJob_Suspend_e;
    }

    return idJob_en;
}

// ====================================================================================================================
#if (NVM_DYNAMIC_CONFIGURATION || NVM_RB_INIT_AT_LAYOUT_CHANGE)
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_WriteConfigId(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;
    uint16 maskRequest_u16 = ((1u << NvM_Prv_ServiceBit_WriteAll_e)           |
                              (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e) |
                              (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e));

    NvM_Prv_ProcessMulti_pfct = NvM_Prv_FinishMulti;

    if (((NVM_SERVICE_ID_WRITE_ALL == QueueEntry_pst->idService_uo) ||
         (NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL == QueueEntry_pst->idService_uo))
                &&
        NvM_Prv_Block_IsRequestPending(NVM_RB_CONFIG_ID_BLOCK, maskRequest_u16))
    {
        NvM_Prv_Block_SetIdConfig(NVM_COMPILED_CONFIG_ID);
        idJob_en = NvM_Prv_idJob_Write_e;
        QueueEntry_pst->idBlock_uo = NVM_RB_CONFIG_ID_BLOCK;
        QueueEntry_pst->BlockData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(QueueEntry_pst->idBlock_uo);
    }

    return idJob_en;
}
#endif

// ====================================================================================================================
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_FinishMulti(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    uint16 ServiceBitMask_uo;
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;;

    if (NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL == QueueEntry_pst->idService_uo)
    {
        ServiceBitMask_uo = (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e) |
                            (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e);
    }
    else
    {
        ServiceBitMask_uo = (1u << QueueEntry_pst->ServiceBit_uo);
    }

    if (NvM_Prv_JobQueue_IsEmpty())
    {
        NvM_Prv_Block_ClearRequests(NVM_PRV_MULTI_BLOCK, ServiceBitMask_uo);

        NvM_Prv_NotifyMultiEnd(QueueEntry_pst->idService_uo);

        if (NvM_Prv_StartSecondaryMulti(QueueEntry_pst))
        {
            NvM_Prv_ProcessMulti_pfct = NvM_Prv_ProcessUserBlocks;
        }
        else
        {
            // processing multiblock requests is idle
            NvM_Prv_ProcessMulti_pfct = NULL_PTR;
        }
    }
    else
    {
        idJob_en = NvM_Prv_idJob_Suspend_e;
    }

    return idJob_en;
}

// ====================================================================================================================
LOCAL_INLINE void NvM_Prv_NotifyMultiStart(NvM_Prv_idService_tuo idService_uo)
{
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
    //If BswMMultiBlockJobStatusInformation is enabled, the multiblock start callback is not invoked
    (void)idService_uo;
#else
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2771] Invoke multiblock start callback (if configured).
    NvM_Prv_InvokeMultiStartCallback(idService_uo);
#endif
}

// ====================================================================================================================
LOCAL_INLINE void NvM_Prv_NotifyMultiEnd(NvM_Prv_idService_tuo idService_uo)
{
    if (NvM_Prv_CheckIsFinalCallbackRequired(idService_uo))
    {
        // Determine the active service as indicated to the notification receiver
        uint8 idActiveService_uo = idService_uo;

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
        if (idActiveService_uo == NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK)
        {
            // Maintenance shall be an "invisible" operation after WriteAll,
            // so we indicate WriteAll to the notification receiver
            idActiveService_uo = NVM_SERVICE_ID_WRITE_ALL;
        }
#endif
        NvM_Prv_Block_SetRequestResult(NVM_PRV_MULTI_BLOCK, NvM_Prv_Multi_Result_uo);

        // Now trigger the actual (BswM or multiblock callback) notification
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2804]
        // If BswMMultiBlockJobStatusInformation is enabled, provide the muliblock request result to BswM
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2810]
        // If BswMMultiBlockJobStatusInformation is enabled, the multiblock callback if configured is not invoked
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
        BswM_NvM_CurrentJobMode(idActiveService_uo, NvM_Prv_Multi_Result_uo);
#else
        // TRACE[NVM347] Invoke multiblock callback if callback routine is configured
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2839] This also applies to the first init case
        // TRACE[NVM686] Supply the ID of the just finished multiblock operation to the callback owner
        NvM_Prv_InvokeMultiCallback(idActiveService_uo, NvM_Prv_Multi_Result_uo);
#endif
    }
}

// ====================================================================================================================
LOCAL_INLINE boolean NvM_Prv_CheckIsFinalCallbackRequired(NvM_Prv_idService_tuo idService_uo)
{
    boolean isFinalCallbackRequired_b = TRUE;

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2857]
    // The maintenance operation is performed as part of the execution of the NvM_WriteAll job
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2867]
    // WriteAll operation finishes when the writing of the suitable blocks and
    // the maintenance of the redundant blocks are done
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2873]
    // In case of WriteAll, the multiblock callback is called after that the subsequent maintenance phase is finished
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2875]
    // In case of WriteAll, the BswM-notification is done after that the subsequent maintenance phase is finished
#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
    if ((NVM_SERVICE_ID_WRITE_ALL == idService_uo) &&
        NvM_Prv_Block_IsRequestPending(NVM_PRV_MULTI_BLOCK, (1u << NvM_Prv_ServiceBit_Maintain_e)))
    {
        // The multiblock callback is called after the overall WriteAll operation
        // including the maintenance phase hasfinished;
        // the same applies to the BswM notification
        // -> we have to skip any notification here in this case
        // -> we come back once also maintenance is finished
        // (then with NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK as the active service)
        isFinalCallbackRequired_b = FALSE;
    }
#else
    (void)idService_uo;
#endif
    return isFinalCallbackRequired_b;
}

// ====================================================================================================================
/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
LOCAL_INLINE boolean NvM_Prv_StartSecondaryMulti(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    boolean isSecondaryMultiReqStarted;

#if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE)
    if (NVM_SERVICE_ID_READ_ALL == QueueEntry_pst->idService_uo)
    {
        isSecondaryMultiReqStarted = NvM_Prv_InitiateInitAfterReadAll(QueueEntry_pst);
    }
    else // CAUTION: This is an "else if" case separated by the NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS switch
#endif

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
    if (NVM_SERVICE_ID_WRITE_ALL == QueueEntry_pst->idService_uo)
    {
        isSecondaryMultiReqStarted = NvM_Prv_InitiateMaintenance(QueueEntry_pst);
    }
    else
#endif
    {
        isSecondaryMultiReqStarted = FALSE;
    }

#if ( (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE || NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS) == STD_OFF)
    // Unused parameter
    (void)QueueEntry_pst;
#endif

    return isSecondaryMultiReqStarted;
}

// ====================================================================================================================
LOCAL_INLINE NvM_BlockIdType NvM_Prv_FindNextBlock(NvM_BlockIdType idBlock_uo,
                                                   NvM_Prv_idService_tuo idService_uo,
                                                   NvM_Prv_ServiceBit_tuo ServiceBit_uo)
{
    uint16 RequestMask_u16;

#if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE)
    if (NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL == idService_uo)
    {
        RequestMask_u16 = (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e) |
                          (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e);
    }
    else
#else
    (void)idService_uo;
#endif
    {
        RequestMask_u16 = (1u << ServiceBit_uo);
    }

    while ((idBlock_uo < NVM_CFG_NR_BLOCKS) &&
           !NvM_Prv_Block_IsRequestPending(idBlock_uo, RequestMask_u16))
    {
        ++idBlock_uo;
    }
    return idBlock_uo;
}

// ====================================================================================================================
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_GetJobIdForMulti(NvM_Prv_idService_tuo idService_uo,
                                                        NvM_Prv_ServiceBit_tuo ServiceBit_uo,
                                                        NvM_BlockIdType idBlock_uo)
{
    NvM_Prv_idJob_ten idJob_en = NvM_Prv_idJob_Idle_e;
    switch (idService_uo)
    {
        case NVM_SERVICE_ID_READ_ALL:
        {
            if (NVM_RB_CONFIG_ID_BLOCK == idBlock_uo)
            {
                idJob_en = NvM_Prv_idJob_ReadIdConfigForReadAll_e;
            }
            else
            {
                idJob_en = NvM_Prv_idJob_Read_e;
            }
        }
        break;
        case NVM_SERVICE_ID_RB_FIRST_INIT_ALL:
        {
            if ((NVM_BLOCK_DATASET != NvM_Prv_GetBlockType(idBlock_uo)) &&
                NvM_Prv_IsDefaultDataAvailable(idBlock_uo))
            {
                idJob_en = NvM_Prv_idJob_Restore_e;
            }
            else
            {
                idJob_en = NvM_Prv_idJob_InvalidateForFirstInitAll_e;
            }
        }
        break;
#if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE)
        case NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL:
        {
            if (NvM_Prv_Block_IsRequestPending(idBlock_uo, (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e)))
            {
                idJob_en = NvM_Prv_idJob_InvalidateForRemoveNonResistant_e;
            }
            else if (NvM_Prv_Block_IsRequestPending(idBlock_uo, (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e)))
            {
                idJob_en = NvM_Prv_idJob_Write_e;
            }
            else
            {
                // should not happen
            }
        }
        break;
#endif
        default:
            idJob_en = NvM_Prv_GetJobId(ServiceBit_uo);
        break;
    }

    return idJob_en;
}

// ====================================================================================================================
#if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS || NVM_RB_INIT_AT_LAYOUT_CHANGE)
LOCAL_INLINE boolean NvM_Prv_InitiateInitAfterReadAll(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    boolean isInitAfterReadAllStarted = FALSE;
    uint16 maskService_u16 = (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e) |
                             (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e);
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2787] If at least one block has been found during the current
    //   multiblock operation (this search is only done during ReadAll) which is not resistant to changed SW
    //   and a SW change has been detected, then start the multiblock removal sequence here
    // Initialization and removal are done in the same phase after ReadAll
    if (NvM_Prv_Block_IsRequestPending(NVM_PRV_MULTI_BLOCK, maskService_u16))
    {
        // Multiblock removal request pending
        NvM_Prv_Block_SetRequestResult(NVM_PRV_MULTI_BLOCK, NVM_REQ_PENDING);
        NvM_Prv_Multi_Result_uo = NVM_REQ_OK;

        QueueEntry_pst->idService_uo = NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL;
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2787] Start initialization/removal with block 2
        QueueEntry_pst->idBlock_uo = NVM_RB_FIRST_USER_BLOCK;
        // BitServiceAck_uo will be set later
        QueueEntry_pst->BlockData_un.ptrRamBlock_pv = NULL_PTR;

        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2804]
        // If BswMMultiBlockJobStatusInformation is enabled, notify BswM
        // about the state change of the multiblock operation
# if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
        BswM_NvM_CurrentJobMode(NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL, NVM_REQ_PENDING);
# endif

        NvM_Prv_UpdateActiveServiceData(NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL, NvM_Prv_idQueue_Multi_e);
        isInitAfterReadAllStarted = TRUE;
    }

    return isInitAfterReadAllStarted;
}
#endif

// ====================================================================================================================
#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
LOCAL_INLINE boolean NvM_Prv_InitiateMaintenance(NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    boolean isMaintenanceStarted = FALSE;
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2858] Start the maintenance operation
    //     for all redundant blocks after that the writing of the blocks in WriteAll finished
    if (NvM_Prv_Block_IsRequestPending(NVM_PRV_MULTI_BLOCK, (1u << NvM_Prv_ServiceBit_Maintain_e)))
    {
        // Multiblock maintenance request pending
        // The request result of block #0 is already set to PENDING when the maintenance operations starts
        // NvM_Prv_Multi_Result_uo is not used for the maintenance operation
        QueueEntry_pst->idService_uo = NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK;
        // Start the maintenance with block #1/#2 if dynamic config is enabled/disabled
        QueueEntry_pst->idBlock_uo = NVM_CFG_FIRST_USED_BLOCK;
        QueueEntry_pst->ServiceBit_uo = NvM_Prv_ServiceBit_Maintain_e;
        QueueEntry_pst->BlockData_un.ptrRamBlock_pv = NULL_PTR;

        NvM_Prv_UpdateActiveServiceData(NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK, NvM_Prv_idQueue_Multi_e);
        isMaintenanceStarted = TRUE;
    }
    return isMaintenanceStarted;
}
#endif
