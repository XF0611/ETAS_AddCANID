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
 * Component : CanTrcv_Integration.c
 * Date      : Jun 11 2019
 * Version   : 1.0
 * Description  : This module implements Cantrcv integration.
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "CanTrcv_Integration.h"
//#include "CanTrcv_Prv.h"

/*
 **********************************************************************************************************************
 * Define/Macros
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/**********************************************************************************
  Function name		  :	CanTrcv_Init
  Description		  :	This is a stub function
  Parameter	(in)	  :	None
  Parameter	(inout)	:	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks:
***********************************************************************************/
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANTRCV_CODE) CanTrcv_Init(const uint32* ConfigPtr)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.3	Can Tranceiver
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"


/**********************************************************************************
  Function name		  :	CanTrcv_TJA1145_GotoSleep
  Description		  :	This is a stub function
  Parameter	(in)	  :	TransceiverId: Transceiver ID
  Parameter	(inout)	:	None
  Parameter	(out)	  :	None
  Return value		  :	None
  Remarks:
***********************************************************************************/
#define CANTRCV_START_SEC_CODE
#include "CanTrcv_MemMap.h"
FUNC(void, CANIF_CODE) CanTrcv_TJA1145_GotoSleep(VAR(uint8,AUTOMATIC) TransceiverId)
{
    /**
     * @brief ETAS BIP Integration Guide: 6.3.3	Can Tranceiver
     * Customer Integration Required.
     * Please see the integration guide section for further details.
     */
}
#define CANTRCV_STOP_SEC_CODE
#include "CanTrcv_MemMap.h"


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
