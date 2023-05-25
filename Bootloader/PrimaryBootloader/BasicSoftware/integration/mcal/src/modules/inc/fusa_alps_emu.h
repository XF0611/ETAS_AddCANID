#ifndef FUSA_ALPS_EMU_H
#define FUSA_ALPS_EMU_H

#include "fusa_alps_emu_reg.h"
#include "Platform_Types.h"

typedef enum{
    PAD_CTRL_BY_FSM = 0,
    PAD_CTRL_BY_REG = 1,
}safetyPadCtrl_t;

void emu_safety_pad_mode(safetyPadCtrl_t mode);
void emu_safety_pad_val(uint32 val);
void emu_err_pad_val(uint32 val);
void emu_wdt_en(uint32 state);
void emu_wdt_reload(void);
void emu_wdt_init_val(uint32 val);
uint32 emu_wdt_cur_val(void);
void emu_mbist_clk_en(uint32 state);
void emu_glbist_clk_en(uint32 state);
void emu_bb_lbist_clk_en(uint32 state);
void emu_bb_lbist_mode(uint32 mode);
void emu_bb_lbist_en();
void emu_bb_lbist_clear();
uint32 emu_lbist_state();
void emu_set_boot_done();
void emu_reboot_limit_en(uint32 state);
void emu_reboot_limit_cnt(uint32 cnt);
void emu_enter_ss1();
void emu_enter_ss2();
void emu_spared_reg_w(uint32 reg_index, uint32 val);
uint32 emu_spared_reg_r(uint32 reg_index);
void emu_rf_irq_ena(uint32 bits, uint32 state);
void emu_rf_irq_mask(uint32 bits, uint32 state);
uint32 emu_rf_irq_states();
void emu_rf_ss1_ena(uint32 bits, uint32 state);
void emu_rf_ss1_mask(uint32 bits, uint32 state);
uint32 emu_rf_ss1_states();
void emu_rf_ss2_ena(uint32 bits, uint32 state);
void emu_rf_ss2_mask(uint32 bits, uint32 state);
uint32 emu_rf_ss2_states();
void emu_rf_test_ena(uint32 bits, uint32 state);
void emu_rf_err_clr(uint32 bits);
uint32 emu_rf_err_states();
void emu_rf_err_sw_trigger(uint32 bits);

void emu_dig_irq_ena(uint32 bits, uint32 state);
void emu_dig_irq_mask(uint32 bits, uint32 state);
uint32 emu_dig_irq_states();
void emu_dig_ss1_ena(uint32 bits, uint32 state);
void emu_dig_ss1_mask(uint32 bits, uint32 state);
uint32 emu_dig_ss1_states();
void emu_dig_ss2_ena(uint32 bits, uint32 state);
void emu_dig_ss2_mask(uint32 bits, uint32 state);
uint32 emu_dig_ss2_states();
void emu_dig_test_ena(uint32 bits, uint32 state);
void emu_dig_err_clr(uint32 bits);
uint32 emu_dig_err_states();

#endif
