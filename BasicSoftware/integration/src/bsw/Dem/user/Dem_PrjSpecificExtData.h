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
 * Component : Dem_PrjSpecificExtData.h
 * Date      : Jan 16 2020
 * Version   : 2.0
 * Description  : This module implements Dem Project specific extended data for integration functions
 ***********************************************************************************************
*/
#ifndef DEM_PRJSPECIFICEXTDATA_H
#define DEM_PRJSPECIFICEXTDATA_H

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
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"



extern void DemRbClearDTCStart_Fnc(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

extern void DemRbClearDTCFinish_Fnc(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin);

extern void DemCallbackDTCStatusChanged_Fnc(uint32 DTC, uint8 DTCStatusOld, uint8 DTCStatusNew);

extern void Dem_EndOperationCycle_Callback(void);

extern void Dem_ExtendedData_Init(void);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"


#endif /* DEM_PRJSPECIFICEXTDATA_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
