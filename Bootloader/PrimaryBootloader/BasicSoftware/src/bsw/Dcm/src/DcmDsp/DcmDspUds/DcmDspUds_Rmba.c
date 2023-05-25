

#include "DcmDspUds_Rmba_Inf.h"

#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_READMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF)

#include "DcmDspUds_Rmba_Priv.h"

/* Global varibale declaration */
#if(DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF)

#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
static Dcm_MsgLenType Dcm_DspRMBAMaxRespLen_u32;    /* Maximum response length */
static uint32 s_Pendingreaddatalength_u32; /* Variable to indicate the reamining response data to be transmitted
                                     during page buffer */
static uint32 s_FilledReadMemoryLength_u32; /* Variable to Keep track of filled response length */
static Dcm_MsgLenType s_datalength_u32 = DCM_RMBAPAGESIZE;
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
static Dcm_MsgType Dcm_DspRMBARespBufPtr_u8;     /* Pointer to the response buffer */
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static boolean Dcm_RmbaFirstPageSent_b;       /* Whether first page is sent or not? */
static boolean Dcm_Pending_Data_b;         /* Variable to indicates rmba data was pending */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#endif

#define DCM_START_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
static uint32 Dcm_dataMemoryAddress_u32; /*Variable to store the memory address requested by the tester*/
static uint32 Dcm_nrReadMemoryLength_u32; /*Variable to store the memory length requested by the tester*/
#define DCM_STOP_SEC_VAR_CLEARED_32
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
static uint16 Dcm_idxDspRmba_u16; /* Variable to store the index of the Memory address in
                                                   Dcm_RMBAConfig_cast table */
#define DCM_STOP_SEC_VAR_CLEARED_16
#include "Dcm_MemMap.h"
#define DCM_START_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"
static Dcm_OpStatusType Dcm_stRmbaOpstatus_u8;
static uint8 Dcm_dataMemdatasize_u8;                /* variable for storing the data record size*/
static uint8 Dcm_dataMemaddrsize_u8;                /* variable for storing the memory address size*/
#define DCM_STOP_SEC_VAR_CLEARED_8
#include "Dcm_MemMap.h"

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"
/**
 ***********************************************************************************************************************
 * Dcm_RMBAIni : Initialisation function for Read Memory By Address (0x23) Service
 *
 * \param           None
 *
 * \retval          None
 * \seealso
 * \usedresources
 ***********************************************************************************************************************
 */
void Dcm_RMBAIni (void)
{

    if(DCM_PENDING == Dcm_stRmbaOpstatus_u8 )
    {
        (void)(DcmAppl_Dcm_ReadMemory(DCM_CANCEL,Dcm_RMBAConfig_cast[Dcm_idxDspRmba_u16].dataMemoryValue_u8,
                                      Dcm_dataMemoryAddress_u32,Dcm_nrReadMemoryLength_u32,NULL_PTR,NULL_PTR));
    }


    /* Initialize the state of Rmba to IDLE */
    Dcm_SrvOpstatus_u8 = DCM_INITIAL;

    /* Reset the OpStatus */
    Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;

#if(DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF)

    /* Reset of all the static variables */
    Dcm_Pending_Data_b = FALSE;
    s_FilledReadMemoryLength_u32 = 0x00u;
    s_Pendingreaddatalength_u32 = 0x00u;
    Dcm_RmbaFirstPageSent_b = FALSE;

#endif
}

/**
 ***********************************************************************************************************************
 * Dcm_Prv_GetAddressRangeIndex_u8:
 *
 * Calculate the idxIndex_u16 of an MemoryAddress received from the tester in the Dcm_GetIndexOfRmba_u8
 *
 * \param     dataMemAddr_u32             : It is the address from which the tester has requested for read request
 *            nrMemLength_u32             : The length of the data to be read from the start of dataMemAddr_u32
 *            idxIndex_u16(Out parameter) : Index of Dcm_RMBAConfig_cast within which the range of memory address falls.
 * \retval
 *            E_OK                        : idxIndex_u16 successfully calculated
 *            E_NOT_OK                    : Memory address was not found.
 * \seealso:
 *
 ***********************************************************************************************************************
 */

static Std_ReturnType Dcm_Prv_GetAddressRangeIndex_u8(uint32 dataMemAddr_u32,
                                                            uint32 nrMemLength_u32,
                                                            uint16 * idxIndex_u16)
{

    uint16 dataSize_u16;
    uint16 idxLoop_u16;
    Std_ReturnType dataRetVal_u8;

    /* Initialize local variables */
    dataRetVal_u8=E_NOT_OK;

    if((0u != nrMemLength_u32) && (nrMemLength_u32 <= (DCM_MAX_READMEMRANGE - dataMemAddr_u32)))
    {
        dataSize_u16 = Dcm_RmbacalculateTableSize_u16();
        for(idxLoop_u16 = 0;idxLoop_u16<dataSize_u16;idxLoop_u16++)
        {
            if((dataMemAddr_u32 >= Dcm_RMBAConfig_cast[idxLoop_u16].dataReadMemoryRangeLow_u32) &&
                                   ((dataMemAddr_u32 + (nrMemLength_u32-1u))<=
                                   (Dcm_RMBAConfig_cast[idxLoop_u16].dataReadMemoryRangeHigh_u32)))
            {
                *idxIndex_u16 = idxLoop_u16;
                dataRetVal_u8 = E_OK;
                break;
            }
        }
    }
    return (dataRetVal_u8);
}

/**
 ***********************************************************************************************************************
 * Dcm_Prv_RmbaTotalCheckLength :    Checks the address and data size
 *                                   address and data size cannot be 0 bytes and beyond 4 bytes
 *
 * \param               pMsgContext             :   Pointer to message structure
 *                      dataNegRespCode_u8      :   Pointer to a Byte in which to store a negative Response code
 *                                                  in case of detection of an error in the request.
 *
 * \retval              None
 ***********************************************************************************************************************
*/
/*TRACE[SWS_Dcm_00853]
  TRACE[SWS_Dcm_00493]*/
LOCAL_INLINE void Dcm_Prv_RmbaTotalCheckLength(
                                 const Dcm_MsgContextType * pMsgContext,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    Std_ReturnType dataRetGetIdxRMBA_u8;/* Variable to store the return from Binary Search function */
    *dataNegRespCode_u8 = 0x00;

    if((DCM_MINSIZE!=Dcm_dataMemaddrsize_u8)&&(DCM_MINSIZE!=Dcm_dataMemdatasize_u8)&&
      ((Dcm_dataMemdatasize_u8 <=DCM_MAXSIZE)&&(Dcm_dataMemaddrsize_u8<=DCM_MAXSIZE)))
    {
        /*Total Length Check, 1 added is for the address length field format identifier field*/
        if(pMsgContext->reqDataLen == (Dcm_dataMemaddrsize_u8 + Dcm_dataMemdatasize_u8 + 1u))
        {
            /*Calculate the Memory address to read requested by the tester*/
            Dcm_GetMemoryInfo(Dcm_dataMemaddrsize_u8,&(pMsgContext->reqData[1]),&Dcm_dataMemoryAddress_u32);
            /*Calculate the memory size requested by the tester*/
            Dcm_GetMemoryInfo(Dcm_dataMemdatasize_u8,&(pMsgContext->reqData[Dcm_dataMemaddrsize_u8+1]),
                                                       &Dcm_nrReadMemoryLength_u32);

            /* Check if the Memory Address sent from tester is configured in the Server */
            dataRetGetIdxRMBA_u8 = Dcm_Prv_GetAddressRangeIndex_u8(Dcm_dataMemoryAddress_u32,
                                                                   Dcm_nrReadMemoryLength_u32,
                                                                   &Dcm_idxDspRmba_u16);


            if(E_OK == dataRetGetIdxRMBA_u8)
            {
                Dcm_SrvOpstatus_u8 = DCM_CHECKDATA;
            }
            else
            {
                /*Set NRC 0x31 Memory address is not configured or
                 * it is outside the boundaries of lower and upper limits */
                *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
            }
        }
        else
        {
            /*Set NRC 0x31 Total Length check has failed */
            *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }

    }
    else
    {
        /*Set NRC 0x31 addressAndLengthFormat-Identifier is not applicable */
        *dataNegRespCode_u8 = DCM_E_REQUESTOUTOFRANGE;
    }
}

/**
 ***********************************************************************************************************************
 * Dcm_RmbaActiveSecurityLevel :     Check if the Read is allowed in the active security level
 *
 * \param     dataSecurityMask_u32    :   Variable to hold the configured security value
 *            adrRmbaConfig_pcst      :   Pointer to RMBA configuration
 *            adrNegRespCode_pu8      :   Pointer to a Byte in which to store a negative Response code
 *                                        in case of detection of an error in the request.
 *
 * \retval    DCM_RMBA_SUPPORT_CONDITION_VIOLATED   :   Conditions for reading not supported
 *            DCM_RMBA_SUPPORT_SECURITY_VIOLATED    :   Range of Address is not supported in the current security level;
 *                                                      negative response code is set to adrNegRespCode_pu8
 ***********************************************************************************************************************
 */
/*TRACE[SWS_Dcm_00494]*/
LOCAL_INLINE Dcm_RmbaSupportRet_t Dcm_Prv_RmbaActiveSecurityLevel (
             uint32 dataSecurityMask_u32,
             const Dcm_RMBAConfig_tst * adrRmbaConfig_pcst,
             Dcm_NegativeResponseCodeType * adrNegRespCode_pu8)
{
    Std_ReturnType dataModeChkRetval_u8;               /*Return value of mode rule check API*/
    Dcm_RmbaSupportRet_t dataRetVal_en;                      /* return value of this function */

    *adrNegRespCode_pu8 = 0x00;
    dataRetVal_en = DCM_RMBA_SUPPORT_OK;

            /* Check if the Read is allowed in the active security level*/
           if ((Dcm_DsldGetActiveSecurityMask_u32() & dataSecurityMask_u32) != 0x0uL)
           {

               if(adrRmbaConfig_pcst->adrUserMemReadModeRule_pfct!=NULL_PTR)
               {
                   /* Call the configured API to check for conditions before reading to the memory location */
                   dataModeChkRetval_u8 =(*adrRmbaConfig_pcst->adrUserMemReadModeRule_pfct)
                                         (adrNegRespCode_pu8,Dcm_dataMemoryAddress_u32,Dcm_nrReadMemoryLength_u32,
                                          DCM_SUPPORT_READ);
               }
               else
               {
                   /* Call the DcmAppl API to check for conditions before reading from the memory location */
                   dataModeChkRetval_u8 = DcmAppl_UserMemoryRangeModeRuleService(adrNegRespCode_pu8,
                                                                                 Dcm_dataMemoryAddress_u32,
                                                                                 Dcm_nrReadMemoryLength_u32,
                                                                                 DCM_SUPPORT_READ);
               }
               /* If the mode rule returns TRUE , ignore the NRC value */
               if((dataModeChkRetval_u8!=E_OK))
               {
                   if(*adrNegRespCode_pu8 ==0x00)
                   {
                       *adrNegRespCode_pu8 = DCM_E_CONDITIONSNOTCORRECT;
                   }
                   dataRetVal_en = DCM_RMBA_SUPPORT_CONDITION_VIOLATED;
               }
               else
               {
                   *adrNegRespCode_pu8=0;
               }

#if (DCM_CFG_DSP_MODERULEFORREADMEMORY != DCM_CFG_OFF)
               if((*adrNegRespCode_pu8==0x00) && (NULL_PTR != adrRmbaConfig_pcst->addrRmbaModeRuleChkFnc_pfct))
               {
                  (void)(*(adrRmbaConfig_pcst->addrRmbaModeRuleChkFnc_pfct))(adrNegRespCode_pu8);
               }

               if(*adrNegRespCode_pu8 !=0x00)
               {
                   dataRetVal_en=DCM_RMBA_SUPPORT_CONDITION_VIOLATED;
               }
#endif


           }
           /* If Read is not supported in active security level */
           else
           {
               *adrNegRespCode_pu8 = DCM_E_SECURITYACCESSDENIED;
               dataRetVal_en       = DCM_RMBA_SUPPORT_SECURITY_VIOLATED;
           }
   return dataRetVal_en;
}


/**
 ***********************************************************************************************************************
 * Dcm_Prv_RmbaAccessCheck_u8:
 *
 * Calculate if the Address Range at position idxIndex_u16 in the Dcm_RMBAConfig_cast is supported
 * at this point in time or not.
 *
 * \param     idxIndex_u16:             idxIndex_u16 in Dcm_RMBAConfig_cast
 *            pMsgContext:              Pointer to message structure
 *            adrNegRespCode_pu8:       Pointer to a Byte in which to store a negative Response code
 *                                      in case of detection of
 *                                      an error in the request.
 *
 * \retval    none
 *
 ***********************************************************************************************************************
 */

static void Dcm_Prv_RmbaAccessCheck_u8 (uint16 idxIndex_u16,
                                     const Dcm_MsgContextType * pMsgContext,
                                     Dcm_NegativeResponseCodeType * adrNegRespCode_pu8)
{

    uint32 dataSessionMask_u32; /* Variable to hold the configured sessions value */
    uint32 dataSecurityMask_u32; /* Variable to hold the configured security value */
    const Dcm_RMBAConfig_tst * adrRmbaConfig_pcst; /* Pointer to RMBA configuration */
    Dcm_RmbaSupportRet_t dataRetVal_en;        /* return value of this function */
    Std_ReturnType stGetMemAccess_u8;    /* Return value of Memory access check API */

    /* Initialize local variables */
    dataRetVal_en = DCM_RMBA_SUPPORT_OK;
    *adrNegRespCode_pu8     = 0x00;
    adrRmbaConfig_pcst = &Dcm_RMBAConfig_cast[idxIndex_u16];

    /*Copy the session and security level of RMBA configured*/
    dataSessionMask_u32 = adrRmbaConfig_pcst -> dataAllowedSessRead_u32;
    dataSecurityMask_u32 = adrRmbaConfig_pcst -> dataAllowedSecRead_u32;

    /* Check if the Read is allowed in the active session*/
    if ((Dcm_DsldGetActiveSessionMask_u32() & dataSessionMask_u32) != 0x0uL)
    {
        /* The DcmAppl function to check the memory access permission*/
        stGetMemAccess_u8 = DcmAppl_DcmGetPermissionForMemoryAccess_u8(Dcm_dataMemoryAddress_u32,
                                                                       Dcm_nrReadMemoryLength_u32,DCM_SUPPORT_READ);
        if(stGetMemAccess_u8 != E_OK)
        {
            *adrNegRespCode_pu8 = DCM_E_REQUESTOUTOFRANGE;
            dataRetVal_en = DCM_RMBA_SUPPORT_CONDITION_VIOLATED;
        }
        else
        {
            /* Check if the Read is allowed in the active security level*/
            dataRetVal_en = Dcm_Prv_RmbaActiveSecurityLevel(dataSecurityMask_u32,adrRmbaConfig_pcst,adrNegRespCode_pu8);
        }
    }
    /* If Read is not supported in active session */
    else
    {
        *adrNegRespCode_pu8 = DCM_E_REQUESTOUTOFRANGE;
        dataRetVal_en       = DCM_RMBA_SUPPORT_SESSION_VIOLATED;
    }

    /* Checks if the length of the data to be read is less than available DCM buffer */
    if(dataRetVal_en == DCM_RMBA_SUPPORT_OK)
    {
        if((Dcm_nrReadMemoryLength_u32 <= pMsgContext->resMaxDataLen) || (DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF))
         {
            Dcm_SrvOpstatus_u8 = DCM_PROCESSSERVICE;
         }
        else
        {
            *adrNegRespCode_pu8 = DCM_E_RESPONSETOOLONG;
        }
    }

}

#if(DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
/**
 ***********************************************************************************************************************
 * Dcm_Dsp_RMBAUpdatePage : This function is used to update the Page length and the Response buffer pointer .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */
static void Dcm_Dsp_RMBAUpdatePage(Dcm_MsgType PageBufPtr,
                                   Dcm_MsgLenType PageLen)
{
    /* Update the Response length and the Response buffer pointer */
    Dcm_DspRMBARespBufPtr_u8 = PageBufPtr;

    if(Dcm_DspRMBARespBufPtr_u8 != NULL_PTR)
    {
        Dcm_DspRMBAMaxRespLen_u32 = PageLen;
    }
}
/**
 ***********************************************************************************************************************
 * Dcm_Dsp_RMBATriggerProcessPage :
 *  This function is used to trigger the process page .
 *
 * \param           PageBufPtr    Pointer to response buffer
 *                  PageLen       Page length which can be filled
 *
 * \retval          None
 * \seealso
 *
 ***********************************************************************************************************************
 */
static void Dcm_Dsp_RMBATriggerProcessPage(Dcm_MsgLenType PageLen)
{
    /* If first page is not sent yet */
    if(Dcm_RmbaFirstPageSent_b == FALSE)
    {
        /* Set the variable to TRUE indicating first page is being sent */
        /*first page is sent */
        Dcm_RmbaFirstPageSent_b = TRUE;
    }

    /* Send the page for transmission */
    Dcm_ProcessPage(PageLen);
}

/**
 ***********************************************************************************************************************
 * Dcm_Prv_RmbaPageBufferReadDataRecord : This function is used to call the callout function and return the service return
 *                                 value.
 *
 *
 * \param               pMsgContext         : Pointer to message structure
 *                      dataNegRespCode_u8  : Pointer to a Byte in which to store a negative Response code
 *                                            in case of detection of an error in the request
 *
 * \retval              E_OK            :   Request for read is successful
 *                      DCM_E_PENDING   :   Request for read is not yet finished
 *                      DCM_E_FORCE_RCRRP:  Application requests the transmission of a response Pending
 *                      E_NOT_OK        :   Request for read not successful
 ***********************************************************************************************************************
 */
/* BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-4335 */
static Std_ReturnType Dcm_Prv_RmbaPageBufferReadDataRecord(
                                 Dcm_MsgContextType * pMsgContext,
                                 uint32 s_posnTarget_u32,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /* variable for storing the memory address size */
    Dcm_ReadMemoryRet_t dataReadReturnVal_en;
    Std_ReturnType dataServRet_u8;

    /*Initialization of Local Variables*/
    dataReadReturnVal_en = DCM_READ_FAILED;
    dataServRet_u8 = E_NOT_OK;

    /*All the conditions met, Place a call out to Dcm_ReadMemory*/
    dataReadReturnVal_en = DcmAppl_Dcm_ReadMemory (
            Dcm_stRmbaOpstatus_u8,Dcm_RMBAConfig_cast[Dcm_idxDspRmba_u16].dataMemoryValue_u8,
            Dcm_dataMemoryAddress_u32,s_datalength_u32,&pMsgContext->resData[s_posnTarget_u32],dataNegRespCode_u8);

    if(DCM_READ_OK == dataReadReturnVal_en)
    {
        /* Keep track of the filled response in a page */
        s_FilledReadMemoryLength_u32 += s_datalength_u32;

        /* Check for the remaining bytes of data to be transmitted */
        s_Pendingreaddatalength_u32 = Dcm_nrReadMemoryLength_u32 - s_FilledReadMemoryLength_u32;

        /* To update the address based on the number of filled response bytes */
        Dcm_dataMemoryAddress_u32 += s_datalength_u32;

        /* Adding the remaining bytes left from last page*/
        s_datalength_u32 += Dcm_DsldPduInfo_st.SduLength;

        /* Trigger for process page */
        Dcm_Dsp_RMBATriggerProcessPage(s_datalength_u32);

        /* Check if the filled response length is less than the total reponse length from the request*/
        if(s_FilledReadMemoryLength_u32 < Dcm_nrReadMemoryLength_u32)
        {
            /* Complete response data is not received, to fetch the next page set opstatus to pending */
            Dcm_stRmbaOpstatus_u8 = DCM_PENDING;
            *dataNegRespCode_u8 = 0x00u;
            dataServRet_u8 = DCM_E_PENDING;
        }
        else
        {
            /*Response is already filled by the application*/
            /* Set final response length */
            pMsgContext->resDataLen = Dcm_nrReadMemoryLength_u32;
            dataServRet_u8 = DCM_E_PENDING;
            /* Reset the state of RMBA state machine to Initial */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;
            *dataNegRespCode_u8 = 0X00u;
            s_datalength_u32 = 0x00u;
            s_posnTarget_u32 = 0x00u;
            Dcm_Pending_Data_b = FALSE;
            s_FilledReadMemoryLength_u32 = 0x00u;
            s_Pendingreaddatalength_u32 = 0x00u;
        }
    }
    else if(DCM_READ_PENDING == dataReadReturnVal_en)
    {
        Dcm_stRmbaOpstatus_u8 = DCM_PENDING;
        *dataNegRespCode_u8 = 0x00u;
        dataServRet_u8 = DCM_E_PENDING;
    }
    else if(DCM_READ_FORCE_RCRRP == dataReadReturnVal_en)
    {
        Dcm_stRmbaOpstatus_u8 = DCM_FORCE_RCRRP_OK;
        Dcm_SrvOpstatus_u8 = DCM_FORCE_RCRRP_OK;
        *dataNegRespCode_u8 = 0x00u;
        dataServRet_u8 = DCM_FORCE_RCRRP_OK;

        /* Check if first page is already sent */
        if(Dcm_RmbaFirstPageSent_b == TRUE)
        {
            /* Call process page with zero length to stop the response transmission immediately */
            Dcm_ProcessPage(0);
            /*Reset the filled page length to zero, since the response is triggered*/
            s_FilledReadMemoryLength_u32 = 0x00u;
            s_datalength_u32 = 0x00u;
            s_posnTarget_u32 = 0x00u;
            s_Pendingreaddatalength_u32 = 0x00u;
            Dcm_Pending_Data_b = FALSE;
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;
        }

    }
    else
    {
        if(*dataNegRespCode_u8 == 0x00u)
        {
            /*Read has failed*/
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            dataServRet_u8 = E_NOT_OK;
            /*Reset the filled page length to zero, since the response is triggered*/
            s_FilledReadMemoryLength_u32 = 0x00u;
            s_posnTarget_u32 = 0x00u;
            Dcm_Pending_Data_b = FALSE;
        }
        Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;
    }

    return dataServRet_u8;
}

/**
 ***********************************************************************************************************************
 * Dcm_Prv_RmbaPageBuffer : This function is used to transfer large data using a smaller DCM buffer size.
 *
 *
 * \param               pMsgContext         : Pointer to message structure
 *                      dataNegRespCode_u8  : Pointer to a Byte in which to store a negative Response code
 *                                            in case of detection of an error in the request
 *
 * \retval              E_OK            :   Request for read is successful
 *                      DCM_E_PENDING   :   Request for read is not yet finished
 *                      DCM_E_FORCE_RCRRP:  Application requests the transmission of a response Pending
 *                      E_NOT_OK        :   Request for read not successful
 ***********************************************************************************************************************
 */
/* BSW_SWS_AR4_2_R2_DiagnosticCommunicationManager_Ext-4335 */
static Std_ReturnType Dcm_Prv_RmbaPageBuffer(
                                 Dcm_MsgContextType * pMsgContext,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    /* variable for service return value */
    Std_ReturnType dataServRet_u8;

    /*  Variable to indicate the position of the data to be filled in the response buffer */
    static uint32 s_posnTarget_u32;

    /*Initialization of Local Variables*/
    dataServRet_u8 = E_NOT_OK;

    /* Check whether the page size is less than or equal to the available Dcm Tx buffer */
    if(DCM_RMBAPAGESIZE <= Dcm_DspRMBAMaxRespLen_u32)
    {
        if(Dcm_Pending_Data_b == FALSE)
        {
            /* Calculation the data length to be transmitted during the first page */
            s_datalength_u32 = (Dcm_nrReadMemoryLength_u32 < DCM_RMBAPAGESIZE) ?
                                Dcm_nrReadMemoryLength_u32 : DCM_RMBAPAGESIZE;
            /* flag set to true after the first pagge transmission */
            Dcm_Pending_Data_b = TRUE;
            s_posnTarget_u32 = 0x00u;
            Dcm_DsldPduInfo_st.SduLength = 0x00u;
        }
        else
        {
            /* Calculate the data length when sdulength is other than zero and pending data is greater than page size
             * during the second page transmission */
            if ((Dcm_DsldPduInfo_st.SduLength != 0x00u) && (s_Pendingreaddatalength_u32 > DCM_RMBAPAGESIZE))
            {
                /* Calculation the data length to be transmitted for the consecutive pages */
                s_datalength_u32 = (DCM_RMBAPAGESIZE - Dcm_DsldPduInfo_st.SduLength);

                /* Calculation of the Position of the data to be filled in the response buffer */
                s_posnTarget_u32 = (Dcm_DsldPduInfo_st.SduLength) - DCM_RMBA_POSITIONBYTE;
            }
            else
            {
                /*  Calculate the data length when sdulength is zero */
                if(Dcm_DsldPduInfo_st.SduLength == 0x00u)
                {
                    /* Update the remaining data bytes for the last page transmission */
                    s_datalength_u32 = (Dcm_nrReadMemoryLength_u32 > s_Pendingreaddatalength_u32) ?
                                        DCM_RMBAPAGESIZE : s_Pendingreaddatalength_u32;

                    /* Calculation of the Position of the data to be filled in the response buffer */
                    s_posnTarget_u32 = 0x00u;
                }
                else
                {
                    /* Update the remaining data bytes for the last page transmission */
                    s_datalength_u32 = s_Pendingreaddatalength_u32;

                    /* Calculation of the Position of the data to be filled in the response buffer */
                    s_posnTarget_u32 = (Dcm_DsldPduInfo_st.SduLength) - DCM_RMBA_POSITIONBYTE;
                }
            }

        }

        dataServRet_u8 = Dcm_Prv_RmbaPageBufferReadDataRecord(pMsgContext,
                                                       s_posnTarget_u32,
                                                       dataNegRespCode_u8);
    }
    else
    {
        /*If Page size is greater than the protcol buffer length then NRC 0x14 will be set */
        *dataNegRespCode_u8 = DCM_E_RESPONSETOOLONG;
        dataServRet_u8 = E_NOT_OK;
    }
    return dataServRet_u8;
}
#endif
/**
 ***********************************************************************************************************************
 * Dcm_Prv_NormalRmbaReadMemory : Checks the read operation status and operates accordingly.
 *
 * \param               pMsgContext         : Pointer to message structure
 *                      dataNegRespCode_u8  : Pointer to a Byte in which to store a negative Response code
 *                                            in case of detection of an error in the request
 *
 * \retval              E_OK            :   Request for read is successful
 *                      DCM_E_PENDING   :   Request for read is not yet finished
 *                      DCM_E_FORCE_RCRRP:  Application requests the transmission of a response Pending
 *                      E_NOT_OK        :   Request for read not successful
 ***********************************************************************************************************************
 */
/*TRACE[SWS_Dcm_00495]*/
static Std_ReturnType Dcm_Prv_NormalRmbaReadMemory(
                                 Dcm_MsgContextType * pMsgContext,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    Dcm_ReadMemoryRet_t dataReadReturnVal_en; /* variable for storing the memory address size*/
    Std_ReturnType dataServRet_u8;

    /*Initialization of Local Variables*/
    dataReadReturnVal_en = DCM_READ_FAILED;
    dataServRet_u8=E_NOT_OK;

    /*All the conditions met, Place a call out to Dcm_ReadMemory*/
    dataReadReturnVal_en = DcmAppl_Dcm_ReadMemory (
                          Dcm_stRmbaOpstatus_u8,Dcm_RMBAConfig_cast[Dcm_idxDspRmba_u16].dataMemoryValue_u8,
                          Dcm_dataMemoryAddress_u32,Dcm_nrReadMemoryLength_u32,pMsgContext->resData,dataNegRespCode_u8);

    if(DCM_READ_OK == dataReadReturnVal_en)
    {
        /*Response is already filled by the application*/
        /* Set final response length */
        pMsgContext->resDataLen = Dcm_nrReadMemoryLength_u32;
        dataServRet_u8 = E_OK;
        /* Reset the state of RMBA state machine to Initial */
        Dcm_SrvOpstatus_u8 = DCM_INITIAL;
        Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;
        *dataNegRespCode_u8 = 0X00;
    }
    else if(DCM_READ_PENDING == dataReadReturnVal_en)
    {
        Dcm_stRmbaOpstatus_u8 = DCM_PENDING;
        *dataNegRespCode_u8 = 0x00u;
        dataServRet_u8 = DCM_E_PENDING;
    }
    else if(DCM_READ_FORCE_RCRRP == dataReadReturnVal_en)
    {
        Dcm_stRmbaOpstatus_u8 = DCM_FORCE_RCRRP_OK;
        *dataNegRespCode_u8 = 0x00u;
        dataServRet_u8 = DCM_E_FORCE_RCRRP;
    }
    else
    {
        if(*dataNegRespCode_u8 == 0x00u)
        {
            /*Read has failed*/
            *dataNegRespCode_u8 = DCM_E_GENERALREJECT;
            dataServRet_u8 = E_NOT_OK;
        }

        Dcm_stRmbaOpstatus_u8 = DCM_INITIAL;
    }
    return dataServRet_u8;
}
/**
 ***********************************************************************************************************************
 * Dcm_RmbaDataReadRetValue : Checks the read operation status and operates accordingly.
 *
 * \param               pMsgContext         : Pointer to message structure
 *                      dataNegRespCode_u8  : Pointer to a Byte in which to store a negative Response code
 *                                            in case of detection of an error in the request
 *
 * \retval              E_OK            :   Request for read is successful
 *                      DCM_E_PENDING   :   Request for read is not yet finished
 *                      DCM_E_FORCE_RCRRP:  Application requests the transmission of a response Pending
 *                      E_NOT_OK        :   Request for read not successful
 ***********************************************************************************************************************
 */
/*TRACE[SWS_Dcm_00495]*/
static Std_ReturnType Dcm_Prv_RmbaReadMemory(
                                 Dcm_MsgContextType * pMsgContext,
                                 Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    Std_ReturnType dataServRet_u8;

    /*Initialization of Local Variables*/
    dataServRet_u8 = E_NOT_OK;

#if(DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
    /* Check whether the requested response length is less than the page size */
    dataServRet_u8 = (Dcm_nrReadMemoryLength_u32 < DCM_RMBAPAGESIZE) ?
                     Dcm_Prv_NormalRmbaReadMemory(pMsgContext,dataNegRespCode_u8) :
                     Dcm_Prv_RmbaPageBuffer(pMsgContext,dataNegRespCode_u8);
#else
    dataServRet_u8 = Dcm_Prv_NormalRmbaReadMemory(pMsgContext,dataNegRespCode_u8);
#endif
    return dataServRet_u8;
}

/**
 ***********************************************************************************************************************
 * Dcm_DcmReadMemoryByAddress : Read Memory By Address (0x23) Service
 * RMBA service is used to read the Data using a physical memory address
 *                  sent in the request by the tester
 *
 * \param       OpStatus           : Output status of service
 *              pMsgContext        : Pointer to message structure.
 *              dataNegRespCode_u8 : Pointer to a Byte in which to store a negative Response code
 *                                   in case of detection of an error in the request.
 *
 * \retval      E_OK                :   Request is successful
 *              DCM_E_PENDING       :   Request for read is not yet finished
 *              DCM_E_FORCE_RCRRP   :   Application requests the transmission of a response Pending
 *              E_NOT_OK            :   Request is not successful
 * \seealso
 * \usedresources
 ***********************************************************************************************************************
 */
/* TRACE[SWS_Dcm_00492] */
Std_ReturnType Dcm_DcmReadMemoryByAddress (Dcm_SrvOpStatusType OpStatus,
                                       Dcm_MsgContextType * pMsgContext,
                                       Dcm_NegativeResponseCodeType * dataNegRespCode_u8)
{
    Std_ReturnType dataServRet_u8;   /* Variable to store the return from service */

    /*Initialization of Local Variables*/
    dataServRet_u8=E_NOT_OK;
    *dataNegRespCode_u8=0x0;

   /* For cancellation of service -Rmba is called with opstatus set to Dcm_cancel */
    if(OpStatus == DCM_CANCEL)
    {
        Dcm_RMBAIni();
        dataServRet_u8 = E_OK;
    }
    else
    {
        /* Rmba State Machine */
        if (Dcm_SrvOpstatus_u8 == DCM_INITIAL)
        {
            /* Check for the minimum request length equal to 0x03 bytes as 1 byte addressAndLengthFormatIdentifier
            and  one byte of memoryAddress, one byte of memorySize is mandatory in RMBA request */
            if(pMsgContext->reqDataLen >= DCM_DSP_RMBA_MINREQLEN)
            {
                /* Extract the memory address and data record size information from the request buffer */
                Dcm_dataMemaddrsize_u8 = ((pMsgContext->reqData[0])& (0xFu));
                Dcm_dataMemdatasize_u8 = (uint8)(((pMsgContext->reqData[0])& (0xF0u))>>4u);

                /*address and data size cannot be 0 bytes and beyond 4 bytes(Implicit requirement as call of
                 Dcm_ReadMemory for address and data size parameter is of type uint32)*/
                Dcm_Prv_RmbaTotalCheckLength(pMsgContext, dataNegRespCode_u8);
            }
            else
            {
                /*Set NRC Request Length incorrect */
                *dataNegRespCode_u8 = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            }
        }
        /* Rmba State Machine */
        if (Dcm_SrvOpstatus_u8 == DCM_CHECKDATA)
        {
            /* Call the function to see if there is a Read support @ the address requested by tester */
            Dcm_Prv_RmbaAccessCheck_u8(Dcm_idxDspRmba_u16,pMsgContext,dataNegRespCode_u8);

#if(DCM_CFG_RMBAPAGEDBUFFERSUPPORT != DCM_CFG_OFF)

            /*Variable set to false to indicate that the first page is not set*/
            Dcm_RmbaFirstPageSent_b = FALSE;

            /*Variable to indicates rmba data was pending */
            Dcm_Pending_Data_b = FALSE;

            /* Total data response to be transmitted*/
            pMsgContext->resDataLen = Dcm_nrReadMemoryLength_u32;

            Dcm_adrUpdatePage_pfct = &Dcm_Dsp_RMBAUpdatePage;

            /* Call the Dcm API and start Paged processing */
            Dcm_StartPagedProcessing(pMsgContext);
#endif
        }

        if (Dcm_SrvOpstatus_u8 == DCM_PROCESSSERVICE)
        {
            dataServRet_u8 = Dcm_Prv_RmbaReadMemory(pMsgContext, dataNegRespCode_u8);
        }

        /* Check if Negative response is set */
        if(*dataNegRespCode_u8 != 0x00u)
        {
            /* Reset the state of RMBA state machine to Initial */
            Dcm_SrvOpstatus_u8 = DCM_INITIAL;
            dataServRet_u8=E_NOT_OK;
        }
    }
    return dataServRet_u8;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"
#endif

