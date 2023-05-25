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

#ifndef DIO_ALPS_H
#define DIO_ALPS_H

#define DIO_START_SEC_CODE


/* PORT numbers */
#define PORTA 0
#define PORTB 1

/* ----------------------------[Public API's declaration]-----------------*/

/* *****************************************************************************
*  service name     : Dio_Hal_ReadChannel                                                
*  Syntax           : Dio_LevelType Dio_Hal_ReadChannel(Dio_ChannelType ChannelId)
*  Reentrancy       : Non reentrant                                             
*  Parameters (in)  : ChannelId :Id of DIO channel                 
*  Parameters (out) : None                                                      
*  Return value     : Dio_LevelType
                     STD_HIGH: The physical level of the corresponding pin STD_HIGH
                     STD_LOW : The physical level of the corresponding pin STD_LOW
*  Description      : Returns the value of the specified DIO channel.                       
******************************************************************************/
Dio_LevelType Dio_Hal_ReadChannel(Dio_ChannelType ChannelId );

/* *****************************************************************************
*  service name    : Dio_Hal_WriteChannel                                         
*  Syntax          : void Dio_Hal_WriteChannel(Dio_ChannelType ChannelId, 
                                               Dio_LevelTypeType Level)                                            
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : Channel - Channel ID. 
                     Level   - value to be written.                                 
*  Parameters (out): none
*  Return value    : none           
*  Description     : Service to set a level of a channel.        
******************************************************************************/
void Dio_Hal_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* *****************************************************************************
*  service name    : Dio_Hal_ReadPort                                         
*  Syntax          : Dio_PortLevelType Dio_Hal_ReadPort(Dio_PortType PortId)            
                                                 
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : PortId - ID of DIO Port                            
*  Parameters (out): none
*  Return value    : Dio_PortLevelType - level of all channels of that port.          
*  Description     : Returns the level of all channels of that port.     
******************************************************************************/
Dio_PortLevelType Dio_Hal_ReadPort(Dio_PortType PortId);

/* ****************************************************************************
*  service name     : Dio_Hal_WritePort                                        
*  Syntax           : void Dio_Hal_WritePort(Dio_PortType PortId, 
                                             Dio_PortLevelType Level)                                          
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Reentrant                                                 
*  Parameters (in)  : PortId - Port ID. 
                      Level  - value to be written into the Port.                                 
*  Parameters (out) : none
*  Return value     : none           
*  Description      : service to set a level of a Port.        
******************************************************************************/
void Dio_Hal_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
                            
#if (DIO_FLIP_CHANNEL_API == STD_ON)
/* *********************************************************************************
*  service name    : Dio_Hal_FlipChannel                                
*  Syntax          : Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)                          
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : ChannelId - ID of DIO channel                        
*  Parameters (out): none
*  Return value    : STD_HIGH: The physical level of the corresponding Pin is STD_HIGH. 
                     STD_LOW: The physical level of the corresponding Pin is STD_LOW       
*  Description     : Service to flip (change from 1 to 0 or from 0 to 1) the level 
                     of a channel and return the level of the channel after flip.   
************************************************************************************/
Dio_LevelType Dio_Hal_FlipChannel(Dio_ChannelType ChannelId);
#endif

#define DIO_STOP_SEC_CODE


#endif

