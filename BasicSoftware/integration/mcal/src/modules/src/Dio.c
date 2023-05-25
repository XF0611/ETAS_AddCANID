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

#include "SchM_Dio.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef DIO_SW_MAJOR_VERSION
#error "DIO_SW_MAJOR_VERSION is not defined."
#endif

#ifndef DIO_SW_MINOR_VERSION
#error "DIO_SW_MINOR_VERSION is not defined."
#endif

#ifndef DIO_SW_PATCH_VERSION
#error "DIO_SW_PATCH_VERSION is not defined."
#endif

#if(DIO_SW_MAJOR_VERSION != 3U)
#error "DIO_SW_MAJOR_VERSION does not match."
#endif

#if(DIO_SW_MINOR_VERSION != 4U)
#error "DIO_SW_MINOR_VERSION does not match."
#endif

#if(DIO_SW_PATCH_VERSION != 0U)
#error "DIO_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef DIO_AR_RELEASE_MAJOR_VERSION
#error "DIO_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(DIO_AR_RELEASE_MAJOR_VERSION != 4U)
#error "DIO_AR_RELEASE_MAJOR_VERSION does not match."
#endif

/******************************************************************************
                   Private Variable Definitions                              
*******************************************************************************/
#define DIO_START_SEC_VAR_INIT_UNSPECIFIED
Dio_GlobalType DioGlobal = { .Config = &Dio_ConfigData };
#define DIO_STOP_SEC_VAR_INIT_UNSPECIFIED

#define DIO_START_SEC_CODE

/******************************************************************************
*                    Private Function Declaration                             
******************************************************************************/
static void Dio_lclParamCheck(boolean Expr,
                              uint8 ApiId,
                              uint8 ErrId,
                              Std_ReturnType *RetVal);
static boolean Dio_lclChannelValidation(Dio_ChannelType ChannelId);
static boolean Dio_lclPortValidation(Dio_PortType PortId);

/******************************************************************************
                     Public Functions                          
*******************************************************************************/
/* ****************************************************************************
*  Service name    : Dio_ReadChannel                                               
*  Syntax          : Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)       
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : ChannelId :Id of DIO channel                 
*  Parameters (out): None                                                      
*  Return value    : Dio_LevelType
                     STD_HIGH: The physical level of the pin STD_HIGH
                     STD_LOW : The physical level of the pin STD_LOW
*  Description     : Returns the value of the specified DIO channel.                     
******************************************************************************/
Dio_LevelType Dio_ReadChannel(const Dio_ChannelType ChannelId)
{
    Dio_LevelType Level   = STD_LOW;
    Std_ReturnType RetVal = E_OK;;

    /* DIO_FSR_00175 : Invalid channel requested.
                       Related error code: DIO_E_PARAM_INVALID_CHANNEL_ID */
    
    Dio_lclParamCheck(Dio_lclChannelValidation(ChannelId), DIO_READCHANNEL_ID,
                      DIO_E_PARAM_INVALID_CHANNEL_ID, &RetVal);

    if (RetVal == E_OK)
    {
        Level = Dio_Hal_ReadChannel (ChannelId);
    }
    
    return Level;
}

/* *****************************************************************************
*  Service name    : Dio_WriteChannel                                         
*  Syntax          : void Dio_WriteChannel(Dio_ChannelType ChannelId, 
                                           Dio_LevelType Level)            
*  Service ID      : 0x01                                                   
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : ChannelId - Channel ID. 
                     Level   - value to be written.                                 
*  Parameters (out): none
*  Return value    : none           
*  Description     : Service to set a level of a channel.        
******************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    Std_ReturnType RetVal = E_OK;

    /* DIO_FSR_00175 : Invalid channel requested.
                       Related error code: DIO_E_PARAM_INVALID_CHANNEL_ID */

    Dio_lclParamCheck(Dio_lclChannelValidation(ChannelId),DIO_WRITECHANNEL_ID,
                      DIO_E_PARAM_INVALID_CHANNEL_ID, &RetVal);

    if (RetVal == E_OK)
    {
        Dio_Hal_WriteChannel(ChannelId, Level);
    }
}
        
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
Dio_PortLevelType Dio_ReadPort(const Dio_PortType PortId)
{
    Dio_PortLevelType Level = STD_LOW;
    Std_ReturnType RetVal   = E_OK;

    /* DIO_FSR_00177 : Invalid port requested.
                       Related error code: DIO_E_PARAM_INVALID_PORT_ID */

    Dio_lclParamCheck(Dio_lclPortValidation(PortId),DIO_READPORT_ID, 
                      DIO_E_PARAM_INVALID_PORT_ID, &RetVal);

    if (RetVal == E_OK)
    {
        Level = Dio_Hal_ReadPort(PortId);
    }
    
    return Level;
}

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
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    Std_ReturnType RetVal = E_OK;

    /* DIO_FSR_00177 : Invalid port requested.
                       Related error code: DIO_E_PARAM_INVALID_PORT_ID */

    Dio_lclParamCheck(Dio_lclPortValidation(PortId), 
                      DIO_WRITEPORT_ID, DIO_E_PARAM_INVALID_PORT_ID,
                      &RetVal);

    if (RetVal == E_OK)
    {
      Dio_Hal_WritePort(PortId, Level);
    }   
}

/* ***********************************************************************************
*  Service name    : Dio_ReadChannelGroup                                      
*  Syntax          : Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType 
                                                            *channelGroupIdPtr)            
*  Service ID      : 0x04                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : channelGroupIdPtr - Pointer to ChannelGroup                        
*  Parameters (out): none
*  Return value    : Dio_PortLevelType - Level of a subset of the adjoining 
                                            bits of a port.          
*  Description     : This Service reads a subset of the adjoining bits of a port.     
***************************************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
 
    /* NOT Implemented */
    return 0;
}

/* ************************************************************************************
*  Service name    : Dio_WriteChannelGroup                                      
*  Syntax          : void Dio_WriteChannelGroup(const Dio_ChannelGroupType 
                                                *channelGroupIdPtr,
                                                Dio_PortLevelType Level)            
*  Service ID      : 0x05                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : channelGroupIdPtr - Pointer to ChannelGroup   
                     Level - value to be written
*  Parameters (out): none
*  Return value    : Dio_ChannelGroupType - Level of a subset of the adjoining 
                                            bits of a port.          
*  Description     : This Service reads a subset of the adjoining bits of a port.     
***************************************************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr, 
                           Dio_PortLevelType Level)
{
 
    /* NOT Implemented */
}

#if (DIO_VERSION_INFO_API == STD_ON)
/* ***************************************************************************
*  service name    : Dio_GetVersionInfo                                         
*  Syntax          : void Spi_GetVersionInfo(Std_VersionInfoType *versioninfo)
*  service ID      : 0x12                                                      
*  Sync/Async      : Synchronous                                                
*  Reentrancy      : Reentrant                                                  
*  Parameters (in) : None                                                       
*  Parameters (out): versioninfo -  Pointer to where to store the version       
*                                   information of this module.                 
*  Return value    : None                                                       
*  Description     : This service returns the version information of this
*                    module.
******************************************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{   
    Std_ReturnType RetVal = E_OK;

    /* DIO_FSR_00189 : If DET is enabled for the DIO Driver module,
                       the function Dio_GetVersionInfo shall raise
                       DIO_E_PARAM_POINTER, if the argument is NULL
                       pointer and return without any action */
        
    Dio_lclParamCheck((boolean)(VersionInfo != NULL_PTR), 
                       DIO_GETVERSIONINFO_ID,DIO_E_PARAM_POINTER,
                       &RetVal);
                        
    if (RetVal == E_OK)
    {
        VersionInfo->vendorID         = DIO_VENDOR_ID;
        VersionInfo->moduleID         = DIO_MODULE_ID;
        VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
    }
}
#endif

#if (DIO_FLIP_CHANNEL_API  == STD_ON)
/* ****************************************************************************
*  Service name    : Dio_FlipChannel                                
*  Syntax          : Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)        
*  Service ID      : 0x11                                                  
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                                 
*  Parameters (in) : ChannelId - ID of DIO channel                        
*  Parameters (out): none
*  Return value    : Dio_LevelType
                     STD_HIGH: The physical level of the pin STD_HIGH
                     STD_LOW : The physical level of the pin STD_LOW
*  Description     : Service to flip (change 1 to 0 or 0 to 1) the level of
*                    the channel and return the level of the channel after flip.   
********************************************************************************/
Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId) 
{
    Dio_LevelType Level   = STD_LOW;
    Std_ReturnType RetVal = E_OK;
    
    Dio_lclParamCheck(Dio_lclChannelValidation(ChannelId),
                      DIO_FLIPCHANNEL_ID, DIO_E_PARAM_INVALID_CHANNEL_ID,
                      &RetVal);

    if (RetVal == E_OK)
    {
        Level = Dio_Hal_FlipChannel (ChannelId);
    }
    
    return Level;
}
#endif

/******************************************************************************
*                    Private Function API                           
******************************************************************************/
/*******************************************************************************
*  Service name    : Dio_lclParamCheck                                            
*  Syntax          : void Dio_lclParamCheck(boolean Expr,                         
*                                           uint8 ApiId,                          
*                                           uint8 ErrId,                          
*                                           Std_ReturnType *RetVal)                    
*  Parameters (in) : Expr - TRUE or FALSE                                       
*                    ApiId - API ID                                            
*                    ErrId - ERROR ID                                          
*  Parameter(inout): RetVal                                                         
*  Return value    : None                                                       
*  Description     : Reports Det error if DIO_DEV_ERROR_DETECT == STD_ON.       
*******************************************************************************/
void Dio_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId,
                       Std_ReturnType *RetVal)
{
    /* checks if the expr is TRUE */
    if ((FALSE == Expr) && (E_OK == *RetVal))
    {
        *RetVal = E_NOT_OK;
        #if (DIO_DEV_ERROR_DETECT == STD_ON)
          /* calls Det_ReportError from Det module */
          (void)Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, ApiId, ErrId);
        #endif
    }
}

/*******************************************************************************
*  Service name    : Dio_lclChannelValidation
*  Syntax          : boolean Dio_lclChannelvalidation(Dio_ChannelType ChannelId)
*  Parameters (in) : ChannelId - Channel Id                                         
*  Parameter(inout): None                                                         
*  Return value    : RetVal                                                       
*  Description     : Checks whether the Channel Id is valid or not.       
*******************************************************************************/
static boolean Dio_lclChannelValidation(Dio_ChannelType ChannelId)
{
    const Dio_ChannelConfigType *ChPtr = DioGlobal.Config->DioChannelConfigData;
    boolean RetVal                     = FALSE;
  
    while (DIO_END_OF_LIST != ChPtr->DioChannelId)
    {
        if (ChPtr->DioChannelId == ChannelId)
        {
            RetVal = TRUE;
            break;
        }
        ChPtr++;
    }
  
    return RetVal; 
}

/*******************************************************************************
*  Service name    : Dio_lclPortValidation
*  Syntax          : boolean Dio_lclPortvalidation(Dio_PortType PortId)
*  Parameters (in) : PortId - Port Id                                         
*  Parameter(inout): None                                                         
*  Return value    : RetVal                                                       
*  Description     : Checks whether the Port Id is valid or not.       
*******************************************************************************/
static boolean Dio_lclPortValidation(Dio_PortType PortId)
{
    const Dio_PortType* PortPtr = DioGlobal.Config->DioPortConfigData;
    boolean RetVal              = FALSE;
  
    while (DIO_END_OF_LIST != *PortPtr)
    {
        if (*PortPtr == PortId)
        {
            RetVal = TRUE;
            break;
        }
        PortPtr++;
    }
  
    return RetVal;
}

#define DIO_STOP_SEC_CODE

