

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Fee_Idx_Cfg.h"

#include "Fee_Prv.h"

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Static declarations
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************
*/

/*
 * In the end this shall be a constant "variable" or a define.
 * To better find out the maximal limit that is possible it is currently located in RAM.
 * Here we can change it with the debugger which is important for testing.
 *
 */
#define FEE_START_SEC_CONST_32
#include "Fee_MemMap.h"
uint32 const Fee_Prv_ConfigMaxEffort_cu32 = 1024u;
#define FEE_STOP_SEC_CONST_32
#include "Fee_MemMap.h"

/**
 * \brief   Find the block number (i.e. index) in the block properties table by persistent ID.
 *
 * \param   persistentId_u16    The persistent ID that shall be checked
 *
 * \return  The block number (i.e. index) of the corresponding block in the block properties table - 0xFFFF if unknown.
*/
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
uint16 Fee_Prv_ConfigGetBlockNrByPersistentId(uint16 persistentId_u16)
{
    register uint16_least   a_u16       = 0u;
    register uint16_least   b_u16       = FEE_NUM_BLOCKS;
    register uint16_least   check_u16   = 0u;
    uint16                  result_u16  = 0xFFFFu;

    /*
     * The Fee_Rb_BlockProperties_st struct is stored ascending by persistent ID.
     * This function uses that fact and does a binary search.
     */
    /* Evaluation of while loop: The while loop will be exited after a maximum of FEE_NUM_BLOCKS/2 calls, which is a constant value.
     * Hence, the while loop will not be stuck infinetly. */
    while(a_u16 != b_u16)
    {
        check_u16 = a_u16 + ((b_u16 - a_u16) / 2u);
        if(Fee_Rb_BlockProperties_st[check_u16].BlockPersistentId_u16 < persistentId_u16)
        {
            if(check_u16 == a_u16)
            {
                /*
                 * This can only happen if a_u16 + 1 = b_u16.
                 * Either b_u16 was once checked and was not index of the searched persistent ID
                 * or b_u16 is still = FEE_NUM_BLOCKS (which is 1 bigger than the last valid index of Fee_Rb_BlockProperties_st)
                 * and the searched persistent ID is bigger than the biggest configured persistent ID.
                 * So if this branch is reached the persistent ID is not known.
                 */
                b_u16 = a_u16;
            }
            else
            {
                a_u16 = check_u16;
            }
        }
        else if(Fee_Rb_BlockProperties_st[check_u16].BlockPersistentId_u16 > persistentId_u16)
        {
            b_u16 = check_u16;
        }
        else
        {
            b_u16       = a_u16;
            result_u16  = (uint16)check_u16;
        }
    }

    return(result_u16);
}

/**
 * \brief   Get the length of a block specified by its block number.
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \return  The length of the specified block
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
uint16 Fee_Prv_ConfigGetBlockLengthByBlockNr(uint16 blockNr_u16)
{
    return(Fee_Rb_BlockProperties_st[blockNr_u16].Length_u16);
}

/**
 * \brief   Get the length of a block specified by its persistent ID.
 *
 * \param   persistentId_u16    Persistent ID of this block
 *
 * \return  The length of the specified block - 0 if the passed persistent ID is unknown.
*/
uint16 Fee_Prv_ConfigGetBlockLengthByPersistentId(uint16 persistentId_u16)
{
    uint16 result_u16   = 0u;
    uint16 index_u16    = Fee_Prv_ConfigGetBlockNrByPersistentId(persistentId_u16);

    if(0xFFFFu != index_u16)
    {
        result_u16 = Fee_Rb_BlockProperties_st[index_u16].Length_u16;
    }

    return(result_u16);
}

/**
 * \brief   Get the persistent ID of a specified block.
 *
 * \param   blockNr_u16 Block number as defined in Fee_Cfg.h
 *
 * \return  Persistent ID of the specified block
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
uint16 Fee_Prv_ConfigGetBlockPersistentIdByBlockNr(uint16 blockNr_u16)
{
    return(Fee_Rb_BlockProperties_st[blockNr_u16].BlockPersistentId_u16);
}

/**
 * \brief   returns the uint16 statusbits of the selected block
 *
 * \param   blockNr_u16 Block number as defined in Fee_Cfg.h
 *
 * \return  status bytes of the selected block
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
uint16 Fee_Prv_ConfigGetBlockStatusBitsByBlockNr(uint16 blockNr_u16)
{
    return Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16;
}

/**
 * \brief   Get the requester of that block. Can be either NvM or adapter.
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  FEE_PRV_REQUESTER_NVM_E     Block is handled by the NvM
 * \retval  FEE_PRV_REQUESTER_ADAPTER_E Block is handled by the adapter
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
Fee_Prv_ConfigRequester_ten Fee_Prv_ConfigGetBlockRequesterByBlockNr(uint16 blockNr_u16)
{
    Fee_Prv_ConfigRequester_ten result_en;
    uint32 requesterId_u32 = (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 & FEE_PRV_CONFIG_REQUESTER_BITS) >> FEE_PRV_CONFIG_REQUESTER_POS;
    if(1u == requesterId_u32)
    {
        result_en = FEE_PRV_REQUESTER_NVM_E;
    }
    else if(2u == requesterId_u32)
    {
        result_en = FEE_PRV_REQUESTER_ADAPTER_E;
    }
    else
    {
        result_en = FEE_PRV_REQUESTER_DEBUG_E;
    }
    return (result_en);
}

/**
 * \brief   Get the job configured job end notification for a specified block.
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \return Function pointer that is called after finishing a job successfully.
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
Fee_Prv_ConfigNotification_tpfn Fee_Prv_ConfigGetJobEndNotification(uint16 blockNr_u16)
{
# if(TRUE == FEE_PRV_CFG_BLK_CBK_END_USED)
    return(Fee_Rb_BlockProperties_st[blockNr_u16].JobEndNotification_pfn);
# else
    (void)blockNr_u16;
    return(NULL_PTR);
# endif
}

/**
 * \brief   Get the job configured job error notification for a specified block.
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \return Function pointer that is called after finishing a job with errors.
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
Fee_Prv_ConfigNotification_tpfn Fee_Prv_ConfigGetJobErrorNotification(uint16 blockNr_u16)
{
# if(TRUE == FEE_PRV_CFG_BLK_CBK_ERR_USED)
    return(Fee_Rb_BlockProperties_st[blockNr_u16].JobErrorNotification_pfn);
# else
    (void)blockNr_u16;
    return(NULL_PTR);
# endif
}

/**
 * \brief   This function returns the start address of the physical sector.
 *
 * \param   phySectNo_u8     Physical sector number for which the start address is required.
 *
 * \return  Start address of the given physical sector.
 *
 * \attention  This is a simple get-function without check if phySectNo_u8 is valid (< FEE_NUM_FLASH_BANKS_AVAILABLE)!
*/
uint32 Fee_Prv_ConfigGetPhysStartAddress (uint8 physSectNr_u8)
{
    return(Fee_Rb_FlashProp_st[physSectNr_u8].Fee_PhysStartAddress_u32);
}

/**
 * \brief   This function returns the end address of the physical sector.
 *
 * \param   phySectNo_u8     Physical sector number for which the end address is required.
 *
 * \return  End address of the given physical sector.
 *
 * \attention  This is a simple get-function without check if phySectNo_u8 is valid (< FEE_NUM_FLASH_BANKS_AVAILABLE)!
*/
uint32 Fee_Prv_ConfigGetPhysEndAddress (uint8 physSectNr_u8)
{
    return(Fee_Rb_FlashProp_st[physSectNr_u8].Fee_PhysEndAddress_u32);
}

/**
 * \brief   This function finds the fee redundant block index for a known block. This is required to know the position
 *          of the block in the redundant cache.
 *
 * \param   natBlockNr_u16     This is the native index for the block for which redundant index has to be returned
 * \param   redBlockNr_pu16    Pointer in which redundant block index is copied.
 *                                Only if E_OK is returned, this variable is altered.
 *
 * \retval  E_OK       The redundant block index was found.
 * \retval  E_NOT_OK   Either an invalid index was passed or an index for non redundant block was passed as parameter.
*/
/* MR12 RULE 8.13 VIOLATION: deactivated function. The parameter is required to be changable if the function is active */
Std_ReturnType Fee_Prv_ConfigGetRedBlockNrByNatBlockNr(uint16 natBlockNr_u16, uint16 * const redBlockNr_pu16)
{
    Std_ReturnType retVal_en;
# if(defined(FEE_PRV_NR_RED_BLOCKS) && (0 != FEE_PRV_NR_RED_BLOCKS))


    uint16_least   idxLowerBoundary_qu16       = 0u;
    uint16_least   idxUpperBloundary_qu16      = FEE_PRV_NR_RED_BLOCKS;
    uint16_least   idxCurrentCheck_qu16        = 0u;
    boolean        blockFound_b = FALSE;

    if (natBlockNr_u16 < FEE_NUM_BLOCKS)
    {
        /*
         * The Fee_Prv_RedBlks_acu16 array is stored ascending by Fee Block Index.
         * This function uses that fact and does a binary search.
         */
        /* Evaluation of while loop: The while loop will be exited after a maximum of FEE_PRV_NR_RED_BLOCKS/2 calls, which is a constant value.
         * Hence, the while loop will not be stuck infinetly. */
        while(idxLowerBoundary_qu16 != idxUpperBloundary_qu16)
        {
            idxCurrentCheck_qu16 = idxLowerBoundary_qu16 + ((idxUpperBloundary_qu16 - idxLowerBoundary_qu16) / 2u);
            if(Fee_Prv_RedBlks_acu16[idxCurrentCheck_qu16] < natBlockNr_u16)
            {
                if(idxCurrentCheck_qu16 == idxLowerBoundary_qu16)
                {
                    /*
                     * There are gaps allowed between the native indexes configured as redundant blocks.
                     * As soon as a native Fee index is not present in the layout,
                     * upper and lower boundary are narrowing down the position the index is missing at.
                     * As soon as the lower is pointing to the position one below and the upper at the following index,
                     * the search has to be aborted and the index is detected as not being in this list
                     */
                    idxUpperBloundary_qu16 = idxLowerBoundary_qu16;
                }
                else
                {
                    idxLowerBoundary_qu16 = idxCurrentCheck_qu16;
                }
            }
            else if(Fee_Prv_RedBlks_acu16[idxCurrentCheck_qu16] > natBlockNr_u16)
            {
                idxUpperBloundary_qu16 = idxCurrentCheck_qu16;
            }
            else
            {
                // return the found redundant index
                *redBlockNr_pu16  = (uint16)idxCurrentCheck_qu16;
                blockFound_b = TRUE;

                // fulfill the loop exit criterion
                idxUpperBloundary_qu16 = idxLowerBoundary_qu16;

            }
        }

        if(TRUE == blockFound_b)
        {
            retVal_en = E_OK;
        }
        else
        {
            /* Block is not redundant block */
            retVal_en = E_NOT_OK;
        }
    }
    else
    {
        /* Invalid index passed */


        retVal_en = E_NOT_OK;
    }
# else
    (void)natBlockNr_u16;
    (void)redBlockNr_pu16;
    retVal_en = E_NOT_OK;
# endif
    return(retVal_en);
}

/**
 * \brief   This function returns the fee native block index for a known block. This is required to know the position
 *          of the block in the native cache.
 *
 * \param   redBlockNr_u16     This is the redundant index for the block for which native index has to be returned
 * \param   natBlockNr_pu16    Pointer in which native block index is copied. When invalid parameter is passed, this variable is untouched.
 *
 * \return  E_OK       The redundant block index was found.
 *          E_NOT_OK   Either an invalid index was passed or an index for non redundant block was passed as parameter.
*/
/* MR12 RULE 8.13 VIOLATION: deactivated function. The parameter is required to be changable if the function is active */
Std_ReturnType Fee_Prv_ConfigGetNatBlockNrByRedBlockNr(uint16 redBlockNr_u16, uint16 * const natBlockNr_pu16)
{
    Std_ReturnType          retVal_en;

# if(defined(FEE_PRV_NR_RED_BLOCKS) && (0 != FEE_PRV_NR_RED_BLOCKS))

    if (redBlockNr_u16 < FEE_PRV_NR_RED_BLOCKS)
    {
        *natBlockNr_pu16 = Fee_Prv_RedBlks_acu16[redBlockNr_u16];
        retVal_en = E_OK;
    }
    else
    {
        /* Invalid index passed */
        retVal_en = E_NOT_OK;
    }

# else
    (void)redBlockNr_u16;
    (void)natBlockNr_pu16;

    retVal_en = E_NOT_OK;
# endif

    return(retVal_en);
}

/**
 * \brief   Check whether a block with the specified persistent ID is known by configuration.
 *
 * \param persistentId_u16  Peristent ID which shall be checked
 *
 * \retval  TRUE    There exists a block in the configuration with the specified persistent ID
 * \retval  FALSE   There does not exist a block in the configuration with the specified persistent ID
*/
boolean Fee_Prv_ConfigIsBlockKnownByPersistentId(uint16 persistentId_u16)
{
    boolean result_b = FALSE;

    if(0xFFFFu != Fee_Prv_ConfigGetBlockNrByPersistentId(persistentId_u16))
    {
        result_b = TRUE;
    }

    return(result_b);
}

/**
 * \brief   Check whether migration feature is active for a block.
 *
 * \param   persistentId_u16 Persistent ID of the specified block
 *
 * \retval  TRUE    migration is active
 * \retval  FALSE   migration is not active, or the passed persistent ID unknown
*/
boolean Fee_Prv_ConfigIsBlockMigrationActiveByPersistentId(uint16 persistentId_u16)
{
   boolean result_b = FALSE;

   uint16 blockNr_u16 = Fee_Prv_ConfigGetBlockNrByPersistentId(persistentId_u16);

   if ( (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 &
           FEE_PRV_CONFIG_FIRST_INIT_MIGRATION_BITS) == FEE_PRV_CONFIG_FIRST_INIT_MIGRATION_BITS
      )

   {
       result_b = TRUE;
   }

   return(result_b);
}

/**
 * \brief   Check whether the block length is variable and calculated on runtime.
 *
 * \param  blockNr_u16 Block number as defined in Fee_Cfg.h
 *
 * \retval  TRUE    block length is variable
 * \retval  FALSE   block length is not variable
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
boolean Fee_Prv_ConfigIsBlockLengthVariableByBlockNr(uint16 blockNr_u16)
{
   boolean result_b = FALSE;

   if ( (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 &
           FEE_PRV_CONFIG_VAR_BLOCK_LENGTH_BITS) == FEE_PRV_CONFIG_VAR_BLOCK_LENGTH_BITS
      )
   {
       result_b = TRUE;
   }

   return(result_b);
}

/**
 * \brief   Does a specified block support writing from an unstable buffer? (For example EnvRam)
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  TRUE    The block does support writing from an unstable buffer
 *                  This means if the flash content is not equal to the RAM content after writing this is not an error
 * \retval  FALSE   The block does not support writing from an unstable buffer
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
boolean Fee_Prv_ConfigIsBlockRobustnessActiveByBlockNr(uint16 blockNr_u16)
{
    boolean result_b = FALSE;

    if(0u != (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 & FEE_PRV_CONFIG_ROBUSTNESS_BITS))
    {
        result_b = TRUE;
    }

    return(result_b);
}

/**
 * \brief   Is a specified block stored twice in write or invalidate order?
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  TRUE    The block is stored twice when writing or invalidating
 * \retval  FALSE   The block is single stored when writing or invalidating
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
boolean Fee_Prv_ConfigIsBlockDoubleStorageActiveByBlockNr(uint16 blockNr_u16)
{
    boolean result_b = FALSE;

    if(0u != (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 & FEE_PRV_CONFIG_DOUBLESTORAGE_BITS))
    {
        result_b = TRUE;
    }

    return(result_b);
}

/**
 * \brief   Shall the block be ignored during flash reorganization even when the software does not know it by configuration?
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  TRUE    The block shall not be ignored during flash reorganization even when the software does not know it by configuration
 * \retval  FALSE   The block shall be ignored during flash reorganization if it is not known by configuration
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
boolean Fee_Prv_ConfigIsBlockSurvivalActiveByBlockNr(uint16 blockNr_u16)
{
    boolean result_b = FALSE;

    if(0u != (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 & FEE_PRV_CONFIG_SURVIVAL_BITS))
    {
        result_b = TRUE;
    }

    return(result_b);
}

/**
 * \brief   Is the block configured to be NoFallback?
 *
 * \param   blockNr_u16     Block number as defined in Fee_Cfg.h - This is not the persistent ID!
 *
 * \retval  TRUE    The block has the NoFallback config active
 * \retval  FALSE   The block doesn't have the NoFallback config active
 *
 * \attention  This is a simple get-function without check if blockNr_u16 is valid (< FEE_NUM_BLOCKS)!
*/
boolean Fee_Prv_ConfigIsBlockNoFallbackActiveByBlockNr(uint16 blockNr_u16)
{
    boolean result_b = FALSE;

    if(0u != (Fee_Rb_BlockProperties_st[blockNr_u16].Flags_u16 & FEE_PRV_CONFIG_NOFALLBACK_BITS))
    {
        // NoFallback block
        result_b = TRUE;
    }

    return (result_b);
}

#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"
#endif
