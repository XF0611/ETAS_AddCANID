/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_ImageContext.h
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

#ifndef  IMAGEM_IMAGE_CONTEXT_H
#define  IMAGEM_IMAGE_CONTEXT_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "Std_Types.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define IMAGEM_SHA256_DIGEST_LEN   32u
/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/

typedef struct
{
    uint8 imageId;
    uint32 blockOffset;
    uint32 blockLength;
    boolean isInit;
    boolean isValid;
}ImageM_ImageContextType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/
extern ImageM_ImageContextType *ImageM_GetImgContextInstance(void);
extern Std_ReturnType ImageM_InitImageBlock(ImageM_ImageContextType * const pImageContext,
                                                uint8 imageId,
                                                uint32 blockOffset,
                                                uint32 blockLength);

extern Std_ReturnType ImageM_ImageBlockErase(ImageM_ImageContextType * const pImageContext);

extern Std_ReturnType ImageM_ImageBlockWrite(ImageM_ImageContextType * const pImageContext,
                                                uint32 offset,
                                                const uint8 *pData,
                                                uint32 len);

extern Std_ReturnType ImageM_InitHashSHA256Block(ImageM_ImageContextType * const pImageContext);
extern Std_ReturnType ImageM_SHA256HashBlockUpdate(ImageM_ImageContextType * const pImageContext,
                                                    const uint8 *pData,
                                                    uint32 length);
extern uint8 *ImageM_SHA256HashBlockFinish(ImageM_ImageContextType * const pImageContext);

extern Std_ReturnType ImageM_X509CertVerify(ImageM_ImageContextType * const pImageContext,
                                                const uint8 *pCert,
                                                uint32 length);
extern Std_ReturnType ImageM_InitSignature(ImageM_ImageContextType * const pImageContext);
extern Std_ReturnType ImageM_SignatureAdddata(ImageM_ImageContextType * const pImageContext,
                                                const uint8 *pData,
                                                uint32 length);
extern Std_ReturnType ImageM_SignatureFinishAndVerify(ImageM_ImageContextType * const pImageContext,
                                                const uint8 *pSignature,
                                                uint32 length);
extern Std_ReturnType ImageM_CertificateCloseSession(void);

extern Std_ReturnType ImageM_SBL_Write();
#endif
