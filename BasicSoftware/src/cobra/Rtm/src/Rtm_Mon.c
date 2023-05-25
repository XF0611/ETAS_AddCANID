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
 * Component : Rtm_Mon.c
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/


/*! \file
 *
 * \brief Runtime measurement hardware independent part, monitoring of isrs and tasks
 *
 * This file contains the end task and isr hook handler.
 *
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm_Prv.h"
#include "Rtm_Driver_Prv.h"
#include "Rte_Rtm.h" /* Intended crossscheck of the fn prototypes are identical between for BSW and ASW/SWC for
                                some modules acc. to AUTOSAR
                                if this file is not generated by the Rte, please use the template provided */

/*
 ***********************************************************************************************************************
 * Local preprocessor defines
 ***********************************************************************************************************************
 */

#define RTM_TASK_MON_NUM ((sizeof(Rtm_MonTaskList)-1)/sizeof(Rtm_MonTaskList_Type))
#ifdef RTM_MON_ISR_LIST
# define RTM_ISR_MON_NUM ((sizeof(Rtm_MonIsrList)-1)/sizeof(Rtm_MonIsrList_Type))
#else
# define RTM_ISR_MON_NUM 0
#endif
#ifdef RTM_MON_IPC_LIST
# define RTM_IPC_MON_NUM ((sizeof(Rtm_MonIpcList)-1)/sizeof(Rtm_MonIpcList_Type))
#else
# define RTM_IPC_MON_NUM 0
#endif
#ifdef RTM_MON_DIRECTIPC_LIST
# define RTM_DIRECTIPC_MON_NUM ((sizeof(Rtm_MonDirectIpcList)-1)/sizeof(Rtm_MonIpcList_Type))
#else
# define RTM_DIRECTIPC_MON_NUM 0
#endif
#ifdef RTM_MON_PROCESS_LIST
# define RTM_PROCESS_MON_NUM ((sizeof(Rtm_MonProcessList)-1)/sizeof(Rtm_MonProcessList_Type))
#else
# define RTM_PROCESS_MON_NUM 0
#endif


/*
 ***********************************************************************************************************************
 * Prototypes of local functions only used here (maybe auto inlined)
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_CODE
#include "Rtm_MemMap.h"
static FUNC (void, RTM_CODE) Rtm_Mon_Idle (void);
#define RTM_STOP_SEC_CODE
#include "Rtm_MemMap.h"


/*
 ***********************************************************************************************************************
 * Variable definition
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_CONST_UNSPECIFIED
#include "Rtm_MemMap.h"
/* Generate constant with configured tasks */
CONST (Rtm_MonTaskList_Type, RTM_CONST) Rtm_MonTaskList[] = { RTM_MON_TASK_LIST };
#ifdef RTM_MON_ISR_LIST
/* Generate constant with configured isrs */
CONST (Rtm_MonIsrList_Type, RTM_CONST) Rtm_MonIsrList[] = { RTM_MON_ISR_LIST };
#endif
#ifdef RTM_MON_IPC_LIST
/* Generate constant with configured IPCs */
CONST (Rtm_MonIpcList_Type, RTM_CONST) Rtm_MonIpcList[] = { RTM_MON_IPC_LIST };
#endif
#ifdef RTM_MON_DIRECTIPC_LIST
/* Generate constant with configured DirectIPCs */
CONST (Rtm_MonIpcList_Type, RTM_CONST) Rtm_MonDirectIpcList[] = { RTM_MON_DIRECTIPC_LIST };
#endif
#ifdef RTM_MON_PROCESS_LIST
/* Generate constant with configured DirectIPCs */
CONST (Rtm_MonProcessList_Type, RTM_CONST) Rtm_MonProcessList[] = { RTM_MON_PROCESS_LIST };
#endif
#define RTM_STOP_SEC_CONST_UNSPECIFIED
#include "Rtm_MemMap.h"


#define RTM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"

/* System load output values */
VAR (uint16, RTM_VAR_CLEARED) Rtm_MonSystemLoadPer64k_au16[RTM_NUM_CORES];

/* Task monitoring output values */
VAR (Rtm_MonTask_Type, RTM_VAR_CLEARED) Rtm_MonTask[RTM_TASK_MON_NUM];
#ifdef RTM_MON_ISR_LIST
VAR (Rtm_MonIsr_Type, RTM_VAR_CLEARED) Rtm_MonIsr[RTM_ISR_MON_NUM];
#endif
#ifdef RTM_MON_IPC_LIST
VAR (Rtm_MonIpc_Type, RTM_VAR_CLEARED) Rtm_MonIpc[RTM_IPC_MON_NUM];
#endif
#ifdef RTM_MON_DIRECTIPC_LIST
VAR (Rtm_MonIpc_Type, RTM_VAR_CLEARED) Rtm_MonDirectIpc[] = {RTM_DIRECTIPC_MON_NUM};
#endif
#ifdef RTM_MON_PROCESS_LIST
VAR (Rtm_MonProcess_Type, RTM_VAR_CLEARED) Rtm_MonProcess[RTM_PROCESS_MON_NUM];
#endif

/* Sample counter for statistical values (avrg runtime and load) */
VAR (uint8, RTM_VAR_CLEARED) Rtm_MonSamples;

/* Core runtime profiles */
VAR (Rtm_CoreLifecycle_Type, RTM_VAR_CLEARED) Rtm_CoreLifecycle[RTM_NUM_CORES];

#define RTM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"

#define RTM_START_SEC_VAR_NO_INIT_8
#include "Rtm_MemMap.h"

/* Lookup table, index is TaskID, content is index of output value array */
static VAR (uint8, RTM_VAR_NO_INIT) Rtm_MonTaskLookupTable[RTM_NUM_TASKS];

#ifdef RTM_MON_ISR_LIST
/* Lookup table, index is IsrID, content is index of output value array */
static VAR (uint8, RTM_VAR_NO_INIT) Rtm_MonIsrLookupTable[RTM_NUM_ISRS];
#endif

#define RTM_STOP_SEC_VAR_NO_INIT_8
#include "Rtm_MemMap.h"

/* Check number of configured tasks, compiler error will occur if more than 255 are configured */
RTM_CHECK_UPPERLIMIT(TaskCountForMon, RTM_TASK_MON_NUM, 255);
#ifdef RTM_MON_ISR_LIST
/* Check number of configured ISRs, compiler error will occur if more than 255 are configured */
RTM_CHECK_UPPERLIMIT(IsrCountForMon, RTM_ISR_MON_NUM, 255);
#endif


/*
 ***********************************************************************************************************************
 * Code
 ***********************************************************************************************************************
 */

 
#define RTM_START_SEC_CODE_SLOW
#include "Rtm_MemMap.h"

/** Rtm_Mon_Init ***************************************************************************************/
/*!
 * \brief       Initializing function for runtime monitoring.
 *
 * \param       void
 * \return      void
 *
 * It shall be called before a task or ISR hook is called.
 */
FUNC (void, RTM_CODE_SLOW) Rtm_Mon_Init (void)
{
    uint32 idx;

    /* Init minimas */
    for (idx = 0; idx < RTM_TASK_MON_NUM; idx++)
    {
        Rtm_MonTask[idx].minRT = RTM_INIT_MIN_U32;
        Rtm_MonTask[idx].minResponse = RTM_INIT_MIN_U32;
    }
    /* Init task lookup table */
    for (idx = 0; idx < RTM_NUM_TASKS; idx++)
    {
        Rtm_MonTaskLookupTable[idx] = RTM_MAX_U8;
    }
    for (idx = 0; idx < RTM_TASK_MON_NUM; idx++)
    {
        Rtm_MonTaskLookupTable [Rtm_ConvertTaskID2Idx(Rtm_MonTaskList[idx].idTask)] = (uint8)idx;
    }
#ifdef RTM_MON_ISR_LIST
    /* Init for ISR monitoring */
    {
        /* Init minimas */
        for (idx = 0; idx < RTM_ISR_MON_NUM; idx++)
        {
            Rtm_MonIsr[idx].minRT = RTM_INIT_MIN_U32;
        }
        /* Init ISR lookup table */
        for (idx = 0; idx < RTM_NUM_ISRS; idx++)
        {
            Rtm_MonIsrLookupTable[idx] = RTM_MAX_U8;
        }
        for (idx = 0; idx < RTM_ISR_MON_NUM; idx++)
        {
            Rtm_MonIsrLookupTable [Rtm_ConvertIsrID2Idx(Rtm_MonIsrList[idx].idIsr)] = (uint8)idx;
        }
    }
#endif
#ifdef RTM_MON_IPC_LIST
    /* Init for IPC monitoring */
    {
        /* Init minimas */
        for (idx = 0; idx < RTM_IPC_MON_NUM; idx++)
        {
            Rtm_MonIpc[idx].minResponse = RTM_INIT_MIN_U32;
        }
    }
#endif
#ifdef RTM_MON_DIRECTIPC_LIST
    /* Init for DirectIPC monitoring */
    {
        /* Init minimas */
        for (idx = 0; idx < RTM_DIRECTIPC_MON_NUM; idx++)
        {
            Rtm_MonDirectIpc[idx].minResponse = RTM_INIT_MIN_U32;
        }
    }
#endif
#ifdef RTM_MON_PROCESS_LIST
    /* Init for Process monitoring */
    {
        /* Init minimas */
        for (idx = 0; idx < RTM_PROCESS_MON_NUM; idx++)
        {
            Rtm_MonProcess[idx].minDuration = RTM_INIT_MIN_U32;
        }
    }
#endif
}

#define RTM_STOP_SEC_CODE_SLOW
#include "Rtm_MemMap.h"



#define RTM_START_SEC_CODE
#include "Rtm_MemMap.h"

/** Rtm_Mon_MainFunction ***************************************************************************************/
/*!
 * \brief       Main function for sub module monitor
 *
 * \param       void
 * \return      void
 *
 * This should be called from Rtm_MainFunction.
 */
FUNC (void, RTM_CODE) Rtm_Mon_MainFunction (void)
{
#if (RTM_MON_SAMPLES_PER_MINUTE < RTM_MAINFCT_CALLS_PER_MINUTE)
    /* Check for sample generation of monitoring */
    static Rtm_MonSample ctDiv = 0;
    ++ctDiv;
    if (ctDiv >= RTM_MAINFCT_CALLS_PER_MINUTE / RTM_MON_SAMPLES_PER_MINUTE)
    {
        ctDiv = 0;
#endif
        /* Generate new samples */
        ++Rtm_MonSamples;
        
        /* Acquire system load from OS */
        Rtm_Mon_Idle ();
#if (RTM_MON_SAMPLES_PER_MINUTE < RTM_MAINFCT_CALLS_PER_MINUTE)
    }
#endif
}

/** Rtm_Mon_Idle ***********************************************************************************************/
/*!
 * \brief       Get the systemload
 *
 * \param       void
 * \return      void
 *
 * This should be called frequently before the free running timer can overflow
 */
static FUNC (void, RTM_CODE) Rtm_Mon_Idle (void)
{
    static uint32 idleTimestamp[RTM_NUM_CORES];
    static uint32 stopWatchTimestamp;
    uint32 stopwatch, load;
    uint8_least coreId;
    stopwatch = (uint32)Os_Cbk_GetStopwatch();

    for (coreId = 0; coreId < RTM_NUM_CORES; coreId++)
    {
        #if (RTM_NUM_CORES > 1U)
        /* Multicore */
        uint32 idleTime = Rtm_GetIdleTime(coreId);
        #else
        /* Single core */
        uint32 idleTime = Rtm_GetIdleTime(OS_CORE_CURRENT);
        #endif
        
        load = (uint32)(Rtm_Mul64kDiv32 (idleTime - idleTimestamp[coreId], stopwatch - stopWatchTimestamp));
        if (load > 0xffffu)
        {
            load = 0;
        }
        else if (load == 0)
        {
            load = 0xffffu;
        }
        else
        {
            load = 0x10000u - load;
        }

        Rtm_MonSystemLoadPer64k_au16[coreId] = (uint16)load;
        idleTimestamp[coreId] = idleTime;
    }
    stopWatchTimestamp = stopwatch;
}

#define RTM_STOP_SEC_CODE
#include "Rtm_MemMap.h"


#define RTM_START_SEC_CODE_FAST
#include "Rtm_MemMap.h"


/** Rtm_IsrEnd ************************************************************************************************/
/*!
 * \brief       Handler for the ISR end hook from RTA-OS
 *
 * \param       Rtm_IsrType  ID of ISR that comes to an end
 * \return      void
 *
 * This is called from RTA-OS whenever a ISR ends its execution. It collects the runtime/response time of
 * selected isrs.
 */
FUNC (void, RTM_CODE_FAST) Rtm_IsrEnd (Rtm_IsrType isrId)
{
#ifdef RTM_MON_ISR_LIST
    uint32 isrIndex;

    /* Convert identifier to index */
    isrIndex = Rtm_ConvertIsrID2Idx (isrId);

    /* Should this ISR be monitored? (Get index through lookup table in RAM) */
    if (Rtm_MonIsrLookupTable[isrIndex] < RTM_ISR_MON_NUM)
    {
        uint32 runTime;
        Rtm_MonIsr_Type* isrMonitor;

        /* Get the summed up net runtime of the actual ISR */
        runTime = Rtm_GetExecutionTime ();

        /* Generate pointer to output structure */
        isrMonitor = &Rtm_MonIsr [ Rtm_MonIsrLookupTable[isrIndex] ];

        /* Store net runtime */
        isrMonitor->curRT = runTime;
        if (isrMonitor->minRT > runTime)
        {
            isrMonitor->minRT = runTime;
        }
        if (isrMonitor->maxRT < runTime)
        {
            isrMonitor->maxRT = runTime;
        }

        /* Store worst case stack */
        isrMonitor->wcstack = Rtm_GetISRMaxStackUsage(isrId);

        /* Count call */
        isrMonitor->ctCallsTotal_u32++;

# if (RTM_CTRL == STD_ON)
        /* Check for selected ISR that is also monitored */
        {
            sint8_least concId = (sint8_least)RTM_CHECK_ID (isrId);
            if (concId >= 0)
            {
                Rtm_Ctrl_ConvertMonMsr (concId, runTime, 0);
            }
        }
# endif

# if (RTM_MON_ISR_STAT == STD_ON)
        /* Accumulate for average */
        isrMonitor->internal_st.accuRT += runTime;

        /* Generate average? */
        if (isrMonitor->ctSampleProcessed_u8 != Rtm_MonSamples)
        {
            const uint32 accuRT = isrMonitor->internal_st.accuRT;
            /* Get free running counter value */
            uint32 stopwatch = Rtm_GetStopwatch();

            /* Calculate calls */
            isrMonitor->callCount = isrMonitor->ctCallsTotal_u32 - isrMonitor->internal_st.ctBeginCalls_u32;

            /* Calculate average (calls surely != 0) */
            isrMonitor->averageRT = accuRT / isrMonitor->callCount;
            /* Calculate load */
            runTime = (uint32)(Rtm_Mul64kDiv32 (accuRT, stopwatch - isrMonitor->internal_st.tiBeginFree_u32));
            isrMonitor->loadPer64k_u16 = (uint16) ((runTime < 0xfffful) ? runTime : 0xffffu);

            /* Set internal structure for next sample */
            isrMonitor->internal_st.accuRT = 0;
            isrMonitor->internal_st.tiBeginFree_u32 = stopwatch;
            isrMonitor->internal_st.ctBeginCalls_u32 = isrMonitor->ctCallsTotal_u32;

            /* Signal sample is generated */
            isrMonitor->ctSampleProcessed_u8 = Rtm_MonSamples;
        }
# endif
    }
    else
#endif /* #ifdef RTM_MON_ISR_LIST */
#if (RTM_CTRL == STD_ON)
    /* Check for selected ISR that is not monitored */
    {
        sint8_least concId = (sint8_least)RTM_CHECK_ID (isrId);
        if (concId >= 0)
        {
            Rtm_Ctrl_ConvertMonMsr (concId, Rtm_GetExecutionTime (), 0);
        }
    }
#else
    {
    }
#endif
}

/** Rtm_TaskEnd ***********************************************************************************************/
/*!
 * \brief       Handler for the task end hook from RTA-OS
 *
 * \param       Rtm_TaskType  ID of task that comes to an end
 * \return      void
 *
 * This is called from RTA-OS whenever a task ends its execution. It collects the runtime/response time of
 * selected tasks and calls necessary functions for cross core communication of the controlled measure.
 */
FUNC (void, RTM_CODE_FAST) Rtm_TaskEnd (Rtm_TaskType taskId)
{
    uint32 taskIndex;

    /* Convert identifier to index */
    taskIndex = Rtm_ConvertTaskID2Idx (taskId);

    /* Should this task be monitored? (Get index through lookup table in RAM) */
    if (Rtm_MonTaskLookupTable[taskIndex] < RTM_TASK_MON_NUM)
    {
        uint32 runTime, stopwatch, respTime;
        Rtm_MonTask_Type* pTaskMonitor;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();
        /* Get the summed up net runtime of the actual task */
        runTime = Rtm_GetExecutionTime ();

        /* Generate pointer to output structure */
        pTaskMonitor = &Rtm_MonTask [ Rtm_MonTaskLookupTable[taskIndex] ];

        /* Get the activation time and calculate response time (overflow from max(uint32) to 0 is considered) */
        respTime = stopwatch - Rtm_GetActivationTime(taskId);
        /* Store response time */
        pTaskMonitor->curResponse = respTime;
        if (pTaskMonitor->minResponse > respTime)
        {
            pTaskMonitor->minResponse = respTime;
        }
        if (pTaskMonitor->maxResponse < respTime)
        {
            pTaskMonitor->maxResponse = respTime;
        }

        /* Store net runtime */
        pTaskMonitor->curRT = runTime;
        if (pTaskMonitor->minRT > runTime)
        {
            pTaskMonitor->minRT = runTime;
        }
        if (pTaskMonitor->maxRT < runTime)
        {
            pTaskMonitor->maxRT = runTime;
        }

        /* Store worst case stack */
        pTaskMonitor->wcstack = Rtm_GetTaskMaxStackUsage(taskId);

        /* Count call */
        pTaskMonitor->ctCallsTotal_u32++;

#if (RTM_CTRL == STD_ON)
        /* Check for selected task that is also monitored */
        {
            sint8_least concId = (sint8_least)RTM_CHECK_ID (taskId);
            if (concId >= 0)
            {
                Rtm_Ctrl_ConvertMonMsr (concId, runTime, respTime);
            }
        }
#endif

#if (RTM_MON_TASK_STAT == STD_ON)
        /* Accumulate for average */
        pTaskMonitor->internal_st.accuRT += runTime;

        /* Generate average? */
        if (pTaskMonitor->ctSampleProcessed_u8 != Rtm_MonSamples)
        {
            const uint32 accuRT = pTaskMonitor->internal_st.accuRT;
            
            /* Calculate calls */
            pTaskMonitor->callCount = pTaskMonitor->ctCallsTotal_u32 - pTaskMonitor->internal_st.ctBeginCalls_u32;

            /* Calculate average (calls surely != 0) */
            pTaskMonitor->averageRT = accuRT / pTaskMonitor->callCount;
            /* Calculate load */
            runTime = (uint32)(Rtm_Mul64kDiv32 (accuRT, stopwatch - pTaskMonitor->internal_st.tiBeginFree_u32));
            pTaskMonitor->loadPer64k_u16 = (uint16) ((runTime < 0xfffful) ? runTime : 0xffffu);

            /* Set internal structure for next sample */
            pTaskMonitor->internal_st.accuRT = 0;
            pTaskMonitor->internal_st.tiBeginFree_u32 = stopwatch;
            pTaskMonitor->internal_st.ctBeginCalls_u32 = pTaskMonitor->ctCallsTotal_u32;

            /* Signal sample is generated */
            pTaskMonitor->ctSampleProcessed_u8 = Rtm_MonSamples;
        }
#endif
    }
#if (RTM_CTRL == STD_ON)
    else
    /* Check for selected task that is not monitored */
    {
        sint8_least concId = (sint8_least)RTM_CHECK_ID (taskId);
        if (concId >= 0)
        {
            Rtm_Ctrl_ConvertMonMsr (concId, Rtm_GetExecutionTime (), 
               Rtm_GetStopwatch() - Rtm_GetActivationTime(taskId));
        }
    }
#endif

#if (RTM_CROSSCORECOM == STD_ON)
    /* Check if a slave process should run */
    if (Rtm_Ctrl_CrossCoreCommand_u32 != 0)
    {
        const uint8_least coreID_qu8 = (uint8_least) Rtm_GetLogicalCoreID();
        /* Generate uint8 pointer to uint32 cross core command variable because we cannot use masks to access
         * it because of possible concurrent accesses from other cores */
        /* MISRA RULE 11.4 VIOLATION: Cast of pointer is necessary in this case */
        uint8* const crossCoreCommand_pu8 = (uint8*)&Rtm_Ctrl_CrossCoreCommand_u32;
        /* Command pending for this core? */
        if (crossCoreCommand_pu8[coreID_qu8])
        {
            Rtm_Ctrl_CrossCoreMainFunction ((uint8_least)coreID_qu8, crossCoreCommand_pu8[coreID_qu8]);
            crossCoreCommand_pu8[coreID_qu8] = 0;
        }
    }
#endif
}

/** Rtm_GetTaskIdx ********************************************************************************************/
/*!
 * \brief       Gives the index of the configured tasks 
 *
 * \param       TaskType task  Task whose index needs to be obtained
 * \return      void
 *
 */
FUNC(uint32, RTM_CODE) Rtm_GetTaskIdx(TaskType task)  
{
    uint32 taskIndex_u32=0x00;

	taskIndex_u32 = Rtm_MonTaskLookupTable [Rtm_ConvertTaskID2Idx(task)];

	if(taskIndex_u32 == (uint32)RTM_MAX_U8)
	{
		taskIndex_u32 = RTM_TASKNOTUSED;
	}

	return taskIndex_u32;
}

/** Rtm_CoreCheckpoint ****************************************************************************************/
/*!
 * \brief       Record the core runtime state from startup to shutdown at interested checkpoints.
 *
 * \param       Rtm_CoreCheckpoint_ValueType Checkpoint of CPU runtime execution.
 * \return      void
 *
 */
FUNC(void, RTM_CODE) Rtm_CoreCheckpoint(Rtm_CoreCheckpoint_ValueType cp)
{
	if((Rtm_GetLogicalCoreID() >= RTM_NUM_CORES) || (cp > RTM_LC_CP_CORE_LAST))
	{
		/* This is not a known cpu core. */
# ifdef RTM_DEBUG
            Rtm_Assert();
# endif
	}
	else
	{
		/* known CPU core. */
		Rtm_CoreLifecycle[Rtm_GetLogicalCoreID()].core_cp[cp].cp = cp;
        Rtm_CoreLifecycle[Rtm_GetLogicalCoreID()].core_cp[cp].ctCp++;
        Rtm_CoreLifecycle[Rtm_GetLogicalCoreID()].core_cp[cp].cpTimestamp = Rtm_GetStopwatch();
	}
}

/** Rtm_IpcStart **********************************************************************************************/
/*!
 * \brief       Handler for the Ipc start hook from IPC
 *
 * \param       Rtm_IpcType  ID of IPC that comes to an start of transmission.
 * \return      void
 *
 * This is called from IPC whenever a IPC start its transmission. It collects the runtime/response time of
 * selected IPC channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_IpcStart (Rtm_IpcType ipcId)
{
#if (RTM_MON_IPC_STAT == STD_ON)
    /* Should this IPC be monitored? */
    if (ipcId < RTM_IPC_MON_NUM)
    {
        uint32 stopwatch = 0;
        Rtm_MonIpc_Type* pIpcMonitor = 0;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pIpcMonitor = &Rtm_MonIpc [ ipcId ];

        pIpcMonitor->tiCallBegin_u32 = stopwatch;
    }
#endif
}

/** Rtm_IpcEnd ************************************************************************************************/
/*!
 * \brief       Handler for the Ipc end hook from IPC
 *
 * \param       Rtm_IpcType  ID of IPC that comes to an end of transmission.
 * \return      void
 *
 * This is called from IPC whenever a IPC end its transmission. It collects the runtime/response time of
 * selected IPC channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_IpcEnd (Rtm_IpcType ipcId)
{
#if (RTM_MON_IPC_STAT == STD_ON)
    /* Should this IPC be monitored? */
    if (ipcId < RTM_IPC_MON_NUM)
    {
        uint32 stopwatch, respTime = 0;
        Rtm_MonIpc_Type* pIpcMonitor = 0;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pIpcMonitor = &Rtm_MonIpc [ ipcId ];

        /* Get the activation time and calculate response time (overflow from max(uint32) to 0 is considered) */
        respTime = stopwatch - pIpcMonitor->tiCallBegin_u32;

        /* Store response time */
        pIpcMonitor->curResponse = respTime;
        if (pIpcMonitor->minResponse > respTime)
        {
            pIpcMonitor->minResponse = respTime;
        }
        if (pIpcMonitor->maxResponse < respTime)
        {
            pIpcMonitor->maxResponse = respTime;
        }

        /* Count call */
        pIpcMonitor->ctCallsTotal_u32++;

        /* Accumulate for average */
        pIpcMonitor->internal_st.accuRT += respTime;

        /* Generate average? */
        if (pIpcMonitor->ctSampleProcessed_u8 != Rtm_MonSamples)
        {
            const uint32 accuRT = pIpcMonitor->internal_st.accuRT;
            
            /* Calculate calls */
            pIpcMonitor->callCount = pIpcMonitor->ctCallsTotal_u32 - pIpcMonitor->internal_st.ctBeginCalls_u32;

            /* Calculate average (calls surely != 0) */
            pIpcMonitor->averageRT = accuRT / pIpcMonitor->callCount;

            /* Set internal structure for next sample */
            pIpcMonitor->internal_st.accuRT = 0;
            pIpcMonitor->internal_st.ctBeginCalls_u32 = pIpcMonitor->ctCallsTotal_u32;

            /* Signal sample is generated */
            pIpcMonitor->ctSampleProcessed_u8 = Rtm_MonSamples;
        }

    }
#endif
}

/** Rtm_DirectIpcStart ****************************************************************************************/
/*!
 * \brief       Handler for the Ipc start hook from DirectIPC
 *
 * \param       Rtm_IpcType  ID of DirectIPC that comes to an start of transmission.
 * \return      void
 *
 * This is called from DirectIPC whenever a DirectIPC start its transmission. It collects the runtime/response
 * time of selected DirectIPC channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_DirectIpcStart (Rtm_IpcType ipcId)
{
#if (RTM_MON_DIRECTIPC_STAT == STD_ON)
    /* Should this DirectIPC be monitored? */
    if (ipcId < RTM_DIRECTIPC_MON_NUM)
    {
        uint32 stopwatch;
        Rtm_MonIpc_Type* pIpcMonitor;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pIpcMonitor = &Rtm_MonDirectIpc [ ipcId ];

        pIpcMonitor->tiCallBegin_u32 = stopwatch;
    }
#endif
}


/** Rtm_DirectIpcEnd ******************************************************************************************/
/*!
 * \brief       Handler for the Ipc end hook from DirectIPC
 *
 * \param       Rtm_IpcType  ID of DirectIPC that comes to an end of transmission.
 * \return      void
 *
 * This is called from DirectIPC whenever a DirectIPC end its transmission. It collects the runtime/response
 * time of selected IPC channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_DirectIpcEnd (Rtm_IpcType ipcId)
{
#if (RTM_MON_DIRECTIPC_STAT == STD_ON)
    /* Should this DirectIPC be monitored? */
    if (ipcId < RTM_DIRECTIPC_MON_NUM)
    {
        uint32 stopwatch, respTime;
        Rtm_MonIpc_Type* pIpcMonitor;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pIpcMonitor = &Rtm_MonDirectIpc [ ipcId ];

        /* Get the activation time and calculate response time (overflow from max(uint32) to 0 is considered) */
        respTime = stopwatch - pIpcMonitor->tiCallBegin_u32;

        /* Store response time */
        pIpcMonitor->curResponse = respTime;
        if (pIpcMonitor->minResponse > respTime)
        {
            pIpcMonitor->minResponse = respTime;
        }
        if (pIpcMonitor->maxResponse < respTime)
        {
            pIpcMonitor->maxResponse = respTime;
        }

        /* Count call */
        pIpcMonitor->ctCallsTotal_u32++;

        /* Accumulate for average */
        pIpcMonitor->internal_st.accuRT += respTime;

        /* Generate average? */
        if (pIpcMonitor->ctSampleProcessed_u8 != Rtm_MonSamples)
        {
            const uint32 accuRT = pIpcMonitor->internal_st.accuRT;
            
            /* Calculate calls */
            pIpcMonitor->callCount = pIpcMonitor->ctCallsTotal_u32 - pIpcMonitor->internal_st.ctBeginCalls_u32;

            /* Calculate average (calls surely != 0) */
            pIpcMonitor->averageRT = accuRT / pIpcMonitor->callCount;

            /* Set internal structure for next sample */
            pIpcMonitor->internal_st.accuRT = 0;
            pIpcMonitor->internal_st.ctBeginCalls_u32 = pIpcMonitor->ctCallsTotal_u32;

            /* Signal sample is generated */
            pIpcMonitor->ctSampleProcessed_u8 = Rtm_MonSamples;
        }

    }
#endif
}

/** Rtm_ProcessStart ****************************************************************************************/
/*!
 * \brief       Handler for the Process start hook from Process
 *
 * \param       Rtm_ProcessType  ID of Process that comes to an start of Process.
 * \return      void
 *
 * This is called from Process whenever a Process start its Process. It collects the runtime/response
 * time of selected Process channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_ProcessStart (Rtm_ProcessType processId)
{
#if (RTM_MON_PROCESS_STAT == STD_ON)
    /* Should this Process be monitored? */
    if (processId < RTM_PROCESS_MON_NUM)
    {
        uint32 stopwatch;
        Rtm_MonProcess_Type* pProcessMonitor;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pProcessMonitor = &Rtm_MonProcess [ processId ];

        pProcessMonitor->tiCallBegin_u32 = stopwatch;
    }
#endif
}

/** Rtm_ProcessEnd ******************************************************************************************/
/*!
 * \brief       Handler for the Process end hook from Process
 *
 * \param       Rtm_IpcType  ID of Process that comes to an end of Process.
 * \return      void
 *
 * This is called from Process whenever a Process end its Process. It collects the runtime/response
 * time of selected Process channel.
 */
FUNC (void, RTM_CODE_FAST) Rtm_ProcessEnd (Rtm_ProcessType processId)
{
#if (RTM_MON_PROCESS_STAT == STD_ON)
    /* Should this Process be monitored? */
    if (processId < RTM_PROCESS_MON_NUM)
    {
        uint32 stopwatch, durationTime;
        Rtm_MonProcess_Type* pProcessMonitor;

        /* Get free running counter value */
        stopwatch = Rtm_GetStopwatch();

        /* Generate pointer to output structure */
        pProcessMonitor = &Rtm_MonProcess [ processId ];

        /* Get the duration time and calculate response time (overflow from max(uint32) to 0 is considered) */
        durationTime = stopwatch - pProcessMonitor->tiCallBegin_u32;

        /* Store response time */
        pProcessMonitor->curDuration = durationTime;
        if (pProcessMonitor->minDuration > durationTime)
        {
            pProcessMonitor->minDuration = durationTime;
        }
        if (pProcessMonitor->maxDuration < durationTime)
        {
            pProcessMonitor->maxDuration = durationTime;
        }

        /* Count call */
        pProcessMonitor->ctCallsTotal_u32++;


        /* Accumulate for average */
        pProcessMonitor->internal_st.accuRT += durationTime;

        /* Generate average? */
        if (pProcessMonitor->ctSampleProcessed_u8 != Rtm_MonSamples)
        {
            const uint32 accuRT = pProcessMonitor->internal_st.accuRT;
            
            /* Calculate calls */
            pProcessMonitor->callCount = pProcessMonitor->ctCallsTotal_u32 - pProcessMonitor->internal_st.ctBeginCalls_u32;

            /* Calculate average (calls surely != 0) */
            pProcessMonitor->averageRT = accuRT / pProcessMonitor->callCount;

            /* Set internal structure for next sample */
            pProcessMonitor->internal_st.accuRT = 0;
            pProcessMonitor->internal_st.ctBeginCalls_u32 = pProcessMonitor->ctCallsTotal_u32;

            /* Signal sample is generated */
            pProcessMonitor->ctSampleProcessed_u8 = Rtm_MonSamples;
        }

    }
#endif
}

#define RTM_STOP_SEC_CODE_FAST
#include "Rtm_MemMap.h"

/*
 ***********************************************************************************************************************
 * End of file
 ***********************************************************************************************************************
*/