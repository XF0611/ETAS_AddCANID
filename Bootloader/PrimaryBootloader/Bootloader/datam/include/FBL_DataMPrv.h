/*********************************************************************************************************************
*
* Description:     DataM module private interface
* FileName:        FBL_DataMPrv.h
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
*/
#ifndef  FBL_DATAMPRV_H
#define  FBL_DATAMPRV_H

/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "FBL_DataMPrvCfg.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/
/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef enum
{
    FBL_DATAM_BLOCK_STATUS_SAVED,     /* Saved to dflash */
    FBL_DATAM_BLOCK_STATUS_NOT_SAVED, /* ram mirror modified and nvm write request not sent */
    FBL_DATAM_BLOCK_STATUS_PENDING    /* ram mirror modified and nvm write request sent */
} Fbl_DataM_BlockStatusType;

typedef struct
{

    Fbl_DataM_BlockStatusType   BlockStatus;
    boolean                     NvmBlockFault;
    uint32                      BlockStartTimeStamp;
}Fbl_DataM_NvmBlockStatsType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/
extern boolean Fbl_DataM_RamMirrorLoaded;
extern Fbl_DataM_NvmBlockStatsType Fbl_DataM_NvmBlockStats[FBL_DATAM_NUM_NVM_BLOCKS];
extern Fbl_DataM_BlockDescriptorType Fbl_DataM_NvmBlockInfo[FBL_DATAM_NUM_NVM_BLOCKS];
extern Fbl_DataM_BlockDescriptorType AppData;
extern Fbl_DataM_BlockDescriptorType ApplicationData;
extern Fbl_DataM_BlockDescriptorType ApplicationSoftware;
extern Fbl_DataM_BlockDescriptorType BootSoftware;
extern Fbl_DataM_BlockDescriptorType External_ReprogrammingRequest;
/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/
/**
 * @brief The API is to update the status when the writing of a block has been completed.
 *
 * @param u8ServiceIdP [in] service ID of NVM 
 * @param JobResultP [in] the job results of NvM refer to #NvM_RequestResultType
 * @param BlockIndex_u8 [in] block ID of a block refer to #FBL_DATAM_NVM_BLOCK_ID
 */
extern void Fbl_DataM_UpdateBlockStatusEvent(  const uint8 u8ServiceIdP,
                                        const NvM_RequestResultType JobResultP,
                                        const uint8 BlockIndex_u8);
/**
 * @brief 
 * 
 * @param ServiceId 
 * @param JobResult 
 */
extern void Fbl_DataM_MultiBlockJobEndNotification(uint8 ServiceId, NvM_RequestResultType JobResult);
/**
 * @brief 
 * 
 * @param pTo 
 * @param pFrom 
 * @param size 
 * @return Std_ReturnType 
 */
extern Std_ReturnType Fbl_DataM_ReadFromNvmBlockVarLength (uint8 *  pTo, const uint8 * pFrom, const uint8 size);
/**
 * @brief 
 * 
 * @param pTo 
 * @param value 
 * @param blockIndex 
 * @param size 
 * @return Std_ReturnType 
 */
extern Std_ReturnType Fbl_DataM_WriteToNvmBlockVarLength(uint8 * pTo, const uint8 *value, const uint8 blockIndex, const uint8 size);
extern Std_ReturnType Fbl_DataM_ReadFromNvmBlockVarLengthFrom (uint8 *  pTo, const uint8 * pFrom, const uint8 size, const uint8 start_bit);
#endif
