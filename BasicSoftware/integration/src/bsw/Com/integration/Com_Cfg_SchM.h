/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef COM_CFG_SCHM_H
#define COM_CFG_SCHM_H

#include "SchM.h"

#if ( COM_ECUC_RB_RTE_IN_USE == STD_ON )
/* If the RTE code generation is Involved please include the below file */
#include "SchM_Com.h"
#endif /* #if ( COM_ECUC_RB_RTE_IN_USE == STD_ON ) */

#define SchM_Enter_Com_TxIpduProtArea(FUNCTIONNAME) SchM_Enter_Com_TxIpduProtArea_##FUNCTIONNAME()
#define SchM_Exit_Com_TxIpduProtArea(FUNCTIONNAME) SchM_Exit_Com_TxIpduProtArea_##FUNCTIONNAME()

#define SchM_Enter_Com_RxSigBuff(FUNCTIONNAME) SchM_Enter_Com_RxSigBuff_##FUNCTIONNAME()
#define SchM_Exit_Com_RxSigBuff(FUNCTIONNAME) SchM_Exit_Com_RxSigBuff_##FUNCTIONNAME()

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_INVALIDATESIGNAL(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_INVALIDATESIGNAL(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_MAINFUNCTIONTX(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_MAINFUNCTIONTX(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_MAINFUNCTION_ROUTE_SIGNALS(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_MAINFUNCTION_ROUTE_SIGNALS(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDDYNSIGNAL(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDDYNSIGNAL(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDIPDU_DATA(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDIPDU_DATA(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDIPDU_TXFLAGS(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDIPDU_TXFLAGS(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDSIGNAL(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDSIGNAL(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDSIGNALGRP(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDSIGNALGRP(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRIGGERTRANSMIT(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRIGGERTRANSMIT(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRIGGERIPDUSENDWITHMETADATA(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRIGGERIPDUSENDWITHMETADATA(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SIGTXCHANGEMODE(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SIGTXCHANGEMODE(void);

#if (defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP))
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SIGGRPTXCHANGEMODE(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SIGGRPTXCHANGEMODE(void);
#endif

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TXCONFIRMATION(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TXCONFIRMATION(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SEND_DYNGROUPSIGNAL(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SEND_DYNGROUPSIGNAL(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_IPDUGROUPSTART(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_IPDUGROUPSTART(void);

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SWITCHTXIPDU(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SWITCHTXIPDU(void);

#ifdef COM_METADATA_SUPPORT
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRANSMIT_METADATA(void);
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRANSMIT_METADATA(void);
#endif

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE void SchM_Enter_Com_TxGrpSigBuffer(void);
LOCAL_INLINE void SchM_Exit_Com_TxGrpSigBuffer(void);
#endif /* #ifdef COM_TX_SIGNALGROUP */

LOCAL_INLINE void SchM_Enter_Com_RxPduBuffer(void);
LOCAL_INLINE void SchM_Exit_Com_RxPduBuffer(void);

#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigSecBuff (void);
LOCAL_INLINE void SchM_Exit_Com_RxGrpSigSecBuff (void);
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if defined( COM_RX_SIGNALGROUP_ARRAY )
LOCAL_INLINE void SchM_Enter_Com_RxSigGrpArrayBuff( void );
LOCAL_INLINE void SchM_Exit_Com_RxSigGrpArrayBuff( void );
#endif /* #if defined( COM_RX_SIGNALGROUP_ARRAY ) */

#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigBuff(void);
LOCAL_INLINE void SchM_Exit_Com_RxGrpSigBuff (void);
#endif /* #ifdef COM_RX_SIGNALGROUP */

LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_MAINFUNCTIONRX (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_MAINFUNCTIONRX (void);

LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_RECEIVESIGNAL (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_RECEIVESIGNAL  (void);

LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_RXINDICATION (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_RXINDICATION (void);

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
LOCAL_INLINE void SchM_Enter_Com_RxSigDynBuff (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigDynBuff (void);
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigDynBuff (void);
LOCAL_INLINE void SchM_Exit_Com_RxGrpSigDynBuff (void);
#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#ifdef COM_RxSigUpdateTimeout
LOCAL_INLINE void SchM_Enter_Com_RxSigToTicks (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigToTicks (void);
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
LOCAL_INLINE void SchM_Enter_Com_RxSigGrpToTicks(void);
LOCAL_INLINE void SchM_Exit_Com_RxSigGrpToTicks(void);
#endif /*#ifdef COM_RxSigGrpUpdateTimeout*/

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

LOCAL_INLINE void SchM_Enter_Com_RxSigGwMcLockProtArea (void);
LOCAL_INLINE void SchM_Exit_Com_RxSigGwMcLockProtArea (void);

#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_PDUBASEDLOCKSENABLED
#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)
LOCAL_INLINE void SchM_Enter_Com_RxGateway(void);
LOCAL_INLINE void SchM_Exit_Com_RxGateway(void);
#endif

LOCAL_INLINE void SchM_Enter_Com_RxIPduProcess(void);
LOCAL_INLINE void SchM_Exit_Com_RxIPduProcess(void);

LOCAL_INLINE void SchM_Enter_Com_RxIndication(void);
LOCAL_INLINE void SchM_Exit_Com_RxIndication(void);

LOCAL_INLINE void SchM_Enter_Com_TxIPdu(void);
LOCAL_INLINE void SchM_Exit_Com_TxIPdu(void);
#endif /* #ifdef COM_PDUBASEDLOCKSENABLED */

/* NOTE to Integrator : These below Exclusive areas can be defined depending on the project scenario,
 */

/* NOTE on Exclusive area INVALIDATESIGNAL:
Resource:
The signal value is being updated in the Tx Ipdu buffer
Scenario:
The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
If any other signal of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger-
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the stack
 * 3) Com_SendSignal
 *   a. If a signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */

LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_INVALIDATESIGNAL (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_INVALIDATESIGNAL (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area MAINFUNCTIONTX:
Resource:
The set of Tx Timers and Tx flags are updated
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 2) Com_Prv_SigTxChangeMode/Com_Prv_SigGrpTxChangeMode
 *   a. a set of tx timers and Tx Flags are updated
 * 3) Com_TxConfirmation
 *   a. a set of tx timers and Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_MAINFUNCTIONTX(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_MAINFUNCTIONTX(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area MAINFUNCTION_ROUTE_SIGNALS:
Resource: The set of Tx flags are updated
Scenario: The below are the possibilites of shared resource access(to avoid read-modify-write)
 * Write Operation:
 * 1) Com_Prv_SendIpdu
 * 2) Com_MainFunctionTx
 * 3) Com_TxConfirmation
 * 4) Com_TriggerIPDUSendWithMetaData
 * 5) Com_SendSignal/Com_SendDynSignal/Com_SendSignalGroup/Com_SendSignalGroupArray
 * 6) Com_MainFunctionRouteSignals
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_MAINFUNCTION_ROUTE_SIGNALS(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_MAINFUNCTION_ROUTE_SIGNALS(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area SENDDYNSIGNAL:
Resource:
The Data is updated into the Tx Ipdu buffer for a Dynamic signal
Scenario:
The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendSignal
 *   a. If a signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * 5)Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDDYNSIGNAL(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDDYNSIGNAL(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area SEND_DYNGROUPSIGNAL:
Resource:
The Data is updated into the Tx Ipdu buffer for a Dynamic group signal
Scenario:
The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_SendIPdu
 *   a. If the transmission is initiated for the Ipdu from Com_MainfunctionTx/Event trigger then Callout can update Tx Ipdu
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendSignal
 *   a. If a signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignalGroup
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * 5)Com_MainfunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */
LOCAL_INLINE FUNC(void, COM_CODE) SchM_Enter_Com_TxIpduProtArea_SEND_DYNGROUPSIGNAL(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE FUNC(void, COM_CODE) SchM_Exit_Com_TxIpduProtArea_SEND_DYNGROUPSIGNAL(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

/* NOTE on Exclusive area SENDIPDU_DATA:
Resource:
TxIPdu buffer
The set of Tx Timers and Tx flags are updated

(a. The Data is updated into the Tx Ipdu buffer via Ipdu callout
 b. IpduCounter value is updated into the Tx Ipdu buffer
 c. Default metadata is updated into the Tx Ipdu buffer
 d. Data is read by the other ComStack modules)
Scenario:
The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_SendSignal
 *   a. If a signal update is called for a signal from the same Ipdu
 * 3) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * 5)Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * Read Operation :
 * 6) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 * 7) Com_Prv_SigTxChangeMode/Com_Prv_SigGrpTxChangeMode
 *   a. The data of signal group buffers is read here
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDIPDU_DATA(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDIPDU_DATA(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area SENDIPDU_TXFLAGS:
Resource:
The set of Tx Timers and Tx flags are updated
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_InternalMainFunctionTx
 *   a. The Tx Ipdu data and Tx Flags are updated
 * 2) Com_Prv_SigTxChangeMode/Com_Prv_SigGrpTxChangeMode
 *   a. a set of tx timers and Tx Flags are updated when called from
 *   Com_SendSignal/Com_SendSignalGroup/Com_SendSignalGroupArray/Com_MainFunctionRouteSignals
 * 3) Com_TxConfirmation
 *   a. a set of tx timers and Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDIPDU_TXFLAGS(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDIPDU_TXFLAGS(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area SENDSIGNAL:
Resource:
The Data is updated into the Tx Ipdu buffer for a signal
Scenario:
The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * 5)Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDSIGNAL(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDSIGNAL(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
/* NOTE on Exclusive area SENDSIGNALGRP:
Resource:
The Data is updated into the Tx Ipdu buffer for a signal group from a shadow buffer
Scenario:
The below are the possibilites of shared resource access (Inconsistent Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignal
 *   a. If a signal update is called for a signal  from the same Ipdu
 * 5) Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * 6) Com_SendSignalGroupArray:
 *   a. If a SignalGroup update is called for a SignalGroup from the same IPdu
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SENDSIGNALGRP(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SENDSIGNALGRP(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
/* NOTE on Exclusive area TRIGGERTRANSMIT:
Resource:
The Data is being read from the Tx Ipdu buffer
Dynamic length is being read.
Scenario:
The below are the possibilites of shared resource access (Inconsistent read of Tx Ipdu data)
If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 4) Com_SendSignal
 *   a. If a signal update is called for a signal  from the same Ipdu
 * 5)Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * 6) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group data update is called for a signal group  from the same Ipdu
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRIGGERTRANSMIT(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRIGGERTRANSMIT(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area TRIGGERIPDUSENDWITHMETADATA:
 * Resource:
 * The Data is updated into the Tx Ipdu buffer for a signal
 * Scenario:
 * The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
 * If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 3) Com_SendSignal
 *   a. If a signal update is called for a signal  from the same Ipdu
 * 4) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 5) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 * 6) Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * 7) Com_TriggerIPDUSendWithMetaData:
 *      Tx IPdu Buffer can be updated in this API
 *
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRIGGERIPDUSENDWITHMETADATA(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRIGGERIPDUSENDWITHMETADATA(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

/* NOTE on Exclusive area SIGTXCHANGEMODE:
Resource:
The set of Tx Timers and Tx flags are updates
Scenario:
The below are the possibilites of shared resource access(Inconsistent read of Tx Ipdu data)
If any other signal of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 2) Com_InternalMainFunctionTx
 *   a. a set of tx timers and Tx Flags are updated
 * 3) Com_TxConfirmation
 *   a. a set of tx timers and Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SIGTXCHANGEMODE(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SIGTXCHANGEMODE(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

#if (defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP))
/* NOTE on Exclusive area SIGGRPTXCHANGEMODE:
Resource:
The set of Tx group signal flags are updated and read operation of Tx group signal shadow buffers
Scenario:
The below are the possibilites of shared resource access(Inconsistent read of Tx Ipdu data)
If any other signalgroup of this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1)Com_MainFunctionRouteSignals:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu, this could result in inconsistent read here
 * 2)Com_SendSignalGroup/Com_SendSignalGroupArray:
 *   a. If a signal group data update is triggered for a signal group from the same Ipdu
 * 3) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 4) Com_InternalMainFunctionTx
 *   a. a set of tx timers and Tx Flags are updated
 * 5) Com_TxConfirmation
 *   a. a set of tx timers and Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SIGGRPTXCHANGEMODE(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SIGGRPTXCHANGEMODE(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #if (defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP)) */

/* NOTE on Exclusive area TXCONFIRMATION:
Resource:
The set of Tx Timers and Tx flags are updated
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 2) Com_Prv_SigTxChangeMode/Com_Prv_SigGrpTxChangeMode
 *   a. a set of tx timers and Tx Flags are updated when called from Com_SendSignal/Com_SendSignalGroup/Com_SendSignalGroupArray
 * 3) Com_InternalMainFunctionTx
 *   a. a set of tx timers and Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TXCONFIRMATION(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TXCONFIRMATION(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area IPDUGROUPSTART:
 * Resource:
 * Tx Ipdu data buffer
 * Scenario:
 * Lock is applicable only in case Signal/SignalGroup data is updated while TxIpdu is stopped.
 * The below are the possibilites of shared resource access(Inconsistent Tx Ipdu data)
 * If any other signal/Signal group of a this particular Ipdu is being updated by any of the below API's
 * Write Operation:
 * 1) Com_InvalidateSignal
 *   a. If a signal update is called for a signal from the same Ipdu with invalid value
 * 2) Com_SendSignal
 *   a. If a signal update is called for a signal  from the same Ipdu
 * 3) Com_SendDynSignal
 *   a. If a dynamic signal update is called for a signal from the same Ipdu
 * 4) Com_InvalidateSignalGroup
 *   a. If a signal group is invalidated for the same Ipdu
 * 5) Com_SendSignalGroup/Com_SendSignalGroupArray
 *   a. If a signal group update is called for a signal group from the same Ipdu
 *
 * Read Operation :
 * 1) Com_TriggerTransmit
 *   a.The data of the Ipdu could be read here(even for stopped TxIpdu)
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_IPDUGROUPSTART(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_IPDUGROUPSTART(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area SWITCHTXIPDU:
 * Resource:
 * Tx Ipdu timers and flags
 * Scenario:
 * The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_Prv_SendIpdu
 *   a. If the transmission is initiated for the Ipdu from Com_InternalMainFunctionTx/Event trigger then
 *      a1. Callout can update Tx Ipdu
 *      a2. Metadata can be updated
 *      a3. IpduCounter value can be updated.
 *   b. The Tx Ipdu data can be updated by other modules along the communication stack
 * 2) Com_Prv_SigTxChangeMode/Com_Prv_SigGrpTxChangeMode
 *   a. a set of tx timers and Tx Flags are updated when called from Com_SendSignal/Com_SendSignalGroup/Com_SendSignalGroupArray
 * 3) Com_InternalMainFunctionTx
 *   a. a set of tx timers and Tx Flags are updated
 * 4) Com_TxConfirmation or Com_TpTxConfirmation
 *   a. a set of Tx Flags are updated
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_SWITCHTXIPDU(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_SWITCHTXIPDU(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

#ifdef COM_METADATA_SUPPORT
/* NOTE on Exclusive area TRANSMIT_METADATA:
 * Resource:
 * Tx metadata Ipdu
 * Scenario:
 * The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_SendSignalWithMetaData
 *   a. During transmission of signal with metadata value
 * 2) Com_SendSignalGroupWithMetaData
 *   a. During transmission of signal group with metadata value
 */
LOCAL_INLINE void SchM_Enter_Com_TxIpduProtArea_TRANSMIT_METADATA(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_TxIpduProtArea_TRANSMIT_METADATA(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif

#ifdef COM_TX_SIGNALGROUP
/* NOTE on Exclusive area of GRPSIGNALBUFFER: Can be generated
 * The Group signal shadow buffer is read/written as below
 * Write Operation:
 * 1) Com_InvalidateShadowSignal
 * 2) Com_UpdateShadowSignal
 * Read Operation :
 * 1) Com_SendSignalGroup
 * 2) Com_IPduGroupControl to change the state of a Ipdu group to Started.
 */

LOCAL_INLINE void SchM_Enter_Com_TxGrpSigBuffer(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_TxGrpSigBuffer(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_TX_SIGNALGROUP */

/* NOTE on Exclusive area of RXIPDUBUFFER:
 * Critical section: RxIPdu-buffer, RxIPdu-Flags
 * Scenario:
 * Write operation:
 *  a. Com_RxIndication:             RxIPdu-buffer and RxIPdu-Flags
 *  b. Com_InternalMainFunctionRx:   RxIPdu-Flags
 *  c. Com_MainFunctionRouteSignals: RxIPdu-Flags
 * Read operation:
 *  a. Com_InternalMainFunctionRx:   RxIPdu-buffer and RxIPdu-Flags
 *  b. Com_MainFunctionRouteSignals: RxIPdu-Flags
 *
 * If there is a possibility that the above mentioned APIs can interrupt each other,
 * then this exclusive area needs to be enabled.
 */
LOCAL_INLINE void SchM_Enter_Com_RxPduBuffer(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_RxPduBuffer(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


#ifdef COM_RxSigUpdateTimeout
/* NOTE on Exclusive area of Rx-Signal with UpdateBit
 * The signal buffer is read/written as below
 * Write Operation:
 * 1) Com_ReceptionDMControl
 * 2) Com_InternalMainFunctionRx
 * 3) Com_RxIndication
 * Read Operation :
 * 1) Com_ReceptionDMControl
 * 2) Com_InternalMainFunctionRx
 * 3) Com_RxIndication
 * */
LOCAL_INLINE void SchM_Enter_Com_RxSigToTicks (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigToTicks (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
/* NOTE on Exclusive area of SigUpdatebitTimer: Can be generated
 * The Group signal shadow buffer is read/written as below
 * Write Operation:
 * 1) Com_ReceptionDMControl
 * 2) Com_InternalMainFunctionRx
 * 3) Com_RxIndication
 * Read Operation :
 * 1) Com_ReceptionDMControl
 * 2) Com_InternalMainFunctionRx
 * 3) Com_RxIndication
 * */
LOCAL_INLINE void SchM_Enter_Com_RxSigGrpToTicks (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigGrpToTicks (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /*#ifdef COM_RxSigGrpUpdateTimeout */

/* NOTE on Exclusive area MAINFUNCTIONRX:
Resource:
1. The Rx signal buffer is updated here in case of timeout
2. Rx Timeout operation consistency is to be ensured using this lock
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_RxIndication
 *   a. Signal data is updated per signal into the signal buffers
 *   b. Signal data is updated for float signals
 *    Read Operation:
 * 2) Com_ReceiveSignal
 *   a. Signal data is read from the signal buffers.
 */
LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_MAINFUNCTIONRX (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}
LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_MAINFUNCTIONRX (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area RXINDICATION:
Resource:
1. The Rx signal buffer is updated here in case of Ipdu reception
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_InternalMainFunctionRx
 *   a. Signal data is updated per signal after timeout
 *    Read Operation:
 * 2) Com_ReceiveSignal
 *   a. Signal data is read from the signal buffers.
 */
LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_RXINDICATION (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_RXINDICATION (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


/* NOTE on Exclusive area RECEIVESIGNAL:
Resource:
1. The Rx signal buffer is read here after Ipdu reception
Scenario:
The below are the possibilites of shared resource access
 * Write Operation:
 * 1) Com_InternalMainFunctionRx
 *   a. Signal data is updated per signal after timeout
 * 2) Com_RxIndication
 *   a. Signal data is updated per signal into the signal buffers
 *   b. Signal data is updated for float signals
 */
LOCAL_INLINE void SchM_Enter_Com_RxSigBuff_RECEIVESIGNAL (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigBuff_RECEIVESIGNAL  (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}


#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
/* NOTE on Exclusive area of Rx Signal buffer: Can be generated
 * The application buffer is updated here, and the protection is intended for Rx dynamic signal buffer
 * Write Operation:
 * 1) Com_RxIndication
 * Read Operation :
 * 1) Com_ReceiveSignal
 */
LOCAL_INLINE void SchM_Enter_Com_RxSigDynBuff (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigDynBuff (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
/* NOTE on Exclusive area of Rx Group Signal buffer: Can be generated
 * The application buffer is updated here, and the protection is intended for Rx dynamic group signal buffer
 * Write Operation:
 * 1) Com_RxIndication
 * Read Operation :
 * 1) Com_ReceiveDynSignal
 */
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigDynBuff (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxGrpSigDynBuff (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT */

#ifdef COM_RX_SIGNALGROUP
/* NOTE on Exclusive area of RXGRPSIGNALSECONDBUFFER: Can be generated
 * The Group signal shadow buffer is read/written as below
 * Write Operation:
 * 1) Com_RxIndication
 * 2) Com_InternalMainFunctionRx
 * Read Operation :
 * 1) Com_ReceiveSignalGroup
 * 2) Com_Prv_PackRxGrpSigGwBufferData - SignalGroup Gateway from Rx to Tx GroupSignal shadow buffer
 */
/* As the lock is around the loop for all the group signals the Interrupt locking time can vary*/
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigSecBuff (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxGrpSigSecBuff(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if defined( COM_RX_SIGNALGROUP_ARRAY )
/* NOTE on Exclusive area of RX SignalGroup Array Buffer: Can be generated
 * The SignalGroup Array buffer is read/written as below
 * Write Operation:
 *  1) Com_RxIndication
 *  2) Com_InternalMainFunctionRx
 * Read Operation :
 *  1) Com_ReceiveSignalGroupArray
 */
/* As the lock is around the loop for all the group signals the Interrupt locking time can vary*/
LOCAL_INLINE void SchM_Enter_Com_RxSigGrpArrayBuff( void )
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigGrpArrayBuff( void )
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #if defined( COM_RX_SIGNALGROUP_ARRAY ) */

#ifdef COM_RX_SIGNALGROUP
/* NOTE on Exclusive area of Rx group Signal buffer: Can be generated
 * The application buffer is updated here, and the protection is intended for Rx group signal buffer
 * Write Operation:
 * 1) Com_ReceiveSignalGroup
 * Read Operation :
 * 1) Com_ReceiveShadowSignal
 *
 * Use case scenario: Use below locks only in case, Com_ReceiveSignalGroup & Com_ReceiveShadowSignal api's
 * can interrupt each other. If Com_ReceiveSignalGroup & Com_ReceiveShadowSignal api's are called from the
 * same task context, then below locks can be left empty.
 */
/* As the lock is around the loop for all the group signals the Interrupt locking time can vary*/
LOCAL_INLINE void SchM_Enter_Com_RxGrpSigBuff (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxGrpSigBuff (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

/* NOTE on Exclusive area when accessing the Gw buffers of signals -- This is a multicore lock:
 * If there is a possibility that in an explicit multi-core scenario
 * Write Operation:
 * 1) Com_RxIndication (Multiple Calls of Com_RxIndication) in one core
 * Read Operation :
 * 1) Com_MainFunctionRouteSignals in other core
 * There can be inconsitencies in the data access, hence this area is enabled
 */

LOCAL_INLINE void SchM_Enter_Com_RxSigGwMcLockProtArea (void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxSigGwMcLockProtArea (void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}

#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)*/


/**********************************************************************************************************************
 * Pdu based locks : Below locks are applicable only in case 'ComEnablePduBasedLocks' parameter is enabled and
 * use these locks to avoid multiple short locks
 *********************************************************************************************************************/
/* This lock shall be enabled in Com_MainFunctionRouteSignals()
 * Which protects the complete Rx Ipdu processing from the queue and avoids multiple short locks:
 * The below locks can be disabled and a common Pdu lock (SchM_Enter_Com_RxGateway) can be enabled
 * SchM_Enter_Com_RxSigGwMcLockProtArea()
 * SchM_Enter_Com_RxGrpSigSecBuff()
 * SchM_Enter_Com_TxGrpSigBuffer()
 */
# if (defined(COM_PDUBASEDLOCKSENABLED)) && (defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY))
LOCAL_INLINE void SchM_Enter_Com_RxGateway(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxGateway(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
# endif /* # if (defined(COM_PDUBASEDLOCKSENABLED)) && (defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)) */


/* This lock shall be enabled in Com_InternalMainFunctionRx()
 * Which protects the complete Rx Ipdu processing and avoids multiple short locks:
 * The below locks can be disabled and a common Pdu(SchM_Enter_Com_RxIPduProcess) lock can be enabled
 * SchM_Enter_Com_RxPduBuffer()
 * SchM_Enter_Com_RxSigBuff_MAINFUNCTIONRX()
 * SchM_Enter_Com_RxSigToTicks()
 * SchM_Enter_Com_RxSigGrpToTicks()
 */
# ifdef COM_PDUBASEDLOCKSENABLED
LOCAL_INLINE void SchM_Enter_Com_RxIPduProcess(void)
{
     /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxIPduProcess(void)
{
     /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
# endif /* #ifdef COM_PDUBASEDLOCKSENABLED */


/* This lock shall be enabled in Com_RxIndication()
 * Which protects the complete Rx Ipdu processing and avoids multiple short locks:
 * The below locks can be disabled and a common Pdu (SchM_Enter_Com_RxIndication) lock can be enabled
 * SchM_Enter_Com_RxPduBuffer()
 * SchM_Enter_Com_RxSigToTicks()
 * SchM_Enter_Com_RxSigBuff_RXINDICATION()
 * SchM_Enter_Com_RxSigDynBuff()
 * SchM_Enter_Com_RxSigGrpToTicks()
 * SchM_Enter_Com_RxSigGrpArrayBuff()
 * SchM_Enter_Com_RxGrpSigSecBuff()
 */
# ifdef COM_PDUBASEDLOCKSENABLED
LOCAL_INLINE void SchM_Enter_Com_RxIndication(void)
{
     /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_RxIndication(void)
{
     /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
# endif /* #ifdef COM_PDUBASEDLOCKSENABLED */


/* This lock shall be enabled in Com_Prv_SendIpdu()
 * Which protects the complete Tx Ipdu processing and avoids multiple short locks:
 * The below locks can be disabled and a common Pdu (SchM_Enter_Com_TxIPdu) lock can be enabled
 * SchM_Enter_Com_TxIpduProtArea()
 */
# ifdef COM_PDUBASEDLOCKSENABLED
LOCAL_INLINE void SchM_Enter_Com_TxIPdu(void)
{
    /*The integrator shall place his code here which would disable interrupt or acquire lock*/
}

LOCAL_INLINE void SchM_Exit_Com_TxIPdu(void)
{
    /*The integrator shall place his code here which would re-enable interrupt or release lock */
}
# endif /* #ifdef COM_PDUBASEDLOCKSENABLED */


#endif /* COM_CFG_SCHM_H */

