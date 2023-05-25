/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include <Mcu.h>
#include "Mcu_Alps.h"

#if(MCU_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

//#include "SchM_Mcu.h" 

/*******************************************************************************
**                      Imported Compiler Switch Checks                       **
*******************************************************************************/
#ifndef MCU_SW_MAJOR_VERSION
#error "MCU_SW_MAJOR_VERSION is not defined."
#endif

#ifndef MCU_SW_MINOR_VERSION
#error "MCU_SW_MINOR_VERSION is not defined."
#endif

#ifndef MCU_SW_PATCH_VERSION
#error "MCU_SW_PATCH_VERSION is not defined."
#endif

#if(MCU_SW_MAJOR_VERSION != 3U)
#error "MCU_SW_MAJOR_VERSION does not match."
#endif

#if(MCU_SW_MINOR_VERSION != 4U)
#error "MCU_SW_MINOR_VERSION does not match."
#endif

#if(MCU_SW_PATCH_VERSION != 0U)
#error "MCU_SW_PATCH_VERSION does not match."
#endif

/* AUTOSAR Secification File Version Check */
#ifndef MCU_AR_RELEASE_MAJOR_VERSION
#error "MCU_AR_RELEASE_MAJOR_VERSION is not defined."
#endif

/* AUTOSAR Major and Minor release version check  */
#if(MCU_AR_RELEASE_MAJOR_VERSION != 4U)
#error "MCU_AR_RELEASE_MAJOR_VERSION does not match."
#endif

/* Inter Module Check for the correct version usage in the used modules */

/******************************************************************************
*                       Private Variable Definitions                            
******************************************************************************/
#define MCU_START_SEC_VAR_INIT_8

static boolean McuInitialized = FALSE;
static boolean McuClkInit     = FALSE;
#define MCU_STOP_SEC_VAR_INIT_8


#define MCU_START_SEC_CODE

/******************************************************************************
*                    Private Function Declaration                             
******************************************************************************/
static void Mcu_lclParamCheck(boolean Expr,
                              uint8 ApiId, 
                              uint8 ErrId, 
                              Std_ReturnType *RetVal);
							  
/******************************************************************************
*                         Public Functions                                     
******************************************************************************/

/******************************************************************************
*  Service name    : Mcu_Init                                                  
*  Syntax          : void Mcu_Init( const Mcu_ConfigType* ConfigPtr )          
*  Service ID      : 0x00                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : ConfigPtr - Pointer to MCU driver configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for MCU initialization                            
******************************************************************************/
void Mcu_Init(const Mcu_ConfigType *ConfigPtr)
{
    Std_ReturnType RetVal = E_OK;
    
	/* Mcu_FSR_001 */
    Mcu_lclParamCheck((boolean)(NULL_PTR != ConfigPtr), MCU_INIT_SID,
                       MCU_E_PARAM_POINTER, &RetVal);
                       
    if(RetVal == E_OK)
    {
        Mcu_Hal_Init(ConfigPtr);
        McuInitialized = TRUE;
    }
}


/******************************************************************************
*  Service name    : Mcu_InitRamSection                                                
*  Syntax          : Std_ReturnType Mcu_InitRamSection( 
                                            Mcu_RamSectionType RamSection)                         
*  Service ID      : 0x01                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : RamSection - Selects RAM memory section provided in 
                                  configuration set.                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the RAM section wise.                        
******************************************************************************/
Std_ReturnType Mcu_InitRamSection(Mcu_RamSectionType RamSection)
{
    Std_ReturnType RetVal = E_OK;
    
    /* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE), 
                       MCU_INITRAMSECTION_SID, 
                       MCU_E_UNINIT,
                       &RetVal);
     
    /* SWS_Mcu_00021 */    
    Mcu_lclParamCheck((boolean)(RamSection < MCU_NUM_RAM_SECTORS),
                       MCU_INITRAMSECTION_SID,
                       MCU_E_PARAM_RAMSECTION,
                       &RetVal);
                       
    if(RetVal == E_OK)
    {
        RetVal = Mcu_Hal_InitRamSection(RamSection);
    }
    
    return RetVal;
}


#if (MCU_INIT_CLOCK == STD_ON)
/******************************************************************************
*  Service name    : Mcu_InitClock                                                
*  Syntax          : Std_ReturnType Mcu_InitClock( Mcu_ClockType ClockSetting )                         
*  Service ID      : 0x02                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : Clock Setting                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the PLL & other MCU clock options                        
******************************************************************************/
Std_ReturnType Mcu_InitClock(Mcu_ClockType ClockSetting)
{
    Std_ReturnType RetVal = E_OK;
    
    /* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                       MCU_INITCLOCK_SID,
                       MCU_E_UNINIT,
                       &RetVal);
    
    /* SWS_Mcu_00019 */    
    Mcu_lclParamCheck((boolean)(ClockSetting < MCU_NUM_CLOCK_SETTING),
                       MCU_INITCLOCK_SID,
                       MCU_E_PARAM_CLOCK,
                       &RetVal);
                       
    if(RetVal == E_OK)
    {
        RetVal = Mcu_Hal_InitClock(ClockSetting);
        if(RetVal == E_OK)
        {
        	 McuClkInit = TRUE;
        }
    }
    
    
    return RetVal;
}
#endif


#if (MCU_NO_PLL == STD_OFF)
/******************************************************************************
*  Service name    : Mcu_DistributePllClock                                                
*  Syntax          : Std_ReturnType Mcu_DistributePllClock( void )                         
*  Service ID      : 0x03                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Activates the PLL clock to the MCU clock distribution.                      
******************************************************************************/
Std_ReturnType Mcu_DistributePllClock(void)
{
    Std_ReturnType RetVal = E_OK;
    
    /* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                       MCU_DISTRIBUTEPLLCLK_SID,
                       MCU_E_UNINIT,
                       &RetVal);
                       
    /* SWS_Mcu_00122 */
	/* SWS_Mcu_00142 */
    Mcu_lclParamCheck((boolean)(Mcu_Hal_GetPllStatus() == MCU_PLL_LOCKED),
                       MCU_DISTRIBUTEPLLCLK_SID,
                       MCU_E_PLL_NOT_LOCKED,
                       &RetVal);
                       
    if(RetVal == E_OK)
    {
        RetVal = Mcu_Hal_DistributePllClock();
    }
    
    return RetVal;
}
#endif


/******************************************************************************
*  Service name    : Mcu_GetPllStatus                                                
*  Syntax          : Mcu_PllStatusType Mcu_GetPllStatus( void )                         
*  Service ID      : 0x04                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_PllStatusType - PLL status 
*  Description     : This service provides the lock status of the PLL.                      
******************************************************************************/
Mcu_PllStatusType Mcu_GetPllStatus(void)
{
    Mcu_PllStatusType Status = MCU_PLL_STATUS_UNDEFINED;

    /* SWS_Mcu_00206 */
#if(MCU_NO_PLL != STD_ON)
       Std_ReturnType RetVal = E_OK;
       /* SWS_Mcu_00132 */
	   /* SWS_Mcu_00125 */
       Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                          MCU_GETPLLSTATUS_SID,
                          MCU_E_UNINIT,
                          &RetVal);                    
       if(RetVal == E_OK)
       {
          Status = Mcu_Hal_GetPllStatus(); 
       }
#endif
    
    return Status;
}



/******************************************************************************
*  Service name    : Mcu_GetResetReason                                                
*  Syntax          : Mcu_ResetType Mcu_GetResetReason( void )                         
*  Service ID      : 0x05                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_ResetType
*  Description     : The service reads the reset type from the hardware.                      
******************************************************************************/
Mcu_ResetType Mcu_GetResetReason(void)
{
	Std_ReturnType RetVal = E_OK;
    Mcu_ResetType ResetReason = MCU_RESET_UNDEFINED;
    
    /* SWS_Mcu_00133 */
	/* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                       MCU_GETRESETREASON_SID,
                       MCU_E_UNINIT,
                       &RetVal);
					   
	if(RetVal == E_OK)
    {
       ResetReason = Mcu_Hal_GetResetReason();
    }
    
    return ResetReason;
}


/******************************************************************************
*  Service name    : Mcu_GetResetRawValue                                                
*  Syntax          : Mcu_RawResetType Mcu_GetResetRawValue( void )                       
*  Service ID      : 0x06                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RawResetType
*  Description     : The service reads the reset type from the hardware register.                      
********************************************************************************/
Mcu_RawResetType Mcu_GetResetRawValue(void)
{
    Std_ReturnType RetVal = E_OK;
    Mcu_RawResetType ResetValue = RESET_VAL_UNDEFINED;
    
    /* SWS_Mcu_00135 */
	/* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                       MCU_GETRESETRAWVAL_SID,
                       MCU_E_UNINIT,
                       &RetVal);
                       
    if(RetVal == E_OK)
    {
        ResetValue = Mcu_Hal_GetResetRawValue();
    }
    
    return ResetValue;
}


#if (MCU_PERFORM_RESET_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_PerformReset                                                
*  Syntax          : void Mcu_PerformReset( void )                       
*  Service ID      : 0x07                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Non-reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : The service performs a microcontroller reset.                      
********************************************************************************/
void Mcu_PerformReset(void)
{
    Std_ReturnType RetVal = E_OK;
    
	/* SWS_Mcu_00125 */
    Mcu_lclParamCheck((boolean)(McuInitialized == TRUE),
                       MCU_PERFORMRESET_SID,
                       MCU_E_UNINIT,
                       &RetVal);
                       
    if(RetVal == E_OK)
    {
        Mcu_Hal_PerformReset();
    }
}
#endif


/******************************************************************************
*  Service name    : Mcu_SetMode                                                
*  Syntax          : void Mcu_SetMode( Mcu_ModeType McuMode )                       
*  Service ID      : 0x08                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : McuMode : Set different MCU power modes configured.                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : This service activates the MCU power modes.                     
********************************************************************************/
void Mcu_SetMode(Mcu_ModeType McuMode)
{
    /* NOT IMPLEMENTED : Sleep mode not supported */
}


#if (MCU_VERSION_INFO_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_GetVersionInfo                                                
*  Syntax          : void Mcu_GetVersionInfo(Std_VersionInfoType* versioninfo)                      
*  Service ID      : 0x09                                                      
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : versioninfo : Pointer to where to store the version 
                     information of this module.
*  Description     : This service returns the version information of this module.                     
********************************************************************************/
void Mcu_GetVersionInfo(Std_VersionInfoType *Versioninfo)
{
    Std_ReturnType RetVal = E_OK;
    
	/* MCU_FSR_002 */
    Mcu_lclParamCheck((boolean)(NULL_PTR != Versioninfo),
                      MCU_GETVERSIONINFO_SID,
                      MCU_E_PARAM_POINTER,
                      &RetVal);
    
    if (E_OK == RetVal) {
        Versioninfo->vendorID = MCU_VENDOR_ID;
        Versioninfo->moduleID = MCU_MODULE_ID;
        Versioninfo->sw_major_version = MCU_SW_MAJOR_VERSION;
        Versioninfo->sw_minor_version = MCU_SW_MINOR_VERSION;
        Versioninfo->sw_patch_version = MCU_SW_PATCH_VERSION;
    }
}
#endif

#if (MCU_RAM_STATUS_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_GetRamState                                                
*  Syntax          : Mcu_RamStateType Mcu_GetRamState( void )                      
*  Service ID      : 0x0A                                                    
*  Sync/Async      : Synchronous                                               
*  Reentrancy      : Reentrant                                             
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RamStateType : Status of the Ram Content.
*  Description     : This service provides the actual status of the 
                     microcontroller Ram. (if supported)                     
********************************************************************************/
Mcu_RamStateType Mcu_GetRamState(void)
{
    /* NOT IMPLEMENTED : RAM State not supported by Hardware */
}
#endif

/**********************************************************************************
*  Service name    : Mcu_GetPeripheralClkFreq                                                
*  Syntax          : Mcu_PeripheralClockValType Mcu_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName) 
*  Parameters (in) : PeripheralName                                                      
*  Parameters (out): None                                                       
*  Return value    : Mcu_PeripheralClockValType
*  Description     : It provides the peripheral clock values configured in MCU module.
************************************************************************************/
Mcu_PeripheralClockValType Mcu_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName)
{
	Std_ReturnType RetVal = E_OK;
	Mcu_PeripheralClockValType PeripheralClockVal = 0x00;

	Mcu_lclParamCheck((boolean)(McuClkInit == TRUE),
	                       MCU_GETPERIPHERALCLKFREQ_SID,
	                       MCU_E_UNINIT,
	                       &RetVal);

	if(RetVal == E_OK)
	{
		PeripheralClockVal = Mcu_Hal_GetPeripheralClkFreq(PeripheralName);
	}

	return PeripheralClockVal;
}


/******************************************************************************
*                         Private Function Defination                                     
******************************************************************************/
static void Mcu_lclParamCheck(boolean Expr, uint8 ApiId, uint8 ErrId, Std_ReturnType *RetVal)   
{
    if ((E_OK == *RetVal) && (Expr == FALSE)) {
    #if (MCU_DEV_ERROR_DETECT == STD_ON)
        //calls Det_ReportError from Det module
        (void)Det_ReportError(MCU_MODULE_ID, MCU_INSTANCE_ID, ApiId, ErrId);
    #endif
        *RetVal = E_NOT_OK;
    }
}    
    
#define MCU_STOP_SEC_CODE

