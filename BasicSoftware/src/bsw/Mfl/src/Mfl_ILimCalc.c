


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
 ***************************************************************************************************
 * \brief Calculation service of limited I controller.
 *
 * Calculates the new internal state of limeted I controller
 * Yn = Yn-1 + Ki * dT * Xn-1
 *
 * The result of the difference equation i limited to a range [Limits_cpcst->Min, Limits_cpcst->Max]:
 * If Yn < Limits_cpcst->Min, the new state is set to Limits_cpcst->Min,
 * If Yn > Limits_cpcst->Max, the new state is set to Limits_cpcst->Max.

 * The states Xn-1, Yn and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the paramters Xn-1, Yn and Yn-1.
 *
 * The state Yn
 * describes the current output valu. This value has to be calculated by the differnce equation.
 *
 * The state Yn-1
 * describes the output value, at one time step before the current time.
 * State_cpst->Y1 holds Yn-1, scaled by the factor 2^16: *State_cpst->Y1 = 2^16 * Yn-1.
 * After calculation the old Yn-1 value is replaced by the current output value Yn.
 *
 * The state Xn-1
 * describes the input value, one time step before the current time.
 * State_cpst->X1 holds Xn-1, unscaled: *State_cpst->X1 = Xn-1
 * After calculation the old Xn-1 value is replaced by the current input value Xn.
 *
 * The parameter Ki,
 * physical unit [1/s] describes the amplification of the integral of the input state.
 * Ki is hold by K_f32.
 *
 * The parameter dT,
 * physical unit [s] describes the sample time
 * dT is hold by the variable dT_f32.
 *
 * \param   float32                       X_f32        input value Xn-1
 * \param   Mfl_StateI_Type*              State_cpst   pointer to state structure
 * \param   float32                       K_f32        value for Ki
 * \param   const Mfl_Limits_Type*        Limit_cpst   pointer to limit structure
 * \param   float32                       dT_f32       dT value
 * \return  void
 ***************************************************************************************************
 */
#if (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_ILimCalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, const Mfl_Limits_Type* Limit_cpst,
                  float32 dT_f32)
{
    /* Calculate: Yn = Yn-1 + Ki * dT * Xn-1 */
    Mfl_ICalc(X_f32, State_cpst, K_f32, dT_f32);

    /* Limitation */
    State_cpst->Y1 = Mfl_Limit_f32(State_cpst->Y1, Limit_cpst->Min_C, Limit_cpst->Max_C);
}

#if (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ILIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



