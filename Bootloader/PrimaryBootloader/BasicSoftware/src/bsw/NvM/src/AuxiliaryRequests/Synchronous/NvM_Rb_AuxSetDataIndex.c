
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

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

Std_ReturnType NvM_Rb_AuxSetDataIndex(uint8 DataIndex)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2941] Service used from an auxiliary user for setting the DataIndex
    //                                               of a dataset NVRAM block
    Std_ReturnType stReturn_uo = E_NOT_OK;

    // TRACE[NVM027] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_RB_AUX_SET_DATA_INDEX, 0))
    {
        // Variables used to report DET errors
        NvM_Prv_BlockErrors_tuo errors_uo = 0;

        SchM_Enter_NvM_Main();

        // TRACE[NVM704] Prohibit administrative data block modifications
        //               while a single-block operation is pending or in progress
        if (!NvM_Prv_Block_IsPending(NVM_PRV_AUX_ADMIN_BLOCK, &errors_uo))
        {
            NvM_Prv_Block_SetIdxData(NVM_PRV_AUX_ADMIN_BLOCK, DataIndex);
            stReturn_uo = E_OK;
        }

        SchM_Exit_NvM_Main();

        // Calling foreign DET function is done out of the scheduler lock
        NvM_Prv_ErrorDetection_ReportServiceInitiationErrors(NVM_SERVICE_ID_RB_AUX_SET_DATA_INDEX,
                                                             0u,
                                                             errors_uo);
    }

    // Return whether this operation was successful
    return stReturn_uo;
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif
