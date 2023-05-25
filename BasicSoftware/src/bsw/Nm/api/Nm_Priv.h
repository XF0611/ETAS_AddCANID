
#ifndef NM_PRIV_H
#define NM_PRIV_H

/**
 * \defgroup NM_PRIV_H    Nm_Priv - Private Function of Nm.
 * \n
 */

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface private header file
 *
 * \scope               This is included only by Nm module
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "Nm.h"
#include "Nm_Cfg_SchM.h"


#ifdef NM_TESTCD_CALLBACK
#include "testcd_nm.h"
#endif
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to get Flexray Timer
 */
#define Nm_GetCurrFRTime()                          (SwFRTimer)
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to check if Timer has expired or not
 */
#define Nm_TimerExpired(PrevTimeStamp, Timeout)     ((Nm_GetCurrFRTime() - (PrevTimeStamp)) >= (Timeout))


/**
 * @ingroup NM_PRIV_H
 *
 *  Used for invalid handle ids
 */
#define NM_INVALID_HANDLE                    0xFFu

/**
 * @ingroup NM_PRIV_H
 *
 *  This value indicates that for a given channel, NMS reporting is disabled
 * The Com_SignalIdType is defined as uint16 in COM spec, hence the value of 0xFFFFu is chosen for the same
 */
#define NM_NMS_REPORT_DISABLED                0xFFFFu

/* Service IDs of Nm APIs */

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_INIT                              (0)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_PassiveStartUp
 */
#define NM_SID_PASSIVESTARTUP                    (0x01)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_NetworkRequest
 */
#define NM_SID_NETWORKREQUEST                    (0x02)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_NetworkRelease
 */
#define NM_SID_NETWORKRELEASE                    (0x03)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see  Nm_DisableCommunication
 */
#define NM_SID_DISABLECOMMUNICATION              (0x04)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_EnableCommunication
 */
#define NM_SID_ENABLECOMMUNICATION               (0x05)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_SetUserData
 */
#define NM_SID_SETUSERDATA                       (0x06)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetUserData
 */
#define NM_SID_GETUSERDATA                       (0x07)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetPduData
 */
#define NM_SID_GETPDUDATA                        (0x08)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_RepeatMessageRequest
 */
#define NM_SID_REPEATMESSAGEREQUEST              (0x09)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_GETNODEIDENTIFIER                 (0x0A)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetLocalNodeIdentifier
 */
#define NM_SID_GETLOCALNODEIDENTIFIER            (0x0B)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_CHECKREMOTESLEEPINDICATION        (0x0D)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetState
 */
#define NM_SID_GETSTATE                          (0x0E)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_GETVERSIONINFO                    (0x0F)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_MAINFUNCTION                         (0x10)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_NETWORKSTARTINDICATION            (0x11)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_NETWORKMODE                         (0x12)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_PREPAREBUSSLEEPMODE                 (0x13)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_BUSSLEEPMODE                         (0x14)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_PDURXINDCATION                       (0x15)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_STATECHANGENOTIFICATION              (0x16)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_REMOTESLEEPINDICATION                (0x17)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_REMOTESLEEPCANCELLATION              (0x18)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_SYNCHRONIZATIONPOINT                 (0x19)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_REPEATMSGINDICATION                  (0x1A)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_TXTIMEOUTEXCEPTION                   (0x1B)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_CARWAKEUPINDICATION                   (0x1d)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_COORDREADYTOSLEEPINDICATION              (0x1e)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_COORDREADYTOSLEEPCANCELLATION            (0x1f)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_SYNCHRONIZEMODE                          (0x21)

/* MACRO for DET Support */
#if (NM_DEV_ERROR_DETECT != STD_OFF)
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for functions with return type as void
 */
#define NM_DET_REPORT_ERROR(InstanceId, ApiId, ErrorId)       (void)Det_ReportError((NM_MODULE_ID),(InstanceId),(ApiId),(ErrorId))

#else
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to handle DET error report if device error detection is disabled
 */
#define NM_DET_REPORT_ERROR(InstanceId, ApiId, ErrorId)       do{}while(0)
#endif

/* Defines for NM State Report Feature Implementation */
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from bus sleep mode to repeat message state
 */
#define NM_NMS_RM_BSM                0x01
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from prepare bus sleep mode to repeat message state
 */
#define NM_NMS_RM_PBSM                0x02
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from repeat message to normal operation state
 */
#define NM_NMS_NO_RM                0x04
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from ready sleep to normal operation state
 */
#define NM_NMS_NO_RS                0x08
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from ready sleep to repeat message state
 */
#define NM_NMS_RM_RS                0x10
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from normal operation to repeat message state
 */
#define NM_NMS_RM_NO                0x20
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for resetting the counter
 */
#define NM_COUNTER_RESET            0x00

/**
 * @ingroup NM_PRIV_H
 *
 * Macro for resetting the Coord Ready to sleep bit in CBV
 */
#define NM_RESETBIT                    FALSE

/**
 * @ingroup NM_PRIV_H
 *
 * Macro for setting the Coord Ready to sleep bit in CBV
 */
#define NM_SETBIT                      TRUE


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */


/**
 * @ingroup NM_PRIV_H
 *
 * typedef enum
 *  {\n
 *   NM_INTERNAL_REQUEST,               network request made by Nm internal APIs\n
 *   NM_EXTERNAL_REQUEST                network request made by external module esp ComM\n
 *  } Nm_NetworkRequestSource_ten;\n
 *
 */
typedef enum
{
    NM_INTERNAL_REQUEST,
    NM_EXTERNAL_REQUEST
} Nm_NetworkRequestSource_ten;


/**
 * @ingroup NM_PRIV_H
 *
 * typedef enum
 *  {\n
 *   NM_SYNCPOINT_INVALID,
 *   NM_SYNCPOINT_INIT,                 SyncPoint state is at init \n
 *   NM_SYNCPOINT_READY,                SyncPoint is ready to receive Sync Call back  \n
 *   NM_SYNCPOINT_REACHED,              SyncPoint is reached and callback is called \n
 *  } Nm_SynchType_ten;\n
 *
 */
typedef enum
{
    NM_SYNCPOINT_INVALID,
    NM_SYNCPOINT_INIT,
    NM_SYNCPOINT_READY,
    NM_SYNCPOINT_REACHED
} Nm_SynchType_ten;

/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   NM_CLUSTER_INIT,                   Init state ..Co ordination is over\n
 *   NM_CLUSTER_PASSIVE,                T0 started , aborted shutdown can happen\n
 *   NM_CLUSTER_ACTIVE_ACTIVE,          Active state , active channel kept active by remote nodes\n
 *   NM_CLUSTER_ACTIVE_PASSIVE          Active state , passive channel kept active by active coordinator\n
 *  } Nm_ClusterStateType_ten;
 */
typedef enum
{
    NM_CLUSTER_INIT,
    NM_CLUSTER_PASSIVE,
    NM_CLUSTER_ACTIVE_ACTIVE,
    NM_CLUSTER_ACTIVE_PASSIVE
} Nm_ClusterStateType_ten;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   NM_CHANNEL_STATE_UNINIT,
 *   NM_CHANNEL_STATE_INIT,
 *   NM_CHANNEL_STATE_NORMAL,                    Network is requested in this state\n
 *   NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP,        T0 started..channel timer started waiting to release the network\n
 *   NM_CHANNEL_STATE_WAIT_SLEEP,                network released .. waiting for bussleep indication from nm\n
 *   NM_CHANNEL_STATE_BUS_SLEEP                  Bussleep state possible only after cooridnation has started\n
 *  } Nm_ChannelStateType_ten;
 */
typedef enum
{
    NM_CHANNEL_STATE_UNINIT,
    NM_CHANNEL_STATE_INIT,
    NM_CHANNEL_STATE_NORMAL,
    NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP,
    NM_CHANNEL_STATE_WAIT_SLEEP,
    NM_CHANNEL_STATE_BUS_SLEEP
} Nm_ChannelStateType_ten;

/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   NM_CHANNEL_MODE_INIT,
 *   NM_CHANNEL_MODE_BUS_SLEEP,            Channel Mode set as Bus Sleep when Nm_BusSleepMode Cbk is called\n
 *   NM_CHANNEL_MODE_PREPARE_BUS_SLEEP,    Channel Mode set as Prepare Bus Sleep when Nm_PrepareBusSleepMode Cbk is called\n
 *   NM_CHANNEL_MODE_SYNCHRONIZE,          Channel Mode set as Synchronize when Nm_SynchronizeMode Cbk is called\n
 *   NM_CHANNEL_MODE_NETWORK,
 *  } Nm_ChannelModeType_ten;
 */
typedef enum
{
    NM_CHANNEL_MODE_INIT,
    NM_CHANNEL_MODE_BUS_SLEEP,
    NM_CHANNEL_MODE_PREPARE_BUS_SLEEP,
    NM_CHANNEL_MODE_SYNCHRONIZE,
    NM_CHANNEL_MODE_NETWORK
} Nm_ChannelModeType_ten;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   RESETBIT,                      For clearing SleepCoordbit in control bit vector\n
     SETBIT                         For seting SleepCoordbit in control bit vector\n
 *  }
 */
/*typedef enum
{
    RESETBIT,
    SETBIT
} Nm_SetSleepReadyBitType;
*/


/**
 * @ingroup NM_PRIV_H
 *
 * typedef struct
 * {\n
 *   Nm_ChannelStateType_ten   ChannelState_en;              holds Channel state \n
 *   Nm_ChannelModeType_ten    ChannelMode_en;               holds Channel mode    \n
 *   Nm_TimerType          ShutdownDelayTimeStamp_u32;    variable to monitor shutdown delay \n
 *   boolean               ComMRequest_b;               status of ComM Full Communication request \n
 *   boolean               RemoteSleepStatus_b;         status of remote sleep detection \n
 *   boolean               ChannelModeStatus_b;         status of Channel mode    \n
 *  } Nm_NetworkRamType_tst;
 */
typedef struct
{
    Nm_ChannelStateType_ten ChannelState_en;
    Nm_ChannelModeType_ten  ChannelMode_en;
    Nm_TimerType            ShutdownDelayTimeStamp_u32;
    boolean                 ComMRequest_b;
    boolean                 RemoteSleepStatus_b;
    boolean                 ChannelModeStatus_b;
 } Nm_NetworkRamType_tst;



/**
 * @ingroup NM_PRIV_H
 *
 * typedef struct
 * { \n
 *   Nm_ClusterStateType_ten ClusterState_en;       Holds cluster status \n
 *   Nm_SynchType_ten        SyncPointState_en;    Holds the synchronization state of the cluster\n
 *   uint8               ctComMRequests_u8;    Number of Full Com Requests from ComM
 *   uint8               ctActive_u8;          Number of active channels that are not ready to sleep\n
 *   uint8               ctPassive_u8;         Number of passive channels that are not ready to sleep\n
 *   uint8               ctBusSleep_u8;        Number of channels that are yet to report bussleep..Used to signal the end of coordination\n
 *   boolean             InitStatus_b;         initialisation status \n
 * } Nm_GlobalRamType_tst;
 */
typedef struct
{
    Nm_ClusterStateType_ten ClusterState_en;
    Nm_SynchType_ten        SyncPointState_en;
    uint8                   ctComMRequests_u8;
    uint8                   ctActive_u8;
    uint8                   ctPassive_u8;
    uint8                   ctBusSleep_u8;
    boolean                 InitStatus_b;
} Nm_GlobalRamType_tst;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef struct
 *   {\n
 *   Std_ReturnType (*BusNm_NetworkRelease_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_NetworkRequest_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_PassiveStartUp_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_RequestBusSynchronization_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_CheckRemoteSleepIndication_pfct) ( NetworkHandleType,\n
 *                             boolean * );\n
 *   Std_ReturnType (*BusNm_DisableCommunication_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_EnableCommunication_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_GetLocalNodeIdentifier_pfct) ( NetworkHandleType,\n
 *                                    uint8 * );\n
 *   Std_ReturnType (*BusNm_GetNodeIdentifier_pfct) ( NetworkHandleType,\n
 *                                    uint8 * );\n
 *   Std_ReturnType (*BusNm_GetPduData_pfct) ( NetworkHandleType,\n
 *                                    uint8 * );\n
 *   Std_ReturnType (*BusNm_GetState_pfct) ( NetworkHandleType,\n
 *                                    Nm_StateType * ,\n
 *                                    Nm_ModeType * );\n
 *   Std_ReturnType (*BusNm_GetUserData_pfct) ( NetworkHandleType,\n
 *                                    uint8 * );\n
 *   Std_ReturnType (*BusNm_RepeatMessageRequest_pfct) ( NetworkHandleType);\n
 *   Std_ReturnType (*BusNm_SetUserData_pfct) ( NetworkHandleType,\n
 *                                     const uint8 * const );\n
 *   Std_ReturnType (*BusNm_SetSleepReadyBit_pfct) ( NetworkHandleType,  boolean);
 *   } Nm_BusNmApiType_tst;
 */
typedef struct
{
    Std_ReturnType (*BusNm_NetworkRelease_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_NetworkRequest_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_PassiveStartUp_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_RequestBusSynchronization_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_CheckRemoteSleepIndication_pfct) ( NetworkHandleType nw,
                                           boolean *ntw);
    Std_ReturnType (*BusNm_DisableCommunication_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_EnableCommunication_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_GetLocalNodeIdentifier_pfct) ( NetworkHandleType nw,
                                            uint8 *ntw);
    Std_ReturnType (*BusNm_GetNodeIdentifier_pfct) ( NetworkHandleType nw,
                                            uint8 *ntw );
    Std_ReturnType (*BusNm_GetPduData_pfct) ( NetworkHandleType nw,
                                            uint8 *ntw);
    Std_ReturnType (*BusNm_GetState_pfct) ( NetworkHandleType nw,
                                            Nm_StateType *ntw,
                                            Nm_ModeType *nk);
    Std_ReturnType (*BusNm_GetUserData_pfct) ( NetworkHandleType nw,
                                            uint8 *ntw);
    Std_ReturnType (*BusNm_RepeatMessageRequest_pfct) ( NetworkHandleType nw);
    Std_ReturnType (*BusNm_SetUserData_pfct) ( NetworkHandleType nw,
                                            const uint8 *ntw );
    Std_ReturnType (*BusNm_SetSleepReadyBit_pfct) ( NetworkHandleType nw,  boolean ntw);
} Nm_BusNmApiType_tst;

/**
 * @ingroup NM_PRIV_H
 *
 *  typedef struct
 * {\n
 *   void (*PduRxIndication) (const NetworkHandleType NetworkHandle);\n
 *   void (*RemoteSleepCancellation) (const NetworkHandleType NetworkHandle);\n
 *   void (*RemoteSleepIndication) (const NetworkHandleType NetworkHandle);\n
 *   void (*StateChangeInd) (\n
 *                                              const NetworkHandleType NetworkHandle,\n
 *                                              const Nm_StateType nmPreviousState,\n
 *                                              const Nm_StateType nmCurrentState\n
 *                                             );\n
 *  } Nm_UserCallbackType_tst;
 *
 */
typedef struct
{
    void (*PduRxIndication) (const NetworkHandleType NetworkHandle);
    void (*RemoteSleepCancellation) (const NetworkHandleType NetworkHandle);
    void (*RemoteSleepIndication) (const NetworkHandleType NetworkHandle);
    void (*StateChangeInd) (const NetworkHandleType NetworkHandle, const Nm_StateType nmPreviousState,
                                                                           const Nm_StateType nmCurrentState);

} Nm_UserCallbackType_tst;

/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold configuration data structure
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern const Nm_ConfigType Nm_ConfData_cs[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold the address of <Bus>Nm apis'
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern const Nm_BusNmApiType_tst Nm_BusNmApi[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold the address of user callback functions
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern const Nm_UserCallbackType_tst Nm_UserCallbacks;
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold ComM Id mapped to <Bus>Nm
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern const NetworkHandleType Nm_HandleTable[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold global configuration data structure
 */
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
extern Nm_GlobalRamType_tst Nm_GlobalData_s[];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold global configuration data structure
 */
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
extern Nm_NetworkRamType_tst Nm_ChannelData_s[];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
#endif

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */


#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"
extern  Nm_TimerType SwFRTimer;
#define NM_STOP_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_Prv_NetworkRequest(boolean active_b, NetworkHandleType NetworkHandle_u8 , Nm_NetworkRequestSource_ten source_en );
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_Prv_NetworkRelease(NetworkHandleType NetworkHandle_u8 , Nm_NetworkRequestSource_ten source_en);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_Reset(const NetworkHandleType ClusterIdx);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif /* NM_PRIV_H */

