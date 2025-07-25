#ifndef __DISPLAY_SJXD_H__
#define __DISPLAY_SJXD_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// sjxd is qspi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_SJXD_ENABLED && DRV_DISPLAY_SJXD_ENABLED == 1U)

#define LCD_SJXD_SLEEP_DELAY_MS             (120)//120ms
#define LCD_SJXD_SETUP_DELAY_MS             (100)//100ms
#define LCD_SJXD_OPEN_SREEN_DELAY_MS        (120)//120ms
#define LCD_SJXD_RESET_DELAY_MS             (10)//10ms

//initiate for qspi screen
void display_sjxd_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_sjxd_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_sjxd_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_SJXD_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_SJXD_H__ */