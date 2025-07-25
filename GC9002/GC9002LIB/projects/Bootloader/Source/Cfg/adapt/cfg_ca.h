/**
  ******************************************************************************
  * @file    cfg_ca.h
  * @author  
  * @version V1.0.0
  * @date    15-08-2023
  * @brief   The adaption head file of CFG module
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_CA_H
#define __CFG_CA_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "em_c1.h"
#include "csi_irq.h"

#include "cfg_autogen_c1.h"
#include "cfg_ci.h"
#include "cfg_ct.h"


#ifndef CONFIG_RUN_BOOTLOADER
extern uint32_t g_buzzerPin; 
extern uint32_t g_backLightPin;
extern uint32_t g_touchX_PlusPin; 
extern uint32_t g_touchX_SubPin;
extern uint32_t g_touchY_PlusPin; 
extern uint32_t g_touchY_SubPin;

extern uint32_t g_touchI2c_SclPin;
extern uint32_t g_touchI2c_SdaPin;
extern uint32_t g_touchI2c_IrqPin;
extern uint32_t g_touchI2c_RstPin;

extern uint32_t g_sysTestPin;

extern GPIO_TypeDef* g_buzzerPort;
extern GPIO_TypeDef* g_backLightPort;
extern GPIO_TypeDef* g_touchX_PlusPort;
extern GPIO_TypeDef* g_touchX_SubPort;
extern GPIO_TypeDef* g_touchY_PlusPort;
extern GPIO_TypeDef* g_touchY_SubPort;

extern GPIO_TypeDef* g_touchI2c_SclPort;
extern GPIO_TypeDef* g_touchI2c_SdaPort;
extern GPIO_TypeDef* g_touchI2c_IrqPort;
extern GPIO_TypeDef* g_touchI2c_RstPort;

extern GPIO_TypeDef* g_sysTestPort;

void CFG_vGetPinAndPortInfo(void);

#endif


uint32_t CFG_u32Cfg2Pin(uint8_t cfg);
GPIO_TypeDef* CFG_pCfg2Port(uint8_t cfg);
uint32_t CFG_u32Cfg2UartBaudRate(uint8_t cfg);
uint32_t CFG_u32Cfg2CanBaudRate(uint8_t cfg);
UART_TypeDef* CFG_pCfg2UartChannel(uint8_t cfg);
CANFD_TypeDef* CFG_pCfg2CanChannel(uint8_t cfg);
SPI_TypeDef* CFG_pCfg2SpiChannel(uint8_t cfg);






#ifdef __cplusplus
}
#endif

#endif /* __CFG_CA_H */




