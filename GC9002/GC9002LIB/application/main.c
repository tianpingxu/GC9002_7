/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     main.c
 * @brief    hello world
 * @version  V1.0
 * @date     17. Jan 2018
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "csi_motor.h"
#include "core_rv32.h"
#include "template_def.h"
#include "main.h"
#include "csi_config.h"
#include "csi_kernel.h"
#include "FreeRTOS.h" 
#include "task.h"
#include "csi_gpio_pin.h"
#include "csi_timer.h"




k_task_handle_t g_uwTask1;
k_task_handle_t g_uwTask2;
k_task_handle_t g_uwTask3;

#define TASK_STK_SIZE     1024
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12

#define TIME_QUANTA 100
void test(void);
#define CHECK_RETURN(ret)                      \
    do {                                        \
        if (ret != CSI_OK) {                    \
            while(1);                           \
        }                                       \
    } while(0);

static void task1(void *p)
{
    uint32_t uxSavedInterruptStatus;
    for(;;) {
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        printf("task1 ........\n");
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
		csi_kernel_delay(pdMS_TO_TICKS(2000));
    }
}

static csi_timer_t g_timer;
static uint8_t cb_timer_flag = 0;
#define TEST_GPIOX      PB12
static csi_gpio_pin_t pin;

static void timer_event_cb_reload_fun(struct _csi_timer *timer, void *arg){
    (void)arg;
    csi_gpio_pin_toggle(&pin);
}
static void task2(void *p)
{
    csi_error_t ret = 0;
    ret = csi_gpio_pin_init(&pin,  TEST_GPIOX);
    CHECK_RETURN(ret);
    ret = csi_gpio_pin_mode(&pin, GPIO_MODE_PUSH_PULL);
    CHECK_RETURN(ret);

    /* STEP 1：init timer 0 */
    ret = csi_timer_init(&g_timer, 0);
    CHECK_RETURN(ret);

    /* STEP 2: register callback func */
    ret = csi_timer_attach_callback(&g_timer, timer_event_cb_reload_fun, NULL);
    CHECK_RETURN(ret);

    /* STEP 3：start timer 0 */
//    ret = csi_timer_start(&g_timer, 1000000);
    ret = csi_timer_start_autoreload(&g_timer, 100000);
    CHECK_RETURN(ret);
    for(;;) {
        printf("task2 ........\n");
        csi_kernel_delay(pdMS_TO_TICKS(5000));
    }
}
static void task3(void *p)
{
    uint32_t uxSavedInterruptStatus;
    for(;;) {
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        printf("task3 ........\n");
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
//        test();
        csi_kernel_delay(pdMS_TO_TICKS(2000));
    }
}


#if((defined BSP_DEBUG) && BSP_DEBUG == MAIN)
int main(void)
{	
    printf("Hello FreeRTOS.\n");
	csi_kernel_init();
	
	csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TASK_PRIOR_TASK1, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask1);
	if (g_uwTask1 == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task1! \r\n");
	}
	
	// csi_kernel_task_new((k_task_entry_t)task2, "task2", NULL, TASK_PRIOR_TASK2, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask2);
	// if (g_uwTask2 == NULL) {
	// 	csi_kernel_sched_resume(0);
	// 	printf("Fail to create task2! \r\n");
	// }
    
	csi_kernel_task_new((k_task_entry_t)task3, "task3", NULL, TASK_PRIOR_TASK3, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask3);
	if (g_uwTask3 == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task3! \r\n");
	}
	
	csi_kernel_start();

	
	while(1)
	{
		printf("Hello FreeRTOS.\n");
		HAL_Delay(100*1000);
	}
	return 0;
}
#endif

void kernel_start_in_debug(void)
{
#ifndef CONFIG_KERNEL_NONE
	csi_kernel_init();
	
	csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TASK_PRIOR_TASK1, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask1);
	if (g_uwTask1 == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task1! \r\n");
	}
	
	csi_kernel_task_new((k_task_entry_t)task2, "task2", NULL, TASK_PRIOR_TASK2, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask2);
	if (g_uwTask2 == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task2! \r\n");
	}
	
	csi_kernel_start();
#endif
}







#include "csi_irq.h"
#include "csi_irq_vectors.h"

static RTC_HandleTypeDef handle_rtc = {.Instance = RTC};
static RTC_TimeTypeDef time;
static RTC_AlarmTypeDef sAlarm;
static uint32_t count = 0;

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_RTC_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_RTC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    count++;
	HAL_RTC_AlarmIRQHandler(&handle_rtc);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void test(void)
{
    printf("Hello World\n");
    HAL_RTC_Struct_Init(&handle_rtc);
    
    handle_rtc.Init.date.value = 0x20001230;
    handle_rtc.Init.time.value = 0x00125958;
    handle_rtc.Init.format = RTC_FORMAT_BCD;
    HAL_RTC_Init(&handle_rtc);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(RTC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_RTC_IRQHandler);
#else
    csi_clic_register_irq(RTC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_RTC_IRQHandler);
#endif
	
    HAL_RTC_GetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
    
    sAlarm.time.value = 0x00110158;
    HAL_RTC_SetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
    
    sAlarm.time.value = 0x00110202;
    sAlarm.AlarmMask = RTC_ALARMMASK_ALL;//RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS;
    HAL_RTC_SetAlarm(&handle_rtc, &sAlarm, RTC_FORMAT_BCD);
    while(1)
    {
        printf("rtc irq: count = %u\n", count);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
    }
}