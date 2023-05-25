/*********************************************************************************************************************
*
* Description:     DataM module private configuration implementation
* FileName:        FBL_DataMPrvCfg.c
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
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "NvM.h"
#include "FBL_DataMPrvCfg.h"
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
* Exported Variables Implementation
*********************************************************************************************************************/
uint8 Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock];
const uint8 Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock]
    = {0};

uint8 Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock];
uint8 Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock];
uint8 Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock];

const uint8 Fbl_DataM_RomDataNV_ApplicationDataFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock]
    = {0};
const uint8 Fbl_DataM_RomDataNV_BootSoftwareFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock]
    = {0};
const uint8 Fbl_DataM_RomDataNV_ApplicationSoftwareFingerprintBlock[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock]
    = {0};

Fbl_DataM_BlockDescriptorType Fbl_DataM_NvmBlockInfo[FBL_DATAM_NUM_NVM_BLOCKS] =
{
    {Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock},
    {Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock},
    {Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock},
    {Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock, NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock},
    {NULL_PTR, 0, 1},
    {NULL_PTR, 0, 1},
    {NULL_PTR, 0, 1},
};

Fbl_DataM_BlockDescriptorType AppData= {Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock};
Fbl_DataM_BlockDescriptorType ApplicationData = {Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_ApplicationDataFingerprintBlock};
Fbl_DataM_BlockDescriptorType ApplicationSoftware = {Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock, NvMConf_NvMBlockDescriptor_NV_ApplicationSoftwareFingerprintBlock};
Fbl_DataM_BlockDescriptorType BootSoftware = {Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock,NvMConf_NvMBlockDescriptor_NV_BootSoftwareFingerprintBlock};
Fbl_DataM_BlockDescriptorType External_ReprogrammingRequest = {Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock, NVM_CFG_NV_BLOCK_LENGTH_NV_ExternalReprogrammingRequestFlagBlock, NvMConf_NvMBlockDescriptor_NV_ExternalReprogrammingRequestFlagBlock};

