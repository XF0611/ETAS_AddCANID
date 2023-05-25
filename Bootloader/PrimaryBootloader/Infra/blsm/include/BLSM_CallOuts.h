/******************************************************************************
**                                                                           **
*******************************************************************************
**                                                                           **
**  $FILENAME   : BSLM_CallOuts.h $                                               **
**                                                                           **
**  $CC VERSION : \main\6 $                                                  **
**                                                                           **
**  $DATE       : 2019-08-07 $                                               **
**                                                                           **
**  AUTHOR      : RBVH-AUTOSAR-Engineering                                   **
**                                                                           **
**                                                                           **
**  DESCRIPTION  : This file implements the instance of MCU. User can modify **
**                  when want to use on another hardware platform.           **
**  MAY BE CHANGED BY USER [yes/no]: yes                                     **
**                                                                           **
******************************************************************************/

#ifndef     BLSM_CALLOUTS_H
#define     BLSM_CALLOUTS_H

/*******************************************************************************
**                      Includes
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

/**
 * @brief the first initialization of BLSM which is to initialise low level of MCU.
 * @return none
 *
 */
extern void Fbl_BLSM_InitOne(void);

/**
 * @brief the second initialization of BLSM which is to initialise the communication
 *          and its routine.
 * @return none
 */
extern void Fbl_BLSM_InitTwo(void);

/**
 * @brief the third initialization of BLSM which is to initialise UDS service and its dependencies.
 * @return none.
 */
extern void Fbl_BLSM_InitThree(void);

/**
 * @brief initialise bootloader Appplication.
 * @return None
 *
 */
extern void Fbl_BLSM_Initialize_App(void);

/**
 * @brief Jump to application.
 * @return None
 *
 */
extern void Fbl_BLSM_Jump2App(void);


#ifdef DEVELOPMENT
/**
 * @brief 
 * 
 */
extern void BLSM_StayInBootMonitorMainFunction(void);

/**
 * @brief 
 * 
 */
extern void BLSM_EnableStayInBoot(void);

/**
 * @brief 
 * 
 */
extern void BLSM_IndicatedStayInBoot(void);
#endif

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif

