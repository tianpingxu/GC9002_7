#ifndef __DISPLAY_GC9307N_H__
#define __DISPLAY_GC9307N_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// gc9307n is spi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_GC9307N_ENABLED && DRV_DISPLAY_GC9307N_ENABLED == 1U)

#define LCD_GC9307N_SLEEP_DELAY_MS          (120)//120ms
#define LCD_GC9307N_SETUP_DELAY_MS          (100)//100ms
#define LCD_GC9307N_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_GC9307N_RESET_DELAY_MS          (10)//10ms
#define LCD_GC9307N_DISPLAY_ON_CMD          (0x11)
#define LCD_GC9307N_OPEN_SCREEN_CMD         (0x29)

#define USE_HORIZONTAL                      (0)
#define LCD_WR_REG(reg)                     HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, reg)
#define LCD_WR_DATA8(data)                  HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, data)
//initiate for qspi screen
void display_gc9307n_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_gc9307n_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_gc9307n_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_GC9307N_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_GC9307N_H__ */