
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "NvM.h"

#include "MemIf.h"
// TRACE[NVM089] Check version compatibility of included header files
#if (!defined(MEMIF_AR_RELEASE_MAJOR_VERSION) || (MEMIF_AR_RELEASE_MAJOR_VERSION != NVM_AR_RELEASE_MAJOR_VERSION))
# error "AUTOSAR major version undefined or mismatched"
#endif

#include "NvM_Prv.h"
#include "NvM_Prv_BlockData.h"
#include "NvM_Prv_ErrorDetection.h"

#if (NVM_RB_AUX_INTERFACE == STD_ON)

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"
/**
 * TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3027]
 * Special handling for aux migration result
 *
 * Following request results are defined:
 * NVM_RB_MIGRATION_RESULT_INIT_E              (0) =   Initialization value, result not yet available
 * NVM_RB_MIGRATION_RESULT_NOT_NECESSARY_E     (1) =   RAM size == size on medium
 * NVM_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E   (2) =   RAM size < size on medium (remaining data discarded)
 * NVM_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E    (3) =   RAM size > size on medium (remaining data filled with zero)
 * NVM_RB_MIGRATION_RESULT_NOT_DONE_E          (4) =   Migration not done, because data were not read
 * NVM_RB_MIGRATION_RESULT_DEACTIVATED_E       (5) =   Migrations is deactivated by configuration
 */
static NvM_Rb_MigrationResult_ten NvM_Prv_AuxMigrationResult_en;
#define NVM_STOP_SEC_VAR_CLEARED_8
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

Std_ReturnType NvM_Rb_AuxGetMigrationResult(NvM_Rb_MigrationResult_ten *MigrationResultPtr)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3053] Service to read the auxiliary migration result.
    Std_ReturnType stReturn_uo = E_NOT_OK;

    // TRACE[NVM610] Report the DET error NVM_E_NOT_INITIALIZED when the NVM is not yet initialized
    if (NvM_Prv_ErrorDetection_IsNvmInitialized(NVM_SERVICE_ID_RB_AUX_GET_MIGRATION_RESULT, 0))
    {
        // TRACE[NVM612] Report the DET error NVM_E_PARAM_DATA when a NULL pointer is passed
        //               via the parameter RequestResultPtr
        NvM_Prv_ErrorDetection_Ptr_tun ptr_un;
        ptr_un.ptrMigrationResult_pen = MigrationResultPtr;
        if (NvM_Prv_ErrorDetection_IsPtrValid(NVM_SERVICE_ID_RB_AUX_GET_MIGRATION_RESULT,
                                              0,
                                              NVM_E_PARAM_DATA,
                                              &ptr_un))
        {
            // Parameters are valid
            // Read out the selected block specific migration state from the administrative block
            // No SchM lock can and should be done here to allow calling this function from NvM callbacks without deadlocks

            *MigrationResultPtr = NvM_Prv_AuxMigrationResult_en;

            stReturn_uo = E_OK;
        }
    }
    // Return whether this operation was successful
    return stReturn_uo;
}

/**
 * \brief
 * This function initializes the migration result for auxiliary requests.
 */
void NvM_Prv_InitializeAuxMigrationResult(void)
{
    NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_INIT_E;
}

/**
 * \brief
 * This function sets the migration result for a given block used in an auxiliary request.
 * \details
 * This functions reads the migration result only after an auxiliary read because the block migration will be done
 * by lower layers only during a read request.
 * After reading the migration result from lower layers this function maps MemIf result to NvM result which can be
 * returned to the user.
 * The main function calls this function after finishing an auxiliary request.
 *
 * \param idService_uo
 * Id of the finished auxiliary service
 * \param idBlock_uo
 * ID of the block used in auxiliary request
 */
void NvM_Prv_SetAuxMigrationResult(NvM_Prv_idService_tuo idService_uo, NvM_BlockIdType idBlock_uo)
{
    // TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3027] Calculate aux migration result
    MemIf_Rb_MigrationResult_ten migrationResult_en;

    // Check if a aux read job is active
    if (NVM_SERVICE_ID_RB_AUX_READ_BLOCK == idService_uo)
    {
        migrationResult_en = MemIf_Rb_GetMigrationResult(NvM_Prv_GetIdxDevice(idBlock_uo),
                                                         NvM_Prv_GetIdBlockMemIf(idBlock_uo));

        switch(migrationResult_en)
        {
            case MEMIF_RB_MIGRATION_RESULT_INIT_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_INIT_E;
            break;

            case MEMIF_RB_MIGRATION_RESULT_NOT_NECESSARY_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_NOT_NECESSARY_E;
            break;

            case MEMIF_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_TO_SMALLER_SIZE_E;
            break;

            case MEMIF_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_TO_BIGGER_SIZE_E;
            break;

            case MEMIF_RB_MIGRATION_RESULT_NOT_DONE_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_NOT_DONE_E;
            break;

            case MEMIF_RB_MIGRATION_RESULT_DEACTIVATED_E:
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_DEACTIVATED_E;
            break;

            default:
                // Actually default can not be reached so we set here the initial value for migration result
                NvM_Prv_AuxMigrationResult_en = NVM_RB_MIGRATION_RESULT_INIT_E;
            break;
        }
    }
}

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

#endif
