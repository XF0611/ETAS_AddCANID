
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

static boolean NvM_Prv_SetDataIdx_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_SetDataIdx_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex)
{
    // TRACE[NVM448] Service for setting the DataIndex of a dataset NVRAM block
    // TRACE[NVM264] If the block management type is not NVM_BLOCK_DATASET, this function has no effect in production
    // TRACE[NVM146] The dataset index can also select ROM blocks in case the block is of type DATASET
    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_SET_DATA_INDEX;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    BlockData_st.Result_uo = NVM_REQ_OK;
    BlockData_st.idxDataset_u8 = DataIndex;
    BlockData_st.maskBitsToChange_u8 = 0;
    BlockData_st.maskBitsNewValue_u8 = 0;

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_SetDataIdx_CheckParameter;
    // No check for block data required for set dataset index request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_SetDataIdx_SetBlockData;

    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_nrQueues_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // TRACE[NVM263] The permanent RAM block contents are not modified here
    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_SetDataIdx_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    boolean isParameterValid_b = FALSE;

    if (NvM_Prv_ErrorDetection_IsBlockTypeDataset(BlockData_pcst->QueueEntry_st.idService_uo,
                                                  BlockData_pcst->QueueEntry_st.idBlock_uo))
    {
        if (NvM_Prv_ErrorDetection_IsBlockIdxValid(BlockData_pcst->QueueEntry_st.idService_uo,
                                                   BlockData_pcst->QueueEntry_st.idBlock_uo,
                                                   BlockData_pcst->idxDataset_u8))
        {
            isParameterValid_b = TRUE;
        }
    }
    return isParameterValid_b;
}

static void NvM_Prv_SetDataIdx_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_Prv_Block_SetIdxData(BlockData_pcst->QueueEntry_st.idBlock_uo, BlockData_pcst->idxDataset_u8);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

