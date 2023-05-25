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
#include <Port.h>
#include "Port_Alps.h"

#if(PORT_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

#include "Port_Alps_RegDef.h"

/******************************************************************************
*                       Private Variables                                      
******************************************************************************/
#define PORT_START_SEC_VAR_INIT_PTR

static Port_reg_t Port_Hw_Reg = { .DMU = (Port_DMU_t*)0xBA0000,
                                  .GPIO = (Port_GPIO_t*)0xBE0000
                                };

#define PORT_STOP_SEC_VAR_INIT_PTR
                                  

#define PORT_START_SEC_CODE


/******************************************************************************
*                         Private Functions Prototype
******************************************************************************/
static void Port_lclWriteMuxValue(uint32 MuxName, uint32 MuxVal);

/******************************************************************************
*                         Public Functions                                     
******************************************************************************/
/******************************************************************************
*  service name     : Port_Hal_Init                                                  
*  Syntax           : void Port_Hal_Init(const Port_ConfigType* ConfigPtr)                                                      
*  Reentrancy       : Non reentrant                                             
*  Parameters (in)  : ConfigPtr - Pointer to configuration set                  
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Initializes the Port Driver module.                            
******************************************************************************/
void Port_Hal_Init(const Port_ConfigType *ConfigPtr)
{
    uint32 Direction = 0;
    uint32 Value     = 0;
    uint32 BitIndex  = 0;
    uint32 Idx       = 0;
    uint32 IdxJ      = 0;
    uint32 MuxName   = 0;
    uint32 MuxVal    = 0;
    
    if (ConfigPtr->GpioPinCtrl == DMU_CTRL)
    {
        /* Pins are controlled by debugbus output register. */
        Port_Hw_Reg.DMU->DMU_SYS_DMU_SEL     = 0x1;

        /* Configure pin multiplexing and selecting to GPIO functionality */
        Port_Hw_Reg.DMU->DMU_MUX_SPI_M1      = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_UART_1      = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_CAN_1       = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_I2C_M       = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_PWM_0       = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_PWM_1       = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_CAN_CLK     = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_CLK  = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_SEL  = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_MOSI = 0x4;
        Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_MISO = 0x4;                                  

        if (ConfigPtr->DmuDebugMode == DEBUG_MODE)
        {
            /* controls debugbus output data source, selecting dump with sync:5 */
            Port_Hw_Reg.DMU->DMU_DBG_SRC     = 0x5;

            /* controls input and outupt status for debugbus valid signal 
               enable : 0, disable : 1. */
            Port_Hw_Reg.DMU->DMU_DBG_VAL_OEN = 0x0;

            /* controls input and output status for debugbus data signal 
               enable : 0, disable : 1. */
            Port_Hw_Reg.DMU->DMU_DBG_DAT_OEN = 0xffff0000;
        }
        else if (ConfigPtr->DmuDebugMode == HIL_MODE)
        {
            /* controls input and outupt status for debugbus valid signal 
               enable : 0, disable : 1. */
            Port_Hw_Reg.DMU->DMU_DBG_VAL_OEN = 0x1;
            
            /* controls input and output status for debugbus data signal 
               enable : 0, disable : 1. */
            Port_Hw_Reg.DMU->DMU_DBG_DAT_OEN = 0xffffffff;
        }
        else
        {
        	/*compliant to misra-c*/
        }
     }
     else if (ConfigPtr->GpioPinCtrl == GPIO_CTRL)
     {
        /* Pins are controlled by debug bus output register.*/
        Port_Hw_Reg.DMU->DMU_SYS_DMU_SEL = 0;
            
        for (Idx = 0; Idx < ConfigPtr->NumofMuxCfg; Idx++)
        {
            MuxName = ConfigPtr->MuxCfg[Idx].MuxName;
            MuxVal  = ConfigPtr->MuxCfg[Idx].MuxVal;

            Port_lclWriteMuxValue(MuxName, MuxVal);

            /* check if configured MUX is for GPIO functionality */
            if (ConfigPtr->MuxCfg[Idx].IsGpio == TRUE && ConfigPtr->MuxCfg[Idx].GpioCfg->GpioPinCfg != NULL_PTR)
            {
                for (IdxJ = 0 ; IdxJ < ConfigPtr->MuxCfg[Idx].GpioCfg->NumofPinCfg ; IdxJ++)
                {
                    BitIndex = ConfigPtr->MuxCfg[Idx].GpioCfg->GpioPinCfg[IdxJ].Pin;

                    if (ConfigPtr->MuxCfg[Idx].GpioCfg->GpioPinCfg[IdxJ].Dir == PORT_PIN_IN)
                    {
                        /* write 0 for PORT_PIN_IN as data direction for gpio pin
                           into the corresponding bit position */
                        if (BitIndex <= 15u)
                        {
                            Direction &= ~(1u << BitIndex);
                            Port_Hw_Reg.GPIO->GPIO_SWPORTA_DDR = (Direction);
                        }
                        else
                        {
                            Direction &= ~(1u << (BitIndex % 16u));
                            Port_Hw_Reg.GPIO->GPIO_SWPORTB_DDR = (Direction);
                        }
                    }
                    else
                    {
                        /* write 1 for PORT_PIN_OUT as data direction for gpio pin
                           into the corresponding bit position */
                        if (BitIndex <= 15u)
                        {
                            Direction |= (1u << BitIndex);
                            Port_Hw_Reg.GPIO->GPIO_SWPORTA_DDR = (Direction);
                        }
                        else
                        {
                            Direction |= (1u << (BitIndex % 16u));
                            Port_Hw_Reg.GPIO->GPIO_SWPORTB_DDR = (Direction);
                        }
                    }
                        
                    if (ConfigPtr->MuxCfg[Idx].GpioCfg->GpioPinCfg[IdxJ].InitVal == STD_HIGH)
                    {
                        /* write Initial value for GPIO pin to corresponding bit position */
                        if (BitIndex <= 15u)
                        {
                            Value |= (1u << BitIndex);
                            Port_Hw_Reg.GPIO->GPIO_SWPORTA_DR = (Value);
                        }
                        else
                        {
                            Value |= (1u << (BitIndex % 16u));
                            Port_Hw_Reg.GPIO->GPIO_SWPORTB_DR = (Value);
                        }
                    }
                    else
                    {
                        if (BitIndex <= 15u)
                        {
                            Value &= ~(1u << BitIndex);
                            Port_Hw_Reg.GPIO->GPIO_SWPORTA_DR = (Value);
                        }
                        else
                        {
                            Value &= ~(1u << (BitIndex % 16u));
                            Port_Hw_Reg.GPIO->GPIO_SWPORTB_DR = (Value);
                        }
                    }
                }
            }
        }
    }
	else
	{
	 /*compliant to misra-c*/
	}
}

/******************************************************************************
*                         Private Functions
******************************************************************************/
/******************************************************************************
*  service name     : Port_lclWriteMuxValue
*  Syntax           : void Port_lclWriteMuxValue(uint32 MuxName, uint32 MuxVal)
*  Parameters (in)  : MuxName - Mux ID
*  Parameters (in)  : MuxVal - Mux Value
*  Parameters (out) : None
*  Return value     : None
*  Description      : Initializes the Port Driver module.
******************************************************************************/
static void Port_lclWriteMuxValue(uint32 MuxName, uint32 MuxVal)
{
    switch (MuxName)
    {
        case MUX_QSPI:
            Port_Hw_Reg.DMU->DMU_MUX_QSPI = MuxVal;
            break;
        case MUX_SPI_M1:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_M1 = MuxVal;
            break;
        case MUX_UART_0:
            Port_Hw_Reg.DMU->DMU_MUX_UART_0 = MuxVal;
            break;
        case MUX_UART_1:
            Port_Hw_Reg.DMU->DMU_MUX_UART_1 = MuxVal;
            break;
        case MUX_CAN_0:
            Port_Hw_Reg.DMU->DMU_MUX_CAN_0 = MuxVal;
            break;
        case MUX_CAN_1:
            Port_Hw_Reg.DMU->DMU_MUX_CAN_1 = MuxVal;
            break;
        case MUX_RESET:
            Port_Hw_Reg.DMU->DMU_MUX_RESET = MuxVal;
            break;
        case MUX_SYNC:
            Port_Hw_Reg.DMU->DMU_MUX_SYNC = MuxVal;
            break;
        case MUX_I2C_M:
            Port_Hw_Reg.DMU->DMU_MUX_I2C_M = MuxVal;
            break;
        case MUX_PWM_0:
            Port_Hw_Reg.DMU->DMU_MUX_PWM_0 = MuxVal;
            break;
        case MUX_PWM_1:
            Port_Hw_Reg.DMU->DMU_MUX_PWM_1 = MuxVal;
            break;
        case MUX_ADC_CLK:
            Port_Hw_Reg.DMU->DMU_MUX_ADC_CLK = MuxVal;
            break;
        case MUX_CAN_CLK:
            Port_Hw_Reg.DMU->DMU_MUX_CAN_CLK = MuxVal;
            break;
        case MUX_SPI_M0:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_M0 = MuxVal;
            break;
        case MUX_SPI_S:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_S = MuxVal;
            break;
        case MUX_SPI_S1_CLK:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_CLK = MuxVal;
            break;
        case MUX_SPI_S1_SEL:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_SEL = MuxVal;
            break;
        case MUX_SPI_S1_MOSI:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_MOSI = MuxVal;
            break;
        case MUX_SPI_S1_MISO:
            Port_Hw_Reg.DMU->DMU_MUX_SPI_S1_MISO = MuxVal;
            break;
        case MUX_JTAG:
            Port_Hw_Reg.DMU->DMU_MUX_JTAG = MuxVal;
            break;
        default:
        	/*compliant to misra-c*/
        	break;
    }
}

#define PORT_STOP_SEC_CODE


