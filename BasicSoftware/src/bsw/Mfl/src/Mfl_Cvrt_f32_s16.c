


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
 * Mfl_Cvrt_f32_s16
 *
 * \brief Converts float to fixed point value
 *
 * \param   float32   ValFloat           Floating-point quantity to be converted
 * \param   sint16    ValFixedExponent   Exponent of the fixed-point result of the conversion
 * \return  sint16                       Integer value of the fixed-point result
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

sint16 Mfl_Cvrt_f32_s16(float32 ValFloat, sint16 ValFixedExponent)
{
    sint32 res_s32;
    
    res_s32 = Mfl_Cvrt_f32_s32(ValFloat, ValFixedExponent);
    
    /* Limiting the result to sint16 */
    if(res_s32 > MFL_MAXSINT16)
    {
        res_s32 = MFL_MAXSINT16;
    }
    if(res_s32 < MFL_MINSINT16)
    {
        res_s32 =MFL_MINSINT16;
    }
    
    return((sint16)res_s32);
}

#if (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_S16_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



