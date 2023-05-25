

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

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

// Multi block callback
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern void Fbl_DataM_MultiBlockJobEndNotification(uint8 ServiceId, NvM_RequestResultType JobResult);

// Single block callback of NVRAM block NV_BootSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_BootSoftwareFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_BootSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock[];

// ROM block start address of NVRAM block NV_BootSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_BootSoftwareFingerprintBlock[];

// Single block callback of NVRAM block NV_ApplicationSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_ApplicationSoftwareFingerprintCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_ApplicationSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock[];

// ROM block start address of NVRAM block NV_ApplicationSoftwareFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_ApplicationSoftwareFingerprintBlock[];

// Single block callback of NVRAM block NV_ApplicationDataFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_ApplicationDataFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_ApplicationDataFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock[];

// ROM block start address of NVRAM block NV_ApplicationDataFingerprintBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_ApplicationDataFingerprintBlock[];

// Single block callback of NVRAM block NV_PBLValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_PBLValidFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_PBLValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_PBLValidFlagBlock[];

// ROM block start address of NVRAM block NV_PBLValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_PBLValidFlagBlock[];

// Single block callback of NVRAM block NV_ApplicationValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_ApplicationValidFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_ApplicationValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock[];

// ROM block start address of NVRAM block NV_ApplicationValidFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_ApplicationValidFlagBlock[];

// Single block callback of NVRAM block NV_ExternalReprogrammingRequestFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_ReprogrammingRequestFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_ExternalReprogrammingRequestFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock[];

// ROM block start address of NVRAM block NV_ExternalReprogrammingRequestFlagBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock[];

// Single block callback of NVRAM block NV_PreErasedBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern Std_ReturnType Fbl_DataM_NV_PreErasedCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

// RAM block of NVRAM block NV_PreErasedBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern uint8 Fbl_DataM_RamMirrorNV_PreErasedBlock[];

// ROM block start address of NVRAM block NV_PreErasedBlock
/* MR12 RULE 8.5, 8.6 VIOLATION: If we declared this in NvM_Cfg.h,
                                 we would effectively re-export this to all NvM users */
extern const uint8 Fbl_DataM_RomDataNV_PreErasedBlock[];

/*
 **********************************************************************************************************************
 * Assertions
 **********************************************************************************************************************
*/

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
    {3260u, 2u}, // NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock
    {4628u, 5u}, // NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock
    {16561u, 6u}, // NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock
    {27326u, 4u}, // NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock
    {29884u, 3u}, // NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock
    {30213u, 8u}, // NvMConf_NvMBlockDescriptor_NV_PreErasedBlock
    {32699u, 7u}, // NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock
};

// TRACE[NVM028_Conf]
// Structure containing common configuration options
const NvM_Prv_Common_tst NvM_Prv_Common_cst = {&Fbl_DataM_MultiBlockJobEndNotification, NULL_PTR, NULL_PTR};

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
        // Block descriptor of NVRAM block NV_BootSoftwareFingerprintBlock (NvM block ID: 2, persistent ID: 3260):
        FeeConf_FeeBlockConfiguration_NV_BootSoftwareFingerprintBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[2]), // Block length calculated on compile time
        27u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[2]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_BootSoftwareFingerprintBlock), // ROM block data address
        &Fbl_DataM_NV_BootSoftwareFingerprintBlockCallbackFunction, // Single block callback
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
        3260u, // Persistent ID
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
        // Block descriptor of NVRAM block NV_ApplicationSoftwareFingerprintBlock (NvM block ID: 3, persistent ID: 29884):
        FeeConf_FeeBlockConfiguration_NV_ApplicationSoftwareFingerprintBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[3]), // Block length calculated on compile time
        27u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[3]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_ApplicationSoftwareFingerprintBlock), // ROM block data address
        &Fbl_DataM_NV_ApplicationSoftwareFingerprintCallbackFunction, // Single block callback
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
        29884u, // Persistent ID
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
        // Block descriptor of NVRAM block NV_ApplicationDataFingerprintBlock (NvM block ID: 4, persistent ID: 27326):
        FeeConf_FeeBlockConfiguration_NV_ApplicationDataFingerprintBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[4]), // Block length calculated on compile time
        27u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[4]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_ApplicationDataFingerprintBlock), // ROM block data address
        &Fbl_DataM_NV_ApplicationDataFingerprintBlockCallbackFunction, // Single block callback
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
        27326u, // Persistent ID
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
        // Block descriptor of NVRAM block NV_PBLValidFlagBlock (NvM block ID: 5, persistent ID: 4628):
        FeeConf_FeeBlockConfiguration_NV_PBLValidFlagBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[5]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[5]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_PBLValidFlagBlock), // ROM block data address
        &Fbl_DataM_NV_PBLValidFlagBlockCallbackFunction, // Single block callback
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
        4628u, // Persistent ID
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
        // Block descriptor of NVRAM block NV_ApplicationValidFlagBlock (NvM block ID: 6, persistent ID: 16561):
        FeeConf_FeeBlockConfiguration_NV_ApplicationValidFlagBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[6]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[6]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_ApplicationValidFlagBlock), // ROM block data address
        &Fbl_DataM_NV_ApplicationValidFlagBlockCallbackFunction, // Single block callback
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
        16561u, // Persistent ID
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
        // Block descriptor of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 7, persistent ID: 32699):
        FeeConf_FeeBlockConfiguration_NV_ExternalReprogrammingRequestFlagBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[7]), // Block length calculated on compile time
        4u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[7]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock), // ROM block data address
        &Fbl_DataM_NV_ReprogrammingRequestFlagBlockCallbackFunction, // Single block callback
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
        // Block descriptor of NVRAM block NV_PreErasedBlock (NvM block ID: 8, persistent ID: 30213):
        FeeConf_FeeBlockConfiguration_NV_PreErasedBlock, // MemIf block ID
        (const uint16 *) &(NvM_Prv_BlockLengths_acu16[8]), // Block length calculated on compile time
        3u, // block length in bytes stored on the medium
        0u, // Device index (0: Fee, 1..254: Ea)
        1u, // Number of NV blocks
        1u, // Number of ROM blocks
        &(NvM_Prv_RamBlockAdr_acpv[8]), // RAM block data address calculated on compile time
        /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
        (const void *)&(Fbl_DataM_RomDataNV_PreErasedBlock), // ROM block data address
        &Fbl_DataM_NV_PreErasedCallbackFunction, // Single block callback
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
        30213u, // Persistent ID
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
    // Block length of NVRAM block NV_BootSoftwareFingerprintBlock (NvM block ID: 2)
    27u,
    // Block length of NVRAM block NV_ApplicationSoftwareFingerprintBlock (NvM block ID: 3)
    27u,
    // Block length of NVRAM block NV_ApplicationDataFingerprintBlock (NvM block ID: 4)
    27u,
    // Block length of NVRAM block NV_PBLValidFlagBlock (NvM block ID: 5)
    4u,
    // Block length of NVRAM block NV_ApplicationValidFlagBlock (NvM block ID: 6)
    4u,
    // Block length of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 7)
    4u,
    // Block length of NVRAM block NV_PreErasedBlock (NvM block ID: 8)
    3u,
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
    // Permanent RAM address of NVRAM block NV_BootSoftwareFingerprintBlock (NvM block ID: 2)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock),
    // Permanent RAM address of NVRAM block NV_ApplicationSoftwareFingerprintBlock (NvM block ID: 3)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock),
    // Permanent RAM address of NVRAM block NV_ApplicationDataFingerprintBlock (NvM block ID: 4)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock),
    // Permanent RAM address of NVRAM block NV_PBLValidFlagBlock (NvM block ID: 5)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_PBLValidFlagBlock),
    // Permanent RAM address of NVRAM block NV_ApplicationValidFlagBlock (NvM block ID: 6)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock),
    // Permanent RAM address of NVRAM block NV_ExternalReprogrammingRequestFlagBlock (NvM block ID: 7)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock),
    // Permanent RAM address of NVRAM block NV_PreErasedBlock (NvM block ID: 8)
    /* MR12 DIR 1.1, 11.6 VIOLATION: Casting to void pointer can always be done safely */
    (void *)&(Fbl_DataM_RamMirrorNV_PreErasedBlock),
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
