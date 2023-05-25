/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVEXTENDEDDATAREC_H
#define DEM_CFG_ENVEXTENDEDDATAREC_H

#define DEM_CFG_ENVEXTDATA2DATAELEMENT \
{ \
   DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_OCCURRENCE_COUNTER,   DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_PENDING_COUNTER,   DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGED_COUNTER,   DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGEING_COUNTER,    /* Record_ExtendedDataRecord0x01 */ \
   0 \
}

#define DEM_EXTDATAREC_RECORD_EXTENDEDDATARECORD0X01  1

#define DEM_CFG_ENVEXTDATAREC \
{ \
/*	   RecNum Trigger                       Update Index  */ \
	 { 0,     DEM_TRIGGER_NONE,             FALSE,0    } \
	,{ 1,     DEM_TRIGGER_ON_CONFIRMED,     TRUE ,4    } /* DEM_EXTDATAREC_RECORD_EXTENDEDDATARECORD0X01 */ \
}

#define DEM_CFG_ENVEXTDATAREC_ARRAYLENGTH  (1+1)

#endif

