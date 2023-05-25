/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef FIM_CFG_SCHM_H
#define FIM_CFG_SCHM_H

/* BSW-17389 */
LOCAL_INLINE void SchM_Enter_FiM_Monitor_NoNest(void);
LOCAL_INLINE void SchM_Exit_FiM_Monitor_NoNest(void);
LOCAL_INLINE void SchM_Enter_FiM_Status_NoNest(void);
LOCAL_INLINE void SchM_Exit_FiM_Status_NoNest(void);

LOCAL_INLINE void SchM_Enter_FiM_Monitor_NoNest(void)
{
	/*The integrator shall place his code here which would disable/lock the interrupt*/
}

LOCAL_INLINE void SchM_Exit_FiM_Monitor_NoNest(void)
{
	/* The integrator shall place here the code which would unlock the interrupts */
}

LOCAL_INLINE void SchM_Enter_FiM_Status_NoNest(void)
{
	/*The integrator shall place his code here which would disable/lock the interrupt*/
}

LOCAL_INLINE void SchM_Exit_FiM_Status_NoNest(void)
{
	/* The integrator shall place here the code which would unlock the interrupts */
}

#endif /* include protection */

