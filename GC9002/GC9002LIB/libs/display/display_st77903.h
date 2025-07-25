#ifndef __DISPLAY_ST77903_H__
#define __DISPLAY_ST77903_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// st77903 is qspi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_ST77903_ENABLED && DRV_DISPLAY_ST77903_ENABLED == 1U)

#define LCD_ST77903_SLEEP_DELAY_MS             (120)//120ms
#define LCD_ST77903_SETUP_DELAY_MS             (120)//120ms
#define LCD_ST77903_OPEN_SREEN_DELAY_MS        (120)//120ms
#define LCD_ST77903_RESET_DELAY_MS             (10)//10ms

//initiate for qspi screen
void display_st77903_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_st77903_master_active(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_st77903_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_ST77903_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_ST77903_H__ */