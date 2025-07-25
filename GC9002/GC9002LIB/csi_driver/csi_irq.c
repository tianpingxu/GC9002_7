#include <csi_irq.h>

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static unsigned long NonVectorHandlers[CONFIG_NON_VECTOR_COUNT];
#endif
/**
 * \brief      System Default Exception Handler
 * \details
 * This function provided a default exception and NMI handling code for all exception ids.
 * By default, It will just print some information for debug, Vendor can customize it according to its requirements.
 */
static void system_default_non_vector_handler(unsigned long mcause, unsigned long sp)
{
    printf("irq: %u, NON Vector Handler is Default.\n", (uint32_t)(mcause & 0xFFF));
    while (1);
}
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
/**
 * \brief      Initialize all the non vector handlers
 * \note
 * Called in \ref _init function, used to initialize default non vector handlers for Number of IRQ
 */
void NonVectorHandlers_Init(void)
{
    for(int i = 0; i < CONFIG_NON_VECTOR_COUNT; i++) 
    {
        NonVectorHandlers[i] = (unsigned long)system_default_non_vector_handler;
    }
}
#endif

/**
 * \brief      Common NMI and Exception handler entry
 * \details
 * This function provided a command entry for NMI and exception. Silicon Vendor could modify
 * this template implementation according to requirement.
 * \remarks
 * - RISCV provided common entry for all types of exception. This is proposed code template
 *   for exception entry function, Silicon Vendor could modify the implementation.
 * - For the core_exception_handler template, we provided exception register function \ref Exception_Register_EXC
 *   which can help developer to register your exception handler for specific exception number.
 */
uint32_t non_vector_unified_handler(unsigned long mcause, unsigned long sp)
{
    extern void trap_c(uint32_t *regs);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    if(mcause & 0x80000000)
    {
        uint32_t irq_num = mcause & 0xFFFU;
        NON_VECTOR_HANDLER non_vector_handler = NULL;
        if(irq_num < CONFIG_NON_VECTOR_COUNT)
        {
            non_vector_handler = (NON_VECTOR_HANDLER)NonVectorHandlers[irq_num];
        }
        if(non_vector_handler != NULL)
        {
            non_vector_handler();
        }
        else
        {
            trap_c((uint32_t *)sp);
        }
    }
    else
    {
        trap_c((uint32_t *)sp);
    }
#else
    trap_c((uint32_t *)sp);
#endif
    return 0;
}
/* 
 *9002 中断总结：
 *1.向量中断
 *	向量处理模式时处理器会直接跳到中断服务程序，并没有进行上下文的保存，
 *	因此，中断响应延迟非常之短，从中断源拉高到处理器开始执行中断服务程序中的第一条指令，
 *	基本上只需要硬件进行查表和跳转的时间开销，理想情况下约6个时钟周期。
 *	
 *	中断函数一定要使用特殊的__attribute__ ((interrupt ("machine")))修饰
 *	当中断函数没有子函数调用时，处理器并没有进行上下文的保存，因此，理论上中断服务程序函数本身不能够进行子函数的调用
 *	当中断函数内有子函数调用时，编译器便会自动的插入一段代码进行上下文的保存
 *	注意：这种情况下虽然保证了功能的正确性，但是由于保存上下文造成的开销，
 *	又会事实上还是增大中断的响应延迟（与非向量模式相当）并且造成代码尺寸（Code Size）的膨胀。
 *	因此，在实践中，如果使用向量处理模式，那么不推荐在向量处理模式的中断服务程序函数中调用其他的子函数。
 *	
 *	中断函数可以使用csi_irq_vectors.c中定义默认的中断入口
 *	并且可以选择使用csi_clic_register_irq进行注册自定义的中断函数，注意需要使用特殊的__attribute__ ((interrupt ("machine")))修饰
 * 
 *2.非向量中断
 *	中断函数只能是普通的子函数，类型为void(*handler)(void),
 *	不可使用__attribute__ ((interrupt ("machine")))进行修饰，否则会产生异常
 *	并且需要进行中断入口注册（csi_clic_register_irq）
 * */
/**
 * \brief  Initialize a specific IRQ and register the handler
 * \details
 * This function set vector mode, trigger mode and polarity, interrupt level and priority,
 * assign handler for specific IRQn.
 * \param [in]  irq_num     Number of IRQ.
 * \param [in]  shv         \ref ECLIC_NON_VECTOR_INTERRUPT means non-vector mode, and \ref ECLIC_VECTOR_INTERRUPT is vector mode
 * \param [in]  trig_mode   see \ref ECLIC_TRIGGER_Type
 * \param [in]  lvl         interupt level
 * \param [in]  priority    interrupt priority
 * \param [in]  handler     interrupt handler, if NULL, handler will not be installed
 * \return       -1 means invalid input parameter. 0 means successful.
 * \remarks
 * - This function use to configure specific eclic interrupt and register its interrupt handler and enable its interrupt.
 * - If the vector table is placed in read-only section(FLASHXIP mode), handler could not be installed
 */
int32_t csi_clic_register_irq(uint32_t irq_num, uint8_t shv, CLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, IRQ_Handler_Type handler)
{
    if ((irq_num >= IRQ_VECTORS_SIZE)
    || (shv > CLIC_VECTOR_INTERRUPT)
    || (trig_mode > CLIC_NEGTIVE_EDGE_TRIGGER)){
        return -1;
    }

    /* set interrupt vector mode */
    csi_vic_set_shv_irq(irq_num, shv);
    /* set interrupt trigger mode */
    csi_vic_set_trig_irq(irq_num, trig_mode);
    /* set interrupt level */
    csi_vic_set_level_irq(irq_num, lvl);
    /* set interrupt priority */
    csi_vic_set_priority_irq(irq_num, priority);
    if (handler != NULL) {
        if(shv == CLIC_VECTOR_INTERRUPT)
        {
            /* set interrupt handler entry to vector table */
            csi_vic_set_vector(irq_num, (uint32_t)handler);
        }
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
        else
        {
            NonVectorHandlers[irq_num] = (unsigned long)handler;
        }
#endif
    }
    /* enable interrupt */
    csi_irq_enable(irq_num);
    return 0;
}