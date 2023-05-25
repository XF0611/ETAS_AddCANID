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
 * Component : ExtWdg.c
 * Date      : March 31, 2020
 * Version   : 3.0
 * Description  : This module implements Wdg Driver wrapper for external Wdg as non MCAL modules
 ***********************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "ExtWdg.h"


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


/*
 **********************************************************************************************************************
 * Local Variables
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Function like Macros
 **********************************************************************************************************************
 */
#define EXTWDG_START_SEC_CODE
#include "ExtWdg_MemMap.h" /*lint !e537 permit multiple inclusion */
/*******************************************************************************************
* Name              :   ExtWdg_Init
* Description       :   Initialize the external wdg
* Parameters[in]    :
* Limitations       :
* ReturnType        :
*******************************************************************************************/
void ExtWdg_Init(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.7	External Watchdog
     * User integration required.
     * Please see the integration guide section for further details.
     */
}

/*******************************************************************************************
* Name              :   ExtWdg_SetTriggerCondition
* Description       :   Watchdog triggering routine in case it is called from a BSW task.
* Parameters[in]    :   uint16 timeout - Timeout value (milliseconds) for setting the trigger counter.
* Parameters[out]   :   None
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
FUNC(void, EXTWDG_CODE) ExtWdg_SetTriggerCondition(uint16 timeout)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.7	External Watchdog
     * User integration required.
     * Please see the integration guide section for further details.
     */
}

/*******************************************************************************************
* Name              :   ExtWdg_SetMode
* Description       :   Sets the current mode of External WatchDog.
* Parameters[in]    :   WdgIf_ModeType
* Limitations       :
* ReturnType        :   Std_ReturnType
*******************************************************************************************/
FUNC(Std_ReturnType, EXTWDG_CODE) ExtWdg_SetMode(WdgIf_ModeType Mode)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.7	External Watchdog
     * User integration required.
     * Please see the integration guide section for further details.
     */
	return E_OK;
}
#define EXTWDG_STOP_SEC_CODE
#include "ExtWdg_MemMap.h" /*lint !e537 permit multiple inclusion */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
