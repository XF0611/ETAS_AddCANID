/*********************************************************************************************************************
*
* Description:     ProgM module private configuration implementation
* FileName:        ImageM_FlashPort.c
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
#define BOOTLOADER_COMPONENT_ARCHITECTURE 2
#define SBL_INTEGRATE FALSE

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Os_TimeServices.h"
#include "ImageM_FlashPort.h"
#include "rba_BswSrv.h"
#include "EcuMgt.h"
#include "ImageM_Config.h"
//#include "ImageM_ImageDualBank.h"
#define IMAGEM_DEV_ERROR_MODULE_ID   (IMAGEM_DEV_FLASHPORT_ID)
#include "ImageM_Det.h"

#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
#include "SBL_Flash.h"
#endif
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define IMAGEM_INTERNAL_FLASH_ERASE_VALUE   (0x00)
#define IMAGEM_EXTERNAL_FLASH_ERASE_VALUE   (0xFF) //ETAS BIP Porting
#define IMAGEM_FLASH_AREA_PAGE_SIZE         8u     //ETAS BIP Porting
#define IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE   IMAGEM_FLASH_AREA_PAGE_SIZE * 1u
#define IMAGEM_FLASH_AREA_WRITE_BLOCK_SIZE  IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE * 32u //ETAS BIP Porting
#define IMAGEM_FLASH_MIN_OF_SECTOR_SIZE     0x1000U //ETAS BIP Porting
#define FLASH_SECTOR_SIZE                   0x1000U //ETAS BIP Porting
#define ARRAY_SIZE(x)                       sizeof(x)/sizeof(x[0])                          /* PRQA S 3410 */

#define IMAGEM_FLASH_CACHE_MASK         0xF0000000U
#define IMAGEM_FLASH_CACHE_RANGE        0x80000000U
#define IMAGEM_FLASH_NON_CACHE_RANGE    0xA0000000U
#define IMAGEM_FLASH_BANK_MASK          0xF0F00000U
#define IMAGEM_FLASH_BANK_A_MASK        0x80300000U
#define IMAGEM_FLASH_BANK_AB_OFFSET     0x00300000U

#define IMAGEM_FBL_REGION_START_ADDRESS ((uint32)0x020000U) //ETAS BIP Porting
#define IMAGEM_FBL_REGION_SIZE  ((uint32)(256U*1024U)) //ETAS BIP Porting

#define IMAGEM_APPLICATION_REGION_START_ADDRESS  ((uint32)0x080000U) //ETAS BIP Porting
#define IMAGEM_APPLICATION_REGION_SIZE           ((uint32)(548U*1024U)) //ETAS BIP Porting //zco7sgh align with ldf

#define IMAGEM_DATA_REGION_START_ADDRESS         ((uint32)0x00C000U) //ETAS BIP Porting
#define IMAGEM_DATA_REGION_SIZE                  ((uint32)16U*1024U)

#define IMAGEM_SBL_REGION_START_ADDRESS          ((uint32)0x007B0000U) //ETAS BIP Porting
#define IMAGEM_SBL_REGION_SIZE                   ((uint32)(32*1024U)) //ETAS BIP Porting

#define IMAGEM_APPLICATION1_REGION_START_ADDRESS ((uint32)0x100000U) //ETAS BIP Porting
#define IMAGEM_APPLICATION1_REGION_SIZE          ((uint32)(1024U*1024U)) //ETAS BIP Porting

#define IMAGEM_APPLICATION2_REGION_START_ADDRESS ((uint32)0x200000U) //ETAS BIP Porting
#define IMAGEM_APPLICATION2_REGION_SIZE          ((uint32)(1024U*1024U)) //ETAS BIP Porting

#define IMAGEM_APPLICATION3_REGION_START_ADDRESS ((uint32)0x300000U) //ETAS BIP Porting
#define IMAGEM_APPLICATION3_REGION_SIZE          ((uint32)(1024U*1024U)) //ETAS BIP Porting

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/
#ifndef MEM_COPY
#define MEM_COPY(pDest, pSrc, u32_NumBytes)    rba_BswSrv_MemCopy(pDest, pSrc, u32_NumBytes)
#endif

#ifndef MEM_SET
#define MEM_SET(pDest, xPattern, u32_NumBytes)    rba_BswSrv_MemSet(pDest, xPattern, u32_NumBytes)
#endif
/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/


const ImageM_FlashAreaType PrimarySlotFlashBootloader = {                                          /* PRQA S 1504, 3408 */
    .Address = IMAGEM_FBL_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                     /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(0),        /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_FBL_REGION_SIZE,
};

const ImageM_FlashAreaType PrimarySlotAplication = {                                    /* PRQA S 1504, 3408 */
    .Address = IMAGEM_APPLICATION_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                      /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(1),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_APPLICATION_REGION_SIZE,
};

const ImageM_FlashAreaType PrimarySlotCalibration = {                                   /* PRQA S 1504, 3408 */
    .Address = IMAGEM_DATA_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                      /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(2),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_DATA_REGION_SIZE,
};

const ImageM_FlashAreaType PrimarySlotSecondaryBootloader = {                                   /* PRQA S 1504, 3408 */
    .Address = IMAGEM_SBL_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                     /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(3),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_SBL_REGION_SIZE,
};
const ImageM_FlashAreaType PrimarySlotAplication1 = {                                    /* PRQA S 1504, 3408 */
    .Address = IMAGEM_APPLICATION1_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                     /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(4),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_APPLICATION1_REGION_SIZE,
};
const ImageM_FlashAreaType PrimarySlotAplication2 = {                                    /* PRQA S 1504, 3408 */
    .Address = IMAGEM_APPLICATION2_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                     /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(5),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_APPLICATION2_REGION_SIZE,
};
const ImageM_FlashAreaType PrimarySlotAplication3 = {                                    /* PRQA S 1504, 3408 */
    .Address = IMAGEM_APPLICATION3_REGION_START_ADDRESS,
    .DeviceId = IMAGEM_FLASH_DEVICE_EXTERNAL_FLASH,                                     /* PRQA S 4424 */
    .FaId = IMAGEM_IMAGE_PRIMARY(6),                                                    /* PRQA S 3391, 1843 */
    .Pad = 0,
    .Size = IMAGEM_APPLICATION3_REGION_SIZE,
};

const ImageM_FlashAreaType *ImageM_FlashPort_AreaMap[] =                                /* PRQA S 1514, 3408 */
{
    &PrimarySlotFlashBootloader,
    &PrimarySlotAplication,
    &PrimarySlotCalibration,
    &PrimarySlotSecondaryBootloader,
    &PrimarySlotAplication1,
    &PrimarySlotAplication2,
    &PrimarySlotAplication3,
};

#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
static FlashParamType FlashParam = {FlashFailed, UnInitialized, 0, 0, NULL_PTR};
#endif
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

static const ImageM_FlashAreaType* ImageM_FlashPort_Prv_AreaSearch(uint8 Id)
{
    const ImageM_FlashAreaType *FlsArea = NULL_PTR;
    for (uint8 idx = 0; idx < ARRAY_SIZE(ImageM_FlashPort_AreaMap); idx++)                                              /* PRQA S 3397 */
    {
        if (ImageM_FlashPort_AreaMap[idx]->FaId == Id)
        {
            FlsArea = ImageM_FlashPort_AreaMap[idx];
            break;
        }
    }
    return FlsArea;
}
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/**
 * @brief Open the flash area base on flash area id and return the pointer to the area
 * 
 * @ DaID flash area idea
 * @param FlashAreaPtr pointer to area 
 * @return Std_ReturnType 
 */
Std_ReturnType ImageM_FlashPort_Open(uint8 FaID, const ImageM_FlashAreaType **FlashAreaPtr)                                 /* PRQA S 1503 */
{
    Std_ReturnType ret = E_NOT_OK;

    *FlashAreaPtr =  ImageM_FlashPort_Prv_AreaSearch(FaID);
    if(*FlashAreaPtr!=NULL_PTR)
    {
        ret = E_OK;
    }
    return ret;
}

void ImageM_FlashPort_Close(const ImageM_FlashAreaType *FlashAreaPtr)                                                       /* PRQA S 1503 */
{
    if (FlashAreaPtr != NULL_PTR)
    {
        FlashAreaPtr = NULL_PTR;
    }
    else
    {
        IMAGEM_REPORT_ERROR_NO_CONDITON(1,1);
    }
}


Std_ReturnType ImageM_FlashPort_Write(const ImageM_FlashAreaType *FlashAreaPtr,                                             /* PRQA S 1503 */
                                            uint32 Offset,
                                            const uint8 *Src,
                                            uint32 Len)
{
    uint32 startAddr = 0;
    Std_ReturnType ret = E_NOT_OK;

    startAddr = FlashAreaPtr->Address + Offset;
    IMAGEM_REPORT_ERROR_NOK((Src==NULL_PTR), 2, 1);
    IMAGEM_REPORT_ERROR_NOK((startAddr + Len > FlashAreaPtr->Address + FlashAreaPtr->Size), 2, 1);              /* PRQA S 3397 */
    IMAGEM_REPORT_ERROR_NOK((Len % IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE != 0), 2, 1);                              /* PRQA S 3389, 3397, 1843 */

    if (FlashAreaPtr->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    {
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        FlashParam.address = startAddr;
        FlashParam.length = Len;
        FlashParam.data = Src;
        /*Prepare FlashPara */
        OsIntegration_DisableAllInterrupt();    
        FLASH_DRIVER_WRITE(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam);                                       /* PRQA S 3200 */ /* PRQA S 1840 */ /* PRQA S 0306 */
        OsIntegration_EnableAllInterrupt();                                                                                         /* PRQA S 3200 */
#endif
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        FlashParam.address = startAddr;
        FlashParam.length = Len;
        FlashParam.data = Src;
        /*Prepare FlashPara */
        OsIntegration_DisableAllInterrupt();
#if (SBL_INTEGRATE == TRUE)
        FLASH_DRIVER_WRITE(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam); /* ETAS BIP Porting */
#else
        EXT_FLASH_DRIVER_WRITE(FlashParam.address, FlashParam.data, FlashParam.length); /* ETAS BIP Porting */
#endif 
        if (E_OK == ret)
        {
            FlashParam.errorcode = FlashOk;
        }
        OsIntegration_EnableAllInterrupt();                                                                                         /* PRQA S 3200 */
#endif
    } 
    if(FlashOk == FlashParam.errorcode)                                                     /* PRQA S 1863 */
    {
        ret = E_OK;
    }
    
    return ret;
}

Std_ReturnType ImageM_FlashPort_Erase(const ImageM_FlashAreaType *FlashAreaPtr,                                  /* PRQA S 1503 */       
                                            uint32 Offset,
                                            uint32 Len)
{
    uint32 startAddr = 0;
    Std_ReturnType ret = E_NOT_OK;
    IMAGEM_REPORT_ERROR_NOK((FlashAreaPtr==NULL_PTR), 3, 1);
    startAddr = FlashAreaPtr->Address + Offset;
    IMAGEM_REPORT_ERROR_NOK((startAddr + Len > FlashAreaPtr->Address + FlashAreaPtr->Size), 3, 1);                                      /* PRQA S 3397 */
    IMAGEM_REPORT_ERROR_NOK((Len % IMAGEM_FLASH_MIN_OF_SECTOR_SIZE != 0), 3, 1);                                                        /* PRQA S 3397, 1843 */

    if (FlashAreaPtr->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    { 
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        FlashParam.address = startAddr;
        FlashParam.length = Len;
        /*Prepare FlashPara */      
        OsIntegration_DisableAllInterrupt();
        FLASH_DRIVER_ERASE(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam);                                                 /* PRQA S 3200 */ /* PRQA S 1840 */ /* PRQA S 0306 */ /* PRQA S 3200 */
        OsIntegration_EnableAllInterrupt();
#endif                                                                                                                    /* PRQA S 3200 */
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        FlashParam.address = startAddr;
        FlashParam.length = Len;
        /*Prepare FlashPara */
        OsIntegration_DisableAllInterrupt();   
#if (SBL_INTEGRATE == TRUE)
        FLASH_DRIVER_ERASE(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam); /* ETAS BIP Porting */
#else
        EXT_FLASH_DRIVER_ERASE(FlashParam.address, FlashParam.length); /* ETAS BIP Porting */
#endif 
        if (E_OK == ret)
        {
            FlashParam.errorcode = FlashOk;
        }
        OsIntegration_EnableAllInterrupt();                                                                                         /* PRQA S 3200 */
#endif
    } 
    if(FlashOk == FlashParam.errorcode)                                                                                 /* PRQA S 1863 */
    {
        ret = E_OK;
    }
    
    return ret; 
}


Std_ReturnType ImageM_FlashPort_GetSectors(uint8 FaId,                                                                                  /* PRQA S 1503 */
                                                uint32 *OutNumSectorsPtr,
                                                ImageM_FlashSectorType *OutSectorsPtr)
{
    Std_ReturnType ret = E_NOT_OK;
    const ImageM_FlashAreaType *FlsArea = NULL_PTR;
    uint32 sectorSize = FLASH_SECTOR_SIZE;
    uint32 sectorNum = 0;
    uint32 off = 0;

    FlsArea = ImageM_FlashPort_Prv_AreaSearch(FaId);
    IMAGEM_REPORT_ERROR_NOK((FlsArea==NULL_PTR), 4, 1);
    
    
    if (FlsArea->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    {
        for (off = 0; off < FlsArea->Size; off += sectorSize)
        {
            OutSectorsPtr[sectorNum].Offset = off;
            OutSectorsPtr[sectorNum].Size = sectorSize;
            sectorNum++;
        }
        *OutNumSectorsPtr = sectorNum;
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {/* ETAS BIP Porting */
        for (off = 0; off < FlsArea->Size; off += sectorSize)
        {
            OutSectorsPtr[sectorNum].Offset = off;
            OutSectorsPtr[sectorNum].Size = sectorSize;
            sectorNum++;
        }
        *OutNumSectorsPtr = sectorNum;
    }
    if(OutSectorsPtr != NULL_PTR)
    {
        ret = E_OK;
    }
    return ret;
}

uint32 ImageM_FlashPort_GetSectorOffset(ImageM_FlashSectorType *FlashSector)                                            /* PRQA S 3673, 1503 */
{
    return FlashSector->Offset;
}

uint32 ImageM_FlashPort_GetSectorSize(ImageM_FlashSectorType *FlashSector)                                              /* PRQA S 3673, 1503 */
{
    return FlashSector->Size;
}

uint32 ImageM_FlashPort_GetAreaSize(const ImageM_FlashAreaType *FlashAreaPtr)                                           /* PRQA S 1503 */
{
    return FlashAreaPtr->Size;
}

uint8 ImageM_FlashPort_GetFlashAreaId(ImageM_SlotType Slot, uint8 ImageIndex)                                           /* PRQA S 1503 */
{
    uint8 FaId = IMAGEM_FLASH_AREA_UNDEFINE;
    switch (Slot)
    {
        case IMAGEM_PRIMARY_SLOT:
            FaId = IMAGEM_IMAGE_PRIMARY(ImageIndex);                                                    /* PRQA S 3391, 1863 */
            break;
        case IMAGEM_SECONDARY_SLOT:
            FaId = IMAGEM_IMAGE_SECONDARY(ImageIndex);                                                  /* PRQA S 3391, 1863 */
            break;
        default:
             break;
    }
    return FaId;
}

uint8 ImageM_FlashPort_GetImageIdxByFaId(uint8 FaId)        /* PRQA S 1503 */
{
    uint8 ImageIdx;
    for (ImageIdx = 0; ImageIdx < IMAGEM_IMAGE_NUMBER; ImageIdx++)
    {
        if ((FaId == IMAGEM_IMAGE_PRIMARY(ImageIdx)) || (FaId == IMAGEM_IMAGE_SECONDARY(ImageIdx)))     /* PRQA S 3391 */
        {
            break;
        }
    }
    return ImageIdx;
}

uint32 ImageM_FlashPort_Align(const ImageM_FlashAreaType *FlashAreaPtr)                                                     /* PRQA S 1503 */
{
    uint32 Align = 0;
    IMAGEM_REPORT_ERROR_NOK(FlashAreaPtr == NULL_PTR, 5, 1);
    if(FlashAreaPtr->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    {
#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
        if(FlashAreaPtr->FaId == IMAGEM_IMAGE_SCRATCH)
        {
            Align = IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE; 
        }
        else /*IMAGEM_IMAGE_SCRATCH*/
#endif
        {
            Align = IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE; 
        }/*IMAGEM_IMAGE_PRIMARY || IMAGEM_IMAGE_SECONDARY*/
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {/* ETAS BIP Porting */
#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
        if(FlashAreaPtr->FaId == IMAGEM_IMAGE_SCRATCH)
        {
            Align = IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE; 
        }
        else /*IMAGEM_IMAGE_SCRATCH*/
#endif
        {
            Align = IMAGEM_FLASH_MIN_OF_BYTE_TO_WRITE; 
        }/*IMAGEM_IMAGE_PRIMARY || IMAGEM_IMAGE_SECONDARY*/
    }
    return Align;
}

uint8 ImageM_FlashPort_GetErasedValue(const ImageM_FlashAreaType *FlashAreaPtr)                                                 /* PRQA S 1503 */
{
    uint8 ErasedValue = 0x00;

    IMAGEM_REPORT_ERROR_NOK(FlashAreaPtr == NULL_PTR, 6, 1);

    if(FlashAreaPtr->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    {
        ErasedValue = IMAGEM_INTERNAL_FLASH_ERASE_VALUE;
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {
        ErasedValue = IMAGEM_EXTERNAL_FLASH_ERASE_VALUE; /* ETAS BIP Porting */
    }
    return ErasedValue;
}


Std_ReturnType ImageM_FlashPort_Read(const ImageM_FlashAreaType *FlashAreaPtr, uint32 Offset, uint8 *Dst, uint32 Len)                       /* PRQA S 1503 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint32 ReadAddr;
    IMAGEM_REPORT_ERROR_NOK(Offset  + Len > FlashAreaPtr->Size, 7, IMAGEM_E_INVALID_MEM);                                                   /* PRQA S 3397 */
    IMAGEM_REPORT_ERROR_NOK(Dst == NULL_PTR, 7, IMAGEM_E_PARAM_ADDRESS);

    if(FlashAreaPtr->DeviceId == IMAGEM_FLASH_DEVICE_INTERNAL_FLASH)
    {
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        /*Convert to a absolute address*/
        ReadAddr = FlashAreaPtr->Address + Offset; 
        MEM_COPY(Dst, (const void *)ReadAddr, Len);                                                                                         /* PRQA S 0306, 3200 */
        ret = E_OK;
#endif
    }
    else /* FlashAreaPtr->DeviceId == IMAGEM_EXTERNAL_FLASH_ERASE_VALUE */
    {
#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)
        /*Convert to a absolute address*/
#if (SBL_INTEGRATE == TRUE)
        FlashParam.address = FlashAreaPtr->Address + Offset;
        FlashParam.length = Len;
        FlashParam.data = Dst;
        FLASH_DRIVER_READ(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam);                                                 /* PRQA S 3200 */ /* PRQA S 1840 */ /* PRQA S 0306 */ /* PRQA S 3200 */
#else
        ReadAddr = FlashAreaPtr->Address + Offset; 
        EXT_FLASH_DRIVER_READ(ReadAddr, &Dst, Len); /* ETAS BIP Porting */
#endif 
#endif
    }

    return ret;
}

Std_ReturnType ImageM_FlashPort_Init(void)                       /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;

#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)    
#if (SBL_INTEGRATE == TRUE)
        /* Activate the SBL by invoking FLASH_DRIVER_INIT*/
        FLASH_DRIVER_INIT(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam);                                                                 /* PRQA S 3200 *//* PRQA S 3200 */ /* PRQA S 1840 */ /* PRQA S 0306 */
#else
        EXT_FLASH_DRIVER_INIT(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam); /* ETAS BIP Porting */
#endif 
#endif
    if(FlashOk == FlashParam.errorcode)                                                                                                                 /* PRQA S 1863 */
    {
        Ret = E_OK;
    }
    return Ret;
}

Std_ReturnType ImageM_FlashPort_DeInit(void)                       /* PRQA S 1503 */
{
    Std_ReturnType Ret = E_NOT_OK;

#if (BOOTLOADER_COMPONENT_ARCHITECTURE == 2)    
#if (SBL_INTEGRATE == TRUE)
        /* Deactivate the SBL by invoking FLASH_DRIVER_DEINIT*/
        FLASH_DRIVER_DEINIT(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam);                                                             /* PRQA S 3200 */ /* PRQA S 1840 */ /* PRQA S 0306 */
#else
        EXT_FLASH_DRIVER_DEINIT(IMAGEM_SBL_REGION_START_ADDRESS, &FlashParam); /* ETAS BIP Porting */
#endif 
#endif 
    if(FlashOk == FlashParam.errorcode)                                                                                                  /* PRQA S 1863 */
    {
        Ret = E_OK;
    }
    return Ret;
}
