

#ifndef DEM_PRV_CALLDTCSTCHNGDCBK_H
#define DEM_PRV_CALLDTCSTCHNGDCBK_H

#if (DEM_CFG_J1939DCM == DEM_CFG_J1939DCM_ON)
#include "Dem_Prv_J1939Dcm.h"
#endif

typedef struct  {
    Dem_UdsStatusByteType   status;
    boolean                 isStatusChangeToBeCalculated;
}Dem_DTCStatusAndUpdateInfoType;

#if ( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )||(DEM_CFG_J1939DCM == DEM_CFG_J1939DCM_ON)

DEM_INLINE void Dem_TriggerOn_MultipleDTCStatusChange(const Dem_DTCStatusAndUpdateInfoType* Dem_DTCStatusAndUpdateInfo)
{
    Dem_DtcIdIterator dtcIt;
    Dem_DtcIdType DtcId;
    Dem_UdsStatusByteType StatusNew, StatusOld;
    Dem_DtcCodeType dtcCode;

    for(Dem_DtcIdIteratorNew(&dtcIt); Dem_DtcIdIteratorIsValid(&dtcIt); Dem_DtcIdIteratorNext(&dtcIt))
    {
        DtcId = Dem_DtcIdIteratorCurrent(&dtcIt);
        if(Dem_DTCStatusAndUpdateInfo[DtcId].isStatusChangeToBeCalculated == TRUE)  /* Value accessed for all DTCs - Initialized in Dem_InitializeDTCStatusAndUpdateInfo */
        {
            StatusOld = Dem_DTCStatusAndUpdateInfo[DtcId].status;
            StatusNew = Dem_DtcStatusByteRetrieve (DtcId) & DEM_CFG_DTCSTATUS_AVAILABILITYMASK;
            if(StatusNew != StatusOld)
            {
                dtcCode = Dem_GetDtcCode(DtcId);
#if ( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )
                Dem_CallbackDTCStatusChangedIndication( dtcCode, StatusOld, StatusNew);
#endif
#if (DEM_CFG_J1939DCM == DEM_CFG_J1939DCM_ON)
                Dem_J1939DTCStatusChangedIndicationCallback(DtcId, StatusOld, StatusNew);
#endif
            }
        }
    }
}
#endif //( DEM_CFG_DTC_STATUSCHANGEDCALLBACK == DEM_CFG_DTC_STATUSCHANGEDCALLBACK_ON )

#endif
