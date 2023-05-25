/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_StreamFlash.h
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

#ifndef  IMAGEM_STREAMFLASH_H
#define  IMAGEM_STREAMFLASH_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "ImageM_FlashPort.h"

/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

#define IMAGEM_DEV_STREAMFLASH_ID 5u
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
/**
 * @brief this Function type is invoked with a buffer containing the read back data from the flash after a write has completed.
 * this allow to verify the data that has been correctly stored.
 */
typedef Std_ReturnType (*ImageM_StreamFlashCallbackFn)(uint8 *Buf, uint32 Len, uint32 Offset);
typedef struct
{
    struct
    {
        uint8 *Buff;
        uint32 Len;
        uint32 Used;
    }Buffer;
    struct
    {
        uint32 BytesWritten;
        uint32 Offset;
        uint32 Capacity;
        uint8 FlashAreaId;
        uint8 Pad[3];
        uint32 LastSectorErasedOffset;
        ImageM_StreamFlashCallbackFn Cb;
    }Area;
}ImageM_StreamFlashType;


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

/**
 * @brief 
 * 
 * @param StreamFlashPtr 
 * @param Buff 
 * @param Len 
 * @param FlashAreaId 
 * @param Offset 
 * @param Capacity 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_StreamFlash_InitObject(ImageM_StreamFlashType *StreamFlashPtr, uint8 *Buff, uint32 Len,
                                            uint8 FlashAreaId, uint32 Offset, uint32 FreeSpace, ImageM_StreamFlashCallbackFn Cb);
/**
 * @brief 
 * 
 * @param StreamFlashPtr 
 * @param Data 
 * @param Len 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_StreamFlash_write(ImageM_StreamFlashType *StreamFlashPtr, const uint8 *Data, uint32 Len, boolean Flush);
/**
 * @brief 
 * 
 * @param StreamFlashPtr 
 * @return Std_ReturnType 
 */
extern Std_ReturnType ImageM_StreamFlash_Erase(ImageM_StreamFlashType *StreamFlashPtr, uint32 Offset,uint32 Size);

/**
 * @brief 
 * 
 */
extern Std_ReturnType ImageM_RamCopy(ImageM_StreamFlashType *StreamFlashPtr, const uint8 *Data, uint32 Len, uint32 Offset);
#endif