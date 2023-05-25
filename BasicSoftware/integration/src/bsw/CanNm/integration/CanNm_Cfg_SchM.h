/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef CANNM_CFG_SCHM_H
#define CANNM_CFG_SCHM_H

/* BSW Scheduler header file defining CanNm Exclusive areas. This is included only by CanNm module */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "SchM.h"

/* If the RTE code generation is involved please include the below file */
#include "CanNm_Cfg_Internal.h"
#if (CANNM_ECUC_RB_RTE_IN_USE == STD_ON)
#include "SchM_CanNm.h"
#endif


LOCAL_INLINE void SchM_Enter_CanNm_GetStateNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_GetStateNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_GetPduDataNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_GetPduDataNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_ChangeStateNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_ChangeStateNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_GetUserDataNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_GetUserDataNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_SetUserDataNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_SetUserDataNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_MainFunctionNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_MainFunctionNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_RxIndicationNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_RxIndicationNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_UpdateTxPduDataNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_UpdateTxPduDataNoNest(void);

LOCAL_INLINE void SchM_Enter_CanNm_EnableCommunicationNoNest(void);
LOCAL_INLINE void SchM_Exit_CanNm_EnableCommunicationNoNest(void);

/* NOTE on Exclusive area SchM_Enter_CanNm_GetStateNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area GETSTATE
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_GetStateNoNest (void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_GetStateNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_GetPduDataNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area GETPDUDATA
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_GetPduDataNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_GetPduDataNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_ChangeStateNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area CHANGESTATE
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_ChangeStateNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_ChangeStateNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_GetUserDataNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area GETUSERDATA
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_GetUserDataNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_GetUserDataNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_SetUserDataNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area SETUSERDATA
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_SetUserDataNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_SetUserDataNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_MainFunctionNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area MAINFUNCTION
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_MainFunctionNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_MainFunctionNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_RxIndicationNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area RXINDICATION
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_RxIndicationNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_RxIndicationNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_UpdateTxPduDataNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area UPDATETXPDUDATA
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_UpdateTxPduDataNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_UpdateTxPduDataNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

/* NOTE on Exclusive area SchM_Enter_CanNm_EnableCommunicationNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area ENABLECOMMUNICATION
 *
 * */

LOCAL_INLINE void SchM_Enter_CanNm_EnableCommunicationNoNest(void)
        {
            /*The integrator should place the code here which Disables/Locks the interrupt*/
        }

LOCAL_INLINE void SchM_Exit_CanNm_EnableCommunicationNoNest(void)
        {
            /*The integrator should place the code here which Enables/Unlocks the interrupt*/
        }

#endif /* CANNM_CFG_SCHM_H */


