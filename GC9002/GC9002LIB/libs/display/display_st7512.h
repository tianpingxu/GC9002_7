#ifndef __DISPLAY_ST7512_H__
#define __DISPLAY_ST7512_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// st7512 is 8080-8bit, output Y2 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_ST7512_ENABLED && DRV_DISPLAY_ST7512_ENABLED == 1U)

#define LCD_ST7512_SLEEP_DELAY_MS          (120)//120ms
#define LCD_ST7512_SETUP_DELAY_MS          (100)//100ms
#define LCD_ST7512_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_ST7512_RESET_DELAY_MS          (10)//10ms

//initiate for qspi screen
void display_st7512_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_st7512_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_ST7512_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_ST7512_H__ */