/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 

#ifndef DCM_TYPE_INTEGRATION_H 
#define DCM_TYPE_INTEGRATION_H

/* This file shall be used when dcmRbRTEsupport==false to define additional type definitions
  * Uncomment lines below to use it.
 */
/* BSW-17325 */

#include "Std_Types.h"
#if(DCM_CFG_RTESUPPORT_ENABLED == DCM_CFG_OFF)
/*IOcontrol parameter definition*/
/* typedef uint8 Dcm_InputOutputControlParameterType;*/
#endif
#endif /* DCM_TYPE_INTEGRATION_H */
