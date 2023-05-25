/*============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
=============================================================================*/
#ifndef CAN_ALPS_H
#define CAN_ALPS_H

#include <string.h>


/* MemoryMapped Address*/
#define CAN0_BASE_ADDRESS 0xBB0000U
#define CAN1_BASE_ADDRESS 0xBC0000U
#define CAN_TX_BUFFER_BASE_ADDRESS 0x1000U
#define CAN_RX_BUFFER_BASE_ADDRESS 0x1C00U
#define CAN_RX_SHADOW_ADDRESS 0x2000U
#define CAN_STANDARD_ID_FILTER_BASE_ADDRESS 0x2400U
#define CAN_EXTENDED_ID_FILTER_BASE_ADDRESS 0x2800U

#define CAN_STANDARD_FILTER_SIZE 0x1FC
#define CAN_EXTENDED_FILTER_SIZE 0x1FC
#define CAN_RX_BUFFER_MAX_SIZE 0x3FC
#define CAN_RX_SHADOW_SIZE 0x44

/* Register shifts*/
#define CAN_REG_MCR_CONFIG_SHIFT 1
#define CAN_REG_PCR_FDD_SHIFT 31
#define CAN_REG_PCR_BRS_SHIFT 2
#define CAN_REG_PCR_FDO_SHIFT 3
#define CAN_TX_RX_WATERMARK_SHIFT 24
#define CAN_TX_RX_FIFO_SIZE_SHIFT 16
#define CAN_REG_DBTCR_DSJW_SHIFT 24
#define CAN_REG_DBTCR_DBRP_SHIFT 16
#define CAN_REG_DBTCR_DTSEG1_SHIFT 8
#define CAN_REG_NBTCR_NSJW_SHIFT 25
#define CAN_REG_NBTCR_NBRP_SHIFT 16
#define CAN_REG_NBTCR_NTSEG1_SHIFT 8
#define CAN_REG_IDFCR_RNMFE_SHIFT 25  
#define CAN_REG_IDFCR_XIDFS_SHIFT 16  
#define CAN_REG_IDFCR_RNMFS_SHIFT 9  
#define CAN_REG_PSR_BO_SHIFT 27
#define CAN_REG_MCR_SPACK_SHIFT 5
#define CAN_REG_TXFSTAR_TXFF_SHIFT 6
#define CAN_REG_TXFSTAR_TXFLV_SHIFT 8
#define CAN_REG_RXFSTAR_RXFLV_SHIFT 8
#define CAN_REG_RXFSTAR_RXFE_SHIFT 5
#define CAN_REG_TXESCR_TXFDS_SHIFT 0x4
#define CAN_REG_RXESCR_RXFDS_SHIFT 0x4
#define CAN_REG_ECR_REC_SHIFT 8
#define CAN_REG_TXFSTAR_TXFGI_SHIFT 16
#define CAN_REG_TXFSTAR_TXFLV_SHIFT 8
#define CAN_REG_RXFSTAR_RXFML_SHIFT 7

#define CAN_TXBUFF_STANDARD_ID_SHIFT 18
#define CAN_TXBUFF_EXTENDED_ID_SHIFT 11
#define CAN_RXBUFF_ESI_SHIFT 31
#define CAN_RXBUFF_XTD_SHIFT 30
#define CAN_RXBUFF_RTR_SHIFT 29
#define CAN_RXBUFF_ESI_SHIFT 31
#define CAN_RXBUFF_FIDX_SHIFT 24
#define CAN_RXBUFF_FDF_SHIFT 21
#define CAN_RXBUFF_BRS_SHIFT 20
#define CAN_RXBUFF_DLC_SHIFT 16
#define CAN_STD_ID_FILTER_SFT_SHIFT 30
#define CAN_STD_ID_FILTER_SFEC_SHIFT 27
#define CAN_STD_ID_FILTER_ID_SHIFT 16
#define CAN_EXT_ID_FILTER_SFT_SHIFT 30
#define CAN_EXT_ID_FILTER_SFEC_SHIFT 29
#define CAN_EXT_ID_FILTER_RESERVE_SHIFT 29
#define CAN_EXT_ID_FILTER_ID_SHIFT 16


/* Register Masks*/
#define CAN_REG_MCR_CFG_MASK 0xFFFFFFFD
#define CAN_REG_MCR_LOOPBACK_MASK 0xFFFFFFFB
#define CAN_REG_MCR_MONITOR_MASK 0xFFFFFFF7
#define CAN_REG_MCR_ROPT_MASK 0xFFFFFFEF
#define CAN_REG_MCR_SLEEP_MASK 0xFFFFFFBF
#define CAN_REG_MCR_TEST_MASK 0xFFFFFF7F
#define CAN_REG_PSR_BO_MASK 0x08000000
#define CAN_REG_MCR_SPACK_MASK 0x00000020
#define CAN_REG_TXFSTAR_TXFF_MASK 0x00000040
#define CAN_REG_TXFSTAR_TXFLV_MASK 0x0000FF00
#define CAN_REG_RXFSTAR_RXFLV_MASK 0xFF00
#define CAN_REG_RXFSTAR_RXFE_MASK 0x20
#define CAN_REG_PCR_FDO_MASK 0x8
#define CAN_REG_PCR_BRS_MASK 0x4
#define CAN_REG_TXFSTAR_TXFACK_MASK 0x1
#define CAN_STANDARD_ID_MASK 0x7FF
#define CAN_EXTENDED_ID_MASK 0x1FFFFFFF
#define CAN_REG_ECR_TEC_MASK 0xFF
#define CAN_REG_MCR_CONFIG_MASK 0x2
#define CAN_REG_TXFSTAR_TXFGI_MASK 0xFF0000
#define CAN_REG_RXFSTAR_RXFML_MASK 0x80
#define CAN_REG_ECR_REC_MASK 0x7F00

#define CAN_STD_ID_FILTER_ID_MASK 0x7FF
#define CAN_EXT_ID_FILTER_ID_MASK 0x3FFFFFFF
#define CAN_FILTER_RANGE     0x1
#define CAN_FILTER_TYPE_MASK 0x3
#define CAN_RXBUFF_ESI_MASK 0x80000000
#define CAN_RXBUFF_XTD_MASK 0x40000000
#define CAN_RXBUFF_RTR_MASK 0x20000000
#define CAN_RXBUFF_ID_MASK 0x1FFFFFFF
#define CAN_RXBUFF_ANMF_MASK 0x80000000
#define CAN_RXBUFF_FIDX_MASK 0x7F000000
#define CAN_RXBUFF_FDF_MASK 0x00200000
#define CAN_RXBUFF_BRS_MASK 0x00100000
#define CAN_RXBUFF_DLC_MASK 0x000F0000
#define CAN_RXBUFF_RXTS_MASK 0xFFFF
#define CAN_IDTYPE_EXTEND    0x80000000
#define CAN_IDTYPE_CANFD     0x40000000

/* General Macros*/
#define CAN_ENABLE_INTERRUPT_LINE_0 0
#define CAN_REG_FDO_BRS_SET 0xC
#define CAN_TX_RX_BUF_FIFO_DATA_SZ_64 0x7
#define CAN_TX_RX_WATERMARK_DISABLE 0x0
#define FIFO_MAX_DATA_WORD_LENGTH 18
#define CAN_STORE_RX_FRAME_IN_FIFO 0x0
#define CAN_STORE_RX_FRAME_IN_BUF  0x1
#define CAN_RX_FIFO_WATERMARK 1
#define CAN_TX_FIFO_WATERMARK 1
#define CAN_FIFO_WATERMARK_MASK 0xFF

/*Alps Requirement: If CAN FD is supported TX BUFFER size: 14,
*If CAN FD is not supported TX BUFFER size : 64*/
#if (CAN_FD_SUPPORTED == STD_ON)
#define CAN_TX_RX_FIFO_SIZE    14
#define CAN_FIFO_ELEMENT_SIZE  16
#endif
#if (CAN_FD_SUPPORTED == STD_OFF)
#define CAN_TX_RX_FIFO_SIZE 64
#define CAN_FIFO_ELEMENT_SIZE  2
#endif
/******************************************************************************
*                           Type Definitions
******************************************************************************/
/* CAN Controller Modes*/
typedef enum {
    CAN_NORMAL_MODE = 0,
    CAN_CONFIG_MODE,
    CAN_RESTRICTED_MODE,
    CAN_MONITOR_MODE,
    CAN_SLEEP_MODE,
    CAN_TEST_MODE,
    CAN_LOOP_MODE
} Can_Mode;


/* CAN Fifo modes*/
typedef enum {
    FIFO_BLOCK_MODE = 0,
    FIFO_OVERWRITE_MODE,
    FIFO_INVALID_MODE
} Can_FifoMode;


/* FIFO Operation Mode*/
typedef enum {
    CAN_FIFOBLOCKINGMODE,
    CAN_FIFOOVERWRITEMODE
} FifoOperationMode;

/* Type for holding global information used by the driver */
typedef struct {
    Can_DriverStateType initRun;  /* Driver States */
    const Can_ConfigType *config; /* Pointer to config */

    /* This is a map that maps the HTHs with the Controller and Hoh. It is built
    * during Can_Init and is used to make things faster during a transmit.*/
    Can_ObjectHOHMapType CanHTHMap[CAN_NUM_OF_HTHS];
    
} Can_GlobalType;

typedef struct {
	uint32 Fifo_Levl;
	uint8  cnt;
}Can_Fifo_PendType;

/******************************************************************************
*                         Public Function Declarations                                    
******************************************************************************/
#define CAN_START_SEC_CODE
void Can_Hal_Init(const Can_ConfigType *ConfigPtr);
Std_ReturnType Can_Hal_DeInit(void);
Std_ReturnType Can_Hal_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);
void Can_Hal_EnableControllerInterrupts(uint8 HwIndex);
void Can_Hal_DisableControllerInterrupts(uint8 Controller);
void Can_Hal_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);
void Can_Hal_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr); 
Std_ReturnType Can_Hal_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);
Std_ReturnType Can_Hal_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo);
void Can_Hal_MainFunctionBusOff();
void Can_Hal_MainFunctionWrite();
void Can_Hal_MainFunctionRead();
void Can_Hal_MainFunctionMode();
void Can_Isr(uint8 CanControllerId);
/******************************************************************************
*                    Private Function Declaration                             
******************************************************************************/
/*******************************************************************************
*  Service name    : Can_lclParamCheck                                            
*  Syntax          : void Can_lclParamCheck(boolean expr,                         
*                                           uint8 api_id,                          
*                                           uint8 err_id,                          
*                                           Std_ReturnType *RetVal)                    
*  Parameters (in) : expr - TRUE or FALSE                                       
*                    api_id - API ID                                            
*                    err_id - ERROR ID                                          
*  Parameter(inout): RetVal                                                         
*  Return value    : None                                                       
*  Description     : Reports Det error if CAN_DEV_ERROR_DETECT == STD_ON.       
*******************************************************************************/
void Can_lclParamCheck(boolean expr, uint8 api_id, uint8 err_id, Std_ReturnType *RetVal);
#define CAN_STOP_SEC_CODE
#endif
