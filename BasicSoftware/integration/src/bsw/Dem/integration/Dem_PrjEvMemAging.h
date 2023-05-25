/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef DEM_PRJEVMEMAGING_H
#define DEM_PRJEVMEMAGING_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief Custom Aging
 *
 * If DemAgingCounterType is set to USER_DEFINED, these functions handle the aging instead of the Dem-internal ones.
 */

/**
 * \brief Decide whether an event has aged.
 *
 * \param EventId    Identification of event by ID
 * \param LocId      Location index where the event is stored
 * \param StatusNew  Status of event memory location as defined in DEM_EVMEM_STSMASK_*
 * \return           TRUE if aged and FALSE otherwise
 */
DEM_INLINE Dem_boolean_least Dem_EvMemIsAged(Dem_EventIdType EventId, uint16_least LocId, uint16_least StatusNew)
{
  /*Write project specific code here*/
}

/**
 * \brief Get the aging counter
 *
 * If the aging counter is stored in some special way internally (e.g. counting down instead of up) it should be
 * transformed into a value that goes from zero to the aging threshold
 *
 * \param EventMemory  Pointer to the event memory location
 * \return             The scaled aging counter
 */
DEM_INLINE uint16_least Dem_EvMemGetEventMemAgingCounterScaled(const Dem_EvMemEventMemoryType *EventMemory)
{
	/*Write project specific code here*/
}

/**
 * \brief Called when the aging cycle of the event is started
 *
 * \param EventId            Identification of event by ID
 * \param LocId              Location index where the event is stored
 * \param[in,out] StatusNew  Status of event memory location as defined in DEM_EVMEM_STSMASK_*
 * \param[in,out] WriteSts   Indicates whether the event memory location needs to be saved after the method returns. Set
 *                           `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
DEM_INLINE void Dem_EvMemSetAgingCounterOnAgingCycle(Dem_EventIdType EventId, uint16_least LocId, uint16_least* StatusNew, uint16_least* WriteSts)
{

	/*Write project specific code here
	 *Hint: To set the aging counter on start of new aging cycle
	 *Dem_EvMemSetEventMemAgingCounter() can be used*/
}

/**
 * \brief Called when the event gets reported as failed and is entered or already stored in the event memory
 *
 * \param EventId            Identification of event by ID
 * \param LocId              Location index where the event is stored
 * \param[in,out] StatusNew  Status of event memory location as defined in DEM_EVMEM_STSMASK_*
 * \param[in,out] WriteSts   Indicates whether the event memory location needs to be saved after the method returns. Set
 *                           `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
DEM_INLINE void Dem_EvMemSetAgingCounterOnEventFailed(Dem_EventIdType EventId, uint16_least LocId, uint16_least* StatusNew, uint16_least* WriteSts)
{
	/*Write project specific code here
	 *Hint: To set the aging counter on failed
	 *Dem_EvMemSetEventMemAgingCounter() can be used*/
}

/**
 * \brief Called when the event gets reported as passed and is stored in the event memory
 *
 * \param EventId            Identification of event by ID
 * \param LocId              Location index where the event is stored
 * \param[in,out] StatusNew  Status of event memory location as defined in DEM_EVMEM_STSMASK_*
 * \param[in,out] WriteSts   Indicates whether the event memory location needs to be saved after the method returns. Set
 *                           `*WriteSts |= DEM_EVMEM_WRITEMASK_DATA;` to save the location.
 */
DEM_INLINE void Dem_EvMemSetAgingCounterOnEventPassed(Dem_EventIdType EventId, uint16_least LocId, uint16_least *StatusNew, uint16_least* WriteSts)
{
	 /*Write project specific code here
	 *Hint: To set the aging counter on passed
	 *Dem_EvMemSetEventMemAgingCounter() can be used*/
}

/**
 * \brief Called when the aging cycle of the event is started. Decides whether aging counter processing should be done.
 *
 * If it returns FALSE, Dem_EvMemSetAgingCounterOnAgingCycle() will not be called.
 *
 * \param locationStatus  Status of event memory location as defined in DEM_EVMEM_STSMASK_*
 * \return                TRUE if aging calculation is allowed and FALSE otherwise
 */
DEM_INLINE Dem_boolean_least Dem_EvMemIsAgingCalculationAllowed(uint16_least locationStatus)
{
      /*Write project specific code here */
}
#endif

