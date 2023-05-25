

#ifndef DCMDSPUDS_UDS_PROT_H
#define DCMDSPUDS_UDS_PROT_H

#define  DCM_MAX_READMEMRANGE      0xFFFFFFFFu /* Max Read Memory Range*/
#define  DCM_DSP_SID_ROUTINECONTROL                     0x31u
#if (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) )
extern void Dcm_ControlDtcSettingExit(void);
#endif

#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
extern void Dcm_RdpiSessionCheck(uint32 sessionMask_u32);
extern void Dcm_RdpiSecurityCheck(void);
#endif

#if ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)          ||  \
     (DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)         ||  \
     (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)   ||  \
     (DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)                              ||  \
     (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF))
extern void Dcm_ResetDIDIndexstruct (Dcm_DIDIndexType_tst * idxDidIndexType_st);
#endif

#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF )
Std_ReturnType Dcm_Prv_GetIndexOfDID (
                                                uint16 did,
                                                Dcm_DIDIndexType_tst * idxDidIndexType_st
                                                  );
#endif

#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
extern void Dcm_ResetAsynchFlags(void);
#endif

#if (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF)
#if (DCM_CFG_DSP_SECA_ASP_ENABLED != DCM_CFG_OFF)
extern void Dcm_ResetAsynchSecaFlag(void);
#endif
#endif

#if ((DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)||(DCM_CFG_DIDSUPPORT != DCM_CFG_OFF ))
extern void Dcm_ConvBitsToBytes(uint32 * DataLenInBits) ;
#endif
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Macros for the service identifiers in DSP */
#define  DCM_DSP_SID_ECURESET                           0x11u
#define  DCM_DSP_SID_TESTERPRESENT                      0x3Eu
#define  DCM_DSP_SID_SECURITYACCESS                     0x27u
#define  DCM_DSP_SID_CONTROLDTCSETTING                  0x85u
#define  DCM_DSP_SID_READDTCINFORMATION                 0x19u
#define  DCM_DSP_SID_COMMUNICATIONCONTROL               0x28u
#define  DCM_DSP_SID_READDATABYIDENTIFIER               0x22u
#define  DCM_DSP_SID_WRITEDATABYIDENTIFIER              0x2Eu
#define  DCM_DSP_SID_DIAGNOSTICSESSIONCONTROL           0x10u
#define  DCM_DSP_SID_CLEARDIAGNOSTICINFORMATION         0x14u
#define  DCM_DSP_SID_DYNAMICALLYDEFINEDATAIDENTIFIER    0x2Cu
#define  DCM_DSP_SID_INPUTOUTPUTCONTROLBYIDENTIFIER     0x2Fu
#define  DCM_DSP_SID_REQUESTDOWNLOAD                    0x34u
#define  DCM_DSP_SID_REQUESTUPLOAD                      0x35u
#define  DCM_DSP_SID_REQUESTTRANSFEREXIT                0x37u
#define  DCM_MINSIZE                                    0x00u
#define  DCM_MAXSIZE                                    0x04u
#define  DCM_IOCBI_INIT                                 0xFFu
#define  DCM_VALUE_NULL                                 0x00u

/*
 **********************************************************************************************************
 * Globals Macros for converting byte lengths
 **********************************************************************************************************
 */

/* Convert two uint8 variables to a uint16 variable */

# define DSP_CONV_2U8_TO_U16(hi,lo)                     ((uint16)((uint16)(((uint16)(hi))<<(uint8)8u)| ((uint16)(lo))))

/* Convert four uint8 variables to a uint32 variable */

# define DSP_CONV_4U8_TO_U32(hihi, hilo, lohi, lolo)    ((uint32)((((uint32)(hihi))<<24u)| (((uint32)(hilo))<<16u)| \
                                                (((uint32)(lohi))<<8u) | ((uint32)(lolo))))

/* Convert a uint16 variable to a uint8 MSByte variable */

# define DSP_GET_MSBU16_TO_U8(var)                     ((uint8)(((uint16)(var))>>8u))

/* Convert a uint16 variable to a uint8 LSByte variable */

# define DSP_GET_LSBU16_TO_U8(var)                     ((uint8)(var))

/* Convert two uint16 variables to a uint32 variable */

# define DSP_CONV_2U16_TO_U32(hi,lo)                    ((uint32)((((uint32)(hi))<<16u)| ((uint32)(lo))))

/* Convert a uint32 variable to a uint8 Most Significant Byte variable */

# define DSP_GET_BYTE3U32_TO_U8(var)                   ((uint8)(((uint32)(var))>>24u))

/* Convert a uint32 variable to a uint8 More Significant Byte variable */

# define DSP_GET_BYTE2U32_TO_U8(var)                   ((uint8)(((uint32)(var))>>16u))

/* Convert a uint32 variable to a uint8 Less Significant Byte variable */

# define DSP_GET_BYTE1U32_TO_U8(var)                   ((uint8)(((uint32)(var))>>8u))

/* Convert a uint32 variable to a uint8 Least Significant Byte variable */

# define DSP_GET_BYTE0U32_TO_U8(var)                   ((uint8)(var))

/* Convert a uint32 variable to a uint16 Most Significant variable */

# define DSP_GET_MSWU32_TO_U16(var)                    ((uint16)(((uint32)(var))>>16u))

/* Convert a uint32 variable to a uint16 Least Significant variable */

# define DSP_GET_LSWU32_TO_U16(var)                    ((uint16)(var))

#if ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF) || \
    (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF) ||((DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_CALL_BACK_NUM_PORTS !=0)) || \
    (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF))
/* Macro to define RTE infrastructure error*/
#define DCM_INFRASTRUCTURE_ERROR 0x80u
#endif

/* Macro to define lenth of VIN*/
#if((DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF))

#define DCM_VIN_DATALEN  17

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint8 Dcm_VInData_au8[DCM_VIN_DATALEN];  /*Buffer to store VIN data*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_VinReceived_b;/*Flag to indicate VIN data is received successfully or not */
extern boolean Dcm_GetvinConditionCheckRead;/*Flag to indicate Conditioncheck read function is successfully done or not */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif

#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_ATOMICREAD_DID>0)
extern uint16 Dcm_Prv_AtomicReadDid_index(uint16 DID);
#endif

#if(DCM_CFG_ATOMICWRITE_DID>0)
extern uint16 Dcm_Prv_AtomicWriteDid_index(uint16 DID);
#endif

#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(DCM_CFG_ATOMICREAD_DID>0)
typedef struct
{
    uint16 dataDid_u16;
    Std_ReturnType (*AtomicRead_pfct)(void* AtomicRead_struct);
    void (*AtomicRead_CopyData_pfct)(uint8* targetBuffer);

}AtomicRead_st;
#endif

#if(DCM_CFG_ATOMICWRITE_DID>0)
typedef struct
{
    uint16 dataDid_u16;
    Std_ReturnType (*AtomicWrite_pfct)(void* AtomicWrite_struct);
void (*AtomicWrite_CopyData_pfct)(uint8* requestBuffer);
}AtomicWrite_st;
#endif


/* Definitions of states of ROE service */
typedef enum
{
    DCM_ROE_CLEARED=0,              /* Initialisation state of ROE events*/
    DCM_ROE_STOPPED,              /* State of ROE events when a valid ROE set up request is received from the tester */
    DCM_ROE_STARTED               /* State of ROE events when a ROE start request is received from the tester */
}Dcm_DspRoeEventState_ten;

#if(DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF)

#if(DCM_CFG_DSP_ROEDID_ENABLED != DCM_CFG_OFF)
typedef union
{
    Std_ReturnType (*ROEDID_fp) (Dcm_DspRoeEventState_ten RoeEventStatus );
}un_ROEDID;

typedef struct
{
    un_ROEDID ROEDID_cpv;
    uint16 RoeEventDid_u16;         /*DID for ROE monitoring*/
    uint8 RoeEventId_u8;            /*RoeeventID configured for the DID*/
} DcmDspRoeDidEvents_tst;

typedef struct
{

    boolean Is_PreConfigured_b;/*The flag will be set to TRUE if the event is preconfigured*/
    boolean stDspRoeCtrlStorageState_b;/* Variable to store the storage bit information for ctrl */
    boolean stDspRoeStorageState_b;     /* Variable to store the storage bit information */
    boolean stDspRoeSessionIsDefault_b;  /* Variable stDspRoeSessionIsDefault_b will be TRUE if the session is Default session and the ROE event is started*/
    uint8 stRoeEventWindowTime_u8;      /* Event Window Time information */
    Dcm_DspRoeEventState_ten RoeEventStatus;    /*State of the ROE event for OnchangeofDid or On DTC status change*/
    uint16 SourceAddress_u16;           /*Tester SourceAddress on which the ROE request was Received*/
    uint16 stRoeEventRecord_u16;        /* Event Record Information */
    uint16 stSrvToRespDid_u16;        /* DID for performing service to respond to action */

}DcmDspRoeDidStateVariables_tst;

#endif

#endif
typedef union
{
    Std_ReturnType (*ROEDTC_fp) (Dcm_DspRoeEventState_ten RoeEventStatus );
}un_ROEDTC;
typedef struct
{
    un_ROEDTC ROEDTC_cpv;
    uint8 RoeEventId_u8;            /*RoeeventID configured for the Dtc*/
}DcmDspRoeOnDtcStatusChg_tst;

typedef struct
{
    boolean Is_PreConfigured_b;/*The flag will be set to TRUE if the event is preconfigured*/
    boolean stDspRoeStorageState_b; /* Variable to store the storage bit information */
    boolean stDspRoeCtrlStorageState_b;/* Variable to store the storage bit information for ctrl */
    boolean stDspRoeSessionIsDefault_b; /* Variable stDspRoeSession_b will be TRUE if the session is Default session and the ROE event is started*/
    Dcm_DspRoeEventState_ten RoeEventStatus;    /*State of the ROE event for OnchangeofDid or On DTC status change*/
    uint8 testerReqDTCStatusMask_u8;      /*DTCStatusMask requested to monitor by tester*/
    uint8 stRoeEventWindowTime_u8;     /* Event Window Time information */
    uint16 SourceAddress_u16;           /*Tester SourceAddress on which the ROE request was Received*/
}DcmDspRoeDtcStateVariable_tst;

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED!=DCM_CFG_OFF)
extern void Dcm_ResetRoeOnDtcevents(void);
#endif
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF))

#define DCM_INVALID_NVDBLOCK    0u

/* Definitions of DID signal Data Port Interface Types */


#define USE_BLOCK_ID                                0x00u
#define USE_DATA_ASYNCH_CLIENT_SERVER               0x01u
#define USE_DATA_SENDER_RECEIVER                    0x02u
#define USE_DATA_SENDER_RECEIVER_AS_SERVICE         0x03u
#define USE_DATA_SYNCH_CLIENT_SERVER                0x04u
#define USE_ECU_SIGNAL                              0x05u
#define USE_DATA_SYNCH_FNC                          0x06u
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
#define USE_DATA_RDBI_PAGED_FNC                     0x07u
#endif
#define USE_DATA_ASYNCH_FNC                         0x08u


/* Definitions of DID Port Interface Types */
#define USE_ATOMIC_NV_DATA_INTERFACE                        0x09u
#define USE_ATOMIC_SENDER_RECEIVER_INTERFACE                0x0Au
#define USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE     0x0Bu
#define USE_DATA_ELEMENT_SPECIFIC_INTERFACES                0x0Cu


/* Definitions of IOCONTROLMASK configured for DID */
typedef enum
{
    DCM_CONTROLMASK_NO=0,                            /* NO Control Mask support */
    DCM_CONTROLMASK_INTERNAL,                        /* Internal Control Mask support, Dcm to do the control mask handling internally */
    DCM_CONTROLMASK_EXTERNAL                         /* External Control Mask support, Dcm to forward the control mask received from tester to application, application will do the handling */
}Dcm_Dsp_IocbiCtrlMask_ten;

/* Did Extended Structure Definition */
typedef struct
{
#if ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF))
  uint32 dataAllowedSessRead_u32;          /* Allowed sessions for RDBI (for writing on to dcm buffer by application) */
  uint32 dataAllowedSecRead_u32;           /* Allowed security levels for RDBI(for writing on to dcm buffer by application)*/
  Std_ReturnType (*adrUserReadModeRule_pfct) (Dcm_NegativeResponseCodeType *dataNrc ,uint16 dataDid_u16,Dcm_Direction_t Dcm_Support_Read );
#if(DCM_CFG_DSP_MODERULEFORDIDREAD != DCM_CFG_OFF )
  boolean (*adrRdbiModeRuleChkFnc_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8); /* Mode rule check funtion pointer */
#endif
#endif
#if (DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
  uint32 dataAllowedSessWrite_u32;          /* Allowed sessions For wdbi(for reading) */
  uint32 dataAllowedSecWrite_u32;           /* Allowed security levels for wdbi (for reading) */
  Std_ReturnType (*adrUserWriteModeRule_pfct) (Dcm_NegativeResponseCodeType *dataNrc ,uint16 dataDid_u16 ,Dcm_Direction_t Dcm_Support_Write);
#if(DCM_CFG_DSP_MODERULEFORDIDWRITE != DCM_CFG_OFF )
  boolean (*adrWdbiModeRuleChkFnc_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8); /* Mode rule check funtion pointer */
#endif
#endif
#if (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
  uint32  dataSessBitMask_u32;      /* Session value bit mask for the DID Control */
  uint32  dataSecBitMask_u32;       /* Security level bit mask for the DID Control */
  Std_ReturnType (*adrUserControlModeRule_pfct) (Dcm_NegativeResponseCodeType *dataNrc ,uint16 dataDid_u16,Dcm_Direction_t Dcm_Support_IoControl);
#if(DCM_CFG_DSP_MODERULEFORDIDCONTROL != DCM_CFG_OFF )
  boolean (*adrIocbiModeRuleChkFnc_pfct) (Dcm_NegativeResponseCodeType * Nrc_u8); /* Mode rule check funtion pointer */
#endif
  Dcm_Dsp_IocbiCtrlMask_ten dataCtrlMask_en;        /*Control mask type configured by the use, wheher no control Mask or internal or external*/
  uint8     dataCtrlMaskSize_u8;        /*Control mask size*/
  boolean dataIocbirst_b;               /* enable/disable the iocontrol to be reset on session/security change */
  uint8   statusmaskIOControl_u8;      /* Mask to provide info on the supported IO COntrol parameters */
#endif
} Dcm_ExtendedDIDConfig_tst;


#if( DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_DIDDATA_CS_OR_SR_ENABLED==DCM_CFG_OFF)
/*IOcontrol parameter definition*/
typedef uint8 Dcm_InputOutputControlParameterType;
#endif


typedef union
{
    /* Function pointer for IO request */
    Std_ReturnType (*IOControlrequest_pfct) ( Dcm_InputOutputControlParameterType IoctrlParam,
                                              const uint8 * RequestData,
                                              uint16 dataSignalLength_u16,
                                              uint8 controlMaskLength_u8,
                                              Dcm_OpStatusType IocbiOpStatus_u8,
                                              Dcm_NegativeResponseCodeType * ErrorCode);
}un_IOCntrlReqst;

/* Iocbi related function pointers */
#if(DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)

/* Return control to Ecu function pointers */
typedef union
{
    /*synch c/s and control mask is NO_MASK*/
    Std_ReturnType (*ReturnControlEcu1_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ReturnControlEcu2_pfct) (Dcm_OpStatusType OpStatus,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    /*synch c/s and control mask is internal*/
    Std_ReturnType (*ReturnControlEcu3_pfct) (uint8 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ReturnControlEcu4_pfct) (uint16 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ReturnControlEcu5_pfct) (uint32 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    /*Asynch c/s and control mask is external*/
    Std_ReturnType (*ReturnControlEcu6_pfct) (Dcm_OpStatusType OpStatus,
                                              uint8 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ReturnControlEcu7_pfct) (Dcm_OpStatusType OpStatus,
                                              uint16 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ReturnControlEcu8_pfct) (Dcm_OpStatusType OpStatus,
                                              uint32 controlMask,
                                              Dcm_NegativeResponseCodeType * ErrorCode);
}un_RetCntrlEcu;

/* Reset to default function pointers */
typedef union
{
    /*synch fnc and asycnh fnc and control mask is NO_MASK*/
    Std_ReturnType (*ResetToDefault1_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);

    Std_ReturnType (*ResetToDefault2_pfct) (Dcm_OpStatusType OpStatus,
                                            Dcm_NegativeResponseCodeType * ErrorCode);

    /*synch c/s and control mask is internal*/
    Std_ReturnType (*ResetToDefault3_pfct) (uint8 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ResetToDefault4_pfct) (uint16 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ResetToDefault5_pfct) (uint32 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    /*Asynch c/s and control mask is external*/
    Std_ReturnType (*ResetToDefault6_pfct) (Dcm_OpStatusType OpStatus,
                                            uint8 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ResetToDefault7_pfct) (Dcm_OpStatusType OpStatus,
                                            uint16 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ResetToDefault8_pfct) (Dcm_OpStatusType OpStatus,
                                            uint32 controlMask,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
    /*Asyn c/s and control mask is internal*/
    Std_ReturnType (*ResetToDefault9_pfct) (Dcm_OpStatusType OpStatus);
    /*Asyn c/s and Asyn fnc and control mask is external*/
    Std_ReturnType (*ResetToDefault10_pfct) (Dcm_OpStatusType OpStatus, uint8 controlMask);
    Std_ReturnType (*ResetToDefault11_pfct) (Dcm_OpStatusType OpStatus, uint16 controlMask);
    Std_ReturnType (*ResetToDefault12_pfct) (Dcm_OpStatusType OpStatus, uint32 controlMask);
}un_ResetDefault;

/* Freeze frame state function pointers*/
typedef union
{
    /*synch fnc and asycnh fnc and control mask is NO_MASK*/
    Std_ReturnType (*FreezeCurrentState1_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*FreezeCurrentState2_pfct) (Dcm_OpStatusType OpStatus,
                                                Dcm_NegativeResponseCodeType * ErrorCode);

    /*synch c/s and control mask is internal*/
    Std_ReturnType (*FreezeCurrentState3_pfct) (uint8 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*FreezeCurrentState4_pfct) (uint16 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*FreezeCurrentState5_pfct) (uint32 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
    /*Asynch c/s and control mask is external*/
    Std_ReturnType (*FreezeCurrentState6_pfct) (Dcm_OpStatusType OpStatus,
                                                uint8 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*FreezeCurrentState7_pfct) (Dcm_OpStatusType OpStatus,
                                                uint16 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*FreezeCurrentState8_pfct) (Dcm_OpStatusType OpStatus,
                                                uint32 controlMask,
                                                Dcm_NegativeResponseCodeType * ErrorCode);
#if(DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
    Std_ReturnType (*FreezeCurrentState9_pfct) (Dcm_OpStatusType OpStatus);
    Std_ReturnType (*FreezeCurrentState10_pfct) (Dcm_OpStatusType OpStatus,
                                                 uint8 controlMask);
    Std_ReturnType (*FreezeCurrentState11_pfct) (Dcm_OpStatusType OpStatus,
                                                 uint16 controlMask);
    Std_ReturnType (*FreezeCurrentState12_pfct) (Dcm_OpStatusType OpStatus,
                                                 uint32 controlMask);
#endif
}un_FreezeState;

/* Short term adjustment function pointers */
typedef union
{
    /*synch fnc and asycnh fnc and control mask is NO_MASK*/
    Std_ReturnType (*ShortTermAdjustment1_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ShortTermAdjustment2_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_OpStatusType OpStatus,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);

    /*synch c/s and control mask is internal*/
    Std_ReturnType (*ShortTermAdjustment3_pfct) (const uint8 * ControlStateInfo,
                                                 uint8 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ShortTermAdjustment4_pfct) (const uint8 * ControlStateInfo,
                                                 uint16 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ShortTermAdjustment5_pfct) (const uint8 * ControlStateInfo,
                                                 uint32 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    /*Asynch c/s and control mask is external*/
    Std_ReturnType (*ShortTermAdjustment6_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_OpStatusType OpStatus,
                                                 uint8 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ShortTermAdjustment7_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_OpStatusType OpStatus,
                                                 uint16 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
    Std_ReturnType (*ShortTermAdjustment8_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_OpStatusType OpStatus,
                                                 uint32 controlMask,
                                                 Dcm_NegativeResponseCodeType * ErrorCode);
#if(DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
    Std_ReturnType (*ShortTermAdjustment9_pfct) (const uint8 * ControlStateInfo,
                                                 Dcm_OpStatusType OpStatus);
    Std_ReturnType (*ShortTermAdjustment10_pfct) (const uint8 * ControlStateInfo,
                                                  Dcm_OpStatusType OpStatus,
                                                  uint8 controlMask);
    Std_ReturnType (*ShortTermAdjustment11_pfct) (const uint8 * ControlStateInfo,
                                                  Dcm_OpStatusType OpStatus,
                                                  uint16 controlMask);
    Std_ReturnType (*ShortTermAdjustment12_pfct) (const uint8 * ControlStateInfo,
                                                  Dcm_OpStatusType OpStatus,
                                                  uint32 controlMask);
#endif
}un_TermAdjstmnt;

/* Function pointers to collect the result */
typedef union
{
    Std_ReturnType (*ResetToDefault13_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
}un_RTD_Result;

typedef union
{
    Std_ReturnType (*FreezeCurrentState13_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
}un_FCS_Result;

typedef union
{
    Std_ReturnType (*ShortTermAdjustment13_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
}un_STA_Result;

typedef struct
{
  un_IOCntrlReqst ioControlRequest_cpv;         /*Function pointer for SR interface for IO request*/
  un_RetCntrlEcu  adrReturnControlEcu_cpv;      /* ReturnControlToEcu Function Pointer */
  un_ResetDefault adrResetToDefault_cpv;        /* ResetToDefault Function Pointer */
  un_FreezeState  adrFreezeCurrentState_cpv;    /* FreezeCurrentState Function Pointer */
  un_TermAdjstmnt adrShortTermAdjustment_cpv;   /* ShortTermAdjustment Function Pointer */
#if (DCM_CFG_DSP_IOCBI_ASP_ENABLED != DCM_CFG_OFF)
  un_RTD_Result  adrResetToDefaultResults_cpv;      /* ResetToDefault Results Function Pointer */
  un_FCS_Result  adrFreezeCurrentStateResults_cpv;  /* FreezeCurrentState Results Function Pointer */
  un_STA_Result  adrShortTermAdjustmentResults_cpv; /* ShortTermAdjustment Results Function Pointer */
#endif
}Dcm_SignalDIDIocbiConfig_tst;
#endif

/* Condition Check Read Function Pointers */
typedef union
{
    /* When UsePort is USE_DATA_SYNCH_FNC / DATA_SYNC_CLIENT_SERVER */
    Std_ReturnType (*CondChkReadFunc1_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_ASYNCH_FNC / DATA_ASYNC_CLIENT_SERVER */
    Std_ReturnType (*CondChkReadFunc2_pfct) (Dcm_OpStatusType OpStatus,
                    Dcm_NegativeResponseCodeType * ErrorCode);

    /* When useport is DATA_ASYNC_CLIENT_SERVER */
    Std_ReturnType (*CondChkReadFunc3_pfct) (Dcm_OpStatusType OpStatus);
}un_CondChk;

/* Condition Check Read Result Function Pointer */
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
typedef union
{
    /* To collect the read result when Aysnchronous server call point is configured */
    Std_ReturnType (*CondChkReadResFunc1_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
}un_CondChkResult;
#endif

/*  */
typedef union
{
    /* ReadDatalength function pointer for a normal DID and not a dynamically defined DID */
    Std_ReturnType (*ReadDataLengthFnc1_pf) (uint16 * DataLength);
    /* ReadDatalength function pointer for a dynamically defined DID */
    Std_ReturnType (*ReadDataLengthFnc2_pf) (uint32 * DataLength);
    /* ReadDataLength function pointer for a range did */
    Std_ReturnType (*ReaddatalengthFnc3_pf) ( uint16 DID,Dcm_OpStatusType OpStatus,
                                              uint16 * DataLength);
    /* ReadDatalength function pointer for a normal DID and not a dynamically defined DID */
    Std_ReturnType (*ReadDataLengthFnc4_pf) (Dcm_OpStatusType OpStatus,uint16 * DataLength);
    /* When Asynchronous server point handling is requested */
    Std_ReturnType (*ReaddatalengthFnc5_pf) (void);
    /* ReadDataLength function pointer for a control did*/
    Std_ReturnType (*ReadDataLengthFnc6_pf) (Dcm_OpStatusType OpStatus);
}un_ReadDataLength;

#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
typedef union
{
    /* When Asynchronous server call point is configured to true */
    Std_ReturnType (*ReadDataLenResultFnc1_pf) (uint16 * DataLength);
}un_ReadDataResult;
#endif

/* Wdbi related function pointers */
#if (DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
/* Write Function pointer */
typedef union
{
    /* When UsePort is USE_DATA_SYNC_CLIENT_SERVER and Data Length is variable */
    Std_ReturnType (*WdbiFnc1_pfct) (const uint8 * Data,
                                     uint16 DataLength,
                                     Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_SYNC_CLIENT_SERVER and Data Length is fixed */
    Std_ReturnType (*WdbiFnc2_pfct) (const uint8 * Data,
                                     Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_ASYNC_FNC and Data Length is variable */
    Std_ReturnType (*WdbiFnc3_pfct) (const uint8 * Data,
                                     uint16 DataLength,
                                     Dcm_OpStatusType OpStatus,
                                     Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_ASYNC_FNC and Data Length is fixed */
    Std_ReturnType (*WdbiFnc4_pfct) (const uint8 * Data,
                                     Dcm_OpStatusType OpStatus,
                                     Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_SENDER_RECEIVER */
    Std_ReturnType (*WdbiFnc5_pfct) (boolean Data);
    Std_ReturnType (*WdbiFnc6_pfct) (uint8 Data);
    Std_ReturnType (*WdbiFnc7_pfct) (uint16 Data);
    Std_ReturnType (*WdbiFnc8_pfct) (uint32 Data);
    Std_ReturnType (*WdbiFnc9_pfct) (sint8 Data);
    Std_ReturnType (*WdbiFnc10_pfct) (sint16 Data);
    Std_ReturnType (*WdbiFnc11_pfct) (sint32 Data);
    Std_ReturnType (*WdbiFnc12_pfct) (const uint8 * Data);
    Std_ReturnType (*WdbiFnc13_pfct) (const uint16 * Data);
    Std_ReturnType (*WdbiFnc14_pfct) (const uint32 * Data);
    Std_ReturnType (*WdbiFnc15_pfct) (const sint8 * Data);
    Std_ReturnType (*WdbiFnc16_pfct) (const sint16 * Data);
    Std_ReturnType (*WdbiFnc17_pfct) (const sint32 * Data);
    /* When UsePort is USE_DATA_SYNC_FNC and Data Length is variable */
    Std_ReturnType (*WdbiFnc18_pfct) (const uint8 * Data,
                                      uint16 DataLength,
                                      Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_SYNCH_FNC  and Data Length is fixed */
    Std_ReturnType (*WdbiFnc19_pfct) (const uint8 * Data,
                                      Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_ASYNC_CLIENT_SERVER and Data Length is variable */
    Std_ReturnType (*WdbiFnc20_pfct) (const uint8 * Data,
                                      uint16 DataLength,
                                      Dcm_OpStatusType OpStatus,
                                      Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is USE_DATA_ASYNC_CLIENT_SERVER and Data Length is fixed */
    Std_ReturnType (*WdbiFnc21_pfct) (const uint8 * Data,
                                      Dcm_OpStatusType OpStatus,
                                      Dcm_NegativeResponseCodeType * ErrorCode);
}un_adrWriteFnc;

/* When Asynchronous Server Call Point handling is requested */
#if (DCM_CFG_DSP_WRITE_ASP_ENABLED != DCM_CFG_OFF)
typedef union
{
    /* When Data Length is variable */
    Std_ReturnType (*WdbiFnc22_pfct) (const uint8 * Data,
                                      Dcm_OpStatusType OpStatus);

    /* When  Data Length is fixed */
    Std_ReturnType (*WdbiFnc23_pfct) (const uint8 * Data,
                                      uint16 DataLength,
                                      Dcm_OpStatusType OpStatus);

    Std_ReturnType (*WdbiFnc24_pfct) (Dcm_NegativeResponseCodeType * ErrorCode);
}un_WriteResultFnc;
#endif
#endif /* DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF */

/* Write DID data function pointer */
typedef union
{
    /* Type definitions for the WDBI functions */

    /* When UsePort is configured TRUE i.e. RTE is enabled */
    Std_ReturnType (*WriteFncRange1_pfct) ( uint16 DID,
                                            const uint8 * Data,
                                            Dcm_OpStatusType OpStatus,
                                            uint16 DataLength,
                                            Dcm_NegativeResponseCodeType * ErrorCode);

    /* When UsePort is configured FALSE i.e. RTE is disabled */
    Std_ReturnType (*WriteFncRange2_pfct) ( uint16 DID,
                                            const uint8 * Data,
                                            Dcm_OpStatusType OpStatus,
                                            uint16 DataLength,
                                            Dcm_NegativeResponseCodeType * ErrorCode);
}un_WriteDidFnc;

/* Read Data Function Pointer Types */
typedef union
{
    /* When UsePort is USE_DATA_SYNCH_FNC / DATA_SYNC_CLIENT_SERVER */
    Std_ReturnType (*ReadFunc1_pfct) (uint8 * Data);
    /* When UsePort is USE_DATA_ASYNCH_FNC/DATA_ASYNC_CLIENT_SERVER */
    Std_ReturnType (*ReadFunc2_ptr) (Dcm_OpStatusType OpStatus,
                                     uint8 * Data);
    /* When UsePort is SENDER_RECEIVER */
    Std_ReturnType (*ReadFunc3_pfct) (boolean * Data);
    Std_ReturnType (*ReadFunc4_pfct) (uint16 * Data);
    Std_ReturnType (*ReadFunc5_pfct) (uint32 * Data);
    Std_ReturnType (*ReadFunc6_pfct) (sint8 * Data);
    Std_ReturnType (*ReadFunc7_pfct) (sint16 * Data);
    Std_ReturnType (*ReadFunc8_pfct) (sint32 * Data);
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
    /* When UsePort is USE_DATA_RDBI_PAGED_FNC */
    Std_ReturnType (*ReadFunc9_ptr) (Dcm_OpStatusType OpStatus,
                                     uint8 * Data,
                                     uint32 * BufSize,
                                     Dcm_NegativeResponseCodeType * ErrorCode);
#endif
    /* When UsePort is USE_DATA_SYNCH_FNC / DATA_SYNC_CLIENT_SERVER and when the read call id for DDDID */
    Std_ReturnType (*ReadFunc10_pfct) (uint8 * Data,Dcm_NegativeResponseCodeType * ErrorCode);
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
    /* When Asynchronous server call point handling is used */
    Std_ReturnType (*ReadFunc11_ptr) (Dcm_OpStatusType OpStatus);
#endif
}un_ReadFnc;

#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
typedef union
{
/* When UsePort is USE_DATA_SYNCH_FNC / DATA_SYNC_CLIENT_SERVER */
Std_ReturnType (*ReadResultFunc1_pfct) (uint8 * Data);
}un_ReadResultFnc;
#endif


typedef struct
{
  un_CondChk    adrCondChkRdFnc_cpv;      /* Condition Check Read Function pointer (optional)*/
  /* Function to get the data length (optional) */
  un_ReadDataLength     adrReadDataLengthFnc_pfct;
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
  un_CondChkResult      adrCondChkRdFncResults_cpv;       /* Condition Check Read Results Function pointer (optional)*/
  un_ReadDataResult     adrReadDataLengthFncResults_pfct; /* Function pointer to get result of Read data length  */
#endif
#if (DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
  un_adrWriteFnc adrWriteFnc_cpv;      /* Write Function pointer */
  void* const WriteDataTypeVar;
  void (*GetSignalData_pfct)(void* requestBuffer, uint16 position_u16);
#if (DCM_CFG_DSP_WRITE_ASP_ENABLED != DCM_CFG_OFF)
  un_WriteResultFnc adrWriteFncResults_cpv;     /* Write Function results pointer */
#endif
#endif
#if (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
  uint16 idxDcmDspIocbiInfo_u16; /*Index to DID Signal Substructure Configuration for IOCBI */
#endif
}Dcm_SignalDIDSubStructConfig_tst;

/* Did Signal Data Structure Definition */
typedef struct
{
  un_ReadFnc adrReadFnc_cpv;   /* Read Function pointer */
  void* const ReadDataVar;
  Std_ReturnType (*StoreSignal_pfct)(void* targetBuffer, uint16 position);
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
  un_ReadResultFnc adrReadFncResults_cpv;   /* Read Function pointer to get the Result of Read function */
#endif
  uint16 dataSize_u16;              /* DID Data Size in bits */
#if(DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
  uint16 dataNvmBlockId_u16;        /*NVM block id for USE_BLOCK_ID*/
#endif
  uint16 idxDcmDspControlInfo_u16;  /*Index to DID Control Info structure */
  uint8 dataType_u8;                /* Type of Data configured */
  uint8 usePort_u8;                 /* Type of interface to obtains Data */
#if( DCM_CFG_DSP_ALLSIGNAL_OPAQUE !=DCM_CFG_ON)
  uint8 dataEndianness_u8;          /* Signal endianness type of the signal */
#endif
#if(DCM_CFG_DSP_ALLSIGNAL_FIXED_LENGTH!=DCM_CFG_ON)
  boolean dataFixedLength_b;        /*Signal is of fixed or variable length*/
#endif
#if (DCM_CFG_DSP_DATA_ASYNCHCS_ENABLED != DCM_CFG_OFF)
  boolean UseAsynchronousServerCallPoint_b;
#endif
}Dcm_DataInfoConfig_tst;


typedef struct
{
  uint16 posnSigBit_u16;              /* Signal Position */
  uint16 idxDcmDspDatainfo_u16;  /*DID Signal Substructure Configuration for condition check for read and write and read datalength function*/
  Std_ReturnType (*ReadSenderReceiver_pfct)(void* ReadDataVar);
  Std_ReturnType (*WriteSenderReceiver_pfct)(void* WriteDataTypeVar); /* Write function pointer to support new SenderReceiver as per SWS 4.4.0 */
} Dcm_SignalDIDConfig_tst;
#if(DCM_CFG_DSP_NUMISDIDAVAIL > 0)
typedef Std_ReturnType (*IsDIDAvailFnc_pf) (uint16 DID_u16);
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern const IsDIDAvailFnc_pf Dcm_DIDIsAvail[DCM_CFG_DSP_NUMISDIDAVAIL];
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
/* Did Structure Definition */
typedef struct
{
  uint16 dataDid_u16;       /* Data Identifier Value */
  uint8 didUsePort_u8;      /* Application interface type for DID data elements. Either a single operation for all data elements or data element specific operations. */
  boolean AtomicorNewSRCommunication_b; /* Flag to indicate communication as per SWS 4.x.0 */
  uint16 nrSig_u16;         /* No of signals in the DID */
  uint16 dataMaxDidLen_u16;     /* Maximum size of the DID including Gaps */
  boolean dataFixedLength_b;            /* Fixed length or obtainable using ReadDataLengthFnc */
  boolean dataDynamicDid_b;         /* Dynamically Defined DID */
  const Dcm_SignalDIDConfig_tst * adrDidSignalConfig_pcst;  /* Did Signal Data Configuration */
#if(DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)   /*DID configuration mask, included only if post build is enabled*/
  uint8 dataConfigMask_u8;
#endif
#if(DCM_CFG_DSP_NUMISDIDAVAIL > 0)
uint16      idxDIDSupportedFnc_u16;
#endif
   un_IOCntrlReqst ioControlRequest_cpv;
#if(DCM_CFG_ATOMICREAD_DID>0)
   void* const AtomicRead_var;
#endif
#if(DCM_CFG_ATOMICWRITE_DID>0)
   void* const AtomicWrite_var;
#endif
  const Dcm_ExtendedDIDConfig_tst * adrExtendedConfig_pcst; /* Extended Configuration */
} Dcm_DIDConfig_tst;

typedef struct
{
  Std_ReturnType (*adrDIDAvailableFnc_pfct) ( uint16 DID, Dcm_OpStatusType OpStatus,
                                                       Dcm_DidSupportedType * supported); /* IsDidAvailable function pointer */
  Std_ReturnType (*adrReadFnc_pfct) ( uint16 DID,
                                             uint8 * const Data,
                                             Dcm_OpStatusType OpStatus,
                                             uint16 * const DataLength,
                                             Dcm_NegativeResponseCodeType * const ErrorCode
                                            );      /* ReadDIDdata function pointer */

  un_ReadDataLength adrReadDataLengthFnc_pfct; /* ReadDIDdataLength function pointer */
  un_WriteDidFnc adrWriteFnc_pfct; /* WriteDIDdata function pointer */
  const Dcm_ExtendedDIDConfig_tst * adrExtendedConfig_pcst; /* Extended Configuration */
  uint16 nrDidRangeUpperLimit_u16;      /* Upper limit of DID range */
  uint16 nrDidRangeLowerLimit_u16;      /* Lower limit of DID range */
  uint16 dataMaxDidLen_u16;                 /* Maximum data length in bytes */
  boolean dataRangeHasGaps_b;               /* Dynamically Defined DID */
  boolean nrRangeUsePort_b;             /* Fixed length or obtainable using ReadDataLengthFnc */
} Dcm_DIDRangeConfig_tst;

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if(DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType Dcm_GetActiveRDBIDid(uint16 * dataDid_u16);
#endif
#if(DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType Dcm_GetActiveWDBIDid(uint16 * dataDid_u16);
#endif
#if(DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType Dcm_GetActiveIOCBIDid(uint16 * dataDid_u16);
#endif
#if(DCM_CFG_DSP_READDATABYPERIODICIDENTIFIER_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType Dcm_RdpiIsDidCondtionChkReadSupported(Dcm_DIDIndexType_tst * idxDidIndexType_st);
#endif
#if(DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType Dcm_GetActiveDDDid(uint16 * dataDid_u16);
#endif
extern uint16 Dcm_DIDcalculateTableSize_u16(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if(DCM_CFG_ATOMICREAD_DID>0)
extern const AtomicRead_st AtomicDidRead_cast[DCM_CFG_ATOMICREAD_DID];
#endif
#if(DCM_CFG_ATOMICWRITE_DID>0)
extern const AtomicWrite_st AtomicDidWrite_cast[DCM_CFG_ATOMICWRITE_DID];
#endif
extern const Dcm_DIDConfig_tst Dcm_DIDConfig [];
extern const Dcm_DataInfoConfig_tst Dcm_DspDataInfo_st[];
extern const Dcm_SignalDIDSubStructConfig_tst Dcm_DspDid_ControlInfo_st[];
#if ((DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF))
extern const Dcm_SignalDIDIocbiConfig_tst Dcm_DspIOControlInfo[];
#endif
extern const Dcm_DIDRangeConfig_tst Dcm_DIDRangeConfig_cast[];
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROEDID_ENABLED != DCM_CFG_OFF))
extern const DcmDspRoeDidEvents_tst DcmDspRoeDidEvents[DCM_MAX_DIDROEEVENTS];
#endif
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF))
extern const DcmDspRoeOnDtcStatusChg_tst DcmDspRoeDtcEvent;
#endif
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROEDID_ENABLED != DCM_CFG_OFF))
extern DcmDspRoeDidStateVariables_tst DcmDspRoeDidStateVariables[DCM_MAX_DIDROEEVENTS];
#endif
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) &&  (DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF))
extern DcmDspRoeDtcStateVariable_tst DcmDspRoeDtcStateVariable;
#endif
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern uint16 Dcm_DidSignalIdx_u16;
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Dcm_OpStatusType Dcm_DspDataOpstatus_u8;  /* Variable to store the opstatus*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern boolean Dcm_flgDspDidRangePending_b;
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* temporary solution--> to be removed together with RDPI */
extern boolean Dcm_PeriodicSchedulerRunning_b;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


#if ((DCM_CFG_DSP_ROEDID_ENABLED != DCM_CFG_OFF))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_ResetRoeDidevents(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

extern uint8 Dcm_RoeDidIdx_u8;

#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#if(( DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF ) && (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_Dsp_SecaClearSeed (void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif /*( DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF ) && (DCM_CFG_DSP_SECA_STORESEED != DCM_CFG_OFF)*/
#if ( ( DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF ) && ( DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF ))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern void Dcm_ResetAccessType (void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#if((DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF))
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
extern Std_ReturnType Dcm_VinBuffer_Init(void);
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#endif /* DCM_CFG_DSPUDSSUPPORT_ENABLED */
#endif /* _DCMDSPUDS_UDS_PROT_H  */

