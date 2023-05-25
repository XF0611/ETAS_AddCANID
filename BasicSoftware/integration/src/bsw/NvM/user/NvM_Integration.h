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
 * Component : NvM_Integration.h
 * Date      : Jan 16 2020
 * Version   : 2.0
 * Description  : This module implements NvM integration declarations
 ***********************************************************************************************
*/
#ifndef INTEGRATION_API_NVM_INTEGRATION_H
#define INTEGRATION_API_NVM_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */
#define NVM_INTEGRATION_AR_RELEASE_MAJOR_VERSION 	4
#define NVM_INTEGRATION_AR_RELEASE_MINOR_VERSION	2


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
void NvM_Integration_ReadAll(void);
void NvM_Integration_WriteAll(void);
#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"


#endif /* INTEGRATION_API_NVM_INTEGRATION_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
