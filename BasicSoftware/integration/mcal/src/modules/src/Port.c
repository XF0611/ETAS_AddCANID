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
/* Include own header file */
#include <Port.h>
#include "Port_Alps.h"

#if(PORT_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

//#include "SchM_Port.h"

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef PORT_SW_MAJOR_VERSION
#error "PORT_SW_MAJOR_VERSION is not defined."
#endif

#ifndef PORT_SW_MINOR_VERSION
#error "PORT_SW_MINOR_VERSION is not defined."
#endif

#ifndef PORT_SW_PATCH_VERSION
#error "PORT_SW_PATCH_VERSION is not defined."
#endif

#if(PORT_SW_MAJOR_VERSION != 3U)
#error "PORT_SW_MAJOR_VERSION does not match."
#endif

#if(PORT_SW_MINOR_VERSION != 4U)
#error "PORT_SW_MINOR_VERSION does not match."
#endif

#if(PORT_SW_PATCH_VERSION != 0U)
#error "PORT_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef PORT_AR_RELEASE_MAJOR_VERSION
#error "PORT_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(PORT_AR_RELEASE_MAJOR_VERSION != 4U)
#error "PORT_AR_RELEASE_MAJOR_VERSION does not match."
#endif
/* Inter Module Check for the correct version usage in the used modules */

/* Version check for DET module inclusion */
#if(PORT_DEV_ERROR_DETECT == STD_ON)

#ifndef DET_AR_RELEASE_MAJOR_VERSION
#error "DET_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

#if (DET_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)
#error "DET_AR_RELEASE_MAJOR_VERSION does not match."
#endif

#endif /* PORT_DEV_ERROR_DETECT == STD_ON */

/******************************************************************************
*                       Private Variable Definitions                            
******************************************************************************/
#define PORT_START_SEC_VAR_INIT_8

static boolean PortInitialized = FALSE;
#define PORT_STOP_SEC_VAR_INIT_8



#define PORT_START_SEC_CODE

/******************************************************************************
*                    Private Function Declaration                             
******************************************************************************/
static void Port_lclParamCheck (boolean Expr,
                                uint8 ApiId, 
                                uint8 ErrId, 
                                Std_ReturnType *RelVal);

/******************************************************************************
*                         Public Functions                                     
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
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    Std_ReturnType RetVal = E_OK;

    /* PORT_FSR_00001 : The PORT Driver module shall initialize the whole port
                        structure of the microcontroller. */
    /* PORT_FSR_00077 : If default error detection is enabled the Port Driver 
                        module shall check the function parameters in the order
                        in which they are passed and skip further parameter
                        checking if one check fails. */
    /* PORT_FSR_00002 : The function Port_Init shall initialize all variables
                        used by the PORT driver module to an initial state. */

    Port_lclParamCheck((boolean)(ConfigPtr == NULL_PTR),
                        PORT_INIT_ID, PORT_E_INIT_FAILED, &RetVal);        
   
    if(E_OK == RetVal)
    {
        Port_Hal_Init(ConfigPtr);
        PortInitialized = TRUE;
    }
}
      
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
                          Port_PinDirectionType Direction)  
{
    /* NOT implementated */          
}
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
void Port_RefreshPortDirection(void)    
{
    /* Not implementated */
}

#if (PORT_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name     : Port_GetVersionInfo                                               
*  Syntax           : void Port_GetVersionInfo(Std_VersionInfoType* VersionInfo)         
*  Service ID       : 0x03                                                     
*  Sync/Async       : Synchronous                                               
*  Reentrancy       : Reentrant                                             
*  Parameters (in)  : VersionInfo: Pointer to where to store the version 
                      information of this module              
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Returns the version information of this module.
******************************************************************************/
void Port_GetVersionInfo(Std_VersionInfoType* VersionInfo)         
{
    Std_ReturnType RetVal = E_OK;
    
    /* PORT_FSR_00225 : if Det is enabled, the parameter versioninfo shall be
                        checked for being NULL. The error PORT_E_PARAM_POINTER 
                        shall be reported in case the value is a NULL pointer.
                        */

    Port_lclParamCheck((boolean)(NULL_PTR == VersionInfo),
                       PORT_GET_VERSION_INFO_ID, PORT_E_PARAM_POINTER,
                       &RetVal);
    
    if (E_OK == RetVal)
    {
        VersionInfo->vendorID         = PORT_VENDOR_ID;
        VersionInfo->moduleID         = PORT_MODULE_ID;
        VersionInfo->sw_major_version = PORT_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = PORT_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = PORT_SW_PATCH_VERSION;
    }
}
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
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    /* NOT Implemented */
}
#endif

/******************************************************************************
*                    Private Function API                           
******************************************************************************/
/*******************************************************************************
*  Service name    : Port_lclParamCheck                                            
*  Syntax          : void Port_lclParamCheck(boolean Expr,                         
*                                           uint8 ApiId,                          
*                                           uint8 ErrId,                          
*                                           Std_ReturnType *RetVal)                    
*  Parameters (in) : Expr - TRUE or FALSE                                       
*                    ApiId - API ID                                            
*                    ErrId - ERROR ID                                          
*  Parameter(inout): RetVal                                                         
*  Return value    : None                                                       
*  Description     : Reports Det error if PORT_DEV_ERROR_DETECT == STD_ON.       
*******************************************************************************/
static void Port_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId,
                               Std_ReturnType *RelVal)
{
    /* checks if the Expr is TRUE */
    if ((TRUE == Expr) && (E_OK == *RelVal))
    {
        #if (PORT_DEV_ERROR_DETECT == STD_ON)
            /* calls Det_ReportError from Det module */
            (void)Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, ApiId, ErrId);
        #endif
        *RelVal = E_NOT_OK;
    }
}

#define PORT_STOP_SEC_CODE


