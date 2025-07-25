/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-14     huyt         the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal.h"
#ifdef HAL_RCC_MODULE_ENABLED

static uint32_t PLL_CLK = 0;

__HAL_RCC_DRIVER_ATTRIBUTE void HAL_RCC_VO3318Enable(void)
{
    uint32_t tempreg = 0;
    //enable 1v8 power
    tempreg = RCC->PMU_CTL.reg_u32val;
    ((union _PMU_CTL*)&tempreg)->bit.vo3318_en = 1;
    RCC->PMU_CTL.reg_u32val = tempreg;
    for (int i = 0; i < 1000; i++)
    {
        __NOP();
    }
}
__HAL_RCC_DRIVER_ATTRIBUTE void HAL_RCC_SystemReset(void)
{
    uint32_t tempreg = 0;

    __disable_irq();
    tempreg = RCC->SYS_CLK_SEL.reg_u32val;
    ((union _SYS_CLK_SEL*)&tempreg)->bit.sys_clk_sel = 0;
    RCC->SYS_CLK_SEL.reg_u32val = tempreg;
    
    tempreg = __get_MEXSTATUS();
    tempreg &= (~(MEXSTATUS_RESET_Msk));
    tempreg |= (uint32_t)(0x2 << MEXSTATUS_RESET_Pos);
    __set_MEXSTATUS(tempreg);
}
#if(defined HAL_CONFIG_LOWPOWER && HAL_CONFIG_LOWPOWER == 1U)
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static __HAL_RCC_DRIVER_ATTRIBUTE void TEMPLATE_GPIOx_IRQHandler(void)
#else
static __attribute__((used, section(HAL_CONFIG_SECTION_SRAM), interrupt("machine"))) void TEMPLATE_GPIOx_IRQHandler(void)
#endif
{
    HAL_RCC_SystemReset();
}

extern int32_t csi_clic_register_irq(uint32_t irq_num, uint8_t shv, CLIC_TRIGGER_Type trig_mode, uint8_t lvl, uint8_t priority, void *handler);
static void RCC_SetIOIRQ(struct lowpower_conf conf[], uint8_t size)
{
    IRQn_Type irq;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_ALL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    for (int i = 0; i < size; i++)
    {
        if((conf[i].mode & GPIO_MODE_IT_RISING) == GPIO_MODE_IT_RISING || 
           (conf[i].mode & GPIO_MODE_IT_FALLING) == GPIO_MODE_IT_FALLING || 
           (conf[i].mode & GPIO_MODE_IT_RISING_FALLING) == GPIO_MODE_IT_RISING_FALLING )
        {
            GPIO_InitStruct.Mode = conf[i].mode;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            if((conf[i].mode & GPIO_MODE_IT_FALLING) == GPIO_MODE_IT_FALLING)
            {
                GPIO_InitStruct.Pull = GPIO_PULLUP;
            }
            else if((conf[i].mode & GPIO_MODE_IT_RISING) == GPIO_MODE_IT_RISING)
            {
                GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            }
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            
            GPIO_InitStruct.Pin = conf[i].pin;
            HAL_GPIO_Init(conf[i].port, &GPIO_InitStruct);
            if(conf[i].port == GPIOA)
            {
                irq = GPIOA_IRQn;
            }
            else if(conf[i].port == GPIOB)
            {
                irq = GPIOB_IRQn;
            }
            #if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
            csi_clic_register_irq(irq, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_GPIOx_IRQHandler);
            #else
            csi_clic_register_irq(irq, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_GPIOx_IRQHandler);
            #endif
        }
        else {
            GPIO_InitStruct.Mode = conf[i].mode;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Pin = conf[i].pin;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(conf[i].port, &GPIO_InitStruct);
            HAL_GPIO_WritePin(conf[i].port, conf[i].pin, conf[i].lvl);
        }
    }

}

__HAL_RCC_DRIVER_ATTRIBUTE void HAL_RCC_SysLPEnter(struct lowpower_conf conf[], uint8_t size)
{
    uint32_t tempreg = 0;
    IRQn_Type irq;
    if(conf == NULL || size == 0)
    {
        return;
    }
    __disable_irq();
    csi_vic_disable_sirq(Machine_Software_IRQn);
    csi_vic_disable_sirq(User_Timer_IRQn      );
    csi_vic_disable_sirq(Supervisor_Timer_IRQn);
    csi_vic_disable_sirq(CORET_IRQn           );
    csi_vic_disable_sirq(Machine_External_IRQn);
    csi_vic_disable_sirq(CANFD_IRQn           );
    csi_vic_disable_sirq(UART0_IRQn           );
    csi_vic_disable_sirq(UART1_IRQn           );
    csi_vic_disable_sirq(UART2_IRQn           );
    csi_vic_disable_sirq(TIM0_IRQn            );
    csi_vic_disable_sirq(TIM1_IRQn            );
    csi_vic_disable_sirq(SPI0_IRQn            );
    csi_vic_disable_sirq(I2C0_IRQn            );
    csi_vic_disable_sirq(I2C1_IRQn            );
    csi_vic_disable_sirq(SPI1_IRQn            );
    csi_vic_disable_sirq(I2S_IRQn             );
    csi_vic_disable_sirq(GPIOA_IRQn           );
    csi_vic_disable_sirq(GPIOB_IRQn           );
    csi_vic_disable_sirq(SPI2_IRQn            );
    csi_vic_disable_sirq(ADC_IRQn             );
    csi_vic_disable_sirq(WDT_IRQn             );
    csi_vic_disable_sirq(RTC_IRQn             );
    csi_vic_disable_sirq(CQSPI_IRQn           );
    csi_vic_disable_sirq(VIDEO_CAP_IRQn       );
    csi_vic_disable_sirq(DMA_IRQn             );
    csi_vic_disable_sirq(OTG_IRQn             );
    csi_vic_disable_sirq(AHMI_IRQn            );
    csi_vic_disable_sirq(DISPLAY_IRQn         );
    csi_vic_disable_sirq(CRC32_IRQn           );
    csi_vic_disable_sirq(MOTOR_IRQn           );
    csi_vic_disable_sirq(MAC_IRQn             );
    csi_vic_disable_sirq(EXTENSION_IRQn       );
    csi_vic_disable_sirq(DQSPI_IRQn           );
    RCC_SetIOIRQ(conf, size);
    // select external clock source
    tempreg = RCC->SYS_CLK_SEL.reg_u32val;
    ((union _SYS_CLK_SEL*)&tempreg)->bit.sys_clk_sel = 0;
    RCC->SYS_CLK_SEL.reg_u32val = tempreg;
    // while (1);
    
    // disable ahmi 
    *((volatile unsigned int *)(0x40104000U+4*2)) = 0;
    // disable pll
    tempreg = RCC->SYS_PLL_CTRL.reg_u32val;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_en = 0;
    RCC->SYS_PLL_CTRL.reg_u32val = tempreg;
    // disable display clock
    tempreg = RCC->DISP_CLK_PRES.reg_u32val;
    ((union _DISP_CLK_PRES*)&tempreg)->bit.disp_clken = 0;
    RCC->DISP_CLK_PRES.reg_u32val = tempreg;
    // disable display qspi clock
    tempreg = RCC->DISP_QSPI_CLK_CTL.reg_u32val;
    ((union _DISP_QSPI_CLK_CTL*)&tempreg)->bit.disp_qspi_clken = 0;
    RCC->DISP_QSPI_CLK_CTL.reg_u32val = tempreg;
    // disable qspi clock
    tempreg = RCC->HYPER_CLK_PRES.reg_u32val;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.dqspi_clken = 0;
    ((union _HYPER_CLK_PRES*)&tempreg)->bit.hyper_clken = 0;
    RCC->HYPER_CLK_PRES.reg_u32val = tempreg;
    // disable apb clock
    tempreg = RCC->APB_CLK_CTRL.reg_u32val;
    ((union _APB_CLK_CTRL*)&tempreg)->bit.can_clken = 0;
    ((union _APB_CLK_CTRL*)&tempreg)->bit.i2c_clken = 0;
    ((union _APB_CLK_CTRL*)&tempreg)->bit.spi_clken = 0;
    ((union _APB_CLK_CTRL*)&tempreg)->bit.usb_clken = 0;
    RCC->APB_CLK_CTRL.reg_u32val = tempreg;
    // disable vo3318 clock
    tempreg = RCC->PMU_CTL.reg_u32val;
    ((union _PMU_CTL*)&tempreg)->bit.vo3318_en = 0;
    RCC->PMU_CTL.reg_u32val = tempreg;
    // disable canfd clock
    tempreg = RCC->CAN_FD_ENABLE.reg_u32val;
    ((union _CAN_FD_ENABLE*)&tempreg)->bit.can_fd_enable = 0;
    RCC->CAN_FD_ENABLE.reg_u32val = tempreg;
    // disable mac clock
    RCC->MAC_CTL = 0;
    // disable adc clock
    ADC->CLKDIV = 0;
    // disable uart
    CLEAR_BIT(UART0->CR1, (0x1 << 13));
    CLEAR_BIT(UART1->CR1, (0x1 << 13));
    CLEAR_BIT(UART2->CR1, (0x1 << 13));
    __enable_irq();
    //RCC->DCACHE_SRAM_SHARE.bit.dcache_share_en = 0;
    
    tempreg = RCC->PMU_CTL.reg_u32val;
    ((union _PMU_CTL*)&tempreg)->bit.vtr12 = 0;
    RCC->PMU_CTL.reg_u32val = tempreg;
    while(1);
}
#endif /* HAL_CONFIG_LOWPOWER */
#if(defined HAL_CONFIG_SYSCLK && HAL_CONFIG_SYSCLK == 1U)
/**
  * @brief System clk configuration
  * @param  pll RCC_PLL_Enum
  * @param  ahb_pres RCC_AHB_Pres_Enum
  * @retval None
  */
__HAL_RCC_DRIVER_ATTRIBUTE void HAL_RCC_SYSCLKConfig(RCC_PLL_Enum pll, RCC_AHB_Pres_Enum ahb_pres)
{

    uint8_t pll_q = 0;/* 1~15 */
    uint16_t pll_f = 0;/* 50~511 */
    uint8_t pll_n = 0;/* 1~63 */
    uint32_t tempreg = 0;

    tempreg = RCC->SYS_CLK_SEL.reg_u32val;
    ((union _SYS_CLK_SEL*)&tempreg)->bit.sys_clk_sel = 0;
    RCC->SYS_CLK_SEL.reg_u32val = tempreg;
    
    tempreg = RCC->SYS_PLL_CTRL.reg_u32val;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_rstn = 0;
    RCC->SYS_PLL_CTRL.reg_u32val = tempreg;
    /*
     * input = 8MHz
     * F_CLKOUT = input / N * F / Q
     * input / N = 1
     * */
    switch(pll)
    {
        case RCC_PLL_240M:
        {
            PLL_CLK = 240000000;
            pll_q = 1;/* 1~15 */
            pll_f = 240;/* 50~511 */
            pll_n = 8;/* 1~63 */
            tempreg = RCC->APB_CLK_CTRL.reg_u32val;
            ((union _APB_CLK_CTRL*)&tempreg)->bit.apb_pres = 0x00U;
            RCC->APB_CLK_CTRL.reg_u32val = ((union _APB_CLK_CTRL*)&tempreg)->reg_u32val;
        }break;
        case RCC_PLL_288M:
        {
            PLL_CLK = 288000000;
            pll_q = 1;/* 1~15 */
            pll_f = 288;/* 50~511 */
            pll_n = 8;/* 1~63 */
            tempreg = RCC->APB_CLK_CTRL.reg_u32val;
            ((union _APB_CLK_CTRL*)&tempreg)->bit.apb_pres = 0x01U;
            RCC->APB_CLK_CTRL.reg_u32val = ((union _APB_CLK_CTRL*)&tempreg)->reg_u32val;
        }break;
        case RCC_PLL_432M:
        {
            PLL_CLK = 432000000;
            pll_q = 1;/* 1~15 */
            pll_f = 432;/* 50~511 */
            pll_n = 8;/* 1~63 */
            tempreg = RCC->APB_CLK_CTRL.reg_u32val;
            ((union _APB_CLK_CTRL*)&tempreg)->bit.apb_pres = 0x02U;
            RCC->APB_CLK_CTRL.reg_u32val = ((union _APB_CLK_CTRL*)&tempreg)->reg_u32val;
        }break;
        case RCC_PLL_480M:
        {
            PLL_CLK = 480000000;
            pll_q = 1;/* 1~15 */
            pll_f = 480;/* 50~511 */
            pll_n = 8;/* 1~63 */
            tempreg = RCC->APB_CLK_CTRL.reg_u32val;
            ((union _APB_CLK_CTRL*)&tempreg)->bit.apb_pres = 0x03U;
            RCC->APB_CLK_CTRL.reg_u32val = ((union _APB_CLK_CTRL*)&tempreg)->reg_u32val;
        }break;
        default : while(1);
    }
    RCC->SYS_CLK_PRES.bit.sys_pres = ahb_pres;
    // 1.pll_en = 0
    tempreg = RCC->SYS_PLL_CTRL.reg_u32val;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_en = 0;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_rstn = 0;
    RCC->SYS_PLL_CTRL.reg_u32val = ((union _SYS_PLL_CTRL*)&tempreg)->reg_u32val;
    // 2.set q/f/n
    tempreg = RCC->SYS_PLL_CTRL.reg_u32val;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_q = pll_q;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_f = pll_f;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_n = pll_n;
    RCC->SYS_PLL_CTRL.reg_u32val = ((union _SYS_PLL_CTRL*)&tempreg)->reg_u32val;
    // 3.pll_en = 1
    tempreg = RCC->SYS_PLL_CTRL.reg_u32val;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_rstn = 1;
    ((union _SYS_PLL_CTRL*)&tempreg)->bit.sys_pll_en = 1;
    RCC->SYS_PLL_CTRL.reg_u32val = ((union _SYS_PLL_CTRL*)&tempreg)->reg_u32val;

    while(RCC->SYS_PLL_CTRL.bit.sys_pll_lock == 0);

    tempreg = RCC->SYS_CLK_SEL.reg_u32val;
    ((union _SYS_CLK_SEL*)&tempreg)->bit.sys_clk_sel = 1;
    RCC->SYS_CLK_SEL.reg_u32val = tempreg;
    /* Delay to prevent exception: run from RAM switched to FLASH */
    for(int i = 0; i < 4; i++)
    {
        __NOP();//do nothing
    }
}
#endif /* HAL_CONFIG_SYSCLK */
/**
  * @brief Module reset
  * @param  rstn
  * @retval None
  */
void HAL_RCC_RSTN(RCC_RSTN_Enum rstn)
{
    SET_BIT(RCC->RESET_CTRL.reg_u32val, 0x1 << rstn);
}
/**
  * @brief can pres set
  * @param  pres 0/1/2/3
  * @param  en ENABLE/DISABLE
  * @retval None
  */
void HAL_RCC_CAN_PresSet(uint32_t pres, uint32_t en)
{
    uint32_t tempreg = 0;
	__HAL_RCC_CAN_CLK_ENABLE();
    if(en == ENABLE)
    {
        ((union _CAN_FD_ENABLE*)&tempreg)->bit.can_pres = pres;
        ((union _CAN_FD_ENABLE*)&tempreg)->bit.can_fd_enable = 1;
    }
    else
    {
        ((union _CAN_FD_ENABLE*)&tempreg)->bit.can_pres = RCC->CAN_FD_ENABLE.bit.can_pres;
        ((union _CAN_FD_ENABLE*)&tempreg)->bit.can_fd_enable = 0;
    }
    RCC->CAN_FD_ENABLE.reg_u32val = ((union _CAN_FD_ENABLE*)&tempreg)->reg_u32val;
}

uint32_t GetSystemCoreClock(void)
{
    return HAL_RCC_GetAHBClock();
}
/**
  * @brief  Returns the AHB frequency
  * @retval AHB frequency
  */
uint32_t HAL_RCC_GetAHBClock(void)
{
    switch(RCC->SYS_CLK_PRES.bit.sys_pres)
    {
    case 0: return (HAL_RCC_GetPLLCLKFreq()/2);
    case 1: return (HAL_RCC_GetPLLCLKFreq()/3);
    case 2: return (HAL_RCC_GetPLLCLKFreq()/4);
    case 3: return (HAL_RCC_GetPLLCLKFreq()/6);
    default: break;
    }
    return (HAL_RCC_GetPLLCLKFreq()/2);
}
/**
  * @brief  Returns the APB frequency
  * @retval APB frequency
  */
uint32_t HAL_RCC_GetAPBClock(void)
{
    switch(RCC->APB_CLK_CTRL.bit.apb_pres)
    {
    case 0: return (HAL_RCC_GetPLLCLKFreq()/5);
    case 1: return (HAL_RCC_GetPLLCLKFreq()/6);
    case 2: return (HAL_RCC_GetPLLCLKFreq()/9);
    case 3: return (HAL_RCC_GetPLLCLKFreq()/10);
    default: break;
    }
    #if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
    #endif
    return 0;
}
/**
  * @brief  Returns the PCLK1 frequency
  * @note   Each time PCLK1 changes, this function must be called to update the
  *         right PCLK1 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK1 frequency
  */
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
    /* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
    return HAL_RCC_GetAPBClock();
}

uint32_t HAL_RCC_GetPLLCLKFreq(void)
{
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
#else
    /*
     * F_CLKOUT = 8MHz / N * F / Q
     * */
    return (uint32_t)(8000000 / \
            RCC->SYS_PLL_CTRL.bit.sys_pll_n * \
            RCC->SYS_PLL_CTRL.bit.sys_pll_f / \
            RCC->SYS_PLL_CTRL.bit.sys_pll_q);
#endif
}
void HAL_RCC_ConfigCLKGlobalVariable(void)
{

}

uint32_t HAL_RCC_GetCANFDCLKFreq(uint8_t div)
{
    uint32_t pll = 0;
    uint32_t f_clock = 0;
    pll = HAL_RCC_GetPLLCLKFreq();
    switch(div)
    {
        case CANFD_PRESCALER_0:
            f_clock = pll/6;
        break;
        case CANFD_PRESCALER_1:
            f_clock = pll/11;
            if(f_clock > 39000000 && f_clock <= 40000000)
            {
                f_clock = 40000000;
            }
        break;
        case CANFD_PRESCALER_2:
            f_clock = pll/12;
        break;
        case CANFD_PRESCALER_3:
            f_clock = pll/24;
        break;
        default:
        break;
    }
    return f_clock;
}

void HAL_RCC_ConfigI2SCLK(uint32_t audioFreq, uint32_t byte_cycles)
{
    /*
     * fs=48K
     * SCLK=2*fs*16
     * MCLK=256*fs
     * */
    uint32_t pll_frequency = HAL_RCC_GetPLLCLKFreq();
    uint32_t duty = 0;
    uint32_t mclk = 0;
    uint16_t i2s_duty_l = 0;
    uint16_t i2s_duty_h = 0;

    assert_param(pll_frequency == RCC_PLL_FREQ0 ||
                 pll_frequency == RCC_PLL_FREQ1 ||
                 pll_frequency == RCC_PLL_FREQ2 ||
                 pll_frequency == RCC_PLL_FREQ3);
    mclk = audioFreq*2*byte_cycles;
    duty = pll_frequency/2/mclk;
    i2s_duty_l = duty / 2;
    i2s_duty_h = duty - i2s_duty_l;
    assert_param(i2s_duty_l <= 0xFF && i2s_duty_h <= 0xFF);
    __HAL_RCC_I2S_SET_DIV(i2s_duty_l & 0xFF, i2s_duty_h & 0xFF);
}
uint32_t HAL_RCC_GetFreqofSource(uint16_t source)
{
    if(source == 0)
    {
        return HAL_RCC_GetAPBClock();
    } else if(source == 1){
        return HAL_RCC_GetAHBClock();
    }
    return HAL_RCC_GetAPBClock();
}
HAL_StatusTypeDef HAL_RCC_ConfigSPICLK(uint8_t id, uint32_t hz, uint16_t *cpsdvr, uint16_t *scr)
{
    /* bitrate = Fspiclk/(CPSDVR*(1 + SCR))
     * */
    uint32_t freq = (uint32_t)HAL_RCC_GetAPBClock();
    uint32_t div = 0;
    uint16_t temp_cpsdvr = 0;
    uint16_t temp_scr = 0;
    uint8_t div_src = 1;

    for(int index = 0; index <= 7; index++)
    {
        div_src = 1;
        for(int count = 0; count < index; count++)
        {
            div_src *= 2;
        }
        div = (freq/div_src/hz);
        for(int i = 2; i <= 254; i+=2)
        {
            temp_scr = div/i - 1;
            if(temp_scr <= 255)
            {
                temp_cpsdvr = i;
                break;
            }
        }
        if(((temp_cpsdvr >= 2) && (temp_cpsdvr <= 254)) && (temp_scr <= 0xFF))
        {
            div_src = index;
            goto __exit;
        }
    }
    return HAL_ERROR;
__exit:
    //baudrate = Fspiclk / (CPSDVR * (1 + SCR))
    *cpsdvr = temp_cpsdvr; // Clock predivision coefficient Value range 2-254, only Even number
    *scr = temp_scr;       // Value range 0-255
    __HAL_RCC_SPI_CLK_ENABLE();
    return HAL_OK;
}
#endif /* HAL_RCC_MODULE_ENABLED */
