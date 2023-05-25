#ifndef FUSA_ALPS_RADAR_H
#define FUSA_ALPS_RADAR_H

#include "Platform_Types.h"

void fusa_radar_consistence_threshold_set(float32 threshold);
float32 fusa_radar_consistence_threshold_get(void);
sint32 fusa_radar_gain_consistence_check(void);
void fusa_dac_playback(boolean inner_circle, uint8 inject_num, uint8 out_num, float32 *peak_power);
void fusa_radar_scan_one_time(uint32 frame_id);

#endif