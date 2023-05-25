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
 * Component : Xcp_Integration.c
 * Date      : Jan 16 2020
 * Version   : 1.0
 * Description  : This module implements XCP integration definitions and declarations.
 ***********************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Xcp_Integration.h"


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
CalPage_T	XcpCurrentCalPage = 0;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"


/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
