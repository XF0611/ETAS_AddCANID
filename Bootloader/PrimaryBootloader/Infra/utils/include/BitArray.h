/*********************************************************************************************************************
*
* Description:     
* FileName:        BitArray.h
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
#ifndef  BITARRAY_H
#define  BITARRAY_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
/**
 * @brief number of bit on 1 word
 * 
 */
#define WORD_BITNESS(ba)    (sizeof(ba->Words[0]) * 8)
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/
/**
 * @brief 
 * 
 */
#define BITARRAY_DEF(name, totalBits, mod)  \
    mod uint32 BitArrayWords_##name[(((totalBits + 8 - 1) / 8) + sizeof(uint32) - 1) / sizeof(uint32)] = {0};\
    mod BitArrayType name = { \
                                .NumBits    = totalBits, \
                                .NumWords   = (((totalBits + 8 - 1) / 8) + sizeof(uint32) - 1) / sizeof(uint32),\
                                .Words      = BitArrayWords_##name, \
                            }


#define BITARRAY_CREATE_STATIC(name, totalBits) \
                    BITARRAY_DEF(name, totalBits, static)

#define BITARRAY_CREATE(name, totalBits) \
                    BITARRAY_DEF(name, totalBits,)

/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef struct
{
    uint32 NumBits; /*Number of Bits*/
    uint32 NumWords;/*Number of Words*/
    uint32 *Words;  /*Word of Bits*/
}BitArrayType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/
/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/
extern Std_ReturnType BitArray_SetBit(BitArrayType *pBa, uint32 bit);
extern Std_ReturnType BitArray_ClearBit(BitArrayType *pBa, uint32 bit);
extern Std_ReturnType BitArray_ShiftRight(BitArrayType *pBa, uint32 shift);
extern Std_ReturnType BitArray_ShiftLeft(BitArrayType *pBa, uint32 shift);

#endif
