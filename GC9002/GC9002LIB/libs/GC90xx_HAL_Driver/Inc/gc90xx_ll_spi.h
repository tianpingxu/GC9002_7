/**
  ******************************************************************************
  * @file    gc90xx_ll_spi.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-3-1
  * @brief   This file contains all the functions prototypes for the SPI
  *          firmware library.
  ******************************************************************************
  */
#ifndef __GC90XX_LL_SPI_H__

#define __GC90XX_LL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_SPI_MODULE_ENABLED



/**
  * @brief  Enable SPI peripheral
  * @rmtoll CR1          SPE           LL_SPI_Enable
  * @param  SPIx SPI Instance
  * @retval None
  */
__STATIC_INLINE void LL_SPI_Enable(SPI_TypeDef *SPIx)
{
    SET_BIT(SPIx->CR1, SPI_CR1_SPE);
}
/**
  * @brief  Disable SPI peripheral
  * @note   When disabling the SPI, follow the procedure described in the Reference Manual.
  * @rmtoll CR1          SPE           LL_SPI_Disable
  * @param  SPIx SPI Instance
  * @retval None
  */
__STATIC_INLINE void LL_SPI_Disable(SPI_TypeDef *SPIx)
{
    CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);
}
/**
  * @brief  Check if SPI peripheral is enabled
  * @rmtoll CR1          SPE           LL_SPI_IsEnabled
  * @param  SPIx SPI Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SPI_IsEnabled(SPI_TypeDef *SPIx)
{
    return ((READ_BIT(SPIx->CR1, SPI_CR1_SPE) == (SPI_CR1_SPE)) ? 1UL : 0UL);
}
/** @defgroup SPI_LL_EC_MODE Operation Mode
  * @{
  */
#define LL_SPI_MODE_SLAVE                 SPI_CR1_SLAVE    /*!< Master configuration  */
#define LL_SPI_MODE_MASTER                0x00000000U      /*!< Slave configuration   */
/**
  * @brief  Set SPI operation mode to Master or Slave
  * @note   This bit should not be changed when communication is ongoing.
  * @rmtoll CR1          MSTR          LL_SPI_SetMode\n
  *         CR1          SSI           LL_SPI_SetMode
  * @param  SPIx SPI Instance
  * @param  Mode This parameter can be one of the following values:
  *         @arg @ref LL_SPI_MODE_MASTER
  *         @arg @ref LL_SPI_MODE_SLAVE
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetMode(SPI_TypeDef *SPIx, uint32_t Mode)
{
    MODIFY_REG(SPIx->CR1, SPI_CR1_SLAVE_Msk, Mode);
}
/**
  * @brief  Get SPI operation mode (Master or Slave)
  * @rmtoll CR1          MSTR          LL_SPI_GetMode\n
  *         CR1          SSI           LL_SPI_GetMode
  * @param  SPIx SPI Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SPI_MODE_MASTER
  *         @arg @ref LL_SPI_MODE_SLAVE
  */
__STATIC_INLINE uint32_t LL_SPI_GetMode(SPI_TypeDef *SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR1, SPI_CR1_SLAVE_Msk));
}
#define LL_SPI_PROTOCOL_MOTOROLA                0x00000000U                 /*!< Motorola mode. Used as default value  */
#define LL_SPI_PROTOCOL_TI                      (SPI_CR0_FRF_0)             /*!< TI mode                               */
#define LL_SPI_PROTOCOL_NATIONALMICROWIRE       (SPI_CR0_FRF_1)             /*!< National Microwirte frame format mode */
/**
  * @brief  Set serial protocol used
  * @note   This bit should be written only when SPI is disabled (SPE = 0) for correct operation.
  * @rmtoll CR0          FRF           LL_SPI_SetStandard
  * @param  SPIx SPI Instance
  * @param  Standard This parameter can be one of the following values:
  *         @arg @ref LL_SPI_PROTOCOL_MOTOROLA
  *         @arg @ref LL_SPI_PROTOCOL_TI
  *         @arg @ref LL_SPI_PROTOCOL_NATIONALMICROWIRE
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetStandard(SPI_TypeDef *SPIx, uint32_t Standard)
{
    MODIFY_REG(SPIx->CR0, SPI_CR0_FRF_Msk, Standard);
}
/**
  * @brief  Get serial protocol used
  * @rmtoll CR0          FRF           LL_SPI_GetStandard
  * @param  SPIx SPI Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SPI_PROTOCOL_MOTOROLA
  *         @arg @ref LL_SPI_PROTOCOL_TI
  *         @arg @ref LL_SPI_PROTOCOL_NATIONALMICROWIRE
  */
__STATIC_INLINE uint32_t LL_SPI_GetStandard(SPI_TypeDef *SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR0, SPI_CR0_FRF_Msk));
}
#define LL_SPI_PHASE_1EDGE                 0x00000000U               /*!< First clock transition is the first data capture edge  */
#define LL_SPI_PHASE_2EDGE                 (SPI_CR0_SPH)             /*!< Second clock transition is the first data capture edge */
/**
  * @brief  Set clock phase
  * @note   This bit should not be changed when communication is ongoing.
  *         This bit is not used in SPI TI mode.
  * @rmtoll CR0          CPHA          LL_SPI_SetClockPhase
  * @param  SPIx SPI Instance
  * @param  ClockPhase This parameter can be one of the following values:
  *         @arg @ref LL_SPI_PHASE_1EDGE
  *         @arg @ref LL_SPI_PHASE_2EDGE
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetClockPhase(SPI_TypeDef *SPIx, uint32_t ClockPhase)
{
    MODIFY_REG(SPIx->CR0, SPI_CR0_SPH, ClockPhase);
}
/**
  * @brief  Get clock phase
  * @rmtoll CR0          CPHA          LL_SPI_GetClockPhase
  * @param  SPIx SPI Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SPI_PHASE_1EDGE
  *         @arg @ref LL_SPI_PHASE_2EDGE
  */
__STATIC_INLINE uint32_t LL_SPI_GetClockPhase(SPI_TypeDef *SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR0, SPI_CR0_SPH));
}
#define LL_SPI_POLARITY_LOW                0x00000000U               /*!< Clock to 0 when idle */
#define LL_SPI_POLARITY_HIGH               (SPI_CR0_SPO)             /*!< Clock to 1 when idle */
/**
  * @brief  Set clock polarity
  * @note   This bit should not be changed when communication is ongoing.
  *         This bit is not used in SPI TI mode.
  * @rmtoll CR0          CPOL          LL_SPI_SetClockPolarity
  * @param  SPIx SPI Instance
  * @param  ClockPolarity This parameter can be one of the following values:
  *         @arg @ref LL_SPI_POLARITY_LOW
  *         @arg @ref LL_SPI_POLARITY_HIGH
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetClockPolarity(SPI_TypeDef *SPIx, uint32_t ClockPolarity)
{
    MODIFY_REG(SPIx->CR0, SPI_CR0_SPO, ClockPolarity);
}

/**
  * @brief  Get clock polarity
  * @rmtoll CR0          CPOL          LL_SPI_GetClockPolarity
  * @param  SPIx SPI Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SPI_POLARITY_LOW
  *         @arg @ref LL_SPI_POLARITY_HIGH
  */
__STATIC_INLINE uint32_t LL_SPI_GetClockPolarity(SPI_TypeDef *SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR0, SPI_CR0_SPO));
}
/**
  * @brief  Set baud rate prescaler
  * @note   These bits should not be changed when communication is ongoing. SPI BaudRate = Fspiclk/(CPSDVR*(1 + SCR)).
  * @rmtoll CR1          BR            LL_SPI_SetBaudRatePrescaler
  * @rmtoll CPSR         BR            LL_SPI_GetBaudRatePrescaler
  * @param  SPIx SPI Instance
  * @param  cpsdvr This parameter must be an even number from 2 to 254
  * @param  scr This parameter can be one of the following values: 0~255
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetBaudRatePrescaler(SPI_TypeDef *SPIx, uint32_t cpsdvr, uint32_t scr)
{
    MODIFY_REG(SPIx->CR0, SPI_CR0_SCR_Msk, (scr << SPI_CR0_SCR_Pos));
    MODIFY_REG(SPIx->CPSR, 0xFF, cpsdvr);
}

/**
  * @brief  Get baud rate prescaler
  * @rmtoll CR0          BR            LL_SPI_GetBaudRatePrescaler
  * @rmtoll CPSR         BR            LL_SPI_GetBaudRatePrescaler
  * @param  SPIx SPI Instance
  * @param  cpsdvr This parameter must be an even number from 2 to 254
  * @param  scr This parameter can be one of the following values: 0~255
  */
__STATIC_INLINE void LL_SPI_GetBaudRatePrescaler(SPI_TypeDef *SPIx, uint32_t *cpsdvr, uint32_t *scr)
{
    *cpsdvr = (uint32_t)(READ_BIT(SPIx->CPSR, 0xFF));
    *scr = (uint32_t)(READ_BIT(SPIx->CR0, SPI_CR0_SCR_Msk) >> SPI_CR0_SCR_Pos);
}

/** @defgroup SPI_Data_Size SPI Data Size
  * @{
  */
#define LL_SPI_DATASIZE_4BIT               (SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_5BIT               (SPI_CR0_DSS_2)
#define LL_SPI_DATASIZE_6BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_7BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_1)
#define LL_SPI_DATASIZE_8BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_9BIT               (SPI_CR0_DSS_3)
#define LL_SPI_DATASIZE_10BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_11BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_1)
#define LL_SPI_DATASIZE_12BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_13BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2)
#define LL_SPI_DATASIZE_14BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_0)
#define LL_SPI_DATASIZE_15BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_1)
#define LL_SPI_DATASIZE_16BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
/**
  * @brief  Set frame data width
  * @rmtoll CR0          DFF           LL_SPI_SetDataWidth
  * @param  SPIx SPI Instance
  * @param  DataWidth This parameter can be one of the following values:
  *         @arg @ref LL_SPI_DATAWIDTH_8BIT
  *         @arg @ref LL_SPI_DATAWIDTH_16BIT
  * @retval None
  */
__STATIC_INLINE void LL_SPI_SetDataWidth(SPI_TypeDef *SPIx, uint32_t DataWidth)
{
    MODIFY_REG(SPIx->CR0, SPI_CR0_DSS_Msk, DataWidth);
}

/**
  * @brief  Get frame data width
  * @rmtoll CR0          DFF           LL_SPI_GetDataWidth
  * @param  SPIx SPI Instance
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SPI_DATAWIDTH_8BIT
  *         @arg @ref LL_SPI_DATAWIDTH_16BIT
  */
__STATIC_INLINE uint32_t LL_SPI_GetDataWidth(SPI_TypeDef *SPIx)
{
    return (uint32_t)(READ_BIT(SPIx->CR0, SPI_CR0_DSS_Msk));
}











#endif /* HAL_SPI_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_LL_SPI_H__ */
