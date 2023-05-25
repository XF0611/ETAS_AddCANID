


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
 * \brief Calculation service of an I controller.
 *
 * Calculates the new internal state of an I controller
 * Yn = Yn-1 + Ki * dT * Xn-1
 *
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
 * \param   float32                       dT_f32       dT value
 * \return  void
 ***************************************************************************************************
 */
#if (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

void Mfl_ICalc(float32 X_f32, Mfl_StateI_Type* State_cpst, float32 K_f32, float32 dT_f32)
{
    /* Calculate: Yn = Yn-1 + Ki * dT * Xn-1 */
    /* ------------------------------------- */
    State_cpst->Y1 = State_cpst->Y1 + (K_f32 * State_cpst->X1 * dT_f32);

    /* save state */
    State_cpst->X1 = X_f32;
}

#if (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_ICALC_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/

/*** MFL_CFG_IOUT_F32_LOCATION ***/
#if (MFL_CFG_IOUT_F32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    float32 Mfl_IOut_f32(const Mfl_StateI_Type* State_cpst)
    {
        return (Mfl_Prv_IOut_f32_Inl(State_cpst));
    }

    #if (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_IOUT_F32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_IOUT_F32_LOCATION ***/
#endif

/*********************************************************************************************************************/

/*** MFL_CFG_ISETSTATE_LOCATION ***/
#if (MFL_CFG_ISETSTATE_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    void Mfl_ISetState(Mfl_StateI_Type* State_cpst, float32 X1_f32, float32 Y1_f32)
    {
        Mfl_Prv_ISetState_Inl(State_cpst, X1_f32, Y1_f32);
    }

    #if (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_ISETSTATE_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_ISETSTATE_LOCATION ***/
#endif

/*********************************************************************************************************************/



