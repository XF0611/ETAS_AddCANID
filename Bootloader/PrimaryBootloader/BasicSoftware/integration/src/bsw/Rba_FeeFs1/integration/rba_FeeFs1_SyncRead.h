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
 * Component : rba_FeeFs1_SyncRead.h
 * Date      : Dec 05 2019
 * Version   : 1.0
 * Description  : This is header file of Fee integration functions during platform integration.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables.
 ***********************************************************************************************
*/

/* BSW-1915 */
#ifndef RBA_FEEFS1_SYNCREAD_H
#define RBA_FEEFS1_SYNCREAD_H

#include "Fls.h"
/* BSW-15413 */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
/* END BSW-15413 */
extern Std_ReturnType Fee_Fls_SyncRead( Fls_AddressType SourceAddress,
                                uint8 *TargetAddressPtr,
                                Fls_LengthType Length
                               );

#endif /* RBA_FEEFS1_SYNCREAD_H */
/* END BSW-1915 */
/* BSW-15413 */
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
/* END BSW-15413 */