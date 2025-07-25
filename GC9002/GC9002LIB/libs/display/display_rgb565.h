#ifndef __DISPLAY_rgb565_H__
#define __DISPLAY_rgb565_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// rgb565 is 8080-8bit, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_RGB565_ENABLED && DRV_DISPLAY_RGB565_ENABLED == 1U)

#define LCD_rgb565_SLEEP_DELAY_MS          (120)//120ms
#define LCD_rgb565_SETUP_DELAY_MS          (100)//100ms
#define LCD_rgb565_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_rgb565_RESET_DELAY_MS          (10)//10ms

//initiate for 8080-8bit screen
void display_rgb565_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_rgb565_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_RGB565_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_rgb565_H__ */