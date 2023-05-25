/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/* NOTE: All definitions within this headers have to be adjusted according to the project needs
 * Within the provided template only one example is provided.
 * Please consult the documentation of rba_MemLib for details of the functionality required.
 * Note: Default settings are defined within the rba_MemLib_Inl.h
 */

#ifndef RBA_MEMLIB_CFG_CALLOUTS_H
#define RBA_MEMLIB_CFG_CALLOUTS_H

/*
 ****************************************************************************************************
 * Includes
 ****************************************************************************************************
 */
#include "Std_Types.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Normal configuration of the rba_MemLib_Copy function is "run-time-optimized"
 * However for some small implementations the code-side optimized version is more
 * suitable.
 *
 * STD_OFF:  run-time optimized (DEFAULT)
 * STD_ON:   code-size optimized
*/
#define RBA_MEMLIB_CODE_SIZE_OPTIMIZED   (STD_OFF)


/** ***********************************************************************
 * \brief   Bit-size of the free-running timer
 * The result value of rba_MemLib_Callout_GetTimerTicks wraps around at 2**RBA_MEMLIB_TIMERBITSIZE.
 * The value range is therefore [0 .. (2**RBA_MEMLIB_TIMERBITSIZE)-1 ]
 * Highest value allowed is 32.
 * Smallest value allowed is 20.
 *
 * Example: RBA_MEMLIB_TIMERBITSIZE (32u)   for a 32bit free-running-time
 * Value range is then [0 .. (2**32)-1] = [0 .. 0xFFFFFFFF]
 *
*/
#define RBA_MEMLIB_TIMERBITSIZE   (32u)


/** ***********************************************************************
 * \brief   Information to other modules that the timer functionality is available
 * STD_ON:  Timer Functionality is available  (DEFAULT)
 * STD_OFF: System has no timer functionality implemented
 *
*/
#define RBA_MEMLIB_USE_TIMER   (STD_ON)

/** ***********************************************************************
 * \brief   Activate timer check during initialization
 * In case the check fails the initialization is not finished.
 * This switch is only evaluated if RBA_MEMLIB_BYPASS_TIMER_US == STD_OFF
 * STD_ON:  Timer Functionality check is performed   (DEFAULT)
 * STD_OFF: Timer Functionality check is NOT performed (saves some code)
 *
*/
#define RBA_MEMLIB_USE_TIMER_CHECK   (STD_ON)


/*
 **********************************************************************************************************************
 * Selection macros to replace internal functions
 * by some externally supplied ones (this can be used to same ressources in the system)
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Selection that a user-provided upcounter is provided in
 * STD_ON:  User configures a 32bit upcounter with 1us resolution directly
 * STD_OFF: rba_MemLib is calculating the timer value (DEFAULT)
 *
*/
#define RBA_MEMLIB_BYPASS_TIMER_US  (STD_OFF)

/** ***********************************************************************
 * \brief   Selection that a user-provided memory-operations are provided
 * STD_ON:  User provides suitable memory operations (see below)
 * STD_OFF: rba_MemLib is providing the memory operations (DEFAULT)
 *
*/
#define RBA_MEMLIB_BYPASS_MEM_OPERATIONS  (STD_OFF)


/*
 **********************************************************************************************************************
 * Derived includes
 **********************************************************************************************************************
*/

#if RBA_MEMLIB_USE_TIMER == STD_ON
#include "Mcu.h"
#endif


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Inlines
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Get timer-ticks from a free-running/wrap-around timer (e.g. system-timer)
 * Attention: The timer has to be chosen in a way that
 *    1) timer-resolution <= 1us/LSB
 *    2) timer-resolution * (2**RBA_MEMLIB_TIMERBITSIZE) >= 1000000us = 1s
 * \return  current timer-ticks (resolution <= 1us/LSB), Bitsize see RBA_MEMLIB_TIMERBITSIZE
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Callout_GetTimerTicks(void)
#if RBA_MEMLIB_USE_TIMER != STD_OFF
{
    return(Mcu_Rb_GetSysTicks());
}
#else
{
    return(0uL);
}
#endif

/** ***********************************************************************
 * \brief   Convert a timer-difference in timer-ticks to microseconds(us)
 * Note: no overflow handling is needed, overflows are handled by caller
 * \param   Timer-ticks
 * \return  Timer-ticks converted to microseconds
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertTimerTicksToUs(uint32 tiTicks_u32)
#if RBA_MEMLIB_USE_TIMER != STD_OFF
{
    return(MCU_RB_TICKS_TO_US(tiTicks_u32));
}
#else
{
    (void)tiTicks_u32;
    return(0uL);
}
#endif

/** ***********************************************************************
 * \brief   Convert microseconds to timer-ticks
 * \param   tiUs_u32  microsecond value, passed values <=1000000us=1s.
 * Note: no overflow handling is needed
 * \return  Microseconds converted to timer-ticks
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertUsToTimerTicks(uint32 tiUs_u32)
#if RBA_MEMLIB_USE_TIMER != STD_OFF
{
    return(MCU_RB_US_TO_TICKS(tiUs_u32));
}
#else
{
    (void)tiUs_u32;
    return(0uL);
}
#endif


/*
 **********************************************************************************************************************
 * Inlines Bypass
 **********************************************************************************************************************
*/

/** ***********************************************************************
 * \brief   Bypass to provide a 32bit free-running upcounter with 1us resolution
 *          This is only needed if the system is providing such a timer (otherwise it is simulated by MemLib)
 *          only relevant if : (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US != STD_OFF)
 * \return  Counter/Timer in 1us/LSB resolution (full 32bit range)
*/
#if (RBA_MEMLIB_USE_TIMER != STD_OFF) && (RBA_MEMLIB_BYPASS_TIMER_US != STD_OFF)
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE uint32 rba_MemLib_Callout_BypassTimerUs(void)
{
    return(0uL);
}
#endif





/** ***********************************************************************
 * \brief   Bypass for memory operations
 * It is possible to link individual services back to the rba_MemLib provided ones
*/

#if (RBA_MEMLIB_BYPASS_MEM_OPERATIONS != STD_OFF)

/* In this example to select the rba_BswSrv implementation (if STD_ON is selected) */
#define RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV   (STD_OFF)

#if RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV == STD_ON
#include "rba_BswSrv.h"
#endif



/** ******************************************************************************************
 * \brief               Memory Copy
 * \param   src_pcu8    Pointer to the source buffer - no alignment constraint
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   length_u32  Number of bytes to copy (0 allowed)
*/
/* MR12 RULE 8.13 VIOLATION: dst_pu8 is pointing to a non-const area, therefore const qualifier is not applicable */
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE void rba_MemLib_Callout_MemCopy(uint8 const *src_pcu8, uint8 *dst_pu8, uint32 length_u32)
#if RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV == STD_ON
{
    /* MR12 DIR 1.1 VIOLATION: Cast from a pointer to object type to a pointer to void is intended */
    (void)rba_BswSrv_MemCopy((void*)dst_pu8,(void const*)src_pcu8,length_u32);
}
#else
{
    rba_MemLib_MemCopy_Provided(src_pcu8,dst_pu8,length_u32);
}
#endif


/** ******************************************************************************************
 * \brief               Set a buffer to a defined value
 * \param   dst_pu8     Pointer to the destination buffer - no alignment constraint
 * \param   value_u8    Value to be set
 * \param   length_u32  Number of bytes (0 allowed)
*/
/* MR12 RULE 8.13 VIOLATION: dst_pu8 is pointing to a non-const area, therefore const qualifier is not applicable */
LOCAL_INLINE void rba_MemLib_Callout_MemSet(uint8 * dst_pu8, uint8 value_u8, uint32 length_u32)
#if RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV == STD_ON
{
    /* MR12 DIR 1.1 VIOLATION: Cast from a pointer to object type to a pointer to void is intended */
    (void)rba_BswSrv_MemSet((void*)dst_pu8,(sint32)value_u8,length_u32);
}
#else
{
    rba_MemLib_MemSet_Provided(dst_pu8,value_u8,length_u32);
}
#endif

/** ******************************************************************************************
 * \brief               Compare two buffers
 *                      note: if length=0, then return=TRUE
 * \param   bfr1_pcu8   Pointer to buffer 1 - no alignment constraint
 * \param   bfr2_pcu8   Pointer to buffer 2 - no alignment constraint
 * \param   length_u32  Number of bytes to compare (0 bytes allowed)
 * \retval  TRUE        Buffers are fully equal
 * \retval  FALSE       At least one entry in the buffers are not same
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_Callout_MemCompareBfr(uint8 const * bfr1_pcu8, uint8 const * bfr2_pcu8, uint32 length_u32)
#if RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV == STD_ON
{
    sint32 diff;
    /* MR12 DIR 1.1 VIOLATION: Cast from a pointer to object type to a pointer to void is intended */
    diff=rba_BswSrv_MemCompare((void const*)bfr1_pcu8,(void const*)bfr2_pcu8,length_u32);
    return(diff==0);
}
#else
{
    return(rba_MemLib_MemCompareBfr_Provided(bfr1_pcu8,bfr2_pcu8,length_u32));
}
#endif

/** ******************************************************************************************
 * \brief       Compare a buffer to a defined value
 *                      note: if length=0, then return=TRUE (=equal)
 *                      note: return value of standard-C memcmp is =0 if it is equal and !=0 otherwise
 *                            Hence a return value translation is necessary.
 *                            But at least for length=0 memcmp also returns=0 (equal)
 * \param   bfr_pcu8    Pointer to the buffer - no alignment constraint
 * \param   value_u8    Value that should be compared to
 * \param   length_u32  Amount of bytes to check (0 allowed)
 * \retval  TRUE        Whole buffer contains only value_u8 (equal)
 * \retval  FALSE       At least one entry in the buffer is not equal to value_u8
*/
/* MR12 RULE 5.9 VIOLATION: declaration/definition of LOCAL_INLINE functions are split on different headers for hiding implementation - no issue */
LOCAL_INLINE boolean rba_MemLib_Callout_MemCompareValue(uint8 const * bfr_pcu8, uint8 value_u8, uint32 length_u32)
#if RBA_MEMLIB_BYPASS_MEM_OPERATIONS_BSWSRV == STD_ON
{
    return(rba_MemLib_MemCompareValue_Provided(bfr_pcu8,value_u8,length_u32));
}
#else
{
    return(rba_MemLib_MemCompareValue_Provided(bfr_pcu8,value_u8,length_u32));
}
#endif


#endif      // RBA_MEMLIB_BYPASS_MEM_OPERATIONS



#endif      // Header multi-inclusion protection
