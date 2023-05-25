


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
 * Mfl_Floor_f32
 *
 * Returns the natural number value determined by rounding the argument toward negative infinity
 *
 * \param   float32   ValValue     Floating-point operand
 * \return  float32                Operand rounded to floor
 **********************************************************************************************************************
 */
#if (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Floor_f32(float32 ValValue)
{
    Mfl_f32u32u    res_f32u32u;
    uint32         clear_u32;
    uint8_least    tmp_u8;        
    
    res_f32u32u.f32 = ValValue;
    
    /* Gets the exponential bits, relevant for treatment of the float */
    tmp_u8 = ((uint8) ((res_f32u32u.u32 & 0x7F800000uL) >> 23u));
    
    if(tmp_u8 < 150u)
    {
        if(tmp_u8 < 127u)
        {
            /* Sign is not negative */ 
            if (Mfl_Sign_f32(ValValue) != -1)
            {
                /* Signed zero */
                res_f32u32u.u32 &= 0x80000000uL;
            }
            else
            {
                /* -1 */
                res_f32u32u.u32 = 0xBF800000uL;
            }
        }
        else
        {
                    
            /* Calculate number of mantissa bits to clear */
            tmp_u8 = 150u - tmp_u8;
            
            /* Clear mask, first step: 1 at the position to clear */
            clear_u32  = (1uL << tmp_u8) - 1uL;
                    
            /* Negative case and define if highering the exponent is required */
            if ((Mfl_Sign_f32(ValValue) == -1) && ((res_f32u32u.u32 & clear_u32) > 0uL))
            {
                /* Higher the LSB of the combination "exponent-relevant mantissa" 
                (clear_u32 + 1) leads to a ...000000100000..., where 1 marks the LSB */
                res_f32u32u.u32 += (clear_u32 + 1uL);
            }
            
            /* Clear mask, second step: 1's at the positions NOT to clear */
            clear_u32 ^= 0xFFFFFFFFuL;
            
            /* Clear */
            res_f32u32u.u32 &= clear_u32;
        }
    }
    
    return (res_f32u32u.f32);
}

#if (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_FLOOR_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



