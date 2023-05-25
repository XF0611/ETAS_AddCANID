/*<BASDKey>
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, 2010 Robert Bosch GmbH. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 * Administrative Information (automatically filled in)
 * $Domain____:BASD$
 * $Namespace_:\Comp\BswM$
 * $Class_____:H$
 * $Name______:BswM_CanSM$
 * $Variant___:GITTRANS$
 * $Revision__:5$
 **********************************************************************************************************************
</BASDKey>*/

#ifndef BSWM_CANSM_H
#define BSWM_CANSM_H

#include "BswM.h"
#include "CanSM_BswM.h"
#include "CanSM.h"
#if (!defined(CANSM_AR_RELEASE_MAJOR_VERSION) || (CANSM_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANSM_AR_RELEASE_MINOR_VERSION) || (CANSM_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#define BSWM_START_SEC_CODE
#include "MemMap.h"
/* \brief Function called by CanSM to indicate its current state. */
extern void BswM_CanSM_CurrentState( NetworkHandleType Network, CanSM_BswMCurrentStateType CurrentState );

#define BSWM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* BSWM_CANSM_H */
