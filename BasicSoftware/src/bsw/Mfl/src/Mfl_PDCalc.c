


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
 * Mfl_PDCalc.c
 *
 * \brief Calculation service of PD controller.
 *
 * Calculates the new internal state of PD controller
 * Yn = Kp * ((1 + Tv / dT) * Xn - (Tv / dT) * Xn-1)
 *
 * The rearranged equation used in the implementation becomes to:
 * Yn = Kp * (Xn + (Tv * (Xn - Xn-1) / dT))
 *
 * The states Xn and Xn-1 have an arbitrary physical unit.
 * But, the unit have to be identical for the states Xn, Xn-1, Xn-2, Yn and Yn-1.
 *
 * The paramter Yn,
 * Yn describes the current output value
 *
 * The paramter Yn-1,
 * Yn-1 describes the output value, at a time one time step before the current time
 * State_cpst->Y1 hold Yn-1
 *
 * The parameter Xn,
 * X0 describes the current input value
 *
 * The parameter Xn-1,
 * Xn-1 describes the input value, one time step before the current time
 * State_cpst->X1 holds Xn-1
 * Past calculation the old Xn-1 value is replaced by the current input value Xn
 *
 * The parameter Kp,
 * physical unit [s] describes the amplification of of input
 * Kp is stored in the paramter structure of type Mfl_ParamPD.
 *
 * The parameter Tv,
 * physical unit [s] describes the rate time
 * Tv is stored in the paramter structure of type Mfl_ParamPD.
 *
 * The parameter dT,
 * physical unit [s] describes the sample time
 *
 * \param   float32                        X_f32       Input value Xn
 * \param   Mfl_StatePD_Type*              State_cpst  Pointer to state structure
 * \param   const Mfl_ParamPD_Type*        Param_cpst  Pointer to parameter structure
 * \param   float32                        dT_f32      dT value
 * \return  void
 ***************************************************************************************************
 */
#if (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PDCalc(float32 X_f32, Mfl_StatePD_Type* State_cpst, const Mfl_ParamPD_Type* Param_cpst, float32 dT_f32)
{
    /* Calculate Yn = Kp * (Xn + (Tv * (Xn - Xn-1) / dT)) */
    /* -------------------------------------------------- */
    State_cpst->Y1 = Param_cpst->K_C * (X_f32 + ((Param_cpst->Tv_C * (X_f32 - State_cpst->X1)) / dT_f32));

    /* save state */
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PDCALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/

/*** MFL_CFG_PDOUT_F32_LOCATION ***/
#if (MFL_CFG_PDOUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_PDOut_f32(const Mfl_StatePD_Type* State_cpst)
    {
        return (Mfl_Prv_PDOut_f32_Inl(State_cpst));
    }

    #if (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PDOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PDOUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PDSETPARAM_LOCATION ***/
#if (MFL_CFG_PDSETPARAM_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PDSetParam(Mfl_ParamPD_Type* Param_cpst, float32 K_f32, float32 Tv_f32)
    {
        Mfl_Prv_PDSetParam_Inl(Param_cpst, K_f32, Tv_f32);
    }

    #if (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PDSETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PDSETPARAM_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PDSETSTATE_LOCATION ***/
#if (MFL_CFG_PDSETSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PDSetState(Mfl_StatePD_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
    {
        Mfl_Prv_PDSetState_Inl(State_cpst, X1_f32, Y1_f32);
    }

    #if (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PDSETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PDSETSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/



