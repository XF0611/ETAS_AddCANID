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
 * Component : Rtm_Compiler.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_COMPILER_H
#define RTM_COMPILER_H

/*! \file
 *
 * \brief Runtime measurement private compiler header
 *
 * \note Please note that in the hardware specific driver files compiler dependencies may exist too.
 *
 * This file contains compiler dependencies to enhance portability.
 */


/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm.h"
#include "Rtm_Chk.h"


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Macro to generate inline assembler */
#if (RTM_SIMULATION == STD_ON)
# define RTM_ASMVOLATILE()
#else
# define RTM_ASMVOLATILE asm volatile
#endif

#if (RTM_SIMULATION == STD_ON)
# define RTM_NOINLINE(x) x
#else
/* Macro to prohibit a function to be autoinlined */
/* MISRA RULE 19.10 VIOLATION: Missing () around x is necessary here */
# define RTM_NOINLINE(x) x __attribute__ ((noinline))
#endif


/* Macro to remove warning about unused parameter */
#define RTM_PARAM_UNUSED(x) (x)=(x)

/* Count leading zero algo */
#if (RTM_USE_COUNT_LEADING_ZEROS_ALGO == STD_OFF)
/* Maybe use optimized operation because divisor is 32 Bit */
# define Rtm_Mul64kDiv32(m,d) (((uint64)(m)<<16)/(d))
#else
/* Count leading zero algo, we dont want 64 bit divide, but minimized error */
/* Look how far we can shift to left */
# define Rtm_Mul64kDiv32(m,d) \
({ \
    uint32 result = Rtm_CountLeadingZero32 (m); \
    result = (result < 16) ? result : 16; \
    result = ((m) << result) / ((d) >> (16-result)); \
    result; \
})
#endif

/* Maybe use optimized operation because divisor is 32 Bit */
#define Rtm_MulDiv32(m1,m2,d) (((uint64)(m1)*(m2))/(d))


/*
 ***********************************************************************************************************************
 * Prototypes
 ***********************************************************************************************************************
 */


#endif /* RTM_COMPILER_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

