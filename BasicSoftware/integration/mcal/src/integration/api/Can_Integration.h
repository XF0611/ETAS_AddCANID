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
 * Component : Can_Integration.h
 * Date      : Oct. 14 2021
 * Version   : 1.1
 * Description  : This module implements MCAL integration declarations and definitions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables.
 ***********************************************************************************************
*/

#ifndef CAN_INTEGRATION_H
#define CAN_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Can.h"

/**
 * @brief ETAS BIP Integration Guide: 6.2.2	CanIf-CAN Integration
 * Customer Integration Required.
 * Please see the integration guide section for further details.
 * [$Satisfies $BIP_INTG 093]
 */

/*
 **********************************************************************************************************************
 * Version Information
 **********************************************************************************************************************
*/
/* Porting: To integrate with RTA-CAR9, this MCAL version macro AR 4.3.1 has been converted to AR4.2.2 in MCAL Modules. */

/*
 **********************************************************************************************************************
 * Defines
 **********************************************************************************************************************
 */
#define Can_Config              CanConfigData

/* [$Satisfies $BIP_INTG 094] Porting: This MCAL provides standard interface devinition which requires no porting. */


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */


#endif /* CAN_INTEGRATION_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
