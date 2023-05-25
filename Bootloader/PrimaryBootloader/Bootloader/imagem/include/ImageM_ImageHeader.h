/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_ImageHeader.h
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
GBI81HC          2021/25/3      initial commit
**********************************************************************************************************************/

#ifndef  IMAGEM_IMAGEHEADER_H
#define  IMAGEM_IMAGEHEADER_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "ImageM_FlashPort.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef struct
{
    uint8 PartNumber[24U];
    uint8 imageDeps[1U];
    uint8 Pad1[3U];
    uint8 ExecutableAddress[4];
    uint8 SignatureSize[4];
    uint8 CertificateSize[4];
}ImageM_ImageHeaderType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

extern Std_ReturnType ImageM_ImageHeader_PartNumber_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length);
extern Std_ReturnType ImageM_ImageHeader_ImageVersion_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length);
extern Std_ReturnType ImageM_ImageHeader_ImageDependency_ReadFunc(const ImageM_FlashAreaType *FaPtr, uint8* Data, uint8 Length);
extern boolean ImageM_ImageHeader_IsImagePresent_ReadFunc(const ImageM_FlashAreaType *FaPtr);
#endif /*IMAGEM_H*/
