
#ifndef NVM_PRV_BLOCKDATA_H
#define NVM_PRV_BLOCKDATA_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_BlockDescriptor.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
// Definitions for giving special block IDs some semantics
// ------------------------------------------------------------------------------------------------
/**
 * Definition of the reserved block ID used for multi block operations.
 * TRACE[NVM471]
 */
#define NVM_PRV_MULTI_BLOCK         (0u)

#if (NVM_RB_AUX_INTERFACE == STD_ON)
/**
 * Definition of the block ID used for administrative information related to operations triggered by auxiliary users
 * (for this use case, an additional admin block has been reserved behind all "regular" admin blocks).
 */
# define NVM_PRV_AUX_ADMIN_BLOCK    NVM_CFG_NR_BLOCKS
#endif

//-----------------------------------------------------------------------------------------------------------
// Bit masks for DET errors which can be returned during service initiation.
//-----------------------------------------------------------------------------------------------------------
/**
 * Bit mask definition to access the bit set in case of a block write protected error.
 * (NVM_E_RB_WRITE_PROTECTED)
 */
#define NVM_PRV_BLOCK_WRITE_PROTECTED            (0x01u)
/**
 * Bit mask definition to access the bit set in case of a block write once status unknown error.
( * NVM_E_WRITE_ONCE_STATUS_UNKNOWN)
 */
#define NVM_PRV_BLOCK_WRITE_ONCE_STATUS_UNKNOWN  (0x02u)
/**
 * Bit mask definition to access the bit set in case of a queue full error.
 * (NVM_E_QUEUE_FULL)
 */
#define NVM_PRV_QUEUE_FULL                       (0x04u)
/**
 * Bit mask definition to access the bit set in case of a block pending error.
 * (NVM_E_BLOCK_PENDING)
 */
#define NVM_PRV_BLOCK_PENDING                    (0x08u)

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef boolean (*NvM_Prv_Block_IsNvmEnqueuingForMulti_tpfct)(NvM_BlockIdType idBlock_uo);
typedef uint8 NvM_Prv_BlockErrors_tuo;

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
// Declaration of variables containing administrative data for all NVRAM blocks.
// ------------------------------------------------------------------------------------------------
// TRACE[NVM135] Administrative block is private
#define NVM_START_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"
/**
 * Declaration of the array containing attribute/status information of all NVRAM blocks.
 */
extern uint8 NvM_Prv_stBlock_au8[NVM_CFG_NR_BLOCKS];
#define NVM_STOP_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"


#define NVM_START_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"
#if (defined(NVM_RB_BLOCK_WRITE_COUNTER) && (STD_ON == NVM_RB_BLOCK_WRITE_COUNTER))
/**
 * Declaration of the array containing block write counters.
 *
 * Block write counters are only available if enabled by configuration, see NVM_RB_BLOCK_WRITE_COUNTER.
 */
extern uint32 NvM_Prv_cntrWrite_auo[NVM_CFG_NR_BLOCKS];
#endif
#define NVM_STOP_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"
/**
 * Declaration of the array containing all requests pending on each NVRAM block.
 */
extern uint16 NvM_Prv_stRequests_au16[NVM_PRV_NR_BLOCKS_ADMIN];
#define NVM_STOP_SEC_VAR_CLEARED_16
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/**
 * Declaration of the array containing results of the last asynchronous request for all NVRAM blocks.
 */
extern NvM_RequestResultType NvM_Prv_stRequestResult_au8[NVM_PRV_NR_BLOCKS_ADMIN];
/**
 * Declaration of the array containing indexes for all NVRAM block.
 */
extern uint8 NvM_Prv_idxDataSet_au8[NVM_PRV_NR_BLOCKS_ADMIN];
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

# define NVM_START_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"
/**
 * Declaration of the permanent RAM block for the configuration ID.
 */
extern uint16 NvM_Prv_idConfigStored_u16;
# define NVM_STOP_SEC_VAR_SAVED_ZONE_16
# include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_Block_InitializeData(boolean isSavedZoneDataLost_b);
#if (NVM_RB_AUX_INTERFACE == STD_ON)
extern void NvM_Prv_Block_InitializeDataAux(void);
#endif

extern void NvM_Prv_Block_SetIsNvmEnqueuingForMulti(NvM_Prv_Block_IsNvmEnqueuingForMulti_tpfct IsNvmEnqueuingForMulti_pfct);
extern boolean NvM_Prv_Block_IsNvmEnqueuingForMulti(NvM_BlockIdType idBlock_uo);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline functions
 **********************************************************************************************************************
 */
/* Include the actual definitions of the provided inline functions */
#include "NvM_Prv_BlockData_Inl.h"
#include "NvM_Prv_BlockData_WriteCntr_Inl.h"

/* NVM_PRV_BLOCKDATA_H */
#endif
