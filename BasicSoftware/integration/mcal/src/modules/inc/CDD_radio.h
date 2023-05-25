#ifndef _HAL_RADIO_H_
#define _HAL_RADIO_H_

#include "Platform_Types.h"
#include "CDD_Radar_Common_Cfg.h"
#include "radio_ext.h"

#define DIV_RATIO(val, F_SD) ((val) * 1e3 / 8 / (F_SD) - 16) * (1L<<28)
#define INV_DIV_RATIO(val, F_SD) (1.0 * (val) / (1L<<28) + 16) * 8 * (F_SD) / 1000
#define FREQ_SYNTH_SD_RATE FMCW_SDM_FREQ /* in MHz */

/*!
    Todo
*/
typedef enum{
    RADIO_EVT_FRAME_ACQ_DONE,    /*! Todo */
} RadioEventsComplete_t;

/*!
    Todo
*/
typedef enum
{
	RADIO_LVDS_MODE_NORM_TO_TEST = 0U,
	RADIO_LVDS_MODE_TEST_TO_NORM,
	RADIO_LVDS_MODE_ON,
	RADIO_DDM_CFG,
	RADIO_READ_REG,
	RADIO_WRITE_REG,
	RADIO_GET_TEM,
	RADIO_TX_CH_ON,
	RADIO_SET_TX_PHASE,
	RADIO_GET_TX_PHASE,
	RADIO_PHASE_VAL,
	RADIO_REG_VAL,
	RADIO_IF_OUT_ON,
	RADIO_SINGLE_TONE,
	RADIO_SET_TIA_GAIN,
	RADIO_GET_TIA_GAIN,
	RADIO_SET_VGA1_GAIN,
	RADIO_GET_VGA1_GAIN,
	RADIO_SET_VGA2_GAIN,
	RADIO_GET_VGA2_GAIN,
	RADIO_AGC_ENABLE

} RadioCommandId_e;

typedef enum
{
    RADIO_SET_FRM_CFG = 0U,
    RADIO_SET_DC_CFG,
    RADIO_WR_DC_VAL,
    RADIO_RST_DC_CFG,
} RadioInitPhase_e;

/*!
    struct  RadioInitializationParams_t
*/
typedef RadarCommonInitializationParams_t RadioInitializationParams_t;

/*!
    struct  RadioFrameParams_t
*/
typedef struct
{
    uint32 start_freq;
    uint32 stop_freq;
    uint32 mid_freq;
    uint32 step_up;
    uint32 step_down;
    uint32 cnt_wait;
    uint32 total_cycle;
    uint32 sd_freq;
    sint32 nchirp;
    uint8 frame_type_id;
    uint32 anti_velamb_cycle;
    uint32 chirp_shifting_cyle;
    /* following is for debug purpose */
    uint32 up_cycle;
    uint32 down_cycle;
    uint32 wait_cycle;

    uint32 hp_start_freq;
    uint32 hp_stop_freq;
    uint32 hp_mid_freq;
    uint32 hp_step_down;
} RadioFrameParams_t;

/*!
    struct  RadioStatusReport_t
*/
typedef struct
{
	boolean is_running;
	boolean is_txon;
	boolean is_rxon;
	boolean is_adcon;

    uint32 lock_freq;
    uint32 adc_freq; //20m or 25m

    uint8 vam_status[4]; //Virtual array mode = MIMO
    uint8 txphase_status[8];
} RadioStatusReport_t;

/*!
    struct  RadioDriverCommand_t
*/
typedef struct
{

    RadioCommandId_e cmd_id;
    //uint32 cmd_param0;
    //uint8 cmd_param1;
    //bool cmd_parm2;
    uint32_t *pParam;
    uint32_t *pResult;

} RadioDriverCommand_t;

/*!
    struct  RadioDriverContext_t
*/
typedef struct
{
    bool tx_en;
    bool rx_en;
    bool fmcw_en;
} RadioDriverContext_t;

typedef sint32 (* radio_init_t)(void);

typedef sint32 (* radio_frameConfig_t)(uint8 chId,RadioInitPhase_e radioInitPhase, RadioInitializationParams_t *pParams);

typedef sint32 (* radio_powerSave_t)(uint8 chId, bool tx_en, bool rx_en, bool enable);

typedef sint32 (* radio_auxadcEable_t)(int32_t mode,  int32_t muxin_sel);

typedef sint32 (* radio_auxadcRead_t)(int32_t mode,float32 *val);

typedef sint32 (* radio_frameStart_t)(uint8 chId, RadioDriverContext_t *pRadioContext);

typedef sint32 (* radio_frameStop_t)(uint8 chId);

typedef sint32 (* radio_statusReport_t)(uint8 chId,RadioStatusReport_t *p_devStatus);

typedef sint32 (* radio_eventSet_t)(uint8 chId,RadioEventsComplete_t evt,void *pParams);

typedef sint32 (* radio_command_t)(RadioDriverCommand_t *pRadioCommand);

/*!
    radio_fw_fun_set_t
*/
typedef struct
{
    /*!
        radio_init
    */
    radio_init_t    radio_init_set;

    /*!
        radio_frameConfig
    */
    radio_frameConfig_t    radio_frameConfig_set;

    /*!
        radio_powerSave
    */
    radio_powerSave_t    radio_powerSave_set;

    /*!
        radio_auxadc enable
    */
    radio_auxadcEable_t    radio_auxadc_enable;
    /*!
        radio_auxadc read
    */
    radio_auxadcRead_t    radio_auxadc_read;
    /*!
        radio_frameStart
    */
    radio_frameStart_t    radio_frameStart_set;

    /*!
        radio_frameStop
    */
    radio_frameStop_t    radio_frameStop_set;

    /*!
        radio_statusReport
    */
    radio_statusReport_t    radio_statusReport_set;

    /*!
        radio_eventSet
    */
    radio_eventSet_t    radio_eventSet_set;

    /*!
        radio_command
    */
    radio_command_t    radio_command_set;

} radio_fw_fun_set_t;

//----------------------------------------------------------------------------
///
///  radio initialization, mainly for the pll clock initialization
///
///  \param      void
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_init(void);

//----------------------------------------------------------------------------
///
///  radio configuration before start fmcw start, assign initialization parameters to registers
///
///  \param      frameId                       radio frame type number
///  \param      radioInitPhase                enumeration about configuration phase when radio initialization
///  \param      pParams                       pointer to radio initialization parameters
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_frameConfig(uint8 frameId, RadioInitPhase_e radioInitPhase, RadioInitializationParams_t *pParams);

//----------------------------------------------------------------------------
///
///  Whether to enable the power saving mode
///
///  \param      frameId                       radio frame type number
///  \param      enable                        enable or disable, 1 is enable, 0 is disable
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_powerSave(uint8 frameId, bool tx_en, bool rx_en, bool enable);

//----------------------------------------------------------------------------
///
///  Read auxadc value, get voltage or temperature
///
///  \param      mode                              Select the value you want to read:External voltage / Internal temperature /Internal  voltage
///  \param      muxin_sel                         Configure the corresponding pin:TestMUXN / TestMUXP / TPANA1
///                                                ext voltage: mode=1 muxin_sel=3
///                                                internal temperature:mode=2 muxin_sel=1
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_auxAdc_enable(int32_t mode,  int32_t muxin_sel);

//----------------------------------------------------------------------------
///
///  Read auxadc value, get voltage or temperature
///
///  \param      mode                              Select the value you want to read:External voltage / Internal temperature /Internal  voltage
///  \param      *val                              adc val,point to a float buffer
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_auxAdc_read(int32_t mode,  float32 *val);

//----------------------------------------------------------------------------
///
///  radio start work
///
///  \param      frameId                       radio frame type number
///  \param      pRadioContext                 pointer to struct about radio start parameters, such as tx_en /rx_en /fmcw_en.
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_frameStart(uint8 frameId, RadioDriverContext_t *pRadioContext);

//----------------------------------------------------------------------------
///
///  radio stop work
///
///  \param      frameId                       radio frame type number
///
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_frameStop(uint8 frameId);

//----------------------------------------------------------------------------
///
///  report radio status
///
///  \param      frameId                       radio frame type number
///  \param      p_devStatus                   pointer to struct about radio status.
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_statusReport(uint8 frameId,RadioStatusReport_t *p_devStatus);

//----------------------------------------------------------------------------
///
///  set radio events
///
///  \param      frameId                       radio frame type number
///  \param      evt                           radio event
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_eventSet(uint8 frameId,RadioEventsComplete_t evt,void *pParams);

//----------------------------------------------------------------------------
///
///  A way to call radio driver
///
///  \param      pBasebandCommand               pointer to radio command parameters
///
///
///
///  \return     return 0 on success, else error code.
///
//----------------------------------------------------------------------------
sint32 radio_command(RadioDriverCommand_t *pRadioCommand);

/*!
    function hal_get_radio_handle
*/
radio_fw_fun_set_t *hal_get_radio_handle();

extern RadioInitializationParams_t Radio_CfgData[MAX_FRAME_TYPE];

#endif /* _HAL_RADIO_H_ */
