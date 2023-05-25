/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#ifndef DEM_PRJEVMEMPROJECTEXTENSION_H
#define DEM_PRJEVMEMPROJECTEXTENSION_H
#if (DEM_CFG_EVMEM_PROJECT_EXTENSION)
/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom event memory extension
 *
 * If DemRbEvMemProjectExtension is enabled, the custom data structure will be embedded in each event memory location and
 * the methods will be called when the event memory locations are processed
 */

/**
 * \brief Data structure that will be embedded into each memory location.
 *
 * See Dem_PrjEvmemProjectExtension.h_tpl. The content is fully customizable, but the name has to be kept.
 */
typedef struct
{
    uint8 mycounter; /**< just some example struct content */
} Dem_EvMemProjectExtensionType;


#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/**
 * \brief Called in Dem_Init(). Should initialize the custom data structure.
 */
void Dem_EvMemProjectExtensionInit(void);

/**
 * \brief Called cyclically in the Dem_Main task.
 */
void Dem_EvMemProjectExtensionMain(void);

/**
 * \brief Called when an event becomes unrobust and is stored in event memory.
 *
 * \param EventId           Identification of event by ID
 * \param LocId             Location index where the event is stored
 * \param[in,out] WriteSts  Indicates whether the event memory location needs to be saved after the method returns. Set
 *                          `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
void Dem_EvMemProjectExtensionUnRobust(Dem_EventIdType EventId, uint16_least LocId, uint16_least *writeSts);

/**
 * \brief Called when an event becomes failed and is stored in event memory.
 *
 * \param EventId           Identification of event by ID
 * \param LocId             Location index where the event is stored
 * \param[in,out] WriteSts  Indicates whether the event memory location needs to be saved after the method returns. Set
 *                          `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
void Dem_EvMemProjectExtensionFailed(Dem_EventIdType EventId, uint16_least LocId, uint16_least *writeSts);

/**
 * \brief Called when an operation cycle that affects the event is started.
 *
 * \param EventId           Identification of event by ID
 * \param LocId             Location index where the event is stored
 * \param[in,out] WriteSts  Indicates whether the event memory location needs to be saved after the method returns. Set
 *                          `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
void Dem_EvMemProjectExtensionStartOpCycle(uint16_least LocId, uint16_least *writeSts);

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
#endif
#endif /* DEM_PRJEVMEMPROJECTEXTENSION_H */
