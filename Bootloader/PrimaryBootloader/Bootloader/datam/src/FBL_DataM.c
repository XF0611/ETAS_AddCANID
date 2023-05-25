/*********************************************************************************************************************
*
* Description:     DataM module implementation
* FileName:        FBL_DataM.c
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

#include "FBL_DataM.h"
#include "FBL_DataMPrv.h"
#include "FBL_DataMPrvCfg.h"
#include "Os_TimeServices.h"
#include "BLSM.h"
#include "FBL_DataMPrv.h"
#include "NvM.h"
#include "MemIf.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/**
 * @brief
 *
 */
typedef enum
{
  FBL_DATAM_CMPSTATE_NOT_INIT,
  FBL_DATAM_CMPSTATE_NOT_COMPLETED,
  FBL_DATAM_CMPSTATE_MIRROR_RAM_IS_LOADED
}Fbl_DataM_CmpStateType;

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
static Fbl_DataM_CmpStateType Fbl_DataM_CmpState = FBL_DATAM_CMPSTATE_NOT_INIT;
/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/
static void Fbl_DataM_WriteBlockProcess(void);
static void Fbl_DataM_RequestWriteBlock(uint16 blockIndex);
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/**
 * @brief the API will trigger the write of a block if the status is FBL_DATAM_BLOCK_STATUS_NOT_SAVED
 * @return none
 */
static void Fbl_DataM_WriteBlockProcess(void)
{
    boolean timeout = FALSE;
    uint16 index;

    for(index = 0; index < FBL_DATAM_NUM_NVM_BLOCKS; index++)
    {
        if(FBL_DATAM_BLOCK_STATUS_PENDING == Fbl_DataM_NvmBlockStats[index].BlockStatus)
        {
            timeout = OS_Timer32_IsElapsed(Fbl_DataM_NvmBlockStats[index].BlockStartTimeStamp, FBL_DATAM_OPERATION_A_BLOCK_PENDING_TIMEOUT);
            if(TRUE == timeout)
            {
                /*rewrite if failed */
                Fbl_DataM_NvmBlockStats[index].BlockStatus = FBL_DATAM_BLOCK_STATUS_NOT_SAVED;
                Fbl_DataM_NvmBlockStats[index].NvmBlockFault = TRUE;
            }
        }
        else if((FBL_DATAM_BLOCK_STATUS_NOT_SAVED == Fbl_DataM_NvmBlockStats[index].BlockStatus) &&
                (FALSE == Fbl_DataM_NvmBlockStats[index].NvmBlockFault))
        {
            /*Nvm Write block request */
            Fbl_DataM_RequestWriteBlock(index);
        }
        else
        {
            /*Nothing to do FBL_DATAM_BLOCK_STATUS_SAVED*/
        }
    }
}

/**
 * @brief request to write a block.
 * 
 * @param blockIndex block index of user define refer to #FBL_DATAM_NVM_BLOCK_ID
 */
static void Fbl_DataM_RequestWriteBlock(uint16 blockIndex)
{
    Std_ReturnType ret                              = E_NOT_OK;
    const Fbl_DataM_BlockDescriptorType *pBlockInfo = &Fbl_DataM_NvmBlockInfo[blockIndex];
    Fbl_DataM_NvmBlockStatsType *pBlockStats        = &Fbl_DataM_NvmBlockStats[blockIndex];

    /*Start elapse time of a block*/
    OS_Timer32_Start(&pBlockStats->BlockStartTimeStamp);

    /*NVM Writes the block */
    ret = NvM_WriteBlock(pBlockInfo->NvMBlockId, pBlockInfo->RamMirror);

    if(E_OK == ret)
    {
        /* Set status of the block to  FBL_DATAM_BLOCK_STATUS_PENDING*/
        pBlockStats->BlockStatus = FBL_DATAM_BLOCK_STATUS_PENDING;
    }
    else
    {
        /*Stop timestamp of the block */
        OS_Timer32_Stop(&pBlockStats->BlockStartTimeStamp);
    }
}
/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/**
 * @brief the API is to initiate the DataM component before scheduling.
 * @return none
 */
void Fbl_DataM_Init(void)                                                                                           /* PRQA S 1503 */
{
    uint32 index;
    Fbl_DataM_RamMirrorLoaded = FALSE;
    /*initialize Nvm block stats */
    for(index = 0; index < FBL_DATAM_NUM_NVM_BLOCKS; index++)
    {
        Fbl_DataM_NvmBlockStats[index].BlockStatus          = FBL_DATAM_BLOCK_STATUS_SAVED;
        Fbl_DataM_NvmBlockStats[index].NvmBlockFault        = FALSE;
        Fbl_DataM_NvmBlockStats[index].BlockStartTimeStamp  = 0;
    }
    /*Set State transition to FBL_DATAM_CMPSTATE_RUNNING_MIRROR_NOT_LOADED*/
    Fbl_DataM_CmpState = FBL_DATAM_CMPSTATE_NOT_COMPLETED;
}

/**
 * @brief the API is to de-initiate the DataM component.
 * @return none
 */
void Fbl_DataM_DeInit(void)                                                                                 /* PRQA S 1503 */
{
    uint32 index;
    /*initialize Nvm block stats */
    for(index = 0; index < FBL_DATAM_NUM_NVM_BLOCKS; index++)
    {
        Fbl_DataM_NvmBlockStats[index].BlockStatus          = FBL_DATAM_BLOCK_STATUS_SAVED;
        Fbl_DataM_NvmBlockStats[index].NvmBlockFault        = FALSE;
        Fbl_DataM_NvmBlockStats[index].BlockStartTimeStamp  = 0;
        Fbl_DataM_NvmBlockInfo[index].RamMirror             = NULL_PTR;
        Fbl_DataM_NvmBlockInfo[index].Size                  = 0;
        Fbl_DataM_NvmBlockInfo[index].NvMBlockId            = 0;
    }
    /*Set State transition to FBL_DATAM_CMPSTATE_RUNNING_MIRROR_NOT_LOADED*/
    Fbl_DataM_CmpState = FBL_DATAM_CMPSTATE_NOT_INIT;
}

/**
 * @brief the API is the main function of DataM component.
 * This is a signal handling which is the trigger to write data into flash via Nvm
 * this API shall be called in the loop infinity.
 * @return none
 */
void Fbl_DataM_MainFunction(void)                                                                               /* PRQA S 1503 */
{
    switch (Fbl_DataM_CmpState)
    {
    case FBL_DATAM_CMPSTATE_NOT_INIT:
        /*Nothing todo here. consider to implement a sleep transition */
        break;
    case FBL_DATAM_CMPSTATE_NOT_COMPLETED:
        /*check if  ram mirror was loaded*/
        if(TRUE == Fbl_DataM_RamMirrorLoaded)
        {
            /*Change state transition to FBL_DATAM_CMPSTATE_RUNNING_MIRROR_LOADED */
            Fbl_DataM_CmpState = FBL_DATAM_CMPSTATE_MIRROR_RAM_IS_LOADED;
        }
        break;
    case FBL_DATAM_CMPSTATE_MIRROR_RAM_IS_LOADED:
        /*process write block */
        Fbl_DataM_WriteBlockProcess();
        break;

    default:
        break;
    }
}

/**
 * @brief Nvm Wraper is called by Nvm.
 * 
 * @param cnServiceIdP service ID of Nvm
 * @param cnJobResultP job result of Nvm refer to #NvM_RequestResultType
 */
void Fbl_DataM_MultiBlockJobEndNotification (uint8 ServiceId, NvM_RequestResultType JobResult)                          /* PRQA S 1503 */
{
    if (NVM_SERVICE_ID_READ_ALL == ServiceId)
    {
        Fbl_DataM_RamMirrorLoaded = TRUE;
    }
    /*notify to BLSM to notify a job end*/
    Fbl_BLSM_NfyDataMJobEnd (ServiceId, JobResult);
}
