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

/* Header Protection */
#ifndef PORT_ALPS_H
#define PORT_ALPS_H

#define PORT_START_SEC_CODE

/******************************************************************************
*  Seretice name    : Port_Hal_Init                                                  
*  Syntax           : void Port_Hal_Init(const Port_ConfigType* ConfigPtr)                                                      
*  Reentrancy       : Non reentrant                                             
*  Parameters (in)  : ConfigPtr - Pointer to configuration set                  
*  Parameters (out) : None                                                      
*  Return value     : None                                                      
*  Description      : Initializes the Port Driver module.                            
******************************************************************************/
void Port_Hal_Init(const Port_ConfigType *ConfigPtr);

#define PORT_STOP_SEC_CODE


#endif

