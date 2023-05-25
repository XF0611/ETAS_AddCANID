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
 * Component : LogisticData_Types.h
 * Date      : Sep 21 2019
 * Version   : 1.0
 * Description  : This module implement header files for logisitc data for the platform.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 *                      This module shall be the same as the logistic data ram block used in bootloader.
 ***********************************************************************************************
*/
#ifndef LOGISTICDATA_TYPES_H
#define LOGISTICDATA_TYPES_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"


/*
 **********************************************************************************************************************
 * Type Definitions
 **********************************************************************************************************************
 */
typedef struct
{
    uint8  res[128];
} LogisticData_Type;

/*
 **********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************
 */


#endif /* LOGISTICDATA_TYPES_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
