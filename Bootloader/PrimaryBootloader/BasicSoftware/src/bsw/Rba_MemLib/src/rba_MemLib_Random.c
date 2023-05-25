

#include "rba_MemLib.h"


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_8
#include "rba_MemLib_MemMap.h"

static uint8 rba_MemLib_idxRandom;

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_8
#include "rba_MemLib_MemMap.h"


/****/

#define  RBA_MEMLIB_START_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"

static uint32 rba_MemLib_nrRandom[16];

#define  RBA_MEMLIB_STOP_SEC_VAR_CLEARED_32
#include "rba_MemLib_MemMap.h"



/*
 **********************************************************************************************************************
 * Code
 **********************************************************************************************************************
*/
#define  RBA_MEMLIB_START_SEC_CODE
#include "rba_MemLib_MemMap.h"

/**
 *********************************************************************
 * rba_MemLib_GetRandom_u32():
 * Function returns a 32bit pseudo-random number
 * Algo used: WELL 512
 *
 * If nrRange_u16=0: full 32bit range of result is used
 * else random value is scaled to a interval [0 ; nrRange_u16-1].
 *
 * Note: initial seeding is required
 *
 * \param   nrRange_u16 result range selection (see above)
 * \retval  32bit random value
 *********************************************************************
 */
uint32 rba_MemLib_GetRandom_u32(uint16 nrRange_u16)
{
    uint32 Loc_a, Loc_b, Loc_c, Loc_d;

    Loc_a  = rba_MemLib_nrRandom[rba_MemLib_idxRandom];
    Loc_c  = rba_MemLib_nrRandom[(rba_MemLib_idxRandom + 13u) & 15u];
    Loc_b  = Loc_a ^ Loc_c ^ (Loc_a<<16u) ^ (Loc_c<<15u);
    Loc_c  = rba_MemLib_nrRandom[(rba_MemLib_idxRandom + 9u) & 15u];
    Loc_c ^= (Loc_c>>11u);
    Loc_a = Loc_b^Loc_c;
    rba_MemLib_nrRandom[rba_MemLib_idxRandom] = Loc_a;

    Loc_d = Loc_a ^ ((Loc_a<< 5u) & 0xDA442D24uL);

    rba_MemLib_idxRandom = (rba_MemLib_idxRandom + 15u) & 15u;
    Loc_a = rba_MemLib_nrRandom[rba_MemLib_idxRandom];
    rba_MemLib_nrRandom[rba_MemLib_idxRandom] = Loc_a ^ Loc_b ^ Loc_d ^ (Loc_a<<2u) ^ (Loc_b<<18u) ^ (Loc_c<<28u);

    /* Random result in 32bit range */
    Loc_a=rba_MemLib_nrRandom[rba_MemLib_idxRandom];

    /* result scaling necessary? */
    if (nrRange_u16 !=0u)
    {
        /* only the higher 16bit are considered in this mode */
        Loc_b = Loc_a >>16u;
        Loc_a = (Loc_b * (uint32)(nrRange_u16)) >>16u;
    }

    return Loc_a;
}


/**
 *********************************************************************
 * rba_MemLib_SetRandomSeed():
 * Function initializes the pseudo-random number
 *
 * \param   32bit random seed value
 * \retval  none
 *********************************************************************
 */
void rba_MemLib_SetRandomSeed(uint32 nrSeed_u32)
{
    uint32 idx;
    uint32 Loc_a;

    /* Handling of special parameter value
     * Note: 0 should be avoided */
    if (0uL == nrSeed_u32)
    {
        nrSeed_u32 = 0x12345678uL;
    }

    /* First step: seeding */
    for (idx=0u;idx<16uL;idx++)
    {
        rba_MemLib_nrRandom[idx] = nrSeed_u32;
        nrSeed_u32 = nrSeed_u32 * 9uL;   // ignore overflow
    }

    /* Second step: perform initial dummy reads */
    Loc_a = rba_MemLib_GetRandom_u32(16u)+16u;

    /* Perform a sequence of random reads to improve the initial randomness */
    for (idx=0u;idx<Loc_a;idx++)
    {
        (void)rba_MemLib_GetRandom_u32(0u);
    }

    return;
}


/**
 *********************************************************************
 * rba_MemLib_IsRandomSeeded():
 * Determine whether the random generated is seeded already
 *              return=FALSE (=not seeded)
 * \param   none
 * \retval  Seeding status
 *********************************************************************
 */
boolean rba_MemLib_IsRandomSeeded(void)
{
    boolean flag=FALSE;

    flag = (boolean)(flag || (rba_MemLib_nrRandom[0u] !=0uL));
    flag = (boolean)(flag || (rba_MemLib_nrRandom[3u] !=0uL));

    return(flag);
}

#define  RBA_MEMLIB_STOP_SEC_CODE
#include "rba_MemLib_MemMap.h"
