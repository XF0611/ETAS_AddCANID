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
 * Component : Linker_Cfg.h
 * Date      : Dec 29, 2022
 * Version   : 1.0
 * Description  : This module implements Linker parameters.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef LINKER_CFG_H
#define LINKER_CFG_H

/*! \file
 *
 * \brief Linker configurations.
 *
 * This file contains the configuration definition and macros for the component.
 */

/*
 ***************************************************************************************************
 * Version Information Defines
 ***************************************************************************************************
 */
/* Version of Linker specification */
/**
 * @ingroup LINKER_H
 *
 * Major Version of Linker specification
 */
#define LINKER_AR_RELEASE_MAJOR_VERSION                                4u
/**
 * @ingroup LINKER_H
 *
 * Minor Version of Linker specification
 */
#define LINKER_AR_RELEASE_MINOR_VERSION                                2u
/**
 * @ingroup LINKER_H
 *
 * Revision Version of Linker specification
 */
#define LINKER_AR_RELEASE_REVISION_VERSION                             2u

/* Version of Linker implementation */
/**
 * @ingroup LINKER_H
 *
 * Major Version of Linker implementation
 */
#define LINKER_SW_MAJOR_VERSION                                        1u
/**
 * @ingroup LINKER_H
 *
 * Minor Version of Linker implementation
 */
#define LINKER_SW_MINOR_VERSION                                        0u
/**
 * @ingroup LINKER_H
 *
 * Patch Version of Linker implementation
 */
#define LINKER_SW_PATCH_VERSION                                        0u

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

/*
     **********************************************************************************************************************
     * Imported declarations
     **********************************************************************************************************************
     */
extern __SYSCORE_STACK_START;
extern __SYSCORE_STACK_END;

/*
 ***********************************************************************************************************************
 * Confiugration Definitions - General
 ***********************************************************************************************************************
 */


/* Set to STD_ON if this feature has to be enabled, otherwise set STD_OFF */
#define LINKER_CFG_ENABLE STD_ON    /* Enable Linker component. */

/*
 ***********************************************************************************************************************
 * Confiugration Definitions - TargetFlsTst
 ***********************************************************************************************************************
 */


/*
 ***********************************************************************************************************************
 * Confiugration Definitions - TargetRamTst
 ***********************************************************************************************************************
 */


/*
 ***********************************************************************************************************************
 * Confiugration Definitions - TargetStack
 ***********************************************************************************************************************
 */
/**
 * @brief Stack growing direction on the target.
 * 
 * Normally stack is growning down on most of target hardware.
 * However, if the stack is growing up, then LINKER_STACK_GROWDOWN shall be set FALSE.
 */


#define LINKER_STACK_GROWDOWN TRUE


/**
 * @brief Number of Stacks available in AUTOSAR context.
 * 
 * Normally one core has a single stack, however its target dependent.
 * E.g. for some ARM cores, multiple stacks required for different context for a single core.
 */
#define LINKER_NUMBEROFSTACK 1

/**
 * @brief Size in byte of each configured Stacks available in AUTOSAR context.
 *
 */
#define STACK_LEN_SYSCORE_STACK        4096
#if (LINKER_STACK_GROWDOWN == TRUE)
    #define STACK_START_SYSCORE_STACK  __SYSCORE_STACK_END
    #define STACK_END_SYSCORE_STACK    __SYSCORE_STACK_START
#else
    #define STACK_START_SYSCORE_STACK  __SYSCORE_STACK_START
    #define STACK_END_SYSCORE_STACK    __SYSCORE_STACK_END
#endif


#endif /* LINKER_CFG_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/
