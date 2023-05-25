


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
 ***************************************************************************************************
 * Returns the square root of X.
 *
 * Computes from a positive float to a float its root square value.
 * Returns for negative arguments 0 as IEEE value.
 *
 * \param    X
 * \return   square root of X
 ***************************************************************************************************
 */
#if (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Sqrt_f32(float32 ValValue)
{
    Mfl_f32u32u    Xn_f32u32u;
    float32        Xhalf_f32;
    uint8_least    i_qu8;

    
    if (ValValue > 0.0F)
    {
        /* Algorithm to get the startup for NEWTON iteration of the square root 
           -------------------------------------------------------------------- */
        Xn_f32u32u.f32 = ValValue;
        Xn_f32u32u.u32 = 0x5f3759dfUL - (Xn_f32u32u.u32 >> 1uL);

        /* Algorithm for the Newton iteration of the inverse square root        
           -------------------------------------------------------------------- */
        Xhalf_f32 = 0.5F * ValValue;        

        for (i_qu8 = 0; i_qu8 < 3u; i_qu8++)
        {
            Xn_f32u32u.f32 = Xn_f32u32u.f32 * (1.5F - (Xhalf_f32 * Xn_f32u32u.f32 * Xn_f32u32u.f32));
        }

        /* Inversion of the Xn_f32u32u.f32 leads to the square root             
           -------------------------------------------------------------------- */
        Xn_f32u32u.f32 *= ValValue;
    }
    else
    {
        Xn_f32u32u.f32 = 0.0F;
    }

    return (Xn_f32u32u.f32);
}

#if (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_SQRT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



