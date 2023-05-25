/******************************************************************************
*                            Includes                                          
******************************************************************************/
#include <Mcu.h>
#include "Mcu_Alps.h"
#include "Mcu_Alps_RegDef.h"
#include <string.h>

/******************************************************************************
*                       Private Variables                                      
******************************************************************************/
#define MCU_START_SEC_VAR_INIT_PTR

static Mcu_Reg_t *Mcu_ClkGen_Reg = (Mcu_Reg_t *)0xB20000U;
#define MCU_STOP_SEC_VAR_INIT_PTR


#define MCU_START_SEC_VAR_INIT_UNSPECIFIED

static Mcu_GlobalType McuGlobal;
#define MCU_STOP_SEC_VAR_INIT_UNSPECIFIED



#define MCU_START_SEC_CODE

/******************************************************************************
*                         Private Function Declaration                         
******************************************************************************/
static void Mcu_lclInitDiv(const Mcu_ClockSettingConfigType *ClockCfgPtr);
static void Mcu_lclInitPeripheralClks(const Mcu_ClockSettingConfigType *ClockCfgPtr);
static void Mcu_lclCanClkSourceSelect(const Mcu_ClockSettingConfigType *ClockCfgPtr);

/******************************************************************************
*                         Public Functions                                     
******************************************************************************/
/******************************************************************************
*  Service name    : Mcu_Hal_Init                                                  
*  Syntax          : void Mcu_Hal_Init( const Mcu_ConfigType *ConfigPtr )          
*  Parameters (in) : ConfigPtr - Pointer to configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for MCU initialization                            
******************************************************************************/
void Mcu_Hal_Init(const Mcu_ConfigType *ConfigPtr)
{
    McuGlobal.ConfigPtr = ConfigPtr;
    
    /* SWS_Mcu_00006 */
    McuGlobal.RawResetValue = 0x00;               /* If the hardware does not have a reset status register, 
                                                  the return value shall be 0x0. */
    
    /* SWS_Mcu_00005 */
    McuGlobal.ResetReason = MCU_POWER_ON_RESET;   /* There is no hardware detection of the reset reason, 
                                                   hence the return value shall always be MCU_POWER_ON_RESET */
}


/******************************************************************************
*  Service name    : Mcu_Hal_InitRamSection                                                  
*  Syntax          : Std_ReturnType Mcu_Hal_InitRamSection( 
                                            Mcu_RamSectionType RamSection)          
*  Parameters (in) : RamSection                
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted                      
*                    E_NOT_OK: command has not been accepted                                                       
*  Description     : Service for RAM Section initialization                            
******************************************************************************/
Std_ReturnType Mcu_Hal_InitRamSection(Mcu_RamSectionType RamSection)
{
    uint32 RamSize;
    uint32 RamData;
    uint32 RamPtr;
    
    /* Default data that is written in the Ram section */
    RamData = McuGlobal.ConfigPtr->McuRamSettingConfig[RamSection].McuRamDefaultData;
    
    /* Pointer to the Base Address of RAM */
    RamPtr = McuGlobal.ConfigPtr->McuRamSettingConfig[RamSection].McuRamSectionBaseAddr;
    
    RamSize = McuGlobal.ConfigPtr->McuRamSettingConfig[RamSection].McuRamSectionSize;
    
    /* Initializing the configured RAM section */
    memset((void *)RamPtr, (uint32)RamData, (size_t)RamSize);
    
    return E_OK;
}


#if (MCU_INIT_CLOCK == STD_ON)
/******************************************************************************
*  Service name    : Mcu_Hal_InitClock                                                
*  Syntax          : Std_ReturnType Mcu_Hal_InitClock(Mcu_ClockType ClockSetting)                         
*  Parameters (in) : Clock Setting                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the PLL & other MCU clock options                        
******************************************************************************/
Std_ReturnType Mcu_Hal_InitClock(Mcu_ClockType ClockSetting)
{
    const Mcu_ClockSettingConfigType *ClockCfgPtr;
    
    McuGlobal.ClockSetting = ClockSetting;
    
    ClockCfgPtr = &McuGlobal.ConfigPtr->McuClockSettingConfig[ClockSetting];
    
	/* NOTE : Init PLL should be taken care in the startup code */
	Mcu_lclCanClkSourceSelect(ClockCfgPtr);
    Mcu_lclInitDiv(ClockCfgPtr);
	Mcu_lclInitPeripheralClks(ClockCfgPtr);
    
    return E_OK;
}
#endif


#if (MCU_NO_PLL == STD_OFF)
/******************************************************************************
*  Service name    : Mcu_Hal_DistributePllClock                                                
*  Syntax          : Std_ReturnType Mcu_Hal_DistributePllClock( void )                                                         
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Activates the PLL clock to the MCU clock distribution.                      
******************************************************************************/
Std_ReturnType Mcu_Hal_DistributePllClock(void)
{
    /* Switch the clock domain from slow clock to fast clock i.e., enable PLL outputs */
	
	/* SWS_Mcu_00056 */
	if ((Mcu_ClkGen_Reg->CLKGEN_SEL_300M & 1u) != 0x1)
	{
	    Mcu_ClkGen_Reg->CLKGEN_SEL_300M = 0x1;
	}
	
	if ((Mcu_ClkGen_Reg->CLKGEN_SEL_400M & 1u) != 0x1)
	{
	    Mcu_ClkGen_Reg->CLKGEN_SEL_400M = 0x1;
    }
    return E_OK;
}
#endif


/******************************************************************************
*  Service name    : Mcu_Hal_GetPllStatus                                                
*  Syntax          : Mcu_PllStatusType Mcu_Hal_GetPllStatus( void )                                                          
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_PllStatusType - PLL status 
*  Description     : This service provides the lock status of the PLL.                      
******************************************************************************/
Mcu_PllStatusType Mcu_Hal_GetPllStatus(void)
{
    Mcu_PllStatusType Status;
    
	if(((Mcu_ClkGen_Reg->CLKGEN_READY_PLL) & (uint32)1u) != 0u)
	{
		Status = MCU_PLL_LOCKED;
	}
	else {
		Status = MCU_PLL_UNLOCKED;
	}
    
    return Status;
}

/******************************************************************************
*  Service name    : Mcu_Hal_GetResetReason                                                
*  Syntax          : Mcu_ResetType Mcu_Hal_GetResetReason( void )                                                         
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_ResetType
*  Description     : The service reads the reset type from the hardware.                      
******************************************************************************/
Mcu_ResetType Mcu_Hal_GetResetReason(void)
{
    // returns the set reset reason
    return McuGlobal.ResetReason;
}


/******************************************************************************
*  Service name    : Mcu_Hal_GetResetRawValue                                                
*  Syntax          : Mcu_RawResetType Mcu_Hal_GetResetRawValue( void )                                                    
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RawResetType
*  Description     : The service reads the reset type from the hardware register.                      
********************************************************************************/
Mcu_RawResetType Mcu_Hal_GetResetRawValue(void)
{
    // returns the appropriate reset value
    return McuGlobal.RawResetValue;
}


#if (MCU_PERFORM_RESET_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_Hal_PerformReset                                                
*  Syntax          : void Mcu_Hal_PerformReset( void )                                                                    
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : The service performs a microcontroller reset.                      
********************************************************************************/
void Mcu_Hal_PerformReset(void)
{  
   uint32 ResetVal;
   uint8 i;
   
   ResetVal = McuGlobal.ConfigPtr->McuPeripheralResetConfig;
   
   if((ResetVal & MCU_SW_REBOOT_MASK) == MCU_SW_REBOOT_MASK)
   {
	   /* Software Reboot */
	   Mcu_ClkGen_Reg->CLKGEN_SW_REBOOT = 1u;
   }
   else {
	   /* Other Peripherals reset */
   
       if((ResetVal & MCU_DMA_MASK) == MCU_DMA_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_DMA = 0u;
	       //for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_DMA = 1u;
       }
   
       if((ResetVal & MCU_BB_TOP_MASK) == MCU_BB_TOP_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_BB_TOP = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_BB_TOP = 1u;
       }

       if((ResetVal & MCU_BB_CORE_MASK) == MCU_BB_CORE_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_BB_CORE = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_BB_CORE = 1u;
       }
   
       if((ResetVal & MCU_FLASH_CTRL_MASK) == MCU_FLASH_CTRL_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_FLASH_CTRL = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_FLASH_CTRL = 1u;
       }
   
       if((ResetVal & MCU_CRC_MASK) == MCU_CRC_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CRC = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CRC = 1u;
       }
   
       if((ResetVal & MCU_DMU_MASK) == MCU_DMU_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_DMU = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_DMU = 1u;
       }
   
       if((ResetVal & MCU_UART_0_MASK) == MCU_UART_0_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_UART_0 = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_UART_0 = 1u;
       }
   
       if((ResetVal & MCU_UART_1_MASK) == MCU_UART_1_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_UART_1 = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_UART_1 = 1u;
       }
   
       if((ResetVal & MCU_I2C_MASK) == MCU_I2C_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_I2C = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_I2C = 1u;
       }
   
       if((ResetVal & MCU_SPI_M0_MASK) == MCU_SPI_M0_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_M0 = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_M0 = 1u;
       }
   
       if((ResetVal & MCU_SPI_M1_MASK) == MCU_SPI_M1_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_M1 = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_M1 = 1u;
       }
   
       if((ResetVal & MCU_SPI_S_MASK) == MCU_SPI_S_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_S = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_SPI_S = 1u;
       }
   
       if((ResetVal & MCU_QSPI_MASK) == MCU_QSPI_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_QSPI = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_QSPI = 1u;
       }
   
       if((ResetVal & MCU_GPIO_MASK) == MCU_GPIO_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_GPIO = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_GPIO = 1u;
       }
   
       if((ResetVal & MCU_TIMER_MASK) == MCU_TIMER_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_TIMER = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_TIMER = 1u;
       }
   
       if((ResetVal & MCU_PWM_MASK) == MCU_PWM_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_PWM = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_PWM = 1u;
       }
   
       if((ResetVal & MCU_CAN_0_MASK) == MCU_CAN_0_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CAN_0 = 0u;
	       //for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CAN_0 = 1u;
       }
   
       if((ResetVal & MCU_CAN_1_MASK) == MCU_CAN_1_MASK)
       {
	       /* Assert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CAN_1 = 0u;
	       for (i = 0; i < 10; i++){;}
	       /* Deassert reset */
	       Mcu_ClkGen_Reg->CLKGEN_RSTN_CAN_1 = 1u;
       }
   
       if((ResetVal & MCU_LVDS_MASK) == MCU_LVDS_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_LVDS = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_LVDS = 1u;
       }
   
       if((ResetVal & MCU_RF_TEST_MASK) == MCU_RF_TEST_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_RF_TEST = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_RF_TEST = 1u;
       }
   
       if((ResetVal & MCU_DIG_TEST_MASK) == MCU_DIG_TEST_MASK)
       {
	        /* Assert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_DIG_TEST = 0u;
	        for (i = 0; i < 10; i++){;}
	        /* Deassert reset */
	        Mcu_ClkGen_Reg->CLKGEN_RSTN_DIG_TEST = 1u;
       }
   }
}
#endif


/******************************************************************************
*  Service name    : Mcu_lclInitPeripheralClks                                                
*  Syntax          : void Mcu_lclInitPeripheralClks(const Mcu_ClockSettingConfigType
                                             ClockCfgPtr)                                                                    
*  Parameters (in) : ClockCfgPtr                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : Enables clock for Peripherals.                     
********************************************************************************/
void Mcu_lclInitPeripheralClks(const Mcu_ClockSettingConfigType *ClockCfgPtr)
{
	uint32 ClkEnaVal;	
	ClkEnaVal = ClockCfgPtr->McuClockEnableConfig;
	
	Mcu_ClkGen_Reg->CLKGEN_ENA_FMCW       = ((ClkEnaVal & MCU_FMCW_MASK) == MCU_FMCW_MASK) ? 1 : 0;
	Mcu_ClkGen_Reg->CLKGEN_ENA_DMA        = ((ClkEnaVal & MCU_DMA_MASK) == MCU_DMA_MASK) ? 1 : 0;
	Mcu_ClkGen_Reg->CLKGEN_ENA_BB_TOP     = ((ClkEnaVal & MCU_BB_TOP_MASK) == MCU_BB_TOP_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_BB_CORE    = ((ClkEnaVal & MCU_BB_CORE_MASK) == MCU_BB_CORE_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_FLASH_CTRL = ((ClkEnaVal & MCU_FLASH_CTRL_MASK) == MCU_FLASH_CTRL_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_CRC        = ((ClkEnaVal & MCU_CRC_MASK) == MCU_CRC_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_DMU        = ((ClkEnaVal & MCU_DMU_MASK) == MCU_DMU_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_UART_0     = ((ClkEnaVal & MCU_UART_0_MASK) == MCU_UART_0_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_UART_1     = ((ClkEnaVal & MCU_UART_1_MASK) == MCU_UART_1_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_I2C        = ((ClkEnaVal & MCU_I2C_MASK) == MCU_I2C_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_SPI_M0     = ((ClkEnaVal & MCU_SPI_M0_MASK) == MCU_SPI_M0_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_SPI_M1     = ((ClkEnaVal & MCU_SPI_M1_MASK) == MCU_SPI_M1_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_SPI_S      = ((ClkEnaVal & MCU_SPI_S_MASK) == MCU_SPI_S_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_QSPI       = ((ClkEnaVal & MCU_QSPI_MASK) == MCU_QSPI_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_GPIO       = ((ClkEnaVal & MCU_GPIO_MASK) == MCU_GPIO_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_TIMER      = ((ClkEnaVal & MCU_TIMER_MASK) == MCU_TIMER_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_PWM        = ((ClkEnaVal & MCU_PWM_MASK) == MCU_PWM_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_CAN_0      = ((ClkEnaVal & MCU_CAN_0_MASK) == MCU_CAN_0_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_CAN_1      = ((ClkEnaVal & MCU_CAN_1_MASK) == MCU_CAN_1_MASK) ? 1 : 0;
    Mcu_ClkGen_Reg->CLKGEN_ENA_LVDS       = ((ClkEnaVal & MCU_LVDS_MASK) == MCU_LVDS_MASK) ? 1 : 0;
    
}


/******************************************************************************
*  Service name    : Mcu_lclInitDiv                                                
*  Syntax          : void Mcu_lclInitDiv(const Mcu_ClockSettingConfigType
                                             ClockCfgPtr);                                                                    
*  Parameters (in) : ClockCfgPtr                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : Initializes all the divisor registers for all peripherals                      
********************************************************************************/
void Mcu_lclInitDiv(const Mcu_ClockSettingConfigType *ClockCfgPtr)
{
	Mcu_ClkGen_Reg->CLKGEN_DIV_APB     = ClockCfgPtr->ApbClkDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_APB_REF = ClockCfgPtr->ApbClkRefDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_MEM     = ClockCfgPtr->MemClkDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_AHB     = ClockCfgPtr->AhbClkDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_CPU     = ClockCfgPtr->CpuClkDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_CAN_0   = ClockCfgPtr->Can0ClkDiv;
	Mcu_ClkGen_Reg->CLKGEN_DIV_CAN_1   = ClockCfgPtr->Can1ClkDiv;
}


/******************************************************************************
*  Service name    : Mcu_lclCanClockSourceSelect                                                
*  Syntax          : void Mcu_lclCanClockSourceSelect(const Mcu_ClockSettingConfigType
                                             ClockCfgPtr)                                                                    
*  Parameters (in) : ClockCfgPtr                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : Selects clock source for CAN 0 and CAN 1.                     
********************************************************************************/
void Mcu_lclCanClkSourceSelect(const Mcu_ClockSettingConfigType *ClockCfgPtr)
{
	if (ClockCfgPtr->Mcu_CanClockSourceConfig->Can0ExtClockSource)
	{
		// enable external reference clock for CAN 0
		Mcu_ClkGen_Reg->CLKGEN_SEL_CAN0 = 0x1;
	}
	
	if (ClockCfgPtr->Mcu_CanClockSourceConfig->Can1ExtClockSource)
	{
		// enable external reference clock for CAN 1
		Mcu_ClkGen_Reg->CLKGEN_SEL_CAN1 = 0x1;
	}
}

/**********************************************************************************
*  Service name    : Mcu_Hal_GetPeripheralClkFreq                                                
*  Syntax          : Std_ReturnType Mcu_Hal_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName) 
*  Parameters (in) : PeripheralName                                                      
*  Parameters (out): None                                                  
*  Return value    : Mcu_PeripheralClockValType
*  Description     : It provides the peripheral clock values configured in MCU module.
************************************************************************************/
Mcu_PeripheralClockValType Mcu_Hal_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName)
{
	Mcu_PeripheralClockValType Result = 0x00;

    uint32 Val = 0;
	
	const Mcu_ClockSettingConfigType *ClockCfgPtr;
	ClockCfgPtr = &McuGlobal.ConfigPtr->McuClockSettingConfig[McuGlobal.ClockSetting];

    switch (PeripheralName) 
	{
        case MCU_SYSTEM_REF_CLOCK:
            if (Mcu_ClkGen_Reg->CLKGEN_SEL_400M) 
			{
                Result = MCU_PLL0_OUTPUT_CLOCK_FREQ;
            } else {
                Result = MCU_XTAL_CLOCK_FREQ;
            }
            break;
				
		case MCU_UART0_CLOCK:
        case MCU_UART1_CLOCK:
        case MCU_SPI_M0_CLOCK:
        case MCU_SPI_M1_CLOCK:
        case MCU_SPI_S_CLOCK:
        case MCU_QSPI_CLOCK:
		case MCU_GPIO_CLOCK:
		    Val = Mcu_ClkGen_Reg->CLKGEN_DIV_APB_REF & 0xF;
            Result = Mcu_Hal_GetPeripheralClkFreq(MCU_SYSTEM_REF_CLOCK); // TODO: check recursion
            if (Result > 0)
			{
                if (Val)
				{
                    Result /= (Val + 1);
				}
            }
            break;
			
		case MCU_CAN0_CLOCK:
		    if (ClockCfgPtr->Mcu_CanClockSourceConfig->Can0ExtClockSource) 
			{
				Result = ClockCfgPtr->Mcu_CanClockSourceConfig->CanExtClockFreq;
			}
			else {
                Val = Mcu_ClkGen_Reg->CLKGEN_DIV_CAN_0 & 0xF;
                Result = Mcu_Hal_GetPeripheralClkFreq(MCU_SYSTEM_REF_CLOCK);
                if (Result > 0) 
			    {
                    if (Val) 
			        {
                        Result /= (Val + 1);
                    }
                }
			}
            break;
			
        case MCU_CAN1_CLOCK:
		    if (ClockCfgPtr->Mcu_CanClockSourceConfig->Can1ExtClockSource) 
			{
				Result = ClockCfgPtr->Mcu_CanClockSourceConfig->CanExtClockFreq;
			}
			else {
                Val = Mcu_ClkGen_Reg->CLKGEN_DIV_CAN_1 & 0xF;
                Result = Mcu_Hal_GetPeripheralClkFreq(MCU_SYSTEM_REF_CLOCK);
                if (Result > 0)
			    {
                    if (Val)
			        {
                        Result /= (Val + 1);
                    }
                }
			}
            break;
				
		default   :
            Result = 0x00;
            break;
			
	}
	
	return Result;
}

#define MCU_STOP_SEC_CODE

