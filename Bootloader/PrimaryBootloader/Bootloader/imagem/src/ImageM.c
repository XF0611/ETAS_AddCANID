/*********************************************************************************************************************
*
* Description:     ProgM module private configuration implementation
* FileName:        ImageM.c
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
#include "ImageM.h"
#include "ImageM_ImageUpdate.h"
#include "rba_BswSrv.h"
#include "Crc.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#ifndef MEM_COPY
#define MEM_COPY(pDest, pSrc, u32_NumBytes)    rba_BswSrv_MemCopy(pDest, pSrc, u32_NumBytes)
#else
#define MEM_COPY(pDest, pSrc, u32_NumBytes)
#endif

#ifndef MEM_SET
#define MEM_SET(pDest, xPattern, u32_NumBytes)    rba_BswSrv_MemSet(pDest, xPattern, u32_NumBytes)
#else
#define MEM_SET(pDest, xPattern, u32_NumBytes)
#endif

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
/*A action of image after inspect the input parameters*/
static ImageM_ActionType Action;
static uint8 Err;
//static ImageM_ImageUpdateStateType ImageM_ImageUpdateState;                                                             /* PRQA S 3218 */
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/                                                        /* PRQA S 3447 */                                                               /* PRQA S 3447 */

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
uint8 ImageM_PrvGetFlashAreaIdFromAddr(uint32 Addr, uint32 Len)         /* PRQA S 3408 */                          /* PRQA S 1330, 1505 */
{
    uint8 FaId = IMAGEM_FLASH_AREA_UNDEFINE;
    uint8 ImgIndex;
    const ImageM_FlashAreaType *Fa;
    boolean IsFound = FALSE;

    for(ImgIndex = 0; ImgIndex < IMAGEM_IMAGE_NUMBER; ImgIndex++)                               /* PRQA S 1862 */
    {
        /*open image upload identification*/
        (void)ImageM_FlashPort_Open(IMAGEM_IMAGE_PRIMARY(ImgIndex), &Fa);                     /* PRQA S 3391, 1863 */
        
        if((Addr >= Fa->Address) && 
            ((Addr + Len) <= (Fa->Address + Fa->Size)) &&
            ((Addr + Len) >= Fa->Address) &&
            (Addr <= (Fa->Address + Fa->Size)))
        {
            IsFound = TRUE;
            break;
        }
    }

    if(IsFound == TRUE)
    {
        FaId = Fa->FaId;
    }

    return FaId;
}
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

uint8 ImageM_GetFlashAreaIdFromAddrImpl(uint32 Addr, uint32 Len)                                /* PRQA S 1330, 1505 */
{
    uint8 FaId = IMAGEM_FLASH_AREA_UNDEFINE;
    uint8 ImgIndex;
    const ImageM_FlashAreaType *Fa;
    boolean IsFound = FALSE;

    for(ImgIndex = 0; ImgIndex < IMAGEM_IMAGE_NUMBER; ImgIndex++)                               /* PRQA S 1862 */
    {
        /*open image upload identification*/
        (void)ImageM_FlashPort_Open(IMAGEM_IMAGE_PRIMARY(ImgIndex), &Fa);                     /* PRQA S 3391, 1863 */
        
        if((Addr >= Fa->Address) && 
            ((Addr + Len) <= (Fa->Address + Fa->Size)) &&
            ((Addr + Len) >= Fa->Address) &&
            (Addr <= (Fa->Address + Fa->Size)))
        {
            IsFound = TRUE;
            break;
        }
    }

    if(IsFound == TRUE)
    {
        FaId = Fa->FaId;
    }

    return FaId;
}

Std_ReturnType ImageM_InspectEraseImageImpl(uint32 Address, uint32 Len, ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr)  /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint8 FaId;
    const ImageM_FlashAreaType *Fa = NULL_PTR;
    /*Reset All states and actions before inspect the inputs*/
    MEM_SET(&Action, 0, sizeof Action);                                                                                     /* PRQA S 3200 */
    Err = IMAGEM_E_REQ_UNKNOWN;

    *ErrCodePtr = &Err;
    /*Get Flash Area Id from Address and Len*/
    FaId = ImageM_PrvGetFlashAreaIdFromAddr(Address, Len);  
    if(FaId == IMAGEM_FLASH_AREA_UNDEFINE)
    {
        **ErrCodePtr = IMAGEM_E_REQ_REGION_INVALID;
    }
    else
    {   
        ImageM_FlashPort_Open(FaId, &Fa);               /* PRQA S 3200 */
        *ActionPtr = &Action;
        (*ActionPtr)->FaId = FaId;
        (*ActionPtr)->Offset = Address - Fa->Address;
        (*ActionPtr)->Len = Len;

        *ErrCodePtr = NULL_PTR;
        Ret = E_OK;
    }
    return Ret;
}

Std_ReturnType ImageM_InspectRequestImageImpl(uint32 Address, uint32 Len,                                       /* PRQA S 1503 */
                                                ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr)
{
    Std_ReturnType Ret = E_NOT_OK;
    uint8 FaId;
    const ImageM_FlashAreaType *Fa = NULL_PTR;
    MEM_SET(&Action, 0, sizeof Action);                                                                         /* PRQA S 3200 */
    Err = IMAGEM_E_REQ_UNKNOWN;

    *ErrCodePtr = &Err;
    FaId = ImageM_PrvGetFlashAreaIdFromAddr(Address, Len);

    /*Ensure the request is the same id with erase routine before*/
    if(FaId == IMAGEM_FLASH_AREA_UNDEFINE)
    {
        **ErrCodePtr = IMAGEM_E_REQ_REGION_INVALID;

    }
    else
    {
        ImageM_FlashPort_Open(FaId, &Fa);       /* PRQA S 3200 */
        *ActionPtr = &Action;
        (*ActionPtr)->FaId = FaId;
        (*ActionPtr)->Offset = Address - Fa->Address;
        (*ActionPtr)->Len = Len;
        (*ActionPtr)->BlockLen = IMAGEM_CHUNK_DATA_SIZE; 
        *ErrCodePtr = NULL_PTR;
        Ret = E_OK;
    }
    return Ret;
}
Std_ReturnType ImageM_InspectUploadImageImpl(uint32 Address, uint32 Len, const uint8 *Data,                             /* PRQA S 1503 */
                                                ImageM_ActionType **ActionPtr, uint8 **ErrCodePtr)
{
    Std_ReturnType Ret = E_NOT_OK;
    uint8 FaId;
    const ImageM_FlashAreaType *Fa = NULL_PTR;

    MEM_SET(&Action, 0, sizeof Action);                                                                                 /* PRQA S 3200 */
    Err = IMAGEM_E_REQ_UNKNOWN;

    *ErrCodePtr = &Err;
    FaId = ImageM_PrvGetFlashAreaIdFromAddr(Address, Len);

    /*Ensure the request is the same id with erase routine before*/
    if(FaId == IMAGEM_FLASH_AREA_UNDEFINE)
    {
        **ErrCodePtr = IMAGEM_E_REQ_REGION_INVALID;

    }
    else
    {
        if(Len <= IMAGEM_CHUNK_DATA_SIZE)
        {
            ImageM_FlashPort_Open(FaId, &Fa);           /* PRQA S 3200 */
            *ActionPtr = &Action;
            (*ActionPtr)->FaId = FaId;
            (*ActionPtr)->Offset = Address - Fa->Address;
            (*ActionPtr)->Len = Len;
            (*ActionPtr)->DataPtr = Data;
            *ErrCodePtr = NULL_PTR;
            Ret = E_OK;
        }
        else
        {
            **ErrCodePtr = IMAGEM_E_REQ_OUT_OF_RANGE;
        }
    }
    return Ret;
}

