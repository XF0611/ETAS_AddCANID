


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

/*** MFL_CFG_AVERAGE_F32_F32_LOCATION ***/
#if (MFL_CFG_AVERAGE_F32_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_Average_f32_f32(float32 value1, float32 value2)
    {
        return (Mfl_Prv_Average_f32_f32_Inl(value1, value2));
    }

    #if (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_AVERAGE_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_AVERAGE_F32_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/



