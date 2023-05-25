#include "fusa_alps_dmu.h"
#include "common.h"
#include "Platform_Types.h"

/*************************************************************************
 * common ops
 *************************************************************************/
void fusa_dmu_irq_en(uint32 irq, uint32 state)
{
    uint32 reg_addr = REG_DMU_IRQ_ENA0_31 + (irq / 32) * 4;
    uint32 shift = irq % 32;
    uint32 temp = raw_readl(reg_addr);
    if(state)
    {
        temp |= (0x00000001 << shift);
    }
    else
    {
        temp &= ~(0x00000001 << shift);
    }
    raw_writel(reg_addr, temp);
}

uint32 fusa_dmu_irq_enabled(uint32 irq)
{
    uint32 reg_addr = REG_DMU_IRQ_ENA0_31 + (irq / 32) * 4;
    uint32 shift = irq % 32;
    uint32 temp = raw_readl(reg_addr);
    uint32 ret = TRUE;
    if(temp & (0x00000001 << shift))
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}
void fusa_dmu_cpu_ram_ecc_en(uint32 state)
{
    if(state)
    {
        raw_writel(REG_DMU_SYS_MEMRUN_ENA, 1);
    }
    else
    {
        raw_writel(REG_DMU_SYS_MEMRUN_ENA, 0);
    }
}

void fusa_dmu_cpu_rom_ecc_en(uint32 state)
{
    if(state)
    {
        raw_writel(REG_DMU_SYS_MEMINI_ENA, 1);
    }
    else
    {
        raw_writel(REG_DMU_SYS_MEMINI_ENA, 0);
    }
}

void fusa_dmu_otp_ecc_en(uint32 state)
{
    if(state)
    {
        raw_writel(REG_DMU_SYS_OTP_ECC_EN, 1);
    }
    else
    {
        raw_writel(REG_DMU_SYS_OTP_ECC_EN, 0);
    }
}