#ifndef FUSA_ALPS_BASBAND_H
#define FUSA_ALPS_BASBAND_H

#include "Platform_Types.h"

#define SHADOW_BNK 4
boolean fusa_bb_state_get();
void fusa_bb_state_set(boolean state);
uint32 fusa_bb_switch_mem_access(uint32 table_id);
uint32 fusa_bb_switch_bnk_act(uint32 bnk_id);
uint32 fusa_bb_switch_bnk_mode(uint32 bnk_mode);
void fusa_bb_frame_interleave_pattern(uint8 frame_loop_pattern);
void fusa_bb_mem_ecc_en(uint32 state);
void fusa_bb_mem_ecc_err_clr(void);
void fusa_bb_top_clk_en(uint32 state);
void fusa_bb_core_clk_en(uint32 state);
void fusa_bb_clear_memory(uint32 bank_id);
sint32 fusa_bb_lbist(void);
void fusa_bb_hw_reset_after_force(void);
uint32 fusa_bb_get_rtl_frame_type(void);
uint8 fusa_bb_switch_buf_store(uint8 buf_id);
uint32 fusa_bb_read_first_chirp_data(uint32 ch_index, uint32 data_index, uint32 data_index_max);
void fusa_bb_fft_peak_calc(float32 *peak_power, uint32 fft_size);
void fusa_bb_agc_irq_en(boolean state);
void fusa_bb_agc_clr_irq_status(void);
#endif