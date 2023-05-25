
#ifndef NVM_PRV_SERVICE_H
#define NVM_PRV_SERVICE_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Types.h"
#include "NvM_Prv_BlockData.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
/**
 * Structure definition of block data required for a single-block service
 */
typedef struct
{
    NvM_Prv_QueueEntry_tst QueueEntry_st;

    // Result of the requested service
    NvM_RequestResultType Result_uo;
    // Dataset index
    uint8 idxDataset_u8;

    // masks to set new states in stBlock
    uint8 maskBitsToChange_u8;
    uint8 maskBitsNewValue_u8;

} NvM_Prv_BlockData_tst;

/**
 * \typedef
 * Type definition for functions to check parameters used in a single-block service.
 *
 * This type makes it possible to check parameters used in a single-block service in a common way.
 */
typedef boolean (*NvM_Prv_Service_CheckParameter_tpfct)(NvM_Prv_BlockData_tst const* BlockData_pcst);
/**
 * \typedef
 * Type definition for functions to check block data used in a single-block service.
 *
 * This type makes it possible to check block data used in a single-block service in a common way.
 */
typedef boolean (*NvM_Prv_Service_CheckBlockData_tpfct)(NvM_Prv_BlockData_tst const* BlockData_pcst,
                                                        NvM_Prv_BlockErrors_tuo *Errors_puo);
/**
 * \typedef
 * Type definition for functions to set block data for a single-block service.
 *
 * This type makes it possible to set block data for a single-block service in a common way.
 */
typedef void (*NvM_Prv_Service_SetBlockData_tpfct)(NvM_Prv_BlockData_tst const* BlockData_pcst);

/**
 * Structure definition for configuration data required to initiate a single-block service.
 */
typedef struct
{
    /**
     * This configuration element enables/disables the check for pending asynchronous user services
     * while initiating a single-block service.
     * If this check is enabled then a single-block service will be initiated only if no other asynchronous user
     * services are pending for requested block.
     */
    boolean CheckPendingBlock_b;
    /**
     * This configuration element defines function which checks whether all parameters used in a single-block service
     * are valid.
     * \attention
     * The NvM calls this function with disabled scheduler lock.
     */
    NvM_Prv_Service_CheckParameter_tpfct CheckParameter_pfct;
    /**
     * This configuration element defines function which checks whether all block data used in a single-block service
     * is valid.
     * \attention
     * The NvM calls this function with enabled scheduler lock.
     */
    NvM_Prv_Service_CheckBlockData_tpfct CheckBlockData_pfct;
    /**
     * This configuration element defines function which sets all required block data for a single-block service.
     * \attention
     * The NvM calls this function with enabled scheduler lock.
     */
    NvM_Prv_Service_SetBlockData_tpfct SetBlockData_pfct;

} NvM_Prv_Service_Configuration_tst;


/**
 * \typedef
 * Type definition for functions to update data for all blocks participating in a multi-block service.
 *
 * This type makes it possible to set block data for a multi-block service in a common way.
 */
typedef void (*NvM_Prv_Service_UpdateBlockDataMulti_tpfct)(void);
/**
 * \typedef
 * Type definition for functions to finalize a multi-block service.
 *
 * This type makes it possible to finalize a multi-block service in a common way.
 */
typedef void (*NvM_Prv_Service_FinalizeMulti_tpfct)(void);

/**
 * Structure definition for configuration data required to initiate a multi-block service.
 */
typedef struct
{
    /**
     * This configuration element defines function which updates data as required for all blocks participating
     * in a multi-block service.
     *
     * Typically this function updates data for special block NVM_PRV_MULTI_BLOCK used for multi-block services and
     * calls the function NvM_Prv_Block_SetIsNvmEnqueuingForMulti with appropriate parameter,
     * see also description of NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct.
     * In general this function contains all actions which shall be done with enabled scheduler lock.
     *
     * \attention
     * The NvM calls this function with enabled scheduler lock.
     */
    NvM_Prv_Service_UpdateBlockDataMulti_tpfct UpdateBlockDataForMulti_pfct;
    /**
     * This configuration element defines function which executes all actions required to finalize initiation
     * of a multi-block service.
     *
     * Typically this function finds all blocks which participate in required multi-block service and
     * updates block data correspondingly. Also this function calls the function NvM_Prv_Block_SetIsNvmEnqueuingForMulti,
     * see also description of NvM_Prv_Block_IsNvmEnqueuingForMulti_pfct.
     * In general this function contains all actions which can be done with disabled scheduler lock.
     *
     * \attention
     * The NvM calls this function with disabled scheduler lock.
     */
    NvM_Prv_Service_FinalizeMulti_tpfct FinalizeMulti_pfct;

} NvM_Prv_Service_ConfigurationMulti_tst;

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern Std_ReturnType NvM_Prv_Service_Initiate(NvM_Prv_idQueue_tuo idQueue_uo,
                                               NvM_Prv_BlockData_tst const* BlockData_pcst,
                                               NvM_Prv_Service_Configuration_tst const* ServiceConfiguration_pcst);

extern void NvM_Prv_Service_InitiateMulti(NvM_Prv_idQueue_tuo idQueue_uo,
                                          NvM_Prv_QueueEntry_tst const* QueueEntry_pcst,
                                          NvM_Prv_Service_ConfigurationMulti_tst const* ConfigurationMulti_pcst);

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/*
**********************************************************************************************************************
* Inline functions
**********************************************************************************************************************
*/

/* NVM_PRV_SERVICE_H */
#endif
