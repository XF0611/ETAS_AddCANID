
#ifndef NVM_PRV_INTERNALBUFFER_TYPES_H
#define NVM_PRV_INTERNALBUFFER_TYPES_H
/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef struct
{
    /// Pointer to the internal buffer used in current job if required by block configuration.
    uint8* Buffer_pu8;
    /// Pointer to current used size of the internal buffer in bytes.
    uint16* UsedSizeInBytes_pu16;
    /// Offset in bytes to the plain data (original user data) within the internal buffer.
    /// Encrypted data is always located at the beginning of the internal buffer.
    /// If encryption is disabled then offset to the plain data is 0, i.e. it is located at the beginning
    /// of the internal buffer.
    uint16 OffsetPlainData_u16;

} NvM_Prv_InternalBuffer_st;

/* NVM_PRV_INTERNALBUFFER_TYPES_H */
#endif

