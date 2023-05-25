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
********************************************************************************
**                                                                            **
**  FILENAME   : Port_PBCfg.c                                                 **
**                                                                            **
**                                                                            **
**  DATE, TIME : 2022-12-08, 18:00:15                                     **
**                                                                            **
**  GENERATOR  : Build b210315-0853                                         **
**                                                                            **
**  AUTHOR     : Calterah Semiconductor                                       **
**                                                                            **
**  VENDOR     : Calterah Semiconductor                                       **
**                                                                            **
**  DESCRIPTION : PORT configuration generated out of ECU configuration       **
**                file (Port.bmd)                                             **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                       Includes                                             **
*******************************************************************************/
/* Include module header file */
#include "Port.h"

#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Port_MemMap.h"   




















const Port_GpioPinCfgType Port_GpioPinConfigData0[]=
{
   {  
		
	.Pin        = 13,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 0     
   },
   {  
		
	.Pin        = 14,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 0     
   },
   {  
		
	.Pin        = 15,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 0     
   },
   {  
		
	.Pin        = 16,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 0     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData1[]=
{
   {  
		
	.Pin        = 7,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 8,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData4[]=
{
   {  
		
	.Pin        = 28,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 29,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData5[]=
{
   {  
		
	.Pin        = 26,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 27,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData7[]=
{
   {  
		
	.Pin        = 2,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData8[]=
{
   {  
		
	.Pin        = 1,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData9[]=
{
   {  
		
	.Pin        = 25,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData10[]=
{
   {  
		
	.Pin        = 0,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData12[]=
{
   {  
		
	.Pin        = 17,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 18,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 19,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   },
   {  
		
	.Pin        = 20,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData13[]=
{
   {  
		
	.Pin        = 12,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData14[]=
{
   {  
		
	.Pin        = 11,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData15[]=
{
   {  
		
	.Pin        = 10,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};
const Port_GpioPinCfgType Port_GpioPinConfigData16[]=
{
   {  
		
	.Pin        = 9,        
	.Dir	    = PORT_PIN_OUT,        
	.InitVal 	= 1     
   }
};

const Port_GpioCfgType Port_GpioConfigData[] =
{	
   {
    		
    .NumofPinCfg = 4,	
    .GpioPinCfg = &Port_GpioPinConfigData0[0], 
   } ,         
   {
    		
    .NumofPinCfg = 2,	
    .GpioPinCfg = &Port_GpioPinConfigData1[0], 
   } ,         
   {
    	
    .NumofPinCfg = 0,	
    .GpioPinCfg = NULL_PTR
   } ,     
   {
    	
    .NumofPinCfg = 0,	
    .GpioPinCfg = NULL_PTR
   } ,     
   {
    		
    .NumofPinCfg = 2,	
    .GpioPinCfg = &Port_GpioPinConfigData4[0], 
   } ,         
   {
    		
    .NumofPinCfg = 2,	
    .GpioPinCfg = &Port_GpioPinConfigData5[0], 
   } ,         
   {
    	
    .NumofPinCfg = 0,	
    .GpioPinCfg = NULL_PTR
   } ,     
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData7[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData8[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData9[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData10[0], 
   } ,         
   {
    	
    .NumofPinCfg = 0,	
    .GpioPinCfg = NULL_PTR
   } ,     
   {
    		
    .NumofPinCfg = 4,	
    .GpioPinCfg = &Port_GpioPinConfigData12[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData13[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData14[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData15[0], 
   } ,         
   {
    		
    .NumofPinCfg = 1,	
    .GpioPinCfg = &Port_GpioPinConfigData16[0], 
   }        

};

const Port_MuxCfgType Port_MuxConfigData[] =
{
  {
	.MuxName = MUX_SPI_M1,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[0]
  },
  {
	.MuxName = MUX_UART_1,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[1]
  },
  {
	.MuxName = MUX_CAN_0,      
	.MuxVal = 0,   
	.IsGpio = 0,
	.GpioCfg = &Port_GpioConfigData[2]
  },
  {
	.MuxName = MUX_CAN_1,      
	.MuxVal = 0,   
	.IsGpio = 0,
	.GpioCfg = &Port_GpioConfigData[3]
  },
  {
	.MuxName = MUX_RESET,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[4]
  },
  {
	.MuxName = MUX_SYNC,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[5]
  },
  {
	.MuxName = MUX_I2C_M,      
	.MuxVal = 0,   
	.IsGpio = 0,
	.GpioCfg = &Port_GpioConfigData[6]
  },
  {
	.MuxName = MUX_PWM_0,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[7]
  },
  {
	.MuxName = MUX_PWM_1,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[8]
  },
  {
	.MuxName = MUX_ADC_CLK,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[9]
  },
  {
	.MuxName = MUX_CAN_CLK,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[10]
  },
  {
	.MuxName = MUX_SPI_M0,      
	.MuxVal = 0,   
	.IsGpio = 0,
	.GpioCfg = &Port_GpioConfigData[11]
  },
  {
	.MuxName = MUX_SPI_S,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[12]
  },
  {
	.MuxName = MUX_SPI_S1_CLK,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[13]
  },
  {
	.MuxName = MUX_SPI_S1_SEL,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[14]
  },
  {
	.MuxName = MUX_SPI_S1_MOSI,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[15]
  },
  {
	.MuxName = MUX_SPI_S1_MISO,      
	.MuxVal = 4,   
	.IsGpio = 1,
	.GpioCfg = &Port_GpioConfigData[16]
  }
};

const Port_ConfigType Port_ConfigData =
{	
    .GpioPinCtrl =  GPIO_CTRL, 
	.DmuDebugMode =  NOT_VALID,
    .NumofMuxCfg = 17,
    .MuxCfg  = &Port_MuxConfigData[0]
};

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Port_MemMap.h"

