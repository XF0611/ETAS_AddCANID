/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */

#ifndef RBA_BSWSRV_CFG_H
#define RBA_BSWSRV_CFG_H

/*
 **********************************************************************************************************************
 * This file is for configuration of component rba_BswSrv
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* 64BIT Operations like memset_64, memcpy_64 and memcompare_64 can enable or disable*/
#define RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT       STD_OFF

/* Activate the following #define ONLY for Compiler Library Optimisation. */
#define RBA_BSWSRV_CFG_COMPILER_LIBRARY_SUPPORT     STD_OFF



/* RBA_BSWSRV_CFG_H */
#endif


