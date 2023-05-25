/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_StreamFlash.c
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
#include "Os_TimeServices.h"
#include "ImageM_StreamFlash.h"
#include "rba_BswSrv.h"

#define IMAGEM_DEV_ERROR_MODULE_ID   (IMAGEM_DEV_STREAMFLASH_ID)
#include "ImageM_Det.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/
#define STREAM_BUFFER(pStream)      (pStream->Buffer)                               /* PRQA S 3410 */
#define STREAM_AREA(pStream)        (pStream->Area)                                 /* PRQA S 3410 */
#define FREE_BUFF_SPACE(pStream)    (pStream->Buffer.Len - pStream->Buffer.Used)    /* PRQA S 3410 */

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
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
Std_ReturnType ImageM_StreamFlash_Prv_WriteSync(ImageM_StreamFlashType *StreamFlashPtr);                                /* PRQA S 3447 */
Std_ReturnType ImageM_StreamFlash_Prv_GetSectorFromOffset(ImageM_StreamFlashType *StreamFlashPtr,                       /* PRQA S 3447 */
                                                            uint32 Offset, ImageM_FlashSectorType *SectorPtr);
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
Std_ReturnType ImageM_StreamFlash_Prv_WriteSync(ImageM_StreamFlashType *StreamFlashPtr)                 /* PRQA S 1505 */
{
    Std_ReturnType Ret = E_NOT_OK;
    uint32 PadLength;
    const ImageM_FlashAreaType *Fa;
    uint32 WriteOffset;
    uint8 Pattern;
    IMAGEM_REPORT_ERROR_NOK(StreamFlashPtr->Buffer.Used == 0, 2, IMAGEM_E_INVALID_LENGTH);              /* PRQA S 1843 */

    ImageM_FlashPort_Open(StreamFlashPtr->Area.FlashAreaId, &Fa);                                       /* PRQA S 3200 */
    PadLength = ImageM_FlashPort_Align(Fa);
    if(StreamFlashPtr->Buffer.Used % PadLength)                                                         /* PRQA S 3344 */
    {
        /*Un-alignment Length, Fill the missing bytes*/
        PadLength -= StreamFlashPtr->Buffer.Used % PadLength;
        Pattern = ImageM_FlashPort_GetErasedValue(Fa);
        MEM_SET(StreamFlashPtr->Buffer.Buff + StreamFlashPtr->Buffer.Used, Pattern, PadLength);         /* PRQA S 4443, 0488, 3200 */
    }
    else
    {
        PadLength = 0;
    }
    WriteOffset = StreamFlashPtr->Area.Offset + StreamFlashPtr->Area.BytesWritten;
    /*Invoke ImageM_FlashPort_Write to write data into Flash Area*/
    Ret = ImageM_FlashPort_Write(Fa, WriteOffset,
                                StreamFlashPtr->Buffer.Buff,
                                StreamFlashPtr->Buffer.Used + PadLength);

    if(Ret == E_OK)                                                           /* PRQA S 3397 */               
    {
        if(StreamFlashPtr->Area.Cb)                                           /* PRQA S 3344 */
        {

            /*Invert the data to ensure the caller that can discover a faulty*/
            for(uint32 Index = 0; Index < StreamFlashPtr->Buffer.Used; Index++)
            {
                StreamFlashPtr->Buffer.Buff[Index] = ~StreamFlashPtr->Buffer.Buff[Index];
            }
            /*Read data back*/
            ImageM_FlashPort_Read(Fa,   WriteOffset,            /* PRQA S 3200 */
                                        StreamFlashPtr->Buffer.Buff,
                                        StreamFlashPtr->Buffer.Used);

            StreamFlashPtr->Area.Cb(    StreamFlashPtr->Buffer.Buff,
                                        StreamFlashPtr->Buffer.Used,
                                        WriteOffset);
        }
        /*Update byte written*/
        StreamFlashPtr->Area.BytesWritten += StreamFlashPtr->Buffer.Used;
        /*Reset Used Space of Buffer*/
        StreamFlashPtr->Buffer.Used = 0;

    }
    else
    {
        /*Nothing TODO. Write Failed, Function return E_NOT_OK */
    }

    return Ret;
}

Std_ReturnType ImageM_StreamFlash_Prv_GetSectorFromOffset(ImageM_StreamFlashType *StreamFlashPtr,                           /* PRQA S 3206, 1503 */
                                                            uint32 Offset, ImageM_FlashSectorType *SectorPtr)               /* PRQA S 3206 */
{
    Std_ReturnType Ret = E_NOT_OK;
    return Ret;
}
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
Std_ReturnType ImageM_StreamFlash_InitObject(ImageM_StreamFlashType *StreamFlashPtr, uint8 *Buff, uint32 Len,                           /* PRQA S 1503 */
                                            uint8 FlashAreaId, uint32 Offset, uint32 FreeSpace, ImageM_StreamFlashCallbackFn Cb)
{
    Std_ReturnType Ret = E_OK;
    const ImageM_FlashAreaType *Fa = NULL_PTR;
    uint32 Align;

    IMAGEM_REPORT_ERROR_NOK((StreamFlashPtr == NULL_PTR) ||(Buff == NULL_PTR) , 1, IMAGEM_E_PARAM_ADDRESS);
    IMAGEM_REPORT_ERROR_NOK(FlashAreaId == IMAGEM_FLASH_AREA_UNDEFINE, 1, IMAGEM_E_INVALID_ID);
    /* check alignment of buffer length*/
    (void)ImageM_FlashPort_Open(FlashAreaId, &Fa);
    Align = ImageM_FlashPort_Align(Fa);
    IMAGEM_REPORT_ERROR_NOK(Len % Align, 1 , IMAGEM_E_INVALID_ALIGN);                                                                   /* PRQA S 3344 */
    IMAGEM_REPORT_ERROR_NOK(Offset % Align, 1 , IMAGEM_E_INVALID_ALIGN);                                                                /* PRQA S 3344 */
    IMAGEM_REPORT_ERROR_NOK((Offset +  FreeSpace) > Fa->Size , 1 , IMAGEM_E_INVALID_LENGTH);
    /*Initialize the default values to Stream*/
    StreamFlashPtr->Buffer.Buff = Buff;
    StreamFlashPtr->Buffer.Len = Len;
    StreamFlashPtr->Buffer.Used = 0;

    StreamFlashPtr->Area.FlashAreaId = FlashAreaId;
    StreamFlashPtr->Area.BytesWritten = 0;
    StreamFlashPtr->Area.Capacity = FreeSpace == 0 ? Fa->Size - Offset : FreeSpace;                                                     /* PRQA S 3396, 1843 */
    StreamFlashPtr->Area.Offset = Offset;

    StreamFlashPtr->Area.FlashAreaId = FlashAreaId;
    StreamFlashPtr->Area.LastSectorErasedOffset = 0xFFFFFFFFU;

    StreamFlashPtr->Area.Cb = Cb;
    ImageM_FlashPort_Close(Fa);
    return Ret;
}

Std_ReturnType ImageM_StreamFlash_write(ImageM_StreamFlashType *StreamFlashPtr, const uint8 *Data, uint32 Len, boolean Flush)           /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_OK;
    uint32 BytesProcessed = 0;
    uint32 FreeBufferSpace = 0;
    IMAGEM_REPORT_ERROR_NOK((StreamFlashPtr == NULL_PTR) ||(Data == NULL_PTR) , 2, IMAGEM_E_PARAM_ADDRESS);
    IMAGEM_REPORT_ERROR_NOK(!Len, 2, IMAGEM_E_INVALID_LENGTH);                                                                                  /* PRQA S 4558 */
    
    FreeBufferSpace = FREE_BUFF_SPACE(StreamFlashPtr);

    while((Len - BytesProcessed) >= FreeBufferSpace)
    {
        MEM_COPY(StreamFlashPtr->Buffer.Buff + StreamFlashPtr->Buffer.Used, Data + BytesProcessed, FreeBufferSpace);                            /* PRQA S 0488, 3200 */
        /*Update used space of buffer*/
        StreamFlashPtr->Buffer.Used += FreeBufferSpace;
        /*Execute Flash Write Sync*/
        Ret = ImageM_StreamFlash_Prv_WriteSync(StreamFlashPtr);
        if(Ret == E_OK)
        {
            /*Update Free buffer Space*/
            FreeBufferSpace = FREE_BUFF_SPACE(StreamFlashPtr);
            /*Update the processed Byte*/
            BytesProcessed += FreeBufferSpace;
        }
        else
        {
            /* Return E_NOT_OK when write sync failed*/
            break;
        }
    }

    if(Ret == E_OK)
    {
        /*Incase of len do not align with Buffer Len, place the rest of data to buffer*/
        if(BytesProcessed < Len)
        {
            MEM_COPY(StreamFlashPtr->Buffer.Buff + StreamFlashPtr->Buffer.Used, Data + BytesProcessed, Len - BytesProcessed);                                   /* PRQA S 0488, 3200 */
            /*Update used space of buffer*/
            StreamFlashPtr->Buffer.Used += Len -  BytesProcessed;
            /*FLush the data if request*/
            Ret = Flush  && StreamFlashPtr->Buffer.Used ? ImageM_StreamFlash_Prv_WriteSync(StreamFlashPtr) : E_OK;                                              /* PRQA S 4558, 3396 */
        }
    }

    return Ret;
}

Std_ReturnType ImageM_StreamFlash_Erase(ImageM_StreamFlashType *StreamFlashPtr, uint32 Offset, uint32 Size)                         /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;
    IMAGEM_REPORT_ERROR_NOK(StreamFlashPtr == NULL_PTR, 3, IMAGEM_E_PARAM_ADDRESS);
    const ImageM_FlashAreaType *Fa;

    if(StreamFlashPtr->Area.LastSectorErasedOffset != Offset)
    {   (void)ImageM_FlashPort_Open(StreamFlashPtr->Area.FlashAreaId, &Fa);

        Ret = ImageM_FlashPort_Erase(Fa, Offset, Size);
        if(Ret == E_OK)
        {
            StreamFlashPtr->Area.LastSectorErasedOffset = Offset;
        }
    }

    return Ret;
}


