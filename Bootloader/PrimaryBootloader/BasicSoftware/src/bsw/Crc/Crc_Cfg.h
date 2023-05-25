

#ifndef CRC_CFG_H
#define CRC_CFG_H



/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
* Defines/Macros
**********************************************************************************************************************
*/

/* Version information parameters */
#define CRC_VENDOR_ID                   (6U)
#define CRC_MODULE_ID                   (201U)
#define CRC_SW_MAJOR_VERSION            (8U)
#define CRC_SW_MINOR_VERSION            (0U)
#define CRC_SW_PATCH_VERSION            (0U)
#define CRC_AR_RELEASE_MAJOR_VERSION    (4U)
#define CRC_AR_RELEASE_MINOR_VERSION    (2U)

#define CRC_INITIAL_VALUE8              (0xFFU)
#define CRC_INITIAL_VALUE8H2F           (0xFFU)
#define CRC_INITIAL_VALUE16             (0xFFFFU)
#define CRC_INITIAL_VALUE32             (0xFFFFFFFFUL)
#define CRC_INITIAL_VALUE32P4           (0xFFFFFFFFUL)
#define CRC_INITIAL_VALUE64             (0xFFFFFFFFFFFFFFFFULL)

#define CRC_XOR_VALUE8                  (0xFFU)
#define CRC_XOR_VALUE8H2F               (0xFFU)
#define CRC_XOR_VALUE16                 (0U)
#define CRC_XOR_VALUE32                 (0xFFFFFFFFUL)
#define CRC_XOR_VALUE32P4               (0xFFFFFFFFUL)
#define CRC_XOR_VALUE64                 (0xFFFFFFFFFFFFFFFFULL)

#define  CRC_JUNK                       (8U)

#define  CRC_16_JUNKREST                (8U)

#define  CRC_TABLESIZE                  (256U)

#define  CRC_TABLEMASK                  (0xFFU)

#define  CRC_8_POLYNOMIAL               (0x1dU)                 /* CRC-SAE-J1850 */
#define  CRC_8H2F_POLYNOMIAL            (0x2fU)                 /* CRC-SAE-J1850 */
#define  CRC_16_POLYNOMIAL              (0x1021U)               /* CRC16-CCITT polynomial*/
#define  CRC_32_REV_POLYNOMIAL          (0xEDB88320UL)           
#define  CRC_32P4_REV_POLYNOMIAL        (0xC8DF352FUL)           
#define  CRC_64_REV_POLYNOMIAL          (0xC96C5795D7870F42ULL)  

#define  CRC_8_BITMASK                  (0x80U)
#define  CRC_8H2F_BITMASK               (0x80U)
#define  CRC_16_BITMASK                 (0x8000U)
#define  CRC_32_BITMASK                 (1U)
#define  CRC_32P4_BITMASK               (1U)
#define  CRC_64_BITMASK                 (1ULL)

#define  CRC_HW                 CRC_HW_UNKNOWN

/*
 * Revision of CRC
 */
 
#define CRC_AR_RELEASE_REVISION_VERSION  2

#define CRC_INLINE  LOCAL_INLINE 

/*
 * Versioninfo macro
 */
#define  CRC_VERSION_INFO_API  STD_ON


/*
**********************************************************************************************************************
* Variables
**********************************************************************************************************************
*/


/*
**********************************************************************************************************************
* Extern declarations
**********************************************************************************************************************
*/



#define CRC_START_SEC_CONST_16
#include "Crc_MemMap.h"

extern const uint16 CRC_16_Tbl[CRC_TABLESIZE];

#define CRC_STOP_SEC_CONST_16
#include "Crc_MemMap.h"


#define CRC_START_SEC_CONST_32
#include "Crc_MemMap.h"

extern const uint32 CRC_32_REV_Tbl[CRC_TABLESIZE];

#define CRC_STOP_SEC_CONST_32
#include "Crc_MemMap.h"



#endif /* CRC_CFG_H */


