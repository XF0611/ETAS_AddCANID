#ifndef FUSA_ALPS_CORE_H
#define FUSA_ALPS_CORE_H

#include "Platform_Types.h"
#include "embARC_toolchain.h"
#include "common.h"

#define core_reg_read   arc_lr_reg
#define core_reg_write  arc_sr_reg

#define REG_CPU_ERP_CTRL        0x3f
#define REG_CPU_AUX_IRQ_CAUSE   (0x40a)

Inline void fusa_core_raw_writel(uint32_t addr, uint32_t val)
{
//    void *ptr = (void *)addr;
    Asm("st.di %0, [%1]":: "r"(val), "r"((void*)addr));
}

Inline uint32_t fusa_core_raw_readl(uint32_t addr)
{
    uint32_t ret;
//    void *ptr = (void *)addr;

    Asm("ld.di %0, [%1]":"=r"(ret):"r"((void*)addr));
    return ret;
}

void core_reg_bit_set(uint32 reg, uint32 bit, uint32 state);
uint32 core_irq_case(void);
float32 core_rtc_time_ms(void);

#endif
