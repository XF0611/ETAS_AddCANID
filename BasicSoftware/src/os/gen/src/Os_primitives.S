/*
 * ETAS BIP Porting
 */

#define __ASSEMBLY__

#include "arc.h"
#include "arc_feature_config.h"

	.file "Os_primitives.S"

  .section .Os_primitives, "ax"
  .align 4

  .global Os_Initialize_SP
Os_Initialize_SP:
    mov r0, _f_stack
    sr	r0, [AUX_USER_SP]

