/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_EVBUFF_H
#define DEM_CFG_EVBUFF_H

#include "Std_Types.h"

/* ------------------------------------------ */
/*    DEM_DEBUGDATA_BEFORE_INIT               */
/* ------------------------------------------ */

#define DEM_CFG_DEBUGDATA_BEFOREINIT_ON   STD_ON
#define DEM_CFG_DEBUGDATA_BEFOREINIT_OFF  STD_OFF
#define DEM_CFG_DEBUGDATA_BEFOREINIT   DEM_CFG_DEBUGDATA_BEFOREINIT_OFF

/* ------------------------------------------ */
/*   DEM_DEBUGDATA_FORTIMEBASEDDEBOUNCING     */
/* ------------------------------------------ */

#define DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING_ON   STD_ON
#define DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING_OFF  STD_OFF
#define DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING   DEM_CFG_DEBUGDATA_FORTIMEBASEDDEBOUNCING_OFF

/* ------------------------------------------ */
/* DEM_CFG_FFPRESTORAGESUPPORT                */
/* ------------------------------------------ */
#define DEM_CFG_FFPRESTORAGESUPPORT_OFF  STD_OFF
#define DEM_CFG_FFPRESTORAGESUPPORT_ON   STD_ON
#define DEM_CFG_FFPRESTORAGESUPPORT  DEM_CFG_FFPRESTORAGESUPPORT_ON

/* ------------------------------------------ */
/* DEM_CFG_FFPRESTORAGE_NONVOLATILE           */
/* ------------------------------------------ */
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE_OFF  STD_OFF
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE_ON   STD_ON
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE  DEM_CFG_FFPRESTORAGE_NONVOLATILE_OFF

/* ------------------------------------------ */
/* DEM_CFG_FFPRESTORAGE_NONVOLATILE_IMMEDIATE */
/* ------------------------------------------ */
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE_IMMEDIATE_OFF  STD_OFF
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE_IMMEDIATE_ON   STD_ON
#define DEM_CFG_FFPRESTORAGE_NONVOLATILE_IMMEDIATE  DEM_CFG_FFPRESTORAGE_NONVOLATILE_IMMEDIATE_OFF

/* Number of event-locations */
#define DEM_CFG_EVBUFF_SIZE   5u

/* Number of Max Prestored FF locations */
#define DEM_CFG_PRESTORED_FF_SIZE   5u

#define DEM_CFG_SUPPORT_PROJECTSPECIFIC_FAILUREMEMORY    FALSE

/*Consistency Check (Made Enumeration type to support other options like 'Enable Consistency check and Disable Internal Check' in future implementation*/
#define DEM_CFG_CONSISTENCY_CHECK_OFF          0u
#define DEM_CFG_CONSISTENCY_CHECK_ON           1u

#define DEM_CFG_SUPPORT_PROJECTSPECIFIC_CONSISTENCYCHECK    DEM_CFG_CONSISTENCY_CHECK_OFF

/* delay for storage of rechecked events based on DemTaskTime: (DemTaskTime * EVBUFF_DELAY). Example: 20ms*50= 1 sec; max.: 255 */
/* only configurable for DEM_CFG_EVBUFF_ON */
#define DEM_CFG_EVBUFF_DELAY_SET_WAITINGFORMONITORING  255u

#if(DEM_CFG_DEBUGDATA_BEFOREINIT == DEM_CFG_DEBUGDATA_BEFOREINIT_ON)
#define DEM_DEBUGDATA_BEFOREINIT_PARAM(A,B)    ,A,B
#else
#define DEM_DEBUGDATA_BEFOREINIT_PARAM(A,B)
#endif

#endif

