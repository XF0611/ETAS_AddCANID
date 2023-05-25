


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
 * Returns the arc sine of a floating point number given in radians.
 *
 * \param   float32 value                  The floating point value in range [-1,1] whose arc cosine is to be returned.
 * \return  float32                        The arc sine of the argument, in radians in the range of [-PI/2,PI/2].
 *                                         If the argument is zero, then the result is a zero.
 *
 **********************************************************************************************************************
 */
#if (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_ArcSin_f32(float32 value)
{
    Mfl_f32u32u    theta_f32u32u;
    float32        x_f32;
    float32        y_f32;
    float32        z_f32;
    float32        neg_f32;
    float32        val_f32;
    float32        d_f32;
    float32        tx_f32;
    float32        ty_f32;
    uint32         k_u32;
    boolean        flag_b = FALSE;
    uint32         loop_u32;

    z_f32 = 0.0f;
    theta_f32u32u.f32 = value;

    /* Comparison of value to zero */
    if (theta_f32u32u.u32 != 0x0UL)
    {
        x_f32 = 0.36875612707690253f;
        y_f32 = 0.0f;
        theta_f32u32u.f32 = value;
        val_f32 = 1.0f;
        neg_f32 = 1.0f;

        if (theta_f32u32u.f32 < 0.0f)
        {
            theta_f32u32u.f32 = -theta_f32u32u.f32;
            neg_f32 = -1.0f;
        }

        /* sin(45 degree) =  0.707106769 */
        if(theta_f32u32u.f32 > 0.707106769f)
        {
            /* sin^2(x) + cos^2(x) = 1 
               cos(x) = sqrt(1 - sin^2(x))
               asin(cos(x)) = asin(sin(90 - x)) = 90 - x */
            theta_f32u32u.f32 = Mfl_Sqrt_f32(1.0f - (theta_f32u32u.f32 * theta_f32u32u.f32));
            flag_b = TRUE;
        }

        /* Shift-Add algorithm */
        for (k_u32 = 0uL; k_u32 < 23uL; k_u32++)
        {
            loop_u32 = 2uL;

            while (loop_u32 > 0uL)
            {
                loop_u32--;

                if (y_f32 < theta_f32u32u.f32)
                {
                    d_f32 = 1.0f;
                }
                else
                {
                    d_f32 = -1.0f;
                }

                tx_f32 = x_f32 - (d_f32 * val_f32 * y_f32);
                ty_f32 = y_f32 + (d_f32 * val_f32 * x_f32);

                x_f32 = tx_f32;
                y_f32 = ty_f32;

                z_f32 = z_f32 + (d_f32 * Mfl_ATanTable_caf32[k_u32]);
            }

            val_f32 *= 0.5f;
        }

        if(flag_b)
        {
            /* z_s32 = (90 - x) */
            z_f32 = (1.57079637f - z_f32);
        }

        z_f32 *= neg_f32;
    }

    return z_f32;
}

#if (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ARCSIN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



