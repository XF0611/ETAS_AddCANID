
#ifndef NM_H
#define NM_H

/**
\defgroup NM_H    Nm - Autosar Generic Network Management Interface.
*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface main header file
 *
 * \scope               This is included by all the applications controlling NM
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Types header for common types */
#include "NmStack_Types.h"

#include "ComStack_Types.h"
/* NM Interface configuration header */
#include "Nm_Cfg.h"

#if(NM_ENABLE_INTER_MODULE_CHECKS)
# if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#  error "AUTOSAR major version undefined or mismatched"
# endif
# if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#  error "AUTOSAR minor version undefined or mismatched"
# endif
#endif /* #if(NM_ENABLE_INTER_MODULE_CHECKS) */

#if (NM_AR45_VERSION != STD_OFF)

/* Development Error Codes for DET Support*/
/**
 * @ingroup NM_H
 *
 * Macro to indicate unitialization of Nm.
 */
#define NM_E_UNINIT                0x00

/**
 * @ingroup NM_H
 *
 * Macro to indicate Nm Network handle is not configured.
 */
#define NM_E_INVALID_CHANNEL           0x01

/**
 * @ingroup NM_H
 *
 * Macro to indicate NULL pointer is passed as an argument to a function.
 */
#define    NM_E_PARAM_POINTER       0x02


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
/**
 * @ingroup NM_H
 *
 * This typedef is used to define datatype of Nm_TimerType.
 */

typedef uint32 Nm_TimerType;

/* type definition of the configuration structure containing pointers to configuration structures of
   CAN NM, FR NM , UDP NM and J1939 NM
 */
/**
 * @ingroup NM_H
 *
 * This is the configuration of Nm  \n
 * typedef struct                   \n
 * {                                \n
 *      Nm_TimerType        ShutdownDelay;      This variable consists the value of configured time, for which \n
 *                                              NmCoordinator can wait in Preparewaitsleep before entering into \n
 *                                              waitsleep. \n
 * #if (NM_STATE_REPORT_ENABLED != STD_OFF) \n
 *      Com_SignalIdType    NMSSignal;          This variable decides whether NMS report is enabled or not  \n
 * #endif \n
 *      Nm_BusType_ten          BusNmType;          This variable stores the type of Bus. \n
 *      NetworkHandleType   ClusterIdx;         This variable Stores the index of the cluster. \n
 *      NetworkHandleType   comMHandle;         This variable stores the handle of comM channel. \n
 * } Nm_ConfigType;
 */
typedef struct
{
    Nm_TimerType        ShutdownDelay;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    Com_SignalIdType    NMSSignal;
#endif
    Nm_BusType_ten      BusNmType;
    Nm_ChannelType_ten  NmChannelType;
    NetworkHandleType   ClusterIdx;
    NetworkHandleType   comMHandle;
    boolean             NmChannelSleepMaster;
    boolean             NmSynchronizingNetwork;
    boolean             NmComUserDataSupport;
    boolean             NmPassiveModeEnabled;

} Nm_ConfigType;


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

/**
 * @ingroup NM_H
 *
 *  This is the main function of NM Coordinator \n
 *
 *  @Param None \n
 *  @return None \n
 */
#if NM_ECUC_RB_RTE_IN_USE != STD_ON
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_MainFunction(void);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif



/**
 * @ingroup NM_H
 *
 *  This function initializes the NM Interface. \n
 *
 * @Param ConfigPtr - Pointer to the selected configuration set. \n
 * The Configuration pointer ConfigPtr is currently not used and shall therefore be set NULL_PTR value. \n
 *
 * @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_Init(const Nm_ConfigType * ConfigPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for triggering passive start up of NM, this API calls  \n
 *  CanNm_PassiveStartUp or FrNm_PassiveStartUp or J1939Nm_PassiveStartUp based on the configuration.                     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Passive start of network management has failed.               \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to request Network. This API calls either CanNm_NetworkRequest\n
 *  or FrNm_NetworkRequest based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Request for bus communication has failed.                     \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to release Network, this API calls either CanNm_NetworkRelease\n
 *  or FrNm_NetworkRelease based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error                                                      \n
 *          E_NOT_OK         - Release for bus communication has failed.                     \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to get the current state of NM,                           \n
 *  this API calls CanNm_GetState or FrNm_GetState or J1939Nm_GetState based on the configuration.       \n
 *
 *  @param  In:     NetworkHandle-  Identification of the NM-channel.                       \n
 *  @param  out:    nmStatePtr   -  Pointer to the location where the state of the network  \n
 *                                  management shall be copied.                             \n
 *  @param  out:    nmModePtr    -  Pointer to the location where the mode of the network   \n
 *                                  management shall be copied.                             \n
 *
 *  @return E_OK              -  No error.                                               \n
 *          E_NOT_OK          -  Getting of NM state has failed.                         \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetState
(
    NetworkHandleType NetworkHandle,
    Nm_StateType * nmStatePtr,
    Nm_ModeType * nmModePtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for setting the user data for NM messages transmitted next on \n
 *  the bus, this API calls either CanNm_SetUserData or FrNm_SetUserData based on the channel   \n
 *  handle passed.                                                                              \n
 *
 *  @param  In: NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  In: nmUserDataPtr - Pointer where the user data for the next transmitted NM message \n
 *                              shall be copied from.                                           \n
 *
 *  @return E_OK           - No error.                                                       \n
 *          E_NOT_OK       - Setting of user data has failed.                                \n
 *
 */

#if ((NM_USER_DATA_ENABLED != STD_OFF) && (NM_COM_USER_DATA_SUPPORT == STD_OFF))
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_SetUserData
(
    NetworkHandleType NetworkHandle,
    const uint8 * nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the user data out of the most recently received   \n
 *  NM message, this API calls either CanNm_GetUserData or FrNm_GetUserData based on the        \n
 *  channel handle.                                                                             \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                           \n
 *  @param  out:    nmUserDataPtr - Pointer where user data out of the last successfully received   \n
 *                                  NM message shall be copied to.                              \n
 *
 *  @return E_OK               - No error.                                                   \n
 *          E_NOT_OK           - Getting of user data has failed                             \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetUserData
(
    NetworkHandleType NetworkHandle,
    uint8 * nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the whole NM PDU out of the most recently received        \n
 *  NM message, this API calls either CanNm_GetPduData or FrNm_GetPduData based on the channel handle.  \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmPduData     - Pointer where whole Pdu data out of the most recently received NM   \n
 *                                  message shall be copied to.                                         \n
 *
 *  @return E_OK               - No error.                                                           \n
 *          E_NOT_OK           - Getting of PDU has failed.                                          \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetPduData
(
    NetworkHandleType NetworkHandle,
    uint8 * nmPduData
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier out of the last NM message received on the bus.    \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier out of the last successfully          \n
 *                                  received NM message shall be copied to.                             \n
 *
 *  @return E_OK               - Getting of node identifier is successful.                           \n
 *          E_NOT_OK           - Getting of node identifier has failed.                              \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetNodeIdentifier
(
     NetworkHandleType NetworkHandle,
     uint8 * nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier of the host ECU.                               \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier of host ECU shall be copied to.   \n
 *
 *  @return E_OK               - Getting of node identifier of the host ECU is successful.       \n
 *          E_NOT_OK           - Getting of node identifier of the host ECU has failed.          \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetLocalNodeIdentifier
(
    NetworkHandleType NetworkHandle,
    uint8 * nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface gets the status of remote sleep detection.                               \n
 *
 *  @param  In:     NetworkHandle       - Identification of the NM-channel.                         \n
 *  @param  out:    nmRemoteSleepIndPtr - Pointer to the location where the check result of remote sleep    \n
 *                                        indication shall be copied.                               \n
 *
 *  @return E_OK               - No error.                                                       \n
 *          E_NOT_OK           -Checking of remote sleep indication bits has failed.           \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_CheckRemoteSleepIndication
(
    NetworkHandleType NetworkHandle,
    boolean * nmRemoteSleepIndPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface causes Repeat Message Request to be transmitted next on the bus.         \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Repeat Message Request has failed.                                \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_H
 *
 *  This is the interface to disable NM transmission.                                               \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Disabling of NM PDU transmission ability has failed.              \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the interface to enable NM transmission.                                                \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - enabling of transmission has failed.
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_EnableCommunication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This service returns the version information of this module.                                        \n
 *
 *  @param  out:    nmVerInfoPtr    - Pointer to where to store the version information of this module. \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_GetVersionInfo (Std_VersionInfoType * nmVerInfoPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#if(NM_COORDINATOR_SYNC_SUPPORT_ENABLED !=STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_CoordReadyToSleepIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_CoordReadyToSleepCancellation(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#else

/* Development Error Codes for DET Support*/
/**
 * @ingroup NM_H
 *
 * Macro to indicate unitialization of Nm.
 */
#define NM_E_UNINIT                0x00

/**
 * @ingroup NM_H
 *
 * Macro to indicate Nm Network handle is not configured.
 */
#define NM_E_HANDLE_UNDEF           0x01

/**
 * @ingroup NM_H
 *
 * Macro to indicate NULL pointer is passed as an argument to a function.
 */
#define    NM_E_PARAM_POINTER       0x02


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
/**
 * @ingroup NM_H
 *
 * This typedef is used to define datatype of Nm_TimerType.
 */

typedef uint32 Nm_TimerType;

/* type definition of the configuration structure containing pointers to configuration structures of
   CAN NM, FR NM , UDP NM and J1939 NM
 */
/**
 * @ingroup NM_H
 *
 * This is the configuration of Nm  \n
 * typedef struct                   \n
 * {                                \n
 *      Nm_TimerType        ShutdownDelay;      This variable consists the value of configured time, for which \n
 *                                              NmCoordinator can wait in Preparewaitsleep before entering into \n
 *                                              waitsleep. \n
 * #if (NM_STATE_REPORT_ENABLED != STD_OFF) \n
 *      Com_SignalIdType    NMSSignal;          This variable decides whether NMS report is enabled or not  \n
 * #endif \n
 *      Nm_BusType_ten          BusNmType;          This variable stores the type of Bus. \n
 *      NetworkHandleType   ClusterIdx;         This variable Stores the index of the cluster. \n
 *      NetworkHandleType   comMHandle;         This variable stores the handle of comM channel. \n
 * } Nm_ConfigType;
 */
typedef struct
{
    Nm_TimerType        ShutdownDelay;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    Com_SignalIdType    NMSSignal;
#endif
    Nm_BusType_ten      BusNmType;
    Nm_ChannelType_ten  NmChannelType;
    NetworkHandleType   ClusterIdx;
    NetworkHandleType   comMHandle;
    boolean             NmChannelSleepMaster;
    boolean             NmSynchronizingNetwork;
    boolean             NmNodeDetectionEnabled;
    boolean             NmNodeIdEnabled;
    boolean             NmRepeatMsgIndEnabled;
} Nm_ConfigType;


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

/**
 * @ingroup NM_H
 *
 *  This is the main function of NM Coordinator \n
 *
 *  @Param None \n
 *  @return None \n
 */
#if NM_ECUC_RB_RTE_IN_USE != STD_ON
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_MainFunction(void);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif



/**
 * @ingroup NM_H
 *
 *  This function initializes the NM Interface. \n
 *
 * @Param ConfigPtr - Pointer to the selected configuration set. \n
 * The Configuration pointer ConfigPtr is currently not used and shall therefore be set NULL_PTR value. \n
 *
 * @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_Init(const Nm_ConfigType * ConfigPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for triggering passive start up of NM, this API calls  \n
 *  CanNm_PassiveStartUp or FrNm_PassiveStartUp or J1939Nm_PassiveStartUp based on the configuration.                     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Passive start of network management has failed.               \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to request Network. This API calls either CanNm_NetworkRequest\n
 *  or FrNm_NetworkRequest based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Request for bus communication has failed.                     \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to release Network, this API calls either CanNm_NetworkRelease\n
 *  or FrNm_NetworkRelease based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error                                                      \n
 *          E_NOT_OK         - Release for bus communication has failed.                     \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to get the current state of NM,                           \n
 *  this API calls CanNm_GetState or FrNm_GetState or J1939Nm_GetState based on the configuration.       \n
 *
 *  @param  In:     NetworkHandle-  Identification of the NM-channel.                       \n
 *  @param  out:    nmStatePtr   -  Pointer to the location where the state of the network  \n
 *                                  management shall be copied.                             \n
 *  @param  out:    nmModePtr    -  Pointer to the location where the mode of the network   \n
 *                                  management shall be copied.                             \n
 *
 *  @return E_OK              -  No error.                                               \n
 *          E_NOT_OK          -  Getting of NM state has failed.                         \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetState
(
    NetworkHandleType NetworkHandle,
    Nm_StateType * nmStatePtr,
    Nm_ModeType * nmModePtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for setting the user data for NM messages transmitted next on \n
 *  the bus, this API calls either CanNm_SetUserData or FrNm_SetUserData based on the channel   \n
 *  handle passed.                                                                              \n
 *
 *  @param  In: NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  In: nmUserDataPtr - Pointer where the user data for the next transmitted NM message \n
 *                              shall be copied from.                                           \n
 *
 *  @return E_OK           - No error.                                                       \n
 *          E_NOT_OK       - Setting of user data has failed.                                \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_SetUserData
(
    NetworkHandleType NetworkHandle,
    const uint8 * nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the user data out of the most recently received   \n
 *  NM message, this API calls either CanNm_GetUserData or FrNm_GetUserData based on the        \n
 *  channel handle.                                                                             \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                           \n
 *  @param  out:    nmUserDataPtr - Pointer where user data out of the last successfully received   \n
 *                                  NM message shall be copied to.                              \n
 *
 *  @return E_OK               - No error.                                                   \n
 *          E_NOT_OK           - Getting of user data has failed                             \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetUserData
(
    NetworkHandleType NetworkHandle,
    uint8 * nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the whole NM PDU out of the most recently received        \n
 *  NM message, this API calls either CanNm_GetPduData or FrNm_GetPduData based on the channel handle.  \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmPduData     - Pointer where whole Pdu data out of the most recently received NM   \n
 *                                  message shall be copied to.                                         \n
 *
 *  @return E_OK               - No error.                                                           \n
 *          E_NOT_OK           - Getting of PDU has failed.                                          \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetPduData
(
    NetworkHandleType NetworkHandle,
    uint8 * nmPduData
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier out of the last NM message received on the bus.    \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier out of the last successfully          \n
 *                                  received NM message shall be copied to.                             \n
 *
 *  @return E_OK               - Getting of node identifier is successful.                           \n
 *          E_NOT_OK           - Getting of node identifier has failed.                              \n
 *
 */

#if (NM_NODE_ID_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetNodeIdentifier
(
     NetworkHandleType NetworkHandle,
     uint8 * nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier of the host ECU.                               \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier of host ECU shall be copied to.   \n
 *
 *  @return E_OK               - Getting of node identifier of the host ECU is successful.       \n
 *          E_NOT_OK           - Getting of node identifier of the host ECU has failed.          \n
 *
 */

#if(NM_NODE_ID_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_GetLocalNodeIdentifier
(
    NetworkHandleType NetworkHandle,
    uint8 * nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface gets the status of remote sleep detection.                               \n
 *
 *  @param  In:     NetworkHandle       - Identification of the NM-channel.                         \n
 *  @param  out:    nmRemoteSleepIndPtr - Pointer to the location where the check result of remote sleep    \n
 *                                        indication shall be copied.                               \n
 *
 *  @return E_OK               - No error.                                                       \n
 *          E_NOT_OK           -Checking of remote sleep indication bits has failed.           \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_CheckRemoteSleepIndication
(
    NetworkHandleType NetworkHandle,
    boolean * nmRemoteSleepIndPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface causes Repeat Message Request to be transmitted next on the bus.         \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Repeat Message Request has failed.                                \n
 *
 */

#if (NM_NODE_DETECTION_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/**
 * @ingroup NM_H
 *
 *  This is the interface to disable NM transmission.                                               \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Disabling of NM PDU transmission ability has failed.              \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the interface to enable NM transmission.                                                \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - enabling of transmission has failed.
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern Std_ReturnType Nm_EnableCommunication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This service returns the version information of this module.                                        \n
 *
 *  @param  out:    nmVerInfoPtr    - Pointer to where to store the version information of this module. \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_GetVersionInfo (Std_VersionInfoType * nmVerInfoPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#if(NM_COORDINATOR_SYNC_SUPPORT_ENABLED !=STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_CoordReadyToSleepIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_CoordReadyToSleepCancellation(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif /* NM_AR45_VERSION */

#endif /* NM_H */

