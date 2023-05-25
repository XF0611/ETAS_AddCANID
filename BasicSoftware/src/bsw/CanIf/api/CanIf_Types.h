


#ifndef CANIF_TYPES_H
#define CANIF_TYPES_H

/**************************************************************************************************/
/*              Include Section                                                                   */
/**************************************************************************************************/

/* Include the ComStack definitions */
#include "ComStack_Types.h"                 /*Version check for ComStack_types and CanIf done in CanIf_Priv.h*/

/* Include the Can_GeneralTypes definitions */

#include "Can_GeneralTypes.h"


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/**
 * The PduMode of a channel defines its transmit or receive activity.
 * Communication direction (transmission and/or reception) of the channel can be
 * controlled separately or together by upper layers.
 */
typedef enum
{
    /**
     *
     * = 0
     * Transmit and receive path of the corresponding channel are disabled
     * => no communication mode
     *
     */
    CANIF_OFFLINE = 0,
    /**
     * Transmit path of the corresponding channel is disabled. The receive path is
     * enabled.
     */
    CANIF_TX_OFFLINE = 1,
    /**
     * Transmit path of the corresponding channel is in offline active mode (see
     * SWS_CANIF_00072). The receive path is disabled.
     * This mode requires CanIfTxOfflineActiveSupport = TRUE.
     */
    CANIF_TX_OFFLINE_ACTIVE = 2,
    /**
     *
     * Transmit and receive path of the corresponding channel are enabled
     * => full operation mode
     *
     */
    CANIF_ONLINE = 3,

    /* FC_VariationPoint_START */

    /*PduModes for CanTpTxOfflineMode feature*/
    CANIF_TX_TP_ONLINE = 4,
    CANIF_TX_USER_TP_ONLINE = 5

    /* FC_VariationPoint_END */

}CanIf_PduModeType;



/*Operating modes of a CAN controller*/
typedef enum
{
    /**
     * UNINIT mode. Default mode of each CAN controller after power on.
     */
    CANIF_CS_UNINIT = 0,
    /**
     * The CAN controller is in SLEEP mode and can be woken up by an internal (SW)
     * request or by a network event (This must be supported by CAN hardware.).
     */
    CANIF_CS_SLEEP,
    /**
     * The CAN controller is in full-operational mode.
     */
    CANIF_CS_STARTED,
    /**
     * The CAN controller is halted and does not operate on the network.
     */
    CANIF_CS_STOPPED
}CanIf_ControllerModeType;


/**
 * Return value of CAN L-PDU notification status.
 */
typedef enum
{
    /**
     * = 0
     * No transmit or receive event occurred for the requested L-PDU.
     */
    CANIF_NO_NOTIFICATION = 0,
    /**
     * The requested Rx/Tx CAN L-PDU was successfully transmitted or received.
     */
    CANIF_TX_RX_NOTIFICATION
}CanIf_NotifStatusType;




#endif   /* CANIF_TYPES_H */
