/*
 ***********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 ***********************************************************************************************************************
 ************************************************************************************************
 * Component : Rtm_Chk.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_CHK_H
#define RTM_CHK_H

/*! \file
 *
 * \brief Runtime measurement header to include and check the configuration
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm_Cfg.h"
#include "Os.h"

/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Macros for special values */
#define RTM_MAX_U32    0xfffffffful
#define RTM_MAX_U16    ((uint16)0xffffu)
#define RTM_MAX_U8     ((uint8)0xffu)
#define RTM_MSB_U32    0x80000000ul
#define RTM_MSB_U16    ((uint16)0x8000u)
#define RTM_MSB_U8     ((uint8)0x80u)
#define RTM_LSB_U32    0x00000001ul

/* Macros to check limits without preprocessor */
#define RTM_CHECK_LOWERLIMIT(name,val,min) \
extern uint8 Rtm_StatesSizeNegative_IfToSmall_##name[2*(val) - 2*(min) + 1];
#define RTM_CHECK_UPPERLIMIT(name,val,max) \
extern uint8 Rtm_StatesSizeNegative_IfToGreat_##name[2*(max) - 2*(val) + 1];

/*
 ***********************************************************************************************************************
 * Config check and preparation
 ***********************************************************************************************************************
 */


/* Target selection was made? */
#ifndef RTM_TARGET
# define RTM_TARGET_GENERIC 0
# define RTM_TARGET RTM_TARGET_GENERIC
#endif

/* For controlled measurements */
#if (RTM_CTRL == STD_ON)

/* Check for max number of cores */
# if (RTM_NUM_CORES > 4)
#  error Not more than 4 cores supported by cross core communication
# endif

#endif

/* For monitoring sample generation */
#ifndef RTM_MON_SAMPLES_PER_MINUTE
# define RTM_MON_SAMPLES_PER_MINUTE 60
#endif
/* Choose right type for monitoring sample generation */
#if ((RTM_MAINFCT_CALLS_PER_MINUTE / RTM_MON_SAMPLES_PER_MINUTE) < 0xff)
typedef uint8 Rtm_MonSample;
#elif ((RTM_MAINFCT_CALLS_PER_MINUTE / RTM_MON_SAMPLES_PER_MINUTE) < 0xffff)
typedef uint16 Rtm_MonSample;
#elif ((RTM_MAINFCT_CALLS_PER_MINUTE / RTM_MON_SAMPLES_PER_MINUTE) < 0xffffffff)
typedef uint32 Rtm_MonSample;
#else
# error Call frequency of main function is to high in conjunction with low monitoring sample generation frequency
#endif

/* Too many samples per minute configured? */
#if (RTM_MON_SAMPLES_PER_MINUTE > RTM_MAINFCT_CALLS_PER_MINUTE)
# error RTM_MON_SAMPLES_PER_MINUTE is too high in conjunction with low call frequency of main function
#endif


/* Check for possible overflow of monitoring samples (timer should not count MAX_U32 times in one sample) */
/* how many seconds takes one sample: 60 / RTM_MON_SAMPLES_PER_MINUTE
   how many seconds without overflow: RTM_MAX_U32 / RTM_TICKS_PER_SECOND
   -> (60 / RTM_MON_SAMPLES_PER_MINUTE > RTM_MAX_U32 / RTM_TICKS_PER_SECOND)
*/
/* Not possible with #if statement because values are not known by the preprocessor */
RTM_CHECK_UPPERLIMIT(MonSamplesPerMinuteInv, 60 / RTM_MON_SAMPLES_PER_MINUTE + 1, RTM_MAX_U32 / RTM_TICKS_PER_SECOND);

/* Is this a simulation? */
#ifdef WIN32
# define RTM_SIMULATION STD_ON
#else
# define RTM_SIMULATION STD_OFF
#endif
 

#endif /* RTM_CHK_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

