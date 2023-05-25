/*********************************************************************************************************************
*
* Description:     
* FileName:        SWDL_ValidateApplication.c
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
#include "Dcm.h"
#include "ImageM.h"
#include "ImageM_Config.h"
#include "ImageM_ImageUpdatePriv.h"
#include "ImageM_ImageContext.h"
#include "FBL_DataM.h"
#include "ImageM_ImageHeader.h"
#include "ImageM_FlashPort.h"
#include "Crc.h"
#include "FBL_BootMCfg.h"
#include "ImageM_ImageUpdateStatus.h"
#include "SBL_Flash.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define ECUMGR_CRC_ECU_INITIAL                            (0xFFFFFFFFU)

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

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
#ifdef IMAGEM_USE_EXTERNAL_FLASH
    #define READ_EXTFLS_SIZE 2048
#endif  
Std_ReturnType Fbl_ProgM_StartCheckRoutineCallback(  
                            uint32 dataIn1,                                                            /* PRQA S 1503 */
                         Dcm_OpStatusType OpStatus,                                                                     /* PRQA S 3206 */                                                                        /* PRQA S 1330 */
                         uint8 * dataOut1,                                                                  /* PRQA S 3200 *//* PRQA S 3206 */
                          Dcm_NegativeResponseCodeType * ErrorCode)
{
    Std_ReturnType ret = E_NOT_OK;
    ImageM_ImageContextType *pImgContext;
    const ImageM_FlashAreaType *FaPtr;
    pImgContext = ImageM_GetImgContextInstance();
    ImageM_FlashPort_Open(pImgContext->imageId, &FaPtr);                                                    /* PRQA S 3200 */
    uint32 Flash_CRC = CRC_INITIAL_VALUE32;
#ifdef IMAGEM_USE_EXTERNAL_FLASH
    uint32 srcAddr = FaPtr->Address;
    uint32 remainingBytes = pImgContext->blockLength;
    // uint16 imageTail = (uint16)(pImgContext->blockLength % READ_EXTFLS_SIZE);
    uint32 opBytes = READ_EXTFLS_SIZE;
    uint8  blockRead[READ_EXTFLS_SIZE] = {0};
    boolean isFirstCall = TRUE;

    do
    {
        // if(imageTail != 0u)
        // {/* image size is not READ_EXTREAD_EXTFLS_SIZE_SIZE aligned. */
        //     opBytes = (remainingBytes >= (READ_EXTFLS_SIZE - imageTail))?(READ_EXTFLS_SIZE - imageTail):remainingBytes;
        //     imageTail = 0u;
        // }
        // else
        // {
        //     opBytes = (remainingBytes >= READ_EXTFLS_SIZE)?READ_EXTFLS_SIZE:remainingBytes;
        // }
        opBytes = (READ_EXTFLS_SIZE > remainingBytes) ? remainingBytes : READ_EXTFLS_SIZE;
        EXT_FLASH_DRIVER_READ(srcAddr, &blockRead[0], opBytes);
        Flash_CRC = Crc_CalculateCRC32((const uint8*) &blockRead[0], opBytes, Flash_CRC, isFirstCall);
        srcAddr += opBytes;           
        remainingBytes -= opBytes;   
        isFirstCall = FALSE;                          
    }while(remainingBytes > 0);
#else
    Flash_CRC = Crc_CalculateCRC32((const uint8*) FaPtr->Address,      /* PRQA S 0306 */ /* MISRA DEVIATION: Cast between a pointer to object and an integral type is necessary. */
                                        pImgContext->blockLength,
                                        ECUMGR_CRC_ECU_INITIAL,
                                        TRUE);
#endif

    if(Flash_CRC == dataIn1)
    {
        
        if(pImgContext->imageId == IMAGEM_IMAGE_SBL_ID)
        {
            ImageM_FlashPort_Init();                                                    /* PRQA S 3200 */
        }
        else if((pImgContext->imageId == IMAGEM_IMAGE_CAL_ID) || (pImgContext->imageId == IMAGEM_IMAGE_APP_CORE_5_ID))
        {
            pImgContext->isValid = TRUE;
            ImageM_FlashPort_DeInit();                                  /* PRQA S 3200 */
        }
        else
        {
            pImgContext->isValid = TRUE;
        }
        ImageM_ImageStatus_SynchWriteApplicationValidFlag(pImgContext->isValid,pImgContext->imageId);                       /* PRQA S 3200 */
        ret = E_OK;
        *ErrorCode = E_OK;
    }
    return ret;
}


