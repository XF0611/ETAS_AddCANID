
#include "PduR_Prv.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */


/**
 **************************************************************************************************
 * PduR_DisableRouting : Control function to disable routing of PDUs from one src to one or more
 *                       than one dest layers.
 *
 * \param           PduR_RoutingPathGroupIdType id: RoutingPathGroupId
 * \param           initialize - Flag to initialize the I-PDU buffer to default values.\n
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"


void PduR_DisableRouting( PduR_RoutingPathGroupIdType id,
        boolean initialize)
{
/* checking for mode dependent routing is enabled or not during Pre-compile time */
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
    /* Checking for Routing path id */
    if ((id != 0) && (id <= PDUR_GET_RPGID_MAX) && (PDUR_RPGID_MAP_TAB(id) != PDUR_INVALID_RPG_ID))
    {
        /* Enter Critical Section */
        SchM_Enter_PduR_LockDisableRoutingNoNest();

        if((PDUR_RPG_ENROUTING_INFO(PDUR_RPGID_MAP_TAB(id))) == TRUE)
        {
            /* Disable routing for this RPG */
            (PDUR_RPG_ENROUTING_INFO(PDUR_RPGID_MAP_TAB(id))) = FALSE;
            /* Checking for data*/
            if (PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].targetInfo != NULL_PTR)
            {
                PduR_FifoDynIfPtr       dyn_If;
                PduR_TpSessDynPtr       dyn_Tp;
                PduR_BuffLockType * buffPtr;
                const PduR_TargetInfoType * targetInfo;
                uint8 cntDest;
                uint8 index;

                targetInfo = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].targetInfo;
                cntDest = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].cntDestOfTypeFIFO;

                for(index=0;index < cntDest;index++)
                {

                    switch(targetInfo->pathType)
                    {
                        case PDUR_FIFO_IF:
                        {
                                        uint8 indexArr_8;
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        dyn_If = (PduR_FifoDynIfPtr)targetInfo->destInfo;
                                        dyn_If->used           = 0;
                                        dyn_If->txConfPending  = 0;            /* Reset txConfPending to ignore TtxConfirmation. */
                                        dyn_If->writePtr       = dyn_If->readPtr; /* Remove all elements from the FIFO */
                                        for(indexArr_8 = 0; indexArr_8 < PDUR_FIFO_IF_MAXSIZE; indexArr_8++)
                                        {
                                            dyn_If->actLen[indexArr_8] = 0;
                                        }

                            break;
                        }
                        case PDUR_UCSEB_TP:
                                        /* Buffer Lock needs to be locked */
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        buffPtr = (PduR_BuffLockType * )targetInfo->destInfo;
                                        buffPtr->bufferLock = 0;
                            break;

                        case PDUR_MCSEB_TP:
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        buffPtr = (PduR_BuffLockType * )targetInfo->destInfo;
                                        /* Decrement the lower Layer */
                                        buffPtr->lowerLayers = buffPtr->lowerLayers - 1;

                            break;

                        case PDUR_UCFIFO_TP:
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        dyn_Tp = (PduR_TpSessDynPtr)targetInfo->destInfo;
                                        /*Reset the FIFO TP buffer*/
                                        dyn_Tp->writePtr =  dyn_Tp->readPtr;
                                        dyn_Tp->numRxLength_auo[0] = 0;
                                        dyn_Tp->numTxLength = 0;
                                        dyn_Tp->SduLength_auo[0] = 0;
                            break;

                        default:
                               {
                                     /*This is to suppress misra warning*/
                               }
                            break;
                    }
                    targetInfo++;

                }

            }/*End of if (PDUR_RPG_CONFIG_INFO[id].targetInfo != NULL_PTR)*/
#if defined(PDUR_DEFAULT_VALUE_SUPPORT) && (PDUR_DEFAULT_VALUE_SUPPORT != 0)
            else if((initialize == TRUE) && (PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].defaultValue_info != NULL_PTR) )
            {
                const PduR_DefaultValue_Info * defaultValueInfo;
                const PduR_DefaultValues_fixed * destDfaltVals;
                uint8 * buffAddr;
                uint8 dfltValDestCnt;
                uint8 counter;
                uint8 index;
                uint8 bytePosition;

                defaultValueInfo = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].defaultValue_info;
                destDfaltVals = defaultValueInfo->defaultValueElement;
                dfltValDestCnt = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].cntOfDestWithDefValue;
                buffAddr = defaultValueInfo->begin;
                counter = 0;

                while(counter < dfltValDestCnt)
                {

                    for(index = 0; index < defaultValueInfo->defaultValueSize; index++)
                    {
                        /*Get the byte position*/
                        bytePosition = destDfaltVals[index].bytePosition;
                        buffAddr[bytePosition] = destDfaltVals[index].byteDefaultValue;
                    }
                    /*Point to next destination pdu's default value structure.*/
                    defaultValueInfo++;
                    /*Increment the destination counter value*/
                    counter++;
                }


            }
            /*To Remove Misra Warning*/
            else
			{
                /*do nothing*/
            }
#endif /*#if defined(PDUR_DEFAULT_VALUE_SUPPORT) && (PDUR_DEFAULT_VALUE_SUPPORT != 0)*/
        }/*End of if((PDUR_RPG_ENROUTING_INFO)[(PDUR_RPG_CONFIG_INFO[id].rpg_Idx)] == TRUE)*/
        SchM_Exit_PduR_LockDisableRoutingNoNest();
        /* Exit Critical section */
    }
    else
    {
        /* Routing path table id does not exist and the PduRDevErrorDetect is enabled */
        PDUR_REPORT_ERROR(PDUR_SID_DISABLEROUTING, PDUR_E_ROUTING_TABLE_ID_INVALID);
    }

   #else
        (void)id;
        (void)initialize;
   #endif
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"




/**
 **************************************************************************************************
 * PduR_EnableRouting : Control function to enable routing of PDUs from one src to one or more than
 *                      one dest layers.
 *
 * \param           PduR_RoutingPathGroupIdType id: RoutingPathGroupId
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */


#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

void PduR_EnableRouting( PduR_RoutingPathGroupIdType id)
{
/* check for mode dependent routing is enabled or not during Pre-compile time */
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)

     /* Checking for Routing path id */
     if ((id != 0) && (id <= PDUR_GET_RPGID_MAX))
     {
         /* Enter Critical Section */
         SchM_Enter_PduR_LockEnableRoutingNoNest();

         /* Enable routing for this RPG */
         if ((PDUR_RPG_ENROUTING_INFO(PDUR_RPGID_MAP_TAB(id))) == FALSE)
         {
             (PDUR_RPG_ENROUTING_INFO(PDUR_RPGID_MAP_TAB(id))) = TRUE;

            /* Decrement the lower layers if the Routing path is Multicast SEB */
            if (PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].targetInfo != NULL_PTR)
            {
                PduR_TpSessDynPtr       dyn_Tp;
                PduR_BuffLockType * buffPtr;
                const PduR_TargetInfoType     *targetInfo;
                uint8 cntDest;
                uint8 index;

                targetInfo = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].targetInfo;
                cntDest = PDUR_RPG_CONFIG_INFO[PDUR_RPGID_MAP_TAB(id)].cntDestOfTypeFIFO;
                for(index=0;index < cntDest;index++)
                {
                    switch(targetInfo->pathType)
                    {
                            case PDUR_MCSEB_TP:
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        buffPtr = (PduR_BuffLockType*)targetInfo->destInfo;
                                        /* Increment the lower Layer */
                                        buffPtr->lowerLayers = buffPtr->lowerLayers + 1;

                                 break;
                            case PDUR_UCSEB_TP:
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        buffPtr = (PduR_BuffLockType*)targetInfo->destInfo;
                                        /* Increment the lower Layer */
                                        buffPtr->lowerLayers = 1;
                                        buffPtr->bufferLock = 0;

                                 break;
                            case PDUR_UCFIFO_TP:
                                        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.
                                         * Hence Supressed.*/
                                        dyn_Tp = (PduR_TpSessDynPtr)targetInfo->destInfo;
                                        /*Reset the FIFO TP buffer*/
                                        dyn_Tp->writePtr =  dyn_Tp->readPtr;
                                        dyn_Tp->numRxLength_auo[0] = 0;
                                        dyn_Tp->numTxLength = 0;
                                        dyn_Tp->SduLength_auo[0] = 0;
                                 break;
                            default:
                                   {
                                       /*This is to suppress misra warning*/
                                   }
                                 break;

                    }

                    targetInfo++;

                }

            }
         }


         SchM_Exit_PduR_LockEnableRoutingNoNest();
         /* Exit Critical Section */
     }
     else
     {
        /* Routing path group id does not exist and the PduRDevErrorDetect is enabled */
        PDUR_REPORT_ERROR(PDUR_SID_ENABLEROUTING, PDUR_E_ROUTING_TABLE_ID_INVALID);
     }

   #else
      (void)id;
   #endif
}
/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"

