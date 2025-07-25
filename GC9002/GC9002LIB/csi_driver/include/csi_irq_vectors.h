/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     csi_irq_vectors.h
 * @brief    header File for IRQ Vectors Driver
 * @version  V1.0
 * @date     2023/02/16
 * @model    irq vectors
 ******************************************************************************/
#ifndef __CSI_IRQ_VECTORS_H__
#define __CSI_IRQ_VECTORS_H__

#include <csi_common.h>

#ifdef __cplusplus
extern "C" {
#endif


#define IRQ_VECTORS_SIZE            CONFIG_VECTOR_TABLE_SIZE
#define CONFIG_NON_VECTOR_COUNT     IRQ_VECTORS_SIZE
/**
 * \brief      Vector Handler Function Typedef
 */
typedef void(*IRQ_Handler_Type)(void);

extern void Default_Handler(void);
extern void NMI_IRQHandler(void);


void csi_relocate_vector_table(void);
void csi_irq_vectors_init(void);
IRQ_Handler_Type csi_irq_get_default(uint32_t irq_num);


#define  ATTRIBUTE_ISR __attribute__ ((interrupt ("machine")))

#ifndef CONFIG_KERNEL_NONE
#include "csi_kernel.h"
#include "FreeRTOS.h" 
#include "task.h"

/* Vector interrupt stack point switch. */
#define  CSI_VECTOR_IRQ_ENTER_SP_SWITCH()               \
do{                                                     \
    __ASM volatile("csrw    mscratch, sp");             \
    __ASM volatile("la      sp, g_top_irqstack");   \
}while (0)

#define  CSI_VECTOR_IRQ_EXIT_SP_SWITCH()                \
do{                                                     \
    __ASM volatile("csrr    sp, mscratch");             \
}while (0)

#define  CSI_INTRPT_ENTER()     csi_kernel_intrpt_enter()
#define  CSI_INTRPT_EXIT()      csi_kernel_intrpt_exit()
#else
#define  CSI_VECTOR_IRQ_ENTER_SP_SWITCH()
#define  CSI_VECTOR_IRQ_EXIT_SP_SWITCH()
#define  CSI_INTRPT_ENTER()
#define  CSI_INTRPT_EXIT()
#endif

extern volatile uint32_t g_sys_tick;

ATTRIBUTE_ISR void NMI_IRQHandler(void);
ATTRIBUTE_ISR void CORET_IRQHandler(void);

ATTRIBUTE_ISR void CANFD_IRQHandler(void);
ATTRIBUTE_ISR void UART0_IRQHandler(void);
ATTRIBUTE_ISR void UART1_IRQHandler(void);		
ATTRIBUTE_ISR void UART2_IRQHandler(void);		
ATTRIBUTE_ISR void TIM0_IRQHandler(void);		
ATTRIBUTE_ISR void TIM1_IRQHandler(void);		
ATTRIBUTE_ISR void SPI0_IRQHandler(void);		
ATTRIBUTE_ISR void I2C0_IRQHandler(void);	  
ATTRIBUTE_ISR void I2C1_IRQHandler(void);	  
ATTRIBUTE_ISR void SPI1_IRQHandler(void);	  
ATTRIBUTE_ISR void I2S_IRQHandler(void);		  
ATTRIBUTE_ISR void GPIOA_IRQHandler(void);	  
ATTRIBUTE_ISR void GPIOB_IRQHandler(void);	  
ATTRIBUTE_ISR void SPI2_IRQHandler(void);	  
ATTRIBUTE_ISR void ADC_IRQHandler(void);		  
ATTRIBUTE_ISR void WDG_IRQHandler(void);		  
ATTRIBUTE_ISR void RTC_IRQHandler(void);		  
ATTRIBUTE_ISR void CQDPI_IRQHandler(void);  
ATTRIBUTE_ISR void VIDEOCAP_IRQHandler(void); 
ATTRIBUTE_ISR void DMA_IRQHandler(void);		  
ATTRIBUTE_ISR void OTG_IRQHandler(void);		  
ATTRIBUTE_ISR void AHMI_IRQHandler(void);	  
ATTRIBUTE_ISR void DISP_IRQHandler(void);	  
ATTRIBUTE_ISR void CRC32_IRQHandler(void);	  
ATTRIBUTE_ISR void MOTOR_IRQHandler(void);	  
ATTRIBUTE_ISR void MAC_IRQHandler(void);	  
ATTRIBUTE_ISR void EXTENSION_IRQHandler(void);
ATTRIBUTE_ISR void DQSPI_IRQHandler(void);

#ifdef __cplusplus
}
#endif
#endif /* __CSI_IRQ_VECTORS_H__ */