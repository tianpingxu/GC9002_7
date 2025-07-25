#include "platform.h"
#include "gc90x_bt656.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VIDEO_Vertical_Synchronization_ENABLE

#define VIDEO_BUFFER_NUM    (3)

#if (VIDEO_BUFFER_NUM != 2) && (VIDEO_BUFFER_NUM != 3)
#error "VIDEO_BUFFER_NUM must be 2 or 3 !"
#endif

typedef enum
{
    Video_CVBS = 1,
    Video_DVP,
    Video_AHD,
    Video_RGB
}Video_teType;

typedef enum
{
	BT656_Init = 0,
    BT656_AHMIDone,
	BT656_WaitCaptureDone,
	BT656_CaptureDone
}BT656_teMainSM;

void csi_ahmi_bt656_init(uint8_t videotype);
void csi_ahmi_bt656_enable(void);
void csi_ahmi_bt656_disable(void);
void csi_ahmi_bt656_getParam(uint16_t* Height, uint16_t* Width);
void csi_ahmi_bt656_switchbuffer(uint8_t* flag_frame, uint8_t videotype);
#ifdef __cplusplus
}
#endif
