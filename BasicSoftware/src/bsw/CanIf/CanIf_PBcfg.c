



/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanIf / AR42.10.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 9.2.1_9.2.1                Tool Version
 * $Model______: 2.0.2.1                ECU Parameter Definition Version
 *

 </VersionHead>*/


/******************************************************************************/
/*                                  Include Section                                */
/******************************************************************************/
/* CanIf Private header */
#include "CanIf_Prv.h"



/*
 ******************************************************************************
 * Variables
 ******************************************************************************
 */









/*======================================================================================================================
 *                                VARIANT:  PRE_COMPILE
 *======================================================================================================================
*/







/*====================================================================================================================*/
/*
 *                                  CANIF CONTROLLER CONFIG STRUCTURE             
 *                                     
 * Structure contains following members:
 *{
 *#if(CANIF_PUBLIC_TXBUFFERING ==STD_ON) 
 *  (0) BufferIdPtr,
 *  (1) TotalBufferCount,
 *  (2) TxPduIdPtr,
 *  (3) TotalTxPduCount,
 *#endif
 *  (4) CtrlId,
 *  (5) CtrlCanCtrlRef,
 *  (6) CtrlWakeupSupport,
 *#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
 *  (7) PnCtrlEn            
 *#endif
 * #if CANIF_CFG_PUBLIC_MULTIPLE_DRIVER_SUPPORT == STD_ON
 * CanDrvIndx
 * #endif
 *}
*/


#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static const CanIf_Cfg_CtrlConfig_tst CanIf_CtrlGen_a[]=
{

/*0:ETAS_CAN */
{ /*(4)*/0,   /*(5)*/CanConf_CanController_ETAS_CAN,   /*(6)*/FALSE  }
};





#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


static const CanIf_Cfg_HthConfig_tst CanIf_HthGen_a[]=
{
/*,  *CanIf_CtrlConfigPtr,  CanObjectId,   CanHandleType */

/*0:ETAS_CAN_Tx_Std_MailBox_1*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_1, }

,
/*1:ETAS_CAN_Tx_Std_MailBox_2*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_2, }

,
/*2:ETAS_CAN_Tx_Std_MailBox_3*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_3, }

,
/*3:ETAS_CAN_Tx_Std_MailBox_4*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_4, }

,
/*4:ETAS_CAN_Tx_Std_MailBox_5*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_5, }

,
/*5:ETAS_CAN_Tx_Std_MailBox_6*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_ETAS_CAN], /*(1)*/ CanConf_ETAS_CAN_Tx_Std_MailBox_6, }



};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



/*                                  CANIF TXBUFFER CONFIG STRUCTURE       

 * Structure contains following members:
 *{
 * (0)*CanIf_HthConfigPtr
 *#if (CANIF_PUBLIC_TXBUFFERING == STD_ON)
 * (1)DataBuf
 * (2)CanIdBuf
 * (3)CanIfBufferId;
 * (4)CanIfBufferSize
 * (5)CanIfBufferMaxDataLength 
 *#endif
 *}
 */     

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
                                            
static const CanIf_Cfg_TxBufferConfig_tst CanIf_TxBufferGen_a[]=
{

/*0:Bu_ETAS_CAN_Tx_Std_MailBox_1*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_1]}
,
/*1:Bu_ETAS_CAN_Tx_Std_MailBox_2*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_2]}
,
/*2:Bu_ETAS_CAN_Tx_Std_MailBox_3*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_3]}
,
/*3:Bu_ETAS_CAN_Tx_Std_MailBox_4*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_4]}
,
/*4:Bu_ETAS_CAN_Tx_Std_MailBox_5*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_5]}
,
/*5:Bu_ETAS_CAN_Tx_Std_MailBox_6*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_ETAS_CAN_Tx_Std_MailBox_6]}
};

#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



/*                                  CANIF TXPDU CONFIG STRUCTURE       
 * Structure contains following members:
 *{
 * (0)*CanIf_TxBufferConfigPt
 *#if(CANIF_METADATA_SUPPORT == STD_ON)
 * (1)TxPduCanIdMask
 * (2)MetaDataLength          
 *#endif
 * (3)TxPduId
 * (4)TxPduTargetPduId       
 * (5)TxPduType                
 * (6)TxPduCanIdType
 * (7)TxPduTxUserUL
 * (8)Function pointer to TxPduTxUserULName
 * (9)TxPduCanId
 *
 *#if(CANIF_RB_NODE_CALIBRATION == STD_ON)
 *(10) getTxPduCanId
 *#endif
 *
 *#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
 * (11)TxPduPnFilterPdu
 *#endif
 *
 *#if (CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON)
 * (12)TxPduReadNotifyStatus
 *#endif
 *
 *#if(CANIF_TRIGGERTRANSMIT_SUPPORT== STD_ON)
 * (13)Function pointer to UserTriggerTransmitName
 * (14)TxPduTriggerTransmit
 *#endif
 *
 *#if(CANIF_RB_NODE_CALIBRATION == STD_ON)
 *(15)getTxPduDlc
 *#endif
 *
 *(16)TxTruncEnabled
 *(17)TxPduLength
 *}
 */  


#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
                              

static const CanIf_Cfg_TxPduConfig_tst CanIf_TxPduGen_a[]=
{

/*0:ComTx_0x123_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_1],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_ComTx_0x123_ETAS_CAN_OUT, /*TxPduTargetPduId*/ 0 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/PDUR,      /*UserTxConfirmation*/&PduR_CanIfTxConfirmation, /*TxPduCanId*/ 0x123   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
,
/*1:ComTx_0x55_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_2],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_ComTx_0x55_ETAS_CAN_OUT, /*TxPduTargetPduId*/ 1 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/PDUR,      /*UserTxConfirmation*/&PduR_CanIfTxConfirmation, /*TxPduCanId*/ 0x55   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/6u}
,
/*2:ComTx_0x98_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_3],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_ComTx_0x98_ETAS_CAN_OUT, /*TxPduTargetPduId*/ 2 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/PDUR,      /*UserTxConfirmation*/&PduR_CanIfTxConfirmation, /*TxPduCanId*/ 0x98   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/7u}
,
/*3:NM_Tx_0x600_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_4],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_NM_Tx_0x600_ETAS_CAN_OUT, /*TxPduTargetPduId*/ 0 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/CAN_NM,      /*UserTxConfirmation*/&CanNm_TxConfirmation, /*TxPduCanId*/ 0x600   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
,
/*4:NPdu_UdsTxPhy_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_5],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_NPdu_UdsTxPhy_ETAS_CAN_OUT, /*TxPduTargetPduId*/ CanTpConf_CanTpTxNPdu_NPdu_UdsTxPhy_CanTp2CanIf_Can_Network_ETAS_Channel_CAN , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/CAN_TP,      /*UserTxConfirmation*/&CanTp_TxConfirmation, /*TxPduCanId*/ 0x748   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
,
/*5:XcpTx_0x333_ETAS_CAN_OUT*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Bu_ETAS_CAN_Tx_Std_MailBox_6],     /*TxPduCanIdMask*/0x1FFFFFFF,  /*MetaDataLength*/0,   /*TxPduId*/CanIfConf_CanIfTxPduCfg_XcpTx_0x333_ETAS_CAN_OUT, /*TxPduTargetPduId*/ 0 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/XCP,      /*UserTxConfirmation*/&Xcp_CanIfTxConfirmation, /*TxPduCanId*/ 0x333   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
/*=====================================================================================================================
 *=====================================================================================================================
 *=====================================================================================================================
 */











#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static const CanIf_Cfg_Hrhtype_tst CanIf_Prv_HrhConfig_tacst[7] =
{
          /*HrhInfo_e,    PduIdx_t,  NumRxPdus_u32,  HrhRangeMask_b,  ControllerId_u8 , CanId_t*/
    
/*0: ETAS_CAN_Rx_Std_MailBox_1*/
    {CANIF_PRV_FULL_E,       0u,             1u,            FALSE,           0u    ,   609u       },
/*1: ETAS_CAN_Rx_Std_MailBox_2*/
    {CANIF_PRV_FULL_E,       1u,             1u,            FALSE,           0u    ,   129u       },
/*2: ETAS_CAN_Rx_Std_MailBox_3*/
    {CANIF_PRV_FULL_E,       2u,             1u,            FALSE,           0u    ,   1655u       },
/*3: ETAS_CAN_Rx_Std_MailBox_4*/
    {CANIF_PRV_FULL_E,       3u,             1u,            FALSE,           0u    ,   818u       },
/*4: ETAS_CAN_Rx_Std_MailBox_5*/
    {CANIF_PRV_FULL_E,       4u,             1u,            FALSE,           0u    ,   2015u       },
/*5: ETAS_CAN_Rx_Std_MailBox_6*/
    {CANIF_PRV_FULL_E,       5u,             1u,            FALSE,           0u    ,   1856u       },
/*6: ETAS_CAN_Rx_Std_MailBox_7*/
    {CANIF_PRV_FULL_E,       6u,             1u,            FALSE,           0u    ,   1110u       }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"







#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static const CanIf_Cfg_RxPduType_tst CanIf_Prv_RxPduConfig_tacst[7]=
{   
   /*RxPduReadNotifyReadDataStatus_u8       IndexForUL_u8   CanIdtype_u8    RxPduDlc_u8  RxPduCanIdMask_u32    
  MetadataLength_u8    RxPduCanId      Hrhref_t    RxPduTargetId_t 	*/
    
    /*0:ComRx_0x261_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     6u,           0x20u,            1u,        536870911u,      
        0,                 609u,       0,         PduRConf_PduRSrcPdu_ComRx_0x261_CanIf2PduR_Can_Network_ETAS_Channel_CAN            },
    /*1:ComRx_0x81_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     6u,           0x20u,            8u,        536870911u,      
        0,                 129u,       1,         PduRConf_PduRSrcPdu_ComRx_0x81_CanIf2PduR_Can_Network_ETAS_Channel_CAN            },
    /*2:NM_Rx_0x677_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     1u,           0x20u,            8u,        536870911u,      
        0,                 1655u,       2,         0            },
    /*3:XcpRx_0x332_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     7u,           0x30u,            0u,        536870911u,      
        0,                 818u,       3,         XcpConf_XcpRxPdu_XcpRx_0x332            },
    /*4:NPdu_UdsRxFnc_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     2u,           0x20u,            0u,        536870911u,      
        0,                 2015u,       4,         CanTpConf_CanTpRxNPdu_NPdu_UdsRxFnc_CanIf2CanTp_Can_Network_ETAS_Channel_CAN            },
    /*5:NPdu_UdsRxPhy_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     2u,           0x20u,            0u,        536870911u,      
        0,                 1856u,       5,         CanTpConf_CanTpRxNPdu_NPdu_UdsRxPhy_CanIf2CanTp_Can_Network_ETAS_Channel_CAN            },
    /*6:ComRx_0x456_ETAS_CAN_IN*/
    {
        CANIF_READ_NOTIFSTATUS,     6u,           0x20u,            8u,        536870911u,      
        0,                 1110u,       6,         PduRConf_PduRSrcPdu_ComRx_0x456_CanIf2PduR_Can_Network_ETAS_Channel_CAN            }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
























#define CANIF_START_SEC_CONST_16
#include "CanIf_MemMap.h"

/* Array for mapping Hoh Id(CanObjectId) and Hrh */
static const uint16 CanIf_CFG_HrhIdMapping_au16[] =
{/*0:ETAS_CAN_Rx_Std_MailBox_1*/
0,
/*1:ETAS_CAN_Rx_Std_MailBox_2*/
1,
/*2:ETAS_CAN_Rx_Std_MailBox_3*/
2,
/*3:ETAS_CAN_Rx_Std_MailBox_4*/
3,
/*4:ETAS_CAN_Rx_Std_MailBox_5*/
4,
/*5:ETAS_CAN_Rx_Std_MailBox_6*/
5,
/*6:ETAS_CAN_Rx_Std_MailBox_7*/
6,
/*7:ETAS_CAN_Tx_Std_MailBox_1*/
CANIF_INVALID_ID,
/*8:ETAS_CAN_Tx_Std_MailBox_2*/
CANIF_INVALID_ID,
/*9:ETAS_CAN_Tx_Std_MailBox_3*/
CANIF_INVALID_ID,
/*10:ETAS_CAN_Tx_Std_MailBox_4*/
CANIF_INVALID_ID,
/*11:ETAS_CAN_Tx_Std_MailBox_5*/
CANIF_INVALID_ID,
/*12:ETAS_CAN_Tx_Std_MailBox_6*/
CANIF_INVALID_ID};


#define CANIF_STOP_SEC_CONST_16
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_16
#include "CanIf_MemMap.h"


/* Array for mapping CanIfRxpduId accross the variant */
static const uint16 CanIf_CFG_RxPduIdMapping_au16[] =
{
/*0:ComRx_0x261_ETAS_CAN_IN*/
0,
/*1:ComRx_0x81_ETAS_CAN_IN*/
1,
/*2:NM_Rx_0x677_ETAS_CAN_IN*/
2,
/*3:XcpRx_0x332_ETAS_CAN_IN*/
3,
/*4:NPdu_UdsRxFnc_ETAS_CAN_IN*/
4,
/*5:NPdu_UdsRxPhy_ETAS_CAN_IN*/
5,
/*6:ComRx_0x456_ETAS_CAN_IN*/
6};



#define CANIF_STOP_SEC_CONST_16
#include "CanIf_MemMap.h"










/* array of function pointer which provies Callback name to the UL*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static const CanIf_RxCbk_Prototype CanIf_Prv_ULName_ta__fct[] =
{
    {NULL_PTR},
    {&CanNm_RxIndication},
    {&CanTp_RxIndication},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    {&PduR_CanIfRxIndication},
    {&Xcp_CanIfRxIndication},
    
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"










/* CANIF callback configuration */

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
const CanIf_CallbackFuncType CanIf_Callback =
{
   
    /*5: User_ControllerBusOff */
    &CanSM_ControllerBusOff,
    
    /* 6:User_ControllerModeIndication */
    &CanSM_ControllerModeIndication,
    /*7:User_ControllerErrorPassive*/
    
    
};

#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"









/*This mapping table is generated for finding invalid TxPduIs passed via CanIf APIs in Post-Build.
 *Size of the array is total number of Tx PDUs across the variants. Each element is the index of Tx Pdu config structure.
 * If a TxPdu is not present in this variant, an invalid value 0xFFFF is generated in the particular position.
 */
 
 #define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



static const uint16 CanIf_TxPduId_MappingTable[] = 
{
      
/*ComTx_0x123_ETAS_CAN_OUT*/
0,	      
/*ComTx_0x55_ETAS_CAN_OUT*/
1,	      
/*ComTx_0x98_ETAS_CAN_OUT*/
2,	      
/*NM_Tx_0x600_ETAS_CAN_OUT*/
3,	      
/*NPdu_UdsTxPhy_ETAS_CAN_OUT*/
4,	      
/*XcpTx_0x333_ETAS_CAN_OUT*/
5 
};  







#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/*This mapping table is generated for finding invalid CtrlIds passed via CanIf APIs in Post-Build.
 *Size of the array is total number of controllers across the variants. Each element is the index of Controller config structure.
 * If a CtrlId is not present in this variant, an invalid value 0xFF is generated in the particular position.
 */

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



static const uint8 CanIf_CtrlId_MappingTable[] = 
{

/*ETAS_CAN*/
0 
};  






#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



/* FC_VariationPoint_START */

/* FC_VariationPoint_END */


/*Configuration structure for __KW_COMMON*/


#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

const CanIf_ConfigType CanIf_Config =
{
    
    /* HrhConfig_pcst */
    CanIf_Prv_HrhConfig_tacst,
    /* RxPduConfig_pcst */
    CanIf_Prv_RxPduConfig_tacst,
   /* NumCanRxPduId_t */
   7u,
   /*NumCanCtrl_u8*/
   1,
   /*NumCddRxPdus_t*/
   
   0,
   /*RxPduIdTable_Ptr*/
   &CanIf_CFG_RxPduIdMapping_au16[0],
   /*HrhPduIdTable_Ptr*/
   &CanIf_CFG_HrhIdMapping_au16[0],
   /*CfgSetIndex_u8*/
   0,
   &CanIf_TxPduGen_a[0u],             /*CanIf_TxPduConfigPtr*/
   &CanIf_TxBufferGen_a[0u],          /*CanIf_TxBufferConfigPtr*/
   &CanIf_CtrlGen_a[0u],              /*CanIf_CtrlConfigPtr*/
   6,      /*NumOfTxPdus*/
   6,        /*NumOfTxBuffers*/  
  &CanIf_TxPduId_MappingTable[0],      /*TxPduIdTable_Ptr*/
   &CanIf_CtrlId_MappingTable[0],
   /*RxAutosarUL_Ptr*/
   &CanIf_Prv_ULName_ta__fct[0] /* FC_VariationPoint_END */
     
};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"









