/*********************************************************************************************************************
*
* Description:     
* FileName:        DID_PostSoftwareDownloadStatus_0xD05B.h
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2021. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
UCN1HC          2021/25/12      initial commit
**********************************************************************************************************************/


/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Dcm.h"
/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define DID_D05B_LAST_DOWNLOAD_SUCCESS                  0x00U
#define DID_D05B_LAST_DOWNLOAD_INCOMPLETE               0x01U
#define DID_D05B_FAILED_SIGNATURE_CHECK                 0x02U
#define DID_D05B_SOFTWARE_VERSION_MISMATCH              0x03U
#define DID_D05B_CHECK_VALID_APP_NOT_RUN                0x04U
#define DID_D05B_DOWNLOAD_INPROGRESS_REMOTE_SLAVE       0x05U   //NA
#define DID_D05B_LAST_DOWNLOAD_INCOMPLETE_REMOTE_SLAVE  0x06U   //NA
#define DID_D05B_SOFTWARE_VERSION_MISMATCH_REMOTE_SLAVE 0x07U   //NA
#define DID_D05B_WAIT_VALIDATE_CONFIGURATION            0x08U   //NA
#define DID_D05B_VALIDATING_CONFIGURATION               0x09U   //NA
#define DID_D05B_FAILED_VALIDATE_CONFLICT_CONFIGURATION 0x0AU   
#define DID_D05B_FAILED_VALIDATE_HARDWARE_MISMATCH      0x10U  
#define DID_D05B_FAILED_VALIDATE_INVALID_PARAMETER      0x11U          
#define DID_D05B_CONFIGURE                                  0x12U
#define DID_D05B_FAILED_CONFIGURE_NOT_SUPPORTED             0x13U
#define DID_D05B_FAILED_CONFIGURE_SOFTWARE_EXECUTION_ERROR  0x14U
#define DID_D05B_WAITING_ACTIVATION                         0x15U
#define DID_D05B_REACH_MAX_NUMBER_REPROGRAM_CYCLE           0x16U
#define DID_D05B_FIRST_RUN_TEST_FAILED_COMPATIBILITY        0x17U
#define DID_D05B_INSTALLATION_INPROGRESS                    0x18U
#define DID_D05B_INSTALLATION_SUCCESS                       0x19U
#define DID_D05B_VALIDATE_SUCCESS_WAIT_SWITCH_BANK          0x1AU
#define DID_D05B_VALIDATE_FAILED                            0x1BU
#define DID_D05B_SWITCH_BANK_SUCCESS_PENDING_ACTIVATE       0x20U
#define DID_D05B_SWITCH_BANK_FAILED                         0x21U
#define DID_D05B_SWITCH_BANK_INPROGRESS                     0x22U
#define DID_D05B_FIRST_RUN_TEST_SUCCESS                     0x30U
#define DID_D05B_FIRST_RUN_TEST_INPROGRESS                  0x31U
#define DID_D05B_FIRST_RUN_TEST_FAILED                      0x32U
#define DID_D05B_COMMIT_SUCCESS                             0x40U
#define DID_D05B_COMMIT_FAILED                              0x41U
#define DID_D05B_COMMIT_INPROGRESS                          0x42U
#define DID_D05B_ROLLBACK_SUCCESS_PENDING_REVERT            0x50U
#define DID_D05B_ROLLBACK_FAILED                            0x51U
#define DID_D05B_ROLLBACK_INPROGRESS                        0x52U
#define DID_D05B_TRIAL_EXPIRED_PENDING_REVERT               0x60U
#define DID_D05B_SYNC_PENDING                               0x70U
#define DID_D05B_SYNC_INPROGRESS                            0x71U
#define DID_D05B_SYNC_FAILED                                0x72U
#define DID_D05B_UNDEFINED                                  0xFFU

/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Prototypes
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Variables Implementation
*********************************************************************************************************************/

/*********************************************************************************************************************
* Exported Functions Implementation
*********************************************************************************************************************/

/**
 * @brief 
 * 
 * D05B - State Encoded - 1 Byte
00-Last Software Download Completed Sucessfully 
01-Last Software Download Incomplete 
02-Failed Signature Check 
03-Software Version Mismatch Between Control Modules 
04-Check Valid Application Not Run 
05-Software Download In Progress Between Control Module And Remote Slave Node(s) 
06-Last Software Download to Remote Slave Node(s) Incomplete 
07-Software Version Mismatch Between Control Module and Remote Slave Node(s) 
08-Waiting to validate configuration 
09-Validating Configuration 
0A-Failed To Validate: Conflict Between Values Of Configuration Parameters 
10-Failed To Validate: Installed Hardware Mismatch With Configuration Parameters 
11-Failed To Validate: Configuration Has One Or More Invalid Parameter Values 
12-Configure 
13-Fail to configure: Configuration not supported by software 
14-Fail to configure: Software Execution Error 
15-Waiting Activation 
16-Maximum number of reprogramming cycles has been reached
17-Failed To Validate: Software and Hardware Compatibility 
18-Installation In Progress 
19-Installation Successful 
1A-Validation Successful, Waiting Switch Bank 
1B-Validation Unsuccessful 
20-Switch Bank Successful,Pending Activation 
21-Switch Bank Unsuccessful 
22-Switch Bank In Progress 
30-First Run Test Successful 
31-First Run Test In Progress 
32-First Run Test Unsuccessful 
40-Commit Successful 
41-Commit Unsuccessful 
42-Commit In Progress 
50-Roll back Successful, Pending Roll back Activation 
51-Roll back Unsuccessful 
52-Roll back In Progress 
60-Trial Period Expired, Pending Roll back Activation 
70-Sync Pending 
71-Sync In Progress 
72-Sync Unsuccessful 
FF-Undefined 
 * @param Address 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DID_PostSoftwareDownloadStatus_0xD05B_WriteFunc(uint8 Data);
/**
 * @brief 
 * 
 * @param OpStatus 
 * @param Data 
 * @return Std_ReturnType 
 */
extern Std_ReturnType DID_PostSoftwareDownloadStatus_0xD05B_ReadFunc (Dcm_OpStatusType OpStatus,uint8 * Data);

