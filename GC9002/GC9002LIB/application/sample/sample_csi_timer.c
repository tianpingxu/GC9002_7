#include "sample_def.h"
#include "csi_gpio_pin.h"
#include "csi_timer.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_TIMER_

#define CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);



static csi_timer_t g_timer;
static uint8_t cb_timer_flag = 0;
#define TEST_GPIOX      PB6
static csi_gpio_pin_t pin;

static void timer_event_cb_reload_fun(struct _csi_timer *timer, void *arg){
    (void)arg;
    //__HAL_TIM_GET_COUNTER((TIM_HandleTypeDef *)(timer->priv));
    cb_timer_flag ++;
    if(cb_timer_flag == 5)
    {
        ((TIM_HandleTypeDef *)(timer->priv))->Instance->ARR = 1000;
    }
    if(cb_timer_flag < 10)
    {
        csi_gpio_pin_toggle(&pin);
    }
}

int main(void)
{
    csi_error_t ret = 0;
    ret = csi_gpio_pin_init(&pin,  TEST_GPIOX);
    CHECK_RETURN(ret);
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_OPEN_DRAIN);
    CHECK_RETURN(ret);

    /* STEP 1：init timer 0 */
    ret = csi_timer_init(&g_timer, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_timer_attach_callback(&g_timer, timer_event_cb_reload_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3：start timer 0 */
    // ret = csi_timer_start(&g_timer, 1000000);
    ret = csi_timer_start_autoreload(&g_timer, 1000000);
    CHECK_RETURN(ret);

    /* STEP 4：clear flag and delay */
    cb_timer_flag = 0;
    // mdelay(11000);

    /* STEP 5：judge whether the system enters the interrupt function */
    if (0 == cb_timer_flag) {
        ret = CSI_ERROR;
    }
    while(1)
    {
        mdelay(100);
    }
    /* STEP 6：stop timer 0 */
    csi_timer_stop(&g_timer);

    /* STEP 7：cancel timer */
    csi_timer_detach_callback(&g_timer);

    /* STEP 8：uninit timer */
    csi_timer_uninit(&g_timer);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_TIMER_ */
#endif /* BSP_DEBUG */