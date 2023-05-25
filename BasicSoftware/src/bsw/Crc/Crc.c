

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Crc.h"

/*
 ***********************************************************************************************************************
 * Crc common routines
 ***********************************************************************************************************************
 */

#define CRC_START_SEC_CODE
#include "Crc_MemMap.h"

/**
 **********************************************************************************************************************
 * Crc_GetVersionInfo
 *
 * \This Api gives Version information about Crc.
 *
 * 
 * \param    VersionInfo           Pointer to give the Version information to user.
 *
 * \return                         void
 **********************************************************************************************************************
 */
void Crc_GetVersionInfo(Std_VersionInfoType * const VersionInfo)
{
    if(VersionInfo != NULL_PTR)
    {
    
         VersionInfo->vendorID         = CRC_VENDOR_ID;
         VersionInfo->moduleID         = CRC_MODULE_ID;
         VersionInfo->sw_major_version = CRC_SW_MAJOR_VERSION;
         VersionInfo->sw_minor_version = CRC_SW_MINOR_VERSION;
         VersionInfo->sw_patch_version = CRC_SW_PATCH_VERSION;
    }
}


/**
 **********************************************************************************************************************
 * Crc_CalculateCRC8
 *
 * \brief Calculates the remainder of a polynomial division: input is divided by the CRC_8_POLYNOMIAL polynomial
 *
 * Crc_CalculateCRC8 calculates the remainder of a polynomial division by the CRC polynomial.
 * 
 * CRC calculation starts with an initial value, Crc_StartValue8. 
 *
 * The data stream is transmitted via pointer *Crc_DataPtr and is of length Crc_Length.
 * The crc remainder is modified by a XOR operation using the value CRC_XOR_VALUE8.
 * 
 * \param    Crc_DataPtr           Pointer to start address of data block
 * \param    Crc_Length            Length of data block to be calculated in bytes
 * \param    Crc_StartValue8       Start value when the algorithm starts
 * 
 * \param    Crc_IsFirstCall       Set to TRUE when this is the first call in a sequence or an individual CRC calculation.
 *                                 Calculation starts from CRC_INITIAL_VALUE8, the parameter Crc_StartValue8 is ignored.
 *                                 Set to FALSE when this is a subsequent call in a call sequence, Crc_StartValue8 is 
 *                                 taken as the unmodified return value of the previous call to this function.
 * 
 * \return                         Remainder after polynomial division *const Crc_DataPtr / CRC8
 **********************************************************************************************************************
 */
uint8 Crc_CalculateCRC8(const uint8* Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, boolean Crc_IsFirstCall)
{
    uint32  index;
    uint8  result = Crc_StartValue8;
    uint8  crcTemp;
    if(Crc_DataPtr != NULL_PTR)
    {
        crcTemp = (Crc_IsFirstCall != FALSE) ? ((uint8)CRC_INITIAL_VALUE8) : (Crc_StartValue8^ CRC_XOR_VALUE8);
        
        for (index = 0U; index < Crc_Length; ++index)
        {
            /* Impact of temporary rest on next crc rest */
            crcTemp ^= ((uint8)Crc_DataPtr[index]) ;
            /* Next temporary crc rest */        
            crcTemp = CRC_8_Tbl[crcTemp];    
        }
        result = crcTemp ^ CRC_XOR_VALUE8;
    }
    
    return (result);
}


/**
 **********************************************************************************************************************
 * Crc_CalculateCRC8H2F
 *
 * \brief Calculates the remainder of a polynomial division: input is divided by the CRC_8H2F_POLYNOMIAL polynomial
 *
 * Crc_CalculateCRC8 calculates the remainder of a polynomial division by the CRC polynomial.
 * 
 * CRC calculation starts with an initial value, Crc_StartValue8. 
 *
 * The data stream is transmitted via pointer *Crc_DataPtr and is of length Crc_Length.
 * The crc remainder is modified by a XOR operation using the value CRC_XOR_VALUE8H2F.
 * 
 * \param    Crc_DataPtr           Pointer to start address of data block
 * \param    Crc_Length            Length of data block to be calculated in bytes
 * \param    Crc_StartValue8       Start value when the algorithm starts
 * 
 * \param    Crc_IsFirstCall       Set to TRUE when this is the first call in a sequence or an individual CRC calculation.
 *                                 Calculation starts from CRC_INITIAL_VALUE8, the parameter Crc_StartValue8 is ignored.
 *                                 Set to FALSE when this is a subsequent call in a call sequence, Crc_StartValue8 is 
 *                                 taken as the unmodified return value of the previous call to this function.
 * 
 * \return                         Remainder after polynomial division *const Crc_DataPtr / CRC8
 **********************************************************************************************************************
 */
uint8 Crc_CalculateCRC8H2F(const uint8* Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, boolean Crc_IsFirstCall)
{
    uint32  index;
    uint8  result = Crc_StartValue8;
    uint8  crcTemp;
    if(Crc_DataPtr != NULL_PTR)
    {
        crcTemp = (Crc_IsFirstCall != FALSE) ? ((uint8)CRC_INITIAL_VALUE8) : (Crc_StartValue8^ CRC_XOR_VALUE8H2F);
        
        for (index = 0U; index < Crc_Length; ++index)
        {
            /* Impact of temporary rest on next crc rest */
            crcTemp ^= ((uint8)Crc_DataPtr[index]) ;
            /* Next temporary crc rest */        
            crcTemp = CRC_8H2F_Tbl[crcTemp];    
        }
        result = crcTemp ^ CRC_XOR_VALUE8H2F;
    }
    
    return (result);
}



/**
 **********************************************************************************************************************
 * Crc_CalculateCRC16
 *
 * \brief Calculates the remainder of a polynomial division: input is divided by the CRC_16_POLYNOMIAL polynomial
 *
 * Crc_CalculateCRC16 calculates the remainder of a polynomial division by the CRC polynomial.
 * 
 * CRC calculation starts with an initial value, Crc_StartValue16. 
 *
 * The data stream is transmitted via pointer *Crc_DataPtr and is of length Crc_Length.
 * The crc remainder is modified by a XOR operation using the value CRC_XOR_VALUE16.
 * 
 * \param    Crc_DataPtr           Pointer to start address of data block
 * \param    Crc_Length            Length of data block to be calculated in bytes
 * \param    Crc_StartValue16       Start value when the algorithm starts
 * 
 * \param    Crc_IsFirstCall       Set to TRUE when this is the first call in a sequence or an individual CRC calculation.
 *                                 Calculation starts from CRC_INITIAL_VALUE16, the parameter Crc_StartValue16 is ignored.
 *                                 Set to FALSE when this is a subsequent call in a call sequence, Crc_StartValue16 is 
 *                                 taken as the unmodified return value of the previous call to this function.
 * 
 * \return                         Remainder after polynomial division *const Crc_DataPtr / CRC16
 **********************************************************************************************************************
 */
uint16 Crc_CalculateCRC16(const uint8* Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, boolean Crc_IsFirstCall)
{
    uint32  index;
    uint16  result = Crc_StartValue16;
    uint16  crcTemp;
    if(Crc_DataPtr != NULL_PTR)
    {
        crcTemp = (Crc_IsFirstCall != FALSE) ? ((uint16)CRC_INITIAL_VALUE16) : (Crc_StartValue16^ CRC_XOR_VALUE16);
        
        for (index = 0U; index < Crc_Length; ++index)
        {
            /* Impact of temporary rest on next crc rest */
            crcTemp ^= ((uint16)Crc_DataPtr[index]) << CRC_16_JUNKREST;
            /* Next temporary crc rest */        
            crcTemp =  (crcTemp << (CRC_JUNK)) ^ CRC_16_Tbl[(crcTemp >> CRC_16_JUNKREST) & CRC_TABLEMASK];    
        }
        result = crcTemp ^ CRC_XOR_VALUE16;
    }
    
    return (result);
}


/**
 **********************************************************************************************************************
 * Crc_CalculateCRC32
 *
 * \brief Calculates the remainder of a polynomial division: input is divided by the CRC_32_REV_POLYNOMIAL polynomial
 *
 * Crc_CalculateCRC32 calculates the remainder of a polynomial division by the CRC polynomial.
 * 
 * CRC calculation starts with an initial value, Crc_StartValue32. 
 *
 * The data stream is transmitted via pointer *Crc_DataPtr and is of length Crc_Length.
 * The crc remainder is modified by a XOR operation using the value CRC_XOR_VALUE32.
 * 
 * To reduce the computation time required to reflect the incoming data stream and the final result, the CRC polynomial 
 * is reflected instead. 
 * Using the reflected CRC polynomial with unreflected input and output results in the same CRC value
 * as using the truncated polynomial with reflected input and output data.
 * 
 * \param    Crc_DataPtr           Pointer to start address of data block
 * \param    Crc_Length            Length of data block to be calculated in bytes
 * \param    Crc_StartValue32       Start value when the algorithm starts
 * 
 * \param    Crc_IsFirstCall       Set to TRUE when this is the first call in a sequence or an individual CRC calculation.
 *                                 Calculation starts from CRC_INITIAL_VALUE32, the parameter Crc_StartValue32 is ignored.
 *                                 Set to FALSE when this is a subsequent call in a call sequence, Crc_StartValue32 is 
 *                                 taken as the unmodified return value of the previous call to this function.
 * 
 * \return                         Remainder after polynomial division *const Crc_DataPtr / CRC32
 **********************************************************************************************************************
 */
uint32 Crc_CalculateCRC32(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
    uint32  index;
    uint32  result = Crc_StartValue32;
    uint32  crcTemp;
    if(Crc_DataPtr != NULL_PTR)
    {
        crcTemp = (Crc_IsFirstCall != FALSE) ? ((uint32)CRC_INITIAL_VALUE32) : (Crc_StartValue32^ CRC_XOR_VALUE32);
        
        for (index = 0U; index < Crc_Length; ++index)
        {
            /* Impact of temporary rest on next crc rest */
            crcTemp ^= ((uint32)Crc_DataPtr[index]) ;
            /* Next temporary crc rest */        
            crcTemp =  (crcTemp >> (CRC_JUNK)) ^ CRC_32_REV_Tbl[(crcTemp ) & CRC_TABLEMASK];    
        }
        result = crcTemp ^ CRC_XOR_VALUE32;
    }
    
    return (result);
}


/**
 **********************************************************************************************************************
 * Crc_CalculateCRC32P4
 *
 * \brief Calculates the remainder of a polynomial division: input is divided by the CRC_32P4_REV_POLYNOMIAL polynomial
 *
 * Crc_CalculateCRC32 calculates the remainder of a polynomial division by the CRC polynomial.
 * 
 * CRC calculation starts with an initial value, Crc_StartValue32. 
 *
 * The data stream is transmitted via pointer *Crc_DataPtr and is of length Crc_Length.
 * The crc remainder is modified by a XOR operation using the value CRC_XOR_VALUE32P4.
 * 
 * To reduce the computation time required to reflect the incoming data stream and the final result, the CRC polynomial 
 * is reflected instead. 
 * Using the reflected CRC polynomial with unreflected input and output results in the same CRC value
 * as using the truncated polynomial with reflected input and output data.
 * 
 * \param    Crc_DataPtr           Pointer to start address of data block
 * \param    Crc_Length            Length of data block to be calculated in bytes
 * \param    Crc_StartValue32       Start value when the algorithm starts
 * 
 * \param    Crc_IsFirstCall       Set to TRUE when this is the first call in a sequence or an individual CRC calculation.
 *                                 Calculation starts from CRC_INITIAL_VALUE32, the parameter Crc_StartValue32 is ignored.
 *                                 Set to FALSE when this is a subsequent call in a call sequence, Crc_StartValue32 is 
 *                                 taken as the unmodified return value of the previous call to this function.
 * 
 * \return                         Remainder after polynomial division *const Crc_DataPtr / CRC32
 **********************************************************************************************************************
 */
uint32 Crc_CalculateCRC32P4(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
    uint32  index;
    uint32  result = Crc_StartValue32;
    uint32  crcTemp;
    if(Crc_DataPtr != NULL_PTR)
    {
        crcTemp = (Crc_IsFirstCall != FALSE) ? ((uint32)CRC_INITIAL_VALUE32) : (Crc_StartValue32^ CRC_XOR_VALUE32P4);
        
        for (index = 0U; index < Crc_Length; ++index)
        {
            /* Impact of temporary rest on next crc rest */
            crcTemp ^= ((uint32)Crc_DataPtr[index]) ;
            /* Next temporary crc rest */        
            crcTemp =  (crcTemp >> (CRC_JUNK)) ^ CRC_32P4_REV_Tbl[(crcTemp ) & CRC_TABLEMASK];    
        }
        result = crcTemp ^ CRC_XOR_VALUE32P4;
    }
    
    return (result);
}


#define CRC_STOP_SEC_CODE
#include "Crc_MemMap.h"

