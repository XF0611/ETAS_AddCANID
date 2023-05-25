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
 * Component : Ecu.c
 * Date      : April 8, 2021
 * Version   : 4.0
 * Description  : This module implements Ecu level functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Ecu.h"


/*
 **********************************************************************************************************************
 * Private Define/Macros
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define BSW_START_SEC_VAR_CLEARED_8
#include "Bsw_MemMap.h"

#define BSW_STOP_SEC_VAR_CLEARED_8
#include "Bsw_MemMap.h"


/*
 **********************************************************************************************************************
 * Local Variables
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
 */
#define BSW_START_SEC_CODE
#include "Bsw_MemMap.h"
/*******************************************************************************************
* Name              :   Ecu_SwitchOff
* Description       :   ECU go off switch function which is implementation dependent.
* Parameters[in]    :   void
* Parameters[out]   :   void
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
//zco7sgh to avoid blink(0079D98C(EcuM_Prv_GetResource)) issue
FUNC(void, ECU_CODE)Ecu_SwitchOff(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.4	EcuM Shutdown
     * User integration required, dependent on specific ECU go off strategy.
     * Please see the integration guide section for further details.
     */
    while (1)
    {
        /* code */
    };    
    
}
#define BSW_STOP_SEC_CODE
#include "Bsw_MemMap.h" /*lint !e537 permit multiple inclusion */


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
