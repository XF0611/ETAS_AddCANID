/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */


/* <component>_MemMap.h for AUTOSAR Memory Mapping R4.0 Rev 3 */
/* MISRA RULE 19.15 VIOLATION: MemMap header concept - no protection against repeated inclusion intended */
/**/
#define MEMMAP_ERROR
/*
mappings of modules sections into project sections and
statements for switching the project sections
*/
/* MR12 RULE 4.10, 20.5 VIOLATION:  4.10: Memmap file must be included multiple times,  20.5: AUTOSAR MemMap concept requires #undef, AUTOSAR MemMap requirements are incompatible to MISRA */

#if defined RBA_BSWSRV_START_SEC_CODE
#undef RBA_BSWSRV_START_SEC_CODE
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_CODE
#undef RBA_BSWSRV_STOP_SEC_CODE
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef RBA_BSWSRV_START_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef RBA_BSWSRV_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_CONST_UNSPECIFIED
#undef RBA_BSWSRV_START_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_CONST_UNSPECIFIED
#undef RBA_BSWSRV_STOP_SEC_CONST_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_INIT_16
#undef RBA_BSWSRV_START_SEC_VAR_INIT_16
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_INIT_16
#undef RBA_BSWSRV_STOP_SEC_VAR_INIT_16
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_INIT_32
#undef RBA_BSWSRV_START_SEC_VAR_INIT_32
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_INIT_32
#undef RBA_BSWSRV_STOP_SEC_VAR_INIT_32
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_INIT_8
#undef RBA_BSWSRV_START_SEC_VAR_INIT_8
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_INIT_8
#undef RBA_BSWSRV_STOP_SEC_VAR_INIT_8
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_INIT_BOOLEAN
#undef RBA_BSWSRV_START_SEC_VAR_INIT_BOOLEAN
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_INIT_BOOLEAN
#undef RBA_BSWSRV_STOP_SEC_VAR_INIT_BOOLEAN
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_INIT_UNSPECIFIED
#undef RBA_BSWSRV_START_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef RBA_BSWSRV_STOP_SEC_VAR_INIT_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef RBA_BSWSRV_START_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef RBA_BSWSRV_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_START_SEC_VAR_SAVED_ZONE_8
#undef RBA_BSWSRV_START_SEC_VAR_SAVED_ZONE_8
#undef MEMMAP_ERROR

#elif defined RBA_BSWSRV_STOP_SEC_VAR_SAVED_ZONE_8
#undef RBA_BSWSRV_STOP_SEC_VAR_SAVED_ZONE_8
#undef MEMMAP_ERROR
#endif
#ifdef MEMMAP_ERROR
#error "rba_BswSrv_MemMap.h, wrong pragma command"
#endif
