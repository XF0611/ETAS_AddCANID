


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
 **********************************************************************************************************************
 * Mfl_Trunc_f32
 *
 * \brief  Returns the integer value determined by rounding a float value towards zero
 *
 * Calculation:   Is based on the data representation according to the IEEE-754 standard
 *
 * Restrictions:
 *
 * \param   float32 ValValue  float value, which has to be rounded
 * \return  float32
 **********************************************************************************************************************
 */
#if (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Trunc_f32(float32 ValValue)
{
    Mfl_f32u32u    res_f32u32u;
    uint32         clear_u32;
    uint8_least    tmp_qu8;

    res_f32u32u.f32 = ValValue;
    
    /* Gets the exponential bits, relevant for treatment of the float */
    tmp_qu8 = ((uint8) ((res_f32u32u.u32 & 0x7F800000uL) >> 23u));

    if(tmp_qu8 < 150u)
    {
        if(tmp_qu8 < 127u)
        {
            /* Signed zero */
            res_f32u32u.u32 &= 0x80000000uL;
        }
        else
        {
            /* Calculate number of mantissa bits to clear */
            tmp_qu8 = 150u - tmp_qu8;

            /* Clear mask */
            clear_u32  = (1uL << tmp_qu8) - 1uL;
            clear_u32 ^= 0xFFFFFFFFuL;

            /* Clear */
            res_f32u32u.u32 &= clear_u32;
        }
    }

    return(res_f32u32u.f32);
}

#if (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_TRUNC_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



