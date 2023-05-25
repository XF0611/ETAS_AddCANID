


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
 * Mfl_Cvrt_f32_u16
 *
 * \brief Converts float to fixed point value
 *
 * \param  float32  ValFloat           Floating-point quantity to be converted
 * \param  sint16   ValFixedExponent   Exponent of the fixed-point result of the conversion
 * \return uint16                      Integer value of the fixed-point result
 **********************************************************************************************************************
 */
#if (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

uint16 Mfl_Cvrt_f32_u16(float32 ValFloat, sint16 ValFixedExponent)
{
    uint32 res_u32;
    
    res_u32 = Mfl_Cvrt_f32_u32(ValFloat, ValFixedExponent);
    
    /* Limiting the result to uint16 */
    if(res_u32 > MFL_MAXUINT16_U)
    {
        res_u32 = MFL_MAXUINT16_U;
    }
    
    return((uint16)res_u32);
}

#if (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_CVRT_F32_U16_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



