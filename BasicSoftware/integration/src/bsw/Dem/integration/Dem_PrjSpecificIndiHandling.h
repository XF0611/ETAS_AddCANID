/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef DEM_PRJSPECIFICINDIHANDLING_H
#define DEM_PRJSPECIFICINDIHANDLING_H

#include "Dem_Cfg_Events.h"
#include "Dem_Types.h"

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom indicator handling
 *
 * If DemRbProjectSpecificWarningIndicatorHandling is enabled, these methods will replace the Dem-internal indicator
 * handling.
 */


/**
 * \brief Check if any indicator attribute for the event is active and therefore WIR bit should still be set
 *
 * WIR override (Dem_OverwriteWIRStatus()) and OBD MIL are checked separately.
 *
 * \param EventId  The ID of the event
 * \return         TRUE if any indicator attribute is active => WIR status will be set. FALSE if no indicator attribute
 *                 is active => depending on the WIR override and the OBD MIL status the WIR status may be reset.
 */
#define Dem_isAnyIndicatorAttribOn(X)            (FALSE)

/**
 * \brief Initialize the indicator handling and check if NvM data should be deleted
 *
 * If the deletion of all NvM data is requested (Dem_NvMIsInvalidateAllNVMBlocksRequested()) then the indicator data
 * should be erased.
 */
#define Dem_IndicatorAttributeInitCheckNvM()     do {} while (0);

/**
 * \brief Clear the indicator attributes. Called when the event is cleared.
 *
 * \param EventId     The ID of the event
 * \param isoByteOld  The old status byte of the event
 * \param isoByteNew  The new status byte (after the clear operation)
 */
DEM_INLINE void Dem_ClearIndicatorAttributes(Dem_EventIdType EventId,Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)
{
    /* Project Specific Implementation */
}

/**
 * \brief Process event status change in the event memory.
 *
 * Called when the event status in the event memory changes or the event is entered into event memory.
 *
 * \param EventId     The ID of the event
 * \param isoByteOld  The old status byte in the event memory
 * \param isoByteNew  The new status byte to be stored
 */
DEM_INLINE void Dem_SetIndicatorDeActivation_OnEvMemSetStatusNotification(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)   {}

/**
 * \brief Process event displacement.
 *
 * Called when an event is displaced from the event memory.
 *
 * \param EventId     The ID of the displaced event
 * \param isoByteOld  The old status byte in the event memory
 * \param isoByteNew  The new status byte to be stored
 */
DEM_INLINE void Dem_SetIndicatorDeActivation_OnEvMemClearStatusNotification(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)  {}

/**
 * \brief Process operation cycle change.
 *
 * Called when the operation cycle of the event is started.
 *
 * \param EventId     The ID of the event
 * \param isoByteOld  The old status byte
 * \param isoByteNew  The new status byte after the start of the operation cycle
 */
DEM_INLINE void Dem_SetIndicatorDeActivation_OnOperationCycleChange(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew)  {}

/**
 * \brief Check if the event is configured to use indicator handling
 *
 * \param EventId  The ID of the event
 * \return         TRUE if the event is affected by the indicator handling (even if the indicator is currently not
 *                 active). FALSE if the event is not configured to use indicator handling.
 */
DEM_INLINE Dem_boolean_least Dem_EvtRequestsWarningIndicator (Dem_EventIdType EventId){}

/**
 * \brief Check the consistency of the indicator state with the status byte on init
 *
 * Called in Dem_Init() to update the indicator status to be consistent with the event status byte restored from NvM
 *
 * \param EventId      The ID of the event
 * \param EventStatus  The status byte restored from the NvM. It has already been checked for consistency with the
 *                     event memory.
 */
void Dem_IndicatorAttribute_ConsistencyCheck(Dem_EventIdType EventId, uint16_least EventStatus );

/**
 * \brief Process the event being qualified as passed
 *
 * Called when the event is reported as passed or a pre-passed report leads to the event being qualified as passed.
 *
 * \param EventId     The ID of the event
 * \param isoByteOld  The old status byte before the report
 * \param isoByteNew  The new status byte
 */
void Dem_SetIndicatorDeActivation(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew);

/**
 * \brief Process the event being qualified as failed
 *
 * Called when the event is reported as failed or a pre-failed report leads to the event being qualified as failed.
 *
 * \param EventId     The ID of the event
 * \param isoByteOld  The old status byte before the report
 * \param isoByteNew  The new status byte
 */
void Dem_SetIndicatorActivation(Dem_EventIdType EventId, Dem_UdsStatusByteType isoByteOld, Dem_UdsStatusByteType isoByteNew);

/**
 * \brief Called cyclically in the Dem_Main task.
 *
 * Used to do background processing.
 */
void Dem_IndicatorAttributeMainFunction(void);

/**
 * \brief Initialize the custom indicator handling
 *
 * Called in Dem_Init()
 */
void Dem_IndicatorAttributeInit(void);

/**
 * \brief Shut down the custom indicator handling
 *
 * Called in Dem_Shutdown()
 */
#define Dem_IndicatorAttributeShutdown()         do {} while (0);


#endif
