

#ifndef ECUM_PRV_RTEACTIONS_H
#define ECUM_PRV_RTEACTIONS_H

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "EcuM_Types.h"
/*************************************************************************************************
 Variables
*************************************************************************************************
*/
#if (ECUM_CFG_MODE_HANDLING == STD_ON)

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

extern void EcuM_Prv_RteModeSwitch ( Rte_ModeType_EcuM_Mode Mode);

extern uint8 EcuM_Prv_GetRteMode(void);

#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#endif

#endif /*ECUM_PRV_RTEACTIONS_H*/
