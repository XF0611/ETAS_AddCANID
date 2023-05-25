/*********************************************************************************************************************
*
* Description:     
* FileName:        ImageM_ImageDualBank.h
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
UCN1HC          2022/02/27      initial commit
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "PreEraseUltil.h"
#include "ImageM_Config.h"
#include "ImageM_FlashPort.h"
#include "DataM_MetaDataInfos.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define PRE_ERASED_STATE_TRUE       0x0AU
#define PRE_ERASED_STATE_FALSE      0x00U
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
static uint8 PreEraseUntil_Prv_GetImageIdxByFaId(uint8 FaId);
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
uint8 PreEraseUntil_Prv_GetImageIdxByFaId(uint8 FaId)
{                                        /* PRQA S 3224 */
    uint8 ImageIdx;
    for (ImageIdx = 0; ImageIdx < IMAGEM_IMAGE_NUMBER; ImageIdx++)
    {
        if ((FaId == (IMAGEM_IMAGE_PRIMARY(ImageIdx))) || (FaId == IMAGEM_IMAGE_SECONDARY(ImageIdx)))               /* PRQA S 3391 */   
        {
            break;
        }
    }
    return ImageIdx;
}
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/**
 * @brief Check if the area is empty. If yes, return TRUE
 * 
 * @param ImageId 
 * @param IsEmpty 
 * @return Std_ReturnType 
 */
Std_ReturnType PreEraseUntil_CheckEmpty(uint8 ImageId, boolean *IsEmpty)
{
    Std_ReturnType Ret = E_OK;
    uint8 PreErasedFlag[IMAGEM_IMAGE_NUMBER] = {0};
    uint8 ImageIdx = 0;

    ImageIdx = PreEraseUntil_Prv_GetImageIdxByFaId(ImageId);

    Ret = DataM_MDInfos_ReadPreErasedFlag((uint8 *)&PreErasedFlag);                 /* PRQA S 0310 */
    if ((Ret == E_OK) && (PreErasedFlag[ImageIdx] == PRE_ERASED_STATE_TRUE))
    {
        *IsEmpty = TRUE;
    }
    else
    {
        *IsEmpty = FALSE;
    }
    return Ret;    
}
/**
 * @brief Set the state of PreErase
 * TRUE - The area was erased
 * FALSE - The area was occupied by some data
 * 
 * @param ImageId 
 * @param State 
 * @return Std_ReturnType 
 */
Std_ReturnType PreEraseUntil_WritePreEraseState(uint8 ImageId, boolean State)
{
    uint8 ImageIdx = 0;
    Std_ReturnType  Ret = E_NOT_OK;
    uint8 PreErasedFlag[IMAGEM_IMAGE_NUMBER] = {0};
    
    ImageIdx = PreEraseUntil_Prv_GetImageIdxByFaId(ImageId);
    Ret = DataM_MDInfos_ReadPreErasedFlag((uint8 *)&PreErasedFlag);                 /* PRQA S 0310 */
    if (Ret == E_OK)
    {
        if (State == TRUE)
        {
            PreErasedFlag[ImageIdx] = PRE_ERASED_STATE_TRUE;
        }
        else
        {
            PreErasedFlag[ImageIdx] = PRE_ERASED_STATE_FALSE;
        }
        Ret = DataM_MdInfos_WritePreErasedFlag((const uint8 *)&PreErasedFlag);                  /* PRQA S 0310 */
    } 
    return Ret;
}
