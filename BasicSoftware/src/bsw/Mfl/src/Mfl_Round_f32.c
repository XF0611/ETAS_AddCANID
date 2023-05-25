


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
 * Mfl_Round_f32
 *
 * Returns the integer value determined by rounding the argument toward the near-est whole number
 * Eg. 36.56 will be rounded to 37.00 
 * If the argument is halfway between two integers, it is rounded away from zero
 * Eg. 36.5 will be rounded to 37.00
 *
 * \param   float32   ValValue     Floating-point operand
 * \return  float32                Rounded value of operand
 **********************************************************************************************************************
 */
#if (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Round_f32(float32 ValValue)
{
    Mfl_f32u32u    res1_f32u32u;
    Mfl_f32u32u    res2_f32u32u;
    uint32         clear_u32;
    uint8_least    tmp_u8;
            
    res1_f32u32u.f32 = ValValue;
    
    /* Gets the exponential bits, relevant for treatment of the float */
    tmp_u8 = ((uint8) ((res1_f32u32u.u32 & 0x7F800000uL) >> 23u));
    
    if(tmp_u8 < 150u)
    {
        if(tmp_u8 < 127u)
        {
            /* Signed zero */ 
            res1_f32u32u.u32 &= 0x80000000uL;
            
            if(tmp_u8 == 126u)
            {
                /* Signed 1 */
                res1_f32u32u.u32 |= 0x3F800000uL;
            }
        }
        else
        {
            /* Calculate number of mantissa bits to clear */
            tmp_u8 = 150u - tmp_u8;
            
            /* Clear mask, first step: 1 at the position to clear */
            clear_u32  = (1uL << tmp_u8) - 1uL;
                    
            /* Positive case and define if highering the exponent is required.*/            
            if(((res1_f32u32u.u32 & clear_u32) > 0uL))
            {
                res2_f32u32u.u32 = res1_f32u32u.u32 + (clear_u32 + 1uL);
            
                /* Clear mask, second step: 1's at the positions NOT to clear */
                clear_u32 ^= 0xFFFFFFFFuL;
            
                /* Clear */
                res1_f32u32u.u32 &= clear_u32;
                res2_f32u32u.u32 &= clear_u32;
                    
                /* Result comparison */
                if(Mfl_Abs_f32(res2_f32u32u.f32 - ValValue) <  Mfl_Abs_f32(res1_f32u32u.f32 - ValValue))
                {
                    res1_f32u32u.f32 = res2_f32u32u.f32;
                }
            }
        }
    }
    
    return (res1_f32u32u.f32);
}

#if (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ROUND_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



