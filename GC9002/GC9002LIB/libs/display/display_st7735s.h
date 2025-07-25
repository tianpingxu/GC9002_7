#ifndef __DISPLAY_ST7735S_H__
#define __DISPLAY_ST7735S_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// st7735s is spi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_ST7735S_ENABLED && DRV_DISPLAY_ST7735S_ENABLED == 1U)

#define LCD_ST7735S_SLEEP_DELAY_MS          (120)//120ms
#define LCD_ST7735S_SETUP_DELAY_MS          (100)//100ms
#define LCD_ST7735S_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_ST7735S_RESET_DELAY_MS          (10)//10ms
#define LCD_ST7735S_DISPLAY_ON_CMD          (0x11)
#define LCD_ST7735S_OPEN_SCREEN_CMD         (0x29)

#define USE_HORIZONTAL                      (0)
#define LCD_WR_REG(reg)                     HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_REG, reg)
#define LCD_WR_DATA8(data)                  HAL_DISPLAY_SendCmd(hdisplay, DISPLAY_DATA, data)
//initiate for qspi screen
void display_st7735s_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_st7735s_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_st7735s_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_ST7735S_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_ST7735S_H__ */