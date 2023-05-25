


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
 * Mfl_IntToFloatCvrt_s32_f32
 *
 * \brief Converts integer to floating point value
 *
 * \param  sint32   ValInteger           Integer value to be converted.
 * \return float32                       Float value for the corresponding Integer input.
 **********************************************************************************************************************
 */
 
/*** MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION ***/
#if (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_IntToFloatCvrt_s32_f32(sint32 ValInteger)
    {
        return (Mfl_Prv_IntToFloatCvrt_s32_f32_Inl(ValInteger));
    }

    #if (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_INTTOFLOATCVRT_S32_F32_LOCATION ***/
#endif
    
/*********************************************************************************************************************/



