#include "fusa_alps_emu.h"
#include "Platform_Types.h"
#include "common.h"

/*************************************************************************
 * common ops
 *************************************************************************/
static void reg_bits_op(uint32 reg_addr, uint32 bits, uint32 state)
{
    uint32 temp = raw_readl(reg_addr);
    if(state)
    {
        temp |= bits;
    }
    else
    {
        temp &= ~bits;
    }
    raw_writel(reg_addr, temp);
}
/*************************************************************************
 * ERR/SAFETY STATE PIN OPERATION
 *************************************************************************/
void emu_safety_pad_mode(safetyPadCtrl_t mode)
{
    if(mode == PAD_CTRL_BY_FSM){
        raw_writel(REG_EMU_SAFETY_PAD_CTRL,0);
        raw_writel(REG_EMU_SAFETY_KEY1,0);
        raw_writel(REG_EMU_SAFETY_KEY2,0);
    }else{
        raw_writel(REG_EMU_SAFETY_PAD_CTRL,1);
        raw_writel(REG_EMU_SAFETY_KEY1,0x5a5a5a5a);
        raw_writel(REG_EMU_SAFETY_KEY2,0x5a5a5a5a);
    }
}

void emu_safety_pad_val(uint32 val)
{
    raw_writel(REG_EMU_SAFE_STATE,val);
}
void emu_err_pad_val(uint32 val)
{
    raw_writel(REG_EMU_ERROR_OUT,val);
}
/*************************************************************************
 * EMU WDT OPERATION
 *************************************************************************/
void emu_wdt_en(uint32 state)
{
    raw_writel(REG_EMU_WDT_ENA, state);
}
void emu_wdt_reload()
{
    raw_writel(REG_EMU_WDT_RELOAD, 1);
}
void emu_wdt_init_val(uint32 val)
{
    raw_writel(REG_EMU_WDT_INIT, 1);
}
uint32 emu_wdt_cur_val()
{
    return raw_readl(REG_EMU_WDT_VALUE);
}

/*************************************************************************
 * EMU BIST OPERATION
 *************************************************************************/
void emu_mbist_clk_en(uint32 state)
{
    raw_writel(REG_EMU_MBIST_CLK_ENA, state);
}
void emu_glbist_clk_en(uint32 state)
{
    raw_writel(REG_EMU_GLBIST_CLK_ENA, state);
}
void emu_bb_lbist_clk_en(uint32 state)
{
    raw_writel(REG_EMU_BB_LBIST_CLK_ENA, state);
}
void emu_bb_lbist_mode(uint32 mode)
{
    raw_writel(REG_EMU_BB_LBIST_MODE, mode);
}
void emu_bb_lbist_en()
{
    raw_writel(REG_EMU_BB_LBIST_ENA, 1);
}
void emu_bb_lbist_clear()
{
    raw_writel(REG_EMU_BB_LBIST_CLR, 1);
}
uint32 emu_lbist_state()
{
    return raw_readl(REG_EMU_LBIST_STA);
}
/*************************************************************************
 * EMU MISC OPERATION
 *************************************************************************/
void emu_set_boot_done()
{
    raw_writel(REG_EMU_BOOT_DONE, 1);
}
void emu_reboot_limit_en(uint32 state)
{
    uint32 temp = raw_readl(REG_EMU_REBOOT_LIMIT);
    if(state)
    {
        temp |= (0x01 << 2);
    }
    else
    {
        temp &= ~(0x01 << 2);
    }
    raw_writel(REG_EMU_REBOOT_LIMIT, temp);
}
void emu_reboot_limit_cnt(uint32 cnt)
{
    uint32 temp = raw_readl(REG_EMU_REBOOT_LIMIT);
    if(cnt > 3)
    {
        cnt = 3;
    }
        temp = (temp & (~0x03)) | cnt;
    raw_writel(REG_EMU_REBOOT_LIMIT, temp);
}
void emu_enter_ss1()
{
    raw_writel(REG_EMU_SS1_CTRL, 1);
}
void emu_enter_ss2()
{
    emu_reboot_limit_en(TRUE);
    emu_reboot_limit_cnt(0);
    emu_enter_ss1();
}
void emu_spared_reg_w(uint32 reg_index,uint32 val)
{
    raw_writel(REG_EMU_SPARED_BASE + reg_index, val);
}
uint32 emu_spared_reg_r(uint32 reg_index)
{
    return raw_readl(REG_EMU_SPARED_BASE + reg_index);
}
/*************************************************************************
 * RF OPERATION
 *************************************************************************/
void emu_rf_irq_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_IRQ_ENA, bits, state);
}
void emu_rf_irq_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_IRQ_MASK, bits, state);
}
uint32 emu_rf_irq_states()
{
    return raw_readl(REG_EMU_RF_ERR_IRQ_STA);
}

void emu_rf_ss1_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_SS1_ENA, bits, state);
}
void emu_rf_ss1_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_SS1_MASK, bits, state);
}
uint32 emu_rf_ss1_states()
{
    return raw_readl(REG_EMU_RF_ERR_SS1_STA);
}

void emu_rf_ss2_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_SS2_ENA, bits, state);
}
void emu_rf_ss2_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_ERR_SS2_MASK, bits, state);
}
uint32 emu_rf_ss2_states()
{
    return raw_readl(REG_EMU_RF_ERR_SS2_STA);
}

void emu_rf_test_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_RF_TEST_ENA, bits, state);
}
void emu_rf_err_clr(uint32 bits)
{
    raw_writel(REG_EMU_RF_ERR_CLR, bits);
}
uint32 emu_rf_err_states()
{
    return raw_readl(REG_EMU_RF_ERR_STA);
}
void emu_rf_err_sw_trigger(uint32 bits)
{
    reg_bits_op(REG_EMU_RF_ERR_STA, bits, TRUE);
}
/*************************************************************************
 * DIG OPERATION
 *************************************************************************/
void emu_dig_irq_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_IRQ_ENA, bits, state);
}
void emu_dig_irq_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_IRQ_MASK, bits, state);
}
uint32 emu_dig_irq_states()
{
    return raw_readl(REG_EMU_DIG_ERR_IRQ_STA);
}

void emu_dig_ss1_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_SS1_ENA, bits, state);
}
void emu_dig_ss1_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_SS1_MASK, bits, state);
}
uint32 emu_dig_ss1_states()
{
    return raw_readl(REG_EMU_DIG_ERR_SS1_STA);
}

void emu_dig_ss2_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_SS2_ENA, bits, state);
}
void emu_dig_ss2_mask(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_ERR_SS2_MASK, bits, state);
}
uint32 emu_dig_ss2_states()
{
    return raw_readl(REG_EMU_DIG_ERR_SS2_STA);
}

void emu_dig_test_ena(uint32 bits, uint32 state)
{
    reg_bits_op(REG_EMU_DIG_TEST_ENA, bits, state);
}
void emu_dig_err_clr(uint32 bits)
{
    raw_writel(REG_EMU_DIG_ERR_CLR, bits);
}
uint32 emu_dig_err_states()
{
    return raw_readl(REG_EMU_DIG_ERR_STA);
}
/*************************************************************************
 *
 *************************************************************************/
