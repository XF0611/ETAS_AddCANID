


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

#if (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Pow_f32(float32 ValBase, float32 ValExp)
{
    Mfl_f32u32u    tmp1_f32u32u;
    Mfl_f32u32u    tmp2_f32u32u;
    float32        res_f32;

    tmp1_f32u32u.f32 = ValBase;
    tmp2_f32u32u.f32 = ValExp;

    /* If ValBase and ValExp are Positive/Negative zero, then the result is one.
       If ValBase is Positive/Negative zero and ValExp is a non zero number, then the result is zero
       Check for zero is done in terms of bit pattern of float as floating point
       values shall not be tested for equality and unequality as per MR12 DIR 1.1 */
    if((tmp1_f32u32u.u32 == 0UL) || (tmp1_f32u32u.u32 == 0x80000000UL))
    {
        if(tmp2_f32u32u.u32 == 0UL)
        {
            res_f32 = 1.0F;
        }
        else
        {
            res_f32 = 0.0F;
        }
    }
    else
    {
        /* pow(x, y) = exp(y * log(x)) */
        res_f32 = (Mfl_Exp_f32(Mfl_Log_f32(ValBase) * ValExp));
    }

    return(res_f32);
}

#if (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_POW_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



