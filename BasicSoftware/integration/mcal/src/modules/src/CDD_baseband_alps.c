#include "Platform_Types.h"
#include "CDD_Baseband.h"
#include "calterah_limits.h"
#include "baseband_hw.h"
#include "baseband_alps_FM_reg.h"
#include "CDD_Radio.h"

static bb_isr_callback bb_sam_isr_func = NULL;
static bb_isr_callback bb_done_isr_func = NULL;
bool bb_sam_callback(void)
{
	uint8 teststr[40];
	sprintf(teststr,"enter bb sam done.\r\n");
	Uart_Transmit(0,teststr,strlen(teststr));
	/* enter power save */
	radio_powerSave(0,true,true,true);
}

bool bb_done_callback(void)
{
	uint8 teststr[40];
	sprintf(teststr,"enter bb done done.\r\n");
	Uart_Transmit(0,teststr,strlen(teststr));
}

void baseband_isr_callback(bb_isr_type type,bb_isr_callback func)
{
	if (type == sam_done)
	{
		bb_sam_isr_func = func;
	}else if(type == bb_done)
	{
		bb_done_isr_func = func;
	}else
	{

	}
}

void sam_done_isr(void)
{

	baseband_write_reg(BB_REG_SYS_IRQ_CLR, BB_IRQ_CLEAR_SAM_DONE);

	if (bb_sam_isr_func != NULL)
	{
		bb_sam_isr_func();
	}
}

void bb_done_isr(void)
{

	baseband_write_reg(BB_REG_SYS_IRQ_CLR, BB_IRQ_CLEAR_BB_DONE);

	if (bb_done_isr_func != NULL)
	{
		bb_done_isr_func();
	}
}

static sint32 alps_baseband_init(BasebandInitializationParams_t *cfg, uint8_t cfg_nums,RadioInitializationParams_t *rcfg)
{
	cfg->com_params = rcfg;
	cfg->nvarray = rcfg->nvarray;
	sint32 ret = 0;
	BasebandDriverCommand_t baseband_init_cmd = {0};
	BasebandDriverInit_t bb = {0};
	baseband_init_cmd.bb = &bb;
	
    /* Baseband clock enable */
    baseband_clock_init();

    int_handler_install(INT_BB_SAM,sam_done_isr);
    int_handler_install(INT_BB_DONE,bb_done_isr);

    baseband_isr_callback(sam_done,bb_sam_callback);
    baseband_isr_callback(bb_done,bb_done_callback);

    int_enable(INT_BB_SAM);
    int_enable(INT_BB_DONE);
	/* default enable sample done & bb done irq in DMU module */
	baseband_irq_config(INT_BB_SAM, 1);
	baseband_irq_config(INT_BB_DONE, 1);

    /* Register baseband configuration */
    baseband_init_cmd.command_id = BB_CONFIG_INSTALL;
    baseband_init_cmd.bb->cfg = cfg;
    baseband_init_cmd.cfg_nums = cfg_nums;

    ret = baseband_command(&baseband_init_cmd);

    return ret;
}

static sint32 alps_baseband_frameConfig(uint8 FrameId, BasebandInitPhase_e BasebandInitPhase)
{
    sint32 ret = 0;

    BasebandDriverInit_t *BasebandInitInfo = baseband_driver_info_get(FrameId);
    if(NULL == BasebandInitInfo){
    	ret = -1;
    	return ret;
    }

    switch(BasebandInitPhase)
    {
        case BB_SET_FRM_CFG:
            baseband_compute_sys_params(BasebandInitInfo->cfg, BasebandInitInfo->sys_params);
            baseband_hw_init(BasebandInitInfo);
            break;

        case BB_SET_DC_CFG:
            baseband_dc_calib_set_cfg(BasebandInitInfo->frame_type_id);
            break;

        case BB_CAL_DC_VAL:
            baseband_dc_calib_cal_dc(BasebandInitInfo->cfg);
            break;

        case BB_RST_DC_CFG:
            baseband_dc_calib_rst_cfg(BasebandInitInfo->cfg, false);
            break;

        case BB_SET_SHAD_BNK_CFG:
            baseband_shadow_bnk_init(BasebandInitInfo->cfg);
            break;
        
        case BB_DOA_REINIT_HP:
            baseband_doa_init_huangpu(BasebandInitInfo->cfg, BasebandInitInfo->frame_type_id);
            break;

        default:
            break;
    }

    return ret;
}

static sint32 alps_baseband_run(BasebandDriverContext_t *pBasebandContext)
{
    sint32 ret = 0;
    do
    {
        /* Check whether bb_hw is busy (0xc0005c != 0) */
        if(baseband_hw_status_check() != BB_IDLE)
        {
            ret = 2;
            break;
        }

		baseband_int_clear(BB_IRQ_CLEAR_ALL);

		/* Baseband irq config oin DMU module */
		baseband_irq_config(INT_BB_SAM, (pBasebandContext->sys_irq_en & 0x4));
		baseband_irq_config(INT_BB_DONE, (pBasebandContext->sys_irq_en & 0x1));

        /* Start bb */
        baseband_hw_start_with_params(pBasebandContext->sys_en, BB_IRQ_ENABLE_BB_DONE | BB_IRQ_ENABLE_SAM_DONE);

    } while (0);

    return ret;
}

static sint32 alps_baseband_command(BasebandDriverCommand_t *pBasebandCommand)
{
    sint32 ret = 0;

    switch(pBasebandCommand->command_id){
        case BB_SET_FRAME_TYPE_ID:
            if((uint8_t)*pBasebandCommand->pParam > (NUM_FRAME_TYPE - 1))
            {
                /* Invalid fame_type_id*/
                ret = 1;
                break;
            }

            if(baseband_frame_interleave_cfg((uint8_t)(*pBasebandCommand->pParam)) != (uint8_t)(*pBasebandCommand->pParam))
            {
                /* Read back frame type not match with settings */
                ret = 2;
                break;
            }
        break;

        case BB_SWITCH_MEM_TABLE:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            if(*pBasebandCommand->pParam > BB_MEM_MAX_NUM)
            {
                /* Baseband memory table out of range of 0-9 */
                ret = 1;
                break;
            }
            /* Switch to memory table */
            *pBasebandCommand->pResult = baseband_switch_mem_access(*pBasebandCommand->pParam);
        break;

        case BB_WRITE_MEM_TABLE:
            if(NULL == pBasebandCommand->pParam)
            {
                ret = 2;
                break;
            }
            baseband_write_mem_table(*pBasebandCommand->pParam++, *pBasebandCommand->pParam);
        break;

        case BB_READ_MEM_TABLE:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = baseband_read_mem_table(*pBasebandCommand->pParam);
        break;

        case BB_GET_OBJ_INFO:
            if(NULL == pBasebandCommand->pParam)
            {
                ret = 2;
                break;
            }
            pBasebandCommand->pResult = (uint32_t *)baseband_get_obj_info(*pBasebandCommand->pParam);
        break;

        case BB_GET_DC:
            pBasebandCommand->pResult = (uint32_t*)get_dc_offset();
        break;

        case BB_RESET_COMB:
            if(NULL == pBasebandCommand->bb)
            {
                ret = 2;
                break;
            }
            baseband_combiner_reset(pBasebandCommand->bb->cfg);
        break;

        case BB_SWTICH_BUF_STORE:
            //(void)baseband_switch_buf_store((BasebandSamSinker_e)*pBasebandCommand->pParam);
        	pBasebandCommand->pResult = (uint32_t *)baseband_switch_buf_store((BasebandSamSinker_e)*pBasebandCommand->pParam);
            break;

        case BASEBAND_READ_REG:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = baseband_read_reg(*pBasebandCommand->pParam);
        break;

        case BASEBAND_WRITE_REG:
            baseband_write_reg(*pBasebandCommand->pParam++, *pBasebandCommand->pParam);
        break;

        case BB_CONFIG_INSTALL:
            ret = baseband_driver_info_register(pBasebandCommand->bb->cfg, pBasebandCommand->cfg_nums);
        break;

        case BB_GET_CONFIG:
            if(NULL == pBasebandCommand->pParam)
            {
                ret = 2;
                break;
            }
            pBasebandCommand->pResult = (uint32_t *)baseband_driver_info_get((uint8_t)*pBasebandCommand->pParam);
        break;

        case BB_GET_CUR_CONFIG:
        	pBasebandCommand->pResult = (uint32_t *)baseband_cur_driver_info_get();
        break;

        case BB_GET_FRAME_TYPE_ID:
            if(NULL == pBasebandCommand->pResult)
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = (uint32_t)baseband_get_rtl_frame_type();
        break;

        case BB_GET_CFAR_NUM_OBJ:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = baseband_read_reg(BB_REG_CFR_NUMB_OBJ);
        break;

        case BB_GET_CFAR_SIZE_OBJ:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = baseband_read_reg(BB_REG_CFR_SIZE_OBJ);
        break;

        case BB_GET_DOA_NUM_GRP:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = baseband_read_reg(BB_REG_DOA_NUMB_GRP);
        break;

        case BB_DDM_TX_ON_NUM:
            pBasebandCommand->pResult = (uint32_t *)ddm_get_tx_on_num();
        break;

        case BB_DDM_TX_VEL_POS:
            pBasebandCommand->pResult = (uint32_t *)ddm_get_ddm_tx_vel_pos();
        break;

        case BB_DDM_INIT_VEL_NFFT:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = (uint32_t)read_init_vel_nfft((uint8_t)*pBasebandCommand->pParam);
        break;

        case BB_DDM_INIT_NVARRAY:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = (uint32_t)read_init_nvarray((uint8_t)*pBasebandCommand->pParam);
        break;

        case BB_DDM_INIT_TX_GRP:
            if((NULL == pBasebandCommand->pParam) || (NULL == pBasebandCommand->pResult))
            {
                ret = 2;
                break;
            }
            *pBasebandCommand->pResult = (uint32_t)read_init_tx_groups((uint8_t)*pBasebandCommand->pParam);
        break;

        case BB_SET_NVARRAY_REG:
            if(NULL == pBasebandCommand->pParam)
            {
                ret = 2;
                break;
            }
            baseband_write_reg(BB_REG_SYS_SIZE_BPM    , *pBasebandCommand->pParam++);
            baseband_write_reg(BB_REG_SYS_SIZE_VEL_FFT, *pBasebandCommand->pParam);
        break;

        case BB_DONE_WAIT:
            ret = baseband_wait_bb_done((BasebandDoneWait_t *)pBasebandCommand->pParam);
            break;

		case BB_SAM_DONE_WAIT:
			ret = baseband_wait_sam_done((BasebandDoneWait_t *)pBasebandCommand->pParam);
			break;

        case BB_INT_CLEAR:
            (void)baseband_int_clear(*pBasebandCommand->pParam);
            break;

        case BB_INT_ENABLE:
            (void)baseband_irq_config((uint8_t)*pBasebandCommand->pParam, 1);
            break;

        case BB_INT_DISABLE:
            (void)baseband_irq_config((uint8_t)*pBasebandCommand->pParam, 0);
            break;

        case BB_GET_N2D_INFO:
            if((NULL == pBasebandCommand->pParam))
            {
                ret = 2;
                break;
            }
            pBasebandCommand->pResult = (uint32_t *)baseband_get_n2d_info(*pBasebandCommand->pParam);
            break;
        default:
			break;
    }
    return ret;
}

static sint32 alps_baseband_stop(void)
{
	sint32 ret = 0;

    // step1: fmcw_radio_stop
    // step2: baseband_hw_stop
    baseband_hw_stop();
    // step3: dump & HIL stop and restore

    // TODO: Dump stop
    // baseband_dump_stop(); /* stream data dump stop */
    // bb_clk_restore();     /* restore bb clock after dumping sample debug data */
    
    // TODO: For HIL
    // if (true == baseband_data_proc_req()) {
    //         baseband_data_proc_init(); /* re-init data process chain */
    //         dmu_hil_input_mux(HIL_AHB); /* trun off dbgbus input */
    //         dump_done();           /* done signal to  fpga board */
    // }

    // TODO: Dump stop
    // /* dmu will be switched to GPIO mode at end of dump stop */
    // if (io_get_dmumode() != SYS_DMU_SEL_GPIO) {
    //         io_sel_dmumode(SYS_DMU_SEL_GPIO);
    // }

    return ret;
}

void attach_basebandfw_fun_set_alps()
{
    baseband_fw_fun_set_t *p_funset = hal_get_baseband_handle();

    p_funset->baseband_init_set = alps_baseband_init;

    p_funset->baseband_frameConfig_set = alps_baseband_frameConfig;

    p_funset->baseband_run_set = alps_baseband_run;

    p_funset->baseband_command_set = alps_baseband_command;

    p_funset->baseband_stop_set = alps_baseband_stop;

}
