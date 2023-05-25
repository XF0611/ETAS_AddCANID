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
 * Component : Ecu.h
 * Date      : April 8, 2021
 * Version   : 4.0
 * Description  : This module implements declaration of Ecu level functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
#ifndef ECU_H
#define ECU_H

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
 * Public Variables declarations
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define BSW_START_SEC_CODE
#include "Bsw_MemMap.h"
extern FUNC(void, ECU_CODE)Ecu_SwitchOff(void);
#define BSW_STOP_SEC_CODE
#include "Bsw_MemMap.h"


#endif /* ECU_H */


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
