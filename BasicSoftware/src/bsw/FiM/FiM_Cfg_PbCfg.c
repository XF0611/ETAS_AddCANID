

/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/


/*
***************************************************************************************************
* Includes
***************************************************************************************************
*/

#include "FiM_Cfg_PbCfg.h"
#include "FiM_Priv_Data.h"


#define FIM_START_SEC_CONST
#include "FiM_MemMap.h"

/*
***************************************************************************************************
* inhibit offset data structure
***************************************************************************************************
*/
static const FiM_NumOffsetEventType FiM_Cfg_NumOffsetEvent_auo[FIM_CFG_NUMBEROFDEMEVENTIDS+2] =
{
     0  ,                             /* Element 0 denotes the invalid event Id */
     0  ,                             /* Start Index of Event Id 1 (DemConf_DemEventParameter_ADC_E_WRITEVERIFY_FAILURE) */ 
     0  ,                             /* Start Index of Event Id 2 (DemConf_DemEventParameter_CAN_E_TIMEOUT_FAILURE) */ 
     0  ,                             /* Start Index of Event Id 3 (DemConf_DemEventParameter_CAN_E_UNINTENDED_INTERRUPT) */ 
     0  ,                             /* Start Index of Event Id 4 (DemConf_DemEventParameter_DIO_E_GET_CONTROL_FAILURE) */ 
     0  ,                             /* Start Index of Event Id 5 (DemConf_DemEventParameter_DTC_0x000001_Event) */ 
     0  ,                             /* Start Index of Event Id 6 (DemConf_DemEventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event) */ 
     1  ,                             /* Start Index of Event Id 7 (DemConf_DemEventParameter_DTC_0xc07388_Event) */ 
     1  ,                             /* Start Index of Event Id 8 (DemConf_DemEventParameter_DTC_0xc12687_Event) */ 
     1  ,                             /* Start Index of Event Id 9 (DemConf_DemEventParameter_DTC_0xc14587_Event) */ 
     1  ,                             /* Start Index of Event Id 10 (DemConf_DemEventParameter_DTC_0xc22a87_Event) */ 
     1  ,                             /* Start Index of Event Id 11 (DemConf_DemEventParameter_DTC_0xc41487_Event) */ 
     1  ,                             /* Start Index of Event Id 12 (DemConf_DemEventParameter_DTC_0xd56217_Event) */ 
     1  ,                             /* Start Index of Event Id 13 (DemConf_DemEventParameter_DTC_0xd56316_Event) */ 
     1  ,                             /* Start Index of Event Id 14 (DemConf_DemEventParameter_DTC_0xe00141_Event) */ 
     1  ,                             /* Start Index of Event Id 15 (DemConf_DemEventParameter_DTC_0xe00142_Event) */ 
     1  ,                             /* Start Index of Event Id 16 (DemConf_DemEventParameter_DTC_0xe26200_Event) */ 
     1  ,                             /* Start Index of Event Id 17 (DemConf_DemEventParameter_ETH_E_ACCESS) */ 
     1  ,                             /* Start Index of Event Id 18 (DemConf_DemEventParameter_ETH_E_ALIGNMENT) */ 
     1  ,                             /* Start Index of Event Id 19 (DemConf_DemEventParameter_ETH_E_AVB_DMAC) */ 
     1  ,                             /* Start Index of Event Id 20 (DemConf_DemEventParameter_ETH_E_CRC) */ 
     1  ,                             /* Start Index of Event Id 21 (DemConf_DemEventParameter_ETH_E_DESC_DATA) */ 
     1  ,                             /* Start Index of Event Id 22 (DemConf_DemEventParameter_ETH_E_FRAME_RECEIVE) */ 
     1  ,                             /* Start Index of Event Id 23 (DemConf_DemEventParameter_ETH_E_INTERRUPT_CTL_CHECK) */ 
     1  ,                             /* Start Index of Event Id 24 (DemConf_DemEventParameter_ETH_E_LATECOLLISION) */ 
     1  ,                             /* Start Index of Event Id 25 (DemConf_DemEventParameter_ETH_E_MODULE_STOP_CHECK) */ 
     1  ,                             /* Start Index of Event Id 26 (DemConf_DemEventParameter_ETH_E_MULTIPLECOLLISION) */ 
     1  ,                             /* Start Index of Event Id 27 (DemConf_DemEventParameter_ETH_E_OPMODE) */ 
     1  ,                             /* Start Index of Event Id 28 (DemConf_DemEventParameter_ETH_E_OVERSIZEFRAME) */ 
     1  ,                             /* Start Index of Event Id 29 (DemConf_DemEventParameter_ETH_E_PAYLOAD_CRC) */ 
     1  ,                             /* Start Index of Event Id 30 (DemConf_DemEventParameter_ETH_E_REG_READ_VERIFY) */ 
     1  ,                             /* Start Index of Event Id 31 (DemConf_DemEventParameter_ETH_E_REG_WRITE_VERIFY) */ 
     1  ,                             /* Start Index of Event Id 32 (DemConf_DemEventParameter_ETH_E_RESIDUAL_BIT_FRAME) */ 
     1  ,                             /* Start Index of Event Id 33 (DemConf_DemEventParameter_ETH_E_RX_FRAMES_LOST) */ 
     1  ,                             /* Start Index of Event Id 34 (DemConf_DemEventParameter_ETH_E_SINGLECOLLISION) */ 
     1  ,                             /* Start Index of Event Id 35 (DemConf_DemEventParameter_ETH_E_TIMEOUT) */ 
     1  ,                             /* Start Index of Event Id 36 (DemConf_DemEventParameter_ETH_E_TIMERINC_FAILED) */ 
     1  ,                             /* Start Index of Event Id 37 (DemConf_DemEventParameter_ETH_E_TRANSMIT_RETRY_OVER) */ 
     1  ,                             /* Start Index of Event Id 38 (DemConf_DemEventParameter_ETH_E_UNDERSIZEFRAME) */ 
     1  ,                             /* Start Index of Event Id 39 (DemConf_DemEventParameter_FLS_E_COMPARE_FAILED) */ 
     1  ,                             /* Start Index of Event Id 40 (DemConf_DemEventParameter_FLS_E_ERASE_FAILED) */ 
     1  ,                             /* Start Index of Event Id 41 (DemConf_DemEventParameter_FLS_E_GET_CONTROL_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 42 (DemConf_DemEventParameter_FLS_E_GET_SEMAPHORE_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 43 (DemConf_DemEventParameter_FLS_E_READ_FAILED) */ 
     1  ,                             /* Start Index of Event Id 44 (DemConf_DemEventParameter_FLS_E_RELEASE_SEMAPHORE_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 45 (DemConf_DemEventParameter_FLS_E_UNEXPECTED_FLASH_ID) */ 
     1  ,                             /* Start Index of Event Id 46 (DemConf_DemEventParameter_FLS_E_WRITE_FAILED) */ 
     1  ,                             /* Start Index of Event Id 47 (DemConf_DemEventParameter_FLS_E_WRITE_VERIFY_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 48 (DemConf_DemEventParameter_GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 49 (DemConf_DemEventParameter_GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 50 (DemConf_DemEventParameter_GPT_E_INTERRUPT_CONTROLLER_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 51 (DemConf_DemEventParameter_GPT_E_WRITEVERIFY_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 52 (DemConf_DemEventParameter_MCU_E_CHECK_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 53 (DemConf_DemEventParameter_MCU_E_CLOCK_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 54 (DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 55 (DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 56 (DemConf_DemEventParameter_MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 57 (DemConf_DemEventParameter_MCU_E_GET_CONTROL_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 58 (DemConf_DemEventParameter_MCU_E_INTERNAL_BUS_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 59 (DemConf_DemEventParameter_MCU_E_READ_BACK_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 60 (DemConf_DemEventParameter_MCU_E_TIMEOUT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 61 (DemConf_DemEventParameter_MCU_E_WRITE_TIMEOUT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 62 (DemConf_DemEventParameter_PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 63 (DemConf_DemEventParameter_PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 64 (DemConf_DemEventParameter_PORT_E_FUSE_MONITORING_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 65 (DemConf_DemEventParameter_PORT_E_GET_CONTROL_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 66 (DemConf_DemEventParameter_PORT_E_WRITE_VERIFY_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 67 (DemConf_DemEventParameter_SPI_E_DATA_TX_TIMEOUT_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 68 (DemConf_DemEventParameter_SPI_E_HARDWARE_ERROR) */ 
     1  ,                             /* Start Index of Event Id 69 (DemConf_DemEventParameter_SPI_E_INTERRUPT_CONTROLLER_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 70 (DemConf_DemEventParameter_SPI_E_UNINTENDED_EXECUTION_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 71 (DemConf_DemEventParameter_SPI_E_UNINTENDED_MODULE_STOP_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 72 (DemConf_DemEventParameter_SPI_E_WRITE_VERIFY_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 73 (DemConf_DemEventParameter_WDG_E_DISABLE_REJECTED) */ 
     1  ,                             /* Start Index of Event Id 74 (DemConf_DemEventParameter_WDG_E_MODE_FAILED) */ 
     1  ,                             /* Start Index of Event Id 75 (DemConf_DemEventParameter_WDG_E_VALUE_COUNTER_FAILED) */ 
     1  ,                             /* Start Index of Event Id 76 (DemConf_DemEventParameter_WDG_E_WRITEVERIFY_FAILURE) */ 
     1  ,                             /* Start Index of Event Id 77 (DemConf_DemEventParameter_WDG_E_WRITE_REGISTER_FAILED) */ 
     1                                /* Last Element denotes the invalid event Id */
};

static const FiM_NumOffsetComponentType FiM_Cfg_NumOffsetComponent_auo[FIM_CFG_NUMBEROFDEMCOMPONENTIDS+2] =
{
     0  ,                             /* Element 0 denotes the invalid component Id */

     0                                /* Last Element denotes the invalid component Id */
};


/*
***************************************************************************************************
* inhibit matrix - FID for events
***************************************************************************************************
*/
#define FIM_CFG_MAX_TOTAL_LINKS_EVENT_CALC    (1)
static const FiM_FunctionIdType FiM_CfgInhibitSourceMatrix_au16[FIM_CFG_MAX_TOTAL_LINKS_EVENT_CALC] =
{
   /* <----- Fim_FIds -----> */ 

   /* DemEvent 0: no FIds available */ 

   /* DemEvent 0: no FIds available */ 

   /* DemEvent 0: no FIds available */ 

   /* DemEvent 0: no FIds available */ 

   /* DemEvent 0: no FIds available */ 

   // DemConf_DemEventParameter_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event 0 - 0 

FiMConf_FiMFID_FiMFID

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 

   /* DemEvent 1: no FIds available */ 
};


/*
***************************************************************************************************
* inhibit matrix - mask for events
***************************************************************************************************
*/
static const FiM_InhibitionMaskType FiM_CfgInhibitMaskMatrix_au16[FIM_CFG_MAX_TOTAL_LINKS_EVENT_CALC] =
{
   /* <----- Fid->DemEvent Inhibit mask -----> */ 

   /* DemEvent 0: no inhibition mask available */ 

   /* DemEvent 0: no inhibition mask available */ 

   /* DemEvent 0: no inhibition mask available */ 

   /* DemEvent 0: no inhibition mask available */ 

   /* DemEvent 0: no inhibition mask available */ 

   // DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event 0 - 0 

FIM_CFG_LAST_FAILED

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 

   /* DemEvent 1: no inhibition mask available */ 
};


/*
***************************************************************************************************
* inhibit matrix - FID for components
***************************************************************************************************
*/
static const FiM_FunctionIdType FiM_CfgInhibitSourceMatrixComponent_au16[1] =
{
   FIM_CFG_INVALIDFID /* to have at least one entry in the array */
};


const FiM_ConfigType FiMConfigSet =
{
   FiM_Cfg_NumOffsetEvent_auo,
   FiM_CfgInhibitSourceMatrix_au16,
   FiM_CfgInhibitMaskMatrix_au16,
   FiM_Cfg_NumOffsetComponent_auo,
   FiM_CfgInhibitSourceMatrixComponent_au16
};

#define FIM_STOP_SEC_CONST
#include "FiM_MemMap.h"


