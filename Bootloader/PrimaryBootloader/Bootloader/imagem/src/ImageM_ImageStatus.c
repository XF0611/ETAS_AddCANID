
#include "ImageM_ImageUpdateStatus.h"
#include "ImageM_ImageUpdatePriv.h"
#include "ImageM_FlashArea.h"
#include "FBL_DataM.h"
#include "DataM_MetaDataInfos.h"
#include "FBL_BootMCfg.h"


Std_ReturnType ImageM_ImageStatus_ReadApplicationValidFlag( uint8 *data)                                                                                                             /* PRQA S 1503 */
{
    return DataM_MDInfos_ReadApplicationValidFlag(data);
}

Std_ReturnType ImageM_ImageStatus_SynchWriteApplicationValidFlag(uint8 isValib , uint8 ImageId)                                                                                                             /* PRQA S 1503 */
{
    uint8 AppValid[4];
    DataM_MDInfos_ReadApplicationValidFlag((uint8 *)AppValid);                                                                  /* PRQA S 3200 */
    switch (ImageId)
    {
    case IMAGEM_IMAGE_CAL_ID:
        AppValid[0] = isValib ? (uint8)(FBL_BOOTM_CALIB_VALIDITY_TRUE) : (uint8)(FBL_BOOTM_CALIB_VALIDITY_FALSE);               /* PRQA S 3344 *//* PRQA S 4559 */
        break;
    case IMAGEM_IMAGE_APP_ID:
        AppValid[1] = isValib ? (uint8)((AppValid[1] & 0xF0) | (FBL_BOOTM_APPLICATION_0_VALIDITY_TRUE >> 8u))  : (uint8)((AppValid[1] & 0xF0) | (FBL_BOOTM_APPLICATION_0_VALIDITY_FALSE >> 8u));                   /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */ /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;
    case IMAGEM_IMAGE_APP_CORE_1_ID:
        AppValid[1] = isValib ? (uint8)((AppValid[1] & 0x0F) | (FBl_BOOTM_APPLICATION_1_VALIDITY_TRUE >> 8u))  : (uint8)((AppValid[1] & 0x0F) | (FBL_BOOTM_APPLICATION_1_VALIDITY_FALSE >> 8u));                   /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */ /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;
    case IMAGEM_IMAGE_APP_CORE_2_ID:
        AppValid[2] = isValib ? (uint8)((AppValid[2] & 0xF0) | (FBL_BOOTM_APPLICATION_2_VALIDITY_TRUE >> 16u))  : (uint8)((AppValid[2] & 0xF0) | (FBL_BOOTM_APPLICATION_2_VALIDITY_FALSE >> 16u));                  /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */ /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;
    case IMAGEM_IMAGE_APP_CORE_3_ID:
        AppValid[2] = isValib ? (uint8)((AppValid[2] & 0x0F) | (FBl_BOOTM_APPLICATION_3_VALIDITY_TRUE >> 16u)): (uint8)((AppValid[2] & 0x0F) | (FBL_BOOTM_APPLICATION_3_VALIDITY_FALSE >> 16u));                  /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */  /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;
    case IMAGEM_IMAGE_APP_CORE_4_ID:
        AppValid[3] = isValib ? (uint8)((AppValid[3] & 0xF0) | (FBL_BOOTM_APPLICATION_4_VALIDITY_TRUE >> 24u)) :  (uint8)((AppValid[3] & 0xF0) | (FBL_BOOTM_APPLICATION_4_VALIDITY_FALSE >> 24u));                 /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */ /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;   
    case IMAGEM_IMAGE_APP_CORE_5_ID:
        AppValid[3] = isValib ? (uint8)((AppValid[3] & 0x0F) | (FBl_BOOTM_APPLICATION_5_VALIDITY_TRUE >> 24u)) :  (uint8)((AppValid[3] & 0x0F) | (FBL_BOOTM_APPLICATION_5_VALIDITY_FALSE >> 24u));                 /* PRQA S 1861 *//* PRQA S 4542 *//* PRQA S 1821 *//* PRQA S 4532 */ /* PRQA S 3344 *//* PRQA S 4559 *//* PRQA S 1891 */
        break;          
    default:
        break;
    }
    return FBL_DataM_SynchWriteNvmApplicationValid((const uint8 *)AppValid);
}
