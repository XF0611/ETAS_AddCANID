/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, OS_ENABLE_FI, OS_DISABLE_FI, OS_ENABLE_F, OS_DISABLE_F, OS_ENABLE_I, OS_DISABLE_I) */
/* OS_ENABLE_FI, OS_DISABLE_FI, OS_ENABLE_F, OS_DISABLE_F, OS_ENABLE_I, OS_DISABLE_I */

/* ETAS BIP Porting */
#include "arc_exception.h"
#include "alps.h"


#ifdef _lint /* Declaring compiler inlines and intrinsics */
  #define OS_READ_AUX_IRQ_CTRL()
#else
  #define OS_READ_AUX_IRQ_CTRL() _arc_aux_read(AUX_IRQ_CTRL)
#endif

/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern FUNC(void, OS_CODE_FAST) Os_Initialize_SP(void); /* Implemented in assembler */
extern FUNC(void, OS_CODE_FAST) Os_SetSP(MemoryStartAddressType s);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
