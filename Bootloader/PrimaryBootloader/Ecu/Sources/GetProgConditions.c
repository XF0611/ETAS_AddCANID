/*********************************************************************************************************************
*
* Description:     
* FileName:        SWDL_GetProgConditions.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/


/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Dcm.h"
#include "FBL_BootM.h"
#include "DcmCore_DslDsd_Prot.h"
#include "DcmDspUds_Dsc_Prot.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/
/**
 * @ingroup DCM_TPL
 * Dcm_GetProgConditions :-\n
 * This API is used for restoring the Programming Information by the application.\n
 * This API should  restore the Programming conditions from the Non volatile memory which was stored during Jump To Boot.\n
 * In this API user has to restore the protocol information required to start Dcm\n
 *
 *  @param[in] ProgConditions \n
 *  The following information needs to be filled by the user in ProgConditions\n
 *     1. ProtocolID - protocol which needs to be started as default\n
 *     2. Tester Source address - Connection will be derived from this input\n
 *     3. SID - service identifier (In case of Warm request/Warm response)\n
 *     4. SubFncId - Subfunction ID\n
 *     5. StoreType - Storage Type used for restoring, Warm Request/Warm Response/Warm Init\n
 *     6. SessionLevel - Session level which needs to be started\n
 *     7. SecurityLevel - Security level which needs to be started\n
 *     8. ReqResLen - Total request/response length including SID and Subfunc\n
 *     9. Request/Response bytes which follows the SID and Subfunc\n
 *     10.Number of Response pending 0x78 triggered before the jump\n
 *     11.Elapsed time in Drive or Boot\n
 *     12. NumOfSecurityLevels - Number of security levels configured,
 *           If the configuration parameter DcmDspSecurityStoreDelayCountAndTimerOnJump  is set to True, then the below information has to be stored
 *     13. SecurityInformation[].SecurityLevel = Security level\n
 *     14.SecurityInformation[].DelayCount = The failed attempt count per SecurityLevel\n
 *     15.SecurityInformation[].ResidualDelay = Remaining delay (either remaining powerOndelay/remaining delay time) per SecurityLevel which will be started in Dcm per security level\n
 *     16.freeForProjectUse 6 bytes of free space is provided for projects to store additional information like CAN ID, BAUD Rate, etc..\n
 *
 * @param[out]      ProgConditions : Pointer to the location where the programming conditions  to be stored.
 *
 *
 * @retval                 DCM_WARM_START : The ECU starts from a bootloader jump\n
 * @retval                 DCM_COLD_START : Start ECU Normally\n
 *
 */
FUNC(Dcm_EcuStartModeType,DCM_APPL_CODE) Dcm_GetProgConditions                                                                          /* PRQA S 1503, 3408 */
                                   (P2VAR(Dcm_ProgConditionsType,AUTOMATIC,DCM_INTERN_DATA) ProgConditions)
{
    VAR(Dcm_EcuStartModeType,AUTOMATIC) retVal = DCM_COLD_START;
    Fbl_BootM_ReprogrammingRequestFlagStateType reprogrammingRequestFlagState;
    uint16 dataTimingValue_u16;
	/* MISRA RULE 0310: Casting to different object pointer type. */
    if(Fbl_BootM_ReadReprogrammingRequestFlag(&reprogrammingRequestFlagState) == E_OK &&                                            /* PRQA S 3397 */
        FBL_BOOTM_REPROGRAMMING_REQUEST_FLAG_STATE_ON == reprogrammingRequestFlagState)
    {
        /*Make a positvie response*/
        ProgConditions->StoreType       = DCM_WARMRESPONSE_TYPE;
        retVal = DCM_WARM_START;
        ProgConditions->TesterSourceAddr    = 0x00;
        ProgConditions->Sid                 = 0x50;
        ProgConditions->SubFncId            = 0x02;
        ProgConditions->ProtocolId          = DCM_UDS_ON_CAN;
        ProgConditions->SessionLevel        = DCM_PROGRAMMING_SESSION;
        ProgConditions->SecurityLevel       = 1;
        ProgConditions->ReqResLen           = 0x7;
        ProgConditions->ResponseRequired    = TRUE;

        /* Fill P2 Max Time */
        dataTimingValue_u16 = (uint16)(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].P2_max_u32 / 1000u);
        Dcm_ProgConditions_st.ReqResBuf[0] = (uint8)(dataTimingValue_u16 >> 8u);
        Dcm_ProgConditions_st.ReqResBuf[1] = (uint8)(dataTimingValue_u16 & 0x00ffu);
        /* Fill P2 Star Max Time */
        dataTimingValue_u16 = (uint16)(Dcm_Dsp_Session[Dcm_ctDiaSess_u8].P2str_max_u32 / 10000u);
        Dcm_ProgConditions_st.ReqResBuf[2] = (uint8)(dataTimingValue_u16 >> 8u);
        Dcm_ProgConditions_st.ReqResBuf[3] = (uint8)(dataTimingValue_u16 & 0x00ffu);
    }
    return(retVal);
}
