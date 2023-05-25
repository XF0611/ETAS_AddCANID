
// TRACE[NVM550] Header file specifying all public NvM types
#ifndef NVM_TYPES_H
#define NVM_TYPES_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
// TRACE[SWS_NvM_00553] Make pre-compile time configuration accessible via public header
#include "NvM_Cfg.h"

// This header file controls centrally all definitions and declarations which will be provided by RTE generator,
// for details please see coding guidelines Rule BSW_ServiceRTE_002.
#include "rba_MemLib_UseRte.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
#ifdef RBA_MEMLIB_USE_RTE

#include "Rte_NvM_Type.h"

#else
// Following type definitions will be used only of RTE generator is not used
/**
 * Definition of the implementation data type for the pointer to a void used in public NvM APIs.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 */
typedef void* NvM_Rb_VoidPtr;

/**
 * Definition of the implementation data type for the pointer to a constant void used in public NvM APIs.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 */
typedef const void* NvM_Rb_ConstVoidPtr;

/**
 * Definition of the implementation data type for block ID used in public NvM APIs.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 * TRACE[SWS_NvM_00471]
 */
typedef uint16 NvM_BlockIdType;

/**
 * Definition of the implementation data type for an array with reserved bytes used in the structure
 * to store write request counters in persistant way.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3127]
 */
typedef uint8 NvM_Rb_WriteMon_Reserved_au8[22];

/**
 * Definition of the implementation data type for a structure to store write request counters in persistant way.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3127]
 */
typedef struct
{
    /// Maximum number of bytes written since last power-on
    uint32 nrBytesWrittenMax_u32;
    /// Accumulated number of all write requests
    uint32 cntrWritesAbs_u32;
    /// Maximum number of write requests since last power-on
    uint16 cntrWritesMax_u16;

    /// Reserved bytes
    NvM_Rb_WriteMon_Reserved_au8 Reserved_au8;

} NvM_Rb_WriteMon_tst;

/**
 * Definition of the implementation data type for possible results of asynchronous requests used in public NvM APIs.
 * If RTE generator is used then this type definition is located in Rte_Type.h
 * TRACE[SWS_NvM_00083]
 * TRACE[SWS_NvM_00470]
 */
typedef uint8 NvM_RequestResultType;

/**
 * Last asynchronous request has been finished successfully.
 * This is the default value after reset.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_OK                 0u
/**
 * Last asynchronous request has been finished unsuccessfully.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_NOT_OK             1u
/**
 * Last asynchronous request is currently pending.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_PENDING            2u
/**
 * Result of the last asynchronous read request is a data integrity failure.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_INTEGRITY_FAILED   3u
/**
 * The referenced block was skipped during execution of NvM_ReadAll or NvM_WriteAll.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_BLOCK_SKIPPED      4u
/**
 * The referenced NV block is invalidated.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_NV_INVALIDATED     5u
/**
 * The multi block request NvM_WriteAll was canceled by calling NvM_CancelWriteAll.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_CANCELED           6u
/**
 * The required redundancy of the referenced NV block is lost (obsolete).
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_REDUNDANCY_FAILED  7u
/**
 * The referenced NV block had the default values copied to the RAM image.
 * If RTE generator is used then this type definition is located in Rte_NvM_Type.h
 * TRACE[SWS_NvM_00470]
 */
# define NVM_REQ_RESTORED_FROM_ROM  8u

#endif

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3050] Enumeration for NvM migration status information
typedef enum
{
    NVM_RB_MIGRATION_RESULT_INIT_E              = 0,    // Initialization value, result not yet available
    NVM_RB_MIGRATION_RESULT_NOT_NECESSARY_E     = 1,    // RAM size == size on medium (migration not necessary)
    NVM_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E   = 2,    // RAM size < size on medium (remaining data discarded)
    NVM_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E    = 3,    // RAM size > size on medium (remaining data filled with zero)
    NVM_RB_MIGRATION_RESULT_NOT_DONE_E          = 4,    // Migration not done, because data were not read
    NVM_RB_MIGRATION_RESULT_DEACTIVATED_E       = 5     // Migrations is deactivated by configuration
}NvM_Rb_MigrationResult_ten;

// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2751] Enumeration for general NvM status information
typedef enum
{
    NVM_RB_STATUS_UNINIT = 0,   // NvM is not initialized yet
    NVM_RB_STATUS_IDLE = 1,     // NvM is currently idle
    NVM_RB_STATUS_BUSY = 2      // NvM is currently busy
} NvM_Rb_StatusType;

// TRACE[NVM137] Enumeration of block management types (public because the enum literal names are defined by AUTOSAR)
typedef enum
{
    NVM_BLOCK_NATIVE,     // TRACE[NVM557] Block is a standard block (no redundant storage, no dataset features)
    NVM_BLOCK_REDUNDANT,  // TRACE[NVM558] Like a native block, but with redundant storage on the memory media
    NVM_BLOCK_DATASET     // TRACE[NVM559] Allows several NV blocks and ROM blocks for one RAM block
} NvM_BlockManagementType;

// Enumeration of NvM activities
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef enum
{
    NVM_PRV_ACTIVITY_NOT_INIT = 0,  // NvM not initialized, job processing prohibited
    NVM_PRV_ACTIVITY_IDLE,          // Arbitrate incoming requests (last arbitration did not find any request)
    NVM_PRV_ACTIVITY_BUSY,          // NvM process user requests
    NVM_PRV_ACTIVITY_RAM_BLOCK_CRC  // Check PRAM block state and recalculate CRC if required
} NvM_Prv_Activities_ten;

/**
 * Definition of enumeration for the block request type.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3061] Enumeration for the block request type
typedef enum
{
    /// Block request type is not available, e.g. NvM not yet initialized.
    NvM_Rb_RequestType_NA_e     = 0,
    /// Multi block request is active.
    NvM_Rb_RequestType_Multi_e,
    /// Single block request is active.
    NvM_Rb_RequestType_Single_e

} NvM_Rb_RequestType_ten;

/**
 * This macro generates lists where each entry corresponds to a CRC type provided by NvM.
 *
 * Each entry of the macro shall look like follows:
 *
 * ENTRY(Type)
 *
 * with:
 * Type     CRC type for which the entry is generated
 */
#define LIST_CRC_TYPE(ENTRY)    \
        ENTRY(NoCrc)            \
        ENTRY(8_Bit)            \
        ENTRY(16_Bit)           \
        ENTRY(32_Bit)           \

#define LIST_ENTRY_CRC_TYPE(Type)     NvM_Prv_Crc_Type_##Type##_e,

/**
 * \brief
 * Definition of different CRC types provided by NvM.
 */
typedef enum
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_CRC_TYPE(LIST_ENTRY_CRC_TYPE)

    //-------------------------------------------------------------------------
    // This is actually no CRC type,
    // in fact this enum value defines count of CRC types available in NvM and
    // can be used for array definition
    //-------------------------------------------------------------------------
    NvM_Prv_Crc_Type_Count_e

} NvM_Prv_Crc_Type_ten;

/**
 * This macro generates lists where each entry corresponds to a job provided by NvM.
 *
 * Each entry of the macro shall look like follows:
 *
 * ENTRY(Name)
 *
 * with:
 * Name          Name of the job for which the entry is generated
 */
#define LIST_ID_JOB(ENTRY)                      \
        ENTRY(Idle)                             \
        ENTRY(Read)                             \
        ENTRY(Write)                            \
        ENTRY(Erase)                            \
        ENTRY(Restore)                          \
        ENTRY(Maintain)                         \
        ENTRY(Validate)                         \
        ENTRY(Invalidate)                       \
        ENTRY(ReadIdConfigForReadAll)           \
        ENTRY(InvalidateForFirstInitAll)        \
        ENTRY(RestoreForImplicitRecovery)       \
        ENTRY(InvalidateForRemoveNonResistant)  \
        ENTRY(RecalcRamBlkCrc)                  \
        ENTRY(WriteAll)                         \
        ENTRY(Suspend)                          \
        ENTRY(Invalid)

#define LIST_ENTRY_ID_JOB(Name)             NvM_Prv_idJob_##Name##_e,

// Enumeration of NvM-internal job IDs
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef enum
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_ID_JOB(LIST_ENTRY_ID_JOB)

    //-------------------------------------------------------------------------
    // This is actually no job ID,
    // in fact this enum value defines count of jobs available in NvM and
    // can be used for array definition
    //-------------------------------------------------------------------------
    NvM_Prv_idJob_Count_e

} NvM_Prv_idJob_ten;

/**
 * This macro generates lists where each entry corresponds to a service bit provided by NvM.
 *
 * Each entry of the macro shall look like follows:
 *
 * ENTRY(ServiceBit, JobName)
 *
 * with:
 * ServiceBit   Service bit for which the entry is generated
 * JobName      Name of the job for which the entry is generated
 */
#define LIST_SERVICE_BIT(ENTRY)                                         \
        ENTRY(ReadAll,              Read                            )   \
        ENTRY(RemoveNonResistant,   InvalidateForRemoveNonResistant )   \
        ENTRY(WriteAll,             WriteAll                        )   \
        ENTRY(FirstInitAll,         Idle                            )   \
        ENTRY(Maintain,             Maintain                        )   \
        ENTRY(InitAtLayoutChange,   Write                           )   \
        ENTRY(ValidateAll,          Validate                        )   \
        ENTRY(NotUsed_0,            Invalid                         )   \
        ENTRY(Read,                 Read                            )   \
        ENTRY(Write,                Write                           )   \
        ENTRY(Invalidate,           Invalidate                      )   \
        ENTRY(Erase,                Erase                           )   \
        ENTRY(Restore,              Restore                         )   \
        ENTRY(NotUsed_1,            Invalid                         )   \
        ENTRY(NotUsed_2,            Invalid                         )   \
        ENTRY(NotUsed_3,            Invalid                         )   \
        ENTRY(Unspecified,          Invalid                         )

#define LIST_ENTRY_SERVICE_BIT(ServiceBit, Unused)      NvM_Prv_ServiceBit_##ServiceBit##_e,

/**
 * \brief
 * Definition of different bits for all asynchronous services provided by NvM.
 * \details
 * These bits are used to define corresponding masks for service flags in the administrative blocks.
 * A set bit in the administrative block signals that the corresponding service has been requested.
 *
 * Definition of bit masks as anonymous enumeration instead of several prepocessor defines is done
 * to get better structuring to show that all these bit masks belongs together.
 */
enum
{
    /* MR12 RULE 20.7 VIOLATION: To create the list the argument shall be not parenthesized */
    LIST_SERVICE_BIT(LIST_ENTRY_SERVICE_BIT)

    NvM_Prv_ServiceBit_nr_e
};

/// Type definition of different bits for all services provided by NvM.
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef uint16 NvM_Prv_ServiceBit_tuo;

/// Type definition for services provided by NvM.
/// NvM defines individual service IDs within NvM.h since these IDs are part of the public interface.
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef uint8 NvM_Prv_idService_tuo;

/// Type definition for DET errors which can be reported by NvM.
/// NvM defines individual DET error IDs within NvM.h since these IDs are part of the public interface.
typedef uint8 NvM_Prv_idDetError_tuo;

/**
 * \brief
 * Definition of different queue IDs used by NvM.
 * \details
 * The order of IDs defines also the priority of the corresponding queue in ascending order .
 */
enum
{
    /// ID of the queue for multi-block requests
    NvM_Prv_idQueue_Multi_e,

#if (NVM_RB_AUX_INTERFACE == STD_ON)
    /// ID of the queue for auxiliary single block requests
    NvM_Prv_idQueue_Aux_e,
#endif

    /// ID of the queue for single block requests with standard priority
    NvM_Prv_idQueue_Standard_e,

// TRACE[NVM564] NvMJobPrioritization enables/disables immediate job processing
#if (NVM_JOB_PRIORITIZATION == STD_ON)
    /// ID of the queue for single block requests with immediate priority
    NvM_Prv_idQueue_Immediate_e,
#endif

    //-------------------------------------------------------------------------
    // Add IDs of new queues before this line
    /// Number of queues available in NvM.
    /// Additionally this value can be used as special ID to avoid enqueuing a request into a queue or
    /// as invalid queue ID.
    NvM_Prv_idQueue_nrQueues_e
};

/// Type definition for queues used by NvM.
typedef uint8 NvM_Prv_idQueue_tuo;

/// Type definition for largest CRC used by NvM.
// TRACE[SWS_NvM_00544] Define data type to allocate memory for largest CRC used by NvM.
typedef union
{
    uint8   Crc8_u8;
    uint16  Crc16_u16;
    uint32  Crc32_u32;

} NvM_Prv_Crc_tun;

typedef union
{
    void*   ptrRamBlock_pv;
    uint8*  ptrRamBlock_pu8;

} NvM_Prv_ptrRamBlock_tun;

typedef union
{
    void const*   ptrRomBlock_pcv;
    uint8 const*  ptrRomBlock_pcu8;

} NvM_Prv_ptrRomBlock_tun;

/// Structure for holding one queue entry.
/// This structure is used for all services (single-block and multi-block).
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef struct
{
    /// ID of the requested block service
    NvM_Prv_idService_tuo idService_uo;

    /// ID of the block for which a service is requested
    NvM_BlockIdType idBlock_uo;

    /// Service bit for the requested block service
    NvM_Prv_ServiceBit_tuo ServiceBit_uo;

    /// Pointer to the buffer holding the block data
    // TRACE[NVM088] No assumptions can be made about alignment, hence a void pointer
    NvM_Prv_ptrRamBlock_tun BlockData_un;

} NvM_Prv_QueueEntry_tst;

// Structure for holding all state information which is not block-specific
// (to have these variables gathered in one struct both optimizes resources and eases debugging)
// TRACE[NVM507] Private type added to public NvM_Types.h to allow AUTOSAR debugging
typedef struct
{
    // General NvM state
    // TRACE[NVM399] Flag indicating whether the NvM already has been initialized
    // TRACE[NVM510] This flag is available for debugging
    NvM_Prv_Activities_ten  Activity_rAMwM_en;
    // ID of the queue with currently ongoing request (NvM_Prv_idQueue_nrQueues_e if no request is currently ongoing)
    NvM_Prv_idQueue_tuo idQueueActive_uo;
    /// ID of the currently active block service
    NvM_Prv_idService_tuo idServiceActive_uo;

} NvM_Prv_MainStates_tst;

/* NVM_TYPES_H */
#endif
