
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_InternalBuffer.h"
#include "NvM_Prv_ExplicitSynchronization.h"

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
#if (NVM_PRV_EXPLICIT_SYNC == STD_ON)
LOCAL_INLINE boolean NvM_Prv_ExplicitSync_IsBlockSizeValid(NvM_BlockIdType idBlock_uo);

LOCAL_INLINE NvM_Prv_JobResult_ten NvM_Prv_ExplicitSync_CallCopyFct(NvM_Prv_ExplicitSync_Copy_tpfct CopyFunction_pfct,
                                                                    uint8* cntrExpSyncOperations_pu8,
                                                                    void* RamBlock_pv);
#endif

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"
/**
 * \brief
 * This private function checks sizes of all configured blocks.
 * \details
 * This function checks whether the NvM can copy all blocks configured for the explicit synchronization
 * into the internal buffer without buffer overflow. If at least one block configured for the explicit synchronization
 * is bigger than internal buffer then the check will fail.
 *
 * The check will be done only if Runtime RAM Block Configuration and explicit synchronization are enabled.
 * Otherwise the check will always succeed (this function will return TRUE).
 *
 * \return
 * - TRUE = all blocks configured for the explicit synchronization can be copied into the internal buffer,
 *          i.e. internal buffer is big enough
 * - FALSE = at least one block configured for the explicit synchronization is bigger than the internal buffer,
 *           i.e. internal buffer is too small
 */
boolean NvM_Prv_ExplicitSync_CheckBlockSizes(void)
{
    boolean areBlockSizesValid_b = TRUE;

#if ((NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON) && (NVM_PRV_EXPLICIT_SYNC == STD_ON))
    NvM_BlockIdType idBlock_uo;

    for (idBlock_uo = NVM_CFG_FIRST_USED_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM))
        {
            if (NvM_Prv_GetBlockSize(idBlock_uo) > NvM_Prv_InternalBuffer_GetSize())
            {
                // BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3100 Indicate bad initialization
                areBlockSizesValid_b = FALSE;
            }
        }
    }
#endif

    return areBlockSizesValid_b;
}

/**
 * \brief
 * This private function copies user data into/from the given buffer via the function defined by the passed function
 * pointer.
 * \details
 * This function returns a job step result and that means that this function was designed to be used only
 * in job step functions.
 * This function copies user data only if explicit synchronization is enabled. If explicit synchronization is disabled
 * then this function always returns TRUE.
 *
 * \attention
 * The caller has to make sure that this function will be invoked only for blocks with enabled explicit synchronization.
 *
 * \param CopyFunction_pfct
 * Pointer to the function which copies user data into/from the given buffer
 * \param idBlock_uo
 * ID of the block for which user data will be copied
 * \param cntrExpSyncOperations_pcu8
 * Pointer to the constant counter for operations to copy user data
 * \param RamBlock_pv
 * Pointer to the buffer for user data
 *
 * \return
 * Job step result, possible values:
 * - NvM_Prv_JobResult_Pending_e   = Copying of user data is still pending
 * - NvM_Prv_JobResult_Failed_e    = Copying of user data has failed
 * - NvM_Prv_JobResult_Succeeded_e = Copying of user data has succeeded
 */
NvM_Prv_JobResult_ten NvM_Prv_ExplicitSync_CopyData(NvM_Prv_ExplicitSync_Copy_tpfct CopyFunction_pfct,
                                                    NvM_BlockIdType idBlock_uo,
                                                    uint8 const* cntrExpSyncOperations_pcu8,
                                                    uint8* RamBlock_pu8)
{
    NvM_Prv_JobResult_ten stExplicitSync_en;

#if (NVM_PRV_EXPLICIT_SYNC == STD_ON)

    // The following union is necessary to remove an otherwise unavoidable compiler warning
    // due to a discarded const qualifier
    union
    {
        uint8 const* cntrOperations_pcu8;
        uint8* cntrOperations_pu8;
    } cntrOperations_un;

    NvM_Prv_ptrRamBlock_tun RamBlock_un;

    cntrOperations_un.cntrOperations_pcu8 = cntrExpSyncOperations_pcu8;
    RamBlock_un.ptrRamBlock_pu8 = RamBlock_pu8;

    // Check whether block size is valid
    if (NvM_Prv_ExplicitSync_IsBlockSizeValid(idBlock_uo))
    {
        stExplicitSync_en = NvM_Prv_ExplicitSync_CallCopyFct(CopyFunction_pfct,
                                                             cntrOperations_un.cntrOperations_pu8,
                                                             RamBlock_un.ptrRamBlock_pv);
    }
    else
    {
        // Explicit synchronization failed
        stExplicitSync_en = NvM_Prv_JobResult_Failed_e;
    }
#else
    (void)CopyFunction_pfct;
    (void)idBlock_uo;
    (void)cntrExpSyncOperations_pcu8;
    (void)RamBlock_pu8;
    stExplicitSync_en = NvM_Prv_JobResult_Succeeded_e;
#endif

    return stExplicitSync_en;
}


#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
#if (NVM_PRV_EXPLICIT_SYNC == STD_ON)

/**
 * \brief
 * This private function checks if configured size of a block exceeds the size of the NvM internal buffer.
 * \details
 * The check will be done only if Runtime RAM Block Configuration is enabled.
 * If Runtime RAM Block Configuration is disabled then the check will always succeed (this function will return TRUE).
 *
 * \param idBlock_uo
 * ID of the block for which configured block size will be checked
 *
 * \return
 * - TRUE = configured block size does not exceed the size of the NvM internal buffer,
 *          user data can be copied safely into/from the internal buffer
 * - FALSE = configured block size exceeds the size of the NvM internal buffer,
 *           user data cannot be copied safely into/from the internal buffer
 */
LOCAL_INLINE boolean NvM_Prv_ExplicitSync_IsBlockSizeValid(NvM_BlockIdType idBlock_uo)
{
    boolean isBlockSizeValid_b = TRUE;

#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3049]
    // Only needed if Runtime Block Configuration feature is active
    // In this case RAM mirror length is configured by user and not automatically by BCT
    // => a length missmatch could occurs
    // Check whether the currently NV block length does not exceeds the RAM mirror length
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3049] block size greater than size of the RAM mirror
    //                                               used for explicit synchronization
    isBlockSizeValid_b = NvM_Prv_ErrorDetection_IsBlockSizeValidForExplicitSync(NvM_Prv_GetActiveService(),
                                                                                idBlock_uo,
                                                                                NvM_Prv_InternalBuffer_GetSize());
#else
    (void)idBlock_uo;
#endif
    return isBlockSizeValid_b;
}

/**
 * \brief
 * This private function calls copy function defined by the passed function pointer to copy user data
 * into/from the given buffer.
 * \details
 * This function returns a job step result and that means that this function was designed to be used only
 * in job step functions.
 * This function succeeds only if copy function has succeeded too. If copy function fails then this function returns
 * that copying of user data is still pending. Only if copy function has failed as often as configured
 * (see NVM_REPEAT_MIRROR_OPERATIONS) this function will fail too.
 *
 * \param CopyFunction_pfct
 * Pointer to the function which copies user data into/from the given buffer
 * \param cntrExpSyncOperations_pu8
 * Pointer to the counter for operations to copy user data
 * \param RamBlock_pv
 * Pointer to the buffer for user data
 *
 * \return
 * Job step result, possible values:
 * - NvM_Prv_JobResult_Pending_e   = Copying of user data is still pending
 * - NvM_Prv_JobResult_Failed_e    = Copying of user data has failed
 * - NvM_Prv_JobResult_Succeeded_e = Copying of user data has succeeded
 */
LOCAL_INLINE NvM_Prv_JobResult_ten NvM_Prv_ExplicitSync_CallCopyFct(NvM_Prv_ExplicitSync_Copy_tpfct CopyFunction_pfct,
                                                                    uint8* cntrExpSyncOperations_pu8,
                                                                    void* RamBlock_pv)
{
    NvM_Prv_JobResult_ten stCopyFct_en;

    // Check whether valid copy function is available
    if ((NULL_PTR != CopyFunction_pfct))
    {
        if (E_OK == CopyFunction_pfct(RamBlock_pv))
        {
            // Copy function for explicit synchronization succeeded
            stCopyFct_en = NvM_Prv_JobResult_Succeeded_e;
            // Reset counter for the next explicit sync operation
            *cntrExpSyncOperations_pu8 = 0;
        }
        else
        {
            // Copy function for explicit synchronization failed

            (*cntrExpSyncOperations_pu8)++;
            // TRACE[NVM579] The maximal number of mirror operation retries is NVM_REPEAT_MIRROR_OPERATIONS
            if (*cntrExpSyncOperations_pu8 > NVM_REPEAT_MIRROR_OPERATIONS)
            {
                // Explicit synchronization failed finally
                stCopyFct_en = NvM_Prv_JobResult_Failed_e;
                // Reset counter for the next explicit sync operation
                *cntrExpSyncOperations_pu8 = 0;
            }
            else
            {
                // -> Retry mirror operation in next cycle
                stCopyFct_en = NvM_Prv_JobResult_Pending_e;
            }
        }
    }
    else
    {
        stCopyFct_en = NvM_Prv_JobResult_Failed_e;
        // Reset counter for the next explicit sync operation
        *cntrExpSyncOperations_pu8 = 0;
    }
    return stCopyFct_en;
}
#endif
