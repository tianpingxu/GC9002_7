/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-03     huyt         the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal.h"
#ifdef HAL_EXTENSION_MODULE_ENABLED

/**
  * @brief  Division calculation (unsigned)
  * @param  dividend Value of unsigned dividend.
  * @param  divisor  Value of unsigned divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivUnsigned(uint64_t dividend, uint32_t divisor, uint64_t *quotient, uint32_t *remainder)
{
    assert_param(quotient != NULL);
    assert_param(remainder != NULL);
    uint32_t tempreg = 0;
    while(EXTENSION->div_ctrl.bit.div_start == 1);// Wait for the last time to end
    EXTENSION->dividend.reg_u64val = dividend;
    EXTENSION->divisor = divisor;

    tempreg = EXTENSION->div_ctrl.reg_u32val;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_mode = 0;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_start = 1;
    EXTENSION->div_ctrl.reg_u32val = tempreg;

    while(EXTENSION->div_ctrl.bit.div_done == 0);
    if(EXTENSION->div_ctrl.bit.div_err == 1)
    {
        return HAL_ERROR;
    }
    *quotient = EXTENSION->div_quotient.reg_u64val;
    *remainder = EXTENSION->div_remainder;
    return HAL_OK;
}
/**
  * @brief  Division calculation (signed)
  * @param  dividend Value of signed dividend.
  * @param  divisor  Value of signed divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivSigned(int64_t dividend, int32_t divisor, int64_t *quotient, int32_t *remainder)
{
    assert_param(quotient != NULL);
    assert_param(remainder != NULL);
    uint32_t tempreg = 0;
    while(EXTENSION->div_ctrl.bit.div_start == 1);// Wait for the last time to end
    EXTENSION->divisor = (uint32_t)divisor;
    EXTENSION->dividend.reg_u64val = (uint64_t)dividend;

    tempreg = EXTENSION->div_ctrl.reg_u32val;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_mode = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_start = 1;
    EXTENSION->div_ctrl.reg_u32val = tempreg;

    while(EXTENSION->div_ctrl.bit.div_done == 0);
    if(EXTENSION->div_ctrl.bit.div_err == 1)
    {
        return HAL_ERROR;
    }
    *quotient = (int64_t)EXTENSION->div_quotient.reg_u64val;
    *remainder = (int32_t)EXTENSION->div_remainder;
    return HAL_OK;
}
/**
  * @brief  Get Division calculation Value
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_GetDivVal(int64_t *quotient, uint32_t *remainder)
{
    assert_param(quotient != NULL);
    assert_param(remainder != NULL);
    *quotient = (int64_t)EXTENSION->div_quotient.reg_u64val;
    *remainder = EXTENSION->div_remainder;
    return HAL_OK;
}
/**
  * @brief  Division calculation (unsigned)
  * @param  dividend Value of unsigned dividend.
  * @param  divisor  Value of unsigned divisor.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivUnsigned_IT(uint64_t dividend, uint32_t divisor)
{
    uint32_t tempreg = 0;
    while(EXTENSION->div_ctrl.bit.div_start == 1);// Wait for the last time to end
    EXTENSION->dividend.reg_u64val = dividend;
    EXTENSION->divisor = divisor;

    tempreg = EXTENSION->div_ctrl.reg_u32val;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_start = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_mode = 0;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_enable = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_clear = 1;
    EXTENSION->div_ctrl.reg_u32val = tempreg;
    return HAL_OK;
}
/**
  * @brief  Division calculation (signed)
  * @param  dividend Value of signed dividend.
  * @param  divisor  Value of signed divisor.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivSigned_IT(int64_t dividend, int32_t divisor)
{
    uint32_t tempreg = 0;
    while(EXTENSION->div_ctrl.bit.div_start == 1);// Wait for the last time to end
    EXTENSION->divisor = (uint32_t)divisor;
    EXTENSION->dividend.reg_u64val = (uint64_t)dividend;

    tempreg = EXTENSION->div_ctrl.reg_u32val;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_start = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_mode = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_enable = 1;
    ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_clear = 1;
    EXTENSION->div_ctrl.reg_u32val = tempreg;
    return HAL_OK;
}

/**
  * @brief  Cordic calculation (Rotation)
  * @param  x_i cordic initial value of x
  * @param  y_i
  * @param  z_i
  * @param  x_o pointer to cordic_x_o.
  * @param  y_o pointer to cordic_y_o.
  * @param  z_o pointer to cordic_z_o.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicRotation(uint32_t x_i, uint32_t y_i, uint32_t z_i, uint32_t *x_o, uint32_t *y_o, uint32_t *z_o)
{
    assert_param(x_o != NULL);
    assert_param(y_o != NULL);
    assert_param(z_o != NULL);
    uint32_t tempreg = 0;
    while(EXTENSION->cordic_ctrl.bit.cordic_start == 1);// Wait for the last time to end
    EXTENSION->cordic_x_i = x_i;
    EXTENSION->cordic_y_i = y_i;
    EXTENSION->cordic_z_i = z_i;

    tempreg = EXTENSION->cordic_ctrl.reg_u32val;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_mode = 0;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_start = 1;
    EXTENSION->cordic_ctrl.reg_u32val = tempreg;

    while(EXTENSION->cordic_ctrl.bit.cordic_done == 0);
    *x_o = EXTENSION->cordic_x_o;
    *y_o = EXTENSION->cordic_y_o;
    *z_o = EXTENSION->cordic_z_o;
    return HAL_OK;
}
/**
  * @brief  Cordic calculation (Vectoring)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @param  x_o pointer to cordic_x_o.
  * @param  y_o pointer to cordic_y_o.
  * @param  z_o pointer to cordic_z_o.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicVectoring(uint32_t x_i, uint32_t y_i, uint32_t z_i, uint32_t *x_o, uint32_t *y_o, uint32_t *z_o)
{
    assert_param(x_o != NULL);
    assert_param(y_o != NULL);
    assert_param(z_o != NULL);
    uint32_t tempreg = 0;
    while(EXTENSION->cordic_ctrl.bit.cordic_start == 1);// Wait for the last time to end
    EXTENSION->cordic_x_i = x_i;
    EXTENSION->cordic_y_i = y_i;
    EXTENSION->cordic_z_i = z_i;

    tempreg = EXTENSION->cordic_ctrl.reg_u32val;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_mode = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_start = 1;
    EXTENSION->cordic_ctrl.reg_u32val = tempreg;

    while(EXTENSION->cordic_ctrl.bit.cordic_done == 0);
    *x_o = EXTENSION->cordic_x_o;
    *y_o = EXTENSION->cordic_y_o;
    *z_o = EXTENSION->cordic_z_o;
    return HAL_OK;
}
/**
  * @brief  Get Cordic calculation Value
  * @param  x_o pointer to cordic_x_o.
  * @param  y_o pointer to cordic_y_o.
  * @param  z_o pointer to cordic_z_o.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_GetCordicVal(uint32_t *x_o, uint32_t *y_o, uint32_t *z_o)
{
    assert_param(x_o != NULL);
    assert_param(y_o != NULL);
    assert_param(z_o != NULL);
    *x_o = EXTENSION->cordic_x_o;
    *y_o = EXTENSION->cordic_y_o;
    *z_o = EXTENSION->cordic_z_o;
    return HAL_OK;
}
/**
  * @brief  Cordic calculation (Rotation)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicRotation_IT(uint32_t x_i, uint32_t y_i, uint32_t z_i)
{
    uint32_t tempreg = 0;
    while(EXTENSION->cordic_ctrl.bit.cordic_start == 1);// Wait for the last time to end
    EXTENSION->cordic_x_i = x_i;
    EXTENSION->cordic_y_i = y_i;
    EXTENSION->cordic_z_i = z_i;

    tempreg = EXTENSION->cordic_ctrl.reg_u32val;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_start = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_mode = 0;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_enable = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_clear = 1;
    EXTENSION->cordic_ctrl.reg_u32val = tempreg;
    return HAL_OK;
}
/**
  * @brief  Cordic calculation (Vectoring)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicVectoring_IT(uint32_t x_i, uint32_t y_i, uint32_t z_i)
{
    uint32_t tempreg = 0;
    while(EXTENSION->cordic_ctrl.bit.cordic_start == 1);// Wait for the last time to end
    EXTENSION->cordic_x_i = x_i;
    EXTENSION->cordic_y_i = y_i;
    EXTENSION->cordic_z_i = z_i;

    tempreg = EXTENSION->cordic_ctrl.reg_u32val;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_start = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_mode = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_enable = 1;
    ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_clear = 1;
    EXTENSION->cordic_ctrl.reg_u32val = tempreg;
    return HAL_OK;
}
/**
  * @brief  This function handles EXTENSION interrupt request.
  * @retval None
  */
void HAL_EXTENSION_IRQHandler(void)
{
    uint32_t tempreg = 0;
    tempreg = EXTENSION->div_ctrl.reg_u32val;
    if(((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_status == 1 && 
       ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_enable == 1)
    {
        if(((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_err == 1)
        {
            HAL_EXTENSION_DivErrCallback();
        }
        ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_enable = 0;
        ((union _EXTENSION_div_ctrl *)&tempreg)->bit.div_intr_clear = 1;
        EXTENSION->div_ctrl.reg_u32val = tempreg;
        HAL_EXTENSION_DivCpltCallback();
    }
    
    tempreg = EXTENSION->cordic_ctrl.reg_u32val;
    if(((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_status == 1 && 
       ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_enable == 1)
    {
        ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_enable = 0;
        ((union _EXTENSION_cordic_ctrl *)&tempreg)->bit.cordic_intr_clear = 1;
        EXTENSION->cordic_ctrl.reg_u32val = tempreg;
        HAL_EXTENSION_CordicCpltCallback();
    }
}
/**
  * @brief  Div completed callback.
  * @retval None
  */
__WEAK void HAL_EXTENSION_DivCpltCallback(void)
{
    
    
}
/**
  * @brief  Cordic completed callback.
  * @retval None
  */
__WEAK void HAL_EXTENSION_CordicCpltCallback(void)
{
    
    
}
/**
  * @brief  Div Error callback.
  * @retval None
  */
__WEAK void HAL_EXTENSION_DivErrCallback(void)
{
    
    
}
#endif
