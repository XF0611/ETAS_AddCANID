


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
 * Returns the arc cosine of a floating point number given in radians.
 *
 * \param   float32 value                  The floating point value in range [-1,1] whose arc cosine is to be returned.
 * \return  float32                        The arc cosine of the argument, in radians in the range of [0,PI].
 *
 **********************************************************************************************************************
 */
#if (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_ArcCos_f32(float32 value)
{
    float32 z_f32;

    /* Arccos computed according to PI/2 - arcsin(value) */
    z_f32 = 1.5707963267948966f - Mfl_ArcSin_f32(value);

    return z_f32;
}

#if (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ARCCOS_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



