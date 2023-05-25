#ifndef SBL_FLASH_H
#define SBL_FLASH_H

#include "Std_Types.h"
/*Access macro*/

#define FLASH_DRIVER_INIT_OFFSET    0x00000000 /* offset from start address */
#define FLASH_DRIVER_DEINIT_OFFSET  0x00000004 /* offset from start address */
#define FLASH_DRIVER_ERASE_OFFSET   0x00000008 /* offset from start address */
#define FLASH_DRIVER_WRITE_OFFSET   0x0000000C /* offset from start address */
#define FLASH_DRIVER_READ_OFFSET    0x00000010 /* offset from start address */


/* sa holds flash driver start address, pp contains pointer the pointer to the parameter structure */
#define FLASH_DRIVER_INIT(sa, pp)   (*(FlashFct*) ((sa) + FLASH_DRIVER_INIT_OFFSET))(pp)
#define FLASH_DRIVER_DEINIT(sa, pp) (*(FlashFct*) ((sa) + FLASH_DRIVER_DEINIT_OFFSET))(pp)
#define FLASH_DRIVER_ERASE(sa, pp)  (*(FlashFct*) ((sa) + FLASH_DRIVER_ERASE_OFFSET))(pp)
#define FLASH_DRIVER_WRITE(sa, pp)  (*(FlashFct*) ((sa) + FLASH_DRIVER_WRITE_OFFSET))(pp)
#define FLASH_DRIVER_READ(sa, pp)   (*(FlashFct*) ((sa) + FLASH_DRIVER_READ_OFFSET))(pp)

/* this macro is preresented for a specified mcu target*/
#define FLASH_DRIVER_MAX_OF_BYTE_TO_WRITE   2048
#define FLASH_DRIVER_MIN_OF_BYTE_TO_WRTIE   256

/* standard error codes */
#define FlashOk             0x00 /* called function succeeded */
#define FlashFailed         0x01 /* called function failed */
#define UnInitialized       0x02
#define Initialized         0x03

/* controller specific error codes - only an example!!! */
#define FlashInvalidParam   0x04
#define FlashInvalidAddress 0x05
#define FlashInvalidSize    0x06
#define FlashInvalidData    0x07

typedef unsigned short FlashResultType;
/* result of flash driver routines */
typedef unsigned long AddressType;
/* logical address */
typedef unsigned long LengthType;
/* length (in bytes) */
typedef unsigned char DataType;
typedef unsigned char IsInitializedType;

typedef struct
{
    /* retrun value / error code: output parameters */
    FlashResultType     errorcode;   /* return value / error code */
    /* is initialized*/
    IsInitializedType   isInit;
    /* erase / write: input parameters */
    AddressType         address; /* logical target address */
    LengthType          length;  /* lenght information (in bytes) */
    const DataType     *data;    /* pointer to data buffer */
} FlashParamType;

typedef void (*FlashFct) ( FlashParamType* ); /* prototype of flash driver routine */ /* PRQA S 1336 */

typedef struct
{
    FlashFct Init;
    FlashFct Deinit;
    FlashFct Erase;
    FlashFct Write;
    FlashFct Read;
}tFlashHeader;

extern void SBL_FlashInit(FlashParamType* FlashParam);
extern void SBL_FlashDeinit(FlashParamType* FlashParam);
extern void SBL_FlashErase(FlashParamType* FlashParam);
extern void SBL_FlashWrite(FlashParamType* FlashParam);
extern void SBL_FlashRead(FlashParamType* FlashParam);
extern uint8 errId;
extern const tFlashHeader FlashHeader;

#endif
