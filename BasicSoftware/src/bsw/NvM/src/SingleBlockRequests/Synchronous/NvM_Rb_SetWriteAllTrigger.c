
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_Service.h"
#include "NvM_Prv_BlockData.h"

#if (NVM_RB_SET_WRITEALL_TRIGGER_API == STD_ON)
/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_SetWriteAllTrg_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_SetWriteAllTrg_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 *********************************************************************
 * NvM_Rb_SetWriteAllTrigger(NvM_BlockIdType BlockId, boolean WriteAllTrigger):
 * Service for triggering a multi-block write without any check.
 * That means from the user point of view, it would be possible to trigger a NvM_WriteAll
 * without caring about:
 *  - the state of the RAM block
 *  - the single job queue
 *  - the current state of the latest job which has been triggered
 *
 * Possible values for WriteAllTrigger:
 * TRUE: This function trigger the NvM_WriteAll for the selected BlockID
 * FALSE:This function will not trigger NvM_WriteAll
 *       Alternative triggers such as a VALID/CHANGED RAM block state are not reset
 *
 * \param    BlockId:         Id of the corresponding block
 * \param    WriteAllTrigger: Enable/Disable the trigger
 * \return   Function result
 * \retval   E_OK:      request was accepted
 * \retval   E_NOT_OK:  request was not accepted
 *********************************************************************
 */
Std_ReturnType NvM_Rb_SetWriteAllTrigger(NvM_BlockIdType BlockId, boolean WriteAllTrigger)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977] Service for triggering a multi-block write job
    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_RB_SET_WRITEALL_TRIGGER;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    BlockData_st.Result_uo = NVM_REQ_OK;
    BlockData_st.idxDataset_u8 = 0;
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977] Reset the flag for the storage of NvM Data during writeall
    BlockData_st.maskBitsToChange_u8 = NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    if (WriteAllTrigger)
    {
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977]
        // Set the flag for the storage of NvM Data during writeall
        BlockData_st.maskBitsToChange_u8 = NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL;
        BlockData_st.maskBitsNewValue_u8 = NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL;
    }

    ServiceConfiguration_st.CheckPendingBlock_b = FALSE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_SetWriteAllTrg_CheckParameter;
    // No check for block data required for read block request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_SetWriteAllTrg_SetBlockData;

    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_nrQueues_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

static boolean NvM_Prv_SetWriteAllTrg_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // -> check whether the selected block has an associated RAM block
    return (NULL_PTR != NvM_Prv_GetPRamBlockAddress(BlockData_pcst->QueueEntry_st.idBlock_uo));
}

static void NvM_Prv_SetWriteAllTrg_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_Prv_Block_SetState(BlockData_pcst->QueueEntry_st.idBlock_uo,
                           BlockData_pcst->maskBitsToChange_u8,
                           BlockData_pcst->maskBitsNewValue_u8);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif
