
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

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_FirstInitAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo);
static void NvM_Prv_FirstInitAll_UpdateBlockData(void);
static void NvM_Prv_FirstInitAll_FindBlocks(void);
static void NvM_Prv_FirstInitAll_Finalize(void);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#ifdef NVM_CFG_API_RB_FIRST_INIT_ALL

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern void NVM_CFG_API_RB_FIRST_INIT_ALL(void);

void NvM_Rb_FirstInitAll(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    NVM_CFG_API_RB_FIRST_INIT_ALL();
}

#endif

/**
 * This function initiates a multi block (re-) initialization operation request.
 */
#ifdef NVM_CFG_API_RB_FIRST_INIT_ALL
void NvM_Rb_FirstInitAllBase(void)
#else
void NvM_Rb_FirstInitAll(void)
#endif
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2826] Service to trigger a multi block (re-) initialization operation
    NvM_Prv_QueueEntry_tst QueueEntry_st;
    NvM_Prv_Service_ConfigurationMulti_tst Configuration_st;

    // put all service parameters in a structure for a queue entry
    QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_FIRST_INIT_ALL;
    QueueEntry_st.idBlock_uo = NVM_CFG_FIRST_USED_BLOCK;
    QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_FirstInitAll_e;
    QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;

    // put all required functions in a structure for multi-block-service configuration
    Configuration_st.UpdateBlockDataForMulti_pfct = NvM_Prv_FirstInitAll_UpdateBlockData;
    Configuration_st.FinalizeMulti_pfct = NvM_Prv_FirstInitAll_Finalize;

    NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_Multi_e, &QueueEntry_st, &Configuration_st);
}

static void NvM_Prv_FirstInitAll_UpdateBlockData(void)
{
    // TRACE[NVM185] Set multi block request result to NVM_REQ_PENDING on successful enqueuing
    NvM_Prv_Block_SetRequestResult(NVM_PRV_MULTI_BLOCK, NVM_REQ_PENDING);
    // The actual initialization operation is not done here, just the request for doing it is signaled
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_FirstInitAll_e);

    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_FirstInitAll_IsBlockParticipating);
}

static boolean NvM_Prv_FirstInitAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo)
{
    // The following conditions must be fulfilled for initializing a block during FirstInitAll:
    return (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_FIRST_INIT_ALL));
}

static void NvM_Prv_FirstInitAll_FindBlocks(void)
{
    NvM_BlockIdType idBlock_uo;

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2830] Iterate over all used blocks to find the blocks
    //                                               which can and shall be initialized
    for (idBlock_uo = NVM_CFG_FIRST_USED_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3033] Ignore blocks with length null
        if (NvM_Prv_GetBlockSize(idBlock_uo) > 0)
        {
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2830] Only initialize blocks which are selected for FirstInitAll
            // Further conditions are already checked in the configuration scripts
            if (NvM_Prv_FirstInitAll_IsBlockParticipating(idBlock_uo))
            {
                // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
                // This is done right away to allow an immediate NvM_GetErrorStatus call returning a meaningful result
                NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_PENDING);

                // Internally signal that this block has been requested to be written
                NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_FirstInitAll_e);
            }
            else
            {
                // Set job result to NVM_REQ_BLOCK_SKIPPED if not initialized
                // This is done right away to allow an immediate NvM_GetErrorStatus call returning a meaningful result
                // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2879] If a block is currently PENDING due
                //                                               to a queued/running single block operation,
                //                                               the request result shouldn't be set to SKIPPED
                if (NVM_REQ_PENDING != NvM_Prv_Block_GetRequestResult(idBlock_uo))
                {
                    NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_BLOCK_SKIPPED);
                }
            }
        }
    }
}

static void NvM_Prv_FirstInitAll_Finalize(void)
{
    NvM_Prv_FirstInitAll_FindBlocks();
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NULL_PTR);

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2804] If BswMMultiBlockJobStatusInformation is enabled,
    //                                               notify BswM about the state change of the multiblock operation.
    // Calling foreign BswM function is done out of the Schm lock
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
    BswM_NvM_CurrentJobMode(NVM_SERVICE_ID_RB_FIRST_INIT_ALL, NVM_REQ_PENDING);
#endif
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

