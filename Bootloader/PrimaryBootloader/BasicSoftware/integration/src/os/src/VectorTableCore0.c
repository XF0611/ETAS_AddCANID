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
 * Component : VectorTableCore0.c
 * Date      : April 7 2022
 * Version   : 2.0
 * Description  : This module implements Vector Table Initialization of Core0.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "VectorTableCore0.h"
#include "Os.h"


/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */


/*
 ***************************************************************************************************
 * Functions
 ***************************************************************************************************
 */
#define ECUM_START_SEC_CALLOUT_CODE
//ETAS BIP Porting TODO #include "EcuM_MemMap.h"
/**********************************************************************************
  Function name     : VectorTableCore0_Init
  Description       : master core vector table initialization task.
  Parameter (in)    : None
  Parameter (inout) : None
  Parameter (out)   : None
  Return value      : None
  Remarks: This task is called only once during master core start up.
***********************************************************************************/

void VectorTableCore0_Init(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.10 Initialize Interrupts
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
    Os_InitializeVectorTable();
}
#define ECUM_STOP_SEC_CALLOUT_CODE
//ETAS BIP Porting TODO #include "EcuM_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
