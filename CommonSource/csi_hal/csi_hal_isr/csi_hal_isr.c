#include "csi_hal_isr.h"
#include "platform.h"
#include "AHMICfgDefinition.h"

#if defined(CHIP_GC9002)
#include "adcshare_c1.h"
#elif defined(CHIP_GC9005)
#include "rtthread.h"
#else
#include "CPU1_CMSDK_CM0.h"
#endif
extern void DISPLAY_HandlerISRCB(void);
extern void AHMI_HandlerISRCB(void);
extern void MidTimer_IRQHandler(void);
extern void TouchTim_IRQHandler(void);
extern void Video_Cap_HandlerISRCB(void);

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
#define  ATTRIBUTE_CSI_HAL_ISR 
#else
#if defined(CHIP_GC9002)
#define  ATTRIBUTE_CSI_HAL_ISR __attribute__ ((interrupt ("machine"))) 
#else
#define  ATTRIBUTE_CSI_HAL_ISR __attribute__((interrupt))
#endif
#endif

//#define  ATTRIBUTE_CSI_HAL_ISR __attribute__ ((interrupt ("machine")))

#else
#define  ATTRIBUTE_CSI_HAL_ISR 
#endif 


#if defined(CHIP_GC9002)
ATTRIBUTE_CSI_HAL_ISR void DISPLAY_HandlerISR(void)
#else
ATTRIBUTE_CSI_HAL_ISR void DISPLAY_IRQHandler(void)
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    /* enter interrupt */
    rt_interrupt_enter();
#endif

	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	#else
	NVIC_ClearPendingIRQ(DISPLAY_IRQn);
	#endif
//    printf("===== DISPLAY_IRQHandler\n");
    DISPLAY_HandlerISRCB();

#if defined(CHIP_GC9005)
    /* leave interrupt */
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if defined(CHIP_GC9002)
ATTRIBUTE_CSI_HAL_ISR void AHMI_HandlerISR(void)
#else
ATTRIBUTE_CSI_HAL_ISR void AHMI_CORE_IRQHandler(void)
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    /* enter interrupt */
    rt_interrupt_enter();
#endif

	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	#else
	NVIC_ClearPendingIRQ(AHMI_IRQn);
	#endif
//	printf("===== AHMI_CORE_IRQHandler\n");
	AHMI_HandlerISRCB();

#if defined(CHIP_GC9005)
    /* leave interrupt */
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if defined(CHIP_GC9002)
extern TIM_HandleTypeDef htim1;
ATTRIBUTE_CSI_HAL_ISR void TIM1_IRQHandler(void)
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    /* enter interrupt */
    rt_interrupt_enter();
#endif

	__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);

#if defined(CHIP_GC9005)
//	TouchTim_IRQHandler();
#endif

#if defined(CHIP_GC9005)
    /* leave interrupt */
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#endif

#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
extern TIM_HandleTypeDef htim0;
ATTRIBUTE_CSI_HAL_ISR void TIM0_IRQHandler(void)
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    /* enter interrupt */
    rt_interrupt_enter();
#endif

	__HAL_TIM_CLEAR_IT(&htim0, TIM_IT_UPDATE);
	
	MidTimer_IRQHandler();

#if defined(CHIP_GC9002)
	extern bool GetIfTouchNeedWork(void);
	extern bool GetIfADCShareNeedWork(void);
#if 0
    if(GetIfTouchNeedWork())
    {
        TouchTim_IRQHandler();
    }
#endif

	if(GetIfADCShareNeedWork())
	{
		ADCSHR_vTrigCycleSample();
	}
#endif

#if defined(CHIP_GC9005)
    /* leave interrupt */
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#endif

#ifdef VIDEO_EN
#if defined(CHIP_GC9002)
ATTRIBUTE_CSI_HAL_ISR void Video_Cap_HandlerISR(void)
#else
void VIDEO_CAP_IRQHandler(void)
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

#if defined(CHIP_GC9005)
    /* enter interrupt */
    rt_interrupt_enter();
#endif
    
	Video_Cap_HandlerISRCB();

#if defined(CHIP_GC9005)
    /* leave interrupt */
    rt_interrupt_leave();
#endif
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#endif
