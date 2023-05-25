


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

#define SRVLIBS

#include "Mfl.h"


const float32 Mfl_LogTable_caf32[] =
{
    0.693147182464599610f,
    0.40546509623527527000f,
    0.223143547773361210f,
    0.117783032357692720f,
    0.06062462180852890f,
    0.0307716578245162960f,
    0.0155041869729757310f,
    0.00778214028105139730f,
    0.00389864039607346060f,
    0.00195122009608894590f,
    0.000976085953880101440f,
    0.000488162069814279680f,
    0.00024411082267761230f,
    0.000122062861919403080f,
    0.0000610332936048507690f,
    0.0000305171124637126920f,
    0.0000152586726471781730f,
    0.00000762936542741954330f,
    0.00000381468998966738580f,
    0.00000190734681382309650f,
    0.000000953673861658899110f,
    0.000000476837044516287280f,
    0.000000238418550679853070f
};
/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
 */


/**
 **********************************************************************************************************************
 * Returns the natural (base-e) logarithm of the operand.
 *
 * \param   float32 value            Floating-point operand for log computation. value shall be passed as > 0.0 value.
 * \return  float32                  Natural (base-e) logarithm of the operand.
 *
 **********************************************************************************************************************
 */
#if (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_Log_f32(float32 ValValue)
{
    float32 t_f32, e_f32, v_f32, x_f32, reciprocal_f32;
    float32 val_f32;
    uint32 k_u32;

    x_f32 = ValValue;
    reciprocal_f32 = 1.0f;

    if (x_f32 < 1.0f)
    {
        reciprocal_f32 = -1.0f;
        x_f32 = 1.0f / x_f32;
    }

    /* shift-add algorithm */
    t_f32 = 0.0f;
    e_f32 = 1.0f;
    v_f32 = 1.0f;

    for (k_u32 = 1; k_u32 < 22uL; k_u32++)
    {
        v_f32 *= 0.5f;
        val_f32 = e_f32 + (e_f32 * v_f32);

        /* MR12 RULE 14.1 VIOLATION:Floating point comparison is required in order ensure the correct 
        behavior of the function */
        while (val_f32 <= x_f32)
        {
            t_f32 += Mfl_LogTable_caf32[k_u32];
            e_f32 = val_f32;
            val_f32 = e_f32 + (e_f32 * v_f32);
        }
    }

    t_f32 *= reciprocal_f32;

    return t_f32;
}

#if (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_LOG_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/





