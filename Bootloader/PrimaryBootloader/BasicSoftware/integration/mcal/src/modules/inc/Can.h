/*============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
=============================================================================*/
#ifndef CAN_H
#define CAN_H

#include "Platform_Types.h"
#include "ComStack_Types.h"
#include "Can_GeneralTypes.h"
#include "Std_Types.h"

/* Macro definitions for API Service ID */
#define CAN_INIT_SID                        0x00u   /* Service Id for CAN_Init API */
#define CAN_DEINIT_SID                      0x10u   /* Service Id for CAN_DeInit API */
#define CAN_GETVERSIONINFO_SID              0x07u   /* Service Id for CAN_DeInit API */
#define CAN_SETBAUDRATE_SID                 0x0fu  
#define CAN_SETCONTROLLERMODE_SID           0x03u
#define CAN_DISABLECONTROLLERINTERRUPTS_SID 0x04u
#define CAN_ENABLECONTROLLERINTERRUPTS_SID  0x05u
#define CAN_GETCONTROLLERERRORSTATE_SID     0x11u
#define CAN_GETCONTROLLERMODE_SID           0x12u
#define CAN_WRITE_SID                       0x06u
#define CAN_MAINFUNCTIONWRITE_SID           0x01u
#define CAN_MAINFUNCTIONREAD_SID            0x08u
#define CAN_MAINFUNCTIONBUSOFF_SID          0x09u
#define CAN_MAINFUNCTIONMODE_SID            0X0cu

/* Macro definitions for Error Codes specified by AUTOSAR */
#define CAN_E_PARAM_POINTER             0x01u /* APIs called with a wrong parameter */
#define CAN_E_PARAM_HANDLE              0x02u /* APIs called with a wrong parameter */
#define CAN_E_PARAM_DATA_LENGTH         0x03u /* API service called with wrong parameter */
#define CAN_E_PARAM_CONTROLLER          0x04u /* API service called with wrong parameter */
#define CAN_E_UNINIT                    0x05u /* API service used without module initialization */
#define CAN_E_TRANSITION                0x06u /* Invalid transition for the current mode */
#define CAN_E_PARAM_BAUDRATE            0x07u /* Parameter Baudrate has an invalid value */
#define CAN_E_INIT_FAILED               0x09u /* Invalid configuration set selection */

/* Runtime Errors*/
#define CAN_E_DATALOST                  0x01u /* Received CAN message is lost*/

/* Below macros are used to store pduhandle info */
#define CAN_TX_FIFO_BUFFER_LENGTH  64u
#define CAN_TX_FIFO_BUFFER_SIZE    15u

/* ***************************[GLOBAL STRUCTURES]****************************/
typedef enum {
    CAN_CTRL_0 = 0x0,
    CAN_CTRL_1 = 0x1,
} CanControllerIdType;

typedef enum {
	CAN_FILTER_ELEMENT_DISABLED = 0,
	CAN_RANGE_FILTER = 1,
	CAN_CLASSIC_BIT_MASK_FILTER,
	CAN_DUAL_ID_FILTER
} Can_Alps_FilterType;


/* Standard filter element */
typedef struct {
    uint32 Id0;
    uint32 Id1;
    Can_Alps_FilterType FilterType;
} Can_Alps_StdFilterElementType;


/* Extended filter elements*/
typedef struct {
    uint32 Id0;
    uint32 Id1;
    Can_Alps_FilterType FilterType;
} Can_Alps_ExtFilterElementType;

/* CAN_FSR_00323 Req-4.3.1*/
typedef enum {
    BASIC,
    FULL1 //zco7sgh
} Can_HohType;


/*CAN_FSR_00327 Req-4.3.1*/
typedef enum {
    RECEIVE,
    TRANSMIT
} Can_HohObjectType;


/* Can_IdTypeType
L-PDU identifier type Definition
Represents the Identifier of an L-PDU.*/
/* CAN_FSR_00065 Req-4.3.1*/
typedef enum {
    EXTENDED,
    MIXED,
    STANDARD
} Can_IdTypeType;


/*Used to Configure Busoff Processing*/
typedef enum {
    INTERRUPT,
    POLLING
} Can_ProcessType;


/* CAN_FSR_00473 Req-4.3.1*/
typedef struct {
    /* Specifies the buadrate of the Controller in kbps.*/
    uint16 CanControllerFdBaudRate;

    /* Specifies propagation delay in time quantas.*/
    uint8 CanControllerFdPropSeg;

    /* Specifies phase segment 1 in time quantas.*/
    uint8 CanControllerFdSeg1;

    /* Specifies phase segment 2 in time quantas.*/
    uint8 CanControllerFdSeg2;

    /* Specifies the resynchronization jump width in time quantas.*/
    uint8 CanControllerFdSyncJumpWidth;

    /* Specifies the Transceiver Delay Compensation Offset in ns. If not specified Transceiver Delay Compensation is disabled.*/
    uint32 CanControllerTrcvDelayCompensationOffset;

    /* Specifies if the bit rate switching shall be used for transmissions.If FALSE: CAN FD frames shall be sent without bit rate switching.*/
    boolean CanControllerTxBitRateSwitch;
} Can_ControllerFdBaudrateConfigType;


/* CAN_FSR_00414 Req-4.3.1*/
typedef struct {
    /* Uniquely identifies a specific baud rate configuration.*/
    uint16 CanControllerBaudRateConfigID;

    /* Specifies the buadrate of the Controller in kbps.*/
    uint16 CanControllerBaudRate;

    /* Specifies propagation delay in time quantas.*/
    uint8 CanControllerPropSeg;

    /* Specifies phase segment 1 in time quantas.*/
    uint8 CanControllerSeg1;

    /* Specifies phase segment 2 in time quantas.*/
    uint8 CanControllerSeg2;

    /* Specifies the resynchronization jump width in time quantas.*/
    uint8 CanControllerSyncJumpWidth;

    /* Specifies CAN FD baud rates */
    const Can_ControllerFdBaudrateConfigType *CanControllerFdBaudrateConfig;

} Can_ControllerBaudrateConfigType;


/* Represents pending state of CAN Controller*/
typedef enum {          
    CAN_CTRL_INDICATION_UINIT,
    CAN_CTRL_INDICATION_PENDING_START,
    CAN_CTRL_INDICATION_PENDING_STOP,
    CAN_CTRL_INDICATION_PENDING_SLEEP,
    CAN_CTRL_INDICATION_NONE
} Can_CtrlPendingStateIndicationType;

/**
* @brief   CAN Controller function related type.
* This structure record the statistic data of the CAN Controller.
*/
typedef struct {
    uint32 TxSuccessCount;
    uint32 RxSuccessCount;
    uint32 TxErrorCount;
    uint32 RxErrorCount;
    uint32 BusOffCount;
    uint32 RxFifofull;
} Can_StatisticsType;


/* Can_DriverStateType */
typedef enum {          
    CAN_UNINIT = 0,
    CAN_READY
} Can_DriverStateType;

/* CAN_FSR_00468 :This container is only valid for HRHs and contains
* the configuration (parameters) of one hardware filter.*/
typedef struct {
    /* Standard ID Filter Parameters*/
    const Can_Alps_StdFilterElementType *CanStdFilterElements;

    /* Extended ID Filter Parameters*/
    const Can_Alps_ExtFilterElementType *CanExtFilterElements; 
    
    /* Number of configured Standard Id filters*/
    uint16 NumOfStdIdFilters;
    
    /* Number of configured Extended Id filters*/
    uint16 NumofExtIdFilters;
    
} CanHwFilterType;


/* CAN_FSR_00324: Contains the configuration (parameters) of CAN Hardware Objects.*/
typedef struct {
    /* Specifies the value which is used to pad unspecified data in CAN FD.
    * If the length of a PDU which was requested to be sent does not match the allowed
    * DLC values, the remaining bytes up to the next possible value shall be padded with this value. */
    uint8 CanFdPaddingValue;

    /*Number of hardware objects used to implement one HOH*/
    uint32 CanHwObjectCount;  

    /* PC/PB, Specifies the type (Full-CAN or Basic-CAN) of a hardware object. */
    Can_HohType CanHandleType;

    /* PC/PB Holds the handle ID of HRH or HTH. The value of this parameter is unique
    *  in a given CAN Driver, and it should start with 0 and continue without any
    *  gaps. The HRH and HTH Ids are defined under two different name-spaces.
    *  Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3 */
    uint32 CanObjectId;  

    /* PC/PB, Specifies whether the IdValue is of type - standard identifier -
    * extended identifier - mixed mode Implementation Type: Can_IdType */
    Can_IdTypeType CanIdType;

    /* PC/PB, Specifies if the HardwareObject is used as Transmit or as Receive object */
    Can_HohObjectType CanObjectType;
	
	/* This parameter defines if or if not Can supports the trigger-transmit API for this handle*/
	boolean CanTriggerTransmitEnable;

    /* Pointer to the list of the Hardware Filters*/
    const CanHwFilterType *CanHwFilter;

    /* controller ID that this HOH belongs to */
    CanControllerIdType CanControllerId;
} CanHardwareObject;


/* CAN_FSR_00354: Contains the configuration parameters of the CAN controller(s).*/
typedef struct {
    /* Defines if a CAN controller is used in the configuration */
    boolean CanControllerActivation;

    /* Enables/disables API Can_MainFunction_BusOff() for handling busoff events in polling mode*/
    Can_ProcessType CanBusOffProcessing;

    /* Enables/disables API Can_MainFunction_Read() handling PDU reception events in polling mode*/ 
    Can_ProcessType CanRxProcessing;

    /* Enables/disables API Can_MainFunction_Write() for handling PDU transmission events in polling mode*/ 
    Can_ProcessType CanTxProcessing;

    /* Specifies the default configured baud rate(from the list CanControllerBaudrateConfig)*/
    const Can_ControllerBaudrateConfigType *CanControllerDefaultBaudrate;

    /* Specifies the supported baud rates*/
    const Can_ControllerBaudrateConfigType *CanControllerBaudrateConfig;

    /* Specifies number of configured baudrates in CanControllerBaudrateConfig list*/
    uint16 CanControllerSupportedBaudratesCount;

    /* Reference to the CPU clock configuration, which is set in the MCU driver configuration*/
    uint32 CanCpuClockRef;

    /* This parameter provides the controller ID which is unique in a given CAN
    * Driver. The value for this parameter starts with 0 and continue without any gaps.*/
    const CanControllerIdType CanControllerId;
    
    /* List of Hoh id's that belong to this controller*/
    const CanHardwareObject *CanHoh;    
	
	/* Reject Non-Matching Standard Id Frame*/
	boolean AlpsStdRejectNoMatch;    /* 1 - reject non matching */
    
	/* Reject Non-Matching Extended Id Frame*/
	boolean AlpsExtRejectNoMatch;    /* 1 - reject non matching */
} Can_ControllerConfigType;


/* CAN_FSR_00343: Contains the configuration parameters of the AUTOSAR CAN module.*/
typedef struct {
    const Can_ControllerConfigType *CanController;
} Can_ConfigSetType;


/* CAN_FSR_00413: Can_ConfigType*/
typedef struct {
    /* multiple configuration set container for CAN Driver*/
    const Can_ConfigSetType *CanConfigSet;
} Can_ConfigType;

#include "Can_Cfg.h"
/*=============================================================================== */

/* Type for holding references for easy access */
typedef struct {
    CanControllerIdType CanControllerRef; /* controller ID Refference*/
    const CanHardwareObject *CanHOHRef; /* hardware object Refference*/
}Can_ObjectHOHMapType;


/* Type for holding information about each Controller */
typedef struct {
	Can_ControllerStateType State; /* Represents Canif_ControllerStateType states*/

    /* Used to trace interrupt Enabling and Disabling*/
    uint32 LockCount;     

    /* State transition*/
    Can_ControllerStateType transition;

    /* Pending state indicator*/
    Can_CtrlPendingStateIndicationType PendingStateIndication;
    
    /* Store pduhandle info*/
    PduIdType StorePduHandleInfo[CAN_TX_FIFO_BUFFER_SIZE];
    uint8 StorePduHandleIn;  /* Used to store pduhandleinfo*/
    uint8 StorePduHandleOut; /* Used to get pduhandleinfo*/
    
    /* Holds info about Hardware Object Handle which is used during Reception*/
    Can_HwType CanHwTypeObj;
    
    /* Holds info about PduInfoType which is used during Reception*/
    PduInfoType PduInfoObj;
	
	/*Updated baudrate*/
	const Can_ControllerBaudrateConfigType *CanControllerUpdatedBaudrateConfig;
    
    #if (SUPPORT_CAN_STATISTICS == STD_ON)
    /* Success/Failure Statistics*/
    Can_StatisticsType Stats;
    #endif
} CanUnitType;

/*********************[TX FIFO ELEMENT]********************/
/* CAN Tx Fifo*/
typedef struct {
    union {
        uint32 R;
        struct {
            uint32 ID:29;
            uint32 RTR:1;
            uint32 XTD:1;
            uint32 ESI:1;
        } B;
    } T0;                          

    union {
        uint32 R;
        struct {
            uint32 RESERVED:16;
            uint32 DLC:4;
            uint32 BRS:1;
			uint32 FDF:1;
			uint32 RESERVED1:1;
            uint32 EFC:1;
            uint32 MM:8;
        } B;
    } T1;                          

    union {
        /*If CAN_FD_SUPPORTED is supported, length = [64] else length =[8]*/
        #if (CAN_FD_SUPPORTED == STD_ON)
        	#define FIFO_DATA_LENGTH 64
        #else
			#if (CAN_FD_SUPPORTED == STD_OFF)
				#define FIFO_DATA_LENGTH 8
			#endif
        #endif
        uint8 DATA[FIFO_DATA_LENGTH];  /* Data buffer*/
    } TX_DATA;                        
}Can_Alps_TxFifo;


/*********************[RX FIFO ELEMENT]********************/
/* CAN Rx Fifo*/
typedef struct {
    union {
        uint32 R;
        struct {
        	uint32 ID:29;
            uint32 RTR:1;
            uint32 XTD:1;
            uint32 ESI:1;
        } B;
    } R0;                          

    union {
        uint32 R;
        struct {     
            uint32 RXTS:16;
            uint32 DLC:4;
            uint32 BRS:1;
            uint32 FDF:1;
            uint32 RESERVED:2;
            uint32 FIDX:7;         
            uint32 ANMF:1;
        } B;
    } R1;                          

    union {
        /*If CAN_FD_SUPPORTED is supported, length = [64] else length =[8]*/
        #if (CAN_FD_SUPPORTED == STD_ON)
        	#define FIFO_DATA_LENGTH 64
        #else
        	#if (CAN_FD_SUPPORTED == STD_OFF)
        		#define FIFO_DATA_LENGTH 8
			#endif
        #endif
        uint8 DATA[FIFO_DATA_LENGTH];  /* Data buffer*/
    } RX_DATA;                        
}Can_Alps_RxFifo;

/******************************[STANDARD FILTER ELEMENT]*********************/
/* Standard Filter Element Structure*/
typedef struct {
    union {
        uint32 R;
        union {
            uint32 SFT:2;
            uint32 SFEC:3;
            uint32 SFID1:11;
            uint32 RES:5; /* Reserved*/
            uint32 SFID0:11;
        }B;
    }Std_Id_Filter_ele;
}Can_Alps_StdIdFilterElement;


/******************************[EXTENDED FILTER ELEMENT]**********************/
/* Extended Filter Element Structure*/
typedef struct {
    union {
        uint32 R;
        union {
            uint32 EFT:2;
            uint32 RES:1; /* Reserved*/
            uint32 EFID0:29;
            uint32 EFEC:3;
            uint32 EFID1:29;
        }B;
    }Ext_Id_Filter_ele;
}Can_Alps_ExtIdFilterElement;


/******************************************************************************
*                         Public Function Declarations                                    
******************************************************************************/
#define CAN_START_SEC_CODE

/******************************************************************************
*  Service name    : Can_Init                                                  
*  Syntax          : void Can_Init(const Can_ConfigType* ConfigPtr)          
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ConfigPtr - Pointer to CAN configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function initializes the module.                            
******************************************************************************/
extern void Can_Init(const Can_ConfigType* ConfigPtr);

#if (CAN_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name    : Can_GetVersionInfo                                                  
*  Syntax          : void Can_GetVersionInfo(Std_VersionInfoType* VersionInfo )       
*  Service ID      : 0x07                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                  
*  Parameters (out): VersionInfo - Pointer to where to store the version information of this module.                                                      
*  Return value    : None                                                      
*  Description     : This function returns the version information of this module.                            
******************************************************************************/
extern void Can_GetVersionInfo(Std_VersionInfoType* VersionInfo );
#endif

/******************************************************************************
*  Service name    : Can_DeInit                                                  
*  Syntax          : void Can_DeInit(void);          
*  Service ID      : 0x10                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : None                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function de-initializes the module.                            
******************************************************************************/
extern void Can_DeInit(void);

#if (CAN_SET_BAUDRATE_API == STD_ON)
/******************************************************************************
*  Service name    : Can_SetBaudrate                                                  
*  Syntax          : Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)         
*  Service ID      : 0x0F                                                     
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant for different Controllers. Non reentrant for the same Controller.                                             
*  Parameters (in) : Controller - CAN controller, whose baud rate shall be set
*                  : BaudRateConfigID - references a baud rate configuration by ID                 
*  Parameters (out): None                                                      
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted                                                    
*  Description     : Sets the baud rate configuration of the CAN controller.                            
******************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);
#endif

/******************************************************************************
*  Service name    : Can_SetControllerMode                                                  
*  Syntax          : Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)         
*  Service ID      : 0x03                                                      
*  Sync/Async      : ASynchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : Controller - CAN controller for which the status shall be changed   
*                  : Transition - CAN controller State transition type               
*  Parameters (out): None                                                      
*  Return value    : Std_ReturnType - E_OK: Request accepted, E_NOT_OK: Request not accepted                                                      
*  Description     : Performs software triggered state transitions of the CAN controller State machine.                            
******************************************************************************/
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);


/******************************************************************************
*  Service name    : Can_DisableControllerInterrupts                                                  
*  Syntax          : void Can_DisableControllerInterrupts(uint8 Controller)        
*  Service ID      : 0x04                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : Controller - CAN controller for which interrupts shall be disabled.                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Disables all interrupts for this CAN controller.                            
******************************************************************************/
void Can_DisableControllerInterrupts(uint8 Controller);


/******************************************************************************
*  Service name    : Can_EnableControllerInterrupts                                                  
*  Syntax          : void Can_EnableControllerInterrupts(uint8 Controller)          
*  Service ID      : 0x05                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : Controller - CAN controller for which interrupts shall be re-enabled                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Enables all allowed interrupts                      
******************************************************************************/
void Can_EnableControllerInterrupts(uint8 Controller);


/******************************************************************************
*  Service name    : Can_GetControllerErrorState                                                  
*  Syntax          : Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);          
*  Service ID      : 0x11                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ControllerId - Abstracted CanIf ControllerId which is assigned to a CAN controller, which is requested for ErrorState.                  
*  Parameters (out): ErrorStatePtr - Pointer to a memory location, where the error state of the CAN controller will be stored.                                                     
*  Return value    : Std_ReturnType - E_OK: Error state request has been accepted.
*                  : -E_NOT_OK: Error state request has not been accepted.                                 
*  Description     : This service obtains the error state of the CAN controller.                            
******************************************************************************/
Std_ReturnType Can_GetControllerErrorState( uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);


/******************************************************************************
*  Service name    : Can_GetControllerMode                                                  
*  Syntax          : Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr);          
*  Service ID      : 0x12                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : Controller - CAN controller for which the status shall be requested.                  
*  Parameters (out): ControllerModePtr - Pointer to a memory location, where the current mode of the CAN controller will be stored.                                                      
*  Return value    : Std_ReturnType - E_OK: Controller mode request has been accepted.
*                  : -E_NOT_OK: Controller mode request has not been accepted.                                                      
*  Description     : This service reports about the current status of the requested CAN controller.                            
******************************************************************************/
Std_ReturnType Can_GetControllerMode( uint8 Controller, Can_ControllerStateType* ControllerModePtr);


/******************************************************************************
*  Service name    : Can_Write                                                  
*  Syntax          : Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo );         
*  Service ID      : 0x06                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : Hth - information which HW-transmit handle shall be used for transmit.   
*                  : PduInfo - Pointer to SDU user memory, Data Length and Identifier.                 
*  Parameters (out): None                                                      
*  Return value    : Std_ReturnType - E_OK: Write command has been accepted.
*                  : -E_NOT_OK: development error occurred.
*                  : -CAN_BUSY: No TX hardware buffer available.                                                      
*  Description     : This function is called by CanIf to pass a CAN message to CanDrv for transmission.                            
******************************************************************************/
Std_ReturnType Can_Write( Can_HwHandleType Hth, const Can_PduType* PduInfo );

#if (CAN_MAIN_FUNCTION_WRITE_POLLING_ALLOWED == STD_ON)
/******************************************************************************
*  Service name    : Can_MainFunction_Write                                                  
*  Syntax          : void Can_MainFunction_Write( void );          
*  Service ID      : 0x01                                                                                                                                                 
*  Parameters (in) : None                
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Performs the polling of TX confirmation when CAN_TX_PROCESSING is set to POLLING.                            
******************************************************************************/
void Can_MainFunction_Write( void );
#endif

#if (CAN_MAIN_FUNCTION_READ_POLLING_ALLOWED == STD_ON)
/******************************************************************************
*  Service name    : Can_MainFunction_Read                                                  
*  Syntax          : void Can_MainFunction_Read( void );          
*  Service ID      : 0x08                                                                                                  
*  Parameters (in) : None                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.                            
******************************************************************************/
void Can_MainFunction_Read( void );
#endif

#if (CAN_MAIN_FUNCTION_BUSOFF_POLLING_ALLOWED == STD_ON)
/******************************************************************************
*  Service name    : Can_MainFunction_BusOff                                                  
*  Syntax          : void Can_MainFunction_BusOff( void );          
*  Service ID      : 0x09                                                                                                   
*  Parameters (in) : None                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Performs the polling of bus-off events that are configured statically as 'to be polled'.                            
******************************************************************************/
void Can_MainFunction_BusOff( void );
#endif

/******************************************************************************
*  Service name    : Can_MainFunction_Mode                                                  
*  Syntax          : void Can_MainFunction_Mode( void );         
*  Service ID      : 0x0C                                                                                                   
*  Parameters (in) : None                 
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function performs the polling of CAN controller mode transitions.                            
******************************************************************************/
void Can_MainFunction_Mode( void );


/******************************************************************************
*  Service name    : Can_MainFunction_Wakeup                                                  
*  Syntax          : void Can_MainFunction_Wakeup( void );         
*  Service ID      : 0x0A                                                                                                   
*  Parameters (in) : None                 
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function performs the polling of wake-up events.                            
******************************************************************************/
void Can_MainFunction_Wakeup( void );


/******************************************************************************
*  Service name    : Can_CheckWakeup                                                  
*  Syntax          : Std_ReturnType Can_CheckWakeup( uint8 Controller );         
*  Service ID      : 0x0B                                                                                                   
*  Parameters (in) : Controller - CAN controller ID
*  Parameters (out): None                                                      
*  Return value    : Std_ReturnType - E_OK: API call has been accepted.
*                  : -E_NOT_OK: API call has not been accepted.                                                      
*  Description     : This function checks if a wakeup has occurred for the given controller.                          
******************************************************************************/
Std_ReturnType Can_CheckWakeup(uint8 Controller);


/******************************************************************************
*  Service name    : Can_GetStatisticsInfo                                                  
*  Syntax          : void Can_GetStatisticsInfo(uint8 Controller, Can_StatisticsType *Can_StatisticsData)                                                                                                          
*  Parameters (in) : Controller - CAN controller ID,
*				   : Can_StatisticsData - Pointer to store statistics info               
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function provides statistics information.                            
******************************************************************************/
void Can_GetStatisticsInfo(uint8 Controller, Can_StatisticsType *Can_StatisticsData);


/******************************************************************************
*  Service name    : OS_ISR_Canx_InterruptHandler
*  Syntax          : void OS_ISR_Canx_InterruptHandler(void)
*  Parameters (in) : none
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : This function provides handles interrupts of CAN module.                            
******************************************************************************/
//ETAS BIP Porting void OS_ISR_Can0_InterruptHandler(void *ptr);
void OS_ISR_Can0_InterruptHandler(void);
//ETAS BIP Porting void OS_ISR_Can1_InterruptHandler(void *ptr);
void OS_ISR_Can1_InterruptHandler(void);
#define CAN_STOP_SEC_CODE

#endif
