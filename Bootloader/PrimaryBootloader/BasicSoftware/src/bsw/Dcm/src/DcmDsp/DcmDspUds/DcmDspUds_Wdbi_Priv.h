

#ifndef DCMDSPUDS_WDBI_PRIV_H
#define DCMDSPUDS_WDBI_PRIV_H



/**
 ***************************************************************************************************
            Write Data by Identifier service
 ***************************************************************************************************
 */
#if(DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF)

#define DCM_DSP_WDBI_MINREQLEN 0x03u /* Minimum Request length for WDBI service */

#if(DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF)
LOCAL_INLINE boolean Dcm_Prv_SenderReceiverFixedLength(uint32  Datasize_u, uint8 DataType)
{
    boolean Status = FALSE;
    if( ((Datasize_u == 1u) && (DataType == DCM_BOOLEAN)) ||
            ((Datasize_u == 1u) &&
            ((DataType == DCM_UINT8) || (DataType == DCM_SINT8)))  ||
            ((Datasize_u == 2u) &&
            ((DataType == DCM_UINT16) || (DataType == DCM_SINT16)))  ||
            ((Datasize_u == 4u) &&
            ((DataType == DCM_UINT32) || (DataType == DCM_SINT32))) )

            {
               Status=TRUE;

            }

     return Status;
}

LOCAL_INLINE boolean Dcm_Prv_SenderReceiverVariableLength(uint32  Datasize_u, uint8 DataType)
{
    boolean Status = FALSE;
    if(((Datasize_u > 1u) &&
      ((DataType == DCM_UINT8) || (DataType == DCM_SINT8)))  ||
      ((Datasize_u > 2u) &&
      ((DataType == DCM_UINT16) || (DataType == DCM_SINT16)))  ||
      ((Datasize_u > 4u) &&
      ((DataType == DCM_UINT32) || (DataType == DCM_SINT32))) )

         {

         Status=TRUE;

         }

    return Status;

}
#endif

#endif


#endif   /* _DCMDSPUDS_WDBI_PRIV_H */

