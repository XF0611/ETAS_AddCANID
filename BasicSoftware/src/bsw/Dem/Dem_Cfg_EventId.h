/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_EVENTID_H
#define DEM_CFG_EVENTID_H

#define DEM_EVENTID_INVALID                                          0u
#define DEM_EVENTID_COUNT         77u
#define DEM_EVENTID_ARRAYLENGTH   (DEM_EVENTID_COUNT+1u)

#define DemConf_DemEventParameter_ADC_E_WRITEVERIFY_FAILURE          1u
#define DemConf_DemEventParameter_CAN_E_TIMEOUT_FAILURE              2u
#define DemConf_DemEventParameter_CAN_E_UNINTENDED_INTERRUPT         3u
#define DemConf_DemEventParameter_DIO_E_GET_CONTROL_FAILURE          4u
#define DemConf_DemEventParameter_DTC_0x000001_Event                 5u
#define DemConf_DemEventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event  6u
#define DemConf_DemEventParameter_DTC_0xc07388_Event                 7u
#define DemConf_DemEventParameter_DTC_0xc12687_Event                 8u
#define DemConf_DemEventParameter_DTC_0xc14587_Event                 9u
#define DemConf_DemEventParameter_DTC_0xc22a87_Event                 10u
#define DemConf_DemEventParameter_DTC_0xc41487_Event                 11u
#define DemConf_DemEventParameter_DTC_0xd56217_Event                 12u
#define DemConf_DemEventParameter_DTC_0xd56316_Event                 13u
#define DemConf_DemEventParameter_DTC_0xe00141_Event                 14u
#define DemConf_DemEventParameter_DTC_0xe00142_Event                 15u
#define DemConf_DemEventParameter_DTC_0xe26200_Event                 16u
#define DemConf_DemEventParameter_ETH_E_ACCESS                       17u
#define DemConf_DemEventParameter_ETH_E_ALIGNMENT                    18u
#define DemConf_DemEventParameter_ETH_E_AVB_DMAC                     19u
#define DemConf_DemEventParameter_ETH_E_CRC                          20u
#define DemConf_DemEventParameter_ETH_E_DESC_DATA                    21u
#define DemConf_DemEventParameter_ETH_E_FRAME_RECEIVE                22u
#define DemConf_DemEventParameter_ETH_E_INTERRUPT_CTL_CHECK          23u
#define DemConf_DemEventParameter_ETH_E_LATECOLLISION                24u
#define DemConf_DemEventParameter_ETH_E_MODULE_STOP_CHECK            25u
#define DemConf_DemEventParameter_ETH_E_MULTIPLECOLLISION            26u
#define DemConf_DemEventParameter_ETH_E_OPMODE                       27u
#define DemConf_DemEventParameter_ETH_E_OVERSIZEFRAME                28u
#define DemConf_DemEventParameter_ETH_E_PAYLOAD_CRC                  29u
#define DemConf_DemEventParameter_ETH_E_REG_READ_VERIFY              30u
#define DemConf_DemEventParameter_ETH_E_REG_WRITE_VERIFY             31u
#define DemConf_DemEventParameter_ETH_E_RESIDUAL_BIT_FRAME           32u
#define DemConf_DemEventParameter_ETH_E_RX_FRAMES_LOST               33u
#define DemConf_DemEventParameter_ETH_E_SINGLECOLLISION              34u
#define DemConf_DemEventParameter_ETH_E_TIMEOUT                      35u
#define DemConf_DemEventParameter_ETH_E_TIMERINC_FAILED              36u
#define DemConf_DemEventParameter_ETH_E_TRANSMIT_RETRY_OVER          37u
#define DemConf_DemEventParameter_ETH_E_UNDERSIZEFRAME               38u
#define DemConf_DemEventParameter_FLS_E_COMPARE_FAILED               39u
#define DemConf_DemEventParameter_FLS_E_ERASE_FAILED                 40u
#define DemConf_DemEventParameter_FLS_E_GET_CONTROL_FAILURE          41u
#define DemConf_DemEventParameter_FLS_E_GET_SEMAPHORE_FAILURE        42u
#define DemConf_DemEventParameter_FLS_E_READ_FAILED                  43u
#define DemConf_DemEventParameter_FLS_E_RELEASE_SEMAPHORE_FAILURE    44u
#define DemConf_DemEventParameter_FLS_E_UNEXPECTED_FLASH_ID          45u
#define DemConf_DemEventParameter_FLS_E_WRITE_FAILED                 46u
#define DemConf_DemEventParameter_FLS_E_WRITE_VERIFY_FAILURE         47u
#define DemConf_DemEventParameter_GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  48u
#define DemConf_DemEventParameter_GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  49u
#define DemConf_DemEventParameter_GPT_E_INTERRUPT_CONTROLLER_FAILURE  50u
#define DemConf_DemEventParameter_GPT_E_WRITEVERIFY_FAILURE          51u
#define DemConf_DemEventParameter_MCU_E_CHECK_FAILURE                52u
#define DemConf_DemEventParameter_MCU_E_CLOCK_FAILURE                53u
#define DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  54u
#define DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  55u
#define DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE  56u
#define DemConf_DemEventParameter_MCU_E_GET_CONTROL_FAILURE          57u
#define DemConf_DemEventParameter_MCU_E_INTERNAL_BUS_FAILURE         58u
#define DemConf_DemEventParameter_MCU_E_READ_BACK_FAILURE            59u
#define DemConf_DemEventParameter_MCU_E_TIMEOUT_FAILURE              60u
#define DemConf_DemEventParameter_MCU_E_WRITE_TIMEOUT_FAILURE        61u
#define DemConf_DemEventParameter_PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  62u
#define DemConf_DemEventParameter_PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  63u
#define DemConf_DemEventParameter_PORT_E_FUSE_MONITORING_FAILURE     64u
#define DemConf_DemEventParameter_PORT_E_GET_CONTROL_FAILURE         65u
#define DemConf_DemEventParameter_PORT_E_WRITE_VERIFY_FAILURE        66u
#define DemConf_DemEventParameter_SPI_E_DATA_TX_TIMEOUT_FAILURE      67u
#define DemConf_DemEventParameter_SPI_E_HARDWARE_ERROR               68u
#define DemConf_DemEventParameter_SPI_E_INTERRUPT_CONTROLLER_FAILURE  69u
#define DemConf_DemEventParameter_SPI_E_UNINTENDED_EXECUTION_FAILURE  70u
#define DemConf_DemEventParameter_SPI_E_UNINTENDED_MODULE_STOP_FAILURE  71u
#define DemConf_DemEventParameter_SPI_E_WRITE_VERIFY_FAILURE         72u
#define DemConf_DemEventParameter_WDG_E_DISABLE_REJECTED             73u
#define DemConf_DemEventParameter_WDG_E_MODE_FAILED                  74u
#define DemConf_DemEventParameter_WDG_E_VALUE_COUNTER_FAILED         75u
#define DemConf_DemEventParameter_WDG_E_WRITEVERIFY_FAILURE          76u
#define DemConf_DemEventParameter_WDG_E_WRITE_REGISTER_FAILED        77u

#endif

