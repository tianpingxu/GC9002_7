/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_gpio.h
 * @brief    header File for GPIO Driver
 * @version  V1.0
 * @date     2023/02/16
 * @model    gpio
 ******************************************************************************/
#ifndef __CSI_GPIO_H__
#define __CSI_GPIO_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_GPIO_ENABLED
#define CSI_GPIO_CNT    2

// #define CSI_GET_PIN(PORTx, PIN) (unsigned int)((unsigned int)((PORTx)-'A')*32 + (PIN))
#define CSI_GET_PIN(PIN)        (0x1 << (PIN))
typedef enum
{
    GPIO_DIRECTION_INPUT,   //input mode
    GPIO_DIRECTION_OUTPUT,  //output mode
}csi_gpio_dir_t;
typedef enum
{
    GPIO_MODE_PULLNONE,     //悬空输入
    GPIO_MODE_PULLUP,       //上拉输入
    GPIO_MODE_PULLDOWN,     //下拉输入
    GPIO_MODE_OPEN_DRAIN,   //开漏输出
    GPIO_MODE_PUSH_PULL,    //推挽输出
}csi_gpio_mode_t;
typedef enum
{
    GPIO_IRQ_MODE_RISING_EDGE,  //上升沿中断模式
    GPIO_IRQ_MODE_FALLING_EDGE, //下降沿中断模式
    GPIO_IRQ_MODE_BOTH_EDGE,    //双边沿模式
    GPIO_IRQ_MODE_LOW_LEVEL,    //低电平模式
    GPIO_IRQ_MODE_HIGH_LEVEL,   //高电平模式
}csi_gpio_irq_mode_t;
typedef enum
{
    GPIO_PIN_LOW,   //低电平
    GPIO_PIN_HIGH,  //高电平
}csi_gpio_pin_state_t;

#define _PA_BASE 0
#define _PB_BASE 32
#define _PC_BASE 64
#define _PD_BASE 96
#define _GPIOA(n)    (_PA_BASE + (n))
#define _GPIOB(n)    (_PB_BASE + (n))
typedef enum
{
    PA0 = _GPIOA(0),
    PA1 = _GPIOA(1),
    PA2 = _GPIOA(2),
    PA3 = _GPIOA(3),
    PA4 = _GPIOA(4),
    PA5 = _GPIOA(5),
    PA6 = _GPIOA(6),
    PA7 = _GPIOA(7),
    PA8 = _GPIOA(8),
    PA9 = _GPIOA(9),
    PA10 = _GPIOA(10),
    PA11 = _GPIOA(11),
    PA12 = _GPIOA(12),
    PA13 = _GPIOA(13),
    PA14 = _GPIOA(14),
    PA15 = _GPIOA(15),
    PA16 = _GPIOA(16),
    PA17 = _GPIOA(17),
    PA18 = _GPIOA(18),
    PA19 = _GPIOA(19),
    PA20 = _GPIOA(20),
    PA21 = _GPIOA(21),
    PA22 = _GPIOA(22),
    PA23 = _GPIOA(23),
    PA24 = _GPIOA(24),
    PA25 = _GPIOA(25),
    PA26 = _GPIOA(26),
    PA27 = _GPIOA(27),
    PA28 = _GPIOA(28),
    PA29 = _GPIOA(29),
    PA30 = _GPIOA(30),
    PA31 = _GPIOA(31),
    
    PB0 = _GPIOB(0),
    PB1 = _GPIOB(1),
    PB2 = _GPIOB(2),
    PB3 = _GPIOB(3),
    PB4 = _GPIOB(4),
    PB5 = _GPIOB(5),
    PB6 = _GPIOB(6),
    PB7 = _GPIOB(7),
    PB8 = _GPIOB(8),
    PB9 = _GPIOB(9),
    PB10 = _GPIOB(10),
    PB11 = _GPIOB(11),
    PB12 = _GPIOB(12),
    PB13 = _GPIOB(13),
    PB14 = _GPIOB(14),
    PB15 = _GPIOB(15),
    PB16 = _GPIOB(16),
    PB17 = _GPIOB(17),
    PB18 = _GPIOB(18),
    PB19 = _GPIOB(19),
    PB20 = _GPIOB(20),
    PB21 = _GPIOB(21),
    PB22 = _GPIOB(22),
    PB23 = _GPIOB(23),
    PB24 = _GPIOB(24),
    PB25 = _GPIOB(25),
    PB26 = _GPIOB(26),
    PB27 = _GPIOB(27),
    PB28 = _GPIOB(28),
    PB29 = _GPIOB(29),
    PB30 = _GPIOB(30),
    PB31 = _GPIOB(31),
    PIN_NOT_USED,
}pin_name_t;
typedef enum
{
    PA0_DISP_CK    = GPIO_AF1,
    PA1_DISP_R3    = GPIO_AF1,
    PA2_DISP_R4    = GPIO_AF1,
    PA3_DISP_R5    = GPIO_AF1,
    PA4_DISP_R6    = GPIO_AF1,
    PA5_DISP_R7    = GPIO_AF1,
    PA6_DISP_G2    = GPIO_AF1,
    PA7_DISP_G3    = GPIO_AF1,
    PA8_DISP_G4    = GPIO_AF1,
    PA9_DISP_G5    = GPIO_AF1,
    PA10_DISP_G6   = GPIO_AF1,
    PA11_DISP_G7   = GPIO_AF1,
    PA12_DISP_B3   = GPIO_AF1,
    PA13_DISP_B4   = GPIO_AF1,
    PA14_DISP_B5   = GPIO_AF1,
    PA15_DISP_B6   = GPIO_AF1,
    PA16_DISP_B7   = GPIO_AF1,
    PA17_DISP_HS   = GPIO_AF1,
    PA18_DISP_VS   = GPIO_AF1,
    PA19_DISP_DE   = GPIO_AF1,
    PB0_MDC        = GPIO_AF1,
    PB1_MDIO       = GPIO_AF1,
    PB2_RMII_CLK   = GPIO_AF1,
    PB3_RMII_TXEN  = GPIO_AF1,
    PB4_RMII_TXD0  = GPIO_AF1,
    PB5_RMII_TXD1  = GPIO_AF1,
    PB6_UART1_RX   = GPIO_AF1,
    PB7_UART1_TX   = GPIO_AF1,
    PB8_UART2_RX   = GPIO_AF1,
    PB9_UART2_TX   = GPIO_AF1,
    PB10_SPI0_MISO = GPIO_AF1,
    PB11_SPI0_MOSI = GPIO_AF1,
    PB12_SPI0_NSS  = GPIO_AF1,
    PB13_SPI0_SCK  = GPIO_AF1,
    PB14_SDA0      = GPIO_AF1,
    PB15_SCL0      = GPIO_AF1,
//    PB16_          = GPIO_AF1,
    PB17_I2S_SCK   = GPIO_AF1,
    PB18_I2S_WS    = GPIO_AF1,
    PB19_I2S_DO    = GPIO_AF1,
    
//    PA0_             = GPIO_AF2,
//    PA1_             = GPIO_AF2,
    PA2_DISP_SPI_SCK = GPIO_AF2,
    PA3_DISP_SPI_CSN = GPIO_AF2,
    PA4_DISP_SPI_DQ0 = GPIO_AF2,
    PA5_DISP_SPI_DQ1 = GPIO_AF2,
    PA6_DISP_SPI_DQ2 = GPIO_AF2,
    PA7_DISP_SPI_DQ3 = GPIO_AF2,
//    PA8_             = GPIO_AF2,
    PA9_SPI0_MISO    = GPIO_AF2,
    PA10_SPI0_MOSI   = GPIO_AF2,
    PA11_SPI0_NSS    = GPIO_AF2,
    PA12_SPI0_SCK    = GPIO_AF2,
    PA13_SPI2_MISO   = GPIO_AF2,
    PA14_SPI2_MOSI   = GPIO_AF2,
    PA15_UART2_RX    = GPIO_AF2,
    PA16_UART2_TX    = GPIO_AF2,
    PA17_SPI2_NSS    = GPIO_AF2,
    PA18_SPI2_SCK    = GPIO_AF2,
//    PA19_            = GPIO_AF2,
    PB0_UART1_RX     = GPIO_AF2,
    PB1_UART1_TX     = GPIO_AF2,
    PB2_UART0_RX     = GPIO_AF2,
    PB3_UART0_TX     = GPIO_AF2,
    PB4_UART2_CTS    = GPIO_AF2,
    PB5_UART2_RTS    = GPIO_AF2,
    PB6_UART2_CK     = GPIO_AF2,
    PB7_UART1_CTS    = GPIO_AF2,
    PB8_UART1_RTS    = GPIO_AF2,
    PB9_UART1_CK     = GPIO_AF2,
//    PB10_            = GPIO_AF2,
//    PB11_            = GPIO_AF2,
//    PB12_            = GPIO_AF2,
//    PB13_            = GPIO_AF2,
//    PB14_            = GPIO_AF2,
//    PB15_            = GPIO_AF2,
    PB16_TIM1_1      = GPIO_AF2,
    PB17_TIM1_2      = GPIO_AF2,
    PB18_TIM1_3      = GPIO_AF2,
    PB19_TIM1_4      = GPIO_AF2,
    
    PA0_DISP_MPU_SCK  = GPIO_AF3,
    PA1_DISP_MPU_WRN  = GPIO_AF3,
    PA2_DISP_MPU_DCN  = GPIO_AF3,
    PA3_DISP_MPU_RDN  = GPIO_AF3,
    PA4_DISP_MPU_CSN  = GPIO_AF3,
    PA5_DISP_MPU_DB0  = GPIO_AF3,
    PA6_DISP_MPU_DB1  = GPIO_AF3,
    PA7_DISP_MPU_DB2  = GPIO_AF3,
    PA8_DISP_MPU_DB3  = GPIO_AF3,
    PA9_DISP_MPU_DB4  = GPIO_AF3,
    PA10_DISP_MPU_DB5 = GPIO_AF3,
    PA11_DISP_MPU_DB6 = GPIO_AF3,
    PA12_DISP_MPU_DB7 = GPIO_AF3,
//    PA13_             = GPIO_AF3,
//    PA14_             = GPIO_AF3,
    PA15_TEARING      = GPIO_AF3,
    PA16_CLKOUT       = GPIO_AF3,
//    PA17_             = GPIO_AF3,
    PA18_SPI1_MISO    = GPIO_AF3,
    PA19_SPI1_MOSI    = GPIO_AF3,
    PB0_SPI1_NSS      = GPIO_AF3,
    PB1_SPI1_SCK      = GPIO_AF3,
//    PB2_              = GPIO_AF3,
//    PB3_              = GPIO_AF3,
//    PB4_              = GPIO_AF3,
//    PB5_              = GPIO_AF3,
    PB6_CAN_TX        = GPIO_AF3,
    PB7_CAN_RX        = GPIO_AF3,
//    PB8_              = GPIO_AF3,
//    PB9_              = GPIO_AF3,
//    PB10_             = GPIO_AF3,
//    PB11_             = GPIO_AF3,
    PB12_TIM0_1       = GPIO_AF3,
    PB13_TIM0_2       = GPIO_AF3,
    PB14_TIM0_3       = GPIO_AF3,
    PB15_TIM0_4       = GPIO_AF3,
//    PB16_             = GPIO_AF3,
    PB17_VBUS_EN      = GPIO_AF3,
//    PB18_             = GPIO_AF3,
//    PB19_             = GPIO_AF3,
    
    PA0_UART0_RX      = GPIO_AF4,
    PA1_UART0_TX      = GPIO_AF4,
    PA2_UART1_RX      = GPIO_AF4,
    PA3_UART1_TX      = GPIO_AF4,
    PA4_UART2_RX      = GPIO_AF4,
    PA5_UART2_TX      = GPIO_AF4,
    PA6_SDA1          = GPIO_AF4,
    PA7_SCL1          = GPIO_AF4,
//    PA8_              = GPIO_AF4,
//    PA9_              = GPIO_AF4,
//    PA10_             = GPIO_AF4,
//    PA11_             = GPIO_AF4,
    PA12_DISP_SPI_SCK = GPIO_AF4,
    PA13_DISP_SPI_CSN = GPIO_AF4,
    PA14_DISP_SPI_DQ0 = GPIO_AF4,
//    PA15_             = GPIO_AF4,
//    PA16_             = GPIO_AF4,
//    PA17_             = GPIO_AF4,
//    PA18_             = GPIO_AF4,
//    PA19_             = GPIO_AF4,
//    PB0_              = GPIO_AF4,
    PB1_VBUS_EN       = GPIO_AF4,
//    PB2_              = GPIO_AF4,
//    PB3_              = GPIO_AF4,
    PB4_TIM0_1        = GPIO_AF4,
    PB5_TIM0_2        = GPIO_AF4,
    PB6_TIM0_3        = GPIO_AF4,
    PB7_TIM0_4        = GPIO_AF4,
    PB8_TIM1_1        = GPIO_AF4,
    PB9_TIM1_2        = GPIO_AF4,
    PB10_TIM1_3       = GPIO_AF4,
    PB11_TIM1_4       = GPIO_AF4,
//    PB12_             = GPIO_AF4,
//    PB13_             = GPIO_AF4,
//    PB14_             = GPIO_AF4,
//    PB15_             = GPIO_AF4,
    PB16_SPI1_MISO    = GPIO_AF4,
    PB17_SPI1_MOSI    = GPIO_AF4,
    PB18_SPI1_NSS     = GPIO_AF4,
    PB19_SPI1_SCK     = GPIO_AF4,
    
    PA0_I2S_SCK      = GPIO_AF5,
    PA1_I2S_WS       = GPIO_AF5,
    PA2_I2S_DO       = GPIO_AF5,
//    PA3_             = GPIO_AF5,
//    PA4_             = GPIO_AF5,
    PA5_DISP_SPI_DCN = GPIO_AF5,
    PA6_CAN_TX       = GPIO_AF5,
    PA7_CAN_RX       = GPIO_AF5,
//    PA8_             = GPIO_AF5,
//    PA9_             = GPIO_AF5,
//    PA10_            = GPIO_AF5,
//    PA11_            = GPIO_AF5,
//    PA12_            = GPIO_AF5,
    PA13_UART0_CTS   = GPIO_AF5,
    PA14_UART0_RTS   = GPIO_AF5,
    PA15_UART0_CK    = GPIO_AF5,
    PA16_TIM0_1      = GPIO_AF5,
    PA17_TIM0_2      = GPIO_AF5,
    PA18_TIM0_3      = GPIO_AF5,
    PA19_TIM0_4      = GPIO_AF5,
    PB0_TIM1_1       = GPIO_AF5,
    PB1_TIM1_2       = GPIO_AF5,
    PB2_TIM1_3       = GPIO_AF5,
    PB3_TIM1_4       = GPIO_AF5,
//    PB4_             = GPIO_AF5,
//    PB5_             = GPIO_AF5,
    PB6_RMII_RXDV    = GPIO_AF5,
    PB7_RMII_RXD0    = GPIO_AF5,
    PB8_RMII_RXD1    = GPIO_AF5,
//    PB9_             = GPIO_AF5,
//    PB10_            = GPIO_AF5,
//    PB11_            = GPIO_AF5,
//    PB12_            = GPIO_AF5,
//    PB13_            = GPIO_AF5,
    PB14_UART0_TX    = GPIO_AF5,
    PB15_UART0_RX    = GPIO_AF5,
//    PB16_            = GPIO_AF5,
//    PB17_            = GPIO_AF5,
//    PB18_            = GPIO_AF5,
//    PB19_            = GPIO_AF5,
    
    PA0_MOTOR_PWM0 = GPIO_AF6,
    PA1_MOTOR_PWM1 = GPIO_AF6,
    PA2_MOTOR_PWM2 = GPIO_AF6,
    PA3_MOTOR_PWM3 = GPIO_AF6,
    PA4_SM_M0C0_P  = GPIO_AF6,
    PA5_SM_M0C0_N  = GPIO_AF6,
    PA6_SM_M0C1_P  = GPIO_AF6,
    PA7_SM_M0C1_N  = GPIO_AF6,
    PA8_TIM0_1     = GPIO_AF6,
    PA9_TIM0_2     = GPIO_AF6,
    PA10_TIM0_3    = GPIO_AF6,
    PA11_TIM0_4    = GPIO_AF6,
    PA12_TIM1_1    = GPIO_AF6,
    PA13_TIM1_2    = GPIO_AF6,
    PA14_TIM1_3    = GPIO_AF6,
    PA15_TIM1_4    = GPIO_AF6,
    PA16_UART1_CTS = GPIO_AF6,
    PA17_UART1_RTS = GPIO_AF6,
    PA18_UART1_CK  = GPIO_AF6,
    PA19_          = GPIO_AF6,
    PB0_VI_CK      = GPIO_AF6,
    PB1_VI_HS      = GPIO_AF6,
    PB2_VI_VS      = GPIO_AF6,
    PB3_VI_D0      = GPIO_AF6,
    PB4_VI_D1      = GPIO_AF6,
    PB5_VI_D2      = GPIO_AF6,
    PB6_VI_D3      = GPIO_AF6,
    PB7_VI_D4      = GPIO_AF6,
    PB8_VI_D5      = GPIO_AF6,
    PB9_VI_D6      = GPIO_AF6,
    PB10_VI_D7     = GPIO_AF6,
//    PB11_          = GPIO_AF6,
//    PB12_          = GPIO_AF6,
//    PB13_          = GPIO_AF6,
//    PB14_          = GPIO_AF6,
//    PB15_          = GPIO_AF6,
//    PB16_          = GPIO_AF6,
//    PB17_          = GPIO_AF6,
    PB18_CAN_TX    = GPIO_AF6,
    PB19_CAN_RX    = GPIO_AF6,

    PA0_TIM0_1      = GPIO_AF7,
    PA1_TIM0_2      = GPIO_AF7,
    PA2_TIM0_3      = GPIO_AF7,
    PA3_TIM0_4      = GPIO_AF7,
    PA4_TIM1_1      = GPIO_AF7,
    PA5_TIM1_2      = GPIO_AF7,
    PA6_TIM1_3      = GPIO_AF7,
    PA7_TIM1_4      = GPIO_AF7,
    PA8_MOTOR_PWM0  = GPIO_AF7,
    PA9_MOTOR_PWM1  = GPIO_AF7,
    PA10_MOTOR_PWM2 = GPIO_AF7,
    PA11_MOTOR_PWM3 = GPIO_AF7,
    PA12_SM_M1C0_P  = GPIO_AF7,
    PA13_SM_M1C0_N  = GPIO_AF7,
    PA14_SM_M1C1_P  = GPIO_AF7,
    PA15_SM_M1C1_N  = GPIO_AF7,
    PA16_MOTOR_PWM0 = GPIO_AF7,
    PA17_MOTOR_PWM1 = GPIO_AF7,
    PA18_MOTOR_PWM2 = GPIO_AF7,
    PA19_MOTOR_PWM3 = GPIO_AF7,
    PB0_SM_M2C0_P   = GPIO_AF7,
    PB1_SM_M2C0_N   = GPIO_AF7,
    PB2_SM_M2C1_P   = GPIO_AF7,
    PB3_SM_M2C1_N   = GPIO_AF7,
    PB4_MOTOR_PWM0  = GPIO_AF7,
    PB5_MOTOR_PWM1  = GPIO_AF7,
    PB6_MOTOR_PWM2  = GPIO_AF7,
    PB7_MOTOR_PWM3  = GPIO_AF7,
    PB8_SM_M3C0_P   = GPIO_AF7,
    PB9_SM_M3C0_N   = GPIO_AF7,
    PB10_SM_M3C1_P  = GPIO_AF7,
    PB11_SM_M3C1_N  = GPIO_AF7,
    PB12_MOTOR_PWM0 = GPIO_AF7,
    PB13_MOTOR_PWM1 = GPIO_AF7,
    PB14_MOTOR_PWM2 = GPIO_AF7,
    PB15_MOTOR_PWM3 = GPIO_AF7,
    PB16_MOTOR_PWM0 = GPIO_AF7,
    PB17_MOTOR_PWM1 = GPIO_AF7,
    PB18_MOTOR_PWM2 = GPIO_AF7,
    PB19_MOTOR_PWM3 = GPIO_AF7,
}pin_func_t;


#define CSI_GPIO_PORT_NUM   2
struct pin_irq_map
{
    uint32_t gpiox;
    IRQn_Type irqno;
};
extern const struct pin_irq_map g_pin_irq_map[CSI_GPIO_PORT_NUM];
struct pin_irq_hdr
{
    uint32_t        gpiox;
    uint32_t        mode;
    void (*hdr[32])(void *args);
    void            *args[32];
};
extern struct pin_irq_hdr pin_irq_hdr_tab[CSI_GPIO_PORT_NUM];

typedef struct _csi_gpio
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_gpio gpio, uint32_t pins, void *arg);//User callback function
    void *arg;          //Callback function argument
    void *priv;         //Device private variable
}csi_gpio_t;
/*
 * csi gpio port id
 * */
#define CSI_GPIO_PORT_A     (uint32_t)GPIOA
#define CSI_GPIO_PORT_B     (uint32_t)GPIOB

csi_error_t csi_gpio_init(csi_gpio_t *gpio, uint32_t port_idx);
csi_error_t csi_gpio_uninit(csi_gpio_t *gpio);
csi_error_t csi_gpio_dir(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_dir_t dir);
csi_error_t csi_gpio_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_mode_t mode);
csi_error_t csi_gpio_irq_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_irq_mode_t mode);
csi_error_t csi_gpio_irq_enable(csi_gpio_t *gpio, uint32_t pin_mask, bool enable);
csi_error_t csi_gpio_debonce(csi_gpio_t *gpio, uint32_t pin_mask, bool enable);
void        csi_gpio_write(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_pin_state_t value);
uint32_t    csi_gpio_read(csi_gpio_t *gpio, uint32_t pin_mask);
void        csi_gpio_toggle(csi_gpio_t *gpio, uint32_t pin_mask);
/* Register Interrupt Handler entry. This is for csi_gpio_pin driver. 
 * If CONFIG_CSI_NON_VECTOR set 1, callback must be modified by __attribute__ ((interrupt ("machine")))
 */
csi_error_t csi_gpio_attach_callback(csi_gpio_t *gpio, void *callback, void *arg);
void        csi_gpio_detach_callback(csi_gpio_t *gpio);

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])

static inline int32_t port2bitno(uint32_t gpiox)
{
    if((GPIO_TypeDef *)gpiox == GPIOA)
    {
        return 0;
    }
    else if((GPIO_TypeDef *)gpiox == GPIOB)
    {
        return 1;
    }
    return -1;
}

#endif /* CONFIG_CSI_DRV_GPIO_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_GPIO_H__ */