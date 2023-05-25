/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




/* BSW-17396 */
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
 * \brief   Information to other modules that the timer functionality is available
 * STD_ON:  Timer Functionality is available  (DEFAULT)
 * STD_OFF: System has no timer functionality implemented
 *
*/
#define RBA_MEMLIB_USE_TIMER   (STD_OFF)


/*
 **********************************************************************************************************************
 * Derived includes
 **********************************************************************************************************************
*/


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
LOCAL_INLINE uint32 rba_MemLib_Callout_GetTimerTicks(void)
{
    return(0uL);
}

/** ***********************************************************************
 * \brief   Convert a timer-difference in timer-ticks to microseconds(us)
 * Note: no overflow handling is needed, overflows are handled by caller
 * \param   Timer-ticks
 * \return  Timer-ticks converted to microseconds
*/
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertTimerTicksToUs(uint32 tiTicks_u32)
{
    (void)tiTicks_u32;
    return(0uL);
}

/** ***********************************************************************
 * \brief   Convert microseconds to timer-ticks
 * \param   tiUs_u32  microsecond value, passed values <=1000000us=1s.
 * Note: no overflow handling is needed
 * \return  Microseconds converted to timer-ticks
*/
LOCAL_INLINE uint32 rba_MemLib_Callout_ConvertUsToTimerTicks(uint32 tiUs_u32)
{
    (void)tiUs_u32;
    return(0uL);
}


/*
 **********************************************************************************************************************
 * Inlines Bypass
 **********************************************************************************************************************
*/



#endif      // Header multi-inclusion protection
