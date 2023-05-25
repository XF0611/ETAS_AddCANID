

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Constants
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Functions
 **********************************************************************************************************************
*/
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_UnpackSignal
 Description      : Internal function used for unpacking the signal from Ipdu Buffer
 Parameter        : endianess_u8    - Little/Big Endian
                    bitPos_uo      - Signal Bit position
                    bitSize_uo      - Signal Bit size
                    *srcBuf_pcu8   - IPdu buffer
                    isSigned_b      - Is Signal of signed type
 Return value     : Com_SigMax_tuo - return unpack signal value
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
Com_SigMax_tuo Com_Prv_UnpackSignal(
                            uint8                       endianess_u8,
                            Com_Bitposition_tuo         bitPos_uo,
                            Com_Bitsize_tuo             bitSize_uo,
                            const uint8 *               srcBuf_pcu8,
                            boolean                     isSigned_b
                               )
{
    /* Bfx - AR specific service library is not used, for the signal unpacking operations for below reasons
     *  1. There is a function Bfx_PutBits takes a variable of type of data (uint8/16/32) as source variable and
     *      a Input data pointer of same respective type (uint8/16/32/64),
     *  2. It takes bit string from a source variable and puts them in destination data ptr.
     *  On the similar lines,If this api is used in Com, there will still be many bit type operations which
     *  have to be performed in the Com code, Hence there is no much help, as bit operations (significant in number)
     *  are done both in Com and SrvLib
     */

    Com_SigMax_tuo      aData_uo;
    PduLengthType       byteNo_uo;
    uint16_least        bitsLeft_qu16;
    uint16_least        totalBitsCopied_qu16;
    uint8_least         bitOffsetInByte_qu8;

    /* Finding the 1st Byte No */
    byteNo_uo          = (PduLengthType)(bitPos_uo >> 3u);
    bitOffsetInByte_qu8 = (uint8_least)(bitPos_uo % 8u);

    aData_uo = (Com_SigMax_tuo)(*(srcBuf_pcu8 + byteNo_uo) >> bitOffsetInByte_qu8);

    if (bitSize_uo >= (8u - bitOffsetInByte_qu8))
    {
        /* This Part will be executed if the Signal is placed in more than 1 byte */
        totalBitsCopied_qu16 = (uint16_least)(8u - bitOffsetInByte_qu8);
    }
    else
    {
        /* This Part will be executed if the Signal is placed within the same byte
         * Once data is copied, ensure the remaining bits are zero */
        aData_uo = aData_uo & (uint8)(~(uint8)(0xFFu << bitSize_uo));
        totalBitsCopied_qu16 = bitSize_uo;
    }

    while (totalBitsCopied_qu16 < bitSize_uo)
    {
        bitsLeft_qu16 = bitSize_uo - totalBitsCopied_qu16;

        if(endianess_u8 != COM_BIGENDIAN)
        {
            byteNo_uo++;
        }
        else
        {
            byteNo_uo--;
        }

        if(bitsLeft_qu16 >= 8u)
        {
            /* The data has been typecasted to Com_SigMax_tuo, to ensure data is not lost after the shifting. */
            aData_uo |= (((Com_SigMax_tuo)(*(srcBuf_pcu8 + byteNo_uo))) << totalBitsCopied_qu16);

            totalBitsCopied_qu16 = totalBitsCopied_qu16 + 8u;
        }
        else
        {
            /* The data has been typecasted to Com_SigMax_tuo, to ensure data is not lost after the shifting. */
            aData_uo |=
            (((Com_SigMax_tuo)(*(srcBuf_pcu8 + byteNo_uo) & ((uint8)(0xFFu >> (8u - bitsLeft_qu16))))) << totalBitsCopied_qu16);

            totalBitsCopied_qu16 = totalBitsCopied_qu16 + bitsLeft_qu16;
        }
    }

    /* For an integer signed-signal type, perform sign extension if the signal is of negative value */
    if ((isSigned_b) && ((aData_uo >> (bitSize_uo - COM_ONE)) != COM_ZERO))
    {
        aData_uo = Com_PerformSignExtension(aData_uo, bitSize_uo);
    }

    return(aData_uo); /* Return the Unpacked data */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/*
 **********************************************************************************************************************
 Function name    : Com_Prv_UnpackOpaqueSignal
 Description      : Internal function used for unpacking a UINT8_N opaque signal from Ipdu Buffer
 Parameter        : bitPos_uo,signalLength_uo,*srcBuf_pcu8
 Return value     : uint32
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
uint32 Com_Prv_UnpackOpaqueSignal(Com_Bitposition_tuo bitPos_uo, Com_Bitsize_tuo signalLength_uo, const uint8 * srcBuf_pcu8)
{
    uint16_least        byteNo_qu16;
    uint32              aData_u32;

    aData_u32 = COM_ZERO;

    /*Find the First Byte of the signal: Bit_Pos >> 8  */
    byteNo_qu16 = (bitPos_uo >> 3u);

    while (signalLength_uo > COM_ZERO)
    {
        aData_u32 = (uint32)( aData_u32 << 8u) | (uint32)(*(srcBuf_pcu8 + byteNo_qu16));
        byteNo_qu16 ++;
        signalLength_uo--;
    }

    /*Return the Unpacked data*/
    return(aData_u32);

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/* FC_VariationPoint_START */
#if defined (COM_RXSIG_FLOAT64SUPP ) || defined(COM_RXGRPSIG_FLOAT64SUPP)
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_UnPackFloatSignal
 Description      : Internal function used for packing the signal into Ipdubuffer
 Parameter        : endianess_u8 -> Little Endian/Big Endian
                  : bitPos_uo ->   Position of the signal in the ipdu
                  : srcBuf_pcu8 ->   Buffer where signal data is available
 Return value     : The function returns the unpacked Float64 value
 Operation        : This function only packs Float64 signals/ group signals into Tx Ipdu buffer.
 There can be two possibilities in packing a float signal
 1. Signal lies in the byte boundary:
  If signal lies in byte boundary, then all the 8 bytes of float data (64 bits) is copied into the uint8 destination
  buffer using memcpy function

 2. Signal does not lie in the byte boundary:
  If the signal bit position is not in the byte boundary, then the first and last bytes are displaced from the byte boundary
  and need a bit copy of corresponding displaced bits. Where as the other 7 bytes of data can be copied with memcpy function
  i.e : If the Bit position of the signal is 21,
        1. first three bits are packed in 3rd byte of IPdu buffer with LSB being bit number 21.
        2. Next 7 bytes of float data is copied into the next 7 bytes of IPdu buffer
        3. The 11th byte of Ipdu buffer is then packed with remaining 5 bits of data.
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
float64 Com_Prv_UnPackFloatSignal(uint8 endianess_u8, Com_Bitposition_tuo bitPos_uo, const uint8 * srcBuf_pcu8)
{

    float64             destBuf_f64;
    PduLengthType       byteNo_uo;
    uint8_least         bitOffsetInByte_qu8;
    uint8_least         index_qu8;
    uint8               destData_au8[8] = {0u,0u,0u,0u,0u,0u,0u,0u};

    byteNo_uo = (PduLengthType)(bitPos_uo >> 3u);
    bitOffsetInByte_qu8 = (uint8)(bitPos_uo % 8u);

    if(bitOffsetInByte_qu8 == COM_ZERO)
    {
        if(endianess_u8 == COM_LITTLEENDIAN)
        {
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type based on the pointers address alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy( (void *)destData_au8, (const void *)(srcBuf_pcu8+byteNo_uo), COM_F64_SIZE_IN_BYTES );

        }
        else
        {
            for(index_qu8 = COM_ZERO; index_qu8 < COM_F64_SIZE_IN_BYTES; index_qu8++)
            {
                destData_au8[index_qu8] = *(srcBuf_pcu8+byteNo_uo);
                byteNo_uo--;
            }
        }
    }
    else
    {
        if(endianess_u8 == COM_LITTLEENDIAN)
        {
            /* Run the loop for 8-times with an offset to Ipdu buffer */
            for(index_qu8 = COM_ZERO; index_qu8 < COM_F64_SIZE_IN_BYTES; index_qu8++)
            {
                /* Below Function is to be masked for protection ? */
                /* Copy the left over part of first byte and additional part of next byte into the Ipdu buffer */
                destData_au8[index_qu8] = (uint8)((uint8)(*(srcBuf_pcu8+byteNo_uo)) >> bitOffsetInByte_qu8) |
                                        (uint8)((*(srcBuf_pcu8+(byteNo_uo + COM_ONE))) << (8u-bitOffsetInByte_qu8));

                byteNo_uo++;
            }
        }
        else
        {
            /* Decrement the byte number to point to the next byte */
            for(index_qu8 = COM_ZERO; index_qu8 < COM_F64_SIZE_IN_BYTES; index_qu8++)
            {
                /* Below Function is to be masked for protection ? */
                /* Copy the left over part of first byte and additional part of next byte into the Ipdu buffer */
                destData_au8[index_qu8] = (uint8)((*(srcBuf_pcu8+byteNo_uo)) >> bitOffsetInByte_qu8) |
                                        (uint8)((*(srcBuf_pcu8+(byteNo_uo - COM_ONE))) << (8u-bitOffsetInByte_qu8));

                byteNo_uo--;
            }
        }
    }
    /* Convert the data into float64 type */
    /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
       But the void pointer is always deferenced to a type based on the pointers address alignment.
       Hence the alignment warning can safely be ignored. */
    (void)rba_BswSrv_MemCopy( (void *)&destBuf_f64, (const void *)destData_au8, COM_F64_SIZE_IN_BYTES );

    return(destBuf_f64);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined (COM_RXSIG_FLOAT64SUPP ) || defined(COM_RXGRPSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */

