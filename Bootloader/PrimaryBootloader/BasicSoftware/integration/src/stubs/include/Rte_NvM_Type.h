

#ifndef RTE_NvM_TYPE_H
#define RTE_NvM_TYPE_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "NvM_Cfg.h"

// TRACE[NVM089] Check version compatibility of included header files
#if (!defined(STD_TYPES_AR_RELEASE_MAJOR_VERSION) || (STD_TYPES_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
 #error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(STD_TYPES_AR_RELEASE_MINOR_VERSION) || ((STD_TYPES_AR_RELEASE_MINOR_VERSION != 0) && \
                                                      (STD_TYPES_AR_RELEASE_MINOR_VERSION != 2)))
 #error "AUTOSAR minor version undefined or mismatched"
#endif

/*
 **********************************************************************************************************************
 * Type definitions (RTE generator would put this into Rte_Type.h)
 **********************************************************************************************************************
*/

// TRACE[NVM471] Type for block IDs
typedef uint16 NvM_BlockIdType;

typedef uint8 NvM_Rb_WriteMon_Reserved_au8[22];
/**
 * Definition of a structure to store write request counters in persistant way.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3127] Structure type for write request counters
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

// TRACE[NVM083] Type for asynchronous request results
typedef uint8 NvM_RequestResultType;

/*
 **********************************************************************************************************************
 * Defines/Macros (RTE generator would put this into Rte_NvM_Type.h)
 **********************************************************************************************************************
*/

// TRACE[NVM470] Request results
#define NVM_REQ_OK                  0       // Last asynchronous read/write/control request finished successfully
#define NVM_REQ_NOT_OK              1       // Last asynchronous read/write/control request finished unsuccessfully
#define NVM_REQ_PENDING             2       // An asynchronous read/write/control request is currently pending
#define NVM_REQ_INTEGRITY_FAILED    3       // Data integrity failure for last read operation
#define NVM_REQ_BLOCK_SKIPPED       4       // Referenced block skipped in NvM_ReadAll or NvM_WriteAll
#define NVM_REQ_NV_INVALIDATED      5       // The reference block is invalidated
#define NVM_REQ_CANCELED            6       // Multi-block write request is canceled
#define NVM_REQ_REDUNDANCY_FAILED   7       // The required redundancy of the referenced NV block is lost
#define NVM_REQ_RESTORED_FROM_ROM   8       // The referenced NV block has been restored from ROM

/* RTE_NVM_TYPE_H */
#endif

/*<BASDKey>
**********************************************************************************************************************
* $History__:$
**********************************************************************************************************************
</BASDKey>*/

/*<BASDKey>
**********************************************************************************************************************
* End of header file: $Name___:$
**********************************************************************************************************************
</BASDKey>*/
