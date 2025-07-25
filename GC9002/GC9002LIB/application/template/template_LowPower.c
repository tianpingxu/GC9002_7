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

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_LOWPOWER_
static k_task_handle_t g_uwTask1;
static k_task_handle_t g_uwTask3;

#define TASK_STK_SIZE     1024
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12

#define TIME_QUANTA 100
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

static void task3(void *p)
{
    uint32_t uxSavedInterruptStatus;
    struct lowpower_conf conf[2];
    for(;;) {
        uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
        printf("task3 ........\n");
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );
        csi_kernel_delay(pdMS_TO_TICKS(2000));
        // wake up pin
        conf[0].port = GPIOB;
        conf[0].pin = GPIO_PIN_5;
        conf[0].mode = GPIO_MODE_INPUT | GPIO_MODE_IT_FALLING;
        conf[0].lvl = 0;// wake up pin, this parameter can be ignored
        // other pin
        conf[1].port = GPIOB;
        conf[1].pin = GPIO_PIN_6;
        conf[1].mode = GPIO_MODE_OUTPUT_PP;
        conf[1].lvl = GPIO_PIN_RESET;
        HAL_RCC_SysLPEnter(conf, 2);
    }
}


int main(void)
{	
    printf("Hello FreeRTOS.\n");
	csi_kernel_init();
	
	csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TASK_PRIOR_TASK1, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask1);
	if (g_uwTask1 == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task1! \r\n");
	}
	
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
#endif
