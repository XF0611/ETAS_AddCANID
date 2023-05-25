
#ifndef NVM_PRV_CRC_TYPES_H
#define NVM_PRV_CRC_TYPES_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Job_Types.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/// Type definition for indexes to get access to RAM block CRC.
typedef uint16 NvM_Prv_Crc_idxRamBlock_tuo;

/// Type definition of CRC calculation functions for different CRC types.
typedef void (*NvM_Prv_Crc_Calculation_tpfct)(NvM_Prv_Crc_tun* dataCrc_pun,
                                              uint8 const* Data_pcu8,
                                              uint32 DataLength_u32,
                                              boolean isFirstCall_b);

/// Type definition of functions to set recalculated RAM block CRC for different CRC types.
typedef void (*NvM_Prv_Crc_SetRamBlockCrc_tpfct)(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                                 NvM_Prv_Crc_tun const* Crc_pcun);

/// Type definition of functions to get current RAM block CRC for different CRC types.
typedef void (*NvM_Prv_Crc_GetRamBlockCrc_tpfct)(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                                 NvM_Prv_Crc_tun* Crc_pun);

/// Type definition of functions to check recalculated RAM block CRC for different CRC types.
typedef boolean (*NvM_Prv_Crc_CheckRamBlockCrc_tpfct)(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                                      NvM_Prv_Crc_tun const* Crc_pcun);

/// Type definition of functions to extract the Nv CRC from the internal buffer and check it with the
/// CRC calculated during the read-operation
typedef boolean (*NvM_Prv_Crc_CheckNvCrc_tpfct)(uint8 const* intBuf_pcu8,
                                                uint16 const* ptrUsedSize_pcu16,
                                                NvM_Prv_Crc_tun const* nvCrc_pcun);

/// Type definition of functions to append the calculated CRC to the data in the internal buffer before the acutal
/// write operation
typedef void (*NvM_Prv_Crc_AppendNvCrc_tpfct)(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                              uint8* intBuf_pu8,
                                              uint16* ptrUsedSize_pu16);

/// Structure definition with all functions required for CRC handling for different CRC types.
typedef struct
{
    /// Pointer to the function to calculate CRC
    NvM_Prv_Crc_Calculation_tpfct       Calculation_pfct;
    /// Pointer to the function to set the recalculated RAM block CRC
    NvM_Prv_Crc_SetRamBlockCrc_tpfct    SetRamBlockCrc_pfct;
    /// Pointer to the function to get the current RAM block CRC
    NvM_Prv_Crc_GetRamBlockCrc_tpfct    GetRamBlockCrc_pfct;
    /// Pointer to the function to check the recalculated RAM block CRC
    NvM_Prv_Crc_CheckRamBlockCrc_tpfct  CheckRamBlockCrc_pfct;
    /// Pointer to the function to extract the Nv CRC
    NvM_Prv_Crc_CheckNvCrc_tpfct        CheckNvCrc_pfct;
    /// Pointer to the function to append the CRC to the data in the internal buffer
    NvM_Prv_Crc_AppendNvCrc_tpfct       AppendNvCrc_pfct;

} NvM_Prv_Crc_FctTable_tst;

/// Structure definition with all state variables required for CRC handling.
typedef struct
{
    /// Counter for bytes remaining during CRC recalculation
    uint16                          cntrCrcCalculation_uo;
    /// State of the CRC recalculation.
    /// Due to more efficient implementation NvM uses here the same type as for NVM jobs.
    /// CRC handling uses only the states NvM_Prv_JobStepResult_Succeeded_e and NvM_Prv_JobStepResult_Pending_e.
    NvM_Prv_JobResult_ten           stCalculation_en;
    /// Index of the permanent RAM block CRC to be recalculated in the background.
    NvM_Prv_Crc_idxRamBlock_tuo     idxIdBlockRecalc_uo;

} NvM_Prv_Crc_dataCalculation_tst;

/* NVM_PRV_CRC_TYPES_H */
#endif

