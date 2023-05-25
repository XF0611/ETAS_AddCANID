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
 * Component : Fls_Integration.h
 * Date      : June 1 2021
 * Version   : 1.0
 * Description  : This module implements MCAL integration declarations and definitions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables.
 ***********************************************************************************************
*/

#ifndef FLS_INTEGRATION_H
#define FLS_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Fls.h"


/**
 * @brief ETAS BIP Integration Guide: 6.2.4	Fls-MCAL Integration
 * Customer Integration Required.
 * Please see the integration guide section for further details.
 */

/*
 **********************************************************************************************************************
 * Version Information
 **********************************************************************************************************************
*/
/* ETAS BIP Porting: no redeclaration of FLS_AR_RELEASE_xxx_VERSION required.*/

/*
 **********************************************************************************************************************
 * Defines
 **********************************************************************************************************************
 */
#define FLS_BASE_ADDRESS    FlsBaseAddress       /* [$BIP_INTG 098]: no redeclaration of FLS_BASE_ADDRESS required as Fls_Read already implemented.*/

/* ETAS BIP Porting: no redeclaration of Fls_Config required for vendor specific prefix.*/
/* ETAS BIP Porting: no redeclaration of Fls_LengthType, Fls_AddressType, Fls_ConfigType required for vendor specific prefix.*/

/* [$BIP_INTG 099]: no redeclaration of API Fls_MainFunction, Fls_Init, Fls_Read, Fls_Write, Fls_Erase, Fls_GetJobResult,
   Fls_Compare, Fls_BlankCheck required for vendor specific prefix.
 */


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
extern FUNC(void, FLS_APPL_CODE) Fee_JobEndNotification(void);
extern FUNC(void, FLS_APPL_CODE) Fee_JobErrorNotification(void);

#endif /* FLS_INTEGRATION_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
