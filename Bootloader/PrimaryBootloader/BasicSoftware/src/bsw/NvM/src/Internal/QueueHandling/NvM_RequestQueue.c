
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv_Queue.h"
#include "NvM_Prv_RequestQueue.h"
#include "NvM_Prv_ProcessMultiBlock.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/**
 * \brief
 * Definition of a union to get different types of pointer to user request data without casting.
 * \details
 * The NvM uses such unions to avoid MISRA warnings.
 */
typedef union
{
    /// Byte pointer to user request data.
    uint8*                          RequestData_pu8;
    /// Byte pointer to constant user request data.
    uint8 const*                    RequestData_pcu8;
    /// Pointer to user request data using original data type.
    NvM_Prv_QueueEntry_tst*         RequestData_pst;
    /// Pointer to constant user request data using original data type.
    NvM_Prv_QueueEntry_tst const*   RequestData_pcst;

} NvM_Prv_Request_Data_tun;

/**
 * \brief
 * Definition of the structure which defines a queue for user requests.
 * \details
 * This definition extends the circular buffer queue provided by the unit NvM_Queue (see NvM_Prv_Queue_tst)
 * by adding following elements:
 * - array to store enqueued user requests
 * - pointer to a special variable to monitor minimum number of free entries in the queue if required
 */
typedef struct
{
    /// Circular buffer queue.
    NvM_Prv_Queue_tst               Queue_st;
    /// Constant pointer to the array to store user request data.
    NvM_Prv_QueueEntry_tst* const   data_cpo;
    /// Constant pointer to the component global variable which holds the minimum number of free entries in the queue.
    uint8* const                    minNrFreeEntries_cpu8;

} NvM_Prv_Request_Queue_tst;

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */
/**
 * \brief
 * Size definition of the queue for multi-block requests.
 * \details
 * Queue for multi-block requests has always only one entry.
 */
#define NVM_PRV_SIZE_MULTI_QUEUE    (1u)

/**
 * \brief
 * Size definition of the queue for auxiliary requests.
 * \details
 * If enabled by the parameter NvMRbAuxInterface the queue for auxiliary requests has only one entry,
 * otherwise the queue for auxiliary requests is not available -> size = 0.
 */
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2933] Queue for auxiliary requests has only one single entry
#if (NVM_RB_AUX_INTERFACE == STD_ON)
# define NVM_PRV_SIZE_AUX_QUEUE     (1u)
#else
# define NVM_PRV_SIZE_AUX_QUEUE     (0u)
#endif

/**
 * \brief
 * Size definition of the queue for single-block requests with immediate priority.
 * \details
 * If enabled by the parameter NvMJobPrioritization the queue size is defined by the parameter NvMSizeImmediateJobQueue,
 * otherwise the queue is not available -> size = 0.
 * \note
 * If the immediate queue is disabled then NVM_SIZE_IMMEDIATE_JOB_QUEUE = 1 due to the implementation
 * in corrresponding BCT-action. For this reason queue management is using own constant to get queue size 0.
 */
#if (NVM_JOB_PRIORITIZATION == STD_ON)
# define NVM_PRV_SIZE_IMMEDIATE_QUEUE   (NVM_SIZE_IMMEDIATE_JOB_QUEUE)
#else
# define NVM_PRV_SIZE_IMMEDIATE_QUEUE   (0u)
#endif

/**
 * \brief
 * Size definition of the queue for all requests provided by the NvM.
 */
#define NVM_PRV_SIZE_OVERALL_QUEUE (NVM_PRV_SIZE_MULTI_QUEUE    +   \
                                    NVM_PRV_SIZE_AUX_QUEUE      +   \
                                    NVM_SIZE_STANDARD_JOB_QUEUE +   \
                                    NVM_PRV_SIZE_IMMEDIATE_QUEUE)

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
// Queue monitoring
#define NVM_START_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"
/// Minimum number of free entries in the standard queue reached so far during the runtime of the ECU.
uint8 NvM_Rb_minNrFreeStdQueueEntries_u8;

#if (NVM_JOB_PRIORITIZATION == STD_ON)
/// Minimum number of free entries in the immediate queue reached so far during the runtime of the ECU.
uint8 NvM_Rb_minNrFreeImmQueueEntries_u8;
#endif

#define NVM_STOP_SEC_VAR_SAVED_ZONE_8
#include "NvM_MemMap.h"


#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/// Definition of the array for all user requests.
static NvM_Prv_QueueEntry_tst NvM_Prv_RequestQueueEntries_ast[NVM_PRV_SIZE_OVERALL_QUEUE];

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

/// Definition of the array for all queues for user requests provided by the NvM.
static NvM_Prv_Request_Queue_tst NvM_Prv_RequestQueues_ast[NvM_Prv_idQueue_nrQueues_e] =
{
    // queue for multi-block requests
    {
        {
            0,                                                          // idxHead_u16
            NVM_PRV_SIZE_MULTI_QUEUE,                                   // cntrFreeEntries_u16
            NVM_PRV_SIZE_MULTI_QUEUE                                    // size_cu16
        },
        &NvM_Prv_RequestQueueEntries_ast[0],                            // data_cpo
        NULL_PTR                                                        // minNrFreeEntries_cpu8
    },

#if (NVM_RB_AUX_INTERFACE == STD_ON)
    // queue for auxiliary requests
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2932] Own queue for auxiliary jobs
    {
        {
            0,                                                          // idxHead_u16
            NVM_PRV_SIZE_AUX_QUEUE,                                     // cntrFreeEntries_u16
            NVM_PRV_SIZE_AUX_QUEUE                                      // size_cu16
        },
        &NvM_Prv_RequestQueueEntries_ast[NVM_PRV_SIZE_MULTI_QUEUE],     // data_cpo
        NULL_PTR,                                                       // minNrFreeEntries_cpu8
    },
#endif

    // queue for single-block requests with standard priority
    {
        {
            0,                                                          // idxHead_u16
            NVM_SIZE_STANDARD_JOB_QUEUE,                                // cntrFreeEntries_u16
            NVM_SIZE_STANDARD_JOB_QUEUE                                 // size_cu16
        },
        &NvM_Prv_RequestQueueEntries_ast[NVM_PRV_SIZE_MULTI_QUEUE +
                                          NVM_PRV_SIZE_AUX_QUEUE],      // data_cpo
        &NvM_Rb_minNrFreeStdQueueEntries_u8,                            // minNrFreeEntries_cpu8
    }

// TRACE[NVM564] NvMJobPrioritization enables/disables immediate job processing
#if (NVM_JOB_PRIORITIZATION == STD_ON)
    ,
    // queue for single-block requests with immediate priority
    {
        {
            0,                                                          // idxHead_u16
            NVM_PRV_SIZE_IMMEDIATE_QUEUE,                               // cntrFreeEntries_u16
            NVM_PRV_SIZE_IMMEDIATE_QUEUE,                               // size_cu16
        },
        &NvM_Prv_RequestQueueEntries_ast[NVM_PRV_SIZE_MULTI_QUEUE +
                                          NVM_PRV_SIZE_AUX_QUEUE   +
                                          NVM_SIZE_STANDARD_JOB_QUEUE], // data_cpo
        &NvM_Rb_minNrFreeImmQueueEntries_u8,                            // minNrFreeEntries_cpu8
    }
#endif
};

#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

/*
**********************************************************************************************************************
* Code
**********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * \brief
 * This function initializes all queues for user requests used by the NvM.
 * \details
 * Queue data stored in the saved zone will be initialized only if is lost.
 *
 * \param[in] isSavedZoneDataLost_b
 * Status information of the data in the saved zone RAM:
 * - true = data in the saved zone RAM is lost after the reset
 * - false = data in the saved zone RAM is unchanged after the reset
 */
void NvM_Prv_RequestQueue_Initialize(boolean isSavedZoneDataLost_b)
{
    NvM_Prv_idQueue_tuo idQueue_uo;

    for (idQueue_uo = 0; idQueue_uo < NvM_Prv_idQueue_nrQueues_e; ++idQueue_uo)
    {
        NvM_Prv_Queue_Initialize(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st);

        if (isSavedZoneDataLost_b && (NULL_PTR != NvM_Prv_RequestQueues_ast[idQueue_uo].minNrFreeEntries_cpu8))
        {
            *(NvM_Prv_RequestQueues_ast[idQueue_uo].minNrFreeEntries_cpu8) =
                    (uint8)NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st.size_cu16;
        }
    }
}

/**
 * \brief
 * This function enqueues a user request into the given queue.
 *
 * \param[in] idQueue_uo
 * Id of the queue to enqueue the passed user request.
 * \param[in] QueueEntry_pcst
 * Pointer to the constant user request to be enqueued into the passed queue.
 *
 * \return
 * - true = passed user request enqueued successfully
 * - false = enqueuing of the passed user request has failed
 */
boolean NvM_Prv_RequestQueue_Enqueue(NvM_Prv_idQueue_tuo idQueue_uo,
                                     NvM_Prv_QueueEntry_tst const* QueueEntry_pcst)
{
    boolean isRequestEnqueued_b = FALSE;

    NvM_Prv_Request_Data_tun QueueBuffer_un;
    NvM_Prv_Request_Data_tun RequestData_un;

    QueueBuffer_un.RequestData_pcst = NvM_Prv_RequestQueues_ast[idQueue_uo].data_cpo;
    RequestData_un.RequestData_pcst = QueueEntry_pcst;

    // Queue is full -> enqueueing another entry is not possible
    if (!NvM_Prv_Queue_IsFull(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st))
    {
        uint8* minNrFreeEntries_pu8 = NvM_Prv_RequestQueues_ast[idQueue_uo].minNrFreeEntries_cpu8;

        (void)NvM_Prv_Queue_Enqueue(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st,
                                    QueueBuffer_un.RequestData_pu8,
                                    RequestData_un.RequestData_pcu8,
                                    sizeof(NvM_Prv_QueueEntry_tst));

        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2808]
        // Update the minimum number of standard queue free entries
        // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-2809]
        // Update the minimum number of immediate queue free entries
        if ((minNrFreeEntries_pu8 != NULL_PTR) &&
            (NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st.cntrFreeEntries_u16 < *minNrFreeEntries_pu8))
        {
            *minNrFreeEntries_pu8 = (uint8)NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st.cntrFreeEntries_u16;
        }

        isRequestEnqueued_b = TRUE;
    }

    return isRequestEnqueued_b;
}

/**
 * \brief
 * This function dequeues a user request from the given queue.
 *
 * \param[in] idQueue_uo
 * Id of the queue to dequeue passed user request.
 */
void NvM_Prv_RequestQueue_Dequeue(NvM_Prv_idQueue_tuo idQueue_uo)
{
    NvM_Prv_Queue_Dequeue(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st);
}

/**
 * \brief
 * This function determines which request will be executed as next by the NvM.
 * \details
 * Normally this function goes through all available queues due to the their priority in descending order.
 * And if one queue has an entry to be executed then the pointer to this entry will be returned.
 *
 * If multi-block request is active then this function checks only if immediate queue has an entry.
 * And if immediate queue has no entries or is not available then this function returns the pointer
 * to the multi-block queue.
 *
 * \param[out] QueueEntry_ppst
 * Pointer to the pointer to the queue entry to return the parameter of the next executed request.
 *
 * \return
 * ID of the queue from which the next request to be processed has been taken.
 */
NvM_Prv_idQueue_tuo NvM_Prv_RequestQueue_GetEntryToProcess(NvM_Prv_QueueEntry_tst** QueueEntry_ppst)
{
    NvM_Prv_idQueue_tuo idQueue_uo = NvM_Prv_idQueue_nrQueues_e;

    *QueueEntry_ppst = NULL_PTR;
    while ((NULL_PTR == *QueueEntry_ppst) && (idQueue_uo > 0u))
    {
        --idQueue_uo;

        if (NvM_Prv_Multi_IsInProgress() && (NvM_Prv_idQueue_Standard_e == idQueue_uo))
        {
            idQueue_uo = NvM_Prv_idQueue_Multi_e;
        }

        if (!NvM_Prv_Queue_IsEmpty(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st))
        {
            NvM_Prv_QueueEntry_tst* data_cpo = NvM_Prv_RequestQueues_ast[idQueue_uo].data_cpo;
            *QueueEntry_ppst = &data_cpo[NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st.idxHead_u16];
        }
    }
    if (NULL_PTR == *QueueEntry_ppst)
    {
        idQueue_uo = NvM_Prv_idQueue_nrQueues_e;
    }

    return idQueue_uo;
}

/**
 * \brief
 * This private function check whether a given multi-block request is currently enqueued.
 *
 * \param[in] idService_uo
 * ID of the multi-block request to checked
 *
 * \return
 * - true = given multi-block request is currently enqueued
 * - false = given multi-block request is currently NOT enqueued
 */
boolean NvM_Prv_RequestQueue_IsMultiEnqueued(NvM_Prv_idService_tuo idService_uo)
{
    boolean IsEnqueuedMulti_b = FALSE;
    if (!NvM_Prv_Queue_IsEmpty(&NvM_Prv_RequestQueues_ast[NvM_Prv_idQueue_Multi_e].Queue_st))
    {
        uint16 idxHead_u16 = NvM_Prv_RequestQueues_ast[NvM_Prv_idQueue_Multi_e].Queue_st.idxHead_u16;
        NvM_Prv_idService_tuo idServiceEnqueued_uo =
                NvM_Prv_RequestQueues_ast[NvM_Prv_idQueue_Multi_e].data_cpo[idxHead_u16].idService_uo;
        if (idService_uo == idServiceEnqueued_uo)
        {
            IsEnqueuedMulti_b = TRUE;
        }
    }
    return IsEnqueuedMulti_b;
}

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

boolean NvM_Prv_RequestQueue_IsEmpty(NvM_Prv_idQueue_tuo idQueue_uo)
{
    return NvM_Prv_Queue_IsEmpty(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st);
}

uint16 NvM_Prv_RequestQueue_GetFreeEntries(NvM_Prv_idQueue_tuo idQueue_uo)
{
    return NvM_Prv_Queue_GetFreeEntries(&NvM_Prv_RequestQueues_ast[idQueue_uo].Queue_st);
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

