#ifndef MCU_ALPS_H
#define MCU_ALPS_H

/* Macro definitions */
#define MCU_PLL0_OUTPUT_CLOCK_FREQ  400000000 /* 400 MHz */
#define MCU_XTAL_CLOCK_FREQ         50000000  /* 50 MHz  */

/* Enum Definitions */
typedef enum {
    MCU_SW_REBOOT_MASK    = 0x1,
    MCU_FMCW_MASK         = 0x1,
    MCU_DMA_MASK          = 0x2,
    MCU_BB_TOP_MASK       = 0x4,
    MCU_BB_CORE_MASK      = 0x8,
    MCU_FLASH_CTRL_MASK   = 0x10,
    MCU_CRC_MASK          = 0x20,
    MCU_DMU_MASK          = 0x40,
    MCU_UART_0_MASK       = 0x80,
    MCU_UART_1_MASK       = 0x100,
    MCU_I2C_MASK          = 0x200,
    MCU_SPI_M0_MASK       = 0x400,
    MCU_SPI_M1_MASK       = 0x800,
    MCU_SPI_S_MASK        = 0x1000,
    MCU_QSPI_MASK         = 0x2000,
    MCU_GPIO_MASK         = 0x4000,
    MCU_TIMER_MASK        = 0x8000,
    MCU_PWM_MASK          = 0x10000,
    MCU_CAN_0_MASK        = 0x20000,
    MCU_CAN_1_MASK        = 0x40000,
    MCU_LVDS_MASK         = 0x80000,
    MCU_RF_TEST_MASK      = 0x100000,
    MCU_DIG_TEST_MASK     = 0x200000,
    MCU_ROM_MASK          = 0x400000,
    MCU_RAM_MASK          = 0x800000,
    MCU_DBGBUS_MASK       = 0x1000000
} Mcu_PeripheralType;


/* Structure definitions */
typedef struct {
    const Mcu_ConfigType  *ConfigPtr;
    Mcu_RawResetType      RawResetValue;
    Mcu_ResetType         ResetReason;
    Mcu_ClockType         ClockSetting;
} Mcu_GlobalType;

/* Public Function Declarations */
#define MCU_START_SEC_CODE

/******************************************************************************
*  Service name    : Mcu_Hal_Init                                                  
*  Syntax          : void Mcu_Hal_Init( const Mcu_ConfigType *ConfigPtr )          
*  Parameters (in) : ConfigPtr - Pointer to configuration set                  
*  Parameters (out): None                                                      
*  Return value    : None                                                      
*  Description     : Service for MCU initialization                            
******************************************************************************/
void Mcu_Hal_Init(const Mcu_ConfigType *ConfigPtr); 

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
Std_ReturnType Mcu_Hal_InitRamSection(Mcu_RamSectionType RamSection);

#if (MCU_INIT_CLOCK == STD_ON)
/******************************************************************************
*  Service name    : Mcu_Hal_InitClock                                                
*  Syntax          : Std_ReturnType Mcu_Hal_InitClock( Mcu_ClockType ClockSetting )                         
*  Parameters (in) : Clock Setting                                                      
*  Parameters (out): None                                                      
*  Return value    : E_OK: command has been accepted         
*                    E_NOT_OK: command has not been accepted 
*  Description     : Service to initalize the PLL & other MCU clock options                        
******************************************************************************/
Std_ReturnType Mcu_Hal_InitClock(Mcu_ClockType ClockSetting);
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
Std_ReturnType Mcu_Hal_DistributePllClock(void);
#endif

/******************************************************************************
*  Service name    : Mcu_Hal_GetPllStatus                                                
*  Syntax          : Mcu_PllStatusType Mcu_Hal_GetPllStatus( void )                                                                     
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_PllStatusType - PLL status 
*  Description     : This service provides the lock status of the PLL.                      
******************************************************************************/
Mcu_PllStatusType Mcu_Hal_GetPllStatus(void);
 
/******************************************************************************
*  Service name    : Mcu_Hal_GetResetReason                                                
*  Syntax          : Mcu_ResetType Mcu_Hal_GetResetReason( void )                                                                      
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_ResetType
*  Description     : The service reads the reset type from the hardware.                      
******************************************************************************/
Mcu_ResetType Mcu_Hal_GetResetReason(void);

/******************************************************************************
*  Service name    : Mcu_Hal_GetResetRawValue                                                
*  Syntax          : Mcu_RawResetType Mcu_Hal_GetResetRawValue( void )                                                                   
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : Mcu_RawResetType
*  Description     : The service reads the reset type from the hardware register.                      
********************************************************************************/
Mcu_RawResetType Mcu_Hal_GetResetRawValue(void); 

#if (MCU_PERFORM_RESET_API == STD_ON)
/******************************************************************************
*  Service name    : Mcu_Hal_PerformReset                                                
*  Syntax          : void Mcu_Hal_PerformReset( void )                                                                    
*  Parameters (in) : None                                                      
*  Parameters (out): None                                                      
*  Return value    : None
*  Description     : The service performs a microcontroller reset.                      
********************************************************************************/
void Mcu_Hal_PerformReset(void);
#endif

/**********************************************************************************
*  Service name    : Mcu_Hal_GetPeripheralClkFreq                                                
*  Syntax          : Mcu_PeripheralClockValType Mcu_Hal_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName) 
*  Parameters (in) : PeripheralName                                                      
*  Parameters (out): None                                                       
*  Return value    : Mcu_PeripheralClockValType
*  Description     : It provides the peripheral clock values configured in MCU module.
************************************************************************************/
Mcu_PeripheralClockValType Mcu_Hal_GetPeripheralClkFreq(Mcu_PeriphNameType PeripheralName);

#define MCU_STOP_SEC_CODE


#endif
