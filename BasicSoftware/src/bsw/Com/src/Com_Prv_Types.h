

#ifndef COM_PRV_TYPES_H
#define COM_PRV_TYPES_H

/**
 * \defgroup COM_PRV_TYPES_H
 * This interface provides the private exported types of Com module.\n
 * To use this interface include the header <b>Com_Prv_Types.h</b>
 */

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains flags that are intialized before calling Com_Prv_SendIpdu internal API \n
 *
 * typedef struct \n
 * { \n
 *     unsigned int isEventTrig_u1        : 1; Set, If the call to Com_Prv_SendIpdu is not from MainFunctionTx. \n
 *     unsigned int isModeChangd_u1       : 1; Set, If Mode change has happened for the Ipdu \n
 *     unsigned int isSigTriggered_u1     : 1; Set, If signal transfer property is triggered(1)/pending(0) \n
 *     unsigned int isTimeoutReq_u1       : 1; Set, If Timeout monitoring is required \n
 *     unsigned int ignoreRepetitions_u1  : 1; Set, If configured repetitions are to be ignored \n
 *     unsigned int isSwtIpduTxMode_u1    : 1;Set , If the call is made from Com_SwitchIpduTxMode \n
 * } Com_SendIpduInfo_tst; \n
 *
 * IMPORTANT NOTE!!
 * If bitfields are added/removed from here, ensure to add/remove same number of initialize('0') bits in
 * COM_CLEAR_SENDIPDU_FLAGS macro too, which is used to intialize instance of 'Com_SendIpduInfo' structure
 */
typedef struct
{
    unsigned int isEventTrig_u1        : 1;
    unsigned int isModeChangd_u1       : 1;
    unsigned int isSigTriggered_u1     : 1;
    unsigned int isTimeoutReq_u1       : 1;
    unsigned int ignoreRepetitions_u1  : 1;
    unsigned int isSwtIpduTxMode_u1    : 1;
} Com_SendIpduInfo_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure is contains the Transmission Mode information \n
 *
 * typedef struct\n
 * {\n
 *   uint16              timePeriod_u16;            Time Period Factor\n
 *   uint16              timeOffset_u16;            Time Offset Factor\n
 *   uint16              repetitionPeriod_u16;      Repetition Period Factor\n
 *   uint8               numOfRepetitions_u8;       Number of repetitions\n
 *   uint8               mode_u8;                   Transmission Mode\n
 *   boolean             mixedPhaseShift_b;         In MIXED mode, it indicate whether Phase Shift is enabled\n
 *
 * } Com_TransModeInfo_tst;
 */
typedef struct
{
    uint16                                          timePeriod_u16;
    uint16                                          timeOffset_u16;
    uint16                                          repetitionPeriod_u16;
    uint8                                           numOfRepetitions_u8;
    /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
    uint8                                           mode_u8;
    boolean                                         mixedPhaseShift_b;
#else
    /* FC_VariationPoint_END */
    uint8                                           mode_u8;
    /* FC_VariationPoint_START */
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    /* FC_VariationPoint_END */
} Com_TransModeInfo_tst;


/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the current TxMode State.\n
 *
 * typedef const Com_TransModeInfo_tst * Com_TMConstPtr_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_TransModeInfo_tst * Com_TMConstPtr_tpcst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx Signals \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_TxInvalid\n
 *      uint32                  dataInvalidVal_u32;         DataInvalid Value\n
 *   #endif\n
 *   #ifndef COM_INITVALOPTIMIZATION\n
 *      uint32                  initVal_u32;                Initial Value\n
 *   #endif\n
 *      uint16                  txSignalFields_u16;         Signal Property\n
 *
 *   #ifdef COM_TxSigUpdateBit\n
 *      Com_Bitposition_tuo     updateBitPos_uo;            Update Bit Position of the signal\n
 *   #endif\n
 *      Com_Bitposition_tuo     bitPos_uo;                  Bit Position of the signal\n
 *      Com_Bitsize_tuo         bitSize_uo;                 BitSize of the signal if Signal Type is != uint8_N\n
 *                                                          Len of the Signal if Signal Type is == uint8_N\n
 *   #ifdef COM_TxFilters\n
 *      Com_Filter_tuo          idxFilter_uo;               Index to the Filter Array depending on Filter Algorithm\n
 *   #endif\n
 *
 *   #ifdef COM_EffectiveSigTOC\n
 *      Com_OldVal_tuo          idxOldValue_uo;             Index to old value of the signal\n
 *   #endif\n
 *
 *      Com_IpduId_tuo          idComIPdu_uo;               Ipdu Refernce Id\n
 *   #ifdef COM_INITVALOPTIMIZATION\n
 *      Com_OptData_tuo         idxInitValue_uo;    Initial Value Index\n
 *   #endif\n
 *      uint8                   general_u8;                 Type/Endianess of the signal\n
 *
 * } Com_Prv_xTxSigCfg_tst;
 */
typedef struct
{
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_TxIPduNotification
    /*
     * Notification callback function for Signal. This function is called by Com_CallTxNotifyCallback().
     */
    void (*notification_pfct) (void);
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_ERRORNOTIFICATION
    /* Error Notification part*/
    void (*errorNotification_pfct) (void);
#endif

#ifdef COM_TxIPduTimeOutNotify
    /* Timeout Notification part*/
    void (*timeoutNotification_pfct) (void);
#endif
#endif/* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */
#ifdef COM_TxInvalid
    uint32                                          dataInvalidVal_u32;
#endif /* #if Com_TxInvalid */
#ifndef COM_INITVALOPTIMIZATION
    uint32                                          initVal_u32;
#endif

    /*
    typedef struct
    {
        uint16 TransProp       : 3;    Transfer Property of signal
        uint16 FilterAlgorithm : 4;    Filter Algorithm of signal
        uint16 DataInvalidType : 1;    DataInvalid Action
        uint16 TimeOutEnabled  : 1;    Check If time out is configured
    } Com_TxSigPropType;
    */
    uint16                                          txSignalFields_u16;

#ifdef COM_TxSigUpdateBit
    Com_Bitposition_tuo                             updateBitPos_uo;
#endif

    Com_Bitposition_tuo                             bitPos_uo;

    Com_Bitsize_tuo                                 bitSize_uo;

#ifdef COM_TxFilters
    Com_Filter_tuo                                  idxFilter_uo;
#endif

#ifdef COM_EffectiveSigTOC
    Com_OldVal_tuo                                  idxOldValue_uo;
#endif

    Com_IpduId_tuo                                  idComIPdu_uo;
#ifdef COM_INITVALOPTIMIZATION
    Com_OptData_tuo                                 idxInitValue_uo;
#endif
    /*
    typedef struct
    {
        uint8 Type:5;        DataType
        uint8 Endianess:1;   Endianess
        uint8 UpdBitConf:1;  IsUpdateBitConfigured
        uint8 Not_Used:1;
    } Com_GeneralType;
    */
    uint8                                           general_u8;

} Com_Prv_xTxSigCfg_tst;
/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx signal static configuration.\n
 *
 * typedef const Com_Prv_xTxSigCfg_tst * Com_TxSigCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xTxSigCfg_tst * Com_TxSigCfg_tpcst;


/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx Signals \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_RxSignalNotify\n
 *      void (*notification_pfct) (void);    Notification Signal part\n
 *   #endif\n
 *   #ifdef COM_RxSigInvalidNotify\n
 *      void (*invalidNotification_pfct) (void);    Invalid Notification\n
 *   #endif\n
 *   #ifdef COM_RxSigInvalid\n
 *      uint32                  dataInvalidVal_u32;    DataInvalid Value\n
 *   #endif\n
 *   #ifndef COM_INITVALOPTIMIZATION\n
 *      uint32                  initVal_u32;           Initial Value\n
 *   #endif\n
 *
 *   #ifdef COM_RxSigUpdateBit\n
 *      Com_Bitposition_tuo     updateBitPos_uo;        Update Bit Position of the signal\n
 *   #endif\n
 *       Com_Bitposition_tuo     bitPos_uo;             Bit Position of the signal\n
 *       Com_SigBuffIndex_tuo    idxSigBuff_uo;         Index to signal buffer\n
 *       Com_Bitsize_tuo         bitSize_uo;            BitSize of the signal if Signal Type is != uint8_N\n
 *                                                      Len of the Signal if Signal Type is == uint8_N\n

 *
 *   #ifdef COM_RxFilters\n
 *      Com_Filter_tuo          idxFilter_uo;           Index to the Filter Array depending on Filter Algorithm\n
 *   #endif\n
 *      Com_IpduId_tuo          idComIPdu_uo;           Ipdu Refernce Id\n
 *    #ifdef COM_INITVALOPTIMIZATION\n
 *      Com_OptData_tuo                                 idxInitValue_uo;    Initial Value Index\n
 *   #endif\n
 *      uint8                   general_u8;             Type/Endianess of the signal\n
 *      uint8                   rxSignalFields_u8;      Transfer Property/DataTimeoutAction/DataInvalidAction/FilterAlgorithm of the signal\n
 *
 * } Com_Prv_xRxSigCfg_tst;
 */
typedef struct
{
#ifdef COM_RxSignalNotify
    /* Notification Signal part */
    void (*notification_pfct) (void);
#endif
#ifdef COM_RxSigInvalidNotify
    void (*invalidNotification_pfct) (void);
#endif /* #ifdef COM_RxSigInvalidNotify */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_RxIPduTimeoutNotify
    /* Timeout Notification part*/
    void (*timeoutNotification_pfct) (void);
#endif
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */
#ifdef COM_RxSigInvalid
    uint32                                          dataInvalidVal_u32;
#endif /* #ifdef COM_RxSigInvalid */
#ifndef COM_INITVALOPTIMIZATION
    uint32                                          initVal_u32;
#endif
#ifdef COM_RxSigUpdateBit
    Com_Bitposition_tuo                             updateBitPos_uo;
#endif
    Com_Bitposition_tuo                             bitPos_uo;
    Com_SigBuffIndex_tuo                            idxSigBuff_uo;
    Com_Bitsize_tuo                                 bitSize_uo;
#ifdef COM_RxFilters
    /* Filter */
    Com_Filter_tuo                                  idxFilter_uo;
#endif

    Com_IpduId_tuo                                  idComIPdu_uo;
#ifdef COM_INITVALOPTIMIZATION
    Com_OptData_tuo                                 idxInitValue_uo;
#endif
    /*
    typedef struct
    {
        uint8 Type:5;        DataType
        uint8 Endianess:1;   Endianess
        uint8 UpdBitConf:1;  IsUpdateBitConfigured
        uint8 Not_Used:1;
    } Com_GeneralType;
    */
    uint8                                           general_u8;
    /*
    typedef struct
    {
        uint8 DataInvalidType:2;    Data Invalid Action
        uint8 FilterAlgorithm:4;    Filter Algorithm
        uint8 DataTimeoutType:1;    DataTimeout Action
        uint8 IsGwSignal:1;         Is gateway signal
    }Com_RxSigPropType;
    */
    uint8                                           rxSignalFields_u8;

} Com_Prv_xRxSigCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx signal static configuration.\n
 *
 * typedef const Com_Prv_xRxSigCfg_tst * Com_RxSigCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xRxSigCfg_tst * Com_RxSigCfg_tpcst;



/* Structure for Tx Signal Group  */
#ifdef COM_TX_SIGNALGROUP

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx SignalGroupArray static configuration \n
 *
 * typedef struct\n
 * {\n
 *      PduLengthType  firstByteNo_uo;  The start position of the uint8-array representation of the communication data
 *                                      shall be the byte offset of the group signal in this signal group with the
 *                                      smallest ComBitPosition.
 *      PduLengthType  length_uo;       The length of the uint8-array representation of the communication data shall
 *                                      be the number of bytes which all the ComGroupSignals in this ComSignalGroup
 *                                      occupy in the ComIPdu.
 * } Com_Prv_xTxSigGrpArrayCfg_tst;\n
 */
# ifdef COM_TX_SIGNALGROUP_ARRAY
typedef struct
{
    PduLengthType                                   firstByteNo_uo;
    PduLengthType                                   length_uo;

} Com_Prv_xTxSigGrpArrayCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx SignalGroup Array static configuration.\n
 */
typedef const Com_Prv_xTxSigGrpArrayCfg_tst * Com_TxSigGrpArrayCfg_tpcst;

# endif /* # ifdef COM_TX_SIGNALGROUP_ARRAY */

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx Signal Groups \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_TxSigGrpUpdateBit\n
 *      Com_Bitposition_tuo         updateBitPos_uo;            Update Bit Position of the signal group\n
 *   #endif\n
 *      Com_IpduId_tuo              idComIPdu_uo;               Ipdu Refernce Id\n
 *      Com_TxIntGrpSignalId_tuo    idFirstGrpSig_uo;          Index to First Group Signal in the Signal group\n
 *      Com_NoOfTxGrpSignal_tuo     numOfGrpSig_uo;             No Of Group Signals present in the Signal group\n
 *      uint8                       txSignalGrpFields_u8;       Properties of Tx Signal Group\n
 * } Com_Prv_xTxSigGrpCfg_tst;
 *
 */
typedef struct
{
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_TxIPduNotification
    /*
     * Notification callback function for SignalGroup. This function is called by Com_CallTxNotifyCallback().
     */
    void (*notification_pfct) (void);
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_ERRORNOTIFICATION
    /* Error Notification part*/
    void (*errorNotification_pfct) (void);
#endif

#ifdef COM_TxIPduTimeOutNotify
    /* Timeout Notification part*/
    void (*timeoutNotification_pfct) (void);
#endif
#endif/* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

#ifdef COM_TxSigGrpUpdateBit
    Com_Bitposition_tuo                             updateBitPos_uo;
#endif
    Com_IpduId_tuo                                  idComIPdu_uo;
    Com_TxIntGrpSignalId_tuo                        idFirstGrpSig_uo;
    Com_NoOfTxGrpSignal_tuo                         numOfGrpSig_uo;

#ifdef COM_TX_SIGNALGROUP_ARRAY
    Com_TxSigGrpArrayIndex_tuo                      idxSigGrpArray_uo;
#endif
    /* FC_VariationPoint_START */
    /*
        typedef struct
        {
             uint8 TransProp     :3;     Transfer Property of a SignalGroup
             uint8 TimeOutEnabled:1;     COM_TRUE : If time out is confifured
                                         COM_FALSE: If time out is not confifured for the signal group
             uint8 FilterEvalReq :1;     COM_TRUE : If there exist group signal's with filter configured
                                         COM_FALSE: If there don't exist group signal's with filter configured
             uint8 UpdBitConf    :1;     COM_TRUE - Configured, COM_FALSE - Not configured
             uint8 ArrayAccess   :1;     COM_TRUE - Array Access Enabled, COM_FALSE - Array Access Disabled
             uint8 Not_Used      :1;     Unused Bit
        } Com_TxSigGrpPropType;
     */
    /* FC_VariationPoint_END */
    uint8                                           txSignalGrpFields_u8;

} Com_Prv_xTxSigGrpCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx signal group static configuration.\n
 *
 * typedef const Com_Prv_xTxSigGrpCfg_tst * Com_TxSigGrpCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xTxSigGrpCfg_tst * Com_TxSigGrpCfg_tpcst;

#endif



#ifdef COM_RX_SIGNALGROUP

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx SignalGroupArray static configuration \n
 *
 * typedef struct\n
 * {\n
 *      PduLengthType             firstByteNo_uo;       The start position of the uint8-array representation of the
 *                                                      communication data shall be the byte offset of the group signal
 *                                                      in this signal group with the smallest ComBitPosition.
 *      Com_RxSigGrpBuffIndex_tuo idxRxSigGrpBuf_uo;    Index to RxSignalGroup buffer for each of the SignalGroup enabled
 *                                                      with ComSignalGroupArrayAccess
 * } Com_Prv_xRxSigGrpArrayCfg_tst;\n
 */
# ifdef COM_RX_SIGNALGROUP_ARRAY
typedef struct
{
    PduLengthType                                   firstByteNo_uo;
    Com_RxSigGrpBuffIndex_tuo                       idxRxSigGrpBuf_uo;

} Com_Prv_xRxSigGrpArrayCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx SignalGroup Array static configuration.\n
 */
typedef const Com_Prv_xRxSigGrpArrayCfg_tst * Com_RxSigGrpArrayCfg_tpcst;

# endif /* # ifdef COM_RX_SIGNALGROUP_ARRAY */

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx Signal Groups \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_RxSignalGrpNotify\n
 *      void (*notification_pfct) (void);    Notification Signal part\n
 *   #endif\n
 *   #ifdef COM_RxSigGrpInvalidNotify\n
 *      void (*invalidNotification_pfct) (void);    Invalid Notification\n
 *   #endif\n
 *    #ifdef COM_RxSigGrpUpdateBit\n
 *      Com_Bitposition_tuo         updateBitPos_uo;           Update Bit Position of the signal group\n
 *   #endif\n
 *      Com_IpduIdType              idComIPdu_uo;              Ipdu Refernce Id\n
 *      Com_RxIntGrpSignalId_tuo    idFirstGrpSig_uo;         Index to First Group Signal in the Signal group\n
 *      Com_NoOfRxGrpSignal_tuo     numOfGrpSig_uo;            No Of Group Signals present in the Signal group\n
 *      uint8                       rxSignalGrpFields_u8;      Properties of Signal Group\n
 * } Com_Prv_xRxSigGrpCfg_tst;
 */
typedef struct
{
#ifdef COM_RxSignalGrpNotify
    /* Notification Signal part */
    void (*notification_pfct) (void);
#endif
#ifdef COM_RxSigGrpInvalidNotify
    void (*invalidNotification_pfct) (void);
#endif /* #ifdef COM_RxSigGrpInvalidNotify */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_RxIPduTimeoutNotify
    /* Timeout Notification part*/
    void (*timeoutNotification_pfct) (void);
#endif
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */
#ifdef COM_RxSigGrpUpdateBit
    Com_Bitposition_tuo                             updateBitPos_uo;
#endif
    Com_IpduId_tuo                                  idComIPdu_uo;

    Com_RxIntGrpSignalId_tuo                        idFirstGrpSig_uo;

    Com_NoOfRxGrpSignal_tuo                         numOfGrpSig_uo;
#ifdef COM_SIGNALGROUPGATEWAY
    uint16                                          numOfGwSrcGrpSignals_u16;
#endif /* COM_SIGNALGROUPGATEWAY */
    /* Last byte in the IPdu to which the signal group spans till.
     * Now lasyByte is also used in Com_ReceiveSignalGroupArray() to calculate SignalGroup Array Length.
     */
    PduLengthType                                   lastByte_uo;

#ifdef COM_RX_SIGNALGROUP_ARRAY
    Com_RxSigGrpArrayIndex_tuo                      idxSigGrpArray_uo;
#endif

/* FC_VariationPoint_START */
    /*
    typedef struct
    {
        uint8 DataInvalidType:2; DataInvalid Action
        uint8 DataTimeoutType:1; DataTimeout Action
        uint8 IsGwSignal:1;      SignalGroup Gateway
        uint8 UpdBitConf:1;      Is UpdateBit set
        uint8 ArrayAccess:1;     Is SignalGroup Array Access enabled
        uint8 FilterEvalReq:1;   Is Filter processing required at any of its GroupSignal
        uint8 Not_Used:1;
    } Com_RxSigGrpPropertyType;
    */
/* FC_VariationPoint_END */
    uint8                                           rxSignalGrpFields_u8;

} Com_Prv_xRxSigGrpCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx signal group static configuration.\n
 *
 * typedef const Com_Prv_xRxSigGrpCfg_tst * Com_RxSigGrpCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xRxSigGrpCfg_tst * Com_RxSigGrpCfg_tpcst;

#endif /* #ifdef COM_RX_SIGNALGROUP */


#ifdef COM_TX_SIGNALGROUP

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx Group Signals \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_TxGrpSigInvalid\n
 *      uint32                      dataInvalidVal_u32;        DataInvalid Value\n
 *   #endif\n
 *   #ifndef COM_INITVALOPTIMIZATION\n
 *      uint32                      initVal_u32;               Initial Value\n
 *   #endif
 *      Com_Bitposition_tuo         bitPos_uo;                 Bit Position of the signal\n
 *      Com_TxGrpSigBuffIndex_tuo   idxTxGrpSigBuff_uo;        Group Signal Buffer Index\n
 *      Com_TxIntSignalGroupId_tuo  idSigGrp_uo;               Signal Group Id
 *      Com_Bitsize_tuo             bitSize_uo;                BitSize of the signal\n
 *
 *      uint16                      txGrpSigFields_u16;        Type/Endianess/transfer property/invalid value
 *                                                             configured/Is dynamic group signal of the  group signal\n
 *   #ifdef COM_TxFilters\n
 *      Com_Filter_tuo              idxFilter_uo;              Index to the Filter Array depending on Filter Algorithm\n
 *      uint8                       filterAlgo_u8;             Filter Algorithm of the Group Signal
 *   #endif\n
 *
 *   #ifdef COM_EffectiveSigGrpTOC\n
 *      Com_OldVal_tuo              idxOldValue_uo;            Index to old value of the signal\n
 *   #endif\n
 *   #ifdef COM_INITVALOPTIMIZATION\n
 *   Com_OptData_tuo                idxInitValue_uo;           Initial Value Index\n
 *   #endif\n
 *
 * } Com_Prv_xTxGrpSigCfg_tst;
 */

typedef struct
{

#ifdef COM_TxGrpSigInvalid
    uint32                                          dataInvalidVal_u32;
#endif
#ifndef COM_INITVALOPTIMIZATION
    uint32                                          initVal_u32;
#endif
    /*
    typedef struct
    {
        uint16 Type:5;           DataType
        uint16 Endianess:1;      Endianess of Grp signal
        uint16 TransProp:1;      set, if the group signal contributes to the evaluation of transfer property group signal
        uint16 GrpSigInvalid:1;  Indicates if the group signal has configured invalid value
        uint16 IsTxDynGrpSig:1;  Set, if Tx dynamic group signal

    } Com_TxGrpSigPropertyType;
    */
    uint16                                          txGrpSigFields_u16;

    Com_Bitposition_tuo                             bitPos_uo;
    Com_TxGrpSigBuffIndex_tuo                       idxTxGrpSigBuff_uo;

    Com_TxIntSignalGroupId_tuo                      idSigGrp_uo;

    Com_Bitsize_tuo                                 bitSize_uo;

#ifdef COM_TxFilters
    /* Filter */
    Com_Filter_tuo                                  idxFilter_uo;
    uint8                                           filterAlgo_u8;
#endif

#ifdef COM_EffectiveSigGrpTOC
    Com_OldVal_tuo                                  idxOldValue_uo;
#endif
#ifdef COM_INITVALOPTIMIZATION
    Com_OptData_tuo                                 idxInitValue_uo;
#endif

} Com_Prv_xTxGrpSigCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx group signal static configuration.\n
 *
 * typedef const Com_Prv_xTxGrpSigCfg_tst * Com_TxGrpSigCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xTxGrpSigCfg_tst * Com_TxGrpSigCfg_tpcst;

#endif /* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx Group Signals \n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_RxSigGrpInvalid\n
 *      uint32                      dataInvalidVal_u32;        DataInvalid Value\n
 *   #endif\n
 *   #ifndef COM_INITVALOPTIMIZATION\n
 *      uint32                      initVal_u32;               Initial Value\n
 *   #endif\n
 *      Com_Bitposition_tuo         bitPos_uo;                 Bit Position of the signal\n
 *      Com_RxGrpSigBuffIndex_tuo   idxRxGrpSigBuff_uo;         Group Signal Buffer Index\n
 *      Com_RxIntSignalGroupId_tuo  idSigGrp_uo;               Signal Group Id
 *      Com_Bitsize_tuo             bitSize_uo;                 BitSize of the signal\n
 *   #ifdef COM_RxFilters
 *      Com_Filter_tuo              idxFilter_uo;               Index to the filter properties\n
 *      uint8                       filterAlgo_u8;             Filter algorithm as per the configuration
 *   #endif
 *   #ifdef COM_INITVALOPTIMIZATION\n
 *   Com_OptData_tuo                idxInitValue_uo;    Initial Value Index\n
 *   #endif\n
 *      uint8                       rxGrpSigFields_u8;          Type/Endianess of the group signal\n
 *
 * } Com_Prv_xRxGrpSigCfg_tst;
 */
typedef struct
{
#ifdef COM_RxSigGrpInvalid
    uint32                                          dataInvalidVal_u32;
#endif
#ifndef COM_INITVALOPTIMIZATION
    uint32                                          initVal_u32;
#endif
    Com_Bitposition_tuo                             bitPos_uo;
    Com_RxGrpSigBuffIndex_tuo                       idxRxGrpSigBuff_uo;

    Com_RxIntSignalGroupId_tuo                      idSigGrp_uo;

    Com_Bitsize_tuo                                 bitSize_uo;
#ifdef COM_RxFilters
    /* Filter */
    Com_Filter_tuo                                  idxFilter_uo;
    uint8                                           filterAlgo_u8;
#endif

#ifdef COM_INITVALOPTIMIZATION
    Com_OptData_tuo                                 idxInitValue_uo;
#endif
    /*
    typedef struct
    {
        uint8 Type:5;               DataType
        uint8 Endianess:1;          Endianess of grp signal
        uint8 GrpSigInvalid:1;      Has group signal configured invalid value
        uint8 IsRxDynGrpSig:1;      Set, if Rx dynamic group signal
    } Com_RxGrpSigPropertyType;
    */
    uint8                                           rxGrpSigFields_u8;
} Com_Prv_xRxGrpSigCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx group signal static configuration.\n
 *
 * typedef const Com_Prv_xRxGrpSigCfg_tst * Com_RxGrpSigCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xRxGrpSigCfg_tst * Com_RxGrpSigCfg_tpcst;
#endif

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx IPduCounter\n
 *
 * typedef struct\n
 * {\n
 *      Com_Bitposition_tuo  bitPos_uo;    IpduCounter start bit position\n
 *      uint8                bitSize_u8;   IpduCounter bit size\n
 *
 * } Com_Prv_xTxIpduCntrCfg_tst;
 */
#ifdef COM_TX_IPDUCOUNTER

typedef struct
{
    Com_Bitposition_tuo                             bitPos_uo;
    uint8                                           bitSize_u8;

} Com_Prv_xTxIpduCntrCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx IPduCounter static configuration.\n
 *
 * typedef const Com_Prv_xTxIpduCntrCfg_tst * Com_TxIpduCntrCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xTxIpduCntrCfg_tst * Com_TxIpduCntrCfg_tpcst;

#endif /* #ifdef COM_TX_IPDUCOUNTER */

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Tx IPDU \n
 *
 * typedef struct\n
 * {\n
 *      uint8 *                         buffPtr_pu8;                Pointer to the Ipdu Buffer\n
 *      const Com_TransModeInfo_tst *   tMConstPtr_pcst;        Pointer to Transfer mode information\n
 *   #ifdef COM_TxIPduCallOuts\n
 *      boolean (*callOut_pfct) (PduIdType, PduInfoType * ); Ipdu Callout Function\n
 *   #endif\n
 *   #ifdef COM_TxIPduNotification\n
 *      void (*notification_pfct) (void);        Notification callback function for IPDU\n
 *   #endif\n
 *   #ifdef COM_ERRORNOTIFICATION \n
 *      void (*errorNotification_pfct) (void);   Error Notification part\n
 *   #endif\n
 *   #ifdef COM_TxIPduTimeOutNotify\n
 *      void (*timeoutNotification_pfct) (void); Timeout Notification part\n
 *   #endif\n
 *   #ifdef COM_METADATA_SUPPORT\n
 *      Com_MetaDataInfo_tst        metaDataPtr_pcst;           Address to metadata static configuration\n
 *   #endif\n
 *      uint16                      size_uo;                    Size in Bits\n
 *      #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)\n
 *      uint16                      idHandle_u16;               Handle Id\n
 *      #endif\n
 *   #ifdef COM_TxIPduTimeOut\n
 *      uint16                      timeout_u16;                Support Tx IPDU Timeout \n
 *   #endif\n
 *      uint16                      minDelayTime_u16;           MinDelay Time factor\n
 *      uint16                      numOfSig_u16;               No Of Signals present in the IPDU\n
 *   #ifdef COM_TX_SIGNALGROUP\n
 *      uint16                      numOfSigGroups_u16;          No of Signal Groups present in the IPDU\n
 *   #endif\n
 *      PduIdType                   idPduRSrcPdu_uo;            Lower Layer ID\n
 *      Com_TxIntSignalId_tuo       idTxSig_uo;                 Index to First Signal within this Ipdu\n
 *   #ifdef COM_TX_SIGNALGROUP\n
 *      Com_TxIntSignalGroupId_tuo  idFirstTxSigGrp_uo;        Index to First Signal Group within this Ipdu\n
 *   #endif\n
 *      uint16                      txIPduFields_u16;           Transmission Type\n
 *   #ifdef COM_TX_IPDUCOUNTER
 *      Com_TxCntrIdx_tuo           idxTxIpduCntr_uo;           Index to TxIpduCounter static configuration\n
 *   #endif
 *      Com_MainFunc_tuo            idMainFunc_uo;              MainFunction Number to which this IPdu belongs to\n
 *      uint8                       paddingByte_u8;             Padding Byte\n
 *
 * } Com_Prv_xTxIpduInfoCfg_tst;
 */
typedef struct
{
    uint8 *                                         buffPtr_pu8;

    const Com_TransModeInfo_tst *                   tMConstPtr_pcst;
#ifdef COM_TxIPduCallOuts
    /*Ipdu Callout Function*/
    boolean (*callOut_pfct) (PduIdType id, PduInfoType * ptr);
#endif
#ifdef COM_TxIPduNotification
        /*
         * Notification callback function for IPDU. This function is generated through code generator.
         * Individual signal and signal group notifications functions are called from within this function.
         */
    void (*notification_pfct) (void);
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_ERRORNOTIFICATION
    /* Error Notification part*/
    void (*errorNotification_pfct) (void);
#endif

#ifdef COM_TxIPduTimeOutNotify
    /* Timeout Notification part*/
    void (*timeoutNotification_pfct) (void);
#endif

#ifdef COM_METADATA_SUPPORT
    Com_TxMetaData_tpcst                            txMetaData_pcst;
#endif

#if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE)
    PduLengthType                                   size_uo;
#endif

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
    uint16                                          idHandle_u16;
#endif

#ifdef COM_TxIPduTimeOut
    uint16                                          timeout_u16;
#endif

    uint16                                          minDelayTime_u16;

    uint16                                          numOfSig_u16;
#ifdef COM_TX_SIGNALGROUP
    uint16                                          numOfSigGroups_u16;
#endif
#if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE)
    PduIdType                                       idPduRSrcPdu_uo;
#endif
    Com_TxIntSignalId_tuo                           idTxSig_uo;
#ifdef COM_TX_SIGNALGROUP
    Com_TxIntSignalGroupId_tuo                      idFirstTxSigGrp_uo;
#endif
    /*
    typedef struct
    {
        uint16 Signal_Processing:1;         Signal Processing of each signal within the IPDU(DEFERRED(1)/IMMEDIATE(0))
        uint16 TMSCalclation:2;             Transmission mode status
                                                COM_MODE_VALID      (0)  If both the modes are configured
                                                COM_TRUEMODE_VALID  (1)  If TRUE mode is configured
                                                COM_FALSEMODE_VALID (2)  If FALSE mode is configured
                                                COM_MODE_INVALID    (3)  If both the modes are not configured
        uint16 NoneModeTimeOut:1;           Set, if Deadline monitoring for the IPdu for TX mode none
        uint16 ClearUpdBit:2;               Status of where to clear update bit
                                                TRANSMIT        (1)
                                                TRIGGERTRANSMIT (2)
                                                CONFIRMATION    (3)
        uint16 FilterEvalReq:1;             Set, if Filter evaluation required
        uint16 IsDynPdu:1;                  Set, if the Ipdu is of Dynamic type
        uint16 IsGwDestPdu:1;               Set, if the Ipdu has Gw destination signal
        uint16 IsCalloutFrmTrigTrans:1;     Set, if the callout is from Com_TriggerTransmit
        uint16 IsLargeDataPdu:1;            Set, if the IPdu is of Large data TP IPdu
        uint16 isCancelTransmitSupported:1; Set, if the IPdu supports CancelTransmit
        uint16 ipduPartOfIpduGrp:1;         Set, if the Ipdu is referring any IpduGroup
        uint16 defaultTMSStatus:1;          Set, if Ipdu is not referring to any IpduGroup, and
                                                 the filter evaluation during the configuration time resulted in true
        uint16 Is_MetaDataPdu:1;            Set, if IPDU is metadata pdu
        uint16 Not_Used:1;                  Unused bitfields, reserved for future use
    }Com_TxIpduFlagType;
    */
    uint16                                          txIPduFields_u16;
#ifdef COM_TX_IPDUCOUNTER
    Com_TxCntrIdx_tuo                               idxTxIpduCntr_uo;
#endif
    Com_MainFunc_tuo                                idMainFunc_uo;
    uint8                                           paddingByte_u8;

} Com_Prv_xTxIpduInfoCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx IPDU static configuration.\n
 *
 * typedef const Com_Prv_xTxIpduInfoCfg_tst * Com_TxIpduCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xTxIpduInfoCfg_tst * Com_TxIpduCfg_tpcst;


/* Time out information structure for signals/signal groups with update-bit */
#if (defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout))
/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Time out information for signals\n
 *
 * typedef struct\n
 * {\n
 *   #ifdef COM_RxUpdateTimeoutNotify\n
 *      void (*timeoutNotification_pfct) (void); Notification function for timeout\n
 *   #endif\n
 *      uint16          firstTimeout_u16;     First timeout value\n
 *      uint16          timeout_u16;          Ttimeout value\n
 *      uint16          idSig_u16;            Signal Id\n
 * } Com_SignalTimeoutInfo_tst;
 */
typedef struct
{
    /* Error Notification part*/
#ifdef COM_RxUpdateTimeoutNotify
    void (*timeoutNotification_pfct) (void);
#endif /* #ifdef COM_RxUpdateTimeoutNotify */
    uint16                                          firstTimeout_u16;
    uint16                                          timeout_u16;
    uint16                                          idSig_u16;
} Com_SignalTimeoutInfo_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Time out information for signals/signal groups with update-bit\n
 *
 * typedef struct\n
 * {\n
 *      const Com_SignalTimeoutInfo_tst *   timeoutInfoRef_pcst;       Reference to timeout information structure
 *                                                                     for all signals/signal groups, with update-bit\n
 *      uint16 *                            timeoutTicks_pu16;         Pointer to timer array\n
 *      uint8                               numWithUpdateBit_u8;       Number of signals/signal groups with update-bit,
 *                                                                     with enabled deadline monitoring\n
 * } Com_UpdatebitTimeoutInfo_tst;
 */
typedef struct
{
    /* Reference to timeout information structure for all signals/signal groups, with update-bit */
    const Com_SignalTimeoutInfo_tst *               timeoutInfoRef_pcst;
    uint16 *                                        timeoutTicks_pu16;
    uint8                                           numWithUpdateBit_u8;
} Com_UpdatebitTimeoutInfo_tst;

#endif /* #if (defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout)) */

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx IPduCounter\n
 *
 * typedef struct\n
 * {\n
 *      void (*counterErrNotification_pfct) (PduIdType, uint8, uint8); Cbk on mismatch of Rx & Exp cntr\n
 *      Com_Bitposition_tuo         bitPos_uo;          IpduCounter start bit position\n
 *      uint8                       bitSize_u8;         IpduCounter bit size\n
 *      uint8                       threshold_u8;       IpduCounter threshold\n
 *
 * } Com_Prv_xRxIpduCntrCfg_tst;
 */
#ifdef COM_RX_IPDUCOUNTER
typedef struct
{
    void (*counterErrNotification_pfct) (PduIdType Id, uint8 ExpCntr, uint8 RxCntr);
    Com_Bitposition_tuo                             bitPos_uo;
    uint8                                           bitSize_u8;
    uint8                                           threshold_u8;

} Com_Prv_xRxIpduCntrCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx IPduCounter static configuration.\n
 *
 * typedef const Com_Prv_xRxIpduCntrCfg_tst * Com_RxIpduCntrCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xRxIpduCntrCfg_tst * Com_RxIpduCntrCfg_tpcst;

#endif /* #ifdef COM_RX_IPDUCOUNTER */


/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for Rx IPDU \n
 *
 * typedef struct\n
 * {\n
 *      uint8 * buffPtr_pau8;                Pointer to the Ipdu Buffer\n
 *   #ifdef  COM_RxSigUpdateTimeout\n
 *      const Com_UpdatebitTimeoutInfo_tst * signalTimeoutRef_pcst;      Pointer to timeout information structure for
 *                                                                       signals with update-bits, within the IPdu\n
 *   #endif\n
 *   #ifdef  COM_RxSigGrpUpdateTimeout\n
 *      const Com_UpdatebitTimeoutInfo_tst * signalGrpTimeoutRef_pcst;   Pointer to timeout information structure for
 *                                                                       signal groups with update-bits, within the IPdu\n
 *   #endif
 *   #ifdef COM_RxIPduCallOuts\n
 *      boolean (*callOut_pfct) (PduIdType, const PduInfoType * ); Ipdu Callout Function\n
 *   #endif\n
 *   #ifdef COM_RxIPduTimeoutNotify\n
 *      void (*timeoutNotification_pfct) (void); Timeout Notification part\n
 *   #endif\n
 *   #ifdef COM_RxIPduNotification\n
 *      void (*notification_pfct) (void);    Notification callback function for IPDU. This function is generated through code generator\n
 *   #endif\n
 *   #ifdef COM_METADATA_SUPPORT\n
 *      Com_RxMetaDataInfo_tst          rxMetaDataPtr_pcst;         Address to metadata static configuration\n
 *   #endif\n
 *      uint16                          size_uo;                    Size in Bits\n
 *      #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)\n
 *      uint16                          idHandle_u16                Handle Id \n
 *      #endif\n
 *   #ifdef COM_RxIPduTimeout\n
 *      uint16                          firstTimeout_u16;           First time out value after IPDU group start\n
 *      uint16                          timeout_u16;                Support Rx IPDU Timeout \n
 *   #endif\n
 *      uint16                          numOfSig_u16;               No Of Signals present in the IPDU\n
 *   #ifdef COM_RX_SIGNALGROUP\n
 *      uint16                          numOfSigGroups_u16;         No of Signal Groups present in the IPDU\n
 *   #endif\n
 *      Com_RxIntSignalId_tuo           idRxSig_uo;                 Index to First Signal within this Ipdu\n
 *   #ifdef COM_RX_SIGNALGROUP\n
 *      Com_RxIntSignalGroupId_tuo      idFirstRxSigGrp_uo;         Index to First Signal Group within this Ipdu\n
 *   #endif\n
 *   #ifdef COM_RX_IPDUCOUNTER
 *      Com_RxCntrIdx_tuo               idxRxIpduCntr_uo;           Index to RxIpduCounter static configuration\n
 *   #endif
 *      Com_MainFunc_tuo                idMainFunc_uo;              MainFunction Number to which this IPdu belongs to\n
 *      uint8                           rxIPduFields_u8;            Reception IPdu Type\n
 *
 * } Com_Prv_xRxIpduInfoCfg_tst;
 */
typedef struct
{
    uint8 *                                         buffPtr_pau8;

#ifdef  COM_RxSigUpdateTimeout
    const Com_UpdatebitTimeoutInfo_tst *            signalTimeoutRef_pcst;
#endif

#ifdef  COM_RxSigGrpUpdateTimeout
    const Com_UpdatebitTimeoutInfo_tst *            signalGrpTimeoutRef_pcst;
#endif

#ifdef COM_RxIPduCallOuts
    boolean (*callOut_pfct) (PduIdType id, const PduInfoType * ptr);
#endif

#ifdef COM_RxIPduTimeoutNotify
    void (*timeoutNotification_pfct) (void);
#endif

#ifdef COM_RxIPduNotification
    void (*notification_pfct) (void);
#endif

#ifdef COM_METADATA_SUPPORT
    Com_RxMetaData_tpcst                            rxMetaDataPtr_pcst;
#endif
#if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE)
    PduLengthType                                   size_uo;
#endif
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
    uint16                                          idHandle_u16;
#endif
#ifdef COM_RxIPduTimeout
    uint16                                          firstTimeout_u16;
    uint16                                          timeout_u16;
#endif
    uint16                                          numOfSig_u16;
#ifdef COM_RX_SIGNALGROUP
    uint16                                          numOfSigGrp_u16;
#endif
    Com_RxIntSignalId_tuo                           idRxSig_uo;
#ifdef COM_RX_SIGNALGROUP
    Com_RxIntSignalGroupId_tuo                      idFirstRxSigGrp_uo;
#endif
#ifdef COM_SIGNALGATEWAY
    uint16                                          numOfGwSrcSignals_u16;
#endif
#ifdef COM_SIGNALGROUPGATEWAY
    uint16                                          numOfGwSrcSignalGrps_u16;
#endif

#ifdef COM_RX_IPDUCOUNTER
    Com_RxCntrIdx_tuo                               idxRxIpduCntr_uo;
#endif

    Com_MainFunc_tuo                                idMainFunc_uo;

    /*
    typedef struct
    {
        uint8 Signal_Processing:1;  Signal Processing of each signal within the IPDU
                                     COM_TRUE  : COM_DEFERRED  IPdu Processing
                                     COM_FALSE : COM_IMMEDIATE IPdu Processing
        uint8 Notify_Cbk:1;         Set, If any of the Signal within the Ipdu has Notification cbk configured
        uint8 IsRxGwIPdu:1;         Set, if RxIpdu has any data to be Gatewayed
        uint8 IpduPartOfIpduGrp:1;  Set, if the Ipdu is referring any IpduGroup
        uint8 IS_TP_TYPE:1;         Set, if IPDU type is TP otherwise 0 for NORMAL
        uint8 TP_INV_CFG:1;         Set, if TP IPDU contains signal(group) with configured invalidAction
        uint8 Is_MetaDataPdu:1;     Set, if IPDU is metadata pdu
        uint8 Not_Used:1;           Not used Bit field

    } Com_RxIpduFlagType;
    */
    uint8                                           rxIPduFields_u8;

} Com_Prv_xRxIpduInfoCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx IPDU static configuration.\n
 *
 * typedef const Com_Prv_xRxIpduInfoCfg_tst * Com_RxIpduCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xRxIpduInfoCfg_tst * Com_RxIpduCfg_tpcst;


/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the information for IPDU Group \n
 *
 * typedef struct\n
 * {\n
 *      uint16      idFirstIpdu_u16;       First ipdu index in the group \n
 *      uint16      numOfRxPdus_u16;       No. of Rx IPDUs in a particular group\n
 * } Com_Prv_xIpduGrpInfoCfg_tst;
 */
typedef struct
{
    uint16                                          idFirstIpdu_u16;
    uint16                                          numOfRxPdus_u16;

} Com_Prv_xIpduGrpInfoCfg_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the IPDU Group static configuration.\n
 *
 * typedef const Com_Prv_xIpduGrpInfoCfg_tst * Com_IpduGrpCfg_tpcst;\n
 */
/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xIpduGrpInfoCfg_tst * Com_IpduGrpCfg_tpcst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Tx signal flags \n
 *
 * typedef struct\n
 * {\n
 *      uint8   txSigRAMFields_u8; \n
 * } Com_TxSignalFlagType_tst;
 */
typedef struct
{
    uint8                                           txSigRAMFields_u8;

} Com_TxSignalFlagType_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx Signal dynamic configuration.\n
 *
 * typedef const Com_TxSignalFlagType_tst * Com_TxSigRam_tpst;\n
 */
typedef Com_TxSignalFlagType_tst * Com_TxSigRam_tpst;

#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Tx group signal flags \n
 *
 * typedef struct\n
 * {\n
 *      uint8   txGrpSigRAMFields_u8; \n
 * } Com_TxGrpSignalFlagType_tst;
 */
typedef struct
{
    uint8                                           txGrpSigRAMFields_u8;

} Com_TxGrpSignalFlagType_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx Group Signal dynamic configuration.\n
 *
 * typedef const Com_TxGrpSignalFlagType_tst * Com_TxGrpSigRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_TxGrpSignalFlagType_tst * Com_TxGrpSigRam_tpst;

#endif


/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Rx signal flags \n
 *
 * typedef struct\n
 * {\n
 *      uint8   rxSigRAMFields_u8;     Flag used to check if there was any Data Timeout for the signal\n
 * } Com_RxSignalFlagType_tst;
 */
typedef struct
{
    uint8                                           rxSigRAMFields_u8;

} Com_RxSignalFlagType_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx Signal dynamic configuration.\n
 *
 * typedef const Com_RxSignalFlagType_tst * Com_RxSigRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_RxSignalFlagType_tst * Com_RxSigRam_tpst;


#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Rx signal group flags \n
 *
 * typedef struct\n
 * {\n
 *      uint8   rxSigGrpRAMFields_u8; \n
 * } Com_RxSignalGrpFlagType_tst;
 */
typedef struct
{
    uint8                                           rxSigGrpRAMFields_u8;

} Com_RxSignalGrpFlagType_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx Signal Group dynamic configuration.\n
 *
 * typedef const Com_RxSignalGrpFlagType_tst * Com_RxSigGrpRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_RxSignalGrpFlagType_tst * Com_RxSigGrpRam_tpst;
#endif


#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_TYPES_H
 * This structure contains the Tx signal group flags \n
 *
 * typedef struct\n
 * {\n
 *      uint8   txSigGrpRAMFields_u8; \n
 * } Com_TxSignalGrpFlagType_tst;
 */
typedef struct
{
    uint8                                           txSigGrpRAMFields_u8;

} Com_TxSignalGrpFlagType_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx Signal Group dynamic configuration.\n
 *
 * typedef const Com_TxSignalGrpFlagType_tst * Com_TxSigGrpRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_TxSignalGrpFlagType_tst * Com_TxSigGrpRam_tpst;
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */
#endif

/**
 * @ingroup COM_PRV_TYPES_H
 * This is the Tx IPDU RAM structure \n
 *
 * typedef struct\n
 * {\n
 *      Com_TMConstPtrType_tpcst  currentTxModePtr_pcst;            Current Tx Mode state of the Tx IPDU\n
 *   #ifdef COM_TP_IPDUTYPE \n
 *      PduLengthType       txTPIPduLength_uo; \n                   Current Transmission TP Length\n
 *   #endif  #ifdef COM_TP_IPDUTYPE \n
 *      uint16              cntrMinDelayTime_u16;                   Minimum Delay Tick for the Tx IPDU\n
 *   #ifdef COM_TxFilters
 *      uint16              cntrTrueTMC_u16;                        Variable for keeping track of how many signals have TMS state as TRUE\n
 *   #endif
 *      uint16              cntrTimePeriod_u16;                     Tick for the Tx IPDU\n
 *      uint16              cntrRepPeriod_u16;                      For Mixed Mode this Tick is used if N>0\n
 *   #if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
 *       uint16             dynLength_u16;                          Variable to hold dynamic signal or group signal length
 *   #endif
 *   #ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
 *       uint16             dynTxGrpSigLength_u16;                  Variable to hold dynamic group signal length
 *   #endif
 *   #ifdef COM_TxIPduTimeOut\n
 *      uint16              cntrTxTimeout_u16;                      Tick for the Tx Confirmation\n
 *   #endif\n
 *      uint16              txFlags_u16;                            Variable to hold the Tx flags\n
 *      uint8               cntrRepetitions_u8;                     For Number of Repetition, this has to be updated only if
 *                                                                  there is a request from RTE (i.e ComSendSignal, Com_SendSignalGroup)\n
 *      uint8               transMode_u8;                           Variable which holds the LatestTransmissionMode and TxModeState of each signal\n
 * } Com_TxIpduRamData_tst;
 */
typedef struct
{
    Com_TMConstPtr_tpcst                            currentTxModePtr_pcst;
#ifdef COM_METADATA_SUPPORT
    Com_TxMetaData_tpcst                            txMetaData_pcst;
#endif
#ifdef COM_TP_IPDUTYPE
    PduLengthType                                   txTPIPduLength_uo;
#endif
    uint16                                          cntrMinDelayTime_u16;
    uint16                                          cntrTimePeriod_u16;
#ifdef COM_TxFilters
    uint16                                          cntrTrueTMC_u16;
#endif
    uint16                                          cntrRepPeriod_u16;
#if defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT)
    uint16                                          dynLength_u16;
#endif
#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    uint16                                          dynTxGrpSigLength_u16;
#endif
#ifdef COM_TxIPduTimeOut
    uint16                                          cntrTxTimeout_u16;
#endif
    /* FC_VariationPoint_START */
    /*
    typedef struct
    {
        uint16 PDUSTATUS:1;         Flag gives the old status of the Pdu Activation status
        uint16 CONFIR:1;            Used to check whether Confirmation for the Tx frame has been received or not
        uint16 TICKTXTO:1;          Flag which tells whether Timer was started/Stopped
        uint16 MDT:1;               Flag which tells whether Minimum delay time has not completed
        uint16 EVENTTRIG:1;         Flag which stores the information whether IPdu is transmitted in event triggered
                                        call from Com_SendSignal/Com_SendDynSignal/Com_SendSignalGroup/
                                        Com_SendSignalGroupArray/Com_TriggerIpduSend/Com_TriggerIpduSendWithMetadata/
                                        Com_SwitchIpduTxMode
        uint16 ISGWTRIGGRD:1;       Flag which stores if there was a Trigger on the Gw Pdu
        uint16 ISTOENABLED:1;       Flag which stores information if the Timeout timer has to be started for Ipdu.
        uint16 LARGEDATAINPROG:1;   Flag is set, if the Tp Pdu for large data pdu handling is in progress
        uint16 GWIPDUTMSCHANGED:1;  Flag is set, if the Gw-IPDU's Tranmission Mode has changed after its last
                                        transmission from Com
        uint16 RETRYFAILEDTXREQ:1;  Flag is set, if any failed Tx-Requests are present
        uint16 METADATA:1;          Flag remains set, until application provided metadata is transmitted to the
                                        lower layer(In case of ComTxIPduDeferredProcessing)
        uint16 MDTLOAD:1;           Flag which tells whether to load MDT/not to load MDT
        uint16 Not_Used:4;          Un-used bits
    } Com_TxFlagsType;
    */
    /* FC_VariationPoint_END */
    uint16                                         txFlags_u16;
    uint8                                          cntrRepetitions_u8;
    /* FC_VariationPoint_START */
    /*
    typedef struct
    {
        uint8 LatestTransMode:2;    Stores the Latest Transmission Mode of the IPDU
        uint8 TxModeState:1;        Stores the Transmission mode State i.e TRUE/FALSE
        uint8 MixedPhaseShift:1;    MixedPhaseShift status.If this is SET, then current mode is MixedPhaseShift
        uint8 Not_Used:4;           Not used Bit field
    }Com_TransModeType;
    */
    /* FC_VariationPoint_END */
    uint8                                           transMode_u8;

} Com_TxIpduRamData_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Tx IPDU dynamic configuration.\n
 *
 * typedef const Com_TxIpduRamData_tst * Com_TxIpduRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_TxIpduRamData_tst * Com_TxIpduRam_tpst;

/*END:  RAM structure for TX IPDU */

/**
 * @ingroup COM_PRV_TYPES_H
 * This is the Rx IPDU RAM structure \n
 *
 * typedef struct\n
 * {\n
 *      PduLengthType                               rxIPduLength_uo;
 *  #ifdef COM_TP_IPDUTYPE \n
 *      PduLengthType                               rxTPIPduLength_uo;      Current Reception Tp Length\n
 *  #endif \n
 *   #ifdef COM_RxIPduTimeout\n
 *      uint16                                      cntrRxTimeout_u16;            Counter for timeout\n
 *   # ifdef COM_RX_DUAL_CYCLE_SUPPPORT
 *      uint16                                      rxDualCycTimeout_u16;     Counter for dual-cycle timeout\n
 *   # endif\n
 *   #endif\n
 *      uint8                                       rxFlags_u8;             Variable to hold the Rx flags
 * } Com_RxIpduRamData_tst;
 */
typedef struct
{
    PduLengthType                                   rxIPduLength_uo;
#ifdef COM_TP_IPDUTYPE
    PduLengthType                                   rxTPIPduLength_uo;
#endif
#ifdef COM_RxIPduTimeout
    uint16                                          cntrRxTimeout_u16;
# ifdef COM_RX_DUAL_CYCLE_SUPPPORT
    uint16                                          rxDualCycTimeout_u16;
# endif
# ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    uint16                                          dynRxGrpSigLength_u16;
# endif
#endif /* #ifdef COM_RxIPduTimeout */

    /* FC_VariationPoint_START */
    /*
    typedef struct
    {
        uint8 PduStatus:1;              Flag gives the old status of the Pdu Activation status
        uint8 Pdu_DMStatus:1;           Flag used to Indicate Ipdu based reception deadline monitoring status
        uint8 RxIndicationFlag:1;       Flag used to Indicate that Ipdu was received
        uint8 IsRxGwIpduProcessed:1;    Flag used to Indicate if the Gw Ipdu is processed after its reception
        uint8 rxTPIPduFlag:1;           Flag used to Indicate the reception of TP Rx Ipdu
        uint8 IsFirstReception:1;       Flag used in case of first reception of IPdu configured with counter
        uint8 IsInvalid:1;              Flag used in case large-data received IPDU has to be considered as 'invalid'
        uint8 reloadRxTicks:1;          Flag used in case 'new' Rx IPDU deadline value is updated via
                                                Com_SetRxIPduTimeoutTicks

    } Com_RxFlagsType;
     */
    /* FC_VariationPoint_END */

    uint8                                           rxFlags_u8;

} Com_RxIpduRamData_tst;

/**
 * @ingroup COM_PRV_TYPES_H
 * This type defines a pointer type that points to the Rx IPDU dynamic configuration.\n
 *
 * typedef const Com_RxIpduRamData_tst * Com_RxIpduRam_tpst;\n
 */
/* The Below typedef can be used as local variable only */
typedef Com_RxIpduRamData_tst * Com_RxIpduRam_tpst;

typedef struct
{
    PduIdType *                                     rxGwQueuePtr_puo;
    Com_RxGwQueueIndex_tuo                          rxGwQueueWrite_uo;
    Com_RxGwQueueIndex_tuo                          rxGwQueueRead_uo;
} Com_RxGwQueueRAMType_tst;

/* The Below typedef can be used as local variable only */
typedef Com_RxGwQueueRAMType_tst * Com_RxGwQueueRam_tpst;

typedef struct
{
    Com_SignalIdType                                idxGwMapSigDestIdArray_u16;
    uint8                                           destCount_u8;
} Com_Prv_xGwMapSigCfg_tst;

/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xGwMapSigCfg_tst * Com_GwMapSigCfg_tpcst;

typedef struct
{
    Com_SignalIdType                                gwMapDestId_u16;
} Com_Prv_xGwMapSigIdCfg_tst;

typedef const Com_Prv_xGwMapSigIdCfg_tst * Com_GwMapSigIdCfg_tpcst;

#ifdef COM_TX_SIGNALGROUP
typedef struct
{
    Com_TxIntGrpSignalId_tuo                        idxGwMapGrpSigDestIdArray_uo;
    uint8                                           destCount_u8;
} Com_Prv_xGwMapGrpSigCfg_tst;

/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xGwMapGrpSigCfg_tst * Com_GwMapGrpSigCfg_tpcst;

typedef struct
{
    Com_TxIntGrpSignalId_tuo                        gwMapDestGrpSigId_uo;
} Com_Prv_xGwMapGrpSigIdCfg_tst;

/* The Below typedef can be used as local variable only */
typedef const Com_Prv_xGwMapGrpSigIdCfg_tst * Com_GwMapGrpSigIdCfg_tpcst;
#endif /* #ifdef COM_TX_SIGNALGROUP */

/**
 * @ingroup COM_H
 *
 * typedef struct \n
 * { \n
 *     Com_IpduId_tuo           idFirstIpdu_uo;     \n      Start IPduId for given MainFunction
 *     Com_NumOfIpdus_tuo       numOfIpdus_uo;      \n      Number of IPdus mapped for given MainFunction
 *     Com_TimeBase_tuo         timeBaseInMs_uo;    \n      TimeBase in milli seconds
 * } Com_MainFunctionCfgType_tst; \n
 */
typedef struct
{
    Com_IpduId_tuo                                  idFirstIpdu_uo;
    Com_NumOfIpdus_tuo                              numOfIpdus_uo;
    Com_TimeBase_tuo                                timeBaseInMs_uo;
} Com_MainFunctionCfgType_tst;

/**
 * Below structure shall be as an interface to access Rx-buffers generated per Rx-ComMainFunction.
 * RAM-Buffers are generated separately for each of the Rx-ComMainFunction.
 */
typedef struct
{
    uint8 *                                         sigType_pau8;

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
    uint8 *                                         sigTypeDyn_pau8;
#endif

    uint16 *                                        sigType_pau16;

    uint32 *                                        sigType_pau32;

#ifdef COM_RXSIG_INT64
    uint64 *                                        sigType_pau64;
#endif

#ifdef COM_RXSIG_FLOAT64SUPP
    float64 *                                       sigType_paf64;
#endif

#ifdef COM_RX_SIGNALGROUP
    uint8 *                                         rxGrpSigType_pau8;

#ifdef COM_RX_DYNAMIC_GROUPSIGNAL_SUPPORT
    uint8 *                                         rxGrpSigTypeDyn_pau8;
    uint8 *                                         secondRxGrpSigTypeDyn_pau8;
#endif
    uint16 *                                        rxGrpSigType_pau16;

    uint32 *                                        rxGrpSigType_pau32;

#ifdef COM_RXGRPSIG_INT64
    uint64 *                                        rxGrpSigType_pau64;
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
    float64 *                                       rxGrpSigType_paf64;
#endif

    uint8 *                                         secondRxGrpSigType_pau8;

    uint16 *                                        secondRxGrpSigType_pau16;

    uint32 *                                        secondRxGrpSigType_pau32;

#ifdef COM_RXGRPSIG_INT64
    uint64 *                                        secondRxGrpSigType_pau64;
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
    float64 *                                       secondRxGrpSigType_paf64;
#endif

#ifdef COM_RX_SIGNALGROUP_ARRAY
    uint8 *                                         rxSigGrpArrayBuf_pau8;
#endif

#endif/* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_RxIPduDeferredProcessing
    uint8 *                                         deferredBuffer_pau8;
#endif

} Com_Prv_xRxRamBuf_tst;

/**
 * Below structure shall be used as an interface to access Tx-buffers generated per Tx-ComMainFunction.
 * RAM-Buffers are generated separately for each of the Tx-ComMainFunction.
 */
#ifdef COM_TX_SIGNALGROUP
typedef struct
{
    uint8 *                                         txGrpSigType_pau8;

#ifdef COM_TX_DYNAMIC_GROUPSIGNAL_SUPPORT
    uint8 *                                         txGrpSigTypeDyn_pau8;
#endif
    uint16 *                                        txGrpSigType_pau16;

    uint32 *                                        txGrpSigType_pau32;

#ifdef COM_TXGRPSIG_INT64
    uint64 *                                        txGrpSigType_pau64;
#endif

#ifdef COM_TXGRPSIG_FLOAT64SUPP
    float64 *                                       txGrpSigType_paf64;
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */

} Com_Prv_xTxSigGrpRamBuf_tst;
#endif/* #ifdef COM_TX_SIGNALGROUP */

/**
 * Below structure shall be used as an interface to Com Post-Build data structures
 */
typedef struct
{
    const Com_Prv_xTxSigCfg_tst *                   txSigCfg_pcast;

    const Com_SignalIdType *                        txSignalMappingCfg_pcau16;

    const Com_Prv_xRxSigCfg_tst *                   rxSigCfg_pcast;

    const Com_SignalIdType *                        rxSignalMappingCfg_pcau16;

#ifdef COM_TX_SIGNALGROUP
    const Com_Prv_xTxSigGrpCfg_tst *                txSigGrpCfg_pcast;

    const Com_SignalGroupIdType *                   txSignalGrpMappingCfg_pcau16;

#endif

#ifdef  COM_RX_SIGNALGROUP
    const Com_Prv_xRxSigGrpCfg_tst *                rxSigGrpCfg_pcast;

    const Com_SignalGroupIdType *                   rxSignalGrpMappingCfg_pcau16;

#endif

#ifdef COM_TX_SIGNALGROUP
    const Com_Prv_xTxGrpSigCfg_tst *                txGrpSigCfg_pcast;

#endif /* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_RX_SIGNALGROUP
    const Com_Prv_xRxGrpSigCfg_tst *                rxGrpSigCfg_pcast;

#endif /* #ifdef  COM_RX_SIGNALGROUP */

    const Com_Prv_xTxIpduInfoCfg_tst *              txIpduCfg_pcast;

    const PduIdType *                               txIpduMappingCfg_pcauo;

    const Com_Prv_xRxIpduInfoCfg_tst *              rxIpduCfg_pcast;

    const PduIdType *                               rxIpduMappingCfg_pcauo;

    const Com_Prv_xIpduGrpInfoCfg_tst *             ipduGrpCfg_pcast;

    const uint16 *                                  ipduGrpMappingCfg_pcau16;

#ifdef COM_SIGNALGATEWAY
    const Com_Prv_xGwMapSigCfg_tst *                gwSigCfg_pcast;

    const Com_Prv_xGwMapSigIdCfg_tst *              gwSigIdMappingCfg_pcast;
#endif

#ifdef COM_SIGNALGROUPGATEWAY
    const Com_Prv_xGwMapGrpSigCfg_tst *             gwGrpSigCfg_pcast;

    const Com_Prv_xGwMapGrpSigIdCfg_tst *           gwGrpSigIdMappingCfg_pcast;
#endif

    const Com_IpduId_tuo *                          ipduGrpIpduIdCfg_pcauo;

#if defined ( COM_TX_SIGNALGROUP_ARRAY )
    Com_TxSigGrpArrayCfg_tpcst                      txSigGrpArrayCfg_pcast;
#endif

#if defined ( COM_RX_SIGNALGROUP_ARRAY )
    Com_RxSigGrpArrayCfg_tpcst                      rxSigGrpArrayCfg_pcast;
#endif

#if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX)
    const Com_MaskX_tst *                           maskXCfg_pcast;
#endif /* #if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX) */

#ifdef COM_F_MASKEDNEWDIFFERSOLD
    const uint32 *                                  maskCfg_pcau32;
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)
    const Com_POSMinMax_tst *                       posMinMaxCfg_pcast;
#endif /* #if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS) */

#if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)
    const Com_NEGMinMax_tst *                       negMinMaxCfg_pcast;
#endif /* #if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG) */

    const Com_MainFunctionCfgType_tst *             mainFunctionCfg_pcast;

#ifdef COM_TX_IPDUCOUNTER
    const Com_Prv_xTxIpduCntrCfg_tst *              txIpduCntrCfg_pcast;
#endif

#ifdef COM_RX_IPDUCOUNTER
    const Com_Prv_xRxIpduCntrCfg_tst *              rxIpduCntrCfg_pcast;
#endif

#ifdef COM_F_ONEEVERYN
    const Com_OneEveryN_tst *                       oneEveryNCfg_pcast;
#endif

    uint16                                          numOfIpduGroup_u16;

    /* Total number of Pdus mapped to the last IPduGroup member in the generated table - Com_Prv_xIpduGrpCfg_acst */
    Com_IpduId_tuo                                  numOfIpdusInLastIpduGrp_uo;

    Com_SignalIdType                                numOfTxSignals_u16;

    Com_SignalIdType                                numOfRxSignals_u16;

    Com_SignalGroupIdType                           numOfTxSignalGroup_u16;

    Com_SignalGroupIdType                           numOfRxSignalGroup_u16;

    Com_GrpSignalId_tuo                             numOfTxGroupSignal_uo;

    Com_GrpSignalId_tuo                             numOfRxGroupSignal_uo;

    Com_IpduId_tuo                                  numOfTxIpdu_uo;

    Com_IpduId_tuo                                  numOfRxIpdu_uo;

    Com_IpduId_tuo                                  numOfGwSrcIpdu_uo;

    Com_GrpSignalId_tuo                             numOfGrpSigNoGw_uo;

    Com_SignalIdType                                numOfTxSignalsAndGrpSignals_u16;

    Com_SignalIdType                                numOfRxSignalsAndGrpSignals_u16;

#ifdef COM_F_ONEEVERYN
    uint8                                           numOfOneEveryNSigAndGrpSig_u8;
#endif /* #ifdef COM_F_ONEEVERYN */

} Com_ConfigData_tst;


typedef struct
{
#if defined(COM_TXSIG_FLOAT64SUPP ) || defined(COM_TXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXSIG_FLOAT64SUPP)
    const float64 *                                 floatValArrayCfg_pcaf64;
#endif

    const Com_Prv_xRxRamBuf_tst *                   rxRamBufCfg_pcast;

#ifdef COM_TX_SIGNALGROUP
    const Com_Prv_xTxSigGrpRamBuf_tst *             txSigGrpRamBufCfg_pcast;
#endif

#ifdef COM_INITVALOPTIMIZATION
    const uint32 *                                  uniqueInitVal_pcau32;
#endif

#ifdef COM_F_ONEEVERYN
    Com_OneEveryN_tst *                             oneEveryN_past;
#endif

#ifdef COM_F_MASKEDNEWDIFFERSOLD
    uint32 *                                        filterOldVal_pau32;
#endif

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)
    Com_OldValTrigOnChng_tauo *                     oldValTrigOnChng_pauo;
#endif /* #if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC) */

#ifdef COM_TP_IPDUTYPE
    /* PduLengthType can be u8, u16 or u32, hence the type is name as opaque type */
    PduLengthType *                                 tpTxIpduLength_pauo;
#endif

    Com_TxIpduRamData_tst *                         txIpduRam_past;

    Com_RxIpduRamData_tst *                         rxIpduRam_past;

    Com_TxSignalFlagType_tst *                      txSignalFlag_past;

    Com_RxSignalFlagType_tst *                      rxSignalFlag_past;

    uint8 *                                         ipduCounter_pau8;

# if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
    uint8 *                                         ipduCounter_DM_pau8;
# endif

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    PduIdType *                                     rxGwQueue_pauo;

    Com_RxGwQueueRAMType_tst *                      rxGwQueue_pst;
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_RX_SIGNALGROUP
    Com_RxSignalGrpFlagType_tst *                   rxSignalGrpFlag_past;
#endif/* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)
    Com_TxSignalGrpFlagType_tst *                   txSignalGrpFlag_past;
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */

    Com_TxGrpSignalFlagType_tst *                   txGrpSignalFlag_past;
#endif /* #ifdef COM_TX_SIGNALGROUP */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    boolean *                                       isGwSignalsUpdated_pab;
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_TX_IPDUCOUNTER
    uint8 *                                         txIpduCntrVal_pau8;
#endif

#ifdef COM_RX_IPDUCOUNTER
    uint8 *                                         rxIpduLastCntrVal_pau8;
#endif

    uint16                                          numOfRxIpdusInAllVariants_u16;
    uint16                                          numOfRxSignalsInAllVariants_u16;
#ifdef COM_RX_SIGNALGROUP
    uint16                                          numOfRxSigGroupsInAllVariants_u16;
    uint16                                          numOfRxGrpSignalsInAllVariants_u16;
#endif

    uint16                                          numOfTxIpdusInAllVariants_u16;
    uint16                                          numOfTxSignalsInAllVariants_u16;
#ifdef COM_TX_SIGNALGROUP
    uint16                                          numOfTxSigGroupsInAllVariants_u16;
    uint16                                          numOfTxGrpSignalsInAllVariants_u16;
#endif

    uint16                                          numOfRxSig_GrpSigInAllVariants_u16;
    uint16                                          numOfTxSig_GrpSigInAllVariants_u16;
    uint16                                          numOfIPduGroupsInAllVariants_u16;

} Com_CommonData_tst;


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/


#endif /* End of COM_PRV_TYPES_H */

