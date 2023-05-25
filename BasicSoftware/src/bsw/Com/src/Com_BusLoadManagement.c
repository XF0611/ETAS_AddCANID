

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"


#if (COM_COMMON_TX_PERIOD == STD_ON)
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
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
uint16 Com_ReducedPeriod_u16;
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 Function name    : Com_ReduceBusload
 Description      : The below function is used to change the Periodicity of all the Tx frames
                    (periodic mode IPDUs and periodic part of mixed mode IPDUs)to a new value irrespective of their
                    current cycle time. This can be used in the scenario where the Bus load has to be reduced
                    while the tester is flashing other ECU.
                    Input period is expected in resolution of Com_MainFunctionTx() raster time.
                    (for ex: If Com_MainFunctionTx() is configured in 10ms raster then for 30 seconds  Cycletime = 3000)
                    All message period is not changed to new period immediately.If there are message scheduled to be
                    transmitted , then they will be transmitted with old period once. After this the period will be
                    changed to new period.
 NOTE             : Event triggered transmission is not affected by this functionality.
 Parameter        : commonPeriod_u16 - New period (interms of multiples of Com_MainFunctionTx() raster time)
 Return value     : void
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_ReduceBusload(uint16 commonPeriod_u16)
{
    /* Check if COM module is initialised */
# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_ReduceBusload, COM_E_UNINIT);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        Com_ReducedPeriod_u16 = commonPeriod_u16;
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_RestoreBusload
 Description      : The below function is used to change the period of all the Tx frames to a old period from
                    common TX period, set by function Com_ReduceBusload(). After the call of the Com_ReduceBusload(),
                    the configured period is applied immediately (ie the timer is loaded and
                    started but the IPDU is not transmitted immediately).The periodic part of the IPDU is transmitted at
                    the end of the configured period, respecting MDT.
 Parameter        : CycleTime - New period (interms of multiples of Com_mainFunctionTx() raster time)
 Return value     : void
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_RestoreBusload (void)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    uint16_least                idxIdTxIpdu_qu16;

# if (COM_PRV_ERROR_HANDLING == STD_ON)
    if (Com_InitStatus_en == COM_UNINIT)
    {
        COM_DET_REPORT_ERROR(COMServiceId_RestoreBusload, COM_E_UNINIT);
    }
    else
# endif /* end of COM_PRV_ERROR_HANDLING */
    {
        /* common TX period is active */
        if (Com_ReducedPeriod_u16 != COM_ZERO)
        {
            Com_ReducedPeriod_u16 = COM_ZERO;

            txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(0);

            /* Loop through all TX IPDUs */
            for (idxIdTxIpdu_qu16 = COM_ZERO; idxIdTxIpdu_qu16 < COM_GET_NUM_TX_IPDU; idxIdTxIpdu_qu16++)
            {
                /* Reload the configured period for PERIODIC and MIXED mode IPDUs.
                   These IPDUs will be transmitted with configured period , after this function call */
                /* The CurrentTxModePtr points to the structure information of the current TxModeState */
                if (COM_TX_MODE_IS_CYCLIC(txIpduRamPtr_pst->currentTxModePtr_pcst->mode_u8))
                {
                    txIpduRamPtr_pst->cntrTimePeriod_u16 = txIpduRamPtr_pst->currentTxModePtr_pcst->timePeriod_u16;
                }

                txIpduRamPtr_pst++;
            }/* END for loop */
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_COMMON_TX_PERIOD */

