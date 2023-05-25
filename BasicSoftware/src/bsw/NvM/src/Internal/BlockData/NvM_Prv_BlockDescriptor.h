
#ifndef NVM_PRV_BLOCKDESCRIPTOR_H
#define NVM_PRV_BLOCKDESCRIPTOR_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Types.h"

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
#include "NvM_Prv_Crypto_Types.h"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
/**
 * Definition of the immediate job priority for a NVRAM block.
 * TRACE[ECUC_NvM_00477]
 */
#define NVM_PRV_JOB_PRIORITY_IMMEDIATE      (0u)

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/**
 * \typedef
 * Type definition for copy functions used for the explicit synchronization.
 *
 * This type makes it possible for the explicit synchronization to write user data into the NvM internal buffer and
 * to read user data from the NvM internal buffer in a common way.
 */
typedef Std_ReturnType (*NvM_Prv_ExplicitSync_Copy_tpfct)(void* NvMBuffer_pv);

/**
 * Definition of different bit masks to access different configuration flags withis block descriptors
 */
typedef enum
{
    /// Bit mask definition to get information whether a block shall be processed during NvM_ReadAll.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM117_Conf] Parameter NvMBlockDescriptor:NvMSelectBlockForReadAll
    NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL              = 0x00000001,
    /// Bit mask definition to get information whether the block shall be processed during NvM_WriteAll.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM549_Conf] Parameter NvMBlockDescriptor:NvMSelectBlockForWriteAll
    NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL             = 0x00000002,
    /// Bit mask definition to get information whether the block shall be processed during NvM_Rb_FirstInitAll.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2828] Parameter NvMBlockDescriptor:NvMRbSelectBlockForFirstInitAll
    NVM_PRV_BLOCK_FLAG_SELECT_FOR_FIRST_INIT_ALL        = 0x00000004,
    /// Bit mask definition to get information whether the block shall be initialized when a layout change is detected.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2881] Parameter NvMBlockDescriptor:NvMRbInitBlockAtLayoutChange
    NVM_PRV_BLOCK_FLAG_SELECT_FOR_INIT_AT_LAYOUT_CHANGE = 0x00000008,
    /// Bit mask definition to get information whether the block is initially write protected.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM033_Conf] Parameter NvMBlockDescriptor:NvMBlockWriteProt
    NVM_PRV_BLOCK_FLAG_WRITE_PROTECTED                  = 0x00000010,
    /// Bit mask definition to get information whether the block shall be written only once.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM072_Conf] Parameter NvMBlockDescriptor:NvMWriteBlockOnce
    NVM_PRV_BLOCK_FLAG_WRITE_ONCE                       = 0x00000020,
    /// Bit mask definition to get information whether the block shall be resistant to block layout changes.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM483_Conf] ParameterNvMBlockDescriptor:NvMResistantToChangedSw
    NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW          = 0x00000040,
    /// Bit mask definition to get information whether the block shall use explicit synchronization mechanism.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM519_Conf] Parameter NvMBlockDescriptor:NvMBlockUseSyncMechanism
    NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM               = 0x00000080,
    /// Bit mask definition to get information whether the block shall be processed during NvM_ValidateAll.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM557_Conf] Parameter NvMBlockDescriptor:NvMBlockUseAutoValidation
    NVM_PRV_BLOCK_FLAG_USE_AUTO_VALIDATION              = 0x00000100,
    /// Bit mask definition to get information whether the block is using a variable block length.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2996] Parameter NvMBlockDescriptor:NvMRbNvBlockLengthString
    NVM_PRV_BLOCK_FLAG_USE_VARIABLE_BLOCK_LENGTH        = 0x00000200,
    /// Bit mask definition to get information whether the block is configured for migration.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3027] Parameter NvMBlockDescriptor:NvMRbFirstReadDataMigration
    NVM_PRV_BLOCK_FLAG_SELECT_FOR_MIGRATION             = 0x00000400,
    /// Bit mask definition to get information whether the block shall be initialized unconditionally
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3175] Parameter NvMBlockDescriptor:NvMRbRamInitUnconditional
    NVM_PRV_BLOCK_FLAG_RAM_INIT_UNCONDITIONAL           = 0x00000800,
    /// Bit mask definition to get information whether the block shall use CRC compare mechanism.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[ECUC_NvM_00556] Parameter NvMBlockDescriptor:NvMBlockUseCRCCompMechanism
    NVM_PRV_BLOCK_FLAG_USE_CRC_COMP_MECHANISM           = 0x00001000,
    /// Bit mask definition to get information whether RAM CRC shall be calculated for the (permanent RAM) block.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM119_Conf] Parameter NvMBlockDescriptor:NvMCalcRamBlockCrc
    NVM_PRV_BLOCK_FLAG_USE_RAM_CRC                      = 0x00002000,
    /// Bit mask definition to get information whether block consistency is protected via a CRC in NV media.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    /// TRACE[NVM036_Conf] Parameter NvMBlockDescriptor:NvMBlockUseCrc
    NVM_PRV_BLOCK_FLAG_USE_NV_CRC                       = 0x00004000,
    /// Bit mask definition to get information whether the block shall use cryptographic services.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    NVM_PRV_BLOCK_FLAG_USE_CRYPTO                       = 0x00008000,
    /// Bit mask definition to get information whether the block shall have a counter for write requests on it.
    /// This information is stored in the NVRAM block descriptor, see NvM_Prv_BlockDescriptor_tst.stFlags_uo
    NVM_PRV_BLOCK_FLAG_CNTR_WRITE                       = 0x00010000

} NvM_Prv_BlockConfiguration_ten;

/**
 * Definition of the structure for common configuration options of the NvM used during the runtime.
 * TRACE[NVM028_Conf]
 */
typedef struct
{
    /**
     * Pointer to a callback function which shall be invoked on termination of each asynchronous multi-block request.
     * TRACE[NVM468]
     * TRACE[NVM331]
     * TRACE[NVM500_Conf] Parameter NvMCommon:NvMMultiBlockCallback
     */
    void (*MultiBlockCallback_pfct)(uint8 ServiceId, NvM_RequestResultType JobResult);
    /**
     * Pointer to a callback function which shall be invoked on start of each asynchronous multi-block request.
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2762] Parameter NvMCommon:NvMRbMultiBlockStartCallback
     */
    void (*RbMultiBlockStartCallback_pfct)(uint8 ServiceId);
    /**
     * Pointer to a callback function which shall be invoked on termination of each asynchronous single-block request.
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2842]
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2843] Parameter NvMCommon:NvMRbObserverCallback
     */
    Std_ReturnType (*ObserverCallback_pfct)(NvM_BlockIdType BlockId,
                                            uint8 ServiceId,
                                            NvM_RequestResultType JobResult);
} NvM_Prv_Common_tst;

/**
 * Definition of the structure for a NVRAM block descriptor.
 * A NVRAM block descriptor contains all information about a single NVRAM block needed during the runtime.
 * The NVRAM block descriptors are condensed in the NVRAM block descriptor table.
 * TRACE[NVM061_Conf]
 */
typedef struct
{
    /**
     * Block ID to be passed to the MemIf.
     * Do not confuse this ID with NVRAM block ID used within NvM.
     */
    uint16 idBlockMemIf_u16;
    /**
     * Pointer to the block legth in bytes.
     * If runtime calculation of block lengths is enabled by the NvM configuration then block lengths will be located
     * in RAM so pointer points to a variable with block length calculated during runtime.
     * Otherwise if runtime calculation is disabled by the NvM configuration then block legths will be located
     * in ROM so pointer points to a constant with block length configured during compile time.
     * TRACE[NVM479_Conf] Parameter NvMBlockDescriptor:NvMNvBlockLength
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028] Parameter NvMCommon:NvMRbRuntimeRamBlockConfiguration
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2996] Parameter NvMBlockDescriptor:NvMRbNvBlockLengthString
     */
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
    uint16* nrBlockBytes_pu16;
#else
    uint16 const* nrBlockBytes_pu16;

#endif

    /**
     * Block length in bytes stored on the medium.
     * Dependent on the block configuration it is possible that not only plain user data will be stored.
     * Following configuration options are available:
     * - only user data is stored (normal use case)
     * - user data + [signature] + [CRC] = encryption disabled + signing on/off + NV CRC on/off
     * - encrypted user data + [signature] + [CRC] = encryption enabled + signing on/off + NV CRC on/off
     */
    uint16 nrBlockBytesStored_u16;

    /**
     * Device index where the block is located (Fee / Ea).
     * TRACE[NVM035_Conf] Parameter NvMBlockDescriptor:NvMNvramDeviceId
     */
    uint8 idxDevice_u8;
    /**
     * Number of non-volatile blocks in a contiguous area according to the given block management type.
     * TRACE[NVM480_Conf] Parameter NvMBlockDescriptor:NvMNvBlockNum
     */
    uint8 nrNvBlocks_u8;
    /**
     * Number of multiple ROM blocks in a contiguous area according to the given block management type.
     * TRACE[NVM485_Conf] Parameter NvMBlockDescriptor:NvMRomBlockNum
     */
    uint8 nrRomBlocks_u8;
    /**
     * Pointer to the permanent RAM block data.
     * If this pointer is NULL_PTR then no permanent RAM block data has been configured by the user.
     * If runtime calculation of RAM block data addresses is enabled by the NvM configuration
     * then RAM block data addresses will be located in RAM and pointer points to a void pointer
     * calculated during runtime.
     * Otherwise if runtime calculation is disabled by the NvM configuration then RAM block data addresses
     * will be located in ROM and pointer points to a constant void pointer configured during compile time.
     * TRACE[NVM126]
     * TRACE[NVM482_Conf] Parameter NvMBlockDescriptor:NvMRamBlockDataAddress
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028] Parameter NvMCommon:NvMRbRuntimeRamBlockConfiguration
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3086] Parameter NvMBlockDescriptor:NvMRbRamBlockDataAddressString
     */
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
    void** adrRamBlock_ppv;
#else
    void* const* adrRamBlock_ppv;
#endif
    /**
     * Pointer to the ROM block data.
     * If this pointer is NULL_PTR then no ROM block data has been configured by the user.
     * TRACE[NVM020]
     * TRACE[NVM484_Conf] Parameter NvMBlockDescriptor:NvMRomBlockDataAddress
     */
    void const* adrRomBlock_pcv;
    /**
     * Pointer to the block specific callback function which shall be invoked on termination
     * of each asynchronous single-block request.
     * If this pointer is NULL_PTR then no callback has been configured by the user.
     * TRACE[NVM113]
     * TRACE[NVM467]
     * TRACE[NVM330]
     * TRACE[NVM506_Conf] Parameter NvMBlockDescriptor:NvMSingleBlockCallback
     */
    Std_ReturnType (*SingleBlockCallback_pfct)(uint8 ServiceId, NvM_RequestResultType JobResult);
    /**
     * Pointer to the block specific callback function which shall be invoked on start
     * of each asynchronous single-block request.
     * If this pointer is NULL_PTR then no callback has been configured by the user.
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3056]
     * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3062] Parameter NvMBlockDescriptor:NvMRbSingleBlockStartCallback
     */
    Std_ReturnType (*SingleBlockStartCallback_pfct)(uint8 ServiceId);
    /**
     * Pointer to the block specific callback function which shall be invoked if no ROM block data is available
     * for initialization of the block.
     * If this pointer is NULL_PTR then no callback has been configured by the user.
     * TRACE[NVM469]
     * TRACE[NVM352]
     * TRACE[NVM116_Conf] Parameter NvMBlockDescriptor:NvMInitBlockCallback
     */
    Std_ReturnType (*InitBlockCallback_pfct)(void);
    /**
     * Pointer to the block specific callback function to let the application copy data from the NvM mirror
     * to application's RAM block.
     * If this pointer is NULL_PTR then no callback has been configured by the user.
     * TRACE[NVM540]
     * TRACE[NVM542]
     * TRACE[NVM521_Conf] Parameter NvMBlockDescriptor:NvMReadRamBlockFromNvCallback
     */
    Std_ReturnType (*ReadRamBlockFromNvm_pfct)(void* NvMBuffer);
    /**
     * Pointer the block specific callback function to let the application copy data from the application's RAM block
     * to the NvM mirror.
     * If this pointer is NULL_PTR then no callback has been configured by the user.
     * TRACE[NVM539]
     * TRACE[NVM541]
     * TRACE[NVM520_Conf] Parameter NvMBlockDescriptor:NvMWriteRamBlockToNvCallback
     */
    Std_ReturnType (*WriteRamBlockToNvm_pfct)(void* NvMBuffer);
    /**
     * Block management type.
     * TRACE[NVM062_Conf] Parameter NvMBlockDescriptor:NvMBlockManagementType
     */
    NvM_BlockManagementType BlockManagementType_en;
    /**
     * Block job priority.
     * TRACE[NVM477_Conf] Parameter NvMBlockDescriptor:NvMBlockJobPriority
     */
    uint8 JobPriority_u8;
    /**
     * Bit-field containing several configuration parameter which can be represented by a bit.
     * NvM provides special bit mask to access individual bits from this bit-field:
     * - NVM_PRV_BLOCK_FLAG_SELECT_FOR_READ_ALL
     *   TRACE[NVM117_Conf] Parameter NvMBlockDescriptor:NvMSelectBlockForReadAll
     * - NVM_PRV_BLOCK_FLAG_SELECT_FOR_WRITE_ALL
     *   TRACE[NVM549_Conf] Parameter NvMBlockDescriptor:NvMSelectBlockForWriteAll
     * - NVM_PRV_BLOCK_FLAG_SELECT_FOR_FIRST_INIT_ALL
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-2828] Parameter NvMBlockDescriptor:NvMRbSelectBlockForFirstInitAll
     * - NVM_PRV_BLOCK_FLAG_SELECT_FOR_INIT_AT_LAYOUT_CHANGE
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-2881] Parameter NvMBlockDescriptor:NvMRbInitBlockAtLayoutChange
     * - NVM_PRV_BLOCK_FLAG_WRITE_PROTECTED
     *   TRACE[NVM033_Conf] Parameter NvMBlockDescriptor:NvMBlockWriteProt
     * - NVM_PRV_BLOCK_FLAG_WRITE_ONCE
     *   TRACE[NVM072_Conf] Parameter NvMBlockDescriptor:NvMWriteBlockOnce
     * - NVM_PRV_BLOCK_FLAG_RESISTANT_TO_CHANGED_SW
     *   TRACE[NVM483_Conf] Parameter NvMBlockDescriptor:NvMResistantToChangedSw
     * - NVM_PRV_BLOCK_FLAG_USE_SYNC_MECHANISM
     *   TRACE[NVM519_Conf] Parameter NvMBlockDescriptor:NvMBlockUseSyncMechanism
     * - NVM_PRV_BLOCK_FLAG_USE_AUTO_VALIDATION
     *   TRACE[NVM557_Conf] Parameter NvMBlockDescriptor:NvMBlockUseAutoValidation
     * - NVM_PRV_BLOCK_FLAG_USE_VARIABLE_BLOCK_LENGTH
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-2996] Parameter NvMBlockDescriptor:NvMRbNvBlockLengthString
     * - NVM_PRV_BLOCK_FLAG_SELECT_FOR_MIGRATION
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3027] Parameter NvMBlockDescriptor:NvMRbFirstReadDataMigration
     * - NVM_PRV_BLOCK_FLAG_RAM_INIT_UNCONDITIONAL
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3175] Parameter NvMBlockDescriptor:NvMRbRamInitUnconditional
     * - NVM_PRV_BLOCK_FLAG_USE_CRC_COMP_MECHANISM
     *   TRACE[ECUC_NvM_00556] Parameter NvMBlockDescriptor:NvMBlockUseCRCCompMechanism
     * - NVM_PRV_BLOCK_FLAG_USE_RAM_CRC
     *   TRACE[NVM119_Conf] Parameter NvMBlockDescriptor:NvMCalcRamBlockCrc
     * - NVM_PRV_BLOCK_FLAG_USE_NV_CRC
     *   TRACE[NVM036_Conf] Parameter NvMBlockDescriptor:NvMBlockUseCrc
     * - NVM_PRV_BLOCK_FLAG_USE_CRYPTO
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3249] Parameter NvMRbSecServices:NVMRbAddEncryption
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3251] Parameter NvMRbSecServices:NvMRbAddSignature
     * - NVM_PRV_BLOCK_FLAG_USE_WRITE_CNTR
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3295] Parameter NvMCommon:NvMRbCommonWriteCounter
     *   TRACE[BSW_SWS_AR_NVRAMManager_Ext-3296] Parameter NvMBlockDescriptor:NvMRbWriteCounter
     */
    uint32 stFlags_uo;

#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    /**
     * CRC type.
     * TRACE[ECUC_NvM_00476] Parameter NvMBlockDescriptor:NvMBlockCrcType
     */
    NvM_Prv_Crc_Type_ten TypeCrc_en;
    /**
     * Index of the RAM block CRC in the array containing all RAM block CRCs of the same CRC type.
     */
    uint16 idxRamBlockCrc_u16;
#endif

#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
    uint16 PersistentId_u16;   // Persistent Id
    /**
     *
     */
    NvM_Prv_Crypto_BlockConfig_tst CryptoConfig_st;
#endif
} NvM_Prv_BlockDescriptor_tst;

/**
 * Definition of the structure to assign the persistent ID to the block ID for a NVRAM block.
 */
typedef struct
{
    uint16          PersistentId_u16;   // Persistent Id
    NvM_BlockIdType BlockId_u16;        // Block Id
} NvM_Prv_PersId_BlockId_tst;

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
// Declaration of different configuration-dependent data.
// Definition of all this data is done in NvM_Cfg.c.
// ------------------------------------------------------------------------------------------------
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/**
 * Declaration of the container with all common configuration data.
 * TRACE[NVM028_Conf]
 */
extern const NvM_Prv_Common_tst             NvM_Prv_Common_cst;
/**
 * Declaration of the NVRAM block descriptor table.
 * TRACE[NVM061_Conf]
 */
extern const NvM_Prv_BlockDescriptor_tst    NvM_Prv_BlockDescriptors_acst[NVM_CFG_NR_BLOCKS];
/**
 * Declaratiion of the table to get the NvM block ID using the persistent ID.
 */
extern const NvM_Prv_PersId_BlockId_tst     NvM_Prv_PersId_BlockId_acst[NVM_PRV_NR_PERSISTENT_IDS];

/**
 * Declaration of the table with blocks IDs which are configured for RAM block CRC.
 */
#if (defined(NVM_CFG_CRC_NR_RAM_BLOCKS) && (NVM_CFG_CRC_NR_RAM_BLOCKS > 0u))
extern NvM_BlockIdType const NvM_Prv_idBlockRamCrc_cauo[NVM_CFG_CRC_NR_RAM_BLOCKS];
#endif

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#if (NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG == STD_ON)
# define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"

extern uint16 NvM_Prv_BlockLengths_au16[NVM_CFG_NR_BLOCKS];
extern void *NvM_Prv_RamBlockAdr_apv[NVM_CFG_NR_BLOCKS];

# define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "NvM_MemMap.h"
#else
# define NVM_START_SEC_CONST_UNSPECIFIED
# include "NvM_MemMap.h"

extern uint16 const NvM_Prv_BlockLengths_acu16[NVM_CFG_NR_BLOCKS];
extern void * const NvM_Prv_RamBlockAdr_acpv[NVM_CFG_NR_BLOCKS];

# define NVM_STOP_SEC_CONST_UNSPECIFIED
# include "NvM_MemMap.h"
#endif

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

// Init-related functions (defined in NvM_Cfg.c)
extern void NvM_Prv_InitRamBlockProperties(void);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline functions
 **********************************************************************************************************************
 */
/* Include the actual definitions of the provided inline functions */
#include "NvM_Prv_BlockDescriptor_Inl.h"

/* NVM_PRV_BLOCKDESCRIPTOR_H */
#endif
