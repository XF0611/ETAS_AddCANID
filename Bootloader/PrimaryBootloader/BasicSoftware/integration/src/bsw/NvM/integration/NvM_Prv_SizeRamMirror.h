/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


/* BSW-8494 */
#ifndef NVM_PRV_SIZE_RAM_MIRROR_H
#define NVM_PRV_SIZE_RAM_MIRROR_H

#include "NvM_Cfg.h"

/* Rounds the array length up to a multiple of a integer value. The reason why this is needed is that buffers
 which are the destination for Fls_Read need to be aligned to the size of a Cache Line. Furthermore the buffer
 itself needs to have a length which is a multiple of the size of a Cache Line.
 Reason: Then for extern flashed the FLS can read directly into this buffer instead of using the internal
 alignment buffer and copy it then to his buffer.
 Currently max. known Cache Line size is 64 Byte => Round up to a 64 Byte multiple length.*/
#define ROUND_UP_64(N) ((((N) + (64u) - 1u) / (64u)) * (64u))

#if (NVM_DEM_BLOCK_LENGTH_STRING_USED == STD_ON)
#include "Dem_FwdToNvm.h"
#endif

#if (DEM_NVM_CFG_NUM_BLOCKS_TOTAL != 0)

typedef union UnionDemBlockType {
        Dem_GenericNvDataType  Dem_GenericNvData;

        Dem_EvMemEventMemoryType Dem_EvMemEventMemory;

#ifdef DEM_CFG_PRESTORED_FREEZEFRAME_NVMIDS
        Dem_EvBuffEvent Dem_EvBuffEvent;
#endif

#if( DEM_BFM_ENABLED == DEM_BFM_ON )
    rba_DemBfm_Record bfmRecord;
#if(RBA_DEMBFM_EXTENDED_DEBUG_DATA_SUPPORTED == DEM_BFM_ON)
    rba_DemBfm_ExtendedDebugDataRecord bfmExtDbgDataRecord;
#endif
    rba_DemBfm_RecordManagementDataNvm bfmManagementData;
#if (RBA_DEMBFM_COUNTER_IN_NVM_ENABLED == DEM_BFM_ON)
    rba_DemBfm_CounterNvm bfmCounterData;
#endif
#endif

#if (DEM_CFG_EVT_INDICATOR == DEM_CFG_EVT_INDICATOR_ON)
        DEM_ARRAY_DEFINE(Dem_EvtIndicatorAttributeState, Dem_AllEventsIndicatorState, DEM_INDICATOR_ATTRIBUTE_ARRAYLENGTH);
#endif

#if(DEM_CFG_ALLOW_HISTORY == DEM_CFG_ALLOW_HISTORY_ON)
        DEM_BITARRAY_DEFINE(Dem_AllEventsHistoryStatus, DEM_EVENTID_ARRAYLENGTH);
#endif

#if(DEM_CFG_DISTURBANCE_MEMORY == DEM_CFG_DISTURBANCE_MEMORY_ON)
        DEM_ARRAY_DEFINE( Dem_DistMemLocationType, Dem_DistMemLocations, DEM_CFG_DISTURBANCE_MEMORY_ARRAYLENGTH);
#endif

        DEM_ARRAY_DEFINE(      uint8, Dem_AllEventsStatusByte, DEM_EVENTID_ARRAYLENGTH);

#if (DEM_CFG_ERASE_ALL_DATA_SUPPORTED == DEM_CFG_ERASE_ALL_WITH_HASHID_CHECK)
        DEM_ARRAY_DEFINE(Dem_NvmHashIdType, Dem_HashIdInNvm, DEM_NVM_HASH_ID_SIZE);
#endif
#if (DEM_CFG_OBD != DEM_CFG_OBD_OFF)
        rba_DemObdBasic_PdtcMemType obdPdtcMemData[DEM_CFG_PERMANENT_MEMORY_SIZE];
#endif
#if DEM_CFG_OBD_IUMPR
        rba_DemObdBasic_IumprNvDataType rba_DemObdBasic_IumprNvData;
#endif
#if DEM_CFG_OBD_DTR
        rba_DemObdBasic_DtrDataType rba_DemObdBasic_DtrData;
#endif

}UnionDemBlockType;

#define NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING (ROUND_UP_64(DEM_SIZEOF_TYPE(UnionDemBlockType) + NVM_PRV_CFG_SIZE_ADDING_LENGTH_STRING))

#else

#define NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING 0


#endif

#define NVM_PRV_SIZE_RAM_MIRROR  (NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING > NVM_PRV_SIZE_RAM_MIRROR_USER_CONF?\
                NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING : NVM_PRV_SIZE_RAM_MIRROR_USER_CONF)

#if (NVM_CRYPTO_USED == STD_ON)

#define NVM_PRV_CFG_SIZE_CRYPTO_BUFFER (NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING > NVM_PRV_CFG_SIZE_CRYPTO_BUFFER_CFG?\
                NVM_PRV_SIZE_RAM_MIRROR_LENGTH_STRING : NVM_PRV_CFG_SIZE_CRYPTO_BUFFER_CFG)
#else

#define NVM_PRV_CFG_SIZE_CRYPTO_BUFFER NVM_PRV_CFG_SIZE_CRYPTO_BUFFER_CFG

#endif

#endif /* NVM_PRV_SIZE_RAM_MIRROR_H */
/* END BSW-8494 */
