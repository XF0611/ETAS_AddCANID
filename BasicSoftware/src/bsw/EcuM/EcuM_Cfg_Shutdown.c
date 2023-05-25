
#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "EcuM_RunTime.h" /*For run time measurement*/

#include "EcuM_Callout.h"

#include "EcuM_CompilerSpecific.h"

/*Extern module Headers*/
#if (ECUM_STARTUP_DURATION == TRUE)
#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
#include "NvM.h"                    /*for setramblockstatus api*/
// #if (!defined(NVM_AR_RELEASE_MAJOR_VERSION) || (NVM_AR_RELEASE_MAJOR_VERSION != 4))
// #error "AUTOSAR major version undefined or mismatched"
// #endif
// #if (!defined(NVM_AR_RELEASE_MINOR_VERSION) || (NVM_AR_RELEASE_MINOR_VERSION != 2))
// #error "AUTOSAR minor version undefined or mismatched"
// #endif
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED
#endif//ECUM_STARTUP_DURATION
#include "Ecu.h"
/* */
#include "EcuM_Integration.h"
/* */
#include "NvM_Integration.h"
/* */
#include "Rte_ExeMgrUT.h"
/* */
#include "TstM.h"
/* */

/*EcuM Private headers*/
#include "EcuM_Prv.h"

#define ECUM_START_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

/*Variable used to check,whether the execution reached the EcuM_OnGoOffOne() callout.*/
static volatile boolean EcuM_IsOnGoOffOneReached_b;

/*Variable used to check,whether the execution reached the EcuM_OnGoOffTwo() callout.*/
static volatile boolean EcuM_IsOnGoOffTwoReached_b;

/*Variable used to check,whether the execution reached the EcuM_AL_SwitchOff() callout.*/
static volatile boolean EcuM_IsALSwitchOffReached_b;

#define ECUM_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "EcuM_MemMap.h"

#if (ECUM_STARTUP_DURATION == TRUE) /*will activate the Run time measurement*/

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"
EcuM_TimeTicks_tu64 EcuM_Shutdown_begin_ticks_u64;
EcuM_TimeTicks_tu64 EcuM_Shutdown_end_ticks_u64;
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_MemMap.h"

#define ECUM_START_SEC_VAR_CLEARED_32
#include "EcuM_MemMap.h"
/*To store the time taken for shutdown / reset*/
uint32 EcuM_Shutdown_delta_us_u32;
#define ECUM_STOP_SEC_VAR_CLEARED_32
#include "EcuM_MemMap.h"

#endif//ECUM_STARTUP_DURATION

/*************************************************************************************************
 CALLOUTS
 *************************************************************************************************/
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"

/**********************************************************************************
 Function name     :   EcuM_OnGoOffOne
 Description       :   This call allows the system designer to notify that the GO OFF I
 state is about to be entered.
 Parameter (in)    :   None
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   None
 Remarks           :
 ***********************************************************************************/
void EcuM_OnGoOffOne(void) {
	/*Update Flag variable to indicate that the execution reached the particular APi call.
	 *Variable provided only for Debug purpose*/
	EcuM_IsOnGoOffOneReached_b = TRUE;

	(void) EcuM_GoOffOne();

}

/**********************************************************************************
 Function name     :   EcuM_OnGoOffTwo
 Description       :   This call allows the system designer to notify that the GO OFF II
 state is about to be entered.
 Parameter (in)    :   None
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   None
 Remarks           :
 ***********************************************************************************/
void EcuM_OnGoOffTwo(void) {
	/*Update Flag variable to indicate that the execution reached the particular APi call.
	 *Variable provided only for Debug purpose*/
	EcuM_IsOnGoOffTwoReached_b = TRUE;

#if (ECUM_STARTUP_DURATION == TRUE) /*will activate the Run time measurement*/
	EcuM_Shutdown_end_ticks_u64 = EcuM_GetTicks();
	EcuM_Shutdown_delta_us_u32 = EcuM_GetDuration(EcuM_Shutdown_end_ticks_u64,
			EcuM_Shutdown_begin_ticks_u64);

#if (ECUM_CFG_NVM_BLOCK_CONFIGURED == TRUE)
	/*update the RAM variable in NV memory to the current shutdown target*/
	EcuM_Rb_dataShutdownInfo_st.TimeMeasured = EcuM_Shutdown_delta_us_u32;
	(void) NvM_SetRamBlockStatus(NvMConf_NvMBlockDescriptor_ECUM_CFG_NVM_BLOCK,
			TRUE);
#endif//ECUM_CFG_NVM_BLOCK_CONFIGURED
#endif//ECUM_STARTUP_DURATION

	(void) TstM_PostRun();

	(void) NvM_Integration_WriteAll();

	(void) RE_ExeMgr_ExecuteShutdown();

}

/**********************************************************************************
 Function name     :   EcuM_AL_SwitchOff
 Description       :   This callout shall take the code for shutting off the power
 supply of the ECU. If the ECU cannot unpower itself, a reset
 may be an adequate reaction.
 Parameter (in)    :   None
 Parameter (inout) :   None
 Parameter (out)   :   None
 Return value      :   None
 Remarks           :
 ***********************************************************************************/
NORETURN void EcuM_AL_SwitchOff(void) {
	/*Update Flag variable to indicate that the execution reached the particular APi call.
	 *Variable provided only for Debug purpose*/
	EcuM_IsALSwitchOffReached_b = TRUE;

	(void) Ecu_SwitchOff();

}

#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_MemMap.h"
