/*********************************************************************************************************************
*
* Description:     ProgM module private configuration implementation
* FileName:        ImageM_ImageHeader.c
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
#include "ImageM_ImageHeader.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define IMAGEM_IMAGE_HDR_PARTNUMBER_OFFSET      0U
#define IMAGEM_IMAGE_HDR_IMAGEVERSION_OFFSET    12U
#define IMAGEM_IMAGE_HDR_DEPENDENCY_OFFSET      24U

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

const ImageM_ImageHeaderType ImageM_BootImageHeader =               /* PRQA S 3408 */
{
    .PartNumber = {0x52, 0x38, 0x45, 0x32, 0x2D, 0x31, 0x34, 0x43, 0x32, 0x30, 0x30, 0x2D, 0x41, 0x41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .imageDeps = {0xFF},
};
/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
 
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

Std_ReturnType ImageM_ImageHeader_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint32 Offset, uint8 Length)             /* PRQA S 3408 */
{
	Std_ReturnType retVal = E_NOT_OK;
	uint8 *pData = Data;
    uint8 *ImageHeader;

	ImageHeader = (uint8*)(FaPtr->Address);         /* PRQA S 306 */
	for(uint32 Idx = Offset; Idx < Offset + Length; ++Idx)      /* PRQA S 3397 */
	{
		*(pData + Idx - Offset)= *(ImageHeader + Idx);  /* PRQA S 488 */
	}
	retVal = E_OK;
	return retVal;
}

Std_ReturnType ImageM_ImageHeader_PartNumber_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length)
{
    uint32 PartNumberOff = IMAGEM_IMAGE_HDR_PARTNUMBER_OFFSET;

	return ImageM_ImageHeader_ReadFunc(FaPtr, Data, PartNumberOff, Length);
}

Std_ReturnType ImageM_ImageHeader_ImageVersion_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length)
{
    uint32 PartNumberOff = IMAGEM_IMAGE_HDR_IMAGEVERSION_OFFSET;

	return ImageM_ImageHeader_ReadFunc(FaPtr, Data, PartNumberOff, Length);
}

Std_ReturnType ImageM_ImageHeader_ImageDependency_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length)
{
    uint32 DependOff = IMAGEM_IMAGE_HDR_DEPENDENCY_OFFSET;

	return ImageM_ImageHeader_ReadFunc(FaPtr, Data, DependOff, Length);
}

boolean ImageM_ImageHeader_IsImagePresent_ReadFunc(const ImageM_FlashAreaType *FaPtr)
{
    boolean RetVal = FALSE;
    uint32 Offset = IMAGEM_IMAGE_HDR_PARTNUMBER_OFFSET;
    uint8 Data[4U] = {0};

	ImageM_ImageHeader_ReadFunc(FaPtr, Data, Offset, (uint8)4U);        /* PRQA S 3200 */

    if ((Data[0] == 0x52) && (Data[1] == 0x38) && (Data[2] == 0x45) && (Data[3] == 0x32))   /* PRQA S 1863 */
    {
        RetVal = TRUE;
    }

    return RetVal;
}
