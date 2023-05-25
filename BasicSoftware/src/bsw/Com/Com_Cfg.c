


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
#include "Com_Cbk.h"
#define COM_PBCFG_INCLUDE_INT_SYM_NAMES
#include "Com_PBcfg_InternalSymbolicNames.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
Com_IpduGroupVector Com_IpduGrpVector_au8;

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
Com_IpduGroupVector Com_IpduGrpVector_DM_au8;
#endif

#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
/* the Array to hold the Com Tx Ipdu control vector */
Com_TxIpduCtrlVector_tau8 Com_TxIpduControlVector_au8;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* end of COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */

#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
/* the Array to hold the Com Rx Ipdu control vector */
Com_RxIpduCtrlVector_tau8 Com_RxIpduControlVector_au8;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* end of COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */

/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/
/* START: TMS NONE Details  */

#define COM_START_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"
const Com_TransModeInfo_tst Com_NONE_TransModeInfo_cst =
{

    0, /* timePeriod_u16 */
    0, /* timeOffset_u16 */

    0, /* repetitionPeriod_u16 */
    0, /* numOfRepetitions_u8 */

#ifdef COM_MIXEDPHASESHIFT
    COM_TXMODE_NONE, /* mode_u8 */
    COM_FALSE   /* mixedPhaseShift_b status */
#else

    COM_TXMODE_NONE /* mode_u8 */

#endif /* #ifdef COM_MIXEDPHASESHIFT */


};
#define COM_STOP_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"

/* END: TMS NONE Details  */

/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 Rx-MainFunction Timebase - 0.01 s
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_MainFunctionRx(void)
{
#ifdef COM_ENABLE_MAINFUNCTION_RX
    Com_Prv_InternalMainFunctionRx( (Com_MainFunc_tuo)ComMainFunction_Internal_MainFunctionRx );
#endif
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Tx-MainFunction Timebase - 0.01 s
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

void Com_MainFunctionTx(void)
{
    Com_Prv_InternalMainFunctionTx( (Com_MainFunc_tuo)ComMainFunction_Internal_MainFunctionTx );
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/* START : Tx IPDU notification functions */
#ifdef COM_TxIPduNotification
#endif /* #ifdef COM_TxIPduNotification */
/* END : Tx IPDU notification functions */
/* START : Tx IPDU error notification functions */
/* END : Tx IPDU error notification functions */
/* START : Tx IPDU timeout  notification functions */
#ifdef COM_TxIPduTimeOutNotify
#endif /* #ifdef COM_TxIPduTimeOutNotify */
/* END : Tx IPDU timeout  notification functions */


/* START : Rx IPDU timeout  notification functions */
#ifdef COM_RxIPduTimeoutNotify

#endif /* #ifdef COM_RxIPduTimeoutNotify */

/* END : Rx IPDU timeout  notification functions */







#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
/*
 **********************************************************************************************************************
 Function name    : Com_SetTxIPduControlViaRbaNdsEcuVariant
 Description      : Service called by rba_ComScl to set/reset the status of Tx Ipdu
 Parameter        : idIpdu_uo    -> ID of the Tx IPDU
                  : ipduStatus_b -> TxIpdu status maintained by rba_ComScl
 Return value     : none
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_SetTxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo, boolean ipduStatus_b)
{
# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetTxIPduControlViaRbaNdsEcuVariant, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidTxIpduId(idIpdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetTxIPduControlViaRbaNdsEcuVariant, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        uint16   index_u16;
        uint8    bitOffset_u8;

        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idIpdu_uo = COM_GET_TX_IPDU_ID(idIpdu_uo);

        index_u16    = idIpdu_uo >> 3u;
        bitOffset_u8 = (uint8)(idIpdu_uo % 8u);

        if (ipduStatus_b)
        {
            Com_TxIpduControlVector_au8[index_u16] |= (uint8)(1u << bitOffset_u8);
        }
        else
        {
            Com_TxIpduControlVector_au8[index_u16] &= (uint8)(~((uint8)(1u << bitOffset_u8)));
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* end of COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */

#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
/*
 **********************************************************************************************************************
 Function name    : Com_SetRxIPduControlViaRbaNdsEcuVariant
 Description      : Service called by rba_ComScl to set/reset the status of Rx Ipdu
 Parameter        : idIpdu_uo    -> ID of the Rx IPDU
                  : ipduStatus_b -> RxIpdu status maintained by rba_ComScl
 Return value     : none
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_SetRxIPduControlViaRbaNdsEcuVariant(PduIdType idIpdu_uo, boolean ipduStatus_b)
{
# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetRxIPduControlViaRbaNdsEcuVariant, COM_E_UNINIT);
    }
    else if (!Com_Prv_IsValidRxIpduId(idIpdu_uo))
    {
        COM_DET_REPORT_ERROR(COMServiceId_SetRxIPduControlViaRbaNdsEcuVariant, COM_E_PARAM);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        uint16   index_u16;
        uint8    bitOffset_u8;

        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        idIpdu_uo = COM_GET_RX_IPDU_ID(idIpdu_uo);

        index_u16    = idIpdu_uo >> 3u;
        bitOffset_u8 = (uint8)(idIpdu_uo % 8u);

        if (ipduStatus_b)
        {
            Com_RxIpduControlVector_au8[index_u16] |= (uint8)(1u << bitOffset_u8);
        }
        else
        {
            Com_RxIpduControlVector_au8[index_u16] &= (uint8)(~((uint8)(1u << bitOffset_u8)));
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* end of COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */





