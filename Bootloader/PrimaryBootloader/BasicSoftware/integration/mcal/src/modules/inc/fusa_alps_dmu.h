#ifndef FUSA_ALPS_DMU_H
#define FUSA_ALPS_DMU_H

#include "Platform_Types.h"

#define FUSA_DMU_REG_BASE       0x00BA0000
#define REG_DMU_IRQ_ENA0_31     (FUSA_DMU_REG_BASE + 0x500)
#define REG_DMU_SYS_MEMRUN_ENA  (FUSA_DMU_REG_BASE + 0x318)
#define REG_DMU_SYS_MEMINI_ENA  (FUSA_DMU_REG_BASE + 0x31C)
#define REG_DMU_SYS_OTP_ECC_EN  (FUSA_DMU_REG_BASE + 0x328)
#define REG_DMU_FMCW_START      (FUSA_DMU_REG_BASE + 0x018)
#define REG_DMU_MUX_RESET       (FUSA_DMU_REG_BASE + 0x218)
#define REG_DMU_MUX_SYNC        (FUSA_DMU_REG_BASE + 0x21C)
#define REG_DMU_ADC_RSTN        (FUSA_DMU_REG_BASE + 0x00C)

#define FUSA_ANA_IRQ_NUM        (19)
#define FUSA_ANA_IRQ_SS1        (20)
#define FUSA_DIG_IRQ_NUM        (21)
#define FUSA_DIG_IRQ_SS1        (22)
#define FUSA_BB_IRQ_ECC_SB      (23)
#define FUSA_BB_IRQ_DONE        (32)
#define FUSA_BB_IRQ_SAM_DONE    (34)

void fusa_dmu_irq_en(uint32 irq, uint32 state);
uint32 fusa_dmu_irq_enabled(uint32 irq);
void fusa_dmu_cpu_ram_ecc_en(uint32 state);
void fusa_dmu_cpu_rom_ecc_en(uint32 state);
void fusa_dmu_otp_ecc_en(uint32 state);


#endif
