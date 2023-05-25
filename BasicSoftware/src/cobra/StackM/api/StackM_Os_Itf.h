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
 * Component : StackM_Os_Itf.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_OS_ITF_H
#define STACKM_OS_ITF_H

/*! \file
 *
 * \brief Stack Monitor interface to RTA-OS
 *
 * This file contains the interface to RTA-OS. The OS interface is centralized here to enhance portability.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "Os.h"


/*
 ***********************************************************************************************************************
 * Macros
 ***********************************************************************************************************************
 */
#if (OS_NUM_OS_CORES > 1U)
#define StackM_GetCoreId()     ((uint32)GetCoreID())
#else
#define StackM_GetCoreId()     OS_CORE_ID_MASTER
#endif


#endif /* STACKM_OS_ITF_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

