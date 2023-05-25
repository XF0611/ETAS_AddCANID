/*                                                                                                                     
 ************************************************************************************************************************
 *                                                                                                                      
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.                                                    
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to      
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.       
 * All rights reserved in the event of the grant of a patent, utility model or design.                                  
 *                                                                                                                      
 ************************************************************************************************************************
 ************************************************************************************************
 * Component : Rtm_Cfg.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/                                                                                                                     

#ifndef RTM_CFG_H
#define RTM_CFG_H

/*! \file
 *
 * \brief Module Rtm..
 *
 * It supports runtime measurement in series devices of tasks, runables and functions. It also monitores defined tasks.
 * Up to 5 cores and 255 tasks are supported. Please refer to the manual for further details.
 */

/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */


/* Helper macro, do not change */
#define RTM_MON_LIST_ENTRY(x) { x, Os_Entry_##x }

/* Helper macro, do not change */
#define RTM_MON_PROCESS_LIST_ENTRY(x) { Rtm_Mon_Cfg_ProcessId_##x }

/*! General ************************************************************************************************************/

/* Enable/disable version info api (STD_ON or STD_OFF) */
#define RTM_VERSIONINFOAPI STD_ON

/* Definition of interface type, following labels will be accessible:
   Task monitoring labels:
   - Rtm_MonTask: Holds the acquired task monitoring values for the configured tasks (in the same order as
                              configured)
   - Rtm_MonSamples: Actual sample number for average and load update in task monitorings
   Function measure labels:
   - Rtm_Output: Output structure
   - Rtm_Input: Input structure
 * Debugger
   Values are only accessible through the debugger or ECU internal
 * CalibrationMeasurement
   All interface elements will be accessible through a calibration and measurement tool (e.g. INCA). */
#define RTM_INTERFACE_DEBUGGER 1
#define RTM_INTERFACE_CALIBMEAS 2
#define RTM_INTERFACE RTM_INTERFACE_DEBUGGER

/* How often is the main function called in one minute?
 * This depends on the configured value of RtmMainFunctionPeriod, e.g. task in which the runnable is integrated.
 * E.g. 100ms task -> 600
         40ms task -> 1500 */
#define RTM_MAINFCT_CALLS_PER_MINUTE 600

/* How fast is the free running counter clocked */
#define RTM_TICKS_PER_SECOND 100000000

/* Enable/disable callback to acquire idle time for systems with background tasks (Rtm_Cbk_GetIdleTime) */
#define RTM_IDLETIMECALLBACK STD_ON


/*! Lifecycle measurement ********************************************************************************************/
#define RTM_LC_CP_CORE_STARTUP          0U
#define RTM_LC_CP_CORE_SCISTART         1U
#define RTM_LC_CP_CORE_SCISTOP          2U
#define RTM_LC_CP_CORE_OSSTART          3U
#define RTM_LC_CP_CORE_OSSTART_SYNCH    4U
#define RTM_LC_CP_CORE_USERSTART        5U
#define RTM_LC_CP_CORE_COMRUN           6U
#define RTM_LC_CP_CORE_OSSTOP           7U
#define RTM_LC_CP_CORE_PREP_SHUTDOWN    8U
#define RTM_LC_CP_CORE_SHUTDOWN         9U
#define RTM_LC_CP_CORE_UNKNOWN          255U

#define RTM_LC_CP_CORE_LAST             RTM_LC_CP_CORE_SHUTDOWN
#define RTM_LC_CP_CORE_CFG              (RTM_LC_CP_CORE_STARTUP || RTM_LC_CP_CORE_SCISTART || RTM_LC_CP_CORE_SCISTOP || \
                                         RTM_LC_CP_CORE_OSSTART || RTM_LC_CP_CORE_OSSTART_SYNCH || RTM_LC_CP_CORE_USERSTART || \
                                         RTM_LC_CP_CORE_COMRUN  || RTM_LC_CP_CORE_OSSTOP || RTM_LC_CP_CORE_SHUTDOWN)

/*! Controlled measurement ********************************************************************************************/

/* Enable run-time controlled measurement (e.g. over Rtm) */
#define RTM_CTRL STD_ON

/* Number of concurrent measurements per core.
   Increasing this value will speed up the batch measurement but does occupy RAM and breakpoint units or runtime
   in case of missing hardware support.
   The value must be between 1 and 4
*/
/* if RTM_CTRL is off this value has no impact */
#define RTM_CTRL_CONCURRENT_MEASURE 1


/*! Task/ISR and systemload monitoring ********************************************************************************/

/* How often should the monitoring build statistical values per minute (systemload and stats that are turned on below).
 * In a time frame the free running counter must not overflow (e.g. with 100MHz a timeframe is max. 42.9seconds -> 1) */
#define RTM_MON_SAMPLES_PER_MINUTE 12

/* Should the monitoring build statistical values for ISRs */
#define RTM_MON_ISR_STAT STD_ON

/* Should the monitoring build statistical values for tasks */
#define RTM_MON_TASK_STAT STD_ON

/* Should the monitoring build statistical values for process */
#define RTM_MON_PROCESS_STAT STD_ON

/* Which ISRs should be monitored, the order will define the index in the monitoring array output
 * ISR monitoring can be deactivated with an undefined list
 * This list must end with zero! */
#define RTM_MON_ISR_LIST  \
    RTM_MON_LIST_ENTRY(SystemCounter)    , \
    RTM_MON_LIST_ENTRY(Gpt1_Isr)    , \
    { (ISRType)0, 0 }

/* Which tasks should be monitored, the order will define the index in the monitoring array output
 * This list must end with zero! */
#define RTM_MON_TASK_LIST  \
    RTM_MON_LIST_ENTRY(OsTask_ComCore_ASW_10ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_BswBg_100ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_BSW_1ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_AppCore_BSW_10ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_BSW_SwcRequest)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_BSW_10ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_AppCore_ASW_10ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_ASW_50ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_Startup)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_ASW_10ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_BswBg_1ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_ECU_Startup)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_ASW_100ms)    , \
    RTM_MON_LIST_ENTRY(OsTask_SysCore_DRE)    , \
    RTM_MON_LIST_ENTRY(OsTask_ComCore_BSW_10ms)    , \
    { (TaskType)0, 0 }

#define Rtm_Mon_Cfg_ProcessId_NvM_ReadAll					0u
#define Rtm_Mon_Cfg_ProcessId_NvM_WriteAll					1u
#define Rtm_Mon_Cfg_ProcessId_NvM_WriteBlock					2u
#define Rtm_Mon_Cfg_ProcessId_MemIf_MainFunction					3u
#define Rtm_Mon_Cfg_ProcessId_Fee_Init					4u
#define Rtm_Mon_Cfg_ProcessId_NvM_Init					5u
/* Which Process should be monitored, the order will define the index in the monitoring array output
 * This list must end with zero! */
#define RTM_MON_PROCESS_LIST  \
    RTM_MON_PROCESS_LIST_ENTRY(NvM_ReadAll)    , \
    RTM_MON_PROCESS_LIST_ENTRY(NvM_WriteAll)    , \
    RTM_MON_PROCESS_LIST_ENTRY(NvM_WriteBlock)    , \
    RTM_MON_PROCESS_LIST_ENTRY(MemIf_MainFunction)    , \
    RTM_MON_PROCESS_LIST_ENTRY(Fee_Init)    , \
    RTM_MON_PROCESS_LIST_ENTRY(NvM_Init)    , \
    { (Rtm_ProcessType)0 }

#endif /* RTM_CFG_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/
