


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#define SRVLIBS

#include "Mfl.h"


/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */


/**
 ***********************************************************************************************************************
 * Mfl_MedianSort_f32_f32
 *
 * \brief Calculates the median of a float32 array.
 *
 * The function sorts the values of the array in ascending order 
 * and returns the median value. The array is modified in memory by this function.
 *
 * Calculation:
 *              Result =  Array[N/2]                                     if N is even
 *              Result = (Array[N/2] + Array[(N +1)/2]) / 2              if N is odd
 *
 *
 * \param    float32 *       Array              Pointer to the array
 * \param    uint8           N                  Size of the array
 * \return   float32                            Median value
 ***********************************************************************************************************************
 */
#if (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_MedianSort_f32_f32(float32* Array, uint8 N)
{
    uint8 index_u8;
    uint8 tmp_u8;
    float32 res_f32;
    
    /* First value in the array is assigned to result */
    res_f32 = *Array;
    
    /* This check is to avoid access violation */
    if(N > 1u)
    {
        /* Sorting the input array in ascending order */
        Mfl_SortAscend_f32(Array, N);
        
        /* Calculating the index of central element in array */
        index_u8 = (N >> 1u);
        res_f32 = Array[index_u8];

        tmp_u8 = N & 0x1U;
        
        /* Check if N is even */
        if(tmp_u8 == 0U)
        {
            res_f32 = res_f32 + Array[index_u8 - 1u];
            res_f32 = res_f32 / 2.0F;
        }
    }
    
    return(res_f32);
}

#if (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_MEDIANSORT_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



