/*********************************************************************************************************************
*
* Description:     
* FileName:        DataM_MetaDataInfos.h
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/

#ifndef  DATAM_METADATAINFOS_H
#define  DATAM_METADATAINFOS_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "FBL_DataM.h"

/*********************************************************************************************************************
*Exported Defines
*********************************************************************************************************************/
#define IMAGEM_PRE_ERASED_STATE_FALSE   0x00U   /*There is some data on the region */
#define IMAGEM_PRE_ERASED_STATE_TRUE    0x0AU   /*Region is erased before */
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MDInfos_ReadPreErasedFlag(uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MDInfos_ReadApplicationValidFlag(uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MDInfos_ReadPBLValidFlag(uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MDInfos_ReadPostSWDLStatusFlag(uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MDInfos_ReadInProgressSwUpdateBlock(uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MdInfos_WritePreErasedFlag (const uint8 * data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */

extern Std_ReturnType DataM_MdInfos_WriteApplicationValidFlag (const uint8 * data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType FBL_DataM_SynchWriteNvmApplicationValid(const uint8 *data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MdInfos_WritePostSWDLStatusFlag (const uint8 * data);
/**
 * @brief 
 * 
 * @param data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DataM_MdInfos_WriteInProgressSwUpdateFlag (const uint8 * data);
extern Std_ReturnType FBL_DataM_SynchWriteNvmCalibrationValid(const uint8 *data);
#endif/*DATAM_METADATAINFOS_H*/
