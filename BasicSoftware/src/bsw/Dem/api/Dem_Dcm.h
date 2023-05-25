

#ifndef DEM_DCM_H
#define DEM_DCM_H


#include "Dem_Types.h"
#include "Dcm_Types.h"

#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
#include "rba_DemObdBasic_Dcm.h"
#endif

/**
 * \defgroup DEM_DCM    DEM - Interface for the Dcm
 *
 * This interface may only be used by the Dcm. Use by other components is prohibited because some of the functions
 * operate on global state and therefore need to be called in a defined order without interference.
 *
 * To use this interface include the header <b>Dem_Dcm.h</b>
 * @{
 */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/**
 * \defgroup DEM_DCM_DTC    Access DTCs and Status Information
 * @{
 */

/**
 *
 * Dem230: Gets the supported DTC formats of the ECU. The supported formats
 *         are configured via DemTypeOfDTCSupported.
 * @param[in]  ClientId   Unique client id, assigned to the instance of the calling module
 * @return Returns the configured DTC translation format. A combination of
 *         different DTC formats is not possible.
 *
 * @note  The only supported DTC translation format is currently ISO14229-1
 */
Dem_DTCTranslationFormatType Dem_GetTranslationType(uint8 ClientId);


/**
 *
 * Dem213: Gets the DTC Status availability mask.
 * @param[in]   ClientId        Unique client id, assigned to the instance of the calling module
 * @param[out]  DTCStatusMask   The value DTCStatusMask indicates the supported DTC
 *                              status bits from the Dem. All supported information
 *                              is indicated by setting the corresponding status bit
 *                              to 1. See ISO14229-1.
 * @return  E_OK: get of DTC status mask was successful\n
 *          E_NOT_OK: get of DTC status mask failed
 */
Std_ReturnType Dem_GetDTCStatusAvailabilityMask(uint8 ClientId, Dem_UdsStatusByteType* DTCStatusMask);


/**
 *
 * Dem212: Gets the status of a DTC.
 *
 * @param[in]  ClientId  Defines the Unique client id, assigned to the instance of the calling module.
 * @param[out]  DTCStatus  This parameter receives the status information of the requested
 *                         DTC. If the return value of the function call is other than
 *                         DEM_STATUS_OK this parameter does not contain valid data.
 *                         0x00...0xFF match DTCStatusMask as defined in ISO14229-1.
 * @return Std_ReturnType: Status of the requested operation
 */
Std_ReturnType Dem_GetStatusOfDTC(uint8 ClientId, uint8* DTCStatus);


/**
 *
 * Dem232: Gets the severity of the requested DTC.
 *
 * @param[in]  ClientId  Defines the Unique client id, assigned to the instance of the calling module.
 * @param[out]  DTCSeverity  This parameter contains the DTCSeverityMask according to ISO14229-1.
 * @return Std_ReturnType: Status of DTC severity of the requested DTC
 */
Std_ReturnType Dem_GetSeverityOfDTC(uint8 ClientId, Dem_DTCSeverityType* DTCSeverity);


/**
 *
 * Dem594: Gets the functional unit of the requested DTC.
 *
 * @param[in] ClientId  Defines the Unique client id, assigned to the instance of the calling module
 * @param[out]  DTCFunctionalUnit  Functional unit value of this DTC.
 * @return Std_ReturnType: Status of DTC functional unit of the requested DTC
 */
Std_ReturnType Dem_GetFunctionalUnitOfDTC(uint8 ClientId, uint8* DTCFunctionalUnit );


/**
 *
 * Dem208: The function Dem_SetDTCFilter (refer to chapter 8.3.4.1.6) shall set the filter mask
 *         attributes to be used for the sub-sequent calls of Dem_GetNumberOfFilteredDTC(),
 *         Dem_GetNextFilteredDTC(), Dem_GetNextFilteredDTCAndFDC(), as well as
 *         Dem_GetNextFilteredDTCAndSeverity().
 *
 * @param[in]  ClientId  Unique client id, assigned to the instance of the calling module
 * @param[in]  DTCStatusMask  According ISO14229-1 StatusOfDTC Values:\n
 *                            0x00: Report all supported DTCs\n
 *                            0x01...0xFF: Match DTCStatusMask as defined in ISO14229-1
 * @param[in]  DTCFormat  Defines the output-format of the requested DTC values for the sub-sequent API calls.
 * @param[in]  DTCOrigin  If the Dem supports more than one event memory this parameter
 *                        is used to select the source memory the DTCs shall be read from.
 * @param[in]  FilterWithSeverity  This flag defines whether severity information (ref. to
 *                                 parameter below) shall be used for filtering. This is to
 *                                 allow for coexistence of DTCs with and without severity information.
 * @param[in]  DTCSeverityMask  This parameter contains the DTCSeverityMask according to ISO14229-1
 *                              (see for example Service 0x19, subfunction 0x08)
 * @param[in]  FilterForFaultDetectionCounter  This flag defines whether the fault detection counter
 *                                             information shall be used for filtering. This is to
 *                                             allow for coexistence of DTCs with and without fault
 *                                             detection counter information. If fault detection counter
 *                                             information is filter criteria, only those DTCs with a
 *                                             fault detection counter value between 1 and 0x7E shall
 *                                             be reported. Remark: If the event does not use the
 *                                             debouncing inside Dem, then the Dem must request this
 *                                             information via GetFaultDetectionCounter.
 * @return  Status of the operation to (re-)set a DTC filter.
 */
Std_ReturnType Dem_SetDTCFilter(uint8 ClientId,
                                         uint8 DTCStatusMask,
                                         Dem_DTCFormatType DTCFormat,
                                         Dem_DTCOriginType DTCOrigin,
                                         boolean FilterWithSeverity,
                                         Dem_DTCSeverityType DTCSeverityMask,
                                         boolean FilterForFaultDetectionCounter);


/**
 *
 * Dem214: Gets the number of a filtered DTC.
 *
 * @param[in]  ClientId  Unique client id, assigned to the instance of the calling module
 * @param[out]  NumberOfFilteredDTC  The number of DTCs matching the defined status mask.
 * @return  Status of the operation to retrieve a number of DTC from the Dem.
 * @see Dem_SetDTCFilter()
 */
Std_ReturnType Dem_GetNumberOfFilteredDTC(uint8 ClientId, uint16* NumberOfFilteredDTC);


/**
 *
 * Dem215: Gets the next filtered DTC.
 *
 * @param[in]  ClientId  Unique client id, assigned to the instance of the calling module
 * @param[out]  DTC  Receives the DTC value returned by the function. If the return value of the
 *                   function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]  DTCStatus  This parameter receives the status information of the requested DTC. It
 *                         follows the format as defined in ISO14229-1 If the return value of the function
 *                         call is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @return  Status of the operation to retrieve a DTC from the Dem.
 * @see Dem_SetDTCFilter()
 */
Std_ReturnType Dem_GetNextFilteredDTC(uint8 ClientId, uint32* DTC, uint8* DTCStatus);


/**
 *
 * Dem227: Gets the current DTC and its associated Fault Detection Counter (FDC) from the Dem. The interface has an
 *         asynchronous behavior, because the FDC might be received asynchronously from a SW-C, too.
 *
 * @param[in]  ClientId  Unique client id, assigned to the instance of the calling module
 * @param[out]  DTC : Receives the DTC value returned by the function. If the return value of the function is
 *                    other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]  DTCFaultDetectionCounter : This parameter receives the Fault Detection Counter information of the
 *                    requested DTC. If the return value of the function call is other than DEM_FILTERED_OK this
 *                    parameter does not contain valid data.
 * @return  Status of the operation to retrieve a DTC from the Dem.
 * @see Dem_SetDTCFilter()
 */
Std_ReturnType Dem_GetNextFilteredDTCAndFDC(uint8 ClientId, uint32* DTC, sint8* DTCFaultDetectionCounter);


/**
 *
 * Dem281: Gets the current DTC and its Severity from the Dem.
 *
 * @param[in]   ClientId  Unique client id, assigned to the instance of the calling module
 * @param[out]  DTC  Receives the DTC value returned by the function. If the return value of the function
 *                   is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]  DTCStatus  Receives the status value returned by the function. If the return value
 *                         of the function is other than DEM_FILTERED_OK this parameter does not
 *                         contain valid data.
 * @param[out]  Dem_DTCSeverityType  Receives the severity value returned by the function. If the
 *                                   return value of the function is other than DEM_FILTERED_OK this
 *                                   parameter does not contain valid data.
 * @param[out]  DTCFunctionalUnit  Receives the functional unit value returned by the function. If
 *                                 the return value of the function is other than DEM_FILTERED_OK
 *                                 this parameter does not contain valid data.
 * @return  Status of the operation to retrieve a DTC from the Dem.
 * @see Dem_SetDTCFilter()
 */
Std_ReturnType Dem_GetNextFilteredDTCAndSeverity(uint8 ClientId,
                                                                   uint32* DTC,
                                                                   uint8* DTCStatus,
                                                                   Dem_DTCSeverityType* DTCSeverity,
                                                                   uint8* DTCFunctionalUnit);


/**
 *
 * Dem209: Sets a freeze frame record filter.
 *
 * @param[in]  DTCFormat  Defines the output-format of the requested DTC values for the sub-sequent API calls.
 * @param[out] NumberOfFilteredRecords Number of freeze frame records currently stored in the event memory.
 * @return     Std_ReturnType: Status of the operation to (re-)set a freeze frame record filter
 *
 * @note
 * DEM supports only DTCKind 'DEM_DTC_KIND_ALL_DTCS'.\n
 * In case of PENDING the caller needs to repeat the service with identical parameters.\n
 * This service is only allowed if the DCM task and the DEM task do not preempt each other
 */
Std_ReturnType Dem_SetFreezeFrameRecordFilter(uint8 ClientId, Dem_DTCFormatType DTCFormat, uint16* NumberOfFilteredRecords);


/**
 *
 * Dem215: Gets the current DTC and its associated snapshot record numbers from the Dem.
 *
 * @param[out]  DTC              Receives the DTC value returned by the function.
 *                                If the return value of the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @param[out]  RecordNumber        Freeze frame record number of the reported DTC (relative addressing).
 *                                If the return value of the function is other than DEM_FILTERED_OK this parameter does not contain valid data.
 * @return      Std_ReturnType: Status of the operation to retrieve the next filtered DTC and freeze frame record number
 *
 * @note
 * DEM supports only DTCKind 'DEM_DTC_KIND_ALL_DTCS'.\n
 * In case of PENDING the caller needs to repeat the service with identical parameters.\n
 * This service is only allowed if the DCM task and the DEM task do not preempt each other
 */
Std_ReturnType Dem_GetNextFilteredRecord(uint8 ClientId, uint32* DTC, uint8* RecordNumber);


#if (DEM_CFG_EVMEMGENERIC_SUPPORTED != FALSE)
/**
 *
 * Dem218: Gets the DTC by occurrence time. There is no explicit parameter for the DTC-origin as the origin always is DEM_DTC_ORIGIN_PRIMARY_MEMORY.
 *
 * @param[in]     ClientId       Defines the Unique client id, assigned to the instance of the calling module
 * @param[in]     DTCRequest     This parameter defines the request type of the DTC.
 * @param[out]    DTC            Receives the DTC value returned by the function.
 *                               If the return value of the function is other than DEM_OCCURR_OK this parameter does not contain valid data.
 * @return        E_OK: get of DTC was successful
 *                E_NOT_OK: the call was not successful
 *                DEM_NO_SUCH_ELEMENT: The requested element is not stored
 */
Std_ReturnType Dem_GetDTCByOccurrenceTime(uint8 ClientId, Dem_DTCRequestType DTCRequest, uint32* DTC);
#endif /* DEM_CFG_EVMEMGENERIC_SUPPORTED */

/**@}*//* End of Doxygen subgroup DEM_DCM_DTC */



/**
 * \defgroup DEM_DCM_EVMEM    Access extended data records and FreezeFrame data
 * @{
 */

/**
 *
 * SWS_Dem_00233: Disables the event memory update of a specific DTC (only one at one time)
 *
 * @param[in]   ClientId : Unique client id, assigned to the instance of the calling module.
 *
 * @return       E_OK: DTC record successfully updated.
 * @return       E_NOT_OK: No DTC selected
 * @return       DEM_WRONG_DTC: Selected DTC value in selected format does not exist or a group of DTC was selected.
 * @return       DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @return       DEM_PENDING: Disabling the DTC record update is currently in progress. The caller shall call this function again at a later moment.
 * @return       DEM_BUSY: A different Dem_SelectDTC dependent operation of this client is currently in progress.
 *
 * @note
 * DEM is not locking any DTC record.\n
 * Instead a copy of the DTC record is taken for further processing on when calling API functions like Dem_SelectExtendedDataRecord and Dem_SelectExtendedDataRecord.
 */
Std_ReturnType Dem_DisableDTCRecordUpdate(uint8 ClientId);


/**
 *
 * SWS_Dem_00234: Enables the event memory update of the DTC disabled by Dem_DisableDTCRecordUpdate() before.
 *
 * @param[in]   ClientId: Unique client id, assigned to the instance of the calling module.
 *
 * @return       E_OK: DTC record successfully updated.
 * @return       E_NOT_OK: No DTC selected
 * @return       DEM_WRONG_DTC: Selected DTC value in selected format does not exist or a group of DTC was selected.
 * @return       DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist
 * @return       DEM_PENDING: Disabling the DTC record update is currently in progress. The caller shall call this function again at a later moment.
 * @return       DEM_BUSY: A different Dem_SelectDTC dependent operation of this client is currently in progress.
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_EnableDTCRecordUpdate(uint8 ClientId);

/**
 *
 * SWS_Dem_91017 : Sets the filter to be used by Dem_GetNextExtendedDataRecord and Dem_GetSizeOfExtendedDataRecordSelection.
 *
 * @param[in]       ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[in]       ExtendedDataNumber: Identification/Number of requested extended data record.
 *                                      Additionally the values 0xFE and 0xFF are explicitly allowed to request the overall size of all OBD records / all records.
 * @return          E_OK: Extended data record successfully selected.
 * @return          E_NOT_OK : Not DTC selected
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 * @return          DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client is currently in progress.
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_SelectExtendedDataRecord(uint8 ClientId, uint8 ExtendedDataNumber);

/**
 *
 * SWS_Dem_00239 : Gets extended data record for the DTC selected by Dem_SelectExtendedDataRecord. The function stores the data in the provided DestBuffer.
 *
 * @param[in]       ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[inout]    BufSize: When the function is called this parameter contains the maximum number of data bytes that can be written to the buffer.
 *                            The function returns the actual number of written data bytes in this parameter.
 * @param[out]       DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the extended data record shall be written to.
 *                                The format is: {ExtendedDataRecord- Number, data[0], data[1], ..., data[n]}
 * @return          E_OK: Extended data record successfully selected.
 * @return          E_NOT_OK : selection function is not called.
 * @return          DEM_BUFFER_TOO_SMALL : provided buffer size too small.
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 * @return          DEM_NO_SUCH_ELEMENT: Found no (further) element matching the filter criteria
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_GetNextExtendedDataRecord(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);

/**
 *
 * SWS_Dem_00240 : Gets the size of Extended Data Record by DTC selected by the call of Dem_SelectExtendedDataRecord.
 *
 * @param[in]       ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]      SizeOfExtendedDataRecord: Size of the requested extended data record(s) including record number.
 *                                              The format for a single ExtendedDataRecord is: {RecordNumber, data[1], ..., data[N]}
 *
 * @return          E_OK: Size returned successfully
 * @return          E_NOT_OK : selection function is not called.
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 * @return          DEM_NO_SUCH_ELEMENT: Record number is not supported by configuration and therefore invalid
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_GetSizeOfExtendedDataRecordSelection(uint8 ClientId, uint16* SizeOfExtendedDataRecord);

/**
 *
 * SWS_Dem_91015 : Sets the filter to be used by Dem_GetNextFreezeFrameData and Dem_GetSizeOfFreezeFrameSelection.
 *
 * @param[in]       ClientId :    Unique client id, assigned to the instance of the calling module.
 * @param[in]       RecordNumber: Unique identifier for a snapshot record as defined in ISO 14229-1. The value 0xFF is a placeholder referencing all snapshot records of the addressed DTC.
 *                              The value 0x00 indicates the DTC-specific WWHOBD snapshot record.
 * @return          E_OK: Extended data record successfully selected.
 * @return          E_NOT_OK : Not DTC selected
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 * @return          DEM_BUSY: A different Dem_SelectDTC dependent operation according to SWS_Dem_01253 of this client is currently in progress.
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_SelectFreezeFrameData(uint8 ClientId, uint8 RecordNumber);

/**
 *
 * SWS_Dem_00236 : Gets freeze frame data by the DTC selected by Dem_SelectFreezeFrameData. The function stores the data in the provided DestBuffer.
 *
 * @param[in]       ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]      BufSize: When the function is called this parameter contains the maximum number of data bytes that can be written to the buffer.
 *                      The function returns the actual number of written data bytes in this parameter.
 * @param[out]      DestBuffer: This parameter contains a byte pointer that points to the buffer, to which the freeze frame data record shall be written to.
 *                      The format is: {RecordNumber, NumOfDIDs, DID[1], data[1], ..., DID[N], data[N]}
 * @return          E_OK: Size and buffer successfully returned.
 * @return          E_NOT_OK : selection function is not called.
 * @return          DEM_BUFFER_TOO_SMALL : provided buffer size too small.
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_GetNextFreezeFrameData(uint8 ClientId, uint8* DestBuffer, uint16* BufSize);

/**
 *
 * SWS_Dem_00238 : Gets the size of freeze frame data by DTC selected by the call of Dem_SelectFreezeFrameData.
 *
 * @param[in]       ClientId: Unique client id, assigned to the instance of the calling module.
 * @param[out]      SizeOfFreezeFrame: Number of bytes in the requested freeze frame record.
 *
 * @return          E_OK: Size returned successfully
 * @return          E_NOT_OK : selection function is not called.
 * @return          DEM_WRONG_DTC: Selected DTC value in selected format does not exist.
 * @return          DEM_WRONG_DTCORIGIN: Selected DTCOrigin does not exist.
 * @return          DEM_PENDING: Selecting the extended data record is currently in progress. The caller shall call this function again at a later moment.
 * @return          DEM_NO_SUCH_ELEMENT: Record number is not supported by configuration and therefore invalid
 *
 * @note
 * In case of DEM_PENDING the caller needs to repeat the service with identical parameters.
 *
 */
Std_ReturnType Dem_GetSizeOfFreezeFrameSelection(uint8 ClientId, uint16* SizeOfFreezeFrame);

/**
 * \defgroup DEM_DCM_STORAGE    DTC storage
 * @{
 */

/**
 *
 * Dem242: Disables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 *
 * @param[in]  ClientId : Unique client id, assigned to the instance of the calling module.
 * @return  E_OK: The operation was successful;
 * @return  DEM_PENDING: The started operation is currently in progress. The caller shall call this function again at a later moment.
 * @return  E_NOT_OK : Invalid ClientId.
 */
Std_ReturnType Dem_DisableDTCSetting(uint8 ClientId);


/**
 *
 * Dem243: (Re)-Enables the DTC setting for all DTCs assigned to the DemEventMemorySet of the addressed client.
 *
 * @param[in]  ClientId : Unique client id, assigned to the instance of the calling module.
 * @return  E_OK: The operation was successful;
 * @return  DEM_PENDING: The started operation is currently in progress. The caller shall call this function again at a later moment.
 * @return  E_NOT_OK : Invalid ClientId.
 */
Std_ReturnType Dem_EnableDTCSetting(uint8 ClientId);

/**@}*//* End of Doxygen subgroup DEM_DCM_STORAGE */


/**
 *
 * Dem91023: Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs.
 *
 * @param[in]  ClientId  Defines the Unique client id, assigned to the instance of the calling module
 * @return  Std_ReturnType: Status of DTC client selection operation
 */
Std_ReturnType Dem_GetDTCSelectionResult(uint8 ClientId);

/**
 *
 * Dem91020: Provides information if the last call to Dem_SelectDTC has selected a valid DTC or group of DTCs ,
 *           respecting the settings if the Dem shall clear only all DTCs.
 *
 * @param[in]  ClientId  Defines the Unique client id, assigned to the instance of the calling module
 * @return Std_ReturnType: Status of DTC client selection for clear
 */
Std_ReturnType Dem_GetDTCSelectionResultForClearDTC(uint8 ClientId);


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/**@}*//* End of Doxygen group DEM_DCM */

#endif /* DEM_DCM_H */
