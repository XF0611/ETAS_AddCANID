

#ifndef RBA_MEMLIB_INL_H
#define RBA_MEMLIB_INL_H

#include "Std_Types.h"

/*
 **********************************************************************************************************************
 * Declarations - DO NOT USE OUTSIDE rba_MemLib!
 **********************************************************************************************************************
*/


#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"

/* Memory copy operation */
extern void    rba_MemLib_MemCopy_Provided (uint8 const * src_pcu8, uint8 * dst_pu8,        uint32 length_u32);

/* Memory set operation using a 8bit value */
extern void    rba_MemLib_MemSet_Provided  (uint8 * dst_pu8,        uint8 value_u8,         uint32 length_u32);

/* Memory Buffer compare */
extern boolean rba_MemLib_MemCompareBfr_Provided(uint8 const * bfr1_pcu8,uint8 const * bfr2_pcu8,uint32 length_u32);

/* Memory 8bit value compare */
extern boolean rba_MemLib_MemCompareValue_Provided(uint8 const * bfr_pcu8, uint8 value_u8,         uint32 length_u32);


#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"


/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Callout_GetTimerTicks(void);
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertTimerTicksToUs(uint32 tiTicks_u32);
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertUsToTimerTicks(uint32 tiUs_u32);


/*
 **********************************************************************************************************************
 * Project Extensions/replacements
 **********************************************************************************************************************
*/

/* Include the configured template file that implements parts of the actual functionality */
#include "rba_MemLib_Cfg_Callouts.h"

/*
 **********************************************************************************************************************
 * Default values for silent macros to be defined in rba_MemLib_Cfg_Callouts.h
 **********************************************************************************************************************
*/
/* Selection of the timer check per default */
#ifndef RBA_MEMLIB_USE_TIMER_CHECK
#define RBA_MEMLIB_USE_TIMER_CHECK STD_ON
#endif

/* Default: STD_OFF=run-time optimized */
#ifndef RBA_MEMLIB_CODE_SIZE_OPTIMIZED
#define RBA_MEMLIB_CODE_SIZE_OPTIMIZED   (STD_OFF)
#endif

#ifndef RBA_MEMLIB_BYPASS_TIMER_US
#define RBA_MEMLIB_BYPASS_TIMER_US  (STD_OFF)
#endif

#ifndef RBA_MEMLIB_BYPASS_MEM_OPERATIONS
#define RBA_MEMLIB_BYPASS_MEM_OPERATIONS  (STD_OFF)
#endif

/*
 **********************************************************************************************************************
 * Defines (derived from configured template)
 **********************************************************************************************************************
*/
#if !defined(RBA_MEMLIB_USE_TIMER)
#error  RBA_MEMLIB_USE_TIMER definition is needed in rba_MemLib_Cfg_Callout.h (Default: STD_ON)
#endif

#if RBA_MEMLIB_USE_TIMER != STD_OFF
/* Definition of the Bitmask for wrap-around handling */
#if  (!defined(RBA_MEMLIB_TIMERBITSIZE)) || (RBA_MEMLIB_TIMERBITSIZE<20u) || (RBA_MEMLIB_TIMERBITSIZE>32u)
#error  RBA_MEMLIB_TIMERBITSIZE needs to be defined in range betwen 20u and 32u depending on used timer in rba_MemLib_Cfg_Callout.h
#endif

#if (RBA_MEMLIB_TIMERBITSIZE >= 32u)
#define RBA_MEMLIB_TIMERBITMASK    (0xFFFFFFFFuL)
#else
#define RBA_MEMLIB_TIMERBITMASK    ((1uL<<RBA_MEMLIB_TIMERBITSIZE)-1uL)
#endif

#else
#define RBA_MEMLIB_TIMERBITMASK    (0x0uL)
#endif



/*
 **********************************************************************************************************************
 * Protected variable - do not use outside rba_MemLib
 **********************************************************************************************************************
*/
#if (RBA_MEMLIB_USE_TIMER != STD_OFF)
#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

extern uint32 rba_MemLib_tiTimerUs;

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"
#endif


/* only for simulation purpose */
#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && defined(RBA_MEMLIB_VECU_ENABLE) && (RBA_MEMLIB_VECU_ENABLE == STD_ON)

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

extern uint32 rba_MemLib_tiTimerUs_Incr;            /* Timer increment with each update step */

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

#endif



/** ***********************************************************************
 * \brief               Copy data from src_pcu8 to dst_pu8 in uint8 steps (increasing addresses)
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopy(uint8 const *src_pcu8, uint8 *dst_pu8, uint32 length_u32)
#if (RBA_MEMLIB_BYPASS_MEM_OPERATIONS == STD_OFF)
{
    rba_MemLib_MemCopy_Provided(src_pcu8,dst_pu8,length_u32);
}
#else
{
    rba_MemLib_Callout_MemCopy(src_pcu8,dst_pu8,length_u32);
}
#endif


/** ******************************************************************************************
 * \brief               Set a buffer to a defined value
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    Value to be set
 * \param   length_u32  Number of bytes (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemSet(uint8 * dst_pu8, uint8 value_u8, uint32 length_u32)
#if (RBA_MEMLIB_BYPASS_MEM_OPERATIONS == STD_OFF)
{
    rba_MemLib_MemSet_Provided(dst_pu8,value_u8,length_u32);
}
#else
{
    rba_MemLib_Callout_MemSet(dst_pu8,value_u8,length_u32);
}
#endif

/** ******************************************************************************************
 * \brief               Compare two buffers
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu8   Pointer to buffer 1 - no alignment constraint
 * \param   bfr2_pcu8   Pointer to buffer 2 - no alignment constraint
 * \param   length_u32  Number of bytes (0 allowed)
 * \retval  TRUE        Buffers are fully equal
 * \retval  FALSE       At least one entry in the buffers are not same
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareBfr(uint8 const * bfr1_pcu8, uint8 const * bfr2_pcu8, uint32 length_u32)
#if (RBA_MEMLIB_BYPASS_MEM_OPERATIONS == STD_OFF)
{
    boolean flg;
    flg=rba_MemLib_MemCompareBfr_Provided(bfr1_pcu8,bfr2_pcu8,length_u32);
    return(flg);
}
#else
{
    boolean flg;
    flg=rba_MemLib_Callout_MemCompareBfr(bfr1_pcu8,bfr2_pcu8,length_u32);
    return(flg);
}
#endif

/** ******************************************************************************************
 * \brief       Compare a buffer to a defined value
 *                      note: if length=0, then return=TRUE
 * \param   bfr_pcu8    Pointer to the buffer - no alignment constraint
 * \param   value_u8    Value that should be compared to
 * \param   length_u32  Amount of bytes (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareValue(uint8 const * bfr_pcu8, uint8 value_u8, uint32 length_u32)
#if (RBA_MEMLIB_BYPASS_MEM_OPERATIONS == STD_OFF)
{
    boolean flg;
    flg=rba_MemLib_MemCompareValue_Provided(bfr_pcu8,value_u8,length_u32);
    return(flg);
}
#else
{
    boolean flg;
    flg=rba_MemLib_Callout_MemCompareValue(bfr_pcu8,value_u8,length_u32);
    return(flg);
}
#endif



/** ***********************************************************************
 * \brief   Get timer-ticks from a free-running virtually calculated 1us timer
 *          When using this timer, it is necessary that
 *          rba_MemLib_UpdateTimerUs is called sooner than the internal timer
 *          wraps around
 * \return  current timer-ticks (resolution 1us/LSB) (full 32bit range)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_GetTimerUs(void)
{
#if (RBA_MEMLIB_USE_TIMER != STD_OFF)
    return(rba_MemLib_tiTimerUs);
#else
    return(0uL);
#endif
}


/** ***********************************************************************
 * \brief   Convert a timer-difference in timer-ticks to microseconds(us)
 * \param   Timer-ticks
 * \return  Timer-ticks converted to microseconds
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_ConvertTimerTicksToUs(uint32 tiTicks_u32)
{
    uint32 tiDiff_u32;

    /* Limit the passed time tick value to the valid bitmask / handling wrap-around */
    tiDiff_u32 = (tiTicks_u32 & RBA_MEMLIB_TIMERBITMASK);

    /* Convert corrected difference to us */
    tiDiff_u32 = rba_MemLib_Callout_ConvertTimerTicksToUs(tiDiff_u32);

    return tiDiff_u32;
}

/** ***********************************************************************
 * \brief   Calculate time difference of two time stamps captured
 *          via rba_MemLib_GetTimerTicks()
 *          Wrap around is considered.
 * \param   tiCurr Timer-ticks of current time reading
 * \param   tiRef  Timer-ticks of reference
 * \return  Difference of tiCurr - tiRef with considered wrap-around
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_CalcDiffInTimerTicks(uint32 tiCurr_u32, uint32 tiRef_u32)
{
    uint32 tiDiff_u32;

    /* Difference without considering the bit-size and the wrap around properties */
    tiDiff_u32 = tiCurr_u32 - tiRef_u32;

    /* cut to the relevant bitmask to handle a wrap-around depending
     * on the bit-size of the timer */
    return (tiDiff_u32 & RBA_MEMLIB_TIMERBITMASK);
}




/** ***********************************************************************
 * \brief   Get the maximum of 2 values
 * \return  MAX(a,b)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Max(uint32 a_u32, uint32 b_u32)
{
    uint32 result_u32;

    if(a_u32 > b_u32)
    {
        result_u32 = a_u32;
    }
    else
    {
        result_u32 = b_u32;
    }

    return(result_u32);
}

/** ***********************************************************************
 * \brief   Get the minimum of 2 values
 * \return  MIN(a,b)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Min(uint32 a_u32, uint32 b_u32)
{
    uint32 result_u32;

    if(a_u32 < b_u32)
    {
        result_u32 = a_u32;
    }
    else
    {
        result_u32 = b_u32;
    }

    return(result_u32);
}

/** ***********************************************************************
 * \brief   Compare a passed val_u32 to a threshold (thres_u32)
 *          if it is reach or exceeded a LimRes_u32 value is set
 *          otherwise the passed val_u32 is returned
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_LimHigh(uint32 val_u32, uint32 thres_u32, uint32 LimRes_u32)
{
    uint32 result_u32;

    if(val_u32 >= thres_u32)
    {
        result_u32 = LimRes_u32;
    }
    else
    {
        result_u32 = val_u32;
    }

    return(result_u32);
}


/** ***********************************************************************
 * \brief   Compare a passed val_u32 to a threshold (thres_u32)
 *          if it is reach or below a LimRes_u32 value is set
 *          otherwise the passed val_u32 is returned
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_LimLow(uint32 val_u32, uint32 thres_u32, uint32 LimRes_u32)
{
    uint32 result_u32;

    if(val_u32 <= thres_u32)
    {
        result_u32 = LimRes_u32;
    }
    else
    {
        result_u32 = val_u32;
    }

    return(result_u32);
}


/** ***********************************************************************
 * \brief               Copy data from A to B in uint8 steps
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes to copy - no alignment constraint, 0 allowed
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopyU8U8(uint8 const * src_pcu8, uint8 * dst_pu8, uint32 length_u32)
{
    uint8   const * curSrc_pcu8;
    uint8   const * endSrc_pcu8;
    uint8         * curDst_pu8;

    endSrc_pcu8 = src_pcu8 + length_u32;
    curDst_pu8  = dst_pu8;

    for(curSrc_pcu8 = src_pcu8; curSrc_pcu8 < endSrc_pcu8; curSrc_pcu8++)
    {
        *curDst_pu8 = *curSrc_pcu8;
        curDst_pu8++;
    }
}

/** ***********************************************************************
 * \brief               Copy data from A to B in uint32 steps
 * \param   src_pcu8    Pointer to the source buffer - uint32 alignment required
 * \param   dst_pu8     Pointer to the destination buffer - uint32 alignment required
 * \param   length_u32  Number of bytes to copy - uint32 alignment required (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopyU32U32(uint32 const * src_pcu32, uint32 * dst_pu32, uint32 length_u32)
{
    uint32  const * curSrc_pcu32;
    uint32  const * endSrc_pcu32;
    uint32        * curDst_pu32;

    endSrc_pcu32    = src_pcu32 + (length_u32 >> 2u);
    curDst_pu32     = dst_pu32;

    for(curSrc_pcu32 = src_pcu32; curSrc_pcu32 < endSrc_pcu32; curSrc_pcu32++)
    {
        *curDst_pu32 = *curSrc_pcu32;
        curDst_pu32++;
    }
}



/**
 * \brief   Convert data from 4 bytes-array big endian to native uint32
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 4byte array (input value)
 * \return  converted 32bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv4ByteBigEndToNative_u32(uint8 const * input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 24uL);
    output_u32  |= (((uint32)(input_pcu8[1])) << 16uL);
    output_u32  |= (((uint32)(input_pcu8[2])) << 8uL);
    output_u32  |= (((uint32)(input_pcu8[3])) << 0uL);


    return(output_u32);
}

/**
 * \brief   Convert data from 3 bytes-array big endian to native 24bit value
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 3byte array (input value)
 * \return  converted 24bit data (stored in 32bit return value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv3ByteBigEndToNative_u32(uint8 const * input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 16uL);
    output_u32  |= (((uint32)(input_pcu8[1])) << 8uL);
    output_u32  |= (((uint32)(input_pcu8[2])) << 0uL);


    return(output_u32);
}

/**
 * \brief   Convert data from 2 bytes big endian to native uint16
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 2byte array (input value)
 * \return  converted 16bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint16 rba_MemLib_Cnv2ByteBigEndToNative_u16(uint8 const * input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 8uL);
    output_u32  |= (((uint32)(input_pcu8[1])) << 0uL);


    return((uint16)output_u32);
}

/**
 * \brief   Convert data from native uint32 to 4 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     32bit input value
 * \param   output_pu8    pointer to a 4byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd4Byte(uint32 input_u32, uint8 * output_pu8)
{

    output_pu8[3] = (uint8)(input_u32 >> 0uL);
    output_pu8[2] = (uint8)(input_u32 >> 8uL);
    output_pu8[1] = (uint8)(input_u32 >> 16uL);
    output_pu8[0] = (uint8)(input_u32 >> 24uL);

    return;
}

/**
 * \brief   Convert data from native 24bit-value to 3 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     24bit input value (value has to fit, not limited)
 * \param   output_pu8    pointer to a 3byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd3Byte(uint32 input_u32, uint8 * output_pu8)
{

    output_pu8[2] = (uint8)(input_u32 >> 0uL);
    output_pu8[1] = (uint8)(input_u32 >> 8uL);
    output_pu8[0] = (uint8)(input_u32 >> 16uL);

    return;
}


/**
 * \brief   Convert data from native uint16 to 2 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u16     16bit input value
 * \param   output_pu8    pointer to a 2byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd2Byte(uint16 input_u16, uint8 * output_pu8)
{

    output_pu8[1] = (uint8)((uint32)input_u16 >> 0uL);
    output_pu8[0] = (uint8)((uint32)input_u16 >> 8uL);

    return;
}




/**
 * \brief   Convert data from 4 bytes little endian to native uint32
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 4byte array (input value)
 * \return  converted 32bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv4ByteLtlEndToNative_u32(uint8 const* input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 0u);
    output_u32  |= (((uint32)(input_pcu8[1])) << 8u);
    output_u32  |= (((uint32)(input_pcu8[2])) << 16u);
    output_u32  |= (((uint32)(input_pcu8[3])) << 24u);


    return(output_u32);
}

/**
 * \brief   Convert data from 3 bytes-array little endian to native 24bit value
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 3byte array (input value)
 * \return  converted 24bit data (stored in 32bit return value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv3ByteLtlEndToNative_u32(uint8 const * input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 0u);
    output_u32  |= (((uint32)(input_pcu8[1])) << 8u);
    output_u32  |= (((uint32)(input_pcu8[2])) << 16u);


    return(output_u32);
}

/**
 * \brief   Convert data from 2 bytes little endian to native uint16
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 2byte array (input value)
 * \return  converted 16bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint16 rba_MemLib_Cnv2ByteLtlEndToNative_u16(uint8 const* input_pcu8)
{
    uint32 output_u32;

    output_u32  =  (((uint32)(input_pcu8[0])) << 0uL);
    output_u32  |= (((uint32)(input_pcu8[1])) << 8uL);


    return((uint16)output_u32);
}



/**
 * \brief   Convert data from native uint32 to 4 bytes little endian
 * (first byte is lowest-value byte)
 *
 * \param   input_u32     32bit input data
 * \param   output_pu8    pointer to a 4byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd4Byte(uint32 input_u32, uint8 *output_pu8)
{

    output_pu8[0] = (uint8)(input_u32 >> 0uL);
    output_pu8[1] = (uint8)(input_u32 >> 8uL);
    output_pu8[2] = (uint8)(input_u32 >> 16uL);
    output_pu8[3] = (uint8)(input_u32 >> 24uL);

    return;
}

/**
 * \brief   Convert data from native 24bit-value to 3 bytes little endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     24bit input value (value has to fit, not limited)
 * \param   output_pu8    pointer to a 3byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd3Byte(uint32 input_u32, uint8 * output_pu8)
{

    output_pu8[0] = (uint8)(input_u32 >> 0uL);
    output_pu8[1] = (uint8)(input_u32 >> 8uL);
    output_pu8[2] = (uint8)(input_u32 >> 16uL);

    return;
}

/**
 * \brief   Convert data from native uint16 to 2 bytes little endian
 * (first byte is lowest-value byte)
 *
 * \param   input_u16     16bit input data
 * \param   output_pu8    pointer to a 2byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd2Byte(uint16 input_u16, uint8 *output_pu8)
{

    output_pu8[0] = (uint8)((uint32)input_u16 >> 0uL);
    output_pu8[1] = (uint8)((uint32)input_u16 >> 8uL);

    return;
}
#endif

