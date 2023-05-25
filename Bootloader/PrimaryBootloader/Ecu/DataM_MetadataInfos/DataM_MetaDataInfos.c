/*********************************************************************************************************************
*
* Description:     
* FileName:        DataM_MetaDataInfos.c
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
#include "DataM_MetaDataInfos.h"
#include "MemIf.h"
#include "ImageM_Config.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
uint8 Fbl_DataM_RamMirrorNV_PreErasedBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_PreErasedBlock];                                  /* PRQA S 1504, 3408 */
uint8 Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock];                /* PRQA S 1504, 3408 */
uint8 Fbl_DataM_RamMirrorNV_PBLValidFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock];        /* PRQA S 3408, 1504 */
      

const uint8 Fbl_DataM_RomDataNV_PreErasedBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_PreErasedBlock]                               /* PRQA S 3408 */
    = {0};
const uint8 Fbl_DataM_RomDataNV_ApplicationValidFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock]             /* PRQA S 3408 */
    = {0};

const uint8 Fbl_DataM_RomDataNV_PBLValidFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock]                         /* PRQA S 3408 */
    = {0};
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
/**
 * @brief wrtie to ram mirror block nvm 
 *
 * @param pTo [out] Ram mirror block
 * @param value [in] value to write
 * @param blockIndex [in] block id of user define refer #FBL_DATAM_NVM_BLOCK_ID
 * @param size [in]
 * @return Std_ReturnType result is E_OK        - succeeded.
 *                                  E_NOT_OK    - Failed.
 */
static Std_ReturnType Fbl_DataM_SynchWriteToNvmBlockVarLength(uint8 * pTo, const uint8 *value, const uint8 blockIndex, const uint8 size)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 index;
    boolean isChanged = FALSE;
    NvM_Rb_StatusType NvMStatus;
    MemIf_StatusType MemIfStatus;
    const Fbl_DataM_BlockDescriptorType *pBlockInfo = &Fbl_DataM_NvmBlockInfo[blockIndex];

    if((NULL_PTR != pTo) && (NULL_PTR != value) && (size > (uint8)0))
    {
        /*verify value change */
        for(index = 0; index < size; index++)
        {
            if(pTo[index] != value[index])
            {
                isChanged = TRUE;
                break;
            }
        }
        if(TRUE == isChanged)
        {
            for(index = 0; index < size; index++)
            {
                pTo[index] = value[index];
            }
            /*Update status as FBL_DATAM_BLOCK_STATUS_NOT_SAVED*/
                /*NVM Writes the block */
            /*Update Block Info*/
            Fbl_DataM_NvmBlockInfo[blockIndex].RamMirror = pTo;
            Fbl_DataM_NvmBlockInfo[blockIndex].NvMBlockId = FBL_DATAM_BLOCKINDEX_TO_NVID(blockIndex);               /* PRQA S 4491 */
            Fbl_DataM_NvmBlockInfo[blockIndex].Size = size;
            NvM_WriteBlock(pBlockInfo->NvMBlockId, pBlockInfo->RamMirror);                                          /* PRQA S 3200 */
            do {
                NvM_MainFunction();
                MemIf_Rb_MainFunction();

                NvM_Rb_GetStatus(&NvMStatus);                                                                       /* PRQA S 3200 */
                MemIfStatus = MemIf_Rb_GetStatus();
            } while ( (NvMStatus == NVM_RB_STATUS_BUSY ) || (MemIfStatus == MEMIF_BUSY));   
        }
        ret = E_OK;
    }
    return ret;
}
/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/********************************************************************************************************************
 * NvM block Read functions
 */
Std_ReturnType DataM_MDInfos_ReadPreErasedFlag(uint8 *data)                                                         /* PRQA S 1503 */
{
    return Fbl_DataM_ReadFromNvmBlockVarLength( data,
                                                Fbl_DataM_RamMirrorNV_PreErasedBlock,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_PreErasedBlock);
}

Std_ReturnType DataM_MDInfos_ReadApplicationValidFlag(uint8 *data)                                                  /* PRQA S 1503, 3408 */
{
    return Fbl_DataM_ReadFromNvmBlockVarLength(data,
                                                Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock);
}

Std_ReturnType DataM_MDInfos_ReadCalibValidFlag(uint8 *data)                                               /* PRQA S 1503, 3408 */
{
    return Fbl_DataM_ReadFromNvmBlockVarLength(data,
                                                Fbl_DataM_RamMirrorNV_PBLValidFlagBlock,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock);
}

Std_ReturnType DataM_MDInfos_ReadPBLValidFlag(uint8 *data)                                                /* PRQA S 1503 */
{
    return Fbl_DataM_ReadFromNvmBlockVarLength(data,
                                                Fbl_DataM_RamMirrorNV_PBLValidFlagBlock,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock);
}

/********************************************************************************************************************
 * NvM block Write functions
 */
Std_ReturnType DataM_MdInfos_WritePreErasedFlag(const uint8 * data)                                                 /* PRQA S 1503 */
{
  Std_ReturnType Ret;
  Ret = Fbl_DataM_WriteToNvmBlockVarLength(Fbl_DataM_RamMirrorNV_PreErasedBlock, 
                                            data, 
                                            FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PreErasedBlock), 
                                            NVM_CFG_NV_BLOCK_LENGTH_NV_PreErasedBlock);
  return Ret;
}

Std_ReturnType DataM_MdInfos_WriteApplicationValidFlag (const uint8 * data)                                         /* PRQA S 1503 */
{
  Std_ReturnType Ret;
  Ret = Fbl_DataM_WriteToNvmBlockVarLength(Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock, 
                                            data, 
                                            FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock), 
                                            NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock);
  return Ret;
}

Std_ReturnType DataM_MdInfos_WritePBLValidFlag (const uint8 * data)                                       /* PRQA S 1503, 3408 */
{
  Std_ReturnType Ret;
  Ret = Fbl_DataM_WriteToNvmBlockVarLength(Fbl_DataM_RamMirrorNV_PBLValidFlagBlock, 
                                            data, 
                                            FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock), 
                                            NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock);
  return Ret;
}

/********************************************************************************************************************
 * NvM block IsPersisted functions
 */
boolean DataM_MDInfos_IsPersistedNvMReadPreErasedFlag(void)                                                          /* PRQA S 1503, 3408 */       
{
    boolean retVal = FALSE;
    Fbl_DataM_NvmBlockStatsType *NvMBlockState = &Fbl_DataM_NvmBlockStats[FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PreErasedBlock)];

    if((NvMBlockState->BlockStatus == FBL_DATAM_BLOCK_STATUS_SAVED) && (NvMBlockState->NvmBlockFault == FALSE))
    {
        retVal = TRUE;
    }
    return retVal;
}

boolean DataM_MDInfos_IsPersistedNvMApplicationValidFlag(void)                                                              /* PRQA S 1503, 3408 */
{
    boolean retVal = FALSE;
    Fbl_DataM_NvmBlockStatsType *NvMBlockState = &Fbl_DataM_NvmBlockStats[FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock)];

    if((NvMBlockState->BlockStatus == FBL_DATAM_BLOCK_STATUS_SAVED) && (NvMBlockState->NvmBlockFault == FALSE))
    {
        retVal = TRUE;
    }
    return retVal;
}

boolean FBL_DataM_IsPersistedNvMPBLValidFlag(void)                                                                /* PRQA S 1503, 3408 */
{
    boolean retVal = FALSE;
    Fbl_DataM_NvmBlockStatsType *NvMBlockState = &Fbl_DataM_NvmBlockStats[FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock)];

    if((NvMBlockState->BlockStatus == FBL_DATAM_BLOCK_STATUS_SAVED) && (NvMBlockState->NvmBlockFault == FALSE))
    {
        retVal = TRUE;
    }
    return retVal;
}

/********************************************************************************************************************
 * NvM block callback functions
 */
Std_ReturnType Fbl_DataM_NV_PreErasedCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)                 /* PRQA S 1503, 3408 */
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PreErasedBlock));
    return ret;
}

Std_ReturnType Fbl_DataM_NV_ApplicationValidFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)          /* PRQA S 1503, 3408 */
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock));
    return ret;
}

Std_ReturnType Fbl_DataM_NV_PBLValidFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)        /* PRQA S 1503, 3408 */
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock));
    return ret;
}

/********************************************************************************************************************
 * NvM block write sync functions
 */

Std_ReturnType FBL_DataM_SynchWriteNvmApplicationValid(const uint8 *data)                                                         /* PRQA S 1503 */
{
    return Fbl_DataM_SynchWriteToNvmBlockVarLength(  (uint8 *)Fbl_DataM_RamMirrorNV_ApplicationValidFlagBlock,
                                                data,
                                                FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationValidFlagBlock),
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationValidFlagBlock);
}


Std_ReturnType FBL_DataM_SynchWriteNvmCalibrationValid(const uint8 *data)                                                        /* PRQA S 1503 */
{
    return Fbl_DataM_SynchWriteToNvmBlockVarLength(  (uint8 *)Fbl_DataM_RamMirrorNV_PBLValidFlagBlock,
                                                data,
                                                FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_PBLValidFlagBlock),
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_PBLValidFlagBlock);
}
