


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
 ***********************************************************************************************************************
 * Mfl_PITyp1LimCalc
 *
 * \brief Calculation service of PI Typ1 controller.
 *
 * Calculates the new internal state of PI Typ1 controller
 * Yn = Yn-1 + Kp * (Xn - (1 - (dT / Tn)) * Xn-1)
 *
 * \param   float32                        X_f32        Input value Xn
 * \param   Mfl_StatePI_Type*              State_cpst   Pointer to state structure
 * \param   const Mfl_ParamPI_Type*        Param_cpst   Pointer to parameter structure
 * \param   const Mfl_Limits_Type*         Limit_cpst   Pointer to limit structure
 * \param   float32                        dT_f32       dT value
 * \return  void
 ***********************************************************************************************************************
 */
#if (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PITyp1LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst,
                       const Mfl_Limits_Type* Limit_cpst, float32 dT_f32)
{
    Mfl_PITyp1Calc(X_f32, State_cpst, Param_cpst, dT_f32);

    State_cpst->Y1 = Mfl_Limit_f32(State_cpst->Y1, Limit_cpst->Min_C, Limit_cpst->Max_C);
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP1LIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



