/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENVMAIN_H
#define DEM_CFG_ENVMAIN_H

/* min number of bytes required for storing any eventIds ExtendedData and one FreezeFrame (=> EvBuff) */
#define DEM_CFG_ENVMINSIZE_OF_RAWENVDATA  (16u  + 0u+ 0u) 

/* min number of bytes required for storing any eventIds ExtendedData and multiple FreezeFrame (=> EvMem) */
#define DEM_CFG_ENVMINSIZE_OF_MULTIPLE_RAWENVDATA  (28u + 0u + 0u)

/*min number of bytes required for OBD related data - This is generated for getting Offset for handling J1939 data*/
#define DEM_CFG_OFFSET_OBDRAWENVDATA         0u

#define DEM_CFG_ENVEVENTID2ENVDATA \
{ \
   { 0u,0u }                                                      /* DEM_EVENTID_INVALID */ \
   ,{  0u, 0u }                                                    /* ADC_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* CAN_E_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* CAN_E_UNINTENDED_INTERRUPT */ \
   ,{  0u, 0u }                                                    /* DIO_E_GET_CONTROL_FAILURE */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0x000001_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xc07388_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xc12687_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0X000001_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xc14587_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xc22a87_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xc41487_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xd56217_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xd56316_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xe00141_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xe00142_Event */ \
   ,{  DEM_EXTDATA_EXTENDEDDATACLASS_DTC_0XC22A87_PROPERTIES, DEM_FREEZEFRAME_FREEZEFRAME_DTC_0X000001_DIDSET }  /* DTC_0xe26200_Event */ \
   ,{  0u, 0u }                                                    /* ETH_E_ACCESS */ \
   ,{  0u, 0u }                                                    /* ETH_E_ALIGNMENT */ \
   ,{  0u, 0u }                                                    /* ETH_E_AVB_DMAC */ \
   ,{  0u, 0u }                                                    /* ETH_E_CRC */ \
   ,{  0u, 0u }                                                    /* ETH_E_DESC_DATA */ \
   ,{  0u, 0u }                                                    /* ETH_E_FRAME_RECEIVE */ \
   ,{  0u, 0u }                                                    /* ETH_E_INTERRUPT_CTL_CHECK */ \
   ,{  0u, 0u }                                                    /* ETH_E_LATECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_MODULE_STOP_CHECK */ \
   ,{  0u, 0u }                                                    /* ETH_E_MULTIPLECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_OPMODE */ \
   ,{  0u, 0u }                                                    /* ETH_E_OVERSIZEFRAME */ \
   ,{  0u, 0u }                                                    /* ETH_E_PAYLOAD_CRC */ \
   ,{  0u, 0u }                                                    /* ETH_E_REG_READ_VERIFY */ \
   ,{  0u, 0u }                                                    /* ETH_E_REG_WRITE_VERIFY */ \
   ,{  0u, 0u }                                                    /* ETH_E_RESIDUAL_BIT_FRAME */ \
   ,{  0u, 0u }                                                    /* ETH_E_RX_FRAMES_LOST */ \
   ,{  0u, 0u }                                                    /* ETH_E_SINGLECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_TIMEOUT */ \
   ,{  0u, 0u }                                                    /* ETH_E_TIMERINC_FAILED */ \
   ,{  0u, 0u }                                                    /* ETH_E_TRANSMIT_RETRY_OVER */ \
   ,{  0u, 0u }                                                    /* ETH_E_UNDERSIZEFRAME */ \
   ,{  0u, 0u }                                                    /* FLS_E_COMPARE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_ERASE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_GET_SEMAPHORE_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_READ_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_RELEASE_SEMAPHORE_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_UNEXPECTED_FLASH_ID */ \
   ,{  0u, 0u }                                                    /* FLS_E_WRITE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CHECK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CLOCK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_INTERNAL_BUS_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_READ_BACK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_WRITE_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_FUSE_MONITORING_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_DATA_TX_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_HARDWARE_ERROR */ \
   ,{  0u, 0u }                                                    /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_UNINTENDED_EXECUTION_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* WDG_E_DISABLE_REJECTED */ \
   ,{  0u, 0u }                                                    /* WDG_E_MODE_FAILED */ \
   ,{  0u, 0u }                                                    /* WDG_E_VALUE_COUNTER_FAILED */ \
   ,{  0u, 0u }                                                    /* WDG_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* WDG_E_WRITE_REGISTER_FAILED */ \
}

/*------         Freeze frame        ,         Expanded Freeze frame -------------*/
#define DEM_CFG_J1939_ENVEVENTID2ENVDATA \
{ \
   { 0u,0u }                                                      /* DEM_EVENTID_INVALID */ \
   ,{  0u, 0u }                                                    /* ADC_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* CAN_E_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* CAN_E_UNINTENDED_INTERRUPT */ \
   ,{  0u, 0u }                                                    /* DIO_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* DTC_0x000001_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xc07388_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xc12687_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xc14587_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xc22a87_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xc41487_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xd56217_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xd56316_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xe00141_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xe00142_Event */ \
   ,{  0u, 0u }                                                    /* DTC_0xe26200_Event */ \
   ,{  0u, 0u }                                                    /* ETH_E_ACCESS */ \
   ,{  0u, 0u }                                                    /* ETH_E_ALIGNMENT */ \
   ,{  0u, 0u }                                                    /* ETH_E_AVB_DMAC */ \
   ,{  0u, 0u }                                                    /* ETH_E_CRC */ \
   ,{  0u, 0u }                                                    /* ETH_E_DESC_DATA */ \
   ,{  0u, 0u }                                                    /* ETH_E_FRAME_RECEIVE */ \
   ,{  0u, 0u }                                                    /* ETH_E_INTERRUPT_CTL_CHECK */ \
   ,{  0u, 0u }                                                    /* ETH_E_LATECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_MODULE_STOP_CHECK */ \
   ,{  0u, 0u }                                                    /* ETH_E_MULTIPLECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_OPMODE */ \
   ,{  0u, 0u }                                                    /* ETH_E_OVERSIZEFRAME */ \
   ,{  0u, 0u }                                                    /* ETH_E_PAYLOAD_CRC */ \
   ,{  0u, 0u }                                                    /* ETH_E_REG_READ_VERIFY */ \
   ,{  0u, 0u }                                                    /* ETH_E_REG_WRITE_VERIFY */ \
   ,{  0u, 0u }                                                    /* ETH_E_RESIDUAL_BIT_FRAME */ \
   ,{  0u, 0u }                                                    /* ETH_E_RX_FRAMES_LOST */ \
   ,{  0u, 0u }                                                    /* ETH_E_SINGLECOLLISION */ \
   ,{  0u, 0u }                                                    /* ETH_E_TIMEOUT */ \
   ,{  0u, 0u }                                                    /* ETH_E_TIMERINC_FAILED */ \
   ,{  0u, 0u }                                                    /* ETH_E_TRANSMIT_RETRY_OVER */ \
   ,{  0u, 0u }                                                    /* ETH_E_UNDERSIZEFRAME */ \
   ,{  0u, 0u }                                                    /* FLS_E_COMPARE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_ERASE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_GET_SEMAPHORE_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_READ_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_RELEASE_SEMAPHORE_FAILURE */ \
   ,{  0u, 0u }                                                    /* FLS_E_UNEXPECTED_FLASH_ID */ \
   ,{  0u, 0u }                                                    /* FLS_E_WRITE_FAILED */ \
   ,{  0u, 0u }                                                    /* FLS_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,{  0u, 0u }                                                    /* GPT_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CHECK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CLOCK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_INTERNAL_BUS_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_READ_BACK_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* MCU_E_WRITE_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_FUSE_MONITORING_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_GET_CONTROL_FAILURE */ \
   ,{  0u, 0u }                                                    /* PORT_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_DATA_TX_TIMEOUT_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_HARDWARE_ERROR */ \
   ,{  0u, 0u }                                                    /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_UNINTENDED_EXECUTION_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */ \
   ,{  0u, 0u }                                                    /* SPI_E_WRITE_VERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* WDG_E_DISABLE_REJECTED */ \
   ,{  0u, 0u }                                                    /* WDG_E_MODE_FAILED */ \
   ,{  0u, 0u }                                                    /* WDG_E_VALUE_COUNTER_FAILED */ \
   ,{  0u, 0u }                                                    /* WDG_E_WRITEVERIFY_FAILURE */ \
   ,{  0u, 0u }                                                    /* WDG_E_WRITE_REGISTER_FAILED */ \
}

/* ------------------------------------------ */
/* DEM_CFG_ENVIRONMMENT_DATA_CAPTURE          */
/* ------------------------------------------ */

#define DEM_CFG_CAPTURE_SYNCHRONOUS_TO_REPORTING         1u
#define DEM_CFG_CAPTURE_ASYNCHRONOUS_TO_REPORTING        2u

#define DEM_CFG_ENVIRONMENT_DATA_CAPTURE                   DEM_CFG_CAPTURE_SYNCHRONOUS_TO_REPORTING

#endif

