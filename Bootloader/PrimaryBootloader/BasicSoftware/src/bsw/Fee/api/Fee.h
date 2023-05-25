


#ifndef FEE_H
#define FEE_H

/**
 **********************************************************************************************************************
 * \moduledescription
 *           AUTOSAR FEE
 *           According to Document Version 3.0.0, Part of Release 4.0, Revision 2
 * \scope    INTERN
 **********************************************************************************************************************
 */


/*
 ****************************************************************************************************
 * Includes
 ****************************************************************************************************
 */

#include "Std_Types.h"
#include "MemIf_Types.h"
#include "Fee_Rb_Types.h"
#include "Fee_Cfg.h"

 # if(FEE_AR_RELEASE_MAJOR_VERSION < 4) ||\
    (\
        (FEE_AR_RELEASE_MAJOR_VERSION == 4) &&\
        (FEE_AR_RELEASE_MINOR_VERSION < 2)\
    )
#define FEE_AR42_API_COMPLIANCE STD_OFF
# else
#define FEE_AR42_API_COMPLIANCE STD_ON
# endif

#include "Fee_Rb_Idx.h"

# if(FEE_PRV_DEBUGGING != FALSE)
#include "Mcu.h"
/* FEE_PRV_DEBUGGING */
# endif

/* Fee debug monitor */
#define FEE_PRV_DEBUG_MONITOR FALSE

/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */
#define FEE_START_SEC_CODE
#include "Fee_MemMap.h"

# if(FEE_AR42_API_COMPLIANCE == STD_ON)
extern void Fee_Init(Fee_ConfigType const * ConfigPtr);
# else
extern void Fee_Init(void);
# endif

extern void Fee_Rb_EndInit(void);
extern uint32 Fee_Rb_GetSectChngCnt(void);
extern void Fee_Cancel(void);
extern void Fee_MainFunction(void);
extern Std_ReturnType Fee_Write(uint16 Blocknumber,
                                uint8* DataBufferPtr);
extern Std_ReturnType Fee_Rb_VarLenWrite(uint16 Blocknumber,
                                         uint8* DataBufferPtr,
                                         uint16 Length);
extern Std_ReturnType Fee_Read(uint16 Blocknumber,
                               uint16 BlockOffset,
                               uint8* DataBufferPtr,
                               uint16 Length);
extern Std_ReturnType Fee_Rb_VarLenRead(uint16 Blocknumber,
                                        uint16 BlockOffset,
                                        uint8* DataBufferPtr,
                                        uint16 Length);
extern Std_ReturnType Fee_InvalidateBlock(uint16 Blocknumber);
extern Std_ReturnType Fee_EraseImmediateBlock(uint16 BlockNumber);

# if(FEE_RB_MAINTAIN != FALSE)
extern Std_ReturnType Fee_Rb_BlockMaintenance(uint16 Blocknumber);
# endif

extern MemIf_StatusType Fee_GetStatus(void);
extern MemIf_JobResultType Fee_GetJobResult(void);

extern MemIf_JobResultType Fee_Rb_GetAdapterJobResult(void);
extern MemIf_Rb_MigrationResult_ten Fee_Rb_GetMigrationResult(uint16 Blocknumber);

extern void Fee_Rb_DisableBackgroundOperation(void);
extern void Fee_Rb_EnableBackgroundOperation(void);

# if(STD_ON == FEE_SET_MODE_SUPPORTED)
extern void Fee_SetMode(MemIf_ModeType Mode);
# endif

# if(STD_ON == FEE_VERSION_INFO_API)
extern void Fee_GetVersionInfo(Std_VersionInfoType * VersionInfoPtr);
/* FEE_VERSION_INFO_API */
# endif

# if(defined(FEE_RB_TRIGGER_REORG) && (FEE_RB_TRIGGER_REORG != FALSE))
extern Std_ReturnType Fee_Rb_TriggerReorg(void);
# endif

# if(defined(FEE_RB_ENTER_STOP_MODE) && (FEE_RB_ENTER_STOP_MODE != FALSE))
extern Std_ReturnType Fee_Rb_EnterStopMode(void);
# endif

# if(defined(FEE_RB_GET_NR_FREE_BYTES) && (FEE_RB_GET_NR_FREE_BYTES != FALSE))
extern uint32 Fee_Rb_GetNrFreeBytes(void);
# endif

# if(defined(FEE_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES) && (FEE_RB_GET_NR_FREE_BYTES_AND_FAT_ENTRIES != FALSE))
extern void Fee_Rb_GetNrFreeBytesAndFatEntries(uint32 * nrFreeBytes_pu32, uint32 * nrFreeFatEntries_pu32, boolean isEraseAccepted_b);
# endif

extern Fee_Rb_WorkingStateType_ten Fee_Rb_GetWorkingState(void);

/* End of Fee section */
#define FEE_STOP_SEC_CODE
#include "Fee_MemMap.h"

/* FEE_H */
#endif
