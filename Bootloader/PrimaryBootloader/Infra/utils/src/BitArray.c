/*********************************************************************************************************************
*
* Description:     
* FileName:        BitArray.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "BitArray.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define ASSERT_FAIL_RETURN_E_NOK(cond)   if(cond) return E_NOT_OK;
/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/
Std_ReturnType BitArray_SetBit(BitArrayType *pBa, uint32 bit)       /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint32 Idx, Off;

    ASSERT_FAIL_RETURN_E_NOK(pBa == NULL_PTR);          /* PRQA S 2214 */
    ASSERT_FAIL_RETURN_E_NOK(!pBa->NumBits);            /* PRQA S 2214, 4558 */
    ASSERT_FAIL_RETURN_E_NOK(bit > pBa->NumBits);       /* PRQA S 2214 */

    Idx = bit / WORD_BITNESS(pBa);      /* PRQA S 1840 */
    Off = bit % WORD_BITNESS(pBa);      /* PRQA S 1840 */

    pBa->Words[Idx] |= (uint32)(1 << Off);      /* PRQA S 4543, 4393 */
    Ret = E_OK;
    
    return Ret;
}

Std_ReturnType BitArray_ClearBit(BitArrayType *pBa, uint32 bit)     /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint32 Idx, Off;

    ASSERT_FAIL_RETURN_E_NOK(pBa == NULL_PTR);      /* PRQA S 2214 */
    ASSERT_FAIL_RETURN_E_NOK(!pBa->NumBits);        /* PRQA S 2214, 4558 */
    ASSERT_FAIL_RETURN_E_NOK(bit > pBa->NumBits);   /* PRQA S 2214 */

    Idx = bit / WORD_BITNESS(pBa);      /* PRQA S 1840 */
    Off = bit % WORD_BITNESS(pBa);      /* PRQA S 1840 */

    pBa->Words[Idx] &= (uint32)(~(uint32)(1 << Off));       /* PRQA S 4543, 4393 */
    Ret = E_OK;
    return Ret;
}

Std_ReturnType BitArray_ShiftRight(BitArrayType *pBa, uint32 shift)     /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint32 msbShiftOff, lsbShiftOff, sIdx, eIdx, msbIdx;

    ASSERT_FAIL_RETURN_E_NOK(pBa == NULL_PTR);      /* PRQA S 2214 */
    ASSERT_FAIL_RETURN_E_NOK(!pBa->NumBits);        /* PRQA S 2214, 4558 */

    msbShiftOff = shift % WORD_BITNESS(pBa);        /* PRQA S 1840 */
    lsbShiftOff = WORD_BITNESS(pBa) - msbShiftOff;  /* PRQA S 1840 */
    sIdx = shift / WORD_BITNESS(pBa);               /* PRQA S 1840 */
    eIdx = pBa->NumWords - sIdx - 1;                /* PRQA S 1840 */

    for(uint32 index = 0; index < pBa->NumWords; index++)
    {
        if(index <= eIdx)
        {
            msbIdx = eIdx - index;
            pBa->Words[msbIdx] >>= msbShiftOff;
            if(index != eIdx)
            {
                pBa->Words[msbIdx] |= pBa->Words[msbIdx - 1] << lsbShiftOff;        /* PRQA S 1840 */
            }
        }
    }
    return Ret;
}

Std_ReturnType BitArray_ShiftLeft(BitArrayType *pBa, uint32 shift)      /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint32 msbShiftOff, lsbShiftOff, sIdx, eIdx, msbIdx;

    ASSERT_FAIL_RETURN_E_NOK(pBa == NULL_PTR);      /* PRQA S 2214 */
    ASSERT_FAIL_RETURN_E_NOK(!pBa->NumBits);        /* PRQA S 2214, 4558 */

    msbShiftOff = shift % WORD_BITNESS(pBa);        /* PRQA S 1840 */
    lsbShiftOff = WORD_BITNESS(pBa) - msbShiftOff;  /* PRQA S 1840 */
    sIdx = shift / WORD_BITNESS(pBa);               /* PRQA S 1840 */
    eIdx = pBa->NumWords - sIdx - 1;                /* PRQA S 1840 */

    for(uint32 index = 0; index < pBa->NumWords; index++)
    {
        if(index <= eIdx)
        {
            msbIdx = index + sIdx;
            pBa->Words[msbIdx] <<= msbShiftOff;
            if(index != eIdx)
            {
                pBa->Words[msbIdx] |= pBa->Words[msbIdx + 1] >> lsbShiftOff;        /* PRQA S 1840 */
            } 
        }
    }
    return Ret;
}
