/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef ECUM_CFG_SCHM_H
#define ECUM_CFG_SCHM_H

// ---- Includes --------------------------------------------------------------
/* BSW-15945 */
/*Extern module Headers*/
#include "Rte_Main.h" /*For SchM_Start and SchM_StartTiming if generated by RTE*/
/* END BSW-15945 */

// The integrator shall implement the particular services SchM_Enter and SchM_Exit.


#define SchM_Enter_EcuM()
#define SchM_Exit_EcuM()

/* BSW-15945 */
/* The integrator shall map SchM_Start_Integration and SchM_StartTiming_Integration if RTE generates SchM_Start and SchM_StartTiming.*/
#define SchM_Start_Integration()
#define SchM_StartTiming_Integration()
/* END BSW-15945 */

#endif /* ECUM_CFG_SCHM_H */

// -------- END OF FILE -------------------------------------------------------
