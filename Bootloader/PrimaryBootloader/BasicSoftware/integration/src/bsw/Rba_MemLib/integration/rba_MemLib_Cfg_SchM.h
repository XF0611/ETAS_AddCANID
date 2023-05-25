/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 




#ifndef RBA_MEMLIB_CFG_SCHM_H
#define RBA_MEMLIB_CFG_SCHM_H

/**
 **********************************************************************************************************************
 * \file   rba_MemLib_SchM.h
 * \brief  Schedule manager interface to be used by the rba_MemLib driver
 * \par    Module Description
 * \Schedule manager for rba_MemLib description
 **********************************************************************************************************************
 */


/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
/* Include if available */
/* BSW-17396 */
// #include "rba_BswSrv.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

/* To be defined by the integrator
 * *******************************
 * Exclusive area "rba_MemLib":
 * - This exclusive area protects all accesses to shared ressources within the component,
 * - On multi-core machines, a lock functionality is required which works across all cores which could invoke the component
 * - On single core machines, a global interrupt lock is sufficient.
 */
/* Here for timer handling */
LOCAL_INLINE FUNC(void, RBA_MEMLIB_CODE) SchM_Enter_rba_MemLib_TimerUs(void);
LOCAL_INLINE FUNC(void, RBA_MEMLIB_CODE) SchM_Exit_rba_MemLib_TimerUs(void);

SchM_Enter_rba_MemLib_TimerUs(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}

SchM_Exit_rba_MemLib_TimerUs(void)
{
    /*The integrator shall place his code here which would disable/lock the interrupt*/
}

#endif
