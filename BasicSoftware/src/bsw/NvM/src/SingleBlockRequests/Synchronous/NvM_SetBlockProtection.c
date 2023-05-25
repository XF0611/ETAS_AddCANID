
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Prv_HideRteApi.h"

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

static boolean NvM_Prv_SetBlockProtection_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_SetBlockProtection_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/* MR12 RULE 8.3 VIOLATION: The parameter identifiers in Rte's generated declaration of this depend on Rte vendor */
Std_ReturnType NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled)
{
    // TRACE[NVM450] Service for setting/resetting the write protection for a NV block
    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_SET_BLOCK_PROTECTION;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    BlockData_st.Result_uo = NVM_REQ_OK;
    BlockData_st.idxDataset_u8 = 0;
    // TRACE[NVM578] Allow to disable the write protection state
    BlockData_st.maskBitsToChange_u8 = NVM_BLOCK_STATE_WRITE_PROT;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    if (ProtectionEnabled)
    {
        // TRACE[NVM016] Set the write protection flag in the administrative block according
        //               to the ProtectionEnabled parameter
        BlockData_st.maskBitsToChange_u8 = NVM_BLOCK_STATE_WRITE_PROT;
        BlockData_st.maskBitsNewValue_u8 = NVM_BLOCK_STATE_WRITE_PROT;
    }

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_SetBlockProtection_CheckParameter;
    // No check for block data required to set block protection
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_SetBlockProtection_SetBlockData;

    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_nrQueues_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_SetBlockProtection_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // TRACE[NVM608] Report the DET error NVM_E_BLOCK_CONFIG when the NVRAM block is configured
    //               with NvMWriteBlockOnce = TRUE
    return NvM_Prv_ErrorDetection_IsBlockWriteProtectionChangeable(BlockData_pcst->QueueEntry_st.idService_uo,
                                                                   BlockData_pcst->QueueEntry_st.idBlock_uo);
}

static void NvM_Prv_SetBlockProtection_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_Prv_Block_SetState(BlockData_pcst->QueueEntry_st.idBlock_uo,
                           BlockData_pcst->maskBitsToChange_u8,
                           BlockData_pcst->maskBitsNewValue_u8);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

