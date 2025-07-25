#ifndef __DISPLAY_LEIKE_H__
#define __DISPLAY_LEIKE_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// leike is qspi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_LEIKE_ENABLED && DRV_DISPLAY_LEIKE_ENABLED == 1U)

#define LCD_LEIKE_SLEEP_DELAY_MS             (120)//120ms
#define LCD_LEIKE_SETUP_DELAY_MS             (120)//120ms
#define LCD_LEIKE_OPEN_SREEN_DELAY_MS        (120)//120ms
#define LCD_LEIKE_RESET_DELAY_MS             (10)//10ms

//initiate for qspi screen
void display_leike_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_leike_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_leike_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_LEIKE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_LEIKE_H__ */