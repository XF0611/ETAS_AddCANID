
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Uds_Inf.h"
#include "Rte_Dcm.h"

#if((DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF))

#define DCM_START_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
uint8 Dcm_VInData_au8[DCM_VIN_DATALEN] = {0};  /*Buffer to store VIN data*/
#define DCM_STOP_SEC_VAR_CLEARED_8 /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_VinReceived_b = FALSE; /*Flag to indicate VIN data is received successfully or not */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
static Dcm_DIDIndexType_tst s_Dcm_idxRdbiDidIndexType_GetVIN_st; /*Store the index of DID of either Dcm_DidConfig or Dcm_DIDRangeCOnfig structure*/


#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
boolean Dcm_GetvinConditionCheckRead = FALSE; /*Flag to indicate Conditioncheck read function is successfully done or not */
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/* BSW-14097 */
#define DCM_START_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
static boolean s_VIN_DcmReadDataConditionsRteCallPlaced_b;

/* BSW-14097 */
#endif
#define DCM_STOP_SEC_VAR_CLEARED_BOOLEAN /*Adding this for memory mapping*/
/* END BSW-14097 */
#include "Dcm_MemMap.h"

/* BSW-14097 - Move #endif to prior to #define DCM_STOP_SEC_CODE*/

/* BSW-1478 */
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* END BSW-1478 */
/**
 **********************************************************************************************************************
 * Dcm_GetVin :
 *  This function is called by DOIP for reading the VIN DID.
 *
 * \param           : *Data - pointer to the target buffer(17 bytes) provided by DOIP
 * \return          : Std_ReturnType
 * \retval          : E_OK - if the VIN DID is read successfully
 * 					  E_NOT_OK - if the VIN DID is not read successfully
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */

/* TRACE: [SWS_Dcm_00950] */

Std_ReturnType Dcm_GetVin (uint8 * Data)
{
    Std_ReturnType dataStatusReturn_u8 = E_NOT_OK;
    uint8 indexVinBuff = 0;
    /* Check if VIN data is available or Application buffer is available */
    if((Dcm_VinReceived_b != FALSE) && (Data != NULL_PTR))
    {

        /* Store VIN DID in application Buffer  */
       for(indexVinBuff = 0;indexVinBuff < DCM_VIN_DATALEN ;indexVinBuff++)
       {
          *(Data) = Dcm_VInData_au8[indexVinBuff];
		  Data++;
       }

       if(indexVinBuff == DCM_VIN_DATALEN)
       {
         /*17 bytes data is copied into Application buffer*/
           dataStatusReturn_u8 = E_OK;
       }
    }
    return dataStatusReturn_u8;
}

/**
 **********************************************************************************************************************
 * Dcm_VinBuffer_Init :
 *  This function is called by Dcm_MainFunction for reading the VIN DID data during start up .
 *  if VIN DID data is read successfully , it is stored in Dcm_VInData_au8[]
 *

 * \return          : Std_ReturnType
 * \retval          : E_OK - if the VIN DID is read successfully
 *                    E_NOT_OK - if the VIN DID is not read successfully
 *                    DCM_E_PENDING - More time is required to read the data\n
 * \seealso
 * \usedresources
 **********************************************************************************************************************
 */

/* TRACE: [SWS_Dcm_01174] */

Std_ReturnType Dcm_VinBuffer_Init(void)
{

    Std_ReturnType dataStatusReturn_u8;/* variable to hold the return value */
    Std_ReturnType dataCondChkRetVal_u8; /* Return value of Condition check API */

    Dcm_NegativeResponseCodeType dataNegRespCode_u8; /* variable for Negative response value */
    const Dcm_DIDConfig_tst *ptrDidConfig;
    const Dcm_DataInfoConfig_tst *ptrSigConfig;
    const Dcm_SignalDIDSubStructConfig_tst *ptrControlSigConfig;
    static uint16 idxVinDidSignal_u16 =0x00u;   /* Signal index for the VIN DID */
    dataCondChkRetVal_u8 = E_OK; /* Initialize to E_OK */
    /* Call the API to get the index of the requested VIN DID */
    dataStatusReturn_u8 = Dcm_Prv_GetIndexOfDID(DCM_CFG_VIN_DID,&s_Dcm_idxRdbiDidIndexType_GetVIN_st);
    ptrDidConfig = &Dcm_DIDConfig[s_Dcm_idxRdbiDidIndexType_GetVIN_st.idxIndex_u16];
    /* Initialization of the variables */
    dataNegRespCode_u8=0;
#if(DCM_CFG_DSP_NUMISDIDAVAIL>0)
    /* Check if the DID is supported in current variant */
    /*If data range of Did is a range did or not and supported in the current variant*/
    if((s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataRange_b ==FALSE) && (*Dcm_DIDIsAvail[ptrDidConfig->idxDIDSupportedFnc_u16] != NULL_PTR))
    {
        if((*(IsDIDAvailFnc_pf)(Dcm_DIDIsAvail[ptrDidConfig->idxDIDSupportedFnc_u16]))(DCM_CFG_VIN_DID)!=E_OK)
        {
            dataStatusReturn_u8 = E_NOT_OK;
        }
    }
#endif
    /* Call the Dcm_GetDIDData() API to read the VIN DID */
    /* If the return value is E_OK */
    if(dataStatusReturn_u8 == E_OK)
    {
        /*Xxx_ConditionCheckRead() is sucessfully done or not for each signal */
        if(Dcm_GetvinConditionCheckRead == FALSE)
        {
        while(((idxVinDidSignal_u16<ptrDidConfig->nrSig_u16)&&(dataNegRespCode_u8==0x0)&&(dataCondChkRetVal_u8==E_OK)))
        {
            ptrSigConfig = &Dcm_DspDataInfo_st[ptrDidConfig->adrDidSignalConfig_pcst[idxVinDidSignal_u16].idxDcmDspDatainfo_u16];
            ptrControlSigConfig = &Dcm_DspDid_ControlInfo_st[ptrSigConfig->idxDcmDspControlInfo_u16];
            if((ptrSigConfig->adrReadFnc_cpv.ReadFunc1_pfct)!= NULL_PTR)
            {
                if(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc1_pfct != NULL_PTR)
                {
                    if((ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_FNC) ||
                            (ptrSigConfig->usePort_u8 == USE_DATA_SYNCH_CLIENT_SERVER))
                    {
                        dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc1_pfct))(&dataNegRespCode_u8);
                    }
                    else
                    {
                        if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_FNC) ||
#if(DCM_CFG_RDBIPAGEDBUFFERSUPPORT != DCM_CFG_OFF)
                                (ptrSigConfig->usePort_u8 == USE_DATA_RDBI_PAGED_FNC) ||
#endif
                                (ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER))
                        {
#if (DCM_CFG_DSP_READ_ASP_ENABLED != DCM_CFG_OFF)
                            if((ptrSigConfig->usePort_u8 == USE_DATA_ASYNCH_CLIENT_SERVER) && (ptrSigConfig->UseAsynchronousServerCallPoint_b))
                            {
                                if(!s_VIN_DcmReadDataConditionsRteCallPlaced_b)
                                {
                                    dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc3_pfct))(s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b);
                                    if(dataCondChkRetVal_u8 == E_OK)
                                    {
                                        s_VIN_DcmReadDataConditionsRteCallPlaced_b = TRUE;
                                        dataCondChkRetVal_u8 = DCM_E_PENDING;
                                    }
                                }
                                else
                                {
                                    /* MR12 RULE 1.3 VIOLATION: A function-like macro shall not be invoked without all of its arguments, hence some of the arguments are optional based on configuration */
                                    dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFncResults_cpv.CondChkReadResFunc1_pfct))(&dataNegRespCode_u8);
                                    if(dataCondChkRetVal_u8 == RTE_E_NO_DATA)
                                    {
                                        dataCondChkRetVal_u8 = DCM_E_PENDING;
                                    }
                                    else
                                    {
                                        s_VIN_DcmReadDataConditionsRteCallPlaced_b = FALSE;
                                    }
                                }
                            }
                            else
#endif
                            {
                                dataCondChkRetVal_u8 = (*(ptrControlSigConfig->adrCondChkRdFnc_cpv.CondChkReadFunc2_pfct))(s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b, &dataNegRespCode_u8);
                            }
                        }
                    }
                }
            }


            if(dataCondChkRetVal_u8==E_OK)
            {
                s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b=DCM_INITIAL;
            }
            if(dataCondChkRetVal_u8 != DCM_E_PENDING)
            {
                idxVinDidSignal_u16++;
            }
        }



        }
        if(dataCondChkRetVal_u8==DCM_E_PENDING)
        {
            dataStatusReturn_u8=DCM_E_PENDING;
            s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b=DCM_PENDING;
        }

        else
        {
            /*RESET the index of the signal*/
            idxVinDidSignal_u16 =0x00;
            if(dataCondChkRetVal_u8==E_OK)
            {
                Dcm_GetvinConditionCheckRead=TRUE;
                dataStatusReturn_u8=Dcm_GetDIDData(&s_Dcm_idxRdbiDidIndexType_GetVIN_st,&Dcm_VInData_au8[0]);

                if(dataStatusReturn_u8==DCM_E_PENDING)
                {
                    s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b=DCM_PENDING;

                }

            }
            else
            {

                dataStatusReturn_u8=E_NOT_OK;
            }
        }
        if(dataStatusReturn_u8 == E_OK)
        {
            /*Set flag is true as VIN data is read successfully*/
            Dcm_VinReceived_b = TRUE;
            Dcm_GetvinConditionCheckRead=FALSE;
            s_Dcm_idxRdbiDidIndexType_GetVIN_st.dataopstatus_b=DCM_INITIAL;

        }
    }

    return dataStatusReturn_u8;

}
/* BSW-1478 */
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
/* END BSW-1478 */
#endif  /* ((DCM_CFG_VIN_SUPPORTED != DCM_CFG_OFF) && (DCM_CFG_DIDSUPPORT != DCM_CFG_OFF)) */
