


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
 * Mfl_Cvrt_s16_f32
 *
 * Returns the floating-point result of the conversion, determined according to the following equation.
 *
 * \param   sint16   ValFixedInteger     Integer value of the fixed-point quantity to be converted
 * \param   sint16   ValFixedExponent    Exponent of the fixed-point quantity to be converted
 * \return  float32                      The floating-point result of the conversion
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Cvrt_s16_f32(sint16 ValFixedInteger, sint16 ValFixedExponent)
{
    return(Mfl_Cvrt_s32_f32(ValFixedInteger, ValFixedExponent));
}

#if (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_S16_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



