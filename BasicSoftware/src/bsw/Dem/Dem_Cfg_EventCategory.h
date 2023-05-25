/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_EVENTCATEGORY_H
#define DEM_CFG_EVENTCATEGORY_H

#include "Std_Types.h"

/* ---------------------------------------- */
/* DEM_CFG_EVTCATEGORY                      */
/* ---------------------------------------- */
#define DEM_CFG_EVTCATEGORY_OFF  STD_OFF
#define DEM_CFG_EVTCATEGORY_ON   STD_ON

#define DEM_CFG_EVTCATEGORY  DEM_CFG_EVTCATEGORY_OFF

#if (DEM_CFG_EVTCATEGORY == DEM_CFG_EVTCATEGORY_ON)

typedef uint8 Dem_EvtCategoryPrioType;

#define DemConf_DemEventCategory_INVALIDCATEGORY                      0u
#define DEM_CFG_CATEGORY_COUNT 0
#define DEM_EVENTCATPRIO_ARRAYLENGTH   (DEM_CFG_CATEGORY_COUNT+1u)

#define DemConf_DemEventCategoryPriority_INVALIDPRIO                  0u

//generate the table index is category
#define DEM_CFG_EVTCATEGORYPRIO \
{\
{DemConf_DemEventCategoryPriority_INVALIDPRIO                },\
}
#endif

#endif

