
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.10.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/

/*
 ******************************************************************************
 * Includes
 ******************************************************************************
 */

#include "CanNm_Prv.h"
#include "CanNm_Cfg_Internal.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
CanNm_RamType CanNm_RamData_s[CANNM_NUMBER_OF_CHANNELS]; /* Data structure to hold RAM variables */
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#if (CANNM_EIRACALC_ENABLED != STD_OFF)

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
CanNm_TimerType CanNm_PNTimer_au32[CANNM_MAX_NUMBER_OF_PNS]; /* Timer for each PN */
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"
uint8 CanNm_EIRAGlobalStatus_au8[CANNM_PN_INFOLENGTH];
uint8 CanNm_EIRACurrentStatus_au8[CANNM_PN_INFOLENGTH];
#define CANNM_STOP_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"

#endif

#if (CANNM_ERACALC_ENABLED != STD_OFF)

#define CANNM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"
/* Timer for each PN across the channels for ERA calculation */
CanNm_TimerType CanNm_PNTimer_ERA_au32[CANNM_MAX_NUMBER_OF_PNS * CANNM_NUMBER_OF_ERA_CHANNELS];
#define CANNM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"
uint8 CanNm_ERAGlobalStatus_au8[CANNM_ERA_ARRAYSIZE];
uint8 CanNm_ERACurrentStatus_au8[CANNM_ERA_ARRAYSIZE];
#define CANNM_STOP_SEC_VAR_CLEARED_8
#include "CanNm_MemMap.h"

#endif

