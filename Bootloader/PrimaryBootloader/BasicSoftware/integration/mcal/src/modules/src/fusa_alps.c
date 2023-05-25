#include "fusa_alps.h"
#include "CDD_Fusa.h"
#include "Platform_Types.h"
#include "fusa_alps_core.h"
#include "fusa_alps_dmu.h"
#include "fusa_alps_emu.h"
#include "baseband_alps_FM_reg.h"
#include "fusa_alps_basband.h"
#include "fusa_alps_can.h"
#include "fusa_alps_radio.h"
#include "fusa_alps_radar.h"

#define raw_read_op       fusa_core_raw_readl
#define raw_write_op      fusa_core_raw_writel

static fusa_err_handle_t fusa_err_handler_customer = NULL;

static sint32 exe_cust_err_handle(fusa_item_e item, uint32 *param)
{
    sint32 ret = E_FUSA_NOEXS;
    if(fusa_err_handler_customer != NULL)
    {
        ret = fusa_err_handler_customer(item, param);
    }
    return ret;
}
/***********************************************************************************************************************
 *
 * FUSA ITEM INIT
 *
***********************************************************************************************************************/
/**************************************************************
 *
**************************************************************/
static sint32 init_item1  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_LDO_MT, TRUE);
        emu_rf_irq_mask(RF_ITEM_LDO_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_LDO_MT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_LDO_MT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_LDO_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_LDO_MT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_LDO_MT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_LDO_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_LDO_MT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item2  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radio_3v3_th_set(7.5, -7.5);
    fusa_radio_sm_avdd33_monitor_irq();
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_irq_mask(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_AVDD33_MT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_AVDD33_MT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_AVDD33_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_AVDD33_MT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item3  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_radio_sm_dvdd11_monitor_irq();
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_irq_mask(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_DVDD11_MT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_radio_sm_dvdd11_monitor_irq();
        emu_rf_ss1_ena(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_DVDD11_MT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_radio_sm_dvdd11_monitor_irq();
        emu_rf_ss2_ena(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_DVDD11_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_DVDD11_MT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item4  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_radio_sm_vbg_monitor_irq();
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_VBG_MT, TRUE);
        emu_rf_irq_mask(RF_ITEM_VBG_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_VBG_MT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_radio_sm_vbg_monitor_irq();
        emu_rf_ss1_ena(RF_ITEM_VBG_MT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_VBG_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_VBG_MT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_radio_sm_vbg_monitor_irq();
        emu_rf_ss2_ena(RF_ITEM_VBG_MT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_VBG_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_VBG_MT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item5  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_radio_cpu_clk_lock_det_init();
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_irq_mask(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_radio_cpu_clk_lock_det_init();
        emu_rf_ss1_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_radio_cpu_clk_lock_det_init();
        emu_rf_ss2_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_PLL_LOCK_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item6  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    double pdt_threshold = 5.0;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_radio_set_power_detector_threshold(0, pdt_threshold);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_irq_mask(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RF_POWER_DT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_radio_set_power_detector_threshold(0, pdt_threshold);
        emu_rf_ss1_ena(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RF_POWER_DT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_radio_set_power_detector_threshold(0, pdt_threshold);
        emu_rf_ss2_ena(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_RF_POWER_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RF_POWER_DT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item8  (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_err_clr(RF_ITEM_RX_SAT_DT);
        emu_rf_irq_ena(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_irq_mask(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RX_SAT_DT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_err_clr(RF_ITEM_RX_SAT_DT);
        emu_rf_ss1_ena(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RX_SAT_DT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_err_clr(RF_ITEM_RX_SAT_DT);
        emu_rf_ss2_ena(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_RX_SAT_DT, TRUE);
        emu_rf_test_ena(RF_ITEM_RX_SAT_DT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item11 (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_IF_LOOPBACK, TRUE);
        emu_rf_irq_mask(RF_ITEM_IF_LOOPBACK, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_IF_LOOPBACK, TRUE);
        emu_rf_ss1_mask(RF_ITEM_IF_LOOPBACK, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_IF_LOOPBACK, TRUE);
        emu_rf_ss2_mask(RF_ITEM_IF_LOOPBACK, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item12 (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_RF_LOOPBACK, TRUE);
        emu_rf_irq_mask(RF_ITEM_RF_LOOPBACK, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_RF_LOOPBACK, TRUE);
        emu_rf_ss1_mask(RF_ITEM_RF_LOOPBACK, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_RF_LOOPBACK, TRUE);
        emu_rf_ss2_mask(RF_ITEM_RF_LOOPBACK, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item13 (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radio_chirp_mon_thr_set(0x1f,0x06);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_irq_mask(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_CHIRP_MT, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_ss1_mask(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_CHIRP_MT, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_ss2_mask(RF_ITEM_CHIRP_MT, TRUE);
        emu_rf_test_ena(RF_ITEM_CHIRP_MT, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item14 (fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radio_temp_detector_threshold_set(125.0);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_irq_mask(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_test_ena(RF_ITEM_TEMP_OVER, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_ss1_mask(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_test_ena(RF_ITEM_TEMP_OVER, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_ss2_mask(RF_ITEM_TEMP_OVER, TRUE);
        emu_rf_test_ena(RF_ITEM_TEMP_OVER, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item101(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        emu_dig_irq_ena(DIG_ITEM_BB_LBIST, TRUE);
        emu_dig_irq_mask(DIG_ITEM_BB_LBIST, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_dig_ss1_ena(DIG_ITEM_BB_LBIST, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_BB_LBIST, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_dig_ss2_ena(DIG_ITEM_BB_LBIST, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_BB_LBIST, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item102(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 0, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 1, 1);
        emu_dig_irq_ena(DIG_ITEM_CPU_CCM_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CPU_CCM_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        core_reg_bit_set(REG_CPU_ERP_CTRL, 0, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 1, 1);
        emu_dig_ss1_ena(DIG_ITEM_CPU_CCM_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CPU_CCM_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        core_reg_bit_set(REG_CPU_ERP_CTRL, 0, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 1, 1);
        emu_dig_ss2_ena(DIG_ITEM_CPU_CCM_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CPU_CCM_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item103(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 2, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 3, 1);
        emu_dig_irq_ena(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        core_reg_bit_set(REG_CPU_ERP_CTRL, 2, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 3, 1);
        emu_dig_ss1_ena(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        core_reg_bit_set(REG_CPU_ERP_CTRL, 2, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 3, 1);
        emu_dig_ss2_ena(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CPU_CACHE_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item104(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        fusa_bb_top_clk_en(TRUE);
        fusa_bb_core_clk_en(TRUE);
        fusa_bb_mem_ecc_en(TRUE);
        emu_dig_err_clr(emu_dig_err_states());
        emu_dig_irq_ena(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_bb_top_clk_en(TRUE);
        fusa_bb_core_clk_en(TRUE);
        fusa_bb_mem_ecc_en(TRUE);
        emu_dig_err_clr(emu_dig_err_states());
        emu_dig_ss1_ena(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_bb_top_clk_en(TRUE);
        fusa_bb_core_clk_en(TRUE);
        fusa_bb_mem_ecc_en(TRUE);
        emu_dig_err_clr(emu_dig_err_states());
        emu_dig_ss2_ena(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_BB_SRAM_ECC | DIG_ITEM_BB_ROM_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}

/**************************************************************
 *
**************************************************************/
static sint32 init_item106(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        fusa_dmu_cpu_rom_ecc_en(TRUE);
        emu_dig_irq_ena(DIG_ITEM_CPU_ROM_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CPU_ROM_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_dmu_cpu_rom_ecc_en(TRUE);
        emu_dig_ss1_ena(DIG_ITEM_CPU_ROM_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CPU_ROM_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_dmu_cpu_rom_ecc_en(TRUE);
        emu_dig_ss2_ena(DIG_ITEM_CPU_ROM_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CPU_ROM_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item107(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        fusa_dmu_cpu_ram_ecc_en(TRUE);
        emu_dig_irq_ena(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_dmu_cpu_ram_ecc_en(TRUE);
        emu_dig_ss1_ena(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_dmu_cpu_ram_ecc_en(TRUE);
        emu_dig_ss2_ena(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CPU_SRAM_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item108(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        fusa_dmu_otp_ecc_en(TRUE);
        emu_dig_irq_ena(DIG_ITEM_OTP_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_OTP_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_dmu_otp_ecc_en(TRUE);
        emu_dig_ss1_ena(DIG_ITEM_OTP_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_OTP_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_dmu_otp_ecc_en(TRUE);
        emu_dig_ss2_ena(DIG_ITEM_OTP_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_OTP_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item109(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_clk_en(0, TRUE);
        fusa_can_ecc_en(0, TRUE);
        emu_dig_irq_ena(DIG_ITEM_CAN0_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CAN0_ECC, TRUE);
#endif
#if (FUSA_FEATURE_CAN1 == FEATURE_ON)
        fusa_can_clk_en(1, TRUE);
        fusa_can_ecc_en(1, TRUE);
        emu_dig_irq_ena(DIG_ITEM_CAN1_ECC, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CAN1_ECC, TRUE);
#endif
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_clk_en(0, TRUE);
        fusa_can_ecc_en(0, TRUE);
        emu_dig_ss1_ena(DIG_ITEM_CAN0_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CAN0_ECC, TRUE);
#endif
#if (FUSA_FEATURE_CAN1 == FEATURE_ON)
        fusa_can_clk_en(1, TRUE);
        fusa_can_ecc_en(1, TRUE);
        emu_dig_ss1_ena(DIG_ITEM_CAN1_ECC, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CAN1_ECC, TRUE);
#endif
        break;
    case FUSA_TYPE_SS2:
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_clk_en(0, TRUE);
        fusa_can_ecc_en(0, TRUE);
        emu_dig_ss2_ena(DIG_ITEM_CAN0_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CAN0_ECC, TRUE);
#endif
#if (FUSA_FEATURE_CAN1 == FEATURE_ON)
        fusa_can_clk_en(1, TRUE);
        fusa_can_ecc_en(1, TRUE);
        emu_dig_ss2_ena(DIG_ITEM_CAN1_ECC, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CAN1_ECC, TRUE);
#endif
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item120(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item121(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item126(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item130(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        fusa_fmcw_lockstep_init();
        emu_dig_irq_ena(DIG_ITEM_CHIRP_LS, TRUE);
        emu_dig_irq_mask(DIG_ITEM_CHIRP_LS, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        fusa_fmcw_lockstep_init();
        emu_dig_ss1_ena(DIG_ITEM_CHIRP_LS, TRUE);
        emu_dig_ss1_mask(DIG_ITEM_CHIRP_LS, TRUE);
        break;
    case FUSA_TYPE_SS2:
        fusa_fmcw_lockstep_init();
        emu_dig_ss2_ena(DIG_ITEM_CHIRP_LS, TRUE);
        emu_dig_ss2_mask(DIG_ITEM_CHIRP_LS, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item133(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
        emu_dig_irq_ena(DIG_ITEM_XIP_ECC, TRUE);
        emu_dig_err_clr(DIG_ITEM_XIP_ECC);
        emu_dig_irq_mask(DIG_ITEM_XIP_ECC, TRUE);
        fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_dig_ss1_ena(DIG_ITEM_XIP_ECC, TRUE);
        emu_dig_err_clr(DIG_ITEM_XIP_ECC);
        emu_dig_ss1_mask(DIG_ITEM_XIP_ECC, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_dig_ss2_ena(DIG_ITEM_XIP_ECC, TRUE);
        emu_dig_err_clr(DIG_ITEM_XIP_ECC);
        emu_dig_ss2_mask(DIG_ITEM_XIP_ECC, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/**************************************************************
 *
**************************************************************/
static sint32 init_item201(fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s cur_time=%f\r\n", __func__, fusa_current_time_ms());
    switch (fusa_type)
    {
    case FUSA_TYPE_IRQ:
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
        emu_rf_irq_ena(RF_ITEM_GAIN_CK, TRUE);
        emu_rf_irq_mask(RF_ITEM_GAIN_CK, TRUE);
        fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
        break;
    case FUSA_TYPE_SS1:
        emu_rf_ss1_ena(RF_ITEM_GAIN_CK, TRUE);
        emu_rf_ss1_mask(RF_ITEM_GAIN_CK, TRUE);
        break;
    case FUSA_TYPE_SS2:
        emu_rf_ss2_ena(RF_ITEM_GAIN_CK, TRUE);
        emu_rf_ss2_mask(RF_ITEM_GAIN_CK, TRUE);
        break;
    default:
        ret = E_FUSA_NOSPT;
        break;
    }
    return ret;
}
/***********************************************************************************************************************
 *
 * FUSA ITEMS CONFIG SET
 *
***********************************************************************************************************************/
static sint32 cfg_set_item1(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radar_ldo_monitor_threshold_set((fusa_config_sm1_t *)param);
    return ret;
}
static sint32 cfg_set_item2(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_threshold_t *p = (fusa_threshold_t *)param;
    fusa_radio_3v3_th_set(p->thr_h, p->thr_l);
    return ret;
}
static sint32 cfg_set_item3(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_set_voltage_1v1_th((uint8)thr->thr_h, (uint8)thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item4(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fmcw_radio_set_vbg_monitor_vth((uint8)thr->thr_h, (uint8)thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item6(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_config_sm6_t *cfg = (fusa_config_sm6_t *)param;
    if(cfg != NULL)
    {
        fusa_radio_set_power_detector_threshold(cfg->channel, (double)cfg->threshold);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item8(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}
static sint32 cfg_set_item11(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_if_loopback_thr_set(thr->thr_h, thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item12(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_rf_loopback_thr_set(thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item13(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_chirp_mon_thr_set(thr->thr_h,thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item14(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_temp_detector_threshold_set(thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
static sint32 cfg_set_item201(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radar_consistence_threshold_set(thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}
/***********************************************************************************************************************
 *
 * FUSA ITEMS CONFIG GET
 *
***********************************************************************************************************************/
static sint32 cfg_get_item1(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radar_ldo_monitor_threshold_get((fusa_config_sm1_t *)param);
    return ret;
}

static sint32 cfg_get_item2(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_threshold_t *p = (fusa_threshold_t *)param;
    fusa_radio_3v3_th_get(&p->thr_h, &p->thr_l);
    return ret;
}

static sint32 cfg_get_item3(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        uint8 thr_h = 0;
        uint8 thr_l = 0;
        fusa_radio_get_voltage_1v1_th(&thr_h, &thr_l);
        thr->thr_h = thr_h;
        thr->thr_l = thr_l;
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item4(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        uint8 thr_h = 0;
        uint8 thr_l = 0;
        fmcw_radio_get_vbg_monitor_vth(&thr_h, &thr_l);
        thr->thr_h = thr_h;
        thr->thr_l = thr_l;
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item6(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 cfg_get_item8(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 cfg_get_item11(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_if_loopback_thr_get(&thr->thr_h, &thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item12(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        fusa_radio_rf_loopback_thr_get(&thr->thr_l);
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item13(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        uint8 thr_h = 0;
        uint8 thr_l = 0;
        fusa_radio_chirp_mon_thr_get(&thr_h,&thr_l);
        thr->thr_h = thr_h;
        thr->thr_l = thr_l;
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item14(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        thr->thr_l = fusa_radio_temp_detector_threshold_get();
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}

static sint32 cfg_get_item201(uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
/*     fusa_threshold_t *thr = (fusa_threshold_t *)param;
    if(thr != NULL)
    {
        thr->thr_l = fusa_radar_consistence_threshold_get();
    }
    else
    {
        ret = E_FUSA_PAR;
    } */
    return ret;
}


/***********************************************************************************************************************
 *
 * FUSA ITEMS ENABLE
 *
***********************************************************************************************************************/
static sint32 enable_item1(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_LDO_MT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_LDO_MT, FALSE);
    }
    return ret;
}
static sint32 enable_item2(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_AVDD33_MT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_AVDD33_MT, FALSE);
    }
    return ret;
}
static sint32 enable_item3(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_DVDD11_MT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_DVDD11_MT, FALSE);
    }
    return ret;
}
static sint32 enable_item4(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_VBG_MT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_VBG_MT, FALSE);
    }
    return ret;
}
static sint32 enable_item5(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_PLL_LOCK_DT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_PLL_LOCK_DT, FALSE);
    }
    return ret;
}
static sint32 enable_item6(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, FALSE);
    }
    return ret;
}
static sint32 enable_item8(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_err_clr(RF_ITEM_RX_SAT_DT);
        emu_rf_irq_ena(RF_ITEM_RX_SAT_DT, TRUE);
    } else {
        fusa_bb_agc_irq_en(FALSE);
        fusa_bb_agc_clr_irq_status();
        emu_rf_err_clr(RF_ITEM_RX_SAT_DT);
        emu_rf_irq_ena(RF_ITEM_RX_SAT_DT, FALSE);
    }
    return ret;
}
static sint32 enable_item11(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_IF_LOOPBACK, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_IF_LOOPBACK, FALSE);
    }
    return ret;
}
static sint32 enable_item12(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_RF_LOOPBACK, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_RF_LOOPBACK, FALSE);
    }
    return ret;
}
static sint32 enable_item13(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_CHIRP_MT, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_CHIRP_MT, FALSE);
    }
    return ret;
}
static sint32 enable_item14(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        emu_rf_irq_ena(RF_ITEM_TEMP_OVER, TRUE);
    } else {
        emu_rf_irq_ena(RF_ITEM_TEMP_OVER, FALSE);
    }
    return ret;
}

static sint32 enable_item102(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param){
        core_reg_bit_set(REG_CPU_ERP_CTRL, 0, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 1, 1);
    } else {
        core_reg_bit_set(REG_CPU_ERP_CTRL, 0, 0);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 1, 0);
    }
    return ret;
}
static sint32 enable_item103(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param){
        core_reg_bit_set(REG_CPU_ERP_CTRL, 2, 1);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 3, 1);
    } else {
        core_reg_bit_set(REG_CPU_ERP_CTRL, 2, 0);
        core_reg_bit_set(REG_CPU_ERP_CTRL, 3, 0);
    }
    return ret;
}
static sint32 enable_item104(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        fusa_bb_mem_ecc_en(TRUE);
    } else {
        fusa_bb_mem_ecc_en(FALSE);
    }
    return ret;
}

static sint32 enable_item106(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        fusa_dmu_cpu_rom_ecc_en(TRUE);
    } else {
        fusa_dmu_cpu_rom_ecc_en(FALSE);
    }
    return ret;
}
static sint32 enable_item107(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        fusa_dmu_cpu_ram_ecc_en(TRUE);
    } else {
        fusa_dmu_cpu_ram_ecc_en(FALSE);
    }
    return ret;
}
static sint32 enable_item108(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
        fusa_dmu_otp_ecc_en(TRUE);
    } else {
        fusa_dmu_otp_ecc_en(FALSE);
    }
    return ret;
}
static sint32 enable_item109(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_ecc_en(0, 1);
#endif
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_ecc_en(1, 1);
#endif
    } else {
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_ecc_en(0, 0);
#endif
#if (FUSA_FEATURE_CAN0 == FEATURE_ON)
        fusa_can_ecc_en(1, 0);
#endif
    }
    return ret;
}
static sint32 enable_item120(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){

    } else {

    }
    return ret;
}
static sint32 enable_item125(boolean param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param ){

    } else {

    }
    return ret;
}
static sint32 enable_item126(boolean param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(param ){

    } else {

    }
    return ret;
}
static sint32 enable_item130(boolean param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(param ){
        fusa_fmcw_lockstep_en(TRUE);
    } else {
        fusa_fmcw_lockstep_en(FALSE);
    }
    return ret;
}

static sint32 enable_item201(boolean param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(param ){

    } else {

    }
    return ret;
}

/***********************************************************************************************************************
 *
 * FUSA ITEMS RUN
 *
***********************************************************************************************************************/
static sint32 fusa_periodic_item_err_handler(fusa_item_e item)
{
    sint32 ret = E_FUSA_OK;
    switch (item)
    {
    case FUSA_ITEM_SM11:
        emu_rf_err_sw_trigger(RF_ITEM_IF_LOOPBACK);
        break;
    case FUSA_ITEM_SM12:
        emu_rf_err_sw_trigger(RF_ITEM_RF_LOOPBACK);
        break;
    case FUSA_ITEM_SM201:
        emu_rf_err_sw_trigger(RF_ITEM_GAIN_CK);
        break;

    default:
        ret = E_FUSA_ID;
        break;
    }
    return ret;
}

static sint32 run_item1(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    static volatile float32 times_needed = 0.0;
    static volatile float32 start_time_ms = 0.0;
    static volatile sint32 sm1_status = E_FUSA_CLSED;
    // //log_fusa("%s param=%x\r\n", __func__, param);
    if(param & FUSA_RUN_SET)
    {
        if(sm1_status == E_FUSA_CLSED)
        {
            emu_rf_irq_ena(RF_ITEM_LDO_MT, TRUE);
            times_needed = fusa_radio_sm_ldo_monitor_setting(param & 0xff);
            start_time_ms = fusa_current_time_ms();
            sm1_status = E_FUSA_OPNED;
            ret = E_FUSA_OK;
            //log_fusa("ldo_monitor start time = %f ,time_need = %f\r\n", start_time_ms, times_needed);
        }
        else
        {
            ret = E_FUSA_OPNED;
        }

    }
    else if(param & FUSA_RUN_RESUME)
    {
        if(sm1_status == E_FUSA_OPNED)
        {
            if(fusa_current_time_ms() - start_time_ms > times_needed)
            {
                fusa_radio_sm_ldo_monitor_resume(param & 0xff);
                //log_fusa("ldo_monitor times = %f\r\n", fusa_current_time_ms() - start_time_ms);
                start_time_ms = 0.0;
                times_needed = 0.0;
                sm1_status = E_FUSA_CLSED;
                ret = E_FUSA_OK;
            }
            else
            {
                ret = E_FUSA_DBUSY;
            }
        }
        else
        {
            ret = E_FUSA_CLSED;
        }
    }
    else if(param & FUSA_RUN_RESUME_FORCE)
    {
        fusa_radio_sm_ldo_monitor_resume(param & 0xff);
        start_time_ms = 0.0;
        times_needed = 0.0;
        sm1_status = E_FUSA_CLSED;
        ret = E_FUSA_OK;
    }
    else
    {
        ret = E_FUSA_NSUP;
    }
    return ret;
}

static sint32 run_item6(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, TRUE);
    fusa_radio_sm_power_detector_irq(param);

    return ret;
}

static sint32 run_item8(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 run_item11(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    uint8 frame_type = fusa_radio_get_rtl_frame_type();
    uint8 agc_state = fusa_radio_agc_en_state(frame_type);
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(FALSE);
#endif
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, FALSE);
    }
//    float32 peak[4];
    if(fusa_radio_sm_if_loopback_irq())
    {
        ret = fusa_periodic_item_err_handler(FUSA_ITEM_SM11);
    }
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, TRUE);
    }
    fusa_bb_agc_clr_irq_status();
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(TRUE);
#endif

    return ret;
}

static sint32 run_item12(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    uint8 frame_type = fusa_radio_get_rtl_frame_type();
    uint8 agc_state = fusa_radio_agc_en_state(frame_type);
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(FALSE);
#endif
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, FALSE);
    }
    if(fusa_radio_sm_rf_loopback_irq())
    {
        ret = fusa_periodic_item_err_handler(FUSA_ITEM_SM12);
    }
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, TRUE);
    }
    fusa_bb_agc_clr_irq_status();
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(TRUE);
#endif

    return ret;
}

static sint32 run_item13(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_radio_sm_chirp_monitor_irq();

    return ret;
}

static sint32 run_item14(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_radio_sm_over_temp_detector_irq();
    return ret;
}

static sint32 run_item101(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(FALSE);
#endif
    ret = fusa_bb_lbist();
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(TRUE);
#endif
    return ret;
}

static sint32 run_item120(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 run_item126(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 run_item201(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    //log_fusa("start time ms = %f\r\n", fusa_current_time_ms());
#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(FALSE);
#endif

    if(fusa_radar_gain_consistence_check()!= E_FUSA_OK)
    {
        ret = fusa_periodic_item_err_handler(FUSA_ITEM_SM201);
    }

#if FUSA_FEATURE_SM8 == FEATURE_ON
    enable_item8(TRUE);
#endif
    //log_fusa("end time ms = %f\r\n", fusa_current_time_ms());

    return ret;
}

static sint32 run_item901(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(fusa_radio_sm_ldo_monitor_fault_injection() != 1)
    {
        ret = E_FUSA_SYS;
    }
    return ret;
}

static sint32 run_item902(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
#if FUSA_FEATURE_SM2 == FEATURE_ON
    if(fusa_radio_sm_avdd33_monitor_fault_injection() != 7)
    {
        ret = E_FUSA_SYS;
    }
#endif
#if FUSA_FEATURE_SM3 == FEATURE_ON
    if(fusa_radio_sm_dvdd11_monitor_fault_injection() != 1)
    {
        ret = E_FUSA_SYS;
    }
#endif
    return ret;
}

static sint32 run_item904(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(fusa_radio_sm_vbg_monitor_fault_injection() != 1)
    {
        ret = E_FUSA_SYS;
    }
    return ret;
}

static sint32 run_item905(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(fusa_radio_sm_power_detector_fault_injection(5,0x15) != 1)
    {
        ret = E_FUSA_SYS;
    }
    return ret;
}

static sint32 run_item906(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 run_item907(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    uint8 frame_type = fusa_radio_get_rtl_frame_type();
    uint8 agc_state = fusa_radio_agc_en_state(frame_type);
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, FALSE);
    }
    if(fusa_radio_sm_if_loopback_fault_injection_irq() != 1)
    {
        ret = E_FUSA_SYS;
    }
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, TRUE);
    }
    return ret;
}

static sint32 run_item908(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    uint8 frame_type = fusa_radio_get_rtl_frame_type();
    uint8 agc_state = fusa_radio_agc_en_state(frame_type);
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, FALSE);
    }
    if(fusa_radio_sm_rf_loopback_fault_injection_irq() != 1)
    {
        ret = E_FUSA_SYS;
    }
    if(agc_state)
    {
        fusa_radio_agc_enable(frame_type, TRUE);
    }
    return ret;
}

static sint32 run_item910(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(fusa_radio_sm_over_temp_detector_fault_injection() != 1)
    {
        ret = E_FUSA_SYS;
    }
    return ret;
}

static sint32 run_item911(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(fusa_radio_sm_chirp_monitor_fault_injection() != 1)
    {
        ret = E_FUSA_SYS;
    }
    return ret;
}

/***********************************************************************************************************************
 *
 * FUSA ITEMS ERR INJECT
***********************************************************************************************************************/
static sint32 err_inject_item1(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    emu_rf_irq_ena(RF_ITEM_LDO_MT, TRUE);
    fusa_radio_sm_ldo_monitor_fault_injection();
    return ret;
}

static sint32 err_inject_item2(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(param == 0)
    {
        fusa_radio_3v3_th_set(15, 10);
        fusa_delay_ms(1);
        fusa_radio_3v3_th_set(10, -10);//need set a right threshold immediately
        fusa_radio_start_voltage_detector_3v3();//restart
    }
    else if(param == 1)
    {
    fusa_radio_3v3_th_set(-10, -15);
    fusa_delay_ms(1);
    fusa_radio_3v3_th_set(10, -10);//need set a right threshold immediately
    fusa_radio_start_voltage_detector_3v3();//restart
    }
    else
    {
        ret = E_FUSA_PAR;
    }
    return ret;
}

static sint32 err_inject_item3(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param == 0)
    {
        fusa_radio_set_voltage_1v1_th(1,0);
        fusa_delay_ms(1);
        fusa_radio_set_voltage_1v1_th(1,1);
        fusa_radio_start_voltage_detector_1v1();
    }
    else if(param == 1)
    {
        fusa_radio_set_voltage_1v1_th(0,1);
        fusa_delay_ms(1);
        fusa_radio_set_voltage_1v1_th(1,1);
        fusa_radio_start_voltage_detector_1v1();
    }
    else
    {
        ret = E_FUSA_PAR;
    }
    return ret;
}

static sint32 err_inject_item4(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    if(param == 0)
    {
        fmcw_radio_set_vbg_monitor_vth(1, 0);
        fusa_delay_ms(1);
        fmcw_radio_set_vbg_monitor_vth(1, 1);
        fusa_radio_start_vbg_monitor();
    }
    else if(param == 1)
    {
        fmcw_radio_set_vbg_monitor_vth(0, 1);
        fusa_delay_ms(1);
        fmcw_radio_set_vbg_monitor_vth(1, 1);
        fusa_radio_start_vbg_monitor();
    }
    else
    {
        ret = E_FUSA_PAR;
    }
    return ret;
}

static sint32 err_inject_item5(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    fusa_radio_cpu_clk_lock_err_inject();
    return ret;
}

static sint32 err_inject_item6(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, TRUE);
    fusa_radio_sm_power_detector_fault_injection(5,0x01);
    return ret;
}

static sint32 err_inject_item8(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    ////log_fusa("%s\r\n",__func__);
    return ret;
}

static sint32 err_inject_item11(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
//    (void)radio_powerSave(0, true, true, false);
//    //log_fusa("fusa_radio_sm_if_loopback_fault_injection_irq = %d\r\n", fusa_radio_sm_if_loopback_fault_injection_irq());
//    (void)radio_powerSave(0, true, true, true);
    return ret;
}

static sint32 err_inject_item12(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
//    (void)radio_powerSave(0, true, true, false);
//    //log_fusa("fusa_radio_sm_rf_loopback_fault_injection_irq = %d\r\n", fusa_radio_sm_rf_loopback_fault_injection_irq());
//    (void)radio_powerSave(0, true, true, true);
    return ret;
}

static sint32 err_inject_item13(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_radio_sm_chirp_monitor_fault_injection();
    return ret;
}

static sint32 err_inject_item14(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_radio_temp_detector_threshold_set(-100);
    fusa_radio_sm_over_temp_detector_irq();
    fusa_radio_temp_detector_threshold_set(125.0);
    return ret;
}

static sint32 err_inject_item107(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_dmu_cpu_ram_ecc_en(TRUE);
    raw_read_op(REG_DMU_SYS_MEMRUN_ENA);//must have a read or write or a delay operation,if not, can't inject err success
    uint32 p = 0x77fff0;
    raw_write_op(p,0x12345678);
    //log_fusa("temp = 0x%x\r\n",raw_read_op(p));

    fusa_dmu_cpu_ram_ecc_en(FALSE);
    raw_read_op(REG_DMU_SYS_MEMRUN_ENA);//must have a read or write or a delay operation,if not, can't inject err success

    raw_write_op(p,0x12345671);
    //log_fusa("temp = 0x%x\r\n",raw_read_op(p));

    fusa_dmu_cpu_ram_ecc_en(TRUE);
    raw_read_op(REG_DMU_SYS_MEMRUN_ENA);//must have a read or write or a delay operation,if not, can't inject err success
    uint32 temp = raw_read_op(p);
    //log_fusa("temp = 0x%x\r\n",temp);
    return ret;
}

static sint32 err_inject_item109(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    if(param == 0)
    {
        fusa_can_ecc_err_inject(0, 0);
    }
    else
    {
        fusa_can_ecc_err_inject(1, 0);
    }
    return ret;
}

static sint32 err_inject_item130(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    fusa_fmcw_lockstep_err_inject();
    return ret;
}

static sint32 err_inject_item201(uint32 param)
{
    sint32 ret = E_FUSA_OK;
    //log_fusa("%s\r\n",__func__);
    return ret;
}

/***********************************************************************************************************************
 *
 *
***********************************************************************************************************************/
static sint32 alps_fusa_init_item(fusa_item_e item, fusa_type_e fusa_type, uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    // //log_fusa("%s  ecc sb_status = 0x%x\r\n", __func__, raw_readl(BB_REG_SYS_ECC_SB_STATUS));
    // //log_fusa("%s  ecc db_status = 0x%x\r\n", __func__, raw_readl(BB_REG_SYS_ECC_DB_STATUS));
    // //log_fusa("%s  emu dig_err_status = 0x%x\r\n", __func__, emu_dig_err_states());
    // //log_fusa("%s  emu ana_err_status = 0x%x\r\n", __func__, emu_rf_err_states());
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = init_item1(fusa_type, param);   break;
    case FUSA_ITEM_SM2  : ret = init_item2(fusa_type, param);   break;
    case FUSA_ITEM_SM3  : ret = init_item3(fusa_type, param);   break;
    case FUSA_ITEM_SM4  : ret = init_item4(fusa_type, param);   break;
    case FUSA_ITEM_SM5  : ret = init_item5(fusa_type, param);   break;
    case FUSA_ITEM_SM6  : ret = init_item6(fusa_type, param);   break;
    case FUSA_ITEM_SM8  : ret = init_item8(fusa_type, param);   break;
    case FUSA_ITEM_SM11 : ret = init_item11(fusa_type, param);  break;
    case FUSA_ITEM_SM12 : ret = init_item12(fusa_type, param);  break;
    case FUSA_ITEM_SM13 : ret = init_item13(fusa_type, param);  break;
    case FUSA_ITEM_SM14 : ret = init_item14(fusa_type, param);  break;
    case FUSA_ITEM_SM101: ret = init_item101(fusa_type, param); break;
    case FUSA_ITEM_SM102: ret = init_item102(fusa_type, param); break;
    case FUSA_ITEM_SM103: ret = init_item103(fusa_type, param); break;
    case FUSA_ITEM_SM104: ret = init_item104(fusa_type, param); break;
    case FUSA_ITEM_SM106: ret = init_item106(fusa_type, param); break;
    case FUSA_ITEM_SM107: ret = init_item107(fusa_type, param); break;
    case FUSA_ITEM_SM108: ret = init_item108(fusa_type, param); break;
    case FUSA_ITEM_SM109: ret = init_item109(fusa_type, param); break;
    case FUSA_ITEM_SM120: ret = init_item120(fusa_type, param); break;
    case FUSA_ITEM_SM121: ret = init_item121(fusa_type, param); break;
    case FUSA_ITEM_SM126: ret = init_item126(fusa_type, param); break;
    case FUSA_ITEM_SM130: ret = init_item130(fusa_type, param); break;
    case FUSA_ITEM_SM133: ret = init_item133(fusa_type, param); break;
    case FUSA_ITEM_SM201: ret = init_item201(fusa_type, param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport or unnecessary initializa item, index = %d\r\n", item);
        break;
    }
    return ret;
}
static sint32 alps_fusa_cfg_set(fusa_item_e item,uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = cfg_set_item1(param);   break;
    case FUSA_ITEM_SM2  : ret = cfg_set_item2(param);   break;
    case FUSA_ITEM_SM3  : ret = cfg_set_item3(param);   break;
    case FUSA_ITEM_SM4  : ret = cfg_set_item4(param);   break;
    case FUSA_ITEM_SM6  : ret = cfg_set_item6(param);   break;
    case FUSA_ITEM_SM8  : ret = cfg_set_item8(param);   break;
    case FUSA_ITEM_SM11 : ret = cfg_set_item11(param);  break;
    case FUSA_ITEM_SM12 : ret = cfg_set_item12(param);  break;
    case FUSA_ITEM_SM13 : ret = cfg_set_item13(param);  break;
    case FUSA_ITEM_SM14 : ret = cfg_set_item14(param);  break;
    case FUSA_ITEM_SM201: ret = cfg_set_item201(param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport item, index = %d\r\n", item);
        break;
    }
    return ret;
}
static sint32 alps_fusa_cfg_get(fusa_item_e item,uint32 *param)
{
    sint32 ret = E_FUSA_OK;
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = cfg_get_item1(param);   break;
    case FUSA_ITEM_SM2  : ret = cfg_get_item2(param);   break;
    case FUSA_ITEM_SM3  : ret = cfg_get_item3(param);   break;
    case FUSA_ITEM_SM4  : ret = cfg_get_item4(param);   break;
    case FUSA_ITEM_SM6  : ret = cfg_get_item6(param);   break;
    case FUSA_ITEM_SM8  : ret = cfg_get_item8(param);   break;
    case FUSA_ITEM_SM11 : ret = cfg_get_item11(param);  break;
    case FUSA_ITEM_SM12 : ret = cfg_get_item12(param);  break;
    case FUSA_ITEM_SM13 : ret = cfg_get_item13(param);  break;
    case FUSA_ITEM_SM14 : ret = cfg_get_item14(param);  break;
    case FUSA_ITEM_SM201: ret = cfg_get_item201(param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport item, index = %d\r\n", item);
        break;
    }
    return ret;
}

static sint32 alps_fusa_enable(fusa_item_e item, boolean param)
{
    sint32 ret = E_FUSA_OK;
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = enable_item1(param);   break;
    case FUSA_ITEM_SM2  : ret = enable_item2(param);   break;
    case FUSA_ITEM_SM3  : ret = enable_item3(param);   break;
    case FUSA_ITEM_SM4  : ret = enable_item4(param);   break;
    case FUSA_ITEM_SM5  : ret = enable_item5(param);   break;
    case FUSA_ITEM_SM6  : ret = enable_item6(param);   break;
    case FUSA_ITEM_SM8  : ret = enable_item8(param);   break;
    case FUSA_ITEM_SM11 : ret = enable_item11(param);  break;
    case FUSA_ITEM_SM12 : ret = enable_item12(param);  break;
    case FUSA_ITEM_SM13 : ret = enable_item13(param);  break;
    case FUSA_ITEM_SM14 : ret = enable_item14(param);  break;
    case FUSA_ITEM_SM102: ret = enable_item102(param); break;
    case FUSA_ITEM_SM103: ret = enable_item103(param); break;
    case FUSA_ITEM_SM104: ret = enable_item104(param); break;
    case FUSA_ITEM_SM106: ret = enable_item106(param); break;
    case FUSA_ITEM_SM107: ret = enable_item107(param); break;
    case FUSA_ITEM_SM108: ret = enable_item108(param); break;
    case FUSA_ITEM_SM109: ret = enable_item109(param); break;
    case FUSA_ITEM_SM120: ret = enable_item120(param); break;
    case FUSA_ITEM_SM121: ret = enable_item125(param); break;
    case FUSA_ITEM_SM126: ret = enable_item126(param); break;
    case FUSA_ITEM_SM130: ret = enable_item130(param); break;
    case FUSA_ITEM_SM201: ret = enable_item201(param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport item, index = %d\r\n", item);
        break;
    }
    return ret;
}

static sint32 alps_fusa_run(fusa_item_e item, uint32 param)
{
    sint32 ret = E_FUSA_OK;
    // float32 start_time = fusa_current_time_ms();
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = run_item1(param);   break;
    case FUSA_ITEM_SM6  : ret = run_item6(param);   break;
    case FUSA_ITEM_SM8  : ret = run_item8(param);   break;
    case FUSA_ITEM_SM11 : ret = run_item11(param);  break;
    case FUSA_ITEM_SM12 : ret = run_item12(param);  break;
    case FUSA_ITEM_SM13 : ret = run_item13(param);  break;
    case FUSA_ITEM_SM14 : ret = run_item14(param);  break;
    case FUSA_ITEM_SM101: ret = run_item101(param); break;
    case FUSA_ITEM_SM120: ret = run_item120(param); break;
    case FUSA_ITEM_SM126: ret = run_item126(param); break;
    case FUSA_ITEM_SM201: ret = run_item201(param); break;
    case FUSA_ITEM_SM901: ret = run_item901(param); break;
    case FUSA_ITEM_SM902: ret = run_item902(param); break;
    case FUSA_ITEM_SM904: ret = run_item904(param); break;
    case FUSA_ITEM_SM905: ret = run_item905(param); break;
    case FUSA_ITEM_SM906: ret = run_item906(param); break;
    case FUSA_ITEM_SM907: ret = run_item907(param); break;
    case FUSA_ITEM_SM908: ret = run_item908(param); break;
    case FUSA_ITEM_SM910: ret = run_item910(param); break;
    case FUSA_ITEM_SM911: ret = run_item911(param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport item, index = %d\r\n", item);
        break;
    }
    //  //log_fusa("times = %f\r\n", fusa_current_time_ms() - start_time);
    return ret;
}
static sint32 alps_fusa_err_inject(fusa_item_e item, uint32 param)
{
    sint32 ret = E_FUSA_OK;
    switch (item)
    {
    case FUSA_ITEM_SM1  : ret = err_inject_item1(param);   break;
    case FUSA_ITEM_SM2  : ret = err_inject_item2(param);   break;
    case FUSA_ITEM_SM3  : ret = err_inject_item3(param);   break;
    case FUSA_ITEM_SM4  : ret = err_inject_item4(param);   break;
    case FUSA_ITEM_SM5  : ret = err_inject_item5(param);   break;
    case FUSA_ITEM_SM6  : ret = err_inject_item6(param);   break;
    case FUSA_ITEM_SM8  : ret = err_inject_item8(param);   break;
    case FUSA_ITEM_SM11 : ret = err_inject_item11(param);  break;
    case FUSA_ITEM_SM12 : ret = err_inject_item12(param);  break;
    case FUSA_ITEM_SM13 : ret = err_inject_item13(param);  break;
    case FUSA_ITEM_SM14 : ret = err_inject_item14(param);  break;
    case FUSA_ITEM_SM107: ret = err_inject_item107(param); break;
    case FUSA_ITEM_SM109: ret = err_inject_item109(param); break;
    case FUSA_ITEM_SM130: ret = err_inject_item130(param); break;
    case FUSA_ITEM_SM201: ret = err_inject_item201(param); break;
    default:
        ret = E_FUSA_NOSPT;
        //log_fusa("unsupport item, index = %d\r\n", item);
        break;
    }
    return ret;
}

static sint32 alps_fusa_err_handle_register(fusa_err_handle_t handle)
{
    fusa_err_handler_customer = handle;
    return E_FUSA_OK;
}
static sint32 alps_fusa_check_err_code(uint32 *buf, uint32 buf_len)
{
    uint8 i = 0;

    ////log_fusa("==============functy safety startup err code check====================\r\n");
    uart_printfs("==============functy safety startup err code check====================\r\n");
    buf[i++] = raw_read_op(REG_EMU_REBOOT_CNT);
    buf[i++] = raw_read_op(REG_EMU_REBOOT_LIMIT);
    buf[i++] = raw_read_op(REG_EMU_RF_ERR_SS1_CODE0);
    buf[i++] = raw_read_op(REG_EMU_RF_ERR_SS1_CODE1);
    buf[i++] = raw_read_op(REG_EMU_RF_ERR_SS1_CODE2);
    buf[i++] = raw_read_op(REG_EMU_RF_ERR_SS1_CODE3);
    buf[i++] = raw_read_op(REG_EMU_DIG_ERR_SS1_CODE0);
    buf[i++] = raw_read_op(REG_EMU_DIG_ERR_SS1_CODE1);
    buf[i++] = raw_read_op(REG_EMU_DIG_ERR_SS1_CODE2);
    buf[i++] = raw_read_op(REG_EMU_DIG_ERR_SS1_CODE3);
    buf[i++] = raw_read_op(REG_EMU_RF_ERR_SS2_CODE);
    buf[i++] = raw_read_op(REG_EMU_DIG_ERR_SS2_CODE);

    i = 0;
    ////log_fusa("REG_EMU_REBOOT_CNT            = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_REBOOT_CNT = ",&buf[i++]);
    ////log_fusa("REG_EMU_REBOOT_LIMIT          = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_REBOOT_LIMIT = ",&buf[i++]);
    ////log_fusa("REG_EMU_RF_ERR_SS1_CODE0      = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_RF_ERR_SS1_CODE0 = ",&buf[i++]);
    ////log_fusa("REG_EMU_RF_ERR_SS1_CODE1      = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_RF_ERR_SS1_CODE1 = ",&buf[i++]);
    ////log_fusa("REG_EMU_RF_ERR_SS1_CODE2      = 0x%x\r\n", buf[i++]);
    uart_printfs("EG_EMU_RF_ERR_SS1_CODE2 = ",&buf[i++]);
    ////log_fusa("REG_EMU_RF_ERR_SS1_CODE3      = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_RF_ERR_SS1_CODE3 = ",&buf[i++]);
    ////log_fusa("REG_EMU_DIG_ERR_SS1_CODE0     = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_DIG_ERR_SS1_CODE0 = ",&buf[i++]);
    ////log_fusa("REG_EMU_DIG_ERR_SS1_CODE1     = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_DIG_ERR_SS1_CODE1 = ",&buf[i++]);
    ////log_fusa("REG_EMU_DIG_ERR_SS1_CODE2     = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_DIG_ERR_SS1_CODE2 = ",&buf[i++]);
    ////log_fusa("REG_EMU_DIG_ERR_SS1_CODE3     = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_DIG_ERR_SS1_CODE3 = ",&buf[i++]);
    ////log_fusa("REG_EMU_RF_ERR_SS2_CODE       = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_RF_ERR_SS2_CODE = ",&buf[i++]);
    ////log_fusa("REG_EMU_DIG_ERR_SS2_CODE      = 0x%x\r\n", buf[i++]);
    uart_printfs("REG_EMU_DIG_ERR_SS2_CODE = ",&buf[i++]);

    ////log_fusa("===================================================================\r\n");
    uart_printfs("===================================================================\r\n");
    return E_FUSA_OK;
}
static sint32 alps_fusa_command(FusaCommand_t *p_fusa_cmd)
{
    sint32 ret = E_FUSA_OK;
    switch (p_fusa_cmd->cmd)
    {
    case FUSA_CMD_INIT_BB_MEM:
        fusa_bb_clear_memory(p_fusa_cmd->temp);
        break;
    case FUSA_CMD_SCAN_ONE_TIME:
        fusa_radar_scan_one_time(p_fusa_cmd->temp);
        break;
    default:
        break;
    }
    return ret;
}
static void ana_irq_handle(void)
{
    //log_fusa("%s\r\n", __func__);

    uint32 value_clear = 0;
    uint32 temp[6] = {0};

    fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, FALSE);
    uint32 value = raw_read_op(REG_EMU_RF_ERR_IRQ_STA);

    //log_fusa("[fusa_isr] REG_EMU_RF_ERR_IRQ_STA = 0x%x  time = %f\r\n",value,fusa_current_time_ms());

    if (value & RF_ITEM_LDO_MT) {        //LDO_MT bit
            value_clear = fusa_radio_switch_bank(10);
            temp[0] = fusa_radio_reg_read(0x47);
            temp[1] = fusa_radio_reg_read(0x48);
            temp[2] = fusa_radio_reg_read(0x49);
            temp[3] = fusa_radio_reg_read(0x4a);
            temp[4] = fusa_radio_reg_read(0x4b);
            fusa_radio_switch_bank(value_clear);
            //log_fusa("LDO_MT bit irq set. status 0x4b~0x47 = 0x%x %x %x %x %x\r\n", temp[4], temp[3], temp[2], temp[1], temp[0]);
            exe_cust_err_handle(FUSA_ITEM_SM1, temp);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
            emu_rf_irq_ena(RF_ITEM_LDO_MT, FALSE);
    } else if (value & RF_ITEM_AVDD33_MT) {        //AVDD33_MT bit
            //log_fusa("AVDD33_MT bit irq set.\n");
            /* restart monitor */
            temp[0] = fusa_radio_return_voltage_detector_3v3_irq();
            exe_cust_err_handle(FUSA_ITEM_SM2, temp);
            // fusa_radio_3v3_th_set(10, -10);
            fusa_radio_clear_voltage_detector_3v3();
            fusa_radio_start_voltage_detector_3v3();
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_DVDD11_MT) {        //DVDD11_MT bit
            //log_fusa("DVDD11_MT bit irq set.\n");
            /* restart monitor */
            exe_cust_err_handle(FUSA_ITEM_SM3, NULL);
            // fusa_radio_set_voltage_1v1_th(1,1);
            fusa_radio_clear_voltage_detector_1v1();
            fusa_radio_start_voltage_detector_1v1();
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_VBG_MT) {        //VBG_MT bit
            fusa_radio_clear_vbg_monitor();
            //log_fusa("VBG_MT bit irq set.\n");
            /* restart monitor */
            exe_cust_err_handle(FUSA_ITEM_SM4, NULL);
            // fmcw_radio_set_vbg_monitor_vth(1, 1);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
            fusa_radio_start_vbg_monitor();
    } else if (value & RF_ITEM_PLL_LOCK_DT) {        //PLL_LOCK_DT bit
            //log_fusa("PLL_LOCK_DT bit irq set.\n");
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_RF_POWER_DT) {        //RF_POWER_DT bit
            //log_fusa("RF_POWER_DT bit irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM6, NULL);
            fusa_radio_clear_power_detector();
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
            emu_rf_irq_ena(RF_ITEM_RF_POWER_DT, FALSE);
    } else if (value & RF_ITEM_RX_SAT_DT) {        //RX_SAT_DT bit
            ////log_fusa("RX_SAT_DT bit irq set.\n");
            temp[0] = raw_read_op(BB_REG_AGC_IRQ_STATUS);
            exe_cust_err_handle(FUSA_ITEM_SM8, temp);
            //log_fusa("RX_SAT_DT bit irq set. status = 0x%x\r\n",temp[0]);
            raw_write_op(BB_REG_AGC_SAT_CNT_CLR_FRA,  1);
            raw_write_op(BB_REG_AGC_IRQ_CLR, 0xFFF);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
            // func_safety_clear_reg_emu_rf_err_irq_ena(FUNC_SAFETY_BIT_7_SHIFT);
    } else if (value & RF_ITEM_CHIRP_MT) {                        //CHIRP_MT bit
            //log_fusa("CHIRP_MT bit set irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM13, NULL);
            fusa_radio_clear_chirp_monitor();
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_TEMP_OVER) {                        //TEMP_OVER bit
            //log_fusa("TEMP_OVER bit irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM14, NULL);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_IF_LOOPBACK) {                        //IF_LOOPBACK bit
            //log_fusa("IF_LOOPBACK bit irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM11, NULL);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & RF_ITEM_RF_LOOPBACK) {                        //RX_LOOPBACK bit
            //log_fusa("RF_LOOPBACK bit irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM12, NULL);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else if (value & (1 << 12)) { //GAIN_CK VGA gain consistence check bit
            //log_fusa("GAIN_CK bit set irq set.\n");
            exe_cust_err_handle(FUSA_ITEM_SM201, NULL);
            value_clear = raw_read_op(REG_EMU_RF_ERR_CLR);
            raw_write_op(REG_EMU_RF_ERR_CLR, value | value_clear);
    } else {
            //log_fusa("Unused bit irq set.\n");
    }

    fusa_dmu_irq_en(FUSA_ANA_IRQ_NUM, TRUE);
}
static void dig_irq_handle(void)
{
    uint32 value = 0;
    uint32 value_clear = 0;
    uint32 temp[6];
    //disable digital error irq
    fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, FALSE);
    value = raw_read_op(REG_EMU_DIG_ERR_IRQ_STA);

    //log_fusa("[fusa_isr] REG_EMU_DIG_ERR_IRQ_STA = 0x%x  time = %f\r\n",value,fusa_current_time_ms());

    if (value & DIG_ITEM_BB_LBIST) {
            //log_fusa("BB_LBIST bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM101, NULL);
            emu_bb_lbist_clear();
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CPU_CCM_ECC) {
            //log_fusa("CPU_TCM_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM102, NULL);
            core_reg_bit_set(REG_CPU_ERP_CTRL,31,1);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CPU_CACHE_ECC) {
            //log_fusa("CPU_CACHE_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM103, NULL);
            core_reg_bit_set(REG_CPU_ERP_CTRL,31,1);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if ((value & DIG_ITEM_BB_SRAM_ECC) ||(value & DIG_ITEM_BB_ROM_ECC)) {
            //log_fusa("BB_SRAM_ECC bit irq set.\r\n");
            temp[0] = raw_read_op(BB_REG_SYS_ECC_SB_STATUS);
            temp[1] = raw_read_op(BB_REG_SYS_ECC_DB_STATUS);
            //log_fusa("status sb=0x%x  db=0x%x\r\n",temp[0],temp[1]);
            exe_cust_err_handle(FUSA_ITEM_SM104, temp);
            raw_write_op(BB_REG_SYS_ECC_SB_CLR, temp[0]);
            raw_write_op(BB_REG_SYS_ECC_DB_CLR, temp[1]);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CPU_ROM_ECC) {
            //log_fusa("CPU_ROM_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM106, NULL);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CPU_SRAM_ECC) {
            //log_fusa("CPU_SRAM_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM107, NULL);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_OTP_ECC) {
            //log_fusa("OTP_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM108, NULL);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CAN0_ECC) {
            //log_fusa("CAN0_ECC bit irq set.\r\n");
            temp[0] = 0;
            exe_cust_err_handle(FUSA_ITEM_SM109, temp);
            raw_write_op(REL_REGBASE_CAN0 + REG_CAN_ISR_OFFSET,0x00030000);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CAN1_ECC) {
            //log_fusa("CAN1_ECC bit irq set.\r\n");
            temp[0] = 1;
            exe_cust_err_handle(FUSA_ITEM_SM109, temp);
            raw_write_op(REL_REGBASE_CAN1 + REG_CAN_ISR_OFFSET,0x00030000);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CPU_WDT) {
            //log_fusa("CPU_WDT bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CAN0_ERR) {
            // //log_fusa("CAN0_ERR bit irq set.\r\n");
            // //clear CAN0.IR register,write 1 clear
            // value_clear = raw_read_op(REL_REGBASE_CAN0 + REG_CAN_ISR_OFFSET);
            // //log_fusa("IR = 0X%x\r\n",value_clear);
            // raw_write_op(REL_REGBASE_CAN0 + REG_CAN_ISR_OFFSET, value_clear | BIT_INTERRUPT_BEC | BIT_INTERRUPT_BEU);
            // value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            // raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CAN0_LP) {
            //log_fusa("CAN0_LP bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_I2C_ACK) {
            //log_fusa("I2C_ACK bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_CHIRP_LS) {
            //log_fusa("CHIRP_LS bit irq set.\r\n");
            // value_clear = fusa_radio_switch_bank(NULL,10);
            // fmcw_radio_reg_write(NULL,0x70,0x0d);//clear irq
            // fmcw_radio_reg_write(NULL,0x70,0x0c);
            // fusa_radio_switch_bank(NULL,value_clear);
            fusa_fmcw_lockstep_irq_clr();
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_XIP_ECC) {
            //log_fusa("XIP_ECC bit irq set.\r\n");
            exe_cust_err_handle(FUSA_ITEM_SM133, NULL);
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_SPI_LP) {
            //log_fusa("SPI_LP bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_SPI_CRC) {
            //log_fusa("SPI_CRC bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else if (value & DIG_ITEM_FLASH_CRC) {
            //log_fusa("FLASH_CRC bit irq set.\r\n");
            value_clear = raw_read_op(REG_EMU_DIG_ERR_CLR);
            raw_write_op(REG_EMU_DIG_ERR_CLR, value | value_clear);
    } else {
            //log_fusa("Unused bit irq set.\r\n");
    }

    //enable digital error irq
    fusa_dmu_irq_en(FUSA_DIG_IRQ_NUM, TRUE);
}
static void bb_sb_ecc_irq_handle(void)
{
    uint32 temp[2];
    temp[0] = raw_read_op(BB_REG_SYS_ECC_SB_STATUS);
    temp[1] = raw_read_op(BB_REG_SYS_ECC_DB_STATUS);
    // //log_fusa("[fusa_isr] bb_sb_ecc_err status sb= 0x%x  db = 0x%x  time = %f\r\n", temp[0], temp[1], fusa_current_time_ms());
    if(fusa_bb_state_get())
    {
        if((temp[0] &(~0x2000)) || (temp[1] &(~0x2000)))
        {
            exe_cust_err_handle(FUSA_ITEM_SM104, temp);
        }
        fusa_bb_state_set(FALSE);
    }
    else
    {
        exe_cust_err_handle(FUSA_ITEM_SM104, temp);
    }
    fusa_bb_mem_ecc_err_clr();
}
static void alps_fusa_irq_handle(void)
{
    uint32 irq_num = core_irq_case();
    switch (irq_num)
    {
    case FUSA_ANA_IRQ_NUM   : ana_irq_handle();         break;
    case FUSA_DIG_IRQ_NUM   : dig_irq_handle();         break;
    case FUSA_BB_IRQ_ECC_SB : bb_sb_ecc_irq_handle();   break;
    case FUSA_ANA_IRQ_SS1   : emu_enter_ss1();          break;
    case FUSA_DIG_IRQ_SS1   : emu_enter_ss1();          break;
    default:
        //log_fusa("unintended irq_num = 0x%x\r\n", irq_num);
        break;
    }
    emu_dig_err_clr(DIG_ITEM_CPU_SRAM_ECC);
}


void attach_fusafw_fun_alps(fusa_fw_fun_t *p_func)
{
    emu_set_boot_done();
    p_func->init_item = alps_fusa_init_item;
    p_func->cfg_set = alps_fusa_cfg_set;
    p_func->cfg_get = alps_fusa_cfg_get;
    p_func->run = alps_fusa_run;
    p_func->enable = alps_fusa_enable;
    p_func->err_inject = alps_fusa_err_inject;
    p_func->err_handle_register = alps_fusa_err_handle_register;
    p_func->check_err_code = alps_fusa_check_err_code;
    p_func->fusa_command = alps_fusa_command;
    p_func->irq_handle = alps_fusa_irq_handle;
}
