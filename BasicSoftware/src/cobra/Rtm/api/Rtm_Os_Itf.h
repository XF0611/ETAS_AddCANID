/*
 ***********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 ***********************************************************************************************************************
 ************************************************************************************************
 * Component : Rtm_Os_Itf.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_OS_ITF_H
#define RTM_OS_ITF_H

/*! \file
 *
 * \brief Runtime measurement interface to RTA-OS
 *
 * This file contains the interface to RTA-OS. The OS interface is centralized here to enhance portability.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "Os.h"

/*
 ***********************************************************************************************************************
 * Configuration check
 ***********************************************************************************************************************
 */

#ifndef OS_ACTIVATION_MONITORING
#error "Rtm needs OS activation monitoring to be enabled in RTA-OS"
#endif

#ifndef OS_TIME_MONITORING
#error "Rtm needs activated timing monitoring to be enabled in RTA-OS"
#endif

#ifndef OS_ELAPSED_TIME_RECORDING
#error "Rtm requires elapsed time recording to be enabled in RTA-OS"
#endif

#if OS_NUM_TASKS > 255
# error "More than 255 tasks defined, configuration not tested"
#endif

#if OS_NUM_ISRS > 255
# error "More than 255 isrs defined, configuration not tested"
#endif


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

#define RTM_NUM_TASKS OS_NUM_TASKS
#define RTM_NUM_ISRS OS_NUM_ISRS

/*
 ***********************************************************************************************************************
 * Typedefs
 ***********************************************************************************************************************
 */

typedef TaskType Rtm_TaskType;
typedef ISRType Rtm_IsrType;

/*
 ***********************************************************************************************************************
 * Prototypes
 ***********************************************************************************************************************
 */

LOCAL_INLINE uint32 Rtm_GetStopwatch (void);
LOCAL_INLINE uint32 Rtm_GetIdleTime (uint32 coreId);
LOCAL_INLINE Rtm_TaskType Rtm_GetTaskID (void);
LOCAL_INLINE uint32 Rtm_GetActivationTime (Rtm_TaskType taskId);
LOCAL_INLINE uint32 Rtm_GetExecutionTime (void);
LOCAL_INLINE uint32 Rtm_GetLogicalCoreID (void);
LOCAL_INLINE uint32 Rtm_ConvertTaskID2Idx (Rtm_TaskType taskId);
LOCAL_INLINE Rtm_TaskType Rtm_ConvertTaskIdx2ID (uint32 taskIdx_u32);
LOCAL_INLINE Rtm_IsrType Rtm_ConvertIsrIdx2ID (uint32 isrIdx_u32);
LOCAL_INLINE boolean Rtm_CheckCurrentOSObject (uint32 id);
LOCAL_INLINE uint32 Rtm_GetCurrentContextID (void);

/* Temporary own implementation, because OS version will enable interrupts */
LOCAL_INLINE uint32 Rtm_GetExecutionTime_Driver (void);

/*
 ***********************************************************************************************************************
 * Code
 ***********************************************************************************************************************
 */
/* Get CPU logical core ID who execute this function. */
# if (RTM_NUM_CORES > 1)
#define Rtm_GetLogicalCoreID()    GetCoreID()
#else
#define Rtm_GetLogicalCoreID()    (0U)
#endif

/* Get value of OS provided stop which is usually a tick of free runing counter. */
#define Rtm_GetStopwatch()        Os_Cbk_GetStopwatch()

/* Convert ticks into microseconds us */
#define Rtm_Tickstous(x)        Os_Cbk_Tickstous(x)

/* Get the execution time in ticks consumed by the calling Task/ISR. */
#define Rtm_GetExecutionTime()    GetExecutionTime()

/* Get the maximum stack in bytes consumed by the calling Task/ISR. */
#define Rtm_GetTaskMaxStackUsage(x)  GetTaskMaxStackUsage(x)
#define Rtm_GetISRMaxStackUsage(x)   GetISRMaxStackUsage(x)

/* Get the cumulative execution time in ticks consumed when idle on the specified core. */
#define Rtm_GetIdleTime(coreId)   GetIdleElapsedTime(coreId)        

/* Convert a given task ID to a value from 0 to (OS_NUM_TASKS-1) */
#define Rtm_ConvertTaskID2Idx(taskId) OS_TASKTYPE_TO_INDEX (taskId)


/** Rtm_GetActivationTime *************************************************************************************/
/*!
 * \brief       Get the timestamp of the most recent successful activation of the Task.
 *
 * \param       Rtm_TaskType  Task ID
 * \return      uint32        Timestamp in ticks
 *
 * \note        May be called from task/isr end hook
 */
LOCAL_INLINE uint32 Rtm_GetActivationTime (Rtm_TaskType taskId)
{
    TickType t_tick;
    /* Function for sure called after a task was activated therefore no check if returned value is ok */
    Os_GetTaskActivationTime (taskId, &t_tick);
    return (uint32)t_tick;
}

/** Rtm_GetTaskID *********************************************************************************************/
/*!
 * \brief       Get task ID of current active task
 *
 * \param       void
 * \return      Rtm_TaskType  Task ID
 *
 * \note        May be called inside critical section
 */
LOCAL_INLINE Rtm_TaskType Rtm_GetTaskID (void)
{
    Rtm_TaskType taskId;
    /* Get task id (pointer to structure in flash) */
    /* Function for sure called from Task or ISR context therefore no check if returned value is ok */
    GetTaskID (&taskId);
    return taskId;
}

/** Rtm_ConvertIsrID2Idx **************************************************************************************/
/*!
 * \brief       Convert a given isr ID to a value from 0 to (OS_NUM_ISRS-1)
 *
 * \param       Rtm_IsrType  Task ID
 * \return      uint32               Task index
 *
 * \note        May be called from task/isr end hook
 */
LOCAL_INLINE uint32 Rtm_ConvertIsrID2Idx (Rtm_IsrType isrId)
{
    return (uint32)OS_ISRTYPE_TO_INDEX (isrId);
}

/** Rtm_ConvertTaskIdx2ID *************************************************************************************/
/*!
 * \brief       Convert a given task index to a valid task ID
 *
 * \param       uint32               Task index
 * \return      Rtm_TaskType  Task ID
 */
LOCAL_INLINE Rtm_TaskType Rtm_ConvertTaskIdx2ID (uint32 taskIdx_u32)
{
#ifdef RTM_DEBUG
    if (taskIdx_u32 >= OS_NUM_TASKS)
    {
        Rtm_Assert();
        taskIdx_u32 = OS_NUM_TASKS-1;
    }
#endif
    return (Rtm_TaskType)&Os_const_tasks[taskIdx_u32];
}

/** Rtm_ConvertIsrIdx2ID **************************************************************************************/
/*!
 * \brief       Convert a given isr index to a valid isr ID
 *
 * \param       uint32               ISR index
 * \return      Rtm_IsrType   ISR ID
 */
LOCAL_INLINE Rtm_IsrType Rtm_ConvertIsrIdx2ID (uint32 isrIdx_u32)
{
#ifdef RTM_DEBUG
    if (isrIdx_u32 >= OS_NUM_ISRS)
    {
        Rtm_Assert();
        isrIdx_u32 = OS_NUM_ISRS-1;
    }
#endif
    return &Os_const_isrs[isrIdx_u32];
}

/** Rtm_CheckCurrentOSObject **********************************************************************************/
/*!
 * \brief       Checks the currently running OS object
 *
 * \param       uint32    Task or ISR ID, should not be 0
 * \return      boolean   True if active
 *
 * \note        May be called inside critical section
 */
LOCAL_INLINE boolean Rtm_CheckCurrentOSObject (uint32 id)
{
    boolean ret = FALSE;
    Rtm_IsrType isr = GetISRID();
    /* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
    if ((uint32)isr == id)
    {
        ret = TRUE;
    }
    else if (isr == INVALID_ISR)
    {
        /* No ISR running, check task */
        Rtm_TaskType task;
        GetTaskID (&task);
        /* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
        if ((uint32)task == id)
        {
            ret = TRUE;
        }
    }
    /* Concluding else for misra */
    else
    {  /* Neither ISR nor Task to be measured. */
    }
    return ret;
}

/** Rtm_GetCurrentContextID ***********************************************************************************/
/*!
 * \brief       Returns current context ID (task/ISR ID)
 *
 * \return      uint32   Current context ID
 */
LOCAL_INLINE uint32 Rtm_GetCurrentContextID (void)
{
    Rtm_IsrType isr = GetISRID();
    if (isr == INVALID_ISR)
    {
        /* No ISR running, check task */
        Rtm_TaskType task;
        GetTaskID (&task);
        /* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
        return (uint32)task;
    }
    else
    {
        /* Isr running, return its ID */
        /* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
        return (uint32)isr;
    }
}


#endif /* RTM_OS_ITF_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

