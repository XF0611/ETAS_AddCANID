

/*
 * The order unit is responsible for handling all external calls to the Fee besides the main and init function.
 * It stores the received orders in internal order slots.
 * The main function will poll orders from the order unit and inform the order unit if an order is finished.
 */

#ifndef FEE_PRV_ORDER_H
#define FEE_PRV_ORDER_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if (defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv_Config.h"

extern Std_ReturnType Fee_Prv_OrderDetCheckDeviceName      (Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8);
/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
*/

LOCAL_INLINE MemIf_JobResultType Fee_Prv_OrderGetJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_ConfigRequester_ten requester_en);

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/

/**
 * \brief   Return the result of the last job for the given requester
 *
 * \param   requester_en                Job requester
 *
 * \retval  MEMIF_JOB_OK                Last job executed successfully
 * \retval  MEMIF_JOB_FAILED            Last job failed unexpectedly
 * \retval  MEMIF_JOB_PENDING           Last job is still running
 * \retval  MEMIF_BLOCK_INCONSISTENT    Last job was a read and
 *                                      a) Not a single instance of all instances has consistent data
 *                                      b) The block is not present at all
 * \retval  MEMIF_BLOCK_INVALIDATED     Last job was a read and the block was invalidated intentionally
 *
 * \attention   Since the job result can change asynchronously, a time consuming spin lock is needed in the called
 *              Get<...>JobResult sub-functions! Please consider this when creating do/while wait loops for the Fee.
 */
LOCAL_INLINE MemIf_JobResultType Fee_Prv_OrderGetJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_Prv_ConfigRequester_ten requester_en)
{
    MemIf_JobResultType    result_en;

    /* Query job result for the given requester.
     * Note: For a better readability, the coding rule CCode_BlockStyle_001 is not followed here on purpose. */
    switch(requester_en)
    {
        case FEE_PRV_REQUESTER_NVM_E:        result_en = Fee_Rb_Idx_GetJobResult(FeeDeviceName_en);           break;
        case FEE_PRV_REQUESTER_ADAPTER_E:    result_en = Fee_Rb_Idx_GetAdapterJobResult(FeeDeviceName_en);    break;
        default:                             result_en = MEMIF_JOB_FAILED;                break;
    }

    return(result_en);
}


# endif

/* FEE_PRV_ORDER_H */
#endif
