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
 * Component : Rtm_Prv.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Rtm functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef RTM_PRV_H
#define RTM_PRV_H

/*! \file
 *
 * \brief Runtime measurement private header
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */

#include "Rtm.h"
#include "Rtm_Compiler.h"
#include "Rtm_Cfg_SchM.h"
#include "Rtm_Os_Itf.h"


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */

/* Additional checks are made if defined, RAM and runtime usage is significant larger
 * In normal operation this should never be active */
#if 0
# define RTM_DEBUG
#else
# ifdef RTM_DEBUG
#  error Debug mode activated
# endif
#endif


/* Macros for special values */
#define RTM_ISR_MODE RTM_MSB_U16
#define RTM_INIT_MIN_U32 RTM_MAX_U32


/* For controlled measurements */
#if (RTM_CTRL == STD_ON)
/* Must we run on more than one configured core (to securely transfer data and setup the silicon)? */
/* Are we running in a single core system? */
# if (RTM_NUM_CORES > 1)
/* Definition for cross core commands (up to 4 cores are supported, access to combined commands inside uint32 variable
 * for performance improvement) */
#  define RTM_CROSSCORECOM    STD_ON
/* Enum substitute (because of not defined size of enums in ANSI C) type definition of cross core communication */
#  define RTM_CCC_NOTHING     0
/* Turn on: Initial command to turn a measure cycle on */
#  define RTM_CCC_TURNON      1
/* Trigger: Next cycle is triggered, copy buffer and reconfigure */
#  define RTM_CCC_TRIGGER     2
/* Turn off: End a measure cycle, do not copy something */
#  define RTM_CCC_TURNOFF     3
/* Calib: Calibrate the measure */
#  define RTM_CCC_CALIB       4
# else
#  define RTM_CROSSCORECOM STD_OFF
# endif
#else /* #if (RTM_CTRL == STD_ON) */
# define RTM_CROSSCORECOM STD_OFF
#endif /* #if (RTM_CTRL == STD_ON) */

/* If input structure is not in flash, define the macro to fetch it from RAM */
#if (RTM_INTERFACE != RTM_INTERFACE_CALIBMEAS)
# ifndef SchM_CData_Rtm_Rtm_Input_st
#  define SchM_CData_Rtm_Rtm_Input_st() ( &Rtm_Input )
# endif
#endif


/*
 ***********************************************************************************************************************
 * Monitor typedefs
 ***********************************************************************************************************************
 */

/* Entry function */
typedef void (*Rtm_EntryFunc) (void);

/* Task monitoring init tables */
typedef struct
{
  Rtm_TaskType  idTask;
  Rtm_EntryFunc entryFunc;
} Rtm_MonTaskList_Type;

/* Isr monitoring init tables */
typedef struct
{
  Rtm_IsrType   idIsr;
  Rtm_EntryFunc entryFunc;
} Rtm_MonIsrList_Type;

/* Ipc monitoring init tables */
typedef struct
{
  Rtm_IpcType   idIpc;
} Rtm_MonIpcList_Type;

/* Process monitoring init tables */
typedef struct
{
  Rtm_ProcessType   idProcess;
} Rtm_MonProcessList_Type;
/*
 ***********************************************************************************************************************
 * Prototypes
 ***********************************************************************************************************************
 */

/****** Slow memory ******/
#define RTM_START_SEC_CODE_SLOW
#include "Rtm_MemMap.h"
/* Rtm_Mon.c */
FUNC (void, RTM_CODE_SLOW) Rtm_Mon_Init (void);
/* Rtm_Ctrl.c */
#if (RTM_CTRL == STD_ON)
FUNC (void, RTM_CODE_SLOW) Rtm_Ctrl_Init (void);
#endif
/* Rtm_MT.c */
FUNC (void, RTM_CODE_SLOW) Rtm_MT_Init (void);
#define RTM_STOP_SEC_CODE_SLOW
#include "Rtm_MemMap.h"

/****** Normal memory ******/
#define RTM_START_SEC_CODE
#include "Rtm_MemMap.h"
/* Rtm.c */
RTM_NOINLINE (FUNC (void, RTM_CODE) Rtm_EmptyFunc(void));
/* Rtm_Mon.c */
FUNC (void, RTM_CODE) Rtm_Mon_MainFunction (void);

#if (RTM_CTRL == STD_ON)
/* Rtm_Ctrl.c */
FUNC (void, RTM_CODE) Rtm_Ctrl_MainFunction (void);
# if (RTM_CROSSCORECOM == STD_ON)
FUNC (void, RTM_CODE) Rtm_Ctrl_CrossCoreMainFunction (uint8_least coreId_qu8, uint8_least cmd_qu8);
# endif
#endif

/* Debug and test */
#ifdef RTM_DEBUG
FUNC (void, RTM_CODE) Rtm_Assert (void);
#else
# define Rtm_Assert(x)
#endif
#define RTM_STOP_SEC_CODE
#include "Rtm_MemMap.h"

/****** Fast memory ******/
#define RTM_START_SEC_CODE_FAST
#include "Rtm_MemMap.h"
#if (RTM_CTRL == STD_ON)
/* Rtm_Ctrl.c */
FUNC (void, RTM_CODE_FAST) Rtm_Ctrl_StartMeasure (Rtm_InternalValues_Type *pIntValue);
FUNC (void, RTM_CODE_FAST) Rtm_Ctrl_StartMeasureReent (Rtm_InternalValues_Type *pIntValue);
FUNC (void, RTM_CODE_FAST) Rtm_Ctrl_EndMeasure (Rtm_InternalValues_Type *pIntValue);
FUNC (void, RTM_CODE_FAST) Rtm_Ctrl_ConvertMonMsr (uint8_least id, uint32 runTime, uint32 respTime);
#endif
#define RTM_STOP_SEC_CODE_FAST
#include "Rtm_MemMap.h"


/*
 ***********************************************************************************************************************
 * Extern declarations
 ***********************************************************************************************************************
 */

#define RTM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"

#if (RTM_CTRL == STD_ON)
# if (RTM_CROSSCORECOM == STD_ON)
extern VAR (uint32 ,RTM_VAR_CLEARED) Rtm_Ctrl_CrossCoreCommand_u32;
# endif
#endif

#define RTM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rtm_MemMap.h"

#endif /* RTM_PRV_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

