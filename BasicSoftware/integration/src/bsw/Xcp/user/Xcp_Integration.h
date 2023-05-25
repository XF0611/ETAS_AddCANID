/*
 **********************************************************************************************************************
 *
 * COPYRIGHT RESERVED, ETAS GmbH, 2021. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 **********************************************************************************************************************
 ************************************************************************************************
 * Component : Xcp_Integration.h
 * Date      : Jan 16 2020
 * Version   : 1.0
 * Description  : This module implements XCP integration definitions and declarations.
 ***********************************************************************************************
*/
#ifndef XCP_INTEGRATION_H
#define XCP_INTEGRATION_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Target.h"
#include "OvrlyM.h"


/*
 **********************************************************************************************************************
 *  Type Defines
 **********************************************************************************************************************
 */
typedef enum
{
#if (CAL_MEMORY_MGNT == CAL_MEMORY_ONE_PAGE)
    CalPage_Working = 0,
#elif (CAL_MEMORY_MGNT == CAL_MEMORY_TWO_PAGE)
    CalPage_Reference = 0,
    CalPage_Working,
#else
    #error "incorrect memory management pages configured. Please check!"
#endif
	NUM_OF_CAL_PAGE
} CalPage_T;


/*
 **********************************************************************************************************************
 *  Defines
 **********************************************************************************************************************
 */
/**
 * @brief ETAS BIP Integration Guide: 6.3.6	Xcp
 * Customer Integration Required.
 * Please see the integration guide section for further details.
 * [$Satisfies $BIP_INTG 113]
 */
#if (CAL_MEMORY_MGNT == CAL_MEMORY_TWO_PAGE)
#define XCP_START_FLASH_ADDRESS				/* Start address of XCP on the flash */
#define	XCP_MASK_VALUE						0xF80U			/* The mask value of OMASK register */
#define XCP_OTAR_ADDRESS_MASK				0x0FFFFFE0U		/* The mask to set address of OTAR register */
#define XCP_RABR_ADDRESS_MASK				0x003FFFE0U		/* The mask to set address of RABR register */
#define	XCP_RABR_ENABLE_MASK				0x81000000U		/* The mask to enable OVEN bit and detect redirection to Core 1 DSPR/PSPR memory of RABR register */
#endif

#define XCP_START_RAM_ADDRESS				0x007FA000U		/* Start address of XCP on the RAM */
#define XCP_BLOCK_SIZE						0x4000U		    /* Size of each description block */
#define XCP_TOTAL_SIZE						0x4000U		    /* Total size of XCP on the Flash (= Total size of XCP on the RAM) */
#define XCP_NUMBER_DES_BLOCK				(XCP_TOTAL_SIZE / XCP_BLOCK_SIZE)	/* Number of description blocks */


/*
 **********************************************************************************************************************
 * Public Function declarations
 **********************************************************************************************************************
 */
#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"
extern CalPage_T XcpCurrentCalPage;
#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Xcp_MemMap.h"


#endif /* XCP_INTEGRATION_H */
/*
 **********************************************************************************************************************
 * End of the file
 **********************************************************************************************************************
 */
