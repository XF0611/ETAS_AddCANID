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
 * Component : CanTrcv_Integration.h
 * Date      : Jul 4, 2019
 * Version   : 1.0
 * Description  : This module provides Can Transceiver TJA1145 integration code
 ***********************************************************************************************
*/
#ifndef CANTRCV_INTEGRATION_H
#define CANTRCV_INTEGRATION_H

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


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
extern FUNC(void, CANTRCV_CODE) CanTrcv_Init(const uint32* ConfigPtr);
extern FUNC(void, CANIF_CODE) CanTrcv_TJA1145_GotoSleep(VAR(uint8,AUTOMATIC) TransceiverId);
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"


#endif /* CANTRCV_INTEGRATION_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */

