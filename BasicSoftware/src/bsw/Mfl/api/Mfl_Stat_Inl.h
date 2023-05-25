


#ifndef MFL_STAT_INL_H
#define MFL_STAT_INL_H


/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * Mfl_Prv_Abs_f32_Inl
 * Mfl_Prv_ArrayAverage_f32_f32_Inl
 * Mfl_Prv_Average_f32_f32_Inl
 * Mfl_Prv_Max_f32_Inl
 * Mfl_Prv_Min_f32_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
 */

LOCAL_INLINE float32 Mfl_Prv_Abs_f32_Inl(float32 ValValue);
LOCAL_INLINE float32 Mfl_Prv_ArrayAverage_f32_f32_Inl(const float32* Array, uint32 Count);
LOCAL_INLINE float32 Mfl_Prv_Average_f32_f32_Inl(float32 value1, float32 value2);
LOCAL_INLINE float32 Mfl_Prv_Max_f32_Inl(float32 Value1, float32 Value2);
LOCAL_INLINE float32 Mfl_Prv_Min_f32_Inl(float32 Value1, float32 Value2);

/*
 **********************************************************************************************************************
 * Implementations
 **********************************************************************************************************************
 */

/**
 **********************************************************************************************************************
 * Mfl_Abs_f32
 *
 * \brief Returns the absulute value of an input value
 *
 * Returns the absulute value of an input value
 *
 * \param   float32  ValValue    First value for compare
 * \return  float32              Maximum value
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_ABS_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_Abs_f32_Inl(float32 ValValue)
{
    Mfl_f32u32u       res_f32u32u;

    res_f32u32u.f32 = ValValue;
    
    /* Gets the bits from the float */
    res_f32u32u.u32 &= 0x7FFFFFFFuL;

    return(res_f32u32u.f32);
}
#endif


/**
 ***********************************************************************************************************************
 * Mfl_ArrayAverage_f32_f32
 *
 * \brief Average calculation of Num values provided by array.
 *
 * \param    const* float32[]     Array    pointer to the float32, within the values X[i] to average
 * \param    uint32               Num      length of the array
 * \return   float32
 * \retval   sum(X[i]) / Num
 ***********************************************************************************************************************
 */
/* MR12 RULE 8.13 VIOLATION: The interface is defined with present prototye within AUTOSAR specification */
#ifndef MFL_PRV_ARRAYAVERAGE_F32_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_ArrayAverage_f32_f32_Inl(const float32* Array, uint32 Count)
{
    float32   tmp_f32;
    uint32    i;

    tmp_f32 = Array[0];

    if(Count > 1uL)
    {
        for(i = 1U; i < Count ; i++)
        {
            Array++;
            tmp_f32 += (*(Array));
        }

        /* MR12 RULE 10.4 VIOLATION: Calculation is done correctly. */
        tmp_f32 = tmp_f32 / ((float32)Count) ;
    }

    return (tmp_f32);
}
#endif

/**
 ***********************************************************************************************************************
 * Mfl_Average_f32_f32
 *
 * \brief The function calculates the average of two float32 operands.
 *
 * \param   float32  value1     first operand
 * \param   float32  value2     second operand
 * \return  float32
 * \retval  (value1 + value2) / 2
 ***********************************************************************************************************************
 */
#ifndef MFL_PRV_AVERAGE_F32_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_Average_f32_f32_Inl(float32 value1, float32 value2)
{
    return ((value1 + value2) / 2.0F);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_Max_f32
 *
 * \brief Returns the maximum of two float32 variables
 *
 * Returns the maximum of two float32 variables
 *
 * \param   float32  Value1       First value for compare
 * \param   float32  Value2       Second value for comparebase
 * \return  float32               Maximum value
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_MAX_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_Max_f32_Inl(float32 Value1, float32 Value2)
{
    return ((Value1 > Value2) ? Value1 : Value2);
}
#endif


/**
 **********************************************************************************************************************
 * Mfl_Min_f32
 *
 * \brief Returns the minimum of two float32 variables
 *
 * Returns the minimum of two float32 variables
 *
 * \param   float32  Value1       First value for compare
 * \param   float32  Value2       Second value for comparebase
 * \return  float32               Minimum value
 **********************************************************************************************************************
 */
#ifndef MFL_PRV_MIN_F32_INL_REMAPPED
LOCAL_INLINE float32 Mfl_Prv_Min_f32_Inl(float32 Value1, float32 Value2)
{
    return ((Value1 < Value2) ? Value1 : Value2);
}
#endif




/* MFL_STAT_INL_H */
#endif
