
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
 * Component : Rtm.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_H
#define RTM_H

/*! \file
 *
 * \brief Runtime measurement API header
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm_Chk.h"
#include "Rtm_Types.h"
#include "Rtm_Cfg.h"


/*
 **********************************************************************************************************************
 * Version Information
 **********************************************************************************************************************
*/ 
/* Version identification */
#define RTM_VENDOR_ID                                     (6u)
#define RTM_MODULE_ID                                     (255u)
#define RTM_INSTANCE_ID                                   (0)

/* AUTOSAR specification version information */
#define RTM_AR_RELEASE_MAJOR_VERSION                      4u
#define RTM_AR_RELEASE_MINOR_VERSION                      2u
#define RTM_AR_RELEASE_REVISION_VERSION                   2u

/* Version of Rtm specification */
#define RTM_SW_MAJOR_VERSION                              1u
#define RTM_SW_MINOR_VERSION                              0u
#define RTM_SW_PATCH_VERSION                              0u


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Protocol version for controlled measurement interface to Rtm */
#define RTM_CTRL_PROTOVERSION 1

#define RTM_TASKNOTUSED    (0xFFFFFFFF)

/*! MT measure interface **********************************************************************************************/

/* Normal version for task/isr bodies
 * Parameter must be pointer to Rtm_MTValues_Type */
#define Rtm_MT_StartMeasure(x) Rtm_MT_StartMeasureWroker((x),0)
#define Rtm_MT_EndMeasure(x) Rtm_MT_EndMeasureWroker((x),0)
/* Reentrant and multicore version, expensive
 * Parameter must be pointer to Rtm_MTValuesReentrant */
#if (RTM_NUM_CORES == 1)
# define Rtm_MT_StartMeasureReentrant(x) Rtm_MT_StartMeasureWroker(0,&((*(x))[0]))
# define Rtm_MT_EndMeasureReentrant(x) Rtm_MT_EndMeasureWroker(0,&((*(x))[0]))
#else
# define Rtm_MT_StartMeasureReentrant(x) Rtm_MT_StartMeasureWroker(0,&((*(x))[Rtm_GetLogicalCoreID()]))
# define Rtm_MT_EndMeasureReentrant(x) Rtm_MT_EndMeasureWroker(0,&((*(x))[Rtm_GetLogicalCoreID()]))
#endif

/*! Controlled measure interface **************************************************************************************/

#if (RTM_CTRL == STD_ON)
/* Macro to check for selection */
# if (RTM_CTRL_CONCURRENT_MEASURE == 1)
/* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
#  define RTM_CHECK_ID(ID) (((uint32)(ID) == Rtm_Internal.inputValue[0].idMeasured_u32)?0:-1)
# elif (RTM_CTRL_CONCURRENT_MEASURE == 2)
/* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
#  define RTM_CHECK_ID(ID) (((uint32)(ID) == Rtm_Internal.inputValue[0].idMeasured_u32)?0:            \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[1].idMeasured_u32)?1:-1))
# elif (RTM_CTRL_CONCURRENT_MEASURE == 3)
/* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
#  define RTM_CHECK_ID(ID) (((uint32)(ID) == Rtm_Internal.inputValue[0].idMeasured_u32)?0:            \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[1].idMeasured_u32)?1:           \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[2].idMeasured_u32)?2:-1)))
# elif (RTM_CTRL_CONCURRENT_MEASURE == 4)
/* MISRA RULE 11.3 VIOLATION: Cast from pointer type to integral type is necessary in this case */
#  define RTM_CHECK_ID(ID) (((uint32)(ID) == Rtm_Internal.inputValue[0].idMeasured_u32)?0:            \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[1].idMeasured_u32)?1:           \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[2].idMeasured_u32)?2:           \
                                    (((uint32)(ID) == Rtm_Internal.inputValue[3].idMeasured_u32)?3:-1))))
# else
#  error Not more than 4 concurrent measures supported
# endif

# if (RTM_TARGET == RTM_TARGET_GENERIC)
/* Macro to measure instrumented code sequence with a given ID
 * Parameters are an unicum ID and the codesequence to measure */
#  define RTM_MSR_CODESEQ_ID(ID,CODESEQ)                                                                       \
{                                                                                                                      \
    sint8_least concId = (sint8_least)RTM_CHECK_ID (ID);                                                       \
    if (concId >= 0)                                                                                                   \
    {                                                                                                                  \
        Rtm_Driver_StartMeasure (concId);                                                                       \
        (CODESEQ);                                                                                                     \
        Rtm_Driver_EndMeasure (concId);                                                                         \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        (CODESEQ);                                                                                                     \
    }                                                                                                                  \
}

/* Macro to instrument a code sequence
 * Parameters are an unicum function name or function pointer and the codesequence to measure */
#  define RTM_MSR_CODESEQ(FUNNAME,CODESEQ) RTM_MSR_CODESEQ_ID((FUNNAME),(CODESEQ))

/* Macro to instrument a process call (runnable)
 * Parameter is a process to call and to measure */
#  define RTM_MSR_PROC(PROCNAME) RTM_MSR_CODESEQ((PROCNAME),(PROCNAME)())

# endif /* # if (RTM_TARGET == RTM_TARGET_GENERIC) */
#else /* #if (RTM_CTRL == STD_ON) */

/* Controlled measure off, create empty macros */
# define RTM_MSR_CODESEQ_ID(ID,CODESEQ) (CODESEQ)
# define RTM_MSR_CODESEQ(FUNNAME,CODESEQ) (CODESEQ)
# define RTM_MSR_PROC(PROCNAME) (PROCNAME)()

#endif /* else of #if (RTM_CTRL == STD_ON) */

/*! Lifecycle state measure interface *********************************************************************************/
#define RTM_CORECHECKPOINT_CORE_STARTUP()          Rtm_CoreCheckpoint(RTM_LC_CP_CORE_STARTUP)
#define RTM_CORECHECKPOINT_CORE_SCISTART()         Rtm_CoreCheckpoint(RTM_LC_CP_CORE_SCISTART)
#define RTM_CORECHECKPOINT_CORE_SCISTOP()          Rtm_CoreCheckpoint(RTM_LC_CP_CORE_SCISTOP)
#define RTM_CORECHECKPOINT_CORE_OSSTART()          Rtm_CoreCheckpoint(RTM_LC_CP_CORE_OSSTART)
#define RTM_CORECHECKPOINT_CORE_OSSTART_SYNCH()    Rtm_CoreCheckpoint(RTM_LC_CP_CORE_OSSTART_SYNCH)
#define RTM_CORECHECKPOINT_CORE_USERSTART()        Rtm_CoreCheckpoint(RTM_LC_CP_CORE_USERSTART)
#define RTM_CORECHECKPOINT_CORE_COMRUN()           Rtm_CoreCheckpoint(RTM_LC_CP_CORE_COMRUN)
#define RTM_CORECHECKPOINT_CORE_OSSTOP()           Rtm_CoreCheckpoint(RTM_LC_CP_CORE_OSSTOP)
#define RTM_CORECHECKPOINT_CORE_PREP_SHUTDOWN()    Rtm_CoreCheckpoint(RTM_LC_CP_CORE_PREP_SHUTDOWN)
#define RTM_CORECHECKPOINT_CORE_SHUTDOWN()         Rtm_CoreCheckpoint(RTM_LC_CP_CORE_SHUTDOWN)
#define RTM_CORECHECKPOINT(cp)                     Rtm_CoreCheckpoint(RTM_LC_CP_CORE_##cp##)

/*
 ***********************************************************************************************************************
 * Prototypes - Interface
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_CODE
#include "Rtm_MemMap.h"

#if (RTM_VERSIONINFOAPI == STD_ON)
/* Version info API */
FUNC (void, RTM_CODE) Rtm_GetVersionInfo (Std_VersionInfoType* versionInfo);
#endif

/* System load API, will return load in 64k (0->0% | 0xffff->99.9985%) */
FUNC (uint16, RTM_CODE) Rtm_GetSystemLoad (uint8_least coreId);

/* Main Function */
FUNC (void, RTM_CODE) Rtm_MainFunction (void);

#define RTM_STOP_SEC_CODE
#include "Rtm_MemMap.h"

#define RTM_START_SEC_CODE_FAST
#include "Rtm_MemMap.h"
/* Callouts from OS */
FUNC (void, RTM_CODE_FAST) Rtm_TaskEnd (TaskType taskId);
FUNC (void, RTM_CODE_FAST) Rtm_IsrEnd (ISRType isrId);
FUNC (void, RTM_CODE_FAST) Rtm_IpcStart (Rtm_IpcType ipcId);
FUNC (void, RTM_CODE_FAST) Rtm_IpcEnd (Rtm_IpcType ipcId);
FUNC (void, RTM_CODE_FAST) Rtm_DirectIpcStart (Rtm_IpcType ipcId);
FUNC (void, RTM_CODE_FAST) Rtm_DirectIpcEnd (Rtm_IpcType ipcId);
FUNC (void, RTM_CODE_FAST) Rtm_ProcessStart (Rtm_ProcessType processId);
FUNC (void, RTM_CODE_FAST) Rtm_ProcessEnd (Rtm_ProcessType processId);
#define RTM_STOP_SEC_CODE_FAST
#include "Rtm_MemMap.h"

#define RTM_START_SEC_CODE_SLOW
#include "Rtm_MemMap.h"
/* Init */
FUNC (void, RTM_CODE_SLOW) Rtm_PreOSInit (void);
FUNC (void, RTM_CODE_SLOW) Rtm_Init (void);
FUNC (void, RTM_CODE) Rtm_UploadTraceData (void);
#define RTM_STOP_SEC_CODE_SLOW
#include "Rtm_MemMap.h"

#if (RTM_IDLETIMECALLBACK == STD_ON)
# define RTM_START_SEC_CALLOUT_CODE
# include "Rtm_MemMap.h"
/* Callback to acquire idle time for systems with background tasks */
FUNC (uint32, RTM_CALLOUT_CODE) Rtm_Cbk_GetIdleTime (uint8_least coreId);
# define RTM_STOP_SEC_CALLOUT_CODE
# include "Rtm_MemMap.h"
#endif


/*
 ***********************************************************************************************************************
 * Prototypes - Internal, needed for instrumented measure, interface encapsulated in macros
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_CODE_FAST
#include "Rtm_MemMap.h"

#if ((RTM_CTRL == STD_ON) && (RTM_TARGET == RTM_TARGET_GENERIC))
/* Rtm_Driver_Generic.c */
FUNC (void, RTM_CODE_FAST) Rtm_Driver_StartMeasure (sint8_least id_qu8);
FUNC (void, RTM_CODE_FAST) Rtm_Driver_EndMeasure (sint8_least id_qu8);
#endif
/* Rtm_MT.c */
FUNC (void, RTM_CODE_FAST) Rtm_MT_StartMeasureWroker (Rtm_MTValues_Type* pst,
                                                                        Rtm_MTValuesPerCore_Type* pReent);
FUNC (void, RTM_CODE_FAST) Rtm_MT_EndMeasureWroker (Rtm_MTValues_Type* pst,
                                                                      Rtm_MTValuesPerCore_Type* pReent);

FUNC(uint32, RTM_CODE) Rtm_GetTaskIdx(TaskType task);

FUNC(void, RTM_CODE) Rtm_CoreCheckpoint(Rtm_CoreCheckpoint_ValueType cp);

#define RTM_STOP_SEC_CODE_FAST
#include "Rtm_MemMap.h"


/*
 ***********************************************************************************************************************
 * Extern declarations
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"

/*! Controlled ********************************************************************************************************/

#if (RTM_CTRL == STD_ON)
/* Internal structure for RTM_CHECK_ID */
extern VAR (Rtm_Internal_Type, RTM_VAR_CLEARED) Rtm_Internal;
# if RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS
/* Input structure: Rtm_Input is exported by "SchM_Rtm.h".     *
 *                  if Rtm is integrated in a project without Rte, please *
 *                  use the template "SchM_Rtm.h_tpl"                     */
/* Readonly input structure */
extern VAR (volatile Rtm_Input_Type, RTM_VAR_CLEARED) Rtm_Input;
# endif

/* Output structure */
extern VAR (Rtm_Output_Type, RTM_VAR_CLEARED) Rtm_Output;
#endif


/*! Monitoring ********************************************************************************************************/

/* Sample counter for statistical values from task/isr and load monitoring */
extern VAR (uint8, RTM_VAR_CLEARED) Rtm_MonSamples;

/* System load output values */
extern VAR (uint16, RTM_VAR_CLEARED) Rtm_MonSystemLoadPer64k_au16[RTM_NUM_CORES];

/* Task monitoring output values */
extern VAR (Rtm_MonTask_Type, RTM_VAR_CLEARED) Rtm_MonTask[];
#ifdef RTM_MON_ISR_LIST
/* Isr monitoring output values */
extern VAR (Rtm_MonIsr_Type, RTM_VAR_CLEARED) Rtm_MonIsr[];
#endif
#ifdef RTM_MON_IPC_LIST
/* Ipc monitoring output values */
extern VAR (Rtm_MonIpc_Type, RTM_VAR_CLEARED) Rtm_MonIpc[];
#endif
#ifdef RTM_MON_DIRECTIPC_LIST
/* DirectIpc monitoring output values */
extern VAR (Rtm_MonIpc_Type, RTM_VAR_CLEARED) Rtm_MonDirectIpc[];
#endif

/* Core runtime profiles */
extern VAR (Rtm_CoreLifecycle_Type, RTM_VAR_CLEARED) Rtm_CoreLifecycle[];

#define RTM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"


#endif /* RTM_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

