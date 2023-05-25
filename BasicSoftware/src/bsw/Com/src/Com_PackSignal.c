

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
 Function name    : Com_Prv_PackSignal
 Description      : Internal function used for packing the signal into Ipdubuffer
 Parameter        : endianess_u8  -> Little Endian/Big Endian
                  : bitPos_uo     ->   Position of the signal in the ipdu
                  : bitSize_uo    ->   Size of the signal
                  : srcBuf_uo     ->   Buffer where signal data is available
                  : destBuf_pu8   ->  ponter to buffer where the signal data needs to be packed
 Return value     : None
 **********************************************************************************************************************
*/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
void Com_Prv_PackSignal(
                uint8                   endianess_u8,
                Com_Bitposition_tuo     bitPos_uo,
                Com_Bitsize_tuo         bitSize_uo,
                Com_SigMax_tuo          srcBuf_uo,
                uint8 *                 destBuf_pu8
                   )
{

    /* Bfx - AR specific service library is not used, for the signal packing operations for below reasons
     * 1. There is a function Bfx_GetBits which takes a variable of type (uint8/16/32/64), extracts bits from it,
     * returns the data in the same respective type(uint8/16/32/64).
     * If this api is used in Com, there will still be many bit operations(Bitshift etc..)
     * which have to be performed in the Com code,hence there is no much help,
     * as bit operations (significant in number) are done both in Com and SrvLib.
     */

    PduLengthType       byteNo_uo;
    uint16_least        totalBitsCopied_qu16;
    uint16_least        bitsLeft_qu16;
    uint8_least         bitOffsetInByte_qu8;
    uint8_least         aData_qu8;
    uint8               mask_u8;

    /* Clear unused bits in the source buffer */
    if(bitSize_uo != COM_SIG_MAX_NO_OF_BITS)
    {
        srcBuf_uo = srcBuf_uo & (~(COM_SIG_MAX_DATA << bitSize_uo));
    }

    byteNo_uo          = (PduLengthType)(bitPos_uo >> 3u);
    bitOffsetInByte_qu8 = (uint8_least)(bitPos_uo % 8u);

    aData_qu8 = (uint8_least)(srcBuf_uo << bitOffsetInByte_qu8);

    if(bitSize_uo >= (8u - bitOffsetInByte_qu8))
    {
        /* This Part will be executed if the Signal is placed in more than 1 byte */
        mask_u8 = (uint8)(~((uint8)(0xFFu << bitOffsetInByte_qu8)));
        totalBitsCopied_qu16 = (8u - bitOffsetInByte_qu8);
    }
    else
    {
        /* This Part will be executed if the Signal is placed within the same byte. i.e. bitSize_uo <= 7 */
        mask_u8 = (uint8)(~(((COM_ONE << bitSize_uo) - COM_ONE) << bitOffsetInByte_qu8));
        totalBitsCopied_qu16 = bitSize_uo;
    }

    *(destBuf_pu8+byteNo_uo) = ((*(destBuf_pu8+byteNo_uo)) & mask_u8) | (uint8)aData_qu8;

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

        aData_qu8 = (uint8_least)(srcBuf_uo >> totalBitsCopied_qu16);

        if(bitsLeft_qu16 >= 8u)
        {
            *(destBuf_pu8+byteNo_uo) = (uint8)aData_qu8;
            totalBitsCopied_qu16 = totalBitsCopied_qu16 + 8u;
        }
        else
        {
            mask_u8 = (uint8)(0xFFu << bitsLeft_qu16);
            *(destBuf_pu8+byteNo_uo) = *(destBuf_pu8+byteNo_uo) & mask_u8;
            *(destBuf_pu8+byteNo_uo) = *(destBuf_pu8+byteNo_uo) | (uint8)aData_qu8;

            totalBitsCopied_qu16 = totalBitsCopied_qu16 + bitsLeft_qu16;
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/* FC_VariationPoint_START */
#if (defined(COM_TXSIG_FLOAT64SUPP) || defined(COM_TXGRPSIG_FLOAT64SUPP) ||\
    (defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_RXGRPSIG_FLOAT64SUPP)))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/*
 **********************************************************************************************************************
 Function name    : Com_Prv_PackFloatSignal
 Description      : Internal function used for packing the signal into Ipdubuffer
 Parameter        : endianess_u8    -> Little Endian/Big Endian
                  : bitPos_uo      ->   Position of the signal in the ipdu
                  : src_f64         ->   Buffer where signal data is available
                  : destBuf_pu8    ->  ponter to buffer where the signal data needs to be packed
 Operation        : This function only packs Float64 signals into Tx Ipdu buffer.
 There can be two possibilities in packing a float signal
 1. Signal lies in the byte boundary:
  If signal lies in byte boundary, then all the 8 bytes of float data (64 bits) is copied into the uint8 destination
  buffer using memcpy function

 2. Signal does not lie in the byte boundary:
  If the signal bit position is not in the byte boundary, then the first and last bytes are displaced from the byte
  boundary and need a bit copy of corresponding displaced bits. Where as the other 7 bytes of data can be copied with
  memcpy function
  i.e : If the Bit position of the signal is 21,
        1. first three bits are packed in 3rd byte of IPdu buffer with LSB being bit number 21.
        2. Next 7 bytes of float data is copied into the next 7 bytes of IPdu buffer
        3. The 11th byte of Ipdu buffer is then packed with remaining 5 bits of data.
 Return value     : None
 **********************************************************************************************************************
*/
void Com_Prv_PackFloatSignal(uint8 endianess_u8, Com_Bitposition_tuo bitPos_uo, float64 src_f64, uint8 * destBuf_pu8)
{
    PduLengthType       byteNo_uo;
    PduLengthType       numOfBytes_uo;
    uint8_least         bitOffsetInByte_qu8;

    /* Float64 data type has 8 bytes */
    numOfBytes_uo = 8u;
    byteNo_uo = bitPos_uo >> 3u;
    bitOffsetInByte_qu8 = (uint8)(bitPos_uo % 8u);

    if(bitOffsetInByte_qu8 == COM_ZERO )
    {
        if(endianess_u8 == COM_LITTLEENDIAN)
        {
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type based on the pointers address alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy((void *)(destBuf_pu8+byteNo_uo),(const void *)&src_f64,numOfBytes_uo);

        }
        else
        {
            uint8   index_u8;
            uint8   convdata_au8[8];

            /* This step is needed to perform bit operations on the Src data
             * bit operations cannot be performed on Float data type */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type based on the pointers address alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy((void *)convdata_au8, (const void *)&src_f64, numOfBytes_uo);

            for(index_u8 = COM_ZERO; index_u8 < numOfBytes_uo; index_u8++)
            {
                *(destBuf_pu8+byteNo_uo) = convdata_au8[index_u8];
                byteNo_uo--; /* Decrement the byte number to point to the next byte */
            }
        }
    }
    else
    {
        uint16_least    index_qu16;
        uint8           mask_u8;
        uint8           convData_au8[8];

        /* This step is needed to perform bit operations on the Src data
         * bit operations cannot be performed on Float data type */
        /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
           But the void pointer is always deferenced to a type based on the pointers address alignment.
           Hence the alignment warning can safely be ignored. */
        (void)rba_BswSrv_MemCopy( (void *)convData_au8, (const void *)&src_f64, numOfBytes_uo );

        mask_u8 = (uint8) (~((uint8)(0xFFu << bitOffsetInByte_qu8)));
        /* Copy the first byte into the Ipdu buffer */
        *(destBuf_pu8+byteNo_uo) = (uint8)((uint8)((uint8)(*(destBuf_pu8+byteNo_uo)) & mask_u8) |
                                     (uint8)(convData_au8[0] << bitOffsetInByte_qu8));

        if(endianess_u8 == COM_LITTLEENDIAN)
        {
            /* Increment the byte number to point to the next byte */
            byteNo_uo++;

            /* The last byte is copied outside the loop, hence run the loop for (numOfBytes_uo-1) times */
            for(index_qu16 = COM_ZERO; index_qu16 < (numOfBytes_uo-COM_ONE); index_qu16++)
            {
                /* Below Function is to be masked for protection ? */
                /* Copy the left over part of first byte and additional part of next byte into the Ipdu buffer */
                *(destBuf_pu8+byteNo_uo) = (uint8)((uint8)(convData_au8[index_qu16] >> (8u-bitOffsetInByte_qu8)) |
                                             (uint8)(convData_au8[index_qu16 + COM_ONE] << bitOffsetInByte_qu8));

                byteNo_uo++; /* Increment the byte number to point to the next byte */
            }
            /* Copy the last byte into the Ipdu buffer */
            *(destBuf_pu8+byteNo_uo) = (uint8)((*(destBuf_pu8+byteNo_uo)) & ((uint8)(~mask_u8))) |
                                 (uint8)(convData_au8[7] >> (8u-bitOffsetInByte_qu8));
        }
        else
        {
            /* Decrement the byte number to point to the next byte */
            byteNo_uo--;

            for(index_qu16 = COM_ZERO; index_qu16 < (numOfBytes_uo-COM_ONE); index_qu16++)
            {
                /* Below Function is to be masked for protection */
                /* Copy the left over part of first byte and additional part of next byte into the Ipdu buffer */
                *(destBuf_pu8+byteNo_uo) = (uint8)((uint8)(convData_au8[index_qu16] >> (8u-bitOffsetInByte_qu8)) |
                                             (uint8)((convData_au8[index_qu16 + COM_ONE] << bitOffsetInByte_qu8)));

                byteNo_uo--; /* Decrement the byte number to point to the next byte */
            }
            /* Copy the last byte into the Ipdu buffer */
            *(destBuf_pu8+byteNo_uo) = (uint8)((*(destBuf_pu8+byteNo_uo)) & ((uint8)(~mask_u8))) |
                                 (uint8)(convData_au8[7] >> (8u-bitOffsetInByte_qu8));
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif
/* FC_VariationPoint_END */

