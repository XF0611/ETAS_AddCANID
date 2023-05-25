
#ifndef BSWM_ECUM_H
#define BSWM_ECUM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "BswM_Cfg_General.h"


#if (defined(BSWM_ECUM_ENABLED) && (BSWM_ECUM_ENABLED == TRUE))

#include "EcuM.h"
//#if (!defined(ECUM_AR_RELEASE_MAJOR_VERSION) || (ECUM_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
//#error "AUTOSAR major version undefined or mismatched"
//#endif
//#if (!defined(ECUM_AR_RELEASE_MINOR_VERSION) || (ECUM_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
//#error "AUTOSAR minor version undefined or mismatched"
//#endif

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"
/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
extern void BswM_EcuM_CurrentState(EcuM_StateType CurrentState);
extern void BswM_EcuM_CurrentWakeup(EcuM_WakeupSourceType source, EcuM_WakeupStatusType state);
extern void BswM_EcuM_RequestedState(EcuM_StateType State, EcuM_RunStatusType CurrentStatus);


#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_ECUM_ENABLED */


#endif
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
