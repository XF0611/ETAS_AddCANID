

/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.10.0.0                Module Package Version
 * Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
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
#include "ComM_MemMap.h"

/* ---------------------------------------------------------------------*/
/*  Name : ComM_MainFunction_Can_Network_ETAS_Channel_Can_Network_ETAS                                          */
/*  Description : Main function for Bus Type COMM_BUS_TYPE_CAN  channel Can_Network_ETAS_Channel_Can_Network_ETAS           */
/* ---------------------------------------------------------------------*/

#if ( COMM_ECUC_RB_RTE_IN_USE != STD_ON )
extern void ComM_MainFunction_Can_Network_ETAS_Channel_Can_Network_ETAS(void);
#endif

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"




#endif
