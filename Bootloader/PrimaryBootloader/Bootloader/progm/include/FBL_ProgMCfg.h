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
#ifndef  FBL_PROGMCFG_H
#define  FBL_PROGMCFG_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "rba_BswSrv.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define FBL_PROGM_ROUTINE_STATUS_INCORRECT    ((uint8) 1u)
#define FBL_PROGM_ROUTINE_STATUS_CORRECT      ((uint8) 0u)
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/
#define Fbl_ProgM_MemCopy(dest,src,numBytes) rba_BswSrv_MemCopy((dest),(src),(uint32)(numBytes))
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
  FBL_PROGM_CURRENT_REGION_APPLICATION,
  FBL_PROGM_CURRENT_REGION_CALIBRATION,
  FBL_PROGM_CURRENT_REGION_SIZE
} Fbl_ProgM_CurrentRegionType;

typedef enum
{
  FBL_PROGM_MEM_ID_RAM = 0x00,
  FBL_PROGM_MEM_ID_ROM = 0x06,
  FBL_PROGM_MEM_ID_INVALID = 0xFF
} FBl_ProgM_MemmoryIdentifierType;
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
  FBL_PROGM_VERIFY_DOWNLOAD_RESULT_SUCCESS,     /* request was successful - download was OK*/
  FBL_PROGM_VERIFY_DOWNLOAD_RESULT_PROCESSING,  /* request was accepted, but further queries are require to check if download is indeed ok*/
  FBL_PROGM_VERIFY_DOWNLOAD_RESULT_FAILURE      /* request failed */
} Fbl_ProgM_VerifyDownloadResultType;

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

/**
 * @brief
 *
 */
typedef struct
{
    uint32 Address;
    uint32 Size;
}Fbl_ProgM_RegionType;

/**
 * @brief
 *
 */
typedef struct
{
    Fbl_ProgM_RegionType VerifyRegion;
    uint32 CrcStartValue;
    uint32 CrcTesterValue;
    uint32 CrcEcuValue;
    boolean IsStart;
}Fbl_ProgM_VerifyDowloadType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

#endif
