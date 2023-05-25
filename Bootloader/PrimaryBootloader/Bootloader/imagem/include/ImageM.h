/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_LoaderState.h
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

#ifndef  IMAGEM_H
#define  IMAGEM_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_ImageUpdatePriv.h"

#include "ImageM_ImageUpdate.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define IMAGEM_E_REQ_REGION_INVALID         0x20u
#define IMAGEM_E_REQ_REGION_UNINITIALIZE    0x21u
#define IMAGEM_E_REQ_OUT_OF_RANGE           0x22u
#define IMAGEM_E_REQ_SEGMENT_FAULT          0x23u
#define IMAGEM_E_REQ_ERASE_COMPLETE         0x24u
#define IMAGEM_E_REQ_ERASE_ONGOING          0x25u
#define IMAGEM_E_REQ_ERASE_FAIL             0x26u
#define IMAGEM_E_REQ_UNKNOWN                0xFFu

#define IMAGEM_CHUNK_DATA_SIZE              2048u
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
/**
 * @brief Describe what to do processing of image upload
 * 
 */
typedef struct
{
    uint16  BlockLen;
    uint8 Pad;
    uint8 FaId;
    uint32 Offset;
    uint32 Len;
    const uint8* DataPtr;
}ImageM_ActionType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

/**
 * @brief 
 * 
 * @param RspPtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_GoImpl(void);

/**
 * @brief 
 * 
 * @param state 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_SetUpdateFlagImpl(boolean state);

/**
 * @brief 
 * 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_SetConfirmedFlagImpl(boolean state);
/**
 * @brief 
 * 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_SetPreUpdateImageImpl(boolean state, uint8 FaId);

/**
 * @brief 
 * 
 * @param Address 
 * @param Len 
 * @return uint8 
 */
extern uint8 ImageM_GetFlashAreaIdFromAddrImpl(uint32 Address, uint32 Len);
/**
 * @brief 
 * 
 * @param FlashAreaId 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_InitImageImpl(uint8 FlashAreaId);
/**
 * @brief 
 * 
 * @param Address 
 * @param Len 
 * @param ActionPtr 
 * @param ErrCodePtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_InspectEraseImageImpl(uint32 Address, uint32 Len,
                                                    ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr);
/**
 * @brief 
 * 
 * @param Address 
 * @param Len 
 * @param ActionPtr 
 * @param ErrCodePtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_InspectRequestImageImpl(uint32 Address, uint32 Len,
                                                        ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr);
/**
 * @brief 
 * 
 * @param Address 
 * @param Len 
 * @param Data 
 * @param ActionPtr 
 * @param ErrCodePtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_InspectUploadImageImpl(uint32 Address, uint32 Len, const uint8 *Data,
                                                ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr);
#endif /*IMAGEM_H*/
