
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "CanSM_Prv.h"


/**
***************************************************************************************************
** Function Name        : CanSM_GetCurrentComMode
**
** Service ID           : 0x03
**
** Description          : This service shall put out the current communication mode of a CAN network.
**
** Sync/Async           : Synchronous
**
** Re-entrancy          : Reentrant
**
** Input Parameters     : Network- Whose current communication mode shall be put out
**
** InOut parameter      : None
**
** Output Parameters    : ComM_ModePtr-Pointer, where to put out the current communication mode
**
** Return parameter     : Std_ReturnType- E_OK: Service Accepted, E_NOT_OK: Service Denied

***************************************************************************************************
**/
#define CANSM_START_SEC_CODE
#include "CanSM_MemMap.h"
Std_ReturnType CanSM_GetCurrentComMode( NetworkHandleType network,ComM_ModeType * ComM_ModePtr )
{
    Std_ReturnType stFuncVal;
    boolean CanSM_NwInitStatus_b;
    /*Initialise retVal*/
    stFuncVal = E_NOT_OK;

	/* Report DET : Development Error Handling for CANSM is Uninitialized and returns E_NOT_OK*/
    CANSM_REPORT_ERROR_NOK((CanSM_Init_ab == (boolean)CANSM_UNINITED),
                           (uint8)CANSM_GETCURRENTCOMMODE_SID,
                           (uint8)CANSM_E_UNINIT)

    /* Get the CanSM network Index value corresponding to the ComM channel Id received */
    network = CanSM_GetHandle(network);

    /* Report DET : Development Error for Invalid  network Handle and returns E_NOT_OK */
    CANSM_REPORT_ERROR_NOK(((uint8)network >= (CanSM_ConfigSet_pcst->CanSM_SizeOfCanSMNetworks_u8)),
                                                    (uint8)CANSM_GETCURRENTCOMMODE_SID,
                                                    (uint8)CANSM_E_INVALID_NETWORK_HANDLE)

    /* Report DET : Development Error Handling for CANSM un-initialized Pointer access and returns E_NOT_OK*/
    CANSM_REPORT_ERROR_NOK((ComM_ModePtr == NULL_PTR),
                           (uint8)CANSM_GETCURRENTCOMMODE_SID,
                           (uint8)CANSM_E_PARAM_POINTER)

    /*Make local copy of CanSM_network_Init_ab[network]*/
    CanSM_NwInitStatus_b =  CanSM_Network_Init_ab[network];

    /*Checkif the module is initialised and network is initialised*/
    if((CanSM_Init_ab == (boolean)CANSM_INITED) && (CanSM_NwInitStatus_b != FALSE))
    {
         /*Update the current network Mode status onto ComM_ModePtr*/
         *ComM_ModePtr = CanSM_BusSMMode_au8[network];

         /* return E_OK*/
         stFuncVal = E_OK;
    }

    return(stFuncVal);
}

#define CANSM_STOP_SEC_CODE
#include "CanSM_MemMap.h"
