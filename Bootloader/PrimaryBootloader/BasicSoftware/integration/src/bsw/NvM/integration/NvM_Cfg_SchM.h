/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


#ifndef NVM_CFG_SCHM_H
#define NVM_CFG_SCHM_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
/*
 * Exclusive area "NvM_Main":
 * - This exclusive area protects all accesses to shared ressources within the NvM module, in particular between NvM's
 *   public service APIs and the NvM_MainFunction.
 * - On multi-core machines, a lock functionality is required which works across all cores which could invoke NvM's
 *   public service APIs or schedules the NvM_MainFunction. On single core machines, a global interrupt lock or
 *   (possibly NvM-specific) semaphore is sufficient.
 * - In NvM, all SchM_Enter/Exit_NvM_Main calls are in the same C block, so block enforcing lock macros can be used.
 */
/* BSW-17396 */
/* The integrator shall implement the particular services SchM_Enter and SchM_Exit.*/

LOCAL_INLINE FUNC(void, NvM_CODE) SchM_Enter_NvM_Main(void);
LOCAL_INLINE FUNC(void, NvM_CODE) SchM_Exit_NvM_Main(void);

LOCAL_INLINE FUNC(void, NvM_CODE) SchM_Enter_NvM_Main(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}

LOCAL_INLINE FUNC(void, NvM_CODE) SchM_Exit_NvM_Main(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}


/*
 * Optimization possibility if NvM_MainFunction protection needs not protected against multi-calls at the
 * same time. This requires that above protection functions are used.
 * NVM_RB_USE_PROTECTION = TRUE:    Protection enabled (more code)     (DEFAULT)
 * NVM_RB_USE_PROTECTION = FALSE:   no protection - only useful in single-core/task configurations
 *
 * Detection of reentrant Main Function calls will be enabled irrespective of this feature,
 * only prevention is controlled by this compiler switch
 *
 * The following consequences may result if the feature is enabled in a project where reentrant
 * invocation of the NvM_MainFunction is not handled completely,
 *
 * Additional consumption of runtime if the feature is enabled
 *
 * Priority reversal (i.e., If an interrupted event waits for completion of certain operation upon
 * calling NvM_MainFunction and the interrupt occured in the middle of NvM_MainFunction execution)
 *
 * Jumping out in the middle of NvM_MainFunction to shutdown is clearly forbidden (i.e., If NvM_MainFunction is
 * expected to perform some activity in the shutdown sequence, it will never work anymore, since the
 * interrupted function will have the lock forever)
 *
 */
#define NVM_RB_USE_PROTECTION   (TRUE)

/* NVM_CFG_SCHM_H */
#endif
