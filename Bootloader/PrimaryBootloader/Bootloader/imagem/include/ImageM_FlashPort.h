/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_FLashPort.h
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

#ifndef  IMAGEM_FLASHPORT_H
#define  IMAGEM_FLASHPORT_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_FlashArea.h"

/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define IMAGEM_DEV_FLASHPORT_ID 1u
#define IMAGEM_FLASHPORT_MAX_SECTORS   128u
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
 * @param FaID 
 * @param FlashAreaPtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_Open(uint8 FaID, const ImageM_FlashAreaType **FlashAreaPtr);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 */
extern void ImageM_FlashPort_Close(const ImageM_FlashAreaType *FlashAreaPtr);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @param Offset 
 * @param Src 
 * @param Len 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_Write(const ImageM_FlashAreaType *FlashAreaPtr,
                                            uint32 Offset,
                                            const uint8 *Src,
                                            uint32 Len);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @param Offset 
 * @param Len 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_Erase(const ImageM_FlashAreaType *FlashAreaPtr,
                                            uint32 Offset,
                                            uint32 Len);

/**
 * @brief 
 * 
 * @param FaId 
 * @param OutNumSectorsPtr 
 * @param OutSectorsPtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_GetSectors(uint8 FaId,
                                                uint32 *OutNumSectorsPtr,
                                                ImageM_FlashSectorType *OutSectorsPtr);

/**
 * @brief 
 * 
 * @param FlashSector 
 * @return uint32 
 */
extern uint32 ImageM_FlashPort_GetSectorOffset(ImageM_FlashSectorType *FlashSector);

/**
 * @brief 
 * 
 * @param FlashSector 
 * @return uint32 
 */
extern uint32 ImageM_FlashPort_GetSectorSize(ImageM_FlashSectorType *FlashSector);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @return uint32 
 */
extern uint32 ImageM_FlashPort_GetAreaSize(const ImageM_FlashAreaType *FlashAreaPtr);

/**
 * @brief 
 * 
 * @param Slot 
 * @param ImageIndex 
 * @return uint8 
 */

extern uint8 ImageM_FlashPort_GetFlashAreaId(ImageM_SlotType Slot, uint8 ImageIndex);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @return uint32 
 */
extern uint32 ImageM_FlashPort_Align(const ImageM_FlashAreaType *FlashAreaPtr);
/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @return uint8 
 */
extern uint8 ImageM_FlashPort_GetErasedValue(const ImageM_FlashAreaType *FlashAreaPtr);
/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @param Offset 
 * @param Dst 
 * @param Len 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_ReadFromInactiveBank(const ImageM_FlashAreaType *FlashAreaPtr,
                                            uint32 Offset, uint8 *Dst, uint32 Len);

/**
 * @brief 
 * 
 * @param FlashAreaPtr 
 * @param Offset 
 * @param Dst 
 * @param Len 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_Read(const ImageM_FlashAreaType *FlashAreaPtr,
                                            uint32 Offset, uint8 *Dst, uint32 Len);
/**
 * @brief 
 * 
 * @param FaId 
 * @return uint8 
 */
extern uint8 ImageM_FlashPort_GetImageIdxByFaId(uint8 FaId);
/**
 * @brief 
 * 
 */
extern Std_ReturnType ImageM_FlashPort_Init(void);
/**
 * @brief 
 * 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_FlashPort_DeInit(void);

#endif /*IMAGEM_FLASHPORT_H*/