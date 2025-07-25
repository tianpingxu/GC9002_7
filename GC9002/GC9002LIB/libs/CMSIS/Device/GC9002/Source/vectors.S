/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
 /******************************************************************************
 * @file     vectors.S
 * @brief    define default vector handlers. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     28. Nove 2017
 ******************************************************************************/
#include <csi_config.h>

.section .bss
    .align  2
//    .globl  g_trapstackalloc
//    .global g_trapstackbase
    .global g_top_trapstack
//g_trapstackalloc:
//g_trapstackbase:
//    .space 0
g_top_trapstack:

    .align 2
    .globl g_trap_sp
    .type  g_trap_sp, object
g_trap_sp:
    .long 0
    .size g_trap_sp, .-g_trap_sp

/******************************************************************************
 * Functions:
 *     void trap(void);
 * default exception handler
 ******************************************************************************/
.text

    .align  2
    .global trap
    .type   trap, %function
trap:
    la      t0, g_trap_sp
    addi    t0, t0, -68
    sw      x1, 0(t0)
    sw      x2, 4(t0)
    sw      x3, 8(t0)
    sw      x4, 12(t0)
    sw      x6, 20(t0)
    sw      x7, 24(t0)
    sw      x8, 28(t0)
    sw      x9, 32(t0)
    sw      x10, 36(t0)
    sw      x11, 40(t0)
    sw      x12, 44(t0)
    sw      x13, 48(t0)
    sw      x14, 52(t0)
    sw      x15, 56(t0)
    csrr    a0, mepc
    sw      a0, 60(t0)
    csrr    a0, mstatus
    sw      a0, 64(t0)

    mv      a0, t0
    lw      t0, -4(sp)
    mv      sp, a0
    sw      t0, 16(sp)
    
    jal     trap_c


	.globl __continue_label
	.section ".text"
	.align 2
	.type __continue_label, %function
__continue_label:
    mv a0, a0
    ret

	.globl __bkpt_label
	.section ".text"
	.align 2
	.type __bkpt_label, %function
__bkpt_label:
    ebreak
    ret


.macro PUSH_CONTEXT
    addi sp,sp,-68
    sw x1, 0(sp)
    sw x3, 4(sp)
    sw x4, 8(sp)
    sw x5, 12(sp)
    sw x6, 16(sp)
    sw x7, 20(sp)
    sw x8, 24(sp)
    sw x9, 28(sp)
    sw x10, 32(sp)
    sw x11, 36(sp)
    sw x12, 40(sp)
    sw x13, 44(sp)
    sw x14, 48(sp)
    sw x15, 52(sp)
.endm
.macro POP_CONTEXT
    lw x1, 0(sp)
    lw x3, 4(sp)
    lw x4, 8(sp)
    lw x5, 12(sp)
    lw x6, 16(sp)
    lw x7, 20(sp)
    lw x8, 24(sp)
    lw x9, 28(sp)
    lw x10, 32(sp)
    lw x11, 36(sp)
    lw x12, 40(sp)
    lw x13, 44(sp)
    lw x14, 48(sp)
    lw x15, 52(sp)
    addi sp,sp, 68
.endm
.macro PUSH_CSR
    csrr t0, mcause
    sw t0, 56(sp)
    csrr t0, mepc
    sw t0, 60(sp)
    csrr t0, mstatus
    sw t0, 64(sp)
    addi s0, sp, 68
.endm
.macro POP_CSR
    lw t0, 56(sp)
    csrw mcause, t0
    lw t0, 60(sp)
    csrw mepc, t0
    lw t0, 64(sp)
    csrw mstatus, t0
.endm


/******************************************************************************
 * Functions:
 *     void unified_irq_exc_entry(void);
 * unified irq and exc handler
 ******************************************************************************/
.align 6
.global unified_irq_exc_entry
.weak unified_irq_exc_entry
unified_irq_exc_entry:
#ifndef CONFIG_KERNEL_NONE// freertos enabled
    csrw    mscratch, sp
    la      sp, g_top_irqstack
#endif
    PUSH_CONTEXT
    PUSH_CSR
    
    csrr t0, mcause
    srli t1, t0, 31
    mv a0, sp
    beq t1, x0, trap_c
    
    csrr a0, mcause
    mv a1, sp
    call non_vector_unified_handler
    // 没有ipush/ipop指令, 不支持中断咬尾
    
    nop
    POP_CSR
    POP_CONTEXT
#ifndef CONFIG_KERNEL_NONE// freertos enabled
    csrr    sp, mscratch
#endif
    mret
    

/******************************************************************************
 * Functions:
 *     void Default_Handler(void);
 * default exception handler
 ******************************************************************************/
    .align  6
    .weak   Default_Handler
    .global Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    j      trap
    .size   Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro  def_irq_handler handler_name
    .weak   \handler_name
    .globl  \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler tspend_handler
    def_irq_handler CORET_IRQHandler

	def_irq_handler CANFD_IRQHandler 
	def_irq_handler UART0_IRQHandler 
	def_irq_handler UART1_IRQHandler 		
	def_irq_handler UART2_IRQHandler 		
	def_irq_handler TIM0_IRQHandler			
	def_irq_handler TIM1_IRQHandler			
	def_irq_handler SPI0_IRQHandler			
	def_irq_handler	I2C0_IRQHandler 		  
	def_irq_handler	I2C1_IRQHandler 		  
	def_irq_handler	SPI1_IRQHandler 		  
	def_irq_handler	I2S_IRQHandler			  
	def_irq_handler	GPIOA_IRQHandler		  
	def_irq_handler	GPIOB_IRQHandler		  
	def_irq_handler	SPI2_IRQHandler 		  
	def_irq_handler	ADC_IRQHandler			  
	def_irq_handler	WDG_IRQHandler			  
	def_irq_handler	RTC_IRQHandler			  
	def_irq_handler	CQDPI_IRQHandler		  
	def_irq_handler	Video_Cap_HandlerISR 	  
	def_irq_handler	DMA_IRQHandler			  
	def_irq_handler	OTG_IRQHandler			  
	def_irq_handler	AHMI_HandlerISR 		  
	def_irq_handler	DISPLAY_HandlerISR 		  
	def_irq_handler	CRC32_IRQHandler		  
	def_irq_handler	MOTOR_IRQHandler		  
	def_irq_handler	MAC_IRQHandler			  
	def_irq_handler	EXTENSION_IRQHandler	  
	def_irq_handler	DQSPI_IRQHandler


/******************************************************************************
 * Functions:
 *     void set_sp(uint32_t sp);
 * default exception handler
 ******************************************************************************/
.text
    .align  2
    .global set_sp
    .type   set_sp, %function
set_sp:
    mv	sp,a0
    ret