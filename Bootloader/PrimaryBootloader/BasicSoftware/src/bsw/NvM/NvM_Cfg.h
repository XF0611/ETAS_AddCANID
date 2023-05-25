

// TRACE[NVM552]
// Header file specifying NvM pre-compile time configuration parameters

#ifndef NVM_CFG_H
#define NVM_CFG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2993]
// Function-like macro NVM_CFG_NV_BLOCK_LENGTH shall
// support runtime calculated block lengths
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2997]
// Include declarations of runtime calculated block
// lengths to make it usable in function-like macro NVM_CFG_NV_BLOCK_LENGTH

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* AUTOSAR version information  */
#define NVM_AR_RELEASE_MAJOR_VERSION        4                /* AR major version */
#define NVM_AR_RELEASE_MINOR_VERSION        2                /* AR minor version */
#define NVM_AR_RELEASE_REVISION_VERSION     2                /* AR patch version */

/* SW version information   */
#define NVM_SW_MAJOR_VERSION        17               /* SW major version */
#define NVM_SW_MINOR_VERSION        0                /* SW minor version */
#define NVM_SW_PATCH_VERSION        0                /* SW patch version */

/* Additional defines */
#define NVM_VENDOR_ID               6                          
#define NVM_MODULE_ID               20               /* Module ID NVM */
#define NVM_INSTANCE_ID             0                /* Instance ID NVM */

// ---------------------------------------------------------------------------------------------------------------------
// TRACE[NVM028_Conf]
// Defines for common options (in the same order as in the ParamDef)
// ---------------------------------------------------------------------------------------------------------------------

// TRACE[NVM491_Conf]
// Enables/disables some APIs which are related to NvM API configuration classes
#define NVM_API_CONFIG_CLASS (NVM_API_CONFIG_CLASS_3)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2803]
// Enables/disables the notification of BswM about the current status of the multiblock job
#define NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFORMATION (STD_OFF)

// TRACE[NVM492_Conf]
// ID representing the currently configured block layout
#define NVM_COMPILED_CONFIG_ID (0u)

// TRACE[ECUC_NvM_00119]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3186]
// Enables/disables the CRC (re)calculation for the permanent RAM block
#define NVM_CALC_RAM_BLOCK_CRC          (STD_OFF)
// Number of blocks definitions using RAM block CRC for different CRC types
#define NVM_CFG_CRC_8_NR_RAM_BLOCKS     (0u)
#define NVM_CFG_CRC_16_NR_RAM_BLOCKS    (0u)
#define NVM_CFG_CRC_32_NR_RAM_BLOCKS    (0u)
#define NVM_CFG_CRC_NR_RAM_BLOCKS       (0u)
// TRACE[NVM119_Conf]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3249]
// Enables/disables the usage of CRC for user data encryption
#define NVM_CALC_CRYPTO_CRC             (STD_OFF)
// TRACE[NVM036_Conf]
// TRACE[NVM476_Conf]
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3187]
// Enables/disables the additional storage of CRC in the non-volatile memory
#define NVM_CALC_NV_CRC                 (STD_OFF)

// Enables/disables the usage of CRC services by the NvM
// Usage of CRC services is enabled if at least one of following features is enabled:
// - CRC (re)calculation for the permanent RAM block
// - usage of CRC for user data encryption
// - storage of CRC in the non-volatile memory
#define NVM_CALC_CRC                    (   (NVM_CALC_RAM_BLOCK_CRC == STD_ON)  \
                                         || (NVM_CALC_CRYPTO_CRC == STD_ON)     \
                                         || (NVM_CALC_NV_CRC == STD_ON))

/* BSW-8494 */
#define NVM_PRV_CFG_SIZE_ADDING_LENGTH_STRING     (0u)
#define NVM_PRV_CFG_SIZE_CRYPTO_BUFFER_CFG  (0u)
/* END BSW-8494 */
// TRACE[NVM493_Conf]
// Maximum number of bytes which shall be processed within one cycle of job processing
#define NVM_CRC_NUM_OF_BYTES            (1u)

// The parameter NvMDatasetSelectionBits is not supported in this implementation of NvM

// Enables/disables usage of cryptographic services
#define NVM_CRYPTO_USED                 (STD_OFF)
/* BSW-11964 */
#define NVM_STANDARD_DATA_ENCRYTION_USED  (STD_OFF)

#define NVM_AEAD_DATA_ENCRYPTION_USED  (STD_OFF)

#define NVM_ADD_SIGNATURE_USED  (STD_OFF)

#define NVM_ADD_RANDOM_USED   (STD_OFF)
/* END BSW-11964 */


// TRACE[NVM495_Conf]
// Enables/disables development error detection
#define NVM_DEV_ERROR_DETECT (STD_OFF)

// TRACE[NVM496_Conf]
// Enables/disables switching memory drivers to fast mode
// during performing NvM_ReadAll/NvM_WriteAll
#define NVM_DRV_MODE_SWITCH (STD_OFF)

// TRACE[NVM497_Conf]
// Enables/disables the dynamic configuration management handling
// by the NvM_ReadAll request
#define NVM_DYNAMIC_CONFIGURATION (STD_OFF)

// TRACE[NVM498_Conf]
// Enables/disables job prioritization handling
#define NVM_JOB_PRIORITIZATION (STD_OFF)

// TRACE[ECUC_NvM_00555]
// Defines the cycle time in seconds of the periodic calling of NvM main function
#define NVM_MAIN_FUNCTION_PERIOD (0.01f)

// TRACE[NVM500_Conf]
// Callback routine indicating termination of each asynchronous multi block requests
#define NVM_MULTI_BLOCK_CALLBACK (Fbl_DataM_MultiBlockJobEndNotification)

// TRACE[NVM501_Conf]
// Enables/disables polling mode in NvM
// (disables/enables callback functions for lower layers)
#define NVM_POLLING_MODE (STD_ON)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2945]
// Enables/disables the auxiliary service interface
#define NVM_RB_AUX_INTERFACE (STD_OFF)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2746]
// Version of the generated ARXML file(s)
#define NVM_RB_GEN_ARXML_VERSION (NVM_GEN_ARXML_VERSION_402)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2893]
// Enables/disables the initialization of blocks
#define NVM_RB_INIT_AT_LAYOUT_CHANGE (STD_OFF)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2762]
// Callback routine indicating the start of each asynchronous multi block operation
#define NVM_RB_MULTI_BLOCK_START_CALLBACK (NULL_PTR)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2843]
// Callback routine indicating termination of any NvM job
#define NVM_RB_OBSERVER_CALLBACK (NULL_PTR)

// RAM (re-)intialization definitions
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2786]
// Defines the behavior of NvM module when saved RAM zone is lost
#define NVM_RB_RAM_INIT_CHECK   (NVM_RB_RAM_INIT_CHECK_NONE)
// NvM blocks RAM status bits are initialized with default values.
// Saved RAM zone is considered lost.
#define NVM_RB_RAM_INIT_CHECK_NONE  (0u)
// NvM blocks RAM status bits are not initialized. Only blocks with INVLAID RAM bit have
// their RAM blocks filled in with Nv data during NvM_ReadAll.
#define NVM_RB_RAM_INIT_CHECK_QUICK (1u)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2787]
// Enables/disables enhanced configuration
#define NVM_RB_REMOVE_NON_RESISTANT_BLOCKS (STD_OFF)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977]
// Enables/disables the NvM_Rb_SetWriteAllTrigger API
#define NVM_RB_SET_WRITEALL_TRIGGER_API (STD_ON)

// TRACE[NVM518_Conf]
// Number of retries for mirror operations before postponing the current job
#define NVM_REPEAT_MIRROR_OPERATIONS (0u)

// TRACE[BSW_SWS_AR_NVRAMManager_Ext-3296]
// Enables/disables block write counter
#define NVM_RB_BLOCK_WRITE_COUNTER  (STD_OFF)

// TRACE[NVM502_Conf]
// Enables/disables the NvM_SetRamBlockStatus API
#define NVM_SET_RAM_BLOCK_STATUS_API (STD_ON)

// TRACE[NVM503_Conf]
// Number of queue entries for the immediate priority job queue
#define NVM_SIZE_IMMEDIATE_JOB_QUEUE (1u)

// TRACE[NVM504_Conf]
// Number of queue entries for the standard job queue
#define NVM_SIZE_STANDARD_JOB_QUEUE (1u)

// TRACE[NVM505_Conf]
// Enables/disables the NvM_GetVersionInfo API
#define NVM_VERSION_INFO_API (STD_OFF)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3124]
// Defines the request monitoring function for the NvM
#define NVM_RB_REQ_MONITOR (NVM_RB_REQ_MON_DISABLED)
// Request monitoring disabled
#define NVM_RB_REQ_MON_DISABLED (0u)
// Monitoring of write requests enabled
#define NVM_RB_REQ_MON_WRITE (1u)

// ---------------------------------------------------------------------------------------------------------------------
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2914]
// Names of overloaded API functions
// ---------------------------------------------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------------------------------------
// TRACE[NVM481_Conf]
// IDs for all configured blocks
// ---------------------------------------------------------------------------------------------------------------------

#define NvMConf_NvMBlockDescriptor_NvM_MultiBlock (0u)
#define NvMConf_NvMBlockDescriptor_NvM_ConfigId (1u)
#define NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock (2u)
#define NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock (3u)
#define NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock (4u)
#define NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock (5u)
#define NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock (6u)
#define NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock (7u)
#define NvMConf_NvMBlockDescriptor_NV_PreErasedBlock (8u)


// ---------------------------------------------------------------------------------------------------------------------
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2801]
// Lengths of all configured blocks as define
// ---------------------------------------------------------------------------------------------------------------------

#define NVM_CFG_NV_BLOCK_LENGTH_NvM_MultiBlock (1u)
#define NVM_CFG_NV_BLOCK_LENGTH_NvM_ConfigId (2u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock (27u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock (27u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock (27u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock (4u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock (4u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock (4u)
#define NVM_CFG_NV_BLOCK_LENGTH_NV_PreErasedBlock (3u)


// ---------------------------------------------------------------------------------------------------------------------
// Miscellaneous other definitions
// ---------------------------------------------------------------------------------------------------------------------

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3028]
// Define directive for checking
// Runtime RAM Block Configuration feature enabled/disabled
#define NVM_PRV_RUNTIME_RAM_BLOCK_CONFIG  STD_OFF

// RAM Mirror definitions
// NvM uses a RAM mirror (internal buffer) in following cases:
// - Explicit synchronization is enabled
// - NV CRC is enabled
// - Cryptographic services are enabled
#define NVM_PRV_HAS_RAM_MIRROR  STD_OFF
#define NVM_PRV_SIZE_RAM_MIRROR_USER_CONF (0u)
#define NVM_PRV_EXPLICIT_SYNC   STD_OFF
/* BSW-8494 */
// Block Length String used by Dem definitions
#define NVM_DEM_BLOCK_LENGTH_STRING_USED  STD_OFF

/* END BSW-8494 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2870]
// The implementation of the maintenance operation is done as if this feature was configurable
// Enables/disables the maintenance of redundant blocks (internal definition)
#define NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS (STD_OFF)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3158]
// ID of the first used block
#define NVM_CFG_FIRST_USED_BLOCK    (2u)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2849]
// ID of the last NvM user block
#define NVM_CFG_LAST_USER_BLOCK     (8u)

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3159]
// Number of all configured blocks
#define NVM_CFG_NR_BLOCKS           (9u)

// Additional internal helper definitions
#define NVM_PRV_NR_BLOCKS_ADMIN   (9u)
#define NVM_PRV_NR_PERSISTENT_IDS (7u)

#endif
