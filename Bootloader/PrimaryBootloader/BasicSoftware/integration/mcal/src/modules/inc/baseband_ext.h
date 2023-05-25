/*
 * baseband_ext.h
 *
 *  Created on: June 24, 2022
 *      Author: fei.xu1
 */

#ifndef _BASEBAND_EXT_H_
#define _BASEBAND_EXT_H_

#include "radio_ext.h"

typedef bool (*bb_isr_callback)(void);
typedef enum {
    sam_done,
    bb_done
}bb_isr_type;
#define VEL_DEAMB_MF_EN (false && (NUM_FRAME_TYPE==2))
#define CUSTOM_VEL_DEAMB_MF false
#define MAX_OUTPUT_OBJS 128

#endif /* INC_BASEBAND_EXT_H_ */
