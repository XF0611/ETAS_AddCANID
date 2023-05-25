
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Prv_HideRteApi.h"

#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_SetRamBlockStatus_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_SetRamBlockStatus_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged)
{
    // TRACE[NVM453] Service for setting the RAM block status of an NVRAM block
    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_SET_RAM_BLOCK_STATUS;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    BlockData_st.Result_uo = NVM_REQ_OK;
    BlockData_st.idxDataset_u8 = 0;
    // TRACE[NVM405] BlockChanged FALSE -> set RAM block status to INVALID-UNCHANGED
    // TRACE[NVM453] Invalidate the RAM block and mark block as unchanged
    // TRACE[SWS_NvM_00121] Recalculation of the permanent RAM block CRC is required
    // TRACE[SWS_NvM_00121] Recalculation of the permanent RAM block CRC is not finished
    // TRACE[SWS_NvM_00121] Abort a pending recalculation of the permanent RAM block CRC
    BlockData_st.maskBitsToChange_u8 =    NVM_BLOCK_STATE_RAM_VALID
                                        | NVM_BLOCK_STATE_RAM_CHANGED
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ
                                        | NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING;

    if (BlockChanged)
    {
        uint8 maskBitsNewValue_u8;

        if (NvM_Prv_IsBlockSelected(BlockId, NVM_PRV_BLOCK_FLAG_USE_RAM_CRC))
        {
            maskBitsNewValue_u8 =    NVM_BLOCK_STATE_RAM_VALID
                                   | NVM_BLOCK_STATE_RAM_CHANGED
                                   | NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ;
        }
        else
        {
            maskBitsNewValue_u8 =    NVM_BLOCK_STATE_RAM_VALID
                                   | NVM_BLOCK_STATE_RAM_CHANGED;
        }

        // TRACE[NVM406] BlockChanged TRUE -> set RAM block status to VALID-CHANGED
        // TRACE[NVM453] Validate the RAM block and mark block as changed
        // TRACE[SWS_NvM_00121] Recalculation of the permanent RAM block CRC is not finished for blocks with RAM CRC
        // TRACE[SWS_NvM_00121] Recalculation of the permanent RAM block CRC is finished for blocks without RAM CRC
        // TRACE[SWS_NvM_00121] Abort a pending recalculation of the permanent RAM block CRC

        BlockData_st.maskBitsNewValue_u8 = maskBitsNewValue_u8;
    }
    else
    {
        // Invalid, unchanged, no CRC recalc required/ongoing
        BlockData_st.maskBitsNewValue_u8 = 0u;
    }

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_SetRamBlockStatus_CheckParameter;
    // No check for block data required for set RAM block status request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_SetRamBlockStatus_SetBlockData;

    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_nrQueues_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_SetRamBlockStatus_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // -> check whether the selected block has an associated RAM block
    // TRACE[NVM240] If no permanent RAM block has been configured for the block, this function has no effect
    return (NULL_PTR != NvM_Prv_GetPRamBlockAddress(BlockData_pcst->QueueEntry_st.idBlock_uo));
}

static void NvM_Prv_SetRamBlockStatus_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_Prv_Block_SetState(BlockData_pcst->QueueEntry_st.idBlock_uo,
                          BlockData_pcst->maskBitsToChange_u8,
                          BlockData_pcst->maskBitsNewValue_u8);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

