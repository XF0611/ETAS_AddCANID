/** @file */

#ifndef CDD_FUSA_H
#define CDD_FUSA_H

#include "CDD_Fusa_Cfg.h"
#include "Platform_Types.h"
/***********************************************************************************************************
 *
***********************************************************************************************************/

#if (FUSA_DEBUG == FEATURE_ON)
//#include "print.h"
//#define log_fusa            xprintf
#else
#define log_fusa(...)
#endif

#define FUSA_ERR_CODE_LEN   12

/***********************************************************************************************************
 * FUSA COMMON DEFINE
***********************************************************************************************************/
/* FUSA Error Code Definitions */
#define E_FUSA_OK           (0)         /*!< ok */
#define E_FUSA_SYS          (-5)        /*!< system error */
#define E_FUSA_NOSPT        (-9)        /*!< unsupported features */
#define E_FUSA_RSFN         (-10)       /*!< reserved function code */
#define E_FUSA_RSATR        (-11)       /*!< reserved attribute */
#define E_FUSA_PAR          (-17)       /*!< parameter error */
#define E_FUSA_ID           (-18)       /*!< invalid ID number */
#define E_FUSA_CTX          (-25)       /*!< context error */
#define E_FUSA_MACV         (-26)       /*!< memory access violation */
#define E_FUSA_OACV         (-27)       /*!< object access violation */
#define E_FUSA_ILUSE        (-28)       /*!< illegal service call use */
#define E_FUSA_NOMEM        (-33)       /*!< insufficient memory */
#define E_FUSA_NOID         (-34)       /*!< no ID number available */
#define E_FUSA_NORES        (-35)       /*!< no resource available */
#define E_FUSA_OBJ          (-41)       /*!< object state error */
#define E_FUSA_NOEXS        (-42)       /*!< non-existent object */
#define E_FUSA_QOVR         (-43)       /*!< queue overflow */
#define E_FUSA_RLWAI        (-49)       /*!< forced release from waiting */
#define E_FUSA_TMOUT        (-50)       /*!< polling failure or timeout */
#define E_FUSA_DLT          (-51)       /*!< waiting object deleted */
#define E_FUSA_CLS          (-52)       /*!< waiting object state changed */
#define E_FUSA_WBLK         (-57)       /*!< non-blocking accepted */
#define E_FUSA_BOVR         (-58)       /*!< buffer overflow */
#define E_FUSA_OPNED        (-6)        /*!< device is opened */
#define E_FUSA_CLSED        (-7)        /*!< device is closed */
#define E_FUSA_NOOPS        (-200)      /*!< no device operation api. */
#define E_FUSA_DBUSY        (-201)      /*!< device is busy. */
#define E_FUSA_NODATA       (-202)      /*!< no data in device buffer/fifo. */
#define E_FUSA_NXFER        (-203)      /*!< device xfer configure not matched. */
#define E_FUSA_NSUP         (-204)      /*!< not support. */
#define E_FUSA_PROTOCOL     (-205)      /*!< protocol error. */
#define E_FUSA_NBO          (-206)      /*!< buf off error. */
/* End of Error Code Definitions */

/* FUSA Items enum */
typedef enum{
    FUSA_ITEM_SM1,
    FUSA_ITEM_SM2,
    FUSA_ITEM_SM3,
    FUSA_ITEM_SM4,
    FUSA_ITEM_SM5,
    FUSA_ITEM_SM6,
    FUSA_ITEM_SM8,
    FUSA_ITEM_SM11,
    FUSA_ITEM_SM12,
    FUSA_ITEM_SM13,
    FUSA_ITEM_SM14,
    FUSA_ITEM_SM101,
    FUSA_ITEM_SM102,
    FUSA_ITEM_SM103,
    FUSA_ITEM_SM104,
    FUSA_ITEM_SM105,
    FUSA_ITEM_SM106,
    FUSA_ITEM_SM107,
    FUSA_ITEM_SM108,
    FUSA_ITEM_SM109,
    FUSA_ITEM_SM120,
    FUSA_ITEM_SM121,
    FUSA_ITEM_SM126,
    FUSA_ITEM_SM130,
    FUSA_ITEM_SM133,
    FUSA_ITEM_SM201,
    FUSA_ITEM_SM901,
    FUSA_ITEM_SM902,
    FUSA_ITEM_SM904,
    FUSA_ITEM_SM905,
    FUSA_ITEM_SM906,
    FUSA_ITEM_SM907,
    FUSA_ITEM_SM908,
    FUSA_ITEM_SM910,
    FUSA_ITEM_SM911,
}fusa_item_e;

/*fusa actions type */
typedef enum{
    FUSA_TYPE_IRQ,
    FUSA_TYPE_SS1,
    FUSA_TYPE_SS2,
}fusa_type_e;

/*fusa command enum */
typedef enum{
    FUSA_CMD_INIT_BB_MEM, /* bb buffer init,for sm104*/
    FUSA_CMD_SCAN_ONE_TIME, /* bb buffer init by scan one time,for sm104*/
}FusaCommandId_e;

/* fusa command structure */
typedef struct {
    FusaCommandId_e cmd;
    uint32 temp;
    uint32 *pParam;
    uint32 *pResult;
}FusaCommand_t;

/* fusa items config structure */
typedef struct {
    fusa_item_e item;
    fusa_type_e type;
    sint8 open_flag;
}fusa_str_t;

/* fusa common threshold structure */
typedef struct
{
    float32 thr_h;
    float32 thr_l;
} fusa_threshold_t;

//for fusa_run() use  below definition
#define FUSA_RUN_SET            (0x00000001 << 30)
#define FUSA_RUN_RESUME         (0x00000001 << 29)
#define FUSA_RUN_RESUME_FORCE   (0x00000001 << 28)

/***********************************************************************************************************
 * FUSA Items Special cfg
***********************************************************************************************************/
/******************************
 * SM1
*******************************/

/*SM1 LDO enum*/
typedef enum{
    SM1_LDO25_FMCWPLL       =1,
    SM1_LDO25_RXBBS         =3,
    SM1_LDO25_RXBBN,
    SM1_LDO25_REFPLL_XTAL,
    SM1_LDO25_REFPLL_PLL,
    SM1_LDO25_PMU,
    SM1_LDO25_LVDS,
    SM1_LDO25_ADC23,
    SM1_LDO25_ADC01,
    SM1_LDO13_ADC23,
    SM1_LDO13_ADC01,
    SM1_LDO12_FMCWPLL_MMD,
    SM1_LDO11_TX3           = 16,
    SM1_LDO11_TX2,
    SM1_LDO11_TX1,
    SM1_LDO11_TX0,
    SM1_LDO11_SPI,
    SM1_LDO11_RXRFS         = 22,
    SM1_LDO11_RXRFN,
    SM1_LDO11_REFPLL_VCO,
    SM1_LDO11_REFPLL_MMD2,
    SM1_LDO11_REFPLL_MMD,
    SM1_LDO11_LOMAIN        = 28,
    SM1_LDO11_FMCWPLL_VCO,
    SM1_LDO11_ADC23         = 31,
    SM1_LDO11_ADC01,
    SM1_LDO10_TXLO,
    SM1_LDO10_PA3,
    SM1_LDO10_PA2,
    SM1_LDO10_PA1,
    SM1_LDO10_PA0,
    SM1_LDO10_RXLO,
}fusa_sm1_item_e;

#define SM1_LDO_M_EN(x)     ((uint64)(0x0000000000000001) << x) /* 'x' is ldo which enum by fusa_sm1_item_e */
#define FUSA_SM1_THR_NUM      5 /* sm1 total threshold groups number */
#define FUSA_SM1_PART_NUM     4 /* sm1 total part number */

//for fusa_cfg_xx() use below structure
/*
    High_th
    Low_th
    --> 2.5v 1.3v 1.2v 1.1v 1v

    example
    xx_h = 7.5
    xx_l = -7.5

    the threshold is    (7.5%) ~ (-7.5%)
    typical ±10%
*/
typedef struct {
    fusa_threshold_t thr[FUSA_SM1_THR_NUM];
    uint64 part[FUSA_SM1_PART_NUM];
}fusa_config_sm1_t;


/******************************
 * SM2
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  +15.0 ~ -15.0  means (+15% ~ -15%)
//thr_l:  +15.0 ~ -15.0  means (+15% ~ -15%)

/******************************
 * SM3
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  0 (err_inject), 1 (+8%), 2 (+12%), 3 (+16%)
//thr_l:  0 (err_inject), 1 (-8%), 2 (-12%), 3 (-16%)

//for fusa_err_inject()
// 0: low err inject
// 1: high err inject

/******************************
 * SM4
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  0 (err_inject), 1 (+8%), 2 (+12%), 3 (+16%)
//thr_l:  0 (err_inject), 1 (-8%), 2 (-12%), 3 (-16%)

//for fusa_err_inject()
// 0: low err inject
// 1: high err inject

/******************************
 * SM5
*******************************/
//for fusa_err_inject()
// must be 0, unlock err inject

/******************************
 * SM6
*******************************/
//for fusa_cfg_set(), fusa_cfg_get() is not used for sm6
typedef struct
{
    uint32 channel; //must be 0/1/2/3,used as channel num, means which channel will be used for calculate threshold reg value
    float32 threshold; //5~12 dbm ,if tx power is lower than thr_l,will trigger power detector exception event
}fusa_config_sm6_t;

//for fusa_run()
//param: tx channels, bit3~bit0 -- tx3~tx0
//      example: 0x3 means will be execute power detect on tx1 and tx0

//for fusa_err_inject()
// must be 0, tx power lower err inject

/******************************
 * SM11
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  (dB) default 20db
//thr_l:  (dB) default 16db
//for a specified config, all channels loopback result must be (result >= thr_l) && (result <= thr_h)

//for fusa_err_inject()
// must be 0: err inject

/******************************
 * SM12
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  not used
//thr_l:  (dB) default 15db
//for a specified config, all channels loopback result must be (result >= thr_l)

//for fusa_err_inject()
// must be 0: err inject

/******************************
 * SM13
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h: 1~31  filter, means how many fault occured in a chirp will report an err
//thr_l: 1~7   threshold

//for fusa_err_inject()
// must be 0: err inject

/******************************
 * SM14
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  not used
//thr_l:  (temperature) default +125 *C

//for fusa_err_inject()
// must be 0: temperature over err inject

/******************************
 * SM201
*******************************/
//for fusa_cfg_xx() use fusa_threshold_t
//thr_h:  not used
//thr_l:  (dB), 3 means +/- 3dB, default +/-  5db
//for all channels result must be (result < thr_l)

/***********************************************************************************************************
 * FUSA FUNCTION DEFINE
***********************************************************************************************************/
/*
    hw err status cleared by default code if return 0,else,
    it's nothing operation,hw err status must clear by customer.
*/
typedef sint32 (*fusa_err_handle_t)(fusa_item_e item, uint32 *param);


typedef sint32 (*fusa_init_t)(void);
typedef sint32 (*fusa_init_item_t)(fusa_item_e item, fusa_type_e fusa_type, uint32 *param);
typedef sint32 (*fusa_cfg_set_t)(fusa_item_e item,uint32 *param);
typedef sint32 (*fusa_cfg_get_t)(fusa_item_e item,uint32 *param);
typedef sint32 (*fusa_run_t)(fusa_item_e item, uint32 param);
typedef sint32 (*fusa_enable_t)(fusa_item_e item, boolean param);
typedef sint32 (*fusa_err_inject_t)(fusa_item_e item, uint32 param);
typedef sint32 (*fusa_err_handle_register_t)(fusa_err_handle_t handle);
/* check EMU err code when power on*/
typedef sint32 (*fusa_check_err_code_t)(uint32 *buf, uint32 buf_len);
typedef sint32 (* fusa_command_t)(FusaCommand_t *p_fusa_cmd);
typedef void (*fusa_irq_handle_t)(void);


typedef struct
{
    fusa_init_t init;
    fusa_init_item_t init_item;
    fusa_cfg_set_t cfg_set;
    fusa_cfg_get_t cfg_get;
    fusa_run_t   run;
    fusa_enable_t enable;
    fusa_err_inject_t err_inject;
    fusa_check_err_code_t check_err_code;
    fusa_err_handle_register_t err_handle_register;
    fusa_command_t fusa_command;
    fusa_irq_handle_t irq_handle;
}fusa_fw_fun_t;

/***********************************************************************************************************
 * FUSA application interface
***********************************************************************************************************/
//-------------------------------------------------------------------------------------------------------------------------
/// \function description   : <fusa_init>
///                         : initialize fusa environment
///
/// \params                 : void
///
/// \return                 : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_init(void);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_init_item>
///                          : initialize fusa item to specified fusa type
///
/// \params                  : <item>
///                          : fusa item
///
///                          : <fusa_type>
///                          : FUSA_TYPE_IRQ or FUSA_TYPE_SS1 or FUSA_TYPE_SS2
///
///                          : <*param>
///                          : reserve, must be NULL
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_init_item(fusa_item_e item, fusa_type_e fusa_type, uint32 *param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_cfg_set>
///                          : config paramaters for specified fusa item,it's different for different item
///
/// \param                   : <item>
///                          : fusa item
///
///                          : <*param>
///                          : pointer to param buffer,it's different for different item
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_cfg_set(fusa_item_e item,uint32 *param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_cfg_get>
///                          : get config paramaters for specified fusa item,it's different for different item
///
/// \param                   : <item>
///                          : fusa item
///
///                          : <*param>
///                          : pointer to param buffer,it's different for different item
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_cfg_get(fusa_item_e item,uint32 *param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_run>
///                          : run specified operation for specified fusa item
///
/// \param                   : <item>
///                          : fusa item
///
///                          : <param>
///                          : specified operation,it's different for different items
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_run(fusa_item_e item, uint32 param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_enable>
///                          : enable/disable a specified fusa item
///
/// \param                   : <item>
///                          : fusa item
///
///                          : <param>
///                          : TRUE or FALSE
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_enable(fusa_item_e item, boolean param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_err_inject>
///                          : inject err for a specified fusa item
///
/// \param                   : <item>
///                          : fusa item
///
///                          : <param>
///                          : specified a fault type, it's different for different fusa item
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_err_inject(fusa_item_e item, uint32 param);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_err_handle_register>
///                          : register an customer handler for fusa err event report
///
/// \param                   : <handle>
///                          : customer handler
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_err_handle_register(fusa_err_handle_t handle);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_check_err_code>
///                          : check err code registers in EMU module
///
/// \param                   : <*buf>
///                          : point to a buffer to store err code value
///
///                          : <buf_len>
///                          : the length of buffer must be equal or greater than FUSA_ERR_CODE_LEN
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_check_err_code(uint32 *buf, uint32 buf_len);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : fusa_command
///                          : execurte some opereation not covered by above interface
///
/// \param                   : <*p_fusa_cmd>
///                          : point to cmd structure
///
/// \return                  : E_FUSA_OK if success
//-------------------------------------------------------------------------------------------------------------------------
sint32 fusa_command(FusaCommand_t *p_fusa_cmd);

//-------------------------------------------------------------------------------------------------------------------------
/// \function description    : <fusa_irq_handle>
///                          : fusa irq callback handler,need calling it in irq FUSA_ANA_IRQ(19) FUSA_DIG_IRQ(21) FUSA_BB_IRQ_ECC_SB(23)
///
/// \param                   : void
///
/// \return                  : void
//-------------------------------------------------------------------------------------------------------------------------
void fusa_irq_handle(void);

#endif /* CDD_FUSA_H */
