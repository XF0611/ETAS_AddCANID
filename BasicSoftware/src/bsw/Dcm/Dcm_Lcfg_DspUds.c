

#include "Dcm_Cfg_Prot.h"
#include "Dcm.h"
#include "Rte_Dcm.h"
#include "Rte_Dcm.h"
#include "DcmDspUds_Iocbi_Inf.h"

#include "DcmDspUds_Uds_Prot.h"
#include "rba_BswSrv.h"
#include "SchM_Dcm.h"


#include "ComM_Dcm.h"

#include "DcmDspUds_Rc_Prot.h"


#include "DcmDspUds_Rdtc_Priv.h"

#include "DcmDspUds_Er_Prot.h"



#include "DcmDspUds_Cdtcs_Prot.h"




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
		5000000,				/* P2* Max time in us */
		0x1,				/* Session ID */
		RTE_MODE_DcmDiagnosticSessionControl_DEFAULT_SESSION, /* DcmDiagnosticSessionControl Mode  for the Session Level */
		DCM_NO_BOOT			/* Diagnostic session used for jump to Bootloader */
	},

   /* session configuration for PROGRAMMING_SESSION*/
   {
		50000,				/* P2 Max time in us */
		5000000,				/* P2* Max time in us */
		0x2,				/* Session ID */
		RTE_MODE_DcmDiagnosticSessionControl_PROGRAMMING_SESSION, /* DcmDiagnosticSessionControl Mode  for the Session Level */
		DCM_OEM_BOOT			/* Diagnostic session used for jump to Bootloader */
	},

   /* session configuration for EXTENDED_DIAGNOSTIC_SESSION*/
   {
		50000,				/* P2 Max time in us */
		5000000,				/* P2* Max time in us */
		0x3,				/* Session ID */
		RTE_MODE_DcmDiagnosticSessionControl_EXTENDED_DIAGNOSTIC_SESSION, /* DcmDiagnosticSessionControl Mode  for the Session Level */
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
   /* security configuration for UnlockedL1*/
   {
   
		1000,			/* Delay timer on Power On in DcmTaskTime Counts*/

		1000,			/* Delay time after failed security access in DcmTaskTime Counts */

        {.Dcm_GetSeed_ptr3 = &Rte_Call_SecurityAccess_UnlockedL1_GetSeed}, /* RTE Port for the GetSeed Function */
        {.Dcm_CompareKey_ptr1 = &Rte_Call_SecurityAccess_UnlockedL1_CompareKey}, /* RTE Port for the Compare Key Function */
		0x1, 		/* Security Level */
		4,			/* Security Seed size */
		4,			/* Security Key size */
		3,			/* Number of failed security access after which delay time is active */
		0,			/* Number of failed security access after which security is locked */
		0,			/* Size of the Access Data Record in Seed Request */
		USE_ASYNCH_CLIENT_SERVER,				  		  FALSE /* Flexible length handling is not needed for this security level */
	}
,
   /* security configuration for Unlocked_L3*/
   {
   
		1000,			/* Delay timer on Power On in DcmTaskTime Counts*/

		1000,			/* Delay time after failed security access in DcmTaskTime Counts */

        {.Dcm_GetSeed_ptr3 = &Rte_Call_SecurityAccess_Unlocked_L3_GetSeed}, /* RTE Port for the GetSeed Function */
        {.Dcm_CompareKey_ptr1 = &Rte_Call_SecurityAccess_Unlocked_L3_CompareKey}, /* RTE Port for the Compare Key Function */
		0x3, 		/* Security Level */
		4,			/* Security Seed size */
		4,			/* Security Key size */
		3,			/* Number of failed security access after which delay time is active */
		0,			/* Number of failed security access after which security is locked */
		0,			/* Size of the Access Data Record in Seed Request */
		USE_ASYNCH_CLIENT_SERVER,				  		  FALSE /* Flexible length handling is not needed for this security level */
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
    Routine_Control_203_DcmDspStartRoutineOutSignal_0_StrtOut,
    Routine_Control_ABFF_DcmDspRequestRoutineResultsOutSignal_0_ReqRsltOut,
    DCM_RC_SIGOUTUINT8MAX
} Dcm_DspRoutineSigOutUInt8Idx_ten;

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint8 Dcm_RCSigOut_au8[2];
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

	
    {
        (void)dataSigVal_u32;
        (void)idxSignalIndex_u16;
    }
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"





static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_Routine_Control_203_StrtOutSig[]=
{
   {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
	/*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
	    Routine_Control_203_DcmDspStartRoutineOutSignal_0_StrtOut,           /* Index of the signal */
	    DCM_OPAQUE,       /* Signal Endianness is OPAQUE*/
	   DCM_UINT8  /* Data Type is UINT8 */      
    }
};





















static const Dcm_DspRoutineSignalInfo_tst DcmDspRc_Routine_Control_ABFF_ReqRsltOutSig[]=
{
    {
        0,             /* Start bit position of the signal */
        8,               /* length of the signal */
    /*MR12 RULE 10.3 VIOLATION:This is required for implememtaion as expression of essentially enum type is being converted to unsigned type on assignment.*/
        Routine_Control_ABFF_DcmDspRequestRoutineResultsOutSignal_0_ReqRsltOut,            /* Index of the signal */
        DCM_OPAQUE,       /* Signal Endianness is OPAQUE*/
        DCM_UINT8  /* Data Type is UINT8 */    
    }
};





static const Dcm_DspRoutineInfoType_tst DcmDspRc_Routine_Control_203=
{
    /* Allowed Security Row Bit Mask for Routine*/
  0xFFFFFFFFuL,

    /* Allowed Session Row Bit Mask for Routine */
     0x4uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    NULL_PTR,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    DcmDspRc_Routine_Control_203_StrtOutSig,
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


static const Dcm_DspRoutineInfoType_tst DcmDspRc_X_Routine_Control_ABFE=
{
    /* Allowed Security Row Bit Mask for Routine*/
  0xFFFFFFFFuL,

    /* Allowed Session Row Bit Mask for Routine */
     0x4uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    NULL_PTR,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    NULL_PTR,
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
    0,
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
    0,
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
    0,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    0
};


static const Dcm_DspRoutineInfoType_tst DcmDspRc_Routine_Control_ABFF=
{
    /* Allowed Security Row Bit Mask for Routine*/
  0xFFFFFFFFuL,

    /* Allowed Session Row Bit Mask for Routine */
     0x4uL,
     NULL_PTR,       /*  No User defined Mode rule Function configured  */
    /* Reference to In Signal from StartRoutine */
    NULL_PTR,
    /* Reference to In Signal from StopRoutine */
    NULL_PTR,
/* Reference to In Signal from RequestResultRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StartRoutine */
    NULL_PTR,
    /* Reference to Out Signal from StopRoutine */
    NULL_PTR,
    /* Reference to Out Signal from RequestResultsRoutine */
    DcmDspRc_Routine_Control_ABFF_ReqRsltOutSig,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* (Maximum) Size of Control Option Record i.e. Optional Bytes in Request for RequestResults Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* (Maximum) Size of Status Option Record i.e. Optional Bytes in Response for RequestResults Routine */
    1,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Start Routine */

    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for Stop Routine */
    0,
    /* Minimum Size of Control Option Record i.e. Optional Bytes in Request for RequestResult Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Start Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request Stop Routine */
    0,
    /* Minimum Size of Status Option Record i.e. Optional Bytes in Response for Request RequestResults Routine */
    1,
    /* Number of In Signals configured for StartRoutine */
    0,
    /* Number of In Signals configured for StopRoutine */
    0,
/* Number of In Signals configured for ReqResultsRoutine */
    0,
    /* Number of Out Signals configured for StartRoutine */
    0,
    /* Number of Out Signals configured for StopRoutine */
    0,
    /* Number of Out Signals configured for RequestResultsRoutine */
    1
};






#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static Std_ReturnType Dcm_Dsp_RC_Routine_Control_203_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = Rte_Call_RoutineServices_Routine_Control_203_Start
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[Routine_Control_203_DcmDspStartRoutineOutSignal_0_StrtOut]),
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}


static Std_ReturnType Dcm_Dsp_RC_X_Routine_Control_ABFE_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = Rte_Call_RoutineServices_X_Routine_Control_ABFE_Start
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCNegResCode_u8)
                  );

        break;



    default:
        Dcm_RCNegResCode_u8 = DCM_E_SUBFUNCTIONNOTSUPPORTED;
        break;
    }

    return (dataRetVal_u8);
}


static Std_ReturnType Dcm_Dsp_RC_Routine_Control_ABFF_Func ( uint8 dataSubFunc_u8 )
{
    Std_ReturnType dataRetVal_u8;
    dataRetVal_u8 = E_NOT_OK;

    switch (dataSubFunc_u8)
    {
    case 1u:
        dataRetVal_u8 = Rte_Call_RoutineServices_Routine_Control_ABFF_Start
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCNegResCode_u8)
                  );

        break;


    case 3u:
        dataRetVal_u8 = Rte_Call_RoutineServices_Routine_Control_ABFF_RequestResults
                (
                    Dcm_RCOpStatus_u8,
                    &(Dcm_RCSigOut_au8[Routine_Control_ABFF_DcmDspRequestRoutineResultsOutSignal_0_ReqRsltOut]),
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

    /* Routine_Control_203 */
    {
        0x203,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        TRUE,       /* UsePort is set to TRUE, call will be to RTE ports */
        &DcmDspRc_Routine_Control_203,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_Routine_Control_203_Func,        /* Reference to Call-out function */

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
    /* X_Routine_Control_ABFE */
    {
        0xabfe,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        TRUE,       /* UsePort is set to TRUE, call will be to RTE ports */
        &DcmDspRc_X_Routine_Control_ABFE,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_X_Routine_Control_ABFE_Func,        /* Reference to Call-out function */

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
    /* Routine_Control_ABFF */
    {
        0xabff,        /* Routine Identifier */
        TRUE,      /* Fixed length */
        TRUE,       /* UsePort is set to TRUE, call will be to RTE ports */
        &DcmDspRc_Routine_Control_ABFF,      /* Reference to Routine Info */
        &Dcm_Dsp_RC_Routine_Control_ABFF_Func,        /* Reference to Call-out function */

        TRUE,       /* Is Start Routine supported ? */

        FALSE,      /* Is Stop Routine supported ? */
        FALSE,       /* Stop this routine if active on Session change */
        TRUE,       /* Is RequestSequnceError supported for RequestResults subfunction for Routine which is not started ? */
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),      /*Configuration mask indicating the availability of routine in different DCMConfigsets*/
#endif
        TRUE,       /* Is Request Results Routine supported ? */
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












 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
 #include "Dcm_MemMap.h"
 
 /* Handling of Read APIs as per SWS 4.2 */
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x0100_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x0100_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Year(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Year_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Month(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Month_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Day(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Day_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Hour(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Hour_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Minute(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Minute_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x010B_Second(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x010B_Second_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0x0112_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0x0112_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xB950_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xB950_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xD001_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xD001_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xD901_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xD901_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xE101_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xE101_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xE900_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xE900_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF100_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF100_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF100_New_Data_Object_1(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF100_New_Data_Object_1_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF110_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF110_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF111_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF111_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF112_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF112_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF113_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF113_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF114_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF114_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF115_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF115_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF116_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF116_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF117_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF117_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF118_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF118_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF119_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF119_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11A_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11A_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11B_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11B_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11C_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11C_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11D_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11D_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11E_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11E_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF11F_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF11F_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF120_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF120_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF121_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF121_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF122_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF122_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF122_New_Data_Object_2(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF122_New_Data_Object_2_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF187_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF187_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF18A_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF18A_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF18B_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF18B_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF18C_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF18C_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF190_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF190_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF192_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF192_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF192_New_Data_Object_3(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF192_New_Data_Object_3_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF194_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF194_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF194_New_Data_Object_4(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF194_New_Data_Object_4_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF198_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF198_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF198_New_Data_Object_5(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF198_New_Data_Object_5_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A0_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A0_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A1_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A1_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A2_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A2_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A5_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A5_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A8_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A8_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1A9_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1A9_New_Data_Object_Data(Data));
}
                
              
static Std_ReturnType Dcm_ReadDidSignal_DID_0xF1AA_New_Data_Object(uint8 * Data)
{
     return(Rte_Read_R_DataServices_DID_0xF1AA_New_Data_Object_Data(Data));
}
                
 
  
 
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




    


 /* Handling of Write APIs as per SWS 4.2 */
 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
 #include "Dcm_MemMap.h"
static Std_ReturnType Dcm_WriteDidSignal_DID_0x0100_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0x0100_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF110_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF110_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF115_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF115_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF121_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF121_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF187_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF187_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF18B_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF18B_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF18C_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF18C_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF190_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF190_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF198_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF198_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF198_New_Data_Object_5(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF198_New_Data_Object_5_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF1A8_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF1A8_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF1A9_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF1A9_New_Data_Object_Data(Data));
}
static Std_ReturnType Dcm_WriteDidSignal_DID_0xF1AA_New_Data_Object(const uint8 * Data)
{
    return(Rte_Write_P_DataServices_DID_0xF1AA_New_Data_Object_Data(Data));
}
 
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"




    


/*Handling of  Sender receiver supported IOCBI DIDs*/
 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
 #include "Dcm_MemMap.h"

/*Handling of  Sender receiver supported IOCBI DIDs as per SWS 4.2.0*/



static Std_ReturnType Dcm_IOControlRequestDidSignal_DID_0xD901_New_Data_Object( Dcm_InputOutputControlParameterType IoctrlParam,
                                                                                     const uint8 * RequestData,
                                                                                     uint16 dataSignalLength_u16,
										     uint8 controlMaskLength_u8,
										     Dcm_OpStatusType IocbiOpStatus_u8,
                                                                                     Dcm_NegativeResponseCodeType * ErrorCode);

static Std_ReturnType Dcm_IOControlRequestDidSignal_DID_0xD901_New_Data_Object( Dcm_InputOutputControlParameterType IoctrlParam,
                                                                                     const uint8 * RequestData,
                                                                                     uint16 dataSignalLength_u16,
										      uint8 controlMaskLength_u8,
										     Dcm_OpStatusType IocbiOpStatus_u8,
                                                                                     Dcm_NegativeResponseCodeType * ErrorCode)

{
    Std_ReturnType dataReturnValue_u8;
    static Dcm_IOControlRequest_DID_0xD901_New_Data_ObjectType IOControlReq_DID_0xD901_New_Data_Object;
    static Dcm_IOControlResponseType IOControlResponse;
    dataReturnValue_u8 =0x00;
    if (RequestData !=NULL_PTR)
    {
       if(IocbiOpStatus_u8 != DCM_PENDING)
         {
              IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter = IoctrlParam;
              if(IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter == DCM_SHORT_TERM_ADJUSTMENT)
	             {
	                if(controlMaskLength_u8==0x01u)
	                {
	                 IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
	                }
	                else if(controlMaskLength_u8==0x02u)
	                {
	                IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
	                IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
	                }
	                else if(controlMaskLength_u8==0x03u)
	                {
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+2u)));	                
	                }
	                else
	                {
	                IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+2u)));   
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+3u)));
	                }
	                
	                 DCM_MEMCOPY(&IOControlReq_DID_0xD901_New_Data_Object.controlState,RequestData,dataSignalLength_u16);
	             }
              else
                 {
                    if(controlMaskLength_u8==0x01u)
                    {
                     IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    }
                    else if(controlMaskLength_u8==0x02u)
                    {
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    }
                    else if(controlMaskLength_u8==0x03u)
                    {
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+2u)));                   
                    }
                    else
                    {
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+2u)));   
                    IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+3u)));
                    }
                    
                 }
                IOControlResponse.IOOperationResponse = DCM_IDLE;
                (void)(Rte_Write_IOControlRequest_DID_0xD901_New_Data_Object_IOControlRequest_DID_0xD901_New_Data_Object(&IOControlReq_DID_0xD901_New_Data_Object));
         }
        (void) (Rte_Read_IOControlResponse_DID_0xD901_New_Data_Object_IOOperationResponse(&IOControlResponse.IOOperationResponse));
        if(IOControlResponse.IOOperationResponse == DCM_POSITIVE_RESPONSE )
         {
                dataReturnValue_u8 =E_OK;
                if (IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter == DCM_RETURN_CONTROL_TO_ECU)
                  {
                     IOControlReq_DID_0xD901_New_Data_Object.undercontrol = (IOControlReq_DID_0xD901_New_Data_Object.undercontrol &  (~IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask));
                     IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                  }
                else
                  {
                    IOControlReq_DID_0xD901_New_Data_Object.undercontrol = (IOControlReq_DID_0xD901_New_Data_Object.undercontrol | IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask);
                     IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                  }
         }
      else if(IOControlResponse.IOOperationResponse == DCM_RESPONSE_PENDING)
         {
                 dataReturnValue_u8 =DCM_E_PENDING;
         }
     else if (IOControlResponse.IOOperationResponse == DCM_IDLE)
         {
                 dataReturnValue_u8 =DCM_E_PENDING;
         }
     else
         {
                *ErrorCode = IOControlResponse.IOOperationResponse;
                 IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                 dataReturnValue_u8 =E_NOT_OK;
         }
   }
     else
        {

            /*Requesting the application to return control to ECU during a session transition or securrity level change  */
             IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_RETURN_CONTROL_TO_ECU;
             
            IOControlReq_DID_0xD901_New_Data_Object.IOOperationRequest.controlEnableMask = 0xFFFFFFFF;
            
            IOControlReq_DID_0xD901_New_Data_Object.undercontrol = 0x00;
            (void)(Rte_Write_IOControlRequest_DID_0xD901_New_Data_Object_IOControlRequest_DID_0xD901_New_Data_Object(&IOControlReq_DID_0xD901_New_Data_Object));

         }

  return(dataReturnValue_u8);

}


static Std_ReturnType Dcm_IOControlRequestDidSignal_DID_0xE900_New_Data_Object( Dcm_InputOutputControlParameterType IoctrlParam,
                                                                                     const uint8 * RequestData,
                                                                                     uint16 dataSignalLength_u16,
										     uint8 controlMaskLength_u8,
										     Dcm_OpStatusType IocbiOpStatus_u8,
                                                                                     Dcm_NegativeResponseCodeType * ErrorCode);

static Std_ReturnType Dcm_IOControlRequestDidSignal_DID_0xE900_New_Data_Object( Dcm_InputOutputControlParameterType IoctrlParam,
                                                                                     const uint8 * RequestData,
                                                                                     uint16 dataSignalLength_u16,
										      uint8 controlMaskLength_u8,
										     Dcm_OpStatusType IocbiOpStatus_u8,
                                                                                     Dcm_NegativeResponseCodeType * ErrorCode)

{
    Std_ReturnType dataReturnValue_u8;
    static Dcm_IOControlRequest_DID_0xE900_New_Data_ObjectType IOControlReq_DID_0xE900_New_Data_Object;
    static Dcm_IOControlResponseType IOControlResponse;
    dataReturnValue_u8 =0x00;
    if (RequestData !=NULL_PTR)
    {
       if(IocbiOpStatus_u8 != DCM_PENDING)
         {
              IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter = IoctrlParam;
              if(IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter == DCM_SHORT_TERM_ADJUSTMENT)
	             {
	                if(controlMaskLength_u8==0x01u)
	                {
	                 IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
	                }
	                else if(controlMaskLength_u8==0x02u)
	                {
	                IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
	                IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
	                }
	                else if(controlMaskLength_u8==0x03u)
	                {
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+2u)));	                
	                }
	                else
	                {
	                IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData+dataSignalLength_u16);
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+1u)));
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+2u)));   
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+dataSignalLength_u16+3u)));
	                }
	                
	                 DCM_MEMCOPY(&IOControlReq_DID_0xE900_New_Data_Object.controlState,RequestData,dataSignalLength_u16);
	             }
              else
                 {
                    if(controlMaskLength_u8==0x01u)
                    {
                     IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    }
                    else if(controlMaskLength_u8==0x02u)
                    {
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    }
                    else if(controlMaskLength_u8==0x03u)
                    {
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+2u)));                   
                    }
                    else
                    {
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = *(RequestData);
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+1u)));
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+2u)));   
                    IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = ((IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask <<8)|(*(RequestData+3u)));
                    }
                    
                 }
                IOControlResponse.IOOperationResponse = DCM_IDLE;
                (void)(Rte_Write_IOControlRequest_DID_0xE900_New_Data_Object_IOControlRequest_DID_0xE900_New_Data_Object(&IOControlReq_DID_0xE900_New_Data_Object));
         }
        (void) (Rte_Read_IOControlResponse_DID_0xE900_New_Data_Object_IOOperationResponse(&IOControlResponse.IOOperationResponse));
        if(IOControlResponse.IOOperationResponse == DCM_POSITIVE_RESPONSE )
         {
                dataReturnValue_u8 =E_OK;
                if (IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter == DCM_RETURN_CONTROL_TO_ECU)
                  {
                     IOControlReq_DID_0xE900_New_Data_Object.undercontrol = (IOControlReq_DID_0xE900_New_Data_Object.undercontrol &  (~IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask));
                     IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                  }
                else
                  {
                    IOControlReq_DID_0xE900_New_Data_Object.undercontrol = (IOControlReq_DID_0xE900_New_Data_Object.undercontrol | IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask);
                     IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                  }
         }
      else if(IOControlResponse.IOOperationResponse == DCM_RESPONSE_PENDING)
         {
                 dataReturnValue_u8 =DCM_E_PENDING;
         }
     else if (IOControlResponse.IOOperationResponse == DCM_IDLE)
         {
                 dataReturnValue_u8 =DCM_E_PENDING;
         }
     else
         {
                *ErrorCode = IOControlResponse.IOOperationResponse;
                 IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_IDLE;
                 dataReturnValue_u8 =E_NOT_OK;
         }
   }
     else
        {

            /*Requesting the application to return control to ECU during a session transition or securrity level change  */
             IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.inputOutputControlParameter = DCM_RETURN_CONTROL_TO_ECU;
             
            IOControlReq_DID_0xE900_New_Data_Object.IOOperationRequest.controlEnableMask = 0xFFFFFFFF;
            
            IOControlReq_DID_0xE900_New_Data_Object.undercontrol = 0x00;
            (void)(Rte_Write_IOControlRequest_DID_0xE900_New_Data_Object_IOControlRequest_DID_0xE900_New_Data_Object(&IOControlReq_DID_0xE900_New_Data_Object));

         }

  return(dataReturnValue_u8);

}
 

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



/*Handling of  Sender receiver supported IOCBI DIDs*/
 








/**
 **********************************************************************************************************************
           DID Signal Substructure Configuration for IOCBI
 **********************************************************************************************************************
**/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_SignalDIDIocbiConfig_tst Dcm_DspIOControlInfo[3]=
{
    {
            {NULL_PTR},
            {NULL_PTR},
            {NULL_PTR},
            {NULL_PTR},
            {NULL_PTR}    
    },
    {
            {.IOControlrequest_pfct = &Dcm_IOControlRequestDidSignal_DID_0xD901_New_Data_Object},            /* IOControlRequest Function */
            {NULL_PTR},           /* Return Control To Ecu Function */   
            {NULL_PTR},           /* Reset To Default Function */
            {NULL_PTR},           /* Freeze Current State Function */
            {NULL_PTR}            /* Short Term Adjustment Function */
        
    }
, 
    {
            {.IOControlrequest_pfct = &Dcm_IOControlRequestDidSignal_DID_0xE900_New_Data_Object},            /* IOControlRequest Function */
            {NULL_PTR},           /* Return Control To Ecu Function */   
            {NULL_PTR},           /* Reset To Default Function */
            {NULL_PTR},           /* Freeze Current State Function */
            {NULL_PTR}            /* Short Term Adjustment Function */
        
    }
};
  
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/**
 **********************************************************************************************************************
           DID Signal Substructure Configuration for condition check for read and write and read datalength function
 **********************************************************************************************************************
**/
#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
const Dcm_SignalDIDSubStructConfig_tst Dcm_DspDid_ControlInfo_st[54]=
{
    {
        {NULL_PTR},
        {NULL_PTR}
,
        {NULL_PTR},         
        NULL_PTR,
        NULL_PTR
,
        0
    },



    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0x0100_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

                1    /*Index to DcmDspControlInfoStructure*/   /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

                2    /*Index to DcmDspControlInfoStructure*/   /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF110_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF115_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF121_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF187_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF18B_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF18C_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF190_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF198_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF198_New_Data_Object_5},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {NULL_PTR},           /* Write Data Function */
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF1A8_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF1A9_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
    }
, 
    {

            {NULL_PTR},           /* Condition Check Read Function*/
            {NULL_PTR},           /* Read Data Length Function */
            {.WdbiFnc12_pfct = &Dcm_WriteDidSignal_DID_0xF1AA_New_Data_Object},        /* Write Data Function */  
            NULL_PTR,          /* Write Data type variable */
            NULL_PTR,          /* Get Signal function to copy data */

            0                                                /*IOCBI Sub structure address*/
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
const Dcm_DataInfoConfig_tst Dcm_DspDataInfo_st [53]=
{
    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x0100_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           1,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Day},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           4,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Hour},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           5,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Minute},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           6,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Month},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           3,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Second},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           7,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x010B_Year},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           2,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0x0112_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           8,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xB950_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           8,                                               /*Signal Data Byte Size */
           
           9,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xD001_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           10,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xD901_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           11,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xE101_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           3,                                               /*Signal Data Byte Size */
           
           12,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xE900_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           1,                                               /*Signal Data Byte Size */
           
           13,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF100_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           14,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF100_New_Data_Object_1},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           6,                                               /*Signal Data Byte Size */
           
           15,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF110_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           16,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF111_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           17,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF112_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           18,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF113_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           19,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF114_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           20,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF115_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           21,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF116_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           22,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF117_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           23,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF118_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           24,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF119_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           25,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11A_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           26,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11B_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           27,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11C_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           28,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11D_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           29,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11E_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           30,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF11F_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           31,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF120_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           32,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF121_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           33,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF122_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           34,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF122_New_Data_Object_2},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           35,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF187_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           36,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF18A_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           37,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF18B_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           3,                                               /*Signal Data Byte Size */
           
           38,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF18C_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           16,                                               /*Signal Data Byte Size */
           
           39,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF190_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           17,                                               /*Signal Data Byte Size */
           
           40,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF192_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           41,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF192_New_Data_Object_3},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           42,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF194_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           43,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF194_New_Data_Object_4},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           44,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF198_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           45,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF198_New_Data_Object_5},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           6,                                               /*Signal Data Byte Size */
           
           46,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A0_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           47,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A1_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           48,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A2_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           8,                                               /*Signal Data Byte Size */
           
           49,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A5_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           3,                                               /*Signal Data Byte Size */
           
           50,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A8_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           20,                                               /*Signal Data Byte Size */
           
           51,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1A9_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           52,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
    }
    
    
,    
     {   
            {.ReadFunc1_pfct = &Dcm_ReadDidSignal_DID_0xF1AA_New_Data_Object},           /* Read Data Function */
            NULL_PTR,                                               /* Read Data type variable */
    
            NULL_PTR,                                                   /* API to store and assemble all signals data */
    
    
           5,                                               /*Signal Data Byte Size */
           
           53,    /*Index to DcmDspControlInfoStructure*/
           DCM_UINT8,                    /* Data Type is UINT8*/
	       USE_DATA_SENDER_RECEIVER,      /*DataUsePort is USE_DATA_SENDER_RECEIVER*/
      
     
     
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


/* DID DID_0x0100 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_100_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        0,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0x010B signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_10B_SigConf[6]=
{
 /* Signal Year */
    {
        0,    /* Signal Byte Position */
        6,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal Month */
    {
        1,    /* Signal Byte Position */
        4,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal Day */
    {
        2,    /* Signal Byte Position */
        1,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal Hour */
    {
        3,    /* Signal Byte Position */
        2,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal Minute */
    {
        4,    /* Signal Byte Position */
        3,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal Second */
    {
        5,    /* Signal Byte Position */
        5,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0x0112 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_112_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        7,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xB950 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_B950_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        8,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xD001 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_D001_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        9,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xD901 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_D901_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        10,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xE101 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_E101_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        11,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xE900 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_E900_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        12,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF100 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F100_SigConf[2]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        13,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal New_Data_Object_1 */
    {
        5,    /* Signal Byte Position */
        14,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF110 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F110_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        15,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF111 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F111_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        16,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF112 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F112_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        17,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF113 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F113_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        18,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF114 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F114_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        19,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF115 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F115_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        20,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF116 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F116_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        21,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF117 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F117_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        22,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF118 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F118_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        23,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF119 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F119_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        24,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11A signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11A_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        25,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11B signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11B_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        26,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11C signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11C_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        27,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11D signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11D_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        28,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11E signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11E_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        29,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF11F signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F11F_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        30,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF120 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F120_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        31,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF121 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F121_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        32,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF122 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F183_SigConf[2]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        33,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal New_Data_Object_2 */
    {
        5,    /* Signal Byte Position */
        34,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF187 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F187_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        35,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF18A signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F18A_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        36,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF18B signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F18B_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        37,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF18C signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F18C_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        38,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF190 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F190_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        39,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF192 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F192_SigConf[2]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        40,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal New_Data_Object_3 */
    {
        5,    /* Signal Byte Position */
        41,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF194 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F194_SigConf[2]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        42,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal New_Data_Object_4 */
    {
        5,    /* Signal Byte Position */
        43,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF198 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F198_SigConf[2]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        44,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
, 
 /* Signal New_Data_Object_5 */
    {
        5,    /* Signal Byte Position */
        45,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A0 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A0_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        46,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A1 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A1_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        47,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A2 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A2_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        48,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A5 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A5_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        49,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A8 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A8_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        50,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1A9 signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1A9_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        51,
        NULL_PTR,
        NULL_PTR           /* Write Data Function */
    }
};

/* DID DID_0xF1AA signal configuration */
static const Dcm_SignalDIDConfig_tst DcmDspDid_F1AA_SigConf[1]=
{
 /* Signal New_Data_Object */
    {
        0,    /* Signal Byte Position */
        52,
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
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0x0100_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0x010B_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0x0112_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xB950_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xD001_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xD901_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed IO control Session levels */
    0xFFFFFFFFuL, /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_EXTERNAL,                                   /*Control mask is configured as external, control mask handling to be done by Application*/
        0x4,                                   /*Control mask size in bytes*/
        TRUE,            /* If Enabled allows the IOcontrol(IOCBI DIDS) to be reset on session/security change */
    
        0xD
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xE101_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xE900_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed IO control Session levels */
    0xFFFFFFFFuL, /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_EXTERNAL,                                   /*Control mask is configured as external, control mask handling to be done by Application*/
        0x4,                                   /*Control mask size in bytes*/
        TRUE,            /* If Enabled allows the IOcontrol(IOCBI DIDS) to be reset on session/security change */
    
        0xD
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF100_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF110_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF111_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF112_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF113_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF114_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF115_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF116_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF117_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF118_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF119_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11A_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11B_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11C_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11D_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11E_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF11F_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF120_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF121_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF122_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF187_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF18A_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF18B_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF18C_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF190_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF192_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF194_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF198_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A0_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A1_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A2_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A5_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed Write Session levels */
        0x0uL,           /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A8_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1A9_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
};
static const Dcm_ExtendedDIDConfig_tst Did_extendedConfig_DID_0xF1AA_info=
{
    
    0xffffffffuL, /* Allowed Read Session levels */
    0xFFFFFFFFuL, /* Allowed Read Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
    0x4uL, /* Allowed Write Session levels */
    0x2uL, /* Allowed Write Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        0x0uL,          /* Allowed IO control Session levels */
        0x0uL,          /* Allowed IO control Security levels */
        NULL_PTR,       /*  No User defined Mode rule Function configured  */
        DCM_CONTROLMASK_INTERNAL,                                         /*Default value of Control mask*/
        0x0,                                         /*Control mask size in bytes*/
        FALSE,          /* If disabled does not the IOcontrol(IOCBI DIDS) to be reset on session/security change */
        0x00u
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
        0x100,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_100_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0x0100_info   /*ExtendedConfiguration*/
    }


,
    {
        0x10B,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        6,                          /*No of Signals*/
        6,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_10B_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0x010B_info   /*ExtendedConfiguration*/
    }


,
    {
        0x112,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        1,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_112_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0x0112_info   /*ExtendedConfiguration*/
    }


,
    {
        0xB950,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        8,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_B950_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xB950_info   /*ExtendedConfiguration*/
    }


,
    {
        0xD001,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        1,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_D001_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xD001_info   /*ExtendedConfiguration*/
    }


,
    {
        0xD901,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        1,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_D901_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xD901_info   /*ExtendedConfiguration*/
    }


,
    {
        0xE101,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        3,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_E101_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xE101_info   /*ExtendedConfiguration*/
    }


,
    {
        0xE900,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        1,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_E900_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xE900_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF100,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        2,                          /*No of Signals*/
        11,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F100_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF100_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF110,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F110_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF110_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF111,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F111_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF111_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF112,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F112_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF112_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF113,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F113_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF113_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF114,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F114_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF114_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF115,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F115_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF115_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF116,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F116_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF116_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF117,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F117_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF117_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF118,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F118_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF118_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF119,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F119_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF119_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11A,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11A_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11A_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11B,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11B_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11B_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11C,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11C_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11C_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11D,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11D_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11D_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11E,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11E_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11E_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF11F,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F11F_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF11F_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF120,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F120_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF120_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF121,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F121_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF121_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF183,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        2,                          /*No of Signals*/
        10,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F183_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF122_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF187,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F187_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF187_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF18A,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F18A_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF18A_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF18B,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        3,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F18B_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF18B_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF18C,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        16,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F18C_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF18C_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF190,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        17,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F190_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF190_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF192,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        2,                          /*No of Signals*/
        10,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F192_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF192_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF194,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        2,                          /*No of Signals*/
        10,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F194_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF194_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF198,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        2,                          /*No of Signals*/
        11,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F198_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF198_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A0,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A0_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A0_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A1,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A1_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A1_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A2,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        8,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A2_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A2_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A5,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        3,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A5_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A5_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A8,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        20,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A8_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A8_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1A9,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1A9_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1A9_info   /*ExtendedConfiguration*/
    }


,
    {
        0xF1AA,                        /*DID*/
        USE_DATA_ELEMENT_SPECIFIC_INTERFACES,      /*DidUsePort*/
        FALSE,       /* Flag to indicate Atomic/new SenderReceiver communication as per SWS 4.4.0 or SenderReceiver communication communication as per SWS 4.2.0 */
        1,                          /*No of Signals*/
        5,                            /*TotalByteSize*/
        TRUE,                      /*FixedLength*/
        FALSE,                      /*DynamicallyDefined*/
        DcmDspDid_F1AA_SigConf,        /*DidSignalRef*/
#if (DCM_CFG_POSTBUILD_SUPPORT != DCM_CFG_OFF)
        ((DCM_CFG_CONFIGSET1)|(DCM_CFG_CONFIGSET2)|(DCM_CFG_CONFIGSET3)|(DCM_CFG_CONFIGSET4)|
         (DCM_CFG_CONFIGSET5)|(DCM_CFG_CONFIGSET6)|(DCM_CFG_CONFIGSET7)|(DCM_CFG_CONFIGSET8)),                          /*DidConfigurationMask indicating availability of DID in different configuration sets*/
#endif

        {NULL_PTR},        /* IOControlRequest Function*/


        &Did_extendedConfig_DID_0xF1AA_info   /*ExtendedConfiguration*/
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
        RTE_MODE_DcmEcuReset_HARD,         /* DcmEcuReset Mode  for the ResetType */
        0x1,                              /* ResetType */
        DCM_RESET_NO_BOOT

    },

    {
        RTE_MODE_DcmEcuReset_SOFT,         /* DcmEcuReset Mode  for the ResetType */
        0x3,                              /* ResetType */
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

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

static boolean Dcm_Can_Network_ETAS_Channel_Can_Network_ETAS_IsModeDefault ( void )
{
	boolean dataRetValue_b;

	if ( SchM_Mode_Dcm_R_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS() != RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_TX_NORM_NM )
	{
		dataRetValue_b = FALSE;
	}
	else
	{
		dataRetValue_b = TRUE;
	}
	return (dataRetValue_b);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* Function to map Dcm_CommunicationModeType to Rte_Communication Mode type for Can_Network_ETAS_Channel_Can_Network_ETAS */
static Std_ReturnType Dcm_Can_Network_ETAS_Channel_Can_Network_ETAS_SwitchIndication ( Dcm_CommunicationModeType Mode )
{
	Std_ReturnType dataRetValue_u8;
	switch (Mode)
	{
		case DCM_ENABLE_RX_TX_NORM: dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS( RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_TX_NORM);
			break;
		case DCM_ENABLE_RX_DISABLE_TX_NORM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_DISABLE_TX_NORM);
			break;
		case DCM_DISABLE_RX_ENABLE_TX_NORM: dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_ENABLE_TX_NORM);
			break;
		case DCM_DISABLE_RX_TX_NORMAL:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_TX_NORM);
			break;
		case DCM_ENABLE_RX_TX_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_TX_NM);
			break;
		case DCM_ENABLE_RX_DISABLE_TX_NM: dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS( RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_DISABLE_TX_NM);
			break;
		case DCM_DISABLE_RX_ENABLE_TX_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_ENABLE_TX_NM);
			break;
		case DCM_DISABLE_RX_TX_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_TX_NM);
			break;
		case DCM_ENABLE_RX_TX_NORM_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_TX_NORM_NM);
			break;
		case DCM_ENABLE_RX_DISABLE_TX_NORM_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_DISABLE_TX_NORM_NM);
			break;
		case DCM_DISABLE_RX_ENABLE_TX_NORM_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_ENABLE_TX_NORM_NM);
			break;
		case DCM_DISABLE_RX_TX_NORM_NM:  dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_DISABLE_RX_TX_NORM_NM);
			break;
		default: dataRetValue_u8 = SchM_Switch_Dcm_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS(RTE_MODE_DcmCommunicationControl_Can_Network_ETAS_Channel_Can_Network_ETAS_DCM_ENABLE_RX_TX_NORM);
			break;
	}
	return (dataRetValue_u8);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/*Array  stores the channelID info which is to be informed about the communication mode change if the subnet ID is 0  */
const Dcm_Dsld_AllChannelsInfoType Dcm_AllChannels_ForModeInfo[DCM_CFG_NUM_ALLCHANNELS_MODE_INFO]=
{
	{
		&Dcm_Can_Network_ETAS_Channel_Can_Network_ETAS_SwitchIndication,  	/* Auto generated Dcm function to be called for invoking SchM Switch Indication */
		&Dcm_Can_Network_ETAS_Channel_Can_Network_ETAS_IsModeDefault,		/* Auto generated Dcm function to be called for checking if Active Mode is Default */
 		ComMConf_ComMChannel_Can_Network_ETAS_Channel_Can_Network_ETAS
	}
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"





/**
 ***************************************************************************************************
        Clear Diagnostic Information Service (0x14)
 ***************************************************************************************************
*/











