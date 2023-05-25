/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DEM_EVENTATTRIBUTE_H
#define DEM_EVENTATTRIBUTE_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom event attributes
 *
 * If at least one DemRBDefaultEventCustAttribute is defined, these functions are use to retrieve the data of the
 * custom attributes.
 */

#include "Dem_Events.h"

#if (DEM_CFG_EVT_ATTRIBUTE == DEM_CFG_EVT_ATTRIBUTE_ON)

/**
 * \brief This structure holds the event attributes defined by user.
 *
 * It's user responsibility to add event attributes inside the structure. The name of the structure should be same as
 * mentioned(Dem_EventAttributeType).
 */
typedef struct {
   boolean defect; /**< just some example struct content */
   uint8 value; /**< just some example struct content */
} Dem_EventAttributeType;

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
extern const Dem_EventAttributeType* Dem_getEventUserAttributes(Dem_EventIdType EventId);
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
/**
 * "DEM_EVTS_ATTRIBUTES_INIT" macro shall be defined with event attributes as defined in "Dem_EventAttributeType"
 * structure and initialise the parameters in sequence.
 */
#define DEM_EVTS_USERATTRIBUTES_INIT(DEFECT, VALUE)   \
{                                                 \
   (DEFECT),									  \
   (VALUE)                                     	  \
}

/**
 * \brief These functions are example access functions for the project specific attributes of the Events.
 *
 * They should call Dem_getEventUserAttributes(EventId) to get a pointer to the Dem_EventAttributeType struct where the
 * attributes are stored. Unlike other template functions, these functions are not called by the Dem, so feel free
 * to change their name and signature if needed.
 * The attributes are only available with macro switch setting DEM_CFG_EVT_ATTRIBUTE == DEM_CFG_EVT_ATTRIBUTE_ON which
 * is automatically set if at least one DemRBDefaultEventCustAttribute is configured.
 */
DEM_INLINE boolean Dem_GetEventAttribute_defect(Dem_EventIdType EventId)
{
   return (Dem_getEventUserAttributes(EventId)->defect);
}

/**
 * \brief These functions are example access functions for the project specific attributes of the Events.
 *
 * They should call Dem_getEventUserAttributes(EventId) to get a pointer to the Dem_EventAttributeType struct where the
 * attributes are stored. Unlike other template functions, these functions are not called by the Dem, so feel free
 * to change their name and signature if needed.
 * The attributes are only available with macro switch setting DEM_CFG_EVT_ATTRIBUTE == DEM_CFG_EVT_ATTRIBUTE_ON which
 * is automatically set if at least one DemRBDefaultEventCustAttribute is configured.
 */
DEM_INLINE uint8 Dem_GetEventAttribute_value(Dem_EventIdType EventId)
{
   return (Dem_getEventUserAttributes(EventId)->value);
}

#endif
#endif /* INCLUDE_PROTECTION */

