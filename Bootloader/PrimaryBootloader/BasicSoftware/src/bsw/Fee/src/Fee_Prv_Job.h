

/*
 * The Job unit stores the received orders in internal job slots.
 * The main function will poll jobs from the Job unit and inform the Job unit if a job is finished.
 */

#ifndef FEE_PRV_JOB_H
#define FEE_PRV_JOB_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
# if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

#include "Fee_Prv_Config.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

typedef enum
{
    FEE_PRV_JOBTYPE_NONE_E = 0,
    FEE_PRV_JOBTYPE_READ_E,
    FEE_PRV_JOBTYPE_WRITE_E,
    FEE_PRV_JOBTYPE_INVALIDATE_E,
    FEE_PRV_JOBTYPE_BLOCKMAINTENANCE_E,
    FEE_PRV_JOBTYPE_TRIGGERREORG_E,
    FEE_PRV_JOBTYPE_ENTER_STOP_MODE_E,
    FEE_PRV_JOBTYPE_MAX_E
} Fee_Prv_JobType_ten;

/*
 * Every job is stored in such a job struct.
 * There are two job slots, one for the NVM and one for an adapter.
 */
typedef struct
{
    Fee_Prv_JobType_ten             type_en;                /* Read/Write/Invalidate job                                             */
    uint8                        *  bfr_pu8;                /* Pointer to a variable buffer - needed for read jobs                   */
    uint8                  const *  bfr_pcu8;               /* Pointer to a constant buffer - needed for write jobs                  */
    uint16                          blockNumber_u16;        /* Blocknumber as defined in Fee_Cfg.h - this is NOT the persistent ID ! */
    uint16                          length_u16;             /* Length of the block operation                                         */
    uint16                          offset_u16;             /* Offset of the block operation                                         */
} Fee_Prv_JobDesc_tst;

/* All jobs and their results */
typedef struct
{
    Fee_Prv_JobDesc_tst             jobs_ast[FEE_PRV_REQUESTER_MAX_E];        /* Current jobs                           */
    MemIf_JobResultType             results_aen[FEE_PRV_REQUESTER_MAX_E];     /* Results of the last jobs               */
    MemIf_Rb_MigrationResult_ten    firstMigrationResult_aen[FEE_NUM_BLOCKS]; /* Migration result of the first read job */
} Fee_Prv_Job_tst;

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"
extern void                         Fee_Prv_JobInit(void);
extern Fee_Prv_JobDesc_tst *        Fee_Prv_JobNext(void);
extern void                         Fee_Prv_JobEnd(Fee_Prv_JobDesc_tst * jobDesc_pst,
                                                   MemIf_JobResultType result_en,
                                                   MemIf_Rb_MigrationResult_ten migrationResult_en);
extern Std_ReturnType               Fee_Prv_JobPut(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint8 apiId_u8, Fee_Prv_JobDesc_tst const * jobDesc_pcst);
extern Fee_Prv_JobType_ten          Fee_Prv_JobGetActive(Fee_Prv_ConfigRequester_ten requester_en);
extern MemIf_JobResultType          Fee_Prv_JobGetResult(Fee_Prv_ConfigRequester_ten requester_en);
extern MemIf_Rb_MigrationResult_ten Fee_Prv_JobGetMigrationResult(uint16 blockNr_u16);
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

# endif

/* FEE_PRV_JOB_H */
#endif
