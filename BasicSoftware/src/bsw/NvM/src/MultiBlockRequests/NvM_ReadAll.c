
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

#include "NvM_Prv.h"
#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON))
#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"
//TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2999] the potentially incompatible blocks after layout changes should be
//                                              sanitized by ReadAll at first
static boolean NvM_Prv_isReadAllInitiated_b;
# define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"
#endif

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_ReadAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo);
static void NvM_Prv_ReadAll_UpdateBlockData(void);
static void NvM_Prv_ReadAll_FindBlocks(void);
static void NvM_Prv_ReadAll_Finalize(void);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#ifdef NVM_CFG_API_READ_ALL

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern void NVM_CFG_API_READ_ALL(void);

void NvM_ReadAll(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    NVM_CFG_API_READ_ALL();
}

void NvM_Rb_ReadAllBase(void)

#else

void NvM_ReadAll(void)

#endif
{
    // TRACE[NVM460] Initiates a multi block read request
    // TRACE[NVM356] This service reads the contents of all applicable permanent RAM blocks from their NV counterparts

    NvM_Prv_QueueEntry_tst QueueEntry_st;
    NvM_Prv_Service_ConfigurationMulti_tst Configuration_st;

    // put all service parameters in a structure for a queue entry
    QueueEntry_st.idService_uo = NVM_SERVICE_ID_READ_ALL;
    QueueEntry_st.idBlock_uo = NVM_CFG_FIRST_USED_BLOCK;
    QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_ReadAll_e;
    QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;

    // put all required functions in a structure for multi-block-service configuration
    Configuration_st.UpdateBlockDataForMulti_pfct = NvM_Prv_ReadAll_UpdateBlockData;
    Configuration_st.FinalizeMulti_pfct = NvM_Prv_ReadAll_Finalize;

    // TRACE[NVM646] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_Multi_e, &QueueEntry_st, &Configuration_st);
}

static void NvM_Prv_ReadAll_UpdateBlockData(void)
{
    // TRACE[NVM185] Set request result to NVM_REQ_PENDING on successful enqueuing
    // TRACE[NVM667] Same as NVM185, but specific to NvM_ReadAll
    NvM_Prv_Block_SetRequestResult(NVM_PRV_MULTI_BLOCK, NVM_REQ_PENDING);

    // TRACE[NVM243] The actual read operation is not done here, just the request for doing it is signaled
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_ReadAll_e);

    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_ReadAll_IsBlockParticipating);

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON))
    //TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2999] the "NvM_Prv_isReadAllInitiated_b" will be set to TRUE
    //                                              only if ReadAll is initiated, then the blocks are free
    //                                              for the other requests
    NvM_Prv_isReadAllInitiated_b = TRUE;
#endif
}

static boolean NvM_Prv_ReadAll_IsBlockParticipating(NvM_BlockIdType idBlock_uo)
{
    // The following conditions must be fulfilled for reading a block during ReadAll:
    return (
            // TRACE[NVM118] Only read blocks which are selected for ReadAll.
            //               Only read blocks configured with a permanent RAM block or with explicit sync.
            //               This is already checked in configuration scripts.
            NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL)
           );
}

static void NvM_Prv_ReadAll_FindBlocks(void)
{
    NvM_BlockIdType idBlock_uo;

    // Iterate through all used blocks to find the blocks which can and shall be read
    // TRACE[NVM073] In case of dynamic configuration, start with a read operation for block 1
    // TRACE[NVM248] Otherwise, block 1 is not read during NvM_ReadAll
    // TRACE[NVM669] Block ID 1 is reserved for the config ID
    // The auxiliary interface does not support multi-block operations at all,
    // hence it is sufficient to iterate over all "regular" block IDs only
    for (idBlock_uo = NVM_CFG_FIRST_USED_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3033] Ignore blocks with length null
        if (NvM_Prv_GetBlockSize(idBlock_uo) > 0)
        {
            if (NvM_Prv_ReadAll_IsBlockParticipating(idBlock_uo))
            {
                // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
                // This is done right away to allow an immediate NvM_GetErrorStatus call returning a meaningful result
                // Note: In case of dynamic configuration, this might be revised to NVM_REQ_BLOCK_SKIPPED depending
                // on the config ID comparison result
                NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_PENDING);

                // Internally signal that this block has been requested to be read
                NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_ReadAll_e);
            }
            else
            {
                // TRACE[NVM287] Set job result to NVM_REQ_BLOCK_SKIPPED if not read
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

static void NvM_Prv_ReadAll_Finalize(void)
{
    NvM_Prv_ReadAll_FindBlocks();
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NULL_PTR);

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2804] If BswMMultiBlockJobStatusInformation is enabled,
    //                                               notify BswM about the state change of the multiblock operation.
    // Calling foreign BswM function is done out of the Schm lock
#if (NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION == STD_ON)
    BswM_NvM_CurrentJobMode(NVM_SERVICE_ID_READ_ALL, NVM_REQ_PENDING);
#endif
}

/**
 * This function indicates whether a single block is sanitized or not by ReadAll after the layout changes.
 *
 * If the enhanced dynamic configuration is not activated, there is no need to sanitize the blocks, so the return
 * value is always TRUE.
 *
 * If the enhanced dynamic configuration is activated, after the layout changes, the blocks need to be sanitized
 * by ReadAll at first to avoid the incompatibility caused by changed layout. Therefore, before the ReadAll is
 * initiated, the single block requests which operate with Nv memory should not be accepted on the blocks, whose
 * compatiblility is not sanitized by the ReadAll. In this case, all of the blocks, which are chosen for ReadAll and
 * not resistant to changed software need to be sanitized by ReadAll. If they are not sanitized, the return value
 * is FALSE.
 *
 * \param[in] idBlock_uo
 * ID of the block for which the sanitization by ReadAll will be checked.
 *
 * \return
 * - TRUE:  If the enhanced dynamic configuration is activated,
 *          the current block is sanitized by ReadAll after the layout change.
 *          If the enhanced dynamic configuration is not activated,
 *          there is no need to sanitize the blocks in ReadAll, so the return value is always TRUE.
 * - FALSE: the current block is not sanitized by ReadAll after the layout change.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2999] the potentially incompatible blocks after layout changes should be
//                                               sanitized by ReadAll at first
boolean NvM_Prv_IsSanitizedByReadAll(NvM_BlockIdType idBlock_uo)
{
    //if the enhanced dynamic configuration is not activated, there is no incompatible problem for single blocks
    //therefore, the default return value is true
    boolean isReadAllInitiated_b = TRUE;

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON))
    //if the enhanced dynamic configuration is activated, check whether the current block has incompatible potential
    if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL) &&
        !NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW))
    {
        //if the current block is chosen as a block to ReadAll
        //and it is not resistant to changed software, then this block has incompatible potential,
        //it will check whether ReadAll is initiated, because the incompatible blocks should be sanitized by ReadAll.
        //if not, the single block requests(which operate on NV memory) on the current block will be blocked
        isReadAllInitiated_b = NvM_Prv_isReadAllInitiated_b;
    }
#else
    (void)idBlock_uo;
#endif

    return isReadAllInitiated_b;
}

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON))
/**
 * \brief
 * This function initializes the status of the ReadAll request.
 * \details
 * The status of the ReadAll request will be initialized with FALSE to indicate that ReadAll request has not been
 * executed yet and potentially all blocks can contain incopatible data, s. NvM_Prv_IsSanitizedByReadAll.
 */
void NvM_Prv_InitializeStateReadAll(void)
{
    NvM_Prv_isReadAllInitiated_b = FALSE;
}
#endif

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

void NvM_Prv_ReadAll_SetState(void)
{
# if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) && (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON))
    NvM_Prv_isReadAllInitiated_b = TRUE;
# endif
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

