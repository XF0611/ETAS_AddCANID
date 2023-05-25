


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
 * Mfl_IntToFloatCvrt_u8_f32
 *
 * \brief Converts integer to floating point value
 *
 * \param  uint8    ValInteger           Integer value to be converted.
 * \return float32                       Float value for the corresponding Integer input.
 **********************************************************************************************************************
 */

/*** MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION ***/
#if (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_IntToFloatCvrt_u8_f32(uint8 ValInteger)
    {
        return(Mfl_IntToFloatCvrt_u32_f32(ValInteger));
    }

    #if (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_INTTOFLOATCVRT_U8_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/



