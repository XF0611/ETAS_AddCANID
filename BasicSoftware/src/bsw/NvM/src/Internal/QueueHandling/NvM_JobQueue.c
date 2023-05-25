
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Queue.h"
#include "NvM_Prv_JobQueue.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_InternalBuffer.h"

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
# include "TestCd_NvM.h"
#endif

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/**
 * \brief
 * Definition of a union to get different types of pointer to job structure without casting.
 * \details
 * The NvM uses such unions to avoid MISRA warnings.
 */
typedef union
{
    /// Byte pointer to job structure.
    uint8*                  Job_pu8;
    /// Byte pointer to constant job structure.
    uint8 const*            Job_pcu8;
    /// Pointer to job structure using original data type.
    NvM_Prv_Job_tst*        Job_pst;
    /// Pointer to constant job structure using original data type.
    NvM_Prv_Job_tst const*  Job_pcst;

} NvM_Prv_Job_Data_tun;

/**
 * \brief
 * Definition of the structure which defines a queue for all pending internal jobs.
 * \details
 * This definition extends the circular buffer queue provided by the unit NvM_Queue (see NvM_Prv_Queue_tst)
 * by adding following elements:
 * - array to store enqueued jobs
 *
 * This structure allows parallel job execution.
 * The queue size defines the number of jobs which can be executed in parallel by the NvM.
 */
typedef struct
{
    /// Circular buffer queue.
    NvM_Prv_Queue_tst       Queue_st;
    /// Constant pointer to the array to store job data.
    NvM_Prv_Job_tst* const  data_cpo;

} NvM_Prv_Job_Queue_tst;

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE uint16 NvM_Prv_JobQueue_GetOffsetPlainData(NvM_Prv_JobData_tst const* JobData_pcst);
LOCAL_INLINE uint8* NvM_Prv_JobQueue_GetJobBuffer(NvM_Prv_JobData_tst const* JobData_pcst, uint16 idxEnqueued_u16);
LOCAL_INLINE void NvM_Prv_JobQueue_UpdateIntBufferData(uint16 idxEnqueued_u16, NvM_Prv_JobData_tst* JobData_pst);

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

/// Definition of the array for all internal jobs processed in parallel.
static NvM_Prv_Job_tst NvM_Prv_Jobs_ast[NVM_PRV_NR_PARALLEL_JOB];

#define NVM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "NvM_MemMap.h"

#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include "NvM_MemMap.h"

/// Definition of the queue for internal jobs processed in parallel.
static NvM_Prv_Job_Queue_tst NvM_Prv_Job_Queue_st =
{
    // queue for internal jobs
    {
        0,                          // idxHead_u16
        NVM_PRV_NR_PARALLEL_JOB,    // cntrFreeEntries_u16
        NVM_PRV_NR_PARALLEL_JOB     // size_cu16
    },
    NvM_Prv_Jobs_ast                // data_cpo
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
 * This private function initializes the queue for internal jobs.
 */
void NvM_Prv_JobQueue_Initialize(void)
{
    NvM_Prv_Queue_Initialize(&NvM_Prv_Job_Queue_st.Queue_st);
}

/**
 * \brief
 * This private function enqueues an internal job.
 *
 * \param[in] JobData_pcst
 * Pointer to the constant job data to be enqueued
 */
void NvM_Prv_JobQueue_Enqueue(NvM_Prv_Job_tst const* Job_pcst)
{
    uint16 idxEnqueued_u16;
    NvM_Prv_Job_Data_tun JobData_un;
    NvM_Prv_Job_Data_tun QueueBuffer_un;

    QueueBuffer_un.Job_pst = NvM_Prv_Jobs_ast;
    JobData_un.Job_pcst = Job_pcst;

    idxEnqueued_u16 = NvM_Prv_Queue_Enqueue(&NvM_Prv_Job_Queue_st.Queue_st,
                                            QueueBuffer_un.Job_pu8,
                                            JobData_un.Job_pcu8,
                                            sizeof(NvM_Prv_Job_tst));

    NvM_Prv_JobQueue_UpdateIntBufferData(idxEnqueued_u16, &NvM_Prv_Jobs_ast[idxEnqueued_u16].JobData_st);

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))
    TestCd_NvM_JobMonitor_Start(&NvM_Prv_Jobs_ast[idxEnqueued_u16].JobData_st, idxEnqueued_u16);
#endif
}

/**
 * \brief
 * This private function dequeues an internal job.
 */
void NvM_Prv_JobQueue_Dequeue(void)
{
    NvM_Prv_Queue_Dequeue(&NvM_Prv_Job_Queue_st.Queue_st);
}

/**
 * \brief
 * This private function returns whether the job queue is full.
 *
 * \return
 * - true = job queue is full
 * - false = job queue is not full, i.e. at least one element can be enqueued
 */
boolean NvM_Prv_JobQueue_IsFull(void)
{
    return NvM_Prv_Queue_IsFull(&NvM_Prv_Job_Queue_st.Queue_st);
}

/**
 * \brief
 * This private function returns whether the job queue is empty.
 *
 * \return
 * - true = job queue is empty
 * - false = job queue is not empty, i.e. at least one element is currently enqueued
 */
boolean NvM_Prv_JobQueue_IsEmpty(void)
{
    return NvM_Prv_Queue_IsEmpty(&NvM_Prv_Job_Queue_st.Queue_st);
}

/**
 * \brief
 * This private function returns the pointer to the entry in the job queue identified by the passed index.
 * \details
 * For index 0 this function returns the first (oldest) entry in the job queue.
 * If passed index exceeds number of entries in the job queue then this functions returns a null pointer.
 * Via this function the NvM can iterate over all jobs in the queue.
 *
 * \param[in] idxJob_u16
 * Index of the job in the queue to be returned
 *
 * \return
 * Pointer to the job in the queue identified by the passed index
 */
NvM_Prv_Job_tst* NvM_Prv_JobQueue_GetEntry(uint16 idxJob_u16)
{
    NvM_Prv_Job_tst* Job_pst = NULL_PTR;
    uint16 nrEntries_u16 = NvM_Prv_Queue_GetEntries(&NvM_Prv_Job_Queue_st.Queue_st);

    if (idxJob_u16 < nrEntries_u16)
    {
        uint16 idx_16;
        idx_16 = (NvM_Prv_Job_Queue_st.Queue_st.idxHead_u16 + idxJob_u16) % NvM_Prv_Job_Queue_st.Queue_st.size_cu16;

        Job_pst = &(NvM_Prv_Job_Queue_st.data_cpo[idx_16]);
    }

    return Job_pst;
}

#if (defined(TESTCD_NVM_ENABLED) && (TESTCD_NVM_ENABLED == STD_ON))

uint8* NvM_Prv_JobQueue_GetUsedIntBfr(NvM_BlockIdType idBlock_uo, NvM_Prv_idService_tuo idService_uo)
{
    NvM_Prv_Job_tst const* Job_pcst = NULL_PTR;
    uint16 idxJob_u16 = 0u;
    uint8* IntBfr_pu8 = NULL_PTR;

    do
    {
        // search in all available job entries for job with required block and service ID
        Job_pcst = &NvM_Prv_Jobs_ast[idxJob_u16];
        if ((Job_pcst->JobData_st.idBlock_uo != idBlock_uo) || (Job_pcst->JobData_st.idService_uo != idService_uo))
        {
            ++idxJob_u16;
        }
        else
        {
            IntBfr_pu8 = Job_pcst->JobData_st.IntBuffer_st.Buffer_pu8;
        }

    } while ((NULL_PTR == IntBfr_pu8) && (idxJob_u16 < NVM_PRV_NR_PARALLEL_JOB));

    return IntBfr_pu8;
}

NvM_Prv_JobData_tst* NvM_Prv_JobQueue_GetJobData(NvM_BlockIdType idBlock_uo, NvM_Prv_idService_tuo idService_uo)
{
    NvM_Prv_JobData_tst* JobData_pst = NULL_PTR;
    uint16 idxJob_u16 = 0u;

    do
    {
        // search in all available job entries for job with required block and service ID
        if (   (NvM_Prv_Jobs_ast[idxJob_u16].JobData_st.idBlock_uo != idBlock_uo)
            || (NvM_Prv_Jobs_ast[idxJob_u16].JobData_st.idService_uo != idService_uo))
        {
            ++idxJob_u16;
        }
        else
        {
            JobData_pst = &NvM_Prv_Jobs_ast[idxJob_u16].JobData_st;
        }

    } while ((NULL_PTR == JobData_pst) && (idxJob_u16 < NVM_PRV_NR_PARALLEL_JOB));

    return JobData_pst;
}

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */

LOCAL_INLINE uint16 NvM_Prv_JobQueue_GetOffsetPlainData(NvM_Prv_JobData_tst const* JobData_pcst)
{
    uint16 OffsetPlainData_u16 = 0u;

    if (JobData_pcst->isEncryptionEnabled_b)
    {
        OffsetPlainData_u16 = NvM_Prv_BlkDesc_GetBlockSizeStored(JobData_pcst->idBlock_uo);
    }

    return OffsetPlainData_u16;
}

LOCAL_INLINE uint8* NvM_Prv_JobQueue_GetJobBuffer(NvM_Prv_JobData_tst const* JobData_pcst, uint16 idxEnqueued_u16)
{
    uint8* JobBuffer_pu8;

    if (JobData_pcst->isIntBufferToUse_b)
    {
        uint8* IntBuffer_pu8 = NvM_Prv_InternalBuffer_GetPtr();
        uint32 IntBufferSize_u32 = NvM_Prv_InternalBuffer_GetSize();

        JobBuffer_pu8 = &IntBuffer_pu8[IntBufferSize_u32 * idxEnqueued_u16];
    }
    else
    {
        JobBuffer_pu8 = JobData_pcst->UserData_un.ptrRamBlock_pu8;
    }

    return JobBuffer_pu8;
}

LOCAL_INLINE void NvM_Prv_JobQueue_UpdateIntBufferData(uint16 idxEnqueued_u16, NvM_Prv_JobData_tst* JobData_pst)
{
    uint16* UsedSizeInBytes_pu16 = NvM_Prv_InternalBuffer_GetPtrToUsedSize();

    JobData_pst->IntBuffer_st.UsedSizeInBytes_pu16 = &UsedSizeInBytes_pu16[idxEnqueued_u16];
    JobData_pst->IntBuffer_st.OffsetPlainData_u16  = NvM_Prv_JobQueue_GetOffsetPlainData(JobData_pst);
    JobData_pst->IntBuffer_st.Buffer_pu8           = NvM_Prv_JobQueue_GetJobBuffer(JobData_pst, idxEnqueued_u16);

}

