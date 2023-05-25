
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "rba_MemLib.h"

#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_InternalBuffer.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

# define NVM_START_SEC_VAR_CLEARED_16
# include "NvM_MemMap.h"

static uint16 NvM_Prv_InternalBuffer_UsedSize_au16[NVM_PRV_NR_PARALLEL_JOB];

# define NVM_STOP_SEC_VAR_CLEARED_16
# include "NvM_MemMap.h"

// RAM mirror definition used in explicit synchronization and for Nv CRC and for cryptographic services
// TRACE[NVM512] No NvM mirror is allocated if no block uses explicit synchronization
// TRACE[NVM513] Only one mirror is allocated
#if (defined(NVM_PRV_HAS_RAM_MIRROR) && (NVM_PRV_HAS_RAM_MIRROR == STD_ON))

# if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_OFF)

#  define NVM_START_SEC_VAR_CLEARED_SECURED_CACHE_LINE_ALIGNED_UNSPECIFIED
#  include "NvM_MemMap.h"

uint8 NvM_Prv_RamMirror_au8[NVM_PRV_NR_PARALLEL_JOB * (NVM_PRV_SIZE_RAM_MIRROR + NVM_PRV_CFG_SIZE_CRYPTO_BUFFER)];

#  define NVM_STOP_SEC_VAR_CLEARED_SECURED_CACHE_LINE_ALIGNED_UNSPECIFIED
#  include "NvM_MemMap.h"

# elif (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)

// Runtime RAM Block Configuration feature in combination with explicit synchronization needs definition of
// NvM mirror at runtime
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3029]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3030]
#  define NVM_START_SEC_VAR_CLEARED_SECURED_UNSPECIFIED
#  include "NvM_MemMap.h"

uint8 *NvM_Prv_RamMirror_au8;

#  define NVM_STOP_SEC_VAR_CLEARED_SECURED_UNSPECIFIED
#  include "NvM_MemMap.h"

#  define NVM_START_SEC_VAR_CLEARED_SECURED_32
#  include "NvM_MemMap.h"

uint32 NvM_Prv_RntRamMirrorSize_u32;

#  define NVM_STOP_SEC_VAR_CLEARED_SECURED_32
#  include "NvM_MemMap.h"

# endif

#endif

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

uint8* NvM_Prv_InternalBuffer_GetPtr(void)
{
    uint8* intBuf_pu8;

#if (defined(NVM_PRV_HAS_RAM_MIRROR) && (NVM_PRV_HAS_RAM_MIRROR == STD_ON))
    intBuf_pu8 = NvM_Prv_RamMirror_au8;
#else
    intBuf_pu8 = NULL_PTR;
#endif

    return intBuf_pu8;
}


uint32 NvM_Prv_InternalBuffer_GetSize(void)
{
    uint32 ramMirrorSize_u32 = 0u;

#if ((NVM_PRV_HAS_RAM_MIRROR == STD_ON) && (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON))
    // The user ensures that Ram-Mirror-Size is multiple of NVM_PRV_NR_PARALLEL_JOB
    ramMirrorSize_u32 = NvM_Prv_RntRamMirrorSize_u32 / NVM_PRV_NR_PARALLEL_JOB;
#elif (NVM_PRV_HAS_RAM_MIRROR == STD_ON && (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_OFF))
    ramMirrorSize_u32 = sizeof(NvM_Prv_RamMirror_au8) / NVM_PRV_NR_PARALLEL_JOB;
#else
    ramMirrorSize_u32 = 0u;
#endif

    return ramMirrorSize_u32;
}

uint16* NvM_Prv_InternalBuffer_GetPtrToUsedSize(void)
{
    return NvM_Prv_InternalBuffer_UsedSize_au16;
}

/**
 * \brief
 * This private function copies user data into the internal buffer if required due to the configuration.
 * \details
 * The NvM copies user data into the internal buffer if at least one of following conditions is true:
 * - explicit synchronization is configured for used block and permanent RAM block is in use
 * - NV crc is configured for used block
 * - encryption or signing of user data (or both) is configured for used block
 *
 * \attention
 * Due to the parallel execution of several jobs the internal buffer has a special section for each job.
 * And each job knows which section to use and passes it to this function via parameter IntBuffer_pu8.
 *
 * \param [in] idBlock_uo
 * ID of the used block
 * \param [inout] userData_un
 * Union with pointer to the user data
 * \param [in] IntBuffer_pu8
 * Pointer to the internal buffer to copy user data
 * \param [in] CopyIntoBuf_b
 * Boolean value to set direction of data copying:
 * - TRUE = user data is copied into the internal buffer
 * - FALSE = user data is copied from the internal buffer
 *
 * \return
 * preliminary job result
 */
NvM_Prv_JobResult_ten NvM_Prv_InternalBuffer_CopyData(NvM_BlockIdType idBlock_uo,
                                                      NvM_Prv_ptrRamBlock_tun userData_un,
                                                      uint8* IntBuffer_pu8,
                                                      boolean CopyIntoBuf_b)
{
    NvM_Prv_JobResult_ten stCopy_en = NvM_Prv_JobResult_Succeeded_e;

#if (defined(NVM_PRV_HAS_RAM_MIRROR) && (NVM_PRV_HAS_RAM_MIRROR == STD_ON))

    uint8 const *src_pcu8;
    uint8 *dst_pu8;

    if (CopyIntoBuf_b)
    {
        src_pcu8 = userData_un.ptrRamBlock_pu8;
        dst_pu8 = IntBuffer_pu8;
    }
    else
    {
        src_pcu8 = IntBuffer_pu8;
        dst_pu8 = userData_un.ptrRamBlock_pu8;
    }
    rba_MemLib_MemCopy(src_pcu8, dst_pu8, NvM_Prv_GetBlockSize(idBlock_uo));

#else
    (void)idBlock_uo;
    (void)userData_un;
    (void)IntBuffer_pu8;
    (void)CopyIntoBuf_b;
#endif

    return stCopy_en;
}

#if ((NVM_PRV_HAS_RAM_MIRROR == STD_ON) && (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON))

void NvM_Prv_InitRuntimeRamMirror(uint8* RamMirror_pu8, uint32 RuntimeRamMirrorSize_u32)
{
    // Set explicit synchronization RAM buffer start address and its size
    NvM_Prv_RamMirror_au8 = RamMirror_pu8;
    NvM_Prv_RntRamMirrorSize_u32 = (NVM_PRV_NR_PARALLEL_JOB * RuntimeRamMirrorSize_u32);
}

#endif

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

void NvM_Prv_InternalBuffer_ResetData(void)
{
# if (defined(NVM_PRV_HAS_RAM_MIRROR) && (NVM_PRV_HAS_RAM_MIRROR == STD_ON))

    uint32 i;
    for (i = 0; i < sizeof(NvM_Prv_RamMirror_au8); ++i)
    {
        NvM_Prv_RamMirror_au8[i] = 0;
    }

# endif

    return;
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

