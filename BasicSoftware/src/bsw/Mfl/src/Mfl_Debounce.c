


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

#if (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/* MR12 RULE 8.13 VIOLATION: The interface is defined with present prototye within AUTOSAR specification */
boolean Mfl_Debounce_u8_u8(boolean X, Mfl_DebounceState_Type* State, Mfl_DebounceParam_Type* Param, float32 dT)
{
    float32 timerval_f32;

    /* Check if input state is changed */
    if(X == State->XOld)
    {
        /* Input is not changed, reset timer */
        State->Timer = 0.0F;
    }
    else
    {
        /* Input is changed, check debounce via timer value */

        timerval_f32 = (float32)((X != FALSE) ?  Param->TimeLowHigh : Param->TimeHighLow);

        if(State->Timer >= timerval_f32)
        {
            State->XOld  = X;
            State->Timer = 0.0F;
        }
        else
        {
            State->Timer = State->Timer + dT;
        }
    }

    /* Return internal state = input value */
    return State->XOld;
}

#if (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_DEBOUNCE_U8_U8_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/

/*** MFL_CFG_DEBOUNCEINIT_LOCATION ***/
#if (MFL_CFG_DEBOUNCEINIT_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_DebounceInit(Mfl_DebounceState_Type* State, boolean X)
    {
        Mfl_Prv_DebounceInit_Inl(State, X);
    }

    #if (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_DEBOUNCEINIT_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_DEBOUNCEINIT_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_DEBOUNCESETPARAM_LOCATION ***/
#if (MFL_CFG_DEBOUNCESETPARAM_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_DebounceSetParam(Mfl_DebounceParam_Type* Param, float32 THighLow, float32 TLowHigh)
    {
        Mfl_Prv_DebounceSetParam_Inl(Param, THighLow, TLowHigh);
    }

    #if (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_DEBOUNCESETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_DEBOUNCESETPARAM_LOCATION ***/
#endif

/*********************************************************************************************************************/



