


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

/*** MFL_CFG_PIOUT_F32_LOCATION ***/
#if (MFL_CFG_PIOUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_PIOut_f32(const Mfl_StatePI_Type* State_cpst)
    {
        return (Mfl_Prv_PIOut_f32_Inl(State_cpst));
    }

    #if (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PIOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PIOUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PISETPARAM_LOCATION ***/
#if (MFL_CFG_PISETPARAM_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PISetParam(Mfl_ParamPI_Type* Param_cpst, float32 K_f32, float32 Tnrec)
    {
        Mfl_Prv_PISetParam_Inl(Param_cpst, K_f32, Tnrec);
    }

    #if (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PISETPARAM_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PISETPARAM_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_PISETSTATE_LOCATION ***/
#if (MFL_CFG_PISETSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_PISetState(Mfl_StatePI_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
    {
        Mfl_Prv_PISetState_Inl(State_cpst, X1_f32, Y1_f32);
    }

    #if (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_PISETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_PISETSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/

#if (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_PITyp1Calc(float32 X_f32, Mfl_StatePI_Type* State_cpst, const Mfl_ParamPI_Type* Param_cpst, float32 dT_f32)
{
    /* Calcualte: Yn = Yn-1 + Kp * (Xn - (1 - (dT / Tn)) * Xn-1) */
    /* --------------------------------------------------------- */
    State_cpst->Y1 += Param_cpst->K_C * (X_f32 - ((1.0F - (dT_f32 * Param_cpst->Tnrec_C)) * State_cpst->X1));

    /* save state */
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_PITYP1CALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



