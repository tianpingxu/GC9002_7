#include "sample_def.h"
#include "csi_pin.h"
#include "csi_pwm.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_MOTOR_TIMER_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

static csi_pwm_t g_motor_pwm;
static csi_motor_t g_motor;
void test_motor_timer_callback(struct _csi_motor *motor, csi_motor_event_t event, void *arg)
{
    switch (event)
    {
    case MOTOR_EVENT_DONE:
        break;
    case MOTOR_EVENT_WARNING:
	    *((volatile unsigned int *)(GPIOA_BASE+4*0)) = (*((volatile unsigned int *)(GPIOA_BASE+4*0)) & 0x1) ^ 1;
        break;
    case MOTOR_EVENT_DIV_BY_0:
        break;
    case MOTOR_EVENT_TIME_END:
        break;
    default:
        break;
    }
}
int main(void)
{
    csi_error_t ret = 0;
    printf("Test CSI MOTOR TIMER\n");
//    ret = csi_pin_set_mux(PA1, PA1_MOTOR_PWM1);
    CHECK_RETURN(ret);
    ret = csi_motor_init(&g_motor, MOTOR_CHANNEL1);
    CHECK_RETURN(ret);
    ret = csi_motor_timer_attach_callback(&g_motor, test_motor_timer_callback, NULL);
    CHECK_RETURN(ret);
    // ret = csi_motor_timer_start(&g_motor, 1000);
    ret = csi_motor_timer_start_autoreload(&g_motor, 1000);//us
    CHECK_RETURN(ret);
    while(1)
    {
        printf("Hello\n");
		mdelay(2000);
    }
    return 0;
}
#endif /* _SAMPLE_CSI_MOTOR_TIMER_ */
#endif /* BSP_DEBUG */