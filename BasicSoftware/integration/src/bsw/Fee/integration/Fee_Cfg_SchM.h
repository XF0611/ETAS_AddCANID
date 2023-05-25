/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef FEE_CFG_SCHM_H
#define FEE_CFG_SCHM_H

/**
 **********************************************************************************************************************
 * \file   SchM_Fee.h
 * \brief  Schedule manager interface to be used by the Fee driver
 * \par    Module Description
 * \Schedule manager for FEE description
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
/* Include if available */
//#include "SchM_Default.h


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
/*
 * The implementation of this macro has to ensure that no other
 * Fee component code is executed while the code between
 * SchM_Enter_Fee_Order() and SchM_Exit_Fee_Order() is executed.
 */

/* BSW-17396 */
LOCAL_INLINE FUNC(void, FEE_CODE) SchM_Enter_Fee_Order(void);
LOCAL_INLINE FUNC(void, FEE_CODE) SchM_Exit_Fee_Order(void);
 
LOCAL_INLINE FUNC(void, FEE_CODE) SchM_Enter_Fee_Order(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}

LOCAL_INLINE FUNC(void, FEE_CODE) SchM_Exit_Fee_Order(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}

/*
 * Optimization possibility if Fee_MainFunction protection needs not protected against multi-calls at the
 * same time. This requires that above protection functions are used.
 * FEE_RB_USE_PROTECTION = TRUE:    Protection enabled (more code)     (DEFAULT)
 * FEE_RB_USE_PROTECTION = FALSE:   no protection - only useful in single-core/task configurations
 *
 * Detection of reentrant Main Function calls will be enabled irrespective of this feature,
 * only prevention is controlled by this compiler switch
 *
 * The following consequences may result if the feature is enabled in a project where reentrant
 * invocation of the Fee_Mainfunction is not handled completely,
 *
 * Additional consumption of runtime if the feature is enabled
 *
 * Priority reversal (i.e., If an interrupted event waits for completion of certain operation upon
 * calling Fee_Mainfunction and the interrupt occured in the middle of Fee_Mainfunction execution)
 *
 * Jumping out in the middle of Fee_Mainfunction to shutdown is clearly forbidden (i.e., If Fee_Mainfunction is
 * expected to perform some activity in the shutdown sequence, it will never work anymore, since the
 * interrupted function will have the lock forever)
 *
 */
#define FEE_RB_USE_PROTECTION   (TRUE)
/* FEE_CFG_SCHM_H */
#endif
