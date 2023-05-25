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
#ifndef  FBL_DATAMPRVCFG_H
#define  FBL_DATAMPRVCFG_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/

#include "NvM.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
/* The total number of blocks */
#define FBL_DATAM_NUM_NVM_BLOCKS        NVM_PRV_NR_PERSISTENT_IDS
/* The first block shall be used firstly*/
#define FBL_DATAM_NUM_NVM_FIRST_USE     NVM_CFG_FIRST_USED_BLOCK
/* Block ID of user define*/
#define FBL_DATAM_NVM_BLOCK_ID(id)      (id - FBL_DATAM_NUM_NVM_FIRST_USE)
#define FBL_DATAM_BLOCKINDEX_TO_NVID(index)  (index + FBL_DATAM_NUM_NVM_FIRST_USE)
/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/
typedef struct
{
    uint8 *RamMirror;
    uint8 Size;
    uint16 NvMBlockId;
} Fbl_DataM_BlockDescriptorType;

/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/
extern uint8 Fbl_DataM_RamMirrorNV_ReprogrammingRequestFlagBlock[];
extern const uint8 Fbl_DataM_RomDataNV_ReprogrammingRequestFlagBlock[];
extern uint8 Fbl_DataM_RamMirrorNV_FingerprintBlock[];
extern const uint8 Fbl_DataM_RomDataNV_FingerprintBlock[];

extern uint8 Fbl_DataM_RamMirrorNV_ApplicationDataFingerprintBlock[];
extern uint8 Fbl_DataM_RamMirrorNV_ApplicationSoftwareFingerprintBlock[];
extern uint8 Fbl_DataM_RamMirrorNV_BootSoftwareFingerprintBlock[];
extern const uint8 Fbl_DataM_RomDataNV_ApplicationDataFingerprintBlock[];
extern const uint8 Fbl_DataM_RomDataNV_BootSoftwareFingerprintBlock[];
extern const uint8 Fbl_DataM_RomDataNV_ApplicationSoftwareFingerprintBlock[];

extern Fbl_DataM_BlockDescriptorType Fbl_DataM_NvmBlockInfo[FBL_DATAM_NUM_NVM_BLOCKS];
extern Fbl_DataM_BlockDescriptorType AppData;
extern Fbl_DataM_BlockDescriptorType ApplicationData;
extern Fbl_DataM_BlockDescriptorType ApplicationSoftware;
extern Fbl_DataM_BlockDescriptorType BootSoftware;
extern Fbl_DataM_BlockDescriptorType External_ReprogrammingRequest;



/*********************************************************************************************************************
* Exported functions
*********************************************************************************************************************/

#endif
