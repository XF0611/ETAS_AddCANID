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
 * Component : EcuM_Integration.h
 * Date      : Dec 20, 2019
 * Version   : 2.0
 * Description  : This module implements EcuM integration functions
 ***********************************************************************************************
*/
#ifndef INTEGRATION_API_ECUM_INTEGRATION_H
#define INTEGRATION_API_ECUM_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


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
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"
void EcuM_GoOffOne(void);
#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"


#endif /* INTEGRATION_API_ECUM_INTEGRATION_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
