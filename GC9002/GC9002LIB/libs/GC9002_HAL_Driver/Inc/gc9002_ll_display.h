/**
  ******************************************************************************
  * @file    gc9002_ll_display.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the DISPLAY
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_LL_DISPLAY_H__
#define __GC9002_LL_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"
#include <stdio.h>
#include <stdbool.h>

#ifdef HAL_DISPLAY_MODULE_ENABLED


#define INPUT_COLOR_MODE_RGBA8888 0
#define INPUT_COLOR_MODE_RGB565   1
#define INPUT_COLOR_MODE_Y8       4

#define OUTPUT_COLOR_MODE_Y8 0
#define OUTPUT_COLOR_MODE_Y4 1
#define OUTPUT_COLOR_MODE_Y2 2
#define OUTPUT_COLOR_MODE_Y1 3
#define OUTPUT_COLOR_MODE_RGB565 4
#define OUTPUT_COLOR_MODE_RGB666 5 //reserved
#define OUTPUT_COLOR_MODE_RGB888 6

#define INTERFACE_TYPE_SPI3 0
#define INTERFACE_TYPE_SPI4 1
#define INTERFACE_TYPE_QSPI 3
#define INTERFACE_TYPE_8B   4
#define INTERFACE_TYPE_24B  5

#define RUNNING_TYPE_MA  0 //master active
#define RUNNING_TYPE_MP  1 //master passive
#define RUNNING_TYPE_SP  2 //slave passive
#define RUNNING_TYPE_CFG 3 //configuration mode

typedef struct
{
    uint16_t hpixels;
    uint16_t hsyncpw;
    uint16_t hfnprch;
    uint16_t hbkprch;
    uint16_t vlines;
    uint16_t vsyncpw;
    uint16_t vfnprch;
    uint16_t vbkprch;
}display_timing_param_struct;

//note : start and end is included for rendering
typedef struct
{
    uint16_t start_row;
    uint16_t end_row;
    uint16_t start_col;
    uint16_t end_col;

}display_update_area_struct;

typedef struct
{
    uint8_t set_row_inst;
    uint8_t set_col_inst;
    uint8_t set_ram_inst;
    uint8_t inst_type_sel;
}set_rc_inst_struct;

typedef struct
{
    uint8_t wr_cmd_inst;
    uint8_t wr_color_inst;
    uint8_t wr_4color_inst;
    uint8_t wr_4p_inst;
}set_wqspi_inst_struct;

typedef struct
{
    uint32_t hs_inst;
    uint32_t vs_inst;
    uint32_t qspi_cmd_type;
    uint32_t qspi_color_type;
    uint32_t qspi_rd_type;

}set_active_inst_struct;

typedef struct
{
    uint32_t base_addr;
    uint32_t total_byte_cnt;
    uint32_t burst_length;
}set_master_struct;

void display_init(DISPLAY_TypeDef *display, display_cntl_struct* p_disp_cntl, display_timing_param_struct *p_disp_timing_param, display_update_area_struct *p_display_update_area);
void display_set_cntl(DISPLAY_TypeDef *display, display_cntl_struct* p_disp_cntl);
void display_set_timing_param(DISPLAY_TypeDef *display, display_timing_param_struct *p_disp_timing_param);
void display_set_update_area(DISPLAY_TypeDef *display, display_update_area_struct *p_display_update_area);
void display_send_cmd_array(DISPLAY_TypeDef *display, uint32_t array[], uint32_t num);
/**
  * @brief  send cmd to lcd
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  cmd command of send to lcd.
  * @retval void
  */
static inline void display_send_cmd(DISPLAY_TypeDef *display, uint32_t cmd)
{
    display->cmd_fifo_input.reg_u32val = cmd;
}
void display_send_read_cmd(DISPLAY_TypeDef *display, uint32_t regID, uint32_t read_byte);
/**
  * @brief  Sets thresholds for row or column tearing.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  threshold The threshold for tearing rows or columns.
  * @retval void
  */
static inline void display_set_tearing_threshold(DISPLAY_TypeDef *display, uint32_t threshold)
{
    display->tearing_threshold = threshold;
}

void display_delay(uint32_t num);

/**
  * @brief  display reset.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval void
  */
static inline void display_reset(DISPLAY_TypeDef *display)
{
    display->lcd_rst_cntl = 0;//set reset pin as 0
	// GPIO0_REG->p13 = 0x00000000;
}
/**
  * @brief  set reset pin as high
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval void
  */
static inline void display_set(DISPLAY_TypeDef *display)
{
    display->lcd_rst_cntl = 1;//set reset pin as 1
	// GPIO0_REG->p13 = 0x00000001;
}

void display_set_master(DISPLAY_TypeDef *display, set_master_struct *p_set_master);
void display_set_wqspi_inst(DISPLAY_TypeDef *display, uint8_t wr_4p_inst, uint8_t wr_4color_inst, uint8_t wr_color_inst, uint8_t wr_cmd_inst);
void display_set_streaming_inst(DISPLAY_TypeDef *display, uint8_t qspi_rd_type, uint8_t qspi_color_type, uint8_t qspi_cmd_type, uint8_t vs_inst, uint8_t hs_inst);
void display_set_rc_inst(DISPLAY_TypeDef *display, uint8_t set_row_inst, uint8_t set_col_inst, uint8_t wr_ram_inst, uint8_t inst_type_sel);

/**
  * @brief  read lcd data.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval data of lcd.
  */
static inline uint8_t display_read_lcd_data(DISPLAY_TypeDef *display)
{
    return ((display->data_fifo_output) & 0xff);
}
/**
  * @brief  Example Query whether the lcd data FIFO is empty.
  * @param  display pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval bool true: data fifo empty, otherwise the contrary.
  */
static inline bool display_is_lcd_data_fifo_empty(DISPLAY_TypeDef *display)
{
    return (display->fifo_status.bit.data_fifo_empty != 0);
}

#endif /* HAL_DISPLAY_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_LL_DISPLAY_H__ */