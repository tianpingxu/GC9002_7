#include "sample_def.h"
#include "csi_gpio_pin.h"
#include "csi_rtc.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_RTC_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

#define TEST_GPIOX      PB12
static csi_gpio_pin_t pin;
static csi_rtc_t g_rtc;
static csi_rtc_time_t last_time, base_time, alarm_time;

static void rtc_event_cb_fun(struct _csi_rtc *rtc, void *arg){
    csi_gpio_pin_toggle(&pin);
    csi_rtc_get_time(&g_rtc, &last_time);
    printf("alarm Get: %d/%d/%d %d:%d:%d\n", last_time.tm_year, last_time.tm_mon+1, last_time.tm_mday, 
        last_time.tm_hour, last_time.tm_min, last_time.tm_sec);
    last_time.tm_sec += 10;
    if(last_time.tm_sec >= 60)
    {
        last_time.tm_min ++;
        if(last_time.tm_min >= 60)
        {
            last_time.tm_hour++;
            if(last_time.tm_hour >= 24)
            {
                last_time.tm_mday++;
                if(last_time.tm_mday >= 32)
                {
                    last_time.tm_mon++;
                    if(last_time.tm_mon >= 12)
                    {
                        last_time.tm_year++;
                    }
                }
            }
        }
    }
    last_time.tm_sec %= 60;
    last_time.tm_min %= 60;
    last_time.tm_hour %= 24;
    last_time.tm_mday %= 32;
    last_time.tm_mon %= 12;
    csi_rtc_set_alarm(&g_rtc, &last_time, rtc_event_cb_fun, NULL);
}

int main(void)
{
    csi_error_t ret;
    
    ret = csi_gpio_pin_init(&pin,  TEST_GPIOX);
    CHECK_RETURN(ret);
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_OPEN_DRAIN);
    CHECK_RETURN(ret);
    
    /* STEP 1: init rtc */
    ret = csi_rtc_init(&g_rtc, 0);
    CHECK_RETURN(ret);

    base_time.tm_year    = 120;
    base_time.tm_mon     = 0;
    base_time.tm_mday    = 5;
    base_time.tm_hour    = 23;
    base_time.tm_min     = 59;
    base_time.tm_sec     = 55;

    /* STEP 2: set base time */
    ret = csi_rtc_set_time(&g_rtc, &base_time);
    CHECK_RETURN(ret);

    /* STEP 3: delay 10s */
    mdelay(6000);

    /* STEP 4: read time */
    ret = csi_rtc_get_time(&g_rtc, &last_time);
    CHECK_RETURN(ret);

    /* STEP 5: check time(add 10 sec)   There is a certain error */
    printf("Set: %d/%d/%d %d:%d:%d\n", base_time.tm_year, base_time.tm_mon+1, base_time.tm_mday, 
        base_time.tm_hour, base_time.tm_min, base_time.tm_sec);
    printf("Get: %d/%d/%d %d:%d:%d\n", last_time.tm_year, last_time.tm_mon+1, last_time.tm_mday, 
        last_time.tm_hour, last_time.tm_min, last_time.tm_sec);
    
    
    alarm_time.tm_year    = 120;
    alarm_time.tm_mon     = 0;
    alarm_time.tm_mday    = 6;
    alarm_time.tm_hour    = 0;
    alarm_time.tm_min     = 0;
    alarm_time.tm_sec     = 8;
    /* STEP 6: set alarm time */
    ret = csi_rtc_set_alarm(&g_rtc, &alarm_time, rtc_event_cb_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 6: uinit rtc */
//    csi_rtc_uninit(&g_rtc);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_RTC_ */
#endif /* BSP_DEBUG */