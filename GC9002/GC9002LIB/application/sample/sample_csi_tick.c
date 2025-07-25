#include "sample_def.h"
#include "csi_tick.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_TICK_

#define TICK_CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);
        
int main(void)
{
    csi_error_t ret;
    uint64_t tick;
    uint64_t time_s;
    uint64_t time_ms;
    uint64_t time_us;

    /* init tick， CONFIG_SYSTICK_HZ is 100 */
    ret = csi_tick_init();
    TICK_CHECK_RETURN(ret);

    while(1)
    {
        /* get tick */
        tick = csi_tick_get();
        printf("The tick is 0x%llx\n", tick);

        /* get time (s) */
        time_s = csi_tick_get_s();
        printf("The time is %llu s\n", time_s);

        /* get time (ms) */
        time_ms = csi_tick_get_ms();
        printf("The time is %llu ms\n", time_ms);

        /* get time (us) */
        time_us = csi_tick_get_us();
        printf("The time is %llu us\n\n", time_us);
		mdelay(1000);
    }

    /* uninit tick */
    csi_tick_uninit();
    while(1)
    {
		mdelay(0xEFFFFFF);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_TICK_ */
#endif /* BSP_DEBUG */