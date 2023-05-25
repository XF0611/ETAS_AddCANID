


#ifndef FEE_RB_TYPES_H
#define FEE_RB_TYPES_H

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/* Error codes for API used for DET module (FEE010) */
#define FEE_E_UNINIT                                 0x01u    /* API service called when module was not initialized */
#define FEE_E_INVALID_BLOCK_NO                       0x02u    /* API service called with invalid block number */
#define FEE_E_INVALID_BLOCK_OFS                      0x03u    /* API service called with invalid block offset */
#define FEE_E_PARAM_POINTER                          0x04u    /* API service called with invalid data pointer */
#define FEE_E_INVALID_DATA_PTR                       FEE_E_PARAM_POINTER  /* Backward Compatibility for AR 4.0 */
#define FEE_E_INVALID_BLOCK_LEN                      0x05u    /* API service called with invalid length information */
#define FEE_E_BUSY                                   0x06u    /* API service called while module still busy */
#define FEE_E_BUSY_INTERNAL                          0x07u    /* API service called while module is busing doing internal management operation */
#define FEE_E_INCOMPATIBLE_VERSIONS                  0x08u    /* Included module versions are incompatible */
#define FEE_E_INIT_FAILED                            0x09u    /* API service called when Fee_Init fails */
#define FEE_E_INVALID_VAR_BLK_LEN_CFG                0xFFu    /* API service called with invalid variable block length configuration */
#define FEE_E_INVALID_MIGRATION_CFG                  0xFEu    /* API service called with invalid block migration configuration */
#define FEE_E_INVALID_BLOCK_CFG                      0xFDu    /* API service called with invalid block configuration */
#define FEE_E_INVALID_DEVICE_NAME                    0xFCu    /* Fee_Rb_Idx_ API service called with invalid device name */

/* API Service IDs used for DET module */
#define FEE_SID_INIT                                 0x00u    /* Service ID for the FEE Init function (unused) */
#define FEE_SID_SETMODE                              0x01u    /* Service ID for the FEE set mode function */
#define FEE_SID_READ                                 0x02u    /* Service ID for the FEE read function */
#define FEE_SID_WRITE                                0x03u    /* Service ID for the FEE write function */
#define FEE_SID_CANCEL                               0x04u    /* Service ID for the FEE job cancel function (unused) */
#define FEE_SID_GETSTATUS                            0x05u    /* Service ID for the FEE get status function (unused) */
#define FEE_SID_GETJOBRESULT                         0x06u    /* Service ID for the FEE get job result function for NvM requests */
#define FEE_SID_INVALIDATE                           0x07u    /* Service ID for the FEE invalidate function */
#define FEE_SID_GETVERSIONINFO                       0x08u    /* Service ID for the FEE get version info function */
#define FEE_SID_ERASEIMMEDIATEBLOCK                  0x09u    /* Service ID for the FEE erase immediate block function (unused) */
#define FEE_SID_JOBENDNOTIFICATION                   0x10u    /* Service ID for the FEE job end notification (unused) */
#define FEE_SID_JOBERRORNOTIFICATION                 0x11u    /* Service ID for the FEE job error notification (unused) */
#define FEE_SID_MAINFUNCTION                         0x12u    /* Service ID for the FEE main function (unused) */
#define FEE_SID_RB_GET_WORKING_STATE                 0xF2u    /* Service ID for the FEE get working state API */
#define FEE_SID_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES 0xF3u    /* Service ID for the FEE get free space and free FAT entries API */
#define FEE_SID_RB_END_INIT                          0xF4u    /* Service ID for the FEE end init function */
#define FEE_SID_RB_GET_NR_FREE_BYTES                 0xF5u    /* Service ID for the FEE get free space API */
#define FEE_SID_RB_GET_SECT_CHNG_CNT                 0xF6u    /* Service ID for the FEE get sector change counter API */
#define FEE_SID_RB_ENTER_STOP_MODE                   0xF7u    /* Service ID for the FEE stop mode function */
#define FEE_SID_RB_TRIGGER_REORG                     0xF8u    /* Service ID for the FEE forced sector reorganisation */
#define FEE_SID_RB_ENABLE_BG                         0xF9u    /* Service ID for the FEE enable background operations function */
#define FEE_SID_RB_DISABLE_BG                        0xFAu    /* Service ID for the FEE disable background operations function */
#define FEE_SID_RB_MAINTAIN                          0xFBu    /* Service ID for the FEE maintenance function */
#define FEE_SID_RB_ADAPTERGETJOBRESULT               0xFCu    /* Service ID for the FEE get job result function for adapter layer requests */
#define FEE_SID_RB_VARLENWRITE                       0xFDu    /* Service ID for the FEE Rb variable length write function */
#define FEE_SID_RB_VARLENREAD                        0xFEu    /* Service ID for the FEE Rb variable length read function */
#define FEE_SID_RB_GETMIGRATIONRESULT                0xFFu    /* Service ID for the FEE to read the migration result */

/*
 ***************************************************************************************************
 * Type definition and enums
 ***************************************************************************************************
 */

/* Global Fee_MainFunction state machine type
 * Necessary in public header because it is needed by the Mx17 Adapter for the Eep_GetState function */
typedef enum
{
    FEE_RB_IDLE_E,                   /* Nothing to do, check order queue */
    FEE_RB_WRITE_MODE_E,             /* A write order is currently being proceeded */
    FEE_RB_READ_MODE_E,              /* A read order is currently being proceeded */
    FEE_RB_INVALIDATE_MODE_E,        /* An invalidation is requested */
    FEE_RB_MAINTAIN_MODE_E,          /* An block maintenance is requested */
    FEE_RB_SOFT_SECTOR_REORG_MODE_E, /* Perform a sector reorganization in the background */
    FEE_RB_HARD_SECTOR_REORG_MODE_E, /* Perform a sector reorganization without allowing interruptions */
    FEE_RB_SECTOR_ERASE_E,           /* The sector will be erased */
    FEE_RB_STOPMODE_E                /* Stop mode requested */
}Fee_Rb_WorkingStateType_ten;

/* Possible order types */
/* Note: To be removed when Fs1 is removed - keep it now for compatibility */
typedef enum
{
    FEE_NO_ORDER = 0,           /* there is no order active */
    FEE_READ_ORDER,             /* order-entry belongs to an read job */
    FEE_WRITE_ORDER,            /* order-entry belongs to an write job */
    FEE_INVALIDATE_ORDER,       /* order-entry belongs to an invalidate job  */
    FEE_MAINTAIN_ORDER,         /* order-entry belongs to an maintenance job */
    FEE_FORCED_READ_ORDER       /* order-entry belongs to an forced read job */
}Fee_HlMode_ten;

typedef struct
{
    uint8 dummy_u8;
} Fee_ConfigType;// Postcompile configuration is not supported; the Fee_Init always has to get a NULL_PTR

/* FEE_RB_TYPES_H */
#endif
