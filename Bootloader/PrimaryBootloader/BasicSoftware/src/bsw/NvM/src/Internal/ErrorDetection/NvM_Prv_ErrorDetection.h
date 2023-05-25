
#ifndef NVM_PRV_ERRORDETECTION_H
#define NVM_PRV_ERRORDETECTION_H
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "NvM_Prv_Job_Types.h"
#include "NvM_Prv_Queue_Types.h"
#include "NvM_Prv_BlockData.h"
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))
# include "NvM_Prv_Crypto_ServiceTypes.h"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
// Macro used to initialize the local DET error ID variable used in APIs
#define NVM_PRV_NO_DET_ERROR (0u)

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */
typedef union
{
    void*                       ptrToCheck_pv;
    uint8*                      ptrDataIdx_pu8;
    NvM_RequestResultType*      ptrRequestResult_puo;
    NvM_Rb_MigrationResult_ten* ptrMigrationResult_pen;
    NvM_Rb_StatusType*          ptrStatusType_pen;
    uint16*                     ptrBlockLength_pu16;
    NvM_Prv_idService_tuo*      ptrIdService_puo;
    NvM_BlockIdType*            ptrIdBlock_puo;
    Std_VersionInfoType*        ptrVersionInfo_pst;
    uint32*                     ptrCntrWriteBlock_puo;

} NvM_Prv_ErrorDetection_Ptr_tun;

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */
#define NVM_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include "NvM_MemMap.h"

// ID of the most previous error reported via Det_ReportError within NvM (initialized to NVM_PRV_NO_DET_ERROR)
extern NvM_Prv_idDetError_tuo NvM_Prv_idLastDetError_uo;
// ID of the service which triggered the last DET error (initialized to NVM_SERVICE_ID_INIT)
extern NvM_Prv_idService_tuo NvM_Prv_idServiceLastDetError_uo;
// TRACE[BSW_SWS_AR4_0_R2_NVRAMManager_Ext-3113] Private variable to store block ID used in NvM service
//                                               which has caused the last DET error
extern NvM_BlockIdType NvM_Prv_idBlockLastDetError_uo;

#define NVM_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include "NvM_MemMap.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */
#define NVM_START_SEC_CODE
#include "NvM_MemMap.h"

extern void NvM_Prv_ErrorDetection_InitializeDetError(boolean isSavedZoneDataLost_b);
extern void NvM_Prv_ErrorDetection_InitializeProductionErrors(void);
extern void NvM_Prv_ErrorDetection_SetProductionError(NvM_BlockIdType idBlock_uo, uint8 MaskErrorBit_u8);
extern boolean NvM_Prv_ErrorDetection_IsServiceBitValid(NvM_Prv_idService_tuo idService_uo,
                                                        NvM_BlockIdType idBlock_uo,
                                                        NvM_Prv_ServiceBit_tuo ServiceBit_uo);
extern boolean NvM_Prv_ErrorDetection_IsJobIdValid(NvM_Prv_idService_tuo idService_uo,
                                                   NvM_BlockIdType idBlock_uo,
                                                   NvM_Prv_idJob_ten idJob_en);
extern boolean NvM_Prv_ErrorDetection_IsJobResultMemIfPlausible(NvM_Prv_idService_tuo idService_uo,
                                                                NvM_BlockIdType idBlock_uo,
                                                                NvM_Prv_JobResult_ten JobResult_en);
extern boolean NvM_Prv_ErrorDetection_IsBlockSizeValidForExplicitSync(NvM_Prv_idService_tuo idService_uo,
                                                                      NvM_BlockIdType idBlock_uo,
                                                                      uint32 SizeRamMirror_u32);
extern boolean NvM_Prv_ErrorDetection_IsPtrValid(NvM_Prv_idService_tuo idService_uo,
                                                 NvM_BlockIdType idBlock_uo,
                                                 NvM_Prv_idDetError_tuo idDetError_uo,
                                                 NvM_Prv_ErrorDetection_Ptr_tun const* ptr_pcun);
extern boolean NvM_Prv_ErrorDetection_IsNvmInitialized(NvM_Prv_idService_tuo idService_uo,
                                                       NvM_BlockIdType idBlock_uo);
extern boolean NvM_Prv_ErrorDetection_IsBlockIdValid(NvM_Prv_idService_tuo idService_uo,
                                                     NvM_BlockIdType idBlock_uo,
                                                     boolean isMultiBlockAllowed_b);
extern boolean NvM_Prv_ErrorDetection_IsDefaultDataAvailable(NvM_Prv_idService_tuo idService_uo,
                                                             NvM_BlockIdType idBlock_uo);
extern boolean NvM_Prv_ErrorDetection_IsRamBlockAddressValid(NvM_Prv_idService_tuo idService_uo,
                                                             NvM_BlockIdType idBlock_uo,
                                                             void const* RamBlockAddress_pv);
extern boolean NvM_Prv_ErrorDetection_IsBlockTypeDataset(NvM_Prv_idService_tuo idService_uo,
                                                         NvM_BlockIdType idBlock_uo);
extern boolean NvM_Prv_ErrorDetection_IsBlockIdxValid(NvM_Prv_idService_tuo idService_uo,
                                                      NvM_BlockIdType idBlock_uo,
                                                      uint8 idxData_u8);
extern boolean NvM_Prv_ErrorDetection_IsBlockPriorityImmediate(NvM_Prv_idService_tuo idService_uo,
                                                               NvM_BlockIdType idBlock_uo);
extern boolean NvM_Prv_ErrorDetection_IsBlockWriteProtectionChangeable(NvM_Prv_idService_tuo idService_uo,
                                                                       NvM_BlockIdType idBlock_uo);
extern void NvM_Prv_ErrorDetection_ReportReentrantError(NvM_Prv_idService_tuo idService_uo);

extern void NvM_Prv_ErrorDetection_ReportServiceInitiationErrors(NvM_Prv_idService_tuo idService_uo,
                                                                 NvM_BlockIdType idBlock_uo,
                                                                 NvM_Prv_BlockErrors_tuo Errors_uo);
#if (defined(NVM_CRYPTO_USED) && (NVM_CRYPTO_USED == STD_ON))

extern boolean NvM_Prv_ErrorDetection_IsReadPersistantIdConsistent(NvM_Prv_idService_tuo idService_uo,
                                                                   NvM_BlockIdType idBlock_uo,
                                                                   uint16 ReadPersistantId_u16);

extern boolean NvM_Prv_ErrorDetection_IsCryptoResultPlausible(NvM_Prv_idService_tuo idService_uo,
                                                              NvM_BlockIdType idBlock_uo,
                                                              NvM_Prv_Crypto_ServiceResult_tst const* JobResult_pcst);

#endif

#define NVM_STOP_SEC_CODE
#include "NvM_MemMap.h"

/* NVM_PRV_ERRORDETECTION_H */
#endif
