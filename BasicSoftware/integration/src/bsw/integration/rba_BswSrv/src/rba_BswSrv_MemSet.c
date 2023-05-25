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
 * Brief: MemSet with the same parameters as C90-memset
 *
 * This function implements MemSet with the same parameters as C90-memset
 * It Writes 32 bit data, if possible. So it is save to initialize structures which require consistent 32 bit data.
 *
 * Parameters[in]:  void* xDest_pv, sint32 xPattern_u32, uint32 numBytes_u32
 * Return :         void*
 ***********************************************************************************************************************
 */

void* rba_BswSrv_MemSet(void* xDest_pv, sint32 xPattern_s32, uint32 numBytes_u32)
{
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                      The warning is uncritical since the address range and proper alignment is ensured by the calculation done
                                      before in integer representation. */
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    uint64  adDest_uo = (uint64)xDest_pv;
    uint64  xPattern_uo = (uint8)xPattern_s32; /* only 8 bits must be used */
    uint64* xDest_pu64;
#else
    uint32  adDest_uo = (uint32)xDest_pv;
    uint32  xPattern_uo = (uint8)xPattern_s32; /* only 8 bits must be used */
#endif

    uint32* xDest_pu32;
    uint16* xDest_pu16;
    uint8*  xDest_pu8;
    uint32 ctLoop_u32;

    /* only lower 8 bit are used for initialization */
    xPattern_uo = xPattern_uo + (xPattern_uo <<  8u);
    xPattern_uo = xPattern_uo + (xPattern_uo << 16u);

#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    xPattern_uo = xPattern_uo + (xPattern_uo << 32u);
#endif

    /* 8 bit leading gap */
    if ((numBytes_u32 >= 1u) && ((adDest_uo & 0x01u) != 0u))
    {
        /* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                          The warning is uncritical since the address range and proper alignment is ensured by the calculation done
                                          before in integer representation. */
        xDest_pu8 = (uint8*)adDest_uo;
        numBytes_u32 -= 1u;
        *xDest_pu8 = (uint8)xPattern_uo;
        adDest_uo++;
    }

    /* 16 bit leading gap */
    if ((numBytes_u32 >= 2u) && ((adDest_uo & 0x02u) != 0u))
    {
        /* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                          The warning is uncritical since the address range and proper alignment is ensured by the calculation done
                                          befobamfre in integer representation. */
        xDest_pu16 = (uint16*)adDest_uo;
        numBytes_u32 -= 2u;
        *xDest_pu16 = (uint16)xPattern_uo;
        adDest_uo += 2u;
    }
#if(RBA_BSWSRV_CFG_MACHINE_SUPPORT_64_BIT == STD_ON)
    /* 32 bit leading gap */
    if ((numBytes_u32 >= 4u) && ((adDest_uo & 0x04u) != 0u))
    {
        /* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                          The warning is uncritical since the address range and proper alignment is ensured by the calculation done
                                          befobamfre in integer representation. */
        xDest_pu32 = (uint32*)adDest_uo;
        numBytes_u32 -= 4u;
        *xDest_pu32 = (uint32)xPattern_uo;
        adDest_uo += 4u;
    }


    /* 64 bit set */
	/* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
									 The warning is uncritical since the address range and proper alignment is ensured by the calculation done
									 before in integer representation. */
	xDest_pu64 = (uint64*)adDest_uo;
	if (numBytes_u32 >= 8u)
	{
	   ctLoop_u32 = numBytes_u32 / 8u;
	   numBytes_u32 &= 0x07u;
	   do
	   {
		   *xDest_pu64 = (uint64)xPattern_uo;
		   xDest_pu64++;
		   ctLoop_u32--;
	   } while(ctLoop_u32 > 0u);
	}
	/* To be used by 32 bit set section */
	/* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
	                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
	xDest_pu32 = (uint32*)xDest_pu64;

#else

    /* To be used by 32 bit set section */
    /* MR12 RULE 11.4,11.6 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16/uint32 type.
                                     The warning is uncritical since the address range and proper alignment is ensured by the calculation done
                                     before in integer representation. */
    xDest_pu32 = (uint32*)adDest_uo;
#endif

    /* 32 bit set */

    if (numBytes_u32 >= 4u)
    {
        ctLoop_u32 = numBytes_u32 / 4u;
        numBytes_u32 &= 0x03u;
        do
        {
            *xDest_pu32 = (uint32)xPattern_uo;
            xDest_pu32++;
            ctLoop_u32--;
        } while(ctLoop_u32 > 0u);
    }

    /* 16 bit set */
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required.*/
    xDest_pu16 = (uint16*)xDest_pu32;
    if (numBytes_u32 >= 2u)
    {
        numBytes_u32 -= 2u;
        *xDest_pu16 = (uint16)xPattern_uo;
        xDest_pu16++;
    }

    /* 8 bit set */
    /* MR12 RULE 11.3 VIOLATION: The warning is non-removable since casting is required to manipulate the data as uint8/uint16 type.
                                 The warning is uncritical since the data size is reduced and therefore the alignment is even better than required. */
    xDest_pu8 = (uint8*)xDest_pu16;
    if(numBytes_u32 > 0u)
    {
        *xDest_pu8 = (uint8)xPattern_uo;
    }
    return xDest_pv;
}

#define RBA_BSWSRV_STOP_SEC_CODE
#include "rba_BswSrv_MemMap.h"

#endif /*(RBA_BSWSRV_CFG_COMPILER_LIBRARY_SUPPORT == STD_OFF)*/

