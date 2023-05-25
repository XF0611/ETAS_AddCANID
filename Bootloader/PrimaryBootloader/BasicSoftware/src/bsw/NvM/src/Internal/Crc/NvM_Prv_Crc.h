
#ifndef NVM_PRV_CRC_H
#define NVM_PRV_CRC_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Types.h"
#include "NvM_Prv_Crc_Types.h"
#include "NvM_Prv_BlockDescriptor.h"
/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_Crc_Initialize(void);
extern void NvM_Prv_Crc_Calculate(NvM_Prv_stJob_ten* stJob_pen,
                                  NvM_Prv_JobResult_tst* JobResult_pst,
                                  NvM_Prv_JobData_tst const* JobData_pcst);
extern boolean NvM_Prv_Crc_CheckRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun const* Crc_pcun);
extern void NvM_Prv_Crc_SetRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun const* Crc_pcun);
extern boolean NvM_Prv_Crc_IsRamBlockCrcRecalcRequired(NvM_BlockIdType* idBlock_puo);
extern boolean NvM_Prv_Crc_CheckCrc(NvM_BlockIdType idBlock_uo,
                                    uint8 const* intBuf_pcu8,
                                    uint16 const* nrUsedBytes_pcu16,
                                    NvM_Prv_Crc_tun const* Crc_pcun);
extern void NvM_Prv_Crc_AppendCrc(NvM_BlockIdType idBlock_uo,
                                  uint8* intBuf_pu8,
                                  uint16* nrUsedBytes_pu16,
                                  NvM_Prv_Crc_tun const* Crc_pcun);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
extern void NvM_Prv_Crc_ResetAllRamBlockCrcs(void);
extern void NvM_Prv_Crc_ResetRamBlockCrc(NvM_BlockIdType idBlock_uo);
extern void NvM_Prv_Crc_GetRamBlockCrc(NvM_BlockIdType idBlock_uo, NvM_Prv_Crc_tun* Crc_pun);
extern NvM_BlockIdType NvM_Prv_Crc_GetIdRamBlockCrcRecalc(void);
extern void NvM_Prv_Crc_InjectNvCrcError(uint8* crcByte_pu8);
#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"


/*
**********************************************************************************************************************
* Inline functions
**********************************************************************************************************************
*/
LOCAL_INLINE uint16 NvM_Prv_Crc_GetSizeCrc(NvM_Prv_Crc_Type_ten crcType_en);

/**
 * \brief
 * Function returns the number of bytes needed to store the CRC defined by the parameter crcType.
 *
 * \param crcType_en
 * CrcType
 *
 * \return
 * Number of bytes needed to store the CRC
 */
LOCAL_INLINE uint16 NvM_Prv_Crc_GetSizeCrc(NvM_Prv_Crc_Type_ten crcType_en)
{
    uint16 size_u16;

    switch (crcType_en)
    {
        case NvM_Prv_Crc_Type_8_Bit_e:
            size_u16 = 1u;
        break;

        case NvM_Prv_Crc_Type_16_Bit_e:
            size_u16 = 2u;
        break;

        case NvM_Prv_Crc_Type_32_Bit_e:
            size_u16 = 4u;
        break;

        default:
            size_u16 = 0u;
        break;
    }
    return size_u16;
}


/* NVM_PRV_CRC_H */
#endif

