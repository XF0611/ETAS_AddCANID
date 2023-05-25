

#ifndef CRC_PRV_IFX_HW_H
#define CRC_PRV_IFX_HW_H

#include "Mcu.h"
/*
**********************************************************************************************************************
* Typedefines
**********************************************************************************************************************
*/

typedef struct
{
    volatile uint32 IR;
    volatile uint32 RES;
    volatile uint32 CFG;
    volatile uint32 STS;
    volatile uint32 LENGTH;
    volatile uint32 CHECK;
    volatile uint32 CRC;
    volatile uint32 CTR;
}Crc_IfxRegType;

/*
**********************************************************************************************************************
* Defines/Macros
**********************************************************************************************************************
*/

#define CRC_HW_CFG8
#define CRC_HW_CFG8H2F              (0x0000000FU)
#define CRC_HW_CFG16                (0x000000F0U)
#define CRC_HW_CFG32                (0x00000F00U)
#define CRC_HW_POL_MASK             (0x000F0000U)
#define CLC                         (*(volatile uint32*)(0xF0000000UL))

#define CRC0  (*(volatile Crc_IfxRegType*)(0xF0000100UL)) /* [m=0-3] [0xF0000000 + 100 + m*20] */
#define CRC1  (*(volatile Crc_IfxRegType*)(0xF0000120UL))
#define CRC2  (*(volatile Crc_IfxRegType*)(0xF0000140UL))
#define CRC3  (*(volatile Crc_IfxRegType*)(0xF0000160UL))
#define CRC4  (*(volatile Crc_IfxRegType*)(0xF0000180UL))
#define CRC5  (*(volatile Crc_IfxRegType*)(0xF0000200UL))
#define CRC6  (*(volatile Crc_IfxRegType*)(0xF0000220UL))
#define CRC7  (*(volatile Crc_IfxRegType*)(0xF0000240UL))

/**
 **********************************************************************************************************************
 * \brief Calculates a CRC using the Data CRC Function of the IFX microcontroller
 *
 * \param    Config       configuration of the crc channel
 * \param    Data         holds a pointer to the start address of the data block to be calculated
 * \param    Length       length of data block to be calculated in bytes
 * \param    StartValue   start value when the algorithm starts
 *
 * \return   result of CRC calculation
 **********************************************************************************************************************
*/
CRC_INLINE uint32 Crc_Prv_CalcCRC_Hw(uint32 Config, const Crc_DataType *Data, uint32 Length, uint32 StartValue)
{
    static const uint32  LookUp[16]={0x00000000U,0x1DB71064U,0x3B6E20C8U,0x26D930ACU,0x76DC4190U,0x6B6B51F4U,0x4DB26158U,0x5005713CU,
                              0xEDB88320U,0xF00F9344U,0xD6D6A3E8U,0xCB61B38CU,0x9B64C2B0U,0x86D3D2D4U,0xA00AE278U,0xBDBDF21CU};
    const uint8 *Dptr = Data->b8Ptr;
    uint32 result;
    uint32 SoftBytes;
    uint32 HardBytes;
    uint32 Result = StartValue ^ CRC_XOR_VALUE32;

    SoftBytes = ((GetCoreID() > 2U) || (Length < 4U)) ? Length : Length%4U;
    HardBytes = Length - SoftBytes;

    while(SoftBytes--)
    { /* Software method used is: HalfByte method to reduce the overhead of bigger Lookup table. */
        Result = LookUp[(Result ^ (*Dptr     )) & 0x0F] ^ (Result >> 4);
        Result = LookUp[(Result ^ (*Dptr >> 4)) & 0x0F] ^ (Result >> 4);
        Dptr++;
    }

    Result ^= CRC_XOR_VALUE32; /* TODO: check the need for it. */

    if(HardBytes > 0U)
    {
        Crc_IfxRegType Backup;
        volatile Crc_IfxRegType* const IfxReg[] = {&CRC0, &CRC1, &CRC2, &CRC3, &CRC4, &CRC5, &CRC6, &CRC7};
        volatile Crc_IfxRegType* Context;
        uint32 i;

        Context = IfxReg[(GetCoreID() * 4U) + (Config & CRC_HW_POL_MASK) >> 16U];

        /* save register context for Reentrancy */
        Backup.LENGTH = Context->LENGTH;
        Backup.CHECK = Context->CHECK;
        Backup.CRC = Context->CRC;

        /* configure CRC generation process and execute it */
        if(*(&CLC) != 0U)
        {
            Mcu_Rb_ResetEndInit(); /* TODO: Check siginificance of it...! */
            *(&CLC) = 0U;
            Mcu_Rb_SetEndInit();
        }

        Context->CFG = Config;
        Context->CRC = Result;

        HardBytes/= 4U;

        for(i=0U; i<HardBytes; i++)
        {
            Context->IR = (uint32)Dptr[i*4];
        }

         result = Context->RES;

         Backup.LENGTH = Context->LENGTH; /* TODO: Check whether Context->IR is also to be restored. */
         Backup.CRC = Context->CRC;
         Backup.CHECK = Context->CHECK;
    }

    return result;
}

#endif /* CRC_PRV_IFX_HW_H */
