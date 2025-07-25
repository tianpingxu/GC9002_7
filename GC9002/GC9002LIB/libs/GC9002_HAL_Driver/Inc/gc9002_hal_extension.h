/**
  ******************************************************************************
  * @file    gc9002_hal_extension.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the EXTENSION
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_EXTENSION_H__
#define __GC9002_HAL_EXTENSION_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_EXTENSION_MODULE_ENABLED

typedef struct __EXTENSION_TypeDef
{
    union _EXTENSION_div_ctrl
    {
        __IO uint32_t reg_u32val;       /*!< Offset: 0x00   (R/W) */
        struct
        {
            __IO uint32_t div_start         : 1;
            __IO uint32_t div_mode          : 1;
            __IO uint32_t div_done          : 1;
            __IO uint32_t div_err           : 1;
            __IO uint32_t div_intr_status   : 1;
            __IO uint32_t div_intr_enable   : 1;
            __IO uint32_t div_intr_clear    : 1;
            uint32_t                        : 25;
        }bit;
    }div_ctrl;
    __IO uint32_t divisor;              /*!< Offset: 0x04   (R/W) */
    union _EXTENSION_dividend
    {
        __IO uint64_t reg_u64val;       /*!< Offset: 0x08   (R/W) */
        struct
        {
            __IO uint32_t dividend_low;         /*!< Offset: 0x08   (R/W) */
            __IO uint32_t dividend_high;        /*!< Offset: 0x0C   (R/W) */
        }bit;
    }dividend;
    union _EXTENSION_div_quotient
    {
        __IO uint64_t reg_u64val;       /*!< Offset: 0x10   (R/W) */
        struct
        {
            __IO uint32_t div_quotient_low;     /*!< Offset: 0x10   (R/W) */
            __IO uint32_t div_quotient_high;    /*!< Offset: 0x14   (R/W) */
        }bit;
    }div_quotient;
    __IO uint32_t div_remainder;        /*!< Offset: 0x18   (R/W) */
    uint32_t Reserved[9];               /*!< Offset: 0x1C~0x3C   (R/W) */
    union _EXTENSION_cordic_ctrl
    {
        __IO uint32_t reg_u32val;       /*!< Offset: 0x40   (R/W) */
        struct
        {
            __IO uint32_t cordic_start          : 1;
            __IO uint32_t cordic_mode           : 1;
            __IO uint32_t cordic_done           : 1;
            __IO uint32_t cordic_intr_status    : 1;
            __IO uint32_t cordic_intr_enable    : 1;
            __IO uint32_t cordic_intr_clear     : 1;
            uint32_t                            : 26;
        }bit;
    }cordic_ctrl;
    __IO uint32_t cordic_iter;          /*!< Offset: 0x44   (R/W) */
    __IO uint32_t cordic_x_i;           /*!< Offset: 0x48   (R/W) */
    __IO uint32_t cordic_y_i;           /*!< Offset: 0x4C   (R/W) */
    __IO uint32_t cordic_z_i;           /*!< Offset: 0x50   (R/W) */
    __IO uint32_t cordic_x_o;           /*!< Offset: 0x54   (R/W) */
    __IO uint32_t cordic_y_o;           /*!< Offset: 0x58   (R/W) */
    __IO uint32_t cordic_z_o;           /*!< Offset: 0x5C   (R/W) */
}EXTENSION_TypeDef;
#define EXTENSION_BASE                  (0x50100000UL)
#define EXTENSION                       ((EXTENSION_TypeDef*)EXTENSION_BASE)

/**
  * @brief  Division calculation (unsigned)
  * @param  dividend Value of unsigned dividend.
  * @param  divisor  Value of unsigned divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivUnsigned(uint64_t dividend, uint32_t divisor, uint64_t *quotient, uint32_t *remainder);
/**
  * @brief  Division calculation (signed)
  * @param  dividend Value of signed dividend.
  * @param  divisor  Value of signed divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivSigned(int64_t dividend, int32_t divisor, int64_t *quotient, int32_t *remainder);
/**
  * @brief  Get Division calculation Value
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_GetDivVal(int64_t *quotient, uint32_t *remainder);
/**
  * @brief  Division calculation (unsigned)
  * @param  dividend Value of unsigned dividend.
  * @param  divisor  Value of unsigned divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivUnsigned_IT(uint64_t dividend, uint32_t divisor);
/**
  * @brief  Division calculation (signed)
  * @param  dividend Value of signed dividend.
  * @param  divisor  Value of signed divisor.
  * @param  quotient pointer to quotient.
  * @param  remainder pointer to remainder.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_DivSigned_IT(int64_t dividend, int32_t divisor);
/**
  * @brief  Cordic calculation (Rotation)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @param  x_o pointer to cordic_x_o.
  * @param  y_o pointer to cordic_y_o.
  * @param  z_o pointer to cordic_z_o.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicRotation(uint32_t x_i, uint32_t y_i, uint32_t z_i, uint32_t *x_o, uint32_t *y_o, uint32_t *z_o);
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
HAL_StatusTypeDef HAL_EXTENSION_CordicVectoring(uint32_t x_i, uint32_t y_i, uint32_t z_i, uint32_t *x_o, uint32_t *y_o, uint32_t *z_o);
/**
  * @brief  Get Cordic calculation Value
  * @param  x_o pointer to cordic_x_o.
  * @param  y_o pointer to cordic_y_o.
  * @param  z_o pointer to cordic_z_o.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_GetCordicVal(uint32_t *x_o, uint32_t *y_o, uint32_t *z_o);
/**
  * @brief  Cordic calculation (Rotation)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicRotation_IT(uint32_t x_i, uint32_t y_i, uint32_t z_i);
/**
  * @brief  Cordic calculation (Vectoring)
  * @param  x_i
  * @param  y_i
  * @param  z_i
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_EXTENSION_CordicVectoring_IT(uint32_t x_i, uint32_t y_i, uint32_t z_i);

/**
  * @brief  This function handles EXTENSION interrupt request.
  * @retval None
  */
void HAL_EXTENSION_IRQHandler(void);
void HAL_EXTENSION_DivCpltCallback(void);
void HAL_EXTENSION_CordicCpltCallback(void);
void HAL_EXTENSION_DivErrCallback(void);

#endif/* HAL_DMA_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_EXTENSION_H__ */
