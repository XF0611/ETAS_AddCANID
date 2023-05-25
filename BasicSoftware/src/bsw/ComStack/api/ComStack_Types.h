

/*
*****************************************************************************************************************
* Based on AUTOSAR_SWS_CommunicationStackTypes.pdf; AR4.2; Rev 2 and BSW_CodingGuidelines_Cur_v.1.10.pdf
*****************************************************************************************************************
*/
/* TRACE[SWS_COMTYPE_00016]: protected against multiple inclusion. */
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

/*TRACE[SWS_COMTYPE_00001]: include file structure */
#include "Std_Types.h"                  /* Standard AUTOSAR types */
#include "ComStack_Cfg.h"               /* ECU dependent part of ComStack Types */


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* TRACE[Rule BSW_VersionInfo_002]: Module Vendor Identifier */
/* TRACE[SRS_BSW_00374]  */
#define COMTYPE_VENDOR_ID 0x0006u

/* TRACE[Rule BSW_VersionInfo_003]: Module Identifier  */
/* TRACE[SRS_BSW_00374]  */
#define COMTYPE_MODULE_ID 0x00C4u


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/
/*TRACE[SWS_COMTYPE_00036]: PNCHandleType used to store the identifier of a partial network cluster.*/
typedef uint8   PNCHandleType;

/*TRACE[SWS_COMTYPE_00031]: TPParameterType-Specify the parameter to which the value has to be changed (TP_STMIN or TP_BS or TP_BC).*/
typedef enum
{
    TP_STMIN = 0x00,
    TP_BS    = 0x01,
    TP_BC    = 0x02
} TPParameterType;

/*TRACE[SWS_COMTYPE_00012]: BufReq_ReturnType-Variables*/
typedef enum
{
    BUFREQ_OK       = 0x00,
    BUFREQ_E_NOT_OK = 0x01,
    BUFREQ_E_BUSY   = 0x02,
    BUFREQ_E_OVFL   = 0x03
} BufReq_ReturnType;

/*TRACE[SWS_COMTYPE_00027]: TpDataStateType-Variables of this type shall be used to store the state of TP buffer.*/
typedef enum
{
    TP_DATACONF = 0x00,
    TP_DATARETRY = 0x01,
    TP_CONFPENDING = 0x02
} TpDataStateType;

/*TRACE[SWS_COMTYPE_00037]:RetryInfoType-Variables */
typedef struct
{
    TpDataStateType TpDataState;
    PduLengthType TxTpDataCnt;
} RetryInfoType;

/*TRACE[SWS_COMTYPE_00038]:NetworkHandleType-Variables*/
typedef uint8   NetworkHandleType;

/*TRACE[SWS_COMTYPE_00039]: IcomConfigIdType-IcomConfigIdType defines the configuration ID.*/
typedef uint8   IcomConfigIdType;

/*TRACE[SWS_COMTYPE_00040]: IcomSwitch_ErrorType-IcomSwitch_ErrorType*/
typedef enum
{
    ICOM_SWITCH_E_OK,
    ICOM_SWITCH_E_FAILED
} IcomSwitch_ErrorType;



/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/


/* COMSTACK_TYPES_H */
#endif
