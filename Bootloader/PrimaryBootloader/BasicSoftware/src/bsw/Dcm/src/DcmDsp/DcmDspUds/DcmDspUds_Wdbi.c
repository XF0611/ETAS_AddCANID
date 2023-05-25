

#include "DcmDspUds_Wdbi_Inf.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)

#include "DcmDspUds_Wdbi_Priv.h"
#include "DcmDspUds_Wdbi_Inf.h"
/* Global varibale declaration */
#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_OpStatusType Dcm_stDspWdbiOpStatus_u8;
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean Dcm_flgNvmWritePending_b;          /* variable to hold the NVM write status*/
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DIDIndexType_tst s_Dcm_idxwdbiDidIndexType;    /*Store the index of DID of either Dcm_DidConfig or Dcm_DIDRangeCOnfig structure*/
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static uint16 s_dataDID_u16; /*Store the DID value sent from the tester */
#define DCM_STOP_SEC_VAR_CLEARED_16 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)

static boolean s_loopbreak_b;

#endif



static Dcm_SupportRet_t dataRetWriteSupport_en;      /* Return from Dcm_GetSupportOfIndex */



static Std_ReturnType dataRetWriteFunc_u8;


/***************************************************************************************************
* Dcm_Prv_WDBIINIT_Pending : Initialisaton function for Write Data By Identifier (0x2E) Service when
*                            request is pending
*
* \param           None
*
* \retval          None
* \seealso
* \usedresources
**************************************************************************************************
*/
static void Dcm_Prv_WDBIINIT_Pending(void)
        {
    Dcm_NegativeResponseCodeType dataNegResCode;
#if((DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF)||   \
        (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF))
    const Dcm_DIDConfig_tst * ptrDidConfig;
    const Dcm_DataInfoConfig_tst * ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst * ptrControlSigConfig;
#endif
    dataNegResCode = 0x0;

#if((DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF)||   \
        (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF))

    ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
        ptrSigConfig  = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
        ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];

#if(DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)

        if(ptrSigConfig->idxDcmDspControlInfo_u16 > 0)
        {
            /* If the DID use the Asynchronous operations */
            if((ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc1_pfct != NULL_PTR) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)
            #if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
            ||(ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC)
            #endif
            ))
            {
#if(DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF)
                /*Check if Fixed length or obtainable using ReadDataLengthFnc*/
                if(ptrDidConfig->dataFixedLength_b != FALSE)
                {
#if(DCM_CFG_DSP_WRITE_FIXED_LENGTH != DCM_CFG_OFF)
                    /* Call function to cancel the pending condition check operation for a DID */
                    (void)(*(ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc4_pfct))(NULL_PTR,DCM_CANCEL,&dataNegResCode);
#endif
                }
                else
                {
#if(DCM_CFG_DSP_WRITE_VARIABLE_LENGTH != DCM_CFG_OFF)
                    /* Call function to cancel the pending condition check operation for a DID */
                    (void)(*(ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc3_pfct))(NULL_PTR,0x0,DCM_CANCEL,&dataNegResCode);
#endif
                }
#endif
            }
#if(DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF)
            /*Check if write function is not NULL and use post is asynchronous*/
            else if ((ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc1_pfct != NULL_PTR)&& (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER))
            {
                /*Check if Fixed length or obtainable using ReadDataLengthFnc*/
                if(ptrDidConfig->dataFixedLength_b != FALSE)
                {
#if(DCM_CFG_DSP_WRITE_FIXED_LENGTH != DCM_CFG_OFF)
                    /* Call function to cancel the pending condition check operation for a DID */
                    (void)(*(ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc21_pfct))(NULL_PTR,DCM_CANCEL,&dataNegResCode);
#endif
                }
                else
                {
#if(DCM_CFG_DSP_WRITE_VARIABLE_LENGTH != DCM_CFG_OFF)
                    /* Call function to cancel the pending condition check operation for a DID */
                    (void)(*(ptrControlSigConfig->adrWriteFnc_cpv.WdbiFnc20_pfct))(NULL_PTR,0x0,DCM_CANCEL,&dataNegResCode);
#endif
                }

            }
#endif
            else
            {
                (void)dataNegResCode;
                /* Do nothing. Added to avoid MISRA warning */
            }
        }
        else
        {
            (void)dataNegResCode;
        }

#endif

#if ((DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF))
        if(ptrSigConfig->usePort_u8 == USE_BLOCK_ID)
        {
            /* Locking the unlocked block */
            NvM_SetBlockLockStatus(ptrSigConfig->dataNvmBlockId_u16,TRUE);
        }
#endif
#endif

        }
 /***************************************************************************************************
 * Dcm_Dcm_WDBIInit : Initialisaton function for Write Data By Identifier (0x2E) Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void Dcm_Dcm_WDBIInit (void)
{

#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
    const Dcm_DIDRangeConfig_tst * ptrDidRangeConfig;
    Dcm_NegativeResponseCodeType dataNegResCode=0u;
#endif

    dataRetWriteFunc_u8=0x00;

    /* Check if there are any pending operations and if the did is a range did or not */

    if((Dcm_stDspWdbiOpStatus_u8 == DCM_PENDING) && (s_Dcm_idxwdbiDidIndexType.dataRange_b== FALSE))
    {
       /* MR12 RULE 2.2 VIOLATION : The function is redundant with no side effects and this required for implementation. MISRA C:2012 Rule-2.2 */
       Dcm_Prv_WDBIINIT_Pending();

    }
    else
    {
#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
        ptrDidRangeConfig=&Dcm_DIDRangeConfig_cast[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
        /*Check if pointer to write function is not NULL and if the did is a range did or not */
        if ((ptrDidRangeConfig->adrWriteFnc_pfct.WriteFncRange1_pfct != NULL_PTR) && (s_Dcm_idxwdbiDidIndexType.dataRange_b!=FALSE))
        {



                (void)(*(ptrDidRangeConfig->adrWriteFnc_pfct.WriteFncRange1_pfct))(0x0,NULL_PTR,DCM_CANCEL,0x0,&dataNegResCode);

        }
        else
        {

            (void)dataNegResCode;

        }
#endif
    }
    /* Initialize the state of WDBI to IDLE */
    Dcm_SrvOpstatus_u8 =  DCM_INITIAL;
    /* Reset the Signal index to 0 */
    Dcm_DidSignalIdx_u16 = 0x0;
#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
    /*Update the flag Dcm_flgNvmWritePending_b is to FALSE*/
    Dcm_flgNvmWritePending_b = FALSE;
#endif
    /* Reset the OpStatus */
    Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
    Dcm_ResetDIDIndexstruct(&s_Dcm_idxwdbiDidIndexType);        /*This function is invoked to reset all the elements of DID index structure to its default value*/
}
//#if ((DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
#if(DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF)

/***********************************************************************************************************************
 Function name    : Dcm_Prv_SenderReceiverVariableSignalGetBuffer
 Syntax           : Dcm_Prv_SenderReceiverVariableSignalGetBuffer(pMsgContext,ptrDidConfig,ptrSigConfig,nrElements_u16)
 Description      : TRACE[[SWS_Dcm_00639],[SWS_Dcm_00687] This function gets the requested Write siganl fromthe request and
                    store into the buffer
 Parameter        : Dcm_DIDConfig_tst: Pointer to DID configuration structure
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    nrElements_u16:  size of signals data
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Prv_SenderReceiverVariableSignalGetBuffer
                            (const Dcm_MsgContextType * pMsgContext,
                             const Dcm_DIDConfig_tst * ptrDidConfig,
                             const Dcm_DataInfoConfig_tst * ptrSigConfig,
                             uint16 nrElements_u16
                             )
 {

    uint16 idxLoop_u16 = 0u; /* Number of elements in the signal */
    uint32 dataSignal_u32 = 0u;    /* counter to loop the array of signals */

    /*Local variable to store endianness of signal */
    #if( DCM_CFG_DSP_ALLSIGNAL_OPAQUE==DCM_CFG_OFF)
                        uint8 DataEndiannessType_u8 =ptrSigConfig->dataEndianness_u8;
    #else
                        uint8 DataEndiannessType_u8 =DCM_OPAQUE;
    #endif

    do
    {
        switch(ptrSigConfig->dataType_u8)
        {
#if(DCM_CFG_DSP_WRITE_UINT8_SR_ENABLED != DCM_CFG_OFF)
           case DCM_UINT8:
                /* BSW-17603 *//* [$DD_BSWCODE 40524] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                                (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*8)),\
                                 &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                                );
                /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_au8[idxLoop_u16]=(uint8)dataSignal_u32;
                break;
#endif

#if(DCM_CFG_DSP_WRITE_UINT16_SR_ENABLED != DCM_CFG_OFF)
           case DCM_UINT16:
                /* BSW-17603 *//* [$DD_BSWCODE 40525] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                              (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*16)),\
                              &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                              );
                /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_au16[idxLoop_u16]=(uint16)dataSignal_u32;
                break;
#endif

#if(DCM_CFG_DSP_WRITE_UINT32_SR_ENABLED != DCM_CFG_OFF)
           case DCM_UINT32:
                /* BSW-17603 *//* [$DD_BSWCODE 40526] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                              (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*32)),\
                              &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                              );
                /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_au32[idxLoop_u16]=dataSignal_u32;
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT8_SR_ENABLED != DCM_CFG_OFF)
           case DCM_SINT8:
                /* BSW-17603 *//* [$DD_BSWCODE 40527] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                               (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*8)),\
                               &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                               );
                /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_as8[idxLoop_u16]=(sint8)dataSignal_u32;
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT16_SR_ENABLED != DCM_CFG_OFF)
           case DCM_SINT16:
                /* BSW-17603 *//* [$DD_BSWCODE 40528] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                                 (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*16)),\
                                 &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                                 );
                 /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_as16[idxLoop_u16]=(sint16)dataSignal_u32;
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT32_SR_ENABLED != DCM_CFG_OFF)
           case DCM_SINT32:
                /* BSW-17603 *//* [$DD_BSWCODE 40529] */
                dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                                (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8 +(uint16)(idxLoop_u16*32)),\
                                &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                                );
                /* Copy the part of the signal to the local Signal array */
                Dcm_DspArraySignal_as32[idxLoop_u16]=(sint32)dataSignal_u32;
                break;
#endif

           default:
                  {
                  /*empty default*/
                  break;
                  }
             }
       idxLoop_u16++;
        /* loop for all the elements of the signal */
    }while((idxLoop_u16 < nrElements_u16));


}


/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteSenderReceiverVariableLengthSignal
 Syntax           : Dcm_Prv_WriteSenderReceiverVariableLengthSignal( pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc,nrElements_u16)
 Description      : TRACE[SWS_Dcm_00687]This function writes DID signal using SenderReceiver interface for Variable length signal
 Parameter        : Dcm_DIDConfig_tst: Pointer to DID configuration structure
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    ptrWriteFnc:  Pointer to Write function
                    nrElements_u16 : Size of Variable length Signal
 Return value     : none
***********************************************************************************************************************/

static void Dcm_Prv_WriteSenderReceiverVariableLengthSignal
                            (const Dcm_MsgContextType * pMsgContext,
                             const Dcm_DIDConfig_tst * ptrDidConfig,
                             const Dcm_DataInfoConfig_tst * ptrSigConfig,
                             un_adrWriteFnc ptrWriteFnc,
                             uint16 nrElements_u16)
 {
   /* Get the signal value from the request buffer */
   Dcm_Prv_SenderReceiverVariableSignalGetBuffer (pMsgContext,ptrDidConfig,ptrSigConfig,nrElements_u16);

  switch(ptrSigConfig->dataType_u8)
         {
#if(DCM_CFG_DSP_WRITE_UINT8_SR_ENABLED != DCM_CFG_OFF)
         /* Call the configured callback to write the signal */
         case DCM_UINT8:
            dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc12_pfct))(Dcm_DspArraySignal_au8);
            break;
#endif

#if(DCM_CFG_DSP_WRITE_UINT16_SR_ENABLED != DCM_CFG_OFF)
          /* Call the configured callback to write the signal */
         case DCM_UINT16:
            dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc13_pfct))(Dcm_DspArraySignal_au16);
            break;
#endif

#if(DCM_CFG_DSP_WRITE_UINT32_SR_ENABLED != DCM_CFG_OFF)
          /* Call the configured callback to write the signal */
         case DCM_UINT32:
             dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc14_pfct))(Dcm_DspArraySignal_au32);
            break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT8_SR_ENABLED != DCM_CFG_OFF)
            /* Call the configured callback to write the signal */
        case DCM_SINT8:
            dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc15_pfct))(Dcm_DspArraySignal_as8);
            break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT16_SR_ENABLED != DCM_CFG_OFF)
            /* Call the configured callback to write the signal */
        case DCM_SINT16:
            dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc16_pfct))(Dcm_DspArraySignal_as16);
            break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT32_SR_ENABLED != DCM_CFG_OFF)
            /* Call the configured callback to write the signal */
        case DCM_SINT32:
            dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc17_pfct))(Dcm_DspArraySignal_as32);
            break;
#endif

        default:
            {
            /*empty default*/
            break;
            }

         }
  }


/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteSenderReceiverFixedLengthSignal
 Syntax           : Dcm_Prv_WriteSenderReceiverFixedLengthSignal(pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc)
 Description      : TRACE[SWS_Dcm_00639],[SWS_Dcm_00687] This function writed DID signal using SenderReceiver interface
 Parameter        : Dcm_DIDConfig_tst: Pointer to DID configuration structure
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    ptrWriteFnc:  Pointer to Write function
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Prv_WriteSenderReceiverFixedLengthSignal
                            (const Dcm_MsgContextType * pMsgContext,
                             const Dcm_DIDConfig_tst * ptrDidConfig,
                             const Dcm_DataInfoConfig_tst * ptrSigConfig,
                             un_adrWriteFnc ptrWriteFnc)
 {

/* Get the signal value from the request buffer */
uint32 dataSignal_u32;
#if(DCM_CFG_DSP_WRITE_SINT32_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for sint32 signal */
    sint32 dataSignal_s32;
#endif
#if(DCM_CFG_DSP_WRITE_UINT16_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for uint16 signal */
    uint16 dataSignal_u16;
#endif
#if(DCM_CFG_DSP_WRITE_SINT16_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for sint16 signal */
    sint16 dataSignal_s16;
#endif
#if(DCM_CFG_DSP_WRITE_UINT8_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for uint8 signal */
    uint8 dataSignal_u8;
#endif
#if(DCM_CFG_DSP_WRITE_SINT8_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for sint8 signal */
    sint8 dataSignal_s8;
#endif
#if(DCM_CFG_DSP_WRITE_BOOLEAN_SR_ENABLED != DCM_CFG_OFF)
    /* Local variable for boolean signal */
    boolean dataSignal_b;
#endif

/*Local variable to store endianness of signal */
#if( DCM_CFG_DSP_ALLSIGNAL_OPAQUE==DCM_CFG_OFF)
                    uint8 DataEndiannessType_u8 =ptrSigConfig->dataEndianness_u8;
#else
                    uint8 DataEndiannessType_u8 =DCM_OPAQUE;
#endif

/* BSW-17603 *//* [$DD_BSWCODE 40530] */
dataSignal_u32 = Dcm_GetSignal_u32(ptrSigConfig->dataType_u8,\
                                   ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16*8,\
                                   &pMsgContext->reqData[0x02],
DataEndiannessType_u8

                                  );
   switch(ptrSigConfig->dataType_u8)
    {
#if(DCM_CFG_DSP_WRITE_BOOLEAN_SR_ENABLED != DCM_CFG_OFF)
        case DCM_BOOLEAN:
                /* Check if the signal value if True/false */
                dataSignal_b = (dataSignal_u32 != 0u) ? (TRUE):(FALSE);
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc5_pfct))(dataSignal_b);
                break;

#endif
#if(DCM_CFG_DSP_WRITE_UINT8_SR_ENABLED != DCM_CFG_OFF)
        case DCM_UINT8:
                 /* Extract uint8 value from uint32 */
                 dataSignal_u8 = (uint8)dataSignal_u32;
                 dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc6_pfct))(dataSignal_u8);
                 break;
#endif
#if(DCM_CFG_DSP_WRITE_UINT16_SR_ENABLED != DCM_CFG_OFF)
        case DCM_UINT16:
                /* Extract uint16 value from uint32 */
                dataSignal_u16 = (uint16)dataSignal_u32;
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc7_pfct))(dataSignal_u16);
                break;
#endif
#if(DCM_CFG_DSP_WRITE_UINT32_SR_ENABLED != DCM_CFG_OFF)

        case DCM_UINT32:
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc8_pfct))(dataSignal_u32);
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT8_SR_ENABLED != DCM_CFG_OFF)
        case DCM_SINT8:
                /* Extract sint8 value from uint32 */
                dataSignal_s8 = (sint8)dataSignal_u32;
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc9_pfct))(dataSignal_s8);
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT16_SR_ENABLED != DCM_CFG_OFF)
        case DCM_SINT16:
                /* Extract sint16 value from uint32 */
                dataSignal_s16 = (sint16)dataSignal_u32;
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc10_pfct))(dataSignal_s16);
                break;
#endif

#if(DCM_CFG_DSP_WRITE_SINT32_SR_ENABLED != DCM_CFG_OFF)
        case DCM_SINT32:
                /* Extract sint32 value from uint32 */
                dataSignal_s32 = (sint32)dataSignal_u32;
                dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc11_pfct))(dataSignal_s32);
                break;
#endif
          default:
          /* No operation */
            break;

    }

}


/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteSenderReceiverSignal
 Syntax           : Dcm_Prv_WriteSenderReceiverSignal(pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc)
 Description      : TRACE[SWS_Dcm_00639],[SWS_Dcm_00687] This function writed DID signal using SenderReceiver interface
 Parameter        : Dcm_DIDConfig_tst: Pointer to DID configuration structure
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    ptrWriteFnc:  Pointer to Write function
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Prv_WriteSenderReceiverSignal
                            (const Dcm_MsgContextType * pMsgContext,
                             const Dcm_DIDConfig_tst * ptrDidConfig,
                             const Dcm_DataInfoConfig_tst * ptrSigConfig,
                             un_adrWriteFnc ptrWriteFnc)
{
    uint16 nrElements_u16 = 0;

/* Check for Sender Receiver  fixed length Signal */

    if(Dcm_Prv_SenderReceiverFixedLength(ptrSigConfig->dataSize_u16,ptrSigConfig->dataType_u8))
     {

           Dcm_Prv_WriteSenderReceiverFixedLengthSignal (pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc);
     }
      /* Check for Sender Receiver  variable length Signal */
      else
       {
		   if(Dcm_Prv_SenderReceiverVariableLength(ptrSigConfig->dataSize_u16,ptrSigConfig->dataType_u8) )
		   {

           /*MR12 RULE 13.5 VIOLATION:Right hand operand of '&&' or '||' is an expression with possible side effects. MISRA C:2012 Rule-13.5*/
               switch(ptrSigConfig->dataType_u8)
               {

#if((DCM_CFG_DSP_WRITE_UINT8_SR_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_WRITE_SINT8_SR_ENABLED != DCM_CFG_OFF))
               /*Calculate the number of elements in the Array of UINT8/SINT8 signal */
               case DCM_UINT8:
               case DCM_SINT8:
                   nrElements_u16 = (ptrSigConfig->dataSize_u16);
                   break;
#endif
#if((DCM_CFG_DSP_WRITE_UINT16_SR_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_WRITE_SINT16_SR_ENABLED != DCM_CFG_OFF))
               /*Calculate the number of elements in the Array of UINT8/SINT8 signal */
               case DCM_UINT16:
               case DCM_SINT16:
                   nrElements_u16 = (ptrSigConfig->dataSize_u16)/2u;
                    break;
#endif
#if((DCM_CFG_DSP_WRITE_UINT32_SR_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_WRITE_SINT32_SR_ENABLED != DCM_CFG_OFF))
               /* Calculate the number of elements in the Array of UINT8/SINT8 signal */
               case DCM_UINT32:
               case DCM_SINT32:
                   nrElements_u16 = (ptrSigConfig->dataSize_u16)/4u;
                    break;
#endif
               default:
                 {
                   /*empty default*/
                   break;
                 }
              }
		   }

           Dcm_Prv_WriteSenderReceiverVariableLengthSignal( pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc,nrElements_u16);
       }


}
#endif

#if((DCM_CFG_DSP_WRITE_SYNCH_FNC_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_WRITE_SYNCH_CS_ENABLED != DCM_CFG_OFF))
 /***********************************************************************************************************************
  Function name    : Dcm_Prv_WriteSYNCDID
  Syntax           : Dcm_Prv_WriteSYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16)
  Description      : TRACE[[SWS_Dcm_00794]] This function writed DID signal using SYNC interface
  Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                     Dcm_MsgContextType : Pointer to message structure
                     Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                     posnDidSignal_u16:
  Return value     : none
 ***********************************************************************************************************************/
 static void Dcm_Prv_WriteSYNCDID
                             (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                              const Dcm_MsgContextType * pMsgContext,
                              un_adrWriteFnc ptrWriteFnc,
                              const Dcm_DIDConfig_tst * ptrDidConfig,
                              uint16 posnDidSignal_u16)
 {
     /*Check if Fixed length or obtainable using ReadDataLengthFnc*/
     if(ptrDidConfig->dataFixedLength_b != FALSE)
     {    /* Call configured requested DID write function to write DID data  into the server */
#if(DCM_CFG_DSP_WRITE_FIXED_LENGTH != DCM_CFG_OFF)
         dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc2_pfct))
                                 (&pMsgContext->reqData[posnDidSignal_u16], dataNegRespCode_u8);
#endif

     }
#if(DCM_CFG_DSP_WRITE_VARIABLE_LENGTH != DCM_CFG_OFF)
     else
     {

        /* Call configured requested DID write function to write DID data  into the server */
        dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc1_pfct))
                                (&pMsgContext->reqData[posnDidSignal_u16],\
                                 (uint16)(pMsgContext->reqDataLen-0x02u),dataNegRespCode_u8);

     }
#endif
 }

#endif


#if((DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF)\
    || ((DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF )&& (DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF)))

 /***********************************************************************************************************************
  Function name    : Dcm_Prv_WriteASYNCDID
  Syntax           : Dcm_Prv_WriteASYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16)
  Description      : TRACE[[SWS_Dcm_00794]] This function writed DID signal using ASYNC interface
  Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                     Dcm_MsgContextType : Pointer to message structure
                     Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                     posnDidSignal_u16:
  Return value     : none
 ***********************************************************************************************************************/
static void Dcm_Prv_WriteASYNCDID
                           (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                            const Dcm_MsgContextType * pMsgContext,
                            un_adrWriteFnc ptrWriteFnc,
                            const Dcm_DIDConfig_tst * ptrDidConfig,
                            uint16 posnDidSignal_u16)
{
/*Check if Fixed length or obtainable using ReadDataLengthFnc*/
     if(ptrDidConfig->dataFixedLength_b != FALSE)
     {   /* Call configured requested DID write function to write DID data  into the server */
#if(DCM_CFG_DSP_WRITE_FIXED_LENGTH != DCM_CFG_OFF)
         dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc4_pfct))
                               (&pMsgContext->reqData[posnDidSignal_u16],Dcm_stDspWdbiOpStatus_u8,dataNegRespCode_u8);
#endif
     }
     else
     {   /* Call configured requested DID write function to write DID data  into the server */
#if(DCM_CFG_DSP_WRITE_VARIABLE_LENGTH != DCM_CFG_OFF)
         dataRetWriteFunc_u8 = (*(ptrWriteFnc.WdbiFnc3_pfct))
                               (&pMsgContext->reqData[posnDidSignal_u16],(uint16)(pMsgContext->reqDataLen-0x02u),\
                               Dcm_stDspWdbiOpStatus_u8,dataNegRespCode_u8);
#endif
     }
 }
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteDidUsePort
 Syntax           : Dcm_Prv_WriteDidUsePort(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[SWS_Dcm_00395] This function writes DID signal using ASYNC/SYNC/SenderReceiver interface
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    posnDidSignal_u16: : Postion of Requested DID in DID configured table
 Return value     : none
*******************************************************************************************************************/
static void Dcm_Prv_WriteDidUsePort
                           (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                            const Dcm_MsgContextType * pMsgContext,
                            un_adrWriteFnc ptrWriteFnc,
                            const Dcm_DIDConfig_tst * ptrDidConfig,
                            uint16 posnDidSignal_u16)
                            {
    /* Synchronous interface is used for writing */

    const Dcm_DataInfoConfig_tst * ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];

#if(DCM_CFG_DSP_WRITE_NEW_SR_INTERFACE_ENABLED != DCM_CFG_OFF)
    const Dcm_SignalDIDSubStructConfig_tst* ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
#endif

#if(DCM_CFG_DSP_WRITE_SYNCH_FNC_ENABLED != DCM_CFG_OFF)
        if((ptrWriteFnc.WdbiFnc1_pfct != NULL_PTR) && (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC))
        {
            Dcm_Prv_WriteSYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16);
        }
#endif
#if(DCM_CFG_DSP_WRITE_SYNCH_CS_ENABLED != DCM_CFG_OFF)
        if((ptrWriteFnc.WdbiFnc1_pfct != NULL_PTR) && (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER))
        {
            Dcm_Prv_WriteSYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16);
        }
#endif
        /* Asynchronous interface is used for writing */
#if((DCM_CFG_DSP_WRITE_ASYNCH_FNC_ENABLED != DCM_CFG_OFF))
        if((ptrWriteFnc.WdbiFnc1_pfct != NULL_PTR) && ((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC)
#if((DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF))
                || (ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC)
#endif
        ))
        {
            Dcm_Prv_WriteASYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16);
        }
#endif
#if(DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF)
        if((ptrWriteFnc.WdbiFnc1_pfct != NULL_PTR) && (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER))
        {
            Dcm_Prv_WriteASYNCDID(dataNegRespCode_u8,pMsgContext,ptrWriteFnc,ptrDidConfig,posnDidSignal_u16);
        }
#endif

        /* Sender Receiver  interface is used for writing */
#if(DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF)
        if((FALSE == ptrDidConfig->AtomicorNewSRCommunication_b) &&((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)||(ptrSigConfig->usePort_u8 ==USE_DATA_SENDER_RECEIVER_AS_SERVICE))
                && (ptrWriteFnc.WdbiFnc1_pfct != NULL_PTR))
        {
            Dcm_Prv_WriteSenderReceiverSignal (pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc);
        }
#endif

#if(DCM_CFG_DSP_WRITE_NEW_SR_INTERFACE_ENABLED != DCM_CFG_OFF)

        if((TRUE == ptrDidConfig->AtomicorNewSRCommunication_b) && \
           (USE_DATA_ELEMENT_SPECIFIC_INTERFACES == ptrDidConfig->didUsePort_u8 ) && \
           (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].WriteSenderReceiver_pfct != NULL_PTR) && (ptrControlSigConfig->GetSignalData_pfct != NULL_PTR))
        {
            /*MR12 DIR 1.1 VIOLATION:The type of parameter will be known during runtime, so typecast to void* is required. */
            (void)(ptrControlSigConfig->GetSignalData_pfct)(&pMsgContext->reqData[2], ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16);
            dataRetWriteFunc_u8 = (Std_ReturnType)(ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].WriteSenderReceiver_pfct)(ptrControlSigConfig->WriteDataTypeVar);

        }

#endif
}



#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteDIDNvRam
 Syntax           : Dcm_Prv_WriteDIDNvRam(pMsgContext,ptrDidConfig,ptrSigConfig,ptrWriteFnc)
 Description      : TRACE[SWS_Dcm_00541] This function writed DID signal using NVRAM BLOCK
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_DataInfoConfig_tst: Pointer to DID config Structure
                    posnDidSignal_u16:Position of DID in configured table
 Return value     : none
***********************************************************************************************************************/
 static void Dcm_Prv_WriteDIDNvRam
                            (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                             const Dcm_MsgContextType * pMsgContext,
                             const Dcm_DataInfoConfig_tst * ptrSigConfig,
                             uint16 posnDidSignal_u16)
 {

Std_ReturnType dataNvmWriteBlockRetVal_u8;/*Return value for Nvm_WriteBlock*/
Std_ReturnType dataNvmGetErrorRetVal_u8;/*Return value for NvM_GetErrorStatus*/
NvM_RequestResultType dataRequestResult_u8;

/*Check if the flag Dcm_flgNvmWritePending_b is set to FALSE*/
    if(Dcm_flgNvmWritePending_b ==FALSE)
    {
        NvM_SetBlockLockStatus(ptrSigConfig->dataNvmBlockId_u16,FALSE);
        /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as NvM_WriteBlock takes void pointer as input
         * and object type pointer is converted to void pointer*/
        dataNvmWriteBlockRetVal_u8=NvM_WriteBlock(ptrSigConfig->dataNvmBlockId_u16,&pMsgContext->reqData[posnDidSignal_u16]);
        if(dataNvmWriteBlockRetVal_u8 != E_OK)
        {
            /* According to  ISO 14229-1;2013 - General Programming failure has to be returned in case of NvmWrite Failure */
            *dataNegRespCode_u8 = DCM_E_GENERALPROGRAMMINGFAILURE;
            /*Set the flag Dcm_flgNvmWritePending_b is to FALSE*/
            Dcm_flgNvmWritePending_b=FALSE;
            /* Locking the unlocked block */
            NvM_SetBlockLockStatus(ptrSigConfig->dataNvmBlockId_u16,TRUE);
        }
        else
        {
            /*Set the flag Dcm_flgNvmWritePending_b to TRUE*/
            Dcm_flgNvmWritePending_b=TRUE;
            dataRetWriteFunc_u8 =DCM_E_PENDING;
        }
        s_loopbreak_b = TRUE;

    }
    else
    {
        dataNvmGetErrorRetVal_u8= NvM_GetErrorStatus(ptrSigConfig->dataNvmBlockId_u16,&dataRequestResult_u8);
        /*Set the flag Dcm_flgNvmWritePending_b to FALSE*/
        Dcm_flgNvmWritePending_b = FALSE;
        if(dataNvmGetErrorRetVal_u8 != E_OK)
        {
            /* According to  ISO 14229-1;2013 - General Programming failure has to be returned in case of NvmWrite Failure */
            *dataNegRespCode_u8 =DCM_E_GENERALPROGRAMMINGFAILURE;
            /* Locking the unlocked block */
            NvM_SetBlockLockStatus(ptrSigConfig->dataNvmBlockId_u16,TRUE);
            s_loopbreak_b = TRUE;
        }
        else
        {
             switch(dataRequestResult_u8)
             {
                 case NVM_REQ_PENDING:
                         /*Set the flag Dcm_flgNvmWritePending_b to TRUE*/
                         Dcm_flgNvmWritePending_b = TRUE;
                         dataRetWriteFunc_u8 = DCM_E_PENDING;
                         break;
                 case NVM_REQ_OK:
                         dataRetWriteFunc_u8 =E_OK;
                         break;
                 default:
                         *dataNegRespCode_u8 = DCM_E_GENERALPROGRAMMINGFAILURE;
                         break;
             }
        }
        if((dataRequestResult_u8 != NVM_REQ_PENDING) && (dataNvmGetErrorRetVal_u8==E_OK))
        {
            /* Locking the unlocked block */
            NvM_SetBlockLockStatus(ptrSigConfig->dataNvmBlockId_u16,TRUE);
        }
        if(dataRequestResult_u8!=NVM_REQ_OK)
        {
            s_loopbreak_b = TRUE;
        }
      }
}

#endif
#if(DCM_CFG_ATOMICWRITE_DID>0)
 /***********************************************************************************************************************
  Function name    : Dcm_Prv_WriteAtomicDID
  Syntax           : Dcm_Prv_WriteAtomicDID(dataNegRespCode_u8,pMsgContext)
  Description      :  This function performs write opeartion for Atomic Normal DID
                     (i.e which is not a range DID)
  Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                     Dcm_MsgContextType : Pointer to message structure
  Return value     : none
 ***********************************************************************************************************************/
 static void Dcm_Prv_WriteAtomicDID
                         (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                          const Dcm_MsgContextType * pMsgContext)
                          {
     /* Pointers  to DID  configuration */

   const Dcm_DIDConfig_tst * ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
   boolean Rtn_InfrastureError;

 #if(DCM_CFG_ATOMICWRITE_DID>0)
   /*TRACE[SWS_Dcm_01433]*/
     if(TRUE == ptrDidConfig->AtomicorNewSRCommunication_b)
     {
         uint16 AtomicWriteIndex_u16;
         AtomicWriteIndex_u16 = Dcm_Prv_AtomicWriteDid_index(ptrDidConfig->dataDid_u16);
         (void)(AtomicDidWrite_cast[AtomicWriteIndex_u16].AtomicWrite_CopyData_pfct)(&pMsgContext->reqData[2]);
         dataRetWriteFunc_u8=(Std_ReturnType)(AtomicDidWrite_cast[AtomicWriteIndex_u16].AtomicWrite_pfct)(ptrDidConfig->AtomicWrite_var);
         if(dataRetWriteFunc_u8 == E_OK)
         {
             /* Write Operation successfull*/
             *dataNegRespCode_u8=0x00;
             Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;

         }
         else
         {
             Rtn_InfrastureError=Dcm_IsInfrastructureErrorPresent_b(dataRetWriteFunc_u8);
             if(Rtn_InfrastureError == TRUE)
             {
                 /* General Reject is sent when there is Infrastructure error */
                 *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                 dataRetWriteFunc_u8 = DCM_INFRASTRUCTURE_ERROR;
             }
         }
     }
 #endif
 }
#endif
/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteNormalDID
 Syntax           : Dcm_Prv_WriteNormalDID(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[[SWS_Dcm_00395],[SWS_Dcm_00541] This function performs write opeartion for Normal DID
                    (i.e which is not a range DID)
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Prv_WriteNormalDID
                        (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                         const Dcm_MsgContextType * pMsgContext)
                         {
    /* Pointers  to DID  configuration */

  const Dcm_DIDConfig_tst * ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
  const Dcm_DataInfoConfig_tst * ptrSigConfig;
  const Dcm_SignalDIDSubStructConfig_tst * ptrControlSigConfig;

    /* Local variable for the signal position of did in terms of byte numbers*/
  uint16 posnDidSignal_u16;
  uint16 loopIndex_u16 = Dcm_DidSignalIdx_u16;
  boolean Rtn_InfrastureError;

        for(;((loopIndex_u16 < ptrDidConfig->nrSig_u16) && (dataRetWriteFunc_u8 == E_OK)); loopIndex_u16++)
        {
            Dcm_DidSignalIdx_u16 = loopIndex_u16;
            ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].idxDcmDspDatainfo_u16];
            ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
            /*Calculate the signal position of did in terms of byte numbers*/
            posnDidSignal_u16 = (uint16)((ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].posnSigBit_u16) +2u);
            /* Find the port configured for the Signal */
            if(ptrSigConfig->idxDcmDspControlInfo_u16 > 0u)
            {
                /*Write DID using ASYNC/SYNC/Sender Receiver port*/
                Dcm_Prv_WriteDidUsePort(dataNegRespCode_u8,pMsgContext,ptrControlSigConfig->adrWriteFnc_cpv,ptrDidConfig,posnDidSignal_u16);
                if(dataRetWriteFunc_u8 != E_OK)
                {

                    Rtn_InfrastureError=Dcm_IsInfrastructureErrorPresent_b(dataRetWriteFunc_u8);

                    if ((((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) ||
                            (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER) ||
                            ((ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER)||(ptrSigConfig->usePort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE))))&&(Rtn_InfrastureError == TRUE))

                    {

#if((DCM_CFG_DSP_WRITE_ASYNCH_CS_ENABLED != DCM_CFG_OFF) || \
        (DCM_CFG_DSP_WRITE_SYNCH_CS_ENABLED != DCM_CFG_OFF) || \
        (DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF))
                        /* General Reject is sent when there is Infrastructure error */
                        *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
                        dataRetWriteFunc_u8 = DCM_INFRASTRUCTURE_ERROR;
#endif
                    }
                    /*Set the variable to TRUE to break for loop*/
                    s_loopbreak_b =TRUE;
                }
                else
                {
                    /* Write Operation successfull*/
                    *dataNegRespCode_u8=0x00;
                    Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
                }
            }
            else
            {
#if (DCM_CFG_DSP_NVRAM_ENABLED != DCM_CFG_OFF)
                if((ptrSigConfig->usePort_u8 == USE_BLOCK_ID))
                {
                    /* NvRam  interface is used for writing */
                    Dcm_Prv_WriteDIDNvRam(dataNegRespCode_u8,pMsgContext,ptrSigConfig,posnDidSignal_u16);
                }
#endif
            }
            if(s_loopbreak_b == TRUE)
            {
                /* Write Operation is not successfull or pending*/
                s_loopbreak_b = FALSE;
                break;
            }
        }
}


/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteDidMaxLenCheck
 Syntax           : Dcm_Prv_WriteDidMaxLenCheck(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[SWS_Dcm_00473] This function checks the requested maximum DID Data length from the request
                    If requested data length exceeds the maximum data length as per configured max data size then
                    this function sets negative repsonse  DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
 Return value     : none
***********************************************************************************************************************/

static void Dcm_Prv_WriteDidMaxLenCheck
                            (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                             const Dcm_MsgContextType * pMsgContext)
{
    /* Variable to store the length of data record */
uint16 nrLenDataRec_u16;

#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
      /*Check if the did is a range did or not*/
      if(s_Dcm_idxwdbiDidIndexType.dataRange_b == FALSE)
#endif
        {
           /* Copy the total valid length */
           nrLenDataRec_u16 = (Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].dataMaxDidLen_u16+2u);
           /* Check for fixed length signals is  using ReadDataLengthFnc*/
           if(Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].dataFixedLength_b != FALSE)
           {
               /* Check for exact length */
                if(pMsgContext->reqDataLen != nrLenDataRec_u16)
                {   /*Set NRC Request Length incorrect */
                    *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
           }
           else
           {
               /* Check for length within max range */
               if(pMsgContext->reqDataLen > nrLenDataRec_u16)
               {   /*Set NRC Request Length incorrect */
                   *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
               }
               else  if(DcmAppl_DcmCheckWDBIReqLen(s_dataDID_u16,(pMsgContext->reqDataLen-2u))!= E_OK)
               {
                   /* For application to do the exact length check in case of varaible length signal */
                  /*Set NRC Request Length incorrect */
                   *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
               }
               else
               {
                /*Do nothing*/
               }
           }
        }
#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
        else
        {
           nrLenDataRec_u16 = (uint16)(Dcm_DIDRangeConfig_cast[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].dataMaxDidLen_u16+2u);
           /*Check Requested Range  DID Length check */
           if(pMsgContext->reqDataLen > nrLenDataRec_u16)
           {   /*Set NRC Request Length incorrect */
               *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
           }
           else
           {
			   if(DcmAppl_DcmCheckWDBIReqLen(s_dataDID_u16,(pMsgContext->reqDataLen-2u))!= E_OK)
			   {
               /* For application to do the exact length check in case of varaible length signal */
               /*Set NRC Request Length incorrect */
              *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
			   }
           }
        }

#endif
}

/***********************************************************************************************************************
 Function name    : Dcm_Priv_DidWriteSupport
 Syntax           : Dcm_Priv_DidWriteSupport(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[SWS_Dcm_00469],[SWS_Dcm_00470],[SWS_Dcm_00822]This function checks the Maximum data length for
                    requested Did , Session check , security check and mode rule check.
                    If any of the check fails it will update corresponding negative response code
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
 Return value     : none
***********************************************************************************************************************/
static void Dcm_Priv_DidWriteSupport
                            (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                             const Dcm_MsgContextType * pMsgContext
                             )

{
/* Variable to hold the configured sessions value */
uint32 dataSessionMask_u32 = 0;
/* Pointer to extended configuration */
const Dcm_ExtendedDIDConfig_tst * adrExtendedConfig_pcst;

#if ( DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
     if(s_Dcm_idxwdbiDidIndexType.dataRange_b == FALSE)
     {
#endif
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
        adrExtendedConfig_pcst = Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].adrExtendedConfig_pcst;

#endif
#if ( DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
     }
     else
     {
        adrExtendedConfig_pcst = Dcm_DIDRangeConfig_cast[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].adrExtendedConfig_pcst;
     }
#endif
    /* Copy the session level for write configured */
    dataSessionMask_u32  = adrExtendedConfig_pcst->dataAllowedSessWrite_u32;
    /* Check if The Write Access is configured for DID */
    if (((Dcm_DsldGetActiveSessionMask_u32 () & dataSessionMask_u32) != 0x0uL))
    {
        Dcm_Prv_WriteDidMaxLenCheck(dataNegRespCode_u8,pMsgContext);


    }
    else
    {
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
    }

    if(*dataNegRespCode_u8 == 0)
    {
      /* Call the function to see if there is a write support of the DID */
      dataRetWriteSupport_en = Dcm_GetSupportOfIndex(&s_Dcm_idxwdbiDidIndexType, DCM_SUPPORT_WRITE,dataNegRespCode_u8);
      /* If the conditions of writing is supported */
      if(dataRetWriteSupport_en == DCM_SUPPORT_OK)
      {   /*As all chekcs are passed with session and security  Move the state to DCM_PROCESSSERVICE */
          Dcm_stDspWdbiOpStatus_u8  = DCM_INITIAL;
          Dcm_SrvOpstatus_u8 = DCM_PROCESSSERVICE;
          dataRetWriteFunc_u8 = E_OK;
      }
    }
}

/***********************************************************************************************************************
 Function name    :
 Syntax           : Dcm_Priv_DidWriteFuncAvailableCheck(dataNegRespCode_u8)
 Description      : TRACE[SWS_Dcm_00468] This function checks if  Write function  is configured  for requested DID
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
 Return value     : E_OK : Write Function is configured for DID
                    E_NOT_OK : Write Fucntion is not configured for DID
***********************************************************************************************************************/
static Std_ReturnType Dcm_Priv_DidWriteFuncAvailableCheck
                           (Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{

/* Loacal variable to set return value */
Std_ReturnType dataRetVal_u8 = E_NOT_OK;
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
 /* Local Loop variable for DID Signal index  */
uint16 idxDidSignal_u16 = 0;
/* Local  variable for Number of  Signals in DID  */
uint16 nrDidSignal_u16 = 0;
/* Local variable for Configured DID Signal port  */
uint8 useDidPort_u8 = 0;
/* Local variable for active session  */
uint32 alloweWritedSession_u32 = 0;
#endif
/* Local union variable for DID Signal write function  */
un_adrWriteFnc ptrWriteFnc;

/* Initializing the local variables */
ptrWriteFnc.WdbiFnc1_pfct = NULL_PTR;

#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
un_WriteDidFnc ptrWriteRangeFnc;
ptrWriteRangeFnc.WriteFncRange1_pfct = NULL_PTR;
#endif

const Dcm_DIDConfig_tst * ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];

#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
    /*Check if the DID currently under process is a normal DID or range DID*/
    if(s_Dcm_idxwdbiDidIndexType.dataRange_b == FALSE)
#endif
    {
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
        if((ptrDidConfig->AtomicorNewSRCommunication_b == TRUE) && ((ptrDidConfig->didUsePort_u8 == USE_ATOMIC_SENDER_RECEIVER_INTERFACE) || (ptrDidConfig->didUsePort_u8 == USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE)))
        {
#if(DCM_CFG_ATOMICWRITE_DID>0)
            if(ptrDidConfig->AtomicWrite_var == NULL_PTR)
            {
                /*DID sent from tester has no Write Access - Set NRC Request Out Of Range */
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
                /* Report development error "DCM_E_NO_WRITE_ACCESS " */
                DCM_DET_ERROR(DCM_WDBI_ID , DCM_E_NO_WRITE_ACCESS )
            }
            else
            {
                /* Write fucntion is configured for DID signal */
                           dataRetVal_u8 =E_OK;
            }
#endif
        }
        else
        {
        nrDidSignal_u16 = Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].nrSig_u16;

        alloweWritedSession_u32 = Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].\
                                    adrExtendedConfig_pcst->dataAllowedSessWrite_u32;

        /* Check if the Write function is configured for all signals */
        for(idxDidSignal_u16=0x0; idxDidSignal_u16 < nrDidSignal_u16; idxDidSignal_u16++)
        {

            useDidPort_u8 = Dcm_DspDataInfo_st[Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].\
                            adrDidSignalConfig_pcst[idxDidSignal_u16].idxDcmDspDatainfo_u16].usePort_u8;

            ptrWriteFnc = Dcm_DspDid_ControlInfo_st[Dcm_DspDataInfo_st[Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].\
                          adrDidSignalConfig_pcst[idxDidSignal_u16].idxDcmDspDatainfo_u16].idxDcmDspControlInfo_u16].adrWriteFnc_cpv;

            if(((useDidPort_u8 != USE_BLOCK_ID)&&(alloweWritedSession_u32 >0x00u) && (NULL_PTR == ptrWriteFnc.WdbiFnc1_pfct) && (ptrDidConfig->didUsePort_u8 == USE_DATA_ELEMENT_SPECIFIC_INTERFACES) && ((ptrDidConfig->AtomicorNewSRCommunication_b == FALSE) || ((ptrDidConfig->AtomicorNewSRCommunication_b == TRUE) &&  (useDidPort_u8 != USE_DATA_SENDER_RECEIVER) && (useDidPort_u8 != USE_DATA_SENDER_RECEIVER_AS_SERVICE))))
#if(DCM_CFG_DSP_WRITE_NEW_SR_INTERFACE_ENABLED != DCM_CFG_OFF)
                || ((ptrDidConfig->AtomicorNewSRCommunication_b == TRUE) &&  ((useDidPort_u8 == USE_DATA_SENDER_RECEIVER) || (useDidPort_u8 == USE_DATA_SENDER_RECEIVER_AS_SERVICE)) && (ptrDidConfig->adrDidSignalConfig_pcst[Dcm_DidSignalIdx_u16].WriteSenderReceiver_pfct == NULL_PTR) && (ptrDidConfig->didUsePort_u8 == USE_DATA_ELEMENT_SPECIFIC_INTERFACES))
#endif
            )
            {
                break;
            }

        }
        if(idxDidSignal_u16 == nrDidSignal_u16)
        {   /* Write fucntion is configured for DID signal */
            dataRetVal_u8 =E_OK;
        }
        else
        {   /*DID sent from tester has no Write Access - Set NRC Request Out Of Range */
            *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
            /* Report development error "DCM_E_NO_WRITE_ACCESS " */
            DCM_DET_ERROR(DCM_WDBI_ID , DCM_E_NO_WRITE_ACCESS )
        }
#endif
        }
    }
#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
    else
    {
          ptrWriteRangeFnc = Dcm_DIDRangeConfig_cast[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].adrWriteFnc_pfct;
          if(ptrWriteRangeFnc.WriteFncRange1_pfct != NULL_PTR)
          {
              dataRetVal_u8 = E_OK;
          }
          else
          {
              /*DID sent from tester has no Write Access - Set NRC Request Out Of Range */
              *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
              /* Report development error "DCM_E_NO_WRITE_ACCESS " */
              DCM_DET_ERROR(DCM_WDBI_ID , DCM_E_NO_WRITE_ACCESS )
          }
    }
# endif

 return dataRetVal_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_DidAvailableCheck
 Syntax           : Dcm_Prv_DidAvailableCheck(dataNegRespCode_u8)
 Description      : TRACE[SWS_Dcm_00467] This function checks if requested DID is configured in server and
                    Update negative response code  if not available .
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
***********************************************************************************************************************/

static void Dcm_Prv_DidAvailableCheck
                                    (Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
/* Local  Variable to store the return value of Dcm_Prv_GetIndexOfDID function */
    Std_ReturnType dataRetGetDID_u8 = E_NOT_OK;

    /* Check if the DID sent from tester is configured in Server */
    dataRetGetDID_u8 = Dcm_Prv_GetIndexOfDID(s_dataDID_u16, &s_Dcm_idxwdbiDidIndexType);
#if(DCM_CFG_DSP_NUMISDIDAVAIL>0u)
    /*Check if Requested DID is configured and if the did is a range did or not */
    if ((dataRetGetDID_u8 == E_OK)&& (s_Dcm_idxwdbiDidIndexType.dataRange_b==FALSE))
    {
        /* Check if the DID is supported in current variant */
        if(*Dcm_DIDIsAvail[Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].idxDIDSupportedFnc_u16] != NULL_PTR)
        {
            if((*(IsDIDAvailFnc_pf)(Dcm_DIDIsAvail[Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].\
                                                   idxDIDSupportedFnc_u16]))(s_dataDID_u16)!=E_OK)
            {
                dataRetGetDID_u8 = E_NOT_OK;
            }
        }
    }
#endif
    /* Requested DID is configured */
    if(dataRetGetDID_u8 == E_OK)
    {
        /*DID is cofigure for Write Access*/
        dataRetGetDID_u8 = Dcm_Priv_DidWriteFuncAvailableCheck(dataNegRespCode_u8);

		    if(dataRetGetDID_u8 == E_OK)
            {
               /* If DID is available and Witer funtion is configured move the state to next*/
               Dcm_SrvOpstatus_u8 = DCM_CHECKDATA;
               /* Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;*/
            }
    }
    else if(dataRetGetDID_u8 == DCM_E_PENDING)
    {
        dataRetWriteFunc_u8 = DCM_E_PENDING;
    }
    else
    {
        /*DID sent from tester is incorrect - Set NRC Request Out Of Range */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
       /* Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;*/
    }


}



#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteRangeDID
 Syntax           : Dcm_Prv_WriteRangeDID(pMsgContext,dataNegRespCode_u8)
 Description      : TRACE[[SWS_Dcm_00805]] This function perform write opration if requested DID is Range DID
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
 Return value     : none
***********************************************************************************************************************/

static void Dcm_Prv_WriteRangeDID
                        (const Dcm_MsgContextType * pMsgContext,
                         Dcm_NegativeResponseCodeType * dataNegRespCode_u8
                        )

{
 const Dcm_DIDRangeConfig_tst * ptrDidRangeConfig;

 ptrDidRangeConfig = &Dcm_DIDRangeConfig_cast[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
 if(ptrDidRangeConfig->adrWriteFnc_pfct.WriteFncRange1_pfct != NULL_PTR)
 {
    /*Check if Fixed length or obtainable using ReadDataLengthFnc*/
    if(ptrDidRangeConfig->nrRangeUsePort_b != FALSE)
    {
        dataRetWriteFunc_u8 = (*(ptrDidRangeConfig->adrWriteFnc_pfct.WriteFncRange1_pfct))
                              (s_dataDID_u16,&pMsgContext->reqData[0x2],Dcm_stDspWdbiOpStatus_u8,\
                              (uint16)(pMsgContext->reqDataLen-0x02u),dataNegRespCode_u8);
        /* Check for infrastructure error  incase of case RTE use ports */

        if((Dcm_IsInfrastructureErrorPresent_b(dataRetWriteFunc_u8) != FALSE))

        {
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
        }
    }
    else
    {
        dataRetWriteFunc_u8 = (*(ptrDidRangeConfig->adrWriteFnc_pfct.WriteFncRange1_pfct))
                              (s_dataDID_u16,&pMsgContext->reqData[0x2],Dcm_stDspWdbiOpStatus_u8,\
                              (uint16)(pMsgContext->reqDataLen-0x02u),dataNegRespCode_u8);
    }
 }
}
#endif

/***********************************************************************************************************************
 Function name    : Dcm_Prv_WdbiStateProcessing
 Syntax           : Dcm_Prv_WdbiStateProcessing(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[SWS_CanTp_00210] This function chekcs the DCM states while processing the WDBI service request
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure

 Return value     : void
***********************************************************************************************************************/

static void Dcm_Prv_WdbiStateProcessing
                                    ( Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                                      const Dcm_MsgContextType * pMsgContext
                                     )


{
#if(DCM_CFG_ATOMICWRITE_DID>0)
    const Dcm_DIDConfig_tst * ptrDidConfig = NULL_PTR;
#endif
   /* Check WDBI State Machine */
   if(Dcm_SrvOpstatus_u8 == DCM_INITIAL)
   {
       /* Check for the minimum request length equal to 0x03 */
       /* 2 bytes DID + 1 Byte Data */
       if(pMsgContext->reqDataLen >= DCM_DSP_WDBI_MINREQLEN)
       {
           /* Extract the DID information from the request*/
           s_dataDID_u16 = (uint16)(pMsgContext->reqData[0]);
           s_dataDID_u16 = s_dataDID_u16 <<8u;
           s_dataDID_u16 |= (uint16)(pMsgContext->reqData[1]);

           /* DID is available in Server Configuration*/
            Dcm_Prv_DidAvailableCheck(dataNegRespCode_u8);
       }
       else
       {
           /*Set NRC Request Length incorrect */
           *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
       }
   }

   if(Dcm_SrvOpstatus_u8 == DCM_CHECKDATA)
   {
      Dcm_Priv_DidWriteSupport(dataNegRespCode_u8, pMsgContext);
   }


   if(Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE)
   {
       /* loop for all signals for this DID */
#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
       /*Check if the did is a range did or not*/
       if(s_Dcm_idxwdbiDidIndexType.dataRange_b == FALSE)
#endif
       {
#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
#if(DCM_CFG_ATOMICWRITE_DID>0)
           ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16];
           if((USE_ATOMIC_SENDER_RECEIVER_INTERFACE == ptrDidConfig->didUsePort_u8) || (USE_ATOMIC_SENDER_RECEIVER_INTERFACE_AS_SERVICE == ptrDidConfig->didUsePort_u8))
           {
               Dcm_Prv_WriteAtomicDID(dataNegRespCode_u8,pMsgContext);
           }
           else
#endif
           {
               Dcm_Prv_WriteNormalDID(dataNegRespCode_u8,pMsgContext);
           }
#endif
       }
#if (DCM_CFG_DIDRANGE_EXTENSION!=DCM_CFG_OFF)
       else
       {
           Dcm_Prv_WriteRangeDID(pMsgContext,dataNegRespCode_u8);
       }
#endif
   }
}

/***********************************************************************************************************************
 Function name    : Dcm_Prv_WriteDIDServevice_status
 Syntax           : Dcm_Prv_WriteDIDServevice_status(dataNegRespCode_u8,pMsgContext)
 Description      : TRACE[SWS_CanTp_00210] This function updated the status of WDBI Service
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure

 Return value     : None
***********************************************************************************************************************/

static Std_ReturnType Dcm_Prv_WriteDIDServevice_status
                                    (Dcm_NegativeResponseCodeType * dataNegRespCode_u8,
                                     Dcm_MsgContextType * pMsgContext
                                     )

{
/* Loacal variable to set return value */
Std_ReturnType dataRetVal_u8 = E_NOT_OK;



    if( (dataRetWriteFunc_u8 == E_OK) && (*dataNegRespCode_u8 == 0x00) )
    {   /* Frame the positive response by updating DID in response buffer */
        pMsgContext->resData[0] = pMsgContext->reqData[0];
        pMsgContext->resData[1] = pMsgContext->reqData[1];
        pMsgContext->resDataLen = 0x02;
        /* Reset the state of WDBI state machine to IDLE */
        Dcm_SrvOpstatus_u8  = DCM_INITIAL;
        Dcm_DidSignalIdx_u16 = 0x0;
        Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
        dataRetVal_u8=E_OK;
    }
    /* If the write Function needs more time for writing */
    else if((dataRetWriteFunc_u8 == DCM_E_PENDING) || (dataRetWriteSupport_en == DCM_SUPPORT_CONDITION_PENDING))
    {
        /* Remain in the same state, Write function will be called again in next cycle */
        Dcm_stDspWdbiOpStatus_u8 = DCM_PENDING;
        *dataNegRespCode_u8=0x00;
        dataRetVal_u8=DCM_E_PENDING;
    }
    else /* Return is E_NOT_OK */
    {   /*Set Negative response code to DCM_E_GENERALREJECT*/
        if(*dataNegRespCode_u8 == 0x00)
        {
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
        }

        Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
        dataRetVal_u8=E_NOT_OK;
    }
    /* Check if Negative response is set */
    if(*dataNegRespCode_u8 != 0x0)
    {
      dataRetVal_u8=E_NOT_OK;
      /* Reset the state of WDBI state machine to IDLE */
      Dcm_SrvOpstatus_u8  = DCM_INITIAL;
      Dcm_DidSignalIdx_u16 = 0x0;
      Dcm_stDspWdbiOpStatus_u8 = DCM_INITIAL;
    }

return dataRetVal_u8;

}

/***********************************************************************************************************************
 Function name    : Dcm_DcmWriteDataByIdentifier
 Syntax           : Dcm_DcmWriteDataByIdentifier(OpStatus,pMsgContext,dataNegRespCode_u8)
 Description      : TRACE[SWS_Dcm_00255] This function is called by DSD, when SID is set to 0x2E, this service is
                    provided in DSP module and configured in Service table.The WriteDataByIdentifier service allows the
                    client to write information into the server at an internal location specified by the  provided
                    data identifier.
 Parameter        : Dcm_NegativeResponseCodeType: Pointer to negative response code
                    Dcm_MsgContextType : Pointer to message structure
                    Dcm_SrvOpStatusType: Service Opstatus
 Return value     : Std_ReturnType

***********************************************************************************************************************/
Std_ReturnType Dcm_DcmWriteDataByIdentifier
                                (Dcm_SrvOpStatusType OpStatus,
                                 Dcm_MsgContextType * pMsgContext,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{


    Std_ReturnType dataRetVal_u8;/*Return value for Nvm_WriteBlock*/

#if (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)
    s_loopbreak_b = FALSE;
#endif
    /* Initilaization of local variables */
    *dataNegRespCode_u8 = 0x0;
    dataRetWriteFunc_u8 = E_OK;
    dataRetWriteSupport_en = DCM_SUPPORT_OK;
    dataRetVal_u8 = E_NOT_OK;
    /* If OpStatus is set to DCM_CANCEL then call the ini function for resetting */
    if (OpStatus == DCM_CANCEL)
    {
        /* Call the Ini Function */
        Dcm_Dcm_WDBIInit();
        /* Set the return value to E_OK as the Ini function will always be serviced  */
        dataRetVal_u8 = E_OK;
    }
    else
    {
        /* Process the WDBI Service Request  */
        Dcm_Prv_WdbiStateProcessing(dataNegRespCode_u8,pMsgContext);
        /* Update the Service Return value and negative  response*/
        dataRetVal_u8  =   Dcm_Prv_WriteDIDServevice_status(dataNegRespCode_u8,pMsgContext);
    }
        return dataRetVal_u8;
}

/***********************************************************************************************************************
 Function name    : Dcm_GetActiveWDBIDid
 Syntax           : Dcm_GetActiveWDBIDid(uint16 * dataDid_u16)
 Description      : TRACE[SWS_Dcm_00255] his API returns the active DID being processed when WDBI service request
                    is being processed. This function is called from the Dcm API Dcm_GetActiveDid
 Parameter        : dataDid_u16:  Parameter for updating of the DID under processing
                    Dcm_MsgContextType : Pointer to message structure
 Return value     : E_OK The parameter dataDid_u16 contains valid DID value in this case
                    E_NOT_OK  The parameter dataDid_u16 contains invalid data in this case
***********************************************************************************************************************/
Std_ReturnType Dcm_GetActiveWDBIDid(uint16 * dataDid_u16)
{
    Std_ReturnType dataRetVal_u8; /* Variable to store the return value */

    /* Initialisation of local variables  */
    dataRetVal_u8 = E_NOT_OK;

    /* Check if the DID currently under process is a normal DID */

    if(s_Dcm_idxwdbiDidIndexType.dataRange_b==FALSE)
    {
        /* Update the DID value currently under process */
        *dataDid_u16 = Dcm_DIDConfig[s_Dcm_idxwdbiDidIndexType.idxIndex_u16].dataDid_u16;
        /* Update the return value to E_OK */
        dataRetVal_u8 = E_OK;
    }
    else
    {
#if (DCM_CFG_DIDRANGE_EXTENSION != DCM_CFG_OFF)
        /* Update the DID value currently under process */
        *dataDid_u16 = s_Dcm_idxwdbiDidIndexType.dataRangeDid_16;
        /* Update the return value to E_OK */
        dataRetVal_u8 = E_OK;
#endif
    }
    return (dataRetVal_u8);
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif

