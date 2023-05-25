/*
 * MemIf_Types.h
 *
 *  Created on: 2021Äê11ÔÂ18ÈÕ
 *      Author: wei.zhou
 */

#ifndef INC_MEMIF_TYPES_H_
#define INC_MEMIF_TYPES_H_


typedef enum
{
    MEMIF_UNINIT = 0,           /* The underlying abstraction module or device driver has not been initialized  */
    MEMIF_IDLE,                 /* The underlying abstraction module or device driver is currently idle */
    MEMIF_BUSY,                 /* The underlying abstraction module or device driver is currently busy */
    MEMIF_BUSY_INTERNAL         /* The underlying abstraction module is busy with internal management operations.*/
}MemIf_StatusType;

typedef enum
{
    MEMIF_JOB_OK = 0,               /*	The job has been finished successfully */
    MEMIF_JOB_FAILED,               /*	The job has not been finished successfully */
    MEMIF_JOB_PENDING,              /*	The job has not yet been finished */
    MEMIF_JOB_CANCELED,             /*	The job has been canceled */
    MEMIF_BLOCK_INCONSISTENT,       /*	The requested block is inconsistent, it may contain corrupted data */
    MEMIF_BLOCK_INVALID             /*	The requested block has been marked as invalid, the requested operation can not be performed */
}MemIf_JobResultType;


typedef enum
{
    MEMIF_MODE_SLOW = 0,            /* The underlying memory abstraction modules and drivers are working in slow mode */
    MEMIF_MODE_FAST                 /* The underlying memory abstraction modules and drivers are working in fast mode */
}MemIf_ModeType;

#endif /* INC_MEMIF_TYPES_H_ */
