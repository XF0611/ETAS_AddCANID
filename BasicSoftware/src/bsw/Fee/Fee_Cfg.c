

#include "Fee.h"
#include "Fee_Cfg.h"
#include "Fee_Prv.h"
#include "NvM_Prv_SizeRamMirror.h" 



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
     /*   0 */{         0x0230         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_4                       */,
     /*   1 */{         0x03B1         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_5                       */,
     /*   2 */{         0x0A3F         ,   0x0100   ,   0x0180   ,NULL_PTR              ,NULL_PTR               } /* NvBlock_NvBlockDescriptor_StoreImmediateBlock */,
     /*   3 */{         0x103A         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_7                       */,
     /*   4 */{         0x11BB         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_6                       */,
     /*   5 */{         0x24A7         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_1                       */,
     /*   6 */{         0x2526         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_0                       */,
     /*   7 */{         0x36AD         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_2                       */,
     /*   8 */{         0x372C         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_3                       */,
     /*   9 */{         0x43AB         ,   0x0100   ,   0x0400   ,NULL_PTR              ,NULL_PTR               } /* NvBlock_NvBlockDescriptor_StoreCyclicBlock */,
     /*  10 */{         0x4AA1         ,   0x0100   ,  ( DEM_SIZEOF_VAR(Dem_GenericNvData) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_DEM_GENERIC_NV_DATA               */,
     /*  11 */{         0x4D1D         ,   0x0100   ,   0x0080   ,NULL_PTR              ,NULL_PTR               } /* NvMBlock_LogisticData                    */,
     /*  12 */{         0x5106         ,   0x0100   ,  ( DEM_SIZEOF_VAR(Dem_AllEventsStatusByte) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVT_STATUSBYTE                    */,
     /*  13 */{         0x6A8B         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_9                       */,
     /*  14 */{         0x6B0A         ,   0x0100   ,  ( DEM_SIZEOF_TYPE(Dem_EvMemEventMemoryType) + 0)   ,NULL_PTR              ,NULL_PTR               } /* NVM_ID_EVMEM_LOC_8                       */,
     /*  15 */{         0x7FBB         ,   0x0100   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* NV_ExternalReprogrammingRequestFlagBlock */,
     /*  16 */{         0x99E9         ,   0x0101   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* NvMBlockDescriptor_ECUConfiguration      */,
     /*  17 */{         0xA72E         ,   0x0100   ,   0x0060   ,NULL_PTR              ,NULL_PTR               } /* NvBlock_NvBlockDescriptor_StoreAtShutdownBlock */,
     /*  18 */{         0xF2F7         ,   0x0100   ,   0x0004   ,NULL_PTR              ,NULL_PTR               } /* ECUM_CFG_NVM_BLOCK                       */,
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



