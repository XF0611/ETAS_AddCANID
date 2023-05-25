


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

/*** MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION ***/
#if (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    boolean Mfl_HystLeftDelta_f32_u8(float32 X, float32 Lsp, float32 Delta, uint8* State)
    {
        return (Mfl_Prv_HystLeftDelta_f32_u8_Inl(X, Lsp, Delta, State));
    }

    #if (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_HYSTLEFTDELTA_F32_U8_LOCATION ***/
#endif

/*********************************************************************************************************************/



