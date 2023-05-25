#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "Platform_Types.h"
#include "embARC_assert.h"
#include "embARC_error.h"
#include "math.h"
#include "calterah_math.h"
#include "CDD_Radar_Common_Cfg.h"
#include "CDD_radio.h"
#include "baseband_ext.h"


#ifndef SRC_MCAL_INC_RADAR_CDD_BASEBAND_H_
#define SRC_MCAL_INC_RADAR_CDD_BASEBAND_H_


/*!
    struct  BasebandCommonInitializationParams_t
*/
typedef RadarCommonInitializationParams_t BasebandCommonInitializationParams_t;

typedef struct {

    uint8_t dec_factor;
    float adc_sample_start;                            /* chirp sampling start offset in us */
    float adc_sample_end;                              /* chirp sampling end offset in us */
    char rng_win[8];                                   /* range window type: cheb or square */
    char vel_win[8];                                   /* velocity window type: cheb or square */
    float rng_win_params[3];                           /* range window parameters */
    float vel_win_params[3];                           /* velocity window parameters */
    uint16_t rng_nfft;                                 /* FFT size of range gate */
    uint16_t vel_nfft;                                 /* FFT size of Doppler gate */
    uint16_t rng_fft_scalar;                           /* FFT scaling control of range gate FFT */
    uint16_t vel_fft_scalar;                           /* FFT scaling control of velocity gate FFT */
    bool fft_nve_bypass;                               /* FFT NVE mode */
    uint8_t fft_nve_shift;                             /* FFT NVE more shift */
    uint16_t fft_nve_ch_mask;                          /* FFT NVE channel mask */
    float fft_nve_default_value;                       /* FFT NVE default value, take effect only when NVE is bypassed */
    float fft_nve_without_interference;                /* NVE value without interference */
    float fft_nve_threash;                             /* threashold times for identification interference */
    uint16_t fft_nve_rangeindex_start;                 /* start index for calculating nve */
    uint16_t fft_nve_rangeindex_end;                   /* last index for calculating nve */
    uint8_t cfar_pk_en;                                /* peak enabler bit mask */
    uint8_t cfar_pk_win_size1[CFAR_MAX_GRP_NUM];       /* peak detector sliding window size1 */
    uint8_t cfar_pk_win_size2[CFAR_MAX_GRP_NUM];       /* peak detector sliding window size2 */
    uint8_t cfar_pk_threshold[CFAR_MAX_GRP_NUM];       /* exponent threshold for CUT to be considered as object candidate. The larger the value, the lower the threshold is. maximum value is 31 */
    uint8_t cfar_sliding_win;                          /* CFAR sliding window type, 0: retangle, 1: cross */
    uint16_t cfar_recwin_decimate;                     /* decimating scheme for rectangle window, 00b: no decimation, 10b: dopper, 01b: range, 11b: both dimensions */
    uint16_t cfar_recwin_msk[CFAR_MAX_RECWIN_MSK_LEN]; /* Rectangular Window mask. 11x8 elements in total. First 11 elements defines the rectangular mask for first region. 2nd 11 elements are for 2nd region */
    uint16_t cfar_region_algo_type;                    /* CFAR per sub-region CFAR Algo type bitMsk for 8 sub-groups. 00b: CA-CFAR 01b: OS-CFAR 10b: SOGO-CFAR 11b: NR-CFAR */
    float cfar_os_rnk_ratio[CFAR_MAX_GRP_NUM];         /* The scalar of related rank */
    uint8_t cfar_os_rnk_sel[CFAR_MAX_GRP_NUM];         /* The mux control to select the final source of rank threshold, 0:tdec, 1: ratio * number of RCs */
    uint8_t cfar_os_tdec[CFAR_MAX_GRP_NUM];            /* The deterministic  rank threshold */
    float cfar_os_alpha[CFAR_MAX_GRP_NUM];             /* The scalar of CUT power before ranking */
    uint8_t cfar_combine_dirs;                         /* # of combining directions. Value 0 means non-coherent combination */
    float cfar_combine_thetas[MAX_CFAR_DIRS];          /* predefined combining directions of azimuth */
    float cfar_combine_phis[MAX_CFAR_DIRS];            /* predefined combining directions of elevation */
    uint8_t cfar_crswin_rng_size;                      /* cross window length alone range-gate */
    uint8_t cfar_crswin_rng_skip;                      /* cross window guard size alone range-gate */
    uint8_t cfar_crswin_vel_size;                      /* cross window length alone velocity-gate */
    uint8_t cfar_crswin_vel_skip;                      /* cross window guard size alone velocity-gate */
    char cfar_mimo_win[8];                             /* MIMO CFAR window type */
    float cfar_mimo_win_params[3];                     /* MIMO CFAR window parameter */
    uint8_t cfar_noise_type;                           /* 8 region noise level output select register: 1 from CFAR local estimation; 0: from NVE */
    float cfar_nr_alpha[CFAR_MAX_GRP_NUM];             /* Scalar of power of CUT before comparison or ranking */
    float cfar_nr_beta1[CFAR_MAX_GRP_NUM];             /* Lower bound scalar in construction of set of reference powers */
    float cfar_nr_beta2[CFAR_MAX_GRP_NUM];             /* Upper bound scalar in construction of set of reference powers */
    float cfar_nr_rnk_ratio[CFAR_MAX_GRP_NUM];         /* The scalar of related rank */
    uint8_t cfar_nr_rnk_sel[CFAR_MAX_GRP_NUM];         /* The mux control to select the final source of rank threshold in scheme 2 */
    uint8_t cfar_nr_scheme_sel[CFAR_MAX_GRP_NUM];      /* Select output from one of three schemes */
    uint8_t cfar_nr_tdec[CFAR_MAX_GRP_NUM];            /* Pre-programmed Rank threshold for scheme 2 */
    uint16_t cfar_region_sep_rng[3];                   /* RDM region partition in range dimension */
    uint16_t cfar_region_sep_vel[8];                   /* RDM region partition in velocity dimension */
    float cfar_sogo_alpha[CFAR_MAX_GRP_NUM];           /* The scalar of CUT's power before comparison */
    uint8_t cfar_sogo_i[CFAR_MAX_GRP_NUM];             /* The i-th smallest over the averages from selected sides */
    uint8_t cfar_sogo_mask[CFAR_MAX_GRP_NUM];          /* The mask control to select the sides of the cross window; bit0: negative range direction; bit1: positive range direction; bit2: negative velocity direction; bit3: positive velocity direction */
    float cfar_ca_alpha[CFAR_MAX_GRP_NUM];             /* Scalar of CUT power before comparing */
    uint8_t cfar_ca_n[CFAR_MAX_GRP_NUM];               /* Number of maximum to remove before averaging */
    uint8_t doa_mode;                                  /* 2D_doa_mode 0/normal mode 1/combined mode 2/single shot mode */
    uint8_t doa_num_groups;                            /* bfm 2d on/off */
    uint32_t doa_fft_mux[MAX_BFM_GROUP_NUM];           /* normal mode/single shot mode doa rx antenna mux */
    uint32_t combined_doa_fft_mux[4];                  /* combined mode doa rx antenna mux */
    uint8_t doa_method;                                /* selection bit for doa method 0-digital bfm 2-DML */
    uint16_t doa_npoint[MAX_BFM_GROUP_NUM];            /* # of testing point of BFM/DML */
    char doa_samp_space;                               /* doa sampling space. t: uniformly sampling in theta, u: uniformly sampling in sin(theta) */
    uint8_t doa_max_obj_per_bin[MAX_BFM_GROUP_NUM];    /* Max # of objects per bin */
    float bfm_peak_scalar[3];                          /* Non/IPM mode peak scalar */
    float bfm_noise_level_scalar[3];                   /* multiobject noise level scalar for different SNR regime */
    uint8_t bfm_snr_thres[2];                          /* multiobject SNR regime threshold */
    float bfm_az_left;                                 /* Left limit of BFM azimuth angle */
    float bfm_az_right;                                /* Right limit of BFM azimuth angle */
    float bfm_ev_up;                                   /* Up limit of BFM elevated angle */
    float bfm_ev_down;                                 /* Down limit of BFM elevated angle */
    char doa_win[8];                                   /* BFM window type */
    float doa_win_params[3];                           /* BFM window parameter */
    uint8_t bfm_raw_search_step;                       /* raw search step for dbf work in OMP/IMP mode. Value 1 disables fine search */
    uint8_t bfm_fine_search_range;                     /* fine search range for dbf work in OMP/IMP mode */
    bool bfm_iter_search;                              /* enable bit for iterative search for dbf */
    uint8_t bfm_mode;                                  /* 1: OMP; 0: IPM */
    uint8_t bfm_group_idx;                             /* group index when DBF works in single shot mode 0-azimuth 1-elevated 2-aided */
    bool ant_info_from_flash;                          /* whether taking antenna position/compensation from flash */
    uint32_t ant_info_flash_addr;                      /* Starting address where antenna postion/comp on flash */
    antenna_pos_t ant_pos[MAX_ANT_ARRAY_SIZE];         /* unit in wavelength */
    float ant_comps[MAX_ANT_ARRAY_SIZE];               /* unit in deg */
    bool fsm_on;                                       /* function safety on/off */
    uint16_t agc_code[AGC_CODE_ENTRY_NUM];             /* store AGC_init_gain and clip1 and clip2 table bit0 LNA */
    bool agc_align_en;                                 /* AGC align mode on/off */
    bool adc_comp_en;                                  /* If enable adc compensation by left shifting when agc mode is on */
    bool de_vel_amb;                                   /* velocity ambiguity on/off (multi-frame) */
    uint8_t track_fps;                                 /* Frame per second */
    float track_fov_az_left;                           /* angle threshold to drop unstable objects in deg */
    float track_fov_az_right;                          /* angle threshold to drop unstable objects in deg */
    float track_fov_ev_down;                           /* angle threshold to drop unstable objects in deg */
    float track_fov_ev_up;                             /* angle threshold to drop unstable objects in deg */
    float track_near_field_thres;                      /* threshold to drop unstable near-field objects in meter */
    float track_capture_delay;                         /* time delay between first appear and stable tracking in second */
    float track_drop_delay;                            /* time delay between first missing and tracking drop in second */
    float track_vel_pos_ind_portion;                   /* portion of velocity indices are used for positive velocity */
    uint8_t track_obj_snr_sel;                         /* select object SNR type for track - 0: Ratio of DoA peak power to noise / 1: Ratio of RXs FFT average power to noise/ 2: absolute RXs FFT average power in dB */
    bool spk_en;                                       /* enable interference mitigation */
    uint8_t spk_buf_len;                               /* buffer length for detecting spikes */
    bool spk_set_zero;                                 /* set spikes to zero */
    uint8_t spk_ovr_num;                               /* overflow samples threshold to claim spike is detected */
    bool spk_thres_dbl;                                /* double threshold for detecting spikes */
    bool spk_min_max_sel;                              /* choose smaller estimated d_max */
    bool zero_doppler_cancel;                          /* remove static objects from 2DFFT results */
    int8_t anti_velamb_qmin;                           /* velocity de-ambiguity: minium ambiguity factor */
    bool high_vel_comp_en;                             /* compensate range and velocity when object speed is high */
    uint8_t high_vel_comp_method;                      /* method of compensation - 0: compensate automatically  1: compensate by user defined value */
    float vel_comp_usr;                                /* user defined compensation value for velocity */
    float rng_comp_usr;                                /* user defined compensation value for range */
    uint16_t dml_2dsch_start[2];                       /* dml: starting point of angle searching */
    uint8_t dml_2dsch_step[2];                         /* dml: pace of coarse searching */
    uint16_t dml_2dsch_end[2];                         /* dml: ending point of angle searching */
    bool dml_extra_1d_en[2];                           /* dml: enabler for the extra 1d-dml */
    bool dml_p1p2_en[2];                               /* dml: enabler for coef2 and coef3 in RNE */
    float dml_respwr_coef[10];                         /* dml: coefficients for RNE */
    bool acc_rng_hw;                                   /* knob for range interpolation after CFAR for accuracy improvement */
    bool acc_vel_hw;                                   /* knob for velocity interpolation after CFAR for accuracy improvement */
    bool acc_angle_hw;                                 /* knob for angle interpolation after DBF for accuracy improvement */
    uint8_t cas_obj_merg_typ;                          /* cascade master and slave object infos merge type - 0: and merge/ 1: loose and merge/ 2: slave based merge */
    bool sv_read_from_flash;                           /* read steering vector from flash or generate sv from ant_pos - FALSE: sv from ant_pos/ TRUE: sv from flash */
    uint8_t nvarray;
	BasebandCommonInitializationParams_t *com_params;
    uint32* sv_ptr;
} BasebandInitializationParams_t;

typedef struct {
        float carrier_freq;      /* in GHz   */
        float bandwidth;         /* in MHz   */
        float chirp_rampup;      /* in us    */
        float chirp_period;      /* in us    */
        float Fs;                /* in MHz   */
        uint32 rng_nfft;       /* unitless */
        uint32 vel_nfft;       /* unitless */
        uint32 doa_npoint[2];     /* unitless */
        float bfm_az_left;       /* in deg   */
        float bfm_az_right;      /* in deg   */
        float bfm_ev_up;         /* in deg   */
        float bfm_ev_down;       /* in deg   */
        float dml_sin_az_left;   /* in rad */
        float dml_sin_az_step;   /* in rad */
        float dml_sin_ev_down;   /* in rad */
        float dml_sin_ev_step;   /* in rad */
        float trk_fov_az_left;   /* in deg   */
        float trk_fov_az_right;  /* in deg   */
        float trk_fov_ev_up;   /* in deg   */
        float trk_fov_ev_down;  /* in deg   */
        float trk_nf_thres;      /* in m     */
        float trk_drop_delay;    /* in sec   */
        float trk_capt_delay;    /* in sec   */
        uint32 trk_fps;        /* frm/sec  */
        float noise_factor;      /* noise_factor */
        float rng_delta;       /* in m   */
        float vel_delta;       /* in m/s */
        float az_delta;        /* in rad */
        float ev_delta;        /* in rad */
        float az_delta_deg;    /* in deg */
        float ev_delta_deg;    /* in deg */
        float vel_wrap_max;
        float vel_comp;
        float rng_comp;
        float r_k_coeff;
        float r_p_coeff;
        float r_coeff;
        float v_coeff;

    	uint32 start_freq;
    	uint32 stop_freq;
    	uint32 step_up;
    	uint32 step_down;
    	uint32 cnt_wait;
        uint32_t total_cycle;
} BasebandSystemParams_t;
/*!
    struct  BasebandDriverInit_t
*/
typedef struct
{
	BasebandInitializationParams_t *cfg;
	BasebandSystemParams_t *sys_params;
	uint8 frame_type_id;

} BasebandDriverInit_t;
typedef enum
{
    BASEBAND_OP_MODE_BLOCK = 0U,
    BASEBAND_OP_MODE_NONBLOCK
} BasebandDriverOpMode_e;

typedef enum
{
    BB_SET_FRAME_TYPE_ID = 0U,
    BB_SWITCH_MEM_TABLE,
    BB_WRITE_MEM_TABLE,
    BB_READ_MEM_TABLE,
    BB_RESET_COMB,                /* baseband reset each frame when dml mode enabled */
    BB_SWTICH_BUF_STORE,
    BASEBAND_READ_REG,
    BASEBAND_WRITE_REG,
    BB_CONFIG_INSTALL,
    BB_GET_CONFIG,
    BB_GET_CUR_CONFIG,
    BB_GET_FRAME_TYPE_ID,
    BB_DDM_TX_VEL_POS,
    BB_DDM_TX_ON_NUM,
    BB_DDM_INIT_VEL_NFFT,
    BB_DDM_INIT_NVARRAY,
    BB_DDM_INIT_TX_GRP,
    BB_SET_NVARRAY_REG,
    BB_GET_CFAR_SIZE_OBJ,
    BB_GET_CFAR_NUM_OBJ,
    BB_GET_DOA_NUM_GRP,
    BB_GET_DC,
    BB_GET_OBJ_INFO,
    BB_GET_N2D_INFO,
    BB_DONE_WAIT,
	BB_SAM_DONE_WAIT,
    BB_INT_CLEAR,
	BB_INT_ENABLE,
	BB_INT_DISABLE,
} BasebandCommandId_e;

typedef enum
{
	INT_BB_DONE = 32U,
	INT_BB_RESERVE,
	INT_BB_SAM
} BasebandIrq_e;

typedef enum
{
    BB_SET_FRM_CFG = 0U,
    BB_SET_DC_CFG,
    BB_CAL_DC_VAL,
    BB_RST_DC_CFG,
    BB_SET_SHAD_BNK_CFG,
    BB_DOA_REINIT_HP,
} BasebandInitPhase_e;

typedef enum
{
    FRAME_TYPE_0 = 0U,
    FRAME_TYPE_1,
    FRAME_TYPE_2,
    FRAME_TYPE_3,
    SHADOW_BANK
} BasebandFrameType_e;

typedef enum {
    POL_MODE = 0U,
    INT_MODE
} bb_wait_mod_e;

typedef void (*bb_process_callback)(BasebandDriverInit_t *bb);

/*!
    struct  BasebandDriverContext_t
*/
typedef struct
{
    bool fi_set;
    uint8_t frame_type_id; // baseband frame type id for rtl
    uint8_t sys_irq_en;  // baseband sys interrupt enable
    uint16_t sys_en;     // baseband system enable
    BasebandDriverOpMode_e op_mode; //baseband operation, TODO: should move to middleware
} BasebandDriverContext_t;

/*!
    struct  BasebandDriverCommand_t
*/
typedef struct
{
    BasebandCommandId_e command_id;
    BasebandDriverInit_t* bb;
    uint8_t cfg_nums;
    uint32_t *pParam;
    uint32_t *pResult;
}BasebandDriverCommand_t;

/*!
    struct  BasebandDriverCmdRes_t
*/
typedef struct
{
    uint32_t result;
    uint32_t rlt_ptr;
} BasebandDriverCmdRes_t;

/*!
    struct  BasebandDoneWait_t
*/
typedef struct
{
	bb_wait_mod_e mode;
    uint32_t time_out;
} BasebandDoneWait_t;

/*!
    struct  BasebandFrameInterLeaving_t
*/
typedef struct frame_intrlv {        // frame interleaving
        uint8_t            strategy; // FIXMED or ROTATE or AIR_CONDITIONER or VELAMB or customized by yourself
        uint32_t           sw_num;   // loop period or switch number
        uint8_t            sel_0;    // 1st frame type selected
        uint8_t            sel_1;    // 2nd frame type selected
} BasebandFrameInterLeaving_t;

typedef sint32 (* baseband_init_t)(BasebandInitializationParams_t *cfg, uint8_t cfg_nums,RadioInitializationParams_t *rcfg);

typedef sint32 (* baseband_frameConfig_t)(uint8 chId, BasebandInitPhase_e BasebandInitPhase);

typedef sint32 (* baseband_run_t)(BasebandDriverContext_t *pBasebandContext);

typedef sint32 (* baseband_command_t)(BasebandDriverCommand_t *pBasebandCommand);

typedef sint32 (* baseband_stop_t)(void);

/*!
    baseband_fw_fun_set_t
*/
typedef struct
{
    /*!
        baseband_init
    */
    baseband_init_t    baseband_init_set;

    /*!
        baseband_frameConfig
    */
    baseband_frameConfig_t baseband_frameConfig_set;

    /*!
        baseband_run
    */
    baseband_run_t    baseband_run_set;

    /*!
        baseband_command
    */
    baseband_command_t    baseband_command_set;

    /*!
        baseband_stop
    */
    baseband_stop_t    baseband_stop_set;

} baseband_fw_fun_set_t;

//-------------------------------------------------------------------------------------------------------------------------
///
/// \function description: <baseband_init()>
///                      : Performs all baseband related initialization processes,
///                        such as baseband clock and interrupt config, register all configuration parameters.
///
/// \params              : <cfg>
///                      : all baseband related configuration parameters pointer that will be registered.
///
///                      : <cfg_nums>
///                      : the numbers of baseband configuration parameters.
///
/// \return              : 0 --> function execution successfully; != 0 --> error happen in function execution.
///
//-------------------------------------------------------------------------------------------------------------------------
sint32 baseband_init(BasebandInitializationParams_t *cfg, uint8_t cfg_nums,RadioInitializationParams_t *rcfg);

//-------------------------------------------------------------------------------------------------------------------------
///
/// \function description: <baseband_frameConfig()>
///                      : based on the configuration parameters registered by the initialization function,
///                        complete baseband related register configuration,which means that the behavior of one
///                        baseband frame process of is configured.
///
/// \params              : <FrameId>
///                      : the id of each frame.
///
///                      : <BasebandInitPhase>
///                      : baseband process initialization phase,
///                        option value: BB_SET_FRM_CFG       --> complete baseband all hardware engine initialization;
///                                      BB_SET_DC_CFG        --> complete baseband dc calibration feature configuration;
///                                      BB_CAL_DC_VAL        --> ..;
///                                      BB_RST_DC_CFG        --> ..;
///                                      BB_SET_SHAD_BNK_CFG  --> ..;
///                                      BB_DOA_REINIT_HP     --> ..;
///
/// \return              : 0 --> function execution successfully; != 0 --> error happen in function execution.
///
//-------------------------------------------------------------------------------------------------------------------------
sint32 baseband_frameConfig(uint8 FrameId, BasebandInitPhase_e BasebandInitPhase);

//-------------------------------------------------------------------------------------------------------------------------
///
/// \function description: <baseband_run()>
///                      : start baseband hardware according to the previously configured operation process.
///
/// \params              : <pBasebandContext>
///                      : contains the specific baseband hardware behavior of current running.
///
///
/// \return              : 0 --> function execution successfully; != 0 --> error happen in function execution.
///
//-------------------------------------------------------------------------------------------------------------------------
sint32 baseband_run(BasebandDriverContext_t *pBasebandContext);

//-------------------------------------------------------------------------------------------------------------------------
///
/// \function description: <baseband_command()>
///                      : perform specific steps in the form of commands to make the baseband function more flexible.
///
/// \params              : <pBasebandCommand>
///                      : the specific format of the command.
///                        members: command_id  --> the id of each command;
///                                 pParam      --> input parameter pointer carried by the command, type (uint32_t *);
///                                 pResult     --> return result pointer carried by the command, type (uint32_t *);
///                                 cfg_nums    --> the numbers of baseband configuration parameters, type (uint8_t);
///                                 bb          --> input parameter pointer or return result pointer used for command
///                                                 execution,type (BasebandDriverInit_t *).
///
/// \return              : 0 --> function execution successfully; != 0 --> error happen in function execution.
///
//-------------------------------------------------------------------------------------------------------------------------
sint32 baseband_command(BasebandDriverCommand_t *pBasebandCommand);

//-------------------------------------------------------------------------------------------------------------------------
///
/// \function description: <baseband_stop()>
///                      : stop baseband hardware.
///
/// \params              : void.
///
/// \return              : 0 --> function execution successfully; != 0 --> error happen in function execution.
///
//-------------------------------------------------------------------------------------------------------------------------
sint32 baseband_stop(void);

baseband_fw_fun_set_t *hal_get_baseband_handle();

extern BasebandInitializationParams_t Baseband_CfgData[MAX_FRAME_TYPE];

#endif /* SRC_MCAL_INC_RADAR_CDD_BASEBAND_H_ */
