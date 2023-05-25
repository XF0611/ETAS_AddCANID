
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_Service.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_SetBlockLockStatus_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

void NvM_SetBlockLockStatus(NvM_BlockIdType BlockId, boolean BlockLocked)
{
    // TRACE[NVM548] Service for setting the lock status of a permanent RAM block of an NVRAM block

    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for a queue entry
    BlockData_st.QueueEntry_st.idBlock_uo = BlockId;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NULL_PTR;
    BlockData_st.QueueEntry_st.idService_uo = NVM_SERVICE_ID_SET_BLOCK_LOCK_STATUS;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Unspecified_e;
    BlockData_st.Result_uo = NVM_REQ_OK;
    BlockData_st.idxDataset_u8 = 0u;
    BlockData_st.maskBitsToChange_u8 = 0u;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    // Avoid "unused parameter" warning
    (void)BlockLocked;

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_SetBlockLockStatus_CheckParameter;
    // No check for block data required for set block lock status request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    // no block data shall be set
    ServiceConfiguration_st.SetBlockData_pfct = NULL_PTR;

    (void)NvM_Prv_Service_Initiate(NvM_Prv_idQueue_nrQueues_e,
                                   &BlockData_st,
                                   &ServiceConfiguration_st);
}

static boolean NvM_Prv_SetBlockLockStatus_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // TRACE[NVM730] Report the DET error NVM_E_BLOCK_CONFIG when the NVRAM block is configured
    //               with NvMWriteBlockOnce = TRUE
    return NvM_Prv_ErrorDetection_IsBlockWriteProtectionChangeable(BlockData_pcst->QueueEntry_st.idService_uo,
                                                                   BlockData_pcst->QueueEntry_st.idBlock_uo);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

