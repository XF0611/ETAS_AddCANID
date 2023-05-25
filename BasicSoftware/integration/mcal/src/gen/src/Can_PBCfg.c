/*=============================================================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
==============================================================================*/
/*******************************************************************************
**                                                                            **
** Copyright (c) 2020, Calterah Semiconductor, Inc.					          **
**                                                                            **
** All rights reserved.                                                       **
**                                                                            **
** This document contains proprietary information belonging to 				  **
** Calterah Semiconductor Technology. Passing on and copying of this document,** 
** and communication of its contents is not permitted without prior written   **
** authorization.      														  **
**                                                                            **
********************************************************************************
**                                                                            **
**  FILENAME  : Can_PBCfg.c                           						  **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-08, 18:00:15                                      **
**                                                                            **
**  GENERATOR : Build b210315-0853                                          **
**                                                                            **
**  AUTHOR    : Calterah Semiconductor										  **
**                                                                            **
**  VENDOR    : Calterah Semiconductor                                        **
**                                                                            **
**  DESCRIPTION  : CAN configuration generated out of ECU configuration       **
**                 file  (Can.bmd)                                            **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/
/* includes */
#include "Can.h"

#define CAN_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Can_MemMap.h"



static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_0[] =
{
  {
		.Id0 = 0x261, 
	.Id1 = 0x261, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 


static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_1[] =
{
  {
		.Id0 = 0x81, 
	.Id1 = 0x81, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 


static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_2[] =
{
  {
		.Id0 = 0x677, 
	.Id1 = 0x677, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 


static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_3[] =
{
  {
		.Id0 = 0x332, 
	.Id1 = 0x332, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 


static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_4[] =
{
  {
		.Id0 = 0x7df, 
	.Id1 = 0x7df, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 


static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_5[] =
{
  {
		.Id0 = 0x740, 
	.Id1 = 0x740, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};

static const Can_Alps_StdFilterElementType CanStdIdFilterConfig_6[] =
{
  {
		.Id0 = 0x456, 
	.Id1 = 0x456, 
	.FilterType = CAN_CLASSIC_BIT_MASK_FILTER
  },
};
 



static const CanHwFilterType CanHardwareFilterconfig[] = {


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_0[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_1[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_2[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_3[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_4[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_5[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } , 

  {	   
	
	.CanStdFilterElements = &CanStdIdFilterConfig_6[0],  
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 1,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  } ,  


  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  }  ,

  {	   
	 .CanStdFilterElements = NULL_PTR,       
	 .CanExtFilterElements = NULL_PTR,       
	.NumOfStdIdFilters = 0,   
	.NumofExtIdFilters = 0
  }  

};

static const CanHardwareObject CanHardwareObjectConfigController[] = {	

{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_1,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[0],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_2,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[1],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_3,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[2],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_4,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[3],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_5,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[4],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_6,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[5],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Rx_Std_MailBox_7,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 RECEIVE,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 &CanHardwareFilterconfig[6],
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_1,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_2,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_3,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_4,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_5,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  } ,
{
	 .CanFdPaddingValue = 0,      
	.CanHwObjectCount =  13,		
	.CanHandleType	=	 FULL1,
	.CanObjectId	=	 CanConf_ETAS_CAN_Tx_Std_MailBox_6,                 
	.CanIdType		=	 STANDARD,
	.CanObjectType	=	 TRANSMIT,
	.CanTriggerTransmitEnable = FALSE,
	.CanHwFilter 	= 	 NULL_PTR ,
	.CanControllerId =  CanConf_CanController_ETAS_CAN		  }
};

//CAN-FD baudrate setting 
static const Can_ControllerFdBaudrateConfigType CanSupportedFdBaudratesController[] =
{


};




static const Can_ControllerBaudrateConfigType CanSupportedBaudratesController[] =
{
/* Baudrates Configured For Controller 0*/ 

{	
	.CanControllerBaudRateConfigID 	= CanConf_CanController_ETAS_CAN_ETAS_CAN_BR_0,
	.CanControllerBaudRate 			= 500,
	.CanControllerPropSeg 			= 3,
	.CanControllerSeg1 				= 3,
	.CanControllerSeg2 				= 3,
	.CanControllerSyncJumpWidth 	= 1,
		.CanControllerFdBaudrateConfig = NULL_PTR         } 



};
static const Can_ControllerConfigType CanControllerConfigData[] =
{


  {
	.CanControllerActivation		= TRUE,
	.CanBusOffProcessing 			= POLLING,
	.CanRxProcessing 				= POLLING,
	.CanTxProcessing				= POLLING,
	.CanControllerDefaultBaudrate 	= &CanSupportedBaudratesController[0],
    .CanControllerBaudrateConfig 	= &CanSupportedBaudratesController[0],
    .CanControllerSupportedBaudratesCount = 1,
	.CanControllerId	 			= CanConf_CanController_ETAS_CAN,					
				
				
				
				
				
														
	.CanHoh 						= &CanHardwareObjectConfigController[0],
	.AlpsStdRejectNoMatch = TRUE,
	.AlpsExtRejectNoMatch = TRUE,
			.CanCpuClockRef 				= 40000000,	
  }	
  

};

static const Can_ConfigSetType CanConfigSetData = {
 .CanController = &CanControllerConfigData[0]
};



const Can_ConfigType CanConfigData = { 

 .CanConfigSet = &CanConfigSetData
};

#define CAN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Can_MemMap.h"
