/*********************************************************************************************************************
*
* Description:     DataM module private interface
* FileName:        FBL_DataMPrv.h
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
#ifndef  FBL_PROGMPRV_H
#define  FBL_PROGMPRV_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/
/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/

/**
 * @brief
 *
 */
typedef enum
{
  FBL_PROGM_ERASE_RESULT_SUCCESS, /* request successful */
  FBL_PROGM_ERASE_RESULT_FAILURE, /* request failed */
  FBL_PROGM_ERASE_RESULT_BUSY     /* request cannot be accepted at this time - try again */
} Fbl_ProgM_EraseResultType;

/**
 * @brief
 *
 */
typedef enum
{
  FBL_PROGM_WRITE_RESULT_SUCCESS, /* request successful */
  FBL_PROGM_WRITE_RESULT_FAILURE, /* request failed */
  FBL_PROGM_WRITE_RESULT_BUSY,
  FBL_PROGM_WRITE_INVALIDLENGTH
} Fbl_ProgM_WriteResultType;

/**
 * @brief
 *
 */
typedef enum
{
  FBL_PROGM_CHECK_DEPENDENCE_RESULT_SUCCESS,     /* request was successful*/
  FBL_PROGM_CHECK_DEPENDENCE_RESULT_PROCESSING,  /* request was pending*/
  FBL_PROGM_CHECK_DEPENDENCE_RESULT_FAILURE      /* request failed */
} Fbl_ProgM_CheckDependenceResultType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

#endif
