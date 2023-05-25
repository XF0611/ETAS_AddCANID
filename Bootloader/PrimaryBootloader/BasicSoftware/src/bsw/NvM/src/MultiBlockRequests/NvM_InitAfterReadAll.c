
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#if (NVM_DYNAMIC_CONFIGURATION == STD_ON)
static void NvM_Prv_DynConfig(void);
static boolean NvM_Prv_DynConfig_IsBlockParticipating(NvM_BlockIdType idBlock_uo);
static void NvM_Prv_DynConfig_UpdateBlockData(void);
static void NvM_Prv_DynConfig_FindBlocks(void);
static void NvM_Prv_DynConfig_Finalize(void);
#endif

#if (NVM_RB_INIT_AT_LAYOUT_CHANGE == STD_ON)
static void NvM_Prv_InitAtlayoutChange(void);
static boolean NvM_Prv_InitAtLayoutChange_IsBlockParticipating(NvM_BlockIdType idBlock_uo);
static void NvM_Prv_InitAtLayoutChange_UpdateBlockData(void);
static void NvM_Prv_InitAtLayoutChange_FindBlocks(void);
static void NvM_Prv_InitAtLayoutChange_Finalize(void);
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#if ((NVM_DYNAMIC_CONFIGURATION == STD_ON) || (NVM_RB_INIT_AT_LAYOUT_CHANGE == STD_ON))


void NvM_Prv_InitAfterReadAll(NvM_Prv_JobResult_ten JobResult_en)
{
# if (NVM_DYNAMIC_CONFIGURATION == STD_ON)
    if (   ((NvM_Prv_JobResult_Succeeded_e == JobResult_en) && NvM_Prv_Block_HasIdConfigChanged())
        || ((NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == JobResult_en) && NvM_Prv_Block_HasIdConfigChanged())
        || (NvM_Prv_JobResult_Failed_e == JobResult_en)
       )
    {
        NvM_Prv_DynConfig();
    }
#  if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON)
    if (NvM_Prv_Block_HasIdConfigChanged())
    {
        // If no removing is required but configuration ID must be written
        // then service RemoveNonResistant must be set also for multi-block
        // If this bit was set just before then it will be set here again
        NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_RemoveNonResistant_e);
        NvM_Prv_Block_SetRequest(NVM_RB_CONFIG_ID_BLOCK, NvM_Prv_ServiceBit_RemoveNonResistant_e);
        NvM_Prv_Block_SetRequestResult(NVM_RB_CONFIG_ID_BLOCK, NVM_REQ_PENDING);
    }
#  endif
# endif

# if (NVM_RB_INIT_AT_LAYOUT_CHANGE == STD_ON)
    if (   ((NvM_Prv_JobResult_Succeeded_e == JobResult_en) && NvM_Prv_Block_HasIdConfigChanged())
        || ((NvM_Prv_JobResult_Succeeded_MemIfSkipped_e == JobResult_en) && NvM_Prv_Block_HasIdConfigChanged())
        || (NvM_Prv_JobResult_BlockInvalidated_e == JobResult_en)
        || (NvM_Prv_JobResult_BlockInconsistent_e == JobResult_en)
        || (NvM_Prv_JobResult_Failed_e == JobResult_en)
       )
    {
        NvM_Prv_InitAtlayoutChange();
    }
    if (NvM_Prv_Block_HasIdConfigChanged())
    {
        // If no initialization is required but configuration ID must be written
        // then service InitAtLayoutChange must be set also for multi-block
        // If this bit was set just before then it will be set here again
        NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_InitAtLayoutChange_e);
        NvM_Prv_Block_SetRequest(NVM_RB_CONFIG_ID_BLOCK, NvM_Prv_ServiceBit_InitAtLayoutChange_e);
        NvM_Prv_Block_SetRequestResult(NVM_RB_CONFIG_ID_BLOCK, NVM_REQ_PENDING);
    }
# endif
}

#endif


#if (NVM_DYNAMIC_CONFIGURATION == STD_ON)

/**
 * \brief
 * This function initiates the internal multi-block service to handle all blocks which are non-resistant to SW change
 * if dynamic configuration is enabled by configuration.

 * \details
 * This function uses the service pattern to initiate the multi-block service (s. NvM_Prv_Service_InitiateMulti)
 */
static void NvM_Prv_DynConfig(void)
{
    NvM_Prv_QueueEntry_tst QueueEntry_st;
    NvM_Prv_Service_ConfigurationMulti_tst Configuration_st;

    // put all service parameters in the queue entry structure
    QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL;
    QueueEntry_st.idBlock_uo = NVM_RB_FIRST_USER_BLOCK;
    QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_RemoveNonResistant_e;
    QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;

    // put all required functions in the multi-block service configuration structure
    Configuration_st.UpdateBlockDataForMulti_pfct = NvM_Prv_DynConfig_UpdateBlockData;
    Configuration_st.FinalizeMulti_pfct = NvM_Prv_DynConfig_Finalize;

    NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_nrQueues_e, &QueueEntry_st, &Configuration_st);
}


static boolean NvM_Prv_DynConfig_IsBlockParticipating(NvM_BlockIdType idBlock_uo)
{
    boolean IsBlockPartOfRemoveNonResistant_b = FALSE;
    IsBlockPartOfRemoveNonResistant_b = !NvM_Prv_IsBlockSelected(idBlock_uo,
                                                                 NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW);

    return IsBlockPartOfRemoveNonResistant_b;
}


static void NvM_Prv_DynConfig_UpdateBlockData(void)
{
# if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_ON)
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_RemoveNonResistant_e);
# endif

    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_DynConfig_IsBlockParticipating);
}


static void NvM_Prv_DynConfig_FindBlocks(void)
{
    NvM_BlockIdType idBlock_uo;
    for (idBlock_uo = NVM_RB_FIRST_USER_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        if (NvM_Prv_DynConfig_IsBlockParticipating(idBlock_uo))
        {
            NvM_Prv_Block_ClearRequests(idBlock_uo, (1u << NvM_Prv_ServiceBit_ReadAll_e));
# if (NVM_RB_REMOVE_NON_RESISTANT_BLOCKS == STD_OFF)
            NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_BLOCK_SKIPPED);
# else
            NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_RemoveNonResistant_e);
            NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_PENDING);
# endif
        }
    }
}

/**
 * \brief
 * This function finalizes the initiation of the multi-block request for dynamic configuration
 * as required by the service pattern to initiate the multi-block service (s. NvM_Prv_Service_InitiateMulti).
 *
 * \details
 * The NvM provides 2 kinds of dynamic configuration which require the same finalization procedure:
 * - find all blocks which participate in dynamic configuration (s. NvM_Prv_DynConfig_FindBlocks)
 * - reset function to check whether the NvM is currently enqueuing a multi-block service for a block
 *   (s. NvM_Prv_Block_SetIsNvmEnqueuingForMulti)
 */
static void NvM_Prv_DynConfig_Finalize(void)
{
    NvM_Prv_DynConfig_FindBlocks();
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NULL_PTR);
}

#endif

#if (NVM_RB_INIT_AT_LAYOUT_CHANGE == STD_ON)

/**
 * \brief
 * This function initiates the internal multi-block service to initialize all configured blocks at layout change.

 * \details
 * This function uses the service pattern to initiate the multi-block service (s. NvM_Prv_Service_InitiateMulti)
 */
static void NvM_Prv_InitAtlayoutChange(void)
{
    NvM_Prv_QueueEntry_tst QueueEntry_st;
    NvM_Prv_Service_ConfigurationMulti_tst Configuration_st;

    // put all service parameters in the queue entry structure
    QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL;
    QueueEntry_st.idBlock_uo = NVM_RB_FIRST_USER_BLOCK;
    QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_InitAtLayoutChange_e;
    QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;

    // put all required functions in the multi-block service configuration structure
    Configuration_st.UpdateBlockDataForMulti_pfct = NvM_Prv_InitAtLayoutChange_UpdateBlockData;
    Configuration_st.FinalizeMulti_pfct = NvM_Prv_InitAtLayoutChange_Finalize;

    NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_nrQueues_e, &QueueEntry_st, &Configuration_st);
}

/**
 * \brief
 * This function checks whether a block shall participate in initialization at layout change.

 * \details
 * A block participates in initialization at layout change if following conditions are fulfilled:
 * - Block is configured to be initialized at layout change
 *
 * \param idBlock_uo
 * Id of the block to check participation status
 */
static boolean NvM_Prv_InitAtLayoutChange_IsBlockParticipating(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_SELECT_FOR_INIT_AT_LAYOUT_CHANGE);
}

/**
* \brief
* This function updates all block data for initialization at layout change as required by the service pattern
* to initiate the multi-block service (s. NvM_Prv_Service_InitiateMulti).
*
* \details
* Following block data shall be updated with active scheduler lock:
* - Set request to initialize blocks at layout change as pending for multi-block
* - function to check whether the NvM is currently enqueuing a multi-block service for a block
*/
static void NvM_Prv_InitAtLayoutChange_UpdateBlockData(void)
{
    NvM_Prv_Block_SetRequest(NVM_PRV_MULTI_BLOCK, NvM_Prv_ServiceBit_InitAtLayoutChange_e);
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_InitAtLayoutChange_IsBlockParticipating);
}

/**
 * \brief
 * This function finds all blocks which shall participate in initialization at layout change and
 * updates block data as required.
 *
 * \details
 * - If a block shall participate in initialization at layout change (s. NvM_Prv_InitAtLayoutChange_IsBlockParticipating)
 *   then following block data updates are required:
 *   -> set request to initialize blocks at layout change as pending
 *   -> set result of the request to initialize blocks at layout change to NVM_REQ_PENDING
 *
 * \Attention
 * This function updates block data and it is safe to invoke it with disabled scheduler lock.
 */
static void NvM_Prv_InitAtLayoutChange_FindBlocks(void)
{
    NvM_BlockIdType idBlock_uo;
    for (idBlock_uo = NVM_RB_FIRST_USER_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        if (NvM_Prv_InitAtLayoutChange_IsBlockParticipating(idBlock_uo))
        {
            NvM_Prv_Block_SetRequest(idBlock_uo, NvM_Prv_ServiceBit_InitAtLayoutChange_e);
            NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_PENDING);
        }
    }
}

/**
 * \brief
 * This function finalizes the initiation of the multi-block request for initialization at layout change
 * as required by the service pattern to initiate the multi-block service (s. NvM_Prv_Service_InitiateMulti).
 *
 * \details
 * Initialization at layout change requires following finalization procedure:
 * - find all blocks which participate in initialization at layout change (s. NvM_Prv_InitAtLayoutChange_FindBlocks)
 * - reset function to check whether the NvM is currently enqueuing a multi-block service for a block
 *   (s. NvM_Prv_Block_SetIsNvmEnqueuingForMulti)
 */
static void NvM_Prv_InitAtLayoutChange_Finalize(void)
{
    NvM_Prv_InitAtLayoutChange_FindBlocks();
    NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NULL_PTR);
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

