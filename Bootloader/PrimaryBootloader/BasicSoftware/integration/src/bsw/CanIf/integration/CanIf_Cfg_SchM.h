/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef CANIF_CFG_SCHM_H
#define CANIF_CFG_SCHM_H

#include "SchM.h"
#include "SchM_Default.h"
#include "CanIf_Cfg.h"
/* If the RTE code generation is Involved please include the below files */
#include "SchM_CanIf.h"

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
#include "OS.h" /* to get CoreId, Activate Task and Call Interrupt */

/** Acquire a global lock, including interrupts deactivated on the local core */
#define Schm_CanIf_XCore_Enter()  SCHM_ENTER_DEFAULT()

/** release a global lock*/
#define Schm_CanIf_XCore_Exit()  SCHM_EXIT_DEFAULT()

/** This function activates the given Software Interrupt ID */
#define Schm_CanIf_XCore_Activate_SW_Irq(id) /* not implemented */

/** This function immediately activates the task given as parameter*/
#define Schm_CanIf_XCore_Activate_Task(id)  Os_ActivateTask((TaskType)id)

/** This function returns the ID of the current core. Expected result is {0 = Core 0, 1 = Core 1} */
#define Schm_CanIf_XCore_GetCurrentCoreId()  GetCoreID()

#define SCHM_MULTICORE_OPTIMAL STD_ON /* STD_ON: disables SchM Locks for optimal Configuration according to Multicore */
#else
#define SCHM_MULTICORE_OPTIMAL STD_OFF
#endif

/* FC_VariationPoint_END */

/* The integrator shall implement the particular services SchM_Enter and SchM_Exit */

/* Since there is currently only one resource used, there is no need to consider the resource name */

LOCAL_INLINE void SchM_Enter_CanIf_TxBufAccessNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_TxBufAccessNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_RxBufAccessNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_RxBufAccessNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_ControllerState(void);
LOCAL_INLINE void SchM_Exit_CanIf_ControllerState(void);

LOCAL_INLINE void SchM_Enter_CanIf_NewMsgRxWkpEvtNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_NewMsgRxWkpEvtNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_TxRxNotifNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_TxRxNotifNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_DynIdNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_DynIdNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_PNPduStatusNoNest(void);
LOCAL_INLINE void SchM_Exit_CanIf_PNPduStatusNoNest(void);

LOCAL_INLINE void SchM_Enter_CanIf_TxMuxing(void);
LOCAL_INLINE void SchM_Exit_CanIf_TxMuxing(void);



LOCAL_INLINE void SchM_Enter_CanIf_TxBufAccessNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */
    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_TxBufAccessNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_RxBufAccessNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_RxBufAccessNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_ControllerState(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_ControllerState(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

   /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_NewMsgRxWkpEvtNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_NewMsgRxWkpEvtNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_TxRxNotifNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

   /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_TxRxNotifNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_DynIdNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

   /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_DynIdNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

   /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_PNPduStatusNoNest(void)
{
    /* The integrator shall place here the code which would lock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_PNPduStatusNoNest(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}


LOCAL_INLINE void SchM_Enter_CanIf_TxMuxing(void)
{
    /* The integrator shall place here the code which would lock the interrupts */
    /*NOTE: This lock shall ensure to disable TX-request and TX-confirmation interrupts since this is used in
     * CanIf_Transmit() and CanIf_TxConfirmation(); else it would affect the TX-multiplexing behaviour.*/

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

LOCAL_INLINE void SchM_Exit_CanIf_TxMuxing(void)
{
    /* The integrator shall place here the code which would unlock the interrupts */

    /* DD: not needed since the Multicore project uses only Polling configuration and the CAN drv operations are limited to one core */
#if (!defined SCHM_MULTICORE_OPTIMAL) || (SCHM_MULTICORE_OPTIMAL != STD_ON)

#endif
}

#endif /* CANIF_CFG_SCHM_H */



