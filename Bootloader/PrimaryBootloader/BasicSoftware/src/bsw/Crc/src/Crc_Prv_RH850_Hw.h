

#ifndef CRC_PRV_RH850_HW_H
#define CRC_PRV_RH850_HW_H

/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/
#include "Crc_Cfg.h"
/*
**********************************************************************************************************************
* Typedefines
**********************************************************************************************************************
*/

/* Registers of the "Data CRC Function" for Renesas RH850 microcontroller family: */
typedef struct {
    volatile uint32  CIN;    /* input register: DCRx_base */
    volatile uint32  COUT;   /* data register : DCRx_base + 0x04 */
             uint32  RES[6];   /* nothing here */
    volatile uint8   CTL;   /* control register: DCRx_base + 0x20 */
}Crc_RH850RegType;

/*
**********************************************************************************************************************
* Defines/Macros
**********************************************************************************************************************
*/

#define CRC_GENFUNC_CCITT16     (1U)  // 16 bit CCITT CRC polynomial is used

#if(defined(CRC_HW_RH850_R1X) && (CRC_HW == CRC_HW_RH850_R1X))
/* configuration of the CRC generation process */
// Bitmask for CRC variant    : DCRAnPOL = DCRAnCTL[0]
#define CRC_HW_CFG16            (0x05U)    // 8 bit input width, 16-bit CCITT CRC polynomial
#define CRC_HW_CFG1616          (0x11U)    // 16 bit input width, 16-bit CCITT CRC polynomial
#define CRC_HW_CFG32            (0x04U)    // 8 bit input width, 32-bit Ethernet CRC polynomial
#define CRC_HW_POL_MASK         (0x01U)
#else
/* configuration of the CRC generation process:
 * CRC input width: DCRBnISZ = DCRBnCTL[5:4] => 0x2 (8 bit)
 * CRC variant    : DCRBnPOL = DCRBnCTL[1:0]
 */

#define CRC_HW_CFG8             (0x22U)    // 8 bit input width, 8-bit SAE J1850 CRC polynomial
#define CRC_HW_CFG8H2F          (0x23U)    // 8 bit input width, 8-bit 0x2F CRC polynomial
#define CRC_HW_CFG16            (0x21U)    // 8 bit input width, 16-bit CCITT CRC polynomial
#define CRC_HW_CFG1616          (0x11U)    // 16 bit input width, 16-bit CCITT CRC polynomial
#define CRC_HW_CFG32            (0x20U)    // 8 bit input width, 32-bit Ethernet CRC polynomial
#define CRC_HW_POL_MASK         (0x03U)
#endif

#define CRC0   (*(volatile Crc_RH850RegType*)(0xFFF70000UL))
#define CRC1   (*(volatile Crc_RH850RegType*)(0xFFF71000UL))
#define CRC2   (*(volatile Crc_RH850RegType*)(0xFFF72000UL))
#define CRC3   (*(volatile Crc_RH850RegType*)(0xFFF73000UL))
#define CRC4   (*(volatile Crc_RH850RegType*)(0xFFD50000UL))
#define CRC5   (*(volatile Crc_RH850RegType*)(0xFFD51000UL))
#define CRC6   (*(volatile Crc_RH850RegType*)(0xFFD52000UL))
#define CRC7   (*(volatile Crc_RH850RegType*)(0xFFD53000UL))

/**
 **********************************************************************************************************************
 * \brief Calculates a CRC given by the Config using the Data CRC Function of the Renesas microcontroller
 *
 * \param    Config       configuration of the crc channel
 * \param    Data         holds a pointer to the start address of the data block to be calculated
 * \param    Length       length of data block to be calculated in bytes
 * \param    StartValue   start value when the algorithm starts
 *
 * \return   result of CRC calculation
 **********************************************************************************************************************
*/
CRC_INLINE uint32 Crc_Prv_CalcCRC_Hw(uint8 Config, const Crc_DataType *Data, uint32 Length, uint32 StartValue)
{
    Crc_RH850RegType Backup;
    volatile Crc_RH850RegType *Context;
    uint32 i;
    uint32 result;
    #if(defined(CRC_HW_RH850_R1X) && (CRC_HW == CRC_HW_RH850_R1X))
    volatile Crc_RH850RegType* const R1x[] = {&CRC0, &CRC1, &CRC2, &CRC3};
    Context = R1x[Config & CRC_HW_POL_MASK];
    #elif (defined(CRC_HW_RH850_P1XD3) && (CRC_HW == CRC_HW_RH850_P1XD3))
    volatile Crc_RH850RegType* const P1xD3[] = {&CRC4, &CRC0, &CRC5, &CRC1};
    Context = P1xD3[Config & CRC_HW_POL_MASK];
    #else
    volatile Crc_RH850RegType* const P1xD4_D5_D5EDv2[] = {&CRC4, &CRC0, &CRC5, &CRC1, &CRC6, &CRC2, &CRC7, &CRC3};
    Context = P1xD4_D5_D5EDv2[(((GetCoreID()) * 4U) + (Config & (CRC_HW_POL_MASK)))];
    #endif

    /* save register context */
    Backup.CTL = Context->CTL;
    Backup.COUT = Context->COUT;
    //If polynomial of interrupted calculation was not CCITT16 - regardless of DCRBnISZ setting.
    if ((CRC_GENFUNC_CCITT16) != (Backup.CTL & CRC_HW_POL_MASK))
    {
       // XOR operation done by hardware needs to be reversed - but NOT for any CCITT16 computation
        Backup.COUT ^= CRC_XOR_VALUE32;
    }

    /* configure CRC generation process and execute it */
    Context->CTL = Config;
    Context->COUT = StartValue;

    if(Config != CRC_HW_CFG1616)
    {
        const uint8 *Dptr = Data->b8Ptr;
        for(i = 0; i < Length; i++)
        {
            Context->CIN = Dptr[i];
        }
    }
    else
    {
        const uint16 *Dptr = Data->b16Ptr;
        for (i = 0; i < Length; i++)
        {
            Context->CIN = Dptr[i];
        }
    }

    result = Context->COUT;

    /* restore register context */
    Context->CTL = Backup.CTL;
    Context->COUT = Backup.COUT;

    return result;
}

#endif/* CRC_PRV_RH850_HW_H */
