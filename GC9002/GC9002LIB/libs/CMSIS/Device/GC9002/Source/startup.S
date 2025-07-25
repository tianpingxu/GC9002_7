/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

 /******************************************************************************
 * @file     startup.S
 * @brief    startup file. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     29. July 2019
 ******************************************************************************/

#include <csi_config.h>

.section .vectors, "aw", @progbits
    .align  6
    .globl  __Vectors
    .type   __Vectors, @object
__Vectors:
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   tspend_handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   CORET_IRQHandler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler

    /* External interrupts */
	.long   CANFD_IRQHandler 
	.long   UART0_IRQHandler 
	.long   UART1_IRQHandler 		
	.long   UART2_IRQHandler 		
	.long   TIM0_IRQHandler			
	.long   TIM1_IRQHandler			
	.long   SPI0_IRQHandler			
	.long	I2C0_IRQHandler 		  
	.long	I2C1_IRQHandler 		  
	.long	SPI1_IRQHandler 		  
	.long	I2S_IRQHandler			  
	.long	GPIOA_IRQHandler		  
	.long	GPIOB_IRQHandler		  
	.long	SPI2_IRQHandler 		  
	.long	ADC_IRQHandler			  
	.long	WDG_IRQHandler			  
	.long	RTC_IRQHandler			  
	.long	CQDPI_IRQHandler		  
	.long	VIDEO_CAP_IRQHandler 	  
	.long	DMA_IRQHandler			  
	.long	OTG_IRQHandler			  
	.long	AHMI_IRQHandler 		  
	.long	DISPLAY_IRQHandler 		  
	.long	CRC32_IRQHandler		  
	.long	MOTOR_IRQHandler		  
	.long	MAC_IRQHandler			  
	.long	EXTENSION_IRQHandler	  
	.long	DQSPI_IRQHandler					
	.long	Default_Handler 			
	.long	Default_Handler 			
	.long	Default_Handler

    .size   __Vectors, . - __Vectors

    .text
    .align  2
_start:
    .text
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
.option push
.option norelax
    la      gp, __global_pointer$
.option pop
    la      a0, unified_irq_exc_entry    //Default_Handler
    ori     a0, a0, 3
    csrw    mtvec, a0
	csrc	mstatus, 8
	
    la      a0, __Vectors
    csrw    mtvt, a0

    la      sp, g_top_irqstack
    csrw    mscratch, sp

#if(defined CONFIG_BOOT_DCACHE_DISABLE && CONFIG_BOOT_DCACHE_DISABLE == 1U)
	/* Disable Dchache */
	lui		a5, 0x40102
	li		a4, 1
	sw		a4,	0(a5)
	
	lui		a5, 0x40200
	addi	a5, a5,48
	li		a4, 0
	sw		a4,	0(a5)
#else
	/* Enable Dchache */
	lui		a5, 0x40200
	addi	a5, a5,48
	li		a4, 1
	sw		a4,	0(a5)
	
	lui		a5, 0x40102
	li		a4, 0
	sw		a4,	0(a5)
#endif

    /* Load data_in section */
    la      a0, __erodata_in
    la      a1, __data_in_start__
    la      a2, __data_in_end__
    bgeu    a1, a2, 2f
1:
    lw      t0, (a0)
    sw      t0, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b
2:

    /* Clear bss_in section */
    la      a0, __bss_in_start__
    la      a1, __bss_in_end__
    bgeu    a0, a1, 2f
1:
    sw      zero, (a0)
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:

//	jal		init_ldo_and_hyper

    /* Load data_in section */
    la      a0, __erodata_ex
    la      a1, __data_ex_start__
    la      a2, __data_ex_end__
    bgeu    a1, a2, 2f
1:
    lw      t0, (a0)
    sw      t0, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b
2:

    /* Clear bss_in section */
    la      a0, __bss_ex_start__
    la      a1, __bss_ex_end__
    bgeu    a0, a1, 2f
1:
    sw      zero, (a0)
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:

    /* Load flash init section */
    la      a0, __e_f_text
    la      a1, __f_code_start
    la      a2, __f_code_end
    bgeu    a1, a2, 2f
1:
    lw      t0, (a0)
    sw      t0, (a1)
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b
2:

#if(defined __SYSTEM_INIT && __SYSTEM_INIT == 1U)
    jal     SystemInit
#endif

#if(defined __BOARD_INIT && __BOARD_INIT == 1U)
    jal     hw_board_init
#endif

    jal     __main

    .size   Reset_Handler, . - Reset_Handler

__exit:
    j      __exit

.section .bss

    .align  3
    .global g_base_irqstack
    .global g_top_irqstack
g_base_irqstack:
    .space CONFIG_ARCH_INTERRUPTSTACK
g_top_irqstack:
