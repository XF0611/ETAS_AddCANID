

#ifndef FEE_PRV_H
#define FEE_PRV_H

/* Disable the Fee common part when the Fs1 is selected - currently the Fs1 is a complete Fee */
#if(defined(FEE_CFG_FEE_COMMON_ENABLED) && (TRUE == FEE_CFG_FEE_COMMON_ENABLED))

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

/*
 * Collect all sub headers of the Fee as described in the coding guidelines.
*/

#include "Fee_Prv_Config.h"
#include "Fee_Prv_Lib.h"
#include "Fee_Prv_Order.h"
#include "Fee_Prv_Job.h"
#include "Fee_Prv_Medium.h"
#include "Fee_Prv_FsIf.h"
#include "Fee_Prv_Dbg.h"
#include "Fee_Prv_Proc.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/*
 * CPTBLTY: Provide the define FEE_FLAG_SEC_LEVEL_MSK since it is used by BC: MemApl
 *          The define must have the same value as FEE_PRV_CONFIG_DOUBLESTORAGE_BITS
 *          Since FEE_PRV_CONFIG_DOUBLESTORAGE_BITS is private to the config unit,
 *          it must be defined again with the same value.
 */
#define FEE_FLAG_SEC_LEVEL_MSK  (0x0001)

/* List of configured device index along with its name */
typedef struct
{
    uint8                       deviceIdx_u8;                    /* configured FeeRbGeneralDeviceIndex corresponding to the idDeviceSelf */
    Fee_Rb_DeviceName_ten       deviceName_en;                   /* Id of the own device */
}Fee_Prv_Idx_DeviceConfig_tst;

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */

extern const Fee_Prv_Idx_DeviceConfig_tst Fee_Prv_Idx_DeviceConfig_cast[Fee_Rb_Device_Max];

#else
#include "rba_FeeFs1_Prv.h"/* Include the header file for backward compatibility. */
#endif

/* FEE_PRV_H */
#endif
