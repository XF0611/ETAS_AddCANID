
// TRACE[NVM077] Header file specifying all public NvM APIs
#ifndef NVM_H
#define NVM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
  */
#include "Std_Types.h"
#include "NvM_Types.h"

// TRACE[NVM089] Check version compatibility of included header files
#if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (STD_TYPES_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
# error "AUTOSAR major version undefined or mismatched"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
//-----------------------------------------------------------------------------------------------------------
// Service IDs
//-----------------------------------------------------------------------------------------------------------
#define NVM_SERVICE_ID_INIT                                             (0u) // TRACE[NVM447]
#define NVM_SERVICE_ID_SET_DATA_INDEX                                   (1u) // TRACE[NVM448]
#define NVM_SERVICE_ID_GET_DATA_INDEX                                   (2u) // TRACE[NVM449]
#define NVM_SERVICE_ID_SET_BLOCK_PROTECTION                             (3u) // TRACE[NVM450]
#define NVM_SERVICE_ID_GET_ERROR_STATUS                                 (4u) // TRACE[NVM451]
#define NVM_SERVICE_ID_SET_RAM_BLOCK_STATUS                             (5u) // TRACE[NVM453]
#define NVM_SERVICE_ID_READ_BLOCK                                       (6u) // TRACE[NVM454]
#define NVM_SERVICE_ID_WRITE_BLOCK                                      (7u) // TRACE[NVM455]
#define NVM_SERVICE_ID_RESTORE_BLOCK_DEFAULTS                           (8u) // TRACE[NVM456]
#define NVM_SERVICE_ID_ERASE_NV_BLOCK                                   (9u) // TRACE[NVM457]
#define NVM_SERVICE_ID_CANCEL_WRITE_ALL                                (10u) // TRACE[NVM458]
#define NVM_SERVICE_ID_INVALIDATE_NV_BLOCK                             (11u) // TRACE[NVM459]
#define NVM_SERVICE_ID_READ_ALL                                        (12u) // TRACE[NVM460]
#define NVM_SERVICE_ID_WRITE_ALL                                       (13u) // TRACE[NVM461]
#define NVM_SERVICE_ID_MAIN_FUNCTION                                   (14u) // TRACE[NVM464]
#define NVM_SERVICE_ID_GET_VERSION_INFO                                (15u) // TRACE[NVM452]
#define NVM_SERVICE_ID_CANCEL_JOBS                                     (16u) // TRACE[NVM535]
#define NVM_SERVICE_ID_JOB_END_NOTIFICATION                            (17u) // TRACE[NVM462]
#define NVM_SERVICE_ID_JOB_ERROR_NOTIFICATION                          (18u) // TRACE[NVM463]
#define NVM_SERVICE_ID_SET_BLOCK_LOCK_STATUS                           (19u) // TRACE[NVM548]
#define NVM_SERVICE_ID_READ_PRAM_BLOCK                                 (22u) // TRACE[SWS_NvM_00764]
#define NVM_SERVICE_ID_WRITE_PRAM_BLOCK                                (23u) // TRACE[SWS_NvM_00793]
#define NVM_SERVICE_ID_RESTORE_PRAM_BLOCK_DEFAULTS                     (24u) // TRACE[SWS_NvM_00813]
#define NVM_SERVICE_ID_VALIDATE_ALL                                    (25u) // TRACE[NVM855]
#define NVM_SERVICE_ID_RB_GET_WRITE_COUNTER                           (230u) // TRACE[BSW_SWS_AR_NVRAMManager_Ext-3285]
#define NVM_SERVICE_ID_RB_GET_BLOCK_TYPE                              (231u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3169] Service to get block type
#define NVM_SERVICE_ID_RB_IS_INIT_AT_LAYOUT_CHANGE_ENQUEUED           (232u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3167] Service to determine if InitAtLayoutChange is enqueued
#define NVM_SERVICE_ID_RB_IS_BLOCK_SELECTED_FOR_INIT_AT_LAYOUT_CHANGE (233u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3164] Service to determine if block is selected for InitAtLayoutChange
#define NVM_SERVICE_ID_RB_IS_BLOCK_SELECTED_FOR_FIRST_INIT_ALL        (234u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3162] Service to determine if block is selected for FirstInitAll
#define NVM_SERVICE_ID_RB_CLR_WRITE_MON_CNTRS                         (235u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3122] Service to clear write monitoring counters
#define NVM_SERVICE_ID_RB_GET_BLOCK_ID_CAUSING_LAST_DET_ERROR         (236u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3113] Service to get the block ID used in NvM service which has caused the last DET error
#define NVM_SERVICE_ID_RB_GET_REQUEST_TYPE                            (237u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3105] Service to get the type of the currently active block request
#define NVM_SERVICE_ID_RB_AUX_GET_MIGRATION_RESULT                    (238u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3053] Service to read the result of AUX migration operation
#define NVM_SERVICE_ID_RB_GET_MIGRATION_RESULT                        (239u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3052] Service to read the result of migration operation
#define NVM_SERVICE_ID_RB_INIT_RUNTIME_RAM_PROPERTIES                 (240u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3031] Service to initialize the runtime caclulated RAM block properties
#define NVM_SERVICE_ID_RB_SET_WRITEALL_TRIGGER                        (241u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977] Service to trigger NvM_WriteAll even if the RAM state is unchanged
#define NVM_SERVICE_ID_RB_AUX_RESTORE_BLOCK_DEFAULTS                  (242u) // Service ID currently only used for auxiliary reads from DATASET ROM blocks
#define NVM_SERVICE_ID_RB_AUX_GET_DATA_INDEX                          (243u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2940]
#define NVM_SERVICE_ID_RB_AUX_GET_ERROR_STATUS                        (244u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2939]
#define NVM_SERVICE_ID_RB_AUX_INVALIDATE_NV_BLOCK                     (245u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2942]
#define NVM_SERVICE_ID_RB_AUX_READ_BLOCK                              (246u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2943]
#define NVM_SERVICE_ID_RB_AUX_SET_DATA_INDEX                          (247u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2941]
#define NVM_SERVICE_ID_RB_AUX_WRITE_BLOCK                             (248u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2944]
// #define NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK                          (249u) Redundant block maintenance service ID (currently defined in NvM_Prv.h)
#define NVM_SERVICE_ID_RB_GET_BLOCK_ID                                (250u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2840]
#define NVM_SERVICE_ID_RB_FIRST_INIT_ALL                              (251u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2826]
#define NVM_SERVICE_ID_RB_GET_ACTIVE_SERVICE                          (252u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2827]
#define NVM_SERVICE_ID_RB_GET_NV_BLOCK_LENGTH                         (253u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2791]
#define NVM_SERVICE_ID_RB_INIT_AFTER_READ_ALL                         (254u) // INIT_AFTER_READ_ALL and REMOVE_NON_RESISTANT have the same service ID as they are performed together.  Note: REMOVE_NON_RESISTANT service ID is deprecated. INIT_AFTER_READ_ALL service ID should be used instead.
#define NVM_SERVICE_ID_RB_REMOVE_NON_RESISTANT                        (254u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2787]
                                                                             // TODO Note: REMOVE_NON_RESISTANT service ID is deprecated. INIT_AFTER_READ_ALL service ID should be used instead.
#define NVM_SERVICE_ID_RB_GET_STATUS                                  (255u) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2750]


//-----------------------------------------------------------------------------------------------------------
// Definitions of errors NvM reports to the DET
//
//-----------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Development errors
//-------------------------------------------------------------------------------------------------
#define NVM_E_PARAM_BLOCK_ID                (0x0Au) // TRACE[NVM023]
#define NVM_E_PARAM_BLOCK_TYPE              (0x0Bu) // TRACE[NVM023]
#define NVM_E_PARAM_BLOCK_DATA_IDX          (0x0Cu) // TRACE[NVM023]
#define NVM_E_PARAM_ADDRESS                 (0x0Du) // TRACE[NVM023]
#define NVM_E_PARAM_DATA                    (0x0Eu) // TRACE[NVM023]
#define NVM_E_PARAM_POINTER                 (0x0Fu) // TRACE[NVM023]
#define NVM_E_BLOCK_WITHOUT_DEFAULTS        (0x11u) // TRACE[SWS_NvM_00023]
                                                    // TRACE[SWS_NvM_91004]
#define NVM_E_NOT_INITIALIZED               (0x14u) // TRACE[NVM586]
#define NVM_E_BLOCK_PENDING                 (0x15u) // TRACE[NVM587]
#define NVM_E_BLOCK_CONFIG                  (0x18u) // TRACE[NVM590]

// The develeopment error NVM_E_WRITE_ONCE_STATUS_UNKNOWN is used only for AR4.5 and newer.
#define NVM_E_WRITE_ONCE_STATUS_UNKNOWN     (0x1Au) // TRACE[SWS_NvM_91004]

/*---------------------------------------------------------------------------------------
 *
 *                      ATTENTION
 *
 * From AR4.5 the NvM shall provide development error NVM_E_WRITE_PROTECTED = 0x1B.
 * Unfortunately in older NvM versions NVM_E_WRITE_PROTECTED has been used as
 * an error bit mask.
 * The usage of NVM_E_WRITE_PROTECTED as an error bit mask is now deprecated.
 * And NVM_E_RB_WRITE_PROTECTED shall be used as error bit mask instead,
 * see the corresponding definition.
 *
 * In the transition time the NvM uses NVM_E_WRITE_PROTECTED as a development error
 * as well as an error bit mask.
 * To avoid backward compatibility issues in the transition time
 * the value of NVM_E_WRITE_PROTECTED will remain 0x20.
 *
 * After the transition time the NvM will use NVM_E_WRITE_PROTECTED only as a development
 * error with value 0x1B as defined in AR4.5.
 *
 *-------------------------------------------------------------------------------------*/


// TRACE[NVM024] Additional DET errors that are detected because of specific implementation
#define NVM_E_CRYPTO_JOB_ID                 (0xF3u) // A callback for Csm jobs has been invoked for a Csm job not started before
#define NVM_E_LENGTH_RANDOM_NUMBER          (0xF4u) // Length of random number differs from the configured one
#define NVM_E_LENGTH_ENCRYPTED              (0xF5u) // Length of encrypted data differs from the configured one
#define NVM_E_LENGTH_DECRYPTED              (0xF6u) // Length of decrypted data differs from the configured one
#define NVM_E_LENGTH_AEAD_TAG               (0xF7u) // Length of the tag used for AEAD encryption differs from the configured one
#define NVM_E_LENGTH_SIGNATURE              (0xF8u) // Length of signature differs from the configured one
#define NVM_E_PERSISTANT_ID_INCONSISTENT    (0xF9u) // persistant ID read with signed data is inconsistent

#define NVM_E_REENTRANT                     (0xFAu) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3266] Re-entrant main function
#define NVM_E_SINGLE_BLOCK_SERVICE_ID       (0xFBu) // Unknown/invalid single block service ID
#define NVM_E_PRV_JOB_ID                    (0xFCu) // Unknown/unexpected NvM-internal job ID
#define NVM_E_MEMIF_JOB_RESULT              (0xFDu) // Invalid MemIf job result
#define NVM_E_RAM_MIRROR_SIZE               (0xFEu) // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3049] NV block length greater then explicit RAM mirror size

//-------------------------------------------------------------------------------------------------
// Run-time errors
//-------------------------------------------------------------------------------------------------
// TRACE[SWS_NvM_00947] Error code for full NvM queue so the request cannot be queued
#if (NVM_AR_RELEASE_MAJOR_VERSION == 4) && (NVM_AR_RELEASE_MINOR_VERSION >= 3)
#define NVM_E_QUEUE_FULL                    (0xA0u) // A service is requested but NvM queue is full
#endif

//-----------------------------------------------------------------------------------------------------------
// Bit masks for state information in the administrative blocks
//-----------------------------------------------------------------------------------------------------------

// TRACE[NVM128] State of the permanent RAM block
#define NVM_BLOCK_STATE_RAM_VALID               (0x01u) // TRACE[NVM132] RAM block is valid
                                                        // TRACE[NVM133] (invalid = !valid)
#define NVM_BLOCK_STATE_RAM_CHANGED             (0x02u) // RAM block is changed
#define NVM_BLOCK_STATE_RAM_MASK                (0x03u) // RAM block attribute mask
#define NVM_BLOCK_STATE_RAM_VALID_CHANGED       (0x03u) // RAM block is valid and changed

// Extended bit mask which has to be set for triggering NvM_WriteAll
// -------------------------------------------------------
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977] Add a separate flag to enable/disable NvM_SetWriteAllTrigger
#define NVM_RB_BLOCK_STATE_MASK_TRG_WRITEALL    (0x04u)

// TRACE[NVM054] Write protection
#define NVM_BLOCK_STATE_WRITE_PROT              (0x08u) // Block is write protected

/**
 * Bit mask definition to access the bit which indicates (when set) that data of the block has to be written during
 * the next write operation. The bit is set after a CRC recalculation if the comparison of the calculated CRC
 * (over the data in the Permanent RAM Block/RAM Mirror) with the previously stored RAM Block CRC indicates that
 * data has been changed.
 */
#define NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ       (0x10u)
#define NVM_BLOCK_STATE_RAM_CRC_NEW_DATA        NVM_BLOCK_STATE_RAM_CRC_WRITE_REQ

/**
 * Bit mask definition to access the bit which indicates (when set) that a CRC recalculation is currently ongoing
 * for the block.
 */
#define NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING  (0x20u)
#define NVM_BLOCK_STATE_RAM_CRC_RECALC_PENDING  NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING

/**
 * Bit mask definition to access the bit which indicates (when set) that a CRC recalculation over the data
 * of the block is needed.
 */
#define NVM_BLOCK_STATE_RAM_CRC_RECALC_REQ      (0x40u)

/**
 * Bit mask definition to access the bit which indicates (when set) that a read is performed
 * regardless of whether it has been successful or not.
 */
#define NVM_BLOCK_STATE_READ_DONE               (0x80u)

//-----------------------------------------------------------------------------------------------------------
// Bit masks for errors helpful during debugging
//-----------------------------------------------------------------------------------------------------------
/**
 * Bit mask definition to access the bit set in case of failed cryptographic service, see also NvM_Rb_stBlockErrors_au8.
 */
#define NVM_E_RB_CRYPTO_SERVICE_FAILED      (0x01u)

/**
 * Bit mask definition to access the bit set in case of failed integrity for data read from the non-volatile memory,
 * see also NvM_Rb_stBlockErrors_au8.
 *
 * Data integrity is failed if data read from the non-volatile memory is inconsistent (e.g. broken CRC) or
 * not available at all (e.g. data was never written before).
 *
 * \attention
 * Formerly NVM_E_INTEGRITY_FAILED was provided as this bit mask. This definition is deprecated.
 * To avoid backward compatibility issues the NvM provides both definitions for the transition time.
 * After the transition time the NvM will provide only NVM_E_RB_INTEGRITY_FAILED.
 */
#define NVM_E_RB_INTEGRITY_FAILED           (0x10u)
#define NVM_E_INTEGRITY_FAILED              NVM_E_RB_INTEGRITY_FAILED

/**
 * Bit mask definition to access the bit set in case of modifying data (write/erase/invalidate) in the non-volatile
 * memory with enabled write protection, see also NvM_Rb_stBlockErrors_au8.
 *
 * \attention
 * Formerly NVM_E_WRITE_PROTECTED was provided as this bit mask. This definition is deprecated.
 * To avoid backward compatibility issues the NvM provides both definitions for the transition time.
 * After the transition time the NvM will provide only NVM_E_RB_WRITE_PROTECTED.
 */
#define NVM_E_RB_WRITE_PROTECTED            (0x20u)
#define NVM_E_WRITE_PROTECTED               NVM_E_RB_WRITE_PROTECTED

/**
 * Bit mask definition to access the bit set in case of failed user request, see NvM_Rb_stBlockErrors_au8.
 *
 * \attention
 * Formerly NVM_E_REQ_FAILED was provided as this bit mask. This definition is deprecated.
 * To avoid backward compatibility issues the NvM provides both definitions for the transition time.
 * After the transition time the NvM will provide only NVM_E_RB_REQ_FAILED.
 */
#define NVM_E_RB_REQ_FAILED                 (0x40u)
#define NVM_E_REQ_FAILED                    NVM_E_RB_REQ_FAILED

/**
 * Bit mask definition to access the bit set in case of rejecting a user request due to internal queue overflow,
 * see also NvM_Rb_stBlockErrors_au8.
 *
 * \attention
 * Formerly NVM_E_QUEUE_OVERFLOW was provided as this bit mask. This definition is deprecated.
 * To avoid backward compatibility issues the NvM provides both definitions for the transition time.
 * After the transition time the NvM will provide only NVM_E_RB_QUEUE_OVERFLOW.
 */
#define NVM_E_RB_QUEUE_OVERFLOW             (0x80u)
#define NVM_E_QUEUE_OVERFLOW                NVM_E_RB_QUEUE_OVERFLOW


//-----------------------------------------------------------------------------------------------------------
// Definitions for giving special block IDs some semantics
//-----------------------------------------------------------------------------------------------------------
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3157] Block ID used for the configuration ID
#define NVM_RB_CONFIG_ID_BLOCK     (1u)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2848] ID of the first NvM user block
#define NVM_RB_FIRST_USER_BLOCK    (2u)


//-----------------------------------------------------------------------------------------------------------
// Function-like macros
//-----------------------------------------------------------------------------------------------------------
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2801] Function-like macro for retrieving the configured NV block length
//                                               of an NvM block
// NvMBlockName is the configured NvM block name
#define NVM_RB_GET_NV_BLOCK_LENGTH(NvMBlockName) NVM_CFG_NV_BLOCK_LENGTH_##NvMBlockName


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
// Variables
// ---------

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

// Variable saving blocks' errors (useful for debugging)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2811] public block specific production error array
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2812] public summary production errors' variable (index # 0)
// This is a public API and shall as such hide implementation-specific details such as the additional admin block for
// auxiliary users, hence it only has NVM_CFG_NR_BLOCKS entries, not NVM_PRV_NR_BLOCKS_ADMIN
extern uint8 NvM_Rb_stBlockErrors_au8[NVM_CFG_NR_BLOCKS];

#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"

// Queue monitoring
extern uint8 NvM_Rb_minNrFreeStdQueueEntries_u8;
# if (NVM_JOB_PRIORITIZATION == STD_ON)
extern uint8 NvM_Rb_minNrFreeImmQueueEntries_u8;
# endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"

#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)

# define NVM_START_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3131]
/// Number of write requests since last power-on.
extern uint16 NvM_Rb_cntrWrites_u16;

# define NVM_STOP_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"

# define NVM_START_SEC_VAR_SAVED_ZONE_32
# include "NvM_MemMap.h"

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3132]
/// Number of bytes written since last power-on.
extern uint32 NvM_Rb_nrBytesWritten_u32;

# define NVM_STOP_SEC_VAR_SAVED_ZONE_32
# include "NvM_MemMap.h"

# define NVM_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
# include "NvM_MemMap.h"

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3133]
/// ID of the last written block.
extern NvM_BlockIdType NvM_Rb_idLastWrittenBlock_uo;
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3134]
/// Structure to store write request counters in persistant way.
extern NvM_Rb_WriteMon_tst NvM_Rb_WriteMon_st;

# define NVM_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
# include "NvM_MemMap.h"

#endif

// Functions: synchronous requests
// -------------------------------
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

// TRACE[NVM447] Service for resetting all internal variables
extern void NvM_Init(void);

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service for setting the data index for a NVRAM block of type dataset.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00448]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex);
#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2840] Service to get the NvM block ID
//                                               using the persistent ID input parameter
extern Std_ReturnType NvM_Rb_GetBlockId(uint16 PersistentId, NvM_BlockIdType *BlockIdPtr);

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service for getting the currently set data index for a NVRAM block of type dataset.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00449]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8 *DataIndexPtr);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service for setting/resetting the write protection for a NVRAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00450]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_SetBlockProtection(NvM_BlockIdType BlockId, boolean ProtectionEnabled);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to read the block dependent error/status information.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00451]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_GetErrorStatus(NvM_BlockIdType BlockId, NvM_RequestResultType *RequestResultPtr);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service for setting the RAM block status of a permanent RAM block or the status
 * of the explicit synchronization of a NVRAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00453]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged);
#endif

// TRACE[NVM548] Service for setting the lock status of a permanent RAM block of an NVRAM block
extern void NvM_SetBlockLockStatus(NvM_BlockIdType BlockId, boolean BlockLocked);

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2827] Service to read the ID of the currently active service
//                                               of the NVRAM manager
extern Std_ReturnType NvM_Rb_GetActiveService(uint8 *ServiceIdPtr);

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3052] Service to read the result of migration operation
extern Std_ReturnType NvM_Rb_GetMigrationResult(NvM_BlockIdType BlockId, NvM_Rb_MigrationResult_ten *MigrationResultPtr);

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2791] Service for getting the configured NV block length
//                                               in bytes of an NvM block
extern Std_ReturnType NvM_Rb_GetNvBlockLength(NvM_BlockIdType BlockId, uint16 *NvBlockLengthPtr);

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2750] Service to read the overall status of the NVRAM manager
extern Std_ReturnType NvM_Rb_GetStatus(NvM_Rb_StatusType *StatusPtr);

#if (NVM_RB_SET_WRITEALL_TRIGGER_API == STD_ON)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2977]
// Service for triggering the WriteAll even if there is a single-block write ongoing
extern Std_ReturnType NvM_Rb_SetWriteAllTrigger(NvM_BlockIdType BlockId, boolean WriteAllTrigger);
#endif

// TRACE[BSW_SWS_AR_NVRAMManager_Ext-3285] Service to get current block write counter
extern Std_ReturnType NvM_Rb_GetWriteCounter(NvM_BlockIdType idBlock_uo, uint32* cntrWrite_puo);

#if (NVM_VERSION_INFO_API == STD_ON)
// TRACE[NVM286] The function NvM_GetVersionInfo shall be pre compile time configurable
//               by the configuration parameter NvMVersionInfoApi.
// TRACE[NVM650] The function NvM_GetVersionInfo shall be configurable On/Off
//               by the configuration parameter NvMVersionInfoApi.
// TRACE[NVM452] Service to get the version information of the NvM module.
extern void NvM_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
#endif

#if (NVM_RB_AUX_INTERFACE == STD_ON)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2940] Service for getting the currently set DataIndex
//                                               of a dataset NVRAM block
extern Std_ReturnType NvM_Rb_AuxGetDataIndex(uint8 *DataIndexPtr);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2939] Service to read the block dependent error/status information
extern Std_ReturnType NvM_Rb_AuxGetErrorStatus(NvM_RequestResultType *RequestResultPtr);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2941] Service for setting the DataIndex of a dataset NVRAM block
extern Std_ReturnType NvM_Rb_AuxSetDataIndex(uint8 DataIndex);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3053] Service to read the result of AUX migration operation
extern Std_ReturnType NvM_Rb_AuxGetMigrationResult(NvM_Rb_MigrationResult_ten *MigrationResultPtr);
#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3105] Service to get the type of the currently active request
extern NvM_Rb_RequestType_ten NvM_Rb_GetRequestType(void);

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3113] Service to get the block ID used in NvM service
//                                               which has caused the last DET error
extern NvM_BlockIdType NvM_Rb_GetBlockIdCausingLastDetError(void);

#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3122] Service to clear write monitoring counters
extern Std_ReturnType NvM_Rb_ClrWriteMonCntrs(void);
#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3169] Service to get block type
extern NvM_BlockManagementType NvM_Rb_GetBlockType(NvM_BlockIdType BlockId);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3167] Service to determine if InitAtLayoutChange is enqueued
extern boolean NvM_Rb_IsInitAtLayoutChangeEnqueued(void);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3164] Service to determine if block is selected for InitAtLayoutChange
extern boolean NvM_Rb_IsBlockSelectedForInitAtLayoutChange(NvM_BlockIdType BlockId);
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3162] Service to determine if block is selected for FirstInitAll
extern boolean NvM_Rb_IsBlockSelectedForFirstInitAll(NvM_BlockIdType BlockId);


// Functions: asynchronous single block requests
// ---------------------------------------------
#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to copy the data of the NV block to its corresponding RAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00454]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifdef NVM_CFG_API_READ_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2910] Support API overloading for NvM_ReadBlock
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
extern Std_ReturnType NvM_Rb_ReadBlockBase(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to copy the data of the NV block to its corresponding permanent RAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00764]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_ReadPRAMBlock(NvM_BlockIdType BlockId);
#endif

#ifdef NVM_CFG_API_READPRAM_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3280] Support API overloading for NvM_ReadPRAMBlock
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
extern Std_ReturnType NvM_Rb_ReadPRAMBlockBase(NvM_BlockIdType BlockId);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to copy the data of the RAM block to its corresponding NV block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00455]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, NvM_Rb_ConstVoidPtr NvM_SrcPtr);
#endif

#ifdef NVM_CFG_API_WRITE_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2913] Support API overloading for NvM_WriteBlock
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
extern Std_ReturnType NvM_Rb_WriteBlockBase(NvM_BlockIdType BlockId, NvM_Rb_ConstVoidPtr NvM_SrcPtr);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to copy the data of the permanent RAM block to its corresponding NV block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00793]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_WritePRAMBlock(NvM_BlockIdType BlockId);
#endif

#ifdef NVM_CFG_API_WRITEPRAM_BLOCK
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3281] Support API overloading for NvM_WritePRAMBlock
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
extern Std_ReturnType NvM_Rb_WritePRAMBlockBase(NvM_BlockIdType BlockId);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to restore the default data to its corresponding RAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00456]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifdef NVM_CFG_API_RESTORE_BLOCK_DEFAULTS
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2911] Support API overloading for NvM_RestoreBlockDefaults
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_RestoreBlockDefaultsBase(NvM_BlockIdType BlockId, NvM_Rb_VoidPtr NvM_DstPtr);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to restore the default data to its corresponding permanent RAM block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00813]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_RestorePRAMBlockDefaults(NvM_BlockIdType BlockId);
#endif

#ifdef NVM_CFG_API_RESTOREPRAM_BLOCK_DEFAULTS
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3282] Support API overloading for NvM_RestorePRAMBlockDefaults
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_RestorePRAMBlockDefaultsBase(NvM_BlockIdType BlockId);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to erase a NV block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00457]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_EraseNvBlock(NvM_BlockIdType BlockId);
#endif

#ifdef NVM_CFG_API_ERASE_NV_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2907] Support API overloading for NvM_EraseNvBlock
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_EraseNvBlockBase(NvM_BlockIdType BlockId);
#endif

#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service to invalidate a NV block.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00459]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern Std_ReturnType NvM_InvalidateNvBlock(NvM_BlockIdType BlockId);
#endif

#ifdef NVM_CFG_API_INVALIDATE_NV_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2908] Support API overloading for NvM_InvalidateNvBlock
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_InvalidateNvBlockBase(NvM_BlockIdType BlockId);
#endif

#if (NVM_RB_AUX_INTERFACE == STD_ON)
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2942] Service to invalidate a NV block
extern Std_ReturnType NvM_Rb_AuxInvalidateNvBlock(NvM_BlockIdType BlockId);
# ifdef NVM_CFG_API_RB_AUX_INVALIDATE_NV_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3275] Support API overloading for NvM_Rb_AuxInvalidateNvBlock
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_AuxInvalidateNvBlockBase(NvM_BlockIdType BlockId);
# endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2943] Service to copy the data of the NV block to its corresponding RAM block
extern Std_ReturnType NvM_Rb_AuxReadBlock(NvM_BlockIdType BlockId, void *NvM_DstPtr);
# ifdef NVM_CFG_API_RB_AUX_READ_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3276] Support API overloading for NvM_Rb_AuxReadBlock
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_AuxReadBlockBase(NvM_BlockIdType BlockId, void *NvM_DstPtr);
# endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2944] Service to copy the data of the RAM block to its corresponding NV block
extern Std_ReturnType NvM_Rb_AuxWriteBlock(NvM_BlockIdType BlockId, const void *NvM_SrcPtr);
# ifdef NVM_CFG_API_RB_AUX_WRITE_BLOCK
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3277] Support API overloading for NvM_Rb_AuxWriteBlock
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern Std_ReturnType NvM_Rb_AuxWriteBlockBase(NvM_BlockIdType BlockId, const void *NvM_SrcPtr);
# endif
#endif


// Functions: asynchronous multi block requests
// --------------------------------------------
// TRACE[NVM460] Initiates a multi block read request
extern void NvM_ReadAll(void);
#ifdef NVM_CFG_API_READ_ALL
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2909] Support API overloading for NvM_ReadAll
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern void NvM_Rb_ReadAllBase(void);
#endif

// TRACE[NVM461] Initiates a multi block write request
extern void NvM_WriteAll(void);
#ifdef NVM_CFG_API_WRITE_ALL
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2912] Support API overloading for NvM_WriteAll
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern void NvM_Rb_WriteAllBase(void);
#endif

// TRACE[NVM458] Service to cancel a running NvM_WriteAll request
extern void NvM_CancelWriteAll(void);
#ifdef NVM_CFG_API_CANCEL_WRITE_ALL
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3274] Support API overloading for NvM_CancelWriteAll
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern void NvM_Rb_CancelWriteAllBase(void);
#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2826] Service to trigger a multi block (re-) initialization operation
extern void NvM_Rb_FirstInitAll(void);
#ifdef NVM_CFG_API_RB_FIRST_INIT_ALL
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3278] Support API overloading for NvM_Rb_FirstInitAll
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern void NvM_Rb_FirstInitAllBase(void);
#endif

// TRACE[NVM855] Initiates a multi block validation request
extern void NvM_ValidateAll(void);
#ifdef NVM_CFG_API_VALIDATE_ALL
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3279] Support API overloading for NvM_ValidateAll
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2916] Also publish the base function in case of API overloading
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2915] Naming pattern for the base function: Rb infix, Base suffix
    extern void NvM_Rb_ValidateAllBase(void);
#endif

// Functions: scheduled
// --------------------
#ifndef NVM_RB_HIDE_RTEAPI
/**
 * Declaration of the service for performing the processing of the NvM jobs.
 * This declaration is used only if RTE generator is NOT used, for details please see coding guidelines
 * Rule BSW_ServiceRTE_002.
 * TRACE[SWS_NvM_00464]
 */
/* MR12 RULE 8.5 VIOLATION: Multiple declaration of this function is due to the fact that RTE also generates a header file. */
extern void NvM_MainFunction(void);
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_H */
#endif
