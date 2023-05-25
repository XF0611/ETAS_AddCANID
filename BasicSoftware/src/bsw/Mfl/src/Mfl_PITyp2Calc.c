


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
 * Mfl_PITyp2Calc
 *
 * \brief Calculation service of PI Type 2 controller.
 *
 * Calculates the new internal state of PI Type 2 controller
 * Yn = Yn-1 + Kp * ((1 + (dT / Tn)) * Xn) - Xn-1)
 *
 * The states Xn, Xn-1, Yn and Yn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn, Xn-1, Yn and Yn-1.
 *
 * The parameter Yn-1,
 * Yn-1 describes the output value, at a time one time step before the current time
 * The Structure *State_cpst->Y1 holds Yn-1
 *
 * The parameter Xn,
 * Xn describes the current input value and is hold by the paramter X0_f32.
 *
 * The parameter Xn-1,
 * Xn-1 describes the input value, one time step before the current time
 * The Structure *State_cpst->X1 holds Xn-1.
 * Past calculation of Yn, the old Xn-1 value is replaced by the current input value Xn.
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input
 * Kp is stored in the paramter structure of type Mfl_ParamI, component Mfl_ParamPI->Kp_C.
 *
 * The parameter Tn,
 * physical unit [s] describes the rate time
 * Tn is stored in the paramter structure of type Mfl_ParamPI.
 * Tn is a divisor in discretisation. To avoid a divison in the implementation, Tn is  stored by a reciprocal value.
 *
 * The parameter dT,
 * physical unit [s] describes the sample time and is hold by the variable dT_f32.
 * NOTE: dT > 0, otherwise, there is a zero divisor!
 *
 * \param   float32                        X_f32        Input value Xn
 * \param   Mfl_StatePI_Type*              State_cpst   Pointer to state structure
 * \param   const Mfl_ParamPI_Type*        Param_cpst   Pointer to parameter structure
 * \param   float32                        dT_f32       dT value
 * \return  void
 **********************************************************************************************************************
 */
#if (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PITyp2Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32)
{
    /* Calculate: Yn = Yn-1 + Kp * ((1 + (dT / Tn)) * Xn) - Xn-1) */
    /* ---------------------------------------------------------- */
    State_cpst->Y1 += Param_cpst->K_C * (((1.0F + (dT_f32 * Param_cpst->Tnrec_C)) * X_f32) - State_cpst->X1);

    /* save state */
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP2CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



