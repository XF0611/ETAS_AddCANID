

#ifndef  CANSM_H
#define  CANSM_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Cfg.h"
#include "ComM.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*********************************** DET Related error code *****************************************/

/* DET error definition for CanSM uninitialised */
#define CANSM_E_UNINIT                    0x01u

/* DET error definition for NULL POINTER is passed */
#define CANSM_E_PARAM_POINTER             0x02u

/* DET error definition for invalid network handle */
#define CANSM_E_INVALID_NETWORK_HANDLE    0x03u

/* DET error definition for invalid  Controller ID */
#define CANSM_E_PARAM_CONTROLLER          0x04u

/* DET error definition for invalid  tranceiver ID */
#define CANSM_E_PARAM_TRANSCEIVER         0x05u



/* DET error definition for invalid  Mode request for a network failed */
#define CANSM_E_MODE_REQUEST_TIMEOUT      0x0Au

/* DET error definition for Null pointer in case of PBS */
#define CANSM_E_INIT_FAILED               0x0Cu




/* DET error definition for communication mode request */
#define CANSM_E_INVALID_COMM_REQUEST      0x08u

/*************************************Service Id's for the Api's***************************/

/* Service Id for CanSM_Init API */
#define CANSM_INIT_SID                       0x00u

#if (CANSM_VERSION_INFO_API == STD_ON)
/*Service Id for CanSM_GetVersionInfo API */
#define CANSM_GETVERSIONINFO_SID             0x01u
#endif

/* Service Id for CanSM_RequestComMode API */
#define CANSM_REQUESTCOMMODE_SID             0x02u

/* Service Id for CanSM_GetCurrentComMode API */
#define CANSM_GETCURRENTCOMMODE_SID          0x03u

/* Service Id for CanSM_ControllerBusOff API */
#define CANSM_CONTROLLERBUSOFF_SID           0x04u

/*Service Id for CanSM_MainFunction API */
#define CANSM_MAINFUNCTION_SID               0x05u

#if(CANSM_PN_SUPPORT_CONFIGD == STD_ON)
/* Service Id for CanSM_ConfirmPnAvailability API */
#define CANSM_CONFIRMPNAVAILABILITY_SID      0x06
#endif

/* Service Id for CanSM_ControllerModeIndication API */
#define CANSM_CTRLMODEINDICATION_SID         0x07u

#if(CANSM_PN_SUPPORT_CONFIGD == STD_ON)
/* Service Id for CanSM_CheckTransceiverWakeFlagIndication API */
#define CANSM_CLEARTRCVWUFFLAGIND_SID        0x08u
#endif

#if (CANSM_CFG_TRCV_CANIF_SUPPORT == STD_ON)
/* Service Id for CanSM_TransceiverModeIndication API */
#define CANSM_TRCVMODEINDICATION_SID         0x09u
#endif

#if(CANSM_PN_SUPPORT_CONFIGD == STD_ON)
/* Service Id for CanSM_CheckTransceiverWakeFlagIndication */
#define CANSM_CHECKTRCVWAKEFLAGIND_SID       0x0au
#endif

/* Service Id for CanSM_TxTimeoutException API */
#define CANSM_TXTIMEOUTEXCEPTION_SID         0x0bu

/* BSW-16966 */ /* [$DD_BSWCODE 40504] */
/*Service Id for CanSM_StartWakeupSource API */
#define CANSM_STARTWAKEUPSOURCE_SID      0x11u

/* BSW-16966 */ /* [$DD_BSWCODE 40515] */
/*Service Id for CanSM_StoptWakeupSource API */
#define CANSM_STOPWAKEUPSOURCE_SID       0x12u

#if(CANSM_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
/*Service Id for CanSM_SetEcuPassive API */
#define CANSM_ECUPASSIVE_SID                 0x13u
#endif


#if (CANSM_SET_BAUDRATE_API == STD_ON)

/*Service Id for CanSM_SetBaudrate API */
#define CANSM_SETBAUDRATE_SID                0x0du

#endif
/* BSW-16966 */ /* [$DD_BSWCODE 40518] */

/* BSW-16966 */ /* [$DD_BSWCODE 40516] */
/* Service Id for CanSM_GetBusoff_Substate API */
#define CANSM_GETBUSOFFSUBSTATE_SID          0x81u

/* BSW-16966 */ /* [$DD_BSWCODE 40517] */
/*Service Id for CanSM_GetNetworkMode_Substate API */
#define CANSM_GETNETWORKMODESUBSTATE_SID     0x82u


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
/* FC_VariationPoint_START */
#if (CANSM_DSM_REINIT_ENABLED != STD_OFF)
typedef struct
{
    uint8 DEMReinitFlag_u8;
    Dem_EventStatusType                 CanSM_DEM_Eventstatus_u8;
} CanSM_NetworkConf_DEM_tst;
#endif
/* FC_VariationPoint_END */

/*Network Configuration*/
typedef struct
{
    const uint8 * Cntrl_startidx_pu8;
#if (CANSM_CALIBRATION == STD_ON)
    uint16 (*pBorTimeL1_pfct) (void);                     /* address of Count Value for Bus-Off Recovery time in Level 1 */
    uint16 (*pBorTimeL2_pfct) (void);                     /* address of Count Value for Bus-Off Recovery time in Level 2 */
#else
    uint16 BorTimeL1_u16;                 /* Count Value for Bus-Off Recovery time in Level 1 */
    uint16 BorTimeL2_u16;                 /* Count Value for Bus-Off Recovery time in Level 2 */
#endif
    uint16 BorTimeTxEnsured_u16;          /* Value in Counts for No Bus Off Event Check */
    Dem_EventIdType BusOffEventId_uo;                      /* Dem EventID for a bus-off for a CanSM Network handle */
    uint8 Trcv_hndle_u8;                 /* CAN Tranceiver Handle index */                /* Start Index of the Controllers */
    uint8 SizeofController_u8;           /* The number of Controllers present in the CanSM Network*/
    uint8 BorCntL1L2_u8;                 /* Count Value at which bus-off recovery state machine switches from level 1 to level 2 */
    NetworkHandleType ComM_channelId_uo;             /* ComM channel Id  */
#if (CANSM_BOR_TX_CONFIRMATION_POLLING != STD_OFF)
    boolean BorTxConfPolling_b;            /* Bus Off recovery based on Tx confirmation with polling*/
#endif
    boolean BusOffDelayConfig_b;           /*Boolean value to enable/disable bus off delay */
    boolean TrcvPnConfig_b;                /*partial Network enabled/disabled for Transceiver configured to CanSM Network*/
} CanSM_NetworkConf_tst;

/* Values of CANSM states for a Network  */
typedef enum
{
    CANSM_BSM_S_NOCOM = (ComM_ModeType)COMM_NO_COMMUNICATION,
    CANSM_BSM_S_SILENTCOM = (ComM_ModeType)COMM_SILENT_COMMUNICATION,
    CANSM_BSM_S_FULLCOM = (ComM_ModeType)COMM_FULL_COMMUNICATION,
    CANSM_BSM_S_NOT_INITIALIZED,
    CANSM_BSM_S_PRE_NOCOM,
    CANSM_BSM_WUVALIDATION,
    CANSM_BSM_S_PRE_FULLCOM,
    CANSM_BSM_S_SET_BAUDRATE,
    CANSM_BSM_S_SILENTCOM_BOR,
    CANSM_BSM_S_TX_TIMEOUT_EXCEPTION
}CanSM_NetworkModeStateType_ten;

/*This enum is used for Bus off  substates*/
typedef enum
{
    CANSM_BOR_IDLE                  = 0,
    CANSM_S_BUS_OFF_CHECK           = 1,
    CANSM_S_NO_BUS_OFF              = 2,
    CANSM_S_BUS_OFF_RECOVERY_L1     = 3,
    CANSM_S_RESTART_CC              = 4,
    CANSM_S_RESTART_CC_WAIT         = 5,
    CANSM_S_BUS_OFF_RECOVERY_L2     = 6
}CanSM_BusOffRecoveryStateType_ten;


/* This enum is used for storing the states of CanTrcv request */
typedef enum
{
    CANSM_CANTRCV_DEFAULT    = 0,
    CANSM_CANTRCV_NORMAL     = 1,
    CANSM_CANTRCV_SLEEP      = 2,
    CANSM_CANTRCV_STANDBY    = 3
}CanSM_ReqCanTrcv_States;

/* CanSMConfiguration Structure */
typedef struct
{
    const CanSM_NetworkConf_tst * CanSM_NetworkConf_pcst;                           /* Pointer to Network Configuration structure */
    const uint8 * CanSM_NetworktoCtrlConf_pcu8;                                      /* CanSm Network Index mapped to corresponding CAN controller */
    #if(CANSM_GETBUSOFFDELAY_SUPPORT == STD_ON)
    void (*User_GetBusOffDelay) (NetworkHandleType , uint8 * CANSM_APPL_CODE); /* User defined function for Bus Off delay */
    #endif
    const uint16 CanSMModeRequestRepetitionTime_u16;                                              /* Specifies in which time duration the CanSM module shall repeat mode change requests by using the API of the CanIf module*/
    const uint8 CanSMModeRequestRepetitionMax_u8;                                                 /* Holds Maximal amount of mode request repetitions without a respective mode indication */
    uint8 CanSM_SizeOfCanSMNetworks_u8;                                                        /* Holds the number of Networks */
    uint8 CanSM_ActiveConfigset_u8;                                                            /* Active configset index */

}CanSM_ConfigType;


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */
#define CANSM_START_SEC_VAR_CLEARED_8
#include "CanSM_MemMap.h"
extern uint8 CanSM_GetBusOffDelay_Value_u8[CANSM_NUM_CAN_NETWORKS];
#define CANSM_STOP_SEC_VAR_CLEARED_8
#include "CanSM_MemMap.h"

#define CANSM_START_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
extern const CanSM_ConfigType CanSM_ConfigSet[];
#define CANSM_STOP_SEC_CONST_UNSPECIFIED
#include "CanSM_MemMap.h"
/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"


#if (CANSM_ECUC_RB_RTE_IN_USE == STD_OFF)
extern void CanSM_MainFunction(void);
#endif
extern void CanSM_Init( const CanSM_ConfigType* ConfigPtr );

#if (CANSM_VERSION_INFO_API == STD_ON)
extern void CanSM_GetVersionInfo( Std_VersionInfoType * VersionInfo );
#endif

#if (CANSM_SET_BAUDRATE_API == STD_ON)
extern Std_ReturnType CanSM_SetBaudrate(NetworkHandleType Network,const uint16 BaudRateConfigID);
#endif

extern Std_ReturnType CanSM_StartWakeupSource(NetworkHandleType network);
extern Std_ReturnType CanSM_StopWakeupSource(NetworkHandleType network);

extern void CanSM_Rb_DisableBusOffRecovery(NetworkHandleType network);

extern Std_ReturnType CanSM_GetNetworkMode_Substate(NetworkHandleType network,
        CanSM_NetworkModeStateType_ten * NetworkMode_SubstatePtr
        );
extern Std_ReturnType CanSM_GetBusoff_Substate(uint8 ControllerId, CanSM_BusOffRecoveryStateType_ten* BORStatePtr);


#if(CANSM_TX_OFFLINE_ACTIVE_SUPPORT != STD_OFF)
extern Std_ReturnType CanSM_SetEcuPassive( boolean CanSM_Passive );
#endif

#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"

#endif /*CANSM_H*/

