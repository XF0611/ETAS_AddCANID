/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/**
\defgroup CAN_GENERALTYPES_H    CANIF - AUTOSAR interfaces Upper Layer
*/
#ifndef CAN_GENERAL_TYPES_H
#define CAN_GENERAL_TYPES_H

#include "ComStack_Types.h"


/*[SWS_Can_00416-AR431] Can_IdType type info*/
/**********************************************************************************
 ** [SWS_Can_00416-AR431]Can_IdType                                              **
 ** Type: uint32                                                                 **
 ** Range: Standard32Bit: 0..0x400007FF                                          **
 **        Extended32Bit: 0..0xDFFFFFFF                                          **
 ** Description: Represents the Identifier of an L-PDU. The two most significant **
 ** bits specify the frame type:                                                 **
 ** 00 CAN message with Standard CAN ID                                          **
 ** 01 CAN FD frame with Standard CAN ID                                         **
 **********************************************************************************/
typedef uint32 Can_IdType;

/*[SWS_Can_00429] Name  : Can_HwHandleType*/
/**********************************************************************************
**[SWS_Can_00429] 8.2.4 :Can_HwHandleType                                        **
**Type                  : uint8, uint16                                          **
**Range                 : Standard  --0..0x0FF                                   **
**Extended              :--0..0xFFFF Description:                                **
***********************************************************************************/
typedef uint16 Can_HwHandleType;

/**********************************************************************************
 ** [SWS_CAN_00496]Can_HwType                                                    **
 ** Type: Structure                                                              **
 ** Element: Can_IdType       -- CanId                                           **
 **          Can_HwHandleType -- Hoh                                             **
 **          uint8            -- ControllerId                                    **
 ** Description: This type defines a data structure which clearly provides an    **
 ** Hardware Object Handle including its corresponding CAN Controller and        **
 ** therefore CanDrv as well as the specific CanId.                              **
 **********************************************************************************/
typedef struct
{
    Can_IdType CanId;
    Can_HwHandleType Hoh;
    uint8      ControllerId;
}Can_HwType;


/**
 * @ingroup CAN_GENERALTYPES_H
 *
 * This is a Can Pdu Type structure \n
 * typedef struct                   \n
 * {                                \n
 *   uint8 * sdu;   This corresponds to the data unit pointer \n
 *   Can_IdType id;                                 This variable indicates the Can Id.  \n
 *   PduIdType  swPduHandle;                        This variable determines the Pdu Handle \n
 *   uint8      length;                             This corresponds to the length of the Pdu \n
 * }Can_PduType;                    \n
 */
typedef struct
{
    uint8 * sdu;
    Can_IdType id;
    PduIdType  swPduHandle;
    uint8      length;

}Can_PduType;
/**
 * @ingroup CAN_GENERALTYPES_H
 *
 * This enum represents the CAN state Transition type       \n
 *typedef enum                                              \n
 * {                                                            \n
 *  CAN_T_START,           CAN controller transition value to request state STARTED \n
 *  CAN_T_STOP,            CAN controller transition value to request state STOPPED \n
 *  CAN_T_SLEEP,           CAN controller transition value to request state SLEEP   \n
 *  CAN_T_WAKEUP,          CAN controller transition value to request state STOPPED from state SLEEP \n
 *  CAN_T_MAXTRANSITION    Max number of transitions that can happen  \n
 * }Can_StateTransitionType;
 */
typedef enum
{
    CAN_T_START,
    CAN_T_STOP,
    CAN_T_SLEEP,
    CAN_T_WAKEUP,
    CAN_T_MAXTRANSITION

}Can_StateTransitionType;

/**
 * @ingroup CAN_GENERALTYPES_H
 *
 * This enum represents the CAN return type         \n
 * typedef enum                                             \n
 *  {                                                       \n
 *   CAN_OK,                Represents CAN bus has been allocated successfully \n
 *   CAN_NOT_OK,            Error occurred or wakeup event occurred during sleep transition \n
 *   CAN_BUSY               Transmit request could not be processed because no transmit object was available \n
 *  }Can_ReturnType;
 */
typedef enum
{
    CAN_OK,
    CAN_NOT_OK,
    CAN_BUSY

}Can_ReturnType;

/**
 * @ingroup CAN_GENERALTYPES_H
 *
 * This enum indicates the Can tranceiver state             \n
 * typedef enum                                             \n
 * {                                                        \n
 *  CANTRCV_TRCVMODE_NORMAL=0,    Transceiver mode NORMAL   \n
 *  CANTRCV_TRCVMODE_SLEEP,       Transceiver mode SLEEP    \n
 *  CANTRCV_TRCVMODE_STANDBY      Transceiver mode STANDBY  \n
 *}CanTrcv_TrcvModeType;
 */
/* FC_VariationPoint_START */
#if defined(CORE_M4_DME)
/*Do not define the CANIF_SHARED_SYMBOL -> this will force out of compilation some symbols, requiring DME linker to import
them from R52 thus sharing them between DME and R2 */
#else
#define CANIF_SHARED_SYMBOL (1u)
#endif
/* FC_VariationPoint_END */
typedef enum
{
    CANTRCV_TRCVMODE_NORMAL=0,
    CANTRCV_TRCVMODE_SLEEP,
    CANTRCV_TRCVMODE_STANDBY

}CanTrcv_TrcvModeType;

/* This enum indicates the Can tranceiver state */
typedef enum
{
    CANTRCV_WUMODE_ENABLE=0,
    CANTRCV_WUMODE_DISABLE,
    CANTRCV_WUMODE_CLEAR

}CanTrcv_TrcvWakeupModeType;


/**
 * @ingroup CAN_GENERALTYPES_H
 *
 * Wake up reason detected by the CAN transceiver.
 * typedef enum
 * {
 *  CANTRCV_WU_ERROR=0,         Wake up reason not detected due to some error. This may only be reported when error was reported to DEM before \n
 *  CANTRCV_WU_NOT_SUPPORTED,   The transceiver does not support any information for the wake up reason.\n
 *  CANTRCV_WU_BY_BUS,          The transceiver has detected, that the network has caused the wake up of the ECU.\n
 *  CANTRCV_WU_INTERNALLY,      The transceiver has detected, that the network has woken up by the ECU via a request to NORMAL mode. \n
 *  CANTRCV_WU_RESET,           The transceiver has detected, that the "wake up" is due to an ECU reset. \n
 *  CANTRCV_WU_POWER_ON,        The transceiver has detected, that the "wake up" is due to an ECU reset after power on.\n
 *  CANTRCV_WU_BY_PIN           The transceiver has detected a wake-up event at one of the transceiver's pins (not at the CAN bus). \n
 * }CanTrcv_TrcvWakeupReasonType;
 */
typedef enum
{

    CANTRCV_WU_ERROR=0,
    CANTRCV_WU_NOT_SUPPORTED,
    CANTRCV_WU_BY_BUS,
    CANTRCV_WU_INTERNALLY,
    CANTRCV_WU_RESET,
    CANTRCV_WU_POWER_ON,
    CANTRCV_WU_BY_PIN

}CanTrcv_TrcvWakeupReasonType;

/***************************Standard Types****************************************/


/* CAN_FSR_91013 Req-4.3.1*/
typedef enum {
    CAN_CS_UNINIT = 0,
    CAN_CS_STARTED,
    CAN_CS_STOPPED,
    CAN_CS_SLEEP
}Can_ControllerStateType;


/* CAN_FSR_91003 Req-4.3.1*/
typedef enum {
    CAN_ERRORSTATE_ACTIVE,  /* The CAN controller takes fully part in communication. */
    CAN_ERRORSTATE_PASSIVE, /* The CAN controller takes part in communication, but does not send active error frames. */
    CAN_ERRORSTATE_BUSOFF   /* The CAN controller does not take part in communication. */
}Can_ErrorStateType;




/*CAN_FSR_00416 Req-4.3.1- Can_IdType 
* uint16: if only Standard IDs are used 
* uint32: if also Extended IDs are used 
* Most significant 2 bits specify ->
* CAN_ID_TYPE_STANDARD (00), CAN_FD_ID_STANDARD,(01)
* CAN_ID_TYPE_EXTENDED (10), CAN_FD_ID_EXTENDED(11),
*/
typedef uint32 Can_IdType;     




#endif /* CAN_GENERAL_TYPES_H */


