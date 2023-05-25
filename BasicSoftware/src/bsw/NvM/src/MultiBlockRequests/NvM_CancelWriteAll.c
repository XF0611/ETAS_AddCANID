
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_RequestQueue.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_ProcessMultiBlock.h"

/*
 **********************************************************************************************************************
 * Declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

static boolean NvM_Prv_CancelWriteAll(void);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"


/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/

#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

#ifdef NVM_CFG_API_CANCEL_WRITE_ALL
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2917] Declaration of the overloading function shall be done here,
//                                               not in a header file
/* MR12 RULE 8.5 VIOLATION: Not using a header file for this declaration is a customer requirement */
extern void NVM_CFG_API_CANCEL_WRITE_ALL(void);

void NvM_CancelWriteAll(void)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2920] Call the overloading function
    NVM_CFG_API_CANCEL_WRITE_ALL();
}
#endif

/**
 * This function cancels a running NvM_WriteAll request.
 */
#ifdef NVM_CFG_API_CANCEL_WRITE_ALL
void NvM_Rb_CancelWriteAllBase(void)
#else
void NvM_CancelWriteAll(void)
#endif
{
    // TRACE[NVM458] Service to cancel a running NvM_WriteAll request
    // TRACE[NVM019] Terminates NvM_WriteAll in a way that data consistency is not compromised

    // TRACE[NVM637] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_CANCEL_WRITE_ALL, NVM_PRV_MULTI_BLOCK))
    {
        boolean isWriteAllEnqueued_b = FALSE;
        boolean isBlockMaintainanceEnqueued_b = FALSE;

        SchM_Enter_NvM_Main();

        // TRACE[NVM420] This cancellation request is not queued

        // TRACE[NVM233] Check whether a WriteAll operation is currently in progress, otherwise do nothing
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2868] NvM_CancelWriteAll cancels the maintenance phase as well.
        isBlockMaintainanceEnqueued_b = NvM_Prv_RequestQueue_IsMultiEnqueued(NVM_PRV_SERVICE_ID_MAINTAIN_BLOCK);
        isWriteAllEnqueued_b = (NvM_Prv_RequestQueue_IsMultiEnqueued(NVM_SERVICE_ID_WRITE_ALL) ||
                                isBlockMaintainanceEnqueued_b);

        if (isWriteAllEnqueued_b)
        {
            NvM_Prv_Multi_Cancel(NvM_Prv_CancelWriteAll);
        }

        SchM_Exit_NvM_Main();

        // TRACE[NVM232] Immediately return, don't wait for the successful completion of the cancel operation
    }
}

static boolean NvM_Prv_CancelWriteAll(void)
{
    // TRACE[NVM237]
    // In case CancelWriteAll was invoked while WriteAll had been pending,
    // the request result of block #0 is set to CANCELED
    // TRACE[NVM235]
    // The request result of CancelWriteAll is implicitly given
    // by the request result of the canceled WriteAll
    boolean isWriteAllCanceled_b = FALSE;
    NvM_BlockIdType idBlock_uo;

    SchM_Enter_NvM_Main();

    // WriteAll can be canceled only if user blocks has to be cancelled
    for (idBlock_uo = NVM_CFG_FIRST_USED_BLOCK; idBlock_uo < NVM_CFG_NR_BLOCKS; ++idBlock_uo)
    {
        // TRACE[NVM019] Only delete the write all request of blocks for which no MemIf operation is currently ongoing
        if (NvM_Prv_Block_IsRequestPending(idBlock_uo, (1u << NvM_Prv_ServiceBit_WriteAll_e)))
        {
            // Bit mask for any single-block request.
           uint16 maskAnySingle_u16 = (uint16)((1uL << NvM_Prv_ServiceBit_Read_e)       |
                                               (1uL << NvM_Prv_ServiceBit_Write_e)      |
                                               (1uL << NvM_Prv_ServiceBit_Invalidate_e) |
                                               (1uL << NvM_Prv_ServiceBit_Erase_e)      |
                                               (1uL << NvM_Prv_ServiceBit_Restore_e));

            // Signal that WriteAll is cancelled.
            // Note: WriteAll is cancelled if the writing of at least one block was cancelled,
            //       except the internal blocks like NvM_WriteMon
            boolean isBlockInternal_b = FALSE;
#if (NVM_RB_REQ_MONITOR == NVM_RB_REQ_MON_WRITE)
            if (NvMConf_NvMBlockDescriptor_NvM_WriteMon == idBlock_uo)
            {
                isBlockInternal_b = TRUE;
            }
#endif
            if (!isBlockInternal_b)
            {
                isWriteAllCanceled_b = TRUE;
            }

           // TRACE[NVM238] Signal write cancelation by resetting the write request bits
           //               (-> non-preemptive cancelation)
           NvM_Prv_Block_ClearRequests(idBlock_uo, (1u << NvM_Prv_ServiceBit_WriteAll_e));


           // TRACE[NVM238] The currently active job shall not be canceled
           // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2880] If a block is currently PENDING due
           //                                               to a queued/running single block operation,
           //                                               the request result shouldn't be set to CANCELED
           // Note: If a single block request is pending, then the NVM_REQ_PENDING state must originate
           //       from this single block request because:
           //       - Single block requests are not accepted by NvM for blocks which are already busy
           //         with WriteAll (NVM385), so this single block request must have been issued before WriteAll
           //       - Enqueueing single block requests always sets NVM_REQ_PENDING (NVM185) and
           //         the single block request is not cleared, so it must be the origin for the NVM_REQ_PENDING state
           if (!NvM_Prv_Block_IsRequestPending(idBlock_uo, maskAnySingle_u16))
           {
               // TRACE[NVM470] Set job result to NVM_REQ_CANCELED if successfully canceled
               NvM_Prv_Block_SetRequestResult(idBlock_uo, NVM_REQ_CANCELED);
           }
        }

#if (NVM_PRV_MAINTAIN_REDUNDANT_BLOCKS)
        if (NvM_Prv_Block_IsRequestPending(idBlock_uo, (1u << NvM_Prv_ServiceBit_Maintain_e)))
        {
            // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2868] Signal write cancelation by resetting the maintenance
            //                                               request bits (-> non-preemptive cancelation)
            NvM_Prv_Block_ClearRequests(idBlock_uo, (1u << NvM_Prv_ServiceBit_Maintain_e));
        }
#endif
    }

    SchM_Exit_NvM_Main();

    return (isWriteAllCanceled_b);
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

