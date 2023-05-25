
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

#ifdef NVM_CFG_API_READ_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_READ_BLOCK(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifdef NVM_CFG_API_READPRAM_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5, 8.6 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern Std_ReturnType NVM_CFG_API_READPRAM_BLOCK(NvM_BlockIdType BlockId);
#endif

static Std_ReturnType NvM_Prv_ReadBlock(NvM_Prv_idService_tuo idService_uo,
                                        NvM_BlockIdType idBlock_uo,
                                        NvM_Rb_VoidPtr RamBlock_po);
static boolean NvM_Prv_Read_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst);
static void NvM_Prv_Read_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst);

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
 * This public function initiates a single-block request to read a NVRAM-block from the non-volatile memory
 * into the corresponding RAM block.
 *
 * \details
 * A read request into the corresponding RAM block will be initiated only if following conditions are fulfilled:
 * - NvM is initialized
 * - ID of the block to be read is valid
 * - Configured length of the block to be read is valid
 * - In case of enabled enhanced dynamic configuration the read request is initiated only if the block to be read
 *   does not participate in NvM_ReadAll or is resistant to changed software or has been read during NvM_ReadAll
 *   (s.a. NvM_Prv_IsSanitizedByReadAll())
 * - Pointer to the RAM block is valid or in case of a NULL pointer the configuration of the block to be read contains
 *   a valid permanent RAM block (includes also explicit synchronization)
 * - No other operation on the block to be read is requested or in progress
 * - Queue for single-block requests is not full
 *
 * If the block to be read is of type DATASET and the current set index is pointing to a ROM block then this function
 * initiates a restore of the configured defaults value instead of reading data from the non-volatile memory.
 * This function does not check whether the current set index is valid because this check is done within
 * the function NvM_SetDataIndex().
 *
 * In case of a successful initiation of the read request into the configured permanent RAM block this function executes
 * following actions:
 * - if configured permanent RAM block is used then its status is set to INVALID
 * - request result is set to pending state (i.e. NVM_GetErrorStatus will return NVM_REQ_PENDING
 *   for the requested block)
 * - bit which globally represents the request in administrative-block is set
 * - read request is enqueued into the standard priority queue
 *
 * \note
 * Actually the initiation of a single-block request to read a NVRAM-block into the corresponding RAM block is
 * implemented in the private function NvM_Prv_ReadBlock().
 * If user has configured an overloaded API for this request then this function invokes the overloaded API and
 * then within the overloaded API the user is responsible to invoke the function NvM_Rb_ReadBlockBase()
 * which invokes the private function NvM_Prv_ReadBlock().
 * And if user has NOT configured an overloaded API for this request then this function invokes the function
 * NvM_Prv_ReadBlock() directly to get the data written from the corresponding RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be read into the corresponding RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the RAM block for read data
 *
 * \return
 * Initiation status:
 * - E_OK = read request into the configured permanent RAM block has been initiated successfully
 * - E_NO_OK = initiation of the read request into the configured permanent RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr)
{
    // TRACE[SWS_NvM_00454] Service to copy the data of the NV block to its corresponding RAM block

#ifdef NVM_CFG_API_READ_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_READ_BLOCK(BlockId, NvM_DstPtr);
#else
    return NvM_Prv_ReadBlock(NVM_SERVICE_ID_READ_BLOCK, BlockId, NvM_DstPtr);
#endif
}

#ifdef NVM_CFG_API_READ_BLOCK
/**
 * \brief
 * This public function provides the possibility to read a NVRAM-block into the corresponding RAM block
 * from the non-volatile memory within the overloaded API for NvM_ReadBlock().
 *
 * \details
 * To read a NVRAM-block this function invokes the private function NvM_Prv_ReadBlock().
 *
 * \attention
 * This function is only available if user has configured an overloaded API for the request to read the corresponding
 * RAM block from the non-volatile memory.
 * \attention
 * The user is responsible to invoke this function wihin the overloaded API to make sure that NvM reads data
 * into the corresponding RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be read from the corresponding RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the corresponding RAM block for read data
 *
 * \return
 * Initiation status:
 * - E_OK = read request into the corresponding RAM block has been initiated successfully
 * - E_NO_OK = initiation of the read request into the corresponding RAM block has failed
 */
Std_ReturnType NvM_Rb_ReadBlockBase(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr)
{
    return NvM_Prv_ReadBlock(NVM_SERVICE_ID_READ_BLOCK, BlockId, NvM_DstPtr);
}
#endif

/**
 * \brief
 * This public function initiates a single-block request to read a NVRAM-block from the non-volatile memory
 * into the configured permanent RAM block.
 *
 * \details
 * A read request into the configured permanent RAM block will be initiated only if following conditions are fulfilled:
 * - NvM is initialized
 * - ID of the block to be read is valid
 * - Configured length of the block to be read is valid
 * - In case of enabled enhanced dynamic configuration the read request is initiated only if the block to be read
 *   does not participate in NvM_ReadAll or is resistant to changed software or has been read during NvM_ReadAll
 *   (s.a. NvM_Prv_IsSanitizedByReadAll())
 * - Configuration of the block to be read contains a valid permanent RAM block (includes also explicit synchronization)
 * - No other operation on the block to be read is requested or in progress
 * - Queue for single-block requests is not full
 *
 * If the block to be read is of type DATASET and the current set index is pointing to a ROM block then this function
 * initiates a restore of the configured defaults value instead of reading data from the non-volatile memory.
 * This function does not check whether the current set index is valid because this check is done within
 * the function NvM_SetDataIndex().
 *
 * In case of a successful initiation of the read request into the configured permanent RAM block this function executes
 * following actions:
 * - status of the configured permanent RAM block is set to INVALID
 * - request result is set to pending state (i.e. NVM_GetErrorStatus will return NVM_REQ_PENDING
 *   for the requested block)
 * - bit which globally represents the request in administrative-block is set
 * - read request is enqueued into the standard priority queue
 *
 * \note
 * Actually the initiation of a single-block request to read a NVRAM-block into the configured permanent RAM block is
 * implemented in the private function NvM_Prv_ReadBlock().
 * If user has configured an overloaded API for this request then this function invokes the overloaded API and
 * then within the overloaded API the user is responsible to invoke the function NvM_Rb_ReadPRAMBlockBase()
 * which invokes the private function NvM_Prv_ReadBlock().
 * And if user has NOT configured an overloaded API for this request then this function invokes the function
 * NvM_Prv_ReadBlock() directly to get the data read into the configured permanent RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be read into the configured permanent RAM block
 *
 * \return
 * Initiation status:
 * - E_OK = read request into the configured permanent RAM block has been initiated successfully
 * - E_NO_OK = initiation of the read request into the configured permanent RAM block has failed,
 *             i.e. at least one of the condition for successful initiation is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_ReadPRAMBlock(NvM_BlockIdType BlockId)
{
    // TRACE[SWS_NvM_00764] Service to copy the data of the NV block to its corresponding permanent RAM block
    // TRACE[SWS_NvM_00765] The job of this service shall copy the data of the NV block to the permanent RAM block

#ifdef NVM_CFG_API_READPRAM_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    return NVM_CFG_API_READPRAM_BLOCK(BlockId);
#else
    return NvM_Prv_ReadBlock(NVM_SERVICE_ID_READ_PRAM_BLOCK, BlockId, NULL_PTR);
#endif
}

#ifdef NVM_CFG_API_READPRAM_BLOCK
/**
 * \brief
 * This public function provides the possibility to read a NVRAM-block into the configured permanent RAM block
 * from the non-volatile memory within the overloaded API for NvM_ReadBlock().
 *
 * \details
 * To read a NVRAM-block this function invokes the private function NvM_Prv_ReadBlock().
 *
 * \attention
 * This function is only available if user has configured an overloaded API for the request
 * to read the configured permanent RAM block from the non-volatile memory.
 * \attention
 * The user is responsible to invoke this function wihin the overloaded API to make sure that NvM reads data
 * into the configured permanent RAM block.
 *
 * \param[in] BlockId
 * ID of the block to be read from the corresponding RAM block
 * \param[in] NvM_DstPtr
 * Pointer to the configured permanent RAM block for read data
 *
 * \return
 * Initiation status:
 * - E_OK = read request into the configured permanent RAM block has been initiated successfully
 * - E_NO_OK = initiation of the read request into the configured permanent RAM block has failed
 */
Std_ReturnType NvM_Rb_ReadPRAMBlockBase(NvM_BlockIdType BlockId)
{
    return NvM_Prv_ReadBlock(NVM_SERVICE_ID_READ_PRAM_BLOCK, BlockId, NULL_PTR);
}
#endif

/**
 * \brief
 * This local private function implements the actual initiation of a single-block request to read a NVRAM-block
 * from the non-volatile memory into the required RAM block.
 *
 * \details
 * This function prepares block data and service configuration and initiates the requested service by calling
 * the NvM internal function NvM_Prv_Service_Initiate() with prepared data.
 * If pointer to the RAM block passed to this function is a NULL pointer then NvM will read required NVRAM-block
 * into the configured permanent RAM block.
 *
 * \param[in] idService_uo
 * ID of the service to be initiated
 * \param[in] BlockId
 * ID of the block to be read into the RAM block
 * \param[in] RamBlock_po
 * Pointer to the RAM block for read data
 *
 * \return
 * Initiation status:
 * - E_OK = read request into the RAM block has been initiated successfully
 * - E_NOT_OK = initiation of the read request into the RAM block has failed
 */
static Std_ReturnType NvM_Prv_ReadBlock(NvM_Prv_idService_tuo idService_uo,
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
    BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Read_e;
    // TRACE[SWS_NvM_00374] Any assigned NV block is allowed to be read when the block is of type DATASET
    // TRACE[SWS_NvM_00769] If block is of type DATASET then set data index will be used
    if (NvM_Prv_Block_IsInRom(BlockData_st.QueueEntry_st.idBlock_uo,
                              NvM_Prv_Block_GetIdxDataset(BlockData_st.QueueEntry_st.idBlock_uo)))
    {
        // TRACE[SWS_NvM_00354] The block is of type DATASET and the index is pointing to an ROM block ->
        //                      prepare to queue a RESTORE request
        // TRACE[SWS_NvM_00772] The block is of type DATASET and the index is pointing to an ROM block ->
        //                      prepare to queue a RESTORE request
        // TRACE[SWS_NvM_00376] Reading ROM blocks for blocks of type DATASET is possible
        BlockData_st.QueueEntry_st.ServiceBit_uo = NvM_Prv_ServiceBit_Restore_e;
    }

    BlockData_st.Result_uo = NVM_REQ_PENDING;
    BlockData_st.idxDataset_u8 = 0;
    BlockData_st.maskBitsToChange_u8 = 0u;
    BlockData_st.maskBitsNewValue_u8 = 0u;

    // determine whether permanent RAM block shall be used
    if (NULL_PTR == BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv)
    {
        // TRACE[SWS_NvM_00278] Only take the permanent RAM block if no temporary RAM block has been specified
        BlockData_st.QueueEntry_st.BlockData_un.ptrRamBlock_pv = NvM_Prv_GetPRamBlockAddress(idBlock_uo);
    }

    ServiceConfiguration_st.CheckPendingBlock_b = TRUE;
    ServiceConfiguration_st.CheckParameter_pfct = NvM_Prv_Read_CheckParameter;
    // No check for block data required for read block request
    ServiceConfiguration_st.CheckBlockData_pfct = NULL_PTR;
    ServiceConfiguration_st.SetBlockData_pfct = NvM_Prv_Read_SetBlockData;

    // TRACE[SWS_NvM_00614] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[SWS_NvM_00618] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    // TRACE[SWS_NvM_00823] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    // TRACE[SWS_NvM_00824] Report the DET error NVM_E_BLOCK_PENDING when block is already queued or in progress
    // TRACE[SWS_NvM_00826] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
    // TRACE[SWS_NvM_00766] Take over given parameters, queue the read request in the job queue and return
    // TRACE[SWS_NvM_00792] Read job for NvM_ReadPRAMBlock can be accepted only if NvM is initialized
    stReturn_uo = NvM_Prv_Service_Initiate(NvM_Prv_idQueue_Standard_e,
                                           &BlockData_st,
                                           &ServiceConfiguration_st);

    // Return whether this operation was successful
    return stReturn_uo;
}

/**
 * \brief
 * This local private function defines validity checks for all significant parameters provided by the user to ensure
 * that the NvM can initiate the request to read a NVRAM-block safely.
 *
 * \details
 * This function implements following special parameter checks:
 * - check if used RAM block (includes also explicit synchronization) is valid,
 *   if this check fails then this function sets the corresponding DET error
 *   (see NvM_Prv_ErrorDetection_IsRamBlockAddressValid())
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
static boolean NvM_Prv_Read_CheckParameter(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    // TRACE[SWS_NvM_00616] Report the DET error NVM_E_PARAM_ADDRESS when no permanent RAM block is configured and
    //                      a NULL pointer is passed via the parameter NvM_DstPtr
    // TRACE[SWS_NvM_00196] Same as NVM616. It includes the case where explicit synchronization is configured
    //                      for this block and no NvM mirror exists
    // TRACE[SWS_NvM_00825] Report the DET error NVM_E_PARAM_ADDRESS when no permanent RAM block is configured
    //                      (includes also explicit synchronization)
    return NvM_Prv_ErrorDetection_IsRamBlockAddressValid(BlockData_pcst->QueueEntry_st.idService_uo,
                                                         BlockData_pcst->QueueEntry_st.idBlock_uo,
                                                         BlockData_pcst->QueueEntry_st.BlockData_un.ptrRamBlock_pv);
}

/**
 * \brief
 * This local private function defines which block data will be set in case of a successful initiation of the request
 * to read a NVRAM-block.
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
 * Pointer to the constant structure with data to be set in case of a successful initiation of the read request
 */
static void NvM_Prv_Read_SetBlockData(NvM_Prv_BlockData_tst const* BlockData_pcst)
{
    NvM_BlockIdType idBlock_uo = BlockData_pcst->QueueEntry_st.idBlock_uo;
    uint8 maskBitsToChange_u8 = BlockData_pcst->maskBitsToChange_u8;
    uint8 maskBitsNewValue_u8 = BlockData_pcst->maskBitsNewValue_u8;

    if (BlockData_pcst->QueueEntry_st.BlockData_un.ptrRamBlock_pv == NvM_Prv_GetPRamBlockAddress(idBlock_uo))
    {
        // TRACE[SWS_NvM_00198] Set the status of the permanent RAM block to INVALID before processing starts
        //                      in case of a READ request
        // TRACE[SWS_NvM_00227] Set the status of the permanent RAM block to INVALID before processing starts
        //                      in case of a block of type DATASET and a restore from ROM is to be performed
        // TRACE[SWS_NvM_00767] Set the status of the permanent RAM block to INVALID before processing starts
        //                      in case of a READ request
        // TRACE[SWS_NvM_00819] Set the status of the permanent RAM block to INVALID before processing starts
        //                      in case of a block of type DATASET and a restore from ROM is to be performed
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

