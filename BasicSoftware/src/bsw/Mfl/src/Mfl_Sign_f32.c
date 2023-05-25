


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
 * Mfl_Sign_f32
 *
 * Returns the sign of the argument (ValSign), determined according to the following equation. 
 * ValSign =  1, ValValue > 0.0 
 * ValSign =  0, ValValue == 0.0 
 * ValSign = -1, ValValue < 0.0
 *
 * \param   float32   ValValue     Floating-point operand.
 * \return  sint8                  Integer representing the sign of the operand
 **********************************************************************************************************************
 */
#if (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

sint8 Mfl_Sign_f32(float32 ValValue)
{
    if( ValValue > 0.0F)
    {
        return(1);
    }
    else if ( ValValue < 0.0F)
    {
        return(-1);
    }
    else
    {
        return(0);
    }
}

#if (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_SIGN_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



