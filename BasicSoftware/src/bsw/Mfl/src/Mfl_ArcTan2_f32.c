


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
 * Returns the arc tangens of an angle, in the range of [-PI, PI].
 *
 * \param   float32 x                 Input value 1
 * \param   float32 y                 Input value 2
 * \return  float32                   Returns arc tangent for inputs x & y
 **********************************************************************************************************************
 */
#if (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_ArcTan2_f32(float32 x, float32 y)
{
	#if (MFL_ENABLED_AR_VERSION == MFL_AR42)
    Mfl_f32u32u    tmp_f32u32u;
        float32        z_f32;
        float32        tmp_f32;


        tmp_f32u32u.f32 = x;

        /* MR12 DIR 1.1 VIOLATION: Comparision is needed. This check could be replaced if a service is available */
        if (tmp_f32u32u.u32 == 0x0UL)
        {
            /* If the x coordinate is zero, then check
                if(y > 0.0f) then return pi / 2
                if(y = 0.0f) then return zero
                if(y < 0.0f) then return - pi / 2 */

            if(y > 0.0f)
            {
                z_f32 = 1.5707964f;
                return z_f32;
            }
            else if (y < 0.0f)
            {
                z_f32 = - 1.5707964f;
                return z_f32;
            }
            else
            {
                return 0.0f;
            }
        }
        else
        {
            /* Implementation of the ArcTan2 functionality:
               Z = y / x
               if (|Z| < 1) Then
                 Result = Z / (1.0 + (0.28 * Z^2))
                 if(x < 0.0f) then Result = (y < 0.0f) ? Result - pi : Result + pi
               Else
                 Result = (pi / 2) - (Z / (Z^2 + 0.28))
                 if ( y < 0.0f ) Result = Result - PI; */

            z_f32 = y / x;
            tmp_f32 = z_f32 * z_f32;

            if ((Mfl_Abs_f32( z_f32 )) < 1.0f)
            {
                z_f32 = z_f32 / (1.0f + (0.28f * tmp_f32));
                if(x < 0.0f)
                {
                    z_f32 = ((y < 0.0f) ? (z_f32 - 3.1415927f) : (z_f32 + 3.1415927f));
                }
            }
            else
            {
                z_f32 = 1.5707964f - (z_f32 / (tmp_f32 + 0.28f));
                if(y < 0.0f)
                {
                    z_f32 = z_f32 - 3.1415927f;
                }
            }
        }

        return z_f32;
	#elif (MFL_ENABLED_AR_VERSION == MFL_AR44)
    Mfl_f32u32u    tmp_f32u32u;
    float32        z_f32;
    float32        tmp_f32;


    tmp_f32u32u.f32 = x;

    /* MR12 DIR 1.1 VIOLATION: Comparision is needed. This check could be replaced if a service is available */
    if (tmp_f32u32u.u32 == 0x0UL)
    {
        /* If the x coordinate is zero, then check
            if(y > 0.0f) then return pi / 2
            if(y = 0.0f) then return zero
            if(y < 0.0f) then return - pi / 2 */

        if(y > 0.0f)
        {
            return MFL_POS_HALFPI;
        }
        else if (y < 0.0f)
        {
            return MFL_NEG_HALFPI;
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {
        /* Implementation of the ArcTan2 functionality:
           Z = y / x
           if (|Z| < 1) Then
             Result = Z / (1.0 + (0.28 * Z^2))
             if(x < 0.0f) then Result = (y < 0.0f) ? Result - pi : Result + pi
           Else
             Result = (pi / 2) - (Z / (Z^2 + 0.28))
             if ( y < 0.0f ) Result = Result - PI; */

        z_f32 = y / x;
        tmp_f32 = z_f32 * z_f32;

        if ((Mfl_Abs_f32( z_f32 )) < 1.0f)
        {
            z_f32 = z_f32 / (1.0f + (0.28f * tmp_f32));
            if(x < 0.0f)
			{
                z_f32 = ((y < 0.0f) ? (z_f32 - MFL_PI) : (z_f32 + MFL_PI));
			}
        }
        else
        {
            z_f32 = MFL_POS_HALFPI - (z_f32 / (tmp_f32 + 0.28f));
            if(y < 0.0f)
			{
                z_f32 = z_f32 - MFL_PI;
			}
        }

        return z_f32;
    }
    #endif
}

#if (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ARCTAN2_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



