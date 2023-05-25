


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
 * Mfl_Cvrt_s32_f32
 *
 * Returns the floating-point result of the conversion, determined according to the following equation.
 *
 * \param   sint32   ValFixedInteger     Integer value of the fixed-point quantity to be converted
 * \param   sint16   ValFixedExponent    Exponent of the fixed-point quantity to be converted
 * \return  float32                      The floating-point result of the conversion
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Cvrt_s32_f32(sint32 ValFixedInteger, sint16 ValFixedExponent)
{
    Mfl_f32u32u    res_f32u32u;
    uint32         tmp_u32;
    uint32         sign_u32 = 0UL;
    
    /* Remove the sign of ValFixedInteger */
    if(ValFixedInteger < 0L)
    {
        sign_u32 = 1UL;
        tmp_u32 = (uint32)(-ValFixedInteger);
    }
    else
    {
        tmp_u32 = (uint32)(ValFixedInteger);
    }
    
    res_f32u32u.f32 = Mfl_Cvrt_u32_f32(tmp_u32, ValFixedExponent);
    
    /* Place the sign bit at the 31st bit position */
    res_f32u32u.u32 = res_f32u32u.u32 | (sign_u32 << 31u);
    
    return(res_f32u32u.f32);
}

#if (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



