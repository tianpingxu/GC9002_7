/**
  ******************************************************************************
  * @file    gc9002_hal_display.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/11
  * @brief   This file provides all the DISPLAY firmware functions.
  ******************************************************************************
  */
#include "gc90xx_hal.h"
#ifdef HAL_DISPLAY_MODULE_ENABLED

void HAL_DISPLAY_StructInit(DISPLAY_InitTypeDef *DISPLAY_InitStruct)
{
    assert_param(DISPLAY_InitStruct != NULL);
    /* 888 LCD */
    DISPLAY_InitStruct->timing_param.hpixels  = 800;
    DISPLAY_InitStruct->timing_param.hsyncpw  = 20;
    DISPLAY_InitStruct->timing_param.hfnprch  = 210;
    DISPLAY_InitStruct->timing_param.hbkprch  = 46;
    DISPLAY_InitStruct->timing_param.vlines   = 480;
    DISPLAY_InitStruct->timing_param.vsyncpw  = 10;
    DISPLAY_InitStruct->timing_param.vfnprch  = 22;
    DISPLAY_InitStruct->timing_param.vbkprch  = 23;

    DISPLAY_InitStruct->update_area.start_row = 0;
    DISPLAY_InitStruct->update_area.end_row   = 800-1;
    DISPLAY_InitStruct->update_area.start_col = 0;
    DISPLAY_InitStruct->update_area.end_col   = 480-1;
    
    DISPLAY_InitStruct->invert_oclk_en    = DISPLAY_DISABLE;
    DISPLAY_InitStruct->invert_hs         = DISPLAY_DISABLE;
    DISPLAY_InitStruct->invert_vs         = DISPLAY_DISABLE;
    DISPLAY_InitStruct->update_ram        = DISPLAY_ENABLE;
    DISPLAY_InitStruct->fg_enable         = DISPLAY_ENABLE;
    DISPLAY_InitStruct->dithering         = DISPLAY_DISABLE;
    DISPLAY_InitStruct->input_color_mode  = DISPLAY_InputColorMode_RGB565; //rgb565
    DISPLAY_InitStruct->output_color_mode = DISPLAY_OutputColorMode_RGB565; //rgb565 
    DISPLAY_InitStruct->interface_type    = DISPLAY_InterfaceType_24BIT;// 8080-24bit
    DISPLAY_InitStruct->running_type      = DISPLAY_Mode_MasterActive;
    DISPLAY_InitStruct->tearing_en        = DISPLAY_DISABLE;
    DISPLAY_InitStruct->update_en         = DISPLAY_DISABLE;
    DISPLAY_InitStruct->clk_spi_div       = DISPLAY_DIV_32; //lowest clock
    DISPLAY_InitStruct->icg_en            = DISPLAY_DISABLE;
    DISPLAY_InitStruct->lcd_mv            = DISPLAY_DISABLE;

    DISPLAY_InitStruct->master_param.base_addr      = 0;
    DISPLAY_InitStruct->master_param.total_byte_cnt = 0;
    DISPLAY_InitStruct->master_param.burst_length   = 16;

    DISPLAY_InitStruct->rc_inst.inst_type_sel   = 0x00;
    DISPLAY_InitStruct->rc_inst.wr_ram_inst     = 0x2C;
    DISPLAY_InitStruct->rc_inst.set_col_inst    = 0x2A;
    DISPLAY_InitStruct->rc_inst.set_row_inst    = 0x2B;

    DISPLAY_InitStruct->wqspi_inst.wr_4p_inst       = 0x12;
    DISPLAY_InitStruct->wqspi_inst.wr_4color_inst   = 0x12;
    DISPLAY_InitStruct->wqspi_inst.wr_color_inst    = 0x32;
    DISPLAY_InitStruct->wqspi_inst.wr_cmd_inst      = 0x02;

    DISPLAY_InitStruct->streaming_inst.qspi_rd_type     = DISPLAY_QSPI_1_1_1;
    DISPLAY_InitStruct->streaming_inst.qspi_color_type  = DISPLAY_QSPI_1_4_4;
    DISPLAY_InitStruct->streaming_inst.qspi_cmd_type    = DISPLAY_QSPI_1_1_1;
    DISPLAY_InitStruct->streaming_inst.vs_inst          = 0x61;
    DISPLAY_InitStruct->streaming_inst.hs_inst          = 0x60;
    
    DISPLAY_InitStruct->tearing_threshold = 0;

    DISPLAY_InitStruct->rqspi_inst = 0x3;
    DISPLAY_InitStruct->disp_duty_l = 0x18;
    DISPLAY_InitStruct->disp_duty_h = 0x18;
    DISPLAY_InitStruct->disp_qspi_duty_l = 0x3;
    DISPLAY_InitStruct->disp_qspi_duty_h = 0x3;
}
HAL_StatusTypeDef HAL_DISPLAY_Init(DISPLAY_HandleTypeDef *hdisplay)
{
    /* Check the DISPLAY handle allocation */
    if(hdisplay == NULL)
    {
        return HAL_ERROR;
    }
    if(hdisplay->State == HAL_DISPLAY_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        __HAL_LOCK(hdisplay);
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        if (hdisplay->MspInitCallback == NULL)
        {
            hdisplay->MspInitCallback = HAL_DISPLAY_MspInit; /* Legacy weak MspInit  */
        }
        /* Init the low level hardware : GPIO, CLOCK, CLIC... */
        hdisplay->MspInitCallback(hdisplay);
#else
        /* Init the low level hardware : GPIO, CLOCK, CLIC... */
        HAL_DISPLAY_MspInit(hdisplay);
#endif /* USE_HAL_DISPLAY_REGISTER_CALLBACKS */
    }
    hdisplay->State = HAL_DISPLAY_STATE_BUSY;
    
    uint32_t tempreg = 0;
    ((union _disp_cntl *)&tempreg)->bit.enable               = 1;
    ((union _disp_cntl *)&tempreg)->bit.invert_oclk          = hdisplay->Init.invert_oclk_en;
    ((union _disp_cntl *)&tempreg)->bit.invert_hs            = hdisplay->Init.invert_hs;
    ((union _disp_cntl *)&tempreg)->bit.invert_vs            = hdisplay->Init.invert_vs;
    ((union _disp_cntl *)&tempreg)->bit.update_ram           = hdisplay->Init.update_ram;
    ((union _disp_cntl *)&tempreg)->bit.fg_enable            = hdisplay->Init.fg_enable;
    ((union _disp_cntl *)&tempreg)->bit.dithering            = hdisplay->Init.dithering;
    ((union _disp_cntl *)&tempreg)->bit.input_color_mode     = hdisplay->Init.input_color_mode;
    ((union _disp_cntl *)&tempreg)->bit.output_color_mode    = hdisplay->Init.output_color_mode;
    ((union _disp_cntl *)&tempreg)->bit.interface_type       = hdisplay->Init.interface_type;
    ((union _disp_cntl *)&tempreg)->bit.running_type         = hdisplay->Init.running_type;
    ((union _disp_cntl *)&tempreg)->bit.tearing_en           = hdisplay->Init.tearing_en;
    ((union _disp_cntl *)&tempreg)->bit.update_en            = hdisplay->Init.update_en;
    ((union _disp_cntl *)&tempreg)->bit.clk_spi_div          = hdisplay->Init.clk_spi_div;
    ((union _disp_cntl *)&tempreg)->bit.icg_en               = hdisplay->Init.icg_en;
    ((union _disp_cntl *)&tempreg)->bit.lcd_mv               = hdisplay->Init.lcd_mv;
    hdisplay->Instance->display_cntl.reg_u32val = tempreg;
    
    hdisplay->Instance->timing_param0 = (hdisplay->Init.timing_param.hpixels << 16) + 
                                         hdisplay->Init.timing_param.hsyncpw;

    hdisplay->Instance->timing_param1 = (hdisplay->Init.timing_param.hfnprch << 16) + 
                                         hdisplay->Init.timing_param.hbkprch;

    hdisplay->Instance->timing_param2 = (hdisplay->Init.timing_param.vlines << 16) + 
                                         hdisplay->Init.timing_param.vsyncpw;

    hdisplay->Instance->timing_param3 = (hdisplay->Init.timing_param.vfnprch << 16) +
                                         hdisplay->Init.timing_param.vbkprch;

    hdisplay->Instance->row_update_area = (hdisplay->Init.update_area.end_row << 16) + 
                                           hdisplay->Init.update_area.start_row;

    hdisplay->Instance->col_update_area = (hdisplay->Init.update_area.end_col << 16) + 
                                           hdisplay->Init.update_area.start_col;
                                           
    hdisplay->Instance->tearing_threshold = hdisplay->Init.tearing_threshold;
    
    HAL_DISPLAY_SetFrameBuffer(hdisplay,
                               hdisplay->Init.master_param.base_addr, 
                               hdisplay->Init.master_param.total_byte_cnt, 
                               hdisplay->Init.master_param.burst_length);
    tempreg = 0;
    ((union _disp_set_rc_inst *)&tempreg)->bit.set_row_inst  = hdisplay->Init.rc_inst.set_row_inst;
    ((union _disp_set_rc_inst *)&tempreg)->bit.set_col_inst  = hdisplay->Init.rc_inst.set_col_inst;
    ((union _disp_set_rc_inst *)&tempreg)->bit.wr_ram_inst   = hdisplay->Init.rc_inst.wr_ram_inst;
    ((union _disp_set_rc_inst *)&tempreg)->bit.inst_type_sel = hdisplay->Init.rc_inst.inst_type_sel;
    hdisplay->Instance->set_rc_inst.reg_u32val = tempreg;
    tempreg = 0;
    ((union _disp_set_wqspi_inst *)&tempreg)->bit.wr_cmd_inst    = hdisplay->Init.wqspi_inst.wr_cmd_inst;
    ((union _disp_set_wqspi_inst *)&tempreg)->bit.wr_color_inst  = hdisplay->Init.wqspi_inst.wr_color_inst;
    ((union _disp_set_wqspi_inst *)&tempreg)->bit.wr_4color_inst = hdisplay->Init.wqspi_inst.wr_4color_inst;
    ((union _disp_set_wqspi_inst *)&tempreg)->bit.wr_4p_inst     = hdisplay->Init.wqspi_inst.wr_4p_inst;
    hdisplay->Instance->set_wqspi_inst.reg_u32val = tempreg;
    tempreg = 0;
    ((union _disp_set_Streaming_inst *)&tempreg)->bit.hs_inst         = hdisplay->Init.streaming_inst.hs_inst;
    ((union _disp_set_Streaming_inst *)&tempreg)->bit.vs_inst         = hdisplay->Init.streaming_inst.vs_inst;
    ((union _disp_set_Streaming_inst *)&tempreg)->bit.qspi_cmd_type   = hdisplay->Init.streaming_inst.qspi_cmd_type;
    ((union _disp_set_Streaming_inst *)&tempreg)->bit.qspi_color_type = hdisplay->Init.streaming_inst.qspi_color_type;
    ((union _disp_set_Streaming_inst *)&tempreg)->bit.qspi_rd_type    = hdisplay->Init.streaming_inst.qspi_rd_type;
    hdisplay->Instance->set_Streaming_inst.reg_u32val = tempreg;
    hdisplay->Instance->set_rqspi_inst = hdisplay->Init.rqspi_inst;
    hdisplay->ErrorCode = HAL_DISPLAY_ERROR_NONE;
    hdisplay->State     = HAL_DISPLAY_STATE_READY;
    __HAL_UNLOCK(hdisplay);
    return HAL_OK;
}
HAL_StatusTypeDef HAL_DISPLAY_DeInit(DISPLAY_HandleTypeDef *hdisplay)
{
    /* Check the DISPLAY handle allocation */
    if(hdisplay == NULL)
    {
        return HAL_ERROR;
    }
    __HAL_LOCK(hdisplay);
    hdisplay->ErrorCode = HAL_DISPLAY_ERROR_NONE;
    hdisplay->State     = HAL_DISPLAY_STATE_RESET;
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
    if (hdisplay->MspDeInitCallback == NULL)
    {
        hdisplay->MspDeInitCallback = HAL_DISPLAY_MspDeInit; /* Legacy weak MspInit  */
    }
    /* Init the low level hardware : GPIO, CLOCK, CLIC... */
    hdisplay->MspDeInitCallback(hdisplay);
#else
    /* Init the low level hardware : GPIO, CLOCK, CLIC... */
    HAL_DISPLAY_MspDeInit(hdisplay);
#endif /* USE_HAL_DISPLAY_REGISTER_CALLBACKS */
    __HAL_UNLOCK(hdisplay);
    return HAL_OK;
}
HAL_StatusTypeDef HAL_DISPLAY_SwitchMode(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_Mode mode)
{
    /* Check the DISPLAY handle allocation */
    if(hdisplay == NULL)
    {
        return HAL_ERROR;
    }
    if(hdisplay->State == HAL_DISPLAY_STATE_READY)
    {
        union _disp_cntl tempreg;
        __HAL_LOCK(hdisplay);
        hdisplay->State = HAL_DISPLAY_STATE_BUSY;
        tempreg.reg_u32val = hdisplay->Instance->display_cntl.reg_u32val;
        tempreg.bit.running_type = mode;
        hdisplay->Instance->display_cntl.reg_u32val = tempreg.reg_u32val;
        hdisplay->State = HAL_DISPLAY_STATE_READY;
        __HAL_UNLOCK(hdisplay);
    }
    return HAL_OK;
}
HAL_StatusTypeDef HAL_DISPLAY_SetDiv(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_Div_Enum div)
{
    /* Check the DISPLAY handle allocation */
    if(hdisplay == NULL)
    {
        return HAL_ERROR;
    }
    if(hdisplay->State == HAL_DISPLAY_STATE_READY)
    {
        __HAL_LOCK(hdisplay);
        union _disp_cntl tempreg;
        hdisplay->State = HAL_DISPLAY_STATE_BUSY;
        tempreg.reg_u32val = hdisplay->Instance->display_cntl.reg_u32val;
        tempreg.bit.clk_spi_div = div;
        hdisplay->Instance->display_cntl.reg_u32val = tempreg.reg_u32val;
        hdisplay->State = HAL_DISPLAY_STATE_READY;
        __HAL_UNLOCK(hdisplay);
    }
    return HAL_OK;
}
/**
  * @brief  display set framebuffer base addr and bytes.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  base_addr Framebuffer addr.
  * @param  bytes bytes of framebuffer.
  * @param  burst_length .
  * @note If bytes equl 0, the bytes not be written to total_byte_cnt
  * @retval void
  */
void HAL_DISPLAY_SetFrameBuffer(DISPLAY_HandleTypeDef *hdisplay, uint32_t base_addr, uint32_t bytes, uint32_t burst_length)
{
    hdisplay->Instance->base_addr = base_addr;
    if(bytes != 0)
    {
        hdisplay->Instance->total_byte_cnt = bytes;
    }
    if(burst_length != 0)
    {
        hdisplay->Instance->burst_length = burst_length;
    }
}

/**
  * @brief  display reset.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval void
  */
void HAL_DISPLAY_Reset(DISPLAY_HandleTypeDef *hdisplay)
{
    hdisplay->Instance->lcd_rst_cntl = 0;
}
/**
  * @brief  set reset pin as high
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval void
  */
void HAL_DISPLAY_Set(DISPLAY_HandleTypeDef *hdisplay)
{
    hdisplay->Instance->lcd_rst_cntl = 1;
}
/**
  * @brief  Initialize the DISPLAY MSP.
  * @param  hdisplay pointer to a DISPLAY_HandleTypeDef structure that contains
  *               the configuration information for DISPLAY module.
  * @retval None
  */
__WEAK void HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hdisplay);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DISPLAY_MspInit should be implemented in the user file
    */
}

/**
  * @brief  De-Initialize the DISPLAY MSP.
  * @param  hdisplay pointer to a DISPLAY_HandleTypeDef structure that contains
  *               the configuration information for DISPLAY module.
  * @retval None
  */
__WEAK void HAL_DISPLAY_MspDeInit(DISPLAY_HandleTypeDef *hdisplay)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hdisplay);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_DISPLAY_MspDeInit should be implemented in the user file
    */
}

#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_DISPLAY_RegisterCallback(DISPLAY_HandleTypeDef *hdisplay, HAL_DISPLAY_CallbackIDTypeDef CallbackID, pDISPLAY_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(pCallback == NULL)
    {
        /* Update the error code */
        hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hdisplay);
    if (HAL_DISPLAY_STATE_READY == hdisplay->State)
    {
        switch (CallbackID)
        {
        case HAL_DISPLAY_DATA_FIFO_ER_CB_ID:
            hdisplay->data_fifo_er_Callback = pCallback;
        break;
        case HAL_DISPLAY_DATA_FIFO_FW_CB_ID:
            hdisplay->data_fifo_fw_Callback = pCallback;
        break;
        case HAL_DISPLAY_CMD_FIFO_FW_CB_ID:
            hdisplay->cmd_fifo_fw_Callback = pCallback;
        break;
        case HAL_DISPLAY_DATA_FIFO_NOT_EMPTY_ID:
            hdisplay->data_fifo_not_empty_Callback = pCallback;
        break;
        case HAL_DISPLAY_CMD_FIFO_DONE_ID:
            hdisplay->cmd_fifo_done_Callback = pCallback;
        break;
        case HAL_DISPLAY_TEARING_INT_CB_ID:
            hdisplay->tearing_int_Callback = pCallback;
        break;
        case HAL_DISPLAY_FIFO_EMPTY_RD_CB_ID:
            hdisplay->fifo_empty_rd_Callback = pCallback;
        break;
        case HAL_DISPLAY_VS_INT_CB_ID:
            hdisplay->vs_int_Callback = pCallback;
        break;
        default :
            /* Update the error code */
            hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        break;
        }
    }
    else if (HAL_DISPLAY_STATE_RESET == hdisplay->State)
    {
        switch (CallbackID)
        {
        case HAL_DISPLAY_MSPINIT_CB_ID :
            hdisplay->MspInitCallback = pCallback;
        break;
        case HAL_DISPLAY_MSPDEINIT_CB_ID :
            hdisplay->MspDeInitCallback = pCallback;
        break;
        default:
            /* Update the error code */
            hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        break;
        }
    }
    else
    {
        /* Update the error code */
        hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hdisplay);
    return status;
}
HAL_StatusTypeDef HAL_DISPLAY_UnRegisterCallback(DISPLAY_HandleTypeDef *hdisplay, HAL_DISPLAY_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hdisplay);
    if (HAL_DISPLAY_STATE_READY == hdisplay->State)
    {
        switch (CallbackID)
        {
        case HAL_DISPLAY_DATA_FIFO_ER_CB_ID:
            hdisplay->data_fifo_er_Callback = HAL_DISPLAY_IRQCB_data_fifo_er;
        break;
        case HAL_DISPLAY_DATA_FIFO_FW_CB_ID:
            hdisplay->data_fifo_fw_Callback = HAL_DISPLAY_IRQCB_data_fifo_fw;
        break;
        case HAL_DISPLAY_CMD_FIFO_FW_CB_ID:
            hdisplay->cmd_fifo_fw_Callback = HAL_DISPLAY_IRQCB_cmd_fifo_fw;
        break;
        case HAL_DISPLAY_DATA_FIFO_NOT_EMPTY_ID:
            hdisplay->data_fifo_not_empty_Callback = HAL_DISPLAY_IRQCB_data_fifo_not_empty;
        break;
        case HAL_DISPLAY_CMD_FIFO_DONE_ID:
            hdisplay->cmd_fifo_done_Callback = HAL_DISPLAY_IRQCB_cmd_fifo_done;
        break;
        case HAL_DISPLAY_TEARING_INT_CB_ID:
            hdisplay->tearing_int_Callback = HAL_DISPLAY_IRQCB_tearing_int;
        break;
        case HAL_DISPLAY_FIFO_EMPTY_RD_CB_ID:
            hdisplay->fifo_empty_rd_Callback = HAL_DISPLAY_IRQCB_fifo_empty_rd;
        break;
        case HAL_DISPLAY_VS_INT_CB_ID:
            hdisplay->vs_int_Callback = HAL_DISPLAY_IRQCB_vs_int;
        break;
        default:
            /* Update the error code */
            hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else if (HAL_DISPLAY_STATE_RESET == hdisplay->State)
    {
        switch (CallbackID)
        {
        case HAL_DISPLAY_MSPINIT_CB_ID :
            hdisplay->MspInitCallback = HAL_DISPLAY_MspInit;                   /* Legacy weak MspInit              */
        break;
        case HAL_DISPLAY_MSPDEINIT_CB_ID :
            hdisplay->MspDeInitCallback = HAL_DISPLAY_MspDeInit;               /* Legacy weak MspDeInit            */
        break;
        default:
            /* Update the error code */
            hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else
    {
        /* Update the error code */
        hdisplay->ErrorCode |= HAL_DISPLAY_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hdisplay);
    return status;
}
#endif
/**
  * @brief  send cmd to lcd
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  cmd command of send to lcd.
  * @retval void
  */
void HAL_DISPLAY_SendCmd(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_CmdType cmd_type, uint8_t reg_or_data)
{
    union _disp_cmd_fifo_input tempreg;
    tempreg.reg_u32val = 0;
    tempreg.bit.cmd_dcx = cmd_type;
    tempreg.bit.cmd_data = reg_or_data;
    hdisplay->Instance->cmd_fifo_input.reg_u32val = tempreg.reg_u32val;
}
/**
  * @brief  send read command.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  regID ID of register.
  * @param  pdata Pointer to data buffer
  * @param  read_byte The length of the read data.
  * @param  timeout Timeout duration
  * @retval void.
  */
HAL_StatusTypeDef HAL_DISPLAY_ReadReg(DISPLAY_HandleTypeDef *hdisplay, uint32_t regID, uint8_t *pdata, uint32_t read_byte, uint32_t timeout)
{
    /* Init tickstart for timeout management*/
    uint32_t tickstart = HAL_GetTick();
    assert_param(hdisplay != NULL);
    assert_param(hdisplay->Instance != NULL);
    assert_param(pdata != NULL);
    assert_param(read_byte != 0);
    union _disp_cmd_fifo_input tempreg;
    tempreg.reg_u32val = 0;
    tempreg.bit.cmd_rd_size = read_byte;
    tempreg.bit.cmd_rd = 1;
    tempreg.bit.cmd_data = regID;
    /* Process locked */
    __HAL_LOCK(hdisplay);
    if (hdisplay->State == HAL_DISPLAY_STATE_READY)
    {
        hdisplay->State = HAL_DISPLAY_STATE_BUSY;
        hdisplay->Instance->cmd_fifo_input.reg_u32val = tempreg.reg_u32val;
        for (int i = 0; i < read_byte; i++)
        {
            while(HAL_DISPLAY_IsLCDDataFifoEmpty(hdisplay))
            {
                if(timeout != HAL_MAX_DELAY)
                {
                    if((HAL_GetTick() - tickstart) > timeout)
                    {
                        hdisplay->State = HAL_DISPLAY_STATE_READY;
                        /* Process Unlocked */
                        __HAL_UNLOCK(hdisplay);
                        return HAL_TIMEOUT;
                    }
                }
            }
            *pdata = (hdisplay->Instance->data_fifo_output & 0xff);
        }
    }
    hdisplay->State = HAL_DISPLAY_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hdisplay);
    return HAL_OK;
}
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
static inline void DISPLAY_CallbackInvoke(pDISPLAY_CallbackTypeDef callback, void *arg)
{
    if(callback)
    {
        callback(arg);
    }
}
#endif
void HAL_DISPLAY_IRQHandler(DISPLAY_HandleTypeDef *hdisplay)
{
    hdisplay->status = hdisplay->Instance->interrupt;
    hdisplay->Instance->interrupt = 0;
    if(hdisplay->status & DISPLAY_IT_vs_int)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->vs_int_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_vs_int(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_fifo_empty_rd)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->fifo_empty_rd_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_fifo_empty_rd(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_tearing_int)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->tearing_int_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_tearing_int(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_cmd_fifo_done)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->cmd_fifo_done_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_cmd_fifo_done(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_data_fifo_not_empty)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->data_fifo_not_empty_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_data_fifo_not_empty(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_cmd_fifo_fw)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->cmd_fifo_fw_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_cmd_fifo_fw(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_data_fifo_fw)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->data_fifo_fw_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_data_fifo_fw(hdisplay);
#endif
    }
    if(hdisplay->status & DISPLAY_IT_data_fifo_er)
    {
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
        DISPLAY_CallbackInvoke(hdisplay->data_fifo_er_Callback, hdisplay);
#else
        HAL_DISPLAY_IRQCB_data_fifo_er(hdisplay);
#endif
    }
}
/**
  * @brief  Interrupt callback.
  * @param  hdisplay pointer to a DISPLAY_HandleTypeDef structure that contains
  *               the configuration information for DISPLAY module.
  * @retval None
  */
__WEAK void HAL_DISPLAY_IRQCB_vs_int(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_fifo_empty_rd(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_tearing_int(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_cmd_fifo_done(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_data_fifo_not_empty(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_cmd_fifo_fw(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_data_fifo_fw(DISPLAY_HandleTypeDef *hdisplay)
{
}
__WEAK void HAL_DISPLAY_IRQCB_data_fifo_er(DISPLAY_HandleTypeDef *hdisplay)
{
}
void HAL_DISPLAY_GetCCM(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_CCM_Type *ccm)
{
    ccm->Mr0 = hdisplay->Instance->ccm_matrix_r.bit.M0;
    ccm->Mr1 = hdisplay->Instance->ccm_matrix_r.bit.M1;
    ccm->Mr2 = hdisplay->Instance->ccm_matrix_r.bit.M2;
    ccm->Mr3 = hdisplay->Instance->ccm_matrix_r.bit.M3;

    ccm->Mg0 = hdisplay->Instance->ccm_matrix_g.bit.M0;
    ccm->Mg1 = hdisplay->Instance->ccm_matrix_g.bit.M1;
    ccm->Mg2 = hdisplay->Instance->ccm_matrix_g.bit.M2;
    ccm->Mg3 = hdisplay->Instance->ccm_matrix_g.bit.M3;

    ccm->Mb0 = hdisplay->Instance->ccm_matrix_b.bit.M0;
    ccm->Mb1 = hdisplay->Instance->ccm_matrix_b.bit.M1;
    ccm->Mb2 = hdisplay->Instance->ccm_matrix_b.bit.M2;
    ccm->Mb3 = hdisplay->Instance->ccm_matrix_b.bit.M3;
}
void HAL_DISPLAY_SetCCM(DISPLAY_HandleTypeDef *hdisplay, const DISPLAY_CCM_Type *ccm)
{
    union _disp_ccm_matrix tempreg;
    tempreg.bit.M0 = ccm->Mr0;
    tempreg.bit.M1 = ccm->Mr1;
    tempreg.bit.M2 = ccm->Mr2;
    tempreg.bit.M3 = ccm->Mr3;
    hdisplay->Instance->ccm_matrix_r.reg_u32val = tempreg.reg_u32val;
    tempreg.bit.M0 = ccm->Mg0;
    tempreg.bit.M1 = ccm->Mg1;
    tempreg.bit.M2 = ccm->Mg2;
    tempreg.bit.M3 = ccm->Mg3;
    hdisplay->Instance->ccm_matrix_g.reg_u32val = tempreg.reg_u32val;
    tempreg.bit.M0 = ccm->Mb0;
    tempreg.bit.M1 = ccm->Mb1;
    tempreg.bit.M2 = ccm->Mb2;
    tempreg.bit.M3 = ccm->Mb3;
    hdisplay->Instance->ccm_matrix_b.reg_u32val = tempreg.reg_u32val;
}
void HAL_DISPLAY_GetYCT(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_YCT_Type *yct)
{
    yct->Mco0 = hdisplay->Instance->yct_matrix.bit.Mco0;
    yct->Mco1 = hdisplay->Instance->yct_matrix.bit.Mco1;
    yct->Mcg0 = hdisplay->Instance->yct_matrix.bit.Mcg0;
    yct->Mcg1 = hdisplay->Instance->yct_matrix.bit.Mcg1;
}
void HAL_DISPLAY_SetYCT(DISPLAY_HandleTypeDef *hdisplay, const DISPLAY_YCT_Type *yct)
{
    union _disp_yct_matrix tempreg;
    tempreg.bit.Mco0 = yct->Mco0;
    tempreg.bit.Mco1 = yct->Mco1;
    tempreg.bit.Mcg0 = yct->Mcg0;
    tempreg.bit.Mcg1 = yct->Mcg1;
    hdisplay->Instance->yct_matrix.reg_u32val = tempreg.reg_u32val;
}



#endif /* HAL_DISPLAY_MODULE_ENABLED */