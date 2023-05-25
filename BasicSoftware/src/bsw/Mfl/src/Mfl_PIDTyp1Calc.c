


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

/*** MFL_CFG_PIDOUT_F32_LOCATION ***/
#if (MFL_CFG_PIDOUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_PIDOut_f32(const Mfl_StatePID_Type* State_cpst)
    {
        return (Mfl_Prv_PIDOut_f32_Inl(State_cpst));
    }

    #if (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PIDOUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PIDSETPARAM_LOCATION ***/
#if (MFL_CFG_PIDSETPARAM_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PIDSetParam(Mfl_ParamPID_Type* Param_cpst, float32 K_f32, float32 Tv_f32, float32 Tnrec_f32)
    {
        Mfl_Prv_PIDSetParam_Inl(Param_cpst, K_f32, Tv_f32, Tnrec_f32);
    }

    #if (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PIDSETPARAM_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PIDSETSTATE_LOCATION ***/
#if (MFL_CFG_PIDSETSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PIDSetState(Mfl_StatePID_Type* State_cpst, float32 X1_f32, float32 X2_f32, float32 Y1_f32)
    {
        Mfl_Prv_PIDSetState_Inl(State_cpst, X1_f32, X2_f32, Y1_f32);
    }

    #if (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PIDSETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PIDSETSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/

#if (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PIDTyp1Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32)
{
    /* Calculate: Yn = Yn-1 + Kp * ((dT + Tv) * Xn - (dT - dT*dT/Tn + 2Tv) * Xn-1 + (Tv * Xn-2)) / dT */
    /* ---------------------------------------------------------------------------------------------- */
    State_cpst->Y1 = State_cpst->Y1 +
                     (Param_cpst->K_C *
                      (((((dT_f32 + Param_cpst->Tv_C) * X_f32) -
                       (((dT_f32 - ((dT_f32 * dT_f32) * Param_cpst->Tnrec_C)) + 
                         (2.0F * Param_cpst->Tv_C)) * State_cpst->X1)) + 
                         (Param_cpst->Tv_C  * State_cpst->X2)) / dT_f32));

    /* save the states */
    State_cpst->X2 = State_cpst->X1;
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PIDTYP1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



