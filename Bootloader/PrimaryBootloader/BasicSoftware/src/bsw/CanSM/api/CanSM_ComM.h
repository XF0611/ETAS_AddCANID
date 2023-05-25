
#ifndef  CANSM_COMM_H
#define  CANSM_COMM_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Cfg.h"
#include "ComM.h"

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"

extern Std_ReturnType CanSM_RequestComMode( NetworkHandleType network, ComM_ModeType ComM_Mode );

extern Std_ReturnType CanSM_GetCurrentComMode( NetworkHandleType network,ComM_ModeType * ComM_ModePtr );


#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /* CANSM_COMM_H*/

