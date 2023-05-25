
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_BlockData.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
// Administrative block (as separate arrays for memory mapping, debugging, and struct packing reasons)
// TRACE[NVM134]

#define NVM_START_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"
/**
 * Global private array of bit-fields which represents the current attribute/status informations of each NVRAM-block.
 * TRACE[NVM134]
 * TRACE[NVM135]
 * This part of administrative data is located in saved zone RAM due to requirements for quick RAM initialization.
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2786]
 *
 * The current state is part of administrative-block as it is required by AUTOSAR.
 * Following states are combined here as a bit-field:
 *
 * - bit 0 = state of the block validness (access mask = NVM_BLOCK_STATE_RAM_VALID)
 *   TRACE[NVM128]
 *   TRACE[NVM132]
 *   TRACE[NVM133]
 *
 * - bit 1 = state of the block change (access mask = NVM_BLOCK_STATE_RAM_CHANGED)
 *   TRACE[NVM344]
 *   TRACE[NVM345]
 *   TRACE[NVM696]
 *   TRACE[NVM472]
 *
 * - bit 2 = state of the WriteAll trigger (access mask = NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL)
 *   TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2978]
 *
 * - bit 3 = state of the block write protection (access mask = NVM_BLOCK_STATE_WRITE_PROT)
 *   TRACE[NVM054]
 *
 * - bit 4 = bit set signals that RAM block data needs to be written during next write operation as it has been changed
 *           (access mask = NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ)
 *
 * - bit 5 = bit set signals that a RAM block CRC recalculation is currently ongoing
 *           (access mask = NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING)
 *
 * - bit 6 = bit set signals that a RAM block CRC recalculation is required for this block
 *           (access mask = NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ)
 *
 * - bit 7 = state of a block for which a read has been performed regardless of whether it has been successful or not
 *           (access mask = NVM_BLOCK_STATE_READ_DONE)
 */
uint8 NvM_Prv_stBlock_au8[NVM_CFG_NR_BLOCKS];

#define NVM_STOP_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"


#define NVM_START_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"

/**
 * Global private array of write counters for each NVRAM-block.
 *
 * It is possible that a block will not participate in NvM_ReadAll due to several features (quick initialization,
 * RAM-block CRC). To make sure that write counter work correctly in these cases NvM locates them in saved zone RAM,
 * s.a. NvM_Prv_stBlock_au8.
 *
 * NvM allocates this array only if user configures write counter for at least one block.
 */
#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))
uint32 NvM_Prv_cntrWrite_auo[NVM_CFG_NR_BLOCKS];
#endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"


#define NVM_START_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
/**
 * Global private array of bit-fields which represents all requests pending on each NVRAM-block.
 *
 * Pending requests are part of administrative-block as it is required by AUTOSAR.
 * There are user initiated requests and internally initiated requests.
 * Mapping of pending requests to single bits is defined as follows:
 * - bit  0 = user request ReadAll
 *            (access mask = (1u << NvM_Prv_ServiceBit_ReadAll_e))
 * - bit  1 = internal request to remove non-resistant blocks after ReadAll
 *            (access mask = (1u << NvM_Prv_ServiceBit_RemoveNonResistant_e))
 * - bit  2 = user request WriteAll
 *            (access mask = (1u << NvM_Prv_ServiceBit_WriteAll_e))
 * - bit  3 = user request FirstInitAll
 *            (access mask = (1u << NvM_Prv_ServiceBit_FirstInitAll_e))
 * - bit  4 = internal request to maintain redundant blocks
 *            (access mask = (1u << NvM_Prv_ServiceBit_Maintain_e))
 * - bit  5 = internal request to initialize blocks upon layout change
 *            (access mask = (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e))
 * - bit  6 = user request ValidateAll
 *            (access mask = (1u << NvM_Prv_ServiceBit_ValidateAll_e))
 * - bit  7 = not used
 * - bit  8 = user request ReadBlock
 *            (access mask = (1u << NvM_Prv_ServiceBit_Read_e))
 * - bit  9 = user request WriteBlock
 *            (access mask = (1u << NvM_Prv_ServiceBit_Write_e))
 * - bit 10 = user request InvalidateBlock
 *            (access mask = (1u << NvM_Prv_ServiceBit_Invalidate_e))
 * - bit 11 = user request EraseBlock
 *            (access mask = (1u << NvM_Prv_ServiceBit_Erase_e))
 * - bit 12 = user request RestoreBlock
 *            (access mask = (1u << NvM_Prv_ServiceBit_Restore_e))
 *
 * TRACE[NVM134]
 */
uint16 NvM_Prv_stRequests_au16[NVM_PRV_NR_BLOCKS_ADMIN];
#define NVM_STOP_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/**
 * Global private array with results of last request for each NVRAM-block.
 *
 * Request results are part of administrative-block as it is required by AUTOSAR.
 * Following request results are defined:
 * - NVM_REQ_OK                (0) = Last asynchronous request finished successfully
 * - NVM_REQ_NOT_OK            (1) = Last asynchronous request finished unsuccessfully
 * - NVM_REQ_PENDING           (2) = An asynchronous request is currently pending
 * - NVM_REQ_INTEGRITY_FAILED  (3) = Data integrity failure for last read operation
 * - NVM_REQ_BLOCK_SKIPPED     (4) = Referenced block skipped in NvM_ReadAll or NvM_WriteAll
 * - NVM_REQ_NV_INVALIDATED    (5) = Referenced block invalidated
 * - NVM_REQ_CANCELED          (6) = Multiblock write request canceled
 * - NVM_REQ_REDUNDANCY_FAILED (7) = Required redundancy of the referenced block lost
 * - NVM_REQ_RESTORED_FROM_ROM (8) = Referenced block restored from ROM
 *
 * TRACE[NVM134]
 * TRACE[NVM136]
 */
NvM_RequestResultType NvM_Prv_stRequestResult_au8[NVM_PRV_NR_BLOCKS_ADMIN];
/**
 * Global private array with indexes of the dataset blocks to get data from the corresponding data array.
 *
 * Request results are part of administrative-block as it is required by AUTOSAR.
 * TRACE[NVM134]
 * TRACE[NVM144]
 * TRACE[NVM160]
 */
uint8 NvM_Prv_idxDataSet_au8[NVM_PRV_NR_BLOCKS_ADMIN];
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"


# define NVM_START_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"
/**
 * Global private variable for the permanent RAM block for the configuration ID.
 */
uint16 NvM_Prv_idConfigStored_u16;
# define NVM_STOP_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

/**
 * Global private function pointer to check whether the NvM is currently enqueuing a multi-block service for a block.
 *
 * In older NvM versions during the initiation of a multi-block service the NvM has set scheduler lock and
 * has not released it until all blocks has been checked whether they shall participate in this multi-block service.
 * This was necessary to make sure that the NvM rejects all single-block services for blocks which already participate
 * in initiated multi-block service.
 * This implementation means that the scheduler lock time is not deterministic and depends directly on the number
 * of configured blocks.
 *
 * To achieve a deterministic and as short as possible scheduler lock time for all multi-block services
 * the NvM defines this function pointer.
 * Now during the initiation of a multi-block service the NvM sets scheduler lock only to set pending status
 * for the multi-block (NVM_PRV_MULTI_BLOCK) and to update this function pointer
 * (see NvM_Prv_Block_SetIsNvmEnqueuingForMulti()).
 * The check for all user blocks whether they participate in requested multi-block service is done without scheduler
 * lock.
 * Now if the user requests a single-block service for a block the NvM checks if the requested block already
 * participates in initiated multi-block service by calling the function defined by this function pointer
 * (see NvM_Prv_Block_IsNvmEnqueuingForMulti() and NvM_Prv_Block_IsPending()).
 */
static NvM_Prv_Block_IsNvmEnqueuingForMulti_tpfct NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct = NULL_PTR;

#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"


/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This function initializes the data for all NVRAM-blocks.
 *
 * \details
 * Block status informations will be initialized according to the following table.
 *
 * Overview on how the block status bits NvM_Prv_stBlock_au8 must be set
 *
 * Uncond.  | Saved Zone | RAM CRC  || Modification for NvM_Prv_stBlock_au8                         || Row No
 * Init Req | Lost       | Enabeld  ||                                                              || (for ref.)
 * ===================================================================================================================
 *     0    |     0      |    0     || No change needed. To optimise the code "RAM CRC Ongoing"     ||  0
 *          |            |          || and "RAM CRC Recalc Required" are reset also this would not  ||
 *          |            |          || be necessary for functionality.                              ||
 * -------------------------------------------------------------------------------------------------------------------
 *     0    |     0      |    1     || Reset "RAM CRC Ongoing", Reset "RAM CRC Recalc Required".    ||  1
 * -------------------------------------------------------------------------------------------------------------------
 *     0    |     1      |    0     || Reset complete stBlock to 0.                                 ||  2
 * -------------------------------------------------------------------------------------------------------------------
 *     0    |     1      |    1     || Reset "RAM CRC Ongoing", Reset "RAM CRC Recalc Required".    ||  3
 * -------------------------------------------------------------------------------------------------------------------
 *     1    |     0      |    0     || Reset complete stBlock to 0.                                 ||  4
 * -------------------------------------------------------------------------------------------------------------------
 *     1    |     0      |    1     || Reset complete stBlock to 0.                                 ||  5
 * -------------------------------------------------------------------------------------------------------------------
 *     1    |     1      |    0     || Reset complete stBlock to 0.                                 ||  6
 * -------------------------------------------------------------------------------------------------------------------
 *     1    |     1      |    1     || Reset complete stBlock to 0.                                 ||  7
 * -------------------------------------------------------------------------------------------------------------------
 *
 * Remark for Row 1 and Row 3:
 * In case "RAM CRC Ongoing" or "RAM CRC Recalc Required" are set, then this means that a RAM CRC recalculation has
 * been requested (by calling NvM_SetRamBlockStatus(idBlock, TRUE)) and that this calculation did not even start or
 * has been interrupted by a soft-reset. In this case the CRC has not been calculated. After the soft-reset it is
 * not sure that the Permanent RAM Block still contains the same data as it was before the reset, so the CRC
 * calculation has to be "cancelled" by resetting above mentioned two flags.
 *
 * If this function resets the complete NvM_Prv_stBlock_au8 for a NVRAM block then it is sure that the corresponding
 * RAM-block is not consistent and the NVRAM block will participate in NvM_ReadAll.
 * In this case NvM resets the write counter.
 * If this function does not reset the complete NvM_Prv_stBlock_au8 for a NVRAM block then it is possible that the
 * corresponding RAM-block is still available.
 * In this case NvM does not initialize the write counter because there are 2 possibilities:
 * - if RAM-block is still consistent then the write counter is consistent too -> no initialization required
 * - if RAM-block is not consistent then the NVRAM block will participate in NvM_ReadAll and this will
 *   initialize the write counter
 *
 * Additionally this function initializes the function pointer NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct.
 *
 * \param[in] isSavedZoneDataLost_b
 * Status information of the data in the saved zone RAM:
 * - true = data in the saved zone RAM is lost after the reset
 * - false = data in the saved zone RAM is unchanged after the reset
 */
void NvM_Prv_Block_InitializeData(boolean isSavedZoneDataLost_b)
{
    NvM_BlockIdType idBlock_uo;

    // Initialize all administrative blocks
    for (idBlock_uo = NVM_PRV_MULTI_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        boolean isRamBlockCrcEnabled_b = NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC);
        boolean isUnconditionalInitRequired_b = NvM_Prv_IsBlockSelected(idBlock_uo,
                                                                        NVM_PRV_BLOCK_FLAG_RAM_INIT_UNCONDITIONAL);

        // Always reset "RAM CRC Ongoing" and "RAM CRC Recalc Req". For blocks without
        // RAM Block CRC configured this doesn't harm.
        NvM_Prv_stBlock_au8[idBlock_uo] &= ~(  NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING
                                             | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ);

        // Reset stBlock to 0, see Row 2 and Rows 4-7 in table shown in function description.
        if (    isUnconditionalInitRequired_b
             || (isSavedZoneDataLost_b && !isRamBlockCrcEnabled_b) )
        {
            NvM_Prv_stBlock_au8[idBlock_uo] = 0u;
#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))
            NvM_Prv_cntrWrite_auo[idBlock_uo] = 0u;
#endif
        }

        // Set write protection for blocks which are configured to be "initially" write protected
        if (isUnconditionalInitRequired_b || isSavedZoneDataLost_b)
        {
            // TRACE[NVM326] Enable write protection flag if configured accordingly
            if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_WRITE_PROTECTED))
            {
                NvM_Prv_stBlock_au8[idBlock_uo] |= NVM_BLOCK_STATE_WRITE_PROT;
            }
        }

        // No request pending
        NvM_Prv_stRequests_au16[idBlock_uo] = 0u;
        // TRACE[NVM470] The default value after reset is NVM_REQ_OK
        NvM_Prv_stRequestResult_au8[idBlock_uo] = NVM_REQ_OK;
        // TRACE[NVM192] The dataset index is initialized to zero during init
        NvM_Prv_idxDataSet_au8[idBlock_uo] = 0u;
    }

    NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct = NULL_PTR;
}

#if (NVM_RB_AUX_INTERFACE == STD_ON)
/**
 * This function initializes administrative block data for auxiliary interface.
 *
 * This initialization resets all pending requests, request result and dataset index for auxiliary interface.
 * This function does not initialize attribute/status informations because auxiliary interface does not have own
 * attribute/status informations.
 * This function exists only if auxiliary interface is enabled by configuration (see NVM_RB_AUX_INTERFACE).
 */
void NvM_Prv_Block_InitializeDataAux(void)
{
    // No request pending
    NvM_Prv_stRequests_au16[NVM_PRV_AUX_ADMIN_BLOCK] = 0u;
    // TRACE[NVM470] The default value after reset is NVM_REQ_OK
    NvM_Prv_stRequestResult_au8[NVM_PRV_AUX_ADMIN_BLOCK] = NVM_REQ_OK;
    // TRACE[NVM192] The dataset index is initialized to zero during init
    NvM_Prv_idxDataSet_au8[NVM_PRV_AUX_ADMIN_BLOCK] = 0u;
}
#endif

/**
 * This function sets the function pointer NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct.
 *
 * \param[in] IsNvmEnqueuingForMulti_pfct
 * function to check whether the NvM is currently enqueuing a multi-block service for a block
 */
void NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_Block_IsNvmEnqueuingForMulti_tpfct IsNvmEnqueuingForMulti_pfct)
{
    NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct = IsNvmEnqueuingForMulti_pfct;
}

/**
 * This function checks whether the NvM is enqueuing the requested block for a multi-block service.
 *
 * \param[in] idBlock_uo
 * Id of the block to be checked whether it is currently participating in a multi-block service
 *
 * \return
 * Participation status:
 * true = requested block participates already in a multi-block service and cannot participate in further user services
 * false = requested block can participate in user services
 */
boolean NvM_Prv_Block_IsNvmEnqueuingForMulti(NvM_BlockIdType idBlock_uo)
{
    boolean IsBlockPartOfMulti_b = FALSE;

    // if aux service is active then idBlock_uo = NVM_PRV_AUX_ADMIN_BLOCK = NVM_CFG_NR_BLOCKS
    if ((idBlock_uo < NVM_CFG_NR_BLOCKS) && (NULL_PTR != NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct))
    {
        IsBlockPartOfMulti_b = NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct(idBlock_uo);
    }

    return IsBlockPartOfMulti_b;
}

/**
 * \brief
 * This function returns the type of NVRAM storage for given block.
 * \details
 * If passed block ID is invalid then this function returns NVM_BLOCK_NATIVE.
 *
 * \param BlockId
 * Block ID for which the type of NVRAM storage shall be returned.
 *
 * \return
 * NVRAM storage type for given block.
 *
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3169]
 */
NvM_BlockManagementType NvM_Rb_GetBlockType(NvM_BlockIdType BlockId)
{
    NvM_BlockManagementType BlockType_uo = NVM_BLOCK_NATIVE;

    if ((BlockId >= NVM_CFG_FIRST_USED_BLOCK) && (BlockId < NVM_CFG_NR_BLOCKS))
    {
        BlockType_uo = NvM_Prv_GetBlockType(BlockId);
    }

    return BlockType_uo;
}
/**
 * \brief
 * This function returns whether the initialization at layout change is enqueued into the queue
 * for multi block requests but is not yet in progress.
 *
 * \param[global in] NvM_Prv_stRequests_au16[NVM_PRV_MULTI_BLOCK]
 * Element of the global array which contains all pending multi block requests.
 *
 * \return
 * Information whether the initialization at layout change is enqueued:
 * - TRUE = initialization at layout change is enqueued and NOT in progress
 * - FALSE = initialization at layout change is NOT enqueued or is already in progress
 *
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3167]
 */
boolean NvM_Rb_IsInitAtLayoutChangeEnqueued(void)
{
    NvM_Prv_idService_tuo idServiceActive_uo = NvM_Prv_GetActiveService();

    return (NvM_Prv_Block_IsRequestPending(NVM_PRV_MULTI_BLOCK, (1u << NvM_Prv_ServiceBit_InitAtLayoutChange_e)) &&
            (NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL != idServiceActive_uo));
}
/**
 * \brief
 * This function returns whether a block is selected for initialization at layout change.
 * \details
 * If passed block ID is invalid then this function returns FALSE, i.e. a block with invalid ID is never selected
 * for initialization at layout change.
 *
 * \param BlockId
 * Block ID for which the selection status for initialization at layout change shall be returned.
 *
 * \return
 * Selection status for initialization at layout:
 * - TRUE = given block is selected for initialization at layout change
 * - FALSE = given block is NOT selected for initialization at layout change
 *
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3164]
 */
boolean NvM_Rb_IsBlockSelectedForInitAtLayoutChange(NvM_BlockIdType BlockId)
{
    boolean isBlockSelected_b = FALSE;

    if ((BlockId >= NVM_CFG_FIRST_USED_BLOCK) && (BlockId < NVM_CFG_NR_BLOCKS))
    {
        isBlockSelected_b = NvM_Prv_IsBlockSelected(BlockId, NVM_PRV_BLOCK_FLAG_SELECT_FOR_INIT_AT_LAYOUT_CHANGE);
    }
    return isBlockSelected_b;
}
/**
 * \brief
 * This function returns whether a block is selected for first initialization.
 * \details
 * If passed block ID is invalid then this function returns FALSE, i.e. a block with invalid ID is never selected
 * for first initialization.
 *
 * \param BlockId
 * Block ID for which the selection status for first initialization shall be returned.
 *
 * \return
 * Selection status for first initialization:
 * - TRUE = given block is selected for first initialization
 * - FALSE = given block is NOT selected for first initialization
 *
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3162]
 */
boolean NvM_Rb_IsBlockSelectedForFirstInitAll(NvM_BlockIdType BlockId)
{
    boolean isBlockSelected_b = FALSE;

    if ((BlockId >= NVM_CFG_FIRST_USED_BLOCK) && (BlockId < NVM_CFG_NR_BLOCKS))
    {
        isBlockSelected_b = NvM_Prv_IsBlockSelected(BlockId, NVM_PRV_BLOCK_FLAG_SELECT_FOR_FIRST_INIT_ALL);
    }
    return isBlockSelected_b;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

