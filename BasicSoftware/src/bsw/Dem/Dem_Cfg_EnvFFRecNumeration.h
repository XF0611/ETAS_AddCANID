/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVFFRECNUMERATION_H
#define DEM_CFG_ENVFFRECNUMERATION_H

/* ---------------------------------------- */
/* DEM_CFG_FFRECNUM                         */
/* ---------------------------------------- */
#define DEM_CFG_FFRECNUM_CALCULATED   1
#define DEM_CFG_FFRECNUM_CONFIGURED   2
#define DEM_CFG_FFRECNUM  DEM_CFG_FFRECNUM_CONFIGURED

#define DEM_CFG_FFRECCLASS_NUMBEROF_FFRECCLASSES          3
#define DEM_CFG_FFRECCLASS_MAXNUMBEROF_FFFRECNUMS         2

#define DEM_CFG_FFRECNUMCLASSES \
{ \
   {0x1  ,0x2  } , /* DTC_0x000001_Properties_FFRecNumClass */ \
   {0x1  ,0x2  } , /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Properties_FFRecNumClass */ \
   {0x1  ,0x2  }   /* DTC_0xc22a87_Properties_FFRecNumClass */ \
}

#define DEM_CFG_ENVFFREC \
{ \
/*     RecNum    Trigger                       Update  */ \
     { 0,        DEM_TRIGGER_NONE,             FALSE } \
    ,{ 1,        DEM_TRIGGER_ON_CONFIRMED,     TRUE } /* DEM_FFREC_REC_SNAPSHOTREC_1 */ \
    ,{ 2,        DEM_TRIGGER_ON_CONFIRMED,     TRUE } /* DEM_FFREC_REC_SNAPSHOTREC_2 */ \
}

#define DEM_CFG_ENVFFREC_ARRAYLENGTH  (2u+1u)

#define DEM_FFRECNUMCLASS_DTC_0X000001_PROPERTIES_FFRECNUMCLASS   0
#define DEM_FFRECNUMCLASS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_PROPERTIES_FFRECNUMCLASS   1
#define DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS   2

#define DEM_CFG_ENVEVENTID2FFRECNUMCLASS \
{ \
     (0u                                      ) /* DEM_EVENTID_INVALID */ \
   , (0u                                      ) /* ADC_E_WRITEVERIFY_FAILURE */ \
   , (0u                                      ) /* CAN_E_TIMEOUT_FAILURE */ \
   , (0u                                      ) /* CAN_E_UNINTENDED_INTERRUPT */ \
   , (0u                                      ) /* DIO_E_GET_CONTROL_FAILURE */ \
   , (DEM_FFRECNUMCLASS_DTC_0X000001_PROPERTIES_FFRECNUMCLASS ) /* DTC_0x000001_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_PROPERTIES_FFRECNUMCLASS ) /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0X000001_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xc07388_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0X000001_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xc12687_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0X000001_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xc14587_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xc22a87_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xc41487_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xd56217_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xd56316_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xe00141_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xe00142_Event */ \
   , (DEM_FFRECNUMCLASS_DTC_0XC22A87_PROPERTIES_FFRECNUMCLASS ) /* DTC_0xe26200_Event */ \
   , (0u                                      ) /* ETH_E_ACCESS */ \
   , (0u                                      ) /* ETH_E_ALIGNMENT */ \
   , (0u                                      ) /* ETH_E_AVB_DMAC */ \
   , (0u                                      ) /* ETH_E_CRC */ \
   , (0u                                      ) /* ETH_E_DESC_DATA */ \
   , (0u                                      ) /* ETH_E_FRAME_RECEIVE */ \
   , (0u                                      ) /* ETH_E_INTERRUPT_CTL_CHECK */ \
   , (0u                                      ) /* ETH_E_LATECOLLISION */ \
   , (0u                                      ) /* ETH_E_MODULE_STOP_CHECK */ \
   , (0u                                      ) /* ETH_E_MULTIPLECOLLISION */ \
   , (0u                                      ) /* ETH_E_OPMODE */ \
   , (0u                                      ) /* ETH_E_OVERSIZEFRAME */ \
   , (0u                                      ) /* ETH_E_PAYLOAD_CRC */ \
   , (0u                                      ) /* ETH_E_REG_READ_VERIFY */ \
   , (0u                                      ) /* ETH_E_REG_WRITE_VERIFY */ \
   , (0u                                      ) /* ETH_E_RESIDUAL_BIT_FRAME */ \
   , (0u                                      ) /* ETH_E_RX_FRAMES_LOST */ \
   , (0u                                      ) /* ETH_E_SINGLECOLLISION */ \
   , (0u                                      ) /* ETH_E_TIMEOUT */ \
   , (0u                                      ) /* ETH_E_TIMERINC_FAILED */ \
   , (0u                                      ) /* ETH_E_TRANSMIT_RETRY_OVER */ \
   , (0u                                      ) /* ETH_E_UNDERSIZEFRAME */ \
   , (0u                                      ) /* FLS_E_COMPARE_FAILED */ \
   , (0u                                      ) /* FLS_E_ERASE_FAILED */ \
   , (0u                                      ) /* FLS_E_GET_CONTROL_FAILURE */ \
   , (0u                                      ) /* FLS_E_GET_SEMAPHORE_FAILURE */ \
   , (0u                                      ) /* FLS_E_READ_FAILED */ \
   , (0u                                      ) /* FLS_E_RELEASE_SEMAPHORE_FAILURE */ \
   , (0u                                      ) /* FLS_E_UNEXPECTED_FLASH_ID */ \
   , (0u                                      ) /* FLS_E_WRITE_FAILED */ \
   , (0u                                      ) /* FLS_E_WRITE_VERIFY_FAILURE */ \
   , (0u                                      ) /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   , (0u                                      ) /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   , (0u                                      ) /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */ \
   , (0u                                      ) /* GPT_E_WRITEVERIFY_FAILURE */ \
   , (0u                                      ) /* MCU_E_CHECK_FAILURE */ \
   , (0u                                      ) /* MCU_E_CLOCK_FAILURE */ \
   , (0u                                      ) /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   , (0u                                      ) /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   , (0u                                      ) /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */ \
   , (0u                                      ) /* MCU_E_GET_CONTROL_FAILURE */ \
   , (0u                                      ) /* MCU_E_INTERNAL_BUS_FAILURE */ \
   , (0u                                      ) /* MCU_E_READ_BACK_FAILURE */ \
   , (0u                                      ) /* MCU_E_TIMEOUT_FAILURE */ \
   , (0u                                      ) /* MCU_E_WRITE_TIMEOUT_FAILURE */ \
   , (0u                                      ) /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   , (0u                                      ) /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   , (0u                                      ) /* PORT_E_FUSE_MONITORING_FAILURE */ \
   , (0u                                      ) /* PORT_E_GET_CONTROL_FAILURE */ \
   , (0u                                      ) /* PORT_E_WRITE_VERIFY_FAILURE */ \
   , (0u                                      ) /* SPI_E_DATA_TX_TIMEOUT_FAILURE */ \
   , (0u                                      ) /* SPI_E_HARDWARE_ERROR */ \
   , (0u                                      ) /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */ \
   , (0u                                      ) /* SPI_E_UNINTENDED_EXECUTION_FAILURE */ \
   , (0u                                      ) /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */ \
   , (0u                                      ) /* SPI_E_WRITE_VERIFY_FAILURE */ \
   , (0u                                      ) /* WDG_E_DISABLE_REJECTED */ \
   , (0u                                      ) /* WDG_E_MODE_FAILED */ \
   , (0u                                      ) /* WDG_E_VALUE_COUNTER_FAILED */ \
   , (0u                                      ) /* WDG_E_WRITEVERIFY_FAILURE */ \
   , (0u                                      ) /* WDG_E_WRITE_REGISTER_FAILED */ \
}

#endif

