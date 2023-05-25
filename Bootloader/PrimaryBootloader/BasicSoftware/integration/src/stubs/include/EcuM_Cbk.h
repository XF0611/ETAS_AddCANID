

#ifndef ECUM_CBK_H
#define ECUM_CBK_H

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/

#include "Std_Types.h" /*Rule BSW_HeaderInc_004:*/

#include "EcuM_Types.h" /* Required for EcuM exported data types */


/**************************************************************************************************/
/* ALL DECLARATIONS NECESSARY TO INTERACT WITH CALLBACKS AND CALLOUTS OF ECUM MODULE                                                                          */
/**************************************************************************************************/

/************************************************************************************************
 CALLBACK FUNCTION PROTOTYPES
*************************************************************************************************/

extern FUNC( void, ECUM_CODE ) EcuM_SetWakeupEvent( VAR(EcuM_WakeupSourceType, AUTOMATIC) sources );

extern FUNC( void, ECUM_CODE ) EcuM_ValidateWakeupEvent( VAR(EcuM_WakeupSourceType, AUTOMATIC) sources );

extern FUNC( void, ECUM_CALLOUT_CODE ) EcuM_CheckWakeup( VAR(EcuM_WakeupSourceType, AUTOMATIC) wakeupSource );


#endif /*ECUM_CBK_H*/

