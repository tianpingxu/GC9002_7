#include "sample_def.h"
#include "csi_pin.h"
#include "csi_i2s.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_I2S_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

/* 句柄空间一般使用静态空间 */
static csi_i2s_t g_i2s;
#define I2S_TX_BUF_SIZE 256
uint8_t *i2s_tx_buf;    //< 定义接收的缓冲区
static ringbuffer_t tx_ring_buffer;     //< 定义环形缓冲区

volatile uint8_t cb_i2s_transfer_flag = 0;
static void i2s_event_cb_fun(struct _csi_i2s *i2s, csi_i2s_event_t event, void *arg){
    if (event == I2S_EVENT_TRANSMIT_COMPLETE) {
        cb_i2s_transfer_flag = 1;
    }
}

int main(void)
{
    csi_error_t ret;
    csi_i2s_format_t i2s_format;
    i2s_tx_buf = (uint8_t *)0x02001000;
    /* init函数的idx参数，请根据soc的实际情况进行选择 */
    ret = csi_i2s_init(&g_i2s, 0);
    if (ret != CSI_OK) {
        return -1;
    }
    csi_i2s_attach_callback(&g_i2s, i2s_event_cb_fun, NULL);  //< 注册回调函数
    
    i2s_format.mode = I2S_MODE_MASTER;                  //< 设置I2S为主机模式
    i2s_format.protocol = I2S_PROTOCOL_I2S;             //< 设置为I2S协议
    i2s_format.width = I2S_SAMPLE_WIDTH_16BIT;          //< 设置采样宽度为16bits
    i2s_format.rate = I2S_SAMPLE_RATE_48000;            //< 设置采样比率为48K
    i2s_format.polarity = I2S_LEFT_POLARITY_LOW;        //< 设置高电平代表左声道
    csi_i2s_format(&g_i2s, &i2s_format);                //< 调用该接口进行设置I2S
    
    for(int i = 0; i < I2S_TX_BUF_SIZE; i++)
    {
        i2s_tx_buf[i] = i;
    }
    csi_i2s_send(&g_i2s, i2s_tx_buf, I2S_TX_BUF_SIZE);
    csi_i2s_send_async(&g_i2s, i2s_tx_buf, I2S_TX_BUF_SIZE);
    
    while(1)
    {
        if(cb_i2s_transfer_flag != 0)
        {
            cb_i2s_transfer_flag = 0;
            printf("csi_i2s_send async ok\n");
        }
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_I2S_ */
#endif /* BSP_DEBUG */