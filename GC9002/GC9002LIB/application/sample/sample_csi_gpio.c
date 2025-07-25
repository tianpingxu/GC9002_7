#include "sample_def.h"
#include "csi_gpio_pin.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_GPIO_
#define GPIO_CHECK_RETURN(ret)           \
    do {                                 \
        if (ret != CSI_OK) {             \
            while(1);                    \
        }                                \
    } while(0);

#define TEST_NORMAL     0
#define TEST_TRQ        1
#define TEST_MODE       TEST_TRQ
#define TEST_GPIO_IDX   1
#define TEST_GPIOX      CSI_GET_PIN(6)

bool intr_flag = false;
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_GPIOX_EXTI_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_GPIOX_EXTI_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    intr_flag = true;
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static csi_gpio_t gpio;
int main(void)
{
    csi_error_t ret;
    /* Initialize GPIO periphrial */
    ret = csi_gpio_init(&gpio,  TEST_GPIO_IDX);
    GPIO_CHECK_RETURN(ret);
#if(TEST_MODE == TEST_NORMAL)
    /* Set input mode */
    csi_gpio_mode(&gpio, TEST_GPIOX, GPIO_MODE_PUSH_PULL);
    
    csi_gpio_write(&gpio, TEST_GPIOX, GPIO_PIN_LOW);

    mdelay(100);
    while(1)
    {
        csi_gpio_toggle(&gpio, TEST_GPIOX);
		mdelay(100);
    }
#else
    /* Set input mode */
    csi_gpio_mode(&gpio, TEST_GPIOX, GPIO_MODE_PULLUP);
    csi_gpio_irq_mode(&gpio, TEST_GPIOX, GPIO_IRQ_MODE_FALLING_EDGE);
    // attention: this func is irq handler, is not callback
    csi_gpio_attach_callback(&gpio, CSI_GPIOX_EXTI_IRQHandler, NULL);
    csi_gpio_irq_enable(&gpio, TEST_GPIOX, true);
    while(1)
    {
		mdelay(100);
    }
#endif
    return 0;
}

#endif /* _SAMPLE_CSI_GPIO_ */
#endif /* BSP_DEBUG */