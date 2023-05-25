/*********************************************************************************************************************
*
* Description:     DataM module private implementation
* FileName:        FBL_DataMPrv.c
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
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "FBL_DataMPrv.h"
#include "Os_TimeServices.h"
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
Fbl_DataM_NvmBlockStatsType Fbl_DataM_NvmBlockStats[FBL_DATAM_NUM_NVM_BLOCKS];                                              /* PRQA S 1504 */
extern Fbl_DataM_BlockDescriptorType Fbl_DataM_NvmBlockInfo[FBL_DATAM_NUM_NVM_BLOCKS];                                             /* PRQA S 1504 */ /* PRQA S 3447 */ /* PRQA S 3451 */
boolean Fbl_DataM_RamMirrorLoaded = FALSE;                                                                                  /* PRQA S 1514 */

/*********************************************************************************************************************
* Implement Functions Prototypes
*********************************************************************************************************************/

/**
 * @brief The API is to update the status when the writing of a block has been completed.
 *
 * @param u8ServiceIdP [in] service ID of NVM 
 * @param JobResultP [in] the job results of NvM refer to #NvM_RequestResultType
 * @param BlockIndex_u8 [in] block ID of a block refer to #FBL_DATAM_NVM_BLOCK_ID
 */
void Fbl_DataM_UpdateBlockStatusEvent(  const uint8 u8ServiceIdP,                                                           /* PRQA S 1503 */
                                        const NvM_RequestResultType JobResultP,
                                        const uint8 BlockIndex_u8)
{
    if(NVM_SERVICE_ID_WRITE_BLOCK == u8ServiceIdP)
    {
        if((NvM_RequestResultType)NVM_REQ_OK == JobResultP)
        {
            /*Set the block status to FBL_DATAM_BLOCK_STATUS_SAVED*/
            Fbl_DataM_NvmBlockStats[BlockIndex_u8].BlockStatus = FBL_DATAM_BLOCK_STATUS_SAVED;
        }
        else
        {
            /*Update error of block status to TRUE */
            Fbl_DataM_NvmBlockStats[BlockIndex_u8].NvmBlockFault = TRUE;
        }
    }
    else
    {
        if((NVM_SERVICE_ID_READ_BLOCK   == u8ServiceIdP) ||
            (NVM_SERVICE_ID_READ_ALL    == u8ServiceIdP))
        {
            if(((NvM_RequestResultType)NVM_REQ_INTEGRITY_FAILED    == JobResultP) ||
                ((NvM_RequestResultType)NVM_REQ_NV_INVALIDATED     == JobResultP))
            {
                /*Update error of block status to TRUE */
                Fbl_DataM_NvmBlockStats[BlockIndex_u8].NvmBlockFault = TRUE;
            }
        }
    }
    /*Reset All of Block Infos*/
    Fbl_DataM_NvmBlockInfo[BlockIndex_u8].NvMBlockId    = 0;
    Fbl_DataM_NvmBlockInfo[BlockIndex_u8].RamMirror     = NULL_PTR;
    Fbl_DataM_NvmBlockInfo[BlockIndex_u8].Size          = 0;
}


/**
 * @brief the API is to read data from Nvm ram mirror block.
 *
 * @param pTo [out] the destination is written to
 * @param pFrom [in] the ram mirror block nvm
 * @param size [in] size of ram mirror block nvm
 * @return Std_ReturnType result is E_OK        - succeeded.
 *                                  E_NOT_OK    - Failed.
 */
Std_ReturnType Fbl_DataM_ReadFromNvmBlockVarLength (uint8 *  pTo, const uint8 * pFrom, const uint8 size)                        /* PRQA S 1503 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 index;
    if ((NULL_PTR != pFrom) && (NULL_PTR != pTo) && (TRUE == Fbl_DataM_RamMirrorLoaded))
    {
		for(index = 0; index < size; index++)
		{
			pTo[index] = pFrom[index];
		}
		ret = E_OK;
    }
    return ret;
}

Std_ReturnType Fbl_DataM_ReadFromNvmBlockVarLengthFrom (uint8 *  pTo, const uint8 * pFrom, const uint8 size, const uint8 start_bit)    
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 index;
    if ((NULL_PTR != pFrom) && (NULL_PTR != pTo) && (TRUE == Fbl_DataM_RamMirrorLoaded))
    {
		for(index = start_bit; index < start_bit + size; index++)                                    /* PRQA S 3397 */
		{
			pTo[index] = pFrom[index];
		}
		ret = E_OK;
    }
    return ret;
}

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
Std_ReturnType Fbl_DataM_WriteToNvmBlockVarLength(uint8 * pTo, const uint8 *value, const uint8 blockIndex, const uint8 size)            /* PRQA S 1503 */
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 index;
    boolean isChanged = FALSE;

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
            Fbl_DataM_NvmBlockStats[blockIndex].BlockStatus = FBL_DATAM_BLOCK_STATUS_NOT_SAVED;
            /*Update Block Info*/
            Fbl_DataM_NvmBlockInfo[blockIndex].RamMirror = pTo;
            Fbl_DataM_NvmBlockInfo[blockIndex].NvMBlockId = FBL_DATAM_BLOCKINDEX_TO_NVID(blockIndex);               /* PRQA S 4491 */
            Fbl_DataM_NvmBlockInfo[blockIndex].Size = size;
        }
        ret = E_OK;
    }
    return ret;
}
