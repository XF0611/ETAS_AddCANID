


#ifndef RBA_MFLTCCOMMON_CONVERSION_INL_H
#define RBA_MFLTCCOMMON_CONVERSION_INL_H

#ifdef __ghs__
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern sint8 __satb(int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern sint16 __sath(int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern uint8 __satbu(unsigned int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern uint16 __sathu(unsigned int);
#endif

#ifdef __TASKING__
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern sint16 __sath(int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern sint8  __satb(int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern uint16 __sathu(unsigned int);
    /* MR12 RULE 8.2,4.6 VIOLATION: Parameters are considered as per compiler builtins,
    Intrinsic Builtin used for optimization which uses basic types  */
    extern uint8 __satbu(unsigned int);
#endif


/*
**********************************************************************************************************************
* Prototypes
**********************************************************************************************************************
*/

#if (!defined(rba_MflTCCommon_Prv_Sat_s32_s16_Inl))
LOCAL_INLINE sint16 rba_MflTCCommon_Prv_Sat_s32_s16_Inl(sint32 res_s32);
#endif

#if (!defined(rba_MflTCCommon_Prv_Sat_s32_s8_Inl))
LOCAL_INLINE sint8 rba_MflTCCommon_Prv_Sat_s32_s8_Inl(sint32 res_s32);
#endif

#if (!defined(rba_MflTCCommon_Prv_Sat_s32_u16_Inl))
LOCAL_INLINE uint16 rba_MflTCCommon_Prv_Sat_s32_u16_Inl(sint32 res_s32);
#endif

#if (!defined(rba_MflTCCommon_Prv_Sat_s32_u32_Inl))
LOCAL_INLINE uint32 rba_MflTCCommon_Prv_Sat_s32_u32_Inl(sint32 res_s32);
#endif
#if (!defined(rba_MflTCCommon_Prv_Sat_u32_u16_Inl))
LOCAL_INLINE uint16 rba_MflTCCommon_Prv_Sat_u32_u16_Inl(uint32 res_u32);
#endif

#if (!defined(rba_MflTCCommon_Prv_Sat_u32_u8_Inl))
LOCAL_INLINE uint8 rba_MflTCCommon_Prv_Sat_u32_u8_Inl(uint32 res_u32);
#endif

/*
 **********************************************************************************************************************
 *
 * List of services
 *
 * rba_MflTCCommon_Prv_IntToFloatCvrt_u32_f32_Inl
 * rba_MflTCCommon_Prv_IntToFloatCvrt_s32_f32_Inl
 *
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Prototypes
**********************************************************************************************************************
 */


LOCAL_INLINE sint16 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s16_Inl(float32 ValFloat);
LOCAL_INLINE sint32 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s32_Inl(float32 ValFloat);
LOCAL_INLINE sint8 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s8_Inl(float32 ValFloat);
LOCAL_INLINE uint16 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u16_Inl(float32 ValFloat);
LOCAL_INLINE uint32 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u32_Inl(float32 ValFloat);
LOCAL_INLINE uint8 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u8_Inl(float32 ValFloat);


/*
 **********************************************************************************************************************
 * Implementations
**********************************************************************************************************************
 */

LOCAL_INLINE sint16 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s16_Inl(float32 ValFloat)
{
	sint32 tmp_s32;
	
    tmp_s32 = rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s32_Inl(ValFloat);
    
	return (rba_MflTCCommon_Prv_Sat_s32_s16_Inl(tmp_s32));
}

/*********************************************************************************************************************/

LOCAL_INLINE sint32 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s32_Inl(float32 ValFloat)
{
    return ((sint32)ValFloat);
}

/*********************************************************************************************************************/

LOCAL_INLINE sint8 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s8_Inl(float32 ValFloat)
{
	sint32 tmp_s32;
	
    tmp_s32 = rba_MflTCCommon_Prv_FloatToIntCvrt_f32_s32_Inl(ValFloat);
    
	return (rba_MflTCCommon_Prv_Sat_s32_s8_Inl(tmp_s32));
}

/*********************************************************************************************************************/

LOCAL_INLINE uint16 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u16_Inl(float32 ValFloat)
{
	uint32 tmp_u32;
	
    tmp_u32 = rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u32_Inl(ValFloat);
    
	return (rba_MflTCCommon_Prv_Sat_u32_u16_Inl(tmp_u32));
}

/*********************************************************************************************************************/

LOCAL_INLINE uint32 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u32_Inl(float32 ValFloat)
{
    return ((uint32)ValFloat);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint8 rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u8_Inl(float32 ValFloat)
{
	uint32 tmp_u32;
	
    tmp_u32 = rba_MflTCCommon_Prv_FloatToIntCvrt_f32_u32_Inl(ValFloat);
    
	return (rba_MflTCCommon_Prv_Sat_u32_u8_Inl(tmp_u32));
}


/*********************************************************************************************************************/

 LOCAL_INLINE sint16 rba_MflTCCommon_Prv_Sat_s32_s16_Inl(sint32 res_s32)
{

    #ifdef __TASKING__  
      /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */    
        return(__sath((int)res_s32));
    #elif defined __ghs__
     /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */
        return(__sath((int)res_s32));    
    #else
        #ifdef __BUILTIN_TRICORE_SATH
            return(__builtin_tricore_sath (res_s32));
        #else
            asm volatile(" sat.h %0        \n\
                         ":"+d"(res_s32));
                         /*         0          */
            return((sint16)res_s32);
        #endif
    #endif
}

/*********************************************************************************************************************/

LOCAL_INLINE sint8 rba_MflTCCommon_Prv_Sat_s32_s8_Inl(sint32 res_s32)
{
    #ifdef __TASKING__ 
    /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */
        return(__satb((int)res_s32));
    #elif defined __ghs__
     /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */
        return(__satb((int)res_s32));
    #else
        #ifdef __BUILTIN_TRICORE_SATH
            return(__builtin_tricore_satb (res_s32));  
        #else
            asm volatile(" sat.b %0        \n\
                         ":"+d"(res_s32));
                         /*         0          */
            return((sint8)res_s32);
        #endif
    #endif
}

/*********************************************************************************************************************/

LOCAL_INLINE uint16 rba_MflTCCommon_Prv_Sat_s32_u16_Inl(sint32 res_s32)
{
    uint32 res_u32;
    uint16 res_u16;
    
    res_u32 = rba_MflTCCommon_Prv_Sat_s32_u32_Inl(res_s32);
    res_u16 = rba_MflTCCommon_Prv_Sat_u32_u16_Inl(res_u32);
    
    return (res_u16);
}

/*********************************************************************************************************************/

/* MR12 RULE 2.7 VIOLATION: the variable is used in the asm statement */
LOCAL_INLINE uint32 rba_MflTCCommon_Prv_Sat_s32_u32_Inl(sint32 res_s32)
{
    uint32 res_u32;
    
    #ifdef __TASKING__
    
    __asm volatile("  max    %0, %0, #0          \n\
                   " : "=d" (res_s32) : "0" (res_s32) );
    #else 
    
    asm volatile("  max    %[res_s32], %[res_s32], 0   \n\
                 " : [res_s32] "+d" (res_s32) );
    #endif
    
    res_u32 = (uint32)res_s32;
    
    return (res_u32);
}

/*********************************************************************************************************************/

LOCAL_INLINE uint16 rba_MflTCCommon_Prv_Sat_u32_u16_Inl(uint32 res_u32)
{

    #ifdef __TASKING__   
    /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */    
        return(__sathu((unsigned int)res_u32));
    /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */    
    #elif defined __ghs__
        return(__sathu((unsigned int)res_u32));    
    #else
        #ifdef __BUILTIN_TRICORE_SATH
            return(__builtin_tricore_sathu (res_u32));
        #else
            asm volatile(" sat.hu %0        \n\
                         ":"+d"(res_u32));
                         /*         0          */
            return((uint16)res_u32);
        #endif
    #endif
}

 /*********************************************************************************************************************/

LOCAL_INLINE uint8 rba_MflTCCommon_Prv_Sat_u32_u8_Inl(uint32 res_u32)
{
    #ifdef __TASKING__ 
    /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */   
        return(__satbu((unsigned int)res_u32));
    #elif defined __ghs__
    /* MR12 DIR 4.6 VIOLATION: Intrinsic Builtin used for optimization which uses basic types  */   
        return(__satbu((unsigned int)res_u32));
    #else
        #ifdef __BUILTIN_TRICORE_SATH
            return(__builtin_tricore_satbu (res_u32));  
        #else
            asm volatile(" sat.bu %0        \n\
                         ":"+d"(res_u32));
                         /*         0          */
            return((uint8)res_u32);
        #endif
    #endif
}

/*********************************************************************************************************************/

/* RBA_MFLTCCOMMON_CONVERSION_INL_H */
#endif


