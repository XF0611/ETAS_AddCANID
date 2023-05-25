#include "fusa_alps_core.h"
#include "common.h"
#include "Platform_Types.h"
#include "arc.h"

void core_reg_bit_set(uint32 reg,uint32 bit,uint32 state)
{
    uint32 temp = core_reg_read(reg);
    if(state)
    {
        temp |= 0x00000001 << bit;
    }
    else
    {
        temp &= ~(0x00000001 << bit);
    }
    core_reg_write(reg,temp);
}

uint32 core_irq_case()
{
    return core_reg_read(REG_CPU_AUX_IRQ_CAUSE);
}

float32 core_rtc_time_ms(void)
{
    uint64 rtc_counter = (((uint64)core_reg_read(AUX_RTC_HIGH)) << 32) + (uint64)core_reg_read(AUX_RTC_LOW);
    return (float32)((float64)rtc_counter / 300000);
}
