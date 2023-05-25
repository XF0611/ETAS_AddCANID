
#ifndef NM_CBK_H
#define NM_CBK_H

/**
\defgroup NM_CBK_H    Nm - Autosar Generic Network Management Interface.
*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface header file for declaration of callback notifications
 *
 * \scope               This is included by the <Bus>Nm modules
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Types header for common types */
#include "Nm.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Network Mode, this is called by BusNm  \n
 *  when it enters Network mode. This in turn notifies ComM.                            \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                        \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_NetworkMode(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Bus Sleep Mode. This is called by BusNm when it    \n
 *  enters Bus Sleep mode. This in turn notifies ComM about Bus Sleep mode.                         \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_BusSleepMode(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Prepare Bus Sleep Mode, this is not called by  \n
 *  FrNm or J1939Nm, this is called by CanNm when it enters Prepare Bus Sleep mode. This in turn notifies  \n
 *  ComM about Prepare Bus Sleep mode.                                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_PrepareBusSleepMode(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying synchronize Mode, this is  called by  \n
 *  FrNm only \n
 *
 *  @param  In: nmNetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
* @ingroup NM_CBK_H
*
*  This is the NM call-back interface for notification of Network Start,   \n
*  this function in turn notifies ComM.                                    \n
*
*  @param  In: NetworkHandle- Identification of the NM-channel.            \n
*
*  @return None \n
*/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_NetworkStartIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to indicate detection of Remote Sleep status,\n
 *  this is called by either CanNm or FrNm, it in turn notifies user application.   \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_RemoteSleepIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to cancel detection of Remote Sleep status   \n
 *  this is called by either CanNm or FrNm, it in turn notifies user application.   \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_RemoteSleepCancellation(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/* Start of declarations for OEM Specific Extensions */
/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Transmission timeout. This is called by FrNm   \n
 *  when it detects timeout in getting Tx Confirmation.                                         \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_TxTimeoutException(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_NetworkTimeoutException(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Car Wake Up Indication.
 *  This is called by either CanNm or FrNm when CarWakeUp request is received in the NM-PDU    \n


 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#if (NM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_CarWakeUpIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to notify that the NM has received a message.            \n
 *  This is called by either CanNm or FrNm. This interface is available depending on the        \n
 *  configuration parameter NM_PDU_RX_INDICATION_ENABLED. It in turn notifies the application.  \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_PduRxIndication(NetworkHandleType NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to notify state changes. This is called by CanNm  \n
 *  or FrNm or J1939Nm. It in turn notifies the application                                                \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                             \n
 *  @param  In: nmPreviousState - previous state of NM.                                         \n
 *  @param  In: nmCurrentState  - current state of NM.                                          \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_StateChangeNotification
(
    NetworkHandleType nmNetworkHandle,
    Nm_StateType nmPreviousState,
    Nm_StateType nmCurrentState
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is an NM call-back interface to notify that an NM msg is received with Repeat Message      \n
 *  Bit set. This is called by either CanNm or FrNm. This interface is available depending on the   \n
 *  configuration parameter NM_REPEAT_MESSAGE_IND_ENABLED. It in turn notifies the application.     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_CBK_H
 *
 * This is an NM call-back interface to notify the NM Coordinator functionality that this is a suitable     \n
 * point in time to initiate the coordinated shutdown on.This is called by Cyclic channel eg.FrNm.          \n
 * This interface is available depending on the configuration parameter NM_COORDINATOR_SUPPORT_ENABLED.     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
/* End of declarations for OEM Specific Extensions */

#endif /* NM_CBK_H */

