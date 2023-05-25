


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
 * Mfl_DeadTime_f32_f32
 *
 * \brief This routine returns input value with specified delay time
 *
 * calculation of dsintStatic_w as:
 * dsint = dsint + deltadsint with deltadsint = time slot / dead time of the actual X value
 *
 * \Param       X           Input value,        float32
 * \Param       DelayTime   Time to be delayed, float32
 * \Param       StepTime    Sample time,        float32
 * \Param       Param       Parameterstructure
 * \return                  Returns the actual state of the dead time element as sint16 value
 * \seealso     Mfl_DeadTimeParam_Type
 * \usedresources
 ***************************************************************************************************
 */
#if (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_START_SEC_CODE
#elif (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_START_SEC_CODE_SLOW
#elif (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_START_SEC_CODE_FAST
#else
    #define MFL_START_SEC_CODE
#endif
#include "Mfl_MemMap.h"

float32 Mfl_DeadTime_f32_f32(float32 X, float32 DelayTime, float32 StepTime, Mfl_DeadTimeParam_Type* Param)
{
    float32 totalTime_f32;
    
    /* The totalTime_f32 is the sum of last time and the delaytime */
    totalTime_f32 = Param->dsintStatic + DelayTime;

    /* As long as totalTime_f32 is greater than the step time, ring buffer is active */
    /* MR12 RULE 14.1 VIOLATION:Floating point comparison is required in order ensure the correct behavior of 
    the function */
    while(totalTime_f32 > StepTime)
    {
        /* Store the x_value in the location pointed Ptr */
        *(Param->lszStatic) = X;
        Param->lszStatic--;
        
        /* If the pointer has reached the lower buffer border, it is set to the upper border
           If not, the pointer is decremented */
        if (Param->lszStatic < Param->dtbufBegStatic)
        {
            Param->lszStatic = Param->dtbufEndStatic;
        }
        
        /* Decrement totalTime_f32 by steptime */
        totalTime_f32 = totalTime_f32 - StepTime;
    }

    /* Save the actual time */
    Param->dsintStatic = totalTime_f32;

    /* Return the actual pointer */
    return(*(Param->lszStatic));
}

#if (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_NORMAL)
    #define MFL_STOP_SEC_CODE
#elif (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_SLOW)
    #define MFL_STOP_SEC_CODE_SLOW
#elif (MFL_CFG_DEADTIME_F32_F32_LOCATION == MFL_CFG_LOCATION_FAST)
    #define MFL_STOP_SEC_CODE_FAST
#else
    #define MFL_STOP_SEC_CODE
#endif
#include "Mfl_MemMap.h"

/*********************************************************************************************************************/



