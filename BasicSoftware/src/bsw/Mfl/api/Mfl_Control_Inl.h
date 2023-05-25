


#ifndef MFL_CONTROL_INL_H
#define MFL_CONTROL_INL_H


/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * Mfl_Prv_DebounceInit_Inl
 * Mfl_Prv_DebounceSetParam_Inl
 * Mfl_Prv_HystCenterHalfDelta_f32_u8_Inl
 * Mfl_Prv_HystDeltaRight_f32_u8_Inl
 * Mfl_Prv_HystLeftDelta_f32_u8_Inl
 * Mfl_Prv_HystLeftRight_f32_u8_Inl
 * Mfl_Prv_RampCalcJump_Inl
 * Mfl_Prv_RampCalcSwitch_Inl
 * Mfl_Prv_RampCheckActivity_Inl
 * Mfl_Prv_RampGetSwitchPos_Inl
 * Mfl_Prv_RampInitState_Inl
 * Mfl_Prv_RampOut_f32_Inl
 * Mfl_Prv_RampSetParam_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE void Mfl_Prv_DebounceInit_Inl(Mfl_DebounceState_Type* State, boolean X);
LOCAL_INLINE void Mfl_Prv_DebounceSetParam_Inl(Mfl_DebounceParam_Type* Param, float32 THighLow, float32 TLowHigh);
LOCAL_INLINE boolean Mfl_Prv_HystCenterHalfDelta_f32_u8_Inl(float32 X, float32 center, float32 halfDelta, uint8* State);
LOCAL_INLINE boolean Mfl_Prv_HystDeltaRight_f32_u8_Inl(float32 X, float32 Delta, float32 Rsp, uint8* State);
LOCAL_INLINE boolean Mfl_Prv_HystLeftDelta_f32_u8_Inl(float32 X, float32 Lsp, float32 Delta, uint8* State);
LOCAL_INLINE boolean Mfl_Prv_HystLeftRight_f32_u8_Inl(float32 X, float32 Lsp, float32 Rsp, uint8* State);
LOCAL_INLINE void Mfl_Prv_RampCalcJump_Inl(float32 X_f32, Mfl_StateRamp_Type*  State_cpst);
LOCAL_INLINE float32 Mfl_Prv_RampCalcSwitch_Inl(float32 Xa_f32, float32 Xb_f32, boolean Switch_B,
                                                Mfl_StateRamp_Type*  State_cpst);
LOCAL_INLINE boolean Mfl_Prv_RampCheckActivity_Inl(Mfl_StateRamp_Type*  State_cpst);
LOCAL_INLINE boolean Mfl_Prv_RampGetSwitchPos_Inl(Mfl_StateRamp_Type*  State_cpst);
LOCAL_INLINE void Mfl_Prv_RampInitState_Inl(Mfl_StateRamp_Type*  State_cpst, float32 Val_f32);
LOCAL_INLINE float32 Mfl_Prv_RampOut_f32_Inl(const Mfl_StateRamp_Type*  State_cpcst);
LOCAL_INLINE void Mfl_Prv_RampSetParam_Inl(Mfl_ParamRamp_Type*  Param_cpst, float32 SlopePosVal_f32,
                                           float32 SlopeNegVal_f32);

/*
 **********************************************************************************************************************
 * Implementations
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Initialization of internal state of service Mfl_Debounce_u8_u8
 *
 * \param   Mfl_DebounceState_Type *  State_cpst           pointer of state structure
 * \param   boolean X                                      Value for old X value
 * \return  none
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DEBOUNCEINIT_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_DebounceInit_Inl(Mfl_DebounceState_Type* State, boolean X)
{
    State->Timer = 0.0F;
    State->XOld  = X;
}
#endif


/**
 **********************************************************************************************************************
 * Initialization of parameters of service Mfl_Debounce_u8_u8
 *
 * \param   Mfl_DebounceParam_Type *     Param_cpcst       Pointer to parameter structure
 * \param   float32 THighLow                               Time value for high-low switch of boolean signal
 * \param   float32 TLowHigh                               Time value for low-high switch of boolean signal
 * \return  none
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DEBOUNCESETPARAM_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_DebounceSetParam_Inl(Mfl_DebounceParam_Type* Param, float32 THighLow, float32 TLowHigh)
{
    Param->TimeLowHigh = TLowHigh;
    Param->TimeHighLow = THighLow;
}
#endif

/**
 ***********************************************************************************************************************
 * Mfl_HystCenterHalfDelta_f32_u8
 *
 * \brief Hysteresis 32bit where switching points are delta/2 from center
 *
 * Hysteresis with center and left and right side halfdelta switching point.
 *
 * Return value is TRUE if input is greater then center plus halfdelta switching point.
 * Return value is FALSE if input is less then center minus halfdelta switching point.
 * Return value is previous state value if input is in the range of halfDelta around the center
 * switching point.
 *
 * \param     float32   X            Input value
 * \param     float32   center       Center of hysteresis range
 * \param     float32   halfDelta    Half width of hysteresis range
 * \param     uint8*    State        Pointer to internal state
 * \return    boolean                Returns TRUE or FALSE depending on input value and state.
 ***********************************************************************************************************************
 */
#ifndef MFL_PRV_HYSTCENTERHALFDELTA_F32_U8_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_HystCenterHalfDelta_f32_u8_Inl(float32 X, float32 center, float32 halfDelta, uint8* State)
{
    return Mfl_Prv_HystLeftRight_f32_u8_Inl(X, center - halfDelta, center + halfDelta, State);
}
#endif


/**
 ***********************************************************************************************************************
 * Mfl_HystDeltaRight_f32_u8
 *
 * \brief Hysteresis 32bit with right switching point and delta to left switching point
 *
 * Hysteresis with right switching point and delta to left switching point.
 *
 * Return value is TRUE if input is greater then right switching point.
 * Return value is FALSE if input is less then right switching point minus delta.
 * Return value is previous state value if input is between right switching points and right minus delta.
 *
 * \param     float32   X            Input value
 * \param     float32   Delta        Left switching point = rsp - delta
 * \param     float32   Rsp          Right switching point
 * \param     uint8*    State        Pointer to internal state
 * \return    boolean                Returns TRUE or FALSE depending on input value and state.
 ***********************************************************************************************************************
 */
#ifndef MFL_PRV_HYSTDELTARIGHT_F32_U8_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_HystDeltaRight_f32_u8_Inl(float32 X, float32 Delta, float32 Rsp, uint8* State)
{
    return Mfl_Prv_HystLeftRight_f32_u8_Inl(X, Rsp - Delta, Rsp, State);
}
#endif


/**
 ***********************************************************************************************************************
 * Mfl_HystLeftDelta_f32_u8
 *
 * \brief Hysteresis 32bit with left switching point and delta to right switching point
 *
 * Hysteresis with left switching point and delta to right switching point.
 *
 * Return value is TRUE if input is greater then left switching point plus delta.
 * Return value is FALSE if input is less then left switching point.
 * Return value is previous state value if input is between left switching points and left plus delta.
 *
 * \param     float32  X             Input value
 * \param     float32  Lsp           Left switchig point
 * \param     float32  Delta         Right switching point = lsp + delta
 * \param     uint8*   State         Pointer to internal state
 * \return    boolean                Returns TRUE or FALSE depending on input value and state.
 ***********************************************************************************************************************
 */
#ifndef MFL_PRV_HYSTLEFTDELTA_F32_U8_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_HystLeftDelta_f32_u8_Inl(float32 X, float32 Lsp, float32 Delta, uint8* State)
{
    return Mfl_Prv_HystLeftRight_f32_u8_Inl(X, Lsp, Lsp + Delta, State);
}
#endif


/**
 ***********************************************************************************************************************
 * Mfl_HystLeftRight_f32_u8
 *
 * \brief Hysteresis 32bit with left and right switching point
 *
 * Hysteresis with left and right switching point.
 *
 * Return value is TRUE if input is greater then right switching point.
 * Return value is FALSE if input is less then left switching point.
 * Return value is previous state value if input is between left and right switching points.
 *
 * \param     float32  X             Input value
 * \param     float32  Lsp           Left switching point
 * \param     float32  Rsp           Right switching point
 * \param     uint8*   State         Pointer to internal state
 * \return    boolean                Returns TRUE or FALSE depending on input value and state.
 ***********************************************************************************************************************
 */
#ifndef MFL_PRV_HYSTLEFTRIGHT_F32_U8_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_HystLeftRight_f32_u8_Inl(float32 X, float32 Lsp, float32 Rsp, uint8* State)
{
    if(X > Rsp)
    {
        *State = 1u;
    }
    else
    {
        if (X < Lsp)
        {
            *State = 0u;
        }
        /* No else: Previous state will be returned */
    }

    return (*State > 0u);
}
#endif


/**
 **********************************************************************************************************************
 * Activation service for Jump Functionality of ramp functionality
 *
 * Activates Jump Functionality. Must be called before ramp calc service is called.
 *
 * \param   float32 X                             Input value X
 * \param   Mfl_StateRamp_t *       State_cpst    Pointer to status structure of ramp functionality
 * \return  float32                               None
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RAMPCALCJUMP_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_RampCalcJump_Inl(float32 X_f32, Mfl_StateRamp_Type* State_cpst)
{
    float32 delta_f32;

    delta_f32 = X_f32 - State_cpst->State;

    if (((delta_f32 < 0.0F) && (State_cpst->Dir == MFL_RAMPDIR_RISING)) ||
        ((delta_f32 > 0.0F) && (State_cpst->Dir == MFL_RAMPDIR_FALLING)))
    {
        State_cpst->State = X_f32;
        State_cpst->Dir   = MFL_RAMPDIR_END;
    }
}
#endif


/**
 **********************************************************************************************************************
 * Activation service for Switch Functionality of ramp functionality
 *
 * Activates Switch Functionality. Must be called before ramp calc service is called.
 *
 * \param   float32 Xa_f32                        Value for input A
 * \param   float32 Xb_f32                        Value for input B
 * \param   boolean Switch_B                      Switch between input A and B
 * \param   Mfl_StateRamp_t*       State_cpst     Pointer to status structure of ramp functionality
 * \return  float32                               Return value, is input A or B based on parameter Switch_b
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RAMPCALCSWITCH_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_RampCalcSwitch_Inl(float32 Xa_f32, float32 Xb_f32, boolean Switch_B,
                                                Mfl_StateRamp_Type* State_cpst)
{
    float32 X;

    if (Switch_B != FALSE)
    {
        X = Xa_f32;

        if (State_cpst->Switch != MFL_RAMPSWITCH_XA)
        {
            State_cpst->Dir    = MFL_RAMPDIR_END;
            State_cpst->Switch = MFL_RAMPSWITCH_XA;
        }
    }
    else
    {
        X = Xb_f32;

        if (State_cpst->Switch != MFL_RAMPSWITCH_XB)
        {
            State_cpst->Dir    = MFL_RAMPDIR_END;
            State_cpst->Switch = MFL_RAMPSWITCH_XB;
        }
    }

    return (X);
}
#endif

/**
 **********************************************************************************************************************
 * Check if ramp target is reached
 *
 * Mfl_RampCheckActivity checks if ramp service has finished ramping to target value or ramp service is still working.
 *
 * \param   Mfl_StateRamp_t *       State_cpst   Pointer to state of ramp service
 * \return  boolean                              TRUE: Target is reached, FALSE: Target is not reached
 **********************************************************************************************************************
 */

/* MR12 RULE 8.13 VIOLATION: The interface is defined with present prototye within AUTOSAR specification */
#ifndef MFL_PRV_RAMPCHECKACTIVITY_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_RampCheckActivity_Inl(Mfl_StateRamp_Type* State_cpst)
{
    return ((boolean)(State_cpst->Dir != MFL_RAMPDIR_END) ? TRUE : FALSE);
}
#endif


/**
 **********************************************************************************************************************
 * Get information which input value (A or B) will currently be reached from ramp switch functionality
 *
 * Current information will be given readout from flags of ramp switch service
 *
 * \param   Mfl_StateRamp_t * State_cpst   Pointer to flags of ramp switch service
 * \return  boolean                        TRUE: Ramp is switched to input A, FALSE: Ramp is switched to input B
 **********************************************************************************************************************
 */
/* MR12 RULE 8.13 VIOLATION: The interface is defined with present prototye within AUTOSAR specification */
#ifndef MFL_PRV_RAMPGETSWITCHPOS_INL_REMAPPED
LOCAL_INLINE boolean Mfl_Prv_RampGetSwitchPos_Inl(Mfl_StateRamp_Type* State_cpst)
{
    return ((boolean)(State_cpst->Switch > 0) ? TRUE : FALSE);
}
#endif


/**
 **********************************************************************************************************************
 * Setting initial value of internal states of ramp functionality
 *
 * With Mfl_RampInitState initial values of states structure can be set for ramp functionality
 *
 * \param   Mfl_StateRamp_t *       State_cpst   Pointer to state structure of ramp functionality
 * \param   float32 Val_f32                      Initial value to be set
 * \return  void                                 None
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RAMPINITSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_RampInitState_Inl(Mfl_StateRamp_Type* State_cpst, float32 Val_f32)
{
    State_cpst->State  = Val_f32;
    State_cpst->Dir    = MFL_RAMPDIR_END;
    State_cpst->Switch = MFL_RAMPSWITCH_XA;
}
#endif


/**
 **********************************************************************************************************************
 * Out service for ramp functionality
 *
 * Returns the float32 value of ramp functionality.
 *
 * \param   const Mfl_StateRamp_t *       State_cpst  Pointer to status structure of ramp functionality
 * \return  float32                                   None
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RAMPOUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_RampOut_f32_Inl(const Mfl_StateRamp_Type* State_cpcst)
{
    return (State_cpcst->State);
}
#endif


/**
 **********************************************************************************************************************
 * Setting positive and negative slope of ramp functionality
 *
 * With Mfl_RampSetParam positive and negative slope values can be set to parameter structure.
 *
 *
 * \param   Mfl_ParamRamp_t * Param_pcst         Boolean input value
 * \param   float32 SlopeValPos_f32              Positive slope value to be set
 * \param   float32 SlopeValNeg_f32              Negative slope value to be set
 * \return  void                                 None
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_RAMPSETPARAM_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_RampSetParam_Inl(Mfl_ParamRamp_Type* Param_cpst, float32 SlopePosVal_f32,
                                       float32 SlopeNegVal_f32)
{
    Param_cpst->SlopePos = SlopePosVal_f32;
    Param_cpst->SlopeNeg = SlopeNegVal_f32;
}
#endif




/* MFL_CONTROL_INL_H */
#endif
