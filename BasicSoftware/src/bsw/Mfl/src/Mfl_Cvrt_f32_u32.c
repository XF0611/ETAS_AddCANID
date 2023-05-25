


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
 * Mfl_Cvrt_f32_u32
 *
 * \brief Converts float to fixed point value
 *
 * \param  float32   ValFloat           Floating-point quantity to be converted
 * \param  sint16    ValFixedExponent   Exponent of the fixed-point result of the conversion
 * \return uint32                       Integer value of the fixed-point result
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

uint32 Mfl_Cvrt_f32_u32(float32 ValFloat, sint16 ValFixedExponent)
{
    Mfl_f32u32u    tmp_f32u32u;
    uint32         res_u32 = 0UL;
    sint32         exp_s32;
    uint32         mant_u32;
    uint32         tmp_u32;
    uint32         Is_Negative_u32;
    uint32         temp_u32;
    sint32         temp_s32;
    
    tmp_f32u32u.f32 = ValFloat;
    
    /* Get Sign bit of ValFloat */
    Is_Negative_u32 = (tmp_f32u32u.u32 >> 31u);
    
    /* Absolute IEEE754 float representation */
    tmp_f32u32u.u32 = tmp_f32u32u.u32 & 0x7FFFFFFFUL;

    if((tmp_f32u32u.u32 != 0UL) && (Is_Negative_u32 == 0uL))
    {
        /* Get the exponential bits of ValFloat */
        temp_u32 = ((tmp_f32u32u.u32 & 0x7F800000UL) >> 23u);
        exp_s32 = ((sint32)temp_u32);
    
        /* Get 23 bit mantisasa bits of ValFloat */
        mant_u32 = (tmp_f32u32u.u32 & 0x7FFFFFUL) ;
        
        /* If the exponent is zero and mantissa is not zero, then the floating point value is a denormal value */
        if(exp_s32 == 0L)
        {
            tmp_u32 = mant_u32;
            
            /* Compute the number of bits in the mantissa part excluding the leading zero */
            while(tmp_u32 > 0UL)
            {
                tmp_u32 = (tmp_u32 >> 1u);
                exp_s32++;
            }
            
            /* Convert the de normal number to normal number with negative exponent 
               Example: Consider ValFloat = 1.40129846e-045. The exp_s32 is zero and mant_u32 = 0x1. 
               But for further calculation, all values are considered to be normal value. Hence 
               exp_s32 should be -22 and mant_u32 should be 0.
               Using the while loop, exp_s32 is made as 1 and is subtracted from 23 to obtain -22. 
               Mantissa is calculated as mant_u32 << (1 - (-22)) = 0x0 */
            exp_s32 = exp_s32 - 23L;
            temp_s32 = 1L - exp_s32;
            mant_u32 = mant_u32 << ((uint32)temp_s32);
        }
    
        /* Calculate the actual exponent of (ValFloat * 2^ValFixedExponent) by removing the bias value(127)
           added to the exponent as per IEEE754 representation */
        exp_s32 = (exp_s32  + (sint32)ValFixedExponent) - 127L;
        
        /* Check for overflow 
        1. If exp_s32 >= 32, then the value (ValFloat * 2^ValFixedExponent) will be greater than 0xFFFFFFFF
        2. If tmp_f32u32u.u32 = 0x7F800000, then the value is infinity. If tmp_f32u32u.u32 > 0x7F800000, then 
           the floating point value is NAN and there is no representation for it in integer */
        if((exp_s32 >= 32L) || (tmp_f32u32u.u32 >= 0x7F800000UL))
        {
            res_u32 = MFL_MAXUINT32;
        }
        /* Check for underflow
        If exp_s32 < 0, then -1 < (ValFloat * 2^ValFixedExponent) < 1 */
        else if(exp_s32 < 0x0L)
        {
            res_u32 = MFL_MINUINT32;
        }
        else
        {              
            /* Set the virtual 24th bit, as 1 */
            mant_u32 = mant_u32 | 0x800000UL;
            
            /* Compute the integer part from mantissa */
            if(exp_s32 <= 23L)
            {
                temp_s32 = 23L - exp_s32;
                res_u32  = (mant_u32 >> ((uint32)temp_s32));
            }
            else
            {
                temp_s32 = exp_s32 - 23L;
                res_u32 = (mant_u32 << ((uint32)temp_s32));
            }
        }
    }
   
    return (res_u32);
}

#if (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



