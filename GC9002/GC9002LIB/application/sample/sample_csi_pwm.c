#include "sample_def.h"
#include "csi_pin.h"
#include "csi_pwm.h"
#include "csi_irq.h"
#include "soc.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_PWM_

#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);



#define EXAMPLE_PWM_CH_IDX                  2U
#define EXAMPLE_PWM_CAPTURE_CH_IDX          2U
static csi_pwm_t g_pwm0, g_pwm1;
static uint32_t cb_pwm_time = 0;
static csi_pwm_event_t cb_pwm_flag = PWM_EVENT_ERROR;


/* Captured Values */
uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;
/* Capture index */
uint16_t               uhCaptureIndex = 0;
/* Frequency Value */
uint32_t               uwFrequency = 0;

static unsigned int cnt = 0;
static void  pwm0_event_cb_fun(struct _csi_pwm *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg)
{

}
static void  pwm_event_cb_fun(struct _csi_pwm *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg)
{
    switch (event) {
        case PWM_EVENT_CAPTURE_POSEDGE:
        {
            cb_pwm_flag = PWM_EVENT_CAPTURE_POSEDGE;
            cb_pwm_time = time_us;
            
            if(uhCaptureIndex == 0)
            {
                /* Get the 1st Input Capture value */
                uwIC2Value1 = time_us;
                uhCaptureIndex = 1;
            }
            else if(uhCaptureIndex == 1)
            {
                /* Get the 2nd Input Capture value */
                uwIC2Value2 = time_us; 

                /* Capture computation */
                if (uwIC2Value2 > uwIC2Value1)
                {
                    uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
                }
                else  /* (uwIC2Value2 <= uwIC2Value1) */
                {
                    uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2); 
                }
                /* Frequency computation: for this example TIMx (TIM0/1) is clocked by 2*APBclk */      
                uwFrequency = (1000000 / uwDiffCapture);
                cnt++;
                if(cnt%10000 == 0)
                {
                    printf("Freq: %u Hz\n", uwFrequency);
                }
                uhCaptureIndex = 0;
            } 
        }break;
        case PWM_EVENT_CAPTURE_NEGEDGE:
            cb_pwm_flag = PWM_EVENT_CAPTURE_NEGEDGE;
            cb_pwm_time = time_us;
            break;
        case PWM_EVENT_CAPTURE_BOTHEDGE:
            cb_pwm_flag = PWM_EVENT_CAPTURE_BOTHEDGE;
            cb_pwm_time = time_us;
            break;
        default:
            break;
    }
}

int main(void)
{
    csi_error_t ret = 0;
    printf("Test CSI PWM\n");
    /* STEP 1: pwm port */
    ret = csi_pin_set_mux(PB5, PB5_TIM0_2);
    CHECK_RETURN(ret);
    ret = csi_pin_set_mux(PB9, PB9_TIM1_2);
    CHECK_RETURN(ret);

    /* STEP 2: pwm init */
    ret = csi_pwm_init(&g_pwm0, 0);
    CHECK_RETURN(ret);
    ret = csi_pwm_init(&g_pwm1, 1);
    CHECK_RETURN(ret);

    /** 
     * STEP 3: pwm channel 0 configure operating mode and signal period 
     * period time: 30us
     * low level time: 10us
     */
    ret = csi_pwm_out_config(&g_pwm0, EXAMPLE_PWM_CH_IDX, 30, 20, PWM_POLARITY_HIGH);
    CHECK_RETURN(ret);

    /* STEP 4: pwm channel 0 start ouput */
    ret = csi_pwm_attach_callback(&g_pwm0, pwm0_event_cb_fun, NULL);
    ret = csi_pwm_out_start(&g_pwm0, EXAMPLE_PWM_CH_IDX);
    CHECK_RETURN(ret);
    // while(1);
    /* STEP 5: register callback func */
    ret = csi_pwm_attach_callback(&g_pwm1, pwm_event_cb_fun, NULL);
    CHECK_RETURN(ret);
    
    /** 
     * STEP 6: config channel 0 to input capture
     * posedge trigger 10 times into callback func
    */
    ret = csi_pwm_capture_config(&g_pwm1, EXAMPLE_PWM_CAPTURE_CH_IDX, PWM_CAPTURE_POLARITY_POSEDGE, 1000000, 10);
    CHECK_RETURN(ret);

    /* STEP 7: start capture channel 0 */
    ret = csi_pwm_capture_start(&g_pwm1, EXAMPLE_PWM_CAPTURE_CH_IDX);
    CHECK_RETURN(ret);
    
//    /* STEP 8: pwm channel 2 stop output */
//    csi_pwm_out_stop(&g_pwm0, EXAMPLE_PWM_CH_IDX);
//    csi_pwm_capture_stop(&g_pwm1, EXAMPLE_PWM_CAPTURE_CH_IDX);
//
//    /* STEP 9: pwm uninit */
//    csi_pwm_uninit(&g_pwm0);
//    csi_pwm_uninit(&g_pwm1);
//
//    return ret;
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_PWM_ */
#endif /* BSP_DEBUG */