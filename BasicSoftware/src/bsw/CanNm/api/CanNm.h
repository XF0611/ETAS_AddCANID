

#ifndef CANNM_H
#define CANNM_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComStack_Types.h"
#include "CanNm_Cfg_Internal.h"
#include "CanNm_Cfg.h"
#include "CanNm_PBcfg.h"

#if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


#include "Nm.h"
#if (!defined(NM_AR_RELEASE_MAJOR_VERSION) || (NM_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(NM_AR_RELEASE_MINOR_VERSION) || (NM_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Development Error Codes for DET Support*/

#define CANNM_E_NO_INIT               0x01 /* API service used without module initialization */
#define CANNM_E_INVALID_CHANNEL       0x02 /* API service called with wrong channel handle */
#define CANNM_E_INVALID_PDUID         0x03 /* API service called with wrong PDU-ID */
#define CANNM_E_NET_START_IND         0x04 /* Reception of NM PDUs in Bus-Sleep Mode */
#define CANNM_E_INIT_FAILED           0x05 /* CanNm Initialisation has failed */
#define CANNM_E_NETWORK_TIMEOUT       0x11 /* NM-Timeout Timer has abnormally expired outside of the Ready Sleep State*/
#define CANNM_E_PARAM_POINTER         0x12 /* Null pointer has been passed as an argument */


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
extern const CanNm_ConfigType CanNmGlobalConfig;
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_Init(
                                          const CanNm_ConfigType * cannmConfigPtr
                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_PassiveStartUp(
                                                              NetworkHandleType nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_NetworkRequest(
                                                              NetworkHandleType nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_NetworkRelease(
                                                              NetworkHandleType nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_DisableCommunication(
                                                                    NetworkHandleType nmChannelHandle
                                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_EnableCommunication(
                                                                   NetworkHandleType nmChannelHandle
                                                                  );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if((CANNM_USER_DATA_ENABLED != STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF) && \
      (CANNM_COM_USER_DATA_SUPPORT == STD_OFF))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_SetUserData(
                                                           NetworkHandleType nmChannelHandle,
                                                           const uint8 * nmUserDataPtr
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_GetUserData(
                                                           NetworkHandleType nmChannelHandle,
                                                           uint8 * nmUserDataPtr
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if((CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || (CANNM_PN_ENABLED != STD_OFF))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_Transmit(
                                                        PduIdType CanNmTxPduId,
                                                        const PduInfoType * PduInfoPtr
                                                       );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if (CANNM_NODE_ID_ENABLED != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_GetNodeIdentifier(
                                                              NetworkHandleType nmChannelHandle,
                                                                 uint8 * nmNodeIdPtr
                                                                );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"



#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_GetLocalNodeIdentifier(
                                                                      NetworkHandleType nmChannelHandle,
                                                                    uint8 * nmNodeIdPtr
                                                                     );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_RepeatMessageRequest(
                                                                    NetworkHandleType nmChannelHandle
                                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_GetPduData(
                                                          NetworkHandleType nmChannelHandle,
                                                          uint8 * nmPduDataPtr
                                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_GetState(
                                                        NetworkHandleType nmChannelHandle,
                                                        Nm_StateType * nmStatePtr,
                                                        Nm_ModeType * nmModePtr
                                                       );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if (CANNM_VERSION_INFO_API != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_GetVersionInfo(
                                                    Std_VersionInfoType * versioninfo
                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if ((CANNM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF) && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_RequestBusSynchronization(
                                                                         NetworkHandleType nmChannelHandle
                                                                        );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_CheckRemoteSleepIndication(
                                                                          NetworkHandleType nmChannelHandle,
                                                                          boolean * nmRemoteSleepIndPtr
                                                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern Std_ReturnType CanNm_SetSleepReadyBit(
                                                                NetworkHandleType nmChannelHandle,
                                                                boolean nmSleepReadyBit
                                                               );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

/* If RTE is in use then the declaration will be provided by SchM file */
#if (CANNM_ECUC_RB_RTE_IN_USE == STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_MainFunction( void );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if (CANNM_PN_ENABLED != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_ConfirmPnAvailability(
                                                            NetworkHandleType nmChannelHandle
                                                           );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if ((CANNM_PN_ENABLED != STD_OFF) && defined(BSWSIM))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_SetPnState(
                                               const NetworkHandleType nmChannelHandle,
                                               boolean state
                                              );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_SetAllMessageKeepAwake(
                                                           const NetworkHandleType nmChannelHandle,
                                                           boolean state
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern void CanNm_SetPnHandleMultipleNetworkReqState(
                                                                       const NetworkHandleType nmChannelHandle,
                                                                       boolean state
                                                                      );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif
#endif /* CANNM_H */


