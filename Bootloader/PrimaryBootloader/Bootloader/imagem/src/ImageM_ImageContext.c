/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_Image.c
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


/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_ImageContext.h"
#include "PreEraseUltil.h"
#include "ImageM_StreamFlash.h"
#include"ImageM_Config.h"
#include "rba_BswSrv.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define MAX_BUFFER_SIZE         2048U
#define SIZE_OF_ADDR_OR_LENGTH  4U
#define UPDATE_HASH_BUFFER      40U

typedef struct
{
    uint32 StartAddr;
    uint32 BytesWritten;
    uint32 Capacity;
}ImageM_WriteType;
/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
ImageM_StreamFlashType pvBlockStream; //ETAS BIP test tan9sgh static
static uint8 pvBuffer[MAX_BUFFER_SIZE];   /* PRQA S 3218 */
static ImageM_ImageContextType pvImageContext =         /* PRQA S 3218 */
{
    0xFFu,
    0,
    0,
    TRUE,
    TRUE
};
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
ImageM_ImageContextType *ImageM_GetImgContextInstance(void)
{
    return &pvImageContext;
}

Std_ReturnType ImageM_InitImageBlock(ImageM_ImageContextType * const pImageContext,
                                                uint8 imageId,
                                                uint32 blockOffset,
                                                uint32 blockLength)
{
    Std_ReturnType retVal = E_NOT_OK;

    if(pImageContext != NULL_PTR)
    {
        pImageContext->imageId = imageId;
        pImageContext->blockLength = blockLength;
        pImageContext->blockOffset = blockOffset;
        pImageContext->isInit = TRUE;

        retVal = ImageM_StreamFlash_InitObject(&pvBlockStream,
                                            pvBuffer,
                                            MAX_BUFFER_SIZE,
                                            pImageContext->imageId,
                                            pImageContext->blockOffset,
                                            pImageContext->blockLength,
                                            NULL_PTR);
    }
    return retVal;
}

Std_ReturnType ImageM_ImageBlockErase(ImageM_ImageContextType * const pImageContext)        /* PRQA S 3673 */
{
    Std_ReturnType retVal = E_NOT_OK;
    if((pImageContext != NULL_PTR) && (pImageContext->isInit == TRUE))
    {
        retVal = ImageM_StreamFlash_Erase(&pvBlockStream, pImageContext->blockOffset, pImageContext->blockLength);
    }
    return retVal;
}

Std_ReturnType ImageM_ImageBlockWrite(ImageM_ImageContextType * const pImageContext,    /* PRQA S 3673 */
                                                uint32 offset,
                                                const uint8 *pData,
                                                uint32 len)
{
    Std_ReturnType retVal = E_NOT_OK;
    boolean isFlush = FALSE;
    
    if((pImageContext != NULL_PTR) && (pImageContext->isInit == TRUE))
    {
        if (pImageContext->imageId == IMAGEM_IMAGE_SBL_ID)
        {
            retVal = ImageM_RamCopy(&pvBlockStream, pData, len, offset);
        }
        else 
        {
            if(offset + len >= pvBlockStream.Area.Capacity)         /* PRQA S 3397 */
            {
                isFlush = TRUE;
            }
            retVal = ImageM_StreamFlash_write(&pvBlockStream, pData, len, isFlush);
            if ((retVal == E_OK) && (pImageContext->blockOffset == 0U))
            {
                PreEraseUntil_WritePreEraseState(pImageContext->imageId, FALSE);         /* PRQA S 3200 */   
            }
        }

    }
    return retVal;
}
