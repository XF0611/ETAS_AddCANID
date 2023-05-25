/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Dcm_Dem.h
 * Date      : Sep 7 2021
 * Version   : 1.0
 * Description  : This module implements user integration header file included by 
 *                Dem_Cfg_EnvDataElement.h and Dem_Cfg_EnvDataElement_DataSize.h
 ***********************************************************************************************
*/
#ifndef DCM_DEM_H
#define DCM_DEM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define DiagUT_START_SEC_CODE                   
#include "DiagUT_MemMap.h"
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_274_New_Data_Object(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetCSData_CS_FF_57601_Odometer_reading(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_53249_New_Data_Object(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Year(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Month(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Day(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Hour(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Minute(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_267_Second(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_FF_55553_New_Data_Object(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_occurrence_counter(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Fault_pending_counter(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Aged_counter(uint8* Buffer);
extern Std_ReturnType Dcm_DemGetSRData_SR_EDR_Record_ExtendedDataRecord0x01_Ageing_counter(uint8* Buffer);
#define DiagUT_STOP_SEC_CODE  
#include "DiagUT_MemMap.h"


#endif /* DCM_DEM_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
