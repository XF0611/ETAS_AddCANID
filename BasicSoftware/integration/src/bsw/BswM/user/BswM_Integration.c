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
 * Component : BswM_Integration.c
 * Date      : Sep 25 2019
 * Version   : 1.0
 * Description  : This module implements BswM integration functions
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "BswM_Integration.h"
#include "SchM_BswM_Type.h"


/*
 **********************************************************************************************************************
 * Imported Function declarations
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/**********************************************************************************
  Function name		  :	BswM_StartupTwo_CddInit
  Description         :	This function is wrapper for CDD initialization
                        during startup two phase.
  Parameter	(in)	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks:            These CDDs will be initialized before sensor supply, load supply
                      and Dem initialization.
***********************************************************************************/
void BswM_StartupTwo_CddInit(void)
{
    /* This function is wrapper for CDD initialization during startup two phase. */

    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_Run_CddInitZero
  Description         : This function is wrapper for CDD initialization
                        ZERO during run phase.
  Parameter	(in)	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value	  	  :	None
  Remarks:            These CDDs will be initialized after Dem initialization but
                      before sensor supply, load supply.
***********************************************************************************/
void BswM_Run_CddInitZero(void)
{
    /* This function is wrapper for CDD initialization ZERO during run phase. */

    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_Enable_PeripheralSply
  Description		  :	Enable power supply to peripherals, e.g. ADC
 	 	 	 	 	 	conditional pull-up circuit by setting IoHwAb port [x] high.
  Parameter	(in)	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value	   	  :	None
  Remarks:            This peripheral supply control is reserviced on this controller,
                      keep this function for customer possible further extension.
***********************************************************************************/
void BswM_Enable_PeripheralSply(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_Run_CddInitOne
  Description         : This function is wrapper for CDD initialization
                        ONE during run phase.
  Parameter	(in)	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value	  	  :	None
  Remarks:            These CDDs will be initialized after Dem initialization and
                      sensor supply, but before load supply.
***********************************************************************************/
void BswM_Run_CddInitOne(void)
{
    /* This function is wrapper for CDD initialization ONE during run phase. */

    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_PerformSelfHold
  Description		  :	After peripheral supply enabled, controller shall
                        then perform self-hold function by setting SelfHold pin high.
  Parameter	(in)  	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks             : By setting self-hold high, it will lock wake state of SBC.
***********************************************************************************/
void BswM_PerformSelfHold(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_CanTrcv_GotoNormal
  Description		  :	This function request Can Trcv go to Normal.
  Parameter	(in)  	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks             : All Configurable CAN trcv will be set normal.
***********************************************************************************/
void BswM_CanTrcv_GotoNormal(void)
{
    //Customer integration required CanTrcv_SetOpMode(0,CANTRCV_TRCVMODE_NORMAL);
    //Customer integration required CanTrcv_SetOpMode(1,CANTRCV_TRCVMODE_NORMAL);

    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_CanTrcv_GotoSleep
  Description		  :	This function request Can Trcv go to Sleep.
  Parameter	(in)  	  :	None
  Parameter	(inout)	  :	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks             : All Configurable CAN trcv will be set sleep.
***********************************************************************************/
void BswM_CanTrcv_GotoSleep(void)
{
    //Customer integration required CanTrcv_TJA1145_GotoSleep(0);
    /* Other CAN Trcv no sleep and wakeup required. */

    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	BswM_SelfHold_Unlock
  Description       :	This function request Unlock mechanism which will be performed by App.
  Parameter	(in)	  :	None
  Parameter	(inout)	:	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks:
***********************************************************************************/
void BswM_SelfHold_Unlock(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.2	BswM Actions
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     * [$Satisfies $BIP_INTG 101]
     */
}

/**********************************************************************************
  Function name		  :	EthSwtInit
  Description       :	This function initialize EthSwitch.
  Parameter	(in)	  :	None
  Parameter	(inout)	:	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks:
***********************************************************************************/
void EthSwtInit(void)
{
    /**
     * @brief ETAS BIP Integration Guide: EthSw Target dependent initialization.
     * Customer Integration Required.
     */
}

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
