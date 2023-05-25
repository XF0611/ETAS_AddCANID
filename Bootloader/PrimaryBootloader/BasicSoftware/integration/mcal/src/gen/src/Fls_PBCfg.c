/* ==========================================
*   Copyright (c) 2020, Calterah Semiconductor, Inc. 
*   All Rights Reserved.
========================================== */

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
**  FILENAME  : Fls_PBCfg.c                                                   **
**                                                                            **
**                                                                            **
**  DATE, TIME: 2022-12-23, 16:22:51                                      **
**                                                                            **
**  GENERATOR : Build b210315-0853                                          **
**                                                                            **
**  AUTHOR    : Calterah Semiconductor Inc                                    **
**                                                                            **
**  VENDOR    : Calterah Semiconductor                                        **
**                                                                            **
**  DESCRIPTION  : Fls configuration file                                     **
**                                                                            **
**  MAY BE CHANGED BY USER [yes/no]: no                                       **
**                                                                            **
*******************************************************************************/

#include "Fls.h"
#include "Spi.h"

/* Function declaration of Fls Job End Notification */
extern void Fee_JobEndNotification(void);

/* Function declaration of Fls Job Error Notifications */
extern void Fee_JobErrorNotification(void);


FlsExternalDriver FlsSpiReference[] = 
{
};



FlsSector Sector[]=
{
	
	{		
	  .FlsNumberOfSectors = 0U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 65536U
	}, 
	
	{		
	  .FlsNumberOfSectors = 1U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 69632U
	}, 
	
	{		
	  .FlsNumberOfSectors = 2U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 73728U
	}, 
	
	{		
	  .FlsNumberOfSectors = 3U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 77824U
	}, 
	
	{		
	  .FlsNumberOfSectors = 4U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 81920U
	}, 
	
	{		
	  .FlsNumberOfSectors = 5U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 86016U
	}, 
	
	{		
	  .FlsNumberOfSectors = 6U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 90112U
	}, 
	
	{		
	  .FlsNumberOfSectors = 7U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 94208U
	}, 
	
	{		
	  .FlsNumberOfSectors = 8U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 98304U
	}, 
	
	{		
	  .FlsNumberOfSectors = 9U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 102400U
	}, 
	
	{		
	  .FlsNumberOfSectors = 10U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 106496U
	}, 
	
	{		
	  .FlsNumberOfSectors = 11U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 110592U
	}, 
	
	{		
	  .FlsNumberOfSectors = 12U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 114688U
	}, 
	
	{		
	  .FlsNumberOfSectors = 13U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 118784U
	}, 
	
	{		
	  .FlsNumberOfSectors = 14U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 122880U
	}, 
	
	{		
	  .FlsNumberOfSectors = 15U,
	  .FlsPageSize = 256U,
	  .FlsSectorSize = 4096U,
	  .FlsSectorStartaddress = 126976U
	} 
	 
};


const Fls_ConfigType  Fls_Config = 
{


	.FlsCallCycle = 0.001f,                	 

	.FlsJobEndNotification = &Fee_JobEndNotification ,   			 

	.FlsJobErrorNotification = &Fee_JobErrorNotification ,  			 

	.SectorList = Sector,
};




