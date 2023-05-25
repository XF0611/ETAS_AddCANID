

#ifndef RBA_MEMLIB_H
#define RBA_MEMLIB_H

#include "Std_Types.h"
#include "rba_MemLib_Cfg.h"

/** ***********************************************************************
 * \brief   Information to other modules that the timer functionality is available
 * STD_ON:  Timer Functionality is available
 * STD_OFF: System has no timer functionality implemented
 *
 * Macro: RBA_MEMLIB_USE_TIMER
*/

/*
 **********************************************************************************************************************
 *Declaration for timer functions (for external usage)
 * Realized as inline function for optimization
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Get timer-ticks from a free-running virtually calculated 1us timer
 *          When using this timer, it is necessary that
 *          rba_MemLib_UpdateTimerUs is called sooner than the internal timer
 *          wraps around
 * \return  current timer-ticks (resolution 1us/LSB) (full 32bit range)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_GetTimerUs(void);

/** ***********************************************************************
 * \brief   Get timer-ticks from a free-running/wrap-around timer (e.g. system-timer)
 * \return  current timer-ticks (resolution <= 1us/LSB), value range might be smaller than 32bit
*/
#define rba_MemLib_GetTimerTicks()    rba_MemLib_Callout_GetTimerTicks()

/** ***********************************************************************
 * \brief   Convert microseconds to timer-ticks
 * \param   tiUs_u32  microsecond value (1us/LSB resolution), passed values must be <=100000us.
 * \return  Microseconds converted to timer-ticks
*/
#define  rba_MemLib_ConvertUsToTimerTicks(tiUs_u32)  rba_MemLib_Callout_ConvertUsToTimerTicks(tiUs_u32)


/** ***********************************************************************
 * \brief   Convert a timer-difference in timer-ticks to microseconds(us)
 * \param   Timer-ticks
 * \return  Timer-ticks converted to 1us/LSB resolution
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_ConvertTimerTicksToUs(uint32 tiTicks_u32);

/** ***********************************************************************
 * \brief   Calculate time difference of two time stamps captured
 *          via rba_MemLib_GetTimerTicks()
 *          Wrap around is considered.
 * \param   tiCurr Timer-ticks of current time reading
 * \param   tiRef  Timer-ticks of reference
 * \return  Difference of tiCurr - tiRef with considered wrap-around
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_CalcDiffInTimerTicks(uint32 tiCurr_u32, uint32 tiRef_u32);


/*
 **********************************************************************************************************************
 * Declarations of Memory-Operation functions
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief               Copy data from src_pcu8 to dst_pu8 in uint8 steps (increasing addresses)
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes to copy (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopy(uint8 const *src_pcu8, uint8 *dst_pu8, uint32 length_u32);

/** ******************************************************************************************
 * \brief               Set a buffer to a defined value
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    Value to be set
 * \param   length_u32  Number of bytes to copy (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemSet(uint8 * dst_pu8, uint8 value_u8, uint32 length_u32);

/** ******************************************************************************************
 * \brief               Compare two buffers
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu8   Pointer to buffer 1 - no alignment constraint
 * \param   bfr2_pcu8   Pointer to buffer 2 - no alignment constraint
 * \param   length_u32  Number of bytes to compare (0 allowed)
 * \retval  TRUE        Buffers are fully equal
 * \retval  FALSE       At least one entry in the buffers are not same
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareBfr(uint8 const * bfr1_pcu8, uint8 const * bfr2_pcu8, uint32 length_u32);

/** ******************************************************************************************
 * \brief       Compare a buffer to a defined value
 * \param   bfr_pcu8    Pointer to the buffer - no alignment constraint
 * \param   value_u8    Value that should be compared to
 * \param   length_u32  Amount of bytes to check
 * \retval  TRUE        Whole buffer contains only value_u8
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_MemCompareValue(uint8 const * bfr_pcu8, uint8 value_u8, uint32 length_u32);

/*
 **********************************************************************************************************************
 * Declarations of Other MemLib functions
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Get the maximum of 2 values
 * \return  MAX(a,b)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Max(uint32 a_u32, uint32 b_u32);

/** ***********************************************************************
 * \brief   Get the minimum of 2 values
 * \return  MIN(a,b)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Min(uint32 a_u32, uint32 b_u32);

/** ***********************************************************************
 * \brief   Compare a passed val_u32 to a threshold (thres_u32)
 *          if it is reach or exceeded a LimRes_u32 value is set
 *          otherwise the passed val_u32 is returned
 *          Example:
 *          rba_MemLib_LimHigh(value,10u,ERROR)    will return ERROR if value>=10u
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_LimHigh(uint32 val_u32, uint32 thres_u32, uint32 LimRes_u32);


/** ***********************************************************************
 * \brief   Compare a passed val_u32 to a threshold (thres_u32)
 *          if it is reach or below a LimRes_u32 value is set
 *          otherwise the passed val_u32 is returned
 *          Example:
 *          rba_MemLib_LimLow(value,10u,ERROR)    will return ERROR if value<=10u
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_LimLow(uint32 val_u32, uint32 thres_u32, uint32 LimRes_u32);



/** ***********************************************************************
 * \brief               Copy data from A to B in uint8 steps
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes to copy (0 allowed)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopyU8U8(uint8 const * src_pcu8, uint8 * dst_pu8, uint32 length_u32);

/** ***********************************************************************
 * \brief               Copy data from A to B in uint32 steps (assuming 32bit alignment)
 * \param   src_pcu32   Pointer to the source buffer - uint32 alignment required
 * \param   dst_pu32    Pointer to the destination buffer - uint32 alignment required
 * \param   length_u32  Number of bytes to copy
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_MemCopyU32U32(uint32 const * src_pcu32, uint32 * dst_pu32, uint32 length_u32);

/** ************************************************************************
 * \brief   Convert data from 4 bytes-array big endian to native uint32
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 4byte array (input value)
 * \return  converted 32bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv4ByteBigEndToNative_u32(uint8 const * input_pcu8);

/**
 * \brief   Convert data from 3 bytes-array big endian to native 24bit value
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 3byte array (input value)
 * \return  converted 24bit data (stored in 32bit return value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv3ByteBigEndToNative_u32(uint8 const * input_pcu8);

/**
 * \brief   Convert data from 2 bytes big endian to native uint16
 * (first byte is highest-value byte)
 *
 * \param   input_pcu8   pointer to a 2byte array (input value)
 * \return  converted 16bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint16 rba_MemLib_Cnv2ByteBigEndToNative_u16(uint8 const * input_pcu8);

/**
 * \brief   Convert data from 4 bytes little endian to native uint32
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 4byte array (input value)
 * \return  converted 32bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv4ByteLtlEndToNative_u32(uint8 const* input_pcu8);

/**
 * \brief   Convert data from 3 bytes-array little endian to native 24bit value
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 3byte array (input value)
 * \return  converted 24bit data (stored in 32bit return value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Cnv3ByteLtlEndToNative_u32(uint8 const * input_pcu8);

/**
 * \brief   Convert data from 2 bytes little endian to native uint16
 * (first byte is lowest-value byte)
 *
 * \param   input_pcu8   pointer to a 2byte array (input value)
 * \return  converted 16bit data
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint16 rba_MemLib_Cnv2ByteLtlEndToNative_u16(uint8 const* input_pcu8);





/**
 * \brief   Convert data from native uint32 to 4 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     32bit input value
 * \param   output_pu8    pointer to a 4byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd4Byte(uint32 input_u32, uint8 * output_pu8);

/**
 * \brief   Convert data from native 24bit-value to 3 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     24bit input value (value has to fit, not limited)
 * \param   output_pu8    pointer to a 3byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd3Byte(uint32 input_u32, uint8 * output_pu8);

/**
 * \brief   Convert data from native uint16 to 2 bytes big endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u16     16bit input value
 * \param   output_pu8    pointer to a 2byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToBigEnd2Byte(uint16 input_u16, uint8 * output_pu8);

/**
 * \brief   Convert data from native uint32 to 4 bytes little endian
 * (first byte is lowest-value byte)
 *
 * \param   input_u32     32bit input data
 * \param   output_pu8    pointer to a 4byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd4Byte(uint32 input_u32, uint8 *output_pu8);

/**
 * \brief   Convert data from native 24bit-value to 3 bytes little endian array
 * (first byte is highest-value byte)
 *
 * \param   input_u32     24bit input value (value has to fit, not limited)
 * \param   output_pu8    pointer to a 3byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd3Byte(uint32 input_u32, uint8 * output_pu8);

/**
 * \brief   Convert data from native uint16 to 2 bytes little endian
 * (first byte is lowest-value byte)
 *
 * \param   input_u16     16bit input data
 * \param   output_pu8    pointer to a 2byte array (output value)
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_CnvNativeToLtlEnd2Byte(uint16 input_u16, uint8 *output_pu8);



/** ***********************************************************************
 * \brief               to get the version info of the module
 * \param               VersioninfoPtr: Pointer to return the version
 *                          information of this module
*/
#if(defined(RBA_MEMLIB_VERSION_INFO) && (RBA_MEMLIB_VERSION_INFO != STD_OFF))
extern void rba_MemLib_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr);
#endif



/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"

/* Update the internal us-timer of the rba_MemLib */
extern void rba_MemLib_UpdateTimerUs(void);


/** ***********************************************************************
 * \brief       Function returns a 32bit pseudo-random number
 *              (initial seeding required)
 *              If nrRange_u16=0: then the full 32bit range is used for the random value
 *              else a random value in interval [0 ; nrRange_u16-1] is returned
 *
 *              Example: nrRange_u16=100: values from 0 to 99 are returned
*/
extern uint32 rba_MemLib_GetRandom_u32(uint16 nrRange_u16);


/** ***********************************************************************
 * \brief       Random number seeding (call only once)
 *              Ideally, pass changing value like the system time
 *              If 0 is passed then a fixed internal seed is used
*/
extern void rba_MemLib_SetRandomSeed(uint32 nrSeed_u32);

/** ***********************************************************************
 * \brief       Random number seeding (call only once)
 *              Determine whether the random generated is seeded already
 *              return=FALSE (=not seeded)
*/
extern boolean rba_MemLib_IsRandomSeeded(void);



#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"


/*
 **********************************************************************************************************************
 **********************************************************************************************************************
 * END of Public Part
 **********************************************************************************************************************
 **********************************************************************************************************************
*/




























/*
 **********************************************************************************************************************
 **********************************************************************************************************************
 * Inlines and internal functions  (do not use outside rba_MemLib)
 **********************************************************************************************************************
 **********************************************************************************************************************
*/

/* Include the actual definition of the provided inline functions */
#include "rba_MemLib_Inl.h"


#endif
