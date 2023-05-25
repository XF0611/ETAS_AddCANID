/*
 * This is _ioc_memcpy.c, auto-generated for:
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
 */
#include "OsMinusConfig.h"
#include "IKernel.h"
/* Code start */
#include "OsIoc.h"

/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */

/* IOC local copy - can be overridden at link time by user-optimized code */
FUNC(void, OS_CODE) Os_ioc_memcpy(void *dest, const void *source, uint32 length) {
  uint8 *udest = (uint8 *)dest; /* [MISRA 2012 Rule 11.5] */ /*lint !e9079 !e9087 !e923 */
  const uint8 *usrc  = (const uint8 *)source; /* [MISRA 2012 Rule 11.5] */ /*lint !e9079 !e9087 !e923 */
  while (length != 0U) {
    *udest = *usrc;
    udest++;
    usrc++;
    length--;
  }
}
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */




