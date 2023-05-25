
#ifndef BSWM_LINSM_H
#define BSWM_LINSM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "BswM_Cfg_General.h"

#ifdef BSWM_LINSM_ENABLED
#if ( BSWM_LINSM_ENABLED == TRUE )


#include "LinSM.h"
//#if (!defined(LINSM_AR_RELEASE_MAJOR_VERSION) || (LINSM_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
//#error "AUTOSAR major version undefined or mismatched"
//#endif
//#if (!defined(LINSM_AR_RELEASE_MINOR_VERSION) || (LINSM_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
//#error "AUTOSAR minor version undefined or mismatched"
//#endif

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
extern void BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule);
extern void BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState);

#endif /* ( BSWM_LINSM_ENABLED == TRUE ) */
#endif /* BSWM_LINSM_ENABLED */

#endif
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
