#include "sample_def.h"
#include "csi_gpio_pin.h"
#include "csi_wdt.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_WDT_

/**
 * The exact time-out can be calculated according to the formula
 * freq = 48000000 Hz
 * user changeable para is 10 (range is 0 ~ 15)
 * 994ms = ((0x10000 << 10)/ (48000000 / 1000))
 */
#define WDT_TIMEOUT_MS                  (1000U)               ///< timeout: 1.398s
#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);


static csi_wdt_t g_wdt;
static uint8_t cb_wdt_flag = 0;
#define TEST_GPIOX      PB12
static csi_gpio_pin_t pin;

static void wdt_event_cb_fun(struct _csi_wdt *wdt, void *arg){
    (void)wdt;
    (void)arg;
    csi_wdt_feed(&g_wdt);
    cb_wdt_flag = 1;
    csi_gpio_pin_toggle(&pin);
    printf("wdt feed\n");
}

int main(void)
{
    csi_error_t ret;
    printf("hello csi\n");
    ret = csi_gpio_pin_init(&pin,  TEST_GPIOX);
    CHECK_RETURN(ret);
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_PUSH_PULL);
    CHECK_RETURN(ret);

    /* STEP 1: init wdt */
    ret = csi_wdt_init(&g_wdt, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_wdt_attach_callback(&g_wdt, wdt_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3: set timeout time(994ms) */
    ret = csi_wdt_set_timeout(&g_wdt, WDT_TIMEOUT_MS);
    CHECK_RETURN(ret);

    cb_wdt_flag = 0;

    /* STEP 4: start work */
    ret = csi_wdt_start(&g_wdt);
    CHECK_RETURN(ret);
    csi_wdt_feed(&g_wdt);
    csi_gpio_pin_toggle(&pin);
    csi_gpio_pin_toggle(&pin);

    /* STEP 5: delay 993ms */
//    mdelay(WDT_TIMEOUT_MS - 1);

    /* STEP 6:  system should not enters the interrupt function */
    if (cb_wdt_flag) {
        ret = CSI_ERROR;
    }

    /* STEP 7:  feed wdt */
//    ret = csi_wdt_feed(&g_wdt);
    CHECK_RETURN(ret);
    
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_WDT_ */
#endif /* BSP_DEBUG */