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
 * Component : VectorTableCore0.h
 * Date      : Sep 14 2019
 * Version   : 1.0
 * Description  : This module implements heaer file supplied for Vector Table Initialization of Core0.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef VECTORTABLECORE0_H
#define VECTORTABLECORE0_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"
extern void VectorTableCore0_Init(void);
#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"


#endif /* VECTORTABLECORE0_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
