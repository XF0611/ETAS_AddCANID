


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

#if (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/**
 **********************************************************************************************************************
 * Returns the sinus and cosinus of a floating point number given in radians.
 *
 * \param   float32 value       The floating point value in range [-24PI,24PI] whose sine and cosine is to be returned.
 * \param   float32* s          Pointer to computed sine of the argument, in radians.
 * \param   float32* c          Pointer to computed cosine of the argument, in radians.
 * \return  void
 *
 **********************************************************************************************************************
 */
void Mfl_Prv_SinCos_f32(float32 value, float32* s, float32* c)
{

    float32 theta_f32, x_f32, y_f32, z_f32, val_f32, neg_f32, pi_sym_f32;
    float32 old_y_f32;
    uint32 k_u32, modulo_u32, pi_half_u32;

    theta_f32 = value;
    neg_f32 = 1.0f;
    pi_sym_f32 = 1.0f;
    pi_half_u32 = 0UL;

    if (theta_f32 < 0.0f)
    {
        theta_f32 = -theta_f32;
        neg_f32 = -1.0f;
    }

    /* periodic with 2PI -> compute sine of value modulo 2PI */
    /* temporary storage to x_f32*/
    x_f32 = (theta_f32 / (2.0f * 3.1415927f));
    modulo_u32 = ((uint32)x_f32);
    theta_f32 -= (((float32) modulo_u32) * 2.0f * 3.1415927f);

    /* symmtric in PI */
    /* temporary storage to x_f32*/
    x_f32 = theta_f32 / 3.1415927f;
    modulo_u32 = (uint32) (x_f32);
    if (modulo_u32 != 0UL)
    {
        pi_sym_f32 = -1.0f;
    }

    theta_f32 -= (((float32) modulo_u32) * 3.1415927f);

    /* precision regarding [0, PI/2] */
    if (theta_f32 > 1.5707963267948966f)
    {
        theta_f32 -= 1.5707963267948966f;
        pi_half_u32 = 1UL;
    }

    /* shift-add algorithm */
    x_f32 = 0.60725296f;  /* Kappa value */
    y_f32 = 0.0f;
    z_f32 = theta_f32;
    val_f32 = 1.0f;

    for (k_u32 = 0; k_u32 < 16uL; k_u32++)
    {
        float32 d_f32, tx_f32, ty_f32, tz_f32;

        if (z_f32 >= 0.0f)
        {
            d_f32 = 1.0f;
        }
        else
        {
            d_f32 = -1.0f;
        }

        tx_f32 = x_f32 - (d_f32 * val_f32 * y_f32);
        ty_f32 = y_f32 + (d_f32 * val_f32 * x_f32);
        tz_f32 = z_f32 - (d_f32 * Mfl_ATanTable_caf32[k_u32]);

        x_f32 = tx_f32;
        y_f32 = ty_f32;
        z_f32 = tz_f32;

        val_f32 *= 0.5f;
    }

    /* numerical refinement (Euler) */
    old_y_f32 = y_f32;

    if (pi_half_u32 > 0uL)
    {
        /* sin(\alpha \pm \pi\2) = \pm cos(\alpha) */
        y_f32 = x_f32 - (z_f32 * y_f32);

        /* cos(\alpha \pm \pi\2) = \mp cos(\alpha) */
        x_f32 = -(old_y_f32 + (z_f32 * x_f32));
    }
    else
    {
        y_f32 += z_f32 * x_f32;
        x_f32 -= z_f32 * old_y_f32;
    }

    /* djust symmtric value w.r.t. pi */
    y_f32 *= pi_sym_f32;
    x_f32 *= pi_sym_f32;

    /* sin(-x) = -sin(x) */
    y_f32 *= neg_f32;

    *s = y_f32;
    *c = x_f32;
}
/**
 **********************************************************************************************************************
 * Returns the tangent of a floating point number given in radians.
 *
 * \param   float32 value               The floating point value in range [-24PI,24PI] whose tangent is to be returned.
 * \return  float32                     The tangent of the argument, in radians.
 *
 **********************************************************************************************************************
 */
float32 Mfl_Tan_f32(float32 value)
{
    float32 sin_f32, cos_f32;

    Mfl_Prv_SinCos_f32(value, &sin_f32, &cos_f32);

    return (sin_f32 / cos_f32);
}

#if (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_TAN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



