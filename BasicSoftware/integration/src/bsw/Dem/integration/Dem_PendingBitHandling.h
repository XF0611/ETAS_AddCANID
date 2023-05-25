/* BSW-11069 */
/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


#ifndef DEM_PENDING_DTC_BIT_HANDLING_H
#define DEM_PENDING_DTC_BIT_HANDLING_H

/**
 * \file
 * \ingroup DEM_PRJ_TPL
 * \brief This configuration switch the setting of status bit 2 (PendingDTC) is independent from successful event storage
 *
 * TRUE: the setting of status bit 2 (PendingDTC) is independent from successful event storage 
 * FALSE: the setting of status bit 2 (PendingDTC) is dependent from successful event storage 
 */

#define DEM_CFG_SETTING_PENDINGDTC_INDEPENDENT_FROM_STOCO FALSE

#endif
/* END BSW-11069 */
