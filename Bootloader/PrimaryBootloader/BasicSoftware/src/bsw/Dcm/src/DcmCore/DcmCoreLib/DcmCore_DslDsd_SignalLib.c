
#include "Dcm_Cfg_Prot.h"
#include "DcmCore_DslDsd_Inf.h"
#include "Rte_Dcm.h"

#if ((DCM_CFG_ROUTINEARRAY_INSIG != DCM_CFG_OFF)||(DCM_CFG_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEARRAY_INSIG != DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF))
#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"
static const uint8 CalArraySize_u8[6] = {8,16,32,8,16,32};     /* Look up table to get DataType Size : eg: UINT16_N will have value 16 */
#if (LOW_BYTE_FIRST == CPU_BYTE_ORDER)
static  const uint8 EndianessConvLUT_u8[6] = {0,1,0,1,2,3};   /* Look up table for extracting/storing  data bytes from request buffer or to Response buffer */
#else
static const uint8 EndianessConvLUT_u8[6] = {1,0,3,2,1,0};
#endif
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"
#endif

#define DCM_START_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"


/**The below function is enabled when RDBI/WDBI with atleast one sender receiver signal is enabled or RC service with
 * atleast one fixed length signal is available or OBD Model wiht atleast one PID of sender receiver type is available
 * or Dem Sender receiver is enable*/
#if (((DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF ))      ||            \
     ((DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE1_ENABLED != DCM_CFG_OFF)&& (DCM_CFG_DSP_OBDMODE1_SR_ENABLED!=DCM_CFG_OFF))               ||            \
     ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_READ_SR_ENABLED != DCM_CFG_OFF))         ||            \
     ((DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)&&((DCM_CFG_ROUTINEVARLENGTH==DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEVARLENGTH==DCM_CFG_OFF)))                     ||            \
     (DCM_CFG_ROUTINEARRAY_INSIG != DCM_CFG_OFF) || \
     (DCM_CFG_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF) || \
     (DCM_CFG_RCRANGE_ROUTINEARRAY_INSIG != DCM_CFG_OFF) || \
     (DCM_CFG_RCRANGE_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF) || \
     (DCM_CFG_RBA_DEM_SR_ENABLED != DCM_CFG_OFF))
/**
 **************************************************************************************************
 * Function Name    : Dcm_IsEndiannessConversionNecessary
 * Syntax           : boolean Dcm_IsEndiannessConversionNecessary(dataEndianness_u8)
 * Description      : API to check if endianness conversion is required or not,
 *                    if the function returns true endianness conversion is triggered otherwise no
 *
 * Parameters       : dataEndianType_en : The endianness type of the signal.
 *
 * Return value     : boolean
 **************************************************************************************************
 */
static boolean Dcm_IsEndiannessConversionNecessary(uint8 dataEndianness_u8)
{
    boolean flagConvStatus_b;

    /*Initialization of local variables*/

    flagConvStatus_b = FALSE;

    if((LOW_BYTE_FIRST==CPU_BYTE_ORDER) && (DCM_BIG_ENDIAN==dataEndianness_u8))
    {
        /*Endianness Conversion necessary, since cpu is of little endian and signal is of big endian*/

        flagConvStatus_b = TRUE;
    }
    else if((HIGH_BYTE_FIRST==CPU_BYTE_ORDER) && (DCM_LITTLE_ENDIAN==dataEndianness_u8))
    {
        /*Endianness Conversion necessary, since cpu is of big endian and signal is of little endian*/

        flagConvStatus_b = TRUE;
    }
    else
    {
        /*Endianness Conversion not necessary*/

        flagConvStatus_b = FALSE;
    }

    return flagConvStatus_b;
}
#endif

/**The below function is enabled when WDBI with atleast one sender receiver signal is enabled or RC service with
 * atleast one fixed length signal is available*/
#if (((DCM_CFG_DSP_WRITEDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_WRITE_SR_ENABLED != DCM_CFG_OFF ))   ||     \
     (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF) && ((DCM_CFG_ROUTINEVARLENGTH==DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEVARLENGTH==DCM_CFG_OFF)))
/**
 **************************************************************************************************
 * Function Name  : Dcm_GetSignal_u32 : API to get the Signal value from the request buffer
 * Syntax         : uint32 Dcm_GetSignal_u32(xDataType_u8, posnStart_u16, const adrReqBuffer_u8*, dataEndianness_u8)
 * Description    : TRACE [SWS_Dcm_00687],[SWS_Dcm_00639],[SWS_Dcm_00641]  This API will get the Signal value from the request buffer
 *
 * Parameters     : xDataType_u8:   xDataType_u8 of the signal BOOLEAN/UINT8/SINT8 etc
 *                  posnStart_u16:   Start bit position of the signal
 *                  adrReqBuffer_u8:  Pointer to the request buffer
 *                  dataEndianness_u8
 *
 * Return value   : dataSignal_u32: Value of the signal
 **************************************************************************************************
 */
uint32 Dcm_GetSignal_u32(uint8 xDataType_u8,
                                    uint16 posnStart_u16,
                                    const uint8 * adrReqBuffer_u8,
                                    uint8 dataEndianness_u8)
{
    /* Local variables */
    uint16 dataByteNum_u16;
    uint16 dataSignal_u16;
    uint32 dataSignal_u32;
    boolean flagConvStatus_b;

    /* Calculate the Byte position in the request buffer for this signal */
    dataByteNum_u16 = (uint16)((uint32)posnStart_u16/8u);
    dataSignal_u32  = 0;
    dataSignal_u16 = 0;
    /*Endianness Conversion not necessary*/
    flagConvStatus_b = FALSE;

    switch(xDataType_u8)
    {
        case DCM_BOOLEAN:
        {   /* Set the signal if the request boolean signal is set in the request */
            dataSignal_u32 = (uint32)(adrReqBuffer_u8[dataByteNum_u16]);
            dataSignal_u32 &= ((uint32)0x1u << (posnStart_u16 % 8u));
            break;
        }
        case DCM_UINT8:
        case DCM_SINT8:
        {
            dataSignal_u32 = adrReqBuffer_u8[dataByteNum_u16];
            break;
        }
        case DCM_UINT16:
        case DCM_SINT16:
        {
            /* Copy the data from buffer by pointer typecast into an uint16 variable*/
#if(LOW_BYTE_FIRST==CPU_BYTE_ORDER)
                /*Endianness Conversion necessary, since cpu is of little endian and signal is of big endian*/
                dataSignal_u16 = (uint16)(adrReqBuffer_u8[dataByteNum_u16 + 1u]);
                dataSignal_u16 = (dataSignal_u16 << 8u);
                dataSignal_u16 |= (uint16)(adrReqBuffer_u8[dataByteNum_u16]);
#else
                /*Endianness Conversion necessary, since cpu is of big endian and signal is of little endian*/
                dataSignal_u16 = (uint16)(adrReqBuffer_u8[dataByteNum_u16]);
                dataSignal_u16 = (dataSignal_u16 << 8u);
                dataSignal_u16 |= (adrReqBuffer_u8[dataByteNum_u16 + 1u]);
#endif
            /*Check if endianness conversion is necessary or not*/
            flagConvStatus_b=Dcm_IsEndiannessConversionNecessary(dataEndianness_u8);

            /*TRIGGER Endianness conversion*/

            if(TRUE==flagConvStatus_b)
            {
                DCM_ENDIANNESSCONVERSION16(dataSignal_u16);
            }

            dataSignal_u32 = (uint32)dataSignal_u16;
            break;
        }
        case DCM_UINT32:
        case DCM_SINT32:
        {

#if(LOW_BYTE_FIRST==CPU_BYTE_ORDER)

                /*Endianness Conversion necessary, since cpu is of little endian and signal is of big endian*/
                dataSignal_u32  = (uint32)adrReqBuffer_u8[dataByteNum_u16];
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16 + 1u]) << 8u) ;
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16 + 2u]) << 16u);
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16 + 3u]) << 24u);
#else

                /*Endianness Conversion necessary, since cpu is of big endian and signal is of little endian*/
                dataSignal_u32  =   (uint32)adrReqBuffer_u8[dataByteNum_u16 + 3u];
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16 + 2u]) << 8u) ;
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16 + 1u]) << 16u);
                dataSignal_u32 |= (((uint32)adrReqBuffer_u8[dataByteNum_u16]) << 24u);
#endif

            /*Check if endianness conversion is necessary or not*/
            flagConvStatus_b=Dcm_IsEndiannessConversionNecessary(dataEndianness_u8);

            /*TRIGGER Endianness conversion*/

            if(TRUE==flagConvStatus_b)
            {
                DCM_ENDIANNESSCONVERSION32(dataSignal_u32);
            }
            break;
        }
        default:
        {
            /*Since All the Data types are handled already in other cases , there is nothing to do in Default */
            break;
        }
    }

    /* Check if endianness conversion is enabled for the active protocol or the routine */

    return(dataSignal_u32);


}
#endif


/**The below function is enabled when RDBI with atleast one sender receiver signal is enabled or RC service with
 * atleast one fixed length signal is available or OBD Model wiht atleast one PID of sender receiver type is available
 * or Dem Sender receiver is enable*/

#if (\
/* FC_VariationPoint_START */ \
((DCM_CFG_DSPOBDSUPPORT_ENABLED != DCM_CFG_OFF)&&(DCM_CFG_DSP_OBDMODE1_ENABLED != DCM_CFG_OFF)&& (DCM_CFG_DSP_OBDMODE1_SR_ENABLED!=DCM_CFG_OFF)) ||            \
/* FC_VariationPoint_END */ \
     ((DCM_CFG_DSP_READDATABYIDENTIFIER_ENABLED != DCM_CFG_OFF) && (DCM_CFG_DSP_READ_SR_ENABLED != DCM_CFG_OFF)) ||     \
     ((DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)&&((DCM_CFG_ROUTINEVARLENGTH==DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEVARLENGTH==DCM_CFG_OFF)))||\
     (DCM_CFG_RBA_DEM_SR_ENABLED != DCM_CFG_OFF))

/**
 **************************************************************************************************
 * Function Name    : Dcm_StoreSignal
 * Syntax           : void Dcm_StoreSignal(xDataType_u8, posnStart_u16, adrRespBuffer_u8 *, dataSignalValue_u32, dataEndianness_u8)
 * Description      : TRACE [SWS_Dcm_00716],[SWS_Dcm_00641],[SWS_Dcm_00718] API to Store the Signal value to the response buffer
 *
 * Parameters       : SignalSize:  The size of the signal in bits.
 *                    posnStart_u16:    Start bit position of the signal
 *                    adrRespBuffer_u8:  Pointer to the request buffer
 *                    dataSignalValue_u32: Signal value
 *
 * Return value     : None
 * \seealso
 **************************************************************************************************
 */
void Dcm_StoreSignal(uint8 xDataType_u8,
                                    uint16 posnStart_u16,
                                    uint8 * adrRespBuffer_u8,
                                    uint32 dataSignalValue_u32,
                                    uint8 dataEndianness_u8)
{
    uint16 dataByteNum_u16;
    uint16 dataSignalValue_u16;
    boolean flagConvStatus_b;

    /*Initialization of local variables*/
    dataByteNum_u16 = (uint16)((uint32)posnStart_u16/8u);
    /*Endianness Conversion not necessary*/
    flagConvStatus_b = FALSE;

    /*Check if endianness conversion is necessary or not*/
    flagConvStatus_b=Dcm_IsEndiannessConversionNecessary(dataEndianness_u8);

    /* Store the signal in the buffer based on endianness conversion */
    switch(xDataType_u8)
    {
        case DCM_BOOLEAN:
        {
            /* Set the Bit in the buffer if the signal value is set */
            if(dataSignalValue_u32 != 0u)
            {
                adrRespBuffer_u8[dataByteNum_u16] |= (uint8)(0x1u<<(posnStart_u16%8u));
            }
            break;

        }
        case DCM_UINT8:
        case DCM_SINT8:
        {
            adrRespBuffer_u8[dataByteNum_u16] = (uint8)(dataSignalValue_u32);
            break;
        }
        case DCM_UINT16:
        case DCM_SINT16:
        {
            dataSignalValue_u16=(uint16)dataSignalValue_u32;
            /*TRIGGER Endianness conversion*/
            if(FALSE!=flagConvStatus_b)
            {
                DCM_ENDIANNESSCONVERSION16(dataSignalValue_u16);
            }

#if(LOW_BYTE_FIRST==CPU_BYTE_ORDER)

                /*Endianness Conversion necessary, since cpu is of little endian and signal is of big endian*/
                adrRespBuffer_u8[dataByteNum_u16] =  (uint8)(dataSignalValue_u16);
                adrRespBuffer_u8[dataByteNum_u16 + 1u] =  (uint8)(dataSignalValue_u16 >> 8u);

#else

                /*Endianness Conversion necessary, since cpu is of big endian and signal is of little endian*/
                adrRespBuffer_u8[dataByteNum_u16] =  (uint8)(dataSignalValue_u16 >> 8u);
                adrRespBuffer_u8[dataByteNum_u16 + 1u] =  (uint8)(dataSignalValue_u16);
#endif

            break;
        }
        case DCM_UINT32:
        case DCM_SINT32:
        {
            /*TRIGGER Endianness conversion*/
            if(FALSE!=flagConvStatus_b)
            {
                DCM_ENDIANNESSCONVERSION32(dataSignalValue_u32);
            }

#if(LOW_BYTE_FIRST==CPU_BYTE_ORDER)

                /*Endianness Conversion necessary, since cpu is of little endian and signal is of big endian*/
                adrRespBuffer_u8[dataByteNum_u16] =  (uint8)(dataSignalValue_u32);
                adrRespBuffer_u8[dataByteNum_u16 + 1u] =  (uint8)(dataSignalValue_u32 >> 8u);
                adrRespBuffer_u8[dataByteNum_u16 + 2u] =  (uint8)(dataSignalValue_u32 >> 16u);
                adrRespBuffer_u8[dataByteNum_u16 + 3u] =  (uint8)(dataSignalValue_u32 >> 24u);

#else

                /*Endianness Conversion necessary, since cpu is of big endian and signal is of little endian*/
                adrRespBuffer_u8[dataByteNum_u16] =  (uint8)(dataSignalValue_u32 >> 24u);
                adrRespBuffer_u8[dataByteNum_u16 + 1u] =  (uint8)(dataSignalValue_u32 >> 16u);
                adrRespBuffer_u8[dataByteNum_u16 + 2u] =  (uint8)(dataSignalValue_u32 >> 8u);
                adrRespBuffer_u8[dataByteNum_u16 + 3u] =  (uint8)(dataSignalValue_u32);

#endif
            break;
        }
        default:
        {
            /*Since All the Data Types  are handled already in other cases , there is nothing to do in Default */
            break;
        }
    }
}
#endif

#if (DCM_CFG_DSP_ROUTINECONTROL_ENABLED != DCM_CFG_OFF)
#if ( (DCM_CFG_ROUTINEARRAY_INSIG != DCM_CFG_OFF)  || (DCM_CFG_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF) || \
(DCM_CFG_RCRANGE_ROUTINEARRAY_INSIG != DCM_CFG_OFF)||(DCM_CFG_RCRANGE_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF) )

#if ((DCM_CFG_ROUTINEARRAY_INSIG != DCM_CFG_OFF) || (DCM_CFG_RCRANGE_ROUTINEARRAY_INSIG != DCM_CFG_OFF))

/**
 **************************************************************************************************
 * StoreRcBuffer : API to store the requested data into uint16 ot sint16 RC Buffer


 *
 * \param           xDataType_u8:   DataType of the data which has to be stored
 *                  IndexSig_u16:   Position in the buffer where the data has to be stored
 *                  dataSignal_u16:  The data which has to be stored
 *
 * \retval         : NONE

 **************************************************************************************************
 */
static void StoreRcBuffer(uint8 xDataType_u8,uint16 IndexSig_u16, uint16 dataSignal_u16)
{
    if(xDataType_u8 == DCM_UINT16_N)
    {
        Dcm_RCSigInN_au16[IndexSig_u16] = dataSignal_u16;
    }
    else
    {
        Dcm_RCSigInN_as16[IndexSig_u16] = (sint16)dataSignal_u16;
    }
}

/**
 **************************************************************************************************
 * Function Name        : Dcm_RcSetSignalArray
 * Syntax               : void Dcm_RcSetSignalArray (const adrSignal_pcst *, const adrReqBuffer_u8 *)
 * Description          : TRACE [SWS_Dcm_00641] API to store the ReqBuffer Data into appropriate Routine Control SignalType Buffer
                          As reqBuffer contains data in UINT8 chunks the same has to be converted if necessary into uint16/sint16/uint32..... based on
                          Endianess configuration and stored in the Routine Control Signal Array
 *
 * Parameters           : Dcm_DspRoutineSignalInfo_tst :   Structure containing the details of RC Signal configuration
 *                        adrReqBuffer_u8 :    Contains the Address of the RequestBuffer from where the data should be copied
 *
 * Return value         : NONE
 **************************************************************************************************
 */
void Dcm_RcSetSignalArray(const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst,const uint8 * adrReqBuffer_u8)
{
     uint8 xDataType_u8;
     uint16 DataLen_u16;
     uint16 IndexSig_u16;
     uint16 IndexReqData_u16;
     /*Check if endianness conversion is necessary or not*/
     boolean flagConvStatus_b;
     uint16 idx;
     uint16 dataSignal_u16;
     uint32 dataSignal_u32;


     dataSignal_u32 = 0;
     dataSignal_u16 = 0;
     xDataType_u8 = adrSignal_pcst->dataType_u8;
     DataLen_u16 = adrSignal_pcst->dataLength_u16 / CalArraySize_u8[xDataType_u8 - DCM_RCARRAYINDEX];
     flagConvStatus_b=Dcm_IsEndiannessConversionNecessary(adrSignal_pcst->dataEndianness_u8);
     IndexSig_u16 = adrSignal_pcst->idxSignal_u16;
     IndexReqData_u16 = (uint16)((adrSignal_pcst->posnStart_u16)/8);

    switch(xDataType_u8)
    {
        case DCM_UINT8_N:
        {
            /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
             DCM_MEMCOPY(&(Dcm_RCSigInN_au8[IndexSig_u16]),&(adrReqBuffer_u8[IndexReqData_u16]),DataLen_u16);
        }
        break;
        case DCM_SINT8_N:
        {
           /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
            DCM_MEMCOPY(&(Dcm_RCSigInN_as8[IndexSig_u16]),&(adrReqBuffer_u8[IndexReqData_u16]),DataLen_u16);
        }
        break;
        case DCM_UINT16_N:
        case DCM_SINT16_N:
        {

            for(idx=0;idx<DataLen_u16;idx++)
            {

                /* Combine the uint8 contents of Requestbuffer into a uint16 value */
                dataSignal_u16 = (uint16)(((uint16)adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[0]]) + ((uint16)(adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[1]]) * 0x100u));

                IndexReqData_u16 = IndexReqData_u16 + 2;
                /*TRIGGER Endianness conversion*/
                if(TRUE==flagConvStatus_b)
                {
                    DCM_ENDIANNESSCONVERSION16(dataSignal_u16);
                }
                StoreRcBuffer(xDataType_u8,IndexSig_u16,dataSignal_u16);

              IndexSig_u16++;
            }
        }
        break;

        case DCM_UINT32_N:
        case DCM_SINT32_N:
        {
            for(idx=0;idx<DataLen_u16;idx++)
            {

                 /* Combine the uint8 contents of Requestbuffer into a uint32 value */
                 dataSignal_u32 = (uint32)((adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[2]]) + ((adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[3]]) * 0x100u) + ((adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[4]]) *  0x10000u) + ((adrReqBuffer_u8[IndexReqData_u16 + EndianessConvLUT_u8[5]]) * 0x1000000u) );

                IndexReqData_u16 = IndexReqData_u16+4;
                /*TRIGGER Endianness conversion*/
                if(TRUE==flagConvStatus_b)
                {
                    DCM_ENDIANNESSCONVERSION32(dataSignal_u32);
                }

                if(xDataType_u8 == DCM_UINT32_N)
                {
                    Dcm_RCSigInN_au32[IndexSig_u16] = dataSignal_u32;
                }
                else
                {
                    Dcm_RCSigInN_as32[IndexSig_u16] = (sint32)dataSignal_u32;
                }

                IndexSig_u16++;
            }
        }
        break;

        default:
            /*All Data types cases are covered already*/

            break;
    }
}
#endif

#if((DCM_CFG_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF) || (DCM_CFG_RCRANGE_ROUTINEARRAY_OUTSIG != DCM_CFG_OFF))

/**
 **************************************************************************************************
 * Function Name        : Dcm_RcStoreSignalArray
 * Syntax               : void Dcm_RcStoreSignalArray (const adrSignal_pcst *, adrResBuffer_u8 *)
 * Description          : TRACE [SWS_Dcm_00641] API to store the data obtained from Routine Control service into Response buffer
                          As resBuffer should have data in UINT8 chunks the data returned by RC service
                          (for data types uint16,sint32....) has  to be split into uint8 chunks before updating the response
 *
 * Parameters           : Dcm_DspRoutineSignalInfo_tst :   Structure containing the details of RC Signal configuration
 *                        adrResBuffer_u8 :    Contains the Address of the ResponseBuffer to which the data should be filled
 *
 * Return value         : NONE
 **************************************************************************************************
 */
void Dcm_RcStoreSignalArray(const Dcm_DspRoutineSignalInfo_tst * adrSignal_pcst,uint8 * adrResBuffer_u8)
{

     uint8 xDataType_u8;
     uint16 DataLen_u16;
     uint16 IndexSig_u16;
     uint16 IndexResData_u16;
     /*Check if endianness conversion is necessary or not*/
     boolean flagConvStatus_b;
     uint16 idx;
     uint16 dataSignal_u16;
     uint32 dataSignal_u32;

     dataSignal_u32 = 0;
     dataSignal_u16 = 0;
     xDataType_u8 = adrSignal_pcst->dataType_u8;
     DataLen_u16 = adrSignal_pcst->dataLength_u16 / CalArraySize_u8[xDataType_u8 - DCM_RCARRAYINDEX];
     flagConvStatus_b=Dcm_IsEndiannessConversionNecessary(adrSignal_pcst->dataEndianness_u8);
     IndexSig_u16 = adrSignal_pcst->idxSignal_u16;
     IndexResData_u16 = (uint16)(adrSignal_pcst->posnStart_u16/8);

    switch(xDataType_u8)
    {
        case DCM_UINT8_N:
        {
            /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
            DCM_MEMCOPY(&(adrResBuffer_u8[IndexResData_u16]),&(Dcm_RCSigOutN_au8[IndexSig_u16]),DataLen_u16);
        }
        break;

        case DCM_SINT8_N:
        {
           /*MR12 DIR 1.1 VIOLATION:This is required for implememtaion as DCM_MEMCOPY takes void pointer as input and object type pointer is converted to void pointer*/
            DCM_MEMCOPY(&(adrResBuffer_u8[IndexResData_u16]),&(Dcm_RCSigOutN_as8[IndexSig_u16]),DataLen_u16);
        }
        break;

        case DCM_UINT16_N:
        case DCM_SINT16_N:
        {
            for(idx=0;idx<DataLen_u16;idx++)
            {
                dataSignal_u16 = (xDataType_u8 == DCM_UINT16_N) ? Dcm_RCSigOutN_au16[IndexSig_u16]:(uint16)Dcm_RCSigOutN_as16[IndexSig_u16];
                IndexSig_u16++;

                /*TRIGGER Endianness conversion*/
                if(TRUE==flagConvStatus_b)
                {
                    DCM_ENDIANNESSCONVERSION16(dataSignal_u16);
                }

                /* Copy the data from buffer by pointer typecast into an uint16 variable*/

                /* Store the data into response buffer based on Endianess of the system */
                adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[0]] =  (uint8)(dataSignal_u16);
                adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[1]] =  (uint8)(dataSignal_u16 >> 8u);

                 IndexResData_u16 = IndexResData_u16 + 2;
            }
        }
        break;

        case DCM_UINT32_N:
        case DCM_SINT32_N:
        {
            for(idx=0;idx<DataLen_u16;idx++)
            {
                dataSignal_u32 = (xDataType_u8 == DCM_UINT32_N) ? Dcm_RCSigOutN_au32[IndexSig_u16]:(uint32)Dcm_RCSigOutN_as32[IndexSig_u16];
                IndexSig_u16++;

                /*TRIGGER Endianness conversion*/
                if(TRUE==flagConvStatus_b)
                {
                    DCM_ENDIANNESSCONVERSION32(dataSignal_u32);
                }
              /* Copy the data from buffer by pointer typecast into an uint32 variable*/
              /* Split and store the data bytes into response buffer based on endianess of the system */
              adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[2]] =  (uint8)(dataSignal_u32);
              adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[3]] =  (uint8)(dataSignal_u32 >> 8u);
              adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[4]] =  (uint8)(dataSignal_u32 >> 16u);
              adrResBuffer_u8[IndexResData_u16 + EndianessConvLUT_u8[5]] =  (uint8)(dataSignal_u32 >> 24u);

              IndexResData_u16 = IndexResData_u16+4;
            }
        }
        break;

        default:
            /*All Data types are covered already*/
            break;
    }
}

#endif
#endif
#endif

#define DCM_STOP_SEC_CODE /*Adding this for memory mapping*/
#include "Dcm_MemMap.h"



