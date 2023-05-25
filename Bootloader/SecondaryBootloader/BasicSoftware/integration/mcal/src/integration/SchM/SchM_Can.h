#ifndef SCHM_CAN_H
#define SCHM_CAN_H

extern void SchM_Enter_Can_SetControllerMode();
extern void SchM_Exit_Can_SetControllerMode();
extern void SchM_Enter_Can_SetBaudrate();
extern void SchM_Exit_Can_SetBaudrate();
extern void SchM_Enter_Can_EnableControllerInterrupts();
extern void SchM_Exit_Can_EnableControllerInterrupts();
extern void SchM_Enter_Can_DisableControllerInterrupts();
extern void SchM_Exit_Can_DisableControllerInterrupts();
extern void SchM_Enter_Can_Write();
extern void SchM_Exit_Can_Write();

#endif
