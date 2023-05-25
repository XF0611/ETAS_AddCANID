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
 * Component : ExeMgrUT_Pub.h
 * Date      : Jan 28 2020
 * Version   : 1.0
 * Description  : This module provides global interfaces from Execution Manager for the platform.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables.
 ***********************************************************************************************
*/
#ifndef EXEMGRUT_PUB_H
#define EXEMGRUT_PUB_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"


/*
 **********************************************************************************************************************
 * Macros
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define ExeMgrUT_START_SEC_CODE                   
#include "ExeMgrUT_MemMap.h"
extern FUNC (Std_ReturnType, ExeMgr_CODE) RE_ExeMgr_ValidateWakeup(void);
#define ExeMgrUT_STOP_SEC_CODE                       
#include "ExeMgrUT_MemMap.h"


#endif /* EXEMGRUT_PUB_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
