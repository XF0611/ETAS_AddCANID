#ifndef CDD_FUSA_ALPS_H
#define CDD_FUSA_ALPS_H

#include "common.h"
#include "system_clock_init.h"
#include "fusa_alps_core.h"
#include "alps_mmap.h"
#include "CDD_Fusa.h"
#include "alps_timer.h"

//#define fusa_delay_ms           mdelay
//#define fusa_delay_us           udelay
#define fusa_delay_ms           chip_hw_mdelay
#define fusa_delay_us           chip_hw_udelay
#define fusa_current_time_ms    core_rtc_time_ms

void attach_fusafw_fun_alps(fusa_fw_fun_t *p_func);

#endif
