#include "platform.h"
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "gc90xx_hal.h"
#else
#include "gc90x_i2s.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CHIP_GC9002)
#define I2S_TX_FIFO_DEPTH	8
#elif defined(CHIP_GC9003) || defined(CHIP_GC9005)
#define I2S_TX_FIFO_DEPTH	16
#endif

void csi_ahmi_i2s_init(uint32_t SampleRate);
void csi_ahmi_i2s_play(uint16_t *MusicStart, uint32_t MusicSize, uint8_t MusicLoop, uint16_t PlayNum);
void csi_ahmi_i2s_pause(void);
void csi_ahmi_i2s_resume(void);
void csi_ahmi_i2s_stop(void);

#ifdef __cplusplus
}
#endif
