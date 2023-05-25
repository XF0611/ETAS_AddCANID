
/********************************************************************************************************************/
/*                                                                                                                  */
/* TOOL-GENERATED SOURCECODE, DO NOT CHANGE                                                                         */
/*                                                                                                                  */
/********************************************************************************************************************/

#include "Dem_Cfg_Events_DataStructures.h"

const Dem_EvtParam_8Type Dem_EvtParam_8[78] = {
/*MR12 RULE 11.9 VIOLATION: The first row is a invalid index and will never be used*/
{ 0 }, { 1u }, /* ADC_E_WRITEVERIFY_FAILURE */
{ 1u }, /* CAN_E_TIMEOUT_FAILURE */
{ 1u }, /* CAN_E_UNINTENDED_INTERRUPT */
{ 1u }, /* DIO_E_GET_CONTROL_FAILURE */
{ 3u }, /* DTC_0x000001_Event */
{ 40u }, /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */
{ 3u }, /* DTC_0xc07388_Event */
{ 3u }, /* DTC_0xc12687_Event */
{ 3u }, /* DTC_0xc14587_Event */
{ 3u }, /* DTC_0xc22a87_Event */
{ 3u }, /* DTC_0xc41487_Event */
{ 3u }, /* DTC_0xd56217_Event */
{ 3u }, /* DTC_0xd56316_Event */
{ 3u }, /* DTC_0xe00141_Event */
{ 3u }, /* DTC_0xe00142_Event */
{ 3u }, /* DTC_0xe26200_Event */
{ 1u }, /* ETH_E_ACCESS */
{ 1u }, /* ETH_E_ALIGNMENT */
{ 1u }, /* ETH_E_AVB_DMAC */
{ 1u }, /* ETH_E_CRC */
{ 1u }, /* ETH_E_DESC_DATA */
{ 1u }, /* ETH_E_FRAME_RECEIVE */
{ 1u }, /* ETH_E_INTERRUPT_CTL_CHECK */
{ 1u }, /* ETH_E_LATECOLLISION */
{ 1u }, /* ETH_E_MODULE_STOP_CHECK */
{ 1u }, /* ETH_E_MULTIPLECOLLISION */
{ 1u }, /* ETH_E_OPMODE */
{ 1u }, /* ETH_E_OVERSIZEFRAME */
{ 1u }, /* ETH_E_PAYLOAD_CRC */
{ 1u }, /* ETH_E_REG_READ_VERIFY */
{ 1u }, /* ETH_E_REG_WRITE_VERIFY */
{ 1u }, /* ETH_E_RESIDUAL_BIT_FRAME */
{ 1u }, /* ETH_E_RX_FRAMES_LOST */
{ 1u }, /* ETH_E_SINGLECOLLISION */
{ 1u }, /* ETH_E_TIMEOUT */
{ 1u }, /* ETH_E_TIMERINC_FAILED */
{ 1u }, /* ETH_E_TRANSMIT_RETRY_OVER */
{ 1u }, /* ETH_E_UNDERSIZEFRAME */
{ 1u }, /* FLS_E_COMPARE_FAILED */
{ 1u }, /* FLS_E_ERASE_FAILED */
{ 1u }, /* FLS_E_GET_CONTROL_FAILURE */
{ 1u }, /* FLS_E_GET_SEMAPHORE_FAILURE */
{ 1u }, /* FLS_E_READ_FAILED */
{ 1u }, /* FLS_E_RELEASE_SEMAPHORE_FAILURE */
{ 1u }, /* FLS_E_UNEXPECTED_FLASH_ID */
{ 1u }, /* FLS_E_WRITE_FAILED */
{ 1u }, /* FLS_E_WRITE_VERIFY_FAILURE */
{ 1u }, /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
{ 1u }, /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
{ 1u }, /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */
{ 1u }, /* GPT_E_WRITEVERIFY_FAILURE */
{ 1u }, /* MCU_E_CHECK_FAILURE */
{ 1u }, /* MCU_E_CLOCK_FAILURE */
{ 1u }, /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
{ 1u }, /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
{ 1u }, /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */
{ 1u }, /* MCU_E_GET_CONTROL_FAILURE */
{ 1u }, /* MCU_E_INTERNAL_BUS_FAILURE */
{ 1u }, /* MCU_E_READ_BACK_FAILURE */
{ 1u }, /* MCU_E_TIMEOUT_FAILURE */
{ 1u }, /* MCU_E_WRITE_TIMEOUT_FAILURE */
{ 1u }, /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
{ 1u }, /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
{ 1u }, /* PORT_E_FUSE_MONITORING_FAILURE */
{ 1u }, /* PORT_E_GET_CONTROL_FAILURE */
{ 1u }, /* PORT_E_WRITE_VERIFY_FAILURE */
{ 1u }, /* SPI_E_DATA_TX_TIMEOUT_FAILURE */
{ 1u }, /* SPI_E_HARDWARE_ERROR */
{ 1u }, /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */
{ 1u }, /* SPI_E_UNINTENDED_EXECUTION_FAILURE */
{ 1u }, /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */
{ 1u }, /* SPI_E_WRITE_VERIFY_FAILURE */
{ 1u }, /* WDG_E_DISABLE_REJECTED */
{ 1u }, /* WDG_E_MODE_FAILED */
{ 1u }, /* WDG_E_VALUE_COUNTER_FAILED */
{ 1u }, /* WDG_E_WRITEVERIFY_FAILURE */
{ 1u } /* WDG_E_WRITE_REGISTER_FAILED */
};

const Dem_EvtParam_32Type Dem_EvtParam_32[78] =
		{
		/*MR12 RULE 11.9 VIOLATION: The first row is a invalid index and will never be used*/
		{ 0, 0 }, { NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
				+ ((uint32)(DEM_RECOV & 0x1u) << 1) + ((uint32)(0u & 0x1u) << 2)
				+ ((uint32)(0u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
				+ ((uint32)(0u & 0x1u) << 5) + ((uint32)(0u & 0x1u) << 6)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 7)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 8)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 9) + ((uint32)(1u & 0x1u) << 10)
				+ ((uint32)(0u & 0x1u) << 11) + ((uint32)(0u & 0xFu) << 12)
				+ ((uint32)(0u & 0x1u) << 16) + ((uint32)(1u & 0x1u) << 17)
				+ ((uint32)(255u & 0x7u) << 18) + ((uint32)(0u & 0x3u) << 21)
				+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
				+ ((uint32)(0u & 0x1u) << 24) + ((uint32)(0u & 0x1u) << 25) }, /* ADC_E_WRITEVERIFY_FAILURE */
		{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
				+ ((uint32)(DEM_RECOV & 0x1u) << 1) + ((uint32)(0u & 0x1u) << 2)
				+ ((uint32)(0u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
				+ ((uint32)(0u & 0x1u) << 5) + ((uint32)(0u & 0x1u) << 6)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 7)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 8)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 9) + ((uint32)(1u & 0x1u) << 10)
				+ ((uint32)(0u & 0x1u) << 11) + ((uint32)(0u & 0xFu) << 12)
				+ ((uint32)(0u & 0x1u) << 16) + ((uint32)(1u & 0x1u) << 17)
				+ ((uint32)(255u & 0x7u) << 18) + ((uint32)(0u & 0x3u) << 21)
				+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
				+ ((uint32)(0u & 0x1u) << 24) + ((uint32)(0u & 0x1u) << 25) }, /* CAN_E_TIMEOUT_FAILURE */
		{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
				+ ((uint32)(DEM_RECOV & 0x1u) << 1) + ((uint32)(0u & 0x1u) << 2)
				+ ((uint32)(0u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
				+ ((uint32)(0u & 0x1u) << 5) + ((uint32)(0u & 0x1u) << 6)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 7)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 8)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 9) + ((uint32)(1u & 0x1u) << 10)
				+ ((uint32)(0u & 0x1u) << 11) + ((uint32)(0u & 0xFu) << 12)
				+ ((uint32)(0u & 0x1u) << 16) + ((uint32)(1u & 0x1u) << 17)
				+ ((uint32)(255u & 0x7u) << 18) + ((uint32)(0u & 0x3u) << 21)
				+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
				+ ((uint32)(0u & 0x1u) << 24) + ((uint32)(0u & 0x1u) << 25) }, /* CAN_E_UNINTENDED_INTERRUPT */
		{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
				+ ((uint32)(DEM_RECOV & 0x1u) << 1) + ((uint32)(0u & 0x1u) << 2)
				+ ((uint32)(0u & 0x1u) << 3) + ((uint32)(0u & 0x1u) << 4)
				+ ((uint32)(0u & 0x1u) << 5) + ((uint32)(0u & 0x1u) << 6)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 7)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 8)
				+ ((uint32)(
						DemConf_DemOperationCycle_DemOpCycle_DEM_POWER & 0x1u)
						<< 9) + ((uint32)(1u & 0x1u) << 10)
				+ ((uint32)(0u & 0x1u) << 11) + ((uint32)(0u & 0xFu) << 12)
				+ ((uint32)(0u & 0x1u) << 16) + ((uint32)(1u & 0x1u) << 17)
				+ ((uint32)(255u & 0x7u) << 18) + ((uint32)(0u & 0x3u) << 21)
				+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
				+ ((uint32)(0u & 0x1u) << 24) + ((uint32)(0u & 0x1u) << 25) }, /* DIO_E_GET_CONTROL_FAILURE */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0x000001_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0X000001_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(2u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0x000001_Event */
				{
						&Rte_Call_Dem_CBClrEvt_DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0X0D0400_CAN_NETWORK_ETAS_BUSOFF_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(4u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0x0d0400_CAN_Network_ETAS_BusOff_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xc07388_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XC07388_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(2u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xc07388_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xc12687_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XC12687_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(2u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xc12687_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xc14587_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XC14587_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(2u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xc14587_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xc22a87_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XC22A87_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xc22a87_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xc41487_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XC41487_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xc41487_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xd56217_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XD56217_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xd56217_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xd56316_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XD56316_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xd56316_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xe00141_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XE00141_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xe00141_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xe00142_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XE00142_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xe00142_Event */
				{ &Rte_Call_Dem_CBClrEvt_DTC_0xe26200_Event_ClearEventAllowed,
						((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
								+ ((uint32)(DEM_RECOV & 0x1u) << 1)
								+ ((uint32)(1u & 0x1u) << 2)
								+ ((uint32)(0u & 0x1u) << 3)
								+ ((uint32)(0u & 0x1u) << 4)
								+ ((uint32)(1u & 0x1u) << 5)
								+ ((uint32)(1u & 0x1u) << 6)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 7)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 8)
								+ ((uint32)(
										DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
												& 0x1u) << 9)
								+ ((uint32)(0u & 0x1u) << 10)
								+ ((uint32)(
										DEM_DEBMETH_IDX_ARCTRBASECLASS & 0x1u)
										<< 11)
								+ ((uint32)(
										DEM_DEBPARAM_IDX_DTC_0XE26200_EVENT
												& 0xFu) << 12)
								+ ((uint32)(0u & 0x1u) << 16)
								+ ((uint32)(1u & 0x1u) << 17)
								+ ((uint32)(1u & 0x7u) << 18)
								+ ((uint32)(2u & 0x3u) << 21)
								+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u)
										<< 23) + ((uint32)(0u & 0x1u) << 24)
								+ ((uint32)(0u & 0x1u) << 25) }, /* DTC_0xe26200_Event */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_ACCESS */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_ALIGNMENT */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_AVB_DMAC */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_CRC */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_DESC_DATA */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_FRAME_RECEIVE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_INTERRUPT_CTL_CHECK */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_LATECOLLISION */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_MODULE_STOP_CHECK */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_MULTIPLECOLLISION */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_OPMODE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_OVERSIZEFRAME */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_PAYLOAD_CRC */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_REG_READ_VERIFY */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_REG_WRITE_VERIFY */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_RESIDUAL_BIT_FRAME */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_RX_FRAMES_LOST */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_SINGLECOLLISION */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_TIMEOUT */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_TIMERINC_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_TRANSMIT_RETRY_OVER */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* ETH_E_UNDERSIZEFRAME */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_COMPARE_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_ERASE_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_GET_CONTROL_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_GET_SEMAPHORE_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_READ_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_RELEASE_SEMAPHORE_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_UNEXPECTED_FLASH_ID */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_WRITE_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* FLS_E_WRITE_VERIFY_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* GPT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* GPT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* GPT_E_INTERRUPT_CONTROLLER_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* GPT_E_WRITEVERIFY_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_CHECK_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_CLOCK_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_CONFIGURATION_CHECK_TRANSITION_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_GET_CONTROL_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_INTERNAL_BUS_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_READ_BACK_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_TIMEOUT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* MCU_E_WRITE_TIMEOUT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* PORT_E_CONFIGURATION_CHECK_PERMANENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* PORT_E_CONFIGURATION_CHECK_TRANSIENT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* PORT_E_FUSE_MONITORING_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* PORT_E_GET_CONTROL_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* PORT_E_WRITE_VERIFY_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_DATA_TX_TIMEOUT_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_HARDWARE_ERROR */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_INTERRUPT_CONTROLLER_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_UNINTENDED_EXECUTION_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_UNINTENDED_MODULE_STOP_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* SPI_E_WRITE_VERIFY_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* WDG_E_DISABLE_REJECTED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* WDG_E_MODE_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* WDG_E_VALUE_COUNTER_FAILED */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) }, /* WDG_E_WRITEVERIFY_FAILURE */
				{ NULL_PTR, ((uint32)(DEM_DEBOUNCE_FREEZE & 0x1u) << 0)
						+ ((uint32)(DEM_RECOV & 0x1u) << 1)
						+ ((uint32)(0u & 0x1u) << 2)
						+ ((uint32)(0u & 0x1u) << 3)
						+ ((uint32)(0u & 0x1u) << 4)
						+ ((uint32)(0u & 0x1u) << 5)
						+ ((uint32)(0u & 0x1u) << 6)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 7)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 8)
						+ ((uint32)(
								DemConf_DemOperationCycle_DemOpCycle_DEM_POWER
										& 0x1u) << 9)
						+ ((uint32)(1u & 0x1u) << 10)
						+ ((uint32)(0u & 0x1u) << 11)
						+ ((uint32)(0u & 0xFu) << 12)
						+ ((uint32)(0u & 0x1u) << 16)
						+ ((uint32)(1u & 0x1u) << 17)
						+ ((uint32)(255u & 0x7u) << 18)
						+ ((uint32)(0u & 0x3u) << 21)
						+ ((uint32)(DEM_NO_STATUS_BYTE_CHANGE & 0x1u) << 23)
						+ ((uint32)(0u & 0x1u) << 24)
						+ ((uint32)(0u & 0x1u) << 25) } /* WDG_E_WRITE_REGISTER_FAILED */
		};

