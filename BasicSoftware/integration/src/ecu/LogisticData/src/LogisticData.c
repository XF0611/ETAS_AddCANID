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
 * Component : NvBlock_ECU_LogisticData.c
 * Date      : Sep 21 2019
 * Version   : 1.0
 * Description  : This module implement logisitc data for the platform.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 *                      This module shall be the same as the logistic data ram block used in bootloader.
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "LogisticData_Types.h"
#include "Dcm.h"
#include "rba_BswSrv.h"


/*
 **********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************
 */
#define DEFAULT_PLATFORM_START_SEC_VAR_CLEARED_8BIT
#include "MemMap.h"
LogisticData_Type NvBlock_ECU_LogisticData;
uint8 NvBlock_ECU_Configuration[4];
#define DEFAULT_PLATFORM_STOP_SEC_VAR_CLEARED_8BIT
#include "MemMap.h"


/*
 **********************************************************************************************************************
 * Function Definitions
 **********************************************************************************************************************
 */

#define DiagUT_START_SEC_CODE
#include "DiagUT_MemMap.h"
/* user Integration required when DID access function required. */
#define DiagUT_STOP_SEC_CODE
#include "DiagUT_MemMap.h"

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
