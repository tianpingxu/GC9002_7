#ifndef __DISPLAY_TFT128QM071_H__
#define __DISPLAY_TFT128QM071_H__
#ifdef __cplusplus
extern "C" {
#endif
//function for different screen initiate
// tft128qm071 is spi, output rgb565 screen

#include "display_conf.h"
#if(defined DRV_DISPLAY_TFT128QM071_ENABLED && DRV_DISPLAY_TFT128QM071_ENABLED == 1U)

#define LCD_TFT128QM071_SLEEP_DELAY_MS             (120)//120ms
#define LCD_TFT128QM071_SETUP_DELAY_MS             (120)//120ms
#define LCD_TFT128QM071_OPEN_SREEN_DELAY_MS        (120)//120ms
#define LCD_TFT128QM071_RESET_DELAY_MS             (10)//10ms

//initiate for qspi screen
void display_tft128qm071_init(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_tft128qm071_master_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);
void display_tft128qm071_slave_passive(DISPLAY_HandleTypeDef *hdisplay, struct display_conf *config);

#endif /* DRV_DISPLAY_TFT128QM071_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __DISPLAY_TFT128QM071_H__ */