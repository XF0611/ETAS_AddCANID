#ifndef DCM_CFG_TIMER_H
#define DCM_CFG_TIMER_H

/* This file shall be used only when the OS Timer is configured as AutosarSpecific/ProjectSpecific
 * This template shows the implementation for using Autosar OS, Project Specific OS and also Mcu counter ,
 * The user shall take full responsibility of using this template
 * If the project wants to use, Autosar OS, please include the file Os.h
 * If the project wants to use, mcu counter, then include file mcu.h
 * If the project wants to use project specific os, then include file os_bsw.h
 */

#include "Os_Port.h"
#if( DCM_CFG_OSTIMER_USE != FALSE )
#include "ComStack_Types.h"
#include "Std_Types.h"


/* Sample implementation for Autosar OS*/
#include "Os.h"

#define OS_GET_SYSTEM_COUNTER_US(value) OS_TICKS2US_Millisecond_TickCounter(value)

LOCAL_INLINE FUNC(Std_ReturnType,DCM_APPL_CODE) Dcm_GetCounterValue(VAR(uint32,AUTOMATIC) CounterId, P2VAR(uint32,AUTOMATIC,DCM_INTERN_DATA) Value);

/**
 * @ingroup DCM_TPL
 * Dcm_GetCounterValue :-\n
 * This API is used to get the OS ticks for the autosar specific/Mcu OS counter \n
 *
 * @param[in]     CounterId : OS timer counter Id
 * @param[out]    Value    : OS tick count
 *
 * @retval     E_OK :    OS ticks are proper
 * @retval     E_NOT_OK :    OS ticks are not proper
 *
 */
LOCAL_INLINE FUNC(Std_ReturnType,DCM_APPL_CODE) Dcm_GetCounterValue(VAR(uint32,AUTOMATIC) CounterId, P2VAR(uint32,AUTOMATIC,DCM_INTERN_DATA) Value)
{
    VAR(Std_ReturnType,AUTOMATIC) retval_u8 = E_NOT_OK;
    retval_u8 = Os_GetCounterValue(Millisecond_TickCounter, (TickRefType)Value);
    return (retval_u8);
    
}


#endif
#endif /* _DCM_CFG_TIMER_H*/
