/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_DTCID_H
#define DEM_CFG_DTCID_H

#include "Dem_Cfg_EventId.h"

#define DEM_CFG_EVCOMB_DISABLED       1u
#define DEM_CFG_EVCOMB_ONSTORAGE      2u
#define DEM_CFG_EVCOMB_ONRETRIEVAL    3u
#define DEM_CFG_EVCOMB            DEM_CFG_EVCOMB_DISABLED

#define DEM_DTCID_INVALID           0u
#define DEM_DTCID_COUNT             12u
#define DEM_DTCID_ARRAYLENGTH       (DEM_DTCID_COUNT+1u)

#define DemConf_DemDTCClass_DemDTC_DTC_0x000001                      1u
#define DemConf_DemDTCClass_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff  2u
#define DemConf_DemDTCClass_DemDTC_DTC_0xc07388                      3u
#define DemConf_DemDTCClass_DemDTC_DTC_0xc12687                      4u
#define DemConf_DemDTCClass_DemDTC_DTC_0xc14587                      5u
#define DemConf_DemDTCClass_DemDTC_DTC_0xc22a87                      6u
#define DemConf_DemDTCClass_DemDTC_DTC_0xc41487                      7u
#define DemConf_DemDTCClass_DemDTC_DTC_0xd56217                      8u
#define DemConf_DemDTCClass_DemDTC_DTC_0xd56316                      9u
#define DemConf_DemDTCClass_DemDTC_DTC_0xe00141                      10u
#define DemConf_DemDTCClass_DemDTC_DTC_0xe00142                      11u
#define DemConf_DemDTCClass_DemDTC_DTC_0xe26200                      12u

#define DEM_DTCGROUPID_INVALID           0u
#define DEM_DTCGROUPID_COUNT             1u
#define DEM_DTCGROUPID_ARRAYLENGTH       (DEM_DTCGROUPID_COUNT+1u)

// genInfo: for (EvCombination==OFF) the explicit mapping from dtcid to eventId is necessary, because an event may not be assigned to any DTC
#define  DEM_MAP_EVENTID_DTCID   \
const Dem_DtcIdType Dem_MapEventIdToDtcId[DEM_EVENTID_ARRAYLENGTH] = \
{ \
   DEM_DTCID_INVALID \
   ,DEM_DTCID_INVALID                       /* ADC_E_WRITEVERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* CAN_E_TIMEOUT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* CAN_E_UNINTENDED_INTERRUPT */ \
   ,DEM_DTCID_INVALID                       /* DIO_E_GET_CONTROL_FAILURE */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0x000001  /* DTC_0x000001_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff  /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xc07388  /* DTC_0xc07388_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xc12687  /* DTC_0xc12687_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xc14587  /* DTC_0xc14587_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xc22a87  /* DTC_0xc22a87_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xc41487  /* DTC_0xc41487_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xd56217  /* DTC_0xd56217_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xd56316  /* DTC_0xd56316_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xe00141  /* DTC_0xe00141_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xe00142  /* DTC_0xe00142_Event */ \
   ,DemConf_DemDTCClass_DemDTC_DTC_0xe26200  /* DTC_0xe26200_Event */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_ACCESS */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_ALIGNMENT */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_AVB_DMAC */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_CRC */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_DESC_DATA */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_FRAME_RECEIVE */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_INTERRUPT_CTL_CHECK */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_LATECOLLISION */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_MODULE_STOP_CHECK */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_MULTIPLECOLLISION */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_OPMODE */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_OVERSIZEFRAME */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_PAYLOAD_CRC */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_REG_READ_VERIFY */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_REG_WRITE_VERIFY */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_RESIDUAL_BIT_FRAME */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_RX_FRAMES_LOST */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_SINGLECOLLISION */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_TIMEOUT */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_TIMERINC_FAILED */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_TRANSMIT_RETRY_OVER */ \
   ,DEM_DTCID_INVALID                       /* ETH_E_UNDERSIZEFRAME */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_COMPARE_FAILED */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_ERASE_FAILED */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_GET_CONTROL_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_GET_SEMAPHORE_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_READ_FAILED */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_RELEASE_SEMAPHORE_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_UNEXPECTED_FLASH_ID */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_WRITE_FAILED */ \
   ,DEM_DTCID_INVALID                       /* FLS_E_WRITE_VERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* GPT_E_WRITEVERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_CHECK_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_CLOCK_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_GET_CONTROL_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_INTERNAL_BUS_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_READ_BACK_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_TIMEOUT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* MCU_E_WRITE_TIMEOUT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* PORT_E_FUSE_MONITORING_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* PORT_E_GET_CONTROL_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* PORT_E_WRITE_VERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_DATA_TX_TIMEOUT_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_HARDWARE_ERROR */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_UNINTENDED_EXECUTION_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* SPI_E_WRITE_VERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* WDG_E_DISABLE_REJECTED */ \
   ,DEM_DTCID_INVALID                       /* WDG_E_MODE_FAILED */ \
   ,DEM_DTCID_INVALID                       /* WDG_E_VALUE_COUNTER_FAILED */ \
   ,DEM_DTCID_INVALID                       /* WDG_E_WRITEVERIFY_FAILURE */ \
   ,DEM_DTCID_INVALID                       /* WDG_E_WRITE_REGISTER_FAILED */ \
};

#define DEM_MAP_DTCGROUPID_DTCID \
{ {0u,0u}\
 ,{DemConf_DemDTCClass_DemDTC_DTC_0x000001 , DemConf_DemDTCClass_DemDTC_DTC_0xe26200}	/* DEM_DTC_GROUP_ALL_DTCS */\
}

#if (DEM_CFG_EVCOMB == DEM_CFG_EVCOMB_DISABLED)

#define DEM_MAP_DTCID_EVENTID        \
const Dem_MapDtcIdToEventIdType  Dem_MapDtcIdToEventId[DEM_DTCID_ARRAYLENGTH] = \
{ \
   DEM_EVENTID_INVALID \
   ,DemConf_DemEventParameter_DTC_0x000001_Event /* DemDTC_DTC_0x000001 */ \
   ,DemConf_DemEventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event /* DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff */ \
   ,DemConf_DemEventParameter_DTC_0xc07388_Event /* DemDTC_DTC_0xc07388 */ \
   ,DemConf_DemEventParameter_DTC_0xc12687_Event /* DemDTC_DTC_0xc12687 */ \
   ,DemConf_DemEventParameter_DTC_0xc14587_Event /* DemDTC_DTC_0xc14587 */ \
   ,DemConf_DemEventParameter_DTC_0xc22a87_Event /* DemDTC_DTC_0xc22a87 */ \
   ,DemConf_DemEventParameter_DTC_0xc41487_Event /* DemDTC_DTC_0xc41487 */ \
   ,DemConf_DemEventParameter_DTC_0xd56217_Event /* DemDTC_DTC_0xd56217 */ \
   ,DemConf_DemEventParameter_DTC_0xd56316_Event /* DemDTC_DTC_0xd56316 */ \
   ,DemConf_DemEventParameter_DTC_0xe00141_Event /* DemDTC_DTC_0xe00141 */ \
   ,DemConf_DemEventParameter_DTC_0xe00142_Event /* DemDTC_DTC_0xe00142 */ \
   ,DemConf_DemEventParameter_DTC_0xe26200_Event /* DemDTC_DTC_0xe26200 */ \
};

#else

#define DEM_MAP_DTCID_EVENTID        \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0x000001[] = { \
   DemConf_DemEventParameter_DTC_0x000001_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff[] = { \
   DemConf_DemEventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xc07388[] = { \
   DemConf_DemEventParameter_DTC_0xc07388_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xc12687[] = { \
   DemConf_DemEventParameter_DTC_0xc12687_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xc14587[] = { \
   DemConf_DemEventParameter_DTC_0xc14587_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xc22a87[] = { \
   DemConf_DemEventParameter_DTC_0xc22a87_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xc41487[] = { \
   DemConf_DemEventParameter_DTC_0xc41487_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56217[] = { \
   DemConf_DemEventParameter_DTC_0xd56217_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56316[] = { \
   DemConf_DemEventParameter_DTC_0xd56316_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00141[] = { \
   DemConf_DemEventParameter_DTC_0xe00141_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00142[] = { \
   DemConf_DemEventParameter_DTC_0xe00142_Event \
}; \
 \
 \
static const  Dem_EventIdType  Dem_MapDtcIdToEventId_DemDTC_DTC_0xe26200[] = { \
   DemConf_DemEventParameter_DTC_0xe26200_Event \
}; \
 \
 \
 \
const Dem_MapDtcIdToEventIdType  Dem_MapDtcIdToEventId[DEM_DTCID_ARRAYLENGTH] = \
{ \
   { \
      NULL_PTR, \
      0u \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0x000001[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0x000001)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0x000001[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0x0d0400_CAN_Network_ETAS_BusOff[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xc07388[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc07388)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc07388[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xc12687[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc12687)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc12687[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xc14587[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc14587)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc14587[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xc22a87[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc22a87)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc22a87[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xc41487[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc41487)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xc41487[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56217[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56217)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56217[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56316[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56316)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xd56316[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00141[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00141)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00141[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00142[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00142)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe00142[0])) \
   } \
   ,{ \
      &Dem_MapDtcIdToEventId_DemDTC_DTC_0xe26200[0], \
      (uint16)(DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe26200)/DEM_SIZEOF_VAR(Dem_MapDtcIdToEventId_DemDTC_DTC_0xe26200[0])) \
   } \
};


#endif

#endif

