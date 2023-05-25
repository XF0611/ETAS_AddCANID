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
**  FILENAME   : Dio_Cfg.h                                                    **
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

#ifndef DIO_CFG_H
#define DIO_CFG_H


#define DIO_VENDOR_ID		0x8b	        
#define DIO_MODULE_ID       120	
#define DIO_INSTANCE_ID     0	


#define DIO_AR_RELEASE_MAJOR_VERSION	4
#define DIO_AR_RELEASE_MINOR_VERSION	3    
#define DIO_AR_RELEASE_PATCH_VERSION	1 

#define DIO_SW_MAJOR_VERSION    3	
#define DIO_SW_MINOR_VERSION    4	
#define DIO_SW_PATCH_VERSION    0	

/* Pre-Compile parameters */
#define DIO_DEV_ERROR_DETECT           (STD_OFF)
#define DIO_VERSION_INFO_API           (STD_OFF)
#define DIO_FLIP_CHANNEL_API           (STD_OFF)

#define DIO_END_OF_LIST 				0xffu

/*Dio Port Symbolic Names*/
#define DioConf_DioPort_DioPort_0                       ((Dio_PortType)0U)
#define DioConf_DioPort_DioPort_1                       ((Dio_PortType)1U)

/*Dio Channnel Symbolic Names*/

#define DioConf_DioPort_0_DioChannel_0_Pin13  ((Dio_ChannelType)13)	
#define DioConf_DioPort_0_DioChannel_1_Pin14  ((Dio_ChannelType)14)	
#define DioConf_DioPort_0_DioChannel_2_Pin15  ((Dio_ChannelType)15)	
#define DioConf_DioPort_0_DioChannel_3_Pin7  ((Dio_ChannelType)7)	
#define DioConf_DioPort_0_DioChannel_4_Pin8  ((Dio_ChannelType)8)	
#define DioConf_DioPort_0_DioChannel_5_Pin2  ((Dio_ChannelType)2)	
#define DioConf_DioPort_0_DioChannel_6_Pin1  ((Dio_ChannelType)1)	
#define DioConf_DioPort_0_DioChannel_7_Pin0  ((Dio_ChannelType)0)	
#define DioConf_DioPort_0_DioChannel_8_Pin12  ((Dio_ChannelType)12)	
#define DioConf_DioPort_0_DioChannel_9_Pin11  ((Dio_ChannelType)11)	
#define DioConf_DioPort_0_DioChannel_10_Pin10  ((Dio_ChannelType)10)	
#define DioConf_DioPort_0_DioChannel_11_Pin9  ((Dio_ChannelType)9)	


#define DioConf_DioPort_1_DioChannel_0_Pin16  ((Dio_ChannelType)16)	
#define DioConf_DioPort_1_DioChannel_1_Pin28  ((Dio_ChannelType)28)	
#define DioConf_DioPort_1_DioChannel_2_Pin29  ((Dio_ChannelType)29)	
#define DioConf_DioPort_1_DioChannel_3_Pin26  ((Dio_ChannelType)26)	
#define DioConf_DioPort_1_DioChannel_4_Pin27  ((Dio_ChannelType)27)	
#define DioConf_DioPort_1_DioChannel_5_Pin25  ((Dio_ChannelType)25)	
#define DioConf_DioPort_1_DioChannel_6_Pin17  ((Dio_ChannelType)17)	
#define DioConf_DioPort_1_DioChannel_7_Pin18  ((Dio_ChannelType)18)	
#define DioConf_DioPort_1_DioChannel_8_Pin19  ((Dio_ChannelType)19)	
#define DioConf_DioPort_1_DioChannel_9_Pin20  ((Dio_ChannelType)20)	

#endif  /* End of DIO_CFG_H */
