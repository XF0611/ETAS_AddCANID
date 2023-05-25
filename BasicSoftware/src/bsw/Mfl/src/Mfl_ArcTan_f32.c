


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#define SRVLIBS

#include "Mfl.h"


const float32 Mfl_ATanTable_caf32[] =
{
    0.785398163397448280f,
    0.463647609000806090f,
    0.244978663126864140f,
    0.124354994546761440f,
    0.062418809995957350f,
    0.0312398334302682770f,
    0.0156237286204768310f,
    0.00781234106010111110f,
    0.00390623013196697180f,
    0.00195312251647881880f,
    0.000976562189559319460f,
    0.000488281211194898290f,
    0.000244140620149361770f,
    0.000122070311893670210f,
    0.0000610351561742087730f,
    0.0000305175781155260960f,
    0.0000152587890613157620f,
    0.000007629394531101970f,
    0.00000381469726560649610f,
    0.0000019073486328101870f,
    0.000000953674316405960840f,
    0.000000476837158203088840f,
    0.000000238418579101557970f,
    0.000000119209289550780680f
};
/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Returns the arc tangent of a floating point number given in radians.
 *
 * \param   float32 value                  The floating point value whose arc tangent is to be returned.
 * \return  float32                        The arc tangent of the argument, in radians in the range of [-PI/2,PI/2].
 *
 **********************************************************************************************************************
 */
#if (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_ArcTan_f32(float32 value)
{
    Mfl_f32u32u    y_f32u32u;
    float32        x_f32;
    float32        z_f32;
    float32        theta_f32;
    float32        val_f32;
    float32        d_f32;
    float32        tx_f32;
    float32        ty_f32;
    float32        tz_f32;
    uint32         k_u32;

    y_f32u32u.f32 = value;

    /* Comparison of the input value value to zero */
    if (y_f32u32u.u32 == 0x0UL)
    {
        /* If the input is zero the input value will be returned */
        z_f32 = value;
    }
    else
    {
        x_f32 = 1.0f;
        z_f32 = 0.0f;
        theta_f32 = 0.0f;
        val_f32 = 1.0f;

        /* Shift-Add algorithm */
        for (k_u32 = 0uL; k_u32 < 23uL; k_u32++)
        {
            if (y_f32u32u.f32 < theta_f32)
            {
                d_f32 = 1.0f;
            }
            else
            {
                d_f32 = -1.0f;
            }

            tx_f32 = x_f32 - (d_f32 * val_f32 * y_f32u32u.f32);
            ty_f32 = y_f32u32u.f32 + (d_f32 * val_f32 * x_f32);
            tz_f32 = z_f32 - (d_f32 * Mfl_ATanTable_caf32[k_u32]);

            x_f32 = tx_f32;
            y_f32u32u.f32 = ty_f32;
            z_f32 = tz_f32;

            val_f32 *= 0.5f;
        }
    }

    return z_f32;
}

#if (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ARCTAN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



