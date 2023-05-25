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

/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include "Dio.h"
#include "Dio_Alps.h"

#if(DIO_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include "Dio_Alps_RegDef.h"

/* *****************************************************************************
*                       Private Variables                                      
******************************************************************************/
#define DIO_START_SEC_VAR_INIT_PTR

static Dio_reg_t Dio_Hw_Reg = { .DMU = (Dio_DMU_t*)0xBA0000,
                                .GPIO = (Dio_GPIO_t *)0xBE0000
                              };
#define DIO_STOP_SEC_VAR_INIT_PTR


#define DIO_START_SEC_CODE

#define DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId) (ChannelId / 16)
#define DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId)  (ChannelId % 16)

/* *****************************************************************************
*                         Public Functions                                     
******************************************************************************/
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
Dio_LevelType Dio_Hal_ReadChannel(Dio_ChannelType ChannelId) 
{
    uint32 RegVal         = 0;
    Dio_LevelType Level   = STD_LOW;
    Dio_PortType PortId   = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
    Dio_PortLevelType Bit = (Dio_PortLevelType)DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);

    /* DIO_FSR_00089 : Values used by the DIO Driver for the software
                       level of Channels are either STD_HIGH or STD_LOW */
    /* DIO_FSR_00011 : The Dio_ReadChannel function shall read the level
                       of a single DIO channel. */
    /* DIO_FSR_00175 : Invalid channel requested.
                       Related error code: DIO_E_PARAM_INVALID_CHANNEL_ID */

    switch (PortId)
    {
    case PORTA :
        /* SWS_Dio_00027 reading the value of a specified channel */ 
        RegVal = (Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DR & (1u << Bit));
        if ((RegVal >> Bit) == STD_HIGH)
        {
            Level = STD_HIGH;
        }
        break;
    case PORTB : 
        /* SWS_Dio_00027 reading the value of a specified channel */ 
        RegVal = (Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DR & (1u << Bit));
        if((RegVal >> Bit) == STD_HIGH)
        {
            Level = STD_HIGH;
        }
        break;
    default :
        #if (DIO_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(DIO_MODULE_ID,
                            DIO_INSTANCE_ID,
                            DIO_READCHANNEL_ID,
                            DIO_E_PARAM_INVALID_CHANNEL_ID);
        #endif
        break;
    }

    return Level;
}
  
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
void Dio_Hal_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    uint32 RegVal         = 0;
    Dio_PortType PortId   = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
    Dio_PortLevelType Bit = (Dio_PortLevelType)DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);

    /* DIO_FSR_00089 : Values used by the DIO Driver for the software
                       level of Channels are either STD_HIGH or STD_LOW */
    /* DIO_FSR_00006 : The Dio_WriteChannel function shall set the level 
                       of a single DIO channel to STD_HIGH or STD_LOW. */
    /* DIO_FSR_00175 : Invalid channel requested.
                       Related error code: DIO_E_PARAM_INVALID_CHANNEL_ID */

    switch (PortId)
    {
        case PORTA :
            /* SWS_Dio_00028 check if data direction of the channel is configured as output*/
            RegVal = (Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DDR & (1u << Bit));
            if((RegVal >> Bit) == DIO_PIN_OUT)
            {
                if(Level == STD_HIGH)
                {
                    Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DR |= (1u << Bit);
                } 
                else
                {
                    Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DR &= ~(1u << Bit);
                }
            }
            break;
        case PORTB : 
            /* SWS_Dio_00028 check if data direction of the channel is configured as output*/
            RegVal = (Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DDR & (1u << Bit));
            if((RegVal >> Bit) == DIO_PIN_OUT)
            {
                if(Level == STD_HIGH)
                {
                    Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DR |= (1u << Bit);
                } else
                {
                    Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DR &= ~(1u << Bit);
                }
            }
            break;
        default :
            #if (DIO_DEV_ERROR_DETECT == STD_ON)
                Det_ReportError(DIO_MODULE_ID,
                                DIO_INSTANCE_ID,
                                DIO_WRITECHANNEL_ID,
                                DIO_E_PARAM_INVALID_CHANNEL_ID);
            #endif
            break;
    }
}
            
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
Dio_PortLevelType Dio_Hal_ReadPort(Dio_PortType PortId)
{       
    Dio_PortLevelType Level = 0;

    /* DIO_FSR_00013 : The Dio_ReadPort function shall read the levels
                       of all channels of one port */
    /* DIO_FSR_00177 : Invalid port requested.
                       Related error code: DIO_E_PARAM_INVALID_PORT_ID */
    
    switch (PortId)
    {
        case PORTA :
            /* SWS_Dio_00031 */    
            Level = (Dio_PortLevelType)Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DR ;
            break;
        case PORTB :
            /* SWS_Dio_00031 */    
            Level = (Dio_PortLevelType)Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DR ;
            break;
        default :
            #if (DIO_DEV_ERROR_DETECT == STD_ON)
                Det_ReportError(DIO_MODULE_ID,
                                DIO_INSTANCE_ID,
                                DIO_READPORT_ID,
                                DIO_E_PARAM_INVALID_PORT_ID);
            #endif
            break;
    }

    return Level;
}   

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
void Dio_Hal_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    uint32 RegVal = 0;

    /* DIO_FSR_00007 : The Dio_WritePort function shall simultaneously set the levels
                       of all output channels.*/
    /* DIO_FSR_00177 : Invalid port requested.
                       Related error code: DIO_E_PARAM_INVALID_PORT_ID */

    switch (PortId)
    {
        case PORTA : 
            /* SWS_Dio_00034 write into the port */
            RegVal = Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DDR;
            RegVal &= Level;
            Dio_Hw_Reg.GPIO->GPIO_SWPORTA_DR = RegVal;
            break;
        case PORTB : 
            /* SWS_Dio_00034 write into the port */
            RegVal = Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DDR;
            RegVal &= Level;
            Dio_Hw_Reg.GPIO->GPIO_SWPORTB_DR = RegVal;
            break;
        default :
            #if( DIO_DEV_ERROR_DETECT == STD_ON )
                Det_ReportError(DIO_MODULE_ID,
                                DIO_INSTANCE_ID,
                                DIO_READPORT_ID,
                                DIO_E_PARAM_INVALID_PORT_ID);
            #endif
            break;
    }
}

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
Dio_LevelType Dio_Hal_FlipChannel(Dio_ChannelType ChannelId)
{
     Dio_PortLevelType Level = STD_LOW;
     
     Level = Dio_Hal_ReadChannel(ChannelId);
     
     if (Level == STD_HIGH)
     {
        Level = STD_LOW;
     }
     else {
        Level =STD_HIGH;
     }
     
     Dio_Hal_WriteChannel(ChannelId, Level);

     return Level; 
}
#endif

#define DIO_STOP_SEC_CODE

