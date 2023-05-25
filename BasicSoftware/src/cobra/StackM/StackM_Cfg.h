/*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
**********************************************************************************************************************
 ************************************************************************************************
 * Component : StackM_Cfg.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_CFG_H
#define STACKM_CFG_H

/*! \file
 *
 * \brief Stack Monitor configurations.
 *
 * This file contains the configuration definition and macros for the component.
 * Currently, AUTOSAR single core and Multicore CPUs stack are supported. Non-AR CPU stacks are not monitored.
 * 
 * Stacks of different Target architeutre, e.g. PowerPC, AURIX, ARM may have different stacks.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "StackM_Types.h"
#include "Rte_StackM_Type.h"
#include "Os.h"
#include "Linker_Cfg.h"
/*
 ***********************************************************************************************************************
 * Confiugration Definitions - General
 ***********************************************************************************************************************
 */



#define STACKM_DEV_ERROR_DETECT STD_ON

/* Set to STD_ON if this feature has to be enabled, otherwise set STD_OFF */
#define STACKM_CFG_ENABLE STD_ON    /* Enable StackM component. */
#define STACKM_CFG_ENABLETRACE STD_ON    /* Enable Stack utilization tracing feature. */
#define STACKM_CFG_PATTERNINIT STD_ON    /* Enable Stack filling for tracing feature. */


#define STACKM_CFG_DEFAULTFILLUPVALUE ((uint32)0x4C4C4946)  /* "FILL" */
#define STACKM_CFG_LIMITHIGH ((uint32)0x3179656B)  /* "key1" */
#define STACKM_CFG_LIMITLOW ((uint32)0x3279656B)  /* "key2" */


#define STACKM_STACKPOINT_LOWEST ((uint32*)0xFFFFFFFFUL)
/**
 * @brief Stack growing direction on the target.
 * 
 */
#define STACKM_GROWDOWN       LINKER_STACK_GROWDOWN

/**
 * @brief Number of Stacks available in AUTOSAR context.
 * 
 */
#define STACKM_NUMBEROFSTACK       LINKER_NUMBEROFSTACK

/*
 ***********************************************************************************************************************
 * Confiugration Definitions - Target
 ***********************************************************************************************************************
 */

/********** Stack0 parameters **********
***************************************/
#define Stack0_StackId     (0)
#if (STACKM_GROWDOWN == TRUE)
    #define Stack0_Start       ((uint32*)((uint32)&STACK_START_SYSCORE_STACK - 0x00000001))
    #define Stack0_End         ((uint32*)((uint32)&STACK_END_SYSCORE_STACK  + 0x00000000))
    #define Stack0_Limit0      ((uint32*)((uint32)&STACK_START_SYSCORE_STACK - (STACK_LEN_SYSCORE_STACK>>5)))  /* convert to uint32,limit boundary at 12.5% */
    #define Stack0_Limit1      ((uint32*)((uint32)&STACK_START_SYSCORE_STACK - (STACK_LEN_SYSCORE_STACK>>4)))  /* convert to uint32,limit boundary at 25% */
    #define Stack0_Limit2      ((uint32*)((uint32)&STACK_START_SYSCORE_STACK - (STACK_LEN_SYSCORE_STACK>>3)))  /* convert to uint32,limit boundary at 50% */

    /* Check whether the Address to check is out of address space of current stack */
    LOCAL_INLINE boolean STACKM_CBK_Stack0( const uint32* currAddr) {return (((currAddr > Stack0_Start) || (currAddr == 0))? TRUE:FALSE);}
#else
    #error "Stack growing direction not supported in current StackM version !"
#endif

/**
 * @brief STACKM_DEFINE_STACKTABLE：Stack properties to be monitored during runtime.
 * 
 * Parameter descriprion of a stack entry:
 * - CoreId: core which accesses this stack
 * - Start: first 32 bit word of this stack
 * - End: last 32 bit word of this stack
 * - Limit0, Limit1, Limit2: adress where the security level 0, 1 or 2 should be written.
 *                           If it is equalt to STACKM_LIMIT_NOTUSED it will be not used.  
 * - Cbk: In case user want to use the address check callback feature, callback function should be introduced by 
 *        define the callback as the last element of STACKM_DEFINE_STACKTABLE, otherwise define as STACKM_CBK_NOTUSED.
 * Note: the stack table must have exactly STACKM_NUMBEROFSTACK entries      
 */
#define STACKM_DEFINE_STACKTABLE \
	{/* StackMStackCoreID */\
		{0, Stack0_Start, Stack0_End, Stack0_Limit0, Stack0_Limit1, Stack0_Limit2, STACKM_CBK_Stack0} \
	}


#endif /* STACKM_CFG_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/
