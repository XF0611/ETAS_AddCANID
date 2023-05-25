
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "Crc.h"
// TRACE[NVM089] Check version compatibility of included header files
#if (!defined(CRC_AR_RELEASE_MAJOR_VERSION) || (CRC_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
# error "AUTOSAR major version undefined or mismatched"
#endif

#include "rba_MemLib.h"

#include "NvM_Prv_Crc.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_BlockData.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

// Functions to calculate CRC
static void NvM_Prv_Crc_CalculateCrc8(NvM_Prv_Crc_tun* dataCrc_pun,
                                      uint8 const* Data_pcu8,
                                      uint32 DataLength_u32,
                                      boolean isFirstCall_b);

static void NvM_Prv_Crc_CalculateCrc16(NvM_Prv_Crc_tun* dataCrc_pun,
                                       uint8 const* Data_pcu8,
                                       uint32 DataLength_u32,
                                       boolean isFirstCall_b);

static void NvM_Prv_Crc_CalculateCrc32(NvM_Prv_Crc_tun* dataCrc_pun,
                                       uint8 const* Data_pcu8,
                                       uint32 DataLength_u32,
                                       boolean isFirstCall_b);

// Functions to set calculated CRC
static void NvM_Prv_Crc_SetRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                        NvM_Prv_Crc_tun const* Crc_pcun);

static void NvM_Prv_Crc_SetRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun const* Crc_pcun);

static void NvM_Prv_Crc_SetRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun const* Crc_pcun);

// Functions to get current CRC
static void NvM_Prv_Crc_GetRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                        NvM_Prv_Crc_tun* Crc_pun);

static void NvM_Prv_Crc_GetRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun* Crc_pun);

static void NvM_Prv_Crc_GetRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun* Crc_pun);

// Functions to check RAM block CRC
static boolean NvM_Prv_Crc_CheckRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                             NvM_Prv_Crc_tun const* Crc_pcun);

static boolean NvM_Prv_Crc_CheckRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                              NvM_Prv_Crc_tun const* Crc_pcun);

static boolean NvM_Prv_Crc_CheckRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                              NvM_Prv_Crc_tun const* Crc_pcun);

// Functions to extract the Nv Crc from the internal buffer and check it
static boolean NvM_Prv_Crc_CheckCrc8(uint8 const* intBuf_pcu8,
                                     uint16 const* nrUsedBytes_pcu16,
                                     NvM_Prv_Crc_tun const* nvCrc_pcun);

static boolean NvM_Prv_Crc_CheckCrc16(uint8 const* intBuf_pcu8,
                                      uint16 const* nrUsedBytes_pcu16,
                                      NvM_Prv_Crc_tun const* nvCrc_pcun);

static boolean NvM_Prv_Crc_CheckCrc32(uint8 const* intBuf_pcu8,
                                      uint16 const* nrUsedBytes_pcu16,
                                      NvM_Prv_Crc_tun const* nvCrc_pcun);

// Functions to append the Crc to the internal buffer
static void NvM_Prv_Crc_AppendCrc8(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                   uint8* intBuf_pu8,
                                   uint16* nrUsedBytes_pu16);

static void NvM_Prv_Crc_AppendCrc16(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                    uint8* intBuf_pu8,
                                    uint16* nrUsedBytes_pu16);

static void NvM_Prv_Crc_AppendCrc32(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                    uint8* intBuf_pu8,
                                    uint16* nrUsedBytes_pu16);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
/// Definition of the constant array with functions to handle all CRC types provided by the NvM.
static NvM_Prv_Crc_FctTable_tst const NvM_Prv_Crc_FctTable_cast[NvM_Prv_Crc_Type_Count_e] =
{
        // Functions if no CRC is available
        {
                NULL_PTR,
                NULL_PTR,
                NULL_PTR,
                NULL_PTR,
                NULL_PTR,
                NULL_PTR

        },
        // Functions for CRC8 handling
        {
                /// Pointer to the function to calculate CRC8
                NvM_Prv_Crc_CalculateCrc8,
                /// Pointer to the function to set the recalculated RAM block CRC8
                NvM_Prv_Crc_SetRamBlockCrc8,
                /// Pointer to the function to get the current RAM block CRC8
                NvM_Prv_Crc_GetRamBlockCrc8,
                /// Pointer to the function to check the recalculated RAM block CRC8
                NvM_Prv_Crc_CheckRamBlockCrc8,
                /// Pointer to the function to extract the Nv CRC from the internal buffer CRC8
                NvM_Prv_Crc_CheckCrc8,
                /// Pointer to the function to append the CRC to the data in the internal buffer CRC8
                NvM_Prv_Crc_AppendCrc8
        },
        // Functions for CRC16 handling
        {
                /// Pointer to the function to calculate CRC16
                NvM_Prv_Crc_CalculateCrc16,
                /// Pointer to the function to set the recalculated RAM block CRC16
                NvM_Prv_Crc_SetRamBlockCrc16,
                /// Pointer to the function to get the current RAM block CRC16
                NvM_Prv_Crc_GetRamBlockCrc16,
                /// Pointer to the function to check the recalculated RAM block CRC16
                NvM_Prv_Crc_CheckRamBlockCrc16,
                /// Pointer to the function to extract the Nv CRC from the internal buffer CRC16
                NvM_Prv_Crc_CheckCrc16,
                /// Pointer to the function to append the CRC to the data in the internal buffer CRC16
                NvM_Prv_Crc_AppendCrc16
        },
        // Functions for CRC32 handling
        {
                /// Pointer to the function to calculate CRC32
                NvM_Prv_Crc_CalculateCrc32,
                /// Pointer to the function to set the recalculated RAM block CRC32
                NvM_Prv_Crc_SetRamBlockCrc32,
                /// Pointer to the function to get the current RAM block CRC32
                NvM_Prv_Crc_GetRamBlockCrc32,
                /// Pointer to the function to check the recalculated RAM block CRC32
                NvM_Prv_Crc_CheckRamBlockCrc32,
                /// Pointer to the function to extract the Nv CRC from the internal buffer CRC32
                NvM_Prv_Crc_CheckCrc32,
                /// Pointer to the function to append the CRC to the data in the internal buffer CRC32
                NvM_Prv_Crc_AppendCrc32
        }
};
#endif

#define NVM_STOP_SEC_CONST_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"

#if (defined(NVM_CFG_CRC_8_NR_RAM_BLOCKS) && (NVM_CFG_CRC_8_NR_RAM_BLOCKS > 0u))
/// Array definition for all RAM block CRCs of type CRC8 configured in NvM.
static uint8 NvM_Prv_Crc_RamBlockCrc8_au8[NVM_CFG_CRC_8_NR_RAM_BLOCKS];
#endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_SAVED_ZONE_16
#include "NvM_MemMap.h"

#if (defined(NVM_CFG_CRC_16_NR_RAM_BLOCKS) && (NVM_CFG_CRC_16_NR_RAM_BLOCKS > 0u))
/// Array definition for all RAM block CRCs of type CRC16 configured in NvM.
static uint16 NvM_Prv_Crc_RamBlockCrc16_au16[NVM_CFG_CRC_16_NR_RAM_BLOCKS];
#endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_16
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"

#if (defined(NVM_CFG_CRC_32_NR_RAM_BLOCKS) && (NVM_CFG_CRC_32_NR_RAM_BLOCKS > 0u))
/// Array definition for all RAM block CRCs of type CRC32 configured in NvM.
static uint32 NvM_Prv_Crc_RamBlockCrc32_au32[NVM_CFG_CRC_32_NR_RAM_BLOCKS];
#endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_32
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
/// State variables required for CRC handling.
static NvM_Prv_Crc_dataCalculation_tst NvM_Prv_Crc_dataCalculation_st;
#endif

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

 /*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))

static void NvM_Prv_Crc_CalculateCrc8(NvM_Prv_Crc_tun* dataCrc_pun,
                                      uint8 const* Data_pcu8,
                                      uint32 DataLength_u32,
                                      boolean isFirstCall_b)
{
    dataCrc_pun->Crc8_u8 = Crc_CalculateCRC8(Data_pcu8,
                                             DataLength_u32,
                                             dataCrc_pun->Crc8_u8,
                                             isFirstCall_b);
}

static void NvM_Prv_Crc_CalculateCrc16(NvM_Prv_Crc_tun* dataCrc_pun,
                                       uint8 const* Data_pcu8,
                                       uint32 DataLength_u32,
                                       boolean isFirstCall_b)
{
    dataCrc_pun->Crc16_u16 = Crc_CalculateCRC16(Data_pcu8,
                                                DataLength_u32,
                                                dataCrc_pun->Crc16_u16,
                                                isFirstCall_b);
}

static void NvM_Prv_Crc_CalculateCrc32(NvM_Prv_Crc_tun* dataCrc_pun,
                                       uint8 const* Data_pcu8,
                                       uint32 DataLength_u32,
                                       boolean isFirstCall_b)
{
    dataCrc_pun->Crc32_u32 = Crc_CalculateCRC32(Data_pcu8,
                                                DataLength_u32,
                                                dataCrc_pun->Crc32_u32,
                                                isFirstCall_b);
}

static void NvM_Prv_Crc_SetRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                        NvM_Prv_Crc_tun const* Crc_pcun)
{
# if (defined(NVM_CFG_CRC_8_NR_RAM_BLOCKS) && (NVM_CFG_CRC_8_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_8_NR_RAM_BLOCKS)
    {
        NvM_Prv_Crc_RamBlockCrc8_au8[idxRamBlockCrc_uo] = Crc_pcun->Crc8_u8;
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif
}

static void NvM_Prv_Crc_SetRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun const* Crc_pcun)
{
# if (defined(NVM_CFG_CRC_16_NR_RAM_BLOCKS) && (NVM_CFG_CRC_16_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_16_NR_RAM_BLOCKS)
    {
        NvM_Prv_Crc_RamBlockCrc16_au16[idxRamBlockCrc_uo] = Crc_pcun->Crc16_u16;
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif
}

static void NvM_Prv_Crc_SetRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun const* Crc_pcun)
{
# if (defined(NVM_CFG_CRC_32_NR_RAM_BLOCKS) && (NVM_CFG_CRC_32_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_32_NR_RAM_BLOCKS)
    {
        NvM_Prv_Crc_RamBlockCrc32_au32[idxRamBlockCrc_uo] = Crc_pcun->Crc32_u32;
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif
}

/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
static void NvM_Prv_Crc_GetRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                        NvM_Prv_Crc_tun* Crc_pun)
{
# if (defined(NVM_CFG_CRC_8_NR_RAM_BLOCKS) && (NVM_CFG_CRC_8_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_8_NR_RAM_BLOCKS)
    {
        Crc_pun->Crc8_u8 = NvM_Prv_Crc_RamBlockCrc8_au8[idxRamBlockCrc_uo];
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pun;
# endif
}

/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
static void NvM_Prv_Crc_GetRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun* Crc_pun)
{
# if (defined(NVM_CFG_CRC_16_NR_RAM_BLOCKS) && (NVM_CFG_CRC_16_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_16_NR_RAM_BLOCKS)
    {
        Crc_pun->Crc16_u16 = NvM_Prv_Crc_RamBlockCrc16_au16[idxRamBlockCrc_uo];
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pun;
# endif
}

/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
static void NvM_Prv_Crc_GetRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                         NvM_Prv_Crc_tun* Crc_pun)
{
# if (defined(NVM_CFG_CRC_32_NR_RAM_BLOCKS) && (NVM_CFG_CRC_32_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_32_NR_RAM_BLOCKS)
    {
        Crc_pun->Crc32_u32 = NvM_Prv_Crc_RamBlockCrc32_au32[idxRamBlockCrc_uo];
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pun;
# endif
}

static boolean NvM_Prv_Crc_CheckRamBlockCrc8(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                             NvM_Prv_Crc_tun const* Crc_pcun)
{
    boolean isRamBlockCrcConsistent_b = FALSE;

# if (defined(NVM_CFG_CRC_8_NR_RAM_BLOCKS) && (NVM_CFG_CRC_8_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_8_NR_RAM_BLOCKS)
    {
        isRamBlockCrcConsistent_b = (NvM_Prv_Crc_RamBlockCrc8_au8[idxRamBlockCrc_uo] == Crc_pcun->Crc8_u8);
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif

    return isRamBlockCrcConsistent_b;
}

static boolean NvM_Prv_Crc_CheckRamBlockCrc16(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                              NvM_Prv_Crc_tun const* Crc_pcun)
{
    boolean isRamBlockCrcConsistent_b = FALSE;

# if (defined(NVM_CFG_CRC_16_NR_RAM_BLOCKS) && (NVM_CFG_CRC_16_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_16_NR_RAM_BLOCKS)
    {
        isRamBlockCrcConsistent_b = (NvM_Prv_Crc_RamBlockCrc16_au16[idxRamBlockCrc_uo] == Crc_pcun->Crc16_u16);
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif

    return isRamBlockCrcConsistent_b;
}

static boolean NvM_Prv_Crc_CheckRamBlockCrc32(NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo,
                                              NvM_Prv_Crc_tun const* Crc_pcun)
{
    boolean isRamBlockCrcConsistent_b = FALSE;

# if (defined(NVM_CFG_CRC_32_NR_RAM_BLOCKS) && (NVM_CFG_CRC_32_NR_RAM_BLOCKS > 0u))
    if (idxRamBlockCrc_uo < NVM_CFG_CRC_32_NR_RAM_BLOCKS)
    {
        isRamBlockCrcConsistent_b = (NvM_Prv_Crc_RamBlockCrc32_au32[idxRamBlockCrc_uo] == Crc_pcun->Crc32_u32);
    }
# else
    (void)idxRamBlockCrc_uo;
    (void)Crc_pcun;
# endif

    return isRamBlockCrcConsistent_b;
}


// Functions to extract the Nv CRC from the internal buffer used

static boolean NvM_Prv_Crc_CheckCrc8(uint8 const* intBuf_pcu8,
                                     uint16 const* nrUsedBytes_pcu16,
                                     NvM_Prv_Crc_tun const* nvCrc_pcun)
{
    boolean isCrcEqual = FALSE;
    uint8 extractedCrc;

    // extract the Nv CRC
    extractedCrc = intBuf_pcu8[*nrUsedBytes_pcu16];
    // compare the Nv CRC with the CRC calculated
    if (nvCrc_pcun->Crc8_u8 == extractedCrc)
    {
        isCrcEqual = TRUE;
    }

    return isCrcEqual;
}

static boolean NvM_Prv_Crc_CheckCrc16(uint8 const* intBuf_pcu8,
                                      uint16 const* nrUsedBytes_pcu16,
                                      NvM_Prv_Crc_tun const* nvCrc_pcun)
{
    boolean isCrcEqual = FALSE;
    uint16 extractedCrc;

    // extract the Nv CRC
    rba_MemLib_MemCopy(&intBuf_pcu8[*nrUsedBytes_pcu16],
                       (uint8*)(&extractedCrc),
                       sizeof(uint16));
    // compare the Nv CRC with the CRC calculated
    if (nvCrc_pcun->Crc16_u16 == extractedCrc)
    {
        isCrcEqual = TRUE;
    }

    return isCrcEqual;
}

static boolean NvM_Prv_Crc_CheckCrc32(uint8 const* intBuf_pcu8,
                                      uint16 const* nrUsedBytes_pcu16,
                                      NvM_Prv_Crc_tun const* nvCrc_pcun)
{
    boolean isCrcEqual = FALSE;
    uint32 extractedCrc;

    // extract the Nv CRC
    rba_MemLib_MemCopy(&intBuf_pcu8[*nrUsedBytes_pcu16],
                       (uint8*)(&extractedCrc),
                       sizeof(uint32));
    // compare the Nv CRC with the CRC calculated
    if (nvCrc_pcun->Crc32_u32 == extractedCrc)
    {
        isCrcEqual = TRUE;
    }

    return isCrcEqual;
}


// Functions to append the Crc to the internal buffer

static void NvM_Prv_Crc_AppendCrc8(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                   uint8* intBuf_pu8,
                                   uint16* nrUsedBytes_pu16)
{
    // copy 1 Byte directly
    intBuf_pu8[*nrUsedBytes_pu16] = nvCrc_pcun->Crc8_u8;

# if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    NvM_Prv_Crc_InjectNvCrcError(&intBuf_pu8[*nrUsedBytes_pu16]);
# endif

    *nrUsedBytes_pu16 += (uint16)sizeof(uint8);

    return;
}

static void NvM_Prv_Crc_AppendCrc16(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                    uint8* intBuf_pu8,
                                    uint16* nrUsedBytes_pu16)
{
    rba_MemLib_MemCopy((uint8 const*)(&nvCrc_pcun->Crc16_u16),
                       &intBuf_pu8[*nrUsedBytes_pu16],
                       sizeof(uint16));

# if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    NvM_Prv_Crc_InjectNvCrcError(&intBuf_pu8[*nrUsedBytes_pu16]);
# endif

    *nrUsedBytes_pu16 += (uint16)sizeof(uint16);

    return;
}

static void NvM_Prv_Crc_AppendCrc32(NvM_Prv_Crc_tun const* nvCrc_pcun,
                                    uint8* intBuf_pu8,
                                    uint16* nrUsedBytes_pu16)
{
    rba_MemLib_MemCopy((uint8 const*)(&nvCrc_pcun->Crc32_u32),
                       &intBuf_pu8[*nrUsedBytes_pu16],
                       sizeof(uint32));

# if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    NvM_Prv_Crc_InjectNvCrcError(&intBuf_pu8[*nrUsedBytes_pu16]);
# endif

    *nrUsedBytes_pu16 += (uint16)sizeof(uint32);

    return;
}

static NvM_Prv_Crc_FctTable_tst const* NvM_Prv_Crc_GetFctTable(NvM_Prv_Crc_Type_ten Type_en)
{
    if (Type_en >= NvM_Prv_Crc_Type_Count_e)
    {
        Type_en = NvM_Prv_Crc_Type_NoCrc_e;
    }
    return &(NvM_Prv_Crc_FctTable_cast[Type_en]);
}

#endif


void NvM_Prv_Crc_Initialize(void)
{
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))

    NvM_Prv_Crc_dataCalculation_st.idxIdBlockRecalc_uo = 0u;
    NvM_Prv_Crc_dataCalculation_st.stCalculation_en = NvM_Prv_JobResult_Succeeded_e;
    NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo = 0u;

#endif
}

void NvM_Prv_Crc_Calculate(NvM_Prv_stJob_ten* stJob_pen,
                           NvM_Prv_JobResult_tst* JobResult_pst,
                           NvM_Prv_JobData_tst const* JobData_pcst)
{
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))

    // Calculate CRC only for blocks which have configured a CRC
    // Valid CRC type is configured for blocks using:
    // - RAM block CRC
    // - NV CRC
    // - encryption of user data
    NvM_Prv_Crc_Type_ten CrcType_en = NvM_Prv_GetCrcType(JobData_pcst->idBlock_uo);

    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;

    if (NvM_Prv_Crc_Type_NoCrc_e != CrcType_en)
    {
        uint16 nrBytesToCalculate_u16;
        NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(CrcType_en);

        if (NULL_PTR != FctTable_pcst->Calculation_pfct)
        {
            uint16 OffsetData_u16;

            if (NvM_Prv_JobResult_Pending_e != NvM_Prv_Crc_dataCalculation_st.stCalculation_en)
            {
                NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo = JobResult_pst->CrcData_st.Calculation_st.Length_u16;
                NvM_Prv_Crc_dataCalculation_st.stCalculation_en = NvM_Prv_JobResult_Pending_e;
            }

            if (NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo <= NVM_CRC_NUM_OF_BYTES)
            {
                nrBytesToCalculate_u16 = NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo;
            }
            else
            {
                nrBytesToCalculate_u16 = NVM_CRC_NUM_OF_BYTES;
            }

            OffsetData_u16 = (JobResult_pst->CrcData_st.Calculation_st.Length_u16 -
                              NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo);
            FctTable_pcst->Calculation_pfct(&JobResult_pst->CrcData_st.Calculation_st.Crc_un,
                                            &JobResult_pst->CrcData_st.Calculation_st.Buffer_pu8[OffsetData_u16],
                                            nrBytesToCalculate_u16,
                                            JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b);
            JobResult_pst->CrcData_st.Calculation_st.isFirstCall_b = FALSE;

            NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo -= nrBytesToCalculate_u16;
            // Check whether CRC calculation has finished (all bytes proceeded)
            if (0u == NvM_Prv_Crc_dataCalculation_st.cntrCrcCalculation_uo)
            {
                NvM_Prv_Crc_dataCalculation_st.stCalculation_en = NvM_Prv_JobResult_Succeeded_e;
            }
            JobResult_pst->Result_en = NvM_Prv_Crc_dataCalculation_st.stCalculation_en;
        }
    }
#else
    (void)JobData_pcst;
    JobResult_pst->Result_en = NvM_Prv_JobResult_Succeeded_e;
#endif

    // Set current job status at the end of the function to avoid compiler and MISRA warnings
    // that stJob_pen can be a pointer to const.
    *stJob_pen = NvM_Prv_stJob_DoCrc_e;
}

boolean NvM_Prv_Crc_CheckRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun const* Crc_pcun)
{
    boolean isRamBlockCrcConsistent_b = FALSE;

#if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo = NvM_Prv_GetIdxRamBlockCrc(idBlock_uo);

    if (NULL_PTR != FctTable_pcst->CheckRamBlockCrc_pfct)
    {
        isRamBlockCrcConsistent_b = FctTable_pcst->CheckRamBlockCrc_pfct(idxRamBlockCrc_uo, Crc_pcun);
    }
#else
    (void)idBlock_uo;
    (void)Crc_pcun;
#endif

    return isRamBlockCrcConsistent_b;
}

void NvM_Prv_Crc_SetRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun const* Crc_pcun)
{
#if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo = NvM_Prv_GetIdxRamBlockCrc(idBlock_uo);

    if (NULL_PTR != FctTable_pcst->SetRamBlockCrc_pfct)
    {
        FctTable_pcst->SetRamBlockCrc_pfct(idxRamBlockCrc_uo, Crc_pcun);
    }
#else
    (void)idBlock_uo;
    (void)Crc_pcun;
#endif
}


boolean NvM_Prv_Crc_CheckCrc(NvM_BlockIdType idBlock_uo,
                             uint8 const* intBuf_pcu8,
                             uint16 const* nrUsedBytes_pcu16,
                             NvM_Prv_Crc_tun const* Crc_pcun)
{
    boolean isCrcEqual_b = FALSE;

#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_CheckNvCrc_tpfct funcPtr_pfct = FctTable_pcst->CheckNvCrc_pfct;

    if (NULL_PTR != funcPtr_pfct)
    {
        isCrcEqual_b = funcPtr_pfct(intBuf_pcu8, nrUsedBytes_pcu16, Crc_pcun);
    }

# if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    if (TestCd_NvM_st.Arguments.CryptoCrcCheckFail_b)
    {
        isCrcEqual_b = FALSE;
    }
# endif
#else
    (void)idBlock_uo;
    (void)intBuf_pcu8;
    (void)nrUsedBytes_pcu16;
    (void)Crc_pcun;
#endif

    return isCrcEqual_b;
}


/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
void NvM_Prv_Crc_AppendCrc(NvM_BlockIdType idBlock_uo,
                           uint8* intBuf_pu8,
                           uint16* nrUsedBytes_pu16,
                           NvM_Prv_Crc_tun const* Crc_pcun)
{
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_AppendNvCrc_tpfct funcPtr_pfct = FctTable_pcst->AppendNvCrc_pfct;

    // Append the CRC to the user-data in the internal Buffer
    if (NULL_PTR != funcPtr_pfct)
    {
        funcPtr_pfct(Crc_pcun, intBuf_pu8, nrUsedBytes_pu16);
    }

#else
    (void)idBlock_uo;
    (void)intBuf_pu8;
    (void)nrUsedBytes_pu16;
    (void)Crc_pcun;
#endif

    return;
}

/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
boolean NvM_Prv_Crc_IsRamBlockCrcRecalcRequired(NvM_BlockIdType* idBlock_puo)
{
    boolean isRecalculationRequired_b = FALSE;

#if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    NvM_BlockIdType idBlock_uo;
    NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo;
    NvM_Prv_Crc_idxRamBlock_tuo cntrRamBlockCrc_uo = 0u;

    // Find index of the RAM block for which CRC will be recalculated
    do
    {
        ++cntrRamBlockCrc_uo;
        idxRamBlockCrc_uo =
                (NvM_Prv_Crc_dataCalculation_st.idxIdBlockRecalc_uo + cntrRamBlockCrc_uo) % NVM_CFG_CRC_NR_RAM_BLOCKS;
        idBlock_uo = NvM_Prv_idBlockRamCrc_cauo[idxRamBlockCrc_uo];
        isRecalculationRequired_b = NvM_Prv_Block_IsRamCrcRecalcRequired(idBlock_uo);

    } while(!isRecalculationRequired_b &&
            (cntrRamBlockCrc_uo < NVM_CFG_CRC_NR_RAM_BLOCKS));

    if (isRecalculationRequired_b)
    {
        // Set index of the permanent RAM block for which CRC will be recalculated
        NvM_Prv_Crc_dataCalculation_st.idxIdBlockRecalc_uo = idxRamBlockCrc_uo;
        // Set pending state of the RAM block CRC recalculation
        // to detect if user has modified data during recalculation
        SchM_Enter_NvM_Main();
        NvM_Prv_Block_SetState(idBlock_uo,
                               NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING,
                               NVM_BLOCK_STATE_RAM_CRC_RECALC_ONGOING);
        SchM_Exit_NvM_Main();
        *idBlock_puo = idBlock_uo;
    }
#else
    (void)idBlock_puo;
#endif

    return isRecalculationRequired_b;
}


#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

void NvM_Prv_Crc_ResetAllRamBlockCrcs(void)
{
# if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))
    uint16 cntrRamBlockCrc_u16;
# endif

# if (defined(NVM_CFG_CRC_8_NR_RAM_BLOCKS) && (NVM_CFG_CRC_8_NR_RAM_BLOCKS > 0u))
    for (cntrRamBlockCrc_u16 = 0; cntrRamBlockCrc_u16 < NVM_CFG_CRC_8_NR_RAM_BLOCKS; ++cntrRamBlockCrc_u16)
    {
        NvM_Prv_Crc_RamBlockCrc8_au8[cntrRamBlockCrc_u16] = 0;
    }
# endif

# if (defined(NVM_CFG_CRC_16_NR_RAM_BLOCKS) && (NVM_CFG_CRC_16_NR_RAM_BLOCKS > 0u))
    for (cntrRamBlockCrc_u16 = 0; cntrRamBlockCrc_u16 < NVM_CFG_CRC_16_NR_RAM_BLOCKS; ++cntrRamBlockCrc_u16)
    {
        NvM_Prv_Crc_RamBlockCrc16_au16[cntrRamBlockCrc_u16] = 0;
    }
# endif

# if (defined(NVM_CFG_CRC_32_NR_RAM_BLOCKS) && (NVM_CFG_CRC_32_NR_RAM_BLOCKS > 0u))
    for (cntrRamBlockCrc_u16 = 0; cntrRamBlockCrc_u16 < NVM_CFG_CRC_32_NR_RAM_BLOCKS; ++cntrRamBlockCrc_u16)
    {
        NvM_Prv_Crc_RamBlockCrc32_au32[cntrRamBlockCrc_u16] = 0;
    }
# endif
}

void NvM_Prv_Crc_ResetRamBlockCrc(NvM_BlockIdType idBlock_uo)
{
# if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))
    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo = NvM_Prv_GetIdxRamBlockCrc(idBlock_uo);
    NvM_Prv_Crc_tun Crc_un;
    Crc_un.Crc32_u32 = 0u;

    if (NULL_PTR != FctTable_pcst->SetRamBlockCrc_pfct)
    {
        FctTable_pcst->SetRamBlockCrc_pfct(idxRamBlockCrc_uo, &Crc_un);
    }
#else
    (void)idBlock_uo;
#endif
}


/* MR12 RULE 8.13 VIOLATION: The statements within the function can be disabled via a compiler-switch but the parameter
   is still required to be changeable if the statements within the function are active. */
void NvM_Prv_Crc_GetRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun* Crc_pun)
{
# if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))

    NvM_Prv_Crc_FctTable_tst const* FctTable_pcst = NvM_Prv_Crc_GetFctTable(NvM_Prv_GetCrcType(idBlock_uo));
    NvM_Prv_Crc_idxRamBlock_tuo idxRamBlockCrc_uo = NvM_Prv_GetIdxRamBlockCrc(idBlock_uo);

    if (NULL_PTR != FctTable_pcst->GetRamBlockCrc_pfct)
    {
        FctTable_pcst->GetRamBlockCrc_pfct(idxRamBlockCrc_uo, Crc_pun);
    }
# else
    (void)idBlock_uo;
    (void)Crc_pun;
# endif
}

NvM_BlockIdType NvM_Prv_Crc_GetIdRamBlockCrcRecalc(void)
{
# if (defined(NVM_CALC_RAM_BLOCK_CRC) && (NVM_CALC_RAM_BLOCK_CRC == STD_ON))
    return NvM_Prv_idBlockRamCrc_cauo[NvM_Prv_Crc_dataCalculation_st.idxIdBlockRecalc_uo];
# else
    return 0u;
# endif
}


void NvM_Prv_Crc_InjectNvCrcError(uint8* crcByte_pu8)
{
#if (defined(NVM_CALC_CRC) && (NVM_CALC_CRC == STD_ON))
    if (TRUE == TestCd_NvM_st.Arguments.WrongNvCrcEna)
    {
        // Negate the Crc Byte
        *crcByte_pu8 = ~(*crcByte_pu8);
    }
# else
    (void)crcByte_pu8;
# endif

    return;
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

