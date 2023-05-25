

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"


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
 Function name    : Com_DeInit
 Description      : Service for COM DeInitialization
 Parameter        : void
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_DeInit(void)
{
    Com_TxIpduRam_tpst          txIpduRamPtr_pst;
    Com_RxIpduRam_tpst          rxIpduRamPtr_pst;
    uint16_least                index_qu16;

    /*Interrupt is locked due to the updation of Com_InitStatus_en, Ipdu Group Status & IPdu Group DM Status
      In most of the API's, if Ipdu group is stopped, no further processing of data is performed.*/

    /* Check if the module is already Initialised */
    if(Com_InitStatus_en == COM_INIT)
    {
        /* Reset the UNINIT flag */
        Com_InitStatus_en = COM_UNINIT;

        /* clear IpduGroupVector's */
        for (index_qu16 = COM_ZERO; index_qu16 < COM_NUM_ARR_IPDUGRPVECT ; index_qu16++)
        {
            Com_IpduGrpVector_au8[index_qu16]    = COM_ZERO;  /* reset the Ipdu Group vector array */

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
            Com_IpduGrpVector_DM_au8[index_qu16] = COM_ZERO;  /* reset the Dealine Monitoring */
#endif
        }

        /* Start: Reset all Tx Flags */
        txIpduRamPtr_pst = &COM_GET_TXPDURAM_S(0);

        for (index_qu16 = COM_ZERO; index_qu16 < COM_GET_NUM_TX_IPDU; index_qu16++)
        {
            txIpduRamPtr_pst->txFlags_u16 = COM_ZERO;

            COM_GET_IPDUCOUNTER_S(index_qu16 + COM_GET_NUM_RX_IPDU) = COM_ZERO;

#ifdef COM_TxIPduTimeOut
            txIpduRamPtr_pst->cntrTxTimeout_u16     = COM_ZERO;
#endif
            txIpduRamPtr_pst->cntrRepetitions_u8    = COM_ZERO;
            txIpduRamPtr_pst->cntrMinDelayTime_u16  = COM_ZERO;

            txIpduRamPtr_pst++;
        }
        /* End: Reset all Tx Flags */

        /* Start: Reset all Rx Flags */
        rxIpduRamPtr_pst = &COM_GET_RXPDURAM_S(0);

        for (index_qu16 = COM_ZERO; index_qu16 < COM_GET_NUM_RX_IPDU; index_qu16++)
        {
            rxIpduRamPtr_pst->rxFlags_u8      = COM_ZERO;

            COM_GET_IPDUCOUNTER_S(index_qu16)  = COM_ZERO;        /* reset of the Rx ipdu status */
# if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
            COM_GET_IPDUCOUNTER_DM(index_qu16) = COM_ZERO;        /* reset of the Rx dealine monitoring */
# endif

            rxIpduRamPtr_pst++;
        }
        /* End: Reset all Rx Flags */
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

