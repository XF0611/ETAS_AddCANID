/*============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc.
*   All Rights Reserved.
=============================================================================*/
/******************************************************************************
*                          Includes
******************************************************************************/
#include "Platform_Types.h"
#include "Std_Types.h"
#include "Can.h"
#include "Can_Alps.h"
#include "Os.h"
#include "Can_Alps_RegDef.h"

#include "CanIf_Cbk.h"
#include "SchM_Can.h"
#if(CAN_DEV_ERROR_DETECT == STD_ON)
#endif
#include "Mcu.h"


/******************************************************************************
*                           GLOBAL DECLARATIONS
******************************************************************************/
/*Getting base address of the module */
#define CAN_START_SEC_VAR_INIT_PTR
static Can_Reg_t *Can_Hw_Reg[] = {(Can_Reg_t *) CAN0_BASE_ADDRESS, (Can_Reg_t *) CAN1_BASE_ADDRESS};
#define CAN_STOP_SEC_VAR_INIT_PTR

#define CAN_START_SEC_VAR_INIT_UNSPECIFIED
extern CanUnitType CanUnitInfo[CAN_ALPS_CTRL_CONFIG_COUNT];
extern Can_GlobalType Can_Global;
static uint8 CANIDStd_Index[CAN_ALPS_CTRL_CONFIG_COUNT][CANID_STD_NUM];
static uint8 CANIDExt_Index[CAN_ALPS_CTRL_CONFIG_COUNT][CANID_EXT_NUM];
static Can_Fifo_PendType Fifo_Pend[CAN_ALPS_CTRL_CONFIG_COUNT];
static uint32 rx_isr_free_counter[CAN_ALPS_CTRL_CONFIG_COUNT];
#define CAN_STOP_SEC_VAR_INIT_UNSPECIFIED
/******************************************************************************
*                         Private Function Declaration
******************************************************************************/
#define CAN_START_SEC_CODE
static uint32 Can_lclGetBaseAddress(uint8 controllerId);
static void Can_lclReset( uint8 ControllerId);
static void Can_lclConfigControllerMode(uint8 ControllerId, const Can_Mode ControllerMode);
static void Can_lclReinitialize(uint8 Controller);
static uint8 Can_lclGetDataFieldLength(uint8 Length);
static uint8 Can_lclGetPduLength(uint8 Length);
static uint8 Can_lclGetDataWordLength(uint8 Length);
static void Can_lclIsrBusoff(uint8 ControllerId);
static void Can_lclIsrTx(uint8 ControllerId);
static void Can_lclIsrRx(uint8 ControllerId);
static Std_ReturnType Can_lclGetPduHandleData(uint8 Controller, PduIdType *PduId);
static Std_ReturnType Can_lclStorePduHandleData(uint8 Controller, PduIdType PduId);
static void Can_lclResetPduHandleData(CanUnitType* UnitPtr);
static void Can_lclChangeEndianess(uint8 Idx, uint8 *Source, uint8 *Destination);


/******************************************************************************
*                         Public Functions
******************************************************************************/

/******************************************************************************
*  Service name    : Can_Hal_Init
*  Syntax          : void Can_Hal_Init(const Can_ConfigType *ConfigPtr)
*  Parameters (in) : ConfigPtr - Pointer to configuration set
*  Parameters (out): None
*  Return value    : None
*  Description     : Service to perform CAN module initialization
******************************************************************************/
void Can_Hal_Init(const Can_ConfigType *ConfigPtr) {
	CanUnitType *UnitPtr = NULL_PTR;
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;
	uint8 HwIndex = 0;
	uint32 StdIdFilterEle = 0, ExtIdFilterEle0 = 0, ExtIdFilterEle1 = 0, TempVal = 0;
	uint32 *StdIdFilterAddr = NULL_PTR, *ExtIdFilterAddr = NULL_PTR, *RxBuffAddr = NULL_PTR, *RxFifoAddr = NULL;
	uint8 i = 0,j = 0,idx1 = 0u , idx2 = 0u;

	/* Save configuration info*/
	Can_Global.config = ConfigPtr;

	/* loop for all hardware objects configured for transmission and store controller and HOH reference*/
	CtrlCfgPtr  = Can_Global.config->CanConfigSet->CanController;
	if(CtrlCfgPtr != NULL_PTR)
	{
		for(i = 0x0; i < CtrlCfgPtr->CanHoh->CanHwObjectCount; i++)
		{
			if(CtrlCfgPtr->CanHoh[i].CanObjectType == TRANSMIT)
			{
				Can_Global.CanHTHMap[j].CanControllerRef = CtrlCfgPtr->CanHoh[i].CanControllerId;
				Can_Global.CanHTHMap[j].CanHOHRef = &CtrlCfgPtr->CanHoh[i];
				j++;
			}
			else
			{}
		}
	}
	else
	{}


	/* Check for all configured CAN controllers*/
	for(uint8 configId = 0x0; configId < CAN_ALPS_CTRL_CONFIG_COUNT; configId++)
	{
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[configId];
		HwIndex     = CtrlCfgPtr->CanControllerId;

		/* Get base pointers of Standard and Extended filters*/
		StdIdFilterAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_STANDARD_ID_FILTER_BASE_ADDRESS);
		ExtIdFilterAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_EXTENDED_ID_FILTER_BASE_ADDRESS);
		RxBuffAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_RX_BUFFER_BASE_ADDRESS);
		RxFifoAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_RX_SHADOW_ADDRESS);

		/* Initialize to Zero*/
		memset(StdIdFilterAddr, 0x0, CAN_STANDARD_FILTER_SIZE);
		memset(ExtIdFilterAddr, 0x0, CAN_EXTENDED_FILTER_SIZE);
		memset(RxBuffAddr, 0x0, CAN_RX_BUFFER_MAX_SIZE);
		memset(RxFifoAddr, 0x0, CAN_RX_SHADOW_SIZE);

		UnitPtr = &CanUnitInfo[HwIndex];

		/* Initialize Unitptr contents to zero*/
		memset(UnitPtr, 0x0, sizeof(CanUnitType));

		/* SWS_CAN_00259 Req-4.3.1*/
		UnitPtr->State 					= CAN_CS_STOPPED;
		UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_NONE;
		UnitPtr->LockCount 				= 0x0;

		/* Clear stats*/
		#if (SUPPORT_CAN_STATISTICS == STD_ON)
		memset(&UnitPtr->Stats, 0x0, sizeof(Can_StatisticsType));
		#endif

		/* Reset CAN module*/
		Can_lclReset((uint8)CtrlCfgPtr->CanControllerId);

		/* Get the controller in Config-mode*/
		Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

		/* Set baud rate*/
		Can_Hal_SetBaudrate(HwIndex, CtrlCfgPtr->CanControllerDefaultBaudrate->CanControllerBaudRateConfigID);

		/* Get the controller in Config-mode again*/
		Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

		/* Configure TX and RX Fifo size:(FD - 64) (BASIC - 40), Fifo_operation_mode:Blocking, Watermark:64*/
		TempVal = ((uint32)(CAN_TX_FIFO_WATERMARK &(CAN_FIFO_WATERMARK_MASK)) << CAN_TX_RX_WATERMARK_SHIFT) |
		((uint32)CAN_TX_RX_FIFO_SIZE << CAN_TX_RX_FIFO_SIZE_SHIFT);
		Can_Hw_Reg[HwIndex]->TXFCFGR = TempVal;
		Can_Hw_Reg[HwIndex]->TXEFCFGR = TempVal;
		TempVal = ((uint32)(CAN_RX_FIFO_WATERMARK &(CAN_FIFO_WATERMARK_MASK)) << CAN_TX_RX_WATERMARK_SHIFT) |
		((uint32)(CAN_TX_RX_FIFO_SIZE &(0xff)) << CAN_TX_RX_FIFO_SIZE_SHIFT);
		Can_Hw_Reg[HwIndex]->RXFCFGR = TempVal;

		/* Enable Automatic Re-transmission*/
		Can_Hw_Reg[HwIndex]->PCR |= 0x1;

		/* Get the controller in Normal-mode*/
		Can_lclConfigControllerMode(HwIndex, CAN_NORMAL_MODE);

		/* Enable Required interrupts if polling mode is not supported*/
		#if((CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_OFF) || (CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_OFF) || (CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_OFF))
		/* SWS_Can_00204 Req-4.3.1*/
		/* Disable interrupts*/
		if(CanUnitInfo[HwIndex].LockCount == 0)
		{
			Can_Hal_DisableControllerInterrupts(HwIndex);
			CanUnitInfo[HwIndex].LockCount++;
		}
		if(CanUnitInfo[HwIndex].LockCount == 0)
		{
			/** SWS_CAN_00208 Req-4.3.1*/
			/* Do nothing */
		}
		else
		{
			/* Enable interrupts*/
			CanUnitInfo[HwIndex].LockCount--;
			if(CanUnitInfo[HwIndex].LockCount == 0)
			{
				Can_Hal_EnableControllerInterrupts(HwIndex);
			}
			else
			{
				/*compliant to misra-c*/
			}
		}
		#endif

		/* check for all Hardware Objects */
		for(i=0; i<CtrlCfgPtr->CanHoh->CanHwObjectCount;i++ )
		{
			if((HwIndex == CtrlCfgPtr->CanHoh[i].CanControllerId ) && (RECEIVE == CtrlCfgPtr->CanHoh[i].CanObjectType) )
			{
				/* Configure Standard ID filter elements*/
				if(CtrlCfgPtr->CanHoh[i].CanHwFilter->CanStdFilterElements != NULL_PTR)
				{
					/* Temporary pointers for easy access*/
					const Can_Alps_StdFilterElementType *Can_StdFilterElementPtr = CtrlCfgPtr->CanHoh[i].CanHwFilter->CanStdFilterElements;
					for(j = 0; j < CtrlCfgPtr->CanHoh[i].CanHwFilter->NumOfStdIdFilters; j++)
					{
						StdIdFilterEle = ((((uint32)Can_StdFilterElementPtr->FilterType & CAN_FILTER_TYPE_MASK) << CAN_STD_ID_FILTER_SFT_SHIFT)
						| ((uint32)CAN_STORE_RX_FRAME_IN_FIFO << CAN_STD_ID_FILTER_SFEC_SHIFT)
						| ((Can_StdFilterElementPtr->Id0 & CAN_STD_ID_FILTER_ID_MASK) << CAN_STD_ID_FILTER_ID_SHIFT)
						| ((Can_StdFilterElementPtr->Id1 & CAN_STD_ID_FILTER_ID_MASK)));

						/* Configure Standard ID Filter element*/
						*StdIdFilterAddr = StdIdFilterEle;
						/* Increment address pointer*/
						StdIdFilterAddr ++;
						/* Increment pointer to get the next info*/
						Can_StdFilterElementPtr ++;

						/*record the index in the filter setting for the HOH*/
						CANIDStd_Index[configId][idx1] = CtrlCfgPtr->CanHoh[i].CanObjectId ;
						idx1++;
					}
				}

				/* Configure Extended ID filter element*/
				if(CtrlCfgPtr->CanHoh[i].CanHwFilter->CanExtFilterElements != NULL_PTR)
				{
					/* Temporary pointers for easy access*/
					const Can_Alps_ExtFilterElementType *Can_ExtFilterElementPtr = CtrlCfgPtr->CanHoh[i].CanHwFilter->CanExtFilterElements;
					for(j = 0; j < CtrlCfgPtr->CanHoh[i].CanHwFilter->NumofExtIdFilters; j++)
					{
						ExtIdFilterEle0 = ((((uint32)Can_ExtFilterElementPtr->FilterType & CAN_FILTER_TYPE_MASK) << CAN_EXT_ID_FILTER_SFT_SHIFT)
						| ((uint32)Can_ExtFilterElementPtr->Id0 & CAN_EXT_ID_FILTER_ID_MASK));
						ExtIdFilterEle1 = (((uint32)CAN_STORE_RX_FRAME_IN_FIFO << CAN_EXT_ID_FILTER_SFEC_SHIFT)
						| ((uint32)Can_ExtFilterElementPtr->Id1 & CAN_EXT_ID_FILTER_ID_MASK));

						/* Configure Extended Filter ID 0 element*/
						*ExtIdFilterAddr = ExtIdFilterEle0;
						/* Increament address pointer*/
						ExtIdFilterAddr ++;
						/* Configure Extended Filter ID 1 element*/
						*ExtIdFilterAddr = ExtIdFilterEle1;

						/* Increament address pointer*/
						ExtIdFilterAddr ++;
						/* Increament pointer to get the next info*/
						Can_ExtFilterElementPtr ++;

						/*record the index in the filter setting for the HOH*/
						CANIDExt_Index[configId][idx2] = CtrlCfgPtr->CanHoh[i].CanObjectId ;
						idx2++;
					}
				}
			}//end of
			else
			{
				/*compliant to misra-C*/
			}
		}//end of for(i=0; i<CtrlCfgPtr->CanHoh->CanHwObjectCount;i++ )

		/*one more filter for buffer*/
		*StdIdFilterAddr = ((((uint32)CAN_FILTER_RANGE & CAN_FILTER_TYPE_MASK) << CAN_STD_ID_FILTER_SFT_SHIFT)
						| ((uint32)CAN_STORE_RX_FRAME_IN_BUF << CAN_STD_ID_FILTER_SFEC_SHIFT)
						| ((0 & CAN_STD_ID_FILTER_ID_MASK) << CAN_STD_ID_FILTER_ID_SHIFT)
						| ((0x7ff & CAN_STD_ID_FILTER_ID_MASK)));

		*ExtIdFilterAddr = ((((uint32)CAN_FILTER_RANGE & CAN_FILTER_TYPE_MASK) << CAN_EXT_ID_FILTER_SFT_SHIFT)
		| ((uint32)0 & CAN_EXT_ID_FILTER_ID_MASK));
		ExtIdFilterAddr++;
		*ExtIdFilterAddr = (((uint32)CAN_STORE_RX_FRAME_IN_BUF << CAN_EXT_ID_FILTER_SFEC_SHIFT)
		| ((uint32)0x1fffffff & CAN_EXT_ID_FILTER_ID_MASK));


		/* Get the controller in Config-mode again*/
		Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

		/* Configure Acceptance Filter Register*/
		TempVal = (((uint32)CtrlCfgPtr->AlpsExtRejectNoMatch << CAN_REG_IDFCR_RNMFE_SHIFT)
		| ((uint32)(idx2+1) << CAN_REG_IDFCR_XIDFS_SHIFT)
		| ((uint32)CtrlCfgPtr->AlpsStdRejectNoMatch << CAN_REG_IDFCR_RNMFS_SHIFT)
		| (idx1+1));
		Can_Hw_Reg[HwIndex]->IDFCR |= (uint32)TempVal;

		/* Get the controller in Normal-mode*/
		Can_lclConfigControllerMode(HwIndex, CAN_NORMAL_MODE);

		idx1 = 0u;
		idx2 = 0u;

		/* SWS_CAN_00245 Req-4.3.1*/
		Can_Hal_SetControllerMode(HwIndex, CAN_CS_STOPPED);
	}
}

/******************************************************************************
*  Service name    : Can_Hal_DeInit
*  Syntax          : Std_ReturnType Can_Hal_DeInit(void)
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted
*  Description     : Service to perform CAN module De-initialization
******************************************************************************/
Std_ReturnType Can_Hal_DeInit(void) {
	CanUnitType *UnitPtr = NULL_PTR;
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;
	Std_ReturnType RetVal = E_OK;
	uint8 CtlrId;

	for(uint8 configId = 0x0; configId < CAN_ALPS_CTRL_CONFIG_COUNT; configId++)
	{
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[configId];
		CtlrId      = (uint8)CtrlCfgPtr->CanControllerId;

		UnitPtr = &CanUnitInfo[CtlrId];
		/* SWS_CAN_91012 Req-4.3.1, CAN_FSR_91012*/
		Can_lclParamCheck((boolean)(UnitPtr->State == CAN_CS_STARTED), CAN_DEINIT_SID, CAN_E_TRANSITION, &RetVal);
		if(RetVal == E_OK)
		{
			UnitPtr->State = CAN_CS_UNINIT;
			UnitPtr->transition = CAN_CS_UNINIT;
			UnitPtr->LockCount = 0x0;

			/* Clear stats*/
			#if (SUPPORT_CAN_STATISTICS == STD_ON)
			memset(&UnitPtr->Stats, 0x0, sizeof(Can_StatisticsType));
			#endif
		}
	}
	Can_Global.config = NULL_PTR;
	return RetVal;
}

#if (CAN_SET_BAUDRATE_API == STD_ON)
/******************************************************************************
*  Service name    : Can_Hal_SetBaudrate
*  Syntax          : Std_ReturnType Can_Hal_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
*  Parameters (in) : Controller - CAN Controller Id
*                  : BaudRateConfigID - Baudrate Config ID
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted
*  Description     : Service to Set the Baudrate of CAN Controller
******************************************************************************/
Std_ReturnType Can_Hal_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
{
	const Can_ControllerConfigType* CtrlCfgPtr = NULL_PTR;
	const Can_ControllerBaudrateConfigType *BaudratePtr = NULL_PTR;
	Std_ReturnType RetVal = E_OK;
	uint8 HwIndex, Sjw, NTSegment1, NTSegment2;
	uint32 Clock = 0, Tq, Brp, TempVal = 0;
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	/*CAN_FSR_00002 Check if the CANIF state is in STOPPED state*/
	Can_lclParamCheck((boolean)(UnitPtr->State != CAN_CS_STOPPED), CAN_SETBAUDRATE_SID, CAN_E_TRANSITION, &RetVal);
	if(RetVal == E_OK)
	{
		CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[Controller];
		HwIndex = CtrlCfgPtr->CanControllerId;

		/* Find the baudrate Config*/
		for(uint8 i = 0x0; i < CtrlCfgPtr->CanControllerSupportedBaudratesCount; i++)
		{
			if(CtrlCfgPtr->CanControllerBaudrateConfig[i].CanControllerBaudRateConfigID == BaudRateConfigID)
			{
				BaudratePtr = &CtrlCfgPtr->CanControllerBaudrateConfig[i];
				/* Update baudrate index for current container*/
				UnitPtr->CanControllerUpdatedBaudrateConfig = &CtrlCfgPtr->CanControllerBaudrateConfig[i];
			}
		}

		/* SWS_CAN_00493 Req-4.3.1, CAN_FSR_00493 - Check if the parameter BaudRateConfigID has an invalid value*/
		Can_lclParamCheck((boolean)(BaudRateConfigID >= CtrlCfgPtr->CanControllerSupportedBaudratesCount), CAN_SETBAUDRATE_SID, CAN_E_PARAM_BAUDRATE, &RetVal);
		if(RetVal == E_OK)
		{
			/* Get the CAN clock freq */
			if(CtrlCfgPtr->CanControllerId == 0 )
			{
				Clock = Mcu_GetPeripheralClkFreq(MCU_CAN0_CLOCK);
			}
			else if(CtrlCfgPtr->CanControllerId == 1)
			{
				Clock = Mcu_GetPeripheralClkFreq(MCU_CAN1_CLOCK);
			}
			else
			{
				Clock = 0u;   /* compliant to misra-c */
			}

			/* Disable the OS interrupts*/
			SchM_Enter_Can_SetBaudrate();

			/* BRP Calculation*/
			//{.sync_jump_width = 2, .bit_rate_prescale = 7, .segment1 = 5, .segment2 = 2}   500k
			//.sync_jump_width = 2, .bit_rate_prescale = 3, .segment1 = 5, .segment2 = 2  1M
			NTSegment1 	= BaudratePtr->CanControllerPropSeg + BaudratePtr->CanControllerSeg1;
			NTSegment2 	=  BaudratePtr->CanControllerSeg2;
			Tq    		= 1 + NTSegment1 + NTSegment2;
			Sjw   		= BaudratePtr->CanControllerSyncJumpWidth;

			/* Baud rate presclar*/
			Brp = (uint32)((Clock / (BaudratePtr->CanControllerBaudRate * 1000 * Tq)) - 1);

			/* Get the Controller in Configuration status mode to configure NBTCR & DBTCR registers*/
			Can_lclConfigControllerMode(Controller, CAN_CONFIG_MODE);

			/* Configure Nominal Bit rate*/
			TempVal = (((uint32)(Sjw - 0x1) << CAN_REG_NBTCR_NSJW_SHIFT) | (Brp << CAN_REG_NBTCR_NBRP_SHIFT) |
						((uint32)(NTSegment1 - 0x1) << CAN_REG_NBTCR_NTSEG1_SHIFT) | (NTSegment2 - 0x1));
			Can_Hw_Reg[HwIndex]->NBTCR = TempVal;

			/* If CANFD supported, Calculate Data bit rate*/
			#if (CAN_FD_SUPPORTED == STD_ON)
			if(BaudratePtr->CanControllerFdBaudrateConfig != NULL_PTR)
			{
				if(((Can_Hw_Reg[HwIndex]->PCR >> CAN_REG_PCR_FDD_SHIFT) == 0x0))
				{
					uint8 DSjw, DTSegment1, DTSegment2;
					DTSegment1	= BaudratePtr->CanControllerFdBaudrateConfig->CanControllerFdPropSeg + BaudratePtr->CanControllerFdBaudrateConfig->CanControllerFdSeg1;
					DTSegment2 	= BaudratePtr->CanControllerFdBaudrateConfig->CanControllerFdSeg2;
					Tq    		= 1 + DTSegment1 + DTSegment2;
					DSjw   		= BaudratePtr->CanControllerFdBaudrateConfig->CanControllerFdSyncJumpWidth;

					/* Baud rate presclar*/
					Brp = (uint32)(Clock / (BaudratePtr->CanControllerFdBaudrateConfig->CanControllerFdBaudRate * 1000 * Tq)) - 1;
						/* Configure Data Bit rate*/
						TempVal = (((uint32)(DSjw - 0x1) << CAN_REG_DBTCR_DSJW_SHIFT) | (Brp << CAN_REG_DBTCR_DBRP_SHIFT) |
						((uint32)(DTSegment1 - 0x1) << CAN_REG_DBTCR_DTSEG1_SHIFT) | (DTSegment2 - 0x1));
					Can_Hw_Reg[HwIndex]->DBTCR = TempVal;

					/* Enable CAN-FD operations and Bitrate switching */
					Can_Hw_Reg[HwIndex]->PCR = (Can_Hw_Reg[HwIndex]->PCR | CAN_REG_FDO_BRS_SET);
					/* Configure Rx/Tx data field size*/
					Can_Hw_Reg[HwIndex]->RXESCR = (CAN_TX_RX_BUF_FIFO_DATA_SZ_64 << CAN_REG_RXESCR_RXFDS_SHIFT) + CAN_TX_RX_BUF_FIFO_DATA_SZ_64;
					Can_Hw_Reg[HwIndex]->TXESCR = (CAN_TX_RX_BUF_FIFO_DATA_SZ_64 << CAN_REG_TXESCR_TXFDS_SHIFT) + CAN_TX_RX_BUF_FIFO_DATA_SZ_64;
				}
			}
			else
			{
				Can_Hw_Reg[HwIndex]->PCR = (Can_Hw_Reg[HwIndex]->PCR & 0x03U);               //disable the FD operation if the FD baud-rate does not set
				Can_Hw_Reg[HwIndex]->RXESCR = (0U << CAN_REG_RXESCR_RXFDS_SHIFT);
			}
			#endif

			/* Get the controller in normal mode*/
			Can_lclConfigControllerMode(Controller, CAN_NORMAL_MODE);

			/* Enable the OS interrupts*/
			SchM_Exit_Can_SetBaudrate();
		}
	}
	return RetVal;
}
#endif

/******************************************************************************
*  Service name    : Can_Hal_EnableControllerInterrupts
*  Syntax          : void Can_Hal_EnableControllerInterrupts(uint8 HwIndex)
*  Parameters (in) : HwIndex - CAN Controller ID
*  Parameters (out): None
*  Return value    : None
*  Description     : Service to enable CAN Controller Required Interrupts
******************************************************************************/
void Can_Hal_EnableControllerInterrupts(uint8 Controller) {
	const Can_ControllerConfigType* CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[Controller];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
	uint32 IE_temp = 0u ;

	/* Enabled following interrupts:
	*BOE, RXFWE, TXFFE */
	#if(CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_OFF)
		IE_temp |= (1UL << 8U);
	#endif

	#if(CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_OFF)
		IE_temp |= (1UL << 11U);
	#endif

	#if(CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_OFF)
		IE_temp |= (1UL << 22U);
	#endif

	Can_Hw_Reg[HwIndex]->IE = IE_temp;

	/* Enable Interrupt Line*/
	Can_Hw_Reg[HwIndex]->ILE = 0x1 << CAN_ENABLE_INTERRUPT_LINE_0;

	/* Enable interrupts with respect to Interrupt line selected*/
	Can_Hw_Reg[HwIndex]->ILS0R = 0; /*INT0 line selected*/
	Can_Hw_Reg[HwIndex]->ILS1R = 0; /*INT0 line selected*/
}


/******************************************************************************
*  Service name    : Can_Hal_DisableControllerInterrupts
*  Syntax          : void Can_Hal_DisableControllerInterrupts(uint8 Controller)
*  Parameters (in) : Controller - CAN Controller ID
*  Parameters (out): None
*  Return value    : None
*  Description     : Service to disable CAN Controller Interrupts
******************************************************************************/
void Can_Hal_DisableControllerInterrupts(uint8 Controller) {
	const Can_ControllerConfigType* CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[Controller];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;

	/* Disable all the Enabled interrupts:
	*PEDE, BOE, EWE, EPE, RXBNE, RXFLE, RXFFE, RXFWE, RXFEE,
	*TXFFE, TXFEW, TXFEE, TEFLE, TEFFE, TEFWE, TEFEE */
	Can_Hw_Reg[HwIndex]->IE = 0x0;

	/* Disable Interrupt Lines*/
	Can_Hw_Reg[HwIndex]->ILE = 0x0;

	/* Disable interrupts with respect to Interrupt line selected*/
	Can_Hw_Reg[HwIndex]->ILS0R = 0x0; /* INT0 line selected*/
	Can_Hw_Reg[HwIndex]->ILS1R = 0x0; /* INT0 line selected*/
}


/******************************************************************************
*  Service name    : Can_Hal_GetControllerErrorState
*  Syntax          : void Can_Hal_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
*  Parameters (in) : ControllerId - CAN Controller ID
*                  : ErrorStatePtr - Error State pointer
*  Parameters (out): None
*  Return value    : None
*  Description     : Updates the Error state of CAN module in ErrorStatePtr
******************************************************************************/
void Can_Hal_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr) {
	const Can_ControllerConfigType* CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[ControllerId];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
	uint32 RegVal = Can_Hw_Reg[HwIndex]->ECR;
	/* Check Error status*/
	if(((RegVal & CAN_REG_ECR_TEC_MASK) > 127) || (((RegVal & CAN_REG_ECR_REC_MASK) >> CAN_REG_ECR_REC_SHIFT) > 127))
	{
		*ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /*Error Passive*/
	} else {
		*ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;  /*Error Active*/
	}

	RegVal = Can_Hw_Reg[HwIndex]->PSR;
	/* Check Bus-off status*/
	if(((RegVal & CAN_REG_PSR_BO_MASK) >> CAN_REG_PSR_BO_SHIFT) == 0x1)
	{
		*ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;  /*Error BusOff*/
	}
}


/******************************************************************************
*  Service name    : Can_Hal_GetControllerMode
*  Syntax          : void Can_Hal_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr )
*  Parameters (in) : Controller - CAN Controller ID
*                  : ControllerModePtr - CAN controller State pointer
*  Parameters (out): None
*  Return value    : None
*  Description     : Updates the Controller state of CAN module in ControllerModePtr
******************************************************************************/
void Can_Hal_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr ) {
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	/* Configure the Controller state*/
	*ControllerModePtr = UnitPtr->transition;
}


/******************************************************************************
*  Service name    : Can_Hal_SetControllerMode
*  Syntax          : Std_ReturnType Can_Hal_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
*  Parameters (in) : Controller - CAN Controller ID
*                  : Transition - CAN controller State transition type
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted
*  Description     : Configures the CAN module to get into required transition mode
******************************************************************************/
Std_ReturnType Can_Hal_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition) {
	Std_ReturnType RetVal = E_OK;
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	/*CAN_FSR_000003 Error out if the controller is already in Uninitialized State */
	Can_lclParamCheck((boolean)(UnitPtr->State == CAN_CS_UNINIT), CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION, &RetVal);
	switch(Transition)
	{
	case CAN_CS_UNINIT:
		/*CAN_FSR_00004*/
		Can_lclParamCheck((boolean)(!((UnitPtr->State != CAN_CS_SLEEP) !=
						(UnitPtr->State != CAN_CS_STOPPED))), CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION, &RetVal);
		if(RetVal == E_OK)
		{
			/* Disable the OS interrupts*/
			SchM_Enter_Can_SetControllerMode();

			Can_Hal_DeInit();
			Can_Global.initRun = CAN_UNINIT;
			UnitPtr->transition = CAN_CS_UNINIT;
			UnitPtr->State = CAN_CS_UNINIT;
			UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_UINIT;

			/* Enable the OS interrupts*/
			SchM_Exit_Can_SetControllerMode();
		}
		break;
	case CAN_CS_STOPPED:
		if(RetVal == E_OK)
		{
			/* Disable the OS interrupts*/
			SchM_Enter_Can_SetControllerMode();

			/* Get the controller in configuration status*/
			Can_lclConfigControllerMode(Controller, CAN_CONFIG_MODE);

			/* Reset CAN module*/
			Can_lclReset(Controller);

			UnitPtr->State = CAN_CS_STOPPED;
			UnitPtr->transition = CAN_CS_STOPPED;
			UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_PENDING_STOP;

			/* Get the controller in normal mode*/
			Can_lclConfigControllerMode(Controller, CAN_NORMAL_MODE);

			/* Enable the OS interrupts*/
			SchM_Exit_Can_SetControllerMode();
		}
		break;
	case CAN_CS_STARTED:
		/* SWS_CAN_00409 Req-4.3.1, CAN_FSR_00409*/
		Can_lclParamCheck((boolean)(UnitPtr->State != CAN_CS_STOPPED),
						CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION, &RetVal);
		if(RetVal == E_OK)
		{
			/* Disable the OS interrupts*/
			SchM_Enter_Can_SetControllerMode();

			/* SWS_CAN_00384 Req-4.3.1 Perform re-initialization*/
			Can_lclReinitialize(Controller);

			UnitPtr->State = CAN_CS_STARTED;
			UnitPtr->transition = CAN_CS_STARTED;
			UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_PENDING_START;

			/* Enable the OS interrupts*/
			SchM_Exit_Can_SetControllerMode();
		}
		break;
	case CAN_CS_SLEEP:
		/*SWS_CAN_00411 Req-4.3.1, CAN_FSR_00411 */
		Can_lclParamCheck((boolean)!((boolean)(UnitPtr->State != CAN_CS_STOPPED) ^ (boolean)(UnitPtr->State != CAN_CS_SLEEP)),
						CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION, &RetVal);
		if(RetVal == E_OK)
		{
			/* Disable the OS interrupts*/
			SchM_Enter_Can_SetControllerMode();

			/* Enter in sleep mode*/
			Can_lclConfigControllerMode(Controller, CAN_SLEEP_MODE);

			UnitPtr->State = CAN_CS_SLEEP;
			UnitPtr->transition = CAN_CS_SLEEP;
			UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_PENDING_SLEEP;

			/* Enable the OS interrupts*/
			SchM_Exit_Can_SetControllerMode();
		}
		break;
	default:
		/* SWS_CAN_00200 Req-4.3.1, CAN_FSR_00200*/
		#if (CAN_DEV_ERROR_DETECT == STD_ON)
		/* Calls Det_ReportError from Det module*/
		(void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION);
		#endif
		RetVal = E_NOT_OK;
		break;
	}
	return RetVal;
}


/******************************************************************************
*  Service name    : Can_Hal_Write
*  Syntax          : Std_ReturnType Can_Hal_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo)
*  Parameters (in) : Hth - information which HW-transmit handle shall be used for transmit.
*                  : PduInfo - Pointer to SDU user memory, Data Length and Identifier.
*  Parameters (out): None
*  Return value    : Std_ReturnType - E_OK: Write command has been accepted.
*                  : -E_NOT_OK: development error occurred
*                  : -CAN_BUSY: No TX hardware buffer available.
*  Description     : This function populates the CAN Transmit buffer with the
*                  : required messsage to be transmited.
******************************************************************************/
Std_ReturnType Can_Hal_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo) {
	Std_ReturnType RetVal = E_OK;
	const CanHardwareObject *HohObj = NULL_PTR;
	uint8 Controller = 0x0;
	uint8 tempdata[64];

	/* Software TxBuffFifo and initialize its members to zero*/
	Can_Alps_TxFifo TxBuffFifo = {.T0.B = 0x0, .T1.B = 0x0};

	/* Get hoh map*/
	Can_ObjectHOHMapType *HohMap = &Can_Global.CanHTHMap[(Hth - CAN_NUM_OF_HRHS)];

	/* Verify if its the correct map*/
	if(HohMap->CanHOHRef->CanObjectId != Hth)
	{
		#if (CAN_DEV_ERROR_DETECT == STD_ON)
		(void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_WRITE_SID, CAN_E_PARAM_HANDLE);
		#endif
	}
	HohObj = HohMap->CanHOHRef;
	Controller = HohMap->CanControllerRef;
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	/* Get controller Configurations*/
	const Can_ControllerConfigType *CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[Controller];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
	volatile uint32 *TxFifoRegPtr = &Can_Hw_Reg[HwIndex]->TXFELMR2;
	uint32 index = 0;

	/* SWS_CAN 00503 Req-4.3.1*/
	if(CtrlCfgPtr->CanHoh->CanTriggerTransmitEnable != TRUE)
	{
		/* SWS_CAN 00505 Req-4.3.1, CAN_FSR_00505 */
		Can_lclParamCheck((boolean)(NULL_PTR == PduInfo->sdu),
		CAN_WRITE_SID, CAN_E_PARAM_POINTER, &RetVal);
	}
	else
	{
	#if (CAN_TRIGGER_TRANSMIT_USED == STD_ON)
		PduInfoType PduInfoObject;
		PduInfoObject.SduDataPtr = (uint8*)((void*)(&TxBuffFifo.TX_DATA.DATA[0]));
		PduInfoObject.SduLength = CAN_TX_RX_FIFO_SIZE;
		/* SWS_CAN_00504, SWS_CAN_00506 Req-4.3.1, */
		RetVal = CanIf_TriggerTransmit( PduInfo->swPduHandle, &PduInfoObject);
	#endif
	}

	if(RetVal == E_OK)
	{
		/* Enter exclusive area. */
		SchM_Enter_Can_Write();

		/* Check if TX Fifo is not full */
		/* SWS_CAN_00276 Req-4.3.1 Store the swPduHandle*/
 		if (((Can_Hw_Reg[HwIndex]->TXFSTAR & CAN_REG_TXFSTAR_TXFF_MASK) >> CAN_REG_TXFSTAR_TXFF_SHIFT) != 0x1)
		{
 			rx_isr_free_counter[HwIndex] = 0U;					//reset counter
  			Can_Hw_Reg[HwIndex]->IE |= 0x10000;      			//Message RAM Access Failure interrupt enable

			/* SWS_CAN_00486 Req-4.3.1 CAN Frame has to be sent according to the two MSB bits of Can_PduType->id.*/
			if ( CAN_IDTYPE_EXTEND == (PduInfo->id & CAN_IDTYPE_EXTEND))
			{
				TxBuffFifo.T0.B.XTD = 0x1;  /* EXTENDED CAN ID*/
				TxBuffFifo.T0.B.ID = PduInfo->id & CAN_EXTENDED_ID_MASK;
			} else {
				TxBuffFifo.T0.B.XTD = 0x0;  /* STD CAN ID*/
				TxBuffFifo.T0.B.ID = ((PduInfo->id & CAN_STANDARD_ID_MASK) << CAN_TXBUFF_STANDARD_ID_SHIFT); /* STD ID should be at 28:18*/
			}

			TxBuffFifo.T0.B.RTR = 0x0; /* Remote transmission request */
			TxBuffFifo.T0.B.ESI = 0x0; /* Error state indicator */
			TxBuffFifo.T1.B.MM  = 0x0; /* Message marker */
			TxBuffFifo.T1.B.EFC = 0X1; /* Store Tx events */

			/*Configure CAN frame type : FD FRAME or STD FRAME*/
			if( CAN_IDTYPE_CANFD == (PduInfo->id & CAN_IDTYPE_CANFD))
			{
				TxBuffFifo.T1.B.FDF = 1;
			}
			else
			{
				TxBuffFifo.T1.B.FDF = 0;
			}

			if((((Can_Hw_Reg[HwIndex]->PCR & CAN_REG_PCR_BRS_MASK) >> CAN_REG_PCR_BRS_SHIFT) == 0x1))
			{
				TxBuffFifo.T1.B.BRS = UnitPtr->CanControllerUpdatedBaudrateConfig->CanControllerFdBaudrateConfig->CanControllerTxBitRateSwitch;
			}

			/* Configure data length*/
			TxBuffFifo.T1.B.DLC = Can_lclGetPduLength(PduInfo->length);

			/*Copy data temporarily*/
			memcpy(tempdata, PduInfo->sdu, PduInfo->length);

			/* Perform padding of CAN FD data bits, if Pdu->Length is less than allowed DLC value */
			#if (CAN_FD_SUPPORTED == STD_ON)
			/* SWS_CAN_00502 Req-4.3.1*/
			uint32 PadLength = (Can_lclGetDataWordLength(TxBuffFifo.T1.B.DLC) - PduInfo->length);
			if(PadLength != 0)
			{
				for(uint8 i = 0; i < PadLength; i++) {
					tempdata[PduInfo->length + i] = HohObj->CanFdPaddingValue;
				}
			}
			#endif

			/* Change Endiness of data as expected by the hardware*/
			for(uint8 Idx = 0x0; Idx < PduInfo->length; Idx = Idx + 4)
			{
				Can_lclChangeEndianess(Idx, tempdata, TxBuffFifo.TX_DATA.DATA);
			}

			#if(CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_OFF)
			/* Enable TX interrupt if polling mode is not supported: TEFEE, TXFEE, TXFLE, TEFFE */
			Can_Hw_Reg[Controller]->IE |= 0x95;
			#endif

			/* Copy Header 0 in Txfifo*/
			Can_Hw_Reg[HwIndex]->TXFELMR0 = (((uint32)TxBuffFifo.T0.B.ESI << 31) | ((uint32)TxBuffFifo.T0.B.XTD << 30)
									| ((uint32)TxBuffFifo.T0.B.RTR << 29) | (TxBuffFifo.T0.B.ID));

			/* Copy Header 1 in Txfifo*/
			Can_Hw_Reg[HwIndex]->TXFELMR1 = (((uint32)TxBuffFifo.T1.B.MM << 24) | ((uint32)TxBuffFifo.T1.B.EFC << 23)
					| ((uint32)TxBuffFifo.T1.B.FDF << 21) | ((uint32)TxBuffFifo.T1.B.BRS << 20) | ((uint32)TxBuffFifo.T1.B.DLC << 16));

			/* Copy DATA in TXFifo*/

			for(uint8 j = 0; j < CAN_FIFO_ELEMENT_SIZE; j++){

				*TxFifoRegPtr = ((uint32)TxBuffFifo.TX_DATA.DATA[index] << 24) | ((uint32)TxBuffFifo.TX_DATA.DATA[index + 1] << 16)
				| ((uint32)TxBuffFifo.TX_DATA.DATA[index + 2] << 8) | (TxBuffFifo.TX_DATA.DATA[index + 3]);

				/* Increament to point to next data register*/
				TxFifoRegPtr ++;
				index = index + 4;
			}
			Can_lclStorePduHandleData(Controller, PduInfo->swPduHandle);

			/* Increament statistics*/
			#if (SUPPORT_CAN_STATISTICS == STD_ON)
			UnitPtr->Stats.TxSuccessCount++;
			#endif
		} else {
			/* SWS_CAN_00213 Req-4.3.1*/
			RetVal = CAN_BUSY;
		}
		/* Leave exclusive area. */
		SchM_Exit_Can_Write();

		for (uint32 j=0; j<350;j++)  //delay for two FIFO operation > 100 cycle
		{
		  asm("nop");
		}
	}
	return RetVal;
}

/******************************************************************************
*  Service name    : Can_Hal_BusOff
*  Syntax          : void Can_Hal_BusOff()
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : Performs the polling of bus-off events.
******************************************************************************/
#if (CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_ON)
void Can_Hal_MainFunctionBusOff()
{
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;
	uint8 HwIndex;

	/* Bus-off polling events. */
	for (uint8 ControllerId = 0x0; ControllerId < CAN_ALPS_CTRL_CONFIG_COUNT; ControllerId++)
	{
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[ControllerId];
		HwIndex = CtrlCfgPtr->CanControllerId;
		/* Poll until Bus-off event occurs */
		if((((Can_Hw_Reg[HwIndex]->PSR & CAN_REG_PSR_BO_MASK) >> CAN_REG_PSR_BO_SHIFT) != 0x1))
		{
			//do nothing
		}
		else
		{
			Can_lclIsrBusoff(ControllerId);
		}

		if(0 == (Can_Hw_Reg[HwIndex]->TXFSTAR & CAN_REG_TXFSTAR_TXFLV_MASK) >> CAN_REG_TXFSTAR_TXFLV_SHIFT )
		{
			Fifo_Pend[HwIndex].cnt = 0;
		}
		else
		{
			if( Fifo_Pend[HwIndex].Fifo_Levl == ((Can_Hw_Reg[HwIndex]->TXFSTAR & CAN_REG_TXFSTAR_TXFGI_MASK) >> CAN_REG_TXFSTAR_TXFGI_SHIFT) )
			{
				Fifo_Pend[HwIndex].cnt++;
				if(Fifo_Pend[HwIndex].cnt >= 4u)
				{
					Can_lclIsrBusoff(ControllerId);
					Fifo_Pend[HwIndex].cnt = 0;
				}
				else
				{
					/*compliant to misra-c*/
				}
			}
			else
			{
				Fifo_Pend[HwIndex].Fifo_Levl = ((Can_Hw_Reg[HwIndex]->TXFSTAR & CAN_REG_TXFSTAR_TXFGI_MASK) >> CAN_REG_TXFSTAR_TXFGI_SHIFT);
				Fifo_Pend[HwIndex].cnt = 0;
			}
		}
	}
}
#endif


/******************************************************************************
*  Service name    : Can_Hal_MainFunctionWrite
*  Syntax          : void Can_Hal_MainFunctionWrite()
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of TX confirmation
******************************************************************************/
#if (CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_ON)
void Can_Hal_MainFunctionWrite() {
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;
	uint8 HwIndex;

	/* Write polling events. */
	for (uint8 ControllerId = 0x0; ControllerId < CAN_ALPS_CTRL_CONFIG_COUNT; ControllerId++)
	{
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[ControllerId];
		HwIndex = CtrlCfgPtr->CanControllerId;

		/*Poll until Acknowledge is received indicating host wrote message.*/
		if(((Can_Hw_Reg[HwIndex]->TXFSTAR & CAN_REG_TXFSTAR_TXFGI_MASK) >> CAN_REG_TXFSTAR_TXFGI_SHIFT) != 0x0)
		{
			Can_lclIsrTx(ControllerId);

			#if (SUPPORT_CAN_STATISTICS == STD_ON)
			CanUnitType *UnitPtr = &CanUnitInfo[ControllerId];
			/* Increament statistics*/
			UnitPtr->Stats.TxErrorCount = (Can_Hw_Reg[HwIndex]->ECR & 0xFF);

		#endif
		}
	}
}
#endif


/******************************************************************************
*  Service name    : Can_Hal_MainFunctionRead
*  Syntax          : void Can_Hal_MainFunctionRead()
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of RX confirmation
******************************************************************************/
#if (CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_ON)
void Can_Hal_MainFunctionRead() {
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;
	uint8 HwIndex;

	/* Read polling events. */
	for (uint8 ControllerId = 0x0; ControllerId < CAN_ALPS_CTRL_CONFIG_COUNT; ControllerId++)
	{
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[ControllerId];
		HwIndex = CtrlCfgPtr->CanControllerId;
		/* Poll until CAN frame is received in RXFIFO*/
		if(((Can_Hw_Reg[HwIndex]->RXFSTAR & CAN_REG_RXFSTAR_RXFLV_MASK) >> CAN_REG_RXFSTAR_RXFLV_SHIFT) ==  0x0u)
		{
			//do nothing
		}
		else
		{
			Can_lclIsrRx(ControllerId);
		}
	}
}
#endif

/******************************************************************************
*  Service name    : Can_Hal_MainFunctionMode
*  Syntax          : void Can_Hal_MainFunctionMode()
*  Parameters (in) : None
*  Parameters (out): None
*  Return value    : None
*  Description     : This function performs the polling of CAN controller mode transitions
******************************************************************************/
void Can_Hal_MainFunctionMode() {
	CanUnitType *UnitPtr = NULL_PTR;
	Can_ControllerStateType CanMode = CAN_CS_UNINIT;
	uint8 HwIndex;
	uint32 RegVal;
	const Can_ControllerConfigType *CtrlCfgPtr = NULL_PTR;

	/* Read polling events*/
	for (uint8 ControllerId = 0x0; ControllerId < CAN_ALPS_CTRL_CONFIG_COUNT; ControllerId++)
	{
		UnitPtr = &CanUnitInfo[ControllerId];
		CtrlCfgPtr  = &Can_Global.config->CanConfigSet->CanController[ControllerId];
		HwIndex = CtrlCfgPtr->CanControllerId;
		RegVal = Can_Hw_Reg[HwIndex]->MCR;

		/* Check transition*/
		switch(UnitPtr->PendingStateIndication)
		{
		case CAN_CTRL_INDICATION_PENDING_START:
			/* Check if CANIF state in STARTED*/
			if(UnitPtr->State == CAN_CS_STARTED)
			{
				CanMode = CAN_CS_STARTED;
			}
			break;
		case CAN_CTRL_INDICATION_PENDING_STOP:
			/* Check if CANIF state in STOPPED*/
			if(UnitPtr->State == CAN_CS_STOPPED)
			{
				CanMode = CAN_CS_STOPPED;
			}
			break;
		case CAN_CTRL_INDICATION_PENDING_SLEEP:
			/* Check if CANIF state in SLEEP*/
			if(UnitPtr->State == CAN_CS_SLEEP)
			{
				CanMode = CAN_CS_SLEEP;
			}
			break;
		case CAN_CTRL_INDICATION_UINIT:
			CanMode = CAN_CS_UNINIT;
			break;
		case CAN_CTRL_INDICATION_NONE:
		default:
		{
			/*compliant to misrc-c*/
			break;
		}
		}

		if( CanMode != CAN_CS_UNINIT )
		{
			UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_NONE;
			/** SWS_CAN_00370 Req 4.3.1*/
			/** SWS_CAN_00373 Req 4.3.1*/
			CanIf_ControllerModeIndication(ControllerId, CanMode);
		}
	}
}


/******************************************************************************
*                           Private Functions
******************************************************************************/
static uint32 Can_lclGetBaseAddress(uint8 ControllerId)
{
	uint32 addr = 0 ;
	if(ControllerId == 0)
	{
		addr =  CAN0_BASE_ADDRESS;
	}
	else
	{
		addr = CAN1_BASE_ADDRESS;
	}
	return addr;
}


/*******************************************************************************
*  Service name    : Can_lclParamCheck
*  Syntax          : void Can_lclParamCheck(boolean Expr, uint8 ApiId,
*                                           uint8 ErrId, Std_ReturnType *RetVal)
*  Parameters (in) : Expr - TRUE or FALSE
*                    ApiId - API ID
*                    ErrId - ERROR ID
*  Parameter(inout): RetVal
*  Return value    : None
*  Description     : Reports Det error if CAN_DEV_ERROR_DETECT == STD_ON.
*******************************************************************************/
void Can_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal)
{
	/* Checks if the Expr is TRUE*/
	if ((TRUE == Expr) && (E_OK == *RetVal))
	{
	#if (CAN_DEV_ERROR_DETECT == STD_ON)
		/* Calls Det_ReportError from Det module*/
		(void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, ApiId, ErrId);
	#endif
		*RetVal = E_NOT_OK;
	}
}

/*******************************************************************************
*  Service name    : Can_lclReset
*  Syntax          : static void Can_lclReset( uint8 ControllerId)
*  Parameters (in) : CtrlCfgPtr - Holds CAN Configuration info
*  Parameter (out) : --
*  Return value    : --
*  Description     : Resets the CAN Controller
*******************************************************************************/
static void Can_lclReset( uint8 ControllerId) {
	const Can_ControllerConfigType *CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[ControllerId];
	uint8 HwIndex  = (uint8)CtrlCfgPtr->CanControllerId, SetVal = 0x1;

	CanUnitType *UnitPtr = &CanUnitInfo[ControllerId];

	/* Reset Module*/
	Can_Hw_Reg[HwIndex]->MCR |=  SetVal;

	/* Reset Pduhandlebuffer pointers*/
	Can_lclResetPduHandleData(UnitPtr);
}

/*******************************************************************************
*  Service name    : Can_lclConfigControllerMode
*  Syntax          : static void Can_lclConfigControllerMode(Can_ControllerConfigType *CtrlCfgPtr, Can_Mode ControllerMode) {
*  Parameters (in) : CtrlCfgPtr - Holds CAN Configuration info
*                  : ControllerMode - mode to be set
*                  : Value - mode value to be set
*  Parameter (out) : --
*  Return value    : --
*  Description     : Gets the CAN Controller in Required mode(Normal, Config, Loopback, Restricted, Sleep, Monitor, Test)
*******************************************************************************/
static void Can_lclConfigControllerMode(uint8 ControllerId, Can_Mode ControllerMode) {
 	const Can_ControllerConfigType *CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[ControllerId];
	uint8 HwIndex  = CtrlCfgPtr->CanControllerId, Value = 1;
	uint32 ShiftVal = 0x0, MaskVal = 0x0;

	if(ControllerMode == CAN_NORMAL_MODE){
		Value = 0;
		ShiftVal = 1;
		MaskVal = CAN_REG_MCR_CFG_MASK;
	}else if(ControllerMode == CAN_CONFIG_MODE){
		ShiftVal = 1;
		MaskVal = CAN_REG_MCR_CFG_MASK;
	}else if(ControllerMode == CAN_LOOP_MODE){
		ShiftVal = 2;
		MaskVal = CAN_REG_MCR_LOOPBACK_MASK;
	}else if(ControllerMode == CAN_MONITOR_MODE){
		ShiftVal = 3;
		MaskVal = CAN_REG_MCR_MONITOR_MASK;
	}else if(ControllerMode == CAN_RESTRICTED_MODE){
		ShiftVal = 4;
		MaskVal = CAN_REG_MCR_ROPT_MASK;
	}else if(ControllerMode == CAN_SLEEP_MODE){
		ShiftVal = 6;
		MaskVal = CAN_REG_MCR_SLEEP_MASK;
	}else if(ControllerMode == CAN_TEST_MODE){
		ShiftVal = 7;
		MaskVal = CAN_REG_MCR_TEST_MASK;
	}
	else
	{
		/*compliant to misra-c*/
	}

	Can_Hw_Reg[HwIndex]->MCR = (Can_Hw_Reg[HwIndex]->MCR & MaskVal) | (Value << ShiftVal);
}


/*******************************************************************************
*  Service name    : Can_lclReinitialize
*  Syntax          : static void Can_lclReinitialize(uint8 Controller)
*  Parameters (in) : Controller - Controller ID which need to be re-initialized
*  Parameter (out) : --
*  Return value    : --
*  Description     : Re-initializes the CAN Controller
*******************************************************************************/
static void Can_lclReinitialize(uint8 Controller) {
	CanUnitType *UnitPtr = NULL_PTR;
	const Can_ControllerConfigType *CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[Controller];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
	uint32 StdIdFilterEle = 0, ExtIdFilterEle0 = 0, ExtIdFilterEle1 = 0, TempVal = 0;
	uint32 *StdIdFilterAddr = NULL_PTR, *ExtIdFilterAddr = NULL_PTR, *RxBuffAddr = NULL_PTR, *RxFifoAddr = NULL_PTR;
	uint8 i=0 ,j=0,idx1=0u , idx2=0u;

	/* Get base pointers of Standard and Extended filters*/
	StdIdFilterAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_STANDARD_ID_FILTER_BASE_ADDRESS);
	ExtIdFilterAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_EXTENDED_ID_FILTER_BASE_ADDRESS);
	RxBuffAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_RX_BUFFER_BASE_ADDRESS);
	RxFifoAddr = (uint32 *)(Can_lclGetBaseAddress(HwIndex) + CAN_RX_SHADOW_ADDRESS);

	/* Initialize to Zero*/
	memset(StdIdFilterAddr, 0x0, CAN_STANDARD_FILTER_SIZE);
	memset(ExtIdFilterAddr, 0x0, CAN_EXTENDED_FILTER_SIZE);
	memset(RxBuffAddr, 0x0, CAN_RX_BUFFER_MAX_SIZE);
	memset(RxFifoAddr, 0x0, CAN_RX_SHADOW_SIZE);

	UnitPtr = &CanUnitInfo[HwIndex];

	/* Initialize Unitptr contents to zero*/
	memset(UnitPtr, 0x0, sizeof(CanUnitType));

	/* SWS_CAN_00259 Req-4.3.1*/
	UnitPtr->State 					= CAN_CS_STOPPED;
	UnitPtr->PendingStateIndication = CAN_CTRL_INDICATION_NONE;
	UnitPtr->LockCount 				= 0x0;

	/* Clear stats*/
	#if (SUPPORT_CAN_STATISTICS == STD_ON)
	memset(&UnitPtr->Stats, 0x0, sizeof(Can_StatisticsType));
	#endif

	/* Loop for all hardware objects configured for transmission and store controller and HOH reference*/
	for(i = 0x0; i < CtrlCfgPtr->CanHoh->CanHwObjectCount; i++)
	{
		if(CtrlCfgPtr->CanHoh->CanObjectType == TRANSMIT)
		{
			Can_Global.CanHTHMap[j].CanControllerRef = CtrlCfgPtr->CanControllerId;
			Can_Global.CanHTHMap[j].CanHOHRef = &CtrlCfgPtr->CanHoh[i];
			j++;
		}
	}
	/* Reset CAN module*/
	Can_lclReset((uint8)CtrlCfgPtr->CanControllerId);

	/* Get the controller in Config-mode*/
	Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

	/* Check if CAN FD is supported*/
	#if (CAN_FD_SUPPORTED == STD_ON)
	if((Can_Hw_Reg[HwIndex]->PCR >> CAN_REG_PCR_FDD_SHIFT) == 0x0)
	{
		/* Enable CAN-FD operations and Bitrate switching */
		Can_Hw_Reg[HwIndex]->PCR |= 0xC;
		/* Configure Rx data field size*/
		Can_Hw_Reg[HwIndex]->RXESCR = CAN_TX_RX_BUF_FIFO_DATA_SZ_64 << CAN_REG_RXESCR_RXFDS_SHIFT;
	}
	#endif

	/* Set baud rate*/
	Can_Hal_SetBaudrate(HwIndex, CtrlCfgPtr->CanControllerDefaultBaudrate->CanControllerBaudRateConfigID);

	/* Get the controller in Config-mode again*/
	Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

	/* Configure TX and RX Fifo size:(FD - 64) (BASIC - 40), Fifo_operation_mode:Blocking, Watermark:64*/
	TempVal = ((uint32)(CAN_TX_FIFO_WATERMARK &(CAN_FIFO_WATERMARK_MASK)) << CAN_TX_RX_WATERMARK_SHIFT) |
	((uint32)(CAN_TX_RX_FIFO_SIZE) << CAN_TX_RX_FIFO_SIZE_SHIFT);
	Can_Hw_Reg[HwIndex]->TXFCFGR = TempVal;
	Can_Hw_Reg[HwIndex]->TXEFCFGR = TempVal;
	TempVal = ((uint32)(CAN_RX_FIFO_WATERMARK &(CAN_FIFO_WATERMARK_MASK)) << CAN_TX_RX_WATERMARK_SHIFT) |
	((uint32)(CAN_TX_RX_FIFO_SIZE) << CAN_TX_RX_FIFO_SIZE_SHIFT);
	Can_Hw_Reg[HwIndex]->RXFCFGR = TempVal;

	/* Enable Automatic Re-transmission*/
	Can_Hw_Reg[HwIndex]->PCR |= 0x1;

	/* Get the controller in Normal-mode*/
	Can_lclConfigControllerMode(HwIndex, CAN_NORMAL_MODE);

	/* Enable Required interrupts if polling mode is not supported*/
	#if((CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_OFF) || (CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_OFF) || (CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_OFF))
	/* SWS_Can_00204 Req-4.3.1*/
	/* Disable interrupts*/
	if(CanUnitInfo[HwIndex].LockCount == 0)
	{
		Can_Hal_DisableControllerInterrupts(HwIndex);
		CanUnitInfo[HwIndex].LockCount++;
	}
	if(CanUnitInfo[HwIndex].LockCount == 0)
	{
		/** SWS_CAN_00208 Req-4.3.1*/
		/* Do nothing */
	} else {
		/* Enable interrupts*/
		CanUnitInfo[HwIndex].LockCount--;
		if(CanUnitInfo[HwIndex].LockCount == 0)
		{
			Can_Hal_EnableControllerInterrupts(HwIndex);
		}
	}
	#endif

	/* check for all Hardware Objects */
	for(i=0; i<CtrlCfgPtr->CanHoh->CanHwObjectCount;i++ )
	{
		if((HwIndex == CtrlCfgPtr->CanHoh[i].CanControllerId ) && (RECEIVE == CtrlCfgPtr->CanHoh[i].CanObjectType) )
		{
			/* Configure Standard ID filter elements*/
			if(CtrlCfgPtr->CanHoh[i].CanHwFilter->CanStdFilterElements != NULL_PTR)
			{
				/* Temporary pointers for easy access*/
				const Can_Alps_StdFilterElementType *Can_StdFilterElementPtr = CtrlCfgPtr->CanHoh[i].CanHwFilter->CanStdFilterElements;
				for(j = 0; j < CtrlCfgPtr->CanHoh[i].CanHwFilter->NumOfStdIdFilters; j++)
				{
					StdIdFilterEle = ((((uint32)Can_StdFilterElementPtr->FilterType & CAN_FILTER_TYPE_MASK) << CAN_STD_ID_FILTER_SFT_SHIFT)
					| ((uint32)CAN_STORE_RX_FRAME_IN_FIFO << CAN_STD_ID_FILTER_SFEC_SHIFT)
					| ((Can_StdFilterElementPtr->Id0 & CAN_STD_ID_FILTER_ID_MASK) << CAN_STD_ID_FILTER_ID_SHIFT)
					| ((Can_StdFilterElementPtr->Id1 & CAN_STD_ID_FILTER_ID_MASK)));

					/* Configure Standard ID Filter element*/
					*StdIdFilterAddr = StdIdFilterEle;
					/* Increment address pointer*/
					StdIdFilterAddr ++;
					/* Increment pointer to get the next info*/
					Can_StdFilterElementPtr ++;

					/*record the index in the filter setting for the HOH*/
					CANIDStd_Index[HwIndex][idx1] = CtrlCfgPtr->CanHoh[i].CanObjectId ;
					idx1++;
				}
			}

			/* Configure Extended ID filter element*/
			if(CtrlCfgPtr->CanHoh[i].CanHwFilter->CanExtFilterElements != NULL_PTR)
			{
				/* Temporary pointers for easy access*/
				const Can_Alps_ExtFilterElementType *Can_ExtFilterElementPtr = CtrlCfgPtr->CanHoh[i].CanHwFilter->CanExtFilterElements;
				for(j = 0; j < CtrlCfgPtr->CanHoh[i].CanHwFilter->NumofExtIdFilters; j++)
				{
					ExtIdFilterEle0 = ((((uint32)Can_ExtFilterElementPtr->FilterType & CAN_FILTER_TYPE_MASK) << CAN_EXT_ID_FILTER_SFT_SHIFT)
					| ((uint32)Can_ExtFilterElementPtr->Id0 & CAN_EXT_ID_FILTER_ID_MASK));
					ExtIdFilterEle1 = (((uint32)CAN_STORE_RX_FRAME_IN_FIFO << CAN_EXT_ID_FILTER_SFEC_SHIFT)
					| ((uint32)Can_ExtFilterElementPtr->Id1 & CAN_EXT_ID_FILTER_ID_MASK));

					/* Configure Extended Filter ID 0 element*/
					*ExtIdFilterAddr = ExtIdFilterEle0;
					/* Increament address pointer*/
					ExtIdFilterAddr ++;
					/* Configure Extended Filter ID 1 element*/
					*ExtIdFilterAddr = ExtIdFilterEle1;

					/* Increament address pointer*/
					ExtIdFilterAddr ++;
					/* Increament pointer to get the next info*/
					Can_ExtFilterElementPtr ++;

					/*record the index in the filter setting for the HOH*/
					CANIDExt_Index[HwIndex][idx2] = CtrlCfgPtr->CanHoh[i].CanObjectId ;
					idx2++;
				}
			}
		}//end of
		else
		{
			/*compliant to misra-C*/
		}
	}//end of for(i=0; i<CtrlCfgPtr->CanHoh->CanHwObjectCount;i++ )

	/*one more filter for buffer*/
	*StdIdFilterAddr = ((((uint32)CAN_FILTER_RANGE & CAN_FILTER_TYPE_MASK) << CAN_STD_ID_FILTER_SFT_SHIFT)
					| ((uint32)CAN_STORE_RX_FRAME_IN_BUF << CAN_STD_ID_FILTER_SFEC_SHIFT)
					| ((0 & CAN_STD_ID_FILTER_ID_MASK) << CAN_STD_ID_FILTER_ID_SHIFT)
					| ((0x7ff & CAN_STD_ID_FILTER_ID_MASK)));

	*ExtIdFilterAddr = ((((uint32)CAN_FILTER_RANGE & CAN_FILTER_TYPE_MASK) << CAN_EXT_ID_FILTER_SFT_SHIFT)
	| ((uint32)0 & CAN_EXT_ID_FILTER_ID_MASK));
	ExtIdFilterAddr++;
	*ExtIdFilterAddr = (((uint32)CAN_STORE_RX_FRAME_IN_BUF << CAN_EXT_ID_FILTER_SFEC_SHIFT)
	| ((uint32)0x1fffffff & CAN_EXT_ID_FILTER_ID_MASK));

	/* Get the controller in Config-mode again*/
	Can_lclConfigControllerMode(HwIndex, CAN_CONFIG_MODE);

	/* Configure Acceptance Filter Register*/
	TempVal = (((uint32)CtrlCfgPtr->AlpsExtRejectNoMatch << CAN_REG_IDFCR_RNMFE_SHIFT)
	| ((uint32)(idx2+1) << CAN_REG_IDFCR_XIDFS_SHIFT)
	| ((uint32)CtrlCfgPtr->AlpsStdRejectNoMatch << CAN_REG_IDFCR_RNMFS_SHIFT)
	| (idx1+1));
	Can_Hw_Reg[HwIndex]->IDFCR |= (uint32)TempVal;

	/* Get the controller in Normal-mode*/
	Can_lclConfigControllerMode(HwIndex, CAN_NORMAL_MODE);

	UnitPtr->State = CAN_CS_STARTED;
}


/*******************************************************************************
*  Service name    : Can_lclGetDataFieldLength
*  Syntax          : static uint8 Can_lclGetDataFieldLength(uint8 Length)
*  Parameters (in) : Length - Length in bytes
*  Parameter (out) : --
*  Return value    : uint8 hardware expected Length
*  Description     : Returns hardware expected Length to configure DLC field while Transmission
*******************************************************************************/
static uint8 Can_lclGetDataFieldLength(uint8 Length) {
	uint8 DataFieldLength = 0x0;

	if(Length <= 8)
	{
		DataFieldLength = 0;
	}
	else if((Length > 8) && (Length <= 12))
	{
		DataFieldLength = 1;
	}
	else if((Length > 12) && (Length <= 16))
	{
		DataFieldLength = 2;
	}
	else if((Length > 16) && (Length <= 20))
	{
		DataFieldLength = 3;
	}
	else if((Length > 20) && (Length <= 24))
	{
		DataFieldLength = 4;
	}
	else if((Length > 24) && (Length <= 32))
	{
		DataFieldLength = 5;
	}
	else if((Length > 32) && (Length <= 48))
	{
		DataFieldLength = 6;
	}
	else if((Length > 48) && (Length <= 64))
	{
		DataFieldLength = 7;
	}
	else
	{
		/*compliant to misra-c*/
	}

	return DataFieldLength;
}

/*******************************************************************************
*  Service name    : Can_lclGetPduLength
*  Syntax          : static uint8 Can_lclGetPduLength(uint8 Length)
*  Parameters (in) : Length - Length in bytes
*  Parameter (out) : --
*  Return value    : uint8 hardware expected Length
*  Description     : Returns hardware expected Length to configure DLC field while Transmission
*******************************************************************************/
static uint8 Can_lclGetPduLength(uint8 Length) {
	uint8 DataLength = 0x0, dlc = 0;

	if(Length <= 8)
	{
		DataLength = Length;
	}
	else if((Length > 8) && (Length <= 12))
	{
		DataLength = 12;
	}
	else if((Length > 12) && (Length <= 16))
	{
		DataLength = 16;
	}
	else if((Length > 16) && (Length <= 20))
	{
		DataLength = 20;
	}
	else if((Length > 20) && (Length <= 24))
	{
		DataLength = 24;
	}
	else if((Length > 24) && (Length <= 32))
	{
		DataLength = 32;
	}
	else if((Length > 32) && (Length <= 48))
	{
		DataLength = 48;
	}
	else if((Length > 48) && (Length <= 64))
	{
		DataLength = 64;
	}
	else
	{
		/* compliant to misra-c */
	}

	if((DataLength == 32) || (DataLength == 48) || (DataLength == 64))
	{
		dlc = 11 + (DataLength >> 4);
	} else if((DataLength == 12) || (DataLength == 16) || (DataLength == 20) || (DataLength == 24))
	{
		dlc = 6 + (DataLength >> 2);
	}else if(DataLength <= 8)
	{
			dlc = DataLength;
	}
	else
	{
		/* compliant to misra-c*/
	}
	return dlc;
}



/*******************************************************************************
*  Service name    : Can_lclGetDataWordLength
*  Syntax          : static uint8 Can_lclGetDataWordLength(uint8 Length)
*  Parameters (in) : Length - Data Length code
*  Parameter (out) : --
*  Return value    : uint8 hardware expected Length
*  Description     : Returns Data word Length based on DLC field of received frame
*******************************************************************************/
static uint8 Can_lclGetDataWordLength(uint8 Length) {
	uint8 updated_length = 0x0;

	#if (CAN_FD_SUPPORTED == STD_OFF)
	if (Length > 8)
	{
		Length = 8;
	}
	#else
	if (Length > 15)
	{
		Length = 15;
	}
	#endif

	if((Length == 15) || (Length == 14) || (Length == 13))
	{
		updated_length = (Length - 11) << 4;
	} else if((Length == 12) || (Length == 11) || (Length == 10) || (Length == 9)){
		updated_length = (Length - 6) << 2;
	} else if(Length <= 8)
	{
		updated_length = Length;
	}
	else
	{
		/*compliant to misra-c*/
	}

	return ((uint8)(updated_length)); /*In words[sizeof(uint32)]*/
}

/*******************************************************************************
*  Service name    : Can_lclIsrBusoff
*  Syntax          : static void Can_lclIsrBusoff(uint8 ControllerId)
*  Parameters (in) : Controller - CAN Controller ID
*  Parameter (out) : --
*  Return value    : --
*  Description     : Handels ISR Bus-off event
*******************************************************************************/
static void Can_lclIsrBusoff(uint8 ControllerId) {
	/** SWS_CAN_00020 Req 4.3.1*/
	/** SWS_CAN_00272 Req 4.3.1*/
	/* Get the controller in stopped state*/
	(void) Can_Hal_SetControllerMode(ControllerId, CAN_CS_STOPPED);

	#if (SUPPORT_CAN_STATISTICS == STD_ON)
	CanUnitType *UnitPtr = &CanUnitInfo[ControllerId];
	const Can_ControllerConfigType *CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[ControllerId];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
	/* Increament statistics*/
	UnitPtr->Stats.TxErrorCount = (Can_Hw_Reg[HwIndex]->ECR & 0xFF);
	UnitPtr->Stats.RxErrorCount = ((Can_Hw_Reg[HwIndex]->ECR & 0xFF00) >> 8);
	UnitPtr->Stats.BusOffCount++;
	#endif

	/* Indicates a busoff event for CANIF when controller has stopped*/
	CanIf_ControllerBusOff(ControllerId);
}

/*******************************************************************************
*  Service name    : Can_lclIsrTx
*  Syntax          : static void Can_lclIsrTx(uint8 ControllerId)
*  Parameters (in) : Controller - CAN Controller ID
*  Parameter (out) : --
*  Return value    : --
*  Description     : Handels ISR Transmission
*******************************************************************************/
static void Can_lclIsrTx(uint8 ControllerId) {
	/* SWS_CAN_00033 Req 4.3.1*/
	PduIdType pduId;

	/* SWS_CAN_00276 Req 4.3.1*/
	/* Get the oldest TxPdu from the buffer */
	if( E_OK == Can_lclGetPduHandleData(ControllerId, &pduId))
	{
		/* SWS_CAN_00016 Req 4.3.1*/

		if(rx_isr_free_counter[ControllerId] >= 32u)
		{
			 // nothing to do , the canif will not get a confirmation
			rx_isr_free_counter[ControllerId] = 0;
		}
		else
		{
			CanIf_TxConfirmation(pduId);
		}

 		Can_Hw_Reg[ControllerId]->IE &= 0xFFFEFFFF; 	//Message RAM Access Failure interrupt disable
		Fifo_Pend[ControllerId].cnt = 0;
	}
	else
	{
		CanUnitType *UnitPtr = &CanUnitInfo[ControllerId];
		/* Reset the PduHandledata buffer */
		Can_lclResetPduHandleData(UnitPtr);
	}
}

/*******************************************************************************
*  Service name    : Can_lclIsrRx
*  Syntax          : static void Can_lclIsrRx(uint8 ControllerId)
*  Parameters (in) : Controller - CAN Controller ID
*  Parameter (out) : --
*  Return value    : --
*  Description     : Handels ISR Reception
*******************************************************************************/
static void Can_lclIsrRx(uint8 ControllerId) {
	/* Filtered messages are configured to be stored in RX FIFO*/
	const Can_ControllerConfigType *CtrlCfgPtr = &Can_Global.config->CanConfigSet->CanController[ControllerId];
	uint8 HwIndex = CtrlCfgPtr->CanControllerId;
#if (SUPPORT_CAN_STATISTICS == STD_ON)
	uint8 FifoLevelCnt = (uint8)((Can_Hw_Reg[HwIndex]->RXFSTAR & CAN_REG_RXFSTAR_RXFLV_MASK) >> CAN_REG_RXFSTAR_RXFLV_SHIFT);
#endif
	CanUnitType *UnitPtr = &CanUnitInfo[ControllerId];
	Can_HwType *CanHwTypeObjPtr = &UnitPtr->CanHwTypeObj;
	PduInfoType *PduInfoObjPtr = &UnitPtr->PduInfoObj;

	uint8 Datalength = 0;
	uint32 RxHeader0, RxHeader1, CanId;
	uint8 MatchingHohIdx;
	static volatile uint32 *RxFifoRegPtr = NULL_PTR;
	uint32 HoldRxdata[FIFO_MAX_DATA_WORD_LENGTH] = {0u};
	/*Declare a Software Rx FIFOs and perform initialization*/
	Can_Alps_RxFifo RxBuffFifo = {.R0.B = 0x0, .R1.B = 0x0};

	/* Check for Rx fifo message lost*/
	if((((Can_Hw_Reg[HwIndex]->RXFSTAR) & CAN_REG_RXFSTAR_RXFML_MASK) >> CAN_REG_RXFSTAR_RXFML_SHIFT) == 1)
	{
		#if (SUPPORT_CAN_STATISTICS == STD_ON)
		UnitPtr->Stats.RxFifofull++;
		#endif
		#if (CAN_DEV_ERROR_DETECT == STD_ON)
		/* Calls Det_ReportError from Det module */
		(void)Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_MAINFUNCTIONREAD_SID, CAN_E_DATALOST);
		#endif
	}

	/* Check for Rx Errors*/
	if(((Can_Hw_Reg[HwIndex]->ECR & CAN_REG_ECR_REC_MASK ) >> CAN_REG_ECR_REC_SHIFT) != 0)
	{
		#if (SUPPORT_CAN_STATISTICS == STD_ON)
		UnitPtr->Stats.RxErrorCount = (Can_Hw_Reg[HwIndex]->ECR & CAN_REG_ECR_REC_MASK) >> CAN_REG_ECR_REC_SHIFT;
		#endif
	}

	/* loop for all received frames*/
	for(uint8 i = 0; i < CAN_TX_RX_FIFO_SIZE; i++)
	{
		if(CAN_REG_RXFSTAR_RXFE_MASK != (uint8)((Can_Hw_Reg[HwIndex]->RXFSTAR & CAN_REG_RXFSTAR_RXFE_MASK)))
		{
			/* Get the Rx frame headers*/
			RxHeader0 = Can_Hw_Reg[HwIndex]->RXFELMR0;
			RxHeader1 = Can_Hw_Reg[HwIndex]->RXFELMR1;

			/* Fill in software Rxbuff struct*/
			RxBuffFifo.R0.B.ESI = (boolean)((RxHeader0 & CAN_RXBUFF_ESI_MASK) >> CAN_RXBUFF_ESI_SHIFT);
			RxBuffFifo.R0.B.XTD = (boolean)((RxHeader0 & CAN_RXBUFF_XTD_MASK) >> CAN_RXBUFF_XTD_SHIFT);
			RxBuffFifo.R0.B.RTR = (boolean)((RxHeader0 & CAN_RXBUFF_RTR_MASK) >> CAN_RXBUFF_RTR_SHIFT);
			RxBuffFifo.R0.B.ID =  (RxHeader0 & CAN_RXBUFF_ID_MASK);
			RxBuffFifo.R1.B.ANMF = ((RxHeader1 & CAN_RXBUFF_ANMF_MASK) >> CAN_RXBUFF_ESI_SHIFT);
			RxBuffFifo.R1.B.FIDX = ((RxHeader1 & CAN_RXBUFF_FIDX_MASK) >> CAN_RXBUFF_FIDX_SHIFT);
			RxBuffFifo.R1.B.FDF  = (boolean)((RxHeader1 & CAN_RXBUFF_FDF_MASK) >> CAN_RXBUFF_FDF_SHIFT);
			RxBuffFifo.R1.B.BRS  = (boolean)((RxHeader1 & CAN_RXBUFF_BRS_MASK) >> CAN_RXBUFF_BRS_SHIFT);
			RxBuffFifo.R1.B.DLC  = ((RxHeader1 & CAN_RXBUFF_DLC_MASK) >> CAN_RXBUFF_DLC_SHIFT);
			RxBuffFifo.R1.B.RXTS = (RxHeader1 & CAN_RXBUFF_RXTS_MASK);

			/* If Extended Id Frame is received, set the MSB bit of CanId */
			/* SWS_CAN_00423 Req-4.3.1*/
			if(RxBuffFifo.R0.B.XTD == 1)
			{
				CanId = RxBuffFifo.R0.B.ID | 0x80000000;
				/* Get the matched filter index*/
		 		MatchingHohIdx = CANIDExt_Index[HwIndex][(uint8)RxBuffFifo.R1.B.FIDX];
			}
			else
			{
				CanId = (RxBuffFifo.R0.B.ID) >> 18u;
				/* Get the matched filter index*/
		 		MatchingHohIdx = CANIDStd_Index[HwIndex][(uint8)RxBuffFifo.R1.B.FIDX];
			}

			/* If CANFD Frame is received, set the MSB-2 bit of CanId */
			/* SWS_CAN_00501 Req-4.3.1*/
			if(RxBuffFifo.R1.B.FDF == 1)
			{
				CanId = (CanId | CAN_IDTYPE_CANFD);
			}
			else
			{}

			/* Copy received data in Software Rxfifo */
			RxFifoRegPtr = &(Can_Hw_Reg[HwIndex]->RXFELMR2);/*Get the register address*/
			Datalength = Can_lclGetDataWordLength(RxBuffFifo.R1.B.DLC);

			/* Read data from Register and store temporarily,scan all FIFO */
			uint32 Idx = 0;
			for(uint8 loopIdx = 0x0; loopIdx < CAN_FIFO_ELEMENT_SIZE; loopIdx++) {
				HoldRxdata[Idx] = *RxFifoRegPtr;
				RxFifoRegPtr++;
				Idx++;
			}

			/* Change Endiness of data as expected by the hardware*/
			for(uint8 Index = 0x0; Index < (Datalength); Index = Index + 1) {

				RxBuffFifo.RX_DATA.DATA[Index] = ((uint8 *)HoldRxdata)[Index];
			}

			/* Configure required elements to indicate CANIF layer*/
			CanHwTypeObjPtr->CanId = CanId;
			CanHwTypeObjPtr->Hoh  =  MatchingHohIdx ;
			CanHwTypeObjPtr->ControllerId = ControllerId;
			PduInfoObjPtr->SduDataPtr = (uint8*)((void*)(&RxBuffFifo.RX_DATA.DATA[0]));
			PduInfoObjPtr->SduLength = Datalength;


			/* SWS_CAN_00396 Req 4.3.1, SWS_CAN_00279 Req 4.3.1, SWS_CAN_00060 Req 4.3.1*/
			CanIf_RxIndication((const Can_HwType*) CanHwTypeObjPtr, (const PduInfoType*)PduInfoObjPtr);

			rx_isr_free_counter[ControllerId]++;
		}
		else
		{}
	}

	/* Increment statistics*/
	#if (SUPPORT_CAN_STATISTICS == STD_ON)
	UnitPtr->Stats.RxSuccessCount  = UnitPtr->Stats.RxSuccessCount + FifoLevelCnt;
	#endif

}

/*******************************************************************************
*  Service name    : Can_lclGetPduHandleData
*  Syntax          : Std_ReturnType Can_lclGetPduHandleData(uint8 Controller, PduIdType *PduId)
*  Parameters (in) : Controller - CAN Controller ID
*                  : PduId - pointer to PduId type
*  Parameter (out) : --
*  Return value    : Std_ReturnType - E_OK: get Pdu handle data is sucessful
*                  : -E_NOT_OK: PduHandledata buffer Empty
*  Description     : Gets the stored PduId information.
*******************************************************************************/
static Std_ReturnType Can_lclGetPduHandleData(uint8 Controller, PduIdType *PduId)
{
	Std_ReturnType Ret_val = E_OK ;
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	if(UnitPtr->StorePduHandleIn == UnitPtr->StorePduHandleOut)
	{
		Ret_val =  E_NOT_OK; /* PduHandledata buffer Empty*/
	}
	else
	{
		*PduId = UnitPtr->StorePduHandleInfo[UnitPtr->StorePduHandleOut];
		UnitPtr->StorePduHandleOut = ((UnitPtr->StorePduHandleOut + 0x1) % CAN_TX_FIFO_BUFFER_SIZE);
	}
	return Ret_val;
}

/*******************************************************************************
*  Service name    : Can_lclStorePduHandleData
*  Syntax          : Std_ReturnType Can_lclStorePduHandleData(uint8 Controller, PduIdType PduId)
*  Parameters (in) : Controller - CAN Controller ID
*                  : PduId - Pdu Id type
*  Parameter (out) : --
*  Return value    : Std_ReturnType - E_OK: Store Pdu handle data is sucessful
*                  : -E_NOT_OK: PduHandledata buffer Full
*  Description     : Stores the PduId
*******************************************************************************/
static Std_ReturnType Can_lclStorePduHandleData(uint8 Controller, PduIdType PduId)
{
	Std_ReturnType Ret_val = E_OK ;
	CanUnitType *UnitPtr = &CanUnitInfo[Controller];

	if(UnitPtr->StorePduHandleIn == (((UnitPtr->StorePduHandleOut + CAN_TX_FIFO_BUFFER_SIZE) - 1u) % CAN_TX_FIFO_BUFFER_SIZE))
	{
		Ret_val = E_NOT_OK; /* PduHandledata buffer Full*/
	}
	else
	{
		UnitPtr->StorePduHandleInfo[UnitPtr->StorePduHandleIn] = PduId;
		UnitPtr->StorePduHandleIn = (UnitPtr->StorePduHandleIn + 1) % CAN_TX_FIFO_BUFFER_SIZE;
	}
	return Ret_val;
}

/*******************************************************************************
*  Service name    : Can_lclResetPduHandleData
*  Syntax          : void Can_lclResetPduHandleData(CanUnitType* UnitPtr
*  Parameters (in) : UnitPtr - Holds Controller Information
*  Parameter (out) : --
*  Return value    : --
*  Description     : Resets both the StorePduHandle Related pointers.
*******************************************************************************/
static void Can_lclResetPduHandleData(CanUnitType* UnitPtr){
	/* Resets the StorePduHandle pointers*/
	UnitPtr->StorePduHandleIn = 0x0;
	UnitPtr->StorePduHandleOut = 0x0;
}



/*******************************************************************************
*  Service name    : Can_lclChangeEndianess
*  Syntax          : static void Can_lclChangeEndianess(uint8 Idx, uint8 *Source, uint8 *Destination)
*  Parameters (in) : Idx - Index
*                  : Source - Source address of data
*                  : Destination - Destination address where updated data needs to be stored
*  Parameter (out) : --
*  Return value    : --
*  Description     : Change the Endianess of as required by the hardware structure.
*******************************************************************************/
static void Can_lclChangeEndianess(uint8 Idx, uint8 *Source, uint8 *Destination) {
	Destination[Idx] 	 = Source[Idx + 3] ;
	Destination[Idx + 1] = Source[Idx + 2];
	Destination[Idx + 2] = Source[Idx + 1];
	Destination[Idx + 3] = Source[Idx];
}



/*******************************************************************************
*  Service name    : Can_Isr
*  Syntax          : void Can_Isr(uint8 CanControllerId)
*  Parameters (in) : CanControllerId - Can Controller Id
*  Parameter (out) : --
*  Return value    : --
*  Description     : Interrupt Handeler
*******************************************************************************/
void Can_Isr(uint8 CanControllerId)
{
	/* Get valid interrupts*/
	uint32 interruptStatusReg = Can_Hw_Reg[CanControllerId]->IR;
	uint32 interruptEnableReg = Can_Hw_Reg[CanControllerId]->IE;
	uint32 interruptVector = interruptStatusReg & interruptEnableReg;

	/* Disable all the interrupts*/
	Can_Hw_Reg[CanControllerId]->IE = 0x0;

	/* Busoff check. Get Interrupt Status Register and Interrupt Enable Register.*/
	if ((interruptVector & 0x400000) != 0u)
	{
		Can_lclIsrBusoff(CanControllerId);
	}

	/* Tx check. Get Interrupt Status Register and Interrupt Enable Register. */
	else if ((interruptVector & 0x100) != 0u)
	{
		Can_lclIsrTx(CanControllerId);
	}

	/* Rx check. Get Interrupt Status Register and Interrupt Enable Register. */
	else if ((interruptVector & 0x800) != 0u)
	{
		Can_lclIsrRx(CanControllerId);
	}

	else if((interruptVector & 0x10000) != 0u)  	 //Message RAM Access Failure interrupt
	{
		rx_isr_free_counter[CanControllerId]++;
	}

	else
	{
		/*compliant to misra-c*/
	}

	/* SWS_Can_00420 Req 4.3.1*/
	/* Reset the interrupt flag at the end of the ISR */
	Can_Hw_Reg[CanControllerId]->IR = interruptVector;

	/*Re-enable all the required interrupts*/
	/* SWS_Can_00204 Req-4.3.1*/
	/* Disable interrupts*/
	if(CanUnitInfo[CanControllerId].LockCount == 0)
	{
		Can_Hal_DisableControllerInterrupts(CanControllerId);
		CanUnitInfo[CanControllerId].LockCount++;
	}
	if(CanUnitInfo[CanControllerId].LockCount == 0)
	{
		/** SWS_CAN_00208 Req-4.3.1*/
		/* Do nothing */
	} else {
		/* Enable interrupts*/
		CanUnitInfo[CanControllerId].LockCount--;
		if(CanUnitInfo[CanControllerId].LockCount == 0)
		{
			Can_Hal_EnableControllerInterrupts(CanControllerId);
		}
	}
}


#define CAN_STOP_SEC_CODE
