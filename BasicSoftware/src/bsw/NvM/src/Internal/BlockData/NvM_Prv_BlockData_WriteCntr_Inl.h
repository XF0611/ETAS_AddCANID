
#ifndef NVM_PRV_BLOCKDATA_WRITECNTR_INL_H
#define NVM_PRV_BLOCKDATA_WRITECNTR_INL_H

#include "rba_MemLib.h"

/*
**********************************************************************************************************************
* Inline functions declarations
**********************************************************************************************************************
*/
LOCAL_INLINE uint16 NvM_Prv_Block_AppendWriteCounter(NvM_BlockIdType idBlock_uo,
                                                     uint8* Buffer_pu8);
LOCAL_INLINE void NvM_Prv_Block_ExtractWriteCounter(NvM_BlockIdType idBlock_uo,
                                                    uint8 const* Buffer_pu8,
                                                    uint16* SizeInBytes_pu16);

#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))
LOCAL_INLINE uint32 NvM_Prv_Block_GetWriteCounter(NvM_BlockIdType idBlock_uo);
#endif

/*
**********************************************************************************************************************
* Inline functions
**********************************************************************************************************************
*/
/**
 * This function increases the write counter, appends it to the given buffer and returns the size
 * of the appended write counter.
 *
 * The caller of this function has to make sure that given buffer points exactly
 * where the write counter has to be appended.
 * If user has not configured the write counter for the given block then this function appends nothing and returns
 * the write counter size 0.
 *
 * \param idBlock_uo
 * ID of the block for which the write counter will be appended
 * \param Buffer_pu8
 * Pointer to the buffer where write counter will be appended
 *
 * \return
 * Size of the appended write counter
 */
/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
LOCAL_INLINE uint16 NvM_Prv_Block_AppendWriteCounter(NvM_BlockIdType idBlock_uo,
                                                     uint8* Buffer_pu8)
{
    uint16 SizeWriteCntr_u16 = 0u;

#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))

    if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_CNTR_WRITE))
    {
        SizeWriteCntr_u16 = sizeof(NvM_Prv_cntrWrite_auo[idBlock_uo]);

        // TRACE[BSW_SWS_AR_NVRAMManager_Ext-3290] If user requests a write job prior to a read request
        //                                         then NvM shall not increase the write counter
        if (NvM_Prv_Block_IsReadDone(idBlock_uo))
        {
            ++NvM_Prv_cntrWrite_auo[idBlock_uo];
        }

        rba_MemLib_MemCopy((uint8 const*)(&NvM_Prv_cntrWrite_auo[idBlock_uo]),
                           Buffer_pu8,
                           SizeWriteCntr_u16);
    }

#else

    (void)idBlock_uo;
    (void)Buffer_pu8;

#endif  // NVM_RB_BLOCK_WRITE_COUNTER

    return SizeWriteCntr_u16;
}

/**
 * This function extracts the write counter from the given buffer, copies it
 * into the block specific administrative data and updates the size of the given buffer.
 *
 * The caller of this function has to make sure that given buffer contains the write counter at the end
 * of the given buffer.
 * If user has not configured the write counter for the given block then this function extracts nothing,
 * i.e. the administrative data of the corresponding block remains unchanged and the of the given buffer
 * will not be updated.
 *
 * \param idBlock_uo
 * ID of the block for which the write counter will be extracted
 * \param Buffer_pu8
 * Pointer to the buffer with write counter
 * \param SizeInBytes_pu16
 * Size of the buffer with write counter in bytes
 */
/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
LOCAL_INLINE void NvM_Prv_Block_ExtractWriteCounter(NvM_BlockIdType idBlock_uo,
                                                    uint8 const* Buffer_pu8,
                                                    uint16* SizeInBytes_pu16)
{
#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))

    if (NvM_Prv_IsBlockSelected(idBlock_uo, NVM_PRV_BLOCK_FLAG_CNTR_WRITE))
    {
        uint16 SizeWriteCntr_u16 = sizeof(NvM_Prv_cntrWrite_auo[idBlock_uo]);
        *SizeInBytes_pu16 -= SizeWriteCntr_u16;

        rba_MemLib_MemCopy(&Buffer_pu8[*SizeInBytes_pu16],
                           (uint8*)(&NvM_Prv_cntrWrite_auo[idBlock_uo]),
                           SizeWriteCntr_u16);
    }

#else

    (void)idBlock_uo;
    (void)Buffer_pu8;
    (void)SizeInBytes_pu16;

#endif  // NVM_RB_BLOCK_WRITE_COUNTER
}

#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))
/**
 * This function returns the counter of write requests done for the passed block.
 *
 * This function is available only if user has configured the write counter for at least one block.
 *
 * \attention
 * If the passed block has not been read before then this function will return the initialization value 0.
 * It does not matter whether the read job has succeeded or not. It is only important that the NvM has executed
 * a read job requested by the user.
 *
 * \param[in] idBlock_uo
 * ID of the block for which write requests counter is required
 *
 * \return
 * Counter of write requests
 */
LOCAL_INLINE uint32 NvM_Prv_Block_GetWriteCounter(NvM_BlockIdType idBlock_uo)
{
    return NvM_Prv_cntrWrite_auo[idBlock_uo];
}

#endif  // NVM_RB_BLOCK_WRITE_COUNTER


/* NVM_PRV_BLOCKDATA_WRITECNTR_INL_H */
#endif

