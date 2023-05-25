/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#ifndef DEM_CFG_ENABLECONDITION_H
#define DEM_CFG_ENABLECONDITION_H

#include "Std_Types.h"

#define DEM_CFG_ENABLECONDITION_ON    STD_ON
#define DEM_CFG_ENABLECONDITION_OFF   STD_OFF
#define DEM_CFG_ENABLECONDITION       DEM_CFG_ENABLECONDITION_OFF

#define DEM_ENABLECONDITION_COUNT         0u
#define DEM_ENABLECONDITION_ARRAYLENGTH   (DEM_ENABLECONDITION_COUNT)

/* define type depends on projectspecific number of storageconditions */
/* if no storage conditions are support use uint8 to allow empty inline functions */
#if (DEM_ENABLECONDITION_ARRAYLENGTH <= 8) \
	|| (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_OFF)
typedef uint8 Dem_EnCoList;
#elif (DEM_ENABLECONDITION_ARRAYLENGTH <= 16)
typedef uint16 Dem_EnCoList;
#elif (DEM_ENABLECONDITION_ARRAYLENGTH <= 32)
typedef uint32 Dem_EnCoList;
#else
#error DEM currently only supports up to 32 EnableConditions
#endif

#if (DEM_CFG_ENABLECONDITION == DEM_CFG_ENABLECONDITION_ON)

#define DEM_ENCO_ADC_E_WRITEVERIFY_FAILURE      (0u)
#define DEM_ENCO_CAN_E_TIMEOUT_FAILURE          (0u)
#define DEM_ENCO_CAN_E_UNINTENDED_INTERRUPT     (0u)
#define DEM_ENCO_DIO_E_GET_CONTROL_FAILURE      (0u)
#define DEM_ENCO_DTC_0x000001_Event             (0u)
#define DEM_ENCO_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event  (0u)
#define DEM_ENCO_DTC_0xc07388_Event             (0u)
#define DEM_ENCO_DTC_0xc12687_Event             (0u)
#define DEM_ENCO_DTC_0xc14587_Event             (0u)
#define DEM_ENCO_DTC_0xc22a87_Event             (0u)
#define DEM_ENCO_DTC_0xc41487_Event             (0u)
#define DEM_ENCO_DTC_0xd56217_Event             (0u)
#define DEM_ENCO_DTC_0xd56316_Event             (0u)
#define DEM_ENCO_DTC_0xe00141_Event             (0u)
#define DEM_ENCO_DTC_0xe00142_Event             (0u)
#define DEM_ENCO_DTC_0xe26200_Event             (0u)
#define DEM_ENCO_ETH_E_ACCESS                   (0u)
#define DEM_ENCO_ETH_E_ALIGNMENT                (0u)
#define DEM_ENCO_ETH_E_AVB_DMAC                 (0u)
#define DEM_ENCO_ETH_E_CRC                      (0u)
#define DEM_ENCO_ETH_E_DESC_DATA                (0u)
#define DEM_ENCO_ETH_E_FRAME_RECEIVE            (0u)
#define DEM_ENCO_ETH_E_INTERRUPT_CTL_CHECK      (0u)
#define DEM_ENCO_ETH_E_LATECOLLISION            (0u)
#define DEM_ENCO_ETH_E_MODULE_STOP_CHECK        (0u)
#define DEM_ENCO_ETH_E_MULTIPLECOLLISION        (0u)
#define DEM_ENCO_ETH_E_OPMODE                   (0u)
#define DEM_ENCO_ETH_E_OVERSIZEFRAME            (0u)
#define DEM_ENCO_ETH_E_PAYLOAD_CRC              (0u)
#define DEM_ENCO_ETH_E_REG_READ_VERIFY          (0u)
#define DEM_ENCO_ETH_E_REG_WRITE_VERIFY         (0u)
#define DEM_ENCO_ETH_E_RESIDUAL_BIT_FRAME       (0u)
#define DEM_ENCO_ETH_E_RX_FRAMES_LOST           (0u)
#define DEM_ENCO_ETH_E_SINGLECOLLISION          (0u)
#define DEM_ENCO_ETH_E_TIMEOUT                  (0u)
#define DEM_ENCO_ETH_E_TIMERINC_FAILED          (0u)
#define DEM_ENCO_ETH_E_TRANSMIT_RETRY_OVER      (0u)
#define DEM_ENCO_ETH_E_UNDERSIZEFRAME           (0u)
#define DEM_ENCO_FLS_E_COMPARE_FAILED           (0u)
#define DEM_ENCO_FLS_E_ERASE_FAILED             (0u)
#define DEM_ENCO_FLS_E_GET_CONTROL_FAILURE      (0u)
#define DEM_ENCO_FLS_E_GET_SEMAPHORE_FAILURE    (0u)
#define DEM_ENCO_FLS_E_READ_FAILED              (0u)
#define DEM_ENCO_FLS_E_RELEASE_SEMAPHORE_FAILURE  (0u)
#define DEM_ENCO_FLS_E_UNEXPECTED_FLASH_ID      (0u)
#define DEM_ENCO_FLS_E_WRITE_FAILED             (0u)
#define DEM_ENCO_FLS_E_WRITE_VERIFY_FAILURE     (0u)
#define DEM_ENCO_GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  (0u)
#define DEM_ENCO_GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  (0u)
#define DEM_ENCO_GPT_E_INTERRUPT_CONTROLLER_FAILURE  (0u)
#define DEM_ENCO_GPT_E_WRITEVERIFY_FAILURE      (0u)
#define DEM_ENCO_MCU_E_CHECK_FAILURE            (0u)
#define DEM_ENCO_MCU_E_CLOCK_FAILURE            (0u)
#define DEM_ENCO_MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  (0u)
#define DEM_ENCO_MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  (0u)
#define DEM_ENCO_MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE  (0u)
#define DEM_ENCO_MCU_E_GET_CONTROL_FAILURE      (0u)
#define DEM_ENCO_MCU_E_INTERNAL_BUS_FAILURE     (0u)
#define DEM_ENCO_MCU_E_READ_BACK_FAILURE        (0u)
#define DEM_ENCO_MCU_E_TIMEOUT_FAILURE          (0u)
#define DEM_ENCO_MCU_E_WRITE_TIMEOUT_FAILURE    (0u)
#define DEM_ENCO_PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE  (0u)
#define DEM_ENCO_PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE  (0u)
#define DEM_ENCO_PORT_E_FUSE_MONITORING_FAILURE  (0u)
#define DEM_ENCO_PORT_E_GET_CONTROL_FAILURE     (0u)
#define DEM_ENCO_PORT_E_WRITE_VERIFY_FAILURE    (0u)
#define DEM_ENCO_SPI_E_DATA_TX_TIMEOUT_FAILURE  (0u)
#define DEM_ENCO_SPI_E_HARDWARE_ERROR           (0u)
#define DEM_ENCO_SPI_E_INTERRUPT_CONTROLLER_FAILURE  (0u)
#define DEM_ENCO_SPI_E_UNINTENDED_EXECUTION_FAILURE  (0u)
#define DEM_ENCO_SPI_E_UNINTENDED_MODULE_STOP_FAILURE  (0u)
#define DEM_ENCO_SPI_E_WRITE_VERIFY_FAILURE     (0u)
#define DEM_ENCO_WDG_E_DISABLE_REJECTED         (0u)
#define DEM_ENCO_WDG_E_MODE_FAILED              (0u)
#define DEM_ENCO_WDG_E_VALUE_COUNTER_FAILED     (0u)
#define DEM_ENCO_WDG_E_WRITEVERIFY_FAILURE      (0u)
#define DEM_ENCO_WDG_E_WRITE_REGISTER_FAILED    (0u)

#define DEM_CFG_ENCO_INITIALSTATE    (   0u \
                                      )

#endif

#endif

