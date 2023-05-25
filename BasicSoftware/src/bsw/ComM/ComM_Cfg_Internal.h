


/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.10.0.0                Module Package Version
 * Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/




#ifndef  COMM_CFG_INTERNAL_H
#define  COMM_CFG_INTERNAL_H


/***********************************************************************************/
/*                                        INCLUDES                                 */
/***********************************************************************************/
#include "ComM_Cfg.h"




#define COMM_CAN                STD_ON
#define COMM_LIN                STD_OFF
#define COMM_FLXRY              STD_OFF
#define COMM_ETH                STD_OFF

#define COMM_NO_OF_USERS        1
#define COMM_NO_OF_CHANNELS     1

#define COMM_NVM_NO_WAKEUP_SIZE             1
#define COMM_MAX_CHANNEL_REQUESTERS         1
#define COMM_DEV_ERROR_DETECT               STD_ON
#define COMM_SYNCHRONOUS_WAKE_UP            STD_OFF
#define COMM_USERID_MIN                     0     
#define COMM_USERID_MAX                     0

#define COMM_VERSION_INFO_API               STD_OFF

/* Configure Nm Support */
#define COMM_NM_SUPPORT                     STD_ON

/* Configure DCM support */
#define COMM_DCM_SUPPORT                    STD_ON

/* Configure RTE support */
#define COMM_RTE_SUPPORT                    STD_ON
#define COMM_FULLCOMREQ_NOTIF               STD_OFF

/* PNC SUPPORT */
#define COMM_PNC_ENABLED                    STD_OFF

/*Zero Pnc Vector Avoidance*/
#define COMM_ZERO_PNC_VECTOR_AVOIDANCE      STD_OFF

/* PNC ERA SUPPORT */
#define COMM_PNC_GW_ENABLED                 STD_OFF

/* LIMIT TO NOCOM SUPPORT */
#define COMM_LIMIT_TO_NOCOM_ENABLED         STD_ON

/* PREVENT WAKEUP SUPPORT */
#define COMM_PREVENT_WAKEUP_ENABLED         STD_OFF

/*NVM SUPPORT*/
#define COMM_NVM_ENABLED                    STD_OFF

/*MANAGING CHANNEL PRESENT*/
#define COMM_MANAGING_CHANNEL_PRESENT       STD_OFF

#if(COMM_NM_SUPPORT!= STD_OFF)
/* If Nm is used, this switch determines the Nm Variant */
#define COMM_NM_VARIANT                     COMM_NM_VARIANT_FULL
#endif

/* Common inhibition switch */
#define COMM_INHIBITION_ENABLED     ((COMM_PREVENT_WAKEUP_ENABLED != STD_OFF) || (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF))

/* Reset After forcing to No Com */
#define COMM_RESET_AFTER_NO_COM             STD_OFF

#define COMM_MAX_NO_OF_PNCS                 (56)

#define COMM_NO_OF_PNCS                     (0)

/* Configured PNC Vector Offset accross BusNms */
#define COMM_PNC_VECTOR_OFFSET              1u

/* Configured PNC Vector Length accross BusNms */
#define COMM_PNC_VECTOR_LENGTH              7u

/* Configured PNC Vector start bit position */
#define COMM_PNC_VECTOR_STARTBITPOSITION    (COMM_PNC_VECTOR_OFFSET * 8u)

/* Number of channels with ERA signals configured */
#define COMM_NO_OF_CH_WITH_ERA              (0)

/* Number of possible Bus types which have EIRA configured.
   The Const and RAM structures to store EIRA information is based on the ComMChannels configured rather than the actual 
   EIRA Signals present. This is done to keep the implementation common for both pre-compile and post-build variants.
*/
#define COMM_NO_OF_POSSIBLE_BUSTYPECH_WITH_EIRA     (1)







#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
typedef ComM_UserHandleType ComM_InternalHandleArrayType;
typedef struct
{
/* MR12 RULE 1.2 VIOLATION: Parameter COMM_MAX_CHANNEL_REQUESTERS is generated with different value based on 
 * configuration and is not always 1. Reordering the member element ComM_InternalHandleArrayType would potentially 
 * impact the functionality. Since the warning has no memory misalignment, no functional impact and not critical, 
 * this warning can be ignored. */
    uint8 numberOfRequesters;
    ComM_InternalHandleArrayType handleArray[COMM_MAX_CHANNEL_REQUESTERS];
} ComM_UserHandleArrayType;
#endif

/* BusSm API structure  */

typedef struct
{
/* MR12 RULE 8.2 VIOLATION: Parameter identifiers missing in declaration of a function type and will 
 * not impact on functionality */
     Std_ReturnType (*BusSm_RequestComMode) (NetworkHandleType, ComM_ModeType);
     Std_ReturnType (*BusSm_GetCurrentComMode) (NetworkHandleType,
                    ComM_ModeType * ComM_ModePtr);
}ComM_BusSmApiType_tst;


/****************************************Users structure type**************************/
typedef struct
{
    const uint8 *       DirectChannels_pcu8;
#if (COMM_PNC_ENABLED != STD_OFF)
    const uint8 *       Pncs_pcu8;
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
    uint8               NumDirectChannels_u8;
    uint8               NumAllChannels_u8;
#if (COMM_PNC_ENABLED != STD_OFF)
    uint8               NumPnc_u8;
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
}ComM_UsersType_tst;

/****************************************Channels structure type**************************/
typedef struct ComM_ChannelTypeStruct
{
#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))
    const uint8 *                   Pncs_pcu8;
#endif
#if (COMM_INHIBITION_ENABLED)
    const uint8 *                   DirectUsers_pcu8;
#endif
    const uint8 *                   AllUsers_pcu8;
    ComM_BusType_ten                BusType_en;
    ComM_NMVariantType_ten          ComMNmVariant_en;  
#if (COMM_PNC_GW_ENABLED != STD_OFF)
    ComM_PncGatewayType_ten         PncGwType_en;
#endif    /* #if (COMM_PNC_GW_ENABLED != STD_OFF)  */
    uint32                          NmLightTimeout_u32;
    uint16                          TMinFullComModeDuration_u16;
#if (COMM_PNC_ENABLED != STD_OFF)
    uint16                          TxSigId_u16;
#endif /* #if (COMM_PNC_ENABLED != STD_OFF)  */
    NetworkHandleType               ComMChannelId_u8;
#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))
    uint8                           numPncs_u8;
#endif /*#if ((COMM_INHIBITION_ENABLED) &&(COMM_PNC_ENABLED != STD_OFF))  */
#if (COMM_INHIBITION_ENABLED)
    uint8                           numDirectUsers_u8;
    uint8                           InhibitionInitValue_u8;
#endif /* #if (COMM_INHIBITION_ENABLED)  */
    uint8                           numAllUsers_u8;
#if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)
    uint8                           masterChannelId_u8;
#endif  /*  #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  */  
#if(COMM_PNC_ENABLED != STD_OFF)
    boolean                         PncNmRequest_b;
#endif /* #if (COMM_PNC_ENABLED == STD_ON)  */
#if(COMM_NVM_ENABLED != STD_OFF)
    boolean                         ComMNoWakeUpInhibitionNvmStorage_b;
#endif /*#if(COMM_NVM_ENABLED != STD_OFF)*/
#if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)
    boolean                         ComMMasterChannelPresent_b;
#endif  /*  #if (COMM_MANAGING_CHANNEL_PRESENT != STD_OFF)  */
    boolean                         ComMFullCommRequestNotificationEnabled_b;      
}ComM_ChannelType_tst;

/****************************************Pncs structure type**************************/
typedef struct ComM_PncStructType
{
    const uint8 *       Channels_pcu8;          /* Channels Per PNC */
#if(COMM_INHIBITION_ENABLED)
    const uint8 *       Users_pcu8;                /* Users per PNC */
#endif /* #if (COMM_INHIBITION_ENABLED)  */
    uint16              PrepareSleepTimer_u16;
#if(COMM_INHIBITION_ENABLED)
    uint8               NumUsers_u8;
#endif /*#if(COMM_INHIBITION_ENABLED)  */
    uint8               NumChannels_u8;
    uint8               PncId_u8;
#if(COMM_PNC_GW_ENABLED != STD_OFF)
    boolean             isPncGwCoordinated_b;     /* Holds the status if PNC is GW Coordinated */
#endif
}ComM_PncConfigStruct_tst;


/* ComM global config type structure */
#if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE)

#if(COMM_PNC_ENABLED != STD_OFF)
// For Signal array type for ERA and EIRA Rx signals of PNC
typedef const uint16 ComM_SignalArrayType[COMM_NO_OF_PNCS];
#endif      /* #if(COMM_PNC_ENABLED != STD_OFF) */

/* Definition of Global config structure of ComM  */
// Global config structure to store pointers to all config tables
/*typedef struct
{
    const ComM_ChannelType_tst *        ChannelConfig_pcst;          Holds pointer to channel list
    const ComM_UsersType_tst *          UserConfig_pcst;             Holds pointer to user list
#if(COMM_PNC_ENABLED != STD_OFF)
    const ComM_PncConfigStruct_tst *    PncConfig_pcst;              Holds pointer to PNC list
    const ComM_SignalArrayType *        EiraSignalTable_pcst;        Holds pointer to EIRA signal table
#if(COMM_PNC_GW_ENABLED != STD_OFF)
    const ComM_SignalArrayType *        EraSignalTable_pcst;         Holds pointer to ERA signla table
#endif
    boolean                             ComMPncEnabled_b;           Holds actual status of PNC switch
#endif
}ComM_GlobalConfigType_tst;*/

typedef struct
{
    const ComM_ChannelType_tst *        ChannelConfig_pcst;
    const ComM_UsersType_tst *          UserConfig_pcst;
#if(COMM_PNC_ENABLED != STD_OFF)
    const ComM_PncConfigStruct_tst *    PncConfig_pcst;
    const ComM_SignalArrayType *        EiraSignalTable_pcst;
#if(COMM_PNC_GW_ENABLED != STD_OFF)
    const ComM_SignalArrayType *        EraSignalTable_pcst;
#endif
    boolean                             ComMPncEnabled_b;
#endif
}ComM_GlobalConfigType_tst;

#endif  /* #if(COMM_CONFIGURATION_VARIANT != COMM_VARIANT_PRECOMPILE) */

#if (COMM_RTE_SUPPORT != STD_OFF)
extern void ComM_Prv_Rte_Switch_UM_currentMode(ComM_UserHandleType UserId_u8, uint8 lowestMode_u8);
#endif

#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
extern void ComM_CurrentChannelRequest_Rte_Write(uint8 ChannelId, const ComM_UserHandleArrayType * data);
#endif



/*************************************** *Extern Definitions **************************/

#define COMM_START_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"
extern const ComM_UserHandleType ComM_UserId_MappingTable_acst[];
#define COMM_STOP_SEC_CONFIG_DATA_8
#include "ComM_MemMap.h"


#define COMM_START_SEC_CONST_8
#include "ComM_MemMap.h"

#if (COMM_INHIBITION_ENABLED)
extern const ComM_InhibitionStatusType ComM_EcuGroupClassification_Init;
#endif

#define COMM_STOP_SEC_CONST_8
#include "ComM_MemMap.h"


#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"
extern const ComM_BusSmApiType_tst ComM_BusSmApi_acst[];
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_MemMap.h"

#define COMM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"
extern const ComM_ChannelType_tst ComM_ChanelList_acst[];
extern const ComM_UsersType_tst ComM_UserList_acst[];

#if (COMM_PNC_ENABLED != STD_OFF)
extern const ComM_PncConfigStruct_tst ComM_PncList_acst[];
#endif  /*  (COMM_PNC_ENABLED != STD_OFF)  */
#define COMM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "ComM_MemMap.h"


#define COMM_START_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"
#if (COMM_PNC_ENABLED != STD_OFF)

extern const uint16 ComM_EIRA_RxSig_acu16[];

#if(COMM_PNC_GW_ENABLED != STD_OFF)
extern const uint16 ComM_ERA_RxSig_acu16[];
#endif  /*  (COMM_PNC_GW_ENABLED != STD_OFF)  */

#endif  /* COMM_PNC_ENABLED != STD_OFF) */
#define COMM_STOP_SEC_CONFIG_DATA_16
#include "ComM_MemMap.h"




#endif  /* #ifndef  COMM_CFG_H  */
