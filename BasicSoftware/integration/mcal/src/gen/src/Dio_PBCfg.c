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
**  FILENAME   : Dio_PBCfg.c                                                  **
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
**  DESCRIPTION : DIO configuration generated out of ECU configuration        **
**                file (Dio.bmd)                                              **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                             Includes                                       **
*******************************************************************************/
/* Include Dio Module File */
#include "Dio.h"

#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Dio_MemMap.h"



const Dio_PortType DioPortConfigData[]=
{
	DioConf_DioPort_DioPort_0,
	DioConf_DioPort_DioPort_1,
	DIO_END_OF_LIST
};

const Dio_ChannelConfigType DioChannelConfigData[]=
{

  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_0_Pin13,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_1_Pin14,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_2_Pin15,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_3_Pin7,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_4_Pin8,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_5_Pin2,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_6_Pin1,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_7_Pin0,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_8_Pin12,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_9_Pin11,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_10_Pin10,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_0_DioChannel_11_Pin9,
    .DioPinDirection = DIO_PIN_OUT
  },

  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_0_Pin16,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_1_Pin28,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_2_Pin29,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_3_Pin26,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_4_Pin27,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_5_Pin25,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_6_Pin17,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_7_Pin18,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_8_Pin19,
    .DioPinDirection = DIO_PIN_OUT
  },
  {
    .DioChannelId = DioConf_DioPort_1_DioChannel_9_Pin20,
    .DioPinDirection = DIO_PIN_OUT
  },

  DIO_END_OF_LIST

};
	
const Dio_ConfigType Dio_ConfigData=
{

  .DioChannelConfigData = &DioChannelConfigData[0],  
  .DioPortConfigData = &DioPortConfigData[0]
};

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
//ETAS BIP Porting TODO #include "Dio_MemMap.h"
