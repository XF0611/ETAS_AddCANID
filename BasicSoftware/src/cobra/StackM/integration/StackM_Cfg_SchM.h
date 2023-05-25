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
 * Component : StackM_Cfg_SchM.h
 * Date      : Dec 29, 2022
 * Version   : 2.0
 * Description  : This module implements Stack monitor functions.
 *                Note: This file contains sample implementation only. It is not part of the
 *                      production deliverables. 
 ***********************************************************************************************
*/

#ifndef STACKM_CFG_SCHM_H
#define STACKM_CFG_SCHM_H

/*! \file
 *
 * \brief Stack Monitor Exclusive area definition.
 *
 * This file contains the definition of the component to be connected by user during softwre integration.
 */

/*
 ***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************
 */
#include "SchM_Default.h"


/*
 ***********************************************************************************************************************
 * Preprocessor defines
 ***********************************************************************************************************************
 */
/* Mapping with the locks:
    - single core system: 
	  please map the SchM_Enter_StackM_Init()/SchM_Exit_StackM_Init()/SchM_Enter_StackM_Scan()/SchM_Exit_StackM_Scan() 
      macros to the system interrupt lock APIs
    - multi core system:
	  please map the SchM_Enter_StackM_Init()/SchM_Exit_StackM_Init()/SchM_Enter_StackM_Scan()/SchM_Exit_StackM_Scan()
      macros to the system spin lock APIs
*/
#define SchM_Enter_StackM_Init()       SCHM_ENTER_DEFAULT()
#define SchM_Exit_StackM_Init()        SCHM_EXIT_DEFAULT()

/**
 * @brief ETAS BIP Porting RTABIP-77: Merge one software build
 */
#if !defined(TDA4VM_R5FSS)
#define SchM_Enter_StackM_Scan()       SCHM_ENTER_DEFAULT()
#define SchM_Exit_StackM_Scan()        SCHM_EXIT_DEFAULT()
#else
#define SchM_Enter_StackM_Scan()       /* No SchM shall be used. */
#define SchM_Exit_StackM_Scan()        /* No SchM shall be used. */
#endif


#endif /* STACKM_CFG_SCHM_H */

/*
 ***********************************************************************************************************************
 * End of header file
 ***********************************************************************************************************************
*/

