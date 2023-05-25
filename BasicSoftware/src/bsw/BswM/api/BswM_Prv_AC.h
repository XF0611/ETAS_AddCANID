
#ifndef BSWM_PRV_AC_H
#define BSWM_PRV_AC_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#define BSWM_NO_RET_VALUE                          0x03
/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/
typedef void (*BswM_Action_FuncPtr_t) ( Std_ReturnType * action_RetVal_pu8 , const void * dataActionParameter_pv);

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define BSWM_START_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"

#if (defined(BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL) && (BSWM_NO_OF_AC_DEADLINE_MONITOR_CNTRL > 0))

extern boolean BswM_AC_DeadlineMonitoring_ExecFlag_b;
extern Com_IpduGroupVector BswM_AC_DeadlineMonitoring_IpduGroup;

#endif

#define BSWM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_MemMap.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

extern Std_ReturnType BswM_Prv_Evaluate_Action(const void * dataActionParameter_pv, BswM_ActionListItemType_ten isActionType_en);

#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_H */
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/
