/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     trap_c.c
 * @brief    source file for the trap process
 * @version  V1.0
 * @date     12. December 2017
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <csi_config.h>
#include <csi_core.h>

void (*trap_c_callback)(void);

void trap_c(uint32_t *regs)
{
    int i;
    uint32_t vec = 0;
    uint32_t mcause = 0;

    mcause = __get_MCAUSE();
    vec = mcause & 0x3FF;
	
	HAL_RCC_SystemReset();

    while (1)
    {
        printf("CPU Exception: NO.%u\n", vec);
        printf("mcause: 0x%08x\n", mcause);

        for (i = 0; i < 15; i++) {
            printf("x%d: %08x\t", i + 1, regs[i]);

            if ((i % 4) == 3) {
                printf("\n");
            }
        }

        printf("\n");
        printf("mepc   : 0x%08x\n", regs[15]);
        printf("mtval  : 0x%08x\n", __get_MTVAL());
        printf("mstatus: 0x%08x\n", regs[16]);
        printf("\n\n");

        if (trap_c_callback) {
            trap_c_callback();
        }
        for (i = 0; i < 20000000; i++) {
            __NOP();__NOP();__NOP();__NOP();__NOP();
        }
    }
}

