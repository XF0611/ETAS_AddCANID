/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVDATAELEMENT_H
#define DEM_CFG_ENVDATAELEMENT_H

#include "Std_Types.h"
#include "Dem_Cfg_EnvDataElement_DataSize.h"
#include "Dcm_Dem.h"

#define DEM_CFG_ENV_DATAELEMENTS_EXTERN_AVAILABILITY   STD_ON
#define DEM_CFG_ENV_DATAELEMENTS_INTERN_AVAILABILITY  STD_OFF

#define DEM_CFG_READDEM_AGINGCTR_UPCNT_SUPPORTED                         FALSE
#define DEM_CFG_READDEM_AGINGCTR_DOWNCNT_SUPPORTED                       FALSE
#define DEM_CFG_READDEM_OCCCTR_SUPPORTED                                 FALSE
#define DEM_CFG_READDEM_OVFLIND_SUPPORTED                                FALSE
#define DEM_CFG_READDEM_SIGNIFICANCE_SUPPORTED                           FALSE
#define DEM_CFG_READDEM_DEBUG0_SUPPORTED                                 FALSE
#define DEM_CFG_READDEM_DEBUG1_SUPPORTED                                 FALSE
#define DEM_CFG_READDEM_EVENT_ID_SUPPORTED                               FALSE
#define DEM_CFG_READDEM_CURRENT_FDC_SUPPORTED                            FALSE
#define DEM_CFG_READDEM_MAX_FDC_DURING_CURRENT_CYCLE_SUPPORTED           FALSE
#define DEM_CFG_READDEM_MAX_FDC_SINCE_LAST_CLEAR_SUPPORTED               FALSE
#define DEM_CFG_READDEM_CYCLES_SINCE_FIRST_FAILED_SUPPORTED              FALSE
#define DEM_CFG_READDEM_CYCLES_SINCE_LAST_FAILED_SUPPORTED               FALSE
#define DEM_CFG_READDEM_CYCLES_SINCE_LAST_FAILED_EXCLUDING_TNC_SUPPORTED   FALSE
#define DEM_CFG_READDEM_FAILED_CYCLES_SUPPORTED                          FALSE
#define DEM_CFG_READDEM_PRJ_SPECIFIC_DATA_ON_RETRIEVE_SUPPORTED          FALSE
#define DEM_CFG_READDEM_PRJ_SPECIFIC_DATA_ON_REPORT_SUPPORTED            FALSE
#define DEM_CFG_DATAELEMENT_LASTFAILEDSINCELASTFAILEDEXCLUDINGTNC_INCLUDESFAILEDCYCLE    TRUE

#define DEM_DATAELEM_SR_FF_57601_NEW_DATA_OBJECT  1
#define DEM_DATAELEM_SR_FF_267_YEAR         2
#define DEM_DATAELEM_SR_FF_267_MONTH        3
#define DEM_DATAELEM_SR_FF_267_DAY          4
#define DEM_DATAELEM_SR_FF_267_HOUR         5
#define DEM_DATAELEM_SR_FF_267_MINUTE       6
#define DEM_DATAELEM_SR_FF_267_SECOND       7
#define DEM_DATAELEM_SR_FF_274_NEW_DATA_OBJECT  8
#define DEM_DATAELEM_SR_FF_53249_NEW_DATA_OBJECT  9
#define DEM_DATAELEM_SR_FF_55553_NEW_DATA_OBJECT  10
#define DEM_DATAELEM_SR_FF_57601_NEW_DATA_OBJECT  11
#define DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_OCCURRENCE_COUNTER  12
#define DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_PENDING_COUNTER  13
#define DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGED_COUNTER  14
#define DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGEING_COUNTER  15

#define DEM_CFG_ENV_DATAELEMENTS \
{ \
   { DEM_ENV_DATAELEMENTS_EXTCS_INIT(NULL_PTR) DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR) 0, FALSE } \
   ,{ &Dcm_DemGetCSData_CS_FF_57601_Odometer_reading, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR) DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_57601_NEW_DATA_OBJECT , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Year, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_YEAR , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Month, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_MONTH , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Day, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_DAY , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Hour, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_HOUR , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Minute, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_MINUTE , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_267_Second, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_267_SECOND , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_274_New_Data_Object, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_274_NEW_DATA_OBJECT , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_53249_New_Data_Object, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_53249_NEW_DATA_OBJECT , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_FF_55553_New_Data_Object, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_FF_55553_NEW_DATA_OBJECT , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_occurrence_counter, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_OCCURRENCE_COUNTER , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_pending_counter, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_FAULT_PENDING_COUNTER , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Aged_counter, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGED_COUNTER , FALSE } \
   ,{ &Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Ageing_counter, DEM_ENV_DATAELEMENTS_INTERN_INIT(NULL_PTR)DemConf_DemDataElementDataSize_DEM_DATAELEM_SR_EDR_RECORD_EXTENDEDDATARECORD0X01_AGEING_COUNTER , FALSE } \
}

#define DEM_CFG_ENV_DATAELEMENTS_ARRAYLENGTH      (14+1)

#endif

