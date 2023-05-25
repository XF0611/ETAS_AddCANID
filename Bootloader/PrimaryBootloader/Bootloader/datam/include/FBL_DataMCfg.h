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
#ifndef  FBL_DATAMCFG_H
#define  FBL_DATAMCFG_H
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Std_Types.h"
#include "rba_BswSrv.h"
#include "FBL_DataMPrv.h"
#include "ImageM_Config.h"
/*********************************************************************************************************************
* Exported Defines
*********************************************************************************************************************/
#define FBL_DATAM_OPERATION_A_BLOCK_PENDING_TIMEOUT     (1500U) /*Timeout of each block in ms*/

/*********************************************************************************************************************
* Exported Macros
*********************************************************************************************************************/
#define Fbl_DataM_MemCopy(pDest, pSrc, u32_NumBytes)            rba_BswSrv_MemCopy(pDest, pSrc, u32_NumBytes)
#define Fbl_DataM_MemSet(pDest, xPattern, u32_NumBytes)         rba_BswSrv_MemSet(pDest, xPattern, u32_NumBytes)
/*********************************************************************************************************************
* Exported Types
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Variables
*********************************************************************************************************************/


/*********************************************************************************************************************
* Exported Functions
*********************************************************************************************************************/
/* Read_xxx for data stored in NvM */
extern Std_ReturnType FBL_DataM_ReadNvmReprogrammingRequestFlag(uint8 *data);
/* Write_xxx for data stored in NvM */
extern Std_ReturnType FBL_DataM_WriteNvmReprogrammingRequestFlag(const uint8 *data);

#ifdef IMAGEM_SWAP_USING_SCRATCHPAD
extern Std_ReturnType FBL_DataM_WriteNvmAppImageStatus(const uint8 *data);
extern Std_ReturnType FBL_DataM_WriteNvmCalImageStatus(const uint8 *data);
extern Std_ReturnType FBL_DataM_ReadNvmAppImageStatus(uint8 *data);
extern Std_ReturnType FBL_DataM_ReadNvmCalImageStatus(uint8 *data);
#elif defined (IMAGEM_SWAP_USING_HW_DUALBANK)  /*HW SWAP DUAL BANK */
extern Std_ReturnType FBL_DataM_ReadNvmImageUpdateState(uint8 *data) ;
extern Std_ReturnType FBL_DataM_SynchWriteNvmImageUpdateState(const uint8 *data);
#endif  /*IMAGEM_SWAP_USING_SCRATCHPAD */
/* Get status of blocks to store NvM data */
extern boolean FBL_DataM_IsPersistedNvMReprogrammingRequestFlag(void);


/* Read fingerprint from NVM */

extern Std_ReturnType FBL_DataM_ReadBootSoftwareNvmFingerprint(uint8 *data);
extern Std_ReturnType FBL_DataM_ReadApplicationDataNvmFingerprint(uint8 *data);
extern Std_ReturnType FBL_DataM_ReadApplicationSoftwareNvmFingerprint(uint8 *data);



/* Write fingerprint to NVM */

Std_ReturnType FBL_DataM_WriteApplicationDataNvmFingerprint(const uint8 *data);
Std_ReturnType FBL_DataM_WriteBootSoftwareNvmFingerprint(const uint8 *data);
Std_ReturnType FBL_DataM_WriteApplicationSoftwareNvmFingerprint(const uint8 *data);


/* NvM block callback functions */
extern Std_ReturnType Fbl_DataM_NV_ReprogrammingRequestFlagBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);
extern Std_ReturnType Fbl_DataM_NV_FingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);
extern Std_ReturnType Fbl_DataM_NV_BootSoftwareFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);
extern Std_ReturnType Fbl_DataM_NV_ApplicationSoftwareFingerprintCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);
extern Std_ReturnType Fbl_DataM_NV_ApplicationDataFingerprintBlockCallbackFunction(uint8 ServiceId, NvM_RequestResultType JobResult);

#endif
