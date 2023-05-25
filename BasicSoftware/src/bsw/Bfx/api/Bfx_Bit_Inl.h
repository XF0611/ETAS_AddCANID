


#ifndef BFX_BIT_INL_H
#define BFX_BIT_INL_H


/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * Bfx_Prv_ClrBit_u16u8_Inl
 * Bfx_Prv_ClrBit_u32u8_Inl
 * Bfx_Prv_ClrBit_u8u8_Inl
 * Bfx_Prv_ClrBit_u64u8_Inl
 * Bfx_Prv_ClrBitMask_u16u16_Inl
 * Bfx_Prv_ClrBitMask_u32u32_Inl
 * Bfx_Prv_ClrBitMask_u8u8_Inl
 * Bfx_Prv_ClrBitMask_u64u64_Inl
 * Bfx_Prv_CopyBit_u16u8u16u8_Inl
 * Bfx_Prv_CopyBit_u32u8u32u8_Inl
 * Bfx_Prv_CopyBit_u8u8u8u8_Inl
 * Bfx_Prv_CopyBit_u64u8u64u8_Inl
 * Bfx_Prv_GetBit_u16u8_u8_Inl
 * Bfx_Prv_GetBit_u32u8_u8_Inl
 * Bfx_Prv_GetBit_u8u8_u8_Inl
 * Bfx_Prv_GetBit_u64u8_u8_Inl
 * Bfx_Prv_GetBits_u16u8u8_u16_Inl
 * Bfx_Prv_GetBits_u32u8u8_u32_Inl
 * Bfx_Prv_GetBits_u8u8u8_u8_Inl
 * Bfx_Prv_GetBits_u64u8u8_u64_Inl
 * Bfx_Prv_PutBit_u16u8u8_Inl
 * Bfx_Prv_PutBit_u32u8u8_Inl
 * Bfx_Prv_PutBit_u8u8u8_Inl
 * Bfx_Prv_PutBit_u64u8u8_Inl
 * Bfx_Prv_PutBits_u16u8u8u16_Inl
 * Bfx_Prv_PutBits_u32u8u8u32_Inl
 * Bfx_Prv_PutBits_u8u8u8u8_Inl
 * Bfx_Prv_PutBits_u64u8u8u64_Inl
 * Bfx_Prv_PutBitsMask_u16u16u16_Inl
 * Bfx_Prv_PutBitsMask_u32u32u32_Inl
 * Bfx_Prv_PutBitsMask_u8u8u8_Inl
 * Bfx_Prv_PutBitsMask_u64u64u64_Inl
 * Bfx_Prv_RotBitLt_u16u8_Inl
 * Bfx_Prv_RotBitLt_u32u8_Inl
 * Bfx_Prv_RotBitLt_u8u8_Inl
 * Bfx_Prv_RotBitLt_u64u8_Inl
 * Bfx_Prv_RotBitRt_u16u8_Inl
 * Bfx_Prv_RotBitRt_u32u8_Inl
 * Bfx_Prv_RotBitRt_u8u8_Inl
 * Bfx_Prv_RotBitRt_u64u8_Inl
 * Bfx_Prv_SetBit_u16u8_Inl
 * Bfx_Prv_SetBit_u32u8_Inl
 * Bfx_Prv_SetBit_u8u8_Inl
 * Bfx_Prv_SetBit_u64u8_Inl
 * Bfx_Prv_SetBitMask_u16u16_Inl
 * Bfx_Prv_SetBitMask_u32u32_Inl
 * Bfx_Prv_SetBitMask_u8u8_Inl
 * Bfx_Prv_SetBitMask_u64u64_Inl
 * Bfx_Prv_SetBits_u16u8u8u8_Inl
 * Bfx_Prv_SetBits_u32u8u8u8_Inl
 * Bfx_Prv_SetBits_u8u8u8u8_Inl
 * Bfx_Prv_SetBits_u64u8u8u8_Inl
 * Bfx_Prv_ShiftBitLt_u16u8_Inl
 * Bfx_Prv_ShiftBitLt_u32u8_Inl
 * Bfx_Prv_ShiftBitLt_u8u8_Inl
 * Bfx_Prv_ShiftBitLt_u64u8_Inl
 * Bfx_Prv_ShiftBitRt_u16u8_Inl
 * Bfx_Prv_ShiftBitRt_u32u8_Inl
 * Bfx_Prv_ShiftBitRt_u8u8_Inl
 * Bfx_Prv_ShiftBitRt_u64u8_Inl
 * Bfx_Prv_ToggleBitMask_u16u16_Inl
 * Bfx_Prv_ToggleBitMask_u32u32_Inl
 * Bfx_Prv_ToggleBitMask_u8u8_Inl
 * Bfx_Prv_ToggleBitMask_u64u64_Inl
 * Bfx_Prv_ToggleBits_u16_Inl
 * Bfx_Prv_ToggleBits_u32_Inl
 * Bfx_Prv_ToggleBits_u8_Inl
 * Bfx_Prv_ToggleBits_u64_Inl
 * Bfx_Prv_TstBitLnMask_u16u16_u8_Inl
 * Bfx_Prv_TstBitLnMask_u32u32_u8_Inl
 * Bfx_Prv_TstBitLnMask_u8u8_u8_Inl
 * Bfx_Prv_TstBitLnMask_u64u64_u8_Inl
 * Bfx_Prv_TstBitMask_u16u16_u8_Inl
 * Bfx_Prv_TstBitMask_u32u32_u8_Inl
 * Bfx_Prv_TstBitMask_u8u8_u8_Inl
 * Bfx_Prv_TstBitMask_u64u64_u8_Inl
 * Bfx_Prv_TstParityEven_u16_u8_Inl
 * Bfx_Prv_TstParityEven_u32_u8_Inl
 * Bfx_Prv_TstParityEven_u8_u8_Inl
 * Bfx_Prv_TstParityEven_u64_u8_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE void Bfx_Prv_ClrBit_u16u8_Inl(uint16* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_ClrBit_u32u8_Inl(uint32* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_ClrBit_u8u8_Inl(uint8* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_ClrBit_u64u8_Inl(Bfx_uint64* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u16u16_Inl(uint16* Data, uint16 Mask);
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u32u32_Inl(uint32* Data, uint32 Mask);
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u8u8_Inl(uint8* Data, uint8 Mask);
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask);
LOCAL_INLINE void Bfx_Prv_CopyBit_u16u8u16u8_Inl(uint16* DestinationData, uint8 DestinationPosition, uint16 SourceData,
                                                                                                 uint8 SourcePosition);
LOCAL_INLINE void Bfx_Prv_CopyBit_u32u8u32u8_Inl(uint32* DestinationData, uint8 DestinationPosition, uint32 SourceData,
                                                                                                 uint8 SourcePosition);
LOCAL_INLINE void Bfx_Prv_CopyBit_u8u8u8u8_Inl(uint8* DestinationData, uint8 DestinationPosition, uint8 SourceData,
                                                                                                 uint8 SourcePosition);
LOCAL_INLINE void Bfx_Prv_CopyBit_u64u8u64u8_Inl(Bfx_uint64* DestinationData, uint8 DestinationPosition,
                                                                          Bfx_uint64 SourceData, uint8 SourcePosition);
LOCAL_INLINE boolean Bfx_Prv_GetBit_u16u8_u8_Inl(uint16 Data, uint8 BitPn);
LOCAL_INLINE boolean Bfx_Prv_GetBit_u32u8_u8_Inl(uint32 Data, uint8 BitPn);
LOCAL_INLINE boolean Bfx_Prv_GetBit_u8u8_u8_Inl(uint8 Data, uint8 BitPn);
LOCAL_INLINE boolean Bfx_Prv_GetBit_u64u8_u8_Inl(Bfx_uint64 Data, uint8 BitPn);
LOCAL_INLINE uint16 Bfx_Prv_GetBits_u16u8u8_u16_Inl(uint16 Data, uint8 BitStartPn, uint8 BitLn);
LOCAL_INLINE uint32 Bfx_Prv_GetBits_u32u8u8_u32_Inl(uint32 Data, uint8 BitStartPn, uint8 BitLn);
LOCAL_INLINE uint8 Bfx_Prv_GetBits_u8u8u8_u8_Inl(uint8 Data, uint8 BitStartPn, uint8 BitLn);
LOCAL_INLINE Bfx_uint64 Bfx_Prv_GetBits_u64u8u8_u64_Inl(Bfx_uint64 Data, uint8 BitStartPn, uint8 BitLn);
LOCAL_INLINE void Bfx_Prv_PutBit_u16u8u8_Inl(uint16* Data, uint8 BitPn, boolean Status);
LOCAL_INLINE void Bfx_Prv_PutBit_u32u8u8_Inl(uint32* Data, uint8 BitPn, boolean Status);
LOCAL_INLINE void Bfx_Prv_PutBit_u8u8u8_Inl(uint8* Data, uint8 BitPn, boolean Status);
LOCAL_INLINE void Bfx_Prv_PutBit_u64u8u8_Inl(Bfx_uint64* Data, uint8 BitPn, boolean Status);
LOCAL_INLINE void Bfx_Prv_PutBits_u16u8u8u16_Inl(uint16* Data, uint8 BitStartPn, uint8 BitLn, uint16 Pattern);
LOCAL_INLINE void Bfx_Prv_PutBits_u32u8u8u32_Inl(uint32* Data, uint8 BitStartPn, uint8 BitLn, uint32 Pattern);
LOCAL_INLINE void Bfx_Prv_PutBits_u8u8u8u8_Inl(uint8* Data, uint8 BitStartPn, uint8 BitLn, uint8 Pattern);
LOCAL_INLINE void Bfx_Prv_PutBits_u64u8u8u64_Inl(Bfx_uint64* Data, uint8 BitStartPn, uint8 BitLn, Bfx_uint64 Pattern);
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u16u16u16_Inl(uint16* Data, uint16 Pattern, uint16 Mask);
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u32u32u32_Inl(uint32* Data, uint32 Pattern, uint32 Mask);
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u8u8u8_Inl(uint8* Data, uint8 Pattern, uint8 Mask);
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u64u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Pattern, Bfx_uint64 Mask);
LOCAL_INLINE void Bfx_Prv_RotBitLt_u16u8_Inl(uint16* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitLt_u32u8_Inl(uint32* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitLt_u8u8_Inl(uint8* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitLt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitRt_u16u8_Inl(uint16* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitRt_u32u8_Inl(uint32* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitRt_u8u8_Inl(uint8* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_RotBitRt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_SetBit_u16u8_Inl(uint16* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_SetBit_u32u8_Inl(uint32* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_SetBit_u8u8_Inl(uint8* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_SetBit_u64u8_Inl(Bfx_uint64* Data, uint8 BitPn);
LOCAL_INLINE void Bfx_Prv_SetBitMask_u16u16_Inl(uint16* Data, uint16 Mask);
LOCAL_INLINE void Bfx_Prv_SetBitMask_u32u32_Inl(uint32* Data, uint32 Mask);
LOCAL_INLINE void Bfx_Prv_SetBitMask_u8u8_Inl(uint8* Data, uint8 Mask);
LOCAL_INLINE void Bfx_Prv_SetBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask);
LOCAL_INLINE void Bfx_Prv_SetBits_u16u8u8u8_Inl(uint16* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status);
LOCAL_INLINE void Bfx_Prv_SetBits_u32u8u8u8_Inl(uint32* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status);
LOCAL_INLINE void Bfx_Prv_SetBits_u8u8u8u8_Inl(uint8* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status);
LOCAL_INLINE void Bfx_Prv_SetBits_u64u8u8u8_Inl(Bfx_uint64* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status);
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u16u8_Inl(uint16* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u32u8_Inl(uint32* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u8u8_Inl(uint8* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u16u8_Inl(uint16* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u32u8_Inl(uint32* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u8u8_Inl(uint8* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt);
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u16u16_Inl(uint16* Data, uint16 Mask);
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u32u32_Inl(uint32* Data, uint32 Mask);
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u8u8_Inl(uint8* Data, uint8 Mask);
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask);
LOCAL_INLINE void Bfx_Prv_ToggleBits_u16_Inl(uint16* Data);
LOCAL_INLINE void Bfx_Prv_ToggleBits_u32_Inl(uint32* Data);
LOCAL_INLINE void Bfx_Prv_ToggleBits_u8_Inl(uint8* Data);
LOCAL_INLINE void Bfx_Prv_ToggleBits_u64_Inl(Bfx_uint64* Data);
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u16u16_u8_Inl(uint16 Data, uint16 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u32u32_u8_Inl(uint32 Data, uint32 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u8u8_u8_Inl(uint8 Data, uint8 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u64u64_u8_Inl(Bfx_uint64 Data, Bfx_uint64 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u16u16_u8_Inl(uint16 Data, uint16 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u32u32_u8_Inl(uint32 Data, uint32 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u8u8_u8_Inl(uint8 Data, uint8 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u64u64_u8_Inl(Bfx_uint64 Data, Bfx_uint64 Mask);
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u16_u8_Inl(uint16 Data);
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u32_u8_Inl(uint32 Data);
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u8_u8_Inl(uint8 Data);
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u64_u8_Inl(Bfx_uint64 Data);

/*
 **********************************************************************************************************************
 * Implementations
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Bfx_ClrBit_u16u8
 *
 * \brief Clears a bit in the variable Data to "0". The position of the cleared bit is described by the variable
 * BitPn. A priori, range [0,15] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   uint16*   Data    pointer to the 16-bit variable, which should be modified.
 * \param   uint8     BitPn   position which bit shall be cleared.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBIT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBit_u16u8_Inl(uint16* Data, uint8 BitPn)
{
/* Additional typecast is required to remove the MISRA 12 warning */
    *Data &= ((uint16)(~(uint16)(1uL << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBit_u32u8
 *
 * \brief Clears a bit at the variable Data to "0". The position of the cleared bit is described by the variable
 * BitPn. A priori, range [0,31] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   uint32*   Data     pointer to the 32-bit variable, which should be modified.
 * \param   uint8     BitPn    position of the bit, to be cleared.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBIT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBit_u32u8_Inl(uint32* Data, uint8 BitPn)
{
/* Additional typecast is required to remove the MISRA 12 warning */
    *Data &= ((uint32)(~(1uL << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBit_u8u8
 *
 * \brief Clears a bit in the variable Data to "0". The position of the cleared bit is described by the variable
 * BitPn. A priori, range [0,7] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   uint8*   Data     pointer to the 8-bit variable, which should be modified.
 * \param   uint8    BitPn    position which bit shall be cleared.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBIT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBit_u8u8_Inl(uint8* Data, uint8 BitPn)
{
/* Additional typecast is required to remove the MISRA 12 warning */
    *Data &= ((uint8)(~(uint8)(1u << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBit_u64u8
 *
 * \brief Clears a bit at the variable Data to "0". The position of the cleared bit is described by the variable
 * BitPn. A priori, range [0,63] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   Bfx_uint64*   Data     pointer to the 64-bit variable, which should be modified.
 * \param   uint8         BitPn    position of the bit, to be cleared.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBIT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBit_u64u8_Inl(Bfx_uint64* Data, uint8 BitPn)
{
/* Additional typecast is required to remove the MISRA 12 warning */
    *Data &= ((Bfx_uint64)(~(1uLL << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBitMask_u16u16
 *
 * \brief Clears bits in the 16-bit variable Data. Every bit, set in Mask, will be cleared.
 *
 * \param   uint16*     Data   pointer to the variable where the bits should be cleared
 * \param   uint16      Mask   bitmask that defines which bits are cleared
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBITMASK_U16U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u16u16_Inl(uint16* Data, uint16 Mask)
{
    *Data &= ((uint16)(~Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBitMask_u32u32
 *
 * \brief Clears bits in the 32-bit variable Data. Every bit, set in Mask, will be cleared.
 *
 * \param   uint32*   Data   pointer to the variable, where the bits should be cleared
 * \param   uint32    Mask   bitmask that defines which bits are cleared
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBITMASK_U32U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u32u32_Inl(uint32* Data, uint32 Mask)
{
    *Data &= ((uint32)(~Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBitMask_u8u8
 *
 * \brief Clears bits in the 8-bit variable Data. Every bit, set in Mask, will be cleared.
 *
 * \param   uint8* Data   Pointer to the variable where the bits should be cleared
 * \param   uint8  Mask   Bitmask that defines which bits are cleared
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBITMASK_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u8u8_Inl(uint8* Data, uint8 Mask)
{
    *Data &= ((uint8)(~Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ClrBitMask_u64u64
 *
 * \brief Clears bits in the 64-bit variable Data. Every bit, set in Mask, will be cleared.
 *
 * \param   Bfx_uint64*   Data   pointer to the variable, where the bits should be cleared
 * \param   Bfx_uint64    Mask   bitmask that defines which bits are cleared
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_CLRBITMASK_U64U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ClrBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask)
{
    *Data &= ((Bfx_uint64)(~Mask));
}
#endif


/**
**********************************************************************************************************************
 * Bfx_CopyBit_u16u8u16u8
 *
 * \brief Copys a bit form a 16-bit variable to an other
 *
 * Copys a bit from Src[SourceData] to Dest[DestinationData]
 *
 * \param   uint16*    DestinationData    Pointer to bitfield where bit should be copied to.
 * \param   uint8      DestinationPosition      Position within the destionation.
 * \param   uint16     SourceData     Value of the bitfield to copy from.
 * \param   uint8      SourcePosition       Position within the Source.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_COPYBIT_U16U8U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_CopyBit_u16u8u16u8_Inl(uint16* DestinationData, uint8 DestinationPosition, uint16 SourceData, uint8 SourcePosition)
{
    Bfx_Prv_PutBit_u16u8u8_Inl(DestinationData, DestinationPosition, Bfx_Prv_GetBit_u16u8_u8_Inl(SourceData,
                                                                                                      SourcePosition));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_CopyBit_u32u8u32u8
 *
 * \brief Copys a bit form a 32-bit variable to an other
 *
 * Copys a bit from Src[SourceData] to Dest[DestinationData]
 *
 * \param   uint32* DestinationData    Pointer to bitfield where bit should be copied to.
 * \param   uint8   DestinationPosition      Position within the destionation.
 * \param   uint32  SourceData     Value of the bitfield to copy from.
 * \param   uint8   SourcePosition       Position within the Source.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_COPYBIT_U32U8U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_CopyBit_u32u8u32u8_Inl(uint32* DestinationData, uint8 DestinationPosition, uint32 SourceData, uint8 SourcePosition)
{
    Bfx_Prv_PutBit_u32u8u8_Inl(DestinationData, DestinationPosition, Bfx_Prv_GetBit_u32u8_u8_Inl(SourceData,
                                                                                                      SourcePosition));
}
#endif


/**
**********************************************************************************************************************
 * Bfx_CopyBit_u8u8u8u8
 *
 * \brief Copys a bit form a 8-bit variable to an other
 *
 * Copys a bit from Src[SourceData] to Dest[DestinationData]
 *
 * \param   uint8*      DestinationData     Pointer to bitfield where bit should be copied to.
 * \param   uint8       DestinationPosition       Position within the destionation.
 * \param   uint8       SourceData      Value of the bitfield to copy from.
 * \param   uint8       SourcePosition        Position within the Source.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_COPYBIT_U8U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_CopyBit_u8u8u8u8_Inl(uint8* DestinationData, uint8 DestinationPosition, uint8 SourceData, uint8 SourcePosition)
{
    Bfx_Prv_PutBit_u8u8u8_Inl(DestinationData, DestinationPosition, Bfx_Prv_GetBit_u8u8_u8_Inl(SourceData,
                                                                                                      SourcePosition));
}
#endif

/**
 **********************************************************************************************************************
 * Bfx_CopyBit_u64u8u64u8
 *
 * \brief Copys a bit form a 64-bit variable to an other
 *
 * Copys a bit from Src[SourceData] to Dest[DestinationData]
 *
 * \param   Bfx_uint64*     DestinationData    Pointer to bitfield where bit should be copied to.
 * \param   uint8           DestinationPosition      Position within the destionation.
 * \param   Bfx_uint64      SourceData     Value of the bitfield to copy from.
 * \param   uint8           SourcePosition       Position within the Source.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_COPYBIT_U64U8U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_CopyBit_u64u8u64u8_Inl(Bfx_uint64* DestinationData, uint8 DestinationPosition,
                                                                           Bfx_uint64 SourceData, uint8 SourcePosition)
{
    Bfx_Prv_PutBit_u64u8u8_Inl(DestinationData, DestinationPosition, Bfx_Prv_GetBit_u64u8_u8_Inl(SourceData,
                                                                                                      SourcePosition));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBit_u16u8_u8
 *
 * \brief Gets a bit at the variable Data at a described position BitPn.
 * Returns TRUE in case, the bit at position BitPn in variable Data is set, otherwise returns FALSE.
 *
 * \param   uint16  Data      16-bit variable, where should be read from.
 * \param   uint8   BitPn     Position which bit shall be read.
 * \return  boolean           Returns the bit value from Data at position BitPn
 * \retval          TRUE      If the bit at BitPn is not zero
 * \retval          FALSE     If the bit at BitPn is zero
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBIT_U16U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_GetBit_u16u8_u8_Inl(uint16 Data, uint8 BitPn)
{
    return ((((Data) & ((uint16) (1uL << BitPn))) != 0u));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBit_u32u8_u8
 *
 * \brief Gets a bit at the variable Data at a described position BitPn.
 * Returns TRUE in case, the bit at position BitPn in variable Data is set, otherwise returns FALSE.
 *
 * \param   uint32   Data      32-bit Base variable, where should be read from
 * \param   uint8    BitPn     position which bit shall be read
 * \return  boolean            returns the bit value from address Data at position BitPn
 * \retval           TRUE      if the bit at BitPn is one (set)
 * \retval           FALSE     if the bit at BitPn is zero
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBIT_U32U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_GetBit_u32u8_u8_Inl(uint32 Data, uint8 BitPn)
{
    return (((Data) & ((uint32)(1uL << BitPn))) != 0uL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBit_u8u8_u8
 *
 * \brief Gets a bit at the variable Data at a described position BitPn.
 * Returns TRUE in case, the bit at position BitPn in variable Data is set, otherwise returns FALSE.
 *
 * \param   uint8   Data     8-bit Base variable, where should be read from.
 * \param   uint8   BitPn    Position which bit shall be read
 * \return  boolean          Returns the bit value from Data at position BitPn
 * \retval          TRUE     If the bit at BitPn is not zero
 * \retval          FALSE    If the bit at BitPn is zero
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBIT_U8U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_GetBit_u8u8_u8_Inl(uint8 Data, uint8 BitPn)
{
    return (((Data) & ((uint8)(1uL << BitPn))) != 0u);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBit_u64u8_u8
 *
 * \brief Gets a bit at the variable Data at a described position BitPn.
 * Returns TRUE in case, the bit at position BitPn in variable Data is set, otherwise returns FALSE.
 *
 * \param   Bfx_uint64   Data      64-bit Base variable, where should be read from
 * \param   uint8        BitPn     position which bit shall be read
 * \return  boolean            returns the bit value from address Data at position BitPn
 * \retval           TRUE      if the bit at BitPn is one (set)
 * \retval           FALSE     if the bit at BitPn is zero
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBIT_U64U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_GetBit_u64u8_u8_Inl(Bfx_uint64 Data, uint8 BitPn)
{
    return (((Data) & ((Bfx_uint64)(1uLL << BitPn))) != 0uLL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBits_u16u8u8_u16
 *
 * \brief Returns a sequence of the variable Data in form of a bitfield.
 * The bitfield starts at the position BitStartPn and has the bit-length BitLn.
 * The sum of (BitLn + BitStartPn) has to be smaller than 16. Note, these sum is not monitored.
 *
 * \param   uint16  Data         16-bit variable, where the bit sequence should be read from.
 * \param   uint8   BitStartPn   Position of the first bit to be read out.
 * \param   uint8   BitLn        Length of the section, which should be read.
 * \return  uint16               Selected bitfield sequence as uint16
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBITS_U16U8U8_U16_INL_REMAPPED
LOCAL_INLINE uint16 Bfx_Prv_GetBits_u16u8u8_u16_Inl(uint16 Data, uint8 BitStartPn, uint8 BitLn)
{
    return ((uint16)((Data >> BitStartPn) & (BFX_MAXUINT16_U >> (16uL - BitLn))));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBits_u32u8u8_u32
 *
 * \brief Returns a sequence of the variable Data in form of a bitfield.
 * The bitfield starts at the position BitStartPn and has the bit-length BitLn.
 * The sum of (BitLn + BitStartPn) has to be smaller than 32. Note, these sum is not monitored.
 *
 * \param   uint32  Data         32-bit variable, where the bit sequence should be read from.
 * \param   uint8   BitStartPn   Position of the first bit to be read out.
 * \param   uint8   BitLn        Length of the section, which should be read.
 * \return  uint32               Selected bitfield sequence as uint32.
 **********************************************************************************************************************
*/
#ifndef BFX_PRV_GETBITS_U32U8U8_U32_INL_REMAPPED
LOCAL_INLINE uint32 Bfx_Prv_GetBits_u32u8u8_u32_Inl(uint32 Data, uint8 BitStartPn, uint8 BitLn)
{
    return ((Data >> BitStartPn) & (BFX_MAXUINT32_U >> (32uL - BitLn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_GetBits_u8u8u8_u8
 *
 * \brief Returns a sequence of the variable Data in form of a bitfield.
 * The bitfield starts at the position BitStartPn and has the bit-length BitLn.
 * The sum of (BitLn + BitStartPn) has to be smaller than 8. Note, these sum is not monitored.
 *
 * \param   uint32  Data         8-bit variable, where the bit sequence shouLd be read from.
 * \param   uint8   BitStartPn   Position of the first bit to be read out.
 * \param   uint8   BitLn        Length of the section, which should be read.
 * \return  uint8                Selected bitfield sequence as uint8.
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_GETBITS_U8U8U8_U8_INL_REMAPPED
LOCAL_INLINE uint8 Bfx_Prv_GetBits_u8u8u8_u8_Inl(uint8 Data, uint8 BitStartPn, uint8 BitLn)
{
    return ((uint8)((Data >> BitStartPn) & (BFX_MAXUINT8_U >> (8uL - BitLn))));
}
#endif

/**
 **********************************************************************************************************************
 * Bfx_GetBits_u64u8u8_u64
 *
 * \brief Returns a sequence of the variable Data in form of a bitfield.
 * The bitfield starts at the position BitStartPn and has the bit-length BitLn.
 * The sum of (BitLn + BitStartPn) has to be smaller than 64. Note, these sum is not monitored.
 *
 * \param   Bfx_uint64  Data         32-bit variable, where the bit sequence should be read from.
 * \param   uint8       BitStartPn   Position of the first bit to be read out.
 * \param   uint8       BitLn        Length of the section, which should be read.
 * \return  Bfx_uint64               Selected bitfield sequence as Bfx_uint64.
 **********************************************************************************************************************
*/
#ifndef BFX_PRV_GETBITS_U8U8U8_U8_INL_REMAPPED
LOCAL_INLINE Bfx_uint64 Bfx_Prv_GetBits_u64u8u8_u64_Inl(Bfx_uint64 Data, uint8 BitStartPn, uint8 BitLn)
{
    return ((Data >> BitStartPn) & (BFX_MAXUINT64_U >> (64uLL - BitLn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBit_u16u8u8
 *
 * \brief Sets or clears a bit at a given position Pos_u32 in the 16-bit variable Data.
 * A priori, range [0,1] is valid for Status, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,15] is valid for BitPn.
 * Note, neither the given position BitPn nor the given Status Value not monitored.
 *
 * \param   uint16*  Data   pointer to a 16-bit variable, where a bit schould be modified
 * \param   uint8    BitPn  position of the bit, to be set
 * \param   boolean  Status  defines the new bit, 0 clears the bit and 1 sets the bit
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBIT_U16U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBit_u16u8u8_Inl(uint16* Data, uint8 BitPn, boolean Status)
{
    uint8 tmp_u8;
    tmp_u8 = (Status)? 1u : 0u;
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = ((*Data & ((uint16)~(1uL << BitPn))) | ((uint16)((uint16)tmp_u8 << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBit_u32u8u8
 *
 * \brief Sets or clears a bit at a given position BitPn in the 32-bit variable Data.
 * A priori, range [0,1] is valid for Status, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,31] is valid for BitPn.
 * Note, neither the given position BitPn nor the given Status Value not monitored.
 *
 * \param   uint32*     Data        pointer to a 32-bit variable, where a bit schould be modified
 * \param   uint8       BitPn       position of the bit, to be set
 * \param   boolean     Status       defines the new bit, 0 clears the bit and 1 sets the bit
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBIT_U32U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBit_u32u8u8_Inl(uint32* Data, uint8 BitPn, boolean Status)
{
    uint8 tmp_u8;
    tmp_u8 = (Status)? 1u : 0u;
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = ((*Data & ((uint32)~(1uL << BitPn))) | ((uint32)((uint32)tmp_u8 << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBit_u8u8u8
 *
 * \brief Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * A priori, range [0,1] is valid for Status, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,7] is valid for BitPn.
 * Note, neither the given position BitPn nor the given Status Value not monitored.
 *
 * \param  uint8*   Data    pointer to a 8-bit variable, where a bit schould be modified
 * \param  uint8    BitPn   position of the bit, to be set
 * \param  boolean  Status   defines the new bit, 0 clears the bit and 1 sets the bit
 * \return void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBIT_U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBit_u8u8u8_Inl(uint8* Data, uint8 BitPn, boolean Status)
{
    uint8 tmp_u8;
    tmp_u8 = (Status)? 1u : 0u;
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = ((*Data & ((uint8)~(1uL <<BitPn))) | ((uint8)(tmp_u8 << BitPn)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBit_u64u8u8
 *
 * \brief Sets or clears a bit at a given position BitPn in the 64-bit variable Data.
 * A priori, range [0,1] is valid for Status, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,63] is valid for BitPn.
 * Note, neither the given position BitPn nor the given Status Value not monitored.
 *
 * \param   Bfx_uint64*     Data        pointer to a 64-bit variable, where a bit schould be modified
 * \param   uint8           BitPn       position of the bit, to be set
 * \param   boolean         Status       defines the new bit, 0 clears the bit and 1 sets the bit
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBIT_U64U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBit_u64u8u8_Inl(Bfx_uint64* Data, uint8 BitPn, boolean Status)
{
    uint8 tmp_u8;
    tmp_u8 = (Status)? 1u : 0u;
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = ((*Data & ((Bfx_uint64)~(1uLL << BitPn))) | ((Bfx_uint64)((Bfx_uint64)tmp_u8 << BitPn)));
}
#endif



/**
 **********************************************************************************************************************
 * Bfx_Prv_PutBits_u16u8u8u16_Inl
 *
 * \brief Sets or clears a bit at a given position Pos_u32 in the 16-bit variable Data.
 * A priori, range [0,1] is valid for Status, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,15] is valid for BitPn.
 * Note, neither the given position BitPn nor the given Status Value not monitored.
 *
 * \param   uint16*   Data        pointer to a 16-bit variable, where a bit schould be modified
 * \param   uint8     BitStartPn  position of the bit, from where the pattern has to be inserted
 * \param   uint8     BitLn       bit length of the pattern to be inserted
 * \param   uint16    Pattern     pattern of the bit to be inserted
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITS_U16U8U8U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBits_u16u8u8u16_Inl(uint16* Data, uint8 BitStartPn, uint8 BitLn, uint16 Pattern)
{
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = (((uint16) (~(uint16) ((BFX_MAXUINT16_U >> (16uL - BitLn)) << BitStartPn))) & *Data) |
                                                (uint16)(((BFX_MAXUINT16_U >> (16uL - BitLn)) & Pattern) << BitStartPn);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBits_u32u8u8u32
 *
 * \brief Sets or clears a bit at a given position Pos_u32 in the 32-bit variable Data.
 * A priori, range [0,1] is valid for Value, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,31] is valid for BitPn.
 * Note, neither the given position P_u32 nor the given value Val_u32 not monitored.
 *
 * \param   uint32*  Data         pointer to a 32-bit variable, where a bit schould be modified
 * \param   uint8    BitStartPn   position of the bit, from where the pattern has to be inserted
 * \param   uint8    BitLn        bit length of the pattern to be inserted
 * \param   uint32   Pattern      pattern of the bit to be inserted
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITS_U32U8U8U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBits_u32u8u8u32_Inl(uint32* Data, uint8 BitStartPn, uint8 BitLn, uint32 Pattern)
{
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = (((uint32) (~(uint32)((BFX_MAXUINT32_U >> (32uL - BitLn)) << BitStartPn))) & *Data) |
                                                (uint32)(((BFX_MAXUINT32_U >> (32uL - BitLn)) & Pattern) << BitStartPn);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBits_u8u8u8u8
 *
 * \brief Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * A priori, range [0,1] is valid for Value, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,7] is valid for BitPn.
 * Note, neither the given position P_u32 nor the given value Val_u32 not monitored.
 *
 * \param  uint8*   Data          pointer to a 8-bit variable, where a bit schould be modified
 * \param  uint8    BitStartPn    position of the bit, from where the pattern has to be inserted
 * \param  uint8    BitLn         bit length of the pattern to be inserted
 * \param  uint8    Pattern       pattern of the bit to be inserted
 * \return void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITS_U8U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBits_u8u8u8u8_Inl(uint8* Data, uint8 BitStartPn, uint8 BitLn, uint8 Pattern)
{
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = (((uint8) (~(uint8)((BFX_MAXUINT8_U >> (8uL - BitLn)) << BitStartPn))) & *Data) | (uint8)(((BFX_MAXUINT8_U
                                                                            >> (8uL - BitLn)) & Pattern) << BitStartPn);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBits_u64u8u8u64
 *
 * \brief Sets or clears a bit at a given position in the 64-bit variable Data.
 * A priori, range [0,1] is valid for Value, case 0: the bit is cleared, case 1 the bit is set.
 * A priori, range [0,63] is valid for BitPn.
 * Note, neither the given position P_u64 nor the given value Val_u64 not monitored.
 *
 * \param   Bfx_uint64*  Data         pointer to a 32-bit variable, where a bit schould be modified
 * \param   uint8        BitStartPn   position of the bit, from where the pattern has to be inserted
 * \param   uint8        BitLn        bit length of the pattern to be inserted
 * \param   Bfx_uint64   Pattern      pattern of the bit to be inserted
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITS_U64U8U8U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBits_u64u8u8u64_Inl(Bfx_uint64* Data, uint8 BitStartPn, uint8 BitLn, Bfx_uint64 Pattern)
{
    /* Additional typecast is required to remove the MISRA 12 warning */
    *Data = (((Bfx_uint64) (~(Bfx_uint64)((BFX_MAXUINT64_U >> (64uLL - BitLn)) << BitStartPn))) & *Data) |
                                           (Bfx_uint64)(((BFX_MAXUINT64_U >> (64uLL - BitLn)) & Pattern) << BitStartPn);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBitsMask_u16u16u16
 * This function shall put all bits defined in Pattern and for which the corresponding Mask bit is set to 1 in the input
 * data.
 *
 * \param   uint16*   Data      Pointer to input data
 * \param   uint16    Pattern   pattern to be set
 * \param   uint16    Mask      Defines which bits of pattern to be put in data.
 * \return  void
 *
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITSMASK_U16U16U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u16u16u16_Inl(uint16* Data, uint16 Pattern, uint16 Mask)
{
    *Data = (*Data & ((uint16)~(Mask)) ) | (Pattern & Mask);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBitsMask_u32u32u32
 * This function shall put all bits defined in Pattern and for which the corresponding Mask bit is set to 1 in the input
 * data.
 *
 * \param  uint32*   Data      Pointer to input data
 * \param  uint32    Pattern   pattern to be set
 * \param  uint32    Mask      Defines which bits of pattern to be put in data.
 * \return void
 *
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITSMASK_U32U32U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u32u32u32_Inl(uint32* Data, uint32 Pattern, uint32 Mask)
{
    *Data =  (*Data & (~(Mask)) ) | (Pattern & Mask);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBitsMask_u8u8u8
 * This function shall put all bits defined in Pattern and for which the corresponding Mask bit is set to 1 in the input
 * data.
 *
 * \param   uint8*   Data     Pointer to input data
 * \param   uint8    Pattern  pattern to be set
 * \param   uint8    Mask     Defines which bits of pattern to be put in data.
 * \return  void
 *
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITSMASK_U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u8u8u8_Inl(uint8* Data, uint8 Pattern, uint8 Mask)
{
    *Data = (*Data & ((uint8)~(Mask)) ) | (Pattern & Mask);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_PutBitsMask_u64u64u64
 * This function shall put all bits defined in Pattern and for which the corresponding Mask bit is set to 1 in the input
 * data.
 *
 * \param  Bfx_uint64*   Data      Pointer to input data
 * \param  Bfx_uint64    Pattern   pattern to be set
 * \param  Bfx_uint64    Mask      Defines which bits of pattern to be put in data.
 * \return void
 *
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_PUTBITSMASK_U64U64U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_PutBitsMask_u64u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Pattern, Bfx_uint64 Mask)
{
    *Data =  (*Data & (~(Mask)) ) | (Pattern & Mask);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitLt_u16u8_Inl
 *
 * \brief This function shall rotate data to the left by ShiftCnt.
 * The most significant bit is rotated to the least significant bit location for every single bit shift cycle.
 *
 * \param   uint16*     Data       pointer to the 16-bit variable, which should be rotated.
 * \param   uint8       ShiftCnt   the number of bits that should be rotated to the left.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITLT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitLt_u16u8_Inl(uint16* Data, uint8 ShiftCnt)
{
    *Data = ((uint16) (*Data << ShiftCnt) | (uint16) (*Data >> (16u - ShiftCnt)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitLt_u32u8_Inl
 *
 * \brief This function shall rotate data to the left by ShiftCnt.
 * The most significant bit is rotated to the least significant bit location for every single bit shift cycle.
 *
 * \param   uint32*    Data       pointer to the 32-bit variable, which should be rotated.
 * \param   uint8      ShiftCnt   the number of bits that should be rotated to the left.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITLT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitLt_u32u8_Inl(uint32* Data, uint8 ShiftCnt)
{
    *Data = (*Data << ShiftCnt) | (*Data >> (32u - ShiftCnt));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitLt_u8u8_Inl
 *
 * \brief This function shall rotate data to the left by ShiftCnt.
 * The most significant bit is rotated to the least significant bit location for every single bit shift cycle.
 *
 * \param   uint8*    Data       pointer to the 8-bit variable, which should be rotated.
 * \param   uint8     ShiftCnt   the number of bits that should be rotated to the left.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITLT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitLt_u8u8_Inl(uint8* Data, uint8 ShiftCnt)
{
    *Data = ((uint8) (*Data << ShiftCnt) | (uint8) (*Data >> (8u - ShiftCnt)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitLt_u64u8_Inl
 *
 * \brief This function shall rotate data to the left by ShiftCnt.
 * The most significant bit is rotated to the least significant bit location for every single bit shift cycle.
 *
 * \param   Bfx_uint64*    Data       pointer to the 64-bit variable, which should be rotated.
 * \param   uint8          ShiftCnt   the number of bits that should be rotated to the left.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITLT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitLt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt)
{
    *Data = (*Data << ShiftCnt) | (*Data >> (64u - ShiftCnt));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitRt_u16u8_Inl
 *
 * \brief This function shall rotate data to the right by ShiftCnt.
 * The least significant bit is rotated to the most significant bit location for every single bit shift cycle.
 *
 * \param   uint16*  Data       pointer to the 16-bit variable, which should be rotated.
 * \param   uint8    ShiftCnt   the number of bits that should be rotated to the right.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITRT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitRt_u16u8_Inl(uint16* Data, uint8 ShiftCnt)
{
    *Data = ((uint16) (*Data >> ShiftCnt) | (uint16) (*Data << (16u - ShiftCnt)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitRt_u32u8_Inl
 *
 * \brief This function shall rotate data to the right by ShiftCnt.
 * The least significant bit is rotated to the most significant bit location for every single bit shift cycle.
 *
 * \param   uint32*  Data       pointer to the 32-bit variable, which should be rotated.
 * \param   uint8    ShiftCnt   the number of bits that should be rotated to the right.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITRT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitRt_u32u8_Inl(uint32* Data, uint8 ShiftCnt)
{
    *Data = (*Data >> ShiftCnt) | (*Data << (32u - ShiftCnt));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitRt_u8u8_Inl
 *
 * \brief This function shall rotate data to the right by ShiftCnt.
 * The least significant bit is rotated to the most significant bit location for every single bit shift cycle.
 *
 * \param   uint8*  Data       pointer to the 8-bit variable, which should be rotated.
 * \param   uint8   ShiftCnt   the number of bits that should be rotated to the right.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITRT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitRt_u8u8_Inl(uint8* Data, uint8 ShiftCnt)
{
    *Data = ((uint8) (*Data >> ShiftCnt) | (uint8) (*Data << (8u - ShiftCnt)));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_Prv_RotBitRt_u64u8_Inl
 *
 * \brief This function shall rotate data to the right by ShiftCnt.
 * The least significant bit is rotated to the most significant bit location for every single bit shift cycle.
 *
 * \param   Bfx_uint64*  Data       pointer to the 64-bit variable, which should be rotated.
 * \param   uint8    ShiftCnt   the number of bits that should be rotated to the right.
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_ROTBITRT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_RotBitRt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt)
{
    *Data = (*Data >> ShiftCnt) | (*Data << (64u - ShiftCnt));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBit_u16u8
 *
 * \brief Sets a bit at the described position BitPn within the variable Data.
 * A priori, the range [0,15] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   uint16*  Data      pointer to a 16-bit variable, to be modified
 * \param   uint8    BitPn2    position to be set
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBIT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBit_u16u8_Inl(uint16* Data, uint8 BitPn)
{
    *Data |= ((uint16) (1uL << BitPn));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBit_u32u8
 *
 * \brief Sets a bit at a defined position Pos named BitPn within the variable Data.
 * A priori, the range [0,31] is valid for position BitPn. Note, the given position is not monitored.
 *
 * \param   uint32*  Data     pointer to a 32-bit variable, to be modified
 * \param   uint8    BitPn    position to be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBIT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBit_u32u8_Inl(uint32* Data, uint8 BitPn)
{
    *Data |= ((uint32)(1uL << BitPn));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBit_u8u8
 *
 * \brief Sets a bit at a defined position BitPn, in the 8-bit variable Data.
 * A priori, range [0,7] is valid for BitPn. Note, the given position is not monitored.
 *
 * \param   uint8*  Data     pointer to the 8-bit variable, to be modified
 * \param   uint8   BitPn    position to be set
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBIT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBit_u8u8_Inl(uint8* Data, uint8 BitPn)
{
    *Data |= ((uint8)(1uL << BitPn));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBit_u64u8
 *
 * \brief Sets a bit at a defined position Pos named BitPn within the variable Data.
 * A priori, the range [0,63] is valid for position BitPn. Note, the given position is not monitored.
 *
 * \param   Bfx_uint64*  Data     pointer to a 64-bit variable, to be modified
 * \param   uint8        BitPn    position to be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBIT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBit_u64u8_Inl(Bfx_uint64* Data, uint8 BitPn)
{
    *Data |= ((Bfx_uint64)(1uLL << BitPn));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBitMask_u16u16
 *
 * \brief Sets bits at the positions, given in the variable Mask.
 *
 * \param   uint16*  Data   pointer to the variable,  where the bits should be set
 * \param   uint16   Mask   bitmask, defines the bits to set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITMASK_U16U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBitMask_u16u16_Inl(uint16* Data, uint16 Mask)
{
    *Data |= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBitMask_u32u32
 *
 * \brief Sets bits at the positions, given in the variable Mask.
 *
 * \param   uint32*   Data   pointer to the variable,  where the bits should be set
 * \param   uint32    Mask   bitmask. defines the bits to set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITMASK_U32U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBitMask_u32u32_Inl(uint32* Data, uint32 Mask)
{
    *Data |= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBitMask_u8u8
 *
 * \brief Sets bits at the positions, given in the variable Mask.
 *
 * \param   uint8*  Data   pointer to the variable, where the bits should be set
 * \param   uint8   Mask   bitmask, defines the bits to set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITMASK_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBitMask_u8u8_Inl(uint8* Data, uint8 Mask)
{
    *Data |= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBitMask_u64u64
 *
 * \brief Sets bits at the positions, given in the variable Mask.
 *
 * \param   Bfx_uint64*   Data   pointer to the variable,  where the bits should be set
 * \param   Bfx_uint64    Mask   bitmask. defines the bits to set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITMASK_U64U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask)
{
    *Data |= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBits_u16u8u8u8
 *
 * \brief
 *
 * \param   uint16*  Data          pointer to the variable, where the bits should be set
 * \param   uint8    BitStartPn    indicates the starting bit position
 * \param   uint8    BitLn         indicates the bit field length
 * \param   uint8    Status        indicates the status value
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITS_U16U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBits_u16u8u8u8_Inl(uint16* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status)
{
    uint16  mask_u16;


    /* Generate the mask */
    mask_u16 = (uint16) (BFX_MAXUINT16_U >> BitStartPn);
    mask_u16 = (uint16) (mask_u16 << (16u - BitLn));
    mask_u16 = (uint16) (mask_u16 >> (16u - BitStartPn - BitLn));

    /* Set bits on the defined field */
    *Data |= mask_u16;

    /* Clear bits when Status = 0 using the generated mask */
    if (Status == 0u)
    {
        *Data ^= mask_u16;
    }
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBits_u32u8u8u8
 *
 * \brief
 *
 * \param   uint8* Data          pointer to the variable, where the bits should be set
 * \param   uint8  BitStartPn    indicates the starting bit position
 * \param   uint8  BitLn         indicates the bit field length
 * \param   uint8  Status        indicates the status value
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITS_U32U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBits_u32u8u8u8_Inl(uint32* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status)
{
    uint32  mask_u32;


    /* Generate the mask */
    mask_u32 = (uint32) (BFX_MAXUINT32_U >> BitStartPn);
    mask_u32 = (uint32) (mask_u32 << (32u - BitLn));
    mask_u32 = (uint32) (mask_u32 >> (32u - BitStartPn - BitLn));

    /* Set bits on the defined field */
    *Data |= mask_u32;

    /* Clear bits when Status = 0 using the generated mask */
    if (Status == 0u)
    {
        *Data ^= mask_u32;
    }
}
#endif



/**
 **********************************************************************************************************************
 * Bfx_SetBits_u16u8u8u8
 *
 * \brief
 *
 * \param   uint8*  Data          pointer to the variable, where the bits should be set
 * \param   uint8   BitStartPn    indicates the starting bit position
 * \param   uint8   BitLn         indicates the bit field length
 * \param   uint8   Status        indicates the status value
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITS_U8U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBits_u8u8u8u8_Inl(uint8* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status)
{
    uint8  mask_u8;


    /* Generate the mask */
    mask_u8 = (uint8) (BFX_MAXUINT8_U >> BitStartPn);
    mask_u8 = (uint8) (mask_u8 << (8u - BitLn));
    mask_u8 = (uint8) (mask_u8 >> (8u - BitStartPn - BitLn));

    /* Set bits on the defined field */
    *Data |= mask_u8;

    /* Clear bits when Status = 0 using the generated mask */
    if (Status == 0u)
    {
        *Data ^= mask_u8;
    }
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_SetBits_u64u8u8u8
 *
 * \brief
 *
 * \param   Bfx_uint64* Data          pointer to the variable, where the bits should be set
 * \param   uint8       BitStartPn    indicates the starting bit position
 * \param   uint8       BitLn         indicates the bit field length
 * \param   uint8       Status        indicates the status value
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SETBITS_U64U8U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_SetBits_u64u8u8u8_Inl(Bfx_uint64* Data, uint8 BitStartPn, uint8 BitLn, uint8 Status)
{
    Bfx_uint64  mask_u64;


    /* Generate the mask */
    mask_u64 = (Bfx_uint64) (BFX_MAXUINT64_U >> BitStartPn);
    mask_u64 = (Bfx_uint64) (mask_u64 << (64u - BitLn));
    mask_u64 = (Bfx_uint64) (mask_u64 >> (64u - BitStartPn - BitLn));

    /* Set bits on the defined field */
    *Data |= mask_u64;

    /* Clear bits when Status = 0 using the generated mask */
    if (Status == 0u)
    {
        *Data ^= mask_u64;
    }
}
#endif



/**
 **********************************************************************************************************************
 * Bfx_ShiftBitLt_u16u8
 *
 * \brief  Shift the a variable to the left by a specified amount ShiftCnt., is equal to: <<ShiftCnt
 *
 * \param   uint16* Data      pointer to the variable, where the bits should be set
 * \param   uint8   ShiftCnt  shift left count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITLT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u16u8_Inl(uint16* Data, uint8 ShiftCnt)
{
    *Data <<= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitLt_u32u8
 *
 * \brief  Shift the a variable to the left by a specified amount ShiftCnt., is equal to: <<ShiftCnt
 *
 * \param   uint32* Data       pointer to the variable, where the bits should be set
 * \param   uint8   ShiftCnt   shift left count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITLT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u32u8_Inl(uint32* Data, uint8 ShiftCnt)
{
    *Data <<= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitLt_u8u8
 *
 * \brief  Shift the a variable to the left by a specified amount ShiftCnt., is equal to: <<ShiftCnt
 *
 * \param   uint8* Data       pointer to the variable, where the bits should be set
 * \param   uint8  ShiftCnt   shift left count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITLT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u8u8_Inl(uint8* Data, uint8 ShiftCnt)
{
    *Data <<= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitLt_u64u8
 *
 * \brief  Shift the a variable to the left by a specified amount ShiftCnt., is equal to: <<ShiftCnt
 *
 * \param   Bfx_uint64* Data       pointer to the variable, where the bits should be set
 * \param   uint8       ShiftCnt   shift left count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITLT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitLt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt)
{
    *Data <<= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitRt_u16u8
 *
 * \brief  Shift the a variable to the right by a specified amount ShiftCnt., is equal to: >>ShiftCnt
 *
 * \param   uint16* Data       pointer to the variable, where the bits should be set
 * \param   uint8   ShiftCnt   shift right count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITRT_U16U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u16u8_Inl(uint16* Data, uint8 ShiftCnt)
{
    *Data >>= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitRt_u32u8
 *
 * \brief  Shift the a variable to the right by a specified amount ShiftCnt., is equal to: >>ShiftCnt
 *
 * \param   uint32* Data       pointer to the variable, where the bits should be set
 * \param   uint8   ShiftCnt   shift right count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITRT_U32U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u32u8_Inl(uint32* Data, uint8 ShiftCnt)
{
    *Data >>= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitRt_u8u8
 *
 * \brief  Shift the a variable to the right by a specified amount ShiftCnt., is equal to: >>ShiftCnt
 *
 * \param   uint8*  Data       pointer to the variable, where the bits should be set
 * \param   uint8   ShiftCnt   shift right count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITRT_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u8u8_Inl(uint8* Data, uint8 ShiftCnt)
{
    *Data >>= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ShiftBitRt_u64u8
 *
 * \brief  Shift the a variable to the right by a specified amount ShiftCnt., is equal to: >>ShiftCnt
 *
 * \param   Bfx_uint64* Data       pointer to the variable, where the bits should be set
 * \param   uint8       ShiftCnt   shift right count
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_SHIFTBITRT_U64U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ShiftBitRt_u64u8_Inl(Bfx_uint64* Data, uint8 ShiftCnt)
{
    *Data >>= ShiftCnt;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBitMask_u16u16
 *
 * \brief  Is equalt to a XOR operations of a variable and a mask
 *
 * \param   uint16* Data  pointer to the variable, where the bits should be set
 * \param   uint16  Mask  defines the bits to toggle
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITMASK_U16U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u16u16_Inl(uint16* Data, uint16 Mask)
{
    *Data ^= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBitMask_u32u32
 *
 * \brief  Is equalt to a XOR operations of a variable and a mask
 *
 * \param   uint32* Data  pointer to the variable, where the bits should be set
 * \param   uint32  Mask  defines the bits to toggle
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITMASK_U32U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u32u32_Inl(uint32* Data, uint32 Mask)
{
    *Data ^= Mask;
}
#endif

/**
 **********************************************************************************************************************
 * Bfx_ToggleBitMask_u8u8
 *
 * \brief  Is equalt to a XOR operations of a variable and a mask
 *
 * \param   uint8* Data  pointer to the variable, where the bits should be set
 * \param   uint8  Mask  defines the bits to toggle
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITMASK_U8U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u8u8_Inl(uint8* Data, uint8 Mask)
{
    *Data ^= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBitMask_u64u64
 *
 * \brief  Is equalt to a XOR operations of a variable and a mask
 *
 * \param   Bfx_uint64* Data  pointer to the variable, where the bits should be set
 * \param   Bfx_uint64  Mask  defines the bits to toggle
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITMASK_U64U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBitMask_u64u64_Inl(Bfx_uint64* Data, Bfx_uint64 Mask)
{
    *Data ^= Mask;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBits_u16
 *
 * \brief  Is equalt to a  coplement of  a variable
 *
 * \param   uint16 * Data   pointer to the variable, where the bits should be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITS_U16_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBits_u16_Inl(uint16* Data)
{
    *Data ^= 0xFFFFu;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBits_u32
 *
 * \brief  Is equalt to a  coplement of  a variable
 *
 * \param   uint32* Data   pointer to the variable, where the bits should be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITS_U32_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBits_u32_Inl(uint32* Data)
{
    *Data ^= 0xFFFFFFFFuL;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBits_u8
 *
 * \brief  Is equalt to a  coplement of  a variable
 *
 * \param   uint8* Data   pointer to the variable, where the bits should be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITS_U8_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBits_u8_Inl(uint8* Data)
{
    *Data ^= 0xFFu;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_ToggleBits_u64
 *
 * \brief  Is equalt to a  complement of  a variable
 *
 * \param   Bfx_uint64* Data   pointer to the variable, where the bits should be set
 * \return  void
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TOGGLEBITS_U64_INL_REMAPPED
LOCAL_INLINE void Bfx_Prv_ToggleBits_u64_Inl(Bfx_uint64* Data)
{
    *Data ^= 0xFFFFFFFFFFFFFFFFuLL;
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitLnMask_u16u16_u8
 *
 * \brief Tests if minimum one bit corresponding to the mask Mask is also set in the variable Data.
 *
 * \param  uint16   Data  Bitfield which should be checked.
 * \param  uint16   Mask  Bitmask that defines which bit should be "1".
 * \return Returns
 * \retval TRUE           at least one Bit of Mask is set in Data
 * \retval FALSE          absolutely no Bit of Mask is set in Data
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITLNMASK_U16U16_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u16u16_u8_Inl(uint16 Data, uint16 Mask)
{
    return((Data & Mask) != 0uL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitLnMask_u32u32_u8
 *
 * \brief Tests if minimum one bit corresponding to the mask Mask is also set in the variable Data.
 *
 * \param  uint32   Data  Bitfield which should be checked.
 * \param  uint32   Mask  Bitmask that defines which bit should be "1".
 * \return Returns
 * \retval TRUE           at least one Bit of Mask is set in Data
 * \retval FALSE          absolutely no Bit of Mask is set in Data
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITLNMASK_U32U32_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u32u32_u8_Inl(uint32 Data, uint32 Mask)
{
    return ((Data & Mask) != 0uL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitLnMask_u8u8_u8
 *
 * \brief Tests if minimum one bit corresponding to the mask Mask is also set in the variable Data.
 *
 * \param  uint8   Data Bitfield which should be checked.
 * \param  uint8   Mask Bitmask that defines which bit should be "1".
 * \return Returns
 * \retval TRUE         at least one Bit of Mask is set in Data
 * \retval FALSE        absolutely no Bit of Mask is set in Data
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITLNMASK_U8U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u8u8_u8_Inl(uint8 Data, uint8 Mask)
{
    return ((Data & Mask) != 0uL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitLnMask_u64u64_u8
 *
 * \brief Tests if minimum one bit corresponding to the mask Mask is also set in the variable Data.
 *
 * \param  Bfx_uint64   Data  Bitfield which should be checked.
 * \param  Bfx_uint64   Mask  Bitmask that defines which bit should be "1".
 * \return Returns
 * \retval TRUE           at least one Bit of Mask is set in Data
 * \retval FALSE          absolutely no Bit of Mask is set in Data
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITLNMASK_U64U64_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitLnMask_u64u64_u8_Inl(Bfx_uint64 Data, Bfx_uint64 Mask)
{
    return ((Data & Mask) != 0uLL);
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitMask_u16u16_u8
 *
 * \brief Tests if all bits corresponding to the mask Mask are set also in the variable Data.
 *
 * \param   uint16  Data      Base variable which is compared with the mask.
 * \param   uint16  Mask      Bit mask that defines which bits should be set in Base.
 * \return  boolean           Returns if all bits set in mask are set in Base, too.
 * \retval          TRUE      All bits, set in Mask are set in Data.
 * \retval          FALSE     Minimum one bit, set in Mask fails in Data.
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITMASK_U16U16_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u16u16_u8_Inl(uint16 Data, uint16 Mask)
{
    return ((Data & Mask) == (Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitMask_u32u32_u8
 *
 * \brief Tests if all bits corresponding to the mask Mask are set also in the variable Data.
 *
 * \param   uint32  Data      Base variable which is compared with the mask.
 * \param   uint32  Mask      Bit mask that defines which bits should be set in Base.
 * \return  boolean           Returns if all bits set in mask are set in Base, too.
 * \retval          TRUE      All bits, set in Mask are set in Data.
 * \retval          FALSE     Minimum one bit, set in Mask fails in Data.
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITMASK_U32U32_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u32u32_u8_Inl(uint32 Data, uint32 Mask)
{
    return ((Data & Mask) == (Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitMask_u8u8_u8
 *
 * \brief Tests if all bits corresponding to the mask Mask are set also in the variable Data.
 *
 * \param   uint8   Data      Base variable which is compared with the mask.
 * \param   uint8   Mask      Bit mask that defines which bits should be set in Base.
 * \return  boolean           Returns if all bits set in mask are set in Base, too.
 * \retval          TRUE      All bits, set in Mask are set in Data.
 * \retval          FALSE     Minimum one bit, set in Mask fails in Data.
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITMASK_U8U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u8u8_u8_Inl(uint8 Data, uint8 Mask)
{
    return ((Data & Mask) == (Mask));
}
#endif


/**
 **********************************************************************************************************************
 * Bfx_TstBitMask_u64u64_u8
 *
 * \brief Tests if all bits corresponding to the mask Mask are set also in the variable Data.
 *
 * \param   Bfx_uint64  Data      Base variable which is compared with the mask.
 * \param   Bfx_uint64  Mask      Bit mask that defines which bits should be set in Base.
 * \return  boolean           Returns if all bits set in mask are set in Base, too.
 * \retval          TRUE      All bits, set in Mask are set in Data.
 * \retval          FALSE     Minimum one bit, set in Mask fails in Data.
 **********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTBITMASK_U64U64_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstBitMask_u64u64_u8_Inl(Bfx_uint64 Data, Bfx_uint64 Mask)
{
    return ((Data & Mask) == (Mask));
}
#endif


/**
 ***********************************************************************************************************************
 * Bfx_TstParityEven_u16_u8
 *
 * \brief Gets the parity and returns TRUE if the number of bits set is even, otherwise FALSE
 *
 * \param       uint16     Data       Variable to check
 * \return      boolean               if parity is even, return is TRUE, otherwie it is FALSE
 ***********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTPARITYEVEN_U16_U8_INL_REMAPPED
 LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u16_u8_Inl(uint16 Data)
{
     uint16 tmp0_u16;
     uint16 tmp1_u16;
     boolean res_b;

     tmp0_u16 = Data;

     tmp1_u16 = (tmp0_u16 >> 8U);
     tmp0_u16 = (tmp0_u16 ^ tmp1_u16);

     tmp1_u16 = (tmp0_u16 >> 4U);
     tmp0_u16 = (tmp0_u16 ^ tmp1_u16);

     tmp1_u16 = (tmp0_u16 >> 2U);
     tmp0_u16 = (tmp0_u16 ^ tmp1_u16);

     tmp1_u16 = (tmp0_u16 >> 1U);
     tmp0_u16 = (tmp0_u16 ^ tmp1_u16);

     tmp0_u16  = (tmp0_u16 & 0x1U);
     res_b = (boolean)((tmp0_u16 == 0u) ? TRUE : FALSE);

     return (res_b);
}
#endif


/**
 ***********************************************************************************************************************
 * Bfx_TstParityEven_u32_u8
 *
 * \brief Gets the parity and returns TRUE if the number of bits set is even, otherwise FALSE
 *
 * \param       uint32      Data       Variable to check
 * \return      boolean                Parity
 ***********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTPARITYEVEN_U32_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u32_u8_Inl(uint32 Data)
{
    uint32 tmp0_u32;
    uint32 tmp1_u32;
    boolean res_b;

    tmp0_u32 = Data;

    tmp1_u32 = (tmp0_u32 >> 16UL);
    tmp0_u32 = (tmp0_u32 ^ tmp1_u32);

    tmp1_u32 = (tmp0_u32 >> 8UL);
    tmp0_u32 = (tmp0_u32 ^ tmp1_u32);

    tmp1_u32 = (tmp0_u32 >> 4UL);
    tmp0_u32 = (tmp0_u32 ^ tmp1_u32);

    tmp1_u32 = (tmp0_u32 >> 2UL);
    tmp0_u32 = (tmp0_u32 ^ tmp1_u32);

    tmp1_u32 = (tmp0_u32 >> 1UL);
    tmp0_u32 = (tmp0_u32 ^ tmp1_u32);

    tmp0_u32  = (tmp0_u32 & 0x1UL);
    res_b = (boolean)((tmp0_u32 == 0uL) ? TRUE : FALSE);

    return (res_b);
}
#endif


/**
 ***********************************************************************************************************************
 * Bfx_TstParityEven_u8_u8
 *
 * \brief Gets the parity and returns TRUE if the number of bits set is even, otherwise FALSE
 *
 * \param       uint8     Data       Variable to check
 * \return      boolean              if parity is even, return is TRUE, otherwie it is FALSE
 ***********************************************************************************************************************
 */
 #ifndef BFX_PRV_TSTPARITYEVEN_U8_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u8_u8_Inl(uint8 Data)
{
    uint8 tmp0_u8;
    uint8 tmp1_u8;
    boolean res_b;

    tmp0_u8 = Data;

    tmp1_u8 = (tmp0_u8 >> 4U);
    tmp0_u8 = (tmp0_u8 ^ tmp1_u8);

    tmp1_u8 = (tmp0_u8 >> 2U);
    tmp0_u8 = (tmp0_u8 ^ tmp1_u8);

    tmp1_u8 = (tmp0_u8 >> 1U);
    tmp0_u8 = (tmp0_u8 ^ tmp1_u8);

    tmp0_u8  = (tmp0_u8 & 0x1U);
    res_b = (boolean)((tmp0_u8 == 0u) ? TRUE : FALSE);

    return (res_b);
}
#endif


/**
 ***********************************************************************************************************************
 * Bfx_TstParityEven_u64_u8
 *
 * \brief Gets the parity and returns TRUE if the number of bits set is even, otherwise FALSE
 *
 * \param       Bfx_uint64      Data       Variable to check
 * \return      boolean                Parity
 ***********************************************************************************************************************
 */
#ifndef BFX_PRV_TSTPARITYEVEN_U64_U8_INL_REMAPPED
LOCAL_INLINE boolean Bfx_Prv_TstParityEven_u64_u8_Inl(Bfx_uint64 Data)
{
        Bfx_uint64 tmp0_u64;
        Bfx_uint64 tmp1_u64;
        boolean res_b;

        tmp0_u64 = Data;

        tmp1_u64 = (tmp0_u64 >> 32ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp1_u64 = (tmp0_u64 >> 16ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp1_u64 = (tmp0_u64 >> 8ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp1_u64 = (tmp0_u64 >> 4ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp1_u64 = (tmp0_u64 >> 2ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp1_u64 = (tmp0_u64 >> 1ULL);
        tmp0_u64 = (tmp0_u64 ^ tmp1_u64);

        tmp0_u64  = (tmp0_u64 & 0x1ULL);
        res_b = (boolean)((tmp0_u64 == 0uLL) ? TRUE : FALSE);

        return (res_b);
}
#endif



/* BFX_BIT_INL_H */
#endif
