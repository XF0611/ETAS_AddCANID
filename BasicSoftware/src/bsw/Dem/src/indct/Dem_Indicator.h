



#ifndef DEM_INDICATOR_H
#define DEM_INDICATOR_H

#include "Dem_Types.h"


#if (DEM_CFG_INDICATOR == DEM_CFG_INDICATOR_ON)
typedef struct
{
    uint16 blinkingCtr;
    uint16 continousCtr;
    uint16 slowFlashCtr;
    uint16 fastFlashCtr;
} Dem_IndicatorStatus;


#define DEM_START_SEC_VAR_CLEARED
#include "Dem_MemMap.h"
DEM_ARRAY_DECLARE(Dem_IndicatorStatus, Dem_AllIndicatorStatus, DEM_INDICATORID_ARRAYLENGTH);
#define DEM_STOP_SEC_VAR_CLEARED
#include "Dem_MemMap.h"

DEM_INLINE uint16 Dem_IndicatorGetBlinkingCounter(uint8 indicatorId)
{
	return (Dem_AllIndicatorStatus[indicatorId].blinkingCtr);
}

DEM_INLINE uint16 Dem_IndicatorGetContinuousCounter(uint8 indicatorId)
{
	return (Dem_AllIndicatorStatus[indicatorId].continousCtr);
}

DEM_INLINE uint16 Dem_IndicatorGetFastFlashCtr(uint8 indicatorId)
{
    return (Dem_AllIndicatorStatus[indicatorId].fastFlashCtr);
}

DEM_INLINE uint16 Dem_IndicatorGetSlowFlashCtr(uint8 indicatorId)
{
    return (Dem_AllIndicatorStatus[indicatorId].slowFlashCtr);
}

DEM_INLINE void Dem_IndicatorSetContinuousCtr(uint8 indicatorId, uint16 continuousCtr)
{
	Dem_AllIndicatorStatus[indicatorId].continousCtr = continuousCtr;
}

DEM_INLINE void Dem_IndicatorSetBlinkingCtr(uint8 indicatorId, uint16 blinkingCtr)
{
	Dem_AllIndicatorStatus[indicatorId].blinkingCtr = blinkingCtr;
}

DEM_INLINE void Dem_IndicatorSetFastFlashCtr(uint8 indicatorId, uint16 fastFlashCtr)
{
    Dem_AllIndicatorStatus[indicatorId].fastFlashCtr = fastFlashCtr;
}

DEM_INLINE void Dem_IndicatorSetSlowFlashCtr(uint8 indicatorId, uint16 slowFlashCtr)
{
    Dem_AllIndicatorStatus[indicatorId].slowFlashCtr = slowFlashCtr;
}

DEM_INLINE void Dem_IndicatorIncrementBehaviourCounter(uint8 indicatorId, uint8 indicatorBehaviour)
{
    uint16 blinkingCounter = Dem_IndicatorGetBlinkingCounter(indicatorId);
    uint16 countinuousCounter = Dem_IndicatorGetContinuousCounter(indicatorId);
    uint16 fastFlashCounter = Dem_IndicatorGetFastFlashCtr(indicatorId);
    uint16 slowFlashCounter = Dem_IndicatorGetSlowFlashCtr(indicatorId);

    if(indicatorBehaviour == DEM_INDICATOR_CONTINUOUS)
    {
        countinuousCounter++;
        Dem_IndicatorSetContinuousCtr(indicatorId, countinuousCounter);
    }
    else if(indicatorBehaviour == DEM_INDICATOR_BLINKING)
    {
        blinkingCounter++;
        Dem_IndicatorSetBlinkingCtr(indicatorId, blinkingCounter);
    }
    else if (indicatorBehaviour == DEM_INDICATOR_BLINK_CONT)
    {
        countinuousCounter++;
        blinkingCounter++;
        Dem_IndicatorSetContinuousCtr(indicatorId, countinuousCounter);
        Dem_IndicatorSetBlinkingCtr(indicatorId, blinkingCounter);
    }
    else if (indicatorBehaviour == DEM_INDICATOR_FAST_FLASH)
    {
        fastFlashCounter++;
        Dem_IndicatorSetFastFlashCtr(indicatorId, fastFlashCounter);
    }
    else if (indicatorBehaviour == DEM_INDICATOR_SLOW_FLASH)
    {
        slowFlashCounter++;
        Dem_IndicatorSetSlowFlashCtr(indicatorId, slowFlashCounter);
    }
    else
    {
        /* To Satisfy Misra */
    }
}

DEM_INLINE void Dem_IndicatorDecrementBehaviourCounter(uint8 indicatorId, uint8 indicatorBehaviour)
{
    uint16 blinkingCounter = Dem_IndicatorGetBlinkingCounter(indicatorId);
    uint16 countinuousCounter = Dem_IndicatorGetContinuousCounter(indicatorId);
    uint16 fastFlashCounter = Dem_IndicatorGetFastFlashCtr(indicatorId);
    uint16 slowFlashCounter = Dem_IndicatorGetSlowFlashCtr(indicatorId);

    if ((indicatorBehaviour == DEM_INDICATOR_CONTINUOUS)  && (countinuousCounter > 0u))
    {
        countinuousCounter--;
        Dem_IndicatorSetContinuousCtr(indicatorId, countinuousCounter);
    }
    else if ((indicatorBehaviour == DEM_INDICATOR_BLINKING) && (blinkingCounter > 0u))
    {
        blinkingCounter--;
        Dem_IndicatorSetBlinkingCtr(indicatorId, blinkingCounter);
    }
    else if ((indicatorBehaviour == DEM_INDICATOR_BLINK_CONT) && (blinkingCounter > 0u) && (countinuousCounter > 0u))
    {
        blinkingCounter--;
        countinuousCounter--;
        Dem_IndicatorSetContinuousCtr(indicatorId, countinuousCounter);
        Dem_IndicatorSetBlinkingCtr(indicatorId, blinkingCounter);
    }
    else if ((indicatorBehaviour == DEM_INDICATOR_FAST_FLASH) && (fastFlashCounter > 0u))
    {
        fastFlashCounter--;
        Dem_IndicatorSetFastFlashCtr(indicatorId, fastFlashCounter);
    }
    else if ((indicatorBehaviour == DEM_INDICATOR_SLOW_FLASH) && (slowFlashCounter > 0u))
    {
        slowFlashCounter--;
        Dem_IndicatorSetSlowFlashCtr(indicatorId, slowFlashCounter);
    }
    else
    {
        /* To satisfy Misra */
    }
}

DEM_INLINE uint8 Dem_EvtGetIndicatorStatus(uint8 indicatorId)
{
    uint8 IndicatorStatus;

    DEM_ENTERLOCK_MON();

    if (Dem_IndicatorGetFastFlashCtr(indicatorId) > 0u)
    {
        IndicatorStatus = DEM_INDICATOR_FAST_FLASH;
    }
    else if(Dem_IndicatorGetSlowFlashCtr(indicatorId) > 0u)
    {
        IndicatorStatus = DEM_INDICATOR_SLOW_FLASH;
    }
    else if((Dem_IndicatorGetBlinkingCounter(indicatorId) > 0u) && (Dem_IndicatorGetContinuousCounter(indicatorId) == 0u))
    {
        IndicatorStatus = DEM_INDICATOR_BLINKING;
    }
    else if((Dem_IndicatorGetBlinkingCounter(indicatorId) == 0u) && (Dem_IndicatorGetContinuousCounter(indicatorId) > 0u))
    {
        IndicatorStatus = DEM_INDICATOR_CONTINUOUS;
    }
    else if((Dem_IndicatorGetBlinkingCounter(indicatorId) > 0u) && (Dem_IndicatorGetContinuousCounter(indicatorId) > 0u))
    {
        IndicatorStatus = DEM_INDICATOR_BLINK_CONT;
    }
    else
    {
        IndicatorStatus = DEM_INDICATOR_OFF;
    }

    DEM_EXITLOCK_MON();

    return IndicatorStatus;
}

#endif /* DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON */


void Dem_UpdateISO14229WIRStatus (Dem_EventIdType EventId);


#endif
