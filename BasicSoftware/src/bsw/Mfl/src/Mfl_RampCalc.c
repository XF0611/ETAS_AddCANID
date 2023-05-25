


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
 * Basic ramp functionality
 *
 * The ramp increases or decreases a value with slope * dT depending if State > Target or State < Target.
 *
 * \param   float32                          X        Input value for the ramp to reach
 * \param   Mfl_StateRamp_Type*              State_cpst   Actual value of the ramp
 * \param   const Mfl_ParamRamp_Type*        Param_cpcst  Pointer to structure with slope values
 * \param   float32                          dT_f32       Delay time dT
 * \return  void                                          No return value
 **********************************************************************************************************************
 */
#if (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_RampCalc(float32 X_f32, Mfl_StateRamp_Type* State_cpst, const Mfl_ParamRamp_Type* Param_cpcst, float32 dT_f32)
{
    Mfl_f32u32u    tmp1_f32u32u;
    Mfl_f32u32u    tmp2_f32u32u;
    
    /* Input value is below end value */
    if (State_cpst->State < X_f32)
    {
        if (Param_cpcst->SlopePos > 0.0F)
        {
            State_cpst->Dir = MFL_RAMPDIR_RISING;

            /* Calculation of Slope * dT */
            State_cpst->State += Param_cpcst->SlopePos * dT_f32;
            State_cpst->State  = Mfl_Prv_Min_f32_Inl(State_cpst->State, X_f32);
        }
    }
    else
    {
        if (State_cpst->State > X_f32)
        {
            if (Param_cpcst->SlopeNeg > 0.0F)
            {
                State_cpst->Dir = MFL_RAMPDIR_FALLING;

                /* Calculation of Slope * dT */
                State_cpst->State -= Param_cpcst->SlopeNeg * dT_f32;
                State_cpst->State  = Mfl_Prv_Max_f32_Inl(State_cpst->State, X_f32);
            }
        }

    }

    tmp1_f32u32u.f32 = State_cpst->State;
    tmp2_f32u32u.f32 = X_f32;
    
    /* Input value is equal to new end value */
    if (tmp1_f32u32u.u32 == tmp2_f32u32u.u32)
    {
        State_cpst->Dir = MFL_RAMPDIR_END;
    }
}

#if (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_RAMPCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPCALCJUMP_LOCATION ***/
#if (MFL_CFG_RAMPCALCJUMP_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_RampCalcJump(float32 X_f32, Mfl_StateRamp_Type* State_cpst)
    {
        Mfl_Prv_RampCalcJump_Inl(X_f32, State_cpst);
    }

    #if (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCALCJUMP_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPCALCJUMP_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPCALCSWITCH_LOCATION ***/
#if (MFL_CFG_RAMPCALCSWITCH_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_RampCalcSwitch(float32 Xa_f32, float32 Xb_f32, boolean Switch_B, Mfl_StateRamp_Type* State_cpst)
    {
        return (Mfl_Prv_RampCalcSwitch_Inl(Xa_f32, Xb_f32, Switch_B, State_cpst));
    }

    #if (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCALCSWITCH_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPCALCSWITCH_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPCHECKACTIVITY_LOCATION ***/
#if (MFL_CFG_RAMPCHECKACTIVITY_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    boolean Mfl_RampCheckActivity(Mfl_StateRamp_Type* State_cpst)
    {
        return (Mfl_Prv_RampCheckActivity_Inl(State_cpst));
    }

    #if (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPCHECKACTIVITY_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPCHECKACTIVITY_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPGETSWITCHPOS_LOCATION ***/
#if (MFL_CFG_RAMPGETSWITCHPOS_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    boolean Mfl_RampGetSwitchPos(Mfl_StateRamp_Type* State_cpst)
    {
        return (Mfl_Prv_RampGetSwitchPos_Inl(State_cpst));
    }

    #if (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPGETSWITCHPOS_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPGETSWITCHPOS_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPINITSTATE_LOCATION ***/
#if (MFL_CFG_RAMPINITSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_RampInitState(Mfl_StateRamp_Type* State_cpst, float32 Val_f32)
    {
        Mfl_Prv_RampInitState_Inl(State_cpst, Val_f32);
    }

    #if (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPINITSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPINITSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPOUT_F32_LOCATION ***/
#if (MFL_CFG_RAMPOUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_RampOut_f32(const Mfl_StateRamp_Type* State_cpcst)
    {
        return (Mfl_Prv_RampOut_f32_Inl(State_cpcst));
    }

    #if (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPOUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_RAMPSETPARAM_LOCATION ***/
#if (MFL_CFG_RAMPSETPARAM_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_RampSetParam(Mfl_ParamRamp_Type* Param_cpst, float32 SlopePosVal_f32, float32 SlopeNegVal_f32)
    {
        Mfl_Prv_RampSetParam_Inl(Param_cpst, SlopePosVal_f32, SlopeNegVal_f32);
    }

    #if (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_RAMPSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_RAMPSETPARAM_LOCATION ***/
#endif

/*********************************************************************************************************************/



