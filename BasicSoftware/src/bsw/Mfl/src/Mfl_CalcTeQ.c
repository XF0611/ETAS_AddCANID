


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

/*** MFL_CFG_CALCTEQ_F32_LOCATION ***/
#if (MFL_CFG_CALCTEQ_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_CalcTeQ_f32(float32 T1rec_f32, float32 dT_f32)
    {
        return (Mfl_Prv_CalcTeQ_f32_Inl(T1rec_f32, dT_f32));
    }

    #if (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_CALCTEQ_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_CALCTEQ_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_CALCTEQAPP_F32_LOCATION ***/
#if (MFL_CFG_CALCTEQAPP_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_CalcTeQApp_f32(float32 T1rec_f32, float32 dT_f32)
    {
        return (Mfl_Prv_CalcTeQApp_f32_Inl(T1rec_f32, dT_f32));
    }

    #if (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_CALCTEQAPP_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_CALCTEQAPP_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/



