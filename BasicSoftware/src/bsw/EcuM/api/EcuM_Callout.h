


#ifndef ECUM_CALLOUT_H
#define ECUM_CALLOUT_H

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "EcuM_Cfg_Startup.h" /* Required for EcuM post build structures */

/**************************************************************************************************/
/* ALL DECLARATIONS NECESSARY TO INTERACT WITH CALLBACKS AND CALLOUTS OF ECUM MODULE                                                                          */
/**************************************************************************************************/
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"

/*************************************************************************************************
 CALLOUT FUNCTION PROTOTYPES
*************************************************************************************************/

extern void EcuM_ErrorHook( uint16 reason );

#if (ECUM_INTERRUPTS_ENABLE == TRUE)

extern void EcuM_AL_SetProgrammableInterrupts( void );

#endif//ECUM_INTERRUPTS_ENABLE

extern void EcuM_AL_DriverInitZero( void );

extern void EcuM_AL_DriverInitOne(
                                        const EcuM_ConfigType * ConfigPtr );


#if  (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)

extern void EcuM_AL_DriverRestart (
                                        const EcuM_ConfigType * ConfigPtr );

#endif//ECUM_SLEEP_SUPPORT_ENABLE

extern const EcuM_ConfigType * EcuM_DeterminePbConfiguration( void );

extern void EcuM_OnGoOffOne( void );

extern void EcuM_OnGoOffTwo( void );

extern void EcuM_AL_SwitchOff( void );

extern void EcuM_AL_Reset( EcuM_ResetType reset );

extern void EcuM_StartWakeupSources( EcuM_WakeupSourceType wakeupSource );

extern void EcuM_StopWakeupSources( EcuM_WakeupSourceType wakeupSource );

extern void EcuM_CheckValidation( EcuM_WakeupSourceType wakeupSource );

extern void EcuM_SwitchOsAppMode( void );

#if  (ECUM_SLEEP_SUPPORT_ENABLE == TRUE)

extern void EcuM_EnableWakeupSources ( EcuM_WakeupSourceType wakeupSource );

extern void EcuM_DisableWakeupSources ( EcuM_WakeupSourceType wakeupSource );

extern uint8 EcuM_CheckRamHash (void);

extern void EcuM_GenerateRamHash(void);

extern void EcuM_StartCheckWakeup( EcuM_WakeupSourceType wakeupSource);

extern void EcuM_SleepActivity(void);
#endif//ECUM_SLEEP_SUPPORT_ENABLE

#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"

#endif /*ECUM_CALLOUT_H*/

