


#ifndef MFL_CONVERSION_INL_H
#define MFL_CONVERSION_INL_H
/*
 **********************************************************************************************************************
 *
 * List of services
 *Mfl_Prv_FloatToIntCvrt_f32_s16_Inl
 *Mfl_Prv_FloatToIntCvrt_f32_s32_Inl
 *Mfl_Prv_FloatToIntCvrt_f32_s8_Inl
 *Mfl_Prv_FloatToIntCvrt_f32_u16_Inl
 *Mfl_Prv_FloatToIntCvrt_f32_u32_Inl
 *Mfl_Prv_FloatToIntCvrt_f32_u8_Inl
 *Mfl_Prv_FloatToIntCvrt_signed_Inl
 *Mfl_Prv_FloatToIntCvrt_unsigned_Inl
 *Mfl_Prv_IntToFloatCvrt_s32_f32_Inl
 *Mfl_Prv_IntToFloatCvrt_u32_f32_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Hash defines
**********************************************************************************************************************
 */

#define MFL_128F_BITPATTERN             0x43000000UL
#define MFL_256F_BITPATTERN             0x43800000UL
#define MFL_32768F_BITPATTERN           0x47000000UL
#define MFL_65536F_BITPATTERN           0x47800000UL
#define MFL_2147483648F_BITPATTERN      0x4f000000UL
#define MFL_4294967296F_BITPATTERN      0x4f800000UL


 
 
/*
 **********************************************************************************************************************
 * Prototypes
**********************************************************************************************************************
 */


LOCAL_INLINE sint16 Mfl_Prv_FloatToIntCvrt_f32_s16_Inl(float32 ValFloat);
LOCAL_INLINE sint32 Mfl_Prv_FloatToIntCvrt_f32_s32_Inl(float32 ValFloat);
LOCAL_INLINE sint8 Mfl_Prv_FloatToIntCvrt_f32_s8_Inl(float32 ValFloat);
LOCAL_INLINE uint16 Mfl_Prv_FloatToIntCvrt_f32_u16_Inl(float32 ValFloat);
LOCAL_INLINE uint32 Mfl_Prv_FloatToIntCvrt_f32_u32_Inl(float32 ValFloat);
LOCAL_INLINE uint8 Mfl_Prv_FloatToIntCvrt_f32_u8_Inl(float32 ValFloat);
LOCAL_INLINE sint32 Mfl_Prv_FloatToIntCvrt_signed_Inl(float32 ValFloat, uint32 FloatBitPattern, sint32 ValMin, sint32 ValMax);
LOCAL_INLINE uint32 Mfl_Prv_FloatToIntCvrt_unsigned_Inl(float32 ValFloat, uint32 FloatBitPattern, uint32 ValMax);

LOCAL_INLINE float32 Mfl_Prv_IntToFloatCvrt_s32_f32_Inl(sint32 ValInteger);
LOCAL_INLINE float32 Mfl_Prv_IntToFloatCvrt_u32_f32_Inl(uint32 ValInteger);



/*
 **********************************************************************************************************************
 * Implementations
**********************************************************************************************************************
 */
 



LOCAL_INLINE sint16 Mfl_Prv_FloatToIntCvrt_f32_s16_Inl(float32 ValFloat)
{
    sint16 retVal_s16;
    
    retVal_s16 = (sint16)Mfl_Prv_FloatToIntCvrt_signed_Inl(ValFloat, MFL_32768F_BITPATTERN, MFL_MINSINT16, MFL_MAXSINT16);
    
    return (retVal_s16);
}

/*********************************************************************************************************************/
 
LOCAL_INLINE sint32 Mfl_Prv_FloatToIntCvrt_f32_s32_Inl(float32 ValFloat)
{
    sint32 retVal_s32;
    
    retVal_s32 = Mfl_Prv_FloatToIntCvrt_signed_Inl(ValFloat, MFL_2147483648F_BITPATTERN, MFL_MINSINT32, MFL_MAXSINT32);
    
    return (retVal_s32);
}

/*********************************************************************************************************************/

LOCAL_INLINE sint8 Mfl_Prv_FloatToIntCvrt_f32_s8_Inl(float32 ValFloat)
{
    sint8 retVal_s8;
    
    retVal_s8 = (sint8)Mfl_Prv_FloatToIntCvrt_signed_Inl(ValFloat, MFL_128F_BITPATTERN, MFL_MINSINT8, MFL_MAXSINT8);
    
    return (retVal_s8);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint16 Mfl_Prv_FloatToIntCvrt_f32_u16_Inl(float32 ValFloat)
{
    uint16 retVal_u16;
    
    retVal_u16 = (uint16)Mfl_Prv_FloatToIntCvrt_unsigned_Inl(ValFloat, MFL_65536F_BITPATTERN, MFL_MAXUINT16);
    
    return (retVal_u16);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint32 Mfl_Prv_FloatToIntCvrt_f32_u32_Inl(float32 ValFloat)
{
    uint32 retVal_u32;
    
    retVal_u32 = Mfl_Prv_FloatToIntCvrt_unsigned_Inl(ValFloat, MFL_4294967296F_BITPATTERN, MFL_MAXUINT32);
    
    return (retVal_u32);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint8 Mfl_Prv_FloatToIntCvrt_f32_u8_Inl(float32 ValFloat)
{
    uint8 retVal_u8;
    
    retVal_u8 = (uint8)Mfl_Prv_FloatToIntCvrt_unsigned_Inl(ValFloat, MFL_256F_BITPATTERN, MFL_MAXUINT8);
    
    return (retVal_u8);
}

/*********************************************************************************************************************/

LOCAL_INLINE sint32 Mfl_Prv_FloatToIntCvrt_signed_Inl(float32 ValFloat, uint32 FloatBitPattern, sint32 ValMin, sint32 ValMax)
{
    /* Implemented behavior: Return 0 for NaN and saturate to ValMin/ValMax for exceeding range */
    sint32 retVal_s32;
    Mfl_f32u32u in_f32u32u;
    Mfl_f32u32u inUnsigned_f32u32u;
    
    in_f32u32u.f32 = ValFloat;
    inUnsigned_f32u32u.f32 = ValFloat;
    
    /* Do the range checks on the bit pattern of the float value, without the sign bit */
    inUnsigned_f32u32u.u32 &= 0x7FFFFFFFUL;
    /* First case: Value is representable in the given range. */
    if (inUnsigned_f32u32u.u32 < FloatBitPattern)
    {
        retVal_s32 = (sint32)ValFloat;
    }
    /* Second case: Value is too big, saturation necessary.
       0x7F800000UL as float bit pattern means +INF */
    else if (inUnsigned_f32u32u.u32 <= 0x7F800000UL)
    {
        retVal_s32 = ((in_f32u32u.u32>>31) > 0UL) ? ValMax : ValMin;
    }
    /* Third case: Value is NaN */
    else
    {
        retVal_s32 = 0L;
    }
    return (retVal_s32);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint32 Mfl_Prv_FloatToIntCvrt_unsigned_Inl(float32 ValFloat, uint32 FloatBitPattern, uint32 ValMax)
{
    /* Implemented behavior: Return 0 for NaN and saturate to 0/ValMax for exceeding range */
    uint32 retVal_u32;
    Mfl_f32u32u in_f32u32u;
    in_f32u32u.f32 = ValFloat;
    
    retVal_u32 = 0UL;
    /* Do the range checks on the bit pattern of the float value */
    /* Only proceed if the sign bit is not set */
    if (in_f32u32u.u32 < 2147483648UL)
    {
        /* First case: Value is representable in the given range */
        if (in_f32u32u.u32 < FloatBitPattern)
        {
            retVal_u32 = (uint32)ValFloat;
        }
        /* Second case: Value is too big, saturation necessary.
           0x7F800000UL as float bit pattern means +INF */
        else if (in_f32u32u.u32 <= 0x7F800000UL)
        {
            retVal_u32 = ValMax;
        }
        /* Third case: Value is NaN */
        else
        {
            /* retVal_u32 already assigned */
        }
    }
    return (retVal_u32);
}

/*********************************************************************************************************************/

LOCAL_INLINE float32 Mfl_Prv_IntToFloatCvrt_s32_f32_Inl(sint32 ValInteger)
{
#if (MFL_CFG_HW_ROUNDING == MFL_CFG_ON)
    float32 retVal_f32;
    
    retVal_f32 = (float32)ValInteger;
    
    return(retVal_f32);
#else
    Mfl_f32u32u    res_f32u32u;
    uint32         tmp_u32;
    uint32         sign_u32 = 0UL;
    
    /* Remove the sign of ValInteger */
    if(ValInteger < 0L)
    {
        sign_u32 = 1UL;
        
        /* Check for negative boundary value */
        if(ValInteger == MFL_MINSINT32)
        {
            tmp_u32 = 0x80000000uL;
        }
        else
        {
            tmp_u32 = (uint32)(-ValInteger);
        }
    }
    else
    {
        tmp_u32 = (uint32)(ValInteger);
    }
    
    res_f32u32u.f32 = Mfl_Prv_IntToFloatCvrt_u32_f32_Inl(tmp_u32);
    
    /* Place the sign bit at the 31st bit position */
    res_f32u32u.u32 = res_f32u32u.u32 | (sign_u32 << 31u);
    
    return(res_f32u32u.f32);
/*** MFL_CFG_HW_ROUNDING ***/
#endif
}

/*********************************************************************************************************************/
 
LOCAL_INLINE float32 Mfl_Prv_IntToFloatCvrt_u32_f32_Inl(uint32 ValInteger)
{
#if (MFL_CFG_HW_ROUNDING == MFL_CFG_ON)
	float32 retVal_f32;
	
	retVal_f32 = (float32)ValInteger;
    
	return (retVal_f32);
#else
    uint32 tmp_u32;
    uint32 mant_u32;
    sint32 exp_s32;
    uint8 count_u8 = 0U;
    uint32 disbits_u32;
    uint32 tievalue_u32;
    Mfl_f32u32u res_f32u32u;
    res_f32u32u.u32 = 0UL;

    /* If ValInteger is 0x0, then the float value will always be zero */
    if(ValInteger != 0UL)
    {
        tmp_u32 = ValInteger;

        /* Count the (number of bits - 1) in ValInteger without the leading zeros */
        while(tmp_u32 > 1UL)
        {
            count_u8++;
            tmp_u32 = tmp_u32 >> 1U;
        }

        /* Calculate the exponent in IEEE754 format and add the bias value(127) */
        exp_s32 = (sint32)count_u8 + 127L;

        /* Since 1 is implicit for normal value, it is removed from the mantissa part, Converting into normalized form*/
        tmp_u32 = ValInteger ^ (1UL << count_u8);

        /* Mantissa of IEEE754 floationg point representation */
        if(count_u8 >= 23U)
        {
            mant_u32 = tmp_u32 >> (count_u8 - 23UL);
        }
        else
        {
            mant_u32 = tmp_u32 << (23UL - count_u8);
        }

        /* Bit representation of a IEEE754 floating point number */
        res_f32u32u.u32 = ((uint32)exp_s32 << 23UL) | mant_u32;

        /* Rounding the value to the nearest representable floating point value. Here round ties to even is used. 
           Example: 
           ValInteger = 16777217  
           The value 16777217 lies exactly halfway between the two representable float values 16777216 and 16777218. 
           Hence it should be rounded to value having LSB of mantissa as even (i.e. 0).
           
           Comparing the mantissa of these two values,
           16777216: 0000 0000 0000 0000 0000 000
           16777218: 0000 0000 0000 0000 0000 001
           
           The 23rb bit of 16777216 is 0 -- even
                           16777218 is 1 -- odd
                           
           Hence 16777217 should be rounded as 16777216. */
           
        if(count_u8 > 23U)
        {
            /* Value of discarded bits after 23 significant mantissa bits is obtained */
            disbits_u32 = tmp_u32 & (0xFFUL >> (8UL - (count_u8 - 23UL)));
            
            /* Tie value calculation for rounding to even */
            tievalue_u32 = 1UL << (count_u8 - 24UL);
            
            /* Last bit of mantissa */
            tmp_u32 = mant_u32 & 1UL;
            
            if(tmp_u32 == 0UL)
            {
                if(disbits_u32 > tievalue_u32)
                {
                    res_f32u32u.u32 = res_f32u32u.u32 + 1UL;
                }
            }
            else
            {
                if(disbits_u32 >= tievalue_u32)
                {
                    res_f32u32u.u32 = res_f32u32u.u32 + 1UL;
                }
            }
        }
    }
    
    return (res_f32u32u.f32);
/*** MFL_CFG_HW_ROUNDING ***/
#endif
}


/*********************************************************************************************************************/


/* MFL_CONVERSION_INL_H */
#endif


