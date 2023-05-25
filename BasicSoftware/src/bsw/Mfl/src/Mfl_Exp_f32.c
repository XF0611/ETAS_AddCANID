


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#define SRVLIBS

#include "Mfl.h"


const float32 Mfl_ExpIntegralTable_caf32[] =
{ 
    2.71828182845904510f,           /* exp(1)  */
    7.38905609893065040f,           /* exp(2)  */
    54.5981500331442360f,           /* exp(4)  */
    2980.95798704172830f,           /* exp(8)  */
    8886110.52050787210f,           /* exp(16) */
    78962960182680.6870f,           /* exp(32) */
    6235149080811616700000000000.0f /* exp(64) */
};

/* LUT contains ln of values starting from (1+2^-x) where x varies from 1 - 24 based on IEEE754 floating point format*/
const float32 Mfl_ExpDecimalTable_caf32[] =
{
    0.405465096235275270f,
    0.223143547773361210f,
    0.117783032357692720f,
    0.060624621808528900f,
    0.0307716578245162960f,
    0.0155041869729757310f,
    0.00778214028105139730f,
    0.00389864039607346060f,
    0.00195122009608894590f,
    0.000976085953880101440f,
    0.000488162069814279680f,
    0.000244110822677612300f,
    0.000122062861919403080f,
    0.0000610332936048507690f,
    0.0000305171124637126920f,
    0.0000152586726471781730f,
    0.00000762936542741954330f,
    0.00000381468998966738580f,
    0.00000190734681382309650f,
    0.000000953673861658899110f,
    0.000000476837044516287280f,
    0.000000238418550679853070f,
    0.000000119209282445353890f,
    0.0000000596046447753906250f
};
/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Mfl_Prv_GetExpo_Inl
 * Returns the exponential of a floating point number, i.e. e^value.
 *
 * \param   float32 value                        Floating-point operand in range [-24PI, 24PI].
 * \return  float32                              Returns the exponential of the operand value.
 **********************************************************************************************************************
 */
LOCAL_INLINE sint32 Mfl_Prv_GetExpo_Inl(float32 value)
{
    Mfl_f32u32u    res_f32u32u;
    sint32         exp_s32;
    uint32         temp_u32;

    res_f32u32u.f32 = value;
    
    /* Remove sign bit */
    res_f32u32u.u32 &= 0x7FFFFFFFUL;

    /* Then extract exponent (23 bits) */
    temp_u32 = res_f32u32u.u32 >> 23UL;
    exp_s32 = (sint32)(temp_u32);
    
    /* Compute expo by subtracting 127 */
    exp_s32 = exp_s32 - 127L;

    return exp_s32;
}

/**
 **********************************************************************************************************************
 * Returns the exponential of the operand (ValExp), determined according to the following equation. ValExp = e^ValValue
 *
 * \param   float32 ValValue                     Floating-point operand with value range [-24PI, +24PI]
 * \return  float32                              e raised to ValValue power if ValValue lies in the range [-24PI, +24PI]
 *                                               otherwise NAN is returned, i.e. 0x7FF00000 if 24PI is exceeded,
 *                                               0x7FF00001 if -24PI is exceeded
 **********************************************************************************************************************
 */
#if (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Exp_f32(float32 ValValue)
{
    Mfl_f32u32u    tmp_f32u32u;
    float32        value_f32;
    float32        e_f32;
    float32        integral_part_f32;
    float32        t_f32;
    float32        step_f32;
    float32        val_f32;
    sint32         expo_s32;
    uint32         k_u32;
    boolean        is_negative;
    uint32        temp_u32;

    /* Make copy of parameter value, since it is function internally modified */
    value_f32 = ValValue;

    /* Computes e^ValValue and saves the result in variable e */
    e_f32 = 1.0f;

    /* Auxiliary variables for computing e^x | x < 0 */
    is_negative = FALSE;

    integral_part_f32 = 1.0f;
    t_f32 = 0.0f;
    step_f32 = 1.0f;

    /* Check whether input is negative -> computing on positive values only */
    if (value_f32 < 0.0f)
    {
        is_negative = TRUE;
        value_f32 *= -1.0f;
    }

    /* Value range is exceeded, 
     - Positive infinity is returned if ValValue >= 76,
     - 0 is returned if ValValue <= -76; */
    if (value_f32 < 76.0f)
    {
        /* Compution of integral part of value for e^value
         * this is the case as long as the exponent is positive */
        expo_s32 = Mfl_Prv_GetExpo_Inl(value_f32);

        while (expo_s32 >= 0L)
        {
            integral_part_f32 *= Mfl_ExpIntegralTable_caf32[expo_s32];
            temp_u32 = (1u << (uint32)expo_s32);
            value_f32 -= ((float32)temp_u32) ;
            expo_s32 = Mfl_Prv_GetExpo_Inl(value_f32);
        }

        /* Computation of decimal part of value for e^value */
        if (expo_s32 > -127L)
        {
            for (k_u32 = 0UL; k_u32 < 24UL; k_u32++)
            {
                step_f32 *= 0.5f;
                val_f32 = t_f32 + Mfl_ExpDecimalTable_caf32[k_u32];

                /* MR12 RULE 14.1 VIOLATION:Floating point comparison is required in order ensure the correct 
                behavior of the function */
                while (val_f32 <= value_f32)
                {
                    t_f32 = val_f32;
                    e_f32 += e_f32 * step_f32;
                    val_f32 = t_f32 + Mfl_ExpDecimalTable_caf32[k_u32];    
                }
            }
        }

        /* Scale with precomputed value for the integral part of value */
        e_f32 *= integral_part_f32;

        /* Adjust result if input was negative */
        if (is_negative != FALSE)
        {
            e_f32 = 1.0f / e_f32;
        }
    }
    else
    {
        if (is_negative)
        {
            e_f32 = 0.0F; 
        }
        else
        {
            /* 0x7F800000 is the bit representation of positive infinity */
            tmp_f32u32u.u32 = 0x7F800000UL;
            e_f32 = tmp_f32u32u.f32;
        }
    }

    return e_f32;
}

#if (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_EXP_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



