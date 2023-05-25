

#ifndef DEM_ENVRECORDITERATOR_H
#define DEM_ENVRECORDITERATOR_H

#include "Dem_Types.h"

typedef struct
{
    boolean                         isRequestedRecValid;
    uint8                           requestedRecNum;
    uint16                          end_recIndex;
    uint16                          current_recIndex;
    Dem_EventIdType                 EventId;
}Dem_EnvRecordIteratorType;

#endif
