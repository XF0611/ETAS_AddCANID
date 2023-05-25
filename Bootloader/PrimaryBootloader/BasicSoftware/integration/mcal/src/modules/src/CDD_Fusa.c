#include "CDD_Fusa.h"
#include "fusa_alps.h"

#define NULL ((void *) 0U)

fusa_fw_fun_t g_fusafw_f = {NULL};

/**
 * @name: fusa_init
 * @description:initialize fusa environment
 * @param {void}
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_init()
{
    attach_fusafw_fun_alps(&g_fusafw_f);
    return E_FUSA_OK;
}

/**
 * @name: fusa_init_item
 * @description: initialize fusa item to specified fusa type
 * @param {fusa_item_e} item:fusa item
 * @param {fusa_type_e} fusa_type:irq ss1 or ss2
 * @param {uint32} *param: reserve
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_init_item(fusa_item_e item, fusa_type_e fusa_type, uint32 *param)
{
    return (g_fusafw_f.init_item != NULL) ? g_fusafw_f.init_item(item,fusa_type,param) : E_FUSA_NOOPS;
}
/**
 * @name: fusa_cfg_set
 * @description: config paramaters for specified fusa item,it's different for different item
 * @param {fusa_item_e} item: fusa item
 * @param {uint32} *param : pointer to param buffer,it's different for different item
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_cfg_set(fusa_item_e item,uint32 *param)
{
    return (g_fusafw_f.cfg_set != NULL) ? g_fusafw_f.cfg_set(item,param) : E_FUSA_NOOPS;
}
/**
 * @name: fusa_cfg_get
 * @description: get config paramaters for specified fusa item,it's different for different item
 * @param {fusa_item_e} item: fusa item
 * @param {uint32} *param : pointer to param buffer,it's different for different item
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_cfg_get(fusa_item_e item,uint32 *param)
{
    return (g_fusafw_f.cfg_get != NULL) ? g_fusafw_f.cfg_get(item,param) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_run
 * @description: run specified operation for specified fusa item
 * @param {fusa_item_e} item: fusa item
 * @param {uint32} param:specified operation,it's different for different items
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_run(fusa_item_e item, uint32 param)
{
    return (g_fusafw_f.run != NULL) ? g_fusafw_f.run(item,param) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_enable
 * @description: enable/disable a specified fusa item
 * @param {fusa_item_e} item
 * @param {boolean} param TRUE or FALSE
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_enable(fusa_item_e item, boolean param)
{
    return (g_fusafw_f.enable != NULL) ? g_fusafw_f.enable(item,param) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_err_inject
 * @description: inject err for a specified fusa item
 * @param {fusa_item_e} item
 * @param {uint32} param:specified a fault type, it's different for different fusa item
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_err_inject(fusa_item_e item, uint32 param)
{
    return (g_fusafw_f.err_inject != NULL) ? g_fusafw_f.err_inject(item,param) : E_FUSA_NOOPS;
}
/**
 * @name: fusa_err_handle_register
 * @description: register an customer handler for fusa err event report
 * @param {fusa_err_handle_t} handle:customer handler
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_err_handle_register(fusa_err_handle_t handle)
{
    return (g_fusafw_f.err_handle_register != NULL) ? g_fusafw_f.err_handle_register(handle) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_check_err_code
 * @description: check err code registers in EMU module
 * @param {uint32} *buf: point to a buffer to store err code value
 * @param {uint32} buf_len: the length of buffer must be equal or greater than FUSA_ERR_CODE_LEN
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_check_err_code(uint32 *buf, uint32 buf_len)
{
    return (g_fusafw_f.check_err_code != NULL) ? g_fusafw_f.check_err_code(buf,buf_len) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_command
 * @description: execurte some opereation not covered by above interface
 * @param {FusaCommand_t} *p_fusa_cmd : point to cmd structure
 * @return {sint32} : E_FUSA_OK if success
 */
sint32 fusa_command(FusaCommand_t *p_fusa_cmd)
{
    return (g_fusafw_f.fusa_command != NULL) ? g_fusafw_f.fusa_command(p_fusa_cmd) : E_FUSA_NOOPS;
}

/**
 * @name: fusa_irq_handle
 * @description: fusa irq callback handler,need calling it in irq FUSA_ANA_IRQ(19) FUSA_DIG_IRQ(21) FUSA_BB_IRQ_ECC_SB(23)
 * @param {*} :no param
 * @return {*}:no return
 */
void fusa_irq_handle(void)
{
    if(g_fusafw_f.irq_handle !=NULL)
    {
        g_fusafw_f.irq_handle();
    }
}

