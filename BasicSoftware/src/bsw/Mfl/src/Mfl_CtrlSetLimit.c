


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

/*** MFL_CFG_CTRLSETLIMIT_LOCATION ***/
#if (MFL_CFG_CTRLSETLIMIT_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_CtrlSetLimit(Mfl_Limits_Type* Limit_cpst, float32 Min_f32, float32 Max_f32)
    {
        Mfl_Prv_CtrlSetLimit_Inl(Limit_cpst, Min_f32, Max_f32);
    }

    #if (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_CTRLSETLIMIT_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_CTRLSETLIMIT_LOCATION ***/
#endif

/*********************************************************************************************************************/



