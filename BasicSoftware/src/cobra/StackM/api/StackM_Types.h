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
 * Component : StackM_Types.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_TYPES_H
#define STACKM_TYPES_H

/*! \file
 *
 * \brief Stack Monitor type definition
 *
 * This file contains the type definitions of the component.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "Std_Types.h"

/*
 ***********************************************************************************************************************
 * Typedefs
 ***********************************************************************************************************************
 */

/** StackM_ReturnType
 * \brief Type which defines function return values for the StackM module.
 *
 */
typedef enum
{
    STACKM_OK = 0,
    STACKM_NOK_LIMIT,
    STACKM_NOK_INIT,
    STACKM_NOK_ID,
    COREID_NOTUSED
}StackM_ReturnType;

/** StackM_StackMeasureType
 * \brief Type which contains user monitoring runtime data for the StackM module.
 *
 *  - FreeSpace: last measured free space of a stack (in bytes).
 *  - FreePercent: last measured stack space utilization in %.
 *  - CheckWordReached: this is a mask which gives the reached checkword levels.
 */
/* When Rte not generate type StackM_StackMeasureType, user shall define StackM_StackMeasureType here.
typedef struct
{
    uint32  FreeSpace;
    float32 FreePercent;
    uint8   CheckWordReached;
} StackM_StackMeasureType;
*/


#endif /* STACKM_TYPES_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

