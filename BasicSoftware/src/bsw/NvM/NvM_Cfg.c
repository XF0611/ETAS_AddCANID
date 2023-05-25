

// TRACE[NVM321]
// NvM file containing all configuration parameters which are to be implemented as constants

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Prv_HideRteApi.h"

#include "MemIf.h"
// TRACE[NVM089]
// Check version compatibility of included header files
#if (!defined(MEMIF_AR_RELEASE_MAJOR_VERSION) || (MEMIF_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
    #error "AUTOSAR major version undefined or mismatched"
#endif

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
# include "Csm.h"
// TRACE[NVM089] Check version compatibility of included header files
# if (!defined(CSM_AR_RELEASE_MAJOR_VERSION) || (CSM_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(CSM_AR_RELEASE_MINOR_VERSION) || (CSM_AR_RELEASE_MINOR_VERSION < 3))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif

#include "NvM_Prv.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_InternalBuffer.h"

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2997]
// Include custumer/user specific declarations
#include "EcuM.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

// RAM block of NVRAM block NvMBlock_LogisticData
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 NvBlock_ECU_LogisticData[];

// Explicit sync read callback of NVRAM block NVM_ID_DEM_GENERIC_NV_DATA
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_GenericNVDataReadRamBlockFromNvCallback(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_DEM_GENERIC_NV_DATA
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_GenericNVDataWriteRamBlockToNvCallback(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_0
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback0(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_0
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback0(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_1
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback1(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_1
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback1(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_2
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback2(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_2
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback2(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_3
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback3(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_3
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback3(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_4
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback4(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_4
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback4(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_5
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback5(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_5
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback5(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_6
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback6(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_6
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback6(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_7
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback7(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_7
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback7(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_8
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback8(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_8
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback8(void* NvMBuffer);

// Explicit sync read callback of NVRAM block NVM_ID_EVMEM_LOC_9
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback9(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVMEM_LOC_9
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback9(void* NvMBuffer);

// RAM block of NVRAM block NvMBlockDescriptor_ECUConfiguration
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 NvBlock_ECU_Configuration[];

// Explicit sync read callback of NVRAM block NVM_ID_EVT_STATUSBYTE
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EventStatusByteReadRamBlockFromNvCallback(void* NvMBuffer);

// Explicit sync write callback of NVRAM block NVM_ID_EVT_STATUSBYTE
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Dem_EventStatusByteWriteRamBlockToNvCallback(void* NvMBuffer);

// RAM block of NVRAM block NV_ExternalReprogrammingRequestFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock[];

// ROM block start address of NVRAM block NV_ExternalReprogrammingRequestFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock[];

// Single block callback of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock_ShutdownRamBlock[];

// ROM block start address of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock_ShutdownRomBlock[];

// Single block callback of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreCyclicBlock(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreCyclicBlock_CyclicRamBlock[];

// ROM block start address of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreCyclicBlock_CyclicRomBlock[];

// Single block callback of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreImmediateBlock(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreImmediateBlock_ImmediateRamBlock[];

// ROM block start address of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreImmediateBlock_ImmediateRomBlock[];

/*
 **********************************************************************************************************************
 * Assertions
 **********************************************************************************************************************
*/

/* Block length check for Ram block data address of NVRAM block ECUM_CFG_NVM_BLOCK
   Checks the configured block length against the actual data size determined by the included header-file. */
/* MR12 DIR 1.1, RULE 1.1, 8.6, 14.3 VIOLATION: RB compiler assert is using mechanisms which makes the violations necessary. */
COMPILER_RB_ASSERT_GLOBAL(sizeof(EcuM_Rb_dataShutdownInfo_st)==(4u), ECUM_CFG_NVM_BLOCK_CheckRamBlockLength)

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

// Array containing (persistent Id, block Id) couples sorted by ascendant persistent Ids
const NvM_Prv_PersId_BlockId_tst NvM_Prv_PersId_BlockId_acst[NVM_PRV_NR_PERSISTENT_IDS] =
{
    //{PersId, BlockId}
    {560u, 9u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_4
    {945u, 10u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_5
    {2623u, 20u}, // NvMConf_NvMBlockDescriptor_NvBlock_NvBlockDescriptor_StoreImmediateBlock
    {4154u, 12u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_7
    {4539u, 11u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_6
    {9383u, 6u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_1
    {9510u, 5u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_0
    {13997u, 7u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_2
    {14124u, 8u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_3
    {17323u, 19u}, // NvMConf_NvMBlockDescriptor_NvBlock_NvBlockDescriptor_StoreCyclicBlock
    {19105u, 4u}, // NvMConf_NvMBlockDescriptor_NVM_ID_DEM_GENERIC_NV_DATA
    {19741u, 2u}, // NvMConf_NvMBlockDescriptor_NvMBlock_LogisticData
    {20742u, 16u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVT_STATUSBYTE
    {27275u, 14u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_9
    {27402u, 13u}, // NvMConf_NvMBlockDescriptor_NVM_ID_EVMEM_LOC_8
    {32699u, 17u}, // NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock
    {39401u, 15u}, // NvMConf_NvMBlockDescriptor_NvMBlockDescriptor_ECUConfiguration
    {42798u, 18u}, // NvMConf_NvMBlockDescriptor_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock
    {62199u, 3u}, // NvMConf_NvMBlockDescriptor_ECUM_CFG_NVM_BLOCK
};

// TRACE[NVM028_Conf]
// Structure containing common configuration options
const NvM_Prv_Common_tst NvM_Prv_Common_cst = {NULL_PTR, NULL_PTR, NULL_PTR};

// TRACE[NVM061_Conf]
// Array containing block descriptors
// TRACE[NVM140]
// The block descriptor contents are completely determined by configuration
const NvM_Prv_BlockDescriptor_tst NvM_Prv_BlockDescriptors_acst[NVM_CFG_NR_BLOCKS] =
{
    {
        // Block descriptor of NVRAM block NvM_MultiBlock (NvM block ID: 0):
        0u, // MemIf block ID (this block is not stored on any memory device)
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[0]), // Block length calculated on compile time
        1u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[0]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        0u,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        0u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvM_ConfigId (NvM block ID: 1):
        0u, // MemIf block ID (this block is not stored on any memory device)
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[1]), // Block length calculated on compile time
        2u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[1]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        0u,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        0u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvMBlock_LogisticData (NvM block ID: 2, persistent ID: 19741):
        FeeConf_FeeBlockConfiguration_NvMBlock_LogisticData, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[2]), // Block length calculated on compile time
        128u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[2]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        19741u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block ECUM_CFG_NVM_BLOCK (NvM block ID: 3, persistent ID: 62199):
        FeeConf_FeeBlockConfiguration_ECUM_CFG_NVM_BLOCK, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[3]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[3]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        &EcuM_Rb_NvMSingleBlockCallbackFunction, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        62199u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_DEM_GENERIC_NV_DATA (NvM block ID: 4, persistent ID: 19105):
        FeeConf_FeeBlockConfiguration_NVM_ID_DEM_GENERIC_NV_DATA, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[4]), // Block length calculated on compile time
        (DEM_SIZEOF_VAR(Dem_GenericNvData) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[4]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_GenericNVDataReadRamBlockFromNvCallback, // Explicit sync read callback
        &Dem_GenericNVDataWriteRamBlockToNvCallback, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        19105u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_0 (NvM block ID: 5, persistent ID: 9510):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_0, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[5]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[5]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback0, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback0, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        9510u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_1 (NvM block ID: 6, persistent ID: 9383):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_1, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[6]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[6]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback1, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback1, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        9383u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_2 (NvM block ID: 7, persistent ID: 13997):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_2, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[7]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[7]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback2, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback2, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        13997u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_3 (NvM block ID: 8, persistent ID: 14124):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_3, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[8]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[8]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback3, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback3, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        14124u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_4 (NvM block ID: 9, persistent ID: 560):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_4, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[9]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[9]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback4, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback4, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        560u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_5 (NvM block ID: 10, persistent ID: 945):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_5, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[10]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[10]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback5, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback5, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        945u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_6 (NvM block ID: 11, persistent ID: 4539):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_6, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[11]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[11]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback6, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback6, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        4539u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_7 (NvM block ID: 12, persistent ID: 4154):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_7, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[12]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[12]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback7, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback7, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        4154u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_8 (NvM block ID: 13, persistent ID: 27402):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_8, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[13]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[13]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback8, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback8, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        27402u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVMEM_LOC_9 (NvM block ID: 14, persistent ID: 27275):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVMEM_LOC_9, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[14]), // Block length calculated on compile time
        (DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[14]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EvMemNvmReadRamBlockFromNvCallback9, // Explicit sync read callback
        &Dem_EvMemNvMWriteRamBlockToNvCallback9, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        27275u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvMBlockDescriptor_ECUConfiguration (NvM block ID: 15, persistent ID: 39401):
        FeeConf_FeeBlockConfiguration_NvMBlockDescriptor_ECUConfiguration, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[15]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        2u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[15]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_REDUNDANT, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_FIRST_INIT_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        39401u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NVM_ID_EVT_STATUSBYTE (NvM block ID: 16, persistent ID: 20742):
        FeeConf_FeeBlockConfiguration_NVM_ID_EVT_STATUSBYTE, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[16]), // Block length calculated on compile time
        (DEM_SIZEOF_VAR(Dem_AllEventsStatusByte) + 0), // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        0u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[16]), // RAM block data address calculated on compile time
        NULL_PTR, // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        &Dem_EventStatusByteReadRamBlockFromNvCallback, // Explicit sync read callback
        &Dem_EventStatusByteWriteRamBlockToNvCallback, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        20742u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 17, persistent ID: 32699):
        FeeConf_FeeBlockConfiguration_NV_ExternalReprogrammingRequestFlagBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[17]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[17]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock), // ROM block data address
        NULL_PTR, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_NoCrc_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        32699u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock (NvM block ID: 18, persistent ID: 42798):
        FeeConf_FeeBlockConfiguration_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[18]), // Block length calculated on compile time
        96u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[18]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock_ShutdownRomBlock), // ROM block data address
        &Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_RAM_CRC |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_16_Bit_e, // CRC type used for this block
        0u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        42798u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock (NvM block ID: 19, persistent ID: 17323):
        FeeConf_FeeBlockConfiguration_NvBlock_NvBlockDescriptor_StoreCyclicBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[19]), // Block length calculated on compile time
        1024u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[19]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreCyclicBlock_CyclicRomBlock), // ROM block data address
        &Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreCyclicBlock, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_RAM_CRC |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_16_Bit_e, // CRC type used for this block
        1u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        17323u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
    {
        // Block descriptor of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock (NvM block ID: 20, persistent ID: 2623):
        FeeConf_FeeBlockConfiguration_NvBlock_NvBlockDescriptor_StoreImmediateBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[20]), // Block length calculated on compile time
        384u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[20]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Rte_RomBlk_NvBlock_NvBlockDescriptor_StoreImmediateBlock_ImmediateRomBlock), // ROM block data address
        &Rte_NvMNotifyJobFinished_CPT_NvBlock_NvBlockDescriptor_StoreImmediateBlock, // Single block callback
        NULL_PTR, // Single block start callback
        NULL_PTR, // Initialization callback
        NULL_PTR, // Explicit sync read callback
        NULL_PTR, // Explicit sync write callback
        NVM_BLOCK_NATIVE, // Block management type
        1u, // Job priority (0: Immediate, 1: Standard)
        // Block flags
        (uint32)NVM_PRV_BLOCK_FLAG_USE_RAM_CRC |
        (uint32)NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW |
        (uint32)NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL,
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
        NvM_Prv_Crc_Type_16_Bit_e, // CRC type used for this block
        2u, // Index of the RAM block CRC in the corresponding array with same CRC type
#endif
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
        2623u, // Persistent ID
        {
            NULL_PTR, // Pointer to associated data used for AEAD encryption
            {
                0u, // Csm job ID to generate random number
                0u, // Csm job ID to encrypt user data
                0u, // Csm job ID to decrypt user data
                0u, // Csm job ID to generate signature
                0u, // Csm job ID to verify signature
            },
            {
                0u, // Length of the random number
                0u, // Length of encrypted user data
                0u, // Length of encrypted user data
                0u, // Length of the signature
                0u, // Length of the signature
            },
            0u, // Length of associated data used for AEAD encryption
            0u, // Length of the tag stored for AEAD encryption
        }
#endif
    },
};

#if (defined(NVM_CFG_CRC_NR_RAM_BLOCKS) && (NVM_CFG_CRC_NR_RAM_BLOCKS > 0u))
NvM_BlockIdType const NvM_Prv_idBlockRamCrc_cauo[NVM_CFG_CRC_NR_RAM_BLOCKS] =
{
    18u,
    19u,
    20u,
};
#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028]
// Runtime Calculation feature disabled
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_OFF)
// Used to calculate the NV block lengths on compile time
// This variable is mapped into the block descriptor NvM_Prv_BlockDescriptors_acst
const uint16 NvM_Prv_BlockLengths_acu16[NVM_CFG_NR_BLOCKS] =
{
    // Block length of NVRAM block NvM_MultiBlock (NvM block ID: 0)
    1u,
    // Block length of NVRAM block NvM_ConfigId (NvM block ID: 1)
    2u,
    // Block length of NVRAM block NvMBlock_LogisticData (NvM block ID: 2)
    128u,
    // Block length of NVRAM block ECUM_CFG_NVM_BLOCK (NvM block ID: 3)
    4u,
    // Block length of NVRAM block NVM_ID_DEM_GENERIC_NV_DATA (NvM block ID: 4)
    DEM_SIZEOF_VAR(Dem_GenericNvData),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_0 (NvM block ID: 5)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_1 (NvM block ID: 6)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_2 (NvM block ID: 7)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_3 (NvM block ID: 8)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_4 (NvM block ID: 9)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_5 (NvM block ID: 10)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_6 (NvM block ID: 11)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_7 (NvM block ID: 12)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_8 (NvM block ID: 13)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NVM_ID_EVMEM_LOC_9 (NvM block ID: 14)
    DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType),
    // Block length of NVRAM block NvMBlockDescriptor_ECUConfiguration (NvM block ID: 15)
    4u,
    // Block length of NVRAM block NVM_ID_EVT_STATUSBYTE (NvM block ID: 16)
    DEM_SIZEOF_VAR(Dem_AllEventsStatusByte),
    // Block length of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 17)
    4u,
    // Block length of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock (NvM block ID: 18)
    96u,
    // Block length of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock (NvM block ID: 19)
    1024u,
    // Block length of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock (NvM block ID: 20)
    384u,
};
// Used to calculate the RAM block data addresses on compile runtime
// This variable is mapped into the block descriptor NvM_Prv_BlockDescriptors_acst
void * const NvM_Prv_RamBlockAdr_acpv[NVM_CFG_NR_BLOCKS] =
{
    // Permanent RAM address of NVRAM block NvM_MultiBlock (NvM block ID: 0)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NULL_PTR,
    // Permanent RAM address of NVRAM block NvM_ConfigId (NvM block ID: 1)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NULL_PTR,
    // Permanent RAM address of NVRAM block NvMBlock_LogisticData (NvM block ID: 2)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(NvBlock_ECU_LogisticData),
    // Permanent RAM address of NVRAM block ECUM_CFG_NVM_BLOCK (NvM block ID: 3)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(EcuM_Rb_dataShutdownInfo_st),
    // Permanent RAM address of NVRAM block NVM_ID_DEM_GENERIC_NV_DATA (NvM block ID: 4)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_0 (NvM block ID: 5)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_1 (NvM block ID: 6)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_2 (NvM block ID: 7)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_3 (NvM block ID: 8)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_4 (NvM block ID: 9)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_5 (NvM block ID: 10)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_6 (NvM block ID: 11)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_7 (NvM block ID: 12)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_8 (NvM block ID: 13)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NVM_ID_EVMEM_LOC_9 (NvM block ID: 14)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NvMBlockDescriptor_ECUConfiguration (NvM block ID: 15)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(NvBlock_ECU_Configuration),
    // Permanent RAM address of NVRAM block NVM_ID_EVT_STATUSBYTE (NvM block ID: 16)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)NvM_Prv_RamMirror_au8,
    // Permanent RAM address of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 17)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock),
    // Permanent RAM address of NVRAM block NvBlock_NvBlockDescriptor_StoreAtShutdownBlock (NvM block ID: 18)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreAtShutdownBlock_ShutdownRamBlock),
    // Permanent RAM address of NVRAM block NvBlock_NvBlockDescriptor_StoreCyclicBlock (NvM block ID: 19)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreCyclicBlock_CyclicRamBlock),
    // Permanent RAM address of NVRAM block NvBlock_NvBlockDescriptor_StoreImmediateBlock (NvM block ID: 20)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Rte_RamBlk_NvBlock_NvBlockDescriptor_StoreImmediateBlock_ImmediateRamBlock),
};
#endif

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"
/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028]
// Runtime Calculation feature enabled
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
# define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"
// Used to calculate the NV block lengths and RAM block data addresses on runtime
// These variables are mapped into the block descriptor NvM_Prv_BlockDescriptors_acst
uint16 NvM_Prv_BlockLengths_au16[NVM_CFG_NR_BLOCKS];
void *NvM_Prv_RamBlockAdr_apv[NVM_CFG_NR_BLOCKS];
# define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"
#endif

/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/
# define NvM_START_SEC_CODE
# include "NvM_MemMap.h"
/********************************************************************************************
 * Initialization of NV block length and RAM block data address (permanent RAM address)     *
 *                                                                                          *
 * In this case NvMRbRuntimeRamBlockConfiguration is enabled                                *
 * + NV block length is defined either by NvMRbNvBlockLengthString or NvMNvBlockLength      *
 * + RAM block data address is still defined by NvMRamBlockDataAddress but now              *
 *   NvMRamBlockDataAddress can also contain C expressions                                  *
 *                                                                                          *
 * Furthermore if explicit sync feature is enabled the explicit sync buffer is defined here *
 * by setting the start address and calculating the buffer size                             *
 * Start address and end address is defined by user in common options with the parameters   *
 * + NvMRbRuntimeRamBufferAddressStart                                                      *
 * + NvMRbRuntimeRamBufferAddressEnd                                                        *
 *                                                                                          *
 * ******************************************************************************************
*/
/* HIS METRIC STMT VIOLATION IN NvM_Prv_InitRamBlockProperties: Due to the configuration this generated function may be empty */
void NvM_Prv_InitRamBlockProperties(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028]
    // Runtime Calculation feature enabled
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
# if (NVM_PRV_EXPLICIT_SYNC == STD_ON)
    // Calculate explicit synchronization RAM buffer size
    /* MR12 RULE 11.4 VIOLATION: Cast to an integral type is necessary to calculate the size of the object*/
    uint32 RuntimeRamMirrorSize_u32 = (uint32)(0) - (uint32)(0);

    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3029] Calculate explicit synchronization RAM buffer
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3030] Calculate explicit synchronization RAM buffer
    // Set explicit synchronization RAM buffer start address and its size
    NvM_Prv_InitRuntimeRamMirror((uint8 *)(0),
                                 RuntimeRamMirrorSize_u32);
# endif



#endif
}
# define NVM_STOP_SEC_CODE
# include "NvM_MemMap.h"
