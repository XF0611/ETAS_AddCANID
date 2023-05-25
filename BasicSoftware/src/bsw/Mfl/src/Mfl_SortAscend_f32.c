


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
 * Mfl_SortAscend_f32
 *
 * \brief Sort a float32 array in ascending order.
 *
 * Sort a float32 array with the number of Num elements in ascending order.
 * This service modifies directly the memory where the array is stored.
 *
 * Restrictions:
 * - Maximum number of elements in array is 255.
 * - Thus this service is implemented with the "Bubble-Sort" algorithm,
 *   runtime effort increases exponentially by the number of elements.
 *
 * \param    float32*  Array          Pointer to array
 * \param    uint16    Num            Number of elements
 * \return   void
 ***********************************************************************************************************************
 */
#if (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_SortAscend_f32(float32* Array, uint16 Num)
{
    float32        tmp_f32;
    uint16_least   Loop1_qu16;
    uint16_least   Loop2_qu16;

    for(Loop1_qu16 = ((uint16_least)Num); Loop1_qu16 > 1u; Loop1_qu16--)
    {
        for(Loop2_qu16 = 0u; Loop2_qu16 < (Loop1_qu16 - 1u); Loop2_qu16++)
        {
            if(Array[Loop2_qu16] > Array[Loop2_qu16 + 1u])
            {
                tmp_f32             = Array[Loop2_qu16];
                Array[Loop2_qu16]      = Array[Loop2_qu16 + 1u];
                Array[Loop2_qu16 + 1u] = tmp_f32;
            }
        }
    }
}

#if (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_SORTASCEND_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



