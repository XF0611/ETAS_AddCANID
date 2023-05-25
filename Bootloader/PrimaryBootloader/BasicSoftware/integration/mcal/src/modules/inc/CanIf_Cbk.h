#ifndef CANIF_CBK_H
#define CANIF_CBK_H

#include "Platform_Types.h"
#include "Can_GeneralTypes.h"
#include "ComStack_Types.h"

extern void CanIf_ControllerModeIndication(uint8 ControllerId, CanIf_ControllerModeType CanIfMode);

extern void CanIf_TxConfirmation(PduIdType CanTxPduId);

extern void CanIf_RxIndication(const Can_HwType *Mailbox, const PduInfoType *PduInfoPtr);
extern Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType * PduInfoPtr);
extern void CanIf_ControllerBusOff(uint8 ControllerId);


#endif
