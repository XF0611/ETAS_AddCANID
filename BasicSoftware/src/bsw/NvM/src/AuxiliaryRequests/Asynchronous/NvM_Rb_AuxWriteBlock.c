
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

static boolean NvM_Prv_AuxWrite_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static boolean NvM_Prv_AuxWrite_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                               NvM_Prv_BlockErrors_tuo *Errors_puo);
static void NvM_Prv_AuxWrite_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

# ifdef NVM_CFG_API_RB_AUX_WRITE_BLOCK

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_RB_AUX_WRITE_BLOCK(NvM_BlockIdType BlockId, const void *NvM_SrcPtr);

Std_ReturnType NvM_Rb_AuxWriteBlock(NvM_BlockIdType BlockId, const void *NvM_SrcPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_RB_AUX_WRITE_BLOCK(BlockId, NvM_SrcPtr);
}

# endif

/**
 * This function initiates a write request for an auxiliary user.
 */
# ifdef NVM_CFG_API_RB_AUX_WRITE_BLOCK
Std_ReturnType NvM_Rb_AuxWriteBlockBase(NvM_BlockIdType BlockId, const void *NvM_SrcPtr)
# else
Std_ReturnType NvM_Rb_AuxWriteBlock(NvM_BlockIdType BlockId, const void *NvM_SrcPtr)
# endif
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2944] Service used from an auxiliary user to copy the data
    //                                               of the RAM block to its corresponding NV block

    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // The following union is necessary to remove an otherwise unavoidable compiler warning
    // due to a discarded const qualifier
    union
    {
        const void *pcv;
        void *pv;
    } adrRamBlock_un;
    adrRamBlock_un.pcv = NvM_SrcPtr;

    // put all service parameters in a structure for new block data
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = adrRamBlock_un.pv;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_AUX_WRITE_BLOCK;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Write_e;
    BlockData_st.Result_uo = NVM_REQ_PENDING;
    BlockData_st.idxDataset_u8 = 0;
    BlockData_st.maskBitsToChange_u8 = 0u;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_AuxWrite_CheckParameter;
    ServiceConfiguration_st.CheckBlockData_pfct = NvM_Prv_AuxWrite_CheckBlockData;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_AuxWrite_SetBlockData;

    // TRACE[NVM619] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[NVM624] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_Aux_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_AuxWrite_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
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

static boolean NvM_Prv_AuxWrite_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                               NvM_Prv_BlockErrors_tuo *Errors_puo)
{
    // TRACE[NVM209] Check write protection status of this block before enqueuing this request
    // TRACE[NVM377] Check that the dataset index does not point to a ROM block if the NvM block is of type DATASET
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2937] Write protection shall be respected by an auxiliary user
    // TRACE[NVM377] Do the same if a writing to a ROM block of an NvM block of type DATASET is attempted
    // TRACE[NVM217] If a write protected block is attempted to be written, E_NOT_OK is returned
    // TRACE[NVM377] If the dataset index points to a ROM block (in case the NvM block is of type DATASET) and
    //               a write is attempted, E_NOT_OK is returned
    // TRACE[NVM375] Writing to an NV block of a block of type DATASET is not possible
    //               if the block is write protected
    return NvM_Prv_Block_IsWriteable(BlockData_pcst->QueueEntry_st.idBlock_uo,
                                     NvM_Prv_Block_GetIdxDataset(NVM_PRV_AUX_ADMIN_BLOCK),
                                     Errors_puo);
}

static void NvM_Prv_AuxWrite_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // TRACE[NVM185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2929] Use special auxiliary admin block for the request result update
    NvM_Prv_Block_SetRequestResult(NVM_PRV_AUX_ADMIN_BLOCK, BlockData_pcst->Result_uo);
    NvM_Prv_Block_SetRequest(NVM_PRV_AUX_ADMIN_BLOCK, BlockData_pcst->QueueEntry_st.ServiceBit_uo);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif  // NVM_RB_AUX_INTERFACE == STD_ON
