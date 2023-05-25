
#ifndef BSWM_SD_H
#define BSWM_SD_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "BswM_Cfg_General.h"

#ifdef BSWM_SD_ENABLED
#if (BSWM_SD_ENABLED == TRUE)

#include "Sd.h"
//#if (!defined(SD_AR_RELEASE_MAJOR_VERSION) || (SD_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
//#error "AUTOSAR major version undefined or mismatched"
//#endif
//#if (!defined(SD_AR_RELEASE_MINOR_VERSION) || (SD_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
//#error "AUTOSAR minor version undefined or mismatched"
//#endif

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
extern void BswM_Sd_EventHandlerCurrentState(uint16 SdEventHandlerHandleId, Sd_EventHandlerCurrentStateType EventHandlerStatus);
extern void BswM_Sd_ClientServiceCurrentState(uint16 SdClientServiceHandleId, Sd_ClientServiceCurrentStateType CurrentClientState);
extern void BswM_Sd_ConsumedEventGroupCurrentState(uint16 SdConsumedEventGroupHandleId, Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState);


#endif //BSWM_SD_ENABLED == TRUE
#endif //BSWM_SD_ENABLED


#endif
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
