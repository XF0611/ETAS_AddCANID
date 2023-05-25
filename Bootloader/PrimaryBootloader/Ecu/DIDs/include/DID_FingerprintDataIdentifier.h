#ifndef FBL_INFRA_ECUMGR_INCLUDE_ECUMGR_DIDSERVICES_H_
#define FBL_INFRA_ECUMGR_INCLUDE_ECUMGR_DIDSERVICES_H_

/*******************************************************************************
**                      Includes
*******************************************************************************/
#include "Std_Types.h"


/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
#define B(x) str_to_binary_(#x)
#define SET_BIT(var, bit)                     {(var) |= (uint8)(bit) ;} 
#define CLEAR_BIT(var, bit)                   {(var) &= (uint8)(~(bit));}
#define IS_BIT_SET(var, bit)                  ((((var) & (bit))!=0u) ? TRUE:FALSE)
/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
#define ECUMGR_UTIL_LEN_OF_YYMMDD_FORMAT       (3)
#define ECUMGR_UTIL_LEN_OF_YYYYMMDD_FORMAT     (4)

typedef enum{
  ApplicationDataFingerprint = 0,
  ApplicationSoftwareFingerprint,
  BootSoftwareFingerprint
}FingerprintType;

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
 * @brief: Store Finger Printer information
 *
 * @return Std_ReturnType
 * 							- E_OK: Date is valid
 * 							- E_NOT_OK: Date is invalid
 */
extern Std_ReturnType DIDService_WriteFingerprint(FingerprintType Finger,const uint8* data);
extern Std_ReturnType DIDService_ReadFingerPrintInfor(FingerprintType Finger,uint8* data);


/**
 * @brief: Write Finger Printer information to Non-Volatile memory
 *
 * @return Std_ReturnType
 * 							- E_OK: Successfully
 * 							- E_NOT_OK: Fail
 */


//extern Std_ReturnType EcuMgr_DIDService_WriteBootSoftwareFingerPrintInfor(uint32 blockID);

/**
 * @brief: Read Finger Printer information to Non-Volatile memory
 *
 * @return Std_ReturnType
 * 							- E_OK: Successfully
 * 							- E_NOT_OK: Fail
 */
/**
 * @brief 
 * 
 */

extern Std_ReturnType EcuMgr_Util_IsDateFormatValid(const uint8* date, uint8 len);


/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
LOCAL_INLINE unsigned long long str_to_binary_(const char *s)
{
  unsigned long long i = 0u;
  while (*s)
  {
    i <<= 1;
    i += *s++ - '0';
  }
  return i;
}

#endif /* FBL_INFRA_ECUMGR_INCLUDE_ECUMGR_DIDSERVICES_H_ */
