

#ifndef MEMIF_CFG_H
#define MEMIF_CFG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "MemIf_Types.h"
#include "Fee.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
#define MEMIF_VENDOR_ID                   6
#define MEMIF_MODULE_ID                   22
#define MEMIF_INSTANCE_ID                 0

#define MEMIF_SW_MAJOR_VERSION            17
#define MEMIF_SW_MINOR_VERSION            0
#define MEMIF_SW_PATCH_VERSION            0

#define MEMIF_AR_RELEASE_MAJOR_VERSION    4
#define MEMIF_AR_RELEASE_MINOR_VERSION    2
#define MEMIF_AR_RELEASE_REVISION_VERSION 2

/* Switch for version info api */
#define MEMIF_VERSION_INFO_API      STD_OFF

/* Switch for dev error detect */
#define MEMIF_DEV_ERROR_DETECT      STD_OFF
#define MEMIF_FEE_USED      STD_ON
#define MEMIF_NUM_OF_EA_DEVICES      0
#define MEMIF_FEE_AND_EA_USED      STD_OFF
#define MEMIF_MIN_NUM_DEVICES            0
#define MEMIF_MAX_NUM_DEVICES            1
#define MEMIF_RB_NUM_OF_FEE_DEVICES      1


/*
 **********************************************************************************************************************
 * Type definitions
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

#define MemIf_Read(DeviceIndex,BlockNumber,BlockOffset,DataBufferPtr,Length)            Fee_Read(BlockNumber,BlockOffset,DataBufferPtr,Length)
#define MemIf_Write(DeviceIndex,BlockNumber,DataBufferPtr)                              Fee_Write(BlockNumber,DataBufferPtr)
#define MemIf_InvalidateBlock(DeviceIndex,BlockNumber)                                  Fee_InvalidateBlock(BlockNumber)
#define MemIf_GetStatus(DeviceIndex)                                                    Fee_GetStatus()
#define MemIf_GetJobResult(DeviceIndex)                                                 Fee_GetJobResult()
#define MemIf_Cancel(DeviceIndex)                                                       Fee_Cancel()
#define MemIf_EraseImmediateBlock(DeviceIndex,BlockNumber)                              Fee_EraseImmediateBlock(BlockNumber)
#define MemIf_Rb_VarLenRead(DeviceIndex,BlockNumber,BlockOffset,DataBufferPtr,Length)   Fee_Rb_VarLenRead(BlockNumber,BlockOffset,DataBufferPtr,Length)
#define MemIf_Rb_VarLenWrite(DeviceIndex,BlockNumber,DataBufferPtr,Length)              Fee_Rb_VarLenWrite(BlockNumber,DataBufferPtr,Length)
#define MemIf_Rb_GetMigrationResult(DeviceIndex,BlockNumber)                            Fee_Rb_GetMigrationResult(BlockNumber)
/* The Fee block maintenance API is not available under all conditions */
#if( (!(defined(FEE_RB_MAINTAIN))) || (defined(FEE_RB_MAINTAIN) && (FEE_RB_MAINTAIN != FALSE)) )
#define MemIf_Rb_BlockMaintenance(DeviceIndex,BlockNumber)                              Fee_Rb_BlockMaintenance(BlockNumber)
#else
#define MemIf_Rb_BlockMaintenance(DeviceIndex,BlockNumber)                              (E_NOT_OK)
#endif


/* MEMIF_CFG_H */
#endif
