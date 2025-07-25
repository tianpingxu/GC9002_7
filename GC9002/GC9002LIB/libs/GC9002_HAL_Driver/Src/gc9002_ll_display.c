
#include "gc90xx_hal.h"
#ifdef HAL_DISPLAY_MODULE_ENABLED

/**
  * @brief  Initializes the display peripheral registers to their default reset values.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  p_disp_cntl pointer to a display_cntl_struct structure.
  * @param  p_disp_timing_param pointer to a display_timing_param_struct structure.
  * @param  p_display_update_area pointer to a display_update_area_struct structure.
  * @retval void.
  */
void display_init(DISPLAY_TypeDef *display, 
                  display_cntl_struct *p_disp_cntl, 
                  display_timing_param_struct *p_disp_timing_param, 
                  display_update_area_struct *p_display_update_area)
{
    display_set_cntl(display, p_disp_cntl); //set reg0
    display_set_timing_param(display, p_disp_timing_param);
    display_set_update_area(display, p_display_update_area);
    display_set_tearing_threshold(display, 0x500);
}

/**
  * @brief  set display cntl register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  p_disp_cntl pointer to a display_cntl_struct structure.
  * @retval void.
  */
void display_set_cntl(DISPLAY_TypeDef *display, display_cntl_struct *p_disp_cntl)
{
    union _disp_cntl tempreg;
    tempreg.bit.enable               = p_disp_cntl->enable;
    tempreg.bit.invert_oclk          = p_disp_cntl->invert_oclk;
    tempreg.bit.invert_hs            = p_disp_cntl->invert_hs;
    tempreg.bit.invert_vs            = p_disp_cntl->invert_vs;
    tempreg.bit.update_ram           = p_disp_cntl->update_ram;
    tempreg.bit.fg_enable            = p_disp_cntl->fg_enable;
    tempreg.bit.dithering            = p_disp_cntl->dithering;
    tempreg.bit.input_color_mode     = p_disp_cntl->input_color_mode;
    tempreg.bit.output_color_mode    = p_disp_cntl->output_color_mode;
    tempreg.bit.interface_type       = p_disp_cntl->interface_type;
    tempreg.bit.running_type         = p_disp_cntl->running_type;
    tempreg.bit.tearing_en           = p_disp_cntl->tearing_en;
    tempreg.bit.update_en            = p_disp_cntl->update_en;
    tempreg.bit.clk_spi_div          = p_disp_cntl->clk_spi_div;
    tempreg.bit.icg_en               = p_disp_cntl->icg_en;
    tempreg.bit.lcd_mv               = p_disp_cntl->lcd_mv;
    display->display_cntl.reg_u32val = tempreg.reg_u32val;
}

/**
  * @brief  set display timing_paramX(X=0,1,2,3) register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  p_disp_timing_param pointer to a display_timing_param_struct structure.
  * @retval void.
  */
void display_set_timing_param(DISPLAY_TypeDef *display, display_timing_param_struct *p_disp_timing_param)
{
    display->timing_param0 = (p_disp_timing_param->hpixels << 16) + 
                              p_disp_timing_param->hsyncpw;

    display->timing_param1 = (p_disp_timing_param->hfnprch << 16) + 
                              p_disp_timing_param->hbkprch;

    display->timing_param2 = (p_disp_timing_param->vlines << 16) + 
                              p_disp_timing_param->vsyncpw;

    display->timing_param3 = (p_disp_timing_param->vfnprch << 16) +
                              p_disp_timing_param->vbkprch;
}
/**
  * @brief  set display update_area register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  p_display_update_area pointer to a display_update_area_struct structure.
  * @retval void.
  */
void display_set_update_area(DISPLAY_TypeDef *display, display_update_area_struct *p_display_update_area)
{
    display->row_update_area = (p_display_update_area->end_row << 16) + 
                                p_display_update_area->start_row;

    display->col_update_area = (p_display_update_area->end_col << 16) + 
                                p_display_update_area->start_col;

}
/**
  * @brief  send command array.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  array array of commands.
  * @param  num count of commands.
  * @retval void.
  */
void display_send_cmd_array(DISPLAY_TypeDef *display, uint32_t array[], uint32_t num)
{
	uint32_t array_data;
    //int dcx;
    for(int i = 0; i < num; i++)
    {
        display_send_cmd(display, array[i]);
    }
}
/**
  * @brief  send read command.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  regID ID of register.
  * @param  read_byte The length of the read data.
  * @retval void.
  */
void display_send_read_cmd(DISPLAY_TypeDef *display, uint32_t regID, uint32_t read_byte)
{
    union _disp_cmd_fifo_input tempreg;
    tempreg.reg_u32val = 0;
    tempreg.bit.cmd_rd_size = read_byte;
    tempreg.bit.cmd_rd = 1;
    tempreg.bit.cmd_data = regID;
    display->cmd_fifo_input.reg_u32val = tempreg.reg_u32val;
}
/**
  * @brief  delay for display.
  * @param  num 
  * @retval void.
  */
void display_delay(uint32_t num)
{
    uint32_t i;
    for(i = 0; i < num; i++)
    {
        ;
    }
}
/**
  * @brief  set master register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  p_set_master pointer to a set_master_struct structure.
  * @retval void.
  */
void display_set_master(DISPLAY_TypeDef *display, set_master_struct *p_set_master)
{
    display->base_addr      = p_set_master->base_addr;
    display->total_byte_cnt = p_set_master->total_byte_cnt;
    display->burst_length   = p_set_master->burst_length;
}
/**
  * @brief  set wqspi_inst register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  wr_4p_inst Start write a four-line command four-line address four-line data command word.
  * @param  wr_4color_inst Start to write the four-wire address four-wire data command word.
  * @param  wr_color_inst Start write the one-line address four-line data command word.
  * @param  wr_cmd_inst Set the write single-line address single-line data command word.
  * @retval void.
  */
void display_set_wqspi_inst(DISPLAY_TypeDef *display, uint8_t wr_4p_inst, uint8_t wr_4color_inst, uint8_t wr_color_inst, uint8_t wr_cmd_inst)
{
    union _disp_set_wqspi_inst tempreg;
    tempreg.bit.wr_4p_inst = wr_4p_inst;
    tempreg.bit.wr_4color_inst = wr_4color_inst;
    tempreg.bit.wr_color_inst = wr_color_inst;
    tempreg.bit.wr_cmd_inst = wr_cmd_inst;
	display->set_wqspi_inst.reg_u32val = tempreg.reg_u32val;
}
/**
  * @brief  set streaming_inst register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  qspi_rd_type qspi interface input set format.
  * @param  qspi_color_type The qspi interface output sets the color format.
  * @param  qspi_cmd_type Indicates the command output format of the qspi interface.
  * @param  vs_inst Send vs instruction word.
  * @param  hs_inst Send hs instruction word.
  * @retval void.
  */
void display_set_streaming_inst(DISPLAY_TypeDef *display, uint8_t qspi_rd_type, uint8_t qspi_color_type, uint8_t qspi_cmd_type, uint8_t vs_inst, uint8_t hs_inst)
{
    union _disp_set_Streaming_inst tempreg;
    tempreg.bit.qspi_rd_type = qspi_rd_type;
    tempreg.bit.qspi_color_type = qspi_color_type;
    tempreg.bit.qspi_cmd_type = qspi_cmd_type;
    tempreg.bit.vs_inst = vs_inst;
    tempreg.bit.hs_inst = hs_inst;
	display->set_Streaming_inst.reg_u32val = tempreg.reg_u32val;
}
/**
  * @brief  set rc_inst register.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  set_row_inst Set the command word of the command that starts and ends the line.
  * @param  set_col_inst Command word for the command that sets the start and end columns.
  * @param  wr_ram_inst Begins writing the cached command word.
  * @param  inst_type_sel Select the format for sending commands.
  * @retval void.
  */
void display_set_rc_inst(DISPLAY_TypeDef *display, uint8_t set_row_inst, uint8_t set_col_inst, uint8_t wr_ram_inst, uint8_t inst_type_sel)
{
    union _disp_set_rc_inst tempreg;
    tempreg.bit.set_row_inst = set_row_inst;
    tempreg.bit.set_col_inst = set_col_inst;
    tempreg.bit.wr_ram_inst = wr_ram_inst;
    tempreg.bit.inst_type_sel = inst_type_sel;
    display->set_rc_inst.reg_u32val = tempreg.reg_u32val;
}

#endif /* HAL_DISPLAY_MODULE_ENABLED */