
#include "Dcm_Cfg_Prot.h"
#include "DcmDspUds_Uds_Inf.h"
#include "Rte_Dcm.h"
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)


/**
 **************************************************************************************************
 * Dcm_DidServices_F186_ReadData : API to read the Session ID (DID 0xF186)
 * \param          adrData_pu8: pointer to array to be filled
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
Std_ReturnType Dcm_DidServices_F186_ReadData(uint8 * adrData_pu8)
{
    Std_ReturnType dataRetVal_u8;
    Dcm_SesCtrlType dataSessId_u8;

    /* TRACE [SWS_Dcm_00085] */
    /* get current active session Id */
    dataRetVal_u8 = Dcm_GetSesCtrlType(&dataSessId_u8);

    /* and copy it to the response buffer if no error was returned */
    if(dataRetVal_u8 == E_OK)
    {
    	adrData_pu8[0] = (uint8)dataSessId_u8;
    }

    return dataRetVal_u8;
}

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif


