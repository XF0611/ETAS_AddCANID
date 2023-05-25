#include "fusa_alps_can.h"
#include "Platform_Types.h"
#include "common.h"
#include "alps_clock.h"
#include "alps_mmap.h"
#include "fusa_alps_core.h"

void fusa_can_clk_en(uint32 can_id,uint32 state)
{
    (can_id == 0) ? can0_enable(state) : can1_enable(state);
}

void fusa_can_ecc_en(uint32 id, uint32 state)
{
    uint32 addr_base = (id == 0)? REL_REGBASE_CAN0 : REL_REGBASE_CAN1;
    //set CAN_0.MCR.CFG to 0x1
    uint32 value = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, value | BIT_MODE_CTRL_CFG);
    //set CAN_0.MCR.ECCENA to 0x1
    value = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, value | BIT_MODE_CTRL_ECCENA);
    // set CAN_0.IE. set CAN_0.IE.BEUE and CAN_0.IE.BECE to 0x1
    value = fusa_core_raw_readl(addr_base + REG_CAN_ISR_ENABLE_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_ISR_ENABLE_OFFSET, value | BIT_INTERRUPT_BEU | BIT_INTERRUPT_BEC);
    //set CAN_0.ILS0R.BEULS and CAN_0.ILS0R.BECLS to 0x3
    value = fusa_core_raw_readl(addr_base + REG_CAN_ISR_LINE_SELECT0_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_ISR_LINE_SELECT0_OFFSET,
        value |
        (BITS_ISR_LINE_SELECT_ELOLS_MASK << BITS_ISR_LINE_SELECT_BEULS_SHIFT) |
        (BITS_ISR_LINE_SELECT_BEULS_MASK << BITS_ISR_LINE_SELECT_BECLS_SHIFT));
    //clear CAN0.IR register,write 1 clear
    value = fusa_core_raw_readl(addr_base + REG_CAN_ISR_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_ISR_OFFSET, value | BIT_INTERRUPT_BEC | BIT_INTERRUPT_BEU);
    //set CAN_0.ILE.EINT3 to 0x1
    value = fusa_core_raw_readl(addr_base + REG_CAN_ISR_LINE_ENABLE_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_ISR_LINE_ENABLE_OFFSET, value | BIT_ISR_LINE_ENABLE_EINT3);

    //set CAN_0.MCR.CFG to 0x0
    value = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, value & (~BIT_MODE_CTRL_CFG));

}

void fusa_can_ecc_err_inject(uint32 can_id, uint32 state)
{
    uint32 addr_base = (can_id == 0)? REL_REGBASE_CAN0 : REL_REGBASE_CAN1;
        /* disable ecc*/
    //set CAN_0.MCR.CFG to 0x1
    uint32 temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp | BIT_MODE_CTRL_CFG);
    //set CAN_0.MCR.ECCENA to 0x0
    temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp & (~BIT_MODE_CTRL_ECCENA));
    //set CAN_0.MCR.CFG to 0x0
    temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp & (~BIT_MODE_CTRL_CFG));

    fusa_core_raw_writel(addr_base + REG_CAN_TX_BUFFER_OFFSET,0x12345678);

    /* enable ecc*/
    //set CAN_0.MCR.CFG to 0x1
    temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp | BIT_MODE_CTRL_CFG);
    //set CAN_0.MCR.ECCENA to 0x1
    temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp | BIT_MODE_CTRL_ECCENA);
    //set CAN_0.MCR.CFG to 0x0
    temp = fusa_core_raw_readl(addr_base + REG_CAN_MODE_CTRL_OFFSET);
    fusa_core_raw_writel(addr_base + REG_CAN_MODE_CTRL_OFFSET, temp & (~BIT_MODE_CTRL_CFG));

    fusa_core_raw_readl(addr_base + REG_CAN_TX_BUFFER_OFFSET);
}