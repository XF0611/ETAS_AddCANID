


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
 * Returns the cosine of a floating point number given in radians.
 *
 * \param   float32 value               The floating point value in range [-24PI,24PI] whose cosine is to be returned.
 * \return  float32                     The cosine of the argument, in radians in the range of [-1,1].
 *
 **********************************************************************************************************************
 */

#if (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Cos_f32(float32 value)
{
    float32 sin_f32, cos_f32;

    Mfl_Prv_SinCos_f32(value, &sin_f32, &cos_f32);

    return cos_f32;
}

#if (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_COS_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



