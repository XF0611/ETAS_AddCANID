/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#define MEMMAP_ERROR

/* ============================   BSW   ============================ */
/* MR12 RULE 1.2, 4.10, 20.5 VIOLATION:
   1.2: Memmap file must be included multiple times
   4.10: Memmap file must be included multiple times
   20.5: AUTOSAR MemMap concept requires #undef, AUTOSAR MemMap requirements are incompatible to MISRA */
#if defined RBA_DIAGLIB_START_SEC_VAR_CLEARED
	#if defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_CLEARED
		#error RBA_DIAGLIB_WAS_STARTED_SEC_VAR_CLEARED already defined
	#endif
	#define BSW_START_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_START_SEC_VAR_CLEARED
	#define RBA_DIAGLIB_WAS_STARTED_SEC_VAR_CLEARED
#elif defined RBA_DIAGLIB_STOP_SEC_VAR_CLEARED
	#if ! defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_CLEARED
		#error RBA_DIAGLIB_STOP_SEC_VAR_CLEARED without START_SEC
	#else
		#undef RBA_DIAGLIB_WAS_STARTED_SEC_VAR_CLEARED
	#endif
	#define BSW_STOP_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_RAM0_CLEARED_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_STOP_SEC_VAR_CLEARED
#elif defined RBA_DIAGLIB_START_SEC_VAR_INIT
	#if defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_INIT
		#error RBA_DIAGLIB_WAS_STARTED_SEC_VAR_INIT already defined
	#endif
	#define BSW_START_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_START_SEC_VAR_INIT
	#define RBA_DIAGLIB_WAS_STARTED_SEC_VAR_INIT
#elif defined RBA_DIAGLIB_STOP_SEC_VAR_INIT
	#if ! defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_INIT
		#error RBA_DIAGLIB_STOP_SEC_VAR_INIT without START_SEC
	#else
		#undef RBA_DIAGLIB_WAS_STARTED_SEC_VAR_INIT
	#endif
	#define BSW_STOP_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_RAM0_INIT_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_STOP_SEC_VAR_INIT
#elif defined RBA_DIAGLIB_START_SEC_CONST
	#if defined RBA_DIAGLIB_WAS_STARTED_SEC_CONST
		#error RBA_DIAGLIB_WAS_STARTED_SEC_CONST already defined
	#endif
	#define BSW_START_SEC_DEFAULT_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_START_SEC_CONST
	#define RBA_DIAGLIB_WAS_STARTED_SEC_CONST
#elif defined RBA_DIAGLIB_STOP_SEC_CONST
	#if ! defined RBA_DIAGLIB_WAS_STARTED_SEC_CONST
		#error RBA_DIAGLIB_STOP_SEC_CONST without START_SEC
	#else
		#undef RBA_DIAGLIB_WAS_STARTED_SEC_CONST
	#endif
	#define BSW_STOP_SEC_DEFAULT_CONST_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CONST_UNSPECIFIED
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_STOP_SEC_CONST
#elif defined RBA_DIAGLIB_START_SEC_CODE
	#if defined RBA_DIAGLIB_WAS_STARTED_SEC_CODE
		#error RBA_DIAGLIB_WAS_STARTED_SEC_CODE already defined
	#endif
	#define BSW_START_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_START_SEC_CODE
	#define RBA_DIAGLIB_WAS_STARTED_SEC_CODE
#elif defined RBA_DIAGLIB_STOP_SEC_CODE
	#if ! defined RBA_DIAGLIB_WAS_STARTED_SEC_CODE
		#error RBA_DIAGLIB_STOP_SEC_CODE without START_SEC
	#else
		#undef RBA_DIAGLIB_WAS_STARTED_SEC_CODE
	#endif
	#define BSW_STOP_SEC_DEFAULT_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_DEFAULT_CODE
    #undef MEMMAP_ERROR
	#undef RBA_DIAGLIB_STOP_SEC_CODE
#elif defined RBA_DIAGLIB_START_SEC_VAR_SAVED_ZONE
    #if defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_SAVED_ZONE
        #error RBA_DIAGLIB_WAS_STARTED_SEC_VAR_SAVED_ZONE already defined
    #endif
    #define BSW_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef RBA_DIAGLIB_START_SEC_VAR_SAVED_ZONE
    #define RBA_DIAGLIB_WAS_STARTED_SEC_VAR_SAVED_ZONE
#elif defined RBA_DIAGLIB_STOP_SEC_VAR_SAVED_ZONE
    #if ! defined RBA_DIAGLIB_WAS_STARTED_SEC_VAR_SAVED_ZONE
        #error RBA_DIAGLIB_STOP_SEC_VAR_SAVED_ZONE without START_SEC
    #else
        #undef RBA_DIAGLIB_WAS_STARTED_SEC_VAR_SAVED_ZONE
    #endif
    #define BSW_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
    #undef MEMMAP_ERROR
    #undef RBA_DIAGLIB_STOP_SEC_VAR_SAVED_ZONE
#elif defined RBA_DIAGLIB_START_SEC_CODE
    #if defined rba_DiagLib_WAS_STARTED_SEC_CODE
        #error rba_DiagLib_WAS_STARTED_SEC_CODE already defined
    #endif
    #define BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_CODE
    #undef RBA_DIAGLIB_MEMMAP_ERROR
    #undef RBA_DIAGLIB_START_SEC_CODE
    #define rba_DiagLib_WAS_STARTED_SEC_CODE
#elif defined RBA_DIAGLIB_STOP_SEC_CODE
    #if ! defined rba_DiagLib_WAS_STARTED_SEC_CODE
        #error rba_DiagLib_WAS_STARTED_SEC_CODE without START_SEC
    #else
        #undef rba_DiagLib_WAS_STARTED_SEC_CODE
    #endif
    #define BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_CODE
    #undef RBA_DIAGLIB_MEMMAP_ERROR
    #undef RBA_DIAGLIB_STOP_SEC_CODE
/* Added for the memory section generated by the RTE */
#elif defined rba_DiagLib_START_SEC_CODE
    #if defined rba_DiagLib_WAS_STARTED_SEC_CODE
        #error rba_DiagLib_WAS_STARTED_SEC_CODE already defined
    #endif
    #define BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_START_SEC_CODE
    #undef RBA_DIAGLIB_MEMMAP_ERROR
    #undef rba_DiagLib_START_SEC_CODE
    #define rba_DiagLib_WAS_STARTED_SEC_CODE
/* Added for the memory section generated by the RTE */
#elif defined rba_DiagLib_STOP_SEC_CODE
    #if ! defined rba_DiagLib_WAS_STARTED_SEC_CODE
        #error rba_DiagLib_WAS_STARTED_SEC_CODE without START_SEC
    #else
        #undef rba_DiagLib_WAS_STARTED_SEC_CODE
    #endif
    #define BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSW_STOP_SEC_CODE
    #undef RBA_DIAGLIB_MEMMAP_ERROR
    #undef rba_DiagLib_STOP_SEC_CODE
#elif defined MEMMAP_ERROR
    #error "rba_DiagLib_MemMap.h, wrong #pragma command"
#endif

