#ifndef __DISPLAY_888_H__
#define __DISPLAY_888_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// 888 is 8080-8bit, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_888_ENABLED && DRV_DISPLAY_888_ENABLED == 1U)

#define LCD_888_SLEEP_DELAY_MS          (120)//120ms
#define LCD_888_SETUP_DELAY_MS          (100)//100ms
#define LCD_888_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_888_RESET_DELAY_MS          (10)//10ms

//initiate for 8080-8bit screen
void display_888_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_888_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_888_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_888_H__ */