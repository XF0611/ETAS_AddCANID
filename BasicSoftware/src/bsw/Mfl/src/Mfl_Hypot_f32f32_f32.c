


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
 * Mfl_Hypot_f32f32_f32
 *
 * This service computes the length of a vector: 
 * Result = square_root ( x_value * x_value + y_value * y_value)
 *
 * \param   float32   x_value     First argument Recommended input range: [-24PI, +24PI]
 * \param   float32   y_value     Second argument Recommended input range [-24PI, +24PI]
 * \return  float32               Return value of the function
 **********************************************************************************************************************
 */
#if (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Hypot_f32f32_f32(float32 x_value, float32 y_value)
{
    return Mfl_Sqrt_f32((x_value * x_value) + (y_value * y_value));
}

#if (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_HYPOT_F32F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



