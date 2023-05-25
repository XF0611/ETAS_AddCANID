/*********************************************************************************************************************
*
* Description:     BootM module API
* FileName:        FBL_BootMCfg.h
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
*/
#ifndef  FBL_BOOTMCFG_H
#define  FBL_BOOTMCFG_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "NvM.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_ON   ((Fbl_BootM_ReprogrammingRequestFlagStateType)0xB8B7B6B5U)
#define FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_OFF  ((Fbl_BootM_ReprogrammingRequestFlagStateType)0x00U)


#define FBL_BOOTM_CALIB_VALIDITY_TRUE                   ((ImageM_CalibrationValidFlagType)0xFFFFFFC8U)
#define FBL_BOOTM_CALIB_VALIDITY_FALSE                  ((ImageM_CalibrationValidFlagType)0xFFFFFF00U)
#define FBL_BOOTM_VALIDITY_TRUE                         ((ImageM_ValidateFlagType)0x00C6C7C8U)
#define FBL_BOOTM_VALIDITY_TRUE_ARC                     ((ImageM_ValidateFlagType)0x0000F7C8U)
 
#define FBL_BOOTM_APPLICATION_0_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0xFFFF07FFU) 
#define FBl_BOOTM_APPLICATION_1_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0xFFFFC0FFU) 
#define FBL_BOOTM_APPLICATION_2_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0xFF06FFFFU) 
#define FBl_BOOTM_APPLICATION_3_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0xFFC0FFFFU) 
#define FBL_BOOTM_APPLICATION_4_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0x05FFFFFFU) 
#define FBl_BOOTM_APPLICATION_5_VALIDITY_TRUE           ((ImageM_ApplicationValidFlagType)0xC0FFFFFFU) 
#define FBL_BOOTM_APPLICATION_0_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0xFFFFF0FFU) 
#define FBL_BOOTM_APPLICATION_1_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0xFFFF0FFFU)
#define FBL_BOOTM_APPLICATION_2_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0xFFF0FFFFU)
#define FBL_BOOTM_APPLICATION_3_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0xFF0FFFFFU)
#define FBL_BOOTM_APPLICATION_4_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0xF0FFFFFFU)
#define FBL_BOOTM_APPLICATION_5_VALIDITY_FALSE          ((ImageM_ApplicationValidFlagType)0x0FFFFFFFU)
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef uint32  Fbl_BootM_ReprogrammingRequestFlagStateType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/
/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

/**
 * @brief Read reprograming request flag
 *
 * @param flag [out] a pointer is written into.
 * @return Std_ReturnType result is E_OK        - succeeded.
 *                                  E_NOT_OK    - Failed.
 */
extern Std_ReturnType Fbl_BootM_ReadReprogrammingRequestFlag(Fbl_BootM_ReprogrammingRequestFlagStateType *flag);

#endif
