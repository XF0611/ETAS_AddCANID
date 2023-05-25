

#include "Dcm_Cfg_Prot.h"
#include "Dcm.h"
#include "Rte_Dcm.h"

#include "DcmDspUds_Uds_Prot.h"
#include "rba_BswSrv.h"


#include "ComM_Dcm.h"

#include "DcmDspUds_Rc_Prot.h"



#include "DcmDspUds_Er_Prot.h"



#include "DcmDspUds_Cdtcs_Prot.h"


#include "DcmDspUds_RequestDownload_Prot.h"

#include "DcmAppl.h"

#include "DcmCore_DslDsd_Prot.h"




/**
 ***************************************************************************************************
            Session Control (DSC) Service
 ***************************************************************************************************
*/
/* Initialization of the parameters for the supported sessions */
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_Dsp_Session_t Dcm_Dsp_Session[DCM_CFG_DSP_NUMSESSIONS] =
{

   /* session configuration for DEFAULT_SESSION*/
   {
		50000,				/* P2 Max time in us */
		2000000,				/* P2* Max time in us */
		0x1,				/* Session ID */
		DCM_NO_BOOT			/* Diagnostic session used for jump to Bootloader */
	},

   /* session configuration for PROGRAMMING_SESSION*/
   {
		50000,				/* P2 Max time in us */
		2000000,				/* P2* Max time in us */
		0x2,				/* Session ID */
		DCM_NO_BOOT			/* Diagnostic session used for jump to Bootloader */
	},

   /* session configuration for EXTENDED_DIAGNOSTIC_SESSION*/
   {
		50000,				/* P2 Max time in us */
		2000000,				/* P2* Max time in us */
		0x3,				/* Session ID */
		DCM_NO_BOOT			/* Diagnostic session used for jump to Bootloader */
	}
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



/**
 ***************************************************************************************************
            Security Access (SECA) Service
 ***************************************************************************************************
*/
/* Initialization of the parameters for the supported security */
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_Dsp_Security_t Dcm_Dsp_Security[DCM_CFG_DSP_NUMSECURITY] =
{
   /* security configuration for DSP_SEC_FBL*/
   {
   
        0x00u,          /* Delay timer on Power On in DcmTaskTime Counts*/   
        0x00u,          /* Delay time after failed security access in DcmTaskTime Counts */

		{.Dcm_GetSeed_ptr4 = &DcmGetSeedDSP_Fbl_ProgM_GetSeedCallback}, /* Function for the GetSeed Function */
		{.Dcm_CompareKey_ptr3 = &DcmCompareKeyDSP_Fbl_ProgM_CompareKeyCallback}, /* Function for the Compare Key Function */
		0x9, 		/* Security Level */
		4,			/* Security Seed size */
		4,			/* Security Key size */
		3,			/* Number of failed security access after which delay time is active */
		0,			/* Number of failed security access after which security is locked */
		0,			/* Size of the Access Data Record in Seed Request */
		USE_ASYNCH_FNC,				  		  FALSE /* Flexible length handling is not needed for this security level */
	}

};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"















#if (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)
/**
 ***************************************************************************************************
            Routine control (RC) service - start
 ***************************************************************************************************
 */




#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint16 Dcm_RCSigOutN_as16[1];
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint32 Dcm_RCSigOutN_as32[1];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint8 Dcm_RCSigOutN_as8[1];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_RCSigOutN_au32[1];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_RCSigOutN_au16[1];
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


typedef enum
{
    DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,
    DcmDspRoutine_StartEraseMemory_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,
    DcmDspRoutine_CheckProgrammingDependencies_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,
    DcmDspRoutine_CheckProgrammingPreconditions_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,
    DCM_RC_SIGOUTUINT8MAX
} Dcm_DspRoutineSigOutUInt8Idx_ten;

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_RCSigOut_au8[4];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 Dcm_RCSigOutN_au8[1];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"






#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint16 Dcm_RCSigInN_as16[1];
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint32 Dcm_RCSigInN_as32[1];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
sint8 Dcm_RCSigInN_as8[1];
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

typedef enum
{
    DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineInSignal_CrcTest_StrtIn,
    DcmDspRoutine_StartEraseMemory_StartAddress_StrtIn,
    DcmDspRoutine_StartEraseMemory_Length_StrtIn,
    DCM_RC_SIGINUINT32MAX
} Dcm_DspRoutineSigInUInt32Idx_ten;

#define DCM_START_SEC_VAR_CLEARED_32/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint32 Dcm_RCSigIn_au32[3];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_32/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint32 Dcm_RCSigInN_au32[1];
#define DCM_STOP_SEC_VAR_CLEARED_32 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_RCSigInN_au16[1];
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


typedef enum
{
    DcmDspRoutine_StartEraseMemory_AddressAndLengthFormatIdentifier_StrtIn,
    DCM_RC_SIGINUINT8MAX
} Dcm_DspRoutineSigInUInt8Idx_ten;

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_RCSigIn_au8[1];
#define DCM_STOP_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 Dcm_RCSigInN_au8[1];
#define DCM_STOP_SEC_VAR_CLEARED_8/*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

uint32 Dcm_RCGetSigVal_u32 ( uint8 dataSigType_en, uint16 idxSignalIndex_u16 )
{
    uint32 dataSigVal_u32;
   
 if(dataSigType_en == DCM_UINT8)
    {
        dataSigVal_u32 = (uint32)Dcm_RCSigOut_au8[idxSignalIndex_u16];
    }
	else	
	{
        dataSigVal_u32 = 0u;
	}

    return (dataSigVal_u32);
}


void Dcm_RCSetSigVal ( uint8 dataSigType_en, uint16 idxSignalIndex_u16, uint32 dataSigVal_u32)
{

 if(dataSigType_en == DCM_UINT32)
    {
        Dcm_RCSigIn_au32[idxSignalIndex_u16]=dataSigVal_u32;
    }
	else if(dataSigType_en == DCM_UINT8)
    {
        Dcm_RCSigIn_au8[idxSignalIndex_u16]=(uint8)dataSigVal_u32;
    }
	else	
    {
        (void)dataSigVal_u32;
        (void)idxSignalIndex_u16;
    }
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_StartCheckRoutine_StrtInSig[]=
{
     {
        0,            /* Start bit position of the signal */
        32,               /* length of the signal */
         /*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
        DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineInSignal_CrcTest_StrtIn,            /* Index of the signal */
        DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
        DCM_UINT32  /* Data Type is UINT32 */             
    }
};



static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_StartCheckRoutine_StrtOutSig[]=
{
   {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
	/*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
	    DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,           /* Index of the signal */
	    DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
	   DCM_UINT8  /* Data Type is UINT8 */      
    }
};






static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_StartEraseMemory_StrtInSig[]=
{
     {
        0,            /* Start bit position of the signal */
        8,               /* length of the signal */
         /*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
        DcmDspRoutine_StartEraseMemory_AddressAndLengthFormatIdentifier_StrtIn,            /* Index of the signal */
        DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
        DCM_UINT8  /* Data Type is UINT8 */             
    }
,
     {
        8,            /* Start bit position of the signal */
        32,               /* length of the signal */
         /*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
        DcmDspRoutine_StartEraseMemory_StartAddress_StrtIn,            /* Index of the signal */
        DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
        DCM_UINT32  /* Data Type is UINT32 */             
    }
,
     {
        40,            /* Start bit position of the signal */
        32,               /* length of the signal */
         /*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
        DcmDspRoutine_StartEraseMemory_Length_StrtIn,            /* Index of the signal */
        DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
        DCM_UINT32  /* Data Type is UINT32 */             
    }
};



static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_StartEraseMemory_StrtOutSig[]=
{
   {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
	/*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
	    DcmDspRoutine_StartEraseMemory_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,           /* Index of the signal */
	    DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
	   DCM_UINT8  /* Data Type is UINT8 */      
    }
};









static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_CheckProgrammingDependencies_StrtOutSig[]=
{
   {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
	/*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
	    DcmDspRoutine_CheckProgrammingDependencies_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,           /* Index of the signal */
	    DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
	   DCM_UINT8  /* Data Type is UINT8 */      
    }
};









static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_DcmDspRoutine_CheckProgrammingPreconditions_StrtOutSig[]=
{
   {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
	/*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
	    DcmDspRoutine_CheckProgrammingPreconditions_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut,           /* Index of the signal */
	    DCM_BIG_ENDIAN,       /* Signal Endianness is BIG_ENDIAN*/
	   DCM_UINT8  /* Data Type is UINT8 */      
    }
};








static const Dcm_DspRoutineInfoType_tst DcmDspRc_DcmDspRoutine_StartCheckRoutine=
{
    /* Allowed Security Row Bit Mask for Routine*/
   0x2uL,

    /* Allowed Session Row Bit Mask for Routine */
     0x2uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_StartCheckRoutine_StrtInSig,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_StartCheckRoutine_StrtOutSig,
    /* Reference to Out Signal from StopRoutine */
    NULL_PTR,
    /* Reference to Out Signal from RequestResultsRoutine */
    NULL_PTR,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    4,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for RequestResults Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for RequestResults Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    4,

    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for RequestResult Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request RequestResults Routine */
    0,
    /* Number of In Signals configured for StartRoutine */
    1,
    /* Number of In Signals configured for StopRoutine */
    0,
/* Number of In Signals configured for ReqResultsRoutine */
    0,
    /* Number of Out Signals configured for StartRoutine */
    1,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    0
};


static const Dcm_DspRoutineInfoType_tst DcmDspRc_DcmDspRoutine_StartEraseMemory=
{
    /* Allowed Security Row Bit Mask for Routine*/
   0x2uL,

    /* Allowed Session Row Bit Mask for Routine */
     0x2uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_StartEraseMemory_StrtInSig,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_StartEraseMemory_StrtOutSig,
    /* Reference to Out Signal from StopRoutine */
    NULL_PTR,
    /* Reference to Out Signal from RequestResultsRoutine */
    NULL_PTR,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    9,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for RequestResults Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for RequestResults Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    9,

    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for RequestResult Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request RequestResults Routine */
    0,
    /* Number of In Signals configured for StartRoutine */
    3,
    /* Number of In Signals configured for StopRoutine */
    0,
/* Number of In Signals configured for ReqResultsRoutine */
    0,
    /* Number of Out Signals configured for StartRoutine */
    1,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    0
};


static const Dcm_DspRoutineInfoType_tst DcmDspRc_DcmDspRoutine_CheckProgrammingDependencies=
{
    /* Allowed Security Row Bit Mask for Routine*/
   0x2uL,

    /* Allowed Session Row Bit Mask for Routine */
     0x2uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    NULL_PTR,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_CheckProgrammingDependencies_StrtOutSig,
    /* Reference to Out Signal from StopRoutine */
    NULL_PTR,
    /* Reference to Out Signal from RequestResultsRoutine */
    NULL_PTR,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for RequestResults Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for RequestResults Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */

    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for RequestResult Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request RequestResults Routine */
    0,
    /* Number of In Signals configured for StartRoutine */
    0,
    /* Number of In Signals configured for StopRoutine */
    0,
/* Number of In Signals configured for ReqResultsRoutine */
    0,
    /* Number of Out Signals configured for StartRoutine */
    1,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    0
};


static const Dcm_DspRoutineInfoType_tst DcmDspRc_DcmDspRoutine_CheckProgrammingPreconditions=
{
    /* Allowed Security Row Bit Mask for Routine*/
  0xFFFFFFFFuL,

    /* Allowed Session Row Bit Mask for Routine */
  0xFFFFFFFFuL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    NULL_PTR,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    DcmDspRc_DcmDspRoutine_CheckProgrammingPreconditions_StrtOutSig,
    /* Reference to Out Signal from StopRoutine */
    NULL_PTR,
    /* Reference to Out Signal from RequestResultsRoutine */
    NULL_PTR,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for RequestResults Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for RequestResults Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */

    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for RequestResult Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    1,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request RequestResults Routine */
    0,
    /* Number of In Signals configured for StartRoutine */
    0,
    /* Number of In Signals configured for StopRoutine */
    0,
/* Number of In Signals configured for ReqResultsRoutine */
    0,
    /* Number of Out Signals configured for StartRoutine */
    1,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    0
};






#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Std_ReturnType Dcm_Dsp_RC_DcmDspRoutine_StartCheckRoutine_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = Fbl_ProgM_StartCheckRoutineCallback
                (
Dcm_RCSigIn_au32[DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineInSignal_CrcTest_StrtIn],
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[DcmDspRoutine_StartCheckRoutine_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut]),
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}


static Std_ReturnType Dcm_Dsp_RC_DcmDspRoutine_StartEraseMemory_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = SWDL_StartEraseMemoryCallback_0xFF00
                (
Dcm_RCSigIn_au8[DcmDspRoutine_StartEraseMemory_AddressAndLengthFormatIdentifier_StrtIn],
Dcm_RCSigIn_au32[DcmDspRoutine_StartEraseMemory_StartAddress_StrtIn],
Dcm_RCSigIn_au32[DcmDspRoutine_StartEraseMemory_Length_StrtIn],
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[DcmDspRoutine_StartEraseMemory_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut]),
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}


static Std_ReturnType Dcm_Dsp_RC_DcmDspRoutine_CheckProgrammingDependencies_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = SWDL_CheckProgrammingDependenciesCallback_0xFF01
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[DcmDspRoutine_CheckProgrammingDependencies_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut]),
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}


static Std_ReturnType Dcm_Dsp_RC_DcmDspRoutine_CheckProgrammingPreconditions_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = SWDL_CheckProgrammingPreconditionsCallback_0x0215
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[DcmDspRoutine_CheckProgrammingPreconditions_DcmDspStartRoutineOutSignal_StatusRecord_StrtOut]),
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* Array with function pointers configured for RIDs to check if they are supported in the current variant */

const Dcm_DspRoutineType_tst Dcm_DspRoutine_cast[DCM_CFG_RC_NUMRIDS]=
{

    /* DcmDspRoutine_StartCheckRoutine */
    {
        0x202,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        FALSE,      /* UsePort is set to FALSE, call will be to Configured call-back */
        &DcmDspRc_DcmDspRoutine_StartCheckRoutine,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_DcmDspRoutine_StartCheckRoutine_Func,        /* Reference to Call-out function */

        TRUE,       /* Is Start Routine supported ? */

        FALSE,      /* Is Stop Routine supported ? */
        FALSE,       /* Stop this routine if active on Session change */
        TRUE,       /* Is RequestSequnceError supported for RequestResults subfunction for Routine which is not started ? */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),      /*Configuration mask indicating the availability of routine in different DCMConfigsets*/
#endif
        FALSE,      /* Is Request Results Routine supported ? */
    }
,
    /* DcmDspRoutine_CheckProgrammingPreconditions */
    {
        0x215,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        FALSE,      /* UsePort is set to FALSE, call will be to Configured call-back */
        &DcmDspRc_DcmDspRoutine_CheckProgrammingPreconditions,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_DcmDspRoutine_CheckProgrammingPreconditions_Func,        /* Reference to Call-out function */

        TRUE,       /* Is Start Routine supported ? */

        FALSE,      /* Is Stop Routine supported ? */
        FALSE,       /* Stop this routine if active on Session change */
        TRUE,       /* Is RequestSequnceError supported for RequestResults subfunction for Routine which is not started ? */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),      /*Configuration mask indicating the availability of routine in different DCMConfigsets*/
#endif
        FALSE,      /* Is Request Results Routine supported ? */
    }
,
    /* DcmDspRoutine_StartEraseMemory */
    {
        0xff00,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        FALSE,      /* UsePort is set to FALSE, call will be to Configured call-back */
        &DcmDspRc_DcmDspRoutine_StartEraseMemory,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_DcmDspRoutine_StartEraseMemory_Func,        /* Reference to Call-out function */

        TRUE,       /* Is Start Routine supported ? */

        FALSE,      /* Is Stop Routine supported ? */
        FALSE,       /* Stop this routine if active on Session change */
        TRUE,       /* Is RequestSequnceError supported for RequestResults subfunction for Routine which is not started ? */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),      /*Configuration mask indicating the availability of routine in different DCMConfigsets*/
#endif
        FALSE,      /* Is Request Results Routine supported ? */
    }
,
    /* DcmDspRoutine_CheckProgrammingDependencies */
    {
        0xff01,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        FALSE,      /* UsePort is set to FALSE, call will be to Configured call-back */
        &DcmDspRc_DcmDspRoutine_CheckProgrammingDependencies,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_DcmDspRoutine_CheckProgrammingDependencies_Func,        /* Reference to Call-out function */

        TRUE,       /* Is Start Routine supported ? */

        FALSE,      /* Is Stop Routine supported ? */
        FALSE,       /* Stop this routine if active on Session change */
        TRUE,       /* Is RequestSequnceError supported for RequestResults subfunction for Routine which is not started ? */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),      /*Configuration mask indicating the availability of routine in different DCMConfigsets*/
#endif
        FALSE,      /* Is Request Results Routine supported ? */
    }
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Array for storing current status of each RID*/
Dcm_DspRoutineStatusType_ten Dcm_RoutineStatus_aen[DCM_CFG_RC_NUMRIDS];
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




/**
 ***************************************************************************************************
            Routine control (RC) service - end
 ***************************************************************************************************
 */
 
#endif












 



    


 



    


/*Handling of  Sender receiver supported IOCBI DIDs*/
 


/*Handling of  Sender receiver supported IOCBI DIDs*/
 








/**
 **********************************************************************************************************************
           DID Signal Substructure Configuration for condition check for read and write and read datalength function
 **********************************************************************************************************************
**/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_SignalDIDSubStructConfig_tst Dcm_DspDid_ControlInfo_st[4]=
{
    {
        {NULL_PTR},
        {NULL_PTR}
,
        {NULL_PTR},         
        NULL_PTR,
        NULL_PTR
    },



    {

            {.CondChkReadFunc2_pfct = NULL_PTR},          /* Condition Check Read Function */
            {.ReadDataLengthFnc4_pf = NULL_PTR},          /* Read Data Length Function */
            {.WdbiFnc4_pfct = &DID_BootSoftwareFingerprintDataIdentifier_0xF183_WriteFunc},         /* Write Data Function */ 
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

    }
, 
    {

            {.CondChkReadFunc2_pfct = NULL_PTR},          /* Condition Check Read Function */
            {.ReadDataLengthFnc4_pf = NULL_PTR},          /* Read Data Length Function */
            {.WdbiFnc4_pfct = &DID_ApplicationSoftwareFingerprintDataIdentifier_0xF184_WriteFunc},         /* Write Data Function */ 
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

    }
, 
    {

            {.CondChkReadFunc2_pfct = NULL_PTR},          /* Condition Check Read Function */
            {.ReadDataLengthFnc4_pf = NULL_PTR},          /* Read Data Length Function */
            {.WdbiFnc4_pfct = &DID_ApplicationDataFingerprintDataIdentifier_0xF185_WriteFunc},         /* Write Data Function */ 
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

    }

};


#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/**
 ***************************************************************************************************
           DID Signal Configuration
 ***************************************************************************************************
**/

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_DataInfoConfig_tst Dcm_DspDataInfo_st [5]=
{
    
     {   
            {.ReadFunc2_ptr = &DID_ApplicationDataFingerprintDataIdentifier_0xF185_ReadFunc},            /* Read Data Function */ 
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           9,                                                 /*Signal Data Byte Size */
           
           3,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                                          /* Data Type is UINT8 */
	       USE_DATA_ASYNCH_FNC,     /*DataUsePort is USE_DATA_ASYNCH_FNC*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc2_ptr = &DID_ApplicationSoftwareFingerprintDataIdentifier_0xF184_ReadFunc},            /* Read Data Function */ 
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           9,                                                 /*Signal Data Byte Size */
           
           2,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                                          /* Data Type is UINT8 */
	       USE_DATA_ASYNCH_FNC,     /*DataUsePort is USE_DATA_ASYNCH_FNC*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc2_ptr = &DID_BootSoftwareFingerprintDataIdentifier_0xF183_ReadFunc},            /* Read Data Function */ 
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           9,                                                 /*Signal Data Byte Size */
           
           1,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                                          /* Data Type is UINT8 */
	       USE_DATA_ASYNCH_FNC,     /*DataUsePort is USE_DATA_ASYNCH_FNC*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc2_ptr = &DID_EcuSerialNumber_0xF18C_ReadFunc},            /* Read Data Function */ 
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           4,                                                 /*Signal Data Byte Size */
           
           0,                         /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                                          /* Data Type is UINT8 */
	       USE_DATA_ASYNCH_FNC,     /*DataUsePort is USE_DATA_ASYNCH_FNC*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc2_ptr = &DcmDspData_VIN_ReadFunc},            /* Read Data Function */ 
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           17,                                                 /*Signal Data Byte Size */
           
           0,                         /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                                          /* Data Type is UINT8 */
	       USE_DATA_ASYNCH_FNC,     /*DataUsePort is USE_DATA_ASYNCH_FNC*/
      
     
     
    }
    
    
};






#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



/**
 ***************************************************************************************************
           DID Signal Configuration
 ***************************************************************************************************
**/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/* DID DcmDspDid_BootSoftwareFingerprintDataIdentifier signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F183_SigConf[1]=
{
 /* Signal DcmDspDidSignal */
    {
        0,    /* Signal Byte Position */
        2,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DcmDspDid_ApplicationSoftwareFingerprintDataIdentifier signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F184_SigConf[1]=
{
 /* Signal DcmDspDidSignal */
    {
        0,    /* Signal Byte Position */
        1,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DcmDspDid_ApplicationDataFingerprintDataIdentifier signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F185_SigConf[1]=
{
 /* Signal DcmDspDidSignal */
    {
        0,    /* Signal Byte Position */
        0,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DcmDspDid_VIN signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F190_SigConf[1]=
{
 /* Signal DcmDspDidSignal */
    {
        0,    /* Signal Byte Position */
        4,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DcmDspDid_EcuSerialNumber signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F18C_SigConf[1]=
{
 /* Signal DcmDspDidSignal_EcuSerialNumber */
    {
        0,    /* Signal Byte Position */
        3,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};


#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/**
 ***************************************************************************************************
           DID Extended Configuration
 ***************************************************************************************************
*/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DcmDspDidInfo_BootSoftwareFingerprintDataIdentifier=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x6uL, /* Allowed Write Session levels */
    0xFFFFFFFFuL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DcmDspDidInfo_ApplicationSoftwareFingerprintDataIdentifier=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x6uL, /* Allowed Write Session levels */
    0xFFFFFFFFuL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DcmDspDidInfo_ApplicationDataFingerprintDataIdentifier=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x6uL, /* Allowed Write Session levels */
    0xFFFFFFFFuL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DcmDspDidInfo_VIN=
{
    
    0x1uL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DcmDspDidInfo_EcuSerialNumber=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



/**
 ***************************************************************************************************
           DID Configuration Structure
 ***************************************************************************************************
*/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



const Dcm_DIDConfig_tst Dcm_DIDConfig []=
{

    {
        0xF183,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        9,                                /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F183_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DcmDspDidInfo_BootSoftwareFingerprintDataIdentifier   /*ExtendedConfiguration*/
    }


,
    {
        0xF184,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        9,                                /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F184_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DcmDspDidInfo_ApplicationSoftwareFingerprintDataIdentifier   /*ExtendedConfiguration*/
    }


,
    {
        0xF185,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        9,                                /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F185_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DcmDspDidInfo_ApplicationDataFingerprintDataIdentifier   /*ExtendedConfiguration*/
    }


,
    {
        0xF18C,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        4,                                /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F18C_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DcmDspDidInfo_EcuSerialNumber   /*ExtendedConfiguration*/
    }


,
    {
        0xF190,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        17,                                /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F190_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DcmDspDidInfo_VIN   /*ExtendedConfiguration*/
    }



};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_DIDcalculateTableSize_u16(void)
{
  return ((uint16)(sizeof(Dcm_DIDConfig))/(uint16)(sizeof(Dcm_DIDConfig_tst)));
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"







#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_ControlDtcSettingModecheck_b(
/* MR12 RULE 8.13 VIOLATION: The object addressed by NegRespCode_u8 will be modified depending on different configurations */
Dcm_NegativeResponseCodeType * NegRespCode_u8)
{
    Std_ReturnType retVal_u8;
    boolean retVal_b;

    /* Call the DcmAppl API to check if the DTC Setting needs to be re-enabled */
    retVal_u8 =DcmAppl_UserDTCSettingEnableModeRuleService();

    if(retVal_u8!=E_OK)
    {
        (void)NegRespCode_u8;
        retVal_b = FALSE;
    }
    else
    {
        (void)NegRespCode_u8;
        retVal_b = TRUE;
    }
    return (retVal_b);

}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




/**
 ***************************************************************************************************
            Ecu Reset (ER) Service
 ***************************************************************************************************
*/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* Initialization of the parameters for the supported Reset Types */
const Dcm_DspEcuReset_tst Dcm_DspEcuResetType_cast[DCM_CFG_DSP_NUMRESETTYPE] =
{

    {
        0x1,                              /* ResetType */
        DCM_RESET_NO_BOOT

    }
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"





/**
 ***************************************************************************************************
        Communication Control Service
 ***************************************************************************************************
*/

#define DCM_START_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Std_ReturnType (*Dcm_ComMUserReEnableModeRuleRef) (void) = &DcmAppl_UserCommCtrlReEnableModeRuleService;
#define DCM_STOP_SEC_VAR_INIT_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/*Array  stores the channelID info which is to be informed about the communication mode change if the subnet ID is 0  */
const Dcm_Dsld_AllChannelsInfoType Dcm_AllChannels_ForModeInfo[DCM_CFG_NUM_ALLCHANNELS_MODE_INFO]=
{
	{
 		ComMConf_ComMChannel_ComMChannel_Can_Network_Channel
	}
,
	{
 		ComMConf_ComMChannel_ComMChannel_Eth_Network_Channel
	}
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"














/*
****************************************************************************************************
          RequestUpload Configuration Structure
****************************************************************************************************
*/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_RequestDownloadConfig_tst Dcm_RequestDownloadConfig_cast []=
{
    {
        0x0,     /*Lower memory address range allowed for download*/
        0xFFFFFFFE,     /*Lower memory address range allowed for download*/
        0xFFFFFFFFuL,     /* Allowed Write Security levels */
        0           /*Value of Memory Identifier to select the desired memory device*/
    }

};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint16 Dcm_RequestDownloadCalculateTableSize_u16(void)
{
  return ((uint16)(sizeof(Dcm_RequestDownloadConfig_cast))/(uint16)(sizeof(Dcm_RequestDownloadConfig_tst)));
}
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"











