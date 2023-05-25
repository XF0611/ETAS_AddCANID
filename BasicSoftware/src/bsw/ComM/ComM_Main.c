

/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.10.0.0                Module Package Version
 * Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/



/* ---------------------------------------------------------------------*/
/* Inlcude section                                                      */
/* ---------------------------------------------------------------------*/
#include "ComM_Priv.h"


#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/*
 *  Name : ComM_MainFunction_Can_Network_ETAS_Channel_Can_Network_ETAS
 *  Description : Main function for Bus Type COMM_BUS_TYPE_CAN channel Can_Network_ETAS_Channel_Can_Network_ETAS
 *
 */
void ComM_MainFunction_Can_Network_ETAS_Channel_Can_Network_ETAS(void)
{
    ComM_Prv_ChannelMainFunction(0) ;
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


#if (COMM_PNC_GW_ENABLED == STD_ON)



#endif /*  #if (COMM_PNC_GW_ENABLED == STD_ON)  */

#if (COMM_PNC_ENABLED == STD_ON)


/* EIRA call backs are generated for all possible bus types, depending on the bus types configred in
   ComMChannel container. This is to ensure that call backs are always available in Post-build configuration.
   Precompile : same approach is followed to keep the code same. */
#define COMM_START_SEC_CODE
#include "ComM_MemMap.h"

/*
 *  Name : ComM_EIRACallBack_COMM_BUS_TYPE_CAN
 *  Description : EIRA callback for bus type COMM_BUS_TYPE_CAN
 *                  This function will be called whenever EIRA signal for this bus type changes
 */

/* MR12 RULE 8.5 VIOLATION: Multiple declarations of external object or function. This is done on 
 * purpose to reduce the dependency between the modules and this does not have any functional Impact */ 
extern void ComM_EIRACallBack_COMM_BUS_TYPE_CAN(void);

void ComM_EIRACallBack_COMM_BUS_TYPE_CAN(void)
{
    ComM_Prv_EIRA_CallBack(0);
}

#define COMM_STOP_SEC_CODE
#include "ComM_MemMap.h"


#endif /* #if (COMM_PNC_ENABLED == STD_ON)  */

/*----------------------------------------------------------------------*/

/************************************************************************/
