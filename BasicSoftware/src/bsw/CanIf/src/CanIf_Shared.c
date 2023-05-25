


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"




/* Flag to check if CANIF is Initialized(TRUE) or Not Initialized (FALSE)*/
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
boolean CanIf_Prv_InitStatus_b;
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"


CanIf_ControllerStateType CanIf_Prv_ControllerState_ast[CANIF_TOTAL_CTRLS];

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

/*
 ***************************************************************************************************
 * Global Variables/Arrays
 ***************************************************************************************************
 */

/*RAM array for information about Pipe objects*/
#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
CanIf_XCore_PipeRuntimeInfoType_st CanIf_XCore_PipeRuntimeInfo_ast[CANIF_XCORE_MAX_NUM_PIPES];
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
boolean CanIf_XCore_EndBuf_InvalidFlag_b;

boolean CanIf_XCore_RxClrPipeFlag_b;
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"



#endif
/* FC_VariationPoint_END */
