



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

/*0:CANNODE_0 */
{ /*(4)*/0,   /*(5)*/CanConf_CanController_ETAS_CAN,   /*(6)*/FALSE  }
};





#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


static const CanIf_Cfg_HthConfig_tst CanIf_HthGen_a[]=
{
/*,  *CanIf_CtrlConfigPtr,  CanObjectId,   CanHandleType */

/*0:CANNODE_0_Tx_Std_MailBox_1*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_CANNODE_0], /*(1)*/ CanConf_CanHardwareObject_CANNODE_0_Tx_Std_MailBox_1, }



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

/*0:Buffer_CANNODE_0_Tx_Std_MailBox_1*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_CANNODE_0_Tx_Std_MailBox_1]}
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

/*0:UDS_Tx_Phys_Id_0x7B7_CANNODE_0*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Buffer_CANNODE_0_Tx_Std_MailBox_1],    /*TxPduId*/CanIfConf_CanIfTxPduCfg_UDS_Tx_Phys_Id_0x7B7_CANNODE_0, /*TxPduTargetPduId*/ CanTpConf_CanTpTxNPdu_Phys_CanTp2CanIf , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/CAN_TP,      /*UserTxConfirmation*/&CanTp_TxConfirmation, /*TxPduCanId*/ 0x748   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ TRUE ,/*TxPduLength*/8u}
};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
/*=====================================================================================================================
 *=====================================================================================================================
 *=====================================================================================================================
 */











#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static const CanIf_Cfg_Hrhtype_tst CanIf_Prv_HrhConfig_tacst[2] =
{
          /*HrhInfo_e,    PduIdx_t,  NumRxPdus_u32,  HrhRangeMask_b,  ControllerId_u8 , CanId_t*/
    
/*0: CANNODE_0_Rx_Std_MailBox_1*/
    {CANIF_PRV_FULL_E,       0u,             1u,            FALSE,           0u    ,   1856u       },
/*1: CANNODE_0_Rx_Std_MailBox_2*/
    {CANIF_PRV_FULL_E,       1u,             1u,            FALSE,           0u    ,   2015u       }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"







#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static const CanIf_Cfg_RxPduType_tst CanIf_Prv_RxPduConfig_tacst[2]=
{   
   /*RxPduReadNotifyReadDataStatus_u8       IndexForUL_u8   CanIdtype_u8    RxPduDlc_u8      RxPduCanId      Hrhref_t    RxPduTargetId_t 	*/
    
    /*0:UDS_Rx_Phys_Id_0x737_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     2u,           0x20u,            1u,         1856u,       0,         CanTpConf_CanTpRxNPdu_Phys_CanIf2CanTp            },
    /*1:UDS_Rx_Func_Id_0x7DF_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     2u,           0x20u,            1u,         2015u,       1,         CanTpConf_CanTpRxNPdu_Func_CanIf2CanTP            }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



















#define CANIF_START_SEC_CONST_16
#include "CanIf_MemMap.h"

/* Array for mapping Hoh Id(CanObjectId) and Hrh */
static const uint16 CanIf_CFG_HrhIdMapping_au16[] =
{/*0:CANNODE_0_Rx_Std_MailBox_1*/
0,
/*1:CANNODE_0_Rx_Std_MailBox_2*/
1,
/*2:CANNODE_0_Tx_Std_MailBox_1*/
CANIF_INVALID_ID};


#define CANIF_STOP_SEC_CONST_16
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CONST_16
#include "CanIf_MemMap.h"


/* Array for mapping CanIfRxpduId accross the variant */
static const uint16 CanIf_CFG_RxPduIdMapping_au16[] =
{
/*0:UDS_Rx_Phys_Id_0x737_CANNODE_0*/
0,
/*1:UDS_Rx_Func_Id_0x7DF_CANNODE_0*/
1};



#define CANIF_STOP_SEC_CONST_16
#include "CanIf_MemMap.h"










/* array of function pointer which provies Callback name to the UL*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static const CanIf_RxCbk_Prototype CanIf_Prv_ULName_ta__fct[] =
{
    {NULL_PTR},
    {NULL_PTR},
    {&CanTp_RxIndication},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    
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
      
/*UDS_Tx_Phys_Id_0x7B7_CANNODE_0*/
0 
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

/*CANNODE_0*/
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
   2u,
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
   1,      /*NumOfTxPdus*/
   1,        /*NumOfTxBuffers*/  
  &CanIf_TxPduId_MappingTable[0],      /*TxPduIdTable_Ptr*/
   &CanIf_CtrlId_MappingTable[0],
   /*RxAutosarUL_Ptr*/
   &CanIf_Prv_ULName_ta__fct[0] /* FC_VariationPoint_END */
     
};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"









