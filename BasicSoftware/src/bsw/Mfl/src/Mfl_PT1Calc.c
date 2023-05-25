


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
 * Calculation service of PT1 controller.
 *
 * Calculates the new internal state of PT1 controller
 * Yn = exp(- dT/T1) * Yn-1 + Kp * (1 - exp (-dT/T1)) * Xn-1
 * exp(- dT/T1) is the time equivalent TeQ
 *
 * \param   float32                   X_f32       Input value Xn
 * \param   Mfl_StatePT1_Type*        State_cpst  Pointer to state structure
 * \param   float32                   K_f32       Value for Kp
 * \param   float32                   TeQ_f32     Time equivalent value
 * \return  void
 **********************************************************************************************************************
 */
#if (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PT1Calc(float32 X_f32, Mfl_StatePT1_Type* State_cpst, float32 K_f32, float32 TeQ_f32)
{
    /* Calculate Yn = TeQ * Yn-1 + Kp * (1 - TeQ) * Xn-1 */
    /* ------------------------------------------------- */
    State_cpst->Y1 = (TeQ_f32 * State_cpst->Y1) + (K_f32 * (1.0F - TeQ_f32) * State_cpst->X1);

    /* save state */
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PT1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/

/*** MFL_CFG_PT1OUT_F32_LOCATION ***/
#if (MFL_CFG_PT1OUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_PT1Out_f32(const Mfl_StatePT1_Type* State_cpst)
    {
        return (Mfl_Prv_PT1Out_f32_Inl(State_cpst));
    }

    #if (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PT1OUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PT1OUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PT1SETSTATE_LOCATION ***/
#if (MFL_CFG_PT1SETSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PT1SetState(Mfl_StatePT1_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
    {
        Mfl_Prv_PT1SetState_Inl(State_cpst, X1_f32, Y1_f32);
    }

    #if (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PT1SETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PT1SETSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/



