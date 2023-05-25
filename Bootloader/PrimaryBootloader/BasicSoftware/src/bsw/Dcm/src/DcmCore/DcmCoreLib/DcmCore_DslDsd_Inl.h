

#ifndef DCMCORE_DSLDSD_INL_H
#define DCMCORE_DSLDSD_INL_H

/**************************************************************************************************/
/* Included  header files                                                                         */
/**************************************************************************************************/


/**
 **************************************************************************************************
 * Dcm_Prv_ReloadS3Timer :  Inline function to start P3 timer monitoring
 *
 * \param           None
 *
 *
 * \retval          None
 * \seealso
 * \usedresources
 **************************************************************************************************
 */


LOCAL_INLINE void Dcm_Prv_ReloadS3Timer (void);
LOCAL_INLINE void Dcm_Prv_ReloadS3Timer (void)
{

#if(DCM_CFG_KWP_ENABLED != DCM_CFG_OFF)

    if(DCM_IS_KWPPROT_ACTIVE() != FALSE)
    {
        /* if KWP is running protocol start P3 timer */

        DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, Dcm_DsldTimer_st.dataTimeoutP3max_u32,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
    }
    else
    {
        /* if UDS is running protocol start S3 timer */

        DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, DCM_CFG_S3MAX_TIME,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
    }
#else
    /* if UDS is running protocol start S3 timer */

    DCM_TimerStart(Dcm_DsldGlobal_st.dataTimeoutMonitor_u32, DCM_CFG_S3MAX_TIME,Dcm_P2OrS3StartTick_u32,Dcm_P2OrS3TimerStatus_uchr)
#endif
}

/**
 **************************************************************************************************
 * Dcm_StartProtocol : Function to call all RTE configured functions.
 *
 * \param           ProtocolID: protocol id
 *
 *
 * \retval          Std_ReturnType (E_OK,E_NOT_OK,DCM_E_PROTOCOL_NOT_ALLOWED)
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

LOCAL_INLINE Std_ReturnType Dcm_StartProtocol(Dcm_ProtocolType ProtocolID);
LOCAL_INLINE void Dcm_StopProtocol(Dcm_ProtocolType ProtocolID);

LOCAL_INLINE  Std_ReturnType Dcm_StartProtocol(Dcm_ProtocolType ProtocolID)
{
#if((DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_CALL_BACK_NUM_PORTS !=0))
    uint32_least idxIndex_qu32;
#endif
    Std_ReturnType dataReturnType_u8;

    /* Call DcmAppl function to start the protocol */
    dataReturnType_u8 = DcmAppl_DcmStartProtocol(ProtocolID);

#if((DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_CALL_BACK_NUM_PORTS !=0))
    if(dataReturnType_u8 == E_OK)
    {
        /* Call all configured functions in RTE */
        for(idxIndex_qu32 = 0x00 ; idxIndex_qu32<Rte_NPorts_CallbackDCMRequestServices_R() ; idxIndex_qu32++)
        {
            dataReturnType_u8 =((Rte_Ports_CallbackDCMRequestServices_R())[idxIndex_qu32].Call_StartProtocol)(ProtocolID);

            if(Dcm_IsInfrastructureErrorPresent_b(dataReturnType_u8) != FALSE )
            {
                dataReturnType_u8 = DCM_INFRASTRUCTURE_ERROR;

            }
            else if((dataReturnType_u8 == DCM_E_PROTOCOL_NOT_ALLOWED) || (dataReturnType_u8 == E_NOT_OK))
            {
                /* Do nothing */
            }
            else
            {
                dataReturnType_u8 = E_OK;
            }
            if(dataReturnType_u8 != E_OK)
            {
                break;
            }
        }
    }
#endif
    return(dataReturnType_u8);
}

/**
 **************************************************************************************************
 * Dcm_StopProtocol : Function to call all RTE configured functions.
 *
 * \param           ProtocolID: protocol id
 *
 *
 * \retval          void
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

LOCAL_INLINE  void Dcm_StopProtocol(Dcm_ProtocolType ProtocolID)
{
#if((DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_CALL_BACK_NUM_PORTS !=0))
    uint32_least idxIndex_qu32;
#endif

    /* Call DcmAppl function to stop the protocol */
    (void)DcmAppl_DcmStopProtocol(ProtocolID);

#if((DCM_CFG_RTESUPPORT_ENABLED != DCM_CFG_OFF) && (DCM_CFG_CALL_BACK_NUM_PORTS!=0))
    /* Call all configured functions in RTE */
    for(idxIndex_qu32 =0x00 ; idxIndex_qu32<Rte_NPorts_CallbackDCMRequestServices_R() ; idxIndex_qu32++)
    {
        (void)((Rte_Ports_CallbackDCMRequestServices_R())[idxIndex_qu32].Call_StopProtocol)(ProtocolID);
    }
#endif
}

/**
 **************************************************************************************************
 * Dcm_SesCtrlChangeIndication : Function to call all RTE configured functions.
 *
 * \param           dataSesCtrlTypeOld_u8: Old  session
 *                  dataSesCtrlTypeNew_u8: Changed session
 *
 * \retval          Std_ReturnType (but ignored)
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

LOCAL_INLINE Std_ReturnType Dcm_SesCtrlChangeIndication(
        Dcm_SesCtrlType dataSesCtrlTypeOld_u8,
        Dcm_SesCtrlType dataSesCtrlTypeNew_u8
);

LOCAL_INLINE Std_ReturnType Dcm_SesCtrlChangeIndication(
        Dcm_SesCtrlType dataSesCtrlTypeOld_u8,
        Dcm_SesCtrlType dataSesCtrlTypeNew_u8
)
{

    /* Call DcmAppl function to indicate the session change */
    DcmAppl_DcmSesCtrlChangeIndication(dataSesCtrlTypeOld_u8,dataSesCtrlTypeNew_u8);
    return(E_OK);
}


/**
 **************************************************************************************************
 * Dcm_ConfirmationRespPend: Function to call all RTE configured functions on reception of a forced response pend.
 *
 * \param           status: ConfirmationStatus of the forced resp pend
 *
 * \retval          Std_ReturnType
 * \seealso
 * \usedresources
 **************************************************************************************************
 */

LOCAL_INLINE Std_ReturnType Dcm_ConfirmationRespPend(
        Dcm_ConfirmationStatusType status
);
LOCAL_INLINE Std_ReturnType Dcm_ConfirmationRespPend(
        Dcm_ConfirmationStatusType status
)
{

    /* Call DcmAppl function for response pend */
    DcmAppl_ConfirmationRespPend(status);
    return(E_OK);
}

#endif   /* for _DCMCORE_DSLDSD_INL_H */

