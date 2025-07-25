#include "sample_def.h"
#include "csi_gpio_pin.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_GPIO_PIN_

#define TEST_NORMAL     0
#define TEST_TRQ        1
#define TEST_MODE       TEST_TRQ
#define TEST_GPIOX      PB12

bool intr_flag = false;
static void gpio_interrupt_handler(void *arg)
{
    intr_flag = true;
}
static csi_gpio_pin_t pin;
int main(void)
{
    csi_error_t ret = CSI_OK;
    /* Initialize pin periphrial */
    ret = csi_gpio_pin_init(&pin,  TEST_GPIOX);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
#if(TEST_MODE == TEST_NORMAL)
    /* Set input mode */
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_OPEN_DRAIN);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
    
    csi_gpio_pin_write(&pin, GPIO_PIN_LOW);
    mdelay(100);
    while(1)
    {
        csi_gpio_pin_toggle(&pin);
		mdelay(100);
    }
#else
    /* Set input mode */
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_PULLUP);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
    ret = csi_gpio_pin_irq_mode(&pin, GPIO_IRQ_MODE_FALLING_EDGE);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
    ret = csi_gpio_pin_attach_callback(&pin, gpio_interrupt_handler, NULL);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
    ret = csi_gpio_pin_irq_enable(&pin, true);
    if(ret == CSI_ERROR)
    {
        while(1);
    }
    kernel_start_in_debug();
    while(1)
    {
		mdelay(100);
    }
#endif
    return 0;
}

#endif /* _SAMPLE_CSI_GPIO_PIN_ */
#endif /* BSP_DEBUG */