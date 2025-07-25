/**
  ******************************************************************************
  * @file    gc9002_hal_rcc.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the RCC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_RCC_H__
#define __GC9002_HAL_RCC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"
#ifdef HAL_RCC_MODULE_ENABLED

#define __HAL_RCC_DRIVER_ATTRIBUTE 		        __attribute__((used, section(HAL_CONFIG_SECTION_SRAM), optimize(0)))

typedef enum
{
	RCC_PLL_240M = 0x0U,
	RCC_PLL_288M,
	RCC_PLL_432M,
	RCC_PLL_480M,
}RCC_PLL_Enum;

#define RCC_PLL_FREQ0       240000000
#define RCC_PLL_FREQ1       288000000
#define RCC_PLL_FREQ2       432000000
#define RCC_PLL_FREQ3       480000000

typedef enum
{
	RCC_AHB_Pres_2 = 0x0U,
	RCC_AHB_Pres_3,
	RCC_AHB_Pres_4,
	RCC_AHB_Pres_6,
}RCC_AHB_Pres_Enum;
typedef enum
{
	RCC_I2S_RSTN = 0x0U,
	RCC_DISP_RSTN,
	RCC_CQSPI_RSTN,
	RCC_HYPER_RSTN,
	RCC_USB_RSTN,
	RCC_SPI_RSTN,
	RCC_I2C_RSTN,
	RCC_CAN_RSTN,
	RCC_DISP_QSPI_RSTN,
	RCC_DQSPI_RSTN,
}RCC_RSTN_Enum;

void HAL_RCC_VO3318Enable(void);
void HAL_RCC_SystemReset(void);

struct lowpower_conf
{
	GPIO_TypeDef *port;
	uint32_t pin;
	uint32_t mode;
	uint32_t lvl;
};
#if(defined HAL_CONFIG_LOWPOWER && HAL_CONFIG_LOWPOWER == 1U)
void HAL_RCC_SysLPEnter(struct lowpower_conf conf[], uint8_t size);
#else
#define HAL_RCC_SysLPEnter(conf, size)
#endif
#if(defined HAL_CONFIG_SYSCLK && HAL_CONFIG_SYSCLK == 1U)
void HAL_RCC_SYSCLKConfig(RCC_PLL_Enum pll, RCC_AHB_Pres_Enum ahb_pres);
#else
#define HAL_RCC_SYSCLKConfig(pll, ahb_pres)
#endif
void HAL_RCC_RSTN(RCC_RSTN_Enum rstn);
void HAL_RCC_CAN_PresSet(uint32_t pres, uint32_t en);

uint32_t GetSystemCoreClock(void); /*!< System Clock Frequency (Core Clock)  */
uint32_t HAL_RCC_GetAHBClock(void);
uint32_t HAL_RCC_GetAPBClock(void);
uint32_t HAL_RCC_GetPCLK1Freq(void);
uint32_t HAL_RCC_GetPLLCLKFreq(void);
void HAL_RCC_ConfigCLKGlobalVariable(void);
uint32_t HAL_RCC_GetFreqofSource(uint16_t source);
#define __HAL_RCC_I2S_SET_DIV(DUTY_L, DUTY_H)   (RCC->I2S_CLK_PRES.reg_u32val = (((DUTY_L) << 8) | (DUTY_H)))

#define __HAL_RCC_CQSPI_CLK_ENABLE()
#define __HAL_RCC_CQSPI_SET_PRES(PRES)          (RCC->CQSPI_CLK_PRES.bit.cqspi_pres = (PRES))

#define __HAL_RCC_DQSPI_CLK_ENABLE()            (RCC->DQSPI_CLK_PRES.reg_u32val |= (0x3 << 3))
#define __HAL_RCC_DQSPI_SET_PRES                __HAL_RCC_HYPER_SET_PRES
#define __HAL_RCC_HYPER_SET_PRES(PRES)          MODIFY_REG(RCC->HYPER_CLK_PRES.reg_u32val, 0x3, (PRES))

#define __HAL_DCACHE_SRAM_SHARE_ENABLE()		(RCC->DCACHE_SRAM_SHARE.bit.dcache_share_en = 1)

static inline void __HAL_RCC_DISPLAY_SET_DIV(uint16_t duty_l, uint16_t duty_h)
{
	union _DISP_CLK_PRES tempreg;
	tempreg.reg_u32val = RCC->DISP_CLK_PRES.reg_u32val;
	tempreg.bit.disp_clken = 1;
	tempreg.bit.disp_duty_l = duty_l;
	tempreg.bit.disp_duty_h = duty_h;
	RCC->DISP_CLK_PRES.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_DISPLAY_QSPI_SET_DIV(uint16_t duty_l, uint16_t duty_h)
{
	union _DISP_QSPI_CLK_CTL tempreg;
	tempreg.reg_u32val = RCC->DISP_QSPI_CLK_CTL.reg_u32val;
	tempreg.bit.disp_qspi_clken = 1;
	tempreg.bit.disp_qspi_duty_l = duty_l;
	tempreg.bit.disp_qspi_duty_h = duty_h;
	RCC->DISP_QSPI_CLK_CTL.reg_u32val = tempreg.reg_u32val;
}

static inline void __HAL_RCC_USB_CLK_ENABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.usb_clken = 1;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_USB_CLK_DISABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.usb_clken = 0;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_SPI_CLK_ENABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.spi_clken = 1;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_SPI_CLK_DISABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.spi_clken = 0;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_I2C_CLK_ENABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.i2c_clken = 1;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_I2C_CLK_DISABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.i2c_clken = 0;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_CAN_CLK_ENABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.can_clken = 1;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_CAN_CLK_DISABLE(void)
{
	union _APB_CLK_CTRL tempreg;
	tempreg.reg_u32val = RCC->APB_CLK_CTRL.reg_u32val;
	tempreg.bit.can_clken = 0;
	RCC->APB_CLK_CTRL.reg_u32val = tempreg.reg_u32val;
}
static inline void __HAL_RCC_ETH_CLK_ENABLE(void)
{
	RCC->MAC_CTL = 1;
}
static inline void __HAL_RCC_ETH_CLK_DISABLE(void)
{
	RCC->MAC_CTL = 0;
}
uint32_t HAL_RCC_GetCANFDCLKFreq(uint8_t div);
void HAL_RCC_ConfigI2SCLK(uint32_t audioFreq, uint32_t byte_cycles);
HAL_StatusTypeDef HAL_RCC_ConfigSPICLK(uint8_t id, uint32_t hz, uint16_t *cpsdvr, uint16_t *scr);

#endif /* HAL_RCC_MODULE_ENABLED */
#ifdef __cplusplus
  }
#endif
#endif/* __GC9002_HAL_RCC_H__ */
