#include <csi_irq_vectors.h>


static IRQ_Handler_Type s_irqvector[IRQ_VECTORS_SIZE];
static IRQ_Handler_Type s_nmivector;
extern uint32_t __Vectors[];
static uint32_t _SRAM_VECTOR_TABLE[IRQ_VECTORS_SIZE];
/**
  \brief       Interrupt vector table redirection.
  \return      None.
*/
void csi_relocate_vector_table(void)
{
    // 将 __Vectors 处的初始中断向量表拷贝到新地址 _SRAM_VECTOR_TABLE
   memcpy((void *)_SRAM_VECTOR_TABLE, (void *)__Vectors, IRQ_VECTORS_SIZE*4);
    // 将 MTVT 指向 _SRAM_VECTOR_TABLE
   __set_MTVT((uint32_t)_SRAM_VECTOR_TABLE);
}

/**
  \brief       irq handler vectors init.
  \return      None.
*/
void csi_irq_vectors_init(void)
{
    int i;

    for (i = 0; i < IRQ_VECTORS_SIZE; i++) {
        s_irqvector[i] = (IRQ_Handler_Type)csi_vic_get_vector(i);
    }

    s_irqvector[CORET_IRQn] = CORET_IRQHandler;
    s_nmivector = NMI_IRQHandler;
}

/**
  \brief       Get irq default handler.
  \param[in]   irq_num Number of IRQ.
  \return      irq default handler.
*/
IRQ_Handler_Type csi_irq_get_default(uint32_t irq_num)
{
    if(irq_num < IRQ_VECTORS_SIZE)
    {
        return s_irqvector[irq_num];
    }
    else
    {
        return 0;
    }
}


#include <csi_config.h>
#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
volatile uint32_t g_sys_tick = 0;

__WEAK ATTRIBUTE_ISR void NMI_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();
    
    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void CORET_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    extern void csi_tick_config(void);
    csi_tick_config();
    g_sys_tick++;

#if defined(CONFIG_KERNEL_RHINO)
    systick_handler();
#elif defined(CONFIG_KERNEL_FREERTOS)
    extern void xPortSysTickHandler(void);
    xPortSysTickHandler();
#elif defined(CONFIG_KERNEL_UCOS)
    OSTimeTick();
#endif

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}

__WEAK ATTRIBUTE_ISR void CANFD_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();
    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void UART0_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void UART1_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();
    
    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void UART2_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void TIM0_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void TIM1_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void SPI0_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void I2C0_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void I2C1_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void SPI1_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void I2S_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void GPIOA_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void GPIOB_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void SPI2_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void ADC_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void WDG_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void RTC_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void CQDPI_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void VIDEO_CAP_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void DMA_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void OTG_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void AHMI_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void DISPLAY_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();
    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void CRC32_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void MOTOR_IRQHandler(void)
{ 
	CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER(); 
	// ck_motor_intr_handle(); 
	CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void MAC_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void EXTENSION_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
__WEAK ATTRIBUTE_ISR void DQSPI_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    CSI_INTRPT_ENTER();
    
    CSI_INTRPT_EXIT();
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}