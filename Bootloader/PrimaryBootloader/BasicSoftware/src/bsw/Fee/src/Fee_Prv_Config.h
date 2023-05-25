

#ifndef FEE_PRV_CONFIG_H
#define FEE_PRV_CONFIG_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if (defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

/*
 * The Config unit provides easy access to the configuration of the Fee.
 * With the help of this unit the configuration can be stored in a packed way.
 * Then this unit can read the information can provide it in an easy-to-handle way to the Fee.
*/

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

typedef enum
{
    FEE_PRV_REQUESTER_NVM_E         = 0,
    FEE_PRV_REQUESTER_ADAPTER_E     = 1,
    FEE_PRV_REQUESTER_DEBUG_E       = 2,
    FEE_PRV_REQUESTER_MAX_E         = 3
} Fee_Prv_ConfigRequester_ten;

typedef void(*Fee_Prv_ConfigNotification_tpfn)(void);

typedef struct
{
    uint16  BlockPersistentId_u16;
    uint16  Flags_u16;
    uint16  Length_u16;
#  if(TRUE == FEE_PRV_CFG_BLK_CBK_END_USED)
    Fee_Prv_ConfigNotification_tpfn const JobEndNotification_pfn;
#  endif
#  if(TRUE == FEE_PRV_CFG_BLK_CBK_ERR_USED)
    Fee_Prv_ConfigNotification_tpfn const JobErrorNotification_pfn;
#  endif
} Fee_Rb_BlockPropertiesType_tst;

typedef struct
{
    uint32  Fee_PhysStartAddress_u32;    /* Physical sector: start address */
    uint32  Fee_PhysEndAddress_u32;      /* Physical sector: end address   */
} Fee_Rb_FlashProp_tst;

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#if (FEE_RB_USE_ROM_BLOCKTABLE == TRUE)
#define FEE_START_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
extern const Fee_Rb_BlockPropertiesType_tst  Fee_Rb_BlockProperties_st[FEE_NUM_BLOCKS];
#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
#else
#define FEE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
extern       Fee_Rb_BlockPropertiesType_tst  Fee_Rb_BlockProperties_st[FEE_NUM_BLOCKS];
#define FEE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Fee_MemMap.h"
#endif

#define FEE_START_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"
extern const Fee_Rb_FlashProp_tst         Fee_Rb_FlashProp_st[FEE_NUM_FLASH_BANKS_AVAILABLE];
#define FEE_STOP_SEC_CONST_UNSPECIFIED
#include "Fee_MemMap.h"

#define FEE_START_SEC_CONST_32
#include "Fee_MemMap.h"
extern uint32 const Fee_Prv_ConfigMaxEffort_cu32;
#define FEE_STOP_SEC_CONST_32
#include "Fee_MemMap.h"

#  if(defined(FEE_PRV_NR_RED_BLOCKS) && (0 != FEE_PRV_NR_RED_BLOCKS))
#define FEE_START_SEC_CONST_16
#include "Fee_MemMap.h"
extern uint16 const Fee_Prv_RedBlks_acu16[FEE_PRV_NR_RED_BLOCKS];
#define FEE_STOP_SEC_CONST_16
#include "Fee_MemMap.h"
#  endif

/*
 **********************************************************************************************************************
 * Inline declarations
 **********************************************************************************************************************
*/

LOCAL_INLINE boolean Fee_Prv_ConfigIsReorgUnknown(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsSurvivalActive(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsPreprogramming(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsBlankChkRequired(void);
LOCAL_INLINE Std_ReturnType Fee_Prv_ConfigDummyFunc(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsNoFallbackActive(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsMaintainActive(void);
LOCAL_INLINE boolean Fee_Prv_ConfigIsTriggerReorgActive(void);

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

extern uint16                           Fee_Prv_ConfigGetBlockLengthByBlockNr               (uint16 blockNr_u16);
extern uint16                           Fee_Prv_ConfigGetBlockLengthByPersistentId          (uint16 persistentId_u16);
extern uint16                           Fee_Prv_ConfigGetBlockNrByPersistentId              (uint16 persistentId_u16);
extern uint16                           Fee_Prv_ConfigGetBlockPersistentIdByBlockNr         (uint16 blockNr_u16);
extern uint16                           Fee_Prv_ConfigGetBlockStatusBitsByBlockNr           (uint16 blockNr_u16);
extern Fee_Prv_ConfigRequester_ten      Fee_Prv_ConfigGetBlockRequesterByBlockNr            (uint16 blockNr_u16);
extern Fee_Prv_ConfigNotification_tpfn  Fee_Prv_ConfigGetJobEndNotification                 (uint16 blockNr_u16);
extern Fee_Prv_ConfigNotification_tpfn  Fee_Prv_ConfigGetJobErrorNotification               (uint16 blockNr_u16);
extern uint32                           Fee_Prv_ConfigGetPhysStartAddress                   (uint8 physSectNr_u8);
extern uint32                           Fee_Prv_ConfigGetPhysEndAddress                     (uint8 physSectNr_u8);
extern Std_ReturnType                   Fee_Prv_ConfigGetRedBlockNrByNatBlockNr             (uint16 natBlockNr_u16, uint16 * const redBlockNr_pu16);
extern Std_ReturnType                   Fee_Prv_ConfigGetNatBlockNrByRedBlockNr             (uint16 redBlockNr_u16, uint16 * const natBlockNr_pu16);
extern boolean                          Fee_Prv_ConfigIsBlockMigrationActiveByPersistentId  (uint16 persistentId_u16);
extern boolean                          Fee_Prv_ConfigIsBlockLengthVariableByBlockNr        (uint16 blockNr_u16);
extern boolean                          Fee_Prv_ConfigIsBlockKnownByPersistentId            (uint16 persistentId_u16);
extern boolean                          Fee_Prv_ConfigIsBlockRobustnessActiveByBlockNr      (uint16 blockNr_u16);
extern boolean                          Fee_Prv_ConfigIsBlockDoubleStorageActiveByBlockNr   (uint16 blockNr_u16);
extern boolean                          Fee_Prv_ConfigIsBlockSurvivalActiveByBlockNr        (uint16 blockNr_u16);
extern boolean                          Fee_Prv_ConfigIsBlockNoFallbackActiveByBlockNr      (uint16 blockNr_u16);

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

/*
 **********************************************************************************************************************
 * Inlines
 **********************************************************************************************************************
*/

/**
 * \brief   Shall unknown blocks be reorganized or not by default (CB-Mode yes or no)
 *
 * \retval  FALSE   ASW-Mode:   Do throw away unknown blocks during reorganization (if not survival)
 * \retval  TRUE    CB-Mode:    Do not throw away unknown blocks during reorganization
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsReorgUnknown(void)
{
#if defined(FEE_RB_ACT_UNKNOWN_BLOCK_REORG)
    return (FEE_RB_ACT_UNKNOWN_BLOCK_REORG);
#else
    return (FALSE);
#endif
}

/**
 * \brief   Is the survival feature active or not
 *
 * \retval  FALSE   Survival reorg is disabled
 * \retval  TRUE    Survival reorg is activated
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsSurvivalActive(void)
{
#if defined(FEE_RB_ACT_SURVIVAL_EVAL)
    return (FEE_RB_ACT_SURVIVAL_EVAL);
#else
    return (FALSE);
#endif
}

/**
 * \brief   Does the flash preprogram all cells before erasing (yes or no)
 *
 * \retval  FALSE   No preprogramming before erase
 * \retval  TRUE    Preprogramming before erase. This is the default behavior
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsPreprogramming(void)
{
#if defined(FEE_PRV_CFG_ISPREPROGRAMMING)
    return (FEE_PRV_CFG_ISPREPROGRAMMING);
#else
    return (TRUE);
#endif
}

/**
 * \brief   Is blank check required to verify if the page is programmed or erased
 *
 * \retval  FALSE   blank check not required
 * \retval  TRUE    blank check required
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsBlankChkRequired(void)
{
#if defined(FEE_HEADER_RECHECK_ACTIVE)
    return (FEE_HEADER_RECHECK_ACTIVE);
#else
    return (FALSE);
#endif
}

/**
 * \brief function returning E_NOT_OK for mapping not supported APIs
 * \retval E_NOT_OK  The function always returns E_NOT_OK to signal an errorneous call
 */
LOCAL_INLINE Std_ReturnType Fee_Prv_ConfigDummyFunc(void)
{
    return (E_NOT_OK);
}

/**
 * \brief   Is the NoFallback feature active or inactive
 *
 * \retval  FALSE   NoFallback feature inactive
 * \retval  TRUE    NoFallback feature active
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsNoFallbackActive(void)
{
#if defined(FEE_NO_FALLBACK_ACTIVE)
    return (FEE_NO_FALLBACK_ACTIVE);
#else
    return (FALSE);
#endif
}

/**
 * \brief   Is the maintain functionality to maintain the redundant blocks active
 *
 * \retval  FALSE   Maintain feature inactive
 * \retval  TRUE    Maintain feature active
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsMaintainActive(void)
{
#if defined(FEE_RB_MAINTAIN)
    return (FEE_RB_MAINTAIN);
#else
    return (FALSE);
#endif
}

/**
 * \brief   Is the forced sector reorganisation active or inactive
 *
 * \retval  FALSE   Trigger Reorg feature inactive
 * \retval  TRUE    Trigger Reorg feature active
*/
LOCAL_INLINE boolean Fee_Prv_ConfigIsTriggerReorgActive(void)
{
#if defined(FEE_RB_TRIGGER_REORG)
    return (FEE_RB_TRIGGER_REORG);
#else
    return (FALSE);
#endif
}


# endif
/* FEE_PRV_CONFIG_H */
#endif
