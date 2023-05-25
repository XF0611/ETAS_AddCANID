

#include "Fee.h"
#include "Fee_Cfg.h"
#include "Fee_Prv.h"



/* Properties of flash sectors */
#define FEE_START_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
#if (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED))
const Fee_FlashProp_tst Fee_FlashProp_st[FEE_NUM_FLASH_BANKS_AVAILABLE] = {
#else
const Fee_Rb_FlashProp_tst Fee_Rb_FlashProp_st[FEE_NUM_FLASH_BANKS_AVAILABLE] = {
#endif
    { FEE_PHYS_SEC_START0, FEE_PHYS_SEC_END0, FEE_LOG_SEC_START0, FEE_LOG_SEC_END0 }, /* sector 0 */
    { FEE_PHYS_SEC_START1, FEE_PHYS_SEC_END1, FEE_LOG_SEC_START1, FEE_LOG_SEC_END1 }, /* sector 1 */
};
#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"


#if (FEE_RB_USE_ROM_BLOCKTABLE == TRUE)
#define FEE_START_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
#if (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED))
const Fee_BlockPropertiesType_tst Fee_BlockProperties_st[FEE_NUM_BLOCKS] = {
#else
const Fee_Rb_BlockPropertiesType_tst Fee_Rb_BlockProperties_st[FEE_NUM_BLOCKS] = {
#endif
#else
#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
#if (defined(FEE_CFG_FEE1_ENABLED) && (TRUE == FEE_CFG_FEE1_ENABLED))
Fee_BlockPropertiesType_tst Fee_BlockProperties_st[FEE_NUM_BLOCKS] = {
#else
Fee_Rb_BlockPropertiesType_tst Fee_Rb_BlockProperties_st[FEE_NUM_BLOCKS] = {
#endif
#endif

             /* FeeRbBlockPersistentId,  StatusFlags,  BlkLength ,  FeeRbCallbackEnd    ,  FeeRbCallbackError  */
     /*   0 */{         0x0CBC         ,   0x0100   ,   0x001B   ,NULL_PTR              ,NULL_PTR               } /* NV_BootSoftwareFingerprintBlock          */,
     /*   1 */{         0x1214         ,   0x0100   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* NV_PBLValidFlagBlock                     */,
     /*   2 */{         0x40B1         ,   0x0100   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* NV_ApplicationValidFlagBlock             */,
     /*   3 */{         0x6ABE         ,   0x0100   ,   0x001B   ,NULL_PTR              ,NULL_PTR               } /* NV_ApplicationDataFingerprintBlock       */,
     /*   4 */{         0x74BC         ,   0x0100   ,   0x001B   ,NULL_PTR              ,NULL_PTR               } /* NV_ApplicationSoftwareFingerprintBlock   */,
     /*   5 */{         0x7605         ,   0x0100   ,   0x0003   ,NULL_PTR              ,NULL_PTR               } /* NV_PreErasedBlock                        */,
     /*   6 */{         0x7FBB         ,   0x0100   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* NV_ExternalReprogrammingRequestFlagBlock */,
};

#if (FEE_RB_USE_ROM_BLOCKTABLE == TRUE)
#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
#else
#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
#endif

#define FEE_START_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
/* Linked functions of the Fee */
const Fee_LinkedFunctions_tst Fee_LinkedFunctions_cst = 
{
    NULL_PTR
};
#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"



