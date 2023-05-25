


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
 * Mfl_FloatToIntCvrt_f32_u32
 *
 * \brief Converts float to fixed point value
 *
 * \param  float32   ValFloat           Floating-point value to be converted
 * \return uint32                       Integer value for the corresponding floating point input.
 **********************************************************************************************************************
 */
/*** MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION ***/
#if (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION != MFL_CFG_LOCATION_INLINE)
    #if (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_START_SEC_CODE
    #elif (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_START_SEC_CODE_SLOW
    #elif (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_START_SEC_CODE_FAST
    #else
        #define MFL_START_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"

    uint32 Mfl_FloatToIntCvrt_f32_u32(float32 ValFloat)
    {
        /*** MFL_CFG_OPTIMIZATION ***/
        #if ((MFL_CFG_OPTIMIZATION == MFL_CFG_TCCOMMON) || (MFL_CFG_OPTIMIZATION == MFL_CFG_TC27XX))
            return (rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u32_Inl(ValFloat));
        #else
            return (Mfl_Prv_FloatToIntCvrt_f32_u32_Inl(ValFloat));
        /*** MFL_CFG_OPTIMIZATION ***/
        #endif
    }

    #if (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_NORMAL)
        #define MFL_STOP_SEC_CODE
    #elif (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_SLOW)
        #define MFL_STOP_SEC_CODE_SLOW
    #elif (MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION == MFL_CFG_LOCATION_FAST)
        #define MFL_STOP_SEC_CODE_FAST
    #else
        #define MFL_STOP_SEC_CODE
    #endif
    #include "Mfl_MemMap.h"
/*** MFL_CFG_FLOATTOINTCVRT_F32_U32_LOCATION ***/
#endif
/*********************************************************************************************************************/



