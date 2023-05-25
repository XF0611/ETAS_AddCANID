


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
 * Mfl_Cvrt_u32_f32
 *
 * Returns the floating-point result of the conversion, determined according to the following equation.
 *
 * \param   uint32   ValFixedInteger     Integer value of the fixed-point quantity to be converted
 * \param   sint16   ValFixedExponent    Exponent of the fixed-point quantity to be converted
 * \return  float32                      The floating-point result of the conversion
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Cvrt_u32_f32(uint32 ValFixedInteger, sint16 ValFixedExponent)
{
    Mfl_f32u32u    res_f32u32u;
    uint32         tmp_u32;
    uint32         mant_u32;
    sint32         exp_s32;
    sint32         count_s32 = 0;
    
    res_f32u32u.u32 = 0UL;
    
    /* If ValFixedInteger is 0x0, then the float value will always be zero */
    if(ValFixedInteger != 0UL)
    {
        tmp_u32 = ValFixedInteger;
        
        /* Count the (number of bits - 1) in ValFixedInteger without the leading zeros */
        while(tmp_u32 > 1UL)
        {
            count_s32++;
            tmp_u32 = tmp_u32 >> 1u;
        }
        
        /* Calculate the exponent of the (ValFixedInteger * 2^-ValFixedExponent) in IEEE754 format
           and add the bias value(127) */
        exp_s32 = (count_s32 - (sint32)ValFixedExponent) + 127L;
        
        /* Values greater than maximum representable value */
        if(exp_s32 > 254L)
        {
            res_f32u32u.u32 = 0x7F800000UL;
        }
        /* Values less than the minimum representable value */
        else if(exp_s32 < -22L)
        {
            res_f32u32u.u32 = 0x0UL;
        }
        else
        {
            /* Denormal value */
            if(exp_s32 <= 0L)
            {
                tmp_u32 = ValFixedInteger;
                
                /* For denormal value, the floating point value is given by (-1)^sign * Mantissa * 2^(1-127-P) 
                   where P is the mantissa resolution expressed in bits. The exponent bits of denormal value is 
                   always zero. To make it as zero, the calculated exponent(exp_s32) is subtracted from count_u8.
                   Example: Consider ValFixedInteger = 1562(0x61A) and ValFixedExponent = 140.
                   exp_s32 = (10 - 140) + 127 = -3. For Denormal value, the bias is (127 - 1) = 126. Hence the new 
                   exp_s32 = exp_s32 - 1 = -4 which means ValFixedInteger should be left shifted by 4(count_u8 = 14) */
                count_s32 = count_s32 + (1L -exp_s32);
                exp_s32 = 0L;
            }
            /* Normal Value */
            else
            {
                /* Since 1 is implicit for normal value, it is removed from the mantissa part */
                tmp_u32 = ValFixedInteger ^ (1UL << (uint32)count_s32);
            }
            
            /* Mantissa of IEEE754 floationg point representation */
            if(count_s32 >= 23L)
            {
                mant_u32 = tmp_u32 >> ((uint32)count_s32 - 23uL);
            }
            else
            {
                mant_u32 = tmp_u32 << (23uL - (uint32)count_s32);
            }

            /* Bit representation of a IEEE754 floating point number */
            res_f32u32u.u32 = ((uint32)exp_s32 << 23u) | mant_u32;
        
            /* Rounding the value to the nearest representable floating point value. 
               Example: Consider ValFixedInteger = 0x7FFFFFFF and ValFixedExponent = 0x0
               Then count_u8 is 31. exp_s32 will be 158 ,mant_u32 will be 0x7FFFFF. 
               The floating point representation of this will be 4.29496704e+09(0x4F7FFFFF). 
               But in IEEE754 format, the floating point representation of 0xFFFFFFFF is 4.294967296e+09(0x4F800000).
               This is because the mantissa can hold only 23-bits. For more accuracy, the 24-bit is added to 
               the bit representation of the floating point number. */
            if(count_s32 > 23L)
            {
                res_f32u32u.u32 = res_f32u32u.u32 + ((tmp_u32 >> ((uint32)count_s32 - 24uL)) & 1UL);
            }
        }
    }
    
    return(res_f32u32u.f32);
}

#if (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_U32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



