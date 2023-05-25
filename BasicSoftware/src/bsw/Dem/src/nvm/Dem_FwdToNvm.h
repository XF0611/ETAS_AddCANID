 


#ifndef DEM_FWDTONVM_H
#define DEM_FWDTONVM_H

#include "Dem_EventStatusNvData.h"
#include "Dem_EvMemNvData.h"
#include "Dem_GenericNvData.h"
#include "Dem_DisturbanceMemoryNvData.h"

/* only size of Dem_EvBuffEvent is needed in case of event buffer because only pre-stored freeze frames data
 * will be permanently stored in the NVM not Dem_EvtBufferState */
#include "Dem_EvBuffTypes.h"
#include "Dem_IndicatorAttributesNvData.h"
#include "Dem_NvmHashIdNvData.h"

#if (DEM_CFG_OBD == DEM_CFG_OBD_ON)
/* FC_VariationPoint_START */
#include "rba_DemObdBasic_DemNvData.h"
/* FC_VariationPoint_END */
#endif

#if( DEM_BFM_ENABLED == DEM_BFM_ON )
/* FC_VariationPoint_START */
#include "rba_DemBfm_NvData.h"
/* FC_VariationPoint_END */
#endif

#endif
