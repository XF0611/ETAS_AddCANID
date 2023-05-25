
#ifndef NMSTACK_TYPES_H
#define NMSTACK_TYPES_H

/**
\defgroup NM_STACK_TYPES_H    Nm - Autosar Generic Network Management Interface.
*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      Header file for common types for all NMs
 *
 * \scope               This is included by Nm, FrNm, CanNm and J1939Nm
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */


/* Nm_ModeType */
/**
 * @ingroup NM_STACK_TYPES_H
 *  This enum is used to define the Operational modes of the network management.    \n
 *  typedef enum                                                                    \n
 *  {
 *      NM_MODE_BUS_SLEEP,              bus sleep mode                              \n
 *      NM_MODE_PREPARE_BUS_SLEEP,      prepare bus sleep mode                      \n
 *      NM_MODE_SYNCHRONIZE,            synchronize mode                            \n
 *      NM_MODE_NETWORK                 network mode                                \n
 *  } Nm_ModeType;
 */
typedef enum
{
    NM_MODE_BUS_SLEEP,
    NM_MODE_PREPARE_BUS_SLEEP,
    NM_MODE_SYNCHRONIZE,
    NM_MODE_NETWORK
} Nm_ModeType;

/* Nm_StateType */
/**
 * @ingroup NM_STACK_TYPES_H
 *  This enum is used to define States of the network management state machine. \n
 *  typedef enum
 *  {
 *      NM_STATE_UNINIT,            Uninitialized State     \n
 *      NM_STATE_BUS_SLEEP,         Bus-Sleep State         \n
 *      NM_STATE_PREPARE_BUS_SLEEP, Prepare-Bus State       \n
 *      NM_STATE_READY_SLEEP,       Ready Sleep State       \n
 *      NM_STATE_NORMAL_OPERATION,  Normal Operation State  \n
 *      NM_STATE_REPEAT_MESSAGE,    Repeat Message State    \n
 *      NM_STATE_SYNCHRONIZE,       Synchronize State       \n
 *      NM_STATE_OFFLINE            Offline State           \n
 *  } Nm_StateType;
 */
typedef enum
{
    NM_STATE_UNINIT,
    NM_STATE_BUS_SLEEP,
    NM_STATE_PREPARE_BUS_SLEEP,
    NM_STATE_READY_SLEEP,
    NM_STATE_NORMAL_OPERATION,
    NM_STATE_REPEAT_MESSAGE,
    NM_STATE_SYNCHRONIZE,
    NM_STATE_OFFLINE
} Nm_StateType;

/* Nm_BusNmType */
/**
 * @ingroup NM_STACK_TYPES_H
 *  This enum is used to define BusNm Type  \n
 *  typedef enum                                        \n
 *  {
 *      NM_BUSNM_CANNM = 0,         CAN NM type         \n
 *      NM_BUSNM_FRNM  = 1,         FR NM type          \n
 *      NM_BUSNM_LINNM = 2,         LIN NM type         \n
 *      NM_BUSNM_UDPNM = 3,         UDP NM type         \n
 *      NM_BUSNM_GENERICNM = 4,     Generic NM type     \n
 *      NM_BUSNM_J1939NM = 5,        SAE J1939 NM type   \n
 *      NM_BUSNM_UNDEF = 0xFFu     NM type undefined   \n
 *  } Nm_BusNmType;
 */
typedef enum
{
    NM_BUSNM_CANNM = 0,
    NM_BUSNM_FRNM  = 1,
    NM_BUSNM_LINNM = 2,
    NM_BUSNM_UDPNM = 3,
    NM_BUSNM_GENERICNM = 4,
    NM_BUSNM_J1939NM = 5,
    NM_BUSNM_UNDEF = 0xFFu
} Nm_BusNmType;



/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *  NM_ACTIVE_CHANNEL,          Acts as Active Coordinator \n
 *  NM_PASSIVE_CHANNEL          Acts as Passive coordinator\n
 *  } Nm_ChannelType_ten;
 */
typedef enum
{
   NM_ACTIVE_CHANNEL,
   NM_PASSIVE_CHANNEL
} Nm_ChannelType_ten;

#endif /* NMSTACK_TYPES_H */

