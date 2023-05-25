
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

#if (NVM_RB_AUX_INTERFACE == STD_ON)

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_AuxRead_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_AuxRead_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

# ifdef NVM_CFG_API_RB_AUX_READ_BLOCK

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_RB_AUX_READ_BLOCK(NvM_BlockIdType BlockId, void *NvM_DstPtr);

Std_ReturnType NvM_Rb_AuxReadBlock(NvM_BlockIdType BlockId, void *NvM_DstPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_RB_AUX_READ_BLOCK(BlockId, NvM_DstPtr);
}

# endif

/**
 * This function initiates a read request for an auxiliary user.
 */
# ifdef NVM_CFG_API_RB_AUX_READ_BLOCK
Std_ReturnType NvM_Rb_AuxReadBlockBase(NvM_BlockIdType BlockId, void *NvM_DstPtr)
# else
Std_ReturnType NvM_Rb_AuxReadBlock(NvM_BlockIdType BlockId, void *NvM_DstPtr)
# endif
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2943] Service used from an auxiliary user to copy the data
    //                                               of the NV block to its corresponding RAM block

    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for new block data
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NvM_DstPtr;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_AUX_READ_BLOCK;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Read_e;
    BlockData_st.Result_uo = NVM_REQ_PENDING;
    BlockData_st.idxDataset_u8 = 0;
    BlockData_st.maskBitsToChange_u8 = 0u;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    // TRACE[NVM374] Any assigned NV block is allowed to be read when the block is of type DATASET
    if (NvM_Prv_Block_IsInRom(BlockData_st.QueueEntry_st.idBlock_uo,
                              NvM_Prv_Block_GetIdxDataset(NVM_PRV_AUX_ADMIN_BLOCK)))
    {
        // TRACE[NVM354] The block is of type DATASET and the index is pointing to an ROM block ->
        //               prepare to queue a RESTORE request
        // TRACE[NVM376] Reading ROM blocks for blocks of type DATASET is possible
        BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_AUX_RESTORE_BLOCK_DEFAULTS;
        BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Restore_e;
    }

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_AuxRead_CheckParameter;
    // No check for block data required for auxiliary read block request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_AuxRead_SetBlockData;

    // TRACE[NVM614] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[NVM618] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_Aux_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_AuxRead_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    boolean isParameterValid_b = FALSE;

    // TRACE[NVM622] Report the DET error NVM_E_PARAM_ADDRESS when a NULL pointer is passed
    if (NvM_Prv_ErrorDetection_IsRamBlockAddressValid(BlockData_pcst->QueueEntry_st.idService_uo,
                                                      BlockData_pcst->QueueEntry_st.idBlock_uo,
                                                      BlockData_pcst->QueueEntry_st.BlockData_un.ptrRamBlock_pv))
    {
        // If currently set dataset index is out of range for this block
        // -> reject this request with E_NOT_OK and report this situation to Det if Det reporting is enabled
        // Note: Since the set index will be used only for dataset blocks
        //       it is not necessary to check whether the set index is greater than 0 for non-dataset blocks
        // Note: in the standard API, dataset index limits are already checked in the SetDataIndex service,
        //       but this is not possible in the auxiliary interface because its SetDataIndex service has
        //       no BlockId parameter)
        if (NVM_BLOCK_DATASET == NvM_Prv_GetBlockType(BlockData_pcst->QueueEntry_st.idBlock_uo))
        {
            if (NvM_Prv_ErrorDetection_IsBlockIdxValid(BlockData_pcst->QueueEntry_st.idService_uo,
                                                       BlockData_pcst->QueueEntry_st.idBlock_uo,
                                                       NvM_Prv_Block_GetIdxDataset(NVM_PRV_AUX_ADMIN_BLOCK)))
            {
                isParameterValid_b = TRUE;
            }
        }
        else
        {
            isParameterValid_b = TRUE;
        }
    }
    return isParameterValid_b;
}

static void NvM_Prv_AuxRead_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2929] Use special auxiliary admin block for the request result update
    NvM_Prv_Block_SetRequestResult(NVM_PRV_AUX_ADMIN_BLOCK, BlockData_pcst->Result_uo);
    NvM_Prv_Block_SetRequest(NVM_PRV_AUX_ADMIN_BLOCK, BlockData_pcst->QueueEntry_st.ServiceBit_uo);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif
