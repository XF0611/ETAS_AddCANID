


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
 * Calculation service of limited PI Typ2 controller.
 *
 * Calculates the new internal state of limited PI Typ2 controller
 * Yn = Yn-1 + Kp * ((1 + (dT / Tn) * Xn - Xn-1)
 * Yn is limited to min/max values
 *
 * \param   float32                        X0_f32       Input value Xn
 * \param   Mfl_StatePI_Type*              State_cpst   Pointer to state structure
 * \param   const Mfl_ParamPI_Type*        Param_cpst   Pointer to parameter structure
 * \param   const Mfl_Limits_Type*         Limit_cpst   Pointer to limit structure
 * \param   float32                        dT_f32       dT value
 * \return  void
 **********************************************************************************************************************
 */
#if (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PITyp2LimCalc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, 
                       const Mfl_Limits_Type* Limit_cpst, float32 dT_f32)
{
    Mfl_PITyp2Calc(X_f32, State_cpst, Param_cpst, dT_f32);

    State_cpst->Y1 = Mfl_Limit_f32(State_cpst->Y1, Limit_cpst->Min_C, Limit_cpst->Max_C);
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP2LIMCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



