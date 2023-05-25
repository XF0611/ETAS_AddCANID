/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
 



#ifndef DCMAPPL_H
#define DCMAPPL_H

/*TESTCODE-START
#include "DcmTest.h"
TESTCODE-END*/


/*************************************************************************************************/
/* Included header files                                                                         */
/*************************************************************************************************/

/*
 *************************************************************************************************
 *    definitions and Typedefs                                                                   */
/*************************************************************************************************
 */
/*
 * Standard negative response (0x11,0x12 & 0x31)are suppressed during a functional request.
 * This part is implemented in the function Dcm_ProcessingDone().
 * Some customers may need the suppression of additional NRCs. These additional NRCs can be added in the below MACRO.
 * Please refer the example given below to configure additional NRCs to suppress during functional request.
 * NOTE : If 0x78 response is sent first and the NRC generated later is either 0x11, 0x12, 0x31 or one of the NRC
 * configured below, then NRC will not be suppressed.(ISO requirement)
 * As per the ISO requirement When this response code is used, the server shall always send a final response
 * (positive or negative) independently of the suppressPosRspMsgIndicationBit value or the supress requirement for
 * responses with NRCs SNS, SRNS and ROOR on functionally addressed requests.
 * If there is no additional NRC to suppress, the MACRO should be expanded to (0)
 */
/* Ex :
 * #define DCMAPPL_SUPPRESS_NRC(NegRespCode) (((NegRespCode) == NRC_1) || ((NegRespCode) == NRC_2))
*/
#define DCMAPPL_SUPPRESS_NRC(NegRespCode) (0u)


/* User can enter the address definitions, or include the file containing the address definitions, if the address range is entered as a string in
DcmDspWriteMemoryRangeHigh
DcmDspWriteMemoryRangeLow
DcmDspReadMemoryRangeHigh
DcmDspReadMemoryRangeLow */

/* BSW-17211 */
/* Add missing definition when using the UDS services that do not require Dem*/
#ifndef DEM_WRONG_DTC
#define DEM_WRONG_DTC 8
#endif

#ifndef DEM_WRONG_DTCORIGIN
#define DEM_WRONG_DTCORIGIN 9
#endif

#ifndef DEM_CLEAR_MEMORY_ERROR
#define DEM_CLEAR_MEMORY_ERROR 6
#endif
/* END BSW-17211 */

/****************************************************************************************************
*        APPLICATION API DECLARATIONS
****************************************************************************************************/
#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/*TESTCODE-START
#if((DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF))
#define DCM_STORE_DEFINITION_BY_ID                (0x01)
#define DCM_STORE_DEFINITION_BY_MEMORYADDRESS     (0x02)
#endif
TESTCODE-END*/
#if (DCM_CFG_SUPPLIER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmNotification (
                                                     uint8 SID,
                                                     const uint8 * const RequestData,
                                                     PduLengthType RequestLength,
                                                     uint8 RequestType,
                                                     PduIdType DcmRxPduId,
                                                     uint16 SourceAddress,
                                                     Dcm_NegativeResponseCodeType * ErrorCode
                                                     );
#endif

#if (DCM_CFG_MANUFACTURER_NOTIFICATION_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_ManufacturerNotification (
                                               uint8 SID,
                                               const uint8 * const RequestData,
                                               PduLengthType RequestLength,
                                               uint8 RequestType,
                                               PduIdType DcmRxPduId,
                                               uint16 SourceAddress,
                                               Dcm_MsgContextType * adrMsgContext_pst,
                                               Dcm_NegativeResponseCodeType * ErrorCode
                                                                         );
#endif

extern void DcmAppl_DcmGetNRCForMinLengthCheck (
		                                       Dcm_ProtocolType dataProtocolId_u8,
		                                       uint8 dataSid_u8,
                                               Dcm_NegativeResponseCodeType * dataErrorCode_u8
                                                                   );

extern void DcmAppl_DcmConfirmation
(
    Dcm_IdContextType idContext,
    PduIdType dcmRxPduId,
    uint16 SourceAddress,
    Dcm_ConfirmationStatusType status
);

extern void DcmAppl_DcmSesCtrlChangeIndication
(
    Dcm_SesCtrlType SesCtrlTypeOld,
    Dcm_SesCtrlType SesCtrlTypeNew
);

extern Std_ReturnType DcmAppl_DcmStartProtocol(Dcm_ProtocolType ProtocolID);
extern Std_ReturnType DcmAppl_DcmStopProtocol(Dcm_ProtocolType ProtocolID);

extern void DcmAppl_ConfirmationRespPend(
                                                        Dcm_ConfirmationStatusType status
                                                       );



#if(DCM_PAGEDBUFFER_ENABLED != DCM_CFG_OFF)
extern void DcmAppl_DcmCancelPagedBufferProcessing(Dcm_IdContextType idContext);
#endif

#if((DCM_CFG_KWP_ENABLED != DCM_CFG_OFF))
extern void DcmAppl_P3TimeoutIndication(void);
#if(DCM_CFG_SPLITRESPSUPPORTEDFORKWP != DCM_CFG_OFF)
extern void DcmAppl_DcmGetRemainingResponseLength(
                                        uint8 dataSID_u8,
                                        PduLengthType * dataResponseLength
                                        );
#endif
#endif

#if(DCM_CFG_RDPI_ENABLED != DCM_CFG_OFF)
extern void DcmAppl_DcmConfirmationRDPI
(
    Dcm_IdContextType idContext,
    PduIdType dcmRxPduId,
    Dcm_ConfirmationStatusType status
);
#endif

#if(DCM_CFG_NR_CONF_ENABLED != DCM_CFG_OFF)
void DcmAppl_DcmConfirmation_DcmNegResp(
                                                        Dcm_IdContextType idContext,
                                                        PduIdType dcmRxPduId,
                                                        Dcm_ConfirmationStatusType status
                                                      );
#endif

void DcmAppl_DcmConfirmation_GeneralReject (
                                                            Dcm_IdContextType idContext,
                                                            PduIdType dcmRxPduId,
                                                            Dcm_ConfirmationStatusType status
                                                               );



extern void DcmAppl_DcmIndicationFuncTpr(void);
extern void DcmAppl_DcmUpdateRxTable (void);

/* Application function to check if the new request can be accepted.
   for particular project variant*/
extern Std_ReturnType DcmAppl_DcmGetRxPermission (Dcm_ProtocolType ProtocolId,
		PduIdType DcmRxPduId,
		const PduInfoType * info,
		PduLengthType TpSduLength);

extern void DcmAppl_DcmComModeError (uint8 NetworkId);
/* FC_VariationPoint_START */
#if((DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE4_ENABLED != DCM_CFG_OFF))
extern Std_ReturnType DcmAppl_OBD_Mode04(void);
#endif
/* FC_VariationPoint_END */
#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF))
extern Std_ReturnType DcmAppl_DDDI_Read_Memory_Condition (
    const Dcm_DDDI_DEF_MEM_t * record,
    Dcm_NegativeResponseCodeType * NegRespCode);

#if(DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmWriteOrClearDddiInNvM (const Dcm_DddiMainConfig_tst * dataDefinitionOfDDDI,
                                                                            uint16 dataDddId_u16,
                                                                            Dcm_OpStatusType Dcm_StoreDDDiOpstatus_u8,
                                                                            Dcm_DddiWriteOrClear_ten Dcm_DddiWriteOrClear_en);

extern Dcm_RestoreDddiReturn_ten DcmAppl_DcmReadDddiFromNvM (const Dcm_DddiMainConfig_tst * dataDefinitionOfDDDI,
                                                                                 uint16 dataDddId_u16);
#endif

#endif
#if (DCM_CFG_DSP_IOCBI_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmCheckControlMaskAndState(uint16 nrDID_u16, uint8 dataIoParam_u8,
													const uint8 * adrCtlStateAndMask_pcst,uint16 dataCtlStateAndMaskLen_u16
													);
#endif

#if ((DCM_CFG_DSP_WRITEMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_READMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)\
		|| (DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF))
extern Std_ReturnType DcmAppl_DcmGetPermissionForMemoryAccess_u8(uint32 adrMemoryAddress_u32,
																		  uint32 dataDataLength_u32,
																		  Dcm_Direction_t dataDirection_en);
#endif

#if ((DCM_CFG_DSP_READMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) 			||	\
	(DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)    ||  \
	(DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)           || \
	(DCM_CFG_DSP_TRANSFERDATA_ENABLED!= DCM_CFG_OFF))
extern Dcm_ReturnReadMemoryType DcmAppl_Dcm_ReadMemory(Dcm_OpStatusType Rmba_Opstatus,
																	  uint8 memoryid,
																	  uint32 memoryaddress,
																	  uint32 datalength,
																	  uint8 * respbuf,
																	  Dcm_NegativeResponseCodeType * ErrorCode);
#endif

#if (DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmCheckWDBIReqLen(uint16 nrDID_u16, uint32 dataReqLen_u32);
#endif

#if((DCM_CFG_DSP_WRITEMEMORYBYADDRESS_ENABLED != DCM_CFG_OFF) || (DCM_CFG_DSP_TRANSFERDATA_ENABLED!= DCM_CFG_OFF))
extern Dcm_ReturnWriteMemoryType DcmAppl_Dcm_WriteMemory(Dcm_OpStatusType Wmba_Opstatus,
																		uint8 memoryid,
																		uint32 memoryaddress,
																		uint32 datalength,
																		const uint8 * reqbuf,
																		Dcm_NegativeResponseCodeType * ErrorCode);
#endif



#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CONTROLDTCSETTING_ENABLED != DCM_CFG_OFF) )
extern void DcmAppl_DisableDTCSetting (
                                                    Dcm_NegativeResponseCodeType * ErrorCode
                                                          );
extern void DcmAppl_EnableDTCSetting(
													Std_ReturnType DtcEnableStatus
														);
extern void DcmAppl_DcmControlDtcSettingEnableStatus(
		boolean ControlDtcSettingEnableStatus
		);

extern Std_ReturnType DcmAppl_DcmCheckCDTCRecord (
                                    const uint8 * ControlOptionRecord,
                                    uint8 Length,
                                    Dcm_NegativeResponseCodeType * ErrorCode
                                                                       );
#endif



 #define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"

/******************************************************************************************************/
 #define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && ( DCM_CFG_DSP_COMMUNICATIONCONTROL_ENABLED != DCM_CFG_OFF) )
extern void DcmAppl_DcmSwitchCommunicationControl( uint8 NetworkID ,Dcm_CommunicationModeType RequestedMode);
extern Std_ReturnType DcmAppl_DcmCommControlConditionCheck(uint8 ControlType_u8,
                                                                                uint8 DataCommType_u8,
                                                                                const uint8 * RequestData,
                                                                                uint16 RequestLength,
                                                                                Dcm_NegativeResponseCodeType * dataNegRespCode_u8 );
#if(DCM_CFG_RTESUPPORT_ENABLED == DCM_CFG_OFF)
extern boolean DcmAppl_DcmIsComModeDefault( uint8 NetworkID );
#endif
#endif



#if(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmGetPermTxWarmResp(void);
extern Dcm_EcuStartModeType Dcm_GetProgConditions
                                   (Dcm_ProgConditionsType * ProgConditions );
#endif

#if(DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)
extern void DcmAppl_DcmRespWarmInit(void);
extern uint8 DcmAppl_DcmGetStoreType(uint8 dataBootType_u8,uint8 SID);



extern Std_ReturnType Dcm_SetProgConditions(Dcm_OpStatusType OpStatus,
                                                                    const Dcm_ProgConditionsType * ProgConditions);
extern void DcmAppl_DcmStoreRespForJump
                                   (Dcm_ProgConditionsType * ProgConditions );
#endif

#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_CLEARDIAGNOSTICINFORMATION_ENABLED != DCM_CFG_OFF)&& (DCM_ROE_ENABLED != DCM_CFG_OFF))
extern void DcmAppl_DcmClearRoeEventInformation (void);
#endif

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && ( DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED != DCM_CFG_OFF ) )
extern Std_ReturnType DcmAppl_DcmGetSesChgPermission(
                                            Dcm_SesCtrlType SesCtrlTypeActive,
                                            Dcm_SesCtrlType SesCtrlTypeNew,
                                            Dcm_NegativeResponseCodeType * ErrorCode
                                                                        );

#endif

#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ECURESET_ENABLED != DCM_CFG_OFF) )

extern Std_ReturnType DcmAppl_DcmEcuResetPreparation(
                                                                uint8 ResetType,
                                                                Dcm_NegativeResponseCodeType * ErrorCode
                                                                );

#endif

/**
 ***************************************************************************************************
            Response On Event (ROE) service
 ***************************************************************************************************
 */
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_RESPONSEONEVENT_ENABLED != DCM_CFG_OFF))

#if (DCM_CFG_DSP_ROEDID_ENABLED!=DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_DcmStoreRoeDidInfo( uint16 Did,
                                                                      uint8 RoeeventID,
                                                                      Dcm_DspRoeEventState_ten RoeEventStatus,
                                                                      uint16 SourceAddress,
                                                                      boolean dspRoeStorageState_b,
                                                                      Dcm_OpStatusType OpStatus,
                                                                      boolean dspRoeSessionIsDefault_b,
                                                                      Dcm_NegativeResponseCodeType * ErrorCode);

extern Std_ReturnType DcmAppl_DcmGetROEDidInfo(uint16 Did,
                                                  uint8 RoeeventID,
                                                  Dcm_DspRoeEventState_ten * RoeEventStatus,
                                                  uint8 * RoeEventWindowTime_u8,
                                                  uint16 * SourceAddress,
                                                  boolean * dspRoeCtrlStorageState_b,
                                                  boolean * dspRoeSetUpStorageState_b,
                                                  boolean * dspRoeSessionIsDefault_b);
#endif
#if(DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF)

extern Std_ReturnType DcmAppl_DcmGetRoeDTCInfo(uint8 RoeeventID,
        Dcm_DspRoeEventState_ten * RoeEventStatus,
        uint8 * RoeEventWindowTime_u8,
        uint16 * SourceAddress,
        uint8 * StatusMask,
        boolean * dspRoeCtrlStorageState_b,
        boolean * dspRoeSetUpStorageState_b,
        boolean * dspRoeSession_b);
extern Std_ReturnType DcmAppl_DcmStoreRoeDTCInfo( uint8 RoeeventID,
                                                             Dcm_DspRoeEventState_ten RoeEventStatus,
                                                             uint16 SourceAddress,
                                                             uint8 StatusMask,
                                                             boolean dspRoeStorageState_b,
                                                             Dcm_OpStatusType OpStatus,
                                                             boolean dspRoeSession_b,
                                                             Dcm_NegativeResponseCodeType * ErrorCode);

#endif
#if ((DCM_CFG_DSP_ROEDID_ENABLED!=DCM_CFG_OFF) || (DCM_CFG_DSP_ROEONDTCSTATUSCHANGE_ENABLED != DCM_CFG_OFF))
extern void DcmAppl_Switch_DcmResponseOnEvent(uint8 RoeEventId_u8,
                                                                   Dcm_DspRoeEventState_ten RoeEventStatus_en);
extern void DcmAppl_DcmStoreROEcycleCounter ( uint8 RoeeventID,
                                                                         uint8 CycleValue);
extern Std_ReturnType DcmAppl_DcmSetUpRoeEvent (uint8 RoeeventID,
                                        uint8 eventType,
                                        boolean storageState,
                                        uint8 eventWindowTime,
                                                                    const uint8 * eventTypeRecord,
                                                                    const uint8 * serviceToRespondToRecord,
                                                                    Dcm_OpStatusType OpStatus,
                                                                    Dcm_NegativeResponseCodeType * ErrorCode);
#endif

#endif  /* End of DCM_CFG_DSP_RESPONSEONEVENT_ENABLED */


#if ( (DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_SECURITYACCESS_ENABLED != DCM_CFG_OFF) )

#define DCMAPPL_NUMATTGETSEED   0x05            /* Define for number of Attempts to get the valid seed */


extern uint32 DcmAppl_DcmGetUpdatedDelayTime
(
    uint8 SecurityLevel,
    uint8 Delaycount,
    uint32 DelayTime
);

extern uint32 DcmAppl_DcmGetUpdatedDelayForPowerOn
(
    uint8 SecurityLevel,
    uint8 Delaycount,
    uint32 DelayTime
);


extern Dcm_StatusType DcmAppl_DcmCheckSeed
(
    uint32 SeedLen,
    const uint8 * Seed
);

extern void DcmAppl_DcmSecurityLevelLocked(uint8 SecTabIdx);
extern Std_ReturnType DcmAppl_DcmSecaInvalidKey(uint8 SecTabIdx);
extern Std_ReturnType DcmAppl_DcmSecaValidKey(uint8 SecTabIdx);


#endif
#if((DCM_CFG_STORING_ENABLED != DCM_CFG_OFF)||(DCM_CFG_RESTORING_ENABLED != DCM_CFG_OFF))
#if(DCM_CFG_DSP_DIAGNOSTICSESSIONCONTROL_ENABLED == DCM_CFG_OFF)
extern void DcmAppl_DcmGetP2Timings(
                                        uint32 * adrP2Timing_pu32,
                                        uint32 * adrP2StarTiming_pu32,
                                        Dcm_SesCtrlType dataSessionId_u8
                                     );
#endif
#endif

extern void DcmAppl_DcmModifyResponse (
									uint8 dataSid_u8,
									uint8 dataNRC_u8,
									uint8 * adrBufPtr_pu8,
									uint32 * nrBufSize_pu32
							   		);
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF))
Std_ReturnType DcmAppl_DcmCheckRoutineControlOptionRecord(uint16 nrRID_u16,
                                           uint8 dataSubFunc_u8,const uint8 * adrRoutineCtrlOptRec_pcst,uint16 dataRoutineCtrlOptRecSize_u16);
#endif

#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&((DCM_CFG_DSP_REQUESTUPLOAD_ENABLED != DCM_CFG_OFF)||(DCM_CFG_DSP_REQUESTDOWNLOAD_ENABLED!= DCM_CFG_OFF)))
extern Std_ReturnType DcmAppl_CheckEncryptionCompressionMethod(
        uint8 Sid_u8,
        uint8 CompressionMethod,
        uint8 EncryptionMethod );
#endif
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_REQUESTUPLOAD_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_Dcm_ProcessRequestUpload(
        Dcm_OpStatusType OpStatus,
        uint8 DataFormatIdentifier,
        uint32 MemoryAddress,
        uint32 MemorySize,
        uint32 * BlockLength,
        Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_REQUESTDOWNLOAD_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_Dcm_ProcessRequestDownload(
        Dcm_OpStatusType OpStatus,
        uint8 DataFormatIdentifier,
        uint32 MemoryAddress,
        uint32 MemorySize,
        uint32 * BlockLength,
        Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if(DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_REQUESTTRANSFEREXIT_ENABLED != DCM_CFG_OFF)
extern Std_ReturnType DcmAppl_Dcm_ProcessRequestTransferExit(
        Dcm_OpStatusType OpStatus,
        const uint8 * transferRequestParameterRecord,
        uint32 transferRequestParameterRecordSize,
        uint8 * transferResponseParameterRecord,
        uint32 * transferResponseParameterRecordSize,
        Dcm_NegativeResponseCodeType * ErrorCode);
#endif
#if ((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_TRANSFERDATA_ENABLED!= DCM_CFG_OFF))
extern Std_ReturnType DcmAppl_Dcm_CheckPermissionForTransferData(
                                                    uint8 BlockSequenceCounter,
                                                    uint8 memoryId,
                                                    uint32 memoryAddress,
                                                    uint32 * memorySize,
                                                    uint8 * TransferRequestParameterRecord,
                                                    uint32 TransferRequestParameterRecordLength,
                                                    Dcm_TrasferDirection_en dataTransferDirection_ten,
                                                    Dcm_NegativeResponseCodeType * ErrorCode
                                                    );
extern void DcmAppl_DcmMemSizeCheckForTransferData    (
                                                    uint8 dataCompressionMethod_u8,
                                                    uint8 dataMemoryId_u8,
                                                    uint32 dataTransferRequestParameterRecordLength_u32,
                                                    Dcm_TrasferDirection_en dataTransferDirection_ten,
                                                    Dcm_NegativeResponseCodeType * dataErrorCode_u8
                                                    );
#endif
#if((DCM_CFG_DSPUDSSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF))
extern Std_ReturnType DcmAppl_DcmCheckRdbiResponseLength(
                                                    uint32 dataTotalRespLength_u32,
                                                    uint16 dataNumOfDids_u16,
                                                    Dcm_NegativeResponseCodeType * ErrorCode
                                                );
#endif

#if(DCM_CALLAPPLICATIONONREQRX_ENABLED!=DCM_CFG_OFF)
extern void DcmAppl_StartOfReception(Dcm_IdContextType idContext ,
                                                            PduIdType DcmRxPduId,
                                                            PduLengthType RequestLength,
                                                            Dcm_MsgType RxBufferPtr);

extern void DcmAppl_CopyRxData(PduIdType DcmRxPduId,
                                                      PduLengthType RxBufferSize
                                                      );

extern void DcmAppl_TpRxIndication( PduIdType DcmRxPduId,
                                                         Std_ReturnType Result);
#endif
/*TESTCODE-START
#if((DCM_CFG_DSP_DYNAMICALLYDEFINEIDENTIFIER_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_DDDISTORINGTONVRAM_ENABLED != DCM_CFG_OFF))
typedef struct
{
  Dcm_DddiRecord_tst addrRecord_ast[10]; // the size of this array should be atleast the number of records to be added to DDDID
  Dcm_DddiIdContext_tst dataDDDIRecordContext_st;
  Dcm_DddiIdContext_tst dataPDIRecordContext_st;
  uint16 dataDddId_u16;
  uint16 nrMaxNumOfRecords_u16;
} Dcm_DddiStoreInNvMConfig_tst;
extern Dcm_DddiStoreInNvMConfig_tst dataToStoreInNvM_pst[10];
extern uint16 indexOfDDDIToStoreinNvM ;
#endif
TESTCODE-END*/
#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"
#endif
