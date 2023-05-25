/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


/* rba_MemLib_MemMap.h for AUTOSAR Memory Mapping */
/* MR12 DIR 4.10 VIOLATION: MemMap header concept - no protection against repeated inclusion intended */
/* MR12 RULE 20.14 VIOLATION: All #if are balanced, no failure expected */
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA
*/
/* Note related to MR12 RULE 20.5 VIOLATION (refer also to Change Request 126381):
   - MISRA C comment scope needs to be extended to the rest of the header by deleting empty lines.
   - Introduction of comments is ok to structure the code
*/
/*
 **********************************************************************************************************************
 * Variable mapping
 **********************************************************************************************************************
*/
#if defined   RBA_MEMLIB_START_SEC_VAR_CLEARED_8
    #undef    RBA_MEMLIB_START_SEC_VAR_CLEARED_8
    #define   BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
#elif defined RBA_MEMLIB_STOP_SEC_VAR_CLEARED_8
    #undef    RBA_MEMLIB_STOP_SEC_VAR_CLEARED_8
    #define   BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
#elif defined RBA_MEMLIB_START_SEC_VAR_CLEARED_32
    #undef    RBA_MEMLIB_START_SEC_VAR_CLEARED_32
    #define   BSW_START_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
#elif defined RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
    #undef    RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
    #define   BSW_STOP_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/*
 **********************************************************************************************************************
 * Code mapping
 **********************************************************************************************************************
*/
#elif defined RBA_MEMLIB_START_SEC_CODE
    #undef    RBA_MEMLIB_START_SEC_CODE
    #define   BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
#elif defined RBA_MEMLIB_STOP_SEC_CODE
    #undef    RBA_MEMLIB_STOP_SEC_CODE
    #define   BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
/*********************************************************************************************************************/
#else
    #error "rba_MemLib_MemMap.h: unknown MemMap define "
// Intentional empty line

// End
#endif
