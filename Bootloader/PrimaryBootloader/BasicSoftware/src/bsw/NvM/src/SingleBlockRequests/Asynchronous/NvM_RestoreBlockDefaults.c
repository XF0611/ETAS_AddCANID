
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

#ifdef NVM_CFG_API_RESTORE_BLOCK_DEFAULTS
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here, not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_RESTORE_BLOCK_DEFAULTS(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifdef NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here, not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS(NvM_BlockIdType BlockId);
#endif

static Std_ReturnType NvM_Prv_RestoreBlockDefaults(NvM_Prv_idService_tuo idService_uo,
                                                   NvM_BlockIdType idBlock_uo,
                                                   NvM_Rb_VoidPtr RamBlock_po);
static boolean NvM_Prv_Restore_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static boolean NvM_Prv_Restore_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                              NvM_Prv_BlockErrors_tuo *Errors_puo);
static void NvM_Prv_Restore_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

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
 * \brief
 * This public function initiates a single-block request to restore default data into the corresponding RAM block.
 *
 * \details
 * A restore request into the corresponding RAM block will be initiated only if following conditions are fulfilled:
 * - NvM is initialized
 * - ID of the block to be restored is valid
 * - Configured length of the block to be restored is valid
 * - In case of enabled enhanced dynamic configuration the restore request is initiated only if the block to be restored
 *   does not participate in NvM_ReadAll or is resistant to changed software or has been read during NvM_ReadAll
 *   (s.a. NvM_Prv_IsSanitizedByReadAll())
 * - Pointer to the RAM block is valid or in case of a NULL pointer the configuration of the block to be restored
 *   contains a valid permanent RAM block (includes also explicit synchronization)
 * - No other operation on the block to be restored is requested or in progress
 * - Queue for single-block requests is not full
 *
 * In case of a successful initiation of the read request into the configured permanent RAM block this function executes
 * following actions:
 * - if configured permanent RAM block is used then its status is set to INVALID
 * - request result is set to pending state (i.e. NVM_GetErrorStatus will return NVM_REQ_PENDING
 *   for the requested block)
 * - bit which globally represents the request in administrative-block is set
 * - restore request is enqueued into the standard priority queue
 *
 * \note
 * Actually the initiation of a single-block request to restore default data into the corresponding RAM block is
 * implemented in the private function NvM_Prv_RestoreBlockDefaults().
 * If user has configured an overloaded API for this request then this function invokes the overloaded API and
 * then within the overloaded API the user is responsible to invoke the function NvM_Rb_RestoreBlockDefaultsBase()
 * which invokes the private function NvM_Prv_RestoreBlockDefaults().
 * And if user has NOT configured an overloaded API for this request then this function invokes the function
 * NvM_Prv_RestoreBlockDefaults() directly to get the data restored into the corresponding RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be restored into the corresponding RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the RAM block for restored data
 *
 * \return
 * Initiation status:
 * - E_OK = restore request into the corresponding RAM block has been initiated successfully
 * - E_NO_OK = initiation of the restore request into the corresponding RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr)
{
    // TRACE[SWS_NvM_00456] Service to restore the default data to its corresponding RAM block
    // TRACE[SWS_NvM_00012] Trigger a read of default data to RAM
    // TRACE[SWS_NvM_00224] The actual restore operation is not done here, just the request for doing it is queued

#ifdef NVM_CFG_API_RESTORE_BLOCK_DEFAULTS
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_RESTORE_BLOCK_DEFAULTS(BlockId, NvM_DstPtr);
#else
    return NvM_Prv_RestoreBlockDefaults(NVM_SERVICE_ID_RESTORE_BLOCK_DEFAULTS, BlockId, NvM_DstPtr);
#endif
}

#ifdef NVM_CFG_API_RESTORE_BLOCK_DEFAULTS
/**
 * \brief
 * This public function provides the possibility to restore default data into the corresponding RAM block
 * within the overloaded API for NvM_RestoreBlockDefaults().
 *
 * \details
 * To restore the default data this function invokes the private function NvM_Prv_RestoreBlockDefaults().
 *
 * \attention
 * This function is only available if user has configured an overloaded API for the request to restore default data
 * into the corresponding RAM block.
 * \attention
 * The user is responsible to invoke this function wihin the overloaded API to make sure that NvM restores default data
 * into the corresponding RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be restored into the corresponding RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the RAM block for restored data
 *
 * \return
 * Initiation status:
 * - E_OK = restore request into the corresponding RAM block has been initiated successfully
 * - E_NO_OK = initiation of the restore request into the corresponding RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_Rb_RestoreBlockDefaultsBase(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr)
{
    return NvM_Prv_RestoreBlockDefaults(NVM_SERVICE_ID_RESTORE_BLOCK_DEFAULTS, BlockId, NvM_DstPtr);
}
#endif

/**
 * \brief
 * This public function initiates a single-block request to restore default data into the permanent RAM block.
 *
 * \details
 * A restore request into the permanent RAM block will be initiated only if following conditions are fulfilled:
 * - NvM is initialized
 * - ID of the block to be restored is valid
 * - Configured length of the block to be restored is valid
 * - In case of enabled enhanced dynamic configuration the restore request is initiated only if the block to be restored
 *   does not participate in NvM_ReadAll or is resistant to changed software or has been read during NvM_ReadAll
 *   (s.a. NvM_Prv_IsSanitizedByReadAll())
 * - Configuration of the block to be restored contains a valid permanent RAM block (includes also explicit synchronization)
 * - No other operation on the block to be restored is requested or in progress
 * - Queue for single-block requests is not full
 *
 * In case of a successful initiation of the read request into the configured permanent RAM block this function executes
 * following actions:
 * - status of the configured permanent RAM block is set to INVALID
 * - request result is set to pending state (i.e. NVM_GetErrorStatus will return NVM_REQ_PENDING
 *   for the requested block)
 * - bit which globally represents the request in administrative-block is set
 * - restore request is enqueued into the standard priority queue
 *
 * \note
 * Actually the initiation of a single-block request to restore default data into the permanent RAM block is
 * implemented in the private function NvM_Prv_RestoreBlockDefaults().
 * If user has configured an overloaded API for this request then this function invokes the overloaded API and
 * then within the overloaded API the user is responsible to invoke the function NvM_Rb_RestorePRAMBlockDefaultsBase()
 * which invokes the private function NvM_Prv_RestoreBlockDefaults().
 * And if user has NOT configured an overloaded API for this request then this function invokes the function
 * NvM_Prv_RestoreBlockDefaults() directly to get the data restored into the permanent RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be restored into the permanent RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the RAM block for restored data
 *
 * \return
 * Initiation status:
 * - E_OK = restore request into the permanent RAM block has been initiated successfully
 * - E_NO_OK = initiation of the restore request into the permanent RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_RestorePRAMBlockDefaults(NvM_BlockIdType BlockId)
{
    // TRACE[SWS_NvM_00813] Service to restore the default data to its corresponding permanent RAM block
    // TRACE[SWS_NvM_00814] The job of this service shall restore the default data to its corresponding permanent RAM block

#ifdef NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS(BlockId);
#else
    return NvM_Prv_RestoreBlockDefaults(NVM_SERVICE_ID_RESTORE_PRAM_BLOCK_DEFAULTS, BlockId, NULL_PTR);
#endif
}

#ifdef NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS
/**
 * \brief
 * This public function provides the possibility to restore default data into the permanent RAM block
 * within the overloaded API for NvM_RestorePRAMBlockDefaults().
 *
 * \details
 * To restore the default data this function invokes the private function NvM_Prv_RestoreBlockDefaults().
 *
 * \attention
 * This function is only available if user has configured an overloaded API for the request to restore default data
 * into the permanent RAM block.
 * \attention
 * The user is responsible to invoke this function wihin the overloaded API to make sure that NvM restores default data
 * into the permanent RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be restored into the permanent RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the RAM block for restored data
 *
 * \return
 * Initiation status:
 * - E_OK = restore request into the permanent RAM block has been initiated successfully
 * - E_NO_OK = initiation of the restore request into the permanent RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_Rb_RestorePRAMBlockDefaultsBase(NvM_BlockIdType BlockId)
{
    return NvM_Prv_RestoreBlockDefaults(NVM_SERVICE_ID_RESTORE_PRAM_BLOCK_DEFAULTS, BlockId, NULL_PTR);
}
#endif

/**
 * \brief
 * This local private function implements the actual initiation of a single-block request to restore default data
 * into the configured permanent RAM block.
 *
 * \details
 * This function prepares block data and service configuration and initiates the requested service by calling
 * the NvM internal function NvM_Prv_Service_Initiate() with prepared data.
 * If pointer to the RAM block passed to this function is a NULL pointer then NvM will restore default data
 * into the configured permanent RAM block.
 *
 * \param[in] idService_uo
 * ID of the service to be initiated
 * \param[in] BlockId
 * ID of the block to be restored
 * \param[in] RamBlock_po
 * Pointer to the RAM block for restored data
 *
 * \return
 * Initiation status:
 * - E_OK = restore request has been initiated successfully
 * - E_NOT_OK = initiation of the restore request has failed
 */
static Std_ReturnType NvM_Prv_RestoreBlockDefaults(NvM_Prv_idService_tuo idService_uo,
                                                   NvM_BlockIdType idBlock_uo,
                                                   NvM_Rb_VoidPtr RamBlock_po)
{

    Std_ReturnType stReturn_uo = E_NOT_OK;
    NvM_Prv_Service_Configuration_tst ServiceConfiguration_st;
    NvM_Prv_BlockData_tst BlockData_st;

    // put all service parameters in a structure for new block data
    BlockData_st.QueueEntry_st.idBlock_uo = idBlock_uo;
    BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = RamBlock_po;
    BlockData_st.QueueEntry_st.idService_uo = idService_uo;
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Restore_e;
    BlockData_st.Result_uo = NVM_REQ_PENDING;
    BlockData_st.idxDataset_u8 = 0;
    BlockData_st.maskBitsToChange_u8 = 0u;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    // determine whether permanent RAM block shall be used
    if (NULL_PTR == BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv)
    {
        // TRACE[SWS_NvM_00435] Only take the permanent RAM block if no temporary RAM block has been specified
        BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(idBlock_uo);
    }

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_Restore_CheckParameter;
    ServiceConfiguration_st.CheckBlockData_pfct = NvM_Prv_Restore_CheckBlockData;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_Restore_SetBlockData;

    // TRACE[SWS_NvM_00625] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[SWS_NvM_00831] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[SWS_NvM_00832] Report the DET error NVM_E_BLOCK_PENDING when another request is pending for the passed block
    // TRACE[SWS_NvM_00834] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    // TRACE[SWS_NvM_00815] Take over given parameters, queue the read request in the job queue and return
    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_Standard_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

/**
 * \brief
 * This local private function defines validity checks for all significant parameters provided by the user to ensure
 * that the NvM can initiate the request to restore a NVRAM-block safely.
 *
 * \details
 * This function implements following special parameter checks:
 * - check if used RAM block (includes also explicit synchronization) is valid,
 *   if this check fails then this function sets the corresponding DET error
 *   (see NvM_Prv_ErrorDetection_IsRamBlockAddressValid())
 * - check if default data is available,
 *   if this check fails then this function sets the corresponding DET error
 *   (see NvM_Prv_ErrorDetection_IsDefaultDataAvailable())
 *
 * \note
 * The NvM implements all parameter checks which are common for all NvM requests (services) within the unit
 * NvM_Service.c, e.g. check for uninitialized NvM or check for invalid ID of the requested block.
 *
 *
 * \param[in] BlockData_pcst
 * Pointer to the constant structure with data to be checked for validity
 *
 * \return
 * Status of the parameter check:
 * TRUE = all checks passed -> initiation of the read is possible
 * FALSE = at least one check fails -> initiation of the read is NOT possible
 */
static boolean NvM_Prv_Restore_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    boolean isParameterValid_b = FALSE;

    // TRACE[SWS_NvM_00435] Only take the permanent RAM block if no temporary RAM block has been specified
    // TRACE[SWS_NvM_00629] Report the DET error NVM_E_PARAM_ADDRESS when no permanent RAM block is configured and
    //                      a NULL pointer is passed
    // TRACE[SWS_NvM_00894] Report the DET error NVM_E_PARAM_ADDRESS for service NvM_RestorePRAMBlockDefaults
    //                      when no permanent RAM block is configured
    if (NvM_Prv_ErrorDetection_IsRamBlockAddressValid(BlockData_pcst->QueueEntry_st.idService_uo,
                                                      BlockData_pcst->QueueEntry_st.idBlock_uo,
                                                      BlockData_pcst->QueueEntry_st.BlockData_un.ptrRamBlock_pv))
    {
        // TRACE[SWS_NvM_00628] Report the DET error NVM_E_BLOCK_WITHOUT_DEFAULTS
        //                      when default data is not available/configured for the referenced NVRAM block
        // TRACE[SWS_NvM_00886] Report the DET error NVM_E_BLOCK_WITHOUT_DEFAULTS
        //                      when default data is not available/configured for the referenced NVRAM block
        // TRACE[SWS_NvM_00884] Reject restore request if no default data is available
        if (NvM_Prv_ErrorDetection_IsDefaultDataAvailable(BlockData_pcst->QueueEntry_st.idService_uo,
                                                          BlockData_pcst->QueueEntry_st.idBlock_uo))
        {
            isParameterValid_b = TRUE;
        }
    }
    return isParameterValid_b;
}

/**
 * \brief
 * This local private function defines validity checks for the block data to ensure that the NvM can initiate
 * the request to restore a NVRAM-block safely.
 *
 * \details
 * This function implements following special block data checks:
 * - check if configured default data is not ambigious (see NvM_Prv_Block_IsRomDataAmbiguous())
 *
 * \note
 * The NvM implements all block data checks which are common for all NvM requests (services) within the unit
 * NvM_Service.c, e.g. check if another request is pending for the requested block.
 *
 * \attention
 * The unit NvM_Service.c invokes this function with active scheduler lock, that means that this function shall NOT
 * invoke any external functions, e.g. DET functions.
 *
 * \param[in] BlockData_pcst
 * Pointer to the constant structure with data to be checked for validity
 * \param[in] Errors_puo
 * Errors which can occure during block data check. (by reference argument)
 *
 * \return
 * Status of the block data check:
 * TRUE = all checks passed -> initiation of the write request is possible
 * FALSE = at least one check fails -> initiation of the write request is NOT possible
 */
/* MR12 RULE 8.13 VIOLATION: All CheckBlockData functions shall have the same type */
static boolean NvM_Prv_Restore_CheckBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                              NvM_Prv_BlockErrors_tuo *Errors_puo)
{
    // TRACE[SWS_NvM_00266] NvM allows the queuing of a NvM_RestoreBlockDefaults request to restore a user block
    //                      which has only an InitBlockCallback (i.e. no ROM block).
    // TRACE[SWS_NvM_00818] Reject the request to restore a block if block is of type DATASET and at least one RAM block
    //                      is configured and data index points at a NV block
    // In case of DATASET blocks with at least one ROM block, recovery is potentially ambiguous
    // because it is not clear which ROM block instance to take
    // (the case of exactly one ROM block is not specially treated here for simplicity reasons)
    // -> reject this request
    (void)*Errors_puo;
    return (!NvM_Prv_Block_IsRomDataAmbiguous(BlockData_pcst->QueueEntry_st.idBlock_uo));
}

/**
 * \brief
 * This local private function defines which block data will be set in case of a successful initiation of the request
 * to restore a NVRAM-block.
 *
 * \details
 * This function sets following block data:
 * - if configured permanent RAM block is used then its status is set to INVALID
 * - request result is set to pending state (i.e. NVM_GetErrorStatus() will return NVM_REQ_PENDING
 *   for the requested block)
 * - bit which globally represents the request in administrative block is set to signal internally
 *   which request is pending
 *
 * \note
 * The NvM sets block data which are common for all NvM requests (services) within the unit NvM_Service.c,
 * e.g. enqueuing of the block into the required queue.
 *
 * \attention
 * The unit NvM_Service.c invokes this function with active scheduler lock, that means that this function shall NOT
 * invoke any external functions, e.g. DET functions.
 *
 * \param[in] BlockData_pcst
 * Pointer to the constant structure with data to be set in case of a successful initiation of the write request
 */
static void NvM_Prv_Restore_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_BlockIdType idBlock_uo = BlockData_pcst->QueueEntry_st.idBlock_uo;
    uint8 maskBitsToChange_u8 = BlockData_pcst->maskBitsToChange_u8;
    uint8 maskBitsNewValue_u8 = BlockData_pcst->maskBitsNewValue_u8;

    if (BlockData_pcst->QueueEntry_st.BlockData_un.ptrRamBlock_pv == NvM_Prv_GetPRamBlockAddress(idBlock_uo))
    {
        // TRACE[SWS_NvM_00227] Set the status of the permanent RAM block to INVALID before processing starts
        // TRACE[SWS_NvM_00819] Set the status of the permanent RAM block to INVALID before processing starts
        maskBitsToChange_u8 = NVM_BLOCK_STATE_RAM_VALID;
        maskBitsNewValue_u8 = 0u;
    }
    NvM_Prv_Block_SetState(idBlock_uo, maskBitsToChange_u8, maskBitsNewValue_u8);

    // TRACE[SWS_NvM_00185] Set single block request result to NVM_REQ_PENDING on successful enqueuing
    NvM_Prv_Block_SetRequestResult(idBlock_uo, BlockData_pcst->Result_uo);
    NvM_Prv_Block_SetRequest(idBlock_uo, BlockData_pcst->QueueEntry_st.ServiceBit_uo);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

