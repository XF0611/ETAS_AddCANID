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
#ifndef OVRLYM_H
#define OVRLYM_H


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
#define CAL_MEMORY_ONE_PAGE    0UL
#define CAL_MEMORY_TWO_PAGE    1UL
#define CAL_MEMORY_MGNT        CAL_MEMORY_ONE_PAGE

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
extern void Overlay_Init(void);
extern void Overlay_Enable(void);
extern void Overlay_Disable(void);
extern void Overlay_Sync(void);


#endif /*OVRLYM_H*/

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
