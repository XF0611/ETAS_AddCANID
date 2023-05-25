
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM.h"
#include "NvM_Cfg_SchM.h"

#include "NvM_Prv.h"
#include "NvM_Prv_Crc.h"
#include "NvM_Prv_Job.h"
#include "NvM_Prv_JobQueue.h"
#include "NvM_Prv_RequestQueue.h"
#include "NvM_Prv_ErrorDetection.h"
#include "NvM_Prv_ProcessMultiBlock.h"

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_GetNextRequest(NvM_Prv_idQueue_tuo idReqQueue_uo,
                                         NvM_Prv_QueueEntry_tst* Request_pst,
                                         NvM_Prv_MainStates_tst* MainStates_pst);
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_GetNextJob(NvM_Prv_idQueue_tuo idReqQueue_uo,
                                                  NvM_Prv_QueueEntry_tst* QueueEntry_pst);

/*
 **********************************************************************************************************************
 * NvM-private code
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

/**
 * This function determines the next request to be executed by checking all queues for entries to be started.
 *
 * All prioritizations of requests is done here.
 * This function will be invoked by NvM_Mainfunction only if no other request is currently active.
 *
 * \param[global in,out] NvM_Prv_Main_st
 * Global structure with all state information which is not block-specific.
 *
 */
boolean NvM_Prv_MainFunctionArbitrate(NvM_Prv_MainStates_tst* MainStates_pst)
{
    if (!NvM_Prv_JobQueue_IsFull())
    {
        NvM_Prv_QueueEntry_tst* Request_pst = NULL_PTR;

        NvM_Prv_idQueue_tuo idReqQueue_uo = NvM_Prv_RequestQueue_GetEntryToProcess(&Request_pst);

        if (NULL_PTR != Request_pst)
        {
            MainStates_pst->Activity_rAMwM_en = NVM_PRV_ACTIVITY_BUSY;
            NvM_Prv_GetNextRequest(idReqQueue_uo, Request_pst, MainStates_pst);
        }
        else
        {
            if (NvM_Prv_JobQueue_IsEmpty())
            {
                // No request pending -> NvM is completely idle
                NvM_BlockIdType idBlock_uo;

                MainStates_pst->idServiceActive_uo = NVM_SERVICE_ID_INIT;
                MainStates_pst->idQueueActive_uo = NvM_Prv_idQueue_nrQueues_e;

                // check whether RAM block CRC recalculation is required
                if (NvM_Prv_Crc_IsRamBlockCrcRecalcRequired(&idBlock_uo))
                {
                    MainStates_pst->Activity_rAMwM_en = NVM_PRV_ACTIVITY_RAM_BLOCK_CRC;
                    NvM_Prv_Job_StartRecalcRamBlkCrc(idBlock_uo);
                }
                else
                {
                    MainStates_pst->Activity_rAMwM_en = NVM_PRV_ACTIVITY_IDLE;
                }
            }
        }
    }

    return (!NvM_Prv_JobQueue_IsEmpty());
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Inline code
 **********************************************************************************************************************
 */
LOCAL_INLINE void NvM_Prv_GetNextRequest(NvM_Prv_idQueue_tuo idReqQueue_uo,
                                         NvM_Prv_QueueEntry_tst* Request_pst,
                                         NvM_Prv_MainStates_tst* MainStates_pst)
{
    boolean isRequestToDefer_b = FALSE;

    // Request must be defered in following cases:
    // - an immediate request can interrupt a multi-block request only if all jobs for multi-block request
    //   in the job queue has been finished
    // - a multi-block request can start only if all jobs for single-block requests in the job queue has been
    //   finished
    // - a recalculation of the RAM block CRC is currently active
    boolean hasNextReqHigherPrio_b =    (idReqQueue_uo > MainStates_pst->idQueueActive_uo)
                                     && (NvM_Prv_idQueue_Multi_e == MainStates_pst->idQueueActive_uo);
    boolean isNextMultiReqPossible_b =    (NvM_Prv_idQueue_Multi_e == idReqQueue_uo)
                                       && (NvM_Prv_idQueue_Multi_e != MainStates_pst->idQueueActive_uo);

    isRequestToDefer_b = (   !NvM_Prv_JobQueue_IsEmpty()
                          && (   hasNextReqHigherPrio_b
                              || isNextMultiReqPossible_b
                              || (NVM_PRV_ACTIVITY_RAM_BLOCK_CRC == MainStates_pst->Activity_rAMwM_en)
                          )
                         );

    if (!isRequestToDefer_b)
    {
        NvM_Prv_idJob_ten idJob_en;

        if (NVM_SERVICE_ID_INIT == MainStates_pst->idServiceActive_uo)
        {
            MainStates_pst->idServiceActive_uo = Request_pst->idService_uo;
            MainStates_pst->idQueueActive_uo = idReqQueue_uo;
        }

        idJob_en = NvM_Prv_GetNextJob(idReqQueue_uo, Request_pst);

        if (NvM_Prv_idJob_Suspend_e != idJob_en)
        {
            if ((NvM_Prv_idJob_Idle_e != idJob_en) && (NvM_Prv_idJob_Invalid_e != idJob_en))
            {
                NvM_Prv_Job_Prepare(idJob_en, Request_pst, idReqQueue_uo);
            }
            else
            {
                MainStates_pst->idServiceActive_uo = NVM_SERVICE_ID_INIT;
                MainStates_pst->idQueueActive_uo = NvM_Prv_idQueue_nrQueues_e;
            }
        }

        if ((NvM_Prv_idQueue_Multi_e != idReqQueue_uo) || (NvM_Prv_idJob_Idle_e == idJob_en))
        {
            // Set scheduling lock
            SchM_Enter_NvM_Main();
            NvM_Prv_RequestQueue_Dequeue(idReqQueue_uo);
            // Reset scheduling lock
            SchM_Exit_NvM_Main();
        }
    }
}

/**
 * \brief
 * This function determines which job the user request requires.
 * \details
 * If user requests a multi-block service then this function processes multi-block service.
 * The processing of a multi-block service determines which job is required for which block.
 *
 * If user requests a single-block service then this function determines which job is required for the requested block.
 * Additionally this function checks whether the single-block service has requested a valid service by checking
 * the service bit.
 *
 */
LOCAL_INLINE NvM_Prv_idJob_ten NvM_Prv_GetNextJob(NvM_Prv_idQueue_tuo idReqQueue_uo,
                                                  NvM_Prv_QueueEntry_tst* QueueEntry_pst)
{
    NvM_Prv_idJob_ten idJobNext_en = NvM_Prv_idJob_Idle_e;

    if (NvM_Prv_idQueue_Multi_e == idReqQueue_uo)
    {
        // process multi block service
        idJobNext_en = NvM_Prv_Multi_Process(QueueEntry_pst);
    }
    else
    {
        idJobNext_en = NvM_Prv_idJob_Invalid_e;

        // Check whether the service identified by the service bit is valid
        if (NvM_Prv_ErrorDetection_IsServiceBitValid(NVM_SERVICE_ID_MAIN_FUNCTION,
                                                     QueueEntry_pst->idBlock_uo,
                                                     QueueEntry_pst->ServiceBit_uo))
        {
            // process single block service
            idJobNext_en = NvM_Prv_GetJobId(QueueEntry_pst->ServiceBit_uo);
        }
    }

    return idJobNext_en;
}

