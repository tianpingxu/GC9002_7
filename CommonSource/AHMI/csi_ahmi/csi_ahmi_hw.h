#ifndef CSI_AHMI_HW_INIT
#define CSI_AHMI_HW_INIT
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void csi_ahmi_hw_int(void);
void csi_ahmi_SetBacklightBrightness(uint32_t brightness);
uint32_t csi_ahmi_CheckBacklight(void);
void csi_ahmi_StartBacklightForFivePoint(void);
void csi_ahmi_StartBacklight(void);
void csi_ahmi_StopBacklight(void);
void csi_ahmi_InitBuzzer(uint32_t frequence, uint32_t duty, uint32_t dutyDiv, uint32_t duration);
uint32_t csi_ahmi_CheckBuzzer(void);
void csi_ahmi_StartBuzzer(void);
void csi_ahmi_StopBuzzer(void);
void csi_ahmi_InitBacklightAndTimer(void);
void csi_ahmi_StartBuzzerAndTimer(void);
uint8_t csi_ahmi_AHMITouch_InitDevice(void);




#ifdef __cplusplus
}
#endif
#endif /* End of CSI_AHMI_HW_INIT */
