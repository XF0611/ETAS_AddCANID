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
// LOCAL_INLINE uint32 rba_MemLib_Callout_BypassTimerUs(void)




/** ***********************************************************************
 * \brief   Bypass for memory operations
 * It is possible to link individual services back to the rba_MemLib provided ones
*/



#endif      // Header multi-inclusion protection
