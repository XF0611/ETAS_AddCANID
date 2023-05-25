 

#include "Dem_NvmCallbacks.h"
#include "Dem_EventStatus.h"
#include "Dem_DisturbanceMemory.h"
#include "Dem_GenericNvData.h"
#include "Dem_EvMemBase.h"
#include "Dem_EvBuff.h"
#include "Dem_Obd.h"

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

/* Event Memory: Nvm Explicit Synchronization: */
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_EvMemNvMWriteRamBlockToNvCallback(void* NvMBuffer, uint16_least LocId)
{
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_EvMemEventMemory[LocId], DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType));
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Interface is defined by AUTOSAR */
Std_ReturnType Dem_EvMemNvmReadRamBlockFromNvCallback(void* NvMBuffer, uint16_least LocId)
{
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_EvMemEventMemory[LocId], NvMBuffer, DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType));
    return E_OK;
}

/*  Pre-stored Freeze frame : Nvm Explicit Synchronization: */
#if (DEM_CFG_FFPRESTORAGE_NONVOLATILE == DEM_CFG_FFPRESTORAGE_NONVOLATILE_ON)
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_PreStoredFFWriteRamBlockToNvCallback(void* NvMBuffer, uint16_least LocId)
{
    DEM_ENTERLOCK_MON();
    if ((Dem_EvtBuffer.Locations[LocId].eventType == C_EVENTTYPE_PRESTORE) && (Dem_isEventIdValid(Dem_EvtBuffer.Locations[LocId].eventId)))
    {
		/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
        DEM_MEMCPY(NvMBuffer, &Dem_EvtBuffer.Locations[LocId], DEM_SIZEOF_TYPE(Dem_EvBuffEvent));
    }
    else
    {
        DEM_MEMSET(NvMBuffer, 0u, DEM_SIZEOF_TYPE(Dem_EvBuffEvent));
    }

    DEM_EXITLOCK_MON();
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Interface is defined by AUTOSAR */
Std_ReturnType Dem_PreStoredFFReadRamBlockFromNvCallback(void* NvMBuffer, uint16_least LocId)
{
    DEM_ENTERLOCK_MON();
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_EvtBuffer.Locations[LocId], NvMBuffer, DEM_SIZEOF_TYPE(Dem_EvBuffEvent));
    DEM_EXITLOCK_MON();
    return E_OK;
}
#endif


/* Event StatusByte: Nvm Explicit Synchronization: */
/* ATTENTION: When changing the NVM-handling of EventStatusByte, consider the size dependencies during project development (see config-table).*/
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_EventStatusByteWriteRamBlockToNvCallback(void* NvMBuffer)
{
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_AllEventsStatusByte, DEM_SIZEOF_VAR(Dem_AllEventsStatusByte));
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_ReadRamBlockFromNvm( void* NvMBuffer ) */
Std_ReturnType Dem_EventStatusByteReadRamBlockFromNvCallback(void* NvMBuffer)
{
    /* ATTENTION: When changing the NVM-handling of EventStatusByte, consider the size dependencies during project development (see config-table).*/
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
	DEM_MEMCPY(&Dem_AllEventsStatusByte, NvMBuffer, DEM_SIZEOF_VAR(Dem_AllEventsStatusByte));

#if DEM_CFG_CUSTOMIZABLEDTCSTATUSBYTE
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_AllEventsStatusByteCust, NvMBuffer, DEM_SIZEOF_VAR(Dem_AllEventsStatusByte));
#endif

    return E_OK;
}


/* History StatusByte: Nvm Explicit Synchronization: */
#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_HistoryStatusByteWriteRamBlockToNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_AllEventsHistoryStatus, DEM_SIZEOF_VAR(Dem_AllEventsHistoryStatus));
    DEM_EXITLOCK_MON();
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_ReadRamBlockFromNvm( void* NvMBuffer ) */
Std_ReturnType Dem_HistoryStatusByteReadRamBlockFromNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_AllEventsHistoryStatus, NvMBuffer, DEM_SIZEOF_VAR(Dem_AllEventsHistoryStatus));
    DEM_EXITLOCK_MON();
    return E_OK;
}
#endif


/* Disturbance Memory */
/* MR12 RULE 11.3 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_WriteRamBlockFromNvm( void* NvMBuffer ) */
#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_DistMemWriteRamBlockToNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_DistMemLocations, DEM_SIZEOF_VAR(Dem_DistMemLocations));
    DEM_EXITLOCK_MON();
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_ReadRamBlockFromNvm( void* NvMBuffer ) */
Std_ReturnType Dem_DistMemReadRamBlockFromNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_DistMemLocations, NvMBuffer, DEM_SIZEOF_VAR(Dem_DistMemLocations));
    DEM_EXITLOCK_MON();
    return E_OK;
}
#endif


/* Dem_GenericNvData */
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_GenericNVDataWriteRamBlockToNvCallback(void* NvMBuffer)
{
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_GenericNvData, DEM_SIZEOF_VAR(Dem_GenericNvData));
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_ReadRamBlockFromNvm( void* NvMBuffer ) */
Std_ReturnType Dem_GenericNVDataReadRamBlockFromNvCallback(void* NvMBuffer)
{
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_GenericNvData, NvMBuffer, DEM_SIZEOF_VAR(Dem_GenericNvData));
    return E_OK;
}


/* Event Indicator */
#if (DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON)
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_EventIndicatorWriteRamBlockToNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_AllEventsIndicatorState, DEM_SIZEOF_VAR(Dem_AllEventsIndicatorState));
    DEM_EXITLOCK_MON();
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Function defined according to AR Spec in NVM - Std_ReturnType NvM_ReadRamBlockFromNvm( void* NvMBuffer ) */
Std_ReturnType Dem_EventIndicatorReadRamBlockFromNvCallback(void* NvMBuffer)
{
    DEM_ENTERLOCK_MON();
	/* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_AllEventsIndicatorState, NvMBuffer, DEM_SIZEOF_VAR(Dem_AllEventsIndicatorState));
    DEM_EXITLOCK_MON();
    return E_OK;
}
#endif

#if (DEM_CFG_ERASE_ALL_CHECK_DATA_SUPPORTED)
/* MR12 RULE 8.13 VIOLATION: The tool does not recognize that the pointer is used in a memcopy macro and data is written, this is a tool issue */
Std_ReturnType Dem_HashIdWriteRamBlockToNvCallback(void* NvMBuffer)
{
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(NvMBuffer, &Dem_HashIdInNvm, DEM_SIZEOF_VAR(Dem_HashIdInNvm));
    return E_OK;
}

/* MR12 RULE 8.13 VIOLATION: Interface is defined by AUTOSAR */
Std_ReturnType Dem_HashIdReadRamBlockFromNvCallback(void* NvMBuffer)
{
    /* MR12 RULE 11.5 VIOLATION: conversion to void pointer needed for memcpy */
    DEM_MEMCPY(&Dem_HashIdInNvm, NvMBuffer, DEM_SIZEOF_VAR(Dem_HashIdInNvm));
    return E_OK;
}
#endif

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"
