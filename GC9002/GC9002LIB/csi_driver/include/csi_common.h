/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_common.h
 * @brief    Header File for Common Driver
 * @version  V1.0
 * @date     1. April 2023
 * @model    common
 ******************************************************************************/

#ifndef _CSI_COMMON_H_
#define _CSI_COMMON_H_

#include <csi_config.h>
#include <soc.h>
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif


extern void csi_motor_mem_init(void);
__STATIC_INLINE void csi_common_mem_init(void)
{
    csi_motor_mem_init();
}

/// \details driver handle
typedef void *drv_handle_t;

#ifndef CONFIG_PARAM_NOT_CHECK
#define HANDLE_PARAM_CHK(para, err)                                                                 \
    do {                                                                                            \
        if ((int32_t)para == (int32_t)NULL) {                                                       \
            return (err);                                                                           \
        }                                                                                           \
    } while (0)

#define HANDLE_PARAM_CHK_NORETVAL(para, err)                                                        \
    do {                                                                                            \
        if ((int32_t)para == (int32_t)NULL) {                                                       \
            return;                                                                                 \
        }                                                                                           \
    } while (0)
#else
#define HANDLE_PARAM_CHK(para, err)
#define HANDLE_PARAM_CHK_NORETVAL(para, err)
#endif

#ifdef CONFIG_DEBUG_MODE
#define CSI_ASSERT(expr)                            \
        do {                                        \
            if ((int32_t)expr == (int32_t)NULL) {   \
                printf("PROGRAM ASSERT\n");         \
                while(1);                           \
            }                                       \
        } while(0);
#else
#define CSI_ASSERT(expr)        ((void)0U)
#endif

/**
\brief General power states
*/
typedef enum  {
    DRV_POWER_OFF,                        ///< Power off: no operation possible
    DRV_POWER_LOW,                        ///< Low Power mode: retain state, detect and signal wake-up events
    DRV_POWER_FULL,                       ///< Power on: full operation at maximum performance
    DRV_POWER_SUSPEND,                    ///< Power suspend: power saving operation
} csi_power_stat_e;

typedef struct
{
    uint32_t reg_base;      //Peripheral base address
    uint16_t irq_num;       //Peripheral interrupt number
    uint16_t idx;           //Peripheral ID
    void *irq_handler;      //Peripheral interrupt function address
}csi_dev_t;

typedef enum
{
    CSI_OK,
    CSI_ERROR,
    CSI_BUSY,
    CSI_TIMEOUT,
    CSI_UNSUPPORTED,
}csi_error_t;

typedef enum
{
    readable,   //设备可读
    writeable,  //设备可写
    error,      //错误状态
}csi_state_t;

typedef enum
{
    CSI_ADC,
    CSI_AES,
    CSI_CODEC,
    CSI_CRC,
    CSI_DMA,
    CSI_ETB,
    CSI_GPIO,
    CSI_GPIO_PIN,
    CSI_I2S,
    CSI_IIC,
    CSI_IRQ,
    CSI_PIN,
    CSI_PWM,
    CSI_QSPI,
    CSI_RTC,
    CSI_SPI,
    CSI_SPINANDFLASH,
    CSI_SPIFLASH,
    CSI_TICK,
    CSI_TIMER,
    CSI_UART,
    CSI_WDT,
    CSI_MAX,
}csi_peri_class_t;

#define HANDLE_REG_BASE(handle)     (handle->dev.reg_base)
#define HANDLE_IRQ_NUM(handle)      (handle->dev.irq_num)
#define HANDLE_DEV_IDX(handle)      (handle->dev.idx)
#define HANDLE_IRQ_HANDLER(handle)  (handle->dev.irq_handler)

#ifdef CHIP_GC9005
#define ATTRIBUTE_ISR               __INTERRUPT
#define CSI_INTRPT_ENTER()          //rt_interrupt_enter()
#define CSI_INTRPT_EXIT()           //rt_interrupt_leave()
#define csi_clic_register_irq       ECLIC_Register_IRQ
#define CLIC_VECTOR_INTERRUPT       ECLIC_VECTOR_INTERRUPT
#define CLIC_NON_VECTOR_INTERRUPT   ECLIC_NON_VECTOR_INTERRUPT
#define CLIC_LEVEL_TRIGGER          ECLIC_LEVEL_TRIGGER
#define drv_get_apb_freq            HAL_RCC_GetAPBClock
#endif /* CHIP_GC9005 */
#ifdef __cplusplus
}
#endif

#endif /* _DRV_COMMON_H */

