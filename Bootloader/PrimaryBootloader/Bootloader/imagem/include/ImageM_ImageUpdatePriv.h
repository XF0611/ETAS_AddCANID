/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_LoaderState.h
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

#ifndef  IMAGEM_BOOTPRIV_H
#define  IMAGEM_BOOTPRIV_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "ImageM_Config.h"
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

/**
 * @brief describe the infomation of image
 * 
 */
struct ImageM_ImageUpdateInfor
{
    uint32 ReserveHdr[8];
    const ImageM_FlashAreaType *Area;
    ImageM_FlashSectorType *Sectors;
    uint32 NumSectors;
};

/**
 * @brief describe the state of the supported images during boot
 * 
 */
typedef struct
{
    struct ImageM_ImageUpdateInfor Images[IMAGEM_IMAGE_NUMBER][IMAGEM_IMAGE_NUM_SLOTS];
#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
    struct ImageM_ScratchInfor ScratchPad;
#endif
    uint32 WriteSz;
    uint32 CurrImageIdx;
    uint8 SwapType[IMAGEM_IMAGE_NUMBER];
#ifdef IMAGEM_SWAP_USING_HW_DUALBANK
    uint8 NextAction;
#endif
    uint16 Pad;
}ImageM_ImageUpdateStateType;


/**
 * @brief A response object indicates where to jump to the main image execution.
 * 
 */
typedef struct
{
    uint32 ImageAddress;            /* start address of image, always follow address of image header*/
    uint8 FdId;                     /* Flash Device identification*/
    uint8 Pad;
    uint16 Pad1;
}ImageM_ImageUpdateResponseType;

typedef struct
{
    uint32 CurrImgIdx;
    uint32 Size;
    uint8 ErrorStatus;
    uint8 Sync[IMAGEM_IMAGE_NUMBER];
    boolean IsSync;
    const ImageM_FlashAreaType *SrcFa;
    const ImageM_FlashAreaType *DestFa;
}ImageM_ImageUpdateSyncStatusType;

typedef enum
{
    IMAGEM_SYNC_NONE              = 0x00,
    IMAGEM_SYNC_INSPECTED         = 0xA1,
    IMAGEM_SYNC_INITOBJECT        = 0xA2,
    IMAGEM_SYNC_ERASED            = 0xA3,
    IMAGEM_SYNC_WROTE             = 0xA4,
    IMAGEM_SYNC_VERIFIED          = 0xA5,
    IMAGEM_SYNC_DONE              = 0xA6,
    IMAGEM_SYNC_ERROR             = 0xFF
}ImageM_ImageUpdate_SyncType;

#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
/**
 * @brief maintain state of copy progress during swap using scratch pad
 * 
 */
typedef struct
{
    uint32 SwapIdx;
    uint8 State;
    uint8 Swaptype;
    uint32 SwapSize;
    int Source;
}ImageM_ImageUpdateStatusType;

/**
 * @brief a swap status is detected. in case of the interruptible swapping happen. 
 * 
 */
typedef enum
{
    IMAGEM_SWAP_STATUS_NEW      = 0x0F,
    IMAGEM_SWAP_STATUS_SWAPPING = 0x07,
    IMAGEM_SWAP_STATUS_BACKUP   = 0x03,
    IMAGEM_SWAP_STATUS_UPDATED  = 0x00, /*default value */
}IMageM_ImageUpdate_SwapStatusType;

typedef enum
{
    IMAGEM_SWAP_TYPE_NONE       = 0x00,
    IMAGEM_SWAP_TYPE_TEST       = 0x01,
    IMAGEM_SWAP_TYPE_PERMANENT  = 0x02,
    IMAGEM_SWAP_TYPE_REVERT     = 0x03,
    IMAGEM_SWAP_TYPE_PANIC      = 0x04,
}IMageM_ImageUpdate_SwapType;

#elif defined (IMAGEM_SWAP_USING_HW_DUALBANK)  /*HW SWAP DUAL BANK */
typedef enum
{
    IMAGEM_SWAP_TYPE_NONE           = 0x00,
    IMAGEM_SWAP_TYPE_UPDATE         = 0x01,
    IMAGEM_SWAP_TYPE_POST_UPDATE    = 0x02,
    IMAGEM_SWAP_TYPE_TEST           = 0x03,
    IMAGEM_SWAP_TYPE_POST_TEST      = 0x04,
    IMAGEM_SWAP_TYPE_PERMANENT      = 0x05,
    IMAGEM_SWAP_TYPE_POST_PERMANENT = 0x06,
    IMAGEM_SWAP_TYPE_REVERT         = 0x07,
    IMAGEM_SWAP_TYPE_POST_REVERT    = 0x08,
    IMAGEM_SWAP_TYPE_SYNC           = 0x09,
    IMAGEM_SWAP_TYPE_PREUPDATE      = 0x0A,
    IMAGEM_SWAP_TYPE_PANIC          = 0x0F,
}ImageM_ImageUpdate_SwapType;

typedef enum
{
    IMAGEM_ACTION_NONE                  = 0x00,
    IMAGEM_ACTION_RESET_ONLY            = 0x01,
    IMAGEM_ACTION_SWAP_RESET            = 0x02,
    IMAGEM_ACTION_RUN_FIRST_TEST        = 0x03,
    IMAGEM_ACTION_SET_APPVALID_RESET    = 0x04,
}ImageM_ImageUpdate_ActionType;
#endif /* IMAGEM_SWAP_USING_SCRATCHPAD */


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/

#endif /*IMAGEM_BOOTPRIV_H*/
