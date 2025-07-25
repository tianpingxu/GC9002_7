#include "sample_def.h"
#include "csi_pin.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_PIN_

/*
 * 测试条件：
 * 
 *          测试前需要 注释掉board.c中HAL_UART_MspInit接口对UART1的GPIO配置代码段
 * 
 * */
static UART_HandleTypeDef huart = {.Instance = UART1, };

#define CHECK_RETURN(ret)                      \
    do {                                        \
        if (ret != CSI_OK) {                    \
            while(1);                           \
        }                                       \
    } while(0);
    
static void hw_uart_init(void)
{
    /* config the UART */
    HAL_UART_StructInit(&(huart.Init));
    HAL_UART_Init(&huart);
}

int main(void)
{
    csi_error_t  ret;
    ret = csi_pin_set_mux(PB6, PB6_UART1_RX);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mux(PB7, PB7_UART1_TX);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mode(PB6, GPIO_MODE_PUSH_PULL);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mode(PB7, GPIO_MODE_PUSH_PULL);
    CHECK_RETURN(ret);
    ret = csi_pin_set_speed(PB6, PIN_SPEED_LV0);
    CHECK_RETURN(ret);
    ret = csi_pin_set_speed(PB7, PIN_SPEED_LV0);
    CHECK_RETURN(ret);
    ret = csi_pin_set_drive(PB6, PIN_DRIVE_LV0);
    CHECK_RETURN(ret);
    ret = csi_pin_set_drive(PB7, PIN_DRIVE_LV0);
    CHECK_RETURN(ret);
    
    hw_uart_init();
    while(1)
    {
        HAL_UART_SendData(UART1, 'H');
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_PIN_ */
#endif /* BSP_DEBUG */