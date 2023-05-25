
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM_Prv_HideRteApi.h"

#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8 *DataIndexPtr)
{
    // TRACE[NVM449] Service for getting the currently set DataIndex of a dataset NVRAM block
    Std_ReturnType stReturn_uo = E_NOT_OK;

    // TRACE[NVM602] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_GET_DATA_INDEX, BlockId))
    {
        // TRACE[NVM604] Report the DET error NVM_E_PARAM_BLOCK_ID when the passed BlockID is out of range
        if (NvM_Prv_ErrorDetection_IsBlockIdValid(NVM_SERVICE_ID_GET_DATA_INDEX, BlockId, FALSE))
        {
            // TRACE[NVM188] If the NvMDevErrorDetect switch is enabled API parameter checking is enabled
            // TRACE[NVM605] Report the DET error NVM_E_PARAM_DATA when a NULL pointer is passed
            //               via the parameter DataIndexPtr
            NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
            ptr_un.ptrDataIdx_pu8 = DataIndexPtr;
            if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_GET_DATA_INDEX, BlockId, NVM_E_PARAM_DATA, &ptr_un))
            {
                // Parameters are valid
                stReturn_uo = E_OK;
                if (NvM_Prv_ErrorDetection_IsBlockTypeDataset(NVM_SERVICE_ID_GET_DATA_INDEX, BlockId))
                {
                    // TRACE[NVM021] Get the current data index of the selected block
                    //               if its management type is NVM_BLOCK_DATASET
                    // No SchM lock can and should be done here to allow calling this function from NvM callbacks
                    // without deadlocks
                    *DataIndexPtr = NvM_Prv_Block_GetIdxDataset(BlockId);
                }
                else
                {
                    // TRACE[NVM265] For blocks with other management types, set the index pointed to
                    //               by DataIndexPtr to zero
                    *DataIndexPtr = 0;
                }
            }
        }
    }
    // Return whether this operation was successful
    return stReturn_uo;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

