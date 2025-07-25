/**
  ******************************************************************************
  * @file    gc9002_hal_wdt.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the WDT
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_WDT_H__
#define __GC9002_HAL_WDT_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_WDT_MODULE_ENABLED


typedef enum
{
	WDT_PCLK_CYCLES_2,
	WDT_PCLK_CYCLES_4,
	WDT_PCLK_CYCLES_8,
	WDT_PCLK_CYCLES_16,
	WDT_PCLK_CYCLES_32,
	WDT_PCLK_CYCLES_64,
	WDT_PCLK_CYCLES_128,
	WDT_PCLK_CYCLES_256,
}WDT_RPL_TypeDef;

typedef enum
{
	WDT_RESET_MODE,
	WDT_INTERRUPT_MODE,
}WDT_RMOD_TypeDef;

typedef enum
{
	WDT_Timeout_64K_CLOCKS,
	WDT_Timeout_128K_CLOCKS,
	WDT_Timeout_256K_CLOCKS,
	WDT_Timeout_512K_CLOCKS,
	WDT_Timeout_1M_CLOCKS,
	WDT_Timeout_2M_CLOCKS,
	WDT_Timeout_4M_CLOCKS,
	WDT_Timeout_8M_CLOCKS,
	WDT_Timeout_16M_CLOCKS,
	WDT_Timeout_32M_CLOCKS,
	WDT_Timeout_64M_CLOCKS,
	WDT_Timeout_128M_CLOCKS,
	WDT_Timeout_256M_CLOCKS,
	WDT_Timeout_512M_CLOCKS,
	WDT_Timeout_1G_CLOCKS,
	WDT_Timeout_2G_CLOCKS,
}WDT_Timeout_TypeDef;

/**
  * @brief  WDT Init structure definition
  */
typedef struct
{
	uint32_t ResetMode;		/*!< Specifies the Response mode of the WDT.
							   This parameter can be a value of @ref WDT_RMOD_TypeDef */
	uint32_t ResetPulseLen; /*!< Specifies the Reset Pulse Length value of the WDT.
							   This parameter can be a value of @ref WDT_RPL_TypeDef */
	uint32_t Counter;       /*!< Specifies the WDT free-running up-counter value.
							   This parameter can be a value of @ref WDT_Timeout_TypeDef */
}WDT_InitTypeDef;

/**
  * @brief  WDT handle Structure definition
  */
#if (USE_HAL_WDT_REGISTER_CALLBACKS == 1)
typedef struct __WDT_HandleTypeDef
#else
typedef struct
#endif
{
	WDT_TypeDef      *Instance;  /*!< Register base address */
	WDT_InitTypeDef  Init;       /*!< WDT required parameters */
    void *         	 priv_obj;
#if (USE_HAL_WDT_REGISTER_CALLBACKS == 1)
	void             (* IRQCallback)(struct __WDT_HandleTypeDef *hwdg);     	/*!< WDT Interrupt callback  */
	void             (* MspInitCallback)(struct __WDT_HandleTypeDef *hwdg); 	/*!< WDT Msp Init callback   */
	void             (* MspDeInitCallback)(struct __WDT_HandleTypeDef *hwdg); 	/*!< WDT Msp DeInit callback */
#endif
}WDT_HandleTypeDef;

#define WDT_RESET_CMD 				0x76


#define IS_WDT_RMOD(__RMOD__)          		(((__RMOD__) == WDT_RESET_MODE) || \
											 ((__RMOD__) == WDT_INTERRUPT_MODE))
/**
  * @brief  Check IWDG reset pulse length value.
  * @param  __PULSE_LEN__  IWDG pulse length value
  * @retval None
  */
#define IS_WDT_PULSE_LEN(__PULSE_LEN__)      (((__PULSE_LEN__) == WDT_PCLK_CYCLES_2)   || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_4)   || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_8)   || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_16)  || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_32)  || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_64)  || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_128) || \
                                              ((__PULSE_LEN__) == WDT_PCLK_CYCLES_256))
#define IS_WDT_COUNTER(COUNTER)     ((COUNTER) == WDT_Timeout_64K_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_128K_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_256K_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_512K_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_1M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_2M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_4M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_8M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_16M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_32M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_64M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_128M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_256M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_512M_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_1G_CLOCKS || \
                                     (COUNTER) == WDT_Timeout_2G_CLOCKS)

/**
  * @brief  Check IWDG timeout period value.
  * @param  __RELOAD__  IWDG timeout period value
  * @retval None
  */
#define IS_WDT_TIMEOUT_PERIOD(__PERIOD__)           ((__PERIOD__) <= 0xFUL)
/**
  * @brief  Get WDT interrupt status
  * @param  __HANDLE__: pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval interrupt status,0 or 1
  */
#define __HAL_WDT_GET_IT_FLAG(__HANDLE__)	(((__HANDLE__)->Instance->ISR & 0x1) == SET)
/**
  * @brief  Clear WDT interrupt status
  * @param  __HANDLE__: pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval interrupt status,0 or 1
  */
static inline void __HAL_WDT_IT_CLEAR(WDT_HandleTypeDef *hwdt)
{
	uint32_t tempreg = 0;
	tempreg = hwdt->Instance->ICR;
	(void)tempreg;
}
static inline bool __HAL_WDT_GET_EN(WDT_HandleTypeDef *hwdt)
{
	uint32_t tempreg = hwdt->Instance->CR.reg_u32val;
    return (((union _WDT_CR *)&tempreg)->bit.WDT_EN == 1);
}

HAL_StatusTypeDef	HAL_WDT_StructInit(WDT_HandleTypeDef *hwdt);
/* Initialization/de-initialization functions  **********************************/
HAL_StatusTypeDef	HAL_WDT_Init(WDT_HandleTypeDef *hwdt);
void             	HAL_WDT_MspInit(WDT_HandleTypeDef *hwdt);
void 				HAL_WDT_MspDeInit(WDT_HandleTypeDef *hwdt);
/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_WDT_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL WDT Callback ID enumeration definition
  */
typedef enum
{
  	HAL_WDT_IRQ_CB_ID         	= 0x01U,    /*!< WDT interrupt Callback ID    	*/
	HAL_WDT_MSPINIT_CB_ID 		= 0x02U,    /*!< WDT MspInit callback ID 		*/
	HAL_WDT_MSPDEINIT_CB_ID  	= 0x04U     /*!< WDT MspDeInit callback ID   	*/
}HAL_WDT_CallbackIDTypeDef;
/**
  * @brief  HAL WDT Callback pointer definition
  */
typedef  void (*pWDT_CallbackTypeDef)(WDT_HandleTypeDef *huart);  /*!< pointer to an UART callback function */
HAL_StatusTypeDef 	HAL_WDT_RegisterCallback(WDT_HandleTypeDef *hwdt, HAL_WDT_CallbackIDTypeDef CallbackID, pWDT_CallbackTypeDef pCallback);
HAL_StatusTypeDef	HAL_WDT_UnRegisterCallback(WDT_HandleTypeDef *hwdt, HAL_WDT_CallbackIDTypeDef CallbackID);
#endif

/* I/O operation functions ******************************************************/
void 				HAL_WDT_Enable(WDT_HandleTypeDef *hwdt);
void 				HAL_WDT_Disable(WDT_HandleTypeDef *hwdt);
uint32_t 			HAL_WDT_GetCurrentCounterValue(WDT_HandleTypeDef *hwdt);
uint32_t 			HAL_WDT_GetTimeoutCounterValue(WDT_HandleTypeDef *hwdt);
void 				HAL_WDT_SetTimeoutPeriod(WDT_HandleTypeDef *hwdt, WDT_Timeout_TypeDef period);
HAL_StatusTypeDef	HAL_WDT_Refresh(WDT_HandleTypeDef *hwdt);

void				HAL_WDT_IRQHandler(WDT_HandleTypeDef *hwdt);
void 				HAL_WDT_IRQCallback(WDT_HandleTypeDef *hwdt);

#endif/* HAL_WDT_MODULE_ENABLE */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_WDT_H__ */
