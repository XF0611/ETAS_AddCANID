/*
 * This is ValidateResource.c, auto-generated for:
 *   Project: ETAS_EcuC_Partitions_AppCore_EcucValues
 *   Target:  TDA4VM-R5-TI
 *   Variant: TDA4VM_MCU_R5FSS0_CORE0
 *   Version: 5.0.3
 * **************************
 *  WARNING
 *   This source code has been generated for debugging and quality assurance
 *   purposes only. It is recommended that all RTA-OS code used in a series
 *   production ECU is generated and compiled using the RTA-OS Generator tool
 *   to ensure consistency of code and compilation options.
 * **************************
 *   
 */
#include "OsMinusConfig.h"
#include "Os_Safe_Cfg.h"
#include "IKernel.h"
/* Code start */
#ifndef _lint  /* Validation code does 'wrong' things with pointers by design. It is not useful to put a lot of lint dismissals here */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

FUNC(boolean, OS_CODE) Os_IsResourceValid(ResourceType ResID)
{
  return (ResID >= &Os_const_resources[0]) && (ResID < &Os_const_resources[OS_NUM_RESOURCES]) && ( 0 == ((((Os_BiggestCommonType)ResID) - ((Os_BiggestCommonType)&Os_const_resources[0])) % sizeof(Os_ResourceType)));
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

#endif /* _lint */
  

