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
 * Component : StackM.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_H
#define STACKM_H

/*! \file
 *
 * \brief Stack Monitor external declarations.
 *
 * This file contains the declaration that other modules will reference.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "StackM_Cfg.h"


/*
 **********************************************************************************************************************
 * Version Information
 **********************************************************************************************************************
*/
/* Version identification */
#define STACKM_VENDOR_ID                                     (6u)
#define STACKM_MODULE_ID                                     (255u)
#define STACKM_INSTANCE_ID                                   (0)

/* AUTOSAR specification version information */
#define STACKM_AR_RELEASE_MAJOR_VERSION                      4u
#define STACKM_AR_RELEASE_MINOR_VERSION                      2u
#define STACKM_AR_RELEASE_REVISION_VERSION                   2u

/* Version of StackM specification */
#define STACKM_SW_MAJOR_VERSION                              1u
#define STACKM_SW_MINOR_VERSION                              0u
#define STACKM_SW_PATCH_VERSION                              0u


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Development Error Codes for DET Support*/
#if (STACKM_DEV_ERROR_DETECT != STD_OFF)
#define STACKM_E_LIMIT                                       0x01 /* API service called with stack out of range */
#define STACKM_E_NO_INIT                                     0x02 /* API service used without module initialization */
#define STACKM_E_INVALID_ID                                  0x03 /* API service called with invalid stack ID */
#define STACKM_E_INVALID_CORE_ID                             0x04 /* API service called with invalid core ID */
#endif

/* Service IDs of StackM APIs */
/* Indicates the Api Id passed in the call of Det_ReportError for corresponding API's*/
#define STACKM_SID_INIT                                      (0x00)
#define STACKM_SID_SCANSTACK                                 (0x01)


/*
 ***********************************************************************************************************************
 * External declarations
 ***********************************************************************************************************************
 */
#if(STACKM_CFG_ENABLE == STD_ON)

    #define StackM_START_SEC_CODE
    #include "StackM_MemMap.h"

    FUNC(void, STACKM_CODE) StackM_InitCoreStack(void);
    FUNC(void, STACKM_CODE) StackM_InitAllCoreStack(void);
    FUNC(void, STACKM_CODE) StackM_ScanCoreStack(void);
    FUNC(void, STACKM_CODE) StackM_ScanAllStacks(void);
    FUNC(Std_ReturnType, STACKM_CODE) StackM_GetStackInfo(uint32 coreId, uint8 StackId, StackM_StackMeasureType* pStackInfo);
    
    LOCAL_INLINE uint32 StackM_IsLimitExceeded(const StackM_StackMeasureType* pStackInfo, uint8 Level)
	{
		return ((pStackInfo->CheckWordReached & (1 << (Level))) >> (Level));
	}

    #define StackM_STOP_SEC_CODE
    #include "StackM_MemMap.h"

#else

    #define StackM_InitCoreStack()                              (STACKM_OK)
    #define StackM_InitAllCoreStack()                           (STACKM_OK)
    #define StackM_ScanCoreStack()                              (STACKM_OK)
    #define StackM_ScanAllStacks()                              (STACKM_OK)
    #define StackM_GetStackInfo(coreId, StackId, pStackInfo)    (STACKM_OK)
    
    #define StackM_IsLimitExceeded(StackInfo, Level) (0)

#endif

#endif /* STACKM_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

