

#ifndef DEM_EVTRELATEDDATA_H
#define DEM_EVTRELATEDDATA_H

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/* ---------------------------------------- */
/* RTE compatibility functions              */
/* ---------------------------------------- */
#if (DEM_CFG_CHECKAPICONSISTENCY == TRUE)
/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventExtendedDataRecord without the BufSize parameter to provide a standard-compliant RTE interface */
Std_ReturnType Dem_GetEventExtendedDataRecordForRTE(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        uint8* DestBuffer
);

/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventExtendedDataRecord to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventExtendedDataRecord_GeneralEvtInfo(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        uint8* DestBuffer
);

/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventFreezeFrameData without the BufSize parameter to provide a standard-compliant RTE interface */
Std_ReturnType Dem_GetEventFreezeFrameDataForRTE(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        boolean ReportTotalRecord,
        uint16 DataId,
        Dem_MaxDataValueType DestBuffer
);

/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventFreezeFrameData to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventFreezeFrameData_GeneralEvtInfo(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        boolean ReportTotalRecord,
        uint16 DataId,
        Dem_MaxDataValueType DestBuffer
);


/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventFreezeFrameDataEx to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventFreezeFrameDataEx_GeneralEvtInfo(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        uint16 DataId,
        uint8* DestBuffer,
        uint16* BufSize
);

/*MR12 RULE 8.5 VIOLATION: Duplicate of Dem_GetEventExtendedDataRecordEx to make it also available in the GeneralDiagnosticInfo RTE interface */
Std_ReturnType Dem_GetEventExtendedDataRecordEx_GeneralEvtInfo(
        Dem_EventIdType EventId,
        uint8 RecordNumber,
        uint8* DestBuffer,
        uint16* BufSize
);

#endif /* DEM_CFG_CHECKAPICONSISTENCY */


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

#endif
