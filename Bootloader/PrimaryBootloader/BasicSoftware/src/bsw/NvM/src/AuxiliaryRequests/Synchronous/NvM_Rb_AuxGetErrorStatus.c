
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#if (NVM_RB_AUX_INTERFACE == STD_ON)

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_Rb_AuxGetErrorStatus(NvM_RequestResultType *RequestResultPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2939] Service used from an auxiliary user to read the block dependent
    //                                               error/status information

    Std_ReturnType stReturn_uo = E_NOT_OK;

    // TRACE[NVM610] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_RB_AUX_GET_ERROR_STATUS, 0))
    {
        // TRACE[NVM612] Report the DET error NVM_E_PARAM_DATA when a NULL pointer is passed
        //               via the parameter RequestResultPtr
        NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
        ptr_un.ptrRequestResult_puo = RequestResultPtr;
        if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_RB_AUX_GET_ERROR_STATUS, 0, NVM_E_PARAM_DATA, &ptr_un))
        {
            // Parameters are valid
            // TRACE[NVM015] Read out the selected block's state from the administrative block
            // No SchM lock can and should be done here to allow calling this function from NvM callbacks without deadlocks
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2929] Use special auxiliary admin block for the request result update
            *RequestResultPtr = NvM_Prv_Block_GetRequestResult(NVM_PRV_AUX_ADMIN_BLOCK);
            stReturn_uo = E_OK;
        }
    }

    // Return whether this operation was successful
    return stReturn_uo;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif
