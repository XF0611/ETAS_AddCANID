

#ifndef ECUM_CBK_H
#define ECUM_CBK_H

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/

#include "Std_Types.h" /*Rule BSW_HeaderInc_004:*/

#include "EcuM_Types.h" /* Required for EcuM exported data types */

#include "EcuM_Generated_Types.h" /* specified in SWS. */

#include "EcuM_Cfg_Version.h" /*Version details for the implementation*/

#include "EcuM_Cfg_CbkStartup.h"

/**************************************************************************************************/
/* ALL DECLARATIONS NECESSARY TO INTERACT WITH CALLBACKS AND CALLOUTS OF ECUM MODULE                                                                          */
/**************************************************************************************************/

#define ECUM_START_SEC_CODE
#include "EcuM_MemMap.h"

/*************************************************************************************************
 CALLBACK FUNCTION PROTOTYPES
*************************************************************************************************/

extern void EcuM_SetWakeupEvent( EcuM_WakeupSourceType sources );

extern void EcuM_ValidateWakeupEvent( EcuM_WakeupSourceType sources );

extern void EcuM_CheckWakeup( EcuM_WakeupSourceType wakeupSource );

extern void EcuM_EndCheckWakeup( EcuM_WakeupSourceType sources );


#define ECUM_STOP_SEC_CODE
#include "EcuM_MemMap.h"

#endif /*ECUM_CBK_H*/

