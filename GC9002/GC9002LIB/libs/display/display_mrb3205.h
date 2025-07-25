#ifndef __DISPLAY_MRB3205_H__
#define __DISPLAY_MRB3205_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// mrb3205 is 8080-8bit, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_MRB3205_ENABLED && DRV_DISPLAY_MRB3205_ENABLED == 1U)

#define LCD_MRB3205_SLEEP_DELAY_MS          (120)//120ms
#define LCD_MRB3205_SETUP_DELAY_MS          (100)//100ms
#define LCD_MRB3205_OPEN_SREEN_DELAY_MS     (120)//120ms
#define LCD_MRB3205_RESET_DELAY_MS          (10)//10ms

//initiate for 8080-8bit screen
void display_mrb3205_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_mrb3205_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_mrb3205_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_MRB3205_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_MRB3205_H__ */