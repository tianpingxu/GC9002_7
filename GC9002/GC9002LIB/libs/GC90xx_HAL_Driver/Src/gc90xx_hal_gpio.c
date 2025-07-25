/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-22     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_GPIO_MODULE_ENABLED

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
__WEAK void phy_pin_mode_init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    UNUSED(GPIOx);
    UNUSED(GPIO_Pin);
    // This entity function is nerver called
    // This function is called only if the phy is used, and a new entity function needs to be implemented
    assert_param(0);
}
__WEAK void phy_pin_mode_ctrl(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    UNUSED(GPIOx);
    UNUSED(GPIO_Pin);
    // This entity function is nerver called
    // This function is called only if the phy is used, and a new entity function needs to be implemented
    assert_param(0);
}
__WEAK void phy_pin_ch1_all_line_disable(void)
{

}
#endif /* (defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U) */
/**
  * @brief  Initializes the GPIO_InitTypeDef structure to their default reset values.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
  *         contains the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    assert_param(IS_GPIO_INISTRUCT(GPIO_InitStruct));
    GPIO_InitStruct->Pin = GPIO_PIN_0;
    GPIO_InitStruct->Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct->Pull = GPIO_NOPULL;
    GPIO_InitStruct->Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct->SchmidtTrigger = GPIO_SCHMIDT_TRIGGER_DISABLE;
    GPIO_InitStruct->DriverStrength = GPIO_DRIVER_STRENGTH_0;
    GPIO_InitStruct->Alternate = GPIO_AF0;//general IO
#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
    GPIO_InitStruct->AltfMode = GPIO_AFM0;//general IO
#endif /* BSP_GPIO_USE_AFMODE */
}

/**
  * @brief  Deinitializes the GPIO peripheral registers to their default reset values.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
  *         contains the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_INISTRUCT(GPIO_InitStruct));
    (void)GPIOx;
    (void)GPIO_InitStruct;
}

/**
  * @brief  Initializes the GPIO peripheral according to the specified
  *         parameters in the GPIO_InitStruct.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
  *         contains the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_INISTRUCT(GPIO_InitStruct));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->Mode));
    assert_param(IS_GPIO_PULL(GPIO_InitStruct->Pull));
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->Speed));
    assert_param(IS_GPIO_SCHMIDT_TRIGGER(GPIO_InitStruct->SchmidtTrigger));
    assert_param(IS_GPIO_DRIVER_STRENGTH(GPIO_InitStruct->DriverStrength));
    assert_param(IS_GPIO_ALTERNATE(GPIO_InitStruct->Alternate));
#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
    assert_param(IS_GPIO_AFMMODE(GPIO_InitStruct->AltfMode));
#endif

    volatile uint32_t* Px = NULL;
    uint32_t tempreg = 0;
#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_init(GPIOx, GPIO_InitStruct->Pin);
#endif /* BSP_GPIO_USE_PHY */
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_InitStruct->Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            tempreg = 0;
            if(GPIO_InitStruct->Mode == GPIO_MODE_ANALOG)
            {//analog mode, please set input
                tempreg |= GPIO_MODE_INPUT;
            }
            else
            {
                tempreg |= GPIO_InitStruct->Mode;
            }
            tempreg |= GPIO_InitStruct->Pull;
#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
            if(IS_GPIO_PHY_PORT(GPIOx))
            {
#endif /* BSP_GPIO_USE_PHY */
                tempreg |= GPIO_InitStruct->SchmidtTrigger;
                tempreg |= GPIO_InitStruct->Speed;
                tempreg |= GPIO_InitStruct->DriverStrength;
#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
            }
#endif /* BSP_GPIO_USE_PHY */
#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
            tempreg &= ~GPIO_Px_ALTF_MODE;
            tempreg |= GPIO_InitStruct->AltfMode;
#endif /* BSP_GPIO_USE_AFMODE */
            tempreg &= ~GPIO_Px_ALTF;
            tempreg |= GPIO_InitStruct->Alternate;
            *Px = tempreg;
            if(IS_GPIO_IT_MODE(GPIO_InitStruct->Mode))
            {
                *Px |= GPIO_Px_IClr;
                //enable selected port interrupt
                *Px |= GPIO_Px_IEn;
            }
        }
    }
}

/**
  * @brief  Set GPIO Dir.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  Dir: input or output.
  * @retval None
  */
void HAL_GPIO_SetDir(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIO_Dir Dir)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_DIR(Dir));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            if(Dir == GPIO_DIR_INPUT)
            {
                *Px |= GPIO_MODE_INPUT;
            }
            else
            {
                *Px &= ~GPIO_MODE_INPUT;
            }
        }
    }
}

#if(defined BSP_GPIO_USE_AFMODE && BSP_GPIO_USE_AFMODE == 1U)
/**
  * @brief  Set GPIO Alternate mode.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  GPIO_AFM: Alternate mode.
  * @retval None
  */
void HAL_GPIO_SetALTFMode(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_AFM)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_AFMMODE(GPIO_AFM));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px &= ~GPIO_Px_ALTF_MODE_Msk;
            *Px |= GPIO_AFM;
        }
    }
}
#endif /* BSP_GPIO_USE_AFMODE */
/**
  * @brief  Set GPIO Alternate.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  GPIO_AFx: Alternate function mapping.
  * @retval None
  */
void HAL_GPIO_SetALTF(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_AFx)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_ALTERNATE(GPIO_AFx));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px &= ~GPIO_Px_ALTF_Msk;
            *Px |= GPIO_AFx;
        }
    }
}

/**
  * @brief  Get GPIO Alternate.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @retval GPIO_AFx: Alternate function mapping.
  */
uint32_t HAL_GPIO_GetALTF(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            return (*Px) & GPIO_Px_ALTF_Msk;
        }
    }
    return 0;
}

/**
  * @brief  Set GPIO Mode.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  Mode: operating mode.
  * @param  Pull: Pull-up or Pull-Down activation.
  * @retval None
  */
void HAL_GPIO_SetMode(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Mode, uint32_t Pull)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_MODE(Mode));
    assert_param(IS_GPIO_PULL(Pull));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px &= ~(GPIO_MODE_INPUT | GPIO_MODE_OUTPUT_OD | GPIO_PULLUP | GPIO_PULLDOWN);
            *Px |= Mode;
            *Px |= Pull;
        }
    }
}

/**
  * @brief  Set GPIO Speed.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  Speed: speed for the selected pins.
  * @retval None
  */
void HAL_GPIO_SetSpeed(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Speed)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_SPEED(Speed));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px &= ~(GPIO_SPEED_FREQ_HIGH);
            *Px |= Speed;
        }
    }
}

/**
  * @brief  Set GPIO Drive.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  Drive: the DriverStrength for the selected pins.
  * @retval None
  */
void HAL_GPIO_SetDrive(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t Drive)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_DRIVER_STRENGTH(Drive));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px &= ~(GPIO_Px_DS_Msk);
            *Px |= Drive;
        }
    }
}

/**
  * @brief  Set the specified port pin interrupt.
  * @param  GPIOx: pointer to a GPIO_TypeDef structure of the specified GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that
  *         contains the configuration information for the specified GPIO peripheral.
  * @param  NewState: new state of the GPIO interrupts.
  *   			This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void HAL_GPIO_SetInterrupt(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef* GPIO_InitStruct, FunctionalState NewState)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_INISTRUCT(GPIO_InitStruct));
    assert_param(IS_GPIO_FUNCSTATE(NewState));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->Mode));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_InitStruct->Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_InitStruct->Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            if(NewState != DISABLE)
            {
                *Px |= GPIO_Px_IClr;
                *Px |= GPIO_InitStruct->Mode;
                //enable selected port interrupt
                *Px |= GPIO_Px_IEn;
            }
            else
            {
                *Px &= ~GPIO_Px_IEn;
            }
        }
    }
}

/**
  * @brief  Clear the specified port pin interrupt flag.
  * @param  GPIOx: GPIOx(x=A,B,C,D)
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @retval None.
  */
void HAL_GPIO_ClearInterruptFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            *Px |= GPIO_Px_IClr;
        }
    }
}

/**
  * @brief  Get the specified port pin interrupt flag.
  * @param  GPIOx: GPIOx(x=A,B,C,D)
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @retval interrupt status : IntrFlag
  */
uint32_t HAL_GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    return (GPIOx->IntrFlag & GPIO_Pin);
}

/**
  * @brief  Write data to the specified GPIO pin.
  * @param  GPIOx: GPIOx(x=A,B,C,D)
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @param  PinState: only one parameter can be selected which is shown as below:
            \arg  GPIO_PIN_RESET: clear the port pin
            \arg  GPIO_PIN_SET: set the port pin
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIO_PinState PinState)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(PinState));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            if (PinState != GPIO_PIN_RESET)
            {
                *Px |= GPIO_Px_OUT;
            }
            else
            {
                *Px &= ~GPIO_Px_OUT;
            }
        }
    }
}

/**
  * @brief  Read data to the specified GPIO pin.
  * @param  GPIOx: GPIOx(x=A,B,C,D)
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @retval input status of gpio pin: GPIO_PIN_SET or GPIO_PIN_RESET
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            if ((uint32_t) GPIO_PIN_RESET != (*Px & GPIO_Px_IN))
            {
                return GPIO_PIN_SET;
            }
            else
            {
                return GPIO_PIN_RESET;
            }
        }
    }
    return GPIO_PIN_RESET;
}

/**
  * @brief  Toggle output to the specified GPIO pin.
  * @param  GPIOx: GPIOx(x=A,B,C,D)
  * @param  GPIO_Pin: Select GPIO Pin to be operated
  *        This parameter can be: GPIO_PIN_0 ~ GPIO_PIN_31.
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

#if(defined BSP_GPIO_USE_PHY && BSP_GPIO_USE_PHY == 1U)
    phy_pin_mode_ctrl(GPIOx, GPIO_Pin);
#endif
    volatile uint32_t* Px = NULL;
    for (uint32_t pinx = 0;  pinx < 32; pinx++)
    {
        if(((GPIO_Pin >> pinx) & 0x1U) == 0x1U)
        {
            Px = (__IO uint32_t*)((uint32_t*)GPIOx + pinx);
            if ((uint32_t) GPIO_PIN_RESET != (*Px & GPIO_Px_OUT))
            {
                *Px &= ~GPIO_Px_OUT;
            }
            else
            {
                *Px |= GPIO_Px_OUT;
            }
        }
    }
}

void LL_GPIO_SetMode(GPIO_TypeDef* GPIOx, uint32_t mode)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));

    GPIOx->MODE = mode;
}

#endif /* HAL_GPIO_MODULE_ENABLED */
