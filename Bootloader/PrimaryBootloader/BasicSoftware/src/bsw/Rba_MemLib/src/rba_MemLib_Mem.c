

#include "rba_MemLib.h"
#include "rba_MemLib_Prv.h"


/* Preparation for integrity check */
#define RBA_MEMLIB_PRV_MAGIC_NUMBER   (20200713uL)

/* Integration integrity check */
#if (!defined(RBA_MEMLIB_CHECKPROTO_MAGIC_NUMBER) || (RBA_MEMLIB_CHECKPROTO_MAGIC_NUMBER != (RBA_MEMLIB_PRV_MAGIC_NUMBER+2uL)))
#error Integrity error in rba_MemLib_CheckProto.h integration - use the provided header only
#endif



#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"





/** ***********************************************************************
 * \brief               Copy data from src_pcu8 to dst_pu8 in uint8 steps (increasing addresses)
 *
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes to copy (0 allowed)
 */
void rba_MemLib_MemCopy_Provided(uint8 const * src_pcu8, uint8 * dst_pu8, uint32 length_u32)
{
    uint8   const   *   curSrc_pcu8;
    uint8           *   curDst_pu8;
    uint32              lenRemaining_u32;

    /* Prepare running variables - note: parameters shall not be modified */
    curSrc_pcu8         = (uint8 const *)src_pcu8;
    curDst_pu8          =   (uint8*)dst_pu8;
    lenRemaining_u32    = length_u32;

#if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)

    /* Special handling only if 4 or more bytes are to be transfered */
    if (lenRemaining_u32 >= 4u)
    {
        uint32              bytesUnalignedPreSrc_u32;
        uint32              bytesUnalignedPreDst_u32;
        uint32              bytesUnalignedPre_u32;

        /* Check the alignment of the data */
        /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to enable bit operations */
        bytesUnalignedPreSrc_u32  = (uint32)curSrc_pcu8;
        bytesUnalignedPreSrc_u32 &= 3uL;
        bytesUnalignedPreDst_u32  = (uint32)curDst_pu8;
        bytesUnalignedPreDst_u32 &= 3uL;

        /* Aligned bytes copy possible?
         * Only if initial alignment is identical and more than 4 bytes to be transfered */
        if (bytesUnalignedPreSrc_u32 == bytesUnalignedPreDst_u32)
        {
            uint32  addr_u32;
            uint32  const   *   curSrc_pcu32;
            uint32          *   curDst_pu32;
            uint32              bytesAligned_u32;

            /* Aligned copy is possible */
            /* Determine the number of bytes to be precopied: */
            /* alignment: 0 -> pre-copy 0 */
            /* alignment: 1 -> pre-copy 3 */
            /* alignment: 2 -> pre-copy 2 */
            /* alignment: 3 -> pre-copy 1 */
            /* Note: at least 4 bytes are to be copied because of condition above */
            bytesUnalignedPre_u32 = (4uL - bytesUnalignedPreSrc_u32) & 0x3uL;

            /* Perform the Pre-Copy */
            rba_MemLib_MemCopyU8U8(curSrc_pcu8, curDst_pu8, bytesUnalignedPre_u32);

            /* Determine the new copy-base */
            curSrc_pcu8         += bytesUnalignedPre_u32;
            curDst_pu8          += bytesUnalignedPre_u32;
            lenRemaining_u32    -= bytesUnalignedPre_u32;

            /*******************************************************************************/
            /* Source and destination are 32bit aligned at this point.
             * Determine the number of bytes to be transfered in 32bit units */
            bytesAligned_u32 = 0xFFFFFFFCuL & lenRemaining_u32;

            /* explicitely convert the pointers to 32bit as the alignment is 32bit aligned */
            /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
            addr_u32     = (uint32)curSrc_pcu8;
            curSrc_pcu32 = (uint32 const *)addr_u32;
            addr_u32     = (uint32)curDst_pu8;
            curDst_pu32  = (uint32       *)addr_u32;

            /* Copy data in 32bit granularity */
            rba_MemLib_MemCopyU32U32(
                    curSrc_pcu32,
                    curDst_pu32,
                    bytesAligned_u32
            );

            /* Determine the new copy-base */
            lenRemaining_u32    -= bytesAligned_u32;
            curSrc_pcu8         += bytesAligned_u32;
            curDst_pu8          += bytesAligned_u32;
        }
    }

#endif

    /* Unaligned bytes post copy */
    rba_MemLib_MemCopyU8U8(curSrc_pcu8, curDst_pu8, lenRemaining_u32);
    return;
}





/** ***********************************************************************
 * \brief               Compare two buffers (in 1byte steps)
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu8   Pointer to buffer 1 - no alignment constraint
 * \param   bfr2_pcu8   Pointer to buffer 2 - no alignment constraint
 * \param   length_u32  Number of bytes to set - no alignment constraint (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
 *
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareU8U8(uint8 const * bfr1_pcu8, uint8 const * bfr2_pcu8, uint32 length_u32 )
{
    uint32 ct;
    boolean  flgRet = TRUE;

    for(ct=0uL; ct < length_u32; ct++)
    {
        if (*bfr1_pcu8 != *bfr2_pcu8)
        {
            flgRet = FALSE;
            break;
        }
        bfr1_pcu8++;
        bfr2_pcu8++;
    }
    return flgRet;
}



/** ***********************************************************************
 * \brief               Compare two buffers (in 4byte steps)
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu32  Pointer to buffer 1 - alignment constraint!
 * \param   bfr2_pcu32  Pointer to buffer 2 - alignment constraint!
 * \param   length_u32  Number of bytes to set - alignment constraint (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
 *
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareU32U32(uint32 const * bfr1_pcu32, uint32 const * bfr2_pcu32, uint32 length_u32 )
{
    uint32 ct;
    boolean  flgRet = TRUE;

    length_u32 = length_u32/4uL;

    for(ct=0uL; ct < length_u32; ct++)
    {
        if (*bfr1_pcu32 != *bfr2_pcu32)
        {
            flgRet = FALSE;
            break;
        }
        bfr1_pcu32++;
        bfr2_pcu32++;
    }
    return flgRet;
}



/** ******************************************************************************************
 * \brief               Compare two buffers
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu8   Pointer to buffer 1 - no alignment constraint
 * \param   bfr2_pcu8   Pointer to buffer 2 - no alignment constraint
 * \param   length_u32  Number of bytes to compare (0 allowed)
 * \retval  TRUE        Buffers are fully equal
 * \retval  FALSE       At least one entry in the buffers are not same
 */
boolean rba_MemLib_MemCompareBfr_Provided(uint8 const * bfr1_pcu8, uint8 const * bfr2_pcu8, uint32 length_u32)
{
    uint8   const   *   curBfr1_pcu8 = bfr1_pcu8;
    uint8   const   *   curBfr2_pcu8 = bfr2_pcu8;
    uint32   lenRemaining_u32  = length_u32;
    boolean  flgRet = TRUE;

#if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)
    /*comparison of bytes in the buffer in chunks of 4 bytes*/
    if (lenRemaining_u32  >= 4u)
    {
        uint32              bytesUnalignedPreSrc_u32;
        uint32              bytesUnalignedPreDst_u32;
        uint32              bytesUnalignedPre_u32;

        /* Check the alignment of the data */
        /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to enable bit operations */
        bytesUnalignedPreSrc_u32  = (uint32)curBfr1_pcu8;
        bytesUnalignedPreSrc_u32 &= 3uL;
        bytesUnalignedPreDst_u32  = (uint32)curBfr2_pcu8;
        bytesUnalignedPreDst_u32 &= 3uL;


        /* Aligned bytes copy possible?
         * Only if initial alignment is identical and more than 4 bytes to be transfered */
        if (bytesUnalignedPreSrc_u32 == bytesUnalignedPreDst_u32)
        {
            uint32  addr_u32;
            uint32  const   *   curBfr1_pcu32;
            uint32  const   *   curBfr2_pcu32;
            uint32              bytesAligned_u32;

            /* Aligned copy is possible */
            /* Determine the number of bytes to be precopied: */
            /* alignment: 0 -> pre-copy 0 */
            /* alignment: 1 -> pre-copy 3 */
            /* alignment: 2 -> pre-copy 2 */
            /* alignment: 3 -> pre-copy 1 */
            /* Note: at least 4 bytes are to be copied because of condition above */
            bytesUnalignedPre_u32 = (4uL - bytesUnalignedPreSrc_u32) & 0x3uL;

            /* Perform the Pre-Copy */
            flgRet = rba_MemLib_MemCompareU8U8(curBfr1_pcu8, curBfr2_pcu8, bytesUnalignedPre_u32);

            if (flgRet != FALSE)
            {

                /* Determine the new copy-base */
                curBfr1_pcu8        += bytesUnalignedPre_u32;
                curBfr2_pcu8        += bytesUnalignedPre_u32;
                lenRemaining_u32    -= bytesUnalignedPre_u32;

                /*******************************************************************************/
                /* Source and destination are 32bit aligned at this point.
                 * Determine the number of bytes to be transfered in 32bit units */
                bytesAligned_u32 = 0xFFFFFFFCuL & lenRemaining_u32;

                /* explicitely convert the pointers to 32bit as the alignment is 32bit aligned */
                /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
                addr_u32     = (uint32)curBfr1_pcu8;
                curBfr1_pcu32= (uint32 const *)addr_u32;
                /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
                addr_u32     = (uint32)curBfr2_pcu8;
                curBfr2_pcu32= (uint32 const *)addr_u32;

                /* Copy data in 32bit granularity */
                flgRet = rba_MemLib_MemCompareU32U32(
                        curBfr1_pcu32,
                        curBfr2_pcu32,
                        bytesAligned_u32
                );

                /* Determine the new copy-base */
                lenRemaining_u32    -= bytesAligned_u32;
                curBfr1_pcu8        += bytesAligned_u32;
                curBfr2_pcu8        += bytesAligned_u32;
            }
        }
    }
#endif

    #if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)
    if (flgRet != FALSE)
#endif
    {
        /* Unaligned post-compare */
        flgRet = rba_MemLib_MemCompareU8U8(curBfr1_pcu8,curBfr2_pcu8,lenRemaining_u32 );
    }

    return(flgRet);
}





/** ***********************************************************************
 * \brief               Set 8bit value to memory (in 1byte steps)
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    8bit value to be set in the destimation buffer
 * \param   length_u32  Number of bytes to set - no alignment constraint (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemSetU8(uint8 * dst_pu8, uint8 value_u8, uint32 length_u32 )
{
    uint32 ct;

    for(ct=0uL; ct < length_u32; ct++)
    {
        *dst_pu8 = value_u8;
        dst_pu8++;
    }
}



/** ******************************************************************************************
 * \brief               Set a buffer to a defined value
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    Value to be set
 * \param   length_u32  Number of bytes to copy (0 allowed)
 */
void rba_MemLib_MemSet_Provided(uint8 * dst_pu8, uint8 value_u8, uint32 length_u32)
{
    uint8 * curDst_pu8 = dst_pu8;
    uint32  lenRemaining_u32 = length_u32;

#if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)

    /*if data transfer to the buffer is done in chunks of 4 bytes*/
    if(lenRemaining_u32 >= 4u)
    {
        uint32 * destAddress_pu32;
        uint32 value_u32 = (0x01010101uL) * value_u8;
        uint32 bytesUnalignedPre_u32;
        uint32 Loop_u32;
        uint32 bytesAligned_u32;

        /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
        bytesUnalignedPre_u32  = (uint32)curDst_pu8;
        bytesUnalignedPre_u32 &= 3uL;

        /* Aligned set is possible */
        /* Determine the number of bytes to be pre-set: */
        /* alignment: 0 -> pre-copy 0 */
        /* alignment: 1 -> pre-copy 3 */
        /* alignment: 2 -> pre-copy 2 */
        /* alignment: 3 -> pre-copy 1 */
        /* Note: at least 4 bytes are to be copied because of condition above */
        bytesUnalignedPre_u32 = (4uL - bytesUnalignedPre_u32) & 0x3uL;

        /* Perform the Pre-Copy */
        rba_MemLib_MemSetU8(curDst_pu8, value_u8, bytesUnalignedPre_u32);

        /* Determine the new copy-base */
        curDst_pu8          += bytesUnalignedPre_u32;
        {
            /* explicitely convert the pointers to 32bit as the alignment is 32bit aligned */
            /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
            uint32 addr_u32     = (uint32)curDst_pu8;
            destAddress_pu32    = (uint32 *)addr_u32;
        }
        lenRemaining_u32    -= bytesUnalignedPre_u32;

        /*******************************/
        /* Perform 4byte set operation */

        /*Determine the number of loops required to set the buffer with the defined value of 4 bytes*/
        Loop_u32         = lenRemaining_u32 / 4uL;
        bytesAligned_u32 = Loop_u32*4uL;

        /*Sets the buffer with data chunks of 4 bytes*/
        while (Loop_u32>0u)
        {
            *destAddress_pu32 = value_u32;
            destAddress_pu32++;
            Loop_u32--;
        };

        /*Calculate the length of the remaining data once chunks of 4 bytes are determined*/
        lenRemaining_u32 -= bytesAligned_u32;
        curDst_pu8       += bytesAligned_u32;
    }
#endif

    /* Unaligned post-set */
    rba_MemLib_MemSetU8(curDst_pu8,value_u8,lenRemaining_u32 );

    return;
}




/** ***********************************************************************
 * \brief               Compare 8bit value to memory (in 1byte steps)
 *                      note: if length=0, then return=TRUE
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    8bit value to be set in the destimation buffer
 * \param   length_u32  Number of bytes to set - no alignment constraint (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
 *
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareU8(uint8 const * dst_pcu8, uint8 value_u8, uint32 length_u32 )
{
    uint32 ct;
    boolean  flgRet = TRUE;

    for(ct=0uL; ct < length_u32; ct++)
    {
        if (*dst_pcu8 != value_u8)
        {
            flgRet = FALSE;
            break;
        }
        dst_pcu8++;
    }
    return flgRet;
}





/** ******************************************************************************************
 * \brief       Compare a buffer to a defined value
 *                      note: if length=0, then return=TRUE
 * \param   bfr_pcu8    Pointer to the buffer - no alignment constraint
 * \param   value_u8    Value that should be compared to
 * \param   length_u32  Amount of bytes to check (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
 */
boolean rba_MemLib_MemCompareValue_Provided(uint8 const * bfr_pcu8, uint8 value_u8, uint32 length_u32)
{
    uint8 const * curDst_pcu8 = bfr_pcu8;
    uint32  lenRemaining_u32 = length_u32;
    boolean  flgRet = TRUE;

#if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)

    /*if data transfer to the buffer is done in chunks of 4 bytes*/
    if(lenRemaining_u32 >= 4u)
    {
        uint32 const * destAddress_pu32;
        uint32 value_u32 = (0x01010101uL) * value_u8;
        uint32 bytesUnalignedPre_u32;
        uint32 Loop_u32;
        uint32 bytesAligned_u32;

        /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
        bytesUnalignedPre_u32  = (uint32)curDst_pcu8;
        bytesUnalignedPre_u32 &= 3uL;

        /* Aligned set is possible */
        /* Determine the number of bytes to be pre-set: */
        /* alignment: 0 -> pre-copy 0 */
        /* alignment: 1 -> pre-copy 3 */
        /* alignment: 2 -> pre-copy 2 */
        /* alignment: 3 -> pre-copy 1 */
        /* Note: at least 4 bytes are to be copied because of condition above */
        bytesUnalignedPre_u32 = (4uL - bytesUnalignedPre_u32) & 0x3uL;

        /* Perform the Pre-Copy */
        flgRet = rba_MemLib_MemCompareU8(curDst_pcu8, value_u8, bytesUnalignedPre_u32);

        /* Still necessary to process? */
        if (flgRet != FALSE)
        {
            /* Determine the new copy-base */
            curDst_pcu8          += bytesUnalignedPre_u32;
            {
                /* explicitely convert the pointers to 32bit as the alignment is 32bit aligned */
                /* MR12 RULE 11.4 VIOLATION: Casts to uint32 to cast between pointers, alignment is ensured by algo */
                uint32 addr_u32     = (uint32)curDst_pcu8;
                destAddress_pu32    = (uint32 *)addr_u32;
            }
            lenRemaining_u32    -= bytesUnalignedPre_u32;

            /*******************************/
            /* Perform 4byte aligned compare operation */

            /*Determine the number of loops required to set the buffer with the defined value of 4 bytes*/
            Loop_u32         = lenRemaining_u32 / 4uL;
            bytesAligned_u32 = Loop_u32*4uL;

            /*Sets the buffer with data chunks of 4 bytes*/
            while (Loop_u32>0u)
            {
                if (*destAddress_pu32 != value_u32)
                {
                    flgRet = FALSE;
                    break;
                }
                destAddress_pu32++;
                Loop_u32--;
            };

            /*Calculate the length of the remaining data once chunks of 4 bytes are determined*/
            lenRemaining_u32  -= bytesAligned_u32;
            curDst_pcu8       += bytesAligned_u32;
        }
    }
#endif

#if (RBA_MEMLIB_CODE_SIZE_OPTIMIZED == STD_OFF)
    if (flgRet != FALSE)
#endif
    {
        /* Unaligned post-compare */
        flgRet = rba_MemLib_MemCompareU8(curDst_pcu8,value_u8,lenRemaining_u32 );
    }

    return(flgRet);
}



#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"


