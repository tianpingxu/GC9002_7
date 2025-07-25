////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI 
//
// Create Date:   2021/07/08
// File Name:     srv_timer_c1.c
// Project Name:  CPU0_Bootloader_Redesign
// 
// Revision:
// Revision 1.0.0 - File Created 2021/07/08 by Zhu Jiahai
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include "gc90x_it.h"
//#include "CPU1_CMSDK_CM0.h"
//#include "core_cm0.h"
//#include "LoadFiles_SDCard.h"
//#include "qspi_nand.h"
//#include "gc90x_timer.h"

#include "srv_timer_c1.h"
#include "upgrade_c1.h"
#include "csi_irq.h"


SRV_TIMER_tsTimerWorkBuf astTimerWorkbuf[SRV_TIMER_enTimerCount];
bool bTimerInited = FALSE_0;
uint32_t u32TimerTick_1ms = 0;



/**
  * @name       SRV_TIMER__vHwTimer6Init
  * @brief      Timer0 hardware initialization
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
TIM_HandleTypeDef htim_Srv = {.Instance = TIM0};
extern void SRV_TIMER_vMainProcess(void);
void SRV_TIMER__vHwTimer6Init(void)
{
	htim_Srv.Init.Prescaler = HAL_RCC_GetAPBClock() / 1000000 - 1;
	htim_Srv.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_Srv.Init.Period = 1000 - 1;
	htim_Srv.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim_Srv.Init.RepetitionCounter = 0xFFFE;
	htim_Srv.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	
	HAL_TIM_Base_Init(&htim_Srv);
	
	#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
	csi_clic_register_irq(TIM0_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_POSTIVE_EDGE_TRIGGER, 2, 1, SRV_TIMER_vMainProcess);
	#else
	csi_clic_register_irq(TIM0_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_POSTIVE_EDGE_TRIGGER, 2, 1, SRV_TIMER_vMainProcess);
	#endif
	csi_irq_enable(TIM0_IRQn);
	HAL_TIM_Base_Start_IT(&htim_Srv); 
}



/**
  * @name       SRV_TIMER__vInitTimerHandlerAddress
  * @brief      Timer Service main function entry address bond to Interrupt handler
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
void SRV_TIMER__vInitTimerHandlerAddress(void)
{   
    //*(uint32_t *)(T6_HandlerAddr) = (uint32_t)SRV_TIMER_vMainProcess;
}


/**
  * @name       SRV_TIMER_bIfTimeout
  * @brief      check if specific Timer is timeout
  * @note        
  * @param      u8Index: index of Timer
  * @retval     TRUE or FALSE
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
bool SRV_TIMER_bIfTimeout(uint8_t u8Index)
{
    bool ret;
    
    if(u8Index >= SRV_TIMER_enTimerCount)
    {
        ret = FALSE_0;
    }
    else
    {
        if(astTimerWorkbuf[u8Index].u32TimerCurCount >= astTimerWorkbuf[u8Index].u32TimerToCount)
        {
            ret = TRUE_1;
        }
        else
        {
            ret = FALSE_0;
        }
    }

    return ret;
}



/**
  * @name       SRV_TIMER_bIfTimeout
  * @brief      check if specific Timer is timeout
  * @note        
  * @param      u8Index: index of Timer
  * @param      u32Ms: Millisecond to set for the specific Timer
  * @param      cbHandler: callback function to notify caller that timeout event occurs
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
int32_t SRV_TIMER_i32SetTimer(uint8_t u8Index, uint32_t u32Ms, CallbackHandler cbHandler)
{
    /* parameter check */
    if(u8Index >= SRV_TIMER_enTimerCount)
    {
        /* Log error */
        return UPG_ERR;
    }

    if(FALSE_0 == astTimerWorkbuf[u8Index].bFlagRun)
    {
        SRV_TIMER_ENTER_CRITICAL; 
        astTimerWorkbuf[u8Index].bFlagRun = TRUE_1;
        astTimerWorkbuf[u8Index].bFlagCbExecuted = FALSE_0;
        astTimerWorkbuf[u8Index].u32TimerCurCount = 0;
        astTimerWorkbuf[u8Index].u32TimerToCount = u32Ms;
        astTimerWorkbuf[u8Index].pfCbHander = cbHandler;
        SRV_TIMER_EXIT_CRITICAL;
    }
    
    return UPG_OK;
}



/**
  * @name       SRV_TIMER_i32StopTimer
  * @brief      stop the specific Timer
  * @note        
  * @param      u8Index: index of Timer
  * @retval     OK or ERR
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
int32_t SRV_TIMER_i32StopTimer(uint8_t u8Index)
{
    /* parameter check */
    if(u8Index >= SRV_TIMER_enTimerCount)
    {
        /* Log error */
        return UPG_ERR;
    }

    SRV_TIMER_ENTER_CRITICAL; 
    astTimerWorkbuf[u8Index].bFlagRun = FALSE_0;
    astTimerWorkbuf[u8Index].u32TimerCurCount = 0;
    SRV_TIMER_EXIT_CRITICAL;

    return UPG_OK;
}



/**
  * @name       SRV_TIMER_vInit
  * @brief      Initialization of Timer Service module
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
void SRV_TIMER_vInit(void)
{
    memset(astTimerWorkbuf, 0, sizeof(astTimerWorkbuf));
    SRV_TIMER__vInitTimerHandlerAddress();
    SRV_TIMER__vHwTimer6Init();

    bTimerInited = TRUE_1;
}



/**
  * @name       SRV_TIMER_vDeInit
  * @brief      De-initialization of Timer Service module
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
void SRV_TIMER_vDeInit(void)
{
    csi_irq_disable(TIM0_IRQn);
    memset(astTimerWorkbuf, 0, (sizeof(astTimerWorkbuf)));
    bTimerInited = FALSE_0;
}

/**
  * @name       SRV_TIMER_u32GetTimerTick
  * @brief      return timer tick
  * @note        
  * @param      none
  * @retval     timer tick now
  * @author     Xia Tengfei
  * @date       2022-04-28
  */
u32 SRV_TIMER_u32GetTimerTick(void)
{
    return u32TimerTick_1ms;
}



/**
  * @name       SRV_TIMER_vMainProcess
  * @brief      Main function of Timer Service module
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2021-07-08
  */
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
#else
__attribute__ ((interrupt ("machine"))) 
#endif
void SRV_TIMER_vMainProcess(void)
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    int32_t i;
    
	__HAL_TIM_CLEAR_IT(&htim_Srv, TIM_IT_UPDATE);
	
    u32TimerTick_1ms++;
    
    if(FALSE_0 == bTimerInited)
    {
    }
    else
    {
        for(i = 0; i < SRV_TIMER_enTimerCount; i++)
        {
            if(astTimerWorkbuf[i].bFlagRun)
            {
                if(astTimerWorkbuf[i].u32TimerCurCount < astTimerWorkbuf[i].u32TimerToCount)
                {
                    astTimerWorkbuf[i].u32TimerCurCount++;
                }
                else
                {
                    if((FALSE_0 == astTimerWorkbuf[i].bFlagCbExecuted) && (NULL != astTimerWorkbuf[i].pfCbHander))
                    {
                        astTimerWorkbuf[i].pfCbHander();
                        astTimerWorkbuf[i].bFlagCbExecuted = TRUE_1;
                    }
                }
            }
        }

    	extern void Diag_Timer_vIRQHandler(void);
        Diag_Timer_vIRQHandler();
    }
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}







