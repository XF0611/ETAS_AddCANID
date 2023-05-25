

#ifndef FEE_RB_IDX_H
#define FEE_RB_IDX_H

/*
 ***************************************************************************************************
 * Type definition and enums
 ***************************************************************************************************
 */

typedef enum
{
    Fee_Rb_DeviceName = 0,
    /* following enums are not valid device-driver-enums */
    Fee_Rb_Device_Max      /* Number of configured Fee Devices */
} Fee_Rb_DeviceName_ten;

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

#define FEE_CFG_FEE_MULTIINSTANCE_ENABLED                    (FALSE)

/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

extern uint8                    Fee_Rb_GetDeviceIndexFromDeviceName(Fee_Rb_DeviceName_ten FeeDeviceName_en);
extern boolean                  Fee_Rb_IsThisDeviceIndexFee(uint8 DeviceIndex_u8);
extern Fee_Rb_DeviceName_ten    Fee_Rb_GetDeviceNameFromDeviceIndex(uint8 feeDeviceIdx_u8);

extern void Fee_Rb_Idx_Init(Fee_Rb_DeviceName_ten FeeDeviceName_en, Fee_ConfigType const * ConfigPtr);
extern void Fee_Rb_Idx_EndInit(Fee_Rb_DeviceName_ten FeeDeviceName_en);
extern uint32 Fee_Rb_Idx_GetSectChngCnt(Fee_Rb_DeviceName_ten FeeDeviceName_en);
extern void Fee_Rb_Idx_Cancel(Fee_Rb_DeviceName_ten FeeDeviceName_en);
extern void Fee_Rb_Idx_MainFunction(Fee_Rb_DeviceName_ten FeeDeviceName_en);

extern Std_ReturnType Fee_Rb_Idx_Write(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber,
                                uint8* DataBufferPtr);
extern Std_ReturnType Fee_Rb_Idx_VarLenWrite(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber,
                                         uint8* DataBufferPtr,
                                         uint16 Length);

extern Std_ReturnType Fee_Rb_Idx_Read(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber,
                               uint16 BlockOffset,
                               uint8* DataBufferPtr,
                               uint16 Length);
extern Std_ReturnType Fee_Rb_Idx_VarLenRead(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber,
                                        uint16 BlockOffset,
                                        uint8* DataBufferPtr,
                                        uint16 Length);

extern Std_ReturnType Fee_Rb_Idx_InvalidateBlock(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber);

extern Std_ReturnType Fee_Rb_Idx_EraseImmediateBlock(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 BlockNumber);

extern Std_ReturnType Fee_Rb_Idx_BlockMaintenance(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber);

extern MemIf_StatusType Fee_Rb_Idx_GetStatus(Fee_Rb_DeviceName_ten FeeDeviceName_en);

extern MemIf_JobResultType Fee_Rb_Idx_GetJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en);

extern MemIf_JobResultType Fee_Rb_Idx_GetAdapterJobResult(Fee_Rb_DeviceName_ten FeeDeviceName_en);

extern MemIf_Rb_MigrationResult_ten Fee_Rb_Idx_GetMigrationResult(Fee_Rb_DeviceName_ten FeeDeviceName_en, uint16 Blocknumber);

extern void Fee_Rb_Idx_DisableBackgroundOperation(Fee_Rb_DeviceName_ten FeeDeviceName_en);

extern void Fee_Rb_Idx_EnableBackgroundOperation(Fee_Rb_DeviceName_ten FeeDeviceName_en);







extern Fee_Rb_WorkingStateType_ten Fee_Rb_Idx_GetWorkingState(Fee_Rb_DeviceName_ten FeeDeviceName_en);

/* End of Fee section */
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

/* FEE_RB_IDX_H */
#endif

