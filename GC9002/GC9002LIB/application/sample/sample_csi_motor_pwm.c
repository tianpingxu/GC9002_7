#include "sample_def.h"
#include "csi_pin.h"
#include "csi_pwm.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_MOTOR_PWM_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

static csi_pwm_t g_motor_pwm;
int main(void)
{
    csi_error_t ret = 0;
    printf("Test CSI PWM\n");
    /* STEP 1: pwm port */
    // ret = csi_pin_set_mux(PB6, PB6_TIM0_3);
    ret = csi_pin_set_mux(PB6, PB6_MOTOR_PWM2);
    CHECK_RETURN(ret);

    /* STEP 2: pwm init */
    // ret = csi_pwm_init(&g_motor_pwm, 0);
    ret = csi_pwm_init(&g_motor_pwm, CSI_MOTOR_PWM_IDX2);
    CHECK_RETURN(ret);

    /** 
     * STEP 3: pwm channel 0 configure operating mode and signal period 
     * period time: 30us
     * low level time: 10us
     */
    ret = csi_pwm_out_config(&g_motor_pwm, 3, 20, 20, PWM_POLARITY_HIGH);//PWM_POLARITY_HIGH
    CHECK_RETURN(ret);

    /* STEP 4: pwm channel 0 start ouput */
    ret = csi_pwm_out_start(&g_motor_pwm, 3);
    CHECK_RETURN(ret);
    while(1)
    {
        printf("Hello\n");
//		mdelay(100);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_MOTOR_PWM_ */
#endif /* BSP_DEBUG */