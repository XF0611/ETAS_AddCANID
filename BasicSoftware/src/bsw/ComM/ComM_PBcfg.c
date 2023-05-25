

/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.10.0.0                Module Package Version
 * Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/


/***********************************************************************************/
/*                                        INCLUDES                                 */
/***********************************************************************************/
#include "ComStack_Types.h"
#include "ComM_Cfg.h"
#include "ComM_PBcfg.h"
#include "ComM_Priv.h"
#include "Std_Types.h"

/**************************************Start of channel data***************************************/

#define COMM_START_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"
#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))
#endif
#if (COMM_INHIBITION_ENABLED)

static const uint8 ComM_DirectUsersPerChannel_Can_Network_ETAS_Channel_Can_Network_ETAS[] = {
ComMConf_ComMUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS};

#endif


static const uint8 ComM_AllUsersPerChannel_Can_Network_ETAS_Channel_Can_Network_ETAS[] = {
    ComMConf_ComMUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS    };


#define COMM_STOP_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"
/**************************************End of channel data**************************************/

/********************************* Start of channel list *******************************************/
#define COMM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"

const ComM_ChannelType_tst ComM_ChanelList_acst[] = {
{
    /* Main function period is 10000 */
    /* Channel : Can_Network_ETAS_Channel_Can_Network_ETAS */
#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))
    NULL_PTR,
#endif /* ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))  */
#if (COMM_INHIBITION_ENABLED)
    ComM_DirectUsersPerChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,
#endif /*#if (COMM_INHIBITION_ENABLED)  */
    ComM_AllUsersPerChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,
    COMM_BUS_TYPE_CAN,
    FULL,
#if (COMM_PNC_GW_ENABLED != STD_OFF)
    COMM_GATEWAY_TYPE_ACTIVE,
#endif    /* #if (COMM_PNC_GW_ENABLED != STD_OFF)  */
    1000,
    1,
#if (COMM_PNC_ENABLED != STD_OFF)
    0xFFFF,
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
    0,
#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))
    0,
#endif /*#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))  */
#if (COMM_INHIBITION_ENABLED)
    1,
    0x3,
#endif /* #if (COMM_INHIBITION_ENABLED)  */
    1,
#if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)   
    /*Master Channel is not present, it's own Channel Id*/
    ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS,
#endif  /*  #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  */     
#if (COMM_PNC_ENABLED != STD_OFF)
    FALSE,
#endif /* #if (COMM_PNC_ENABLED == STD_ON)  */
#if(COMM_NVM_ENABLED != STD_OFF)
    FALSE,
#endif /*#if(COMM_NVM_ENABLED != STD_OFF)*/
#if(COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  
    FALSE,
    /* No of Slave Channel users  = 0, */
#endif  /* #if(COMM_MANAGING_CHANNEL_PRESENT != STD_OFF) */  
    FALSE    
}
};

#define COMM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"
/*****************************************End of channel list*******************************************/

/*****************************************Start of user data*******************************************/
#define COMM_START_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"


static const uint8 ComM_DirectChannelsPerUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS[] = {
    ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS    
};



#define COMM_STOP_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"

/*****************************************Start of user list*******************************************/

#define COMM_START_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"

const ComM_UserHandleType ComM_UserId_MappingTable_acst[] =
{

0
};

#define COMM_STOP_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"

#define COMM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"

const ComM_UsersType_tst ComM_UserList_acst[] =
{
{
    /* External UserId is 0 */ 
    ComM_DirectChannelsPerUser_ComMUser_Can_Network_ETAS_Channel_Can_Network_ETAS,
#if (COMM_PNC_ENABLED != STD_OFF)
    NULL_PTR,
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
    1,
    1,
#if (COMM_PNC_ENABLED != STD_OFF)
    0
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
}
};

#define COMM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"
/*****************************************End of user list*******************************************/


#if (COMM_PNC_ENABLED == STD_ON)

/*****************************************Start of pnc data*******************************************/
#define COMM_START_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"


#if(COMM_INHIBITION_ENABLED)

#endif

#define COMM_STOP_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"

/*****************************************End of pnc data*******************************************/
/*****************************************Start of pnc list*******************************************/
#if (COMM_PNC_ENABLED != STD_OFF)
#define COMM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"

const ComM_PncConfigStruct_tst ComM_PncList_acst[] =
{
};

#define COMM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
/*****************************************End of pnc list*******************************************/

#endif /* #if (COMM_PNC_ENABLED == STD_ON)  */

#if (COMM_PNC_GW_ENABLED == STD_ON)
#define COMM_START_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"


#define COMM_STOP_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"
#endif /* #if (COMM_PNC_GW_ENABLED == STD_ON)  */

#if (COMM_PNC_ENABLED == STD_ON)
#define COMM_START_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"

const uint16 ComM_EIRA_RxSig_acu16[COMM_NO_OF_POSSIBLE_BUSTYPECH_WITH_EIRA] =
{
    /* COMM_BUS_TYPE_CAN  */
    0xFFFF
};

#define COMM_STOP_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"
#endif /* #if (COMM_PNC_ENABLED == STD_ON)  */

#if (COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)
#define COMM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"
/* Definition of Global config structure of ComM  */
const ComM_GlobalConfigType_tst ComM_GlobalConfigData_cs =
{
    ComM_ChanelList_acst,                /* Pointer to channel list  */
    ComM_UserList,                  /* Pointer to user list  */
#if(COMM_PNC_ENABLED != STD_OFF)
    ComM_PncList,                   /* Pointer to PNC list  */
    ComM_EIRA_RxSig_acu16,                /* Pointer to EIRA Rx Signal table  */
#if(COMM_PNC_GW_ENABLED != STD_OFF)
    ComM_ERA_RxSig_acu16,                 /* Pointer to ERA Rx Signal table  */
#endif
    FALSE    /* Actual value of PNC switch  */
#endif
};

#define COMM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"
#define COMM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"
static const Std_VersionInfoType ComM_VersionInfo =
{
    6,
    12,
    10,
    0,
    0
};
#define COMM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"
#endif      /* #if (COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)  */

#if (COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)
#define COMM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"

/* Definition of Base config structure  */
const ComM_ConfigType ComM_Config =
{
    /* Void pointer initialised with pointer to an instance of structure ComM_GlobalConfigData_cs */
    &ComM_GlobalConfigData_cs,
    &ComM_VersionInfo
};
#define COMM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "ComM_MemMap.h"
#endif      /* #if (COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)  */

