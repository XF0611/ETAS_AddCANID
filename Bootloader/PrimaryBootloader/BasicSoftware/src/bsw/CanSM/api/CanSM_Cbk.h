
#ifndef  CANSM_CBK_H
#define  CANSM_CBK_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanSM_Cfg.h"
#include "CanIf.h"
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"

extern void CanSM_ControllerBusOff(uint8 ControllerId );

extern void CanSM_ControllerModeIndication(uint8 ControllerId,CanIf_ControllerModeType ControllerMode);

#if (CANSM_CFG_TRCV_CANIF_SUPPORT == STD_ON)
extern void CanSM_TransceiverModeIndication(uint8 TransceiverId,CanTrcv_TrcvModeType TransceiverMode);
#endif

#if(CANSM_PN_SUPPORT_CONFIGD == STD_ON)
extern void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver);

extern void CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver);

extern void CanSM_ConfirmPnAvailability(uint8 TransceiverId);
#endif

#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /*CANSM_CBK_H*/

