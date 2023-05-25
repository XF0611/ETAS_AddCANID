/*******************************************************************************
**                                                                            **
** Copyright (C) 2020, Calterah Semiconductor, Inc.                           **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to Calterah       **
** Semiconductor, Inc. Passing on and copying of this document, and           **
** communication of its contents is not permitted without prior written       **
** authorization.                                                             **
**                                                                            **
*******************************************************************************/

/* Header Protection */
#ifndef PORT_H
#define PORT_H 

/* Includes */
#include "Port_Cfg.h"
#include "Std_Types.h"

/* Macro definitions for API Service ID */
#define PORT_INIT_ID                    0x00u /* Service Id for Port_Init API */
#define PORT_SET_PIN_DIRECTION_ID       0x01u /* Service Id for Port_SetPinDirection API */
#define PORT_REFRESH_PORT_DIRECTION_ID  0x02u /* Service Id for Port_RefreshPortDirection API */
#define PORT_GET_VERSION_INFO_ID        0x03u /* Service Id for Port_GetVersionInfo API */
#define PORT_SET_PIN_MODE_ID            0x04u /* Service Id for Port_SetPinMode API */

/* Macro definitions for Error Codes specified by AUTOSAR */
#define PORT_E_PARAM_PIN                0x0A
#define PORT_E_DIRECTION_UNCHANGEABLE   0x0B
#define PORT_E_INIT_FAILED              0x0C
#define PORT_E_PARAM_INVALID_MODE       0x0D
#define PORT_E_MODE_UNCHANGEABLE        0x0E
#define PORT_E_UNINIT                   0x0F
#define PORT_E_PARAM_POINTER            0x10

/* ----------------------------[Type Definitions]-----------------*/
typedef uint8 Port_PinType;
typedef uint16 Port_PinModeType;

/*enum definitions */
typedef enum
{
    PORT_PIN_IN  = 0u, /* INPUT PIN is assigned with value 0*/
    PORT_PIN_OUT = 1u /* OUTPUT PIN is assigned with value 1*/
} Port_PinDirectionType;

typedef enum
{
    PORT_PIN_LEVEL_LOW,
    PORT_PIN_LEVEL_HIGH
} Port_PinLevelValueType;

typedef enum
{
    MUX_QSPI, 
    MUX_SPI_M1, 
    MUX_UART_0,
    MUX_UART_1, 
    MUX_CAN_0, 
    MUX_CAN_1, 
    MUX_RESET, 
    MUX_SYNC, 
    MUX_I2C_M, 
    MUX_PWM_0, 
    MUX_PWM_1, 
    MUX_ADC_CLK,
    MUX_CAN_CLK, 
    MUX_SPI_M0, 
    MUX_SPI_S,
    MUX_SPI_S1_CLK, 
    MUX_SPI_S1_SEL, 
    MUX_SPI_S1_MOSI,
    MUX_SPI_S1_MISO, 
    MUX_JTAG
} Port_MuxIdType;

typedef enum
{
    DEBUG_MODE,         /* Debug Bus mode */
    HIL_MODE            /* Hardware in loop mode */
} Port_DmuDebugModeType;

typedef enum 
{
    GPIO_CTRL,          /* General purpose input/output Mode */
    DMU_CTRL            /* Debug Management Unit */
} Port_GpioPinCtrlType;

/*structure definitions */
typedef struct 
{
    uint32                 Pin;
    Port_PinDirectionType  Dir;
    boolean                InitVal;   /*Intial value of the Gpio pin */
} Port_GpioPinCfgType;

typedef struct 
{
    uint32                    NumofPinCfg;
    const Port_GpioPinCfgType *GpioPinCfg;
} Port_GpioCfgType;

typedef struct 
{
    Port_MuxIdType         MuxName;
    uint32                 MuxVal;
    boolean                IsGpio;
    const Port_GpioCfgType *GpioCfg;
} Port_MuxCfgType;   

typedef struct
{
    Port_GpioPinCtrlType  GpioPinCtrl;
    Port_DmuDebugModeType DmuDebugMode;
    uint32                NumofMuxCfg;       
    const Port_MuxCfgType *MuxCfg;
} Port_ConfigType;

#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
extern const Port_ConfigType Port_ConfigData;
#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#define PORT_START_SEC_CODE

/******************************************************************************
*                    Public Function Declaration                             
******************************************************************************/

/******************************************************************************
*  Service name     : Port_Init                                                  
*  Syntax           : void Port_Init(const Port_ConfigType* ConfigPtr)          
*  Service ID       : 0x00                                                      
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Non reentrant                                             
*  Parameters (in)  : ConfigPtr - Pointer to configuration set                  
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Initializes the Port Driver module.                            
******************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr);

#if ( PORT_SET_PIN_DIRECTION_API == STD_ON )
/******************************************************************************
*  Service name    : Port_SetPinDirection                                                
*  Syntax          : void Port_SetPinDirection(Port_PinType Pin, 
                                               Port_PinDirectionType Direction)       
*  Service ID      : 0x01                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : Pin       :Port Pin ID number
                     Direction :Port Pin Direction
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Sets the port pin direction                       
******************************************************************************/
void Port_SetPinDirection(Port_PinType Pin, 
                          Port_PinDirectionType Direction); 
#endif                         

/******************************************************************************
*  Service name     : Port_RefreshPortDirection
*  Syntax           : void Port_RefreshPortDirection(void)    
*  Service ID       : 0x02                                                     
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Non reentrant                                             
*  Parameters (in)  : None                  
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Refreshes PORT direction                            
******************************************************************************/
void Port_RefreshPortDirection(void);  

#if (PORT_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name     : Port_GetVersionInfo                                               
*  Syntax           : void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)         
*  Service ID       : 0x03                                                     
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Reentrant                                             
*  Parameters (in)  : versioninfo: Pointer to where to store the version 
                      information of this module              
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Returns the version information of this module.
******************************************************************************/
 void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);    
#endif 

#if (PORT_SET_PIN_MODE_API == STD_ON)
/******************************************************************************
*  Service name     : Port_SetPinMode                                                  
*  Syntax           : void Port_SetPinMode(Port_PinType Pin, 
                                           Port_PinModeType Mode)        
*  Service ID       : 0x04                                                     
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Reentrant                                             
*  Parameters (in)  : Pin :Port Pin ID number
                      Mode:New Port Pin mode to be set on port pin.                  
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Sets the PORT pin mode                             
******************************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode); 
#endif      

#define PORT_STOP_SEC_CODE

#endif

