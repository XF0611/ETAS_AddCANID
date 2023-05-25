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
 * Component : TstM.h
 * Date      : April 8, 2021
 * Version   : 4.0
 * Description  : This module implements declaration of Test manager functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/
#ifndef TSTM_H
#define TSTM_H

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
#define TSTM_START_SEC_CODE
#include "TstM_MemMap.h"
extern FUNC(void, TSTM_CODE)TstM_PreRun(void);
extern FUNC(void, TSTM_CODE)TstM_Run(void);
extern FUNC(void, TSTM_CODE)TstM_PostRun(void);
#define TSTM_STOP_SEC_CODE
#include "TstM_MemMap.h"


#endif /* TSTM_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
