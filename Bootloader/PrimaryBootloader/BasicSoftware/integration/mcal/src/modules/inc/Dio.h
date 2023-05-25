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

#ifndef DIO_H
#define DIO_H

#include "Dio_Cfg.h"
#include "Std_Types.h"

#define DIO_READCHANNEL_ID                  0x00u
#define DIO_WRITECHANNEL_ID                 0x01u
#define DIO_READPORT_ID                     0x02u
#define DIO_WRITEPORT_ID                    0x03u
#define DIO_READCHANNELGROUP_ID             0x04u
#define DIO_WRITECHANNELGROUP_ID            0x05u
#define DIO_INIT_ID                         0x10u
#define DIO_FLIPCHANNEL_ID                  0x11u
#define DIO_GETVERSIONINFO_ID               0x12u
#define DIO_GLOBAL_ID                       0xFFu

/*SWS_DIO_00175 - Invalid channel name requested */
#define DIO_E_PARAM_INVALID_CHANNEL_ID      0x0Au
#define DIO_E_PARAM_CONFIG                  0x10u  
#define DIO_E_PARAM_INVALID_PORT_ID         0x14u    
#define DIO_E_PARAM_INVALID_GROUP_ID        0x1Fu   
#define DIO_E_PARAM_POINTER                 0x20u  

typedef enum {
    DIO_PIN_IN  = 0u,   /* INPUT PIN is assigned with value 0*/
    DIO_PIN_OUT = 1u    /* OUTPUT PIN is assigned with value 1*/
} Dio_PinDirectionType;

/* Dio_ChannelType 
SWS_Dio_00182 - Dio_ChannelType
SWS_Dio_00015 - Parameters of type Dio_ChannelType contain the numeric ID of a DIO channel.
SWS_Dio_00180 - The mapping of the ID is implementation specific but not configurable.*/

typedef uint32 Dio_ChannelType;

/* Dio_PortType 
SWS_Dio_00183 - Dio_PortType
SWS_Dio_00018 - Parameters of type Dio_PortType contain the numeric ID of a DIO port. 
SWS_Dio_00181 - The mapping of ID is implementation specific but not configurable.*/

typedef uint32 Dio_PortType;

typedef struct {
    Dio_ChannelType      DioChannelId;
    Dio_PinDirectionType DioPinDirection;
} Dio_ChannelConfigType;

/**
*   @brief Type for the definition of a channel group, which consists of several adjoining channels within a port.
*   This structure contains Dio_PortType, Offset and Mask
*/
typedef struct {
    Dio_PortType DioChannelGroupIdentification;
    uint8        DioPortOffset;
    uint32       DioPortMask;
} Dio_ChannelGroupType;

/* Dio_LevelType 
SWS_Dio_00185 - Dio_LevelType
SWS_Dio_00023 - Dio_LevelType is the type for the possible levels that a DIO channel can have (input or output) */
typedef uint8 Dio_LevelType;


/* Dio_PortLevelType 
SWS_Dio_00186 - Dio_PortLevelType 
SWS_Dio_00103 - The port width within the types defined for the DIO Driver 
                shall be the size of the largest port on the MCU which may be 
                accessed by the DIO Drive.
SWS_Dio_00024 - Dio_PortLevelType is the type for the value of a DIO port*/
typedef uint16 Dio_PortLevelType;

typedef struct {
    const Dio_ChannelConfigType *DioChannelConfigData;
    const Dio_PortType          *DioPortConfigData;
    const Dio_ChannelGroupType  *DioChannelGroupConfigData;
} Dio_ConfigType;

typedef struct {
    const Dio_ConfigType *Config;
} Dio_GlobalType;

/* Dio_ConfigData holds the configuration for Dio module. */
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
extern const Dio_ConfigType Dio_ConfigData;
#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED

/* ----------------------------[Public API's]-----------------*/
#define DIO_START_SEC_CODE


/* ****************************************************************************
*  Service name    : Dio_ReadChannel                                               
*  Syntax          : Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)       
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : ChannelId :Id of DIO channel                 
*  Parameters (out): None                                                      
*  Return value    : Dio_LevelType
                    STD_HIGH: The physical level of the corresponding pin STD_HIGH
                    STD_LOW : The physical level of the corresponding pin STD_LOW
*  Description     : Returns the value of the specified DIO channel.                     
******************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* *****************************************************************************
*  Service name    : Dio_WriteChannel                                         
*  Syntax          : void Dio_WriteChannel(Dio_ChannelType ChannelId, 
                                           Dio_LevelType Level)            
*  Service ID      : 0x01                                                   
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : Channel - Channel ID. 
                     Level   - value to be written.                                 
*  Parameters (out): none
*  Return value    : none           
*  Description     : Service to set a level of a channel.        
******************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* *****************************************************************************
*  Service name    : Dio_ReadPort                                         
*  Syntax          : Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)            
*  Service ID      : 0x02                                                   
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : PortId - ID of DIO Port                            
*  Parameters (out): none
*  Return value    : Dio_PortLevelType - level of all channels of that port.          
*  Description     : Returns the level of all channels of that port.     
******************************************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType portId);

/* ****************************************************************************
*  Service name    : Dio_WritePort                                        
*  Syntax          : void Dio_WritePort(Dio_PortType PortId,
                                        Dio_PortLevelType Level)            
*  Service ID      : 0x03                                                   
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : PortId - ID of the DIO port. 
                     Level  - value to be written into the Port.                                 
*  Parameters (out): none
*  Return value    : none           
*  Description     : Service to set a level of a channel.        
******************************************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/* ***********************************************************************************
*  Service name    : Dio_ReadChannelGroup                                      
*  Syntax          : Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType 
                                                            *ChannelGroupIdPtr)            
*  Service ID      : 0x04                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : channelGroupIdPtr - Pointer to ChannelGroup                        
*  Parameters (out): none
*  Return value    : Dio_PortLevelType - Level of a subset of the adjoining 
                                            bits of a port.          
*  Description     : This Service reads a subset of the adjoining bits of a port.     
***************************************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr);

/* ************************************************************************************
*  Service name    : Dio_WriteChannelGroup                                      
*  Syntax          : void Dio_WriteChannelGroup(const Dio_ChannelGroupType 
                                          *ChannelGroupIdPtr, Dio_PortLevelType Level)            
*  Service ID      : 0x05                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : channelGroupIdPtr - Pointer to ChannelGroup   
                     level - value to be written
*  Parameters (out): none
*  Return value    : Dio_ChannelGroupType - Level of a subset of the adjoining 
                                            bits of a port.          
*  Description     : This Service reads a subset of the adjoining bits of a port.     
***************************************************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                            Dio_PortLevelType Level);

#if (DIO_VERSION_INFO_API == STD_ON)
/* ******************************************************************************
*  Service name    : Dio_GetVersionInfo                                         
*  Syntax          : void Spi_GetVersionInfo(Std_VersionInfoType *VersionInfo)
*  Service ID      : 0x12                                                      
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): VersionInfo -  Pointer to where to store the version       
*                                   information of this module.                 
*  Return value    : None                                                       
*  Description     : This Service returns the version information of this module.
*******************************************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo);
#endif

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/* ******************************************************************************
*  Service name    : Dio_FlipChannel                                
*  Syntax          : Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)        
*  Service ID      : 0x11                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : ChannelId - ID of DIO channel                        
*  Parameters (out): none
*  Return value    : Dio_LevelType
                    STD_HIGH: The physical level of the corresponding pin STD_HIGH
                    STD_LOW : The physical level of the corresponding pin STD_LOW
*  Description     : Service to flip (change 1 to 0 or 0 to 1) the level of the 
                     channel and return the level of the channel after flip.   
***********************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

#define DIO_STOP_SEC_CODE

#endif

