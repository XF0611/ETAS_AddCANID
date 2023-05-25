
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This public function copies the write counter for the given NVRAM block into the provided buffer.
 *
 * \details
 * A block write counter is avialable only if following conditions are fulfilled:
 * - NvM is initialized
 * - ID of the given NVRAM block is valid
 * - Provided buffer for the write counter is valid
 * - block write counter are configured
 *
 * \attention
 * NvM can provide a block write counter only if the corresponding block has been read before (either by NvM_ReadAll or
 * by a single block request). So if user calls this function without a preceding read request then NvM will return 0.
 *
 * \param[in] BlockId
 * ID of the block for which the write counter shall be returned
 * \param[in] cntrWrite_puo
 * Pointer to the buffer for write counter
 *
 * \return
 * Initiation status:
 * - E_OK = current block write counter has been copied into the provided buffer
 * - E_NO_OK = NvM cannot provide current block write counter,
 *             i.e. at least one of the condition above is not fulfilled (see detailed description)
 */
Std_ReturnType NvM_Rb_GetWriteCounter(NvM_BlockIdType idBlock_uo, uint32* cntrWrite_puo)
{
    // TRACE[BSW_SWS_AR_NVRAMManager_Ext-XXXX]
    Std_ReturnType stReturn_uo = E_NOT_OK;

#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))

    // TRACE[NVM602] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_RB_GET_WRITE_COUNTER, idBlock_uo))
    {
        if (NvM_Prv_ErrorDetection_IsBlockIdValid(NVM_SERVICE_ID_RB_GET_WRITE_COUNTER, idBlock_uo, FALSE))
        {
            NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
            ptr_un.ptrCntrWriteBlock_puo = cntrWrite_puo;
            if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_RB_GET_WRITE_COUNTER,
                                                  idBlock_uo,
                                                  NVM_E_PARAM_DATA,
                                                  &ptr_un))
            {
                *cntrWrite_puo = NvM_Prv_Block_GetWriteCounter(idBlock_uo);
                stReturn_uo = E_OK;
            }
        }
    }

#else

    (void)idBlock_uo;
    (void)cntrWrite_puo;

#endif  // NVM_RB_BLOCK_WRITE_COUNTER

    return stReturn_uo;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

