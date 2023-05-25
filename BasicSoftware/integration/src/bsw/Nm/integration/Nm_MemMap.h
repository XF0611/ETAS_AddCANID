/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 


/* MR12 DIR 4.10 VIOLATION: MemMap header concept - no protection against repeated inclusion intended. */
#define MEMMAP_ERROR
#if defined NM_START_SEC_VAR_CLEARED_UNSPECIFIED
    /* UNSPECIFIED */
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif defined NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
 /* Data memory: unspecified size */
#elif defined NM_START_SEC_CONST_UNSPECIFIED
    /* Unspecified size */
    #define BSW_START_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_START_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
#elif defined NM_STOP_SEC_CONST_UNSPECIFIED
    #define BSW_STOP_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_STOP_SEC_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
/* Data memory: 8-bit size */
#elif defined NM_START_SEC_CONST_8
    #define BSW_START_SEC_CONST_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_START_SEC_CONST_8
    #undef MEMMAP_ERROR
#elif defined NM_STOP_SEC_CONST_8
    #define BSW_STOP_SEC_CONST_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_STOP_SEC_CONST_8
    #undef MEMMAP_ERROR
/* Code memory */
#elif defined NM_START_SEC_CODE
  #define  BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
  #undef NM_START_SEC_CODE
  #undef MEMMAP_ERROR
#elif defined NM_STOP_SEC_CODE
  #define  BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
  #undef NM_STOP_SEC_CODE
  #undef MEMMAP_ERROR
/* RAM memory: 32-bit size */
#elif defined NM_START_SEC_VAR_CLEARED_32
    #define BSW_START_SEC_VAR_CLEARED_32
        #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_START_SEC_VAR_CLEARED_32
    #undef MEMMAP_ERROR
#elif defined NM_STOP_SEC_VAR_CLEARED_32
    #define BSW_STOP_SEC_VAR_CLEARED_32
        #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef,
   AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef NM_STOP_SEC_VAR_CLEARED_32
    #undef MEMMAP_ERROR
#endif
#ifdef MEMMAP_ERROR
   #error "Nm_MemMap.h, wrong #pragma command"
#endif



