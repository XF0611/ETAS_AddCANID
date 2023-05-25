#include "system_clock_init.h"
#include "radio_ctrl.h"

//#define NULL 0

static inline void radio_reg_write(uint32_t addr, uint32_t val)
{
    val = val & 0xFF;
    val |= (1 << 15);
    val |= ((addr & 0x7F) << 8);
    raw_writel(0xba0004, (val & 0xFFFF));
}

void mdelay(uint32_t ms)
{
    uint32_t delta;
    uint32_t tick = ms * ARC_TIMER_MS_TICKS;
    uint32_t s_low = arc_lr_reg(0x104);
    uint32_t s_high = arc_lr_reg(0x105);

    while (1) {
        if ((arc_lr_reg(0x105) - s_high) > 0) {
            delta = 0xFFFFFFFFU - s_low + arc_lr_reg(0x104);
        } else {
            delta = arc_lr_reg(0x104) - s_low;
        }

        if (delta >= tick) {
            break;
        }
    }
}

void pll_enable_stub(void)
{
    radio_reg_desc_t *pll_clock_ptr = &pll_clock[0];

    raw_writel(0xba0000, 0x2);
    while (pll_clock_ptr->addr != 0xFF)
    {
        radio_reg_write(pll_clock_ptr->addr, pll_clock_ptr->value);
        if (pll_clock_ptr->af_delay & 0xFF)
        {
            mdelay(pll_clock_ptr->af_delay);
        }
        pll_clock_ptr++;
    }
    raw_writel(0xba0000, 0x0);
}

void system_clock_init (void)
{
#ifdef XTAL_CLOCK_FREQ
    return;
#else

    unsigned char lock_status = 0;
    unsigned char  relock_cnt = 2;

    int cpu_clk_ready = raw_readl(0xb20008);

    if (!cpu_clk_ready)
    {
        pll_enable_stub();
    }

    /*==========This is used to lock the clock to 400MHz==================*/
    fmcw_radio_reg_write(0x00, 0x00);
    fmcw_radio_reg_write(0x03, 0x07);
    fmcw_radio_reg_write(0x05, 0xc0);
    fmcw_radio_reg_write(0x09, 0x20);
    fmcw_radio_reg_write(0x12, 0xc0);
    fmcw_radio_reg_write(0x13, 0xc0);
    fmcw_radio_reg_write(0x14, 0xc0);
    fmcw_radio_reg_write(0x15, 0xb0);
    fmcw_radio_reg_write(0x16, 0xb0);
    fmcw_radio_reg_write(0x1b, 0xeb);
//    fmcw_radio_reg_write(0x1d, 0x19);
//    fmcw_radio_reg_write(0x1e, 0xd0);

    fmcw_radio_reg_write(0x1d, 0x18);
    fmcw_radio_reg_write(0x1e, 0xe0);

    fmcw_radio_reg_write(0x25, 0xa0);
    fmcw_radio_reg_write(0x26, 0x6f);

    /* Try refpll auto-lock twice */
    while (!lock_status && relock_cnt--) {
        fmcw_radio_reg_write(0x00, 0x00);
        fmcw_radio_reg_write(0x7d, 0x00);
        mdelay(2);
        fmcw_radio_reg_write(0x7d, 0x01);
        mdelay(7);
        lock_status = fmcw_radio_reg_read_field(0x7f, 0x00, 0x01);
    }
    /*=============This is used to lock the clock to 400MHz==================*/


    // check if PLL is ready
    if (raw_readl(0xb20004) & 1) {
        // Clock divisor for AHB bus
        raw_writel(0xb20108, 1);
        // Clock divisor for APB bus
        raw_writel(0xb2010c, 3);

        // CLKGEN_SEL_300M for CPU
        raw_writel(0xb20008, 1);
        // CLKGEN_SEL_400M for other modules
        raw_writel(0xb2000c, 1);


        // APB REF clock will be 400Mhz
    }
#endif

}

