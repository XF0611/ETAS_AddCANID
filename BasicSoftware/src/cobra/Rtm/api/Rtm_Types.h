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
 * Component : Rtm_Types.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_TYPES_H
#define RTM_TYPES_H

/*! \file
 *
 * \brief Runtime measurement types header
 */


/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Std_Types.h"
#include "Rtm_Cfg.h"


/*
 ***********************************************************************************************************************
 * Type definitions
 ***********************************************************************************************************************
 */
 
/* Define number of cores */
#define RTM_NUM_CORES OS_NUM_CORES

/********************************************************************
* The following data types are defined in "Rte_Rtm_Type.h".
* If the project has no RTE, please use the template "Rte_Rtm_Type.h_tpl".
*  - Rtm_InputValues_Type
*  - Rtm_OutputValues_Type
*  - Rtm_Output_Type
*  - Rtm_MonTask_Type
*  - Rtm_Input_Type
*  - Rtm_OutputCore_Type
********************************************************************/

/**********************
 * MT measure element *
 **********************/
 
/* Simple structure for MT measurements */
typedef struct
{
    /* Runtime, only updated from outside measure in reentrant case */
    uint32 actualRT;
    uint32 minRT;
    uint32 maxRT;

    /* Call distance, only updated from outside measure in reentrant case */
    uint32 callDistance;

    /* Call counter, reentrant calls not included */
    uint32 callCount;
    
    /* Internal values */
    /* Start timestamp of current call of the free running counter */
    uint32 callBeginStopWatch;

    /* Start timestamp of current call of the net runtime timestamp from Os_GetExecutionTime */
    uint32 callBegin;
} Rtm_MTValues_Type;

/* Structure per core in conjunction with reentrant/protected measurements */
typedef struct
{
    /* Measure structure */
    Rtm_MTValues_Type dataMeasured;

    /* Reentrant call count */
    uint32 cntReentCalls;
    
    /* Internal values */
    /* Context ID, must be volatile to enforce the implemented load/store order */
    volatile uint32 contextID;
} Rtm_MTValuesPerCore_Type;

/* Array of structure in conjunction with reentrant measurements */
typedef Rtm_MTValuesPerCore_Type Rtm_MTValuesReentrant[RTM_NUM_CORES];

/* Calibrate, set only from MTInit process (no core dependency) */
typedef uint32 Rtm_MTCalibrate_Type;


#if (RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS)

/*********************************
 * Lifecycle monitoring elements *
 ********************************/

typedef uint32 Rtm_CoreCheckpoint_ValueType;

typedef struct
{
    /* Core checkpoints. */
    Rtm_CoreCheckpoint_ValueType cp;

    /* Timestamp at the point of core checkpoint. */
    uint32 cpTimestamp;

    /* Call counter */
    uint32 ctCp;
} Rtm_CoreCheckpoint_Type;

/* Result values from Lifecycle monitoring */
typedef struct
{
    /* Lifecycle checkpoints. */
    Rtm_CoreCheckpoint_Type core_cp[RTM_LC_CP_CORE_LAST+1];
} Rtm_CoreLifecycle_Type;


/****************************
 * Task monitoring elements *
 ****************************/

/* Result values from task monitoring */
typedef struct
{
    /* Runtime */
    uint32 curRT;
    uint32 minRT;
    uint32 maxRT;

    /* Response time (activation to end) */
    uint32 curResponse;
    uint32 minResponse;
    uint32 maxResponse;

    /* worst case consumed stack in bytes */
    uint32 wcstack;

    /* Call counter */
    uint32 ctCallsTotal_u32;
    
# if (RTM_MON_TASK_STAT == STD_ON)
    /* Runtime */
    uint32 averageRT;
    /* Call counter */
    uint32 callCount;

    /* Internal elements for average and load calculation */
    struct
    {
        uint32 accuRT;
        uint32 ctBeginCalls_u32;
        uint32 tiBeginFree_u32;
    } internal_st;

    /* Load (0x10000 would be 100%) */
    uint16 loadPer64k_u16;

    /* Which sample has generated average, calls and system load */
    uint8 ctSampleProcessed_u8;
# endif

} Rtm_MonTask_Type;


/***************************
 * Isr monitoring elements *
 ***************************/

/* Result values from task monitoring */
typedef struct
{
    /* Runtime */
    uint32 curRT;
    uint32 minRT;
    uint32 maxRT;

    /* worst case consumed stack in bytes */
    uint32 wcstack;

    /* Call counter */
    uint32 ctCallsTotal_u32;
    
# if (RTM_MON_ISR_STAT == STD_ON)
    /* Runtime */
    uint32 averageRT;
    /* Call counter */
    uint32 callCount;

    /* Internal elements for average and load calculation */
    struct
    {
        uint32 accuRT;
        uint32 ctBeginCalls_u32;
        uint32 tiBeginFree_u32;
    } internal_st;

    /* Load (0x10000 would be 100%) */
    uint16 loadPer64k_u16;

    /* Which sample has generated average, calls and system load */
    uint8 ctSampleProcessed_u8;
# endif

} Rtm_MonIsr_Type;

#endif /* #if (RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS) */

/***************************
 * Ipc monitoring elements *
 ***************************/
typedef uint32 Rtm_IpcType;

/* Result values from Ipc monitoring */
typedef struct
{
    /* Runtime */
    uint32 tiCallBegin_u32;

    /* Response time (activation to end) */
    uint32 curResponse;
    uint32 minResponse;
    uint32 maxResponse;

    /* Call counter */
    uint32 ctCallsTotal_u32;
    
#if ((RTM_MON_IPC_STAT == STD_ON) || (RTM_MON_DIRECTIPC_STAT == STD_ON))
    /* Runtime */
    uint32 averageRT;
    /* Call counter */
    uint32 callCount;

    /* Internal elements for average and load calculation */
    struct
    {
        uint32 accuRT;
        uint32 ctBeginCalls_u32;
    } internal_st;

    /* Which sample has generated average, calls and system load */
    uint8 ctSampleProcessed_u8;
# endif

} Rtm_MonIpc_Type;

/***************************
 * Process monitoring elements *
 ***************************/
typedef uint32 Rtm_ProcessType;

/* Result values from Process monitoring */
typedef struct
{
    /* Runtime */
    uint32 tiCallBegin_u32;

    /* Duration time (start to end) */
    uint32 curDuration;
    uint32 minDuration;
    uint32 maxDuration;

    /* Call counter */
    uint32 ctCallsTotal_u32;
    
#if ((RTM_MON_PROCESS_STAT == STD_ON))
    /* Runtime */
    uint32 averageRT;
    /* Call counter */
    uint32 callCount;

    /* Internal elements for average and load calculation */
    struct
    {
        uint32 accuRT;
        uint32 ctBeginCalls_u32;
    } internal_st;

    /* Which sample has generated average, calls and system load */
    uint8 ctSampleProcessed_u8;
# endif

} Rtm_MonProcess_Type;

#if (RTM_CTRL == STD_ON)

# if (RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS)

/**************************
 * Configuration elements *
 **************************/

/* Type definition of the input for one active measurement */
typedef struct
{
    /* ID or pointer to select the measure item */
    uint32 idMeasured_u32;

    /* Index of task is 0..OS_NUM_TASKS-1  if bit 15 is not set
     * Index of  ISR is 0..OS_NUM_ISRS-1   if bit 15 is set
     * Disable dependency if greater (so set to 0xffff to disable it) */
    uint16 idTask_u16;
} Rtm_InputValues_Type;

/* Type definition of the whole input */
typedef struct
{
    /* An input structure for each possible concurrent measurement */
    Rtm_InputValues_Type configs_ast[RTM_CTRL_CONCURRENT_MEASURE];

    /* Duration for the samples, unit is defined through call frequency of main function.
     * When this value switches from 0 to x a sample will start
     * Value from 1..255  inits the trigger mode -> frontend will signal when it is ready to
     *                    receive new data, but the specified sample time will be preserved
     *  */
    uint8 tiSampleTime_u8;

    /* Trigger counter, must be set to ctSample from output to trigger next sample */
    uint8 ctTrigger_u8;
} Rtm_Input_Type;


/********************
 * Visible elements *
 ********************/

/* Type definition of the output of one active measurement */
typedef struct
{
    /* Runtime */
    uint32 averageRT;
    uint32 minRT;
    uint32 maxRT;

    /* Call distance */
    uint32 averageCallDistance;
    uint32 minCallDistance;
    uint32 maxCallDistance;

    /* Call counter */
    uint32 callCount;
    /* Call counter of reentrant calls (also included in callCount) */
    uint32 cntReentCalls;
} Rtm_OutputValues_Type;

/* Type definition of the output of one core */
typedef struct
{
    /* An output structure for each possible concurrent measurement */
    Rtm_OutputValues_Type results_ast[RTM_CTRL_CONCURRENT_MEASURE];

    /* Duration of the last sample */
    uint32 tiSampleDuration_u32;
} Rtm_OutputCore_Type;

/* Type definition of the whole output (this can be in really slow memory) */
typedef struct
{
    /* An output structure for each possible concurrent measurement */
    Rtm_OutputCore_Type coresValue[RTM_NUM_CORES];

    /* Synchronisation counter, incremented after all output values are valid */
    uint8 cntOutputSample;
} Rtm_Output_Type;


# endif /* #if (RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS) */


/********************************************
 * Internal elements for controlled measure *
 ********************************************/

/* Type definition of constant in flash to deliver informations to the frontend */
typedef struct
{
    uint32 endiannessDetection;
    uint32 ticksPerSecond;
    uint16 samplesPerSecond;
    uint8 protocolVersion;
    uint8 concurrentMeasures;
    uint8 numberOfCores;
} Rtm_CtrlConfig_Type;
 
/* Type definition of the input backup for one active measurement */
typedef struct
{
    /* ID or pointer to select the measure item */
    uint32 idMeasured_u32;

    /* ID (pointer to OS internal structure member) of dependent task or ISR */
    uint32 idTask;
    
    /* Backup of ID for detection of end of idle measure */
    uint32 idMeasuredItemLastSample;
} Rtm_InternalInputValues_Type;

/* Type definition of internal values needed for one active measurement */
typedef struct
{
    /* Mirror of output values, will be updated while measure is running and copied to output when sample is ready */
    /* Runtime */
    uint32 accuRT;
    uint32 minRT;
    uint32 maxRT;
    /* Call counter for runtime (without reentrant calls) */
    uint32 cntCallsRT;

    /* Call distance */
    uint32 accuCallDistance;
    uint32 minCallDistance;
    uint32 maxCallDistance;
    /* Reentrant call counter
     * - MSB is used to signal the trigger function that a measured element is active 
     * - Will be set to RTM_MAX_U32 if call distance holds response time */
    uint32 cntReentCalls;

    /* Start timestamp of current call of the free running counter */
    uint32 callBeginStopWatch;

    /* Start timestamp of current call of the net runtime timestamp from Os_GetExecutionTime
     * - Is 0 if the measured element is not active */
    uint32 callBegin;
} Rtm_InternalValues_Type;

/* Type definition for one core of the internal values */
typedef struct
{
    /* Mirror of output values, will be updated while measure is running and copied to output
     * when sample is ready */
    Rtm_InternalValues_Type dataMeasure[RTM_CTRL_CONCURRENT_MEASURE];

#if (RTM_NUM_CORES != 1)
    /* ID or pointer to select the measure item of this core to check if the buffer is valid for SW measure 
     * between MainFunction and processing of cross core command */
    uint32 idMeasured_u32[RTM_CTRL_CONCURRENT_MEASURE];
#endif
    
    /* Start timestamp of current sample */
    uint32 timeSampleBegin;

    /* Calibration value for net runtime measure */
    uint32 timeCalibrate;
} Rtm_InternalCore_Type;

/* Type definition of the whole internal values */
typedef struct
{
    /* Backup of input values */
    Rtm_InternalInputValues_Type inputValue[RTM_CTRL_CONCURRENT_MEASURE];

    /* All data that must be stored per core */
    Rtm_InternalCore_Type coresValue[RTM_NUM_CORES];
} Rtm_Internal_Type;

#endif /* #if (RTM_CTRL == STD_ON) */

#endif /* RTM_TYPES_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

