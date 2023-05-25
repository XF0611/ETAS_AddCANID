/*
********************************************************************************************************************** 
*
* COPYRIGHT RESERVED, 2017 Robert Bosch GmbH. All rights reserved.
* The reproduction, distribution and utilization of this document as well as the communication of its contents to
* others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
* All rights reserved in the event of the grant of a patent, utility model or design.
*
********************************************************************************************************************** 
*/


/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.3.0.0                Module Package Version
 * Editor_____: 9.0                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/



/* ---------------------------------------------------------------------*/
/* Include protection                                                   */
/* ---------------------------------------------------------------------*/
#ifndef  COMM_MAIN_H
#define  COMM_MAIN_H

#include "ComM_Cfg.h"

/* ---------------------------------------------------------------------*/
/* External declarations                                                */
/* ---------------------------------------------------------------------*/
#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"

/* ---------------------------------------------------------------------*/
/*  Name : ComM_MainFunction_ComMChannel_Can_Network_Channel                                          */
/*  Description : Main function for Bus Type COMM_BUS_TYPE_CAN  channel ComMChannel_Can_Network_Channel           */
/* ---------------------------------------------------------------------*/
extern FUNC(void,COMM_CODE) ComM_MainFunction_ComMChannel_Can_Network_Channel(void);

#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"




#endif
