/*********************************************************************************************************************
*
* Description:     DataM module configuration implementation
* FileName:        FBL_DataMCfg.c
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

#include "FBL_DataMCfg.h"
#include "FBL_DataMPrv.h"

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

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

Std_ReturnType FBL_DataM_ReadNvmReprogrammingRequestFlag(uint8 *data)                                                           /* PRQA S 1503 */
{
    return Fbl_DataM_ReadFromNvmBlockVarLength( data,
                                                Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock);
}

Std_ReturnType FBL_DataM_WriteNvmReprogrammingRequestFlag (const uint8 * data)                                                  /* PRQA S 1503 */
{
  Std_ReturnType Ret;
  Ret = Fbl_DataM_WriteToNvmBlockVarLength(Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock, 
                                            data, 
                                            FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock), 
                                            NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock);
  return Ret;
}
/********************************************************************************************************************
 * NvM block callback functions
 */

Std_ReturnType Fbl_DataM_NV_ReprogrammingRequestFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)         /* PRQA S 1503 */
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock));
    return ret;
}

/*
 * Define function to check writing NvM status.
 */
boolean FBL_DataM_IsPersistedNvMReprogrammingRequestFlag(void)                                                                      /* PRQA S 1503 */
{
    boolean retVal = FALSE;
    Fbl_DataM_NvmBlockStatsType *NvMBlockState = &Fbl_DataM_NvmBlockStats[FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock)];

    if((NvMBlockState->BlockStatus == FBL_DATAM_BLOCK_STATUS_SAVED) && (NvMBlockState->NvmBlockFault == FALSE))
    {
        retVal = TRUE;
    }
    return retVal;
}

/*  
 *  Define function to ReadFingerPrint.
 */
Std_ReturnType FBL_DataM_ReadApplicationDataNvmFingerprint(uint8 *data)
{
    return Fbl_DataM_ReadFromNvmBlockVarLength( data,
                                                ApplicationData.RamMirror,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock);
}
Std_ReturnType FBL_DataM_ReadBootSoftwareNvmFingerprint(uint8 *data)
{
    return Fbl_DataM_ReadFromNvmBlockVarLength( data,
                                                BootSoftware.RamMirror,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock);
}
Std_ReturnType FBL_DataM_ReadApplicationSoftwareNvmFingerprint(uint8 *data)
{               
    return Fbl_DataM_ReadFromNvmBlockVarLength( data,
                                                ApplicationSoftware.RamMirror,
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock);
}

/*  
 *  Define function to WriteFingerPrint.
 */
Std_ReturnType FBL_DataM_WriteApplicationDataNvmFingerprint(const uint8 *data)
{
    return Fbl_DataM_WriteToNvmBlockVarLength(  (uint8 *)ApplicationData.RamMirror,
                                                data,
                                                FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock),
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock);
}
Std_ReturnType FBL_DataM_WriteBootSoftwareNvmFingerprint(const uint8 *data)
{
    return Fbl_DataM_WriteToNvmBlockVarLength(  (uint8 *)BootSoftware.RamMirror,
                                                data,
                                                FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock),
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock);
}
Std_ReturnType FBL_DataM_WriteApplicationSoftwareNvmFingerprint(const uint8 *data)
{
    return Fbl_DataM_WriteToNvmBlockVarLength(  (uint8 *)ApplicationSoftware.RamMirror,
                                                data,
                                                FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock),
                                                NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock);
}
/*
 * Define function to check writing NvM status.
 */
Std_ReturnType Fbl_DataM_NV_ApplicationDataFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock));
    return ret;
}
Std_ReturnType Fbl_DataM_NV_BootSoftwareFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock));
    return ret;
}
Std_ReturnType Fbl_DataM_NV_ApplicationSoftwareFingerprintCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult)
{
    Std_ReturnType ret = E_OK;
    Fbl_DataM_UpdateBlockStatusEvent(   ServiceId,
                                        JobResult,
                                        FBL_DATAM_NVM_BLOCK_ID(NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock));
    return ret;
}
