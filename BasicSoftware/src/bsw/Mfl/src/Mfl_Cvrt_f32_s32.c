


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
 * Mfl_Cvrt_f32_s32
 *
 * \brief Converts float to fixed point value
 *
 * \param   float32   ValFloat           Floating-point quantity to be converted
 * \param   sint16    ValFixedExponent   Exponent of the fixed-point result of the conversion
 * \return  sint32                       Integer value of the fixed-point result
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

sint32 Mfl_Cvrt_f32_s32(float32 ValFloat, sint16 ValFixedExponent)
{
    Mfl_f32u32u    tmp_f32u32u;
    sint32         res_s32;
    uint32         res_u32;
    uint8          Is_Negative_u8;
    uint32         temp_u32;
    
    tmp_f32u32u.f32 = ValFloat;
    
    /* Get Sign bit of ValFloat */
    Is_Negative_u8 = (uint8) (tmp_f32u32u.u32 >> 31u);
    
    /* Absolute IEEE754 float representation */
    tmp_f32u32u.u32 = tmp_f32u32u.u32 & 0x7FFFFFFFUL;
    
    res_u32 = Mfl_Cvrt_f32_u32(tmp_f32u32u.f32, ValFixedExponent);
    
    /* Check for Overflow and Underflow */
    if(res_u32 > (uint32) MFL_MAXSINT32)
    {
        /* Saturation of the result to MAXSINT32 if Is_Negative_u8 is 0 and to 
           MINSINT32 if Is_Negative_u8 is 1 */
        temp_u32 = ( 0x7FFFFFFFuL + (uint32) Is_Negative_u8);
        res_s32 = (sint32)temp_u32;
    }
    else
    {
        if(Is_Negative_u8 == 1u)
        {
            res_s32 = (-( (sint32) res_u32));
        }
        else
        {
            res_s32 = (sint32) res_u32;
        }
    }
   
    return (res_s32);
}

#if (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_S32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



