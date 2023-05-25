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
 * $Name______:BswM_ComM$
 * $Variant___:GITTRANS$
 * $Revision__:7$
 **********************************************************************************************************************
</BASDKey>*/

#ifndef BSWM_COMM_H
#define BSWM_COMM_H

#include "BswM.h"

#include "ComM.h"
#if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#define BSWM_START_SEC_CODE
#include "MemMap.h"
/* \brief Function called by ComM to indicate the current communication mode of a ComM channel. */
extern void BswM_ComM_CurrentMode( NetworkHandleType Network, ComM_ModeType RequestedMode );

#ifdef BSWM_RB_COMMPNC_ENABLED
#if ( BSWM_RB_COMMPNC_ENABLED == TRUE )
/* \brief Function called by ComM to indicate the current mode of the PNC. */
extern void BswM_ComM_CurrentPNCMode( PNCHandleType PNC, ComM_PncModeType RequestedMode );
#endif
#endif

#define BSWM_STOP_SEC_CODE
#include "MemMap.h"


#endif /* BSWM_COMM_H_ */

/*
<BASDKey>
 **********************************************************************************************************************
 * Administrative Information (automatically filled in)
 * $History___:
 * 
 * GITTRANS; 7     18.12.2012 SIH6KOR
 *   Release version number updated at correct place.
 * 
 * GITTRANS; 6     01.10.2012 AJR1KOR
 *   Reviewed and review comments implemented.
 * 
 * GITTRANS; 5     17.08.2012 AJR1KOR
 *   CSCRM00439241 Rework review findings from previous delivery
 * 
 * GITTRANS; 4     29.06.2012 CMS2SI
 *   Update of BswM_ComM.h (PNCType not defined in current ComStack)
 *   BASD_TRANSFER_2012_06
 * 
 * GITTRANS; 3     28.06.2012 AJR1KOR
 *   BASD_TRANSFER_2012_06
 * 
 * GITTRANS; 2     19.06.2012 AJR1KOR
 *   Updated form git : 20120619
 *   SHA1 ID:12bf9995072a3667a6f07d99bcad9fa4b73003f7
 * 
 * GITTRANS; 1     13.06.2012 AJR1KOR
 *   Update transfer from git 13/06/2012
 * 
 * GITTRANS; 0     16.05.2012 CMS2SI
 *   BASD_TRANSFER_2012_05
 * 
 * AR40.0.2.0; 0     04.04.2012 CMS2SI
 *   Renamed P_AR40.0.1.0 to AR40.0.2.0.
 * 
 * P_AR40.0.1.0; 0     22.03.2012 CMS2SI
 *   Pre-Delivery, Git transfer 2012_03
 *   Caveats: API stubs only, configuration and respective code generation not 
 *   available, and therefore no configurable mode request check, mode 
 *   arbitration, action list execution.
 * 
 * $
 **********************************************************************************************************************

 **********************************************************************************************************************
 * End of header file: $Name______:BswM_ComM$
 **********************************************************************************************************************
</BASDKey>
*/

