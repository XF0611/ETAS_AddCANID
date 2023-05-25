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
 * Brief: Memcopy with the same parameters as C90-memcpy
 *
 * This function implements Memcopy with the same parameters as C90-memcpy
 * It copies 32 bit data, if possible. So it is save to copy structures which require consistent 32 bit data.
 *
 * Parameter[in]:       void* xDest_pv, const void* xSrc_pcv, uint32 numBytes_u32
 * Return:              void*
 ***********************************************************************************************************************
 */

/* MR12 RULE 21.2 VIOLATION: In order to support QAC_2777 and QAC_2776 warning of MISRA 2012 Mandatory, rba_bswsrv_memcopy function
                             is mapped to memcpy by Cchecker Tool. This mapping leads to generation of warnings QAC_4603 and QAC_4604,
                             which are false positives and could not be removed.*/
void* rba_BswSrv_MemCopy(void* xDest_pv, const void* xSrc_pcv, uint32 numBytes_u32)
{
    /* MR12 RULE 11.5 VIOLATION: The warning is non-removable since pointer to void is required to have same syntax as C90 interface and
                                 casting is required to manipulate the data. The warning is uncritical since the address range and proper
                                 alignment is ensured by the calculation done before in integer representation. */
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    uint64* xDest_pu64       = (uint64*)xDest_pv;
    const uint64* xSrc_pcu64 = (const uint64*)xSrc_pcv;
    uint32* xDest_pu32 ;
    const uint32* xSrc_pcu32;
#else
    uint32* xDest_pu32       = (uint32*)xDest_pv;
    const uint32* xSrc_pcu32 = (const uint32*)xSrc_pcv;
#endif
    uint16* xDest_pu16;
    const uint16* xSrc_pcu16;
    uint8* xDest_pu8;
    const uint8* xSrc_pcu8;
    uint32 ctLoop_u32;

#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)

    /* 64 bit aligned copy */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32.*/
    if ((numBytes_u32 >= 8u) && ((((uint64)xDest_pu64 | (uint64)xSrc_pcu64) & 0x07u) == 0u))
    {
        ctLoop_u32 = numBytes_u32 / 8u;
        numBytes_u32 &= 0x07u;
        do
        {
            *xDest_pu64 = *xSrc_pcu64;
            xDest_pu64++;
            xSrc_pcu64++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xDest_pu32 = (uint32*)xDest_pu64;
    xSrc_pcu32 = (const uint32*)xSrc_pcu64;

#endif

    /* 32 bit aligned copy */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32. */
    if ((numBytes_u32 >= 4u) && ((((uint32)xDest_pu32 | (uint32)xSrc_pcu32) & 0x03u) == 0u))
    {
        ctLoop_u32 = numBytes_u32 / 4u;
        numBytes_u32 &= 0x03u;
        do
        {
            *xDest_pu32 = *xSrc_pcu32;
            xDest_pu32++;
            xSrc_pcu32++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xDest_pu16 = (uint16*)xDest_pu32;
    xSrc_pcu16 = (const uint16*)xSrc_pcu32;

    /* 16 bit aligned copy */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is uncritical and non-removable since casting is required to manipulate memory address as uint32.*/
    if ((numBytes_u32 >= 2u) && ((((uint32)xDest_pu16 | (uint32)xSrc_pcu16) & 0x01u) == 0u))
    {
        ctLoop_u32 = numBytes_u32 / 2u;
        numBytes_u32 &= 0x01u;
        do
        {
            *xDest_pu16 = *xSrc_pcu16;
            xDest_pu16++;
            xSrc_pcu16++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xDest_pu8 = (uint8*)xDest_pu16;
    xSrc_pcu8 = (const uint8*)xSrc_pcu16;

    /* 8 bit copy for remaining data */
    for(ctLoop_u32 = 0; ctLoop_u32 < numBytes_u32; ctLoop_u32++)
    {
        *xDest_pu8 = *xSrc_pcu8;
        xDest_pu8++;
        xSrc_pcu8++;
    }
    return xDest_pv;
}

#define RBA_BSWSRV_STOP_SEC_CODE
#include "rba_BswSrv_MemMap.h"

#endif /*(RBA_BSWSRV_CFG_COMPILER_LIBRARY_SUPPORT == STD_OFF)*/

