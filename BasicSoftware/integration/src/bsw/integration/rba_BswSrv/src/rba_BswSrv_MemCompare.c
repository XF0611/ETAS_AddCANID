/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */

/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include "rba_BswSrv.h"

/*
 *********************************************************************
 * Defines
 *********************************************************************
 */

/*
 *********************************************************************
 * Variables
 *********************************************************************
 */

/*
 *********************************************************************
 * Functions
 *********************************************************************
 */
#if (RBA_BSWSRV_CFG_COMPILER_LIBRARY_SUPPORT == STD_OFF)

#define RBA_BSWSRV_START_SEC_CODE
#include "rba_BswSrv_MemMap.h"
/**
 ***********************************************************************************************************************
 * Brief: MemCompare with the same parameters as C90-memcmp
 *
 * This function implements MemCompare with the same parameters as C90-memcmp
 * It compares 32 bit data, if possible. So it is save to compare structures which require consistent 32 bit data.
 *
 * Parameter[in]:   const void* xSrc1_pcv, const void* xSrc2_pcv, uint32 numBytes_u32
 * Return :         sint32
 ***********************************************************************************************************************
 */

sint32 rba_BswSrv_MemCompare(const void* xSrc1_pcv, const void* xSrc2_pcv, uint32 numBytes_u32)
{
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    /* MR12 RULE 11.5 VIOLATION: The warning is non-removable since pointer to void is required to have same syntax as C90 interface and
                                 casting is required to manipulate the data. The warning is uncritical since the address range and proper
                                 alignment is ensured by the calculation done before in integer representation. */
    const uint64* xSrc1_pcu64 = (const uint64*)xSrc1_pcv;
    const uint64* xSrc2_pcu64 = (const uint64*)xSrc2_pcv;
    const uint32* xSrc1_pcu32;
    const uint32* xSrc2_pcu32;
#else
    /* MR12 RULE 11.5 VIOLATION: The warning is non-removable since pointer to void is required to have same syntax as C90 interface and
                                 casting is required to manipulate the data. The warning is uncritical since the address range and proper
                                 alignment is ensured by the calculation done before in integer representation.*/
    const uint32* xSrc1_pcu32 = (const uint32*)xSrc1_pcv;
    const uint32* xSrc2_pcu32 = (const uint32*)xSrc2_pcv;
#endif
    const uint16* xSrc1_pcu16;
    const uint16* xSrc2_pcu16;
    const uint8* xSrc1_pcu8;
    const uint8* xSrc2_pcu8;
    uint32 ctLoop_u32;
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    uint64 xTemp1_u64;
    uint64 xTemp2_u64;
#endif
    uint32 xTemp1_u32;
    uint32 xTemp2_u32;
    uint16 xTemp1_u16;
    uint16 xTemp2_u16;
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    /* 64 bit aligned compare */
        /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32.*/
        if ((numBytes_u32 >= 8u) && ((((uint64)xSrc1_pcu64 | (uint64)xSrc2_pcu64) & 0x07u) == 0u))
        {
            ctLoop_u32 = numBytes_u32 / 8u;
            numBytes_u32 &= 0x07u;
            do
            {
                if(*xSrc1_pcu64 != *xSrc2_pcu64)
                {
                    numBytes_u32 = 8u; /* force byte wise check of current word as required for ANSI-C memcmp */
                    xTemp1_u64 = *xSrc1_pcu64; /* use additional buffer to ensure that data is not read twice */
                    xTemp2_u64 = *xSrc2_pcu64;
                    xSrc1_pcu64 = &xTemp1_u64;
                    xSrc2_pcu64 = &xTemp2_u64;
                    break;
                }
                xSrc1_pcu64++;
                xSrc2_pcu64++;
                ctLoop_u32--;
            } while(ctLoop_u32 > 0u);
        }

    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xSrc1_pcu32 = (const uint32*)xSrc1_pcu64;
    xSrc2_pcu32 = (const uint32*)xSrc2_pcu64;
#endif
    /* 32 bit aligned compare */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32.*/
    if ((numBytes_u32 >= 4u) && ((((uint32)xSrc1_pcu32 | (uint32)xSrc2_pcu32) & 0x03u) == 0u))
    {
        ctLoop_u32 = numBytes_u32 / 4u;
        numBytes_u32 &= 0x03u;
        do
        {
            if(*xSrc1_pcu32 != *xSrc2_pcu32)
            {
                numBytes_u32 = 4u; /* force byte wise check of current word as required for ANSI-C memcmp */
                xTemp1_u32 = *xSrc1_pcu32; /* use additional buffer to ensure that data is not read twice */
                xTemp2_u32 = *xSrc2_pcu32;
                xSrc1_pcu32 = &xTemp1_u32;
                xSrc2_pcu32 = &xTemp2_u32;
                break;
            }
            xSrc1_pcu32++;
            xSrc2_pcu32++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xSrc1_pcu16 = (const uint16*)xSrc1_pcu32;
    xSrc2_pcu16 = (const uint16*)xSrc2_pcu32;

    /* 16 bit aligned compare */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32.*/
    if ((numBytes_u32 >= 2u) && ((((uint32)xSrc1_pcu16 | (uint32)xSrc2_pcu16) & 0x01u) == 0u))
    {
        ctLoop_u32 = numBytes_u32 / 2u;
        numBytes_u32 &= 0x01u;
        do
        {
            if(*xSrc1_pcu16 != *xSrc2_pcu16)
            {
                numBytes_u32 = 2u; /* force byte wise check of current word as required for ANSI-C memcmp */
                xTemp1_u16 = *xSrc1_pcu16; /* use additional buffer to ensure that data is not read twice */
                xTemp2_u16 = *xSrc2_pcu16;
                xSrc1_pcu16 = &xTemp1_u16;
                xSrc2_pcu16 = &xTemp2_u16;
                break;
            }
            xSrc1_pcu16++;
            xSrc2_pcu16++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xSrc1_pcu8 = (const uint8*)xSrc1_pcu16;
    xSrc2_pcu8 = (const uint8*)xSrc2_pcu16;

    /* 8 bit compare for remaining data */
    for(ctLoop_u32 = 0; ctLoop_u32 < numBytes_u32; ctLoop_u32++)
    {
        if(*xSrc1_pcu8 != *xSrc2_pcu8)
        {
            return (*xSrc1_pcu8 - *xSrc2_pcu8);
        }
        xSrc1_pcu8++;
        xSrc2_pcu8++;
    }
    return 0;
}

#define RBA_BSWSRV_STOP_SEC_CODE
#include "rba_BswSrv_MemMap.h"

#endif /*(RBA_BSWSRV_CFG_COMPILER_LIBRARY_SUPPORT == STD_OFF)*/

