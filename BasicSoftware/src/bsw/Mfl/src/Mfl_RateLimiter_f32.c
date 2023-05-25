


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

/*** MFL_CFG_RATELIMITER_F32_LOCATION ***/
#if (MFL_CFG_RATELIMITER_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_RateLimiter_f32(float32 newval, float32 oldval, float32 maxdif)
    {
        return (Mfl_Prv_RateLimiter_f32_Inl(newval, oldval, maxdif));
    }

    #if (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RATELIMITER_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RATELIMITER_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/



