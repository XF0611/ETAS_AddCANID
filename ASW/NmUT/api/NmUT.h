/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : NmUT.h
 * Date      : Jan 28 2020
 * Version   : 1.0
 * Description  : This module provides global interfaces from Example NmUT module.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables.
 ***********************************************************************************************
*/
#ifndef NmUT_H
#define NmUT_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "NmStack_Types.h"

/*
 **********************************************************************************************************************
 * Macros
 **********************************************************************************************************************
 */

#define APP_NM_INIT              0U
#define APP_READY_FOR_NM_REQUEST 1U
#define APP_READY_FOR_NM_RELEASE 2U
#define APP_NM_BUS_SLEEP         3U

/*
 **********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************
 */
typedef struct {
    char flag;
    Nm_StateType nm_PreviousState;
    Nm_StateType nm_CurrentState;
    boolean IsNmReceived;
    boolean release_b;
    boolean request_b;
    uint8 nm_rx_data[8];
    uint8 nm_tx_data[8];
    uint8 nmState;
    uint8 nmMode;
    uint8 NotReceiveNMCounter;
}Nm_Test_t;


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */


#endif /* NmUT_H */

/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
