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
 * Component : BswM_Integration.h
 * Date      : Jul 4, 2019
 * Version   : 1.0
 * Description  : This module provides interface for BswM integration
 ***********************************************************************************************
*/
#ifndef BSWM_INTEGRATION_H
#define BSWM_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "BswM.h"
#include "EcuM.h"


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

void BswM_StartupTwo_CddInit(void);
void BswM_Run_CddInitZero(void);
void BswM_Enable_PeripheralSply(void);
void BswM_Run_CddInitOne(void);
void BswM_PerformSelfHold(void);
void BswM_CanTrcv_GotoNormal(void);
void BswM_CanTrcv_GotoSleep(void);
void BswM_SelfHold_Unlock(void);
void EthSwtInit(void);
void PowerOn_Switch(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"


#endif /* BSWM_INTEGRATION_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
