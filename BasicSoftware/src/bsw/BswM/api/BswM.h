
#ifndef BSWM_H
#define BSWM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Std_Types.h"
#include "ComStack_Types.h"


#include "BswM_Types.h"
#include "BswM_Cfg_General.h"
#include "BswM_PBcfg.h"
#include "BswM_Cfg_LE.h"
#include "BswM_Cfg_RL.h"
#include "BswM_Cfg_AC.h"
#include "BswM_Cfg_MRP.h"
#include "BswM_Generic.h"

#if (BSWM_RTE_IN_USE == TRUE)
#include "SchM_BswM.h"
#else
extern void BswM_MainFunction(void);
#endif
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define BSWM_START_SEC_CODE
#include "BswM_MemMap.h"

extern void BswM_Init(const BswM_ConfigType * ConfigPtr);
extern void BswM_Deinit(void);

#ifdef BSWM_VERSIONINFO_API
#if(BSWM_VERSIONINFO_API != STD_OFF)

extern void BswM_GetVersionInfo(Std_VersionInfoType * VersionInfo);

#endif /* ( BSWM_VERSIONINFO_API != STD_OFF ) */
#endif /*  BSWM_VERSIONINFO_API  */


#define BSWM_STOP_SEC_CODE
#include "BswM_MemMap.h"

#endif /* BSWM_H */
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/