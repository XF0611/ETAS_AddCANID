


#ifndef MFL_CONTROLLER_INL_H
#define MFL_CONTROLLER_INL_H


/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * Mfl_Prv_CalcTeQ_f32_Inl
 * Mfl_Prv_CalcTeQApp_f32_Inl
 * Mfl_Prv_CtrlSetLimit_Inl
 * Mfl_Prv_DT1Typ1Out_f32_Inl
 * Mfl_Prv_DT1Typ1SetState_Inl
 * Mfl_Prv_DT1Typ2Out_f32_Inl
 * Mfl_Prv_DT1Typ2SetState_Inl
 * Mfl_Prv_IOut_f32_Inl
 * Mfl_Prv_ISetState_Inl
 * Mfl_Prv_PDOut_f32_Inl
 * Mfl_Prv_PDSetParam_Inl
 * Mfl_Prv_PDSetState_Inl
 * Mfl_Prv_PIDOut_f32_Inl
 * Mfl_Prv_PIDSetParam_Inl
 * Mfl_Prv_PIDSetState_Inl
 * Mfl_Prv_PIOut_f32_Inl
 * Mfl_Prv_PISetParam_Inl
 * Mfl_Prv_PISetState_Inl
 * Mfl_Prv_POut_f32_Inl
 * Mfl_Prv_PT1Out_f32_Inl
 * Mfl_Prv_PT1SetState_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE float32 Mfl_Prv_CalcTeQ_f32_Inl(float32 T1rec_f32, float32 dT_f32);
LOCAL_INLINE float32 Mfl_Prv_CalcTeQApp_f32_Inl(float32 T1rec_f32, float32 dT_f32);
LOCAL_INLINE void Mfl_Prv_CtrlSetLimit_Inl(Mfl_Limits_Type* Limit_cpst, float32 Min_f32, float32 Max_f32);
LOCAL_INLINE float32 Mfl_Prv_DT1Typ1Out_f32_Inl(const Mfl_StateDT1Typ1_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_DT1Typ1SetState_Inl(Mfl_StateDT1Typ1_Type* State_cpst, float32 X1_f32, float32 X2_f32,
                                              float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_DT1Typ2Out_f32_Inl(const Mfl_StateDT1Typ2_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_DT1Typ2SetState_Inl(Mfl_StateDT1Typ2_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_IOut_f32_Inl(const Mfl_StateI_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_ISetState_Inl(Mfl_StateI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_PDOut_f32_Inl(const Mfl_StatePD_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_PDSetParam_Inl(Mfl_ParamPD_Type* Param_cpst, float32 K_f32, float32 Tv_f32);
LOCAL_INLINE void Mfl_Prv_PDSetState_Inl(Mfl_StatePD_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_PIDOut_f32_Inl(const Mfl_StatePID_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_PIDSetParam_Inl(Mfl_ParamPID_Type* Param_cpst, float32 K_f32, float32 Tv_f32,
                                          float32 Tnrec_f32);
LOCAL_INLINE void Mfl_Prv_PIDSetState_Inl(Mfl_StatePID_Type* State_cpst, float32 X1_f32, float32 X2_f32, 
                                          float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_PIOut_f32_Inl(const Mfl_StatePI_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_PISetParam_Inl(Mfl_ParamPI_Type* Param_cpst, float32 K_f32, float32 Tnrec);
LOCAL_INLINE void Mfl_Prv_PISetState_Inl(Mfl_StatePI_Type* State_cpst, float32 X1_f32, float32 Y1_f32);
LOCAL_INLINE float32 Mfl_Prv_POut_f32_Inl(const float32* P_pf32);
LOCAL_INLINE float32 Mfl_Prv_PT1Out_f32_Inl(const Mfl_StatePT1_Type* State_cpst);
LOCAL_INLINE void Mfl_Prv_PT1SetState_Inl(Mfl_StatePT1_Type* State_cpst, float32 X1_f32, float32 Y1_f32);

/*
 **********************************************************************************************************************
 * Implementations
 **********************************************************************************************************************
 */


/**
 **********************************************************************************************************************
 *
 * Mfl_CalcTeQ_f32
 *
 * \brief Calculation of time equivalent TeQ.
 *
 * Calculates time equivalent: TeQ = exp (-dT / T1)
 *
 * The parameter dT
 * physical unit [s] describes the sample time
 * dT is hold by the variable dT_f32 and is allways > 0
 *
 * The parameter T1rec
 * physical unit [1/s] describes the reciprocal value of time constant T1
 * T1 has same unit and quantization like dT but value should be >> dT
 *
 * \param   float32   T1rec_f32    reciprocal value of T1
 * \param   float32   dT_f32       sample time dT
 * \return  float32
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_CALCTEQ_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_CalcTeQ_f32_Inl(float32 T1rec_f32, float32 dT_f32)
{
    return (Mfl_Exp_f32( -(T1rec_f32 * dT_f32)));
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_CalcTeQApp_f32
 *
 * \brief Calculation of approximation of time equivalent TeQ.
 *
 * Calculates approximation of time equivalent: TeQ = 1 - dT / T1
 *
 * The parameter dT
 * physical unit [s] describes the sample time
 * dT is hold by the variable dT_f32.
 *
 * The parameter T1rec
 * physical unit [1/s] describes the reciprocal value of time constant T1
 * T1 has same unit and quantization like dT
 *
 * \param   float32   T1rec_f32    reciprocal value of T1
 * \param   float32   dT_f32       sample time dT
 * \return  float32                calculated approximated time equivalent
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_CALCTEQAPP_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_CalcTeQApp_f32_Inl(float32 T1rec_f32, float32 dT_f32)
{
    return (1.0F - (dT_f32 * T1rec_f32));
}
#endif


/*
 **********************************************************************************************************************
 * Mfl_CtrlSetLimit
 *
 * \brief Sets the Limits vor the Limited Controllers
 *
 * \param   Mfl_Limits_Type*       Limit_cpst  pointer to the structure holding the scaled  min and max
 * \param   float32                Min_f32     unscaled minimum
 * \param   float32                Max_f32     unscaled maximum
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_CTRLSETLIMIT_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_CtrlSetLimit_Inl(Mfl_Limits_Type* Limit_cpst, float32 Min_f32, float32 Max_f32)
{
    Limit_cpst->Min_C = Min_f32;
    Limit_cpst->Max_C = Max_f32;
}
#endif

/**
 **********************************************************************************************************************
 * Mfl_DT1Typ1Out_f32
 *
 * \brief Gets the result of a DT1Typ1 controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StateDT1Typ1_Type*  State_cpst         pointer to state structure
 * \return  float32                                          unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DT1TYP1OUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_DT1Typ1Out_f32_Inl(const Mfl_StateDT1Typ1_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_DT1Typ1SetState
 *
 * \brief Initialises the state structure of a DT1 Type 1 controller.
 *
 * The states Xn-2, Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-2, Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 * The state Xn-2
 * describes the input value, twoe time steps before the current time.
 * State_cpst->X2 holds Xn-1
 *
 * \param   Mfl_StateDT1Typ1_Type*        State_cpst  pointer to state structure
 * \param   float32                       X1_f32      input value Xn-1, unscaled, two time step before current time
 * \param   float32                       X2_f32      input value Xn-2, unscaled, one time step before current time
 * \param   float32                       Y1_f32      input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DT1TYP1SETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_DT1Typ1SetState_Inl(Mfl_StateDT1Typ1_Type* State_cpst, float32 X1_f32, float32 X2_f32,
                                              float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->X2 = X2_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_DT1Typ2Out_f32
 *
 * \brief Gets the result of a DT1Typ2 controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StateDT1Typ2_Type*   State_cpst        pointer to state structure
 * \return  float32                                          unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DT1TYP2OUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_DT1Typ2Out_f32_Inl(const Mfl_StateDT1Typ2_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_DT1Typ2SetState
 *
 * \brief Initialises the state structure of a DT1 Type 2 controller.
 *
 * The states Xn-2, Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-2, Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 * The state Xn-2
 * describes the input value, twoe time steps before the current time.
 * State_cpst->X2 holds Xn-1
 *
 * \param   Mfl_StateDT1Typ2_Type*        State_cpst  pointer to state structure
 * \param   float32                       X1_f32      input value Xn-1, unscaled, two time step before current time
 * \param   float32                       Y1_f32      input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_DT1TYP2SETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_DT1Typ2SetState_Inl(Mfl_StateDT1Typ2_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_IOut_f32
 *
 * \brief Gets the result of an I controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StateI_Type *       State_cpst   pointer to state structure
 * \return  float32                                    unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_IOUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_IOut_f32_Inl(const Mfl_StateI_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif



/**
 **********************************************************************************************************************
 * Mfl_ISetState
 *
 * \brief Initialises the state structure of an I controller.
 *
 * The states Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 *
 * \param   Mfl_StateI_Type *       State_cpst   pointer to state structure
 * \param   float32                 X1_f32       input value Xn-1, unscaled, one time step before current time
 * \param   float32                 Y1_f32       input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_ISETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_ISetState_Inl(Mfl_StateI_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PDOut_f32
 *
 * \brief Gets the result of a PD controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StatePD_Type*        State_cpst     pointer to state structure
 * \return  float32                                      unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PDOUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_PDOut_f32_Inl(const Mfl_StatePD_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PDSetParam
 *
 * \brief Initialises the param structure of a PD controller.
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input state.
 * Kp is hold by a parameter structure, component Mfl_ParamPD_Type->Kp_C.
 *
 * The parameter Tv,
 * physical unit [s] describes the rate time
 * Tv is hold by a parameter structure, component Mfl_ParamPD_Type->Tv_C.
 * Tv is in the range of = [0,01; 100] * dT,  with respect to [Tv_min, Tv_max]
 *
 * \param   Mfl_ParamPD_Type *        Param_cpst   pointer to param structure
 * \param   float32                   K_f32        scaled input for the proportional amplification factor
 * \param   float32                   Tv_f32       scaled input for the time rate
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PDSETPARAM_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PDSetParam_Inl(Mfl_ParamPD_Type* Param_cpst, float32 K_f32, float32 Tv_f32)
{
    Param_cpst->K_C = K_f32;
    Param_cpst->Tv_C = Tv_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PDSetState
 *
 * \brief Initialises the state structure of a PD controller.
 *
 * The states Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 *
 * \param   Mfl_StatePD_Type *       State_cpst   pointer to state structure
 * \param   float32                  X1_f32       input value Xn-1, unscaled, one time step before current time
 * \param   float32                  Y1_f32       input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PDSETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PDSetState_Inl(Mfl_StatePD_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PIDOut_f32
 *
 * \brief Gets the result of a PID, either Type 1 or Type 2, controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StatePID_Type* State_cpst          pointer to state structure
 * \return  float32                                      unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PIDOUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_PIDOut_f32_Inl(const Mfl_StatePID_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PIDSetParam
 *
 * \brief Initialises the param structure of a PD controller.
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input state.
 * Kp is hold by a parameter structure, component Mfl_ParamPID_Type->Kp_C.
 *
 * The parameter Tn,
 * physical unit [s] describes the delay time.
 * Tn is hold by a parameter structure, component Mfl_ParamPID_Type->Tn_C.
 * Tn is in the range of  [0,01; 100] * dT, with respect to [Tn_min, Tn_max]
 * Tn is given by a reciprocal value to avoid a divison in the implementation.
 *
 * The parameter Tv,
 * physical unit [s] describes the rate time
 * Tv is hold by a parameter structure, component Mfl_ParamPID_Type->Tv_C.
 * Tv is in the range of = [0,01; 100] * dT, with respect to [Tv_min, Tv_max]
 *
 * \param   Mfl_ParamPID_Type *       Param_cpst   pointer to param structure
 * \param   float32                   K_f32        scaled input for the proportional amplification factor
 * \param   float32                   Tv_f32       scaled input for the time rate
 * \param   float32                   Tnrec_f32    scaled input for the delay time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PIDSETPARAM_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PIDSetParam_Inl(Mfl_ParamPID_Type* Param_cpst, float32 K_f32, float32 Tv_f32,
                                          float32 Tnrec_f32)
{
    Param_cpst->K_C = K_f32;
    Param_cpst->Tv_C = Tv_f32;
    Param_cpst->Tnrec_C = Tnrec_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PIDSetState
 *
 * \brief Initialises the state structure of a PID, either Type 1 or Type 2, controller.
 *
 * The states Xn-2, Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-2, Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 * The state Xn-2
 * describes the input value, twoe time steps before the current time.
 * State_cpst->X2 holds Xn-1
 *
 * \param   Mfl_StatePID_Type *       State_cpst   pointer to state structure
 * \param   float32                   X1_f32       input value Xn-2, unscaled, two time step before current time
 * \param   float32                   X2_f32       input value Xn-1, unscaled, one time step before current time
 * \param   float32                   Y1_f32       input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PIDSETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PIDSetState_Inl(Mfl_StatePID_Type* State_cpst, float32 X1_f32, float32 X2_f32,
                                          float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->X2 = X2_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PIOut_f32
 *
 * \brief Gets the result of a PI, either Type 1 or Type 2, controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StatePI_Type*         State_cpst   pointer to state structure
 * \return  float32                                      unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PIOUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_PIOut_f32_Inl(const Mfl_StatePI_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PISetParam
 *
 * \brief Initialises the param structure of a PI, either Type 1 or Type 2, controller.
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input state.
 * Kp is hold by a parameter structure, component Mfl_ParamPI_Type->Kp_C.
 *
 * The parameter Tn,
 * physical unit [s] describes the delay time.
 * Tn is hold by a parameter structure, component Mfl_ParamPI_Type->Tn_C.
 * Tn is in the range of  [0,01; 100] * dT, with respect to [Tn_min, Tn_max]
 * Tn is given by a reciprocal value to avoid a divison in the implementation.
 *
 * \param   Mfl_ParamPI_Type *         Param_cpst  pointer to param structure
 * \param   float32                    K_f32       scaled input for the proportional amplification factor
 * \param   float32                    Tnrec       scaled input for the delay time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PISETPARAM_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PISetParam_Inl(Mfl_ParamPI_Type* Param_cpst, float32 K_f32, float32 Tnrec)
{
    Param_cpst->K_C = K_f32;
    Param_cpst->Tnrec_C = Tnrec;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PISetState
 *
 * \brief Initialises the state structure of a PI either Type 1 or Type 2 controller.
 *
 * The states Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 *
 * \param   Mfl_StatePI_Type *        State_cpst   pointer to state structure
 * \param   float32                   X1_f32       input value Xn-1, unscaled, one time step before current time
 * \param   float32                   Y1_f32       input value Yn-1, unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PISETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PISetState_Inl(Mfl_StatePI_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_POut_f32
 *
 * \brief Gets the result of a P controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const float32*        P_ps32  pointer to state structure
 * \return  float32                       unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_POUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_POut_f32_Inl(const float32* P_pf32)
{
    return (*P_pf32);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PT1Out_f32
 *
 * \brief Gets the result of a PT1 controller.
 *
 * The state State_cpst->Y1 describes the output value, at one time step before the current time.
 *
 * \param   const Mfl_StatePT1_Type*        State_cpst   pointer to state structure
 * \return  float32                                      unscaled result of Y1 = state Yn-1
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PT1OUT_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_PT1Out_f32_Inl(const Mfl_StatePT1_Type* State_cpst)
{
    return (State_cpst->Y1);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_PT1SetState
 *
 * \brief Initialises the state structure of a PT1 controller.
 *
 * The states Xn-1 and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn-1 and Yn-1.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1
 *
 * \param   Mfl_StatePT1_Type*        State_cpst   pointer to state structure
 * \param   float32                   X1_f32       input value Xn-1, unscaled, one time step before current time
 * \param   float32                   Y1_f32       input value Yn-1 unscaled, one time step before calculation time
 * \return  void
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_PT1SETSTATE_INL_REMAPPED
LOCAL_INLINE void Mfl_Prv_PT1SetState_Inl(Mfl_StatePT1_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
{
    State_cpst->X1 = X1_f32;
    State_cpst->Y1 = Y1_f32;
}
#endif




/* MFL_CONTROLLER_INL_H */
#endif
