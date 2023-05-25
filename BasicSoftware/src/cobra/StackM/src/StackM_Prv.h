/*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
**********************************************************************************************************************
 ************************************************************************************************
 * Component : StackM_Prv.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_PRV_H
#define STACKM_PRV_H

/*! \file
 *
 * \brief Stack Monitor private header
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "StackM.h"
#include "StackM_Cfg_SchM.h"
#include "StackM_Os_Itf.h"


/*
 ***********************************************************************************************************************
 * Version check
 ***********************************************************************************************************************
 */
#if ((STACKM_CFG_ENABLE != STD_ON) && (STACKM_CFG_ENABLE != STD_OFF))
   #error ('STACKM_CFG_ENABLE must be STD_ON or STD_OFF. No other values are allowed.')
#endif

#if ((STACKM_CFG_ENABLETRACE != STD_ON) && (STACKM_CFG_ENABLETRACE != STD_OFF))
   #error ('STACKM_CFG_ENABLETRACE must be STD_ON or STD_OFF. No other values are allowed.')
#endif

#if ((STACKM_CFG_PATTERNINIT != STD_ON) && (STACKM_CFG_PATTERNINIT != STD_OFF))
   #error ('STACKM_CFG_PATTERNINIT must be STD_ON or STD_OFF. No other values are allowed.')
#endif


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/
typedef boolean (*SkipAddrScan_CbkType)(const uint32* addr);

/** StackM_StackCfgType
 * \brief Type which contains configuration data for the StackM module.
 *
 *  - CoreId: Id of the core to which the given stack belongs.
 *  - Start:  address of the first 32 bit word of the stack.
 *  - End:    address of the last 32 bit word of the stack.
 *  - Limit0: adress on which the 64 bit check word 0 will be written.
 *  - Limit1: adress on which the 64 bit check word 1 will be written.
 *  - Limit2: adress on which the 64 bit check word 2 will be written.
 */
typedef struct
{
   uint32  CoreId;
   uint32* Start;
   uint32* End;
   uint32* Limit0;
   uint32* Limit1;
   uint32* Limit2;
	SkipAddrScan_CbkType SkipAddrScan_Cbk;
} StackM_StackCfgType;

/** StackM_StackDynType
 * \brief Type which contains runtime data for the StackM module.
 *
 *  - ScanResult: last measurement stack consumption.
 *  - CurrentAddress: current internal index of the stack scanning.
 *  - IniFlag: initialization flag. This flag is set once a stack was initialized.
 */
typedef struct
{
   uint32* ScanResult;
   uint32* CurrentAddress;
   uint8   IniFlag;
} StackM_StackDynType;

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define STACKM_LIMIT_NOTUSED  ((uint32*)0)
#define STACKM_CBK_NOTUSED    (SkipAddrScan_CbkType)0

#define STACKM_PRV_OFFSET (0x10)    /* A reletive correct stack offset for a typical function call consumed by the init funciton. */

#define StackM_IsSPWithinStack(coreId, StackId, spNow)    (StackM_StackCfgTable[coreId][StackId].Start >= (spNow))&&(StackM_StackCfgTable[coreId][StackId].End <= (spNow))

/* MACRO for DET Support */
#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
/**
 * Macro for functions with return type as void
 */
#define STACKM_DET_REPORT_ERROR(ApiId, ErrorId)       (void)Det_ReportError((STACKM_MODULE_ID),(STACKM_INSTANCE_ID),(ApiId),(ErrorId))

#else
/**
 * Macro to handle DET error report if device error detection is disabled
 */
#define STACKM_DET_REPORT_ERROR(InstanceId, ApiId, ErrorId)       do{}while(0)
#endif

/*
 ***********************************************************************************************************************
 * Extern declarations
 ***********************************************************************************************************************
 */
#define StackM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "StackM_MemMap.h"
extern VAR(StackM_StackDynType, STACKM_VAR_CLEARED) StackM_StackDynTable[OS_NUM_OS_CORES][STACKM_NUMBEROFSTACK];
#define StackM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "StackM_MemMap.h"

#define StackM_START_SEC_CONST_UNSPECIFIED
#include "StackM_MemMap.h"
extern CONST(StackM_StackCfgType, STACKM_VAR_CONST) StackM_StackCfgTable[OS_NUM_OS_CORES][STACKM_NUMBEROFSTACK];
#define StackM_STOP_SEC_CONST_UNSPECIFIED
#include "StackM_MemMap.h"


#endif /* STACKM_PRV_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

