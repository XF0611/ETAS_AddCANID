
/**
 ***********************************************************************************************************************
 *          Multiple Include Protection
 ***********************************************************************************************************************
 */

#ifndef DCMDSPUDS_SECA_PUB_H
#define DCMDSPUDS_SECA_PUB_H

/**
 ***********************************************************************************************************************
 *          Defines / Macros
 ***********************************************************************************************************************
 */

#if(DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)

#define DCM_PRV_REQUEST_SEED 0
#define DCM_PRV_SEND_KEY    1

/* FC_VariationPoint_START */
                /*Obsolete*/
#ifndef     DCM_REQUEST_SEED
#define     DCM_REQUEST_SEED            DCM_PRV_REQUEST_SEED
#endif
#ifndef     DCM_SEND_KEY
#define     DCM_SEND_KEY                DCM_PRV_SEND_KEY
#endif
/* FC_VariationPoint_END */

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_SecaType: Ram structure for SECA service\n
 * uint8 SeedBuff[DCM_SECA_MAXSEEDSIZE];  SeedBuffer to store calculate seed value,
 *                                        required only if DCM_DSP_SECA_STORESEED is enabled\n
 * uint32  Residual_delay_u32;            Counter to store the Residual Delay count value\n
 * uint8  FailedAtm_cnt_u8;               Counter to store the Failed Attempts \n
 */

/**
 ***********************************************************************************************************************
 *          Type Definitions
 ***********************************************************************************************************************
 */
typedef struct
{
#if (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)
    uint8 SeedBuff[DCM_CFG_SECA_MAXSEEDSIZE];                 /* SeedBuffer to store calculate seed value*/
#endif
    uint32  Residual_delay_u32;                     /* Counter to store the Residual Delay count value*/
    uint8  FailedAtm_cnt_u8;                      /* Counter to store the Failed Attempts */
}Dcm_Dsp_SecaType;

#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Type def for GetSecurityAttemptCounter function prototype
 */
typedef union
{
    Std_ReturnType (*Dcm_GetSecurityAttemptCounter_pfct) (Dcm_OpStatusType OpStatus,uint8 * AttemptCounter);
    /* If Asynchronous server call point handling is requested */
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSecurityAttemptCounter1_pfct) (Dcm_OpStatusType OpStatus);
    Std_ReturnType (*Dcm_GetSecurityAttemptCounter2_pfct) (uint8 * AttemptCounter);
#endif
}un_GetSecurityAttempt;

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Type def for SetSecurityAttemptCounter function prototype
 */
typedef union
{
Std_ReturnType (*Dcm_SetSecurityAttemptCounter_pfct) (Dcm_OpStatusType OpStatus,uint8 AttemptCounter);
/* If Asynchronous server call point handling is requested */
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
Std_ReturnType (*Dcm_SetSecurityAttemptCounter_pfct1) (void);
#endif
}un_SetSecurityAttempt;

#endif

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Type def for GetSeed function prototype
 */
typedef union
{
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr) (const uint8 * SecurityAccessDataRecord,Dcm_OpStatusType OpStatus,uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*Dcm_GetSeed_ptr1) (uint32* SeedLength_u32,const uint8 * SecurityAccessDataRecord,Dcm_OpStatusType OpStatus,uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*Dcm_GetSeed_ptr3) (Dcm_OpStatusType OpStatus,uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr4) (Dcm_SecLevelType SecLevel_u8,uint32 Seedlen_u32,uint32 AccDataRecsize_u32,uint8 * SecurityAccessDataRecord,uint8 * Seed,Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
    /* If asynchronous server call point handling is requested */
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr5) (const uint8 * SecurityAccessDataRecord,Dcm_OpStatusType OpStatus);
    Std_ReturnType (*Dcm_GetSeed_ptr6) (Dcm_OpStatusType OpStatus);
    Std_ReturnType (*Dcm_GetSeed_ptr7) (uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr8) (Dcm_SecLevelType SecLevel_u8,uint32* Seedlen_u32,uint32 AccDataRecsize_u32,uint8 * SecurityAccessDataRecord,uint8 * Seed,Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if(DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr9) (uint32* SeedLength_u32,Dcm_OpStatusType OpStatus,uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    Std_ReturnType (*Dcm_GetSeed_ptr10) (uint32 SeedLength_u32,const uint8 * SecurityAccessDataRecord,Dcm_OpStatusType OpStatus);
    Std_ReturnType (*Dcm_GetSeed_ptr11) (uint32 SeedLength_u32,Dcm_OpStatusType OpStatus);
    Std_ReturnType (*Dcm_GetSeed_ptr12) (uint32* SeedLength_u32,uint8 * Seed,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
}un_GetSeed;

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Type def for compare key function prototype
 */
typedef union {
#if (DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)
Std_ReturnType (*Dcm_CompareKey_ptr1) (const uint8 * Key,Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType * ErrorCode);
#endif
Std_ReturnType (*Dcm_CompareKey_ptr3) (uint32 Key_size_u32,const uint8 * Key,Dcm_OpStatusType OpStatus,Dcm_NegativeResponseCodeType * ErrorCode);
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
Std_ReturnType (*Dcm_CompareKey_ptr4) (const uint8 * Key,Dcm_OpStatusType OpStatus);
/* Typedef for Rte_Result Calls */
Std_ReturnType (*Dcm_CompareKey_ptr5) (Dcm_NegativeResponseCodeType * ErrorCode);
Std_ReturnType (*Dcm_CompareKey_ptr6) (uint32 KeyLength_u32,const uint8 * Key,Dcm_OpStatusType OpStatus);
#endif
}un_CompareKey;

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * DcmDspSecurityUsePort:Defines which kind of interface shall be used for security access.\n
 * USE_ASYNCH_CLIENT_SERVER,\n
 * USE_ASYNCH_FNC,\n
 *
 */
typedef enum
{
    USE_ASYNCH_CLIENT_SERVER,
    USE_ASYNCH_FNC
}DcmDspSecurityUsePort;
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Configuration of Security Access\n
 * Memeber elements;\n
 * uint32               PowerOnDelay_u32;    Delay timer value in counts of DCM_TASK_TIME\n
 * uint32               DelayTime_u32;       Delay time after failed security access in counts of DCM_TASK_TIME\n
 * uint32               numSessBitMask_u32;  Session value bit mask for the security level\n
 * void* const          Dsp_GetSeed_fp;      Function pointer for GetSeed function\n
 * void* const          Dsp_CompareKey_fp;   Function pointer for CompareKey function \n
 * Dcm_SecLevelType     Security_level_u8;   Security Level \n
 * uint8                Seed_size_u8;        Security Seed size\n
 * uint8                Key_size_u8;         Security Key size \n
 * uint8                NumAttDelay_u8;      No of failed security access after which delay time is active \n
 * uint8                NumAttLock_u8;       No of failed security access after which security is locked \n
 * uint8                AccDataRecsize_u8;   Size of the Access Data Record in the request message\n
 * DcmDspSecurityUsePort usePort;            Type of interface to obtains Data
 *
 *  */
typedef struct
{
    uint32              PowerOnDelay_u32;   /* Delay timer value in counts of DCM_TASK_TIME */
    uint32              DelayTime_u32;      /* Delay time after failed security access in counts of DCM_TASK_TIME*/
#if((DCM_CFG_DSP_USE_ASYNCH_CLIENT_SERVER != DCM_CFG_OFF)||(DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_USE_ASYNCH_FNC != DCM_CFG_OFF))
    un_GetSeed         Dsp_GetSeed_fp;      /* Function pointer for GetSeed function */
#endif
    un_CompareKey       Dsp_CompareKey_fp;   /* Function pointer for CompareKey function */
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER != DCM_CFG_OFF)
    un_GetSecurityAttempt         Dsp_GetAttempCounter_fp;   /* Function pointer for GetAttempCounter function */
    un_SetSecurityAttempt         Dsp_SetAttempCounter_fp;   /* Function pointer for SetAttempCounter function */
#endif
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    un_GetSeed Dsp_GetSeed_ResultFp;      /* Function pointer for GetSeed Results Function */
    un_CompareKey Dsp_CompareKey_ResultFp;   /* Fucntion pointer for CompareKey Results Function */
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER != DCM_CFG_OFF)
    un_GetSecurityAttempt Dsp_GetAttempCounter_Resultfp;   /* Function pointer for GetAttempCounter Results function */
    un_SetSecurityAttempt Dsp_SetAttempCounter_Resultfp;   /* Function pointer for SetAttempCounter Results function */
#endif
#endif
    Dcm_SecLevelType    Security_level_u8;  /* Security Level */
    uint32               Seed_size_u32;       /* Security Seed size */
    uint32               Key_size_u32;        /* Security Key size */
    uint8               NumAttDelay_u8;     /* No of failed security access after which delay time is active */
    uint8               NumAttLock_u8;      /* No of failed security access after which security is locked */
    uint32               AccDataRecsize_u32;  /* Size of the Access Data Record in the request message */
    DcmDspSecurityUsePort    usePort;   /* Type of interface to obtain Data */
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
    /* Flag To indicate whether Asnchronous server call point handling is requested */
    boolean   UseAsynchronousServerCallPoint_b;
#endif
    boolean UseFlexibleLength; /* Flag to indicate whetherflexible handling is reqeusted or not */
}Dcm_Dsp_Security_t;
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_SecaPowerOnDelayIni : Initialisaton of PowerOn delay timer for all security levels configured.\n
 * @param           None
 * @retval          None
 */

 /**
 ***********************************************************************************************************************
 *          Extern Declarations
 ***********************************************************************************************************************
 */

/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Gives Elapsed time since the poweron of ECU\n
 */
extern uint32 Dcm_Dsp_SecaGlobaltimer_u32;



extern void Dcm_Dsp_SecaPowerOnDelayIni(void);
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_SecaSesIni : Re-Initialisaton of all security parameters due to session timeout or change in session.\n
 * @param           None
 * @retval          None
 */
extern void Dcm_Dsp_SecaSessIni(void);
#if (DCM_CFG_DSP_SECA_ATTEMPT_COUNTER!=DCM_CFG_OFF)
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_RestoreDelayCount : Function for restoring the Delay count values for each security\n
 *                             level configured in Security Access service.\n
 * @param           None
 * @retval          None
 */
extern void Dcm_Dsp_RestoreDelayCount(void);

#endif
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_Seca:-Array to store seed,failed_attempt_count and residual delay for all
 * configured security levels.
 *
 */
extern Dcm_Dsp_SecaType Dcm_Dsp_Seca[DCM_CFG_DSP_NUMSECURITY];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 * @ingroup DCMDSP_UDS_EXTENDED
 * Dcm_Dsp_Security:-Array to store the configuration of all
 * configured security levels.
 *
 */
extern const Dcm_Dsp_Security_t Dcm_Dsp_Security[DCM_CFG_DSP_NUMSECURITY];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#endif /* _DCMDSPUDS_SECA_PUB_H  */
