
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
# include "BswM_NvM.h"
// TRACE[NVM089] Check version compatibility of included header files
# if (!defined(BSWM_AR_RELEASE_MAJOR_VERSION) || (BSWM_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
#endif

#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_WriteAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo);
static void NvM_Prv_WriteAll_UpdateBlockData(void);
static void NvM_Prv_WriteAll_FindBlocks(void);
static void NvM_Prv_WriteAll_Finalize(void);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#ifdef NVM_CFG_API_WRITE_ALL

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern void NVM_CFG_API_WRITE_ALL(void);

void NvM_WriteAll(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    NVM_CFG_API_WRITE_ALL();
}

void NvM_Rb_WriteAllBase(void)

#else

void NvM_WriteAll(void)

#endif
{
    // TRACE[NVM461] Initiates a multi block write request
    // TRACE[NVM018] This service writes the contents of all permanent RAM blocks to their NV counterparts

    NvM_Prv_QueueEntry_tst QueueEntry_st;
    NvM_Prv_Service_ConfigurationMulti_tst Configuration_st;

    // put all service parameters in the queue entry structure
    QueueEntry_st.idService_uo = NVM_SERVICE_ID_WRITE_ALL;
    QueueEntry_st.idBlock_uo = NVM_RB_FIRST_USER_BLOCK;
    QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_WriteAll_e;
    QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;

    // put all required functions in the multi-block service configuration structure
    Configuration_st.UpdateBlockDataForMulti_pfct = NvM_Prv_WriteAll_UpdateBlockData;
    Configuration_st.FinalizeMulti_pfct = NvM_Prv_WriteAll_Finalize;

    // TRACE[NVM647] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_Multi_e, &QueueEntry_st, &Configuration_st);
}

static void NvM_Prv_WriteAll_UpdateBlockData(void)
{
    // TRACE[NVM185] Set multi block request result to NVM_REQ_PENDING on successful enqueuing
    // TRACE[NVM549] Same as NVM185, but specific to NvM_WriteAll
    NvM_Prv_Block_SetRequestResult(NVM_PRV_MULTI_BLOCK, NVM_REQ_PENDING);
    // TRACE[NVM254] Signal WriteAll to the  NvM by setting the corresponding bit for multi-block,
    //               the actual processing of this request will start asynchronously in NvM main function
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_WriteAll_e);

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2861] The maintenance operation is to be performed even if,
    //                                               at the time when NvM_WriteAll is invoked,
    //                                               no blocks are suitable for WriteAll.
    // -> Signal that a maintenance phase has to be performed
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_Maintain_e);
#endif

    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_WriteAll_IsBlockParticipating);
}

static boolean NvM_Prv_WriteAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo)
{
    boolean IsBlockValid_b;
    boolean IsBlockChanged_b;
    boolean IsTrgWriteAllActive_b;

    NvM_Prv_BlockErrors_tuo errors_uo = 0;
    boolean IsBlockWriteable_b = NvM_Prv_Block_IsWriteable(idBlock_uo, NvM_Prv_Block_GetIdxDataset(idBlock_uo), &errors_uo);
    if(!IsBlockWriteable_b)
    {
        // WriteAll service can not report NVM_E_RB_WRITE_PROTECTED error, only NVM_E_WRITE_ONCE_STATUS_UNKNOWN error
        // can be reported, clear the NVM_PRV_BLOCK_WRITE_PROTECTED bit before calling the ErrorDetection function.
        errors_uo &= ~NVM_PRV_BLOCK_WRITE_PROTECTED;
        NvM_Prv_ErrorDetection_ReportServiceInitiationErrors(NVM_SERVICE_ID_WRITE_ALL,
                                                             idBlock_uo,
                                                             errors_uo);
    }
    IsBlockValid_b = NvM_Prv_Block_IsPRamBlockValid(idBlock_uo);
    IsBlockChanged_b = NvM_Prv_Block_IsChanged(idBlock_uo);
    IsTrgWriteAllActive_b = NvM_Prv_Block_IsTrgWriteAllActive(idBlock_uo);
    // The following conditions must be fulfilled for writing a block during WriteAll:
    return (
            // TRACE[NVM252] Only write blocks which are selected for WriteAll
            //               Only write blocks configured with a permanent RAM block or with explicit sync.
            //               This is already checked in configuration scripts.
            NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL) &&
            // TRACE[NVM432] Only write blocks which are not write protected
            // TRACE[NVM433] Same as NVM432
            // TRACE[NVM377] Check that the dataset index does not point to a ROM block if the NvM block is of type DATASET
            IsBlockWriteable_b &&
            // TRACE[NVM682] Only write blocks where the corresponding RAM block is valid and changed
            (
             (IsBlockValid_b && IsBlockChanged_b) ||
             // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2978] Write block if WriteAll trigger is active
             IsTrgWriteAllActive_b
#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
             // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3140] The NvM shall configure the NVRAM block NvM_WriteMon
             //                                               to be part of NvM_ReadAll and NvM_WriteAll.
             || (NvMConf_NvMBlockDescriptor_NvM_WriteMon == idBlock_uo)
#endif
            )
           );
}

static void NvM_Prv_WriteAll_FindBlocks(void)
{
    NvM_BlockIdType idBlock_uo;

    // Iterate through all used blocks to find the blocks which can and shall be written
    // The auxiliary interface does not support multi-block operations at all, hence it is sufficient
    // to iterate over all "regular" block IDs only
    for (idBlock_uo = NVM_CFG_FIRST_USED_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; idBlock_uo++)
    {
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3033] Ignore blocks with length null
        if (NvM_Prv_GetBlockSize(idBlock_uo) > 0)
        {
            if (NvM_Prv_WriteAll_IsBlockParticipating(idBlock_uo))
            {
                // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
                // TRACE[NVM549] Same as NVM185, but specific to NvM_WriteAll
                // This is done right away to allow an immediate NvM_GetErrorStatus call returning a meaningful result
                NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_PENDING);

                // Internally signal that this block has been requested to be written
                NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_WriteAll_e);
            }
            else
            {
                // TRACE[NVM434] Skip write protected blocks without any error notification
                // TRACE[NVM377] Behave similarly to write protected blocks if the block is of type DATASET and
                //               the dataset-index points to a ROM-block
                // TRACE[NVM375] Writing to an NV block of a block of type DATASET is not possible
                //               if the block is write protected
                // TRACE[NVM298] Set job result to NVM_REQ_BLOCK_SKIPPED if not written
                //               This is done right away to allow an immediate NvM_GetErrorStatus call
                //               returning a meaningful result
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2879] If a block is currently PENDING
                //                                               due to a queued/running single block operation,
                //                                               the request result shall not be set to SKIPPED
                if (NVM_REQ_PENDING != NvM_Prv_Block_GetRequestResult(idBlock_uo))
                {
                    NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_BLOCK_SKIPPED);
                }
            }

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
            if (NVM_BLOCK_REDUNDANT == NvM_Prv_GetBlockType(idBlock_uo))
            {
                // TRACE[NVM474] A corrupted NV block of a redundant NVM block is to be recovered
                //               from the intact NV block during the execution of WriteAll
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2860] A redundant block is maintained
                //                                               even if it is not suitable for WriteAll
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2869] A redundant block is to be maintained
                //                                               even if it is write-protected
                // -> Internally signal that this block has been requested to be maintained
                NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_Maintain_e);

                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2862] The request result of a redundant block
                //                                               which is not suitable for WriteAll is not set
                //                                               to PENDING although its is going to be maintained.
            }
#endif
        }
    }
}

static void NvM_Prv_WriteAll_Finalize(void)
{
    NvM_Prv_WriteAll_FindBlocks();
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NULL_PTR);

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2804] If BswMMultiBlockJobStatusInformation is enabled,
    //                                               notify BswM about the state change of the multiblock operation.
    // Calling foreign BswM function is done out of the Schm lock
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
    BswM_NvM_CurrentJobMode(NVM_SERVICE_ID_WRITE_ALL, NVM_REQ_PENDING);
#endif
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

