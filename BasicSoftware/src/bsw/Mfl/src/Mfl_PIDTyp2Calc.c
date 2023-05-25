


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
 * Calculation service of PID Typ2 controller.
 *
 * Calculates the new internal state of PID Typ2 controller
 * Yn = Yn-1 + Kp * ((1 + (dT / Tn) + (Tv / dT)) * Xn - (1 + (2Tv / dT)) * Xn-1 + (Tv / dT) * Xn-2)
 * or rearranged:
 * Yn = Yn-1 + (Kp / dT) * ((dT + (dT * dT / Tn) + Tv) * Yn - (dT + 2Tv) * Xn-1 + Tv * Xn-2)
 *
 * The states Xn, Xn-1, Xn-2, Yn and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the paramters Xn, Xn-1, Xn-2, Yn and Yn-1.
 *
 * The state Yn
 * describes the current output value. This value has to be calculated by the differnce equation.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time and is hold by * State_cpst->Y1.
 * After calculation the old Yn-1 value is replaced by the current output value Yn.
 *
 * The state Xn
 * describes the current input value and is hold by the parameter X0_s32.
 *
 * The state Xn-1
 * describes the input value, one time step before the current time and is hold by State_cpst->X1.
 * After calculation the old Xn-1 value is replaced by the current input value Xn.
 *
 * The state Xn-2
 * describes the input value, two time steps before the current time and is hold by State_cpst->X2.
 * After calculation the old Xn-2 value is replaced by the value Xn-1.
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input state.
 * Kp is hold by a parameter structure of type Mfl_ParamPID, component Mfl_ParamPID->Kp_C.
 *
 * The parameter Tn,
 * physical unit [s] describes the rate time.
 * Tn is hold by a parameter structure of type Mfl_ParamPID, component Mfl_ParamPID->Tn_C.
 * Tn is given by a reciprocal value to avoid a divison in the implementation.
 *
 * The parameter Tv,
 * physical unit [s] describes the rate time
 * Tv is hold by a parameter structure of type Mfl_ParamPID, component Mfl_ParamPID->Tv_C.
 *
 * The parameter dT,
 * physical unit [s] describes the sample time
 * dT is hold by the variable dT_f32.
 * NOTE: dT > 0, otherwise, there is a zero divisor!
 *
 * \param   float32                         X_f32        Input value Xn
 * \param   Mfl_StatePID_Type*              State_cpst   Pointer to state structure
 * \param   const Mfl_ParamPID_Type*        Param_cpst   Pointer to parameter structure
 * \param   float32                         dT_f32       dT value
 * \return  void
 **********************************************************************************************************************
 */
#if (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PIDTyp2Calc(float32 X_f32, Mfl_StatePID_Type* State_cpst, const Mfl_ParamPID_Type* Param_cpst, float32 dT_f32)
{
    /* Calculate: Yn = Yn-1 + Kp * ((dT + (dT * dT / Tn) + Tv) * Xn - (dT + 2Tv) * Xn-1 + (Tv * Xn-2)) / dT */
    /* ---------------------------------------------------------------------------------------------------- */
    State_cpst->Y1 = State_cpst->Y1 +
                     (Param_cpst->K_C *
                      ((((((dT_f32 + ((dT_f32 * dT_f32) * Param_cpst->Tnrec_C)) +  Param_cpst->Tv_C) * X_f32) -
                       ((dT_f32 + (2.0F * Param_cpst->Tv_C)) * State_cpst->X1)) +
                       (Param_cpst->Tv_C * State_cpst->X2)) / dT_f32));

    /* save of the states */
    State_cpst->X2 = State_cpst->X1;
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PIDTYP2CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



