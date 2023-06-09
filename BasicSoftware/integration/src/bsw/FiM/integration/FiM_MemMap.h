/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/* MemMap.h for AUTOSAR FiM R4.0 Rev 2 */

#define MEMMAP_ERROR

/* ============================   BSW   ============================ */
/* MR12 RULE 1.2, 4.10, 20.5 VIOLATION:
   1.2: Memmap file must be included multiple times
   4.10: Memmap file must be included multiple times
   20.5: AUTOSAR MemMap concept requires #undef, AUTOSAR MemMap requirements are incompatible to MISRA */
#if defined FIM_START_SEC_VAR_CLEARED
    #if defined FIM_WAS_STARTED_SEC_VAR_CLEARED
        #error FIM_WAS_STARTED_SEC_VAR_CLEARED already defined
    #endif
    #define BSW_START_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_START_SEC_VAR_CLEARED
    #define FIM_WAS_STARTED_SEC_VAR_CLEARED
#elif defined FIM_STOP_SEC_VAR_CLEARED
    #if ! defined FIM_WAS_STARTED_SEC_VAR_CLEARED
        #error FIM_STOP_SEC_VAR_CLEARED without START_SEC
    #else
        #undef FIM_WAS_STARTED_SEC_VAR_CLEARED
    #endif
    #define BSW_STOP_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_STOP_SEC_VAR_CLEARED
#elif defined FIM_START_SEC_VAR_INIT
    #if defined FIM_WAS_STARTED_SEC_VAR_INIT
        #error FIM_WAS_STARTED_SEC_VAR_INIT already defined
    #endif
    #define BSW_START_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_START_SEC_VAR_INIT
    #define FIM_WAS_STARTED_SEC_VAR_INIT
#elif defined FIM_STOP_SEC_VAR_INIT
    #if ! defined FIM_WAS_STARTED_SEC_VAR_INIT
        #error FIM_STOP_SEC_VAR_INIT without START_SEC
    #else
        #undef FIM_WAS_STARTED_SEC_VAR_INIT
    #endif
    #define BSW_STOP_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_STOP_SEC_VAR_INIT
#elif defined FIM_START_SEC_CONST
    #if defined FIM_WAS_STARTED_SEC_CONST
        #error FIM_WAS_STARTED_SEC_CONST already defined
    #endif
    #define BSW_START_SEC_DEFAULT_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_START_SEC_CONST
    #define FIM_WAS_STARTED_SEC_CONST
#elif defined FIM_STOP_SEC_CONST
    #if ! defined FIM_WAS_STARTED_SEC_CONST
        #error FIM_STOP_SEC_CONST without START_SEC
    #else
        #undef FIM_WAS_STARTED_SEC_CONST
    #endif
    #define BSW_STOP_SEC_DEFAULT_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef FIM_STOP_SEC_CONST
#elif defined FIM_START_SEC_CODE
    #if defined FIM_WAS_STARTED_SEC_CODE
        #error FIM_WAS_STARTED_SEC_CODE already defined
    #endif
    #define BSW_START_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FIM_START_SEC_CODE
    #define FIM_WAS_STARTED_SEC_CODE
#elif defined FIM_STOP_SEC_CODE
    #if ! defined FIM_WAS_STARTED_SEC_CODE
        #error FIM_STOP_SEC_CODE without START_SEC
    #else
        #undef FIM_WAS_STARTED_SEC_CODE
    #endif
    #define BSW_STOP_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FIM_STOP_SEC_CODE
/* Added for the memory section generated by the RTE */
#elif defined(FiM_START_SEC_CODE)
    #define BSW_START_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FiM_START_SEC_CODE
 #elif defined(FiM_STOP_SEC_CODE)
    #define BSW_STOP_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FiM_STOP_SEC_CODE
#elif defined(FIM_START_SEC_CODE)
    #define BSW_START_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FIM_START_SEC_CODE
 #elif defined(FIM_STOP_SEC_CODE)
    #define BSW_STOP_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
    #undef FIM_STOP_SEC_CODE
#elif defined MEMMAP_ERROR
    #error "FiM_MemMap.h, wrong #pragma command"
#endif

