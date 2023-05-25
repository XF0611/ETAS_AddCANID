/*********************************************************************************************************************
*
* Description:     DID services in ECU
* FileName:        EcuMgr_DIDServices.c
* Company:         ETAS GmbH (www.etas.com)
*
**********************************************************************************************************************
*
* COPYRIGHT RESERVED, Copyright ETAS GmbH 2019. All rights reserved, also regarding any disposal,
* exploitation, reproduction, editing, distribution, as well as in the event of applications
* for industrial property rights.
*
**********************************************************************************************************************
* History:
**********************************************************************************************************************
* Author        Date            Comment
**********************************************************************************************************************
RHT2HC          11/10/2020
**********************************************************************************************************************/
/*********************************************************************************************************************
* Includes
*********************************************************************************************************************/
#include "Fbl_DataM.h"
//#include "EcuMgr_Cfg_DownloadTarget.h"
#include "DID_FingerprintDataIdentifier.h"
#include "FBL_DataMCfg.h"

/*********************************************************************************************************************
* Local Defines
*********************************************************************************************************************/
#define NUMBER_OF_LOGICAL_BLOCK_TYPE 3U
#define DID_APPDATAFINGERPRINT_EACH_BLOCK_LEN 			(NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock/(uint8)NUMBER_OF_LOGICAL_BLOCK_TYPE)
#define DID_APPSOFTFINGERPRINT_EACH_BLOCK_LEN 			(NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationSoftwareFingerprintBlock/(uint8)NUMBER_OF_LOGICAL_BLOCK_TYPE)
#define DID_BOOTFINGERPRINT_EACH_BLOCK_LEN 			(NVM_CFG_NV_BLOCK_LENGTH_NV_BootSoftwareFingerprintBlock/(uint8)NUMBER_OF_LOGICAL_BLOCK_TYPE)

#define ECUMGR_UTIL_NUM_OF_MONTHS                      ((uint8)12)    /* Number of months each year */
#define ECUMGR_UTIL_START_POS_OF_YYYY_FORMAT           ((uint8)0)     /* Start Position of year information in Programming date format */
#define ECUMGR_UTIL_START_POS_OF_MM_FORMAT             ((uint8)2)     /* Start Position of month information in Programming date format */
#define ECUMGR_UTIL_START_POS_OF_DD_FORMAT             ((uint8)3)     /* Start Position of day information in Programming date format */
/*********************************************************************************************************************
* Local Macros
*********************************************************************************************************************/
#define ECUMGR_UTIL_CHECK_BCD_FORMAT(data)             ((((data) >> (uint8)4) < (uint8)10) && (((data) & (uint8)0x0F) < (uint8)10))  /* Check this byte is BCD format */
#define ECUMGR_UTIL_CONVERT_BCD_TO_UINT16(data)        ((((data) >> (uint16)4) * (uint16)10) + ((data) & (uint16)0x0F))                 /* Convert BCD to uint16, E.g: 0x85 to 85 */
/*********************************************************************************************************************
* Local Types
*********************************************************************************************************************/

/*********************************************************************************************************************
* Local Variables
*********************************************************************************************************************/
static uint8 DID_ApplicationDataFingerPrintInfor[DID_APPDATAFINGERPRINT_EACH_BLOCK_LEN];                                    /* PRQA S 3218 */
//static uint8 DID_FingerprintInfor[DID_APPDATAFINGERPRINT_EACH_BLOCK_LEN];
//static uint8 DID_ApplicationSoftwareFingerPrintInfor[DID_APPSOFTFINGERPRINT_EACH_BLOCK_LEN];
//static uint8 DID_BootSoftwareFingerPrintInfor[DID_BOOTFINGERPRINT_EACH_BLOCK_LEN];
/*********************************************************************************************************************
* Local Functions Implementation
*********************************************************************************************************************/
Std_ReturnType EcuMgr_Util_IsDateFormatValid(const uint8* date, uint8 len);                                 /* PRQA S 3447 *//* PRQA S 3451 */
Std_ReturnType EcuMgr_Util_IsDateValid(uint16 year, uint16 month, uint16 day);                                  /* PRQA S 3447 */
Std_ReturnType EcuMgr_Util_ConvertBCD2Uint16(const uint8* data, uint8 len, uint16* CvtVal);                         /* PRQA S 3447 */

/*********************************************************************************************************************
* Global Functions Implementation
*********************************************************************************************************************/

/**
 * @brief: Store Application Data Finger Printer information
 *
 * @return Std_ReturnType
 * 							- E_OK: Date is valid
 * 							- E_NOT_OK: Date is invalid
 */
Std_ReturnType DIDService_StoreApplicationDataFingerPrintInfor(const uint8* data)                       /* PRQA S 3408 */
{
	Std_ReturnType retVal = E_NOT_OK;
	uint32 idx = 0;

	/* Check the date format is valid or not */
	retVal = EcuMgr_Util_IsDateFormatValid(data, ECUMGR_UTIL_LEN_OF_YYMMDD_FORMAT);

	/* Store Finger print information */
	if(retVal == E_OK)
	{
		for(idx = 0; idx < sizeof(DID_ApplicationDataFingerPrintInfor); ++idx)
		{
			DID_ApplicationDataFingerPrintInfor[idx] = data[idx];
		}
	}

	return retVal;
}

/**
 * @brief: Read Finger Print information from Non-volatile Memory
 * 
 * @return Std_ReturnType 
 */

Std_ReturnType DIDService_ReadFingerPrintInfor(FingerprintType Finger,uint8* data)
{
	Std_ReturnType retVal = E_NOT_OK;
	uint8 fingerPrintData[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock] = {0};
	uint32 blkIdx = 0;
	uint32 fpIdx = 0;

	switch (Finger)
	{
	case ApplicationDataFingerprint:
		retVal = FBL_DataM_ReadApplicationDataNvmFingerprint(fingerPrintData);
		if(retVal == E_OK)
		{
			for(blkIdx = 0; blkIdx < (uint32)NUMBER_OF_LOGICAL_BLOCK_TYPE; ++blkIdx)
			{
				*data = (uint8)blkIdx;
				++data;
				for(fpIdx = 0; fpIdx < DID_APPDATAFINGERPRINT_EACH_BLOCK_LEN; ++fpIdx)
				{
					*data = fingerPrintData[(blkIdx*DID_APPDATAFINGERPRINT_EACH_BLOCK_LEN) + fpIdx];
					++data;
				}
			}
		}
		break;
	case ApplicationSoftwareFingerprint:
		retVal = FBL_DataM_ReadApplicationSoftwareNvmFingerprint(fingerPrintData);
		if(retVal == E_OK)
		{
			for(blkIdx = 0; blkIdx < (uint32)NUMBER_OF_LOGICAL_BLOCK_TYPE; ++blkIdx)
			{
				*data = (uint8)blkIdx;
				++data;
				for(fpIdx = 0; fpIdx < DID_APPSOFTFINGERPRINT_EACH_BLOCK_LEN; ++fpIdx)
				{
					*data = fingerPrintData[(blkIdx*DID_APPSOFTFINGERPRINT_EACH_BLOCK_LEN) + fpIdx];
					++data;
				}
			}
		}
		break;
    case BootSoftwareFingerprint:
        retVal = FBL_DataM_ReadBootSoftwareNvmFingerprint(fingerPrintData);
        if(retVal == E_OK)
        {
            for(blkIdx = 0; blkIdx < (uint32)NUMBER_OF_LOGICAL_BLOCK_TYPE; ++blkIdx)
			{
				*data = (uint8)blkIdx;
				++data;
				for(fpIdx = 0; fpIdx < DID_BOOTFINGERPRINT_EACH_BLOCK_LEN; ++fpIdx)
				{
					*data = fingerPrintData[(blkIdx*DID_BOOTFINGERPRINT_EACH_BLOCK_LEN) + fpIdx];
					++data;
				}
			}
        }
        break;
	default:
		break;
	}
	return retVal;
}

/**
 * @brief Write Finger Print to NVM 
 * 
 * @param Finger 
 * @param data 
 * @return Std_ReturnType 
 */

Std_ReturnType DIDService_WriteFingerprint(FingerprintType Finger,const uint8* data){
	Std_ReturnType retVal = E_NOT_OK;
	uint8 fingerPrintData[NVM_CFG_NV_BLOCK_LENGTH_NV_ApplicationDataFingerprintBlock] = {0};
	switch (Finger)
	{
		case ApplicationDataFingerprint:
			retVal = FBL_DataM_ReadApplicationDataNvmFingerprint(fingerPrintData);
			if(retVal == E_OK)
			{
				retVal = FBL_DataM_WriteApplicationDataNvmFingerprint(data);
			}
			break;
		case ApplicationSoftwareFingerprint:
			retVal = FBL_DataM_ReadApplicationSoftwareNvmFingerprint(fingerPrintData);
			if(retVal == E_OK)
			{
				retVal = FBL_DataM_WriteApplicationSoftwareNvmFingerprint(data);
			}
			break;
        case BootSoftwareFingerprint:
        	retVal = FBL_DataM_ReadBootSoftwareNvmFingerprint(fingerPrintData);
			if(retVal == E_OK)
			{
				retVal = FBL_DataM_WriteBootSoftwareNvmFingerprint(data);
			}
			break;
		default:
			retVal = E_NOT_OK;
	}                                                                                                           /* PRQA S 2020 */
	return retVal;
}

/**
 * @brief support func
 * 
 */

Std_ReturnType EcuMgr_Util_IsDateFormatValid(const uint8* date, uint8 len)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint8   fullDateFormat[ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT] = {0, 0, 0, 0};
    uint8   idx = 0;
    uint16  year = 0;
    uint16  month = 0;
    uint16  day = 0;

    if(((uint8)ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT == len) || ((uint8)ECUMGR_UTIL_LEN_OF_YYMMDD_FORMAT == len))
    {
        /* Full fill format data of date.
         * This action is necessary in case format if date is YYMMDD (3 bytes) */
        for(idx = (uint8)ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT - len; idx < (uint8)ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT; idx++)
        {
            fullDateFormat[idx] = date[idx - (uint8)ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT + len];
        }

        /* Calculate year */
        retVal = EcuMgr_Util_ConvertBCD2Uint16((const uint8*)fullDateFormat, ECUMGR_UTIL_START_POS_OF_MM_FORMAT - ECUMGR_UTIL_START_POS_OF_YYYY_FORMAT, &year);

        /* Calculate month */
        retVal = (retVal == E_OK) ? EcuMgr_Util_ConvertBCD2Uint16((const uint8*)(&fullDateFormat[ECUMGR_UTIL_START_POS_OF_MM_FORMAT]), ECUMGR_UTIL_START_POS_OF_DD_FORMAT - ECUMGR_UTIL_START_POS_OF_MM_FORMAT, &month) : retVal;

        /* Calculate day */
        retVal = (retVal == E_OK) ? EcuMgr_Util_ConvertBCD2Uint16((const uint8*)(&fullDateFormat[ECUMGR_UTIL_START_POS_OF_DD_FORMAT]), (uint8)ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT - ECUMGR_UTIL_START_POS_OF_DD_FORMAT, &day) : retVal;

        /* Check date is valid */
        retVal = (retVal == E_OK) ? EcuMgr_Util_IsDateValid(year, month, day) : retVal;
    }

    return retVal;
}


Std_ReturnType EcuMgr_Util_IsDateValid(uint16 year, uint16 month, uint16 day)
{
    Std_ReturnType retVal = E_NOT_OK;
    uint16    maxDay = 0;

    /* Indicate maximum number of days each month */
    static const uint16 EcuMgr_Util_MaxNumOfDaysEachMonthTbl[ECUMGR_UTIL_NUM_OF_MONTHS] =
    {
        31,     /* January */
        29,     /* February - It shall be 28 if this year is not Leap year*/
        31,     /* March */
        30,     /* April */
        31,     /* May */
        30,     /* June */
        31,     /* July */
        31,     /* August */
        30,     /* September */
        31,     /* October */
        30,     /* November */
        31      /* December */
    };

    /* In case this month is February */
    if((uint16)2 == month)
    {
        /* Check this year is Leap year */
        if ((((uint16)0 == (year % (uint16)4))  && ((year % (uint16)100) != (uint16)0))
                || ((uint16)0 == (year % (uint16)400)))
        {
            maxDay = EcuMgr_Util_MaxNumOfDaysEachMonthTbl[month - (uint16)1];
        }
        else
        {
            maxDay = EcuMgr_Util_MaxNumOfDaysEachMonthTbl[month - (uint16)1] - 1u;
        }
    }
    /* In case month is valid */
    else if((month <= ECUMGR_UTIL_NUM_OF_MONTHS) && (month > (uint16)0))
    {
        maxDay = EcuMgr_Util_MaxNumOfDaysEachMonthTbl[month - (uint16)1];
    }
    else
    {
        /* Do nothing */
    }

    if((maxDay >= day) && (day > 0u))
    {
        retVal = E_OK;
    }

    return retVal;
}

Std_ReturnType   EcuMgr_Util_ConvertBCD2Uint16(const uint8* data, uint8 len, uint16* CvtVal)
{
    Std_ReturnType retVal = E_OK;
    uint8   idx = 0;

    *CvtVal = 0;

    for(idx = 0; idx < len; idx++)
    {
        /* Check this bytes is BCD format */
        if(ECUMGR_UTIL_CHECK_BCD_FORMAT(data[idx]))
        {
            /* Calculate convert value. */
            *CvtVal = (*CvtVal * (uint16)100) + (uint16)(ECUMGR_UTIL_CONVERT_BCD_TO_UINT16((uint16)data[idx]));
        }
        else
        {
            retVal = E_NOT_OK;
            break;
        }
    }

    return retVal;
}
