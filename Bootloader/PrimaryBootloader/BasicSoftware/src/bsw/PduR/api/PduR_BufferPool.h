

#ifndef PDUR_BUFFERPOOL_H_
#define PDUR_BUFFERPOOL_H_

#include "PduR_Cfg_Internal.h"

#ifdef PDUR_BUFFERPOOL_SUPPORT
#include "PduR_Types.h"
#include "PduR_BufferPool_Cfg.h"

/* MR12 RULE 20.7 VIOLATION: The Type Conversion is Required */
#define PDUR_GLOBAL_BUFFERPOOL(s)                (PduR_Global_BufferPoolPtr->s)
#define PDUR_GW_TP_BASE_BUFFERPOOL_FIXED       PDUR_GLOBAL_BUFFERPOOL(bufferPoolConst)

/* Local typedefs for buffer pool */
typedef uint16 PduRBufferPoolLengthType;        /* Buffer pool length type */

/**
 * @ingroup PDUR_BUFFERPOOL_H_
 * This structure represents Buffer Pool referred by a TP Gateway routing path.\n
 * typedef struct \n
 *  { \n
 *  boolean                                poolBufferStatus;            Represents Status of buffer whether free or busy\n
 * } PduR_PoolBuffer_Dynamic;
 */
typedef struct
{
   boolean                                 poolBufferStatus;
} PduR_PoolBuffer_Dynamic;


typedef PduR_PoolBuffer_Dynamic * PduR_PoolBuffer_DynamicPtr;

/**
 * @ingroup PDUR_BUFFERPOOL_H_
 * This structure represents Buffer Pool referred by a TP Gateway routing path.\n
 * typedef struct \n
 *  { \n
 *  void * poolBufferPtr;     Pointer for Buffer\n
 *  PduRBufferPoolLengthType            poolBufferLength;    Represents Length of the buffer\n
 * } PduR_PoolBuffer_Fixed;
 */
typedef struct
{
   uint8 * poolBufferPtr;
   PduRBufferPoolLengthType              poolBufferLength;
} PduR_PoolBuffer_Fixed;

typedef const PduR_PoolBuffer_Fixed * PduR_PoolBuffer_FixedPtr;
/**
 * @ingroup PDUR_BUFFERPOOL_H_
 * This structure represents Buffer Pool referred by a TP Gateway routing path.\n
 * typedef struct \n
 *  { \n
 *  PduRBufferPoolLengthType            freeBuffersInPool;    Represents Total no. of free buffers in the Pool\n
 * } PduR_BufferPool_Dynamic;
 */
typedef struct
{
    PduRBufferPoolLengthType            freeBuffersInPool;
} PduR_BufferPool_Dynamic;

typedef PduR_BufferPool_Dynamic * PduR_BufferPool_DynamicPtr;

/**
 * @ingroup PDUR_BUFFERPOOL_H_
 * This structure represents Buffer Pool referred by a TP Gateway routing path.\n
 * typedef struct \n
 *  { \n
 *  PduR_BufferPool_Dynamic * bufferPoolDyn;           Pointer for Pool Manager dynamic structure\n
 *  const PduR_PoolBuffer_Fixed * poolBufferConst;        Pointer to Const Buffer manager table\n
 *  PduR_PoolBuffer_Dynamic * poolBufferDyn;    Pointer to RAM Buffer manager table\n
 *  PduRBufferPoolLengthType                                 maxBufferSize;     Represents max size of buffer in the Pool\n
 *  PduRBufferPoolLengthType                                 totalBuffersInPool;    Represents Total no. of buffers in the Pool\n
 * } PduR_BufferPool_Fixed;
 */
typedef struct
{
   PduR_BufferPool_Dynamic * bufferPoolDyn;
   const PduR_PoolBuffer_Fixed * poolBufferConst;
   PduR_PoolBuffer_Dynamic * poolBufferDyn;
   PduRBufferPoolLengthType                                 maxBufferSize;
   PduRBufferPoolLengthType                                 totalBuffersInPool;
} PduR_BufferPool_Fixed;

typedef const PduR_BufferPool_Fixed * PduR_BufferPool_FixedPtr;

/**
 * @ingroup PDUR_BUFFERPOOL_H_
 * This structure represents Buffer Pool referred by a TP Gateway routing path.\n
 * typedef struct \n
 *  { \n
   const PduR_BufferPool_Fixed * bufferPoolConst;        Pointer to Const Pool manager\n
 * } PduR_BufferPool_ConfigType;
 */
typedef struct
{
   const PduR_BufferPool_Fixed * bufferPoolConst;

} PduR_BufferPool_ConfigType;

/*Extern declarations*/

/*Start: Functions*/
extern void PduR_BufferPoolInit(const PduR_BufferPool_ConfigType * BuffPoolConfigPtr);

extern BufReq_ReturnType PduR_BufferPoolRequest(  uint16 BuffPoolId,
                                                               PduLengthType TpSduLength,
                                                               PduLengthType * PduRBufferSizePtr,
                                                               uint8 * *bufferPtr);

extern void PduR_BufferPoolRelease(uint16 BuffPoolId, const uint8 * bufferPtr);
/*End: Functions*/

/*Start: Global variable */
extern const PduR_BufferPool_ConfigType * PduR_Global_BufferPoolPtr;

extern const PduR_BufferPool_ConfigType PduR_BufferPool_Config;
/*End: Global variable */

#endif /* PDUR_BUFFERPOOL_SUPPORT */
#endif /* PDUR_BUFFERPOOL_H_ */

