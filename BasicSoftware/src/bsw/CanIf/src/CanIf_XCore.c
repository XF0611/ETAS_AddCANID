


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"
#include "rba_BswSrv.h" /*for memcpy*/

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

/*
 ***************************************************************************************************
 * Global Variables/Arrays
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Static functions Declaration
 ***************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static Std_ReturnType CanIf_XCore_Pipe_PushPduHdrToFifo(CanIf_XCore_PipeRuntimeInfoType_st *CanIf_XCore_Pipe_pst, CanIf_XCore_PipeTxRxParams_st* params_pst,
														uint32** PayloadPtr_ppu32, uint32* PayloadLenU32W_pu32);

static void CanIf_XCore_Pipe_Init(uint16 PipeId, const CanIf_XCore_ConfigType *Config_pcst);

static void CanIf_XCore_Pipe_Reset(uint16 PipeId);

static void CanIf_XCore_ClearPipepacket( uint16 PipeId, uint8 ControllerId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Init
 * Syntax           : void CanIf_XCore_Init(const CanIf_XCore_ConfigType * Config_pcst)
 * Description      : This service Initializes global variables and data structures  of the PIPE objects of the
 *                    CanIf XCore for the further processing.
 * Parameter        : Config_pcst
 * Return value     : None
 **********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_XCore_Init(const CanIf_XCore_ConfigType * Config_pcst)
{
    uint16 pipeIdx_u32;

    /*Set the variant pointer*/
    #if CANIF_CONFIGURATION_VARIANT == CANIF_CFG_VAR_PC
    CanIf_XCore_Config_pcst = &CanIf_XCore_Config;
    (void) Config_pcst;
    #else
    CanIf_XCore_Config_pcst = Config_pcst;
    #endif

	for (pipeIdx_u32 = 0; pipeIdx_u32 < CanIf_XCore_Config_pcst->NumPipes_u16; pipeIdx_u32++)
	{
		CanIf_XCore_Pipe_Init(pipeIdx_u32, CanIf_XCore_Config_pcst);
	}
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_CalcAlignSizeU32W
 * Syntax           : static uint32 CanIf_XCore_Pipe_CalcAlignSizeU32W(uint32 LengthBytes_u32)
 * Description      : This service calculate the payload size
 *                    Size in uint32 = the amount of uint32 words that accomodates the given PDU
 * Parameter        : LengthBytes_u32
 * Return value     : uint32
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static uint32 CanIf_XCore_Pipe_CalcAlignSizeU32W(uint32 LengthBytes_u32)
{
    uint32 size_u32 = (LengthBytes_u32 + (sizeof(uint32) - 1u)) / sizeof(uint32);
    return size_u32;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"



/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_Init
 * Syntax           : static void CanIf_XCore_Pipe_Init(uint16 PipeId, const CanIf_XCore_ConfigType * Config_pcst)
 * Description      : This service initializes the PIPE object.
 * Parameter        : PipeId, ConfigPtr
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static void CanIf_XCore_Pipe_Init(uint16 PipeId, const CanIf_XCore_ConfigType * Config_pcst)
{
    /* Pointer to the Pipe object */
    CanIf_XCore_PipeRuntimeInfoType_st * CanIf_XCore_Pipe_pst;

    /* Get the Pipe Object info pointer */
    CanIf_XCore_Pipe_pst = &CanIf_XCore_PipeRuntimeInfo_ast[PipeId];

    CanIf_XCore_Pipe_pst->PipeConfig_pcst = &(Config_pcst->PipeConfigs_ast[PipeId]);

    /* Get the base address of the FIFO RAM */
    CanIf_XCore_Pipe_pst->FifoBaseAddr_pv = CanIf_XCore_Pipe_pst->PipeConfig_pcst->PipeFifoRam_pv;

    /* Compute FIFO end addr: EndAddr = Start + FifoSize - GuardBuffer*/
    /* GuardBuffer: Space for one packet header at the end of FIFO RAM in order to ensure sufficent space for at least one invalid PDU    */
    /* MR12 RULE 11.5 VIOLATION : Warning is "Not Critical".'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
    CanIf_XCore_Pipe_pst->FifoEndAddr_pv = ((uint8*)CanIf_XCore_Pipe_pst->FifoBaseAddr_pv) + CanIf_XCore_Pipe_pst->PipeConfig_pcst->FifoRamSizeBytes_u32 - sizeof(CanIf_XCore_PipePktHdrType_st);

    /* Pipe Reset */
    CanIf_XCore_Pipe_Reset(PipeId);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_Reset
 * Syntax           : static void CanIf_XCore_Pipe_Init(uint16 PipeId)
 * Description      : This service resets the state of the given pipe to the initial state..
 * Parameter        : PipeId
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static void CanIf_XCore_Pipe_Reset(uint16 PipeId)
{
    /* Pointer to the Pipe object */
    CanIf_XCore_PipeRuntimeInfoType_st * CanIf_XCore_Pipe_pst;

    /* Get the Pipe Object info pointer */
    CanIf_XCore_Pipe_pst = &CanIf_XCore_PipeRuntimeInfo_ast[PipeId];

    Schm_CanIf_XCore_Enter();
    {
        /* set all counters to Zero, all pointers to its base address */
        CanIf_XCore_Pipe_pst->PktCnt_u32 = 0u;
        CanIf_XCore_Pipe_pst->PktCntAck_u32 = 0u;
        CanIf_XCore_Pipe_pst->WritePtr_pv = CanIf_XCore_Pipe_pst->FifoBaseAddr_pv;
        CanIf_XCore_Pipe_pst->ReadPtr_pv = CanIf_XCore_Pipe_pst->FifoBaseAddr_pv;
        CanIf_XCore_EndBuf_InvalidFlag_b = FALSE;
        CanIf_XCore_RxClrPipeFlag_b = FALSE;

    }
    Schm_CanIf_XCore_Exit();
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/**********************************************************************************************************************
 * Function name    : CanIf_XCore_PipeWrite
 * Syntax           : void CanIf_XCore_PipeWrite(uint16 PipeId, const CanIf_XCore_PipeTxRxParams_st * params_pst))
 * Description      : This service intiate the request of transporting  PDU from one core to the other core.
 * Parameter        : PipeId, params_pst
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_XCore_PipeWrite(uint16 PipeId, CanIf_XCore_PipeTxRxParams_st * params_pst)
{
    /*Temporary pointer to hold information of a pipe object*/
    const PduInfoType * Canif_XCore_info_pcst;

    /* Pointer to the payload */
    uint32 * PayloadPtr_pu32;

    /* Pointer to the Pipe object */
    CanIf_XCore_PipeRuntimeInfoType_st * CanIf_XCore_Pipe_pst;

    Std_ReturnType retVal = E_NOT_OK;

    uint32 PayloadLenU32W_u32;

    /* Get the Pipe Object info pointer */
    CanIf_XCore_Pipe_pst = &CanIf_XCore_PipeRuntimeInfo_ast[PipeId];

    Canif_XCore_info_pcst = params_pst->PduInfoPtr;

    /* Report Det if source core is different from the current core */
    #if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_XCore_Pipe_pst->PipeConfig_pcst->SrcCoreId_u16 != Schm_CanIf_XCore_GetCurrentCoreId()), CANIF_XCORE_APIID_PIPEWRITE_SID, CANIF_XCORE_E_WRONGCORE)
    #endif

    /* Get the Debug information of the pipe */
    #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
    CanIf_XCore_Pipe_pst->Dbg_st.NumSendCalls_u32 ++;
    CanIf_XCore_Pipe_pst->Dbg_st.SendLastAttemptedPduId = params_pst->PduId;
    CanIf_XCore_Pipe_pst->Dbg_st.SendLastAttemptedPduInfo.SduDataPtr = Canif_XCore_info_pcst->SduDataPtr;
    CanIf_XCore_Pipe_pst->Dbg_st.SendLastAttemptedPduInfo.SduLength = Canif_XCore_info_pcst->SduLength;
    #endif

    /* Allocates sufficient space inside the FIFO to accomodate the given PDU.*/


    retVal = CanIf_XCore_Pipe_PushPduHdrToFifo(CanIf_XCore_Pipe_pst, params_pst, &PayloadPtr_pu32, &PayloadLenU32W_u32);

	if(retVal == E_OK)
	{
	    /* check if it pointer is 32bit aligned. Warning is OK as memory alignment is required */
	    /* MR12 RULE 11.4 VIOLATION: Warning is "Not Critical".Cast from pointer to object to integral type is OK. */
	    /* MR12 DIR 4.6 VIOLATION: Warning is "Not Critical". */
		if (((unsigned int)Canif_XCore_info_pcst->SduDataPtr & 0x3u) == 0x0u)
		{
	         /* MR12 RULE 11.3 VIOLATION: Warning is "Not Critical". The pointer cast is required here for misaligned case.
	            The pointer cast is required here*/
			rba_BswSrv_MemCopy32(PayloadPtr_pu32, (const uint32*) Canif_XCore_info_pcst->SduDataPtr, PayloadLenU32W_u32 * sizeof(uint32));
		}
		else
		{
	         /* MR12 DIR 1.1 VIOLATION: Warning is "Not Critical". */
			(void)rba_BswSrv_MemCopy(PayloadPtr_pu32, Canif_XCore_info_pcst->SduDataPtr, PayloadLenU32W_u32 * sizeof(uint32));
		}

		/* Inform the receiver about the new packet AFTER the COPY is completed*/
		CanIf_XCore_Pipe_pst->PktCnt_u32 = CanIf_XCore_Pipe_pst->PktCnt_u32 + 1u;

	    #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
		CanIf_XCore_Pipe_pst->Dbg_st.NumBytesSendOk_u32 += Canif_XCore_info_pcst->SduLength;
	    #endif

        /* Notify the destination core (or task) */
        switch(CANIF_XCORE_PIPE_GET_FLAG_PROC(CanIf_XCore_Pipe_pst->PipeConfig_pcst->DestFlags_u32))
        {
            case CANIF_XCORE_PIPE_FLAG_PROC_POLLING:
                /*nothing to do in case of polling */
            break;
            case CANIF_XCORE_PIPE_FLAG_PROC_TASK:
                Schm_CanIf_XCore_Activate_Task(CanIf_XCore_Pipe_pst->PipeConfig_pcst->DestRecvId);
            break;
            case CANIF_XCORE_PIPE_FLAG_PROC_SWI:
                Schm_CanIf_XCore_Activate_SW_Irq(CanIf_XCore_Pipe_pst->PipeConfig_pcst->DestRecvId);
            break;
            default:
                #if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
                /*MR12 RULE 14.3 VIOLATION:  Warning is "Not Critical" as the controlling expression is already checked before entering this condition */
                CANIF_DET_REPORT_ERROR(TRUE, CANIF_XCORE_APIID_PIPEWRITE_SID, CANIF_XCORE_E_PARAM);
                retVal = E_NOT_OK;
                #endif
                break;

        }
	}
    else
    {
        /* Increment the debug info NumSendCallsNotOk_u32*/
        #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
        CanIf_XCore_Pipe_pst->Dbg_st.NumSendCallsNotOk_u32 ++;
        #endif
    }

    return retVal;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/**********************************************************************************************************************
 * Function name    : CanIf_XCore_PipeRead
 * Syntax           : void CanIf_XCore_PipeRead(uint16 PipeId)
 * Description      : This function reads the Pipe information.
 * Parameter        : PipeId
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_XCore_PipeRead(uint16 PipeId)
{
    /* Pointer to the Pipe object */
    CanIf_XCore_PipeRuntimeInfoType_st * CanIf_XCore_Pipe_pst;

    /* Get the Pipe Object info pointer */
    CanIf_XCore_Pipe_pst = &CanIf_XCore_PipeRuntimeInfo_ast[PipeId];

    #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
    CanIf_XCore_Pipe_pst->Dbg_st.NumReceiveCalls_u32 ++;
    #endif


    /* in a loop, process all messages until FIFO empty */
    /*MR12 RULE 1.3,13.2 VIOLATION : Warning is "Not Critical".Multiple volatile reads.No issue in code.*/
    while (CanIf_XCore_Pipe_pst->PktCnt_u32 != CanIf_XCore_Pipe_pst->PktCntAck_u32)
    {
        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
        volatile CanIf_XCore_PipePktHdrType_st *PktHdr_pst = (volatile CanIf_XCore_PipePktHdrType_st*) CanIf_XCore_Pipe_pst->ReadPtr_pv;
        uint32 PktFlags_u32 = PktHdr_pst->PktFlags_u32;
        uint32 PayloadLenBytes_u32 = PktHdr_pst->PktPayloadLenBytes_u32;

        uint32 pktLenU32W = CanIf_XCore_Pipe_CalcAlignSizeU32W(PayloadLenBytes_u32) + CANIF_XCORE_PTK_HDR_SIZE_U32W;


        if ((PktFlags_u32 & CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU) == CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU)
        {
            /*invalid. Consume data to end of FIFO*/
            CanIf_XCore_Pipe_pst->ReadPtr_pv = CanIf_XCore_Pipe_pst->FifoBaseAddr_pv;
        }
        else if ((PktFlags_u32 & CANIF_XCORE_PKT_FLAG_INVALID_PDU) == CANIF_XCORE_PKT_FLAG_INVALID_PDU)
        {
            /* MR12 RULE 11.5, 11.8 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
            CanIf_XCore_Pipe_pst->ReadPtr_pv = ((uint32*) CanIf_XCore_Pipe_pst->ReadPtr_pv) + pktLenU32W;
        }
        else
        {
            /* Pointer to Tx Pdu configuration */
            const CanIf_Cfg_TxPduConfig_tst * lTxPduConfig_pst;
            /* Pointer to RxPdu configuration */
            const CanIf_Cfg_RxPduType_tst * RxPduCfg_pcst;

            PduInfoType local_PduInfo_st;

            CanIf_XCore_PipeTxRxParams_st params_st;

            uint16 ltxPduCustId_t;

            /* Read from pipe */
            /* MR12 RULE 11.3, 11.8 VIOLATION: Warning is "Not Critical". The pointer cast is required her.*/
            uint32* PayloadPtr_pu32 = ((uint32*) PktHdr_pst) + CANIF_XCORE_PTK_HDR_SIZE_U32W;

            /*load the params struct */
            params_st.PduId = (PduIdType) (PktFlags_u32 & CANIF_XCORE_PKT_PDUID_MASK);
			params_st.callbackIndex_u8 = (uint8) ((PktFlags_u32 & CANIF_XCORE_PKT_CALLBACKIDX_MASK) >> CANIF_XCORE_PKT_CALLBACKIDX_SHIFT);
			/* MR12 RULE 10.8, 10.5 VIOLATION:Warning is "Not Critical".Explicit conversion of composite expression*/
			params_st.userType_e = (CanIf_Cfg_UserType_ten)((PktFlags_u32 & CANIF_XCORE_PKT_USERTYPE_MASK) >> CANIF_XCORE_PKT_USERTYPE_SHIFT);

			/* create the dest pdu info */
            params_st.PduInfoPtr = &local_PduInfo_st;
            local_PduInfo_st.SduDataPtr = (uint8*) PayloadPtr_pu32;
            local_PduInfo_st.SduLength = PayloadLenBytes_u32;

            ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[params_st.PduId];

            /*Get the address of config structure of requested PDU*/
            lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

            /* Get the pointer to desired Rx pdu */
            RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + params_st.PduId ;


            #if (CANIF_XCORE_DEBUG_ACTIVE == STD_ON)
            CanIf_XCore_Pipe_pst->Dbg_st.NumReceiveCallsOk_u32 ++;
            CanIf_XCore_Pipe_pst->Dbg_st.NumBytesReceiveOk_u32 += PayloadLenBytes_u32;
            CanIf_XCore_Pipe_pst->Dbg_st.ReceiveLastPduId = params_st.PduId;
            CanIf_XCore_Pipe_pst->Dbg_st.ReceiveLastPduInfo = local_PduInfo_st;
            #endif

            /* Report PDU to upper layer using the correct API, depending on the PIPE type*/
            switch(CANIF_XCORE_PIPE_GET_FLAG_TYPE(CanIf_XCore_Pipe_pst->PipeConfig_pcst->DestFlags_u32))
            {
                case CANIF_XCORE_PIPE_FLAG_TYPE_TX:
                    (void)CanIf_XCore_LocalCore_Transmit(params_st.PduId, &local_PduInfo_st);
                break;

                case CANIF_XCORE_PIPE_FLAG_TYPE_RX:
                    #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
					(void)CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &local_PduInfo_st);
                    #endif
                break;

                case CANIF_XCORE_PIPE_FLAG_TYPE_TXCONF:
                    #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
					(void)CanIf_XCore_LocalCore_TxConfirmation(lTxPduConfig_pst);
                    #endif
                break;

                default:
                    #if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
                    /*MR12 RULE 14.3 VIOLATION:  Warning is "Not Critical" as the controlling expression is already checked before entering this condition */
                    CANIF_DET_REPORT_ERROR(TRUE, CANIF_XCORE_APIID_PIPEREAD_SID, CANIF_XCORE_E_PARAM);
                    #endif
                    break;
            }
            /* MR12 RULE 11.5, 11.8 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
           CanIf_XCore_Pipe_pst->ReadPtr_pv = ((uint32*) CanIf_XCore_Pipe_pst->ReadPtr_pv) + pktLenU32W; /* inform the producer. No wraparound possible. */
        }
        CanIf_XCore_Pipe_pst->PktCntAck_u32 = CanIf_XCore_Pipe_pst->PktCntAck_u32 + 1u; /* packet consumed */
    }
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_CreatePduPkt
 * Syntax           : static void CanIf_XCore_Pipe_CreatePduPkt(
 *                                                      CanIf_XCore_PipeRuntimeInfoType_st *CanIf_XCore_Pipe_pst,
 *                                                      volatile void *PktStart_pv,
                                                        const uint32 PktFlags_u32,
                                                        const uint32 PayloadLenBytes_u32,
                                                        uint32 pktLenU32W,
                                                        uint32** PayloadPtr_ppu32)
 * Description      : This function creates a pipe packet.
 * Parameter        : PipeId
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

static void CanIf_XCore_Pipe_CreatePduPkt(CanIf_XCore_PipeRuntimeInfoType_st *CanIf_XCore_Pipe_pst, volatile void *PktStart_pv,\
    const uint32 PktFlags_u32, uint8 ControllerID_u8, const uint32 PayloadLenBytes_u32, uint32 pktLenU32W, uint32** PayloadPtr_ppu32)
{
    /* create the new packet: header start, then fill info and SduLength, advance pointers */
    /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
    volatile CanIf_XCore_PipePktHdrType_st *PktHdr_pst = (volatile CanIf_XCore_PipePktHdrType_st*) PktStart_pv;

    PktHdr_pst->PktFlags_u32 = PktFlags_u32;
    PktHdr_pst->PktPayloadLenBytes_u32 = PayloadLenBytes_u32;
    PktHdr_pst->CtrlID_u8 = ControllerID_u8;

    /* save payload ptr for the upper layer = current writeptr + header size.*/
    /* MR12 RULE 11.5,11.8 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
    *PayloadPtr_ppu32 = ((uint32*) PktStart_pv) + CANIF_XCORE_PTK_HDR_SIZE_U32W;
    /* MR12 RULE 11.5, 11.8 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
    CanIf_XCore_Pipe_pst->WritePtr_pv = ((uint32*)PktStart_pv) + pktLenU32W;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_CreatePduPkt
 * Syntax           : static void CanIf_XCore_Pipe_CreateInvalidPduPkt(
 *                                                  CanIf_XCore_PipeRuntimeInfoType_st *Pipe_pst,
 *                                                      volatile void *PktStart_pv)
 * Description      : This function creates a pipe packet.
 * Parameter        : PipeId
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
static void CanIf_XCore_Pipe_CreateInvalidPduPkt(CanIf_XCore_PipeRuntimeInfoType_st *Pipe_pst, volatile void *PktStart_pv)
{
    /* create the new packet: header start, then fill info and SduLength, advance pointers */
    /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
    volatile CanIf_XCore_PipePktHdrType_st *PktHdr_pst = (volatile CanIf_XCore_PipePktHdrType_st*) PktStart_pv;

    /* Produce to the end of the Fifo by setting the WritePtr_pv to END.
       AND increment pkt count to inform receiver */
    if (CanIf_XCore_EndBuf_InvalidFlag_b == TRUE)
    {
        PktHdr_pst->PktFlags_u32 = CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU;
        Pipe_pst->WritePtr_pv = Pipe_pst->FifoEndAddr_pv;
        /*Increment the packet counter*/
        Pipe_pst->PktCnt_u32 = Pipe_pst->PktCnt_u32 + 1u;
        CanIf_XCore_EndBuf_InvalidFlag_b = FALSE;
    }
    else
    {
        PktHdr_pst->PktFlags_u32 = CANIF_XCORE_PKT_FLAG_INVALID_PDU;
    }
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_Pipe_PushPduHdrToFifo
 * Syntax           : static Std_ReturnType CanIf_XCore_Pipe_PushPduHdrToFifo(
 *                                                            CanIf_XCore_PipeRuntimeInfoType_st *CanIf_XCore_Pipe_pst,
 *                                                            CanIf_XCore_PipeTxRxParams_st* params_pst,
                                                                uint32** PayloadPtr_ppu32,
                                                                uint32* PayloadLenU32W_pu32)
 * Description      : This function performs the actual allocation of sufficient space inside the FIFO to accomodate the given PDU
 * Parameter        : PipeId
 * Return value     : None
 **********************************************************************************************************************/

/*Basic strategy:
- calculate how much space is available in the FIFO RAM from the current WritePtr_pv position until (end of FIFO OR ReadPtr_pv; whichever is smaller)
- if sufficent space PDU ->
            -> write at WritePtr_pv the current PKR header
            -> advance the WritePtr_pv to after the packet
            -> return happy OK
- else
    if (ReadPtr_pv is smaller than WritePtr_pv) there might be space at the start of FIFO
        calculate how much space is available in the FIFO from the FIFO RAM begin until the ReadPtr_pv.
        if sufficent space for PDU
            -> write at the old WritePtr_pv an "invalid packet" big enough to fill the space until the end of FIFO RAM so that the consumer can skip it
            -> move the WritePtr_pv to FIFO RAM start; write there the current PKR header
            -> advance the WritePtr_pv to after the packet
            -> return happy!
*/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: params_pst variable is without const. */
static Std_ReturnType CanIf_XCore_Pipe_PushPduHdrToFifo(CanIf_XCore_PipeRuntimeInfoType_st *CanIf_XCore_Pipe_pst, CanIf_XCore_PipeTxRxParams_st* params_pst,
														uint32** PayloadPtr_ppu32, uint32* PayloadLenU32W_pu32)
{
    /*variable to hold return value*/
    Std_ReturnType retVal = E_NOT_OK;

    /*Variale to hold Space left in words */
    uint32 spaceLeftU32Words_u32 = 0 ;

    volatile void *localReadPtr_vpv  = CanIf_XCore_Pipe_pst->ReadPtr_pv;

    /* Pointer for the PDU information*/
    const PduInfoType *info = params_pst->PduInfoPtr;

    PduIdType PduId = params_pst->PduId;

    /* Varaible to hold the callback index */
    uint8 callbackIndex_u8 = params_pst->callbackIndex_u8;

    /* Varaible to hold the usertype index */
    CanIf_Cfg_UserType_ten userType_e = params_pst->userType_e;

    /*Variable to hold controller Id */
    uint8  CtrlId_u8 = params_pst->ControllerId_u8;

    /* calculate a buffer size in uint32 words that fully includes the PDU payload */
    uint32 payloadLenU32W = CanIf_XCore_Pipe_CalcAlignSizeU32W(info->SduLength);

    /*how much space we need in FIFO for payload and pkt header*/
    uint32 pktLenU32W =  payloadLenU32W + CANIF_XCORE_PTK_HDR_SIZE_U32W;

    /* save payload for upper layer, in case we are successful */
    *PayloadLenU32W_pu32 = payloadLenU32W;

    if (CanIf_XCore_Pipe_pst->WritePtr_pv >= localReadPtr_vpv) /* normal case. no wraparound */
    {

     if((CanIf_XCore_Pipe_pst->WritePtr_pv == localReadPtr_vpv) && (CanIf_XCore_Pipe_pst->PktCnt_u32 != CanIf_XCore_Pipe_pst->PktCntAck_u32))
          {
         retVal = E_NOT_OK; /* not sufficient. FIFO FULL! */
           }
     else
        {
        /*Calculation of current free space in FIFO. Assumption:  all FIFOs must be at least uint32 aligned!*/
        /* MR12 RULE 11.5,11.8 10.4 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
        spaceLeftU32Words_u32 = (((uint8*) CanIf_XCore_Pipe_pst->FifoEndAddr_pv) - ((uint8*) CanIf_XCore_Pipe_pst->WritePtr_pv))  / sizeof(uint32);
        if (spaceLeftU32Words_u32 < pktLenU32W) /* not sufficient space until end  */
        {
            /* how much is there free at FIFO RAM start, until read ptr? */
            /* MR12 RULE 11.5,11.8 10.4 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
            spaceLeftU32Words_u32 = (((uint8*) localReadPtr_vpv) - ((uint8*) CanIf_XCore_Pipe_pst->FifoBaseAddr_pv))  / sizeof(uint32);
            if (spaceLeftU32Words_u32 < pktLenU32W)
            {
                retVal = E_NOT_OK; /* not sufficient. FIFO FULL! */
            }
            else
            {
                /* there is space at FIFO start!! Good!
                   mark the free space at the end of pipe as "invalid packet" and place the packet at FIFO start*/

                /* Set the Endbuffer invalid flag */
                CanIf_XCore_EndBuf_InvalidFlag_b  = TRUE;

                CanIf_XCore_Pipe_CreateInvalidPduPkt(CanIf_XCore_Pipe_pst, CanIf_XCore_Pipe_pst->WritePtr_pv);    /* place special (invalid) packet first*/

                /* The new valid, packet  at FIFO start*/
                CanIf_XCore_Pipe_CreatePduPkt(CanIf_XCore_Pipe_pst, CanIf_XCore_Pipe_pst->FifoBaseAddr_pv,
                        CANIF_XCORE_PKT_HDR_CALC(PduId, (uint32)callbackIndex_u8, userType_e), CtrlId_u8, info->SduLength, pktLenU32W, PayloadPtr_ppu32);
                retVal = E_OK;
            }
        }
        else  /* there is no wraperound and we have enough space => great!!*/
        {
            CanIf_XCore_Pipe_CreatePduPkt(CanIf_XCore_Pipe_pst, CanIf_XCore_Pipe_pst->WritePtr_pv, /*place at current write ptr */
                CANIF_XCORE_PKT_HDR_CALC(PduId, (uint32)callbackIndex_u8, userType_e), CtrlId_u8,  info->SduLength, pktLenU32W, PayloadPtr_ppu32);
            retVal = E_OK;
        }
      }
    }
    else /* wraparound: the free space is between WritePtr_pv and ReadPtr_pv*/
    {
        /* how much is there free, until read ptr? */
        /* MR12 RULE 11.5,11.8 10.4 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
        spaceLeftU32Words_u32 = (((uint8*) localReadPtr_vpv) - ((uint8*) CanIf_XCore_Pipe_pst->WritePtr_pv))  / sizeof(uint32);
        if (spaceLeftU32Words_u32 < pktLenU32W)
        {
            retVal = E_NOT_OK; /* not sufficient. FIFO FULL! */
        }
        else
        {
            CanIf_XCore_Pipe_CreatePduPkt(CanIf_XCore_Pipe_pst, CanIf_XCore_Pipe_pst->WritePtr_pv, /*place at current write ptr */
                CANIF_XCORE_PKT_HDR_CALC(PduId, (uint32)callbackIndex_u8, userType_e),CtrlId_u8, info->SduLength, pktLenU32W, PayloadPtr_ppu32);
            retVal = E_OK;
        }
    }
    return retVal;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_ClearPipes
 * Syntax           : void CanIf_XCore_ClearPipes(uint8 ControllerId)
 * Description      : This function clears the pipe packet assigned to a particular controller for a Tx and Tx confirmation pipe.
 * Parameter        : ControllerId
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
void CanIf_XCore_ClearPipes(uint8 ControllerId)
{
    uint16 PipeIndex;
    uint16 pipeid_u16 = CANIF_XCORE_INVALID_PIPE_ID;

    for (PipeIndex = 0; PipeIndex < CANIF_XCORE_MAX_NUM_PIPES;  PipeIndex++ )
    {

        if (CanIf_XCore_RxClrPipeFlag_b == TRUE)
        {
            /* Fetch the pipe ID fro the PipeMatrix */
            pipeid_u16 = CanIf_XCore_Config_pcst->PipeConfigs_ast[PipeIndex].PipeId_u16;
        }
        else if ((CANIF_XCORE_PIPE_TX == CanIf_XCore_Config_pcst->PipeConfigs_ast[PipeIndex].PipeType) || (CANIF_XCORE_PIPE_TXCONF == CanIf_XCore_Config_pcst->PipeConfigs_ast[PipeIndex].PipeType))
        {
            /* Fetch the pipe ID fro the PipeMatrix */
            pipeid_u16 = CanIf_XCore_Config_pcst->PipeConfigs_ast[PipeIndex].PipeId_u16;
        }
        else
        {
            /* Do nothing */
        }

        if (pipeid_u16 != CANIF_XCORE_INVALID_PIPE_ID)
        {
            /*Clear the pipe packet assigned to requested controller in Tx pipe*/
            CanIf_XCore_ClearPipepacket( pipeid_u16,ControllerId );
        }
    }
    CanIf_XCore_RxClrPipeFlag_b = FALSE;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/**********************************************************************************************************************
 * Function name    : CanIf_XCore_ClearPipepacket
 * Syntax           : void CanIf_XCore_ClearPipepacket( uint16 PipeId, uint8 ControllerId)
 * Description      : This function clears the pipe packet assigned to a particular controller in a pipe.
 * Parameter        : ControllerId
 * Return value     : None
 **********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

static void CanIf_XCore_ClearPipepacket( uint16 PipeId, uint8 ControllerId)
{

   /* Temporary pointer variable*/
    volatile void *lWritePtr_pv;

    uint32 PayloadLenBytes_u32;
    uint32 pktLenU32W;
    uint32 PktFlags_u32;
    uint32 lPktCnt_u32;
    uint32 lPktCntAck_u32;
    uint8 lControllerId_u8;
    PduIdType PduId;

    /* Get the pipe object information */
    CanIf_XCore_PipeRuntimeInfoType_st *Pipe_pst = &CanIf_XCore_PipeRuntimeInfo_ast[PipeId];

    lWritePtr_pv = Pipe_pst->ReadPtr_pv;
    lPktCnt_u32 = Pipe_pst->PktCnt_u32;
    lPktCntAck_u32 = Pipe_pst->PktCntAck_u32;

    while (lPktCnt_u32 != lPktCntAck_u32)
    {
        /* MR12 RULE 11.5 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
        volatile CanIf_XCore_PipePktHdrType_st *PktHdr_pst = (volatile CanIf_XCore_PipePktHdrType_st*) lWritePtr_pv;
        PktFlags_u32 = PktHdr_pst->PktFlags_u32;
        PduId = (PduIdType) (PktFlags_u32 & CANIF_XCORE_PKT_PDUID_MASK);
        lControllerId_u8 = PktHdr_pst->CtrlID_u8;
        PayloadLenBytes_u32 = PktHdr_pst->PktPayloadLenBytes_u32;
        pktLenU32W = CanIf_XCore_Pipe_CalcAlignSizeU32W(PayloadLenBytes_u32) + CANIF_XCORE_PTK_HDR_SIZE_U32W;


        if ((PktFlags_u32 & CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU) == CANIF_XCORE_PKT_FLAG_INVALID_ENDBUF_PDU)
        {
            lWritePtr_pv = Pipe_pst->FifoBaseAddr_pv;
        }
        else
        {
            if (lControllerId_u8 == ControllerId)
            {
                CanIf_XCore_Pipe_CreateInvalidPduPkt(Pipe_pst, lWritePtr_pv);
            }
            /* MR12 RULE 11.5, 11.8 VIOLATION : 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment.*/
            lWritePtr_pv = ((uint32*) lWritePtr_pv) + pktLenU32W;
        }
        lPktCnt_u32 --;
    }
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif
/* FC_VariationPoint_END */
