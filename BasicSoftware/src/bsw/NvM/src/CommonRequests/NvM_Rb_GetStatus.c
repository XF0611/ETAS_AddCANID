
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_ErrorDetection.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_Rb_GetStatus(NvM_Rb_StatusType *StatusPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2750] Service to read the overall status of the NVRAM manager
    Std_ReturnType stReturn_uo = E_NOT_OK;

    // Check provided parameter
    NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
    ptr_un.ptrStatusType_pen = StatusPtr;
    if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_RB_GET_STATUS, 0, NVM_E_PARAM_DATA, &ptr_un))
    {
        // Parameter is valid
        // -> translate internal activity status into public NvM status
        // No SchM lock can and should be done here to allow calling this function from NvM callbacks without deadlocks
        if (!NvM_Prv_IsNvmInitialized())
        {
            *StatusPtr = NVM_RB_STATUS_UNINIT;
        }
        else if (NvM_Prv_IsNvmIdle())
        {
            *StatusPtr = NVM_RB_STATUS_IDLE;
        }
        else
        {
            *StatusPtr = NVM_RB_STATUS_BUSY;
        }

        stReturn_uo = E_OK;
    }
    // Return whether this operation was successful
    return stReturn_uo;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

