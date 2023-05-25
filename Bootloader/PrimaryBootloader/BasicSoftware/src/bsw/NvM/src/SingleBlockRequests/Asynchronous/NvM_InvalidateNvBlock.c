
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

static boolean NvM_Prv_Invalidate_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                                 NvM_Prv_BlockErrors_tuo *Errors_puo);
static void NvM_Prv_Invalidate_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#ifdef NVM_CFG_API_INVALIDATE_NV_BLOCK

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_INVALIDATE_NV_BLOCK(NvM_BlockIdType BlockId);

Std_ReturnType NvM_InvalidateNvBlock(NvM_BlockIdType BlockId)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_INVALIDATE_NV_BLOCK(BlockId);
}

Std_ReturnType NvM_Rb_InvalidateNvBlockBase(NvM_BlockIdType BlockId)

#else

Std_ReturnType NvM_InvalidateNvBlock(NvM_BlockIdType BlockId)

#endif
{
    // TRACE[NVM459] Service to invalidate a NV block

    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_INVALIDATE_NV_BLOCK;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Invalidate_e;
    // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
    BlockData_st.Result_uo = NVM_REQ_PENDING;
    BlockData_st.idxDataset_u8 = 0;
    BlockData_st.maskBitsToChange_u8 = 0;
    BlockData_st.maskBitsNewValue_u8 = 0;

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    // No check for service parameter required for invalidate block request
    ServiceConfiguration_st.CheckParameter_pfct = NULL_PTR;
    ServiceConfiguration_st.CheckBlockData_pfct = NvM_Prv_Invalidate_CheckBlockData;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_Invalidate_SetBlockData;

    // TRACE[NVM638] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[NVM642] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_Standard_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_Invalidate_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                                 NvM_Prv_BlockErrors_tuo *Errors_puo)
{
    // TRACE[NVM424] Check write protection status of this block before enqueuing this request
    // Check that the dataset index does not point to a ROM block if the NvM block is of type DATASET
    // To be compliant with NVM377, do the same if an erase of a ROM block of an NvM-block
    // of type DATASET is attempted
    // TRACE[NVM272] If a write protected block is attempted to be invalidated, E_NOT_OK is returned
    // TRACE[NVM664] If the dataset-index points to a ROM-block (in case the NvM-block is of type DATASET)
    //               and an invalidation is attempted, E_NOT_OK is returned
    return NvM_Prv_Block_IsWriteable(BlockData_pcst->QueueEntry_st.idBlock_uo,
                                     NvM_Prv_Block_GetIdxDataset(BlockData_pcst->QueueEntry_st.idBlock_uo),
                                     Errors_puo);
}

static void NvM_Prv_Invalidate_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_Prv_Block_SetRequestResult(BlockData_pcst->QueueEntry_st.idBlock_uo, BlockData_pcst->Result_uo);
    NvM_Prv_Block_SetRequest(BlockData_pcst->QueueEntry_st.idBlock_uo, BlockData_pcst->QueueEntry_st.ServiceBit_uo);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

