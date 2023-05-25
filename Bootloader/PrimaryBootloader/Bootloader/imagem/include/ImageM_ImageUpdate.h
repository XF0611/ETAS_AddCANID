/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_Boot.h
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

#ifndef  IMAGEM_BOOT_H
#define  IMAGEM_BOOT_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_ImageUpdatePriv.h"

/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

#define IMAGEM_DEV_IMAGEUPDATE_ID   (2u)
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/

/**
 * @brief Attemp to boot the content of primary slot
 * 
 */
#define IMAGEM_IMAGE_UPDATE_STATE_NONE      (uint8)0x00

/**
 * @brief Swap to the secondary slot because
 * absent a conform command from tester
 */
#define IMAGEM_IMAGE_UPDATE_STATE_TESTING   (uint8)0x01

/**
 * @brief 
 * 
 */
#define IMAGEM_IMAGE_UPDATE_STATE_SUCCESS   (uint8)0x02

/**
 * @brief 
 * 
 */
#define IMAGEM_IMAGE_UPDATE_STATE_REVERT    (uint8)0x04

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
 * @param BootStatePtr 
 * @param RspPtr 
 * @return EXPORT 
 */
extern Std_ReturnType ImageM_ImageUpdate_Start(ImageM_ImageUpdateStateType *BootStatePtr);

extern Std_ReturnType ImageM_ImageUpdate_SetConfirmedFlagImpl(boolean state);

extern Std_ReturnType ImageM_ImageUpdate_SetImageUpdate(boolean state);

extern Std_ReturnType ImageM_ImageUpdate_SetImagePreUpdate(boolean state, uint8 FaId);

extern Std_ReturnType ImageM_ImageUpdate_SetImagePresent( boolean state);

#endif /*IMAGEM_BOOTIMAGE_H*/
