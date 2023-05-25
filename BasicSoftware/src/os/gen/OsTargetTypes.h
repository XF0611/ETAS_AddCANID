
#include "Std_Types.h"

#define OS_VOLATILE volatile
/* [MISRA 2012 Dir 4.9] */ /*lint -estring(9026, CAT1_ISR, OS_MAIN, OS_EI, OS_DI, OS_NOP, OS_CLZ, OS_GETCPSR, OS_SETCPSR, OS_ISB, OS_DSB, OS_DMB, OS_READ_COPROC, OS_WRITE_COPROC, OS_READ_AUX_IRQ_CTRL, Os_SVC0) */
/* CAT1_ISR, OS_MAIN, OS_EI, OS_DI, OS_NOP, OS_CLZ, OS_ISB, OS_DSB, OS_DMB, OS_READ_COPROC, OS_WRITE_COPROC, OS_READ_AUX_IRQ_CTRL, Os_SVC0 */


#define CAT1_ISR(x) void x(void) /* [$TgtCore 55] */
typedef int Os_main_int;
#define OS_MAIN() extern Os_main_int main(void); Os_main_int main(void) /* [$TgtCore 56] */

#ifdef _lint /* Declaring compiler inlines and intrinsics */
  //ETAS BIP Porting TODO
#else
  #define OS_EI()  arc_unlock() /* ETAS BIP Porting */
  #define OS_DI()  arc_lock()   /* ETAS BIP Porting */

  /* Using compiler built-ins */
  #define OS_NOP _nop       /* ETAS BIP Porting */
  #define OS_ISB _arc_sync  /* ETAS BIP Porting */
  #define OS_DSB _arc_sync  /* ETAS BIP Porting */
  #define OS_DMB _arc_sync  /* ETAS BIP Porting */
  #define OS_CLZ(val)       /* ETAS BIP Porting __clz((uint32)(val)) */
  #define OS_SWI_USER()   { asm(" mov %r3, 1"); _arc_swi();}   /* ETAS BIP Porting */
  #define OS_SWI_KERNEL() { asm(" mov %r3, 0"); _arc_swi();}   /* ETAS BIP Porting */
#endif

typedef uint32 Os_imaskType;
typedef uint32 Os_tmaskType;
typedef uint32 Os_ISRSourceType;  /* For InterruptSource APIs */





/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern FUNC(void, OS_CODE) Os_InitializeVectorTable(void);
extern FUNC(void, OS_CODE) Os_EnableSourceMask(const Os_ISRSourceType * const maskInfo);
extern FUNC(void, OS_CODE) Os_DisableSourceMask(const Os_ISRSourceType * const maskInfo);
extern FUNC(void, OS_CODE) Os_ClearPendingSource(const Os_ISRSourceType * const maskInfo);
extern FUNC(boolean, OS_CODE) Os_IsDisabled(const Os_ISRSourceType * const maskInfo);
extern FUNC(Os_imaskType, OS_CODE) Os_SetImask(Os_imaskType imask);
extern FUNC(Os_imaskType, OS_CODE) Os_SetImaskDis(Os_imaskType imask);
extern FUNC(void, OS_CODE) Os_SetImaskLowered(Os_imaskType imask);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern FUNC(void, OS_CODE_FAST) Os_FIQHandler(void);
extern FUNC(void, OS_CODE_FAST) Os_IRQHandler(void);
extern FUNC(void, OS_CODE_FAST) Os_ISRWrapper(Os_imaskType previous_imask, uint32 isr_index, uint32 channel);
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_CODE_FAST
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
/* ------------------------------------------------- */
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
extern OS_VOLATILE VAR(Os_imaskType, OS_VAR_NO_INIT) Os_CurrentImask;
/* [MISRA 2012 Rule 20.1] */ /*lint -save -estring(9019, *) */
#define OS_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Os_MemMap.h" /* [MISRA 2012 Dir 4.10] */ /*lint !e537 !e451 */
/*lint -restore */
