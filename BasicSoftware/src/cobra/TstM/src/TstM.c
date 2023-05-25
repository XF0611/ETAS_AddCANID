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
 * Component : TstM.c
 * Date      : April 8, 2021
 * Version   : 4.0
 * Description  : This module implements Test manager functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "TstM.h"
#include "Rte_TstM.h"
#include "StackM.h"
#include "Rtm.h"


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
#define TSTM_START_SEC_VAR_CLEARED_32
#include "TstM_MemMap.h"
VAR(StackM_StackMeasureType, AUTOMATIC) TstM_StackInfo[RTM_NUM_CORES][STACKM_NUMBEROFSTACK];    /* Stack assumption. */
VAR (uint16, RTM_VAR_CLEARED) TstM_SystemLoad[RTM_NUM_CORES];       /* system load. */
VAR (uint32, RTM_VAR_CLEARED) TstM_NvMImd_Timemon[255];
uint32 NvMImd_Timemon_max_us = 0;
uint32 NvMImd_Timemon_min_us = 4294967295;
#define TSTM_STOP_SEC_VAR_CLEARED_32
#include "TstM_MemMap.h"

#define TSTM_START_SEC_VAR_CLEARED_8
#include "TstM_MemMap.h"
uint8 NvMImd_Timemon_mon = 0;
#define TSTM_STOP_SEC_VAR_CLEARED_8
#include "TstM_MemMap.h"

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
#define TSTM_START_SEC_CODE
#include "TstM_MemMap.h"
/*******************************************************************************************
* Name              :   TstM_PreRun
* Description       :   Test Manager pre-run phase tests.
* Parameters[in]    :   void
* Parameters[out]   :   void
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
FUNC(void, TSTM_CODE)TstM_PreRun(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.8	Test Management
     * Customer Integration Required, dependent on specific ECU pre-run test strategy.
     * Please see the integration guide section for further details.
     */
}


/*******************************************************************************************
* Name              :   TstM_Run
* Description       :   Test Manager run phase tests.
* Parameters[in]    :   void
* Parameters[out]   :   void
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
FUNC(void, TSTM_CODE)TstM_Run(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.8	Test Management
     * User Integration Required, dependent on specific ECU run test strategy.
     * Please see the integration guide section for further details.
     */
    uint8 stackId;
    uint8_least coreCnt;

    for(coreCnt = 0; coreCnt <OS_NUM_OS_CORES; coreCnt++)
    {
        for(stackId = 0; stackId < STACKM_NUMBEROFSTACK;  stackId++)
        {
            Rte_Call_GetStackInfo_GetStackInfo(coreCnt, stackId, &TstM_StackInfo[coreCnt][stackId]);
        }
    }

    for(coreCnt = 0; coreCnt < RTM_NUM_CORES; coreCnt++)
    {
        TstM_SystemLoad[coreCnt] = Rtm_GetSystemLoad(coreCnt);
    }

}


/*******************************************************************************************
* Name              :   TstM_PostRun
* Description       :   Test Manager post-run phase tests.
* Parameters[in]    :   void
* Parameters[out]   :   void
* Limitations       :   Care has to be taken while calling this function as it is not reentrant.
* ReturnType        :   void
*******************************************************************************************/
FUNC(void, TSTM_CODE)TstM_PostRun(void)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.8	Test Management
     * Customer Integration Required, dependent on specific ECU post-run test strategy. 
     * Please see the integration guide section for further details.
     */
}
#define TSTM_STOP_SEC_CODE
#include "TstM_MemMap.h" /*lint !e537 permit multiple inclusion */


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
