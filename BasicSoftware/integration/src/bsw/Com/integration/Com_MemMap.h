/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



/* MR12 RULE 1.2 VIOLATION: MemMap header concept - no protection against multiple inclusion intended */
#ifndef COM_MEMMAP_H
#define COM_MEMMAP_H


/* Data memory: unspecified size */
#if defined COM_START_SEC_CONST_UNSPECIFIED
    #define BSW_START_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONST_UNSPECIFIED
#elif defined COM_STOP_SEC_CONST_UNSPECIFIED
    #define BSW_STOP_SEC_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONST_UNSPECIFIED
/* Data memory: 32-bit size */
#elif defined COM_START_SEC_CONST_32
    #define BSW_START_SEC_CONST_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONST_32
#elif defined COM_STOP_SEC_CONST_32
    #define BSW_STOP_SEC_CONST_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONST_32
/* Data memory: 8-bit size */
#elif defined COM_START_SEC_CONST_8
    #define BSW_START_SEC_CONST_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONST_8
#elif defined COM_STOP_SEC_CONST_8
    #define BSW_STOP_SEC_CONST_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONST_8
/* Code memory */
#elif defined COM_START_SEC_CODE
    #define  BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CODE
#elif defined COM_STOP_SEC_CODE
    #define  BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CODE
/* Code that shall go into fast code memory segments - Scratchpad Memory (SPRAM) */
#elif defined COM_START_SEC_CODE_FAST
    #define  BSW_START_SEC_CODE_FAST
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CODE_FAST
#elif defined COM_STOP_SEC_CODE_FAST
    #define  BSW_STOP_SEC_CODE_FAST
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CODE_FAST
/* Start: Used for variables that are initialized with values after every reset */
/* Ram memory: Unspecified size */
#elif defined COM_START_SEC_VAR_INIT_UNSPECIFIED
    #define BSW_START_SEC_VAR_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_INIT_UNSPECIFIED
#elif defined COM_STOP_SEC_VAR_INIT_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_INIT_UNSPECIFIED
/* End: Used for variables that are initialized with values after every reset */
/* RAM memory: Unspecified size */
#elif defined COM_START_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_64
    #define BSW_START_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_64
#elif defined COM_STOP_SEC_VAR_CLEARED_64
    #define BSW_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_64
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_32
    #define BSW_START_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_32
#elif defined COM_STOP_SEC_VAR_CLEARED_32
    #define BSW_STOP_SEC_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_32
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_16
    #define BSW_START_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_16
#elif defined COM_STOP_SEC_VAR_CLEARED_16
    #define BSW_STOP_SEC_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_16
/* RAM memory: 8-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_8
    #define BSW_START_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_8
#elif defined COM_STOP_SEC_VAR_CLEARED_8
    #define BSW_STOP_SEC_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_8
/* RAM memory: Boolean */
#elif defined COM_START_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_START_SEC_VAR_CLEARED_BOOLEAN
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_BOOLEAN
#elif defined COM_STOP_SEC_VAR_CLEARED_BOOLEAN
    #define BSW_STOP_SEC_VAR_CLEARED_BOOLEAN
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_BOOLEAN
/*Core-1*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE1_VAR_CLEARED_8
    #define BSW_START_SEC_CORE1_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE1_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE1_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE1_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE1_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE1_VAR_CLEARED_16
    #define BSW_START_SEC_CORE1_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE1_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE1_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE1_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE1_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE1_VAR_CLEARED_32
    #define BSW_START_SEC_CORE1_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE1_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE1_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE1_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE1_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE1_VAR_CLEARED_64
    #define BSW_START_SEC_CORE1_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE1_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE1_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE1_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE1_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE1_VAR_CLEARED_UNSPECIFIED
/*Core-2*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE2_VAR_CLEARED_8
    #define BSW_START_SEC_CORE2_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE2_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE2_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE2_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE2_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE2_VAR_CLEARED_16
    #define BSW_START_SEC_CORE2_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE2_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE2_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE2_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE2_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE2_VAR_CLEARED_32
    #define BSW_START_SEC_CORE2_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE2_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE2_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE2_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE2_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE2_VAR_CLEARED_64
    #define BSW_START_SEC_CORE2_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE2_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE2_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE2_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE2_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE2_VAR_CLEARED_UNSPECIFIED
/*Core-3*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE3_VAR_CLEARED_8
    #define BSW_START_SEC_CORE3_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE3_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE3_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE3_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE3_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE3_VAR_CLEARED_16
    #define BSW_START_SEC_CORE3_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE3_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE3_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE3_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE3_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE3_VAR_CLEARED_32
    #define BSW_START_SEC_CORE3_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE3_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE3_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE3_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE3_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE3_VAR_CLEARED_64
    #define BSW_START_SEC_CORE3_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE3_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE3_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE3_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE3_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE3_VAR_CLEARED_UNSPECIFIED
/*Core-4*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE4_VAR_CLEARED_8
    #define BSW_START_SEC_CORE4_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE4_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE4_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE4_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE4_VAR_CLEARED_16
    #define BSW_START_SEC_CORE4_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE4_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE4_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE4_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE4_VAR_CLEARED_32
    #define BSW_START_SEC_CORE4_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE4_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE4_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE4_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE4_VAR_CLEARED_64
    #define BSW_START_SEC_CORE4_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE4_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE4_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE4_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE4_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE4_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE4_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE4_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE4_VAR_CLEARED_UNSPECIFIED	
/*Core-5*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE5_VAR_CLEARED_8
    #define BSW_START_SEC_CORE5_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE5_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE4_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE5_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE5_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE5_VAR_CLEARED_16
    #define BSW_START_SEC_CORE5_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE5_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE5_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE5_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE5_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE5_VAR_CLEARED_32
    #define BSW_START_SEC_CORE5_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE5_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE5_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE5_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE5_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE5_VAR_CLEARED_64
    #define BSW_START_SEC_CORE5_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE5_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE5_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE5_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE5_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE5_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE5_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE5_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE5_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE5_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE5_VAR_CLEARED_UNSPECIFIED	
/*Core-6*/	
/* RAM memory: 8-bit size */	
#elif defined COM_START_SEC_CORE6_VAR_CLEARED_8
    #define BSW_START_SEC_CORE6_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE6_VAR_CLEARED_8
#elif defined COM_STOP_SEC_CORE6_VAR_CLEARED_8
    #define BSW_STOP_SEC_CORE6_VAR_CLEARED_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE6_VAR_CLEARED_8
/* RAM memory: 16-bit size */
#elif defined COM_START_SEC_CORE6_VAR_CLEARED_16
    #define BSW_START_SEC_CORE6_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE6_VAR_CLEARED_16
#elif defined COM_STOP_SEC_CORE6_VAR_CLEARED_16
    #define BSW_STOP_SEC_CORE6_VAR_CLEARED_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE6_VAR_CLEARED_16
/* RAM memory: 32-bit size */
#elif defined COM_START_SEC_CORE6_VAR_CLEARED_32
    #define BSW_START_SEC_CORE6_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE6_VAR_CLEARED_32
#elif defined COM_STOP_SEC_CORE6_VAR_CLEARED_32
    #define BSW_STOP_SEC_CORE6_VAR_CLEARED_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE6_VAR_CLEARED_32
/* RAM memory: 64-bit size */
#elif defined COM_START_SEC_CORE6_VAR_CLEARED_64
    #define BSW_START_SEC_CORE6_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE6_VAR_CLEARED_64
#elif defined COM_STOP_SEC_CORE6_VAR_CLEARED_64
    #define BSW_STOP_SEC_CORE6_VAR_CLEARED_64
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE6_VAR_CLEARED_64
/* Ram memory: Unspecified size */	
#elif defined COM_START_SEC_CORE6_VAR_CLEARED_UNSPECIFIED
    #define BSW_START_SEC_CORE6_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CORE6_VAR_CLEARED_UNSPECIFIED
#elif defined COM_STOP_SEC_CORE6_VAR_CLEARED_UNSPECIFIED
    #define BSW_STOP_SEC_CORE6_VAR_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CORE6_VAR_CLEARED_UNSPECIFIED		
/**********************************************************************************************************************
 * PBL Constants
 *********************************************************************************************************************/
/* Data memory: [Post-Build] 32-bit size */
#elif defined COM_START_SEC_CONFIG_DATA_POSTBUILD_32
    #define BSW_START_SEC_CONFIG_DATA_POSTBUILD_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONFIG_DATA_POSTBUILD_32
#elif defined COM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
    #define BSW_STOP_SEC_CONFIG_DATA_POSTBUILD_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
/* Data memory: [Post-Build] Unspecified size */
#elif defined COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #define BSW_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#elif defined COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #define BSW_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
/* Ram memory: [Post-Build] Boolean */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
    #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
/* Ram memory: [Post-Build] 8 -bit size */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_8
    #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_8
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
/* Ram memory: [Post-Build] 16-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_16
    #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_16
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_16
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_16
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_16
/* Ram memory: [Post-Build] 32-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_32
     #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_32
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_32
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_32
/* Ram memory: [Post-Build] 64-bit size */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_64
    #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_64
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_64
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_64
/* Ram memory: [Post-Build] Unspecified size */
#elif defined COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #define BSW_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#elif defined COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #define BSW_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
/* Data memory: [Pre-Compile] 32-bit size */
#elif defined COM_START_SEC_CONFIG_DATA_32
     #define BSW_START_SEC_CONFIG_DATA_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONFIG_DATA_32
#elif defined COM_STOP_SEC_CONFIG_DATA_32
    /* BSW-17388 */
    #define BSW_STOP_SEC_CONFIG_DATA_32
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONFIG_DATA_32	
/* Data memory: [Pre-Compile] UNSPECIFIED size */
#elif defined COM_START_SEC_CONFIG_DATA_UNSPECIFIED
     #define BSW_START_SEC_CONFIG_DATA_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    /* BSW-17388 */
    #define BSW_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #include "Bsw_MemMap.h"
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef. Hence warning is suppressed. */
    #undef COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED	
#else
    #error "Com_MemMap.h, wrong #pragma command"
#endif
#undef COM_MEMMAP_H
#endif /* #ifndef COM_MEMMAP_H */


