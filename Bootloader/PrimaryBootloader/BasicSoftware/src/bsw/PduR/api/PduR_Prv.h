

#ifndef PDUR_PRV_H
#define PDUR_PRV_H

#include "PduR.h"
#include "PduR_Cfg_Internal.h"
#include "PduR_Cfg_SchM.h"

#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
#include "Det.h"   /* For Det_ReportError() */
#if(PDUR_ENABLE_INTER_MODULE_CHECKS)
/* Inter Modular Check */
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #if(PDUR_ENABLE_INTER_MODULE_CHECKS) */
#endif

#include "PduR_Types.h"

#if defined(PDUR_ZERO_COST_OPERATION) && (PDUR_ZERO_COST_OPERATION != STD_OFF)
#include "PduR_ZeroCost_Cfg.h"
#endif

/* ------------------------------------------------------------------- */
/**
 * @ingroup PDUR_PRV_H
 *
 * Checking for PDUR init state
 * */
#define  PDUR_CHECK_STATE_VOID(sid)                     \
   if ( PduR_State == PDUR_UNINIT )                     \
   {                                                    \
      PDUR_REPORT_ERROR((sid), PDUR_E_INVALID_REQUEST); \
      return;                                           \
   }

/**
 * @ingroup PDUR_PRV_H
 *
 * Returns the state and if the PDU Router has not been initialized (PDUR_UNINIT state) all services except PduR_Init() will
 * report the error PDUR_E_INVALID_REQUEST through the Development Error Tracer (DET) when called
 */

#define  PDUR_CHECK_STATE_RET(sid,ret)                  \
   if ( PduR_State == PDUR_UNINIT )                     \
   {                                                    \
      PDUR_REPORT_ERROR((sid), PDUR_E_INVALID_REQUEST); \
      return (ret);                                       \
   }

/**
 * @ingroup PDUR_PRV_H
 *
 * Checks for invalid pointer.If the pointer is invalid, PDUR_E_NULL_POINTER is reported via DET
 */
#define  PDUR_CHECK_PTR_RET(sid, ptr, ret)                      \
   if ( (ptr) == NULL_PTR )                                                \
   {                                                            \
      PDUR_REPORT_ERROR((sid), PDUR_E_NULL_POINTER);        \
      return (ret);                                               \
   }

/**
 * @ingroup PDUR_PRV_H
 *
 * checking for NULL or void pointer
 * */

#define  PDUR_CHECK_PTR_VOID(sid, ptr)                          \
   if ( (ptr) == NULL_PTR)                                                \
   {                                                            \
      PDUR_REPORT_ERROR((sid), PDUR_E_NULL_POINTER);        \
      return;                                                   \
   }


#if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
/**
 * @ingroup PDUR_PRV_H
 *
 * Instance id is added - Autosar Release2.1
 */
#define PDUR_REPORT_ERROR(serviceId, errorId) (void)Det_ReportError(PDUR_MODULE_ID, PDUR_INSTANCE_ID /*InstanceID*/ , serviceId, errorId)  
#else
/**
 * @ingroup PDUR_PRV_H
 *
 * To remove MISRA warning
 */
#define PDUR_REPORT_ERROR(serviceId, errorId) do {} while(0) /* To remove MISRA warning */
#endif

/* ------------------------------------------------------------------------ */
/* Begin a set of variable definitions */


#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"

extern PduR_DsmReInitType PduR_DsmReInitStruct;

#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"


/**
 * @ingroup PDUR_H
 *
 * PduR_Base: Access to configuration  \n
 *
 */
/* ------------------------------------------------------------------------ */
/* Begin a set of variable definitions */

#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"

extern const PduR_PBConfigType * PduR_Base;

/* ------------------------------------------------------------------------ */
/* End section for variable */

#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"

/*
#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"

extern const PduR_GwTp_SessionListType * PduR_GwTp_SessionListPtr;

#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
*/

#if(PDUR_CONFIGURATION_VARIANT == PDUR_VARIANT_PRE_COMPILE)
#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"

extern const PduR_PBConfigType PduR_GlobalPBConfig;

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "PduR_MemMap.h"
#endif

/**
 * @ingroup PDUR_H
 *
 * This Macro is used to point to that particular structure which is generated in PduR_PBConfigType structure
 */
#define PDUR_BASE(s)          (PduR_Base->s)
/**
 * @ingroup PDUR_H
 *
 *
 * This Macro is used to point to that any Lower Transport protocol structure which is generated in PduR_PBConfigType structure
 */
#define PDUR_BASE_LOTP(n,s)       (PduR_Base->LoTpConf[n].s)
/**
 * @ingroup PDUR_H
 *
 * This Macro is used to point to that any Lower Communication Interface which is direct as data provision
 *  structure which is generated in PduR_PBConfigType structure
 */
#define PDUR_BASE_LOIFD(n,s)      (PduR_Base->LoIfDConf[n].s)
/**
 * @ingroup PDUR_H
 *
 * This Macro is used to point to that any Lower Communication Interface which is trigger transmit as data provision
 *  structure which is generated in PduR_PBConfigType structure
 */
#define PDUR_BASE_LOIFTT(n,s)     (PduR_Base->LoIfTTConf[n].s)
/**
 * @ingroup PDUR_H
 *
 * PduR_DcmTransmit: Access to dcmToLo configuration  \n
 */
#define PDUR_BASE_UPTP(n,s)        (PduR_Base->UpTpConf[n].s)       /* PduR_DcmTransmit */

/**
 * @ingroup PDUR_H
 *
 * PduR_<UpIfModule>Transmit: Access to <UpIf>ToLo configuration  \n
 */
#define PDUR_BASE_UPIF(n,s)        (PduR_Base->UpIfConf[n].s)       /* PduR_<UpIfModule>Transmit */

/**
 * @ingroup PDUR_H
 *
 * PduR_<UpTp>CancelReceive: Access to <UpTp>ToLoRx configuration  \n
 */
#define PDUR_BASE_UPTPCANCELRX(n,s)        (PduR_Base->PduR_UpTpToLoTpRxCfg[n].s)       /* PduR_<UpTp>CancelReceive */

/**
 * @ingroup PDUR_H
 *
 * PduR_GF_IFTx: Access to gwToLoMc configuration  \n
 */
#define PDUR_GW_MC_TX_BASE      PDUR_BASE(gwToLoMc)      /* Gateway multicast (If) */

/**
 * @ingroup PDUR_H
 *
 * Gateway multicast If: Access to Gateway multicast If(loIfTxToUpMc) configuration  \n
 */
#define PDUR_LOIF_TX_MC_BASE    PDUR_BASE(loIfTxToUpMc)  /* Gateway multicast If */
/**
 * @ingroup PDUR_H
 *
 * PduR_<Lo>TpCopyTxData, PduR_<Lo>TpTxConfirmation: multicast table \n
 */
#define PDUR_LOTP_TX_MC_BASE    PDUR_BASE(loTpTxToUpMc)  /* PduR_<Lo>TpCopyTxData, PduR_<Lo>TpTxConfirmation: multicast table */
/**
 * @ingroup PDUR_H
 *
 * PduR_<Lo>TpCopyTxData, PduR_<Lo>TpTxConfirmation: multicast state\n
 */
#define PDUR_LOTP_TX_STATE_BASE PDUR_BASE(loTpTxToUpMs)  /* PduR_<Lo>TpCopyTxData, PduR_<Lo>TpTxConfirmation: multicast state */
/**
 * @ingroup PDUR_H
 *
 * PduR_GF_If_RxIndication: Access to Gateway Interface Rx Indication configuration  \n
 */
#define PDUR_GW_IF_TX_BASE      PDUR_BASE(gwIfTx)        /* PduR_GF_If_RxIndication */
/**
 * @ingroup PDUR_H
 *
 * IF Gateway transmission support\n
 */
#define PDUR_GW_IF_BASE         PDUR_BASE(gwIf)          /* IF Gateway transmission support */
/**
 * @ingroup PDUR_H
 *
 * TP Gateway transmission support\n
 */
#define PDUR_GW_TP_BASE         PDUR_BASE(gwTp)          /* TP Gateway transmission support */

/**
 * @ingroup PDUR_H
 *
 * RX TP Reception support\n
 */
#define PDUR_RX_TP_RPG_BASE         PDUR_BASE(rpgRxTp)          /* Rx TP Reception support */

#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
/**
 * @ingroup PDUR_H
 *
 * Routing Path Group support\n
 */
#define PDUR_RPG_CONFIG_INFO    PDUR_BASE(rpg_ConfigInfo)       /* RPG Support */

/**
 * @ingroup PDUR_H
 *
 * Enable\Disable Routing Path depending on RPG config\n
 */
#define PDUR_RPG_ENROUTING_INFO(index)  PduR_Base->pduR_Rpg_Status[index]    /* RPG Support */
#define PDUR_RPGID_MAP_TAB(index)  PduR_Base->rpgId_MappingTab[index]
#endif /*#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)*/

#define PDUR_INVALID_RPG_ID                       0xFFFFu
/**
 * @ingroup PDUR_H
 *
 * Valid No. Of Entries\n
 */
#define PDUR_NR_VALID_IDS(rt)   PDUR_BASE(rt##_NrEntries)

/**
 * @ingroup PDUR_H
 *
 * Access macro to get max RPG Id when pre-compile variant is selected
 */
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
#define PDUR_GET_RPGID_MAX     (PDUR_RPGID_MAX)
#endif

/**
 * @ingroup PDUR_PRV_H
 *
 * FIFO routines
 */
#define PDUR_FIFO_WRITE_INC_WRAP(fifo, val)                         \
   do {                                                           \
      ((val)->writePtr) = ((val)->writePtr) + (fifo)->eltSize;       \
      (val)->writeIndex += 1;                                              \
      if ((val)->writePtr >= ((fifo)->begin + ((fifo)->eltSize * (fifo)->nrElts))) {  \
         (val)->writePtr = (fifo)->begin;                                  \
          (val)->writeIndex = 0u;                                \
      }                                                           \
   } while (0)


#define PDUR_FIFO_READ_INC_WRAP(fifo, val)                         \
   do {                                                           \
      ((val)->readPtr) = ((val)->readPtr) + (fifo)->eltSize;        \
      (val)->readIndex += 1;                                              \
      if ((val)->readPtr >= ((fifo)->begin + ((fifo)->eltSize * (fifo)->nrElts))) {  \
         (val)->readPtr = (fifo)->begin;                                    \
          (val)->readIndex = 0u;                                \
      }                                                           \
   } while (0)

/* Section code start
*/
#define PDUR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"
/* ------------------------------------------------------------------------ */
/**
 * @ingroup PDUR_H
 *
 * State information \n
 */
extern PduR_StateType PduR_State;

/* ------------------------------------------------------------------------ */
/* End the set of variable definitions */

 /**
  * @ingroup PDUR_H
  *
  * Section code stop
  */
#define PDUR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "PduR_MemMap.h"

 /* ------------------------------------------------------------------------ */
 /* Error detection functions */


 #if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)    
  /**
   * @ingroup PDUR_H
   *
   * If any developemnt error is reported, and DevErrorDetect switch is set to off, the below macro points to NULL_PTR
   */
  #define PDUR_DET_API(name)                      (NULL_PTR)
 #else
  /**
   * @ingroup PDUR_H
   *
   *  If any developemnt error is reported, and DevErrorDetect switch is set to off, the below macro points to actual API
   */
  #define PDUR_DET_API(name)                      (name)
 #endif /* PDUR_DEV_ERROR_DETECT */

 /**
  * @ingroup PDUR_PRV_H
  *
  *  This function is to handle TxConfirmation for TP gatewaying, when a single entry buffer
  *  decrements the lock pointer counter for destination PDUs \n
  *
  *  @param  In:      layer - This parameter is used to indicate the PduR layer. \n
  *  @param  In:      gwId  - This parameter is used to indicate the gateway ID. \n
  *  @param  In:      result- This parameter is used to notify the result. \n
  *
  *  @return None \n
  */

 extern void PduR_GF_Tp_Buf_TxConfirmation( PduR_LayerType layer,
                                                               PduIdType gwId,
                                                               Std_ReturnType result );
 /**
  * @ingroup PDUR_PRV_H
  *
  *  PduR_GF_Tp_FIFO_TxConfirmation is used to handle TxConfirmation for TP gatewaying, when a FIFO is being used \n
  *
  *  @param  In:      layer - This parameter is used to indicate the PduR layer \n
  *  @param  In:      gwId  - This parameter is used to indicate the gateway ID \n
  *  @param  In:      result- This parameter is used to notify the result \n
  *
  *  @return None \n
  */
 extern void PduR_GF_Tp_FIFO_TxConfirmation( PduR_LayerType layer,
                                                                PduIdType gwId,
                                                                Std_ReturnType result);

 /*Extern declarations of internal RPG functions */

 /**
  * @ingroup PDUR_RPG_H
  *  **************************************************************************************************
  * PduR_RGP_Tp_StartOfReception - This function will be called by the transport protocol module at the start of receiving an I-PDU.
  *                                                The I-PDU might be fragmented into multiple N-PDUs (FF with one or more following CFs) or might consist of a single N-PDU (SF). The service shall provide the currently available maximum buffer size when invoked with TpSdulength equal to 0.
  *
  *
  * \param(in)       id                - Identification of the I-PDU.
  *                    info              - Pointer to a PduInfoType structure containing the payload data
  *                                           (without protocol information) and payload length of the first
  *                                           frame or single frame of a transport protocol I-PDU reception.
  *                                           Depending on the global parameter MetaDataLength, additional bytes
  *                                           containing MetaData (e.g. the CAN ID) are appended after the payload data.
  *                    TpSduLength          - Total length of the PDU to be received.
  *
  * \param(out)        bufferSizePtr      - Available receive buffer in the receiving module.
  *                                        This parameter will be used to compute the Block Size (BS)
  *                                        in the transport protocol module.
  *
  * \retval          BUFREQ_OK           -  Connection has been accepted. bufferSizePtr indicates the available receive buffer;
  *                                         reception is continued. If no buffer of the requested size is available,
  *                                         a receive buffer size of 0 shall be indicated by bufferSizePtr.
  *                    BUFREQ_E_NOT_OK   -  Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
  *                    BUFREQ_E_OVFL     -  No buffer of the required length can be provided; reception is aborted.
  *                                         bufferSizePtr remains unchanged.
  *
  * \seealso
  * \usedresources
  **************************************************************************************************
  */
 extern BufReq_ReturnType PduR_RPG_Tp_StartOfReception_Func ( PduIdType rpgId,
                                                                         const PduInfoType * info,
                                                                         PduLengthType TpSduLength,
                                                                         PduLengthType * bufferSizePtr);

 /**
  * @ingroup PDUR_RPG_H
  *  **************************************************************************************************
  * PduR_RPG_Tp_CopyRxData - This function is called when transport protocol module have data to copy to
  *                           the receiving module. Several calls may be made during one transportation of an I-PDU.
  *
  * \param(in)       PduIdType    gwId             - Identification of the received I-PDU.
  *                  pduInfoPtr   Info             - Pointer to the buffer (SduDataPtr) and its length (SduLength) containing the data
  *                                                      to be copied by PDU Router module in case of gateway or upper
  *                                                      layer module in case of reception.
  * \param(out)      PduLengthType* bufferSizePtr  - Available receive buffer after data has been copied.
  *
  * \retval          BUFREQ_OK: Buffer request accomplished successful.
  *                    BUFREQ_E_NOT_OK: Buffer request not successful. Buffer cannot be accessed.
  *                    BUFREQ_E_OVFL: No Buffer of the required length can be provided.
  * \seealso
  * \usedresources
  **************************************************************************************************
  */

 extern BufReq_ReturnType PduR_RPG_Tp_CopyRxData_Func ( PduIdType rpgId,
                                                                 const PduInfoType * info,
                                                                 PduLengthType * bufferSizePtr);

 /**
  * @ingroup PDUR_RPG_H
  **************************************************************************************************
  * PduR_RPG_Tp_RxIndication - Called by the transport protocol module after an I-PDU has been received successfully
  *                              or when an error occurred. It is also used to confirm cancellation of an I-PDU.
  *
  * \param(in)       PduIdType    Id                 - Identification of the received I-PDU.
  *                  Std_ReturnType result        - Result of the reception.
  *
  * \param(out)      None
  *
  * \retval            None
  * \seealso
  * \usedresources
  **************************************************************************************************

  */

 extern void PduR_RPG_Tp_RxIndication_Func ( PduIdType rpgId,
                                                        Std_ReturnType result);

 /* ------------------------------------------------------------------------ */
 /* PduR_Init */

  /**
   * @ingroup PDUR_H
   * This API initializes the PDU Router.\n
   *
   * @param in         ConfigPtr :    Pointer to post build configuration.\n
   *
   * @return None \n
   */
 extern void PduR_iInit( const PduR_ConfigType * ConfigPtr );

 #if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)
 /**
  * @ingroup PDUR_H
  * This API initializes the PDU Router.\n
  *
  * @param in       ConfigPtr :   Pointer to post build configuration.\n
  *
  * @return None \n
  */
 extern void PduR_dInit( const PduR_ConfigType * ConfigPtr );

 /* ------------------------------------------------------------------------ */
 /**
  * @ingroup PDUR_H
  *
  * PduR_GetConfigurationId
  * */
 extern PduR_PBConfigIdType PduR_dGetConfigurationId( void );
#endif/*if defined(PDUR_DEV_ERROR_DETECT) && (PDUR_DEV_ERROR_DETECT != STD_OFF)*/

 extern PduR_PBConfigIdType PduR_iGetConfigurationId( void );

 /* ------------------------------------------------------------------------ */
 /**
  * @ingroup PDUR_H
  *      Null function to handle unwanted PduR_xxxIfTriggerTransmit
  *
  * @param in           Id      : ID of N-PDU that shall be received
  * @param in           data    : Data pointer for transmit buffer
  *
  * @return None \n
  */
 extern Std_ReturnType PduR_NF_TriggerTransmit_Func( PduIdType id, PduInfoType * data);

 /* Invalid PDU id handlers ------------------------------------------ */

 /**
  * @ingroup PDUR_H
  *     This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in        Id              :     ID of L-PDU that has been received.\n
  * @param in        bufferSizePtr   :     length identifies the overall number of bytes to be received.\n
  * @param out       ptr             :     Pointer to pointer to PduInfoStructure containing SDU data pointer and SDU length of a receive buffer.\n
  */
 extern BufReq_ReturnType PduR_invId_TpCopyRxData( PduIdType id,
                                                                     const PduInfoType * info,
                                                                     PduLengthType * bufferSizePtr);
 /**
  * @ingroup PDUR_H
  *     This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in        Id              :   ID of L-PDU that has been received.\n
  * @param in        TpSduLength     :   length identifies the overall number of bytes to be received.\n
  * @param in        bufferSizePtr   :   Pointer to pointer to PduInfoStructure containing SDU data pointer and SDU length of a receive buffer.\n
  */
 extern BufReq_ReturnType PduR_invId_TpStartOfReception(PduIdType id,
                                                                     PduLengthType TpSduLength,
                                                                     PduLengthType * bufferSizePtr);
 /**
  * @ingroup PDUR_H
  *           This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in       Id      :    ID of L-PDU that has been received.\n
  * @param in       result  :     Result of the TP reception.\n
  *
  * @return None \n
  */
 extern void PduR_invId_TpRxIndication( PduIdType id,   \
                                                                        Std_ReturnType result);
 /**
  * @ingroup PDUR_H
  *      This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in        id          :ID of CAN N-PDU to be transmitted.\n
  *                  ptr         :Pointer to PduInfoStructure containing SDU data pointer and SDU length of a transmit buffer.\n
  *                  retry       :This parameter is used to retransmit data because problems occurred in transmitting it the last time.\n
  * @param out       availableDataPtr    :Indicates the remaining number of bytes that are available in the PduR Tx buffer.\n
  *
  */
 extern BufReq_ReturnType PduR_invId_TpCopyTxData( PduIdType id, \
                                                                     const PduInfoType * info, \
                                                                     RetryInfoType * retry, \
                                                                     PduLengthType * availableDataPtr );
 /**
  * @ingroup PDUR_H
  *      This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in        id            : ID of I-PDU to be transmitted.\n
  * @param in        result        : Result of the TP reception.\n
  *
  * @return None \n
  */
 extern void PduR_invId_TpTxConfirmation( PduIdType id,      \
                                                             Std_ReturnType result);
 /**
  * @ingroup PDUR_H
  *       This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in         Id   : ID of L-PDU that has been received.\n
  *                   ptr  : Pointer to L-SDU (buffer of received payload).\n
  *
  * @return None \n
  */
 extern void PduR_invId_IfRxIndication( PduIdType id,  \
                                                           const PduInfoType * ptr );
 /**
  * @ingroup PDUR_H
  *        This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in          id -  ID of FlexRay L-PDU that has been transmitted.
  *
  * @return None \n
  */
#if (AUTOSAR_VERSION_AR42 == STD_ON)
extern void PduR_invId_IfTxConfirmation( PduIdType id );
#else
extern void PduR_invId_IfTxConfirmation( PduIdType id,Std_ReturnType result);
#endif


 /**
  * @ingroup PDUR_H
  *    This functions to be invoked when an invalid ID is encountered.\n
  *
  *
  * @param in        id      :   ID of L-PDU that is requested to be transmitted.\n
  *                  data    :   Pointer to place inside the transmit buffer of the L-PDU where data shall be copied to.\n
  */
 extern Std_ReturnType PduR_invId_IfTriggerTransmit( PduIdType id,  \
                                                              PduInfoType * data );
 /**
  * @ingroup PDUR_H
  *      This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in         id     :   ID of I-PDU to be transmitted.\n
  *                   ptr    :   Pointer to pointer to PduInfoStructure containing SDU data pointer and SDU length of a transmit buffer.\n
  */
 extern Std_ReturnType PduR_invId_UpTransmit( PduIdType id, \
                                                                   const PduInfoType * ptr);
 /**
  * @ingroup PDUR_H
  *     This functions to be invoked when an invalid ID is encountered.\n
  *
  * @param in           id: ID of DCM I-PDU to be cancelled the transmission.\n
  */
 extern Std_ReturnType PduR_invId_UpCancelTransmit( PduIdType id);

 /**
 * @ingroup PDUR_H
 *     This functions to be invoked when an invalid ID is encountered.\n
 *
 * @param in           id: ID of UpTp I-PDU to be cancelled the reception.\n
 */
 extern Std_ReturnType PduR_invId_UpCancelReceive( PduIdType id);

 /**
  * @ingroup PDUR_H
  *   The following extern declarations irrespective of whether the tables exist in the PduR_Cfg.c file.
  */

 /* ------------------------------------------------------------------------ */
 /* Begin section for extern constant */

#define PDUR_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upIfRxIndicationTable[]
  * */
 extern const PduR_upIfRxIndicationFuncType PduR_upIfRxIndicationTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upIfTxConfirmationTable[]
  * */
 extern const PduR_upIfTxConfirmationFuncType PduR_upIfTxConfirmationTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upTpCopyRxDataTable[]
  * */
 extern const PduR_upTpCopyRxDataFuncType PduR_upTpCopyRxDataTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upTpStartOfReceptionTable[]
  * */
 extern const PduR_upTpStartOfReceptionFuncType PduR_upTpStartOfReceptionTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upTpRxIndicationTable[]
  * */
 extern const PduR_upTpRxIndicationFuncType PduR_upTpRxIndicationTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upTpCopyTxDataTable[]
  * */
 extern const PduR_upTpCopyTxDataFuncType PduR_upTpCopyTxDataTable[];
 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upTpTxConfirmationTable[]
  * */
 extern const PduR_upTpTxConfirmationFuncType PduR_upTpTxConfirmationTable[];

 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_upIfTriggerTxTable[]
  * */
 extern const PduR_upIfTriggerTxFuncType PduR_upIfTriggerTxTable[];

 /**
  * @ingroup PDUR_H
  * extern declaration of PduR_loTransmitTable[]
  * */
 extern const PduR_loTransmitFuncType PduR_loTransmitTable[];

  /**
  * @ingroup PDUR_H
  * extern declaration of PduR_loCancelTransmitTable[]
  * */
 extern const PduR_loCancelTransmitFuncType PduR_loCancelTransmitTable[];
 /**
 * @ingroup PDUR_H
 * extern declaration of PduR_loCancelRxTable[]
 * */
 extern const PduR_loCancelReceiveFuncType PduR_loCancelRxTable[];

 /* ------------------------------------------------------------------------ */
 /* End section for extern constant */

#define PDUR_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "PduR_MemMap.h"

/* Internal APIs & Interfaces required for COM module */
extern void PduRAppl_IncompatibleGenerator(void);

/* Flags */

/* These flags share a byte with the count of the number of lower layers which we are gatewaying
 * to, therefore they should have large values. */
/**
 * @ingroup PDUR_PRV_H
 *
 * This flag share a byte with the count of the number of lower layers which we are gatewaying
 * to, therefore they should have large values. This is used for the need of Tx \n
 */
#define PDUR_GW_TP_NEED_TX    ((uint8)0x40)
/**
 * @ingroup PDUR_PRV_H
 *
 * This flag share a byte with the count of the number of lower layers which we are gatewaying
 * to, therefore they should have large values. This is used when transmit is called \n
 */
#define PDUR_GW_TP_TX_CALLED  ((uint8)0x20)
/**
 * @ingroup PDUR_PRV_H
 *
 * This flag share a byte with the count of the number of lower layers which we are gatewaying
 * to, therefore they should have large values. This is used when transmit is failed \n
 */
#define PDUR_GW_TP_TX_FAILED  ((uint8)0x80)

#endif /* PDUR_PRIV_H */

